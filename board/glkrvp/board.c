/* Copyright 2017 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Intel GLK-RVP board-specific configuration */

#include "chipset.h"
#include "console.h"
#include "gpio.h"
#include "hooks.h"
#include "host_command.h"
#include "i2c.h"
#include "ioexpander_pca9555.h"
#include "keyboard_scan.h"
#include "lid_switch.h"
#include "power.h"
#include "power_button.h"
#include "spi.h"
#include "switch.h"
#include "system.h"
#include "task.h"
#include "timer.h"
#include "uart.h"
#include "util.h"

#include "gpio_list.h"

#define I2C_PORT_PCA555_PMIC	NPCX_I2C_PORT0_0
#define I2C_ADDR_PCA555_PMIC	0x42
#define PMIC_WRITE(reg, data) pca9555_write(I2C_PORT_PCA555_PMIC, \
		I2C_ADDR_PCA555_PMIC, (reg), (data))
#define PMIC_READ(reg, data) pca9555_read(I2C_PORT_PCA555_PMIC, \
		I2C_ADDR_PCA555_PMIC, (reg), (data))

/* power signal list.  Must match order of enum power_signal. */
const struct power_signal_info power_signal_list[] = {
	{GPIO_RSMRST_L_PGOOD,     1, "RSMRST_L"},
	{GPIO_PCH_SLP_S3_L,       1, "SLP_S3_DEASSERTED"},
	{GPIO_PCH_SLP_S4_L,       1, "SLP_S4_DEASSERTED"},

	{GPIO_ALL_SYS_PGOOD,      1, "ALL_SYS_PGOOD"},
};
BUILD_ASSERT(ARRAY_SIZE(power_signal_list) == POWER_SIGNAL_COUNT);

/* I2C ports */
const struct i2c_port_t i2c_ports[] = {
	{"pmic",      NPCX_I2C_PORT0_0, 100, GPIO_I2C0_SCL0, GPIO_I2C0_SDA0},
	{"master0-1", NPCX_I2C_PORT0_1, 400, GPIO_I2C0_SCL1, GPIO_I2C0_SDA1},
	{"master1",   NPCX_I2C_PORT1,   400, GPIO_I2C1_SCL, GPIO_I2C1_SDA},
	{"master2",   NPCX_I2C_PORT2,   100, GPIO_I2C2_SCL, GPIO_I2C2_SDA},
	{"master3",   NPCX_I2C_PORT3,   100, GPIO_I2C3_SCL, GPIO_I2C3_SDA},
};
const unsigned int i2c_ports_used = ARRAY_SIZE(i2c_ports);

/* Wake-up pins for hibernate */
const enum gpio_signal hibernate_wake_pins[] = {
	GPIO_POWER_BUTTON_L,
};

const int hibernate_wake_pins_used = ARRAY_SIZE(hibernate_wake_pins);

/* Called by APL power state machine when transitioning from G3 to S5 */
static void chipset_pre_init(void)
{
	int data;

	if (PMIC_READ(PCA9555_CMD_OUTPUT_PORT_0, &data))
		return;

	/*
	 * No need to re-init PMIC since settings are sticky across sysjump.
	 * However, be sure to check that PMIC is already enabled. If it is
	 * then there's no need to re-sequence the PMIC.
	 */
	if (system_jumped_to_this_image() && (data & PCA9555_IO_0))
		return;

	/* Enable SOC_3P3_EN_L: Set the Output port O0.1 to low level */
	data &= ~PCA9555_IO_1;
	PMIC_WRITE(PCA9555_CMD_OUTPUT_PORT_0, data);

	/* TODO: Find out from the spec */
	msleep(10);

	/* Enable PMIC_EN: Set the Output port O0.0 to high level */
	PMIC_WRITE(PCA9555_CMD_OUTPUT_PORT_0, data | PCA9555_IO_0);
}
DECLARE_HOOK(HOOK_CHIPSET_PRE_INIT, chipset_pre_init, HOOK_PRIO_DEFAULT);


/* Initialize board. */
static void board_init(void)
{
}
DECLARE_HOOK(HOOK_INIT, board_init, HOOK_PRIO_FIRST);

/* Called on AP S5 -> S3 transition */
static void board_chipset_startup(void)
{
}
DECLARE_HOOK(HOOK_CHIPSET_STARTUP, board_chipset_startup, HOOK_PRIO_DEFAULT);

/* Called on AP S3 -> S5 transition */
static void board_chipset_shutdown(void)
{
}
DECLARE_HOOK(HOOK_CHIPSET_SHUTDOWN, board_chipset_shutdown, HOOK_PRIO_DEFAULT);

void chipset_do_shutdown(void)
{
	int data;

	if (PMIC_READ(PCA9555_CMD_OUTPUT_PORT_0, &data))
		return;

	/* Disable SOC_3P3_EN_L: Set the Output port O0.1 to high level */
	data |= PCA9555_IO_1;
	PMIC_WRITE(PCA9555_CMD_OUTPUT_PORT_0, data);

	/* TODO: Find out from the spec */
	msleep(10);

	/* Disable PMIC_EN: Set the Output port O0.0 to low level */
	PMIC_WRITE(PCA9555_CMD_OUTPUT_PORT_0, data & ~PCA9555_IO_0);
}

void board_hibernate_late(void)
{
}

void board_hibernate(void)
{
	/*
	 * To support hibernate called from console commands, ectool commands
	 * and key sequence, shutdown the AP before hibernating.
	 */
	chipset_do_shutdown();

	/* Added delay to allow AP to settle down */
	msleep(100);

	gpio_set_level(GPIO_SMC_SHUTDOWN, 1);

	/* Power to EC should shut down now */
	while (1)
		;
}

int charge_prevent_power_on(int power_button_pressed)
{
	return 0;
}

int extpower_is_present(void)
{
	return 1;
}

int charge_want_shutdown(void)
{
	return 0;
}

static void pmic_init(void)
{
	/* No need to re-init PMIC since settings are sticky across sysjump. */
	if (system_jumped_to_this_image())
		return;

	/*
	 * PMIC INIT
	 * Configure Port O0.0 as Output port - PMIC_EN
	 * Configure Port O0.1 as Output port - SOC_3P3_EN_L
	 */
	PMIC_WRITE(PCA9555_CMD_CONFIGURATION_PORT_0, 0xfc);

	/*
	 * Set the Output port O0.0 to low level - PMIC_EN
	 * Set the Output port O0.1 to high level - SOC_3P3_EN_L
	 *
	 * POR of PCA9555 port is input with high impedance hence explicitly
	 * configure the SOC_3P3_EN_L to high level.
	 */
	PMIC_WRITE(PCA9555_CMD_OUTPUT_PORT_0, 0xfe);
}
DECLARE_HOOK(HOOK_INIT, pmic_init, HOOK_PRIO_INIT_I2C + 1);
