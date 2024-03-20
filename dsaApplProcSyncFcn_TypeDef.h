/****************************************************************************\
 ***
 *** File                  : dsaApplProcSyncFcn_TypeDef.h
 ***
 *** Generated by dSPACE Tools.
 *** (c) Copyright 2012, dSPACE GmbH. All rights reserved.
\****************************************************************************/

#if !defined(UF_APPLICATION_PROCESS_SYNC_TYPEDEF_H_)
#  define    UF_APPLICATION_PROCESS_SYNC_TYPEDEF_H_

#  include <string.h>
#  include <Dstypes.h>
#  include <DsApplicationInterface.h>

typedef struct ucf_Application_Process_SyncStruct
{

    char* functionBlockName;
    UInt32 Len_functionBlockName;
    Int32 initialValueUsage;
    Int32 systemType; // 0: SCALEXIO, 1: MABX
    const char* getArtifactsDirectory()
    {
        static char buffer[1024];
        const char* applArtifactsDir = dsGetApplDownloadDirPath(1);
        const char* subDir = "ApplicationProcessSync";
        sprintf(buffer, "%s/%s", applArtifactsDir, subDir);
        return buffer;
    }

    UInt32 Key;

    DsCIoDriver *Event_Port_driver;

    struct
    {
        void *rtObjects;
    } Physical_Interface;

    struct
    {
        int fcnModuleCount;
        DsCFcnModule **fcnModules;
        struct
        {
            UInt32 startVal;
            int signalWidth;
            int *fcnModuleIndex;
            int fcnModuleIndexCount;
        } Send_Trigger;

    } Trigger;

} ucf_Application_Process_SyncStruct_T;
#endif
