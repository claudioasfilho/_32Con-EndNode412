# SoC - bt_AM_Conn_EN

The Bluetooth example has a custom service and three Custom Characteristics. 

It advertises as **"PeriDevxxxx"**, where the last 4 digits are the two last bytes of the device Mac Address.

It is meant to be used in conjuction to Central device: https://github.com/claudioasfilho/conn_interval_central_CI 

The Central device scans and connects to up to 32 peripheral devices which advertise a specific service "CCCC".

Once it finds the device, it will subscribe to notifications.

By Pressing PB0 it will start the test, where the Peripheral nodes will send a send a substantial amount of data and after the TEST_TIMEOUT runs out, the Central device will printout on the UART a report showing how much data was received from each connected node and its Upstream throughput.


Bytes 12-17: Device Mac Address

**DataService - UUID: CCCC**
   Characteristics:
      |_ Data_TX -  UUID: 3A4855F62F9C4005815BDF746358668E
                    Properties: Read - 64 bytes
      |_ Data_RX -  UUID: C165393CF3374EFD8C8396226D85DE6E
                    Properties: Read/Write no Response - 5 bytes
      |_ Data_RX -  UUID: b3827d2fc86c46b69fa312e64d7be05d
                    Properties: Notify - 255 bytes

**Other details:**

Hardware: Thunderboard BG22 - **SLTB010A**
https://www.silabs.com/development-tools/thunderboard/thunderboard-bg22-kit?tab=overview 

Compiler: GCC GNU ARM v10.2.1
**Gecko SDK Suite Version: 4.1.2**: Amazon 202012.00, Bluetooth 4.2.0, Bluetooth Mesh 3.0.2, EmberZNet 7.1.2.0, Flex 3.4.2.0, HomeKit 1.2.2.0, MCU 6.3.1.0, Matter Demo, Micrium OS Kernel 5.13.10, OpenThread 2.1.2.0 (GitHub-2ce3d3bf0), Platform 4.1.0.0, USB 1.0.0.0, Wi-SUN 1.3.2.0, Z-Wave SDK 7.18.2.0


## Resources

[Bluetooth Documentation](https://docs.silabs.com/bluetooth/latest/)

[UG103.14: Bluetooth LE Fundamentals](https://www.silabs.com/documents/public/user-guides/ug103-14-fundamentals-ble.pdf)

[QSG169: Bluetooth SDK v3.x Quick Start Guide](https://www.silabs.com/documents/public/quick-start-guides/qsg169-bluetooth-sdk-v3x-quick-start-guide.pdf)

[UG434: Silicon Labs Bluetooth ® C Application Developer's Guide for SDK v3.x](https://www.silabs.com/documents/public/user-guides/ug434-bluetooth-c-soc-dev-guide-sdk-v3x.pdf)

[Bluetooth Training](https://www.silabs.com/support/training/bluetooth)

