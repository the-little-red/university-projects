// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
//
// int main(void){
//
//   char hostname[1024];
//   gethostname(hostname,1024);
//
//   puts(hostname);
//
// }

#include <time.h>
#include <stdio.h>

int main(void)
{
    time_t mytime;
    mytime = time(NULL);
    printf(ctime(&mytime));

    return 0;
}
