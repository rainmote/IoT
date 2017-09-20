#!/usr/bin/python
# -*- coding: utf-8 -*-

import json
import time
import hmac
import hashlib
import requests
import traceback

import urllib3
urllib3.disable_warnings()


class AliyunIot():
    CONF_FILE = 'config.json'

    def __init__(self):
        self.__s = requests.Session()
        self.__config = self.__load_config(self.CONF_FILE)

    def __load_config(self, filename):
        with open(filename) as f:
            return json.load(f, encoding='utf-8')

    def __del__(self):
        self.__s.close()

    def GetSign(self, secret, param):
        black_key_list = ['version', 'sign', 'resources', 'signmethod']
        p = filter(lambda x:x[0] not in black_key_list, param.items())
        c = ''.join([''.join(str(j) for j in i) for i in sorted(p, key=lambda x:x[0])])
        return hmac.new(str(secret), c, hashlib.md5).hexdigest()

    def DeviceAuthentication(self, device_name, client_id, resources = 'mqtt'):
        timestamp = int(time.time())

        param = {'productKey': self.__config['productKey'],
                 'deviceName': device_name,
                 'sign': None,
                 'signmethod': 'hmacmd5',
                 'clientId': client_id,
                 'timestamp': timestamp,
                 'resources': resources}

        param['sign'] = self.GetSign(self.__config['deviceSecret'], param)

        pre = requests.Request('POST', url=self.__config['url'], data=param).prepare()
        res = self.__s.send(pre, verify=False, timeout=3)
        if res.text:
            try:
                resp = json.loads(res.text)
                if resp['code'] == 200:
                    return resp['data']
            except:
                print res.text
                print 'Http request failed.'
                print traceback.print_exc()
        return None,None

if __name__ == '__main__':
    a = AliyunIot()
    print a.DeviceAuthentication('temperature_sensor', '01')
