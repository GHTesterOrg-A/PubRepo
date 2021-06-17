#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#if 0
int main ()
{
   time_t rawtime;
   struct tm *info;
   char buffer[80];
 
   rawtime = (time_t)1620784486;
   
   info = gmtime( &rawtime );
 
   strftime(buffer, 80, "%Y-%m-%dT%H:%M:%SZ", info);

   printf("格式化的日期 & 时间 : |%s|\n", buffer );


   //strftime(buffer, 80, "%Y-%m-%d%T%H:%M:%S%Z", info);
   

   strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %Z", info);
   printf("格式化的日期 & 时间 : |%s|\n", buffer );
 
   char * aaa = "2021-06-01T12:20:15Z";
   















  
   
   //long mod_ts = _mkgmtime64(&ttime
   //time_t test = mod_ts;
   //struct tm *t2 = localtime(&test);
   //char *mod_ts2  = asctime(&ttime);
  // printf("mod_ts=%ld mod_ts2=%s mod_ts3=%ld\n",mod_ts, mod_ts2,mod_ts3);
   //printf("mod_ts3=%ld\n",mod_ts3);

   time_t timenow = time(NULL);
   struct tm *a1 = gmtime(&timenow);
   struct tm *a2 =  localtime(&timenow);
   printf("a1 = %d a2 = %d\n",a1->tm_hour, a2->tm_hour);
  
   struct tm ttime = {0};
                strptime(aaa, "%Y-%m-%dT%H:%M:%SZ", &ttime);
                time_t mod_ts = mktime(&ttime);
  printf("mod_ts=%ld\n",mod_ts);

  struct timeval tv_begin;
  struct timezone tz;
  gettimeofday(&tv_begin, &tz);
  printf("%ld %d %d",tv_begin.tv_sec, tz.tz_minuteswest, tz.tz_dsttime);
   return(0);
}
#endif
#include <stdio.h>
#include <time.h>
 
int main ()
{
   time_t rawtime=1623144298;
   struct tm *info;
   char buffer[80];
   
   //time( &rawtime );
 
   info = localtime( &rawtime );
 
   strftime(buffer, 80, "%Y-%m-%dT%H:%M:%S%z", info);
   printf("格式化的日期 & 时间 : |%s|\n", buffer );
  
   return(0);
}
