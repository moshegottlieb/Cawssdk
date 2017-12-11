#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __clang__
#define PIM_NULLABLE _Nullable
#define PIM_NONNULL _Nonnull
#else // GCC or whatever
#define PIM_NULLABLE
#define PIM_NONNULL
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