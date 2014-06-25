#include "rawBuffer.h"
#include <cstdio>
#include "headset.h"
#include "rawBufferQueue.h"
#include "FFTs.h"
#include "timeUtil.h"
#include "channelMap.h"
#include <thread>
using namespace std;

void write_fft_buffer(int NFFT, rawBuffer& Data, FILE* F);

void fillBuffer(headset& h, rawBuffer& Data, rawQueue& Queue);

void processRawData(rawQueue& Queue, headset& h, bool& stopLoop);

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

void eegResponseTest(headset& h, bool& stop);