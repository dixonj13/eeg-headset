#include <Windows.h>

class signal_indicator{
	int yPos;
	int xPos;
	int channelName;
	int strength;

	void setColor(int s);

public:

	signal_indicator(int y, int x, int channel);

	void updateStrength(HDC hdc);

};