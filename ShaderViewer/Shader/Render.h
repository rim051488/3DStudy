#pragma once
class Render
{
public:
	Render();
	~Render();
	void SetupDepthImage();
	void DrawModelWithDepthShadow();
};

