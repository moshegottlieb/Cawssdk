#include "ses.h"
#include "aws.h"
#include "aws_private.h"
#include <aws/core/Aws.h>
#include <aws/email/SESClient.h>
#include <aws/email/model/SendEmailRequest.h>
#include <aws/email/model/SendEmailResult.h>
#include <cassert>

using namespace Aws::SES;

extern "C" AWSObjectRef SESCreate(const char* region,const char* accessKeyId,const char* secretKey,AWSResult* result){
    assert(region);
    assert(result);
    Aws::Auth::AWSCredentials credentials(accessKeyId, secretKey);
    Aws::Client::ClientConfiguration config;
    config.region = region;
    SESClient* ses = new Aws::SES::SESClient(credentials,config);
    return aws_check_empty(ses,*result);
}

extern "C" void SESDestroy(AWSObjectRef object){
    SESClient& ses = aws_ref<SESClient>(object);
    delete &ses;
}
extern "C" AWSErrorPolicy SESSendEmail(AWSObjectRef object, const char* from, const char** to,int toCount,const char** cc,int ccCount,const char** bcc,int bccCount,const char* subject,const char* body,int isHtml,AWSResult* result){
    SESClient& ses = aws_ref<SESClient>(object);
    assert(result);
    Model::Message message;
    Model::Content message_subject;
    message_subject.SetCharset("UTF-8");
    message_subject.SetData(subject);
    message.SetSubject(message_subject);
    Model::Body message_body;
    Model::Content body_content;
    body_content.SetCharset("UTF-8");
    body_content.SetData(body);
    if (isHtml){
        message_body.SetHtml(body_content);
    } else {
        message_body.SetText(body_content);
    }
    message.SetBody(message_body);
    Model::SendEmailRequest request;
    request.SetMessage(message);
    request.SetSource(from);
    Model::Destination destination;

    typedef struct {
        const char** addr;
        int count;
        std::function<void(const char*)> add;
    } addr;

    addr a[] = {
        { to, toCount , [&destination](const char* addr){
            destination.AddToAddresses(addr);
        }},
        { cc, ccCount , [&destination](const char* addr){
            destination.AddCcAddresses(addr);
        }},
        { bcc, bccCount,  [&destination](const char* addr){
            destination.AddBccAddresses(addr);
        }}
    };

    for (auto address:a){
        for (int i=0;i<address.count;++i){
            address.add(address.addr[i]);
        }
    }

    auto outcome = ses.SendEmail(request);
    return aws_result_assign_outcome(*result,outcome).policy;
}