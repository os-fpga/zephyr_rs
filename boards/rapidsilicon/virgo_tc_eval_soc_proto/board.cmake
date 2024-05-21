# SPDX-License-Identifier: Apache-2.0

board_runner_args(
                    openocd 
                    "--cmd-load=load"
                    "--use-elf"  
                    "--elf-file=/home/junaid/zephyrproject/zephyr/build/zephyr/zephyr.elf" 
                    "--gdb-init=target extended-remote 10.0.8.102:3333"
                 )
set(OPENOCD_FLASH "load")
include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)