#pragma once
#include "ScreenObject.h"

class Text : public ScreenObject {
public:
	Text(Size size, XY pos);
	const char* ID{};

public:
	void _Draw() const override;

private:
	Size size;
	XY pos;
};
