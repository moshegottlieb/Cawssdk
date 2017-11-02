#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "aws.h"

AWSObjectRef SESCreate(const char* region,const char* accessKeyId,const char* secretKey,AWSResult* result);
void SESDestroy(AWSObjectRef object);
AWSErrorPolicy SESSendEmail(AWSObjectRef object, const char* from, const char** to,int toCount,const char** cc,int ccCount,const char** bcc,int bccCount,const char* subject,const char* body,int isHtml,AWSResult* result);

#ifdef __cplusplus
}
#endif