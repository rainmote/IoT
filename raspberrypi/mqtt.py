#!/usr/bin/python
# -*- coding: utf-8 -*-

import time
import json
import traceback
import paho.mqtt.client as mqtt


from util import GetMacAddr
from iot_serial import Serial
from aliyun_iot import AliyunIot 


TOPIC = '/JVMwetHXFGK/temperature_sensor/m2m'

def ConnectCallback(client, userdata, flags, rc):
    print 'Connected:' + str(rc)

dev_id = GetMacAddr()
print dev_id

a = AliyunIot()
conf = a.DeviceAuthentication('temperature_sensor', dev_id)
print conf

client = mqtt.Client(dev_id)
client.on_connect = ConnectCallback

client.tls_set('aliyun_iot.crt')
client.username_pw_set(conf['iotId'], conf['iotToken'])

m = conf['resources']['mqtt']
client.connect(m['host'], m['port'], 80)
client.loop_start()

s = Serial('/dev/ttyUSB0')
while True:
    try:
        msg = json.dumps(json.loads(s.GetData()))
        rc, mid = client.publish(TOPIC, payload=msg)
        if rc == 0:
            print '[%s]publis success, %s' %(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()), msg)
    except:
        traceback.print_exc()
    time.sleep(1)
