/* Copyright 2017 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* reef_it8320 board configuration */

#ifndef __CROS_EC_BOARD_H
#define __CROS_EC_BOARD_H

/*
 * By default, enable all console messages excepted HC, ACPI and event:
 * The sensor stack is generating a lot of activity.
 */
#define CC_DEFAULT     (CC_ALL & ~(CC_MASK(CC_EVENTS) | CC_MASK(CC_LPC)))
#undef CONFIG_HOSTCMD_DEBUG_MODE
#define CONFIG_HOSTCMD_DEBUG_MODE HCDEBUG_OFF

/* EC console commands  */
#define CONFIG_CMD_BATT_MFG_ACCESS
#define CONFIG_CMD_CHARGER_ADC_AMON_BMON
#define CONFIG_CHARGER_SENSE_RESISTOR		10
#define CONFIG_CHARGER_SENSE_RESISTOR_AC	10
#define BD9995X_IOUT_GAIN_SELECT \
		BD9995X_CMD_PMON_IOUT_CTRL_SET_IOUT_GAIN_SET_20V

#define CONFIG_CHARGER_PSYS_READ
#define BD9995X_PSYS_GAIN_SELECT \
		BD9995X_CMD_PMON_IOUT_CTRL_SET_PMON_GAIN_SET_02UAW

/* Battery */
#define CONFIG_BATTERY_DEVICE_CHEMISTRY  "LION"
#define CONFIG_BATTERY_CUT_OFF
#define CONFIG_BATTERY_PRESENT_CUSTOM
#define CONFIG_BATTERY_SMART

/* Charger */
#define CONFIG_CHARGE_MANAGER
#define CONFIG_CHARGE_RAMP_SW
#define CONFIG_CHARGER
#define CONFIG_CHARGER_BD9995X
#define CONFIG_CHARGER_BD9995X_CHGEN
#define CONFIG_CHARGER_DISCHARGE_ON_AC
#define CONFIG_CHARGER_INPUT_CURRENT 512
#define CONFIG_CHARGER_LIMIT_POWER_THRESH_BAT_PCT 1
#define CONFIG_CHARGER_LIMIT_POWER_THRESH_CHG_MW 18000
#define CONFIG_CHARGER_MAINTAIN_VBAT
#define CONFIG_CHARGER_MIN_BAT_PCT_FOR_POWER_ON 1
#define CONFIG_USB_CHARGER
#define CONFIG_CHARGER_PROFILE_OVERRIDE
#define CONFIG_CHARGER_PROFILE_OVERRIDE_COMMON
#undef  CONFIG_CHARGER_PROFILE_VOLTAGE_RANGES
#define CONFIG_CHARGER_PROFILE_VOLTAGE_RANGES 3
#define CONFIG_CHARGE_MANAGER_EXTERNAL_POWER_LIMIT

/* USB-A config */
#define CONFIG_USB_PORT_POWER_SMART
#define CONFIG_USB_PORT_POWER_SMART_DEFAULT_MODE USB_CHARGE_MODE_CDP
#define CONFIG_USB_PORT_POWER_SMART_SIMPLE
#undef CONFIG_USB_PORT_POWER_SMART_PORT_COUNT
#define CONFIG_USB_PORT_POWER_SMART_PORT_COUNT 1
#define GPIO_USB1_ILIM_SEL GPIO_USB_A_CHARGE_EN_L
#define GPIO_USB_CTL1 GPIO_EN_PP5000

/* USB PD config */
#define CONFIG_USB_MUX_PI3USB30532
#define CONFIG_USB_MUX_PS8740
#define CONFIG_CMD_PD_CONTROL
#define CONFIG_USB_PD_ALT_MODE
#define CONFIG_USB_PD_ALT_MODE_DFP
#define CONFIG_USB_PD_DUAL_ROLE
#define CONFIG_USB_PD_DISCHARGE_GPIO
#define CONFIG_USB_PD_LOGGING
#define CONFIG_USB_PD_MAX_SINGLE_SOURCE_CURRENT TYPEC_RP_3A0
#define CONFIG_USB_PD_PORT_MAX_COUNT 2
#define CONFIG_USB_PD_VBUS_DETECT_CHARGER
#define CONFIG_USB_PD_TCPM_ITE_ON_CHIP
#define CONFIG_USB_PD_TCPM_TCPCI
#define CONFIG_USB_PD_TRY_SRC
#define CONFIG_USB_POWER_DELIVERY
#define CONFIG_USB_PD_COMM_LOCKED

#define CONFIG_USBC_SS_MUX
#define CONFIG_USBC_SS_MUX_DFP_ONLY
#define CONFIG_USBC_VCONN
#define CONFIG_USBC_VCONN_SWAP

/* SoC / PCH */
#define CONFIG_HOSTCMD_LPC
#define CONFIG_CHIPSET_APOLLOLAKE
#define CONFIG_CHIPSET_RESET_HOOK
#define CONFIG_POWER_BUTTON
#define CONFIG_POWER_BUTTON_X86
#define CONFIG_POWER_COMMON
#define CONFIG_POWER_S0IX
#define CONFIG_POWER_TRACK_HOST_SLEEP_STATE

/* EC */
#define CONFIG_ADC
#define CONFIG_BOARD_VERSION_CUSTOM
#define CONFIG_EXTPOWER_GPIO
#undef  CONFIG_EXTPOWER_DEBOUNCE_MS
#define CONFIG_EXTPOWER_DEBOUNCE_MS 1000
#define CONFIG_I2C
#define CONFIG_I2C_MASTER
#define CONFIG_KEYBOARD_BOARD_CONFIG
#define CONFIG_KEYBOARD_PROTOCOL_8042
#define CONFIG_KEYBOARD_COL2_INVERTED
#define CONFIG_LED_COMMON
#define CONFIG_LID_SWITCH
#define CONFIG_LOW_POWER_IDLE
#define CONFIG_LTO
#define CONFIG_POWER_SIGNAL_INTERRUPT_STORM_DETECT_THRESHOLD 30
#define CONFIG_TABLET_MODE
#define CONFIG_TEMP_SENSOR
#define CONFIG_THERMISTOR_NCP15WB
#define CONFIG_STEINHART_HART_3V3_13K7_47K_4050B
#define CONFIG_STEINHART_HART_3V3_51K1_47K_4050B
#define CONFIG_DPTF
#define CONFIG_SCI_GPIO GPIO_PCH_SCI_L
#define CONFIG_VBOOT_HASH
#define CONFIG_VOLUME_BUTTONS
#define GPIO_VOLUME_DOWN_L GPIO_EC_VOLDN_BTN_ODL
#define GPIO_VOLUME_UP_L GPIO_EC_VOLUP_BTN_ODL
#define CONFIG_BACKLIGHT_LID
#define CONFIG_WIRELESS
#define CONFIG_WIRELESS_SUSPEND EC_WIRELESS_SWITCH_WLAN_POWER
#define CONFIG_WLAN_POWER_ACTIVE_LOW
#define WIRELESS_GPIO_WLAN_POWER GPIO_WIRELESS_GPIO_WLAN_POWER
#define CONFIG_PWR_STATE_DISCHARGE_FULL

/*
 * Enable 1 slot of secure temporary storage to support
 * suspend/resume with read/write memory training.
 */
#define CONFIG_VSTORE
#define CONFIG_VSTORE_SLOT_COUNT 1

#undef CONFIG_UART_TX_BUF_SIZE
#define CONFIG_UART_TX_BUF_SIZE 4096
#undef CONFIG_UART_RX_BUF_SIZE
#define CONFIG_UART_RX_BUF_SIZE 512

#ifndef __ASSEMBLER__

#include "gpio_signal.h"
#include "registers.h"

/* I2C ports */
#define I2C_PORT_USB_MUX		IT83XX_I2C_CH_C
#define I2C_PORT_BATTERY		IT83XX_I2C_CH_E
#define I2C_PORT_CHARGER		IT83XX_I2C_CH_E

/* ADC signal */
enum adc_channel {
	ADC_TEMP_SENSOR_CHARGER,	/* ADC CH1 */
	ADC_TEMP_SENSOR_AMB,		/* ADC CH2 */
	ADC_BOARD_ID,			/* ADC CH3 */
	ADC_CH_COUNT
};

enum temp_sensor_id {
	TEMP_SENSOR_BATTERY = 0,
	TEMP_SENSOR_AMBIENT,
	TEMP_SENSOR_CHARGER,
	TEMP_SENSOR_COUNT
};

enum reef_it8320_board_version {
	BOARD_VERSION_UNKNOWN = -1,
	BOARD_VERSION_1,
	BOARD_VERSION_2,
	BOARD_VERSION_3,
	BOARD_VERSION_4,
	BOARD_VERSION_5,
	BOARD_VERSION_6,
	BOARD_VERSION_7,
	BOARD_VERSION_8,
	BOARD_VERSION_COUNT,
};

/* TODO: determine the following board specific type-C power constants */
/* FIXME(dhendrix): verify all of the below PD_* numbers */
/*
 * delay to turn on the power supply max is ~16ms.
 * delay to turn off the power supply max is about ~180ms.
 */
#define PD_POWER_SUPPLY_TURN_ON_DELAY  30000  /* us */
#define PD_POWER_SUPPLY_TURN_OFF_DELAY 250000 /* us */

/* delay to turn on/off vconn */
#define PD_VCONN_SWAP_DELAY 5000 /* us */

/* Define typical operating power and max power */
#define PD_OPERATING_POWER_MW 15000
#define PD_MAX_POWER_MW       45000
#define PD_MAX_CURRENT_MA     3000
#define PD_MAX_VOLTAGE_MV     20000

/* Reset PD MCU */
void board_reset_pd_mcu(void);
int board_get_version(void);

#endif /* !__ASSEMBLER__ */

#endif /* __CROS_EC_BOARD_H */
