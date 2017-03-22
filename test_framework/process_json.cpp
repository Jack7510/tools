//
//  process_json.cpp
//  adb_test
//
//  Created by Jack Lee on 17/3/21.
//  Copyright © 2017年 Jack Lee. All rights reserved.
//

//#include "process_json.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

#include <json/json.h>
#include "msg.h"
#include "test_target.h"

typedef void (*func) (const Json::Value& msg, Json::Value& response);
struct stMethod {
    std::string name;
    func        method;
};


//
// {"jsonrpc": "2.0", "method": "sub", "params": [23,  42], "id": 3}
//
void call_add(const Json::Value& root, Json::Value& rsp)
{
    int a = root["params"][0].asInt();
    int b = root["params"][1].asInt();
    
    rsp["result"]  = test_add( a, b );
    rsp["error"] 	= Json::Value::null;
}


//
// {"jsonrpc": "2.0", "method": "sub", "params": {"a": 23, "b": 42}, "id": 3}
//
void call_sub(const Json::Value& root, Json::Value& rsp)
{
    // add some code to do your test job
    int a = root["params"]["a"].asInt();
    int b = root["params"]["b"].asInt();
    
    rsp["result"]  = test_sub( a, b );
    rsp["error"] 	= Json::Value::null;
}


void call_bye(const Json::Value& root, Json::Value& rsp)
{
    // exit
    test_quit();
    
    rsp["result"]	= "OK";
    rsp["error"] 	= Json::Value::null;
}


void call_echo(const Json::Value& root, Json::Value& rsp)
{
    rsp["result"]	= root["params"];
    rsp["error"] 	= Json::Value::null;
    
}


//
//  add command map here
//
struct stMethod method_map[] =
{
    {"echo",    call_echo},
    {"add",     call_add},
    {"sub",     call_sub},
    {"bye",     call_bye},
};



bool check_json(const Json::Value& root, Json::Value& error)
{
    Json::Value err;
    
    /* check the JSON-RPC version => 2.0 */
    if(!root.isObject() || !root.isMember("jsonrpc") ||
       root["jsonrpc"] != "2.0")
    {
        error["id"] = Json::Value::null;
        error["jsonrpc"] = "2.0";
        
        err["code"] = INVALID_REQUEST;
        err["message"] = "Invalid JSON-RPC request.";
        error["error"] = err;
        return false;
    }
    
    if(root.isMember("id") && (root["id"].isArray() || root["id"].isObject()))
    {
        error["id"] = Json::Value::null;
        error["jsonrpc"] = "2.0";
        
        err["code"] = INVALID_REQUEST;
        err["message"] = "Invalid JSON-RPC request.";
        error["error"] = err;
        return false;
    }
    
    /* extract "method" attribute */
    if(!root.isMember("method") || !root["method"].isString())
    {
        error["id"] = Json::Value::null;
        error["jsonrpc"] = "2.0";
        
        err["code"] = INVALID_REQUEST;
        err["message"] = "Invalid JSON-RPC request.";
        error["error"] = err;
        return false;
    }
    
    return true;
}

//
// func: process_cmd
// para: cmd - command string which to be processed
// para: response - JSON string
// return: false if fail
//
int process_cmd(char* cmd, char* response, int size )
{
    // --> { "method": "echo", "params": ["Hello JSON-RPC"], "id": 1}
    // <-- { "result": "Hello JSON-RPC", "error": null, "id": 1}
    Json::Value root;
    Json::Reader reader;
    Json::Value rsp;
    Json::FastWriter writer;
    Json::Value err;
    std::string out;
    std::string strValue = cmd;
    
    if (reader.parse(strValue, root))
    {
        if( check_json(root, err ) == false )
        {
            out = writer.write(err);
        }
        else
        {
            // lookup method name
            int  i;
            std::string name 	= root["method"].asString();
            for ( i = 0; i < sizeof(method_map) / sizeof(struct stMethod) ; i++) {
                if (name == method_map[i].name) {
                    method_map[i].method( root, rsp );
                    break;
                }
            }
            
            // not found
            if (i == sizeof(method_map) / sizeof(struct stMethod))
            {
                /* forge an error response */
                err["code"] = METHOD_NOT_FOUND;
                err["message"] = "Method not found.";
                rsp["error"] = err;
            }
            
            rsp["jsonrpc"]  = "2.0";
            rsp["id"] 		= root["id"];
        }
        
        out = writer.write(rsp);
        out.copy(response, size);
        
        return 1;
    }
    
    return 0;
}


//
// process test proxy message
//
void process_test_msg( message_buf* recv_buf, message_buf* send_buf )
{
    char response[512];
    
    if( GET_MSG_SRC_TID(recv_buf) == TID_TEST_PROXY )
    {
        memset( response, 0, sizeof(response) );
        if( process_cmd( (char*)GET_MSG_TEXT(recv_buf), response, sizeof(response) ) )
        {
            //send msg back to client
            FILL_MSG_BUF( send_buf, response, strlen(response) );
            send_message( TID_TEST_PROXY, GET_MSG_DST_TID(recv_buf), (message_buf*)send_buf );
            fprintf(stdout, "--->%s\n", GET_MSG_TEXT(send_buf));
        }
    }
}
