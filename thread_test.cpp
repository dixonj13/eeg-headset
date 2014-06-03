#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <thread>
#include <ctime>
#include <string>
using namespace std;

/* Program opens two threads. The first thread displays
 * the date & time at the start of the loop and runs until
 * stopped by the variable controlled by the second thread.
 *
 * Need to add mutex's to protect std in & std out...
 */

string date()
{
  time_t t = time(0);
  tm* loc_t = NULL;
  loc_t = localtime(&t);

  string s = to_string(loc_t->tm_mon) + "/" + to_string(loc_t->tm_mday) + "/" + to_string(loc_t->tm_year + 1900) +
            "_" + to_string(loc_t->tm_hour) + ":" + to_string(loc_t->tm_min) + ":" + to_string(loc_t->tm_sec);

  return s;
}

void listen(char& c)
{
  c = getchar();
}

void record(char& c)
{
  printf("\nrecording at %s... (press enter to stop)", date().c_str()); 
  while (!(c > 0)) 
  {
    //...
  }
  printf("LOOP ENDED\n\n");
}

int main(int argc, char** argv)
{
  char c = 0;
  thread t1(record, std::ref(c));
  thread t2(listen, std::ref(c));
  t1.join();
  t2.join();
  printf("goodbye");

  return 0;
}