#include "rawBuffer.h"
#include <cstdio>
#include "headset.h"
#include "rawBufferQueue.h"
#include "FFTs.h"
#include "timeUtil.h"
#include "channelMap.h"
#include <Windows.h>
#include <thread>
using namespace std;

typedef struct recordingPackage
{
  bool stop;
  headset* head;
  HWND hwnd;
  rawQueue Queue;
  size_t timer;

  recordingPackage(bool s, headset* hs, HWND h, rawQueue rQ)
  {
    stop = s;
    head = hs;
    hwnd = h;
    Queue = rQ;
    timer = 0;
  }
} PACK, *PRPACK;

void write_fft_buffer(int NFFT, rawBuffer& Data, FILE* F);

void fillBuffer(headset& h, rawBuffer& Data, rawQueue& Queue);

DWORD WINAPI processRawData(void* pVoid);

//==================================================
//                       date
//==================================================
//Returns the current date.
//==================================================

string date();

//============================================================
//                      add_channel
//============================================================
//adds the channel with ID channelID to headset h.
//============================================================

void add_channel(headset& h, int channelID);

//============================================================
//                      remove_channel
//============================================================
//removes the channel with ID channelID from headset h.
//============================================================

void remove_channel(headset& h, int channelID);

DWORD WINAPI eegResponseTest(void* pVoid);