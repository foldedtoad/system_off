System Off demo with SEEED xiao-ble  
===================================

Overview
********

This sample can be used for basic power measurement and as an example of
deep sleep on Nordic platforms.

RAM Retention
=============

On nRF52-series platforms this also can demonstrate RAM retention.  By selecting
``CONFIG_APP_RETENTION=y`` state related to number of boots, number of times
system off was entered, and total uptime since initial power-on are retained
in a checksummed data structure.  The POWER peripheral is configured to keep
the containing RAM section powered while in system-off mode.

NOTE: The xiao_ble does not have an on-board user button (momentary switch), so
the xiao_ble.overlay file define a gpio to support a button...which you must supply.

Requirements
************

This application uses nRF52-series boards for the demo.  
In particular, the Seeed xiao_ble is used as the primary example.

Sample Output
=============

nRF52 core output
-----------------

.. code-block:: console

   *** Booting Zephyr OS build v2.3.0-rc1-204-g5f2eb85f728d  ***

   nrf52dk system off demo
   Entering system off; press sw0 to restart
