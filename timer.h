#pragma once

struct timespec t1, t2;

#define TIME(...) fprintf(stdout, __VA_ARGS__)
#define START() clock_gettime(CLOCK_MONOTONIC, &t1);
#define STOP(event) \
 do {  \
     double timeSpent = 0.0; \
     struct timespec temp; \
     clock_gettime(CLOCK_MONOTONIC, &t2); \
     if (((&t2)->tv_nsec-(&t1)->tv_nsec)<0) { \
         temp.tv_sec = (&t2)->tv_sec-(&t1)->tv_sec-1; \
         temp.tv_nsec = 1000000000+(&t2)->tv_nsec-(&t1)->tv_nsec; \
     } else { \
         temp.tv_sec = (&t2)->tv_sec-(&t1)->tv_sec; \
         temp.tv_nsec = (&t2)->tv_nsec-(&t1)->tv_nsec; \
     } \
     \
     timeSpent = (temp.tv_nsec) / 1e6; \
     timeSpent = timeSpent + temp.tv_sec*1000; \
     TIME("%s: %4.2f ms\n",event,timeSpent); \
    } while (0)

