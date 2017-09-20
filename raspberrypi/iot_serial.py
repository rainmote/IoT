#!/usr/bin/python
# -*- coding: utf-8 -*-

import serial
import json
import time

class CSerial:
    def __init__(self, port, baudrate = 9600):
        self.__ser = serial.Serial(port, baudrate, timeout=0.5)

    def __del__(self):
        self.__ser.close()

    def GetData(self):
        while True:
            time.sleep(0.1)

            text = self.__ser.read(1)

            if text:
                recv = text + self.__ser.readline()
                return recv.strip().strip('\n').strip('_')

    def FlushInput(self):
        self.__ser.flushInput()


if __name__ == '__main__':
    s = CSerial('/dev/ttyUSB0')
    while True:
        s.FlushInput()

        j = json.dumps(s.GetData())
        print '%s\t%s' %(time.time(), j)
