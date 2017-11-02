#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "aws.h"

AWSObjectRef SQSCreate(const char* accessKeyId,const char* secretKey,AWSResult* result);
void SQSDestroy(AWSObjectRef object);
AWSErrorPolicy SQSSendMessage(AWSObjectRef object, const char* queueUrl, const char* message,AWSResult* result);

typedef int (*SQSMessageReceivedCallback)(void* ctx,const char* id,const char* message,const char* receipt);
AWSErrorPolicy SQSReceiveMessages(AWSObjectRef object, const char* queueUrl,int maxBatch,int timeoutSeconds,void* context,SQSMessageReceivedCallback handler,AWSResult* result);


#ifdef __cplusplus
}
#endif