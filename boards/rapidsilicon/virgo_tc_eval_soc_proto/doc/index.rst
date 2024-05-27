.. virgo:

Rapidsi Virgo
####################

Overview
********

.. image:: img/virgo.jpg
     :align: center
     :alt: Virgo

More information can be found on ' ' websites.

Hardware
********

Summary:

The virgo platform provides following hardware components:

- 1 core 32-bit 266MHz AndeStar v5 RISC-V CPUs
- 64KB on-board ILM
- 32KB on-board DLM
- 2MB SPI flash memory (Can be used for XIP)
- UART
- I2C
- SPI
- GPIO
- PWM
- DMA
- Crypto Modules

Supported Features
==================

The ``virgo`` board configuration supports the following hardware features:

+----------------+------------+----------------------+
| Interface      | Controller | Driver/Component     |
+================+============+======================+
| CLIC           | on-chip    | interrupt_controller |
+----------------+------------+----------------------+
| RISC-V Machine | on-chip    | timer                |
| Timer          |            |                      |
+----------------+------------+----------------------+
| GPIO           | on-chip    | gpio                 |
+----------------+------------+----------------------+
| UART           | on-chip    | serial               |
+----------------+------------+----------------------+
| COUNTER        | on-chip    | counter              |
+----------------+------------+----------------------+
| SPI            | on-chip    | spi                  |
+----------------+------------+----------------------+
| I2C            | on-chip    | i2c                  |
+----------------+------------+----------------------+
| FLASH          | on-chip    | flash                |
+----------------+------------+----------------------+
| HWINFO         | on-chip    | syscon               |
+----------------+------------+----------------------+
| DMA            | on-chip    | dma                  |
+----------------+------------+----------------------+
| WATCHDOG       | on-chip    | wdt                  |
+----------------+------------+----------------------+
| CRYPTO         | on-chip    | pufcc                |
+----------------+------------+----------------------+

Other hardware features are not supported yet.

Connections and IOs
===================

The Rapidsi Virgo platform has 1 GPIO controller. It providing 8 bits of IO.
It is responsible for pin input/output, while the pull-up, etc are controlled
by the pin controller module

Mapping from GPIO controller to the Virgo Evaluation board pins:

+--------------------+--------------------+
| GPIO controller    | Usage / Board pins |
+====================+====================+
| **Push Buttons**   |                    |
+--------------------+--------------------+
| GPIO.x             | -                  |
+--------------------+--------------------+

Other peripheral mapping are listed below:

+-------------+---------------------------------+
| Peripherals | Usage / Board pins              |
+=============+=================================+
| SPI_1       | internal connected to SPI Flash |
+-------------+---------------------------------+

System Clock
------------

The Rapidsi Virgo platform has 266MHz core clock.

Serial Port
-----------

The Rapidsi Virgo platform has 1 UART.
The Zephyr console output is by default assigned to UART0 and the default
settings are 115200 8N1.

Programming and debugging
*************************

For debugging zephyr applications or burning them into a flash, you will need to
connect Rapidsi OpenOCD Port host computer to Virgo Evaluation board and execute the
west flash command by filling in the appropriate IP and Port number in the board.cmake
file.

For Stepwise interactive debugging, follow the procedure mentioned below:

interactive Zephyr Debugging
===========================

In order to do debugging of Zephyr RTOS over SoC prototype, follow the steps below:

1 - In the project navigation pan of the VS Code, create a folder ‘.vscode’. Under this folder, create a file called launch.json as shown in picture below:



Connecting Andes ICE (AICE)
===========================

AICE is used for flashing and debugging the board. Please connect AICE to both
ADP-XC7K board and the host computer as shown in the figure.

.. image:: img/connect_aice.jpg
     :align: center
     :alt: Connect AICE

More information can be found on `AICE-MINI+`_, `AICE-MICRO`_ website

Building
========

You can build applications in the usual way. Here is an example for
the :ref:`hello_world` application.

.. zephyr-app-commands::
   :zephyr-app: samples/hello_world
   :board: adp_xc7k/ae350
   :goals: build

Flashing
========

Before flashing, you have to download ICEman (``ice.zip``) from the
`Andes Development Kit`_.
If you want to use XIP mode (``CONFIG_XIP=y``), you also need to download
the flash burner (``flash.zip``).

At first, you should run ICEman when flashing and debugging program.

.. code-block:: console

   # Enable execute file permission of ICEman
   chmod a+x ./ICEman

   # Running the ICEman server
   sudo ./ICEman -Z v5

.. note::

   To run ICEman commands as a normal user, you will need to install the
   :file:`70-ndsusb-v1.rules` udev rules file (usually by placing it in
   :file:`/etc/udev/rules.d`, then unplugging and plugging the
   AICE adapter in again via USB.).

If ``CONFIG_XIP=n``, you can load the program (``zephyr.elf``) into RAM directly
and execute it.

.. code-block:: console

   # Check the ICEman server is running
   # Load the program into RAM and execute it
   riscv64-zephyr-elf-gdb build/zephyr/zephyr.elf
   (gdb) target remote :1111
   (gdb) monitor reset halt
   (gdb) load
   (gdb) quit

If ``CONFIG_XIP=y``, you need to burn the program (``zephyr.bin``) into flash memory
and execute it.

.. code-block:: console

   # Check the ICEman server is running
   # Burn the program into flash and execute it
   <FLASH>/bin/target_burn_frontend \
       -P 4444 --unlock --verify --image=build/zephyr/zephyr.bin \
       --algorithm-bin=<FLASH>/target_bin/target_SPI_v5_[32|64].bin

   # Note:
   #   1. Assume the flash burner is downloaded to <FLASH> directory
   #   2. For algorithm-bin file, use target_SPI_v5_32.bin in RV32 platform and
   #      use target_SPI_v5_64.bin in RV64 platform

Open a serial terminal with the following settings:

- Speed: 115200
- Data: 8 bits
- Parity: None
- Stop bits: 1

you should see the following message in the terminal:

.. code-block:: console

   ***** Booting Zephyr OS v2.4.0 *****
   Hello World! adp_xc7k

Debugging
=========

.. code-block:: console

   # Check the ICEman server is running
   # Load and debug program
   ./riscv64-zephyr-elf-gdb build/zephyr/zephyr.elf
   (gdb) target remote :1111
   (gdb) monitor reset halt
   (gdb) load

If ``CONFIG_XIP=y``, please follow the flashing section to burn the program into
flash memory first.
Then, you can use GDB to debug program by above commands but do NOT execute ``load``
command since the program has been placed in the flash memory.

References
**********

.. target-notes::

.. _ADP-XC7K160/410: http://www.andestech.com/en/products-solutions/andeshape-platforms/adp-xc7k160-410/

.. _AndeShape AE350: http://www.andestech.com/en/products-solutions/andeshape-platforms/ae350-axi-based-platform-pre-integrated-with-n25f-nx25f-a25-ax25/

.. _AICE-MINI+: http://www.andestech.com/en/products-solutions/andeshape-platforms/aice-mini-plus/

.. _AICE-MICRO: http://www.andestech.com/en/products-solutions/andeshape-platforms/aice-micro/

.. _Andes Development Kit: https://github.com/andestech/Andes-Development-Kit/releases
