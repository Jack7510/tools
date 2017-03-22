#!/local/bin/env python
# file: test_host.py
# auth: Jack Lee
# Date: Mar 20, 2017

import os
import subprocess
import json

CONST_PROXY = './proxy'
CONST_TASK_TEST = 0         # TASK ID

#
# func: build_json_cmd
# desc: build a JSON RPC cmd like this {"method": "echo", "params": ["Hello JSON-RPC"], "id": 1}
# ret : json string
#
def build_json_cmd(method, params, id=1):
    # JSON script is a dict talbe in Python
    cmd = {'jsonrpc': '2.0', 'method' : method, 'params' : params, 'id' : id}
    return json.dumps(cmd)


#
# func: do_proxy_cmd
# desc: execute proxy command with task id and json parameters
# ret : json object of proxy return, and the result
#
def do_proxy_cmd(tid, json_params):
    ret_str = subprocess.check_output([CONST_PROXY, '-t', str(tid), '-j', json_params])
    json_ret = json.loads(ret_str)
    return json_ret, json_ret['error'] == None


def do_bye():
    json_rpc = build_json_cmd("bye", ["Hello JSON-RPC", "world"], 0)
    do_proxy_cmd(CONST_TASK_TEST, json_rpc)


def do_loop_test( times ):
    for i in range(times):
        # build up json parameters
        json_rpc = build_json_cmd("echo", ["Hello JSON-RPC", "world"], i)

        # execute 'adb shell test_proxy' to send test command
        json_rsp, result = do_proxy_cmd(CONST_TASK_TEST, json_rpc)

        # verify test result
        print result, json_rsp


def do_add(a, b):
    # build up json parameters
    json_rpc = build_json_cmd("add", [a, b], 0)

    # execute 'adb shell test_proxy' to send test command
    json_rsp, result = do_proxy_cmd(CONST_TASK_TEST, json_rpc)

    # verify test result
    print result, json_rsp['result']


def do_sub(a, b):
    # build up json parameters
    json_rpc = build_json_cmd("sub", {'a' : a, 'b' : b }, 0)

    # execute 'adb shell test_proxy' to send test command
    json_rsp, result = do_proxy_cmd(CONST_TASK_TEST, json_rpc)

    # verify test result
    print result, json_rsp['result']


# test program
if __name__ == '__main__':

    do_add(1, 2)
    do_sub(2, 1)
    do_loop_test(1000)
    do_bye()
