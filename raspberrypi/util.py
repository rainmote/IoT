#!/usr/bin/python
# -*- coding: utf-8 -*-

import uuid

def GetMacAddr():
    mac = uuid.UUID(int = uuid.getnode()).hex[-12:]
    return ':'.join([mac[x:x+2] for x in range(0, 11, 2)])
