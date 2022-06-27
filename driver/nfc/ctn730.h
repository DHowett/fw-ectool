/* Copyright 2021 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __CROS_EC_CTN730_H
#define __CROS_EC_CTN730_H

#define CTN730_I2C_ADDR 0x28

/* Size of flash address space in bytes */
#define CTN730_FLASH_ADDR_SIZE 3

/* All commands are guaranteed to finish within 1 second. */
#define CTN730_COMMAND_TIME_OUT (1 * SECOND)

/* Message Types */
#define CTN730_MESSAGE_TYPE_COMMAND 0b00
#define CTN730_MESSAGE_TYPE_RESPONSE 0b01
#define CTN730_MESSAGE_TYPE_EVENT 0b10

/* Instruction Codes */
#define WLC_HOST_CTRL_RESET 0b000000
#define WLC_HOST_CTRL_DL_OPEN_SESSION 0b000011
#define WLC_HOST_CTRL_DL_COMMIT_SESSION 0b000100
#define WLC_HOST_CTRL_DL_WRITE_FLASH 0b000101
#define WLC_HOST_CTRL_DUMP_STATUS 0b001100
#define WLC_HOST_CTRL_GENERIC_ERROR 0b001111
#define WLC_HOST_CTRL_BIST 0b000110
#define WLC_CHG_CTRL_ENABLE 0b010000
#define WLC_CHG_CTRL_DISABLE 0b010001
#define WLC_CHG_CTRL_DEVICE_STATE 0b010010
#define WLC_CHG_CTRL_CHARGING_STATE 0b010100
#define WLC_CHG_CTRL_CHARGING_INFO 0b010101

/* WLC_HOST_CTRL_RESET constants */
#define WLC_HOST_CTRL_RESET_CMD_SIZE 1
#define WLC_HOST_CTRL_RESET_RSP_SIZE 1
#define WLC_HOST_CTRL_RESET_EVT_NORMAL_MODE 0x00
#define WLC_HOST_CTRL_RESET_EVT_NORMAL_MODE_SIZE 3
#define WLC_HOST_CTRL_RESET_EVT_DOWNLOAD_MODE 0x01
#define WLC_HOST_CTRL_RESET_EVT_DOWNLOAD_MODE_SIZE 2
#define WLC_HOST_CTRL_RESET_REASON_INTENDED 0x00
#define WLC_HOST_CTRL_RESET_REASON_CORRUPTED 0x01
#define WLC_HOST_CTRL_RESET_REASON_UNRECOVERABLE 0x02
#define WLC_HOST_CTRL_RESET_CMD_MODE_NORMAL 0x00
#define WLC_HOST_CTRL_RESET_CMD_MODE_DOWNLOAD 0x01
#define WLC_HOST_CTRL_RESET_EVT_MIN_SIZE \
	WLC_HOST_CTRL_RESET_EVT_DOWNLOAD_MODE_SIZE

/* WLC_HOST_CTRL_DL_* constants */
#define WLC_HOST_CTRL_DL_OPEN_SESSION_CMD_SIZE 2
#define WLC_HOST_CTRL_DL_OPEN_SESSION_RSP_SIZE 1
#define WLC_HOST_CTRL_DL_WRITE_FLASH_BLOCK_SIZE 128
#define WLC_HOST_CTRL_DL_WRITE_FLASH_CMD_SIZE \
	(CTN730_FLASH_ADDR_SIZE + WLC_HOST_CTRL_DL_WRITE_FLASH_BLOCK_SIZE)
#define WLC_HOST_CTRL_DL_WRITE_FLASH_RSP_SIZE 1
#define WLC_HOST_CTRL_DL_COMMIT_SESSION_CMD_SIZE 4
#define WLC_HOST_CTRL_DL_COMMIT_SESSION_RSP_SIZE 1

/* WLC_CHG_CTRL_ENABLE constants */
#define WLC_CHG_CTRL_ENABLE_CMD_SIZE 2
#define WLC_CHG_CTRL_ENABLE_RSP_SIZE 1

/* WLC_CHG_CTRL_DISABLE constants */
#define WLC_CHG_CTRL_DISABLE_CMD_SIZE 0
#define WLC_CHG_CTRL_DISABLE_RSP_SIZE 1
#define WLC_CHG_CTRL_DISABLE_EVT_SIZE 1

/* WLC_CHG_CTRL_DEVICE_STATE constants */
#define WLC_CHG_CTRL_DEVICE_STATE_DEVICE_DETECTED 0x00
#define WLC_CHG_CTRL_DEVICE_STATE_DEVICE_DEACTIVATED 0x01
#define WLC_CHG_CTRL_DEVICE_STATE_DEVICE_DEVICE_LOST 0x02
#define WLC_CHG_CTRL_DEVICE_STATE_DEVICE_DEVICE_BAD_VERSION 0x03
#define WLC_CHG_CTRL_DEVICE_STATE_DEVICE_DOCKED 0x04
#define WLC_CHG_CTRL_DEVICE_STATE_DEVICE_UNDOCKED 0x05
#define WLC_CHG_CTRL_DEVICE_STATE_EVT_SIZE_DETECTED 8
#define WLC_CHG_CTRL_DEVICE_STATE_EVT_SIZE 1

/* WLC_CHG_CTRL_CHARGING_STATE constants */
#define WLC_CHG_CTRL_CHARGING_STATE_CHARGE_STARTED 0x00
#define WLC_CHG_CTRL_CHARGING_STATE_CHARGE_ENDED 0x01
#define WLC_CHG_CTRL_CHARGING_STATE_CHARGE_STOPPED 0x02
#define WLC_CHG_CTRL_CHARGING_STATE_EVT_SIZE 1

/* WLC_HOST_CTRL_DUMP_STATUS constants */
#define WLC_HOST_CTRL_DUMP_STATUS_CMD_SIZE 1

/* WLC_CHG_CTRL_CHARGING_INFO constants */
#define WLC_CHG_CTRL_CHARGING_INFO_CMD_SIZE 0
#define WLC_CHG_CTRL_CHARGING_INFO_RSP_SIZE 2
#define WLC_CHG_CTRL_CHARGING_INFO_EVT_SIZE 5

/* Status Codes */
enum wlc_host_status {
	WLC_HOST_STATUS_OK = 0x00,
	WLC_HOST_STATUS_PARAMETER_ERROR = 0x01,
	WLC_HOST_STATUS_STATE_ERROR = 0x02,
	WLC_HOST_STATUS_VALUE_ERROR = 0x03,
	WLC_HOST_STATUS_REJECTED = 0x04,
	WLC_HOST_STATUS_RESOURCE_ERROR = 0x10,
	WLC_HOST_STATUS_TXLDO_ERROR = 0x11,
	WLC_HOST_STATUS_ANTENNA_SELECTION_ERROR = 0x12,
	WLC_HOST_STATUS_BIST_FAILED = 0x20,
	WLC_HOST_STATUS_BIST_NO_WLC_CAP = 0x21,
	WLC_HOST_STATUS_BIST_TXLDO_CURRENT_OVERFLOW = 0x22,
	WLC_HOST_STATUS_BIST_TXLDO_CURRENT_UNDERFLOW = 0x23,
	WLC_HOST_STATUS_FW_VERSION_ERROR = 0x30,
	WLC_HOST_STATUS_FW_VERIFICATION_ERROR = 0x31,
	WLC_HOST_STATUS_NTAG_BLOCK_PARAMETER_ERROR = 0x32,
	WLC_HOST_STATUS_NTAG_READ_ERROR = 0x33,
};

struct ctn730_msg {
	uint8_t instruction : 6;
	uint8_t message_type : 2;
	uint8_t length;
	uint8_t payload[];
} __packed;

#endif /* __CROS_EC_CTN730_H */
