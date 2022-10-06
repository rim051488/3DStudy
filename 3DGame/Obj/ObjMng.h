#pragma once
#include <memory>
#include "../common/Math.h"
#include "../Input/Controller.h"
class ObjMng
{
public:
	ObjMng();
	~ObjMng();
	virtual bool Init(void) = 0;

	virtual void Update(float delta) = 0;

	virtual void Draw(void) = 0;

protected:
};

