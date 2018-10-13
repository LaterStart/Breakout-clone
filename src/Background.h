#pragma once
#include "ScreenObject.h"

struct Size;
class Background : public ScreenObject {
public:
	Background(Size windowSize);
	
public:
	void _Draw() const override;

private:
	Size windowSize;
};
