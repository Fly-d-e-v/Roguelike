
#include "platform/PlatformInterface.h"

int main()
{
    printf("HelloWorld");

    PlatformInterface::Init();
     
    PlatformInterface::Deinit();
}