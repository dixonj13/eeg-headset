#include <Windows.h>
#include "edk.h"
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class signal_indicator{
	int yPos;
	int xPos;
	int channelName;

public:

	signal_indicator(int x, int y, int channel);

	void updateStrength(HDC& hdc);

};