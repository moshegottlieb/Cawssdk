#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "aws.h"

AWSObjectRef PIM_NONNULL SQSCreate(const char* PIM_NONNULL region,const char* PIM_NONNULL accessKeyId,const char* PIM_NONNULL secretKey,AWSResult* PIM_NONNULL result);
void SQSDestroy(AWSObjectRef PIM_NONNULL  object);
AWSErrorPolicy SQSSendMessage(AWSObjectRef PIM_NONNULL  object, const char* PIM_NONNULL queueUrl, const char* PIM_NONNULL message,AWSResult* PIM_NONNULL result);

typedef int (*SQSMessageReceivedCallback)(void* PIM_NONNULL ctx,const char* PIM_NONNULL id,const char* PIM_NONNULL message,const char* PIM_NONNULL receipt);
AWSErrorPolicy SQSReceiveMessages(AWSObjectRef PIM_NONNULL object, const char* PIM_NONNULL queueUrl,int maxBatch,int timeoutSeconds,void* PIM_NONNULL context,SQSMessageReceivedCallback PIM_NONNULL handler,AWSResult* PIM_NONNULL result);
AWSErrorPolicy SQSDeleteMessage(AWSObjectRef PIM_NONNULL object,const char* PIM_NONNULL queueUrl, const char* PIM_NONNULL handle,AWSResult* PIM_NONNULL result); 


#ifdef __cplusplus
}
#endif