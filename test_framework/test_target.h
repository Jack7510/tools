//
//  process_json.hpp
//  adb_test
//
//  Created by Jack Lee on 17/3/21.
//  Copyright © 2017年 Jack Lee. All rights reserved.
//

#ifndef __TEST_TARGET_H__
#define __TEST_TARGET_H__

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_REQUEST     -1  // format error
#define METHOD_NOT_FOUND    -2  // method not found

//int process_cmd(char* cmd, char* response, int size );
void process_test_msg( message_buf* recv_buf, message_buf* send_buf );
    
//
// there are functions to test devices
//
int test_quit(void);
int test_add( int a, int b );
int test_sub( int a, int b );
    
#ifdef __cplusplus
}		// extern "C"
#endif


#endif /* __TEST_TARGET_H__ */
