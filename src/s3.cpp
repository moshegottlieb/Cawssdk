#include "s3.h"
#include "aws_private.h"
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace Aws::S3;

extern "C" AWSObjectRef S3ObjectCreate(const char* region,const char* accessKeyId,const char* secretKey,AWSResult* result){
    assert(result);
    assert(region);
    Aws::Client::ClientConfiguration config;
    config.region = region;
    Aws::Auth::AWSCredentials credentials(accessKeyId, secretKey);
    S3Client* s3 = new Aws::S3::S3Client(credentials);
    return aws_check_empty(s3,*result); 
}

extern "C" S3ObjectDesc* S3ObjectDescInit(S3ObjectDesc* od,const char* bucket,const char* key, const char* filename){
    assert(od);
    assert(bucket);
    assert(key);
    assert(filename);
    memset(od,0,sizeof(S3ObjectDesc));
    aws_strcpy(od->bucket,bucket);
    aws_strcpy(od->key,key);
    aws_strcpy(od->filename,filename);
    return od;
}


extern "C" AWSErrorPolicy S3ObjectPutFile(AWSObjectRef object, const char* bucket,const char* key,const char* file,AWSResult* result){
    S3ObjectDesc desc;
    S3ObjectDescInit(&desc,bucket,key,file);
    return S3ObjectPut(object,&desc,result);
}

extern "C" AWSErrorPolicy S3ObjectPut(AWSObjectRef object, S3ObjectDesc* desc,AWSResult* result){
    assert(object);
    assert(desc);
    assert(result);
    S3Client& s3 = aws_ref<S3Client>(object);
    Model::PutObjectRequest object_request;
    object_request.WithBucket(desc->bucket).WithKey(desc->key);
    if (desc->acl[0]){
        object_request.SetACL(Model::ObjectCannedACLMapper::GetObjectCannedACLForName(desc->acl));
    }
    if (desc->storageClass[0]){
        object_request.SetStorageClass(Model::StorageClassMapper::GetStorageClassForName(desc->storageClass));
    }
    if (desc->cacheControl[0]){
        object_request.SetCacheControl(desc->cacheControl);
    }
    // Binary files must also have the std::ios_base::bin flag or'ed in
    auto input_data = Aws::MakeShared<Aws::FStream>("PutObjectInputStream",
        desc->filename, std::ios_base::in | std::ios_base::binary);
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


extern "C" void S3ObjectDescSetBucket(S3ObjectDesc* object, const char* value){
    assert(object);
    aws_strcpy(object->bucket,value);
}
extern "C" void S3ObjectDescSetKey(S3ObjectDesc* object, const char* value){
    assert(object);
    aws_strcpy(object->key,value);
}
extern "C" void S3ObjectDescSetRegion(S3ObjectDesc* object, const char* value){
    assert(object);
    aws_strcpy(object->region,value);
}
extern "C" void S3ObjectDescSetFilename(S3ObjectDesc* object, const char* value){
    assert(object);
    aws_strcpy(object->filename,value);
}
extern "C" void S3ObjectDescSetACL(S3ObjectDesc* object, const char* value){
    assert(object);
    aws_strcpy(object->acl,value);
}
extern "C" void S3ObjectDescSetStorageClass(S3ObjectDesc* object, const char* value){
    assert(object);
    aws_strcpy(object->storageClass,value);
}
extern "C" void S3ObjectDescSetCacheControl(S3ObjectDesc* object, const char* value){
    assert(object);
    aws_strcpy(object->cacheControl,value);
}

extern "C" const char* S3ObjectDescGetBucket(S3ObjectDesc* object){
    assert(object);
    return object->bucket;
}
extern "C" const char* S3ObjectDescGetKey(S3ObjectDesc* object){
    assert(object);
    return object->key;
}
extern "C" const char* S3ObjectDescGetRegion(S3ObjectDesc* object){
    assert(object);
    return object->region;
}
extern "C" const char* S3ObjectDescGetFilename(S3ObjectDesc* object){
    assert(object);
    return object->filename;
}
extern "C" const char* S3ObjectDescGetACL(S3ObjectDesc* object){
    assert(object);
    return object->acl;
}
extern "C" const char* S3ObjectDescGetStorageClass(S3ObjectDesc* object){
    assert(object);
    return object->storageClass;
}
extern "C" const char* S3ObjectDescGetCacheControl(S3ObjectDesc* object){
    assert(object);
    return object->cacheControl;
}
