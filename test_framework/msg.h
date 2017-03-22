/*
 * File		: msg.h
 * Desc		: define message communication interfaces
 * Author	: Jack Lee
 * Date		: Feb 10, 2017
 */

#ifndef __MSG_H__
#define __MSG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Declare the ids of task .
 */
enum TASK_ID
{
	TID_LAUNCHER_HWD = 0,		/* launcher Qt window */
	TID_CALL_HWD,				/* Voice call Qt window */
	TID_CALL_SERVICE,			/* Call service process */
	TID_ALARM_HWD,				/* Alarm Qt window */
	TID_ALARM_SERVCIE,			/* Alarm service process */
	TID_IM_HWD,					/* IM Qt window */
	TID_IM_SERVICE,				/* IM service process */
	TID_TEST_PROXY,				/* test proxy process */
	TID_MAX
};

/*
 * Declare the message structure.
 */
typedef struct message_buf {
	long 	mtype;       		/* message type, must be > 0 */
    int 	dst_task_id;		/* the task id who message will go */
    int 	src_task_id;		/* who send the message */
    int 	msg_len;			/* the length of mtext[] */
    char   	mtext[1];			/* message buff,  */
} MSG_BUF;

/* calc the size of whole message_buf */
#define MSG_BUF_SIZE( msg )		( msg->msg_len + sizeof(MSG_BUF) - 1 )
#define CALC_MSG_BUF_SIZE( len )	( sizeof(MSG_BUF) + len )
#define GET_MSG_SRC_TID(msg)	( msg->src_task_id )
#define GET_MSG_DST_TID(msg)	( msg->dst_task_id )
#define GET_MSG_LEN( msg )		( msg->msg_len )
//#define SET_MSG_LEN(msg, len)	( msg->msg_len = len )
#define GET_MSG_TEXT(msg)		( msg->mtext )
#define FILL_MSG_BUF(msg, content, len) \
{ \
	memcpy( msg->mtext, content, len ); \
    msg->mtext[len] = 0; \
	msg->msg_len = len; \
}

/* malloc message buf */
#define MALLOC_MSG( len )		(MSG_BUF*) malloc( sizeof(MSG_BUF) + len )
#define FREE_MSG( msg )			free( msg )

#define MAX_MSG_BUF_SIZE		512				/* max size of buffer informatoin (mtext[])*/

/*
 * Declare the message interfaces.
 */

#define MSG_WAIT 					0			/* wait until msg receive */
#define MSG_NOWAIT					1			/* do not wait msg */

/* create all queues of task which define in TASK_ID */
int create_message_queue_list(void);

/* remove all queues of task which define in TASK_ID */
int remove_message_queue_list(void);

/*
 * Func: create_message
 * Desc: create a message queue for task id
 * Para: task_id
 * Ret : 0 if OK
 */
int create_message_queue( int task_id );

/*
 * Func: remove_message_queue
 * Desc: remove the message queue of task id in system
 * Para: none
 * Ret : 0 if OK
 */
int remove_message_queue( int task_id );

/*
 * Func: send_message
 * Desc: send message to dst task
 * Para: dst_task_id - task id of recv task id
 *		 src_task_id - task id of sender task id
 *		 msg         - message to be sent
 * Ret : 0 if OK
 */
int send_message( int dst_task_id, int src_task_id, MSG_BUF* msg );

/*
 * Func: recv_message
 * Desc: receive message
 * Para: task_id - task id of message to read
 *		 msg         - message to be sent
 *		 msg_len	 - size of msg
 *		 msgflg		 - MSG_WAIT if wait, otherwise,
 * Ret : < 0 if fail, 0 is OK
 */
int recv_message( int task_id, MSG_BUF* msg, int msg_len, int msgflg );


#ifdef __cplusplus
}		// extern "C"
#endif

#endif 	// __MSG_H__
