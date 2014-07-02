
#include "signalIndicator.h"

signal_indicator::signal_indicator(int x, int y, int channel)
{
	yPos = y;
	xPos = x;
	channelName = channel;
}

void signal_indicator:: updateStrength(HDC& hdc)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	SolidBrush brush(Color::Red);
    graphics.FillEllipse(&brush, 400, 400, 24, 24);

	/*
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	EmoStateHandle eState = EE_EmoStateCreate();
    EE_EEG_ContactQuality_t quality;
    int a = ES_GetNumContactQualityChannels(eState);
    quality = EEG_CQ_NO_SIGNAL; //ES_GetContactQuality(eState, channelName);
	

	SolidBrush brush(Color::White);
	graphics.FillRectangle(&brush, xPos - 3, yPos - 3, 30, 30);

	switch (quality)
	{
	case EEG_CQ_NO_SIGNAL:
	{
		brush.SetColor(Color::Black);
		graphics.FillEllipse(&brush, xPos, yPos, 24, 24);
		break;
	}
	case EEG_CQ_VERY_BAD:
	{
		brush.SetColor(Color::Red);
		graphics.FillEllipse(&brush, xPos, yPos, 24, 24);
		break;
	}
	case EEG_CQ_POOR:
	{
		brush.SetColor(Color::Orange);
		graphics.FillEllipse(&brush, xPos, yPos, 24, 24);
		break;
	}
	case EEG_CQ_FAIR:
	{
		brush.SetColor(Color::Yellow);
		graphics.FillEllipse(&brush, xPos, yPos, 24, 24);
		break;
	}
	case EEG_CQ_GOOD:
	{
		brush.SetColor(Color::Green);
		graphics.FillEllipse(&brush, xPos, yPos, 24, 24);
		break;
	}
	
	break;
	}
	*/
}