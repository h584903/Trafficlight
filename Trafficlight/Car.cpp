#include "framework.h"
#include "Car.h"

int nextID = 1;
int carLength = 60;

Car::Car(bool isVertical, int xCoord, int yCoord)
{
	id = nextID++;
	vertical = isVertical;
	xcoord = xCoord;
	ycoord = yCoord;
	color = RGB(
		(BYTE)(rand() % 255),
		(BYTE)(rand() % 255),
		(BYTE)(rand() % 255)
	);
}

void Car::drawCar(HDC hdc)
{
	HGDIOBJ hOrg = SelectObject(hdc, GetStockObject(DC_BRUSH));

	SetDCBrushColor(hdc, color);

	if (vertical)
	{
		Rectangle(hdc, xcoord - 15, ycoord - carLength, xcoord + 15, ycoord);
	}
	else
	{
		Rectangle(hdc, xcoord - carLength, ycoord - 15, xcoord, ycoord + 15);
	}
}

void Car::moveCar(int length)
{
	if (vertical)
	{
		ycoord += length;
	}
	else
	{
		xcoord += length;
	}
}

int Car::getX()
{
	return xcoord;
}

void Car::setX(int x)
{
	xcoord = x;
}

int Car::getY()
{
	return ycoord;
}

void Car::setY(int y)
{
	ycoord = y;
}

int Car::getCarLength()
{
	return carLength;
}