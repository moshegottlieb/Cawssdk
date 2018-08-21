#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "aws.h"

AWSObjectRef PIM_NONNULL SESCreate(const char* PIM_NULLABLE region,const char* PIM_NONNULL accessKeyId,const char* PIM_NONNULL secretKey,AWSResult* PIM_NONNULL result);
void SESDestroy(AWSObjectRef PIM_NONNULL object);
AWSErrorPolicy SESSendEmail(AWSObjectRef PIM_NONNULL object, const char* PIM_NONNULL from, const char* PIM_NONNULL * PIM_NULLABLE to,int toCount,const char* PIM_NONNULL * PIM_NULLABLE cc,int ccCount,const char* PIM_NONNULL * PIM_NULLABLE bcc,int bccCount,const char* PIM_NONNULL subject,const char* PIM_NONNULL body,int isHtml,AWSResult* PIM_NONNULL result);

#ifdef __cplusplus
}
#endif