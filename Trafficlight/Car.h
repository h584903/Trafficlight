#pragma once

class Car
{
private:
	int id;
	bool vertical;
	int xcoord;
	int ycoord;
	COLORREF color;

public:
	Car(bool isVertical, int xCoord, int yCoord);
	void drawCar(HDC hdc);
	void moveCar(int length);
	int getX();
	void setX(int x);
	int getY();
	void setY(int y);
	static int getCarLength();
};