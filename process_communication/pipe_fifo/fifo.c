#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "sys/stat.h"

int main()
{
    int ret=mkfifo("myfifo",0664);
    if (ret < 0)
        perror("mkfifo");
    return 0;
}
