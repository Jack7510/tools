//
// file: test_target.cpp
// desc: sample of test target
// command line:  test_target tid
//

#include <cstdlib>
#include <iostream>
#include <string>

#include <json/json.h>
#include "msg.h"

#include "test_target.h"

// 关闭这个宏，关闭main代码段
#define TEST_SELF 1


//
//  test_xxx 用于示范测试函数
//
int test_add( int a, int b )
{
    return a + b;
}


int test_sub( int a, int b )
{
    return a - b;
}


static int g_run = 0;

int test_quit(void)
{
    g_run = 0;
    
    return 1;
}



#ifdef TEST_SELF
//
// 以下代码可以不要
//

/**
 * \brief Signal management.
 * \param code signal code
 */
static void signal_handler(int code)
{
    switch(code)
    {
        case SIGINT:
        case SIGTERM:
            g_run = 0;
            break;
        default:
            break;
    }
}


int main( int argc, char* argv[] )
{
	int dst_task_id;
	int src_task_id;
	int msg_len;
	message_buf* send_buf;
	message_buf* recv_buf;

	g_run = 1;
	if(signal(SIGTERM, signal_handler) == SIG_ERR)
    {
        fprintf(stderr, "Error signal SIGTERM will not be handled\n" );
    }

    if(signal(SIGINT, signal_handler) == SIG_ERR)
    {
        fprintf(stderr, "Error signal SIGINT will not be handled\n" );
    }

	if( argc < 2 )
	{
		printf("usage: test_target tid, default as 0 \n");
        src_task_id = 0;
		//return -1;
	}
    else
    {
        src_task_id = atoi(argv[1]);
    }

	dst_task_id = TID_TEST_PROXY;
	msg_len 	= MAX_MSG_BUF_SIZE;
	printf("target id: %d\n", src_task_id );

	send_buf = (message_buf*) malloc( sizeof(message_buf) + msg_len );
	if( send_buf == NULL )
	{
		fprintf(stderr, "out of memory %d\n", __LINE__ );
		return -1;
	}

	recv_buf = (message_buf*) malloc( sizeof(message_buf) + msg_len );
	if( recv_buf == NULL )
	{
		fprintf(stderr, "out of memory %d\n", __LINE__ );
		return -1;
	}

	// create my msg queue
	create_message_queue( src_task_id );

	while( g_run )
	{
		// recv from queue
		memset( recv_buf->mtext, 0, msg_len );
		if( 0 > recv_message( src_task_id, recv_buf, sizeof(message_buf) + msg_len, MSG_WAIT ) )
            continue;
		printf("<---%s\n", recv_buf->mtext );
        
        // from test proxy, do test job
        process_test_msg(recv_buf, send_buf);

	}

	free( send_buf );
	free( recv_buf );

	fprintf(stdout, "Test target exit!\n" );
	return 0;
}

#endif

