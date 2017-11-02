#pragma once

#ifdef __cplusplus
extern "C" {
#endif



typedef void* AWSObjectRef;

void AWSInit();
void AWSDeinit();

#define AWS_STR_LEN 255

typedef enum {
    SUCCESS,
    FINAL,
    SHOULD_RETRY
} AWSErrorPolicy;

typedef struct AWSResult {  
    int code;
    AWSErrorPolicy policy;
    char error[AWS_STR_LEN];
    char message[AWS_STR_LEN];
} AWSResult;


#ifdef __cplusplus
}
#endif