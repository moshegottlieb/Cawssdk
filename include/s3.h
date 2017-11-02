#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "aws.h"

AWSObjectRef S3ObjectCreate(const char* region,const char* accessKeyId,const char* secretKey,AWSResult* result);
void S3ObjectDestroy(AWSObjectRef object);

typedef struct S3ObjectDesc {
    char bucket[AWS_STR_LEN];
    char key[AWS_STR_LEN];
    char region[AWS_STR_LEN];
    char filename[AWS_STR_LEN];
    char acl[AWS_STR_LEN];
    char storageClass[AWS_STR_LEN];
    char cacheControl[AWS_STR_LEN];
} S3ObjectDesc;

S3ObjectDesc* S3ObjectDescInit(S3ObjectDesc* od,const char* bucket,const char* key, const char* filename);
AWSErrorPolicy S3ObjectPut(AWSObjectRef object, S3ObjectDesc*,AWSResult* result);
AWSErrorPolicy S3ObjectPutFile(AWSObjectRef object, const char* bucket,const char* key,const char* file,AWSResult* result);

#ifdef __cplusplus
}
#endif