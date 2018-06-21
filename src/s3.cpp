#include "s3.h"
#include "aws_private.h"
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <stdlib.h>

using namespace Aws::S3;

extern "C" AWSObjectRef S3ObjectCreate(const char* region,const char* accessKeyId,const char* secretKey,AWSResult* result){
    assert(result);
    assert(region);
    Aws::Client::ClientConfiguration config;
    config.region = region;
    Aws::Auth::AWSCredentials credentials(accessKeyId, secretKey);
    S3Client* s3 = new Aws::S3::S3Client(credentials,config);
    return aws_check_empty(s3,*result); 
}

extern "C" S3ObjectDesc* S3ObjectDescCreate(){
    S3ObjectDesc* od = (S3ObjectDesc*)malloc(sizeof(S3ObjectDesc));
    memset(od,0,sizeof(S3ObjectDesc));
    return od;
}

#define S3ObjectDescFree(what) if (what) { free(what); what = NULL; }

extern "C" void S3ObjectDescDestroy(S3ObjectDesc* od){
    if (od){
        S3ObjectDescFree(od->acl);
        S3ObjectDescFree(od->storageClass);
        S3ObjectDescFree(od->cacheControl);
        S3ObjectDescFree(od->contentType);
        free(od);
    }
}

extern "C" AWSErrorPolicy S3ObjectPut(AWSObjectRef object,const char* filename,const char* bucket,const char* key, S3ObjectDesc* desc,AWSResult* result){
    assert(object);
    assert(desc);
    assert(result);
    assert(filename);
    assert(bucket);
    assert(key);
    S3Client& s3 = aws_ref<S3Client>(object);
    Model::PutObjectRequest object_request;
    object_request.WithBucket(bucket).WithKey(key);
    if (desc->acl && desc->acl[0]){
        object_request.SetACL(Model::ObjectCannedACLMapper::GetObjectCannedACLForName(desc->acl));
    }
    if (desc->storageClass && desc->storageClass[0]){
        object_request.SetStorageClass(Model::StorageClassMapper::GetStorageClassForName(desc->storageClass));
    }
    if (desc->cacheControl && desc->cacheControl[0]){
        object_request.SetCacheControl(desc->cacheControl);
    }
    if (desc->contentType && desc->contentType[0]){
        object_request.SetContentType(desc->contentType);
    }
    // Binary files must also have the std::ios_base::bin flag or'ed in
    auto input_data = Aws::MakeShared<Aws::FStream>("PutObjectInputStream",
        filename, std::ios_base::in | std::ios_base::binary);
    object_request.SetBody(input_data);
    auto outcome = s3.PutObject(object_request);
    return aws_result_assign_outcome(*result,outcome).policy;
}

extern "C" AWSErrorPolicy S3ObjectDelete(AWSObjectRef object,const char* bucket,const char* key,AWSResult* result){
    assert(object);
    assert(result);
    S3Client& s3 = aws_ref<S3Client>(object);
    Model::DeleteObjectRequest delete_request;
    delete_request.WithBucket(bucket).WithKey(key);
    auto outcome = s3.DeleteObject(delete_request);
    return aws_result_assign_outcome(*result,outcome).policy;
}

extern "C" void S3ObjectDestroy(AWSObjectRef object){
    S3Client& s3 = aws_ref<S3Client>(object);
    delete &s3;
}

extern "C" void S3ObjectDescSetContentType(S3ObjectDesc* object, const char* value){
    assert(object);
    S3ObjectDescFree(object->contentType);
    if (value) object->contentType = strdup(value);
}

extern "C" void S3ObjectDescSetACL(S3ObjectDesc* object, const char* value){
    assert(object);
    S3ObjectDescFree(object->acl);
    if (value) object->acl = strdup(value);
}
extern "C" void S3ObjectDescSetStorageClass(S3ObjectDesc* object, const char* value){
    assert(object);
    S3ObjectDescFree(object->storageClass);
    if (value) object->storageClass = strdup(value);
}
extern "C" void S3ObjectDescSetCacheControl(S3ObjectDesc* object, const char* value){
    assert(object);
    S3ObjectDescFree(object->cacheControl);
    if (value) object->cacheControl = strdup(value);
}
