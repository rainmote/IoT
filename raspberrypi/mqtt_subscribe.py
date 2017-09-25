#!/usr/bin/python
# -*- coding: utf-8 -*-

from util import GetMacAddr
from iot_serial import Serial
from aliyun_iot import AliyunIot 
import paho.mqtt.client as mqtt

TOPIC = '/JVMwetHXFGK/temperature_sensor/m2m'

def ConnectCallback(client, userdata, flags, rc):
    print 'Connected:' + str(rc)
    client.subscribe(TOPIC)

def MessageCallback(client, userdata, msg):
    print '%s -> %s' %(msg.topic, str(msg.payload))

dev_id = GetMacAddr()
dev_id='test001'

a = AliyunIot('config_subscribe.json')
conf = a.DeviceAuthentication(dev_id)

client = mqtt.Client(dev_id)
client.on_connect = ConnectCallback
client.on_message = MessageCallback

client.tls_set('aliyun_iot.crt')
client.username_pw_set(conf['iotId'], conf['iotToken'])

m = conf['resources']['mqtt']
client.connect(m['host'], m['port'], 80)
client.loop_forever()
