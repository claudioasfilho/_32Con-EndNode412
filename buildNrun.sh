#!/bin/sh

cd GNU\ ARM\ v10.2.1\ -\ Default/
make -j11 all

commander flash bt_CI2_Conn_EN.hex -s 440179444