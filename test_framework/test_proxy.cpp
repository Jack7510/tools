//
// file: test_proxy.cpp
// desc: act as test proxy for Robot Framework and task target
// usage:  test_proxy -t tid
//

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "msg.h"

/* supports the following command-line arguments:
 * -t tid - task id which message would be sent
 * -I - don't produce a keyword index
 * -l lang - produce output in the specified language, lang
 * -o outfile - write output to outfile instead of stdout
 * -v - be verbose; more -v means more diagnostics
 * additional file names are used as input files
 *
 * The optString global tells getopt() which options we
 * support, and which options have arguments.
 */
struct globalArgs_t {
	//int noIndex;				/* -I option */
	//char *langCode;				/* -l option */
	//const char *outFileName;	/* -o option */
	//FILE *outFile;
	//int verbosity;				/* -v option */
	//char **inputFiles;			/* input files */
	//int numInputFiles;			/* # of input files */
    int tid;                        /* -t option */
    char* json_para;				/* -j json rpc param*/
} globalArgs;


//static const char *optString = "Il:o:vh?";
static const char *optString = "j:t:h?";


/* Display program usage, and exit.
 */
void display_usage( void )
{
    fprintf(stderr, "usage: proxy -t tid -j json_string \n tid: 0 - %d", \
         TID_TEST_PROXY - 1 );	/* ... */

	exit( EXIT_FAILURE );
}


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
    	exit(1);
        break;
    default:
        break;
    }
}

//
// func: proxy
// desc: test_proxy main function
// para: tid - the task id of which message would be sent
// para: json_cmd -  json string 
//
int proxy( int tid, char* json_cmd, int len )
{
	int dst_task_id;
	int src_task_id;
	int msg_len;
	MSG_BUF* send_buf;
	MSG_BUF* recv_buf;

    // prepare parameters
	dst_task_id = tid;
	src_task_id = TID_TEST_PROXY;
	msg_len 	= MAX_MSG_BUF_SIZE;
	//printf("dst %d, src %d, len %d\n", dst_task_id, src_task_id, msg_len );

    // create my msg queue
    //remove_message_queue( src_task_id );
	create_message_queue( src_task_id );

    // build up sending message
	send_buf = MALLOC_MSG( msg_len );
	if( send_buf == NULL )
	{
		fprintf(stderr, "out of memory %d\n", __LINE__ );
		return -1;
	}

    // send message to test_target
    FILL_MSG_BUF( send_buf, json_cmd, len );
    send_message( dst_task_id, src_task_id, send_buf );

    // build up receiving message
    recv_buf = MALLOC_MSG( msg_len );
	if( recv_buf == NULL )
	{
		fprintf(stderr, "out of memory %d\n", __LINE__ );
		return -1;
	}
    memset( recv_buf->mtext, 0, msg_len );

    // receive message from test_target
    while( 0 > recv_message( src_task_id, recv_buf, sizeof(MSG_BUF) + msg_len, MSG_WAIT ) )
    {
    	printf("recv_message Error\n");
    	break;
    }
    
	// send json message to host stdout(Python input)
	fprintf(stdout, "%s\n", (char*)GET_MSG_TEXT(recv_buf) );
	
	free( send_buf );
	free( recv_buf );
	return 0;
}


int main(int argc, char* argv[])
{
    int opt = 0;

    if(signal(SIGTERM, signal_handler) == SIG_ERR)
    {
        fprintf(stderr, "Error signal SIGTERM will not be handled\n" );
    }

    if(signal(SIGINT, signal_handler) == SIG_ERR)
    {
        fprintf(stderr, "Error signal SIGINT will not be handled\n" );
    }

	/* Initialize globalArgs before we get to work. */
	globalArgs.tid = -1;
	globalArgs.json_para = NULL;

	/* Process the arguments with getopt(), then
	 * populate globalArgs.
	 */
	opt = getopt( argc, argv, optString );
	while( opt != -1 ) {
		switch( opt ) {
			case 't':
				globalArgs.tid = atoi(optarg);
				break;

			case 'j':
				globalArgs.json_para = optarg;
				break;

			case 'h':	/* fall-through is intentional */
			case '?':
				display_usage();
				break;

			default:
				/* You won't actually get here. */
				break;
		}

		opt = getopt( argc, argv, optString );
	}

    // check tid parameter
    if( -1 == globalArgs.tid || NULL == globalArgs.json_para )
    {
        display_usage();
        return EXIT_FAILURE;
    }

    //printf("json_rpc: %s\n", globalArgs.json_para);
	proxy( globalArgs.tid, globalArgs.json_para, strlen(globalArgs.json_para) );

	return EXIT_SUCCESS;
}
