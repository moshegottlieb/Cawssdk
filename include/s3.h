#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "aws.h"

AWSObjectRef S3ObjectCreate(const char* region,const char* accessKeyId,const char* secretKey,AWSResult* result);
void S3ObjectDestroy(AWSObjectRef object);

typedef struct S3ObjectDesc {
    char* acl;
    char* storageClass;
    char* cacheControl;
    char* contentType;
} S3ObjectDesc;

S3ObjectDesc* S3ObjectDescCreate();
void S3ObjectDescDestroy(S3ObjectDesc* od);

void S3ObjectDescSetACL(S3ObjectDesc* object, const char* value);
void S3ObjectDescSetStorageClass(S3ObjectDesc* object, const char* value);
void S3ObjectDescSetCacheControl(S3ObjectDesc* object, const char* value);
void S3ObjectDescSetContentType(S3ObjectDesc* object, const char* value);

AWSErrorPolicy S3ObjectPut(AWSObjectRef object,const char* filename,const char* bucket,const char* key, S3ObjectDesc* desc,AWSResult* result);

#ifdef __cplusplus
}
#endif