#pragma once
#include <vector>
#include <string>
#include <map>
#include "Math.h"

#define lpImageMng ImageMng::GetInstance()
using VecInt = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstance()
	{
		static ImageMng s_Instance;
		return s_Instance;
	}

	const VecInt& GetID(std::string key);															// 画像の表示
	const VecInt& GetID(std::string f_name, std::string key);										// 画像の表示
	const VecInt& GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt);		// 表示する画像のサイズを指定して表示
	const VecInt& GetID(std::string f_name, std::string key, Vector2I divSize, Vector2I divCnt);	// 表示する画像のサイズを指定して表示

	bool Relese(const std::string& key);
private:
	ImageMng();
	~ImageMng();
	std::map<std::string, VecInt> imageMap_;
};

