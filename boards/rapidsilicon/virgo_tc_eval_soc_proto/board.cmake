# SPDX-License-Identifier: Apache-2.0

board_runner_args(openocd "--use-elf" "--gdb-init=target extended-remote 10.0.8.101:3333" "--gdb-port=3333")
set(OPENOCD_FLASH "debug")
include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)