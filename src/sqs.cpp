#include "sqs.h"
#include "aws.h"
#include "aws_private.h"
#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/SendMessageRequest.h>
#include <aws/sqs/model/SendMessageResult.h>
#include <aws/sqs/model/DeleteMessageRequest.h>
#include <aws/sqs/model/ReceiveMessageRequest.h>
#include <iostream>
#include <cstring>

using namespace Aws::SQS;

extern "C" AWSObjectRef SQSCreate(const char* region,const char* accessKeyId,const char* secretKey,AWSResult* result){
    assert(region);
    assert(result);
    Aws::Auth::AWSCredentials credentials(accessKeyId, secretKey);
    Aws::Client::ClientConfiguration config;
    config.region = region;
    SQSClient* sqs = new Aws::SQS::SQSClient(credentials,config);

    return aws_check_empty(sqs,*result);
}

extern "C" AWSErrorPolicy SQSSendMessage(AWSObjectRef object, const char* queueUrl, const char* message,AWSResult* result){
    SQSClient& sqs = aws_ref<SQSClient>(object);
    Model::SendMessageRequest sm_req;
    sm_req.SetQueueUrl(queueUrl);
    sm_req.SetMessageBody(message);
    auto outcome = sqs.SendMessage(sm_req);
    return aws_result_assign_outcome(*result,outcome).policy;
}

extern "C" AWSErrorPolicy SQSDeleteMessage(AWSObjectRef object,const char* queueUrl, const char* handle,AWSResult* result){
    SQSClient& sqs = aws_ref<SQSClient>(object);
    Model::DeleteMessageRequest request;
    request.SetQueueUrl(queueUrl);
    request.SetReceiptHandle(handle); 
    auto outcome = sqs.DeleteMessage(request);
    return aws_result_assign_outcome(*result,outcome).policy;
}

extern "C" AWSErrorPolicy SQSReceiveMessages(AWSObjectRef object, const char* queueUrl,int maxBatch,int timeout,void* context,SQSMessageReceivedCallback handler,AWSResult* result){
    assert(result);
    SQSClient& sqs = aws_ref<SQSClient>(object);
    Model::ReceiveMessageRequest request;
    request.SetQueueUrl(queueUrl);
    request.SetMaxNumberOfMessages(maxBatch);
    request.SetWaitTimeSeconds(timeout);
    auto should_continue = true;
    do {
        auto outcome = sqs.ReceiveMessage(request);
        if (aws_result_assign_outcome(*result,outcome).policy == SUCCESS){
            for (auto message:outcome.GetResult().GetMessages()){
                auto id = message.GetMessageId();
                auto body = message.GetBody();
                auto receipt = message.GetReceiptHandle();
                if (!(should_continue = handler(context,id.c_str(),body.c_str(),receipt.c_str()))){
                    break;
                }                
            }

        } else {
            should_continue = false;
        }
    } while (should_continue);
    return result->policy;
}


extern "C" void SQSDestroy(AWSObjectRef object){
    SQSClient& sqs = aws_ref<SQSClient>(object);
    delete &sqs;
}
