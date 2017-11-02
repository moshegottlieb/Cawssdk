
#pragma once
#include "aws.h"
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <type_traits>


AWSObjectRef aws_check_empty(AWSObjectRef object,AWSResult& result);
AWSResult& aws_result_assign(AWSResult& result,AWSErrorPolicy policy = SUCCESS,int code = -1,const char* error = NULL, const char* message = NULL);

void aws_strcpy(char* dest,const char* src);

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(e);
}


template <typename OUTCOME>
AWSResult& aws_result_assign_outcome(AWSResult& result,const OUTCOME& outcome){
    if (outcome.IsSuccess()){
        aws_result_assign(result);
    } else {
        auto error = outcome.GetError();
        auto type = error.GetErrorType();
        aws_result_assign(result, error.ShouldRetry()?SHOULD_RETRY:FINAL,to_underlying(type),outcome.GetError().GetMessage().c_str());
    }
    return result;
}


template<typename T>
T& aws_ref(AWSObjectRef ptr){
    return *reinterpret_cast<T*>(ptr);
}
