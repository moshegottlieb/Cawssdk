#include <aws/core/Version.h>
#include <iostream>

int main(int,char** argv){
    std::cout << Aws::Version::GetVersionString() << std::endl;
    return 0;
}