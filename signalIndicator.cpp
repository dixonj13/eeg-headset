#include "signalIndicator.h"
#include "edk.h"
#include <gdiplus.h>
using namespace Gdiplus;

signal_indicator::signal_indicator(int y, int x, int channel)
{
	yPos = y;
	xPos = x;
	channelName = channel;
	strength = 0;
}

void signal_indicator:: updateStrength(HDC hdc){
	EmoStateHandle eState = EE_EmoStateCreate();
	strength = EE_GetContactQuality(eState);
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	SolidBrush brush(Color::White);
	graphics.FillRectangle(&brush, xPos - 3, yPos - 3, 30, 30);

	switch (strength)
	{
	case 0:
	{
		brush.SetColor(Color::Black);
		graphics.FillEllipse(&brush, xPos, yPos, 24, 24);
		break;
	}
	case 1:
	{
		brush.SetColor(Color::Red);
		graphics.FillEllipse(&brush, xPos, yPos, 24, 24);
		break;
	}
	case 2:
	{
		brush.SetColor(Color::Orange);
		graphics.FillEllipse(&brush, xPos, yPos, 24, 24);
		break;
	}
	case 3:
	{
		brush.SetColor(Color::Yellow);
		graphics.FillEllipse(&brush, xPos, yPos, 24, 24);
		break;
	}
	case 4:
	{
		brush.SetColor(Color::Green);
		graphics.FillEllipse(&brush, xPos, yPos, 24, 24);
		break;
	}
	
	break;
	}
}