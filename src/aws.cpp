#include "aws.h"
#include "aws_private.h"
#include <cstring>

static Aws::SDKOptions options;

extern "C" void AWSInit(){
    Aws::InitAPI(options);
}

extern "C" void AWSDeinit(){
    Aws::ShutdownAPI(options);
}

void aws_strcpy(char* dest,const char* src){
    if (src){
        strncpy(dest,src,AWS_STR_LEN);
        dest[AWS_STR_LEN-1] = '\0';
    } else {
        dest[0] = '\0';
    }
}


AWSObjectRef aws_check_empty(AWSObjectRef object,AWSResult& result){
    memset(&result,0,sizeof(AWSResult));
    if (object){
        aws_result_assign(result);
    } else {
        aws_result_assign(result,FINAL,-1,"Out of memory");
    }
    return object;
}


AWSResult& aws_result_assign(AWSResult& result,AWSErrorPolicy policy,int code,const char* error, const char* message){
    memset(&result,0,sizeof(AWSResult));
    result.policy = policy;
    result.code = code;
    aws_strcpy(result.error,error);
    aws_strcpy(result.message,message);
    return result;
}