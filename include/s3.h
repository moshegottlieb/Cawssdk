#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "aws.h"

AWSObjectRef PIM_NONNULL S3ObjectCreate(const char* PIM_NONNULL region,const char* PIM_NONNULL accessKeyId,const char* PIM_NONNULL secretKey,AWSResult* PIM_NONNULL result);
void S3ObjectDestroy(AWSObjectRef PIM_NONNULL object);

typedef struct S3ObjectDesc {
    char* PIM_NULLABLE acl;
    char* PIM_NULLABLE storageClass;
    char* PIM_NULLABLE cacheControl;
    char* PIM_NULLABLE contentType;
} S3ObjectDesc;

S3ObjectDesc* PIM_NONNULL S3ObjectDescCreate();
void S3ObjectDescDestroy(S3ObjectDesc* PIM_NONNULL od);

void S3ObjectDescSetACL(S3ObjectDesc* PIM_NONNULL object, const char* PIM_NONNULL value);
void S3ObjectDescSetStorageClass(S3ObjectDesc* PIM_NONNULL object, const char* PIM_NONNULL value);
void S3ObjectDescSetCacheControl(S3ObjectDesc* PIM_NONNULL object, const char* PIM_NONNULL value);
void S3ObjectDescSetContentType(S3ObjectDesc* PIM_NONNULL object, const char* PIM_NONNULL value);

AWSErrorPolicy S3ObjectPut(AWSObjectRef PIM_NONNULL object,const char* PIM_NONNULL filename,const char* PIM_NONNULL bucket,const char* PIM_NONNULL key, S3ObjectDesc* PIM_NONNULL desc,AWSResult* PIM_NONNULL result);
AWSErrorPolicy S3ObjectDelete(AWSObjectRef PIM_NONNULL object,const char* PIM_NONNULL bucket,const char* PIM_NONNULL key,AWSResult* PIM_NONNULL result);

#ifdef __cplusplus
}
#endif