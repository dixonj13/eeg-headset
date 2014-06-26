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

  recordingPackage(bool s, headset* hs, HWND h)
  {
    stop = s;
    head = hs;
    hwnd = h;
  }
} PACK, *PRPACK;

void write_fft_buffer(int NFFT, rawBuffer& Data, FILE* F);

void fillBuffer(headset& h, rawBuffer& Data, rawQueue& Queue);

void processRawData(rawQueue& Queue, headset& h, bool& stopLoop, HWND hwnd);

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