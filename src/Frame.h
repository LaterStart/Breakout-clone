#pragma once
#include "ScreenObject.h"

class Frame : public ScreenObject {
public:
	Frame(Size size, XY pos, RGB color);
	Frame(Size size, XY pos);

public:
	void _Draw() const override;

private:
	Size size;
	XY pos;
	RGB color;
};
