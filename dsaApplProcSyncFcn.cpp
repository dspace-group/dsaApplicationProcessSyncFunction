/****************************************************************************\
 ***
 *** File                  : dsaApplProcSyncFcn.cpp
 ***
 *** Generated by dSPACE Tools.
 *** (c) Copyright 2012, dSPACE GmbH. All rights reserved.
\****************************************************************************/

#  include "dsaApplProcSyncFcn.h"
#  include <dssimengine_api.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdint.h>

/* For usage of dSPACE defined types. */
#include <dstypes.h>
#include <DsInterruptDriver.h>


#define USER_DRIVER_TYPE DsCInterruptDriver

struct Receive_from_simulink_model
{
	int receive_event;
	int send_event;
};

struct msg_buffer
{
	long msg_type;
	char msg_text[64];
};

struct Application_Process_Sync_USER_DATA_STRUCT;


typedef struct {
      USER_DRIVER_TYPE* pDrv;
      struct Application_Process_Sync_USER_DATA_STRUCT* pUserData;
} Application_Process_Sync_INSTANCE_STRUCT;


#define Application_Process_Sync_INSTANCE_BUFFER_CREATE(P_USER_DATA,PHYS_LAYER_IF) \
            Application_Process_Sync_USER_DATA_STRUCT* P_USER_DATA = new Application_Process_Sync_USER_DATA_STRUCT; \
            memset(P_USER_DATA,0,sizeof(Application_Process_Sync_USER_DATA_STRUCT)); \
            Application_Process_Sync_INSTANCE_STRUCT* pInstanceStruct = new Application_Process_Sync_INSTANCE_STRUCT; \
            memset(pInstanceStruct,0,sizeof(Application_Process_Sync_INSTANCE_STRUCT)); \
            pInstanceStruct->pUserData = P_USER_DATA; \
            pBlockInstance->PHYS_LAYER_IF.rtObjects = pInstanceStruct;


#define Application_Process_Sync_DRIVER_CREATE(P_DRIVER,PHYS_LAYER_IF) \
            USER_DRIVER_TYPE* P_DRIVER = USER_DRIVER_TYPE::create(ErrorList, SimEngineApplGet()); \
            ((Application_Process_Sync_INSTANCE_STRUCT*) (pBlockInstance->PHYS_LAYER_IF.rtObjects))->pDrv = P_DRIVER; \


#define Application_Process_Sync_DRIVER_BIND_FUNCTION(P_DRIVER,FUNCTION) \
            for(int i=0; i<pBlockInstance->FUNCTION.fcnModuleCount; i++) \
            { \
            pBlockInstance->FUNCTION.fcnModules[i]->bindDriver(ErrorList, P_DRIVER); \
            }


#define Application_Process_Sync_DRIVER_TRIGGER_EVENT(P_DRIVER,EVENT) \
            pBlockInstance->EVENT##_driver = P_DRIVER;


#define Application_Process_Sync_DRIVER_BIND_CHANNEL(P_DRIVER,RESOURCE,TYPE,ROLE) \
            P_DRIVER->bindIoChannel(ErrorList, \
            DsCIoChannel::getInstance(ErrorList, DS_IO_MAP(g_ResourceMap, pBlockInstance->resIndex_##RESOURCE )) \
            ,TYPE ,ROLE );


#define Application_Process_Sync_INSTANCE_BUFFER_GET(P_USER_DATA,PHYS_LAYER_IF) \
            Application_Process_Sync_USER_DATA_STRUCT* P_USER_DATA = ((Application_Process_Sync_INSTANCE_STRUCT*) (pBlockInstance->PHYS_LAYER_IF.rtObjects))->pUserData;


#define Application_Process_Sync_DRIVER_GET(P_DRIVER,PHYS_LAYER_IF) \
            USER_DRIVER_TYPE* P_DRIVER = ((Application_Process_Sync_INSTANCE_STRUCT*) (pBlockInstance->PHYS_LAYER_IF.rtObjects))->pDrv;


#define Application_Process_Sync_PARAMETER(PARAM) (pBlockInstance->PARAM)


struct Application_Process_Sync_USER_DATA_STRUCT {
      
      Receive_from_simulink_model model_values;
	msg_buffer send_msg;
	msg_buffer recv_msg;
	int msgid;
	int myThreadValue; 
	uint32_t key;
	pthread_t threadId;
	pthread_attr_t attr;
	
};

void* myWorkerThread(void* arg)
{
	// Create buffer and get driver 
	ucf_Application_Process_SyncStruct_T * pBlockInstance = (ucf_Application_Process_SyncStruct_T*) arg;
	Application_Process_Sync_INSTANCE_BUFFER_GET(myData,Physical_Interface);
	Application_Process_Sync_DRIVER_GET(myDriver,Physical_Interface);
	
      // Set msg type
	myData->recv_msg.msg_type=1;
      // Receive message 
	while (myData->myThreadValue) 
	{
            int result=msgrcv(myData->msgid, &myData->recv_msg, sizeof(myData->recv_msg.msg_text), myData->recv_msg.msg_type, 0); // first message in the queue will be read
            // Trigger the Event Port
            if(result<0) continue;
		myDriver->trigger(0);
            
	}
	
      // Exit the thread
	pthread_exit(NULL);
}


void ucf_dsaApplProcSyncFcn_Create(DsTErrorList ErrorList, ucf_Application_Process_SyncStruct_T * pBlockInstance) {
      // Create user data buffer
      Application_Process_Sync_INSTANCE_BUFFER_CREATE(myData, Physical_Interface);

      // Create driver
      Application_Process_Sync_DRIVER_CREATE(myDriver, Physical_Interface);

      // Declare driver object as trigger for EventPort
      Application_Process_Sync_DRIVER_TRIGGER_EVENT(myDriver, Event_Port);
   
}
void ucf_dsaApplProcSyncFcn_Init(DsTErrorList ErrorList, ucf_Application_Process_SyncStruct_T * pBlockInstance) {
      //Get data buffer
      Application_Process_Sync_INSTANCE_BUFFER_GET(myData, Physical_Interface);

      // Get parameter key
      myData->key = Application_Process_Sync_PARAMETER(Key);
      
      // Create message queue
      myData->msgid=msgget(myData->key, 0666| IPC_CREAT);
	if (myData->msgid==-1){
		printf("Message queue could not be created \n");
	}

      // Receive message and trigger the event via a thread
      pthread_attr_init(&myData->attr);
	myData->myThreadValue=1;
	if (pthread_create(&myData->threadId, &myData->attr, myWorkerThread,pBlockInstance))	
        {
            printf("Ethernet_Receive: Create receiving thread failed.\n");
            ucf_dsaApplProcSyncFcn_Terminate(ErrorList, pBlockInstance);
            return;  
        }

}
void ucf_dsaApplProcSyncFcn_Stop(DsTErrorList ErrorList, ucf_Application_Process_SyncStruct_T * pBlockInstance) {}
void ucf_dsaApplProcSyncFcn_Unload(DsTErrorList ErrorList, ucf_Application_Process_SyncStruct_T * pBlockInstance) {}
void ucf_dsaApplProcSyncFcn_Terminate(DsTErrorList ErrorList, ucf_Application_Process_SyncStruct_T * pBlockInstance) {
      //Get data buffer
      Application_Process_Sync_INSTANCE_BUFFER_GET(myData, Physical_Interface);   

      // Terminate thread
      myData->myThreadValue = 0; 
	pthread_join(myData->threadId, NULL);  
}
void ucf_dsaApplProcSyncFcnTrigger(ucf_Application_Process_SyncStruct_T * pBlockInstance, UInt32 * pValue) {
      
      // Get data buffer
      Application_Process_Sync_INSTANCE_BUFFER_GET(myData, Physical_Interface);   
      
      // Read the enable value
      myData->model_values.send_event =* pValue;
      
      // Send enable message
      
      myData->send_msg.msg_type=1;  
      sprintf(myData->send_msg.msg_text, "%d", myData->model_values.send_event);
      if(myData->model_values.send_event != 1) return;
      myData->myThreadValue=0;
      int rc = msgsnd(myData->msgid, &myData->send_msg, sizeof(myData->send_msg.msg_text), 0);
            if(rc != 0) {
                  fprintf(stdout, "msgsend(msgid: %d) errno: %d (%s)\n", myData->msgid, errno, strerror(errno));
                  fflush(stdout);
                  printf("Message could not be send \n");
            }
	assert(rc == 0);
      
}