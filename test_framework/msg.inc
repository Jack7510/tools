/*
 * File		: msg.inc
 * Desc		: define something which msg.c internal use
 * Author	: Jack Lee
 * Date		: Feb 11, 2017
 */

#ifndef __MSG_INC__
#define __MSG_INC__

#ifdef __cplusplus
extern "C" {
#endif

#define MQ_KEY						0x1000		/* the beginning of message queue key */
#define TASK_TO_MQKEY( task_id )	( task_id + MQ_KEY)	/* convert task id to mqueue key id */
#define CHECK_TASK_ID( task_id )	( (task_id >= TID_LAUNCHER_HWD) && (task_id < TID_MAX) )

/* calc the len of struct mesage_buf for system call msgsnd()  */
#define MSG_LENGTH( msg )	( msg->msg_len + sizeof(MSG_BUF) - 1 - sizeof(long)) 
	

#ifdef __cplusplus
}		// extern "C"
#endif

#endif 	// __MSG_INC__