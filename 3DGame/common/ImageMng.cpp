#include <DxLib.h>
#include "ImageMng.h"
#include "Debug.h"

const VecInt& ImageMng::GetID(std::string key)
{
    // ‰æ‘œ‚ª“Ç‚İ‚ß‚È‚©‚Á‚½‚ç
    if (imageMap_.count(key) == 0)
    {
        DebugLog("‰æ‘œ‚Ì“Ç‚İ‚İ");
        imageMap_[key].resize(1);
        imageMap_[key][0] = LoadGraph(key.c_str());
    }
    return imageMap_[key];
}

const VecInt& ImageMng::GetID(std::string f_name, std::string key)
{
    if (imageMap_.count(key) == 0)
    {
        DebugLog("‰æ‘œ‚Ì“Ç‚İ‚İ");
        imageMap_[key].resize(1);
        imageMap_[key][0] = LoadGraph(f_name.c_str());
    }
    return imageMap_[key];
}

const VecInt& ImageMng::GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt)
{
    if (imageMap_.find(key) == imageMap_.end())
    {
        imageMap_[key].resize(static_cast<size_t>(divCnt.x * divCnt.y));

        if (LoadDivGraph(f_name.c_str(), static_cast<int>(divCnt.x * divCnt.y), static_cast<int>(divCnt.x), static_cast<int>(divCnt.y), static_cast<int>(divSize.x), static_cast<int>(divSize.y), &imageMap_[key][0], true) == -1)
        {
            DebugLog("‰æ‘œ‚Ì“Ç‚İ‚İ¸”s");
        }
    }
    return imageMap_[key];
}

const VecInt& ImageMng::GetID(std::string f_name, std::string key, Vector2I divSize, Vector2I divCnt)
{
    if (imageMap_.count(key) == 0)
    {
        imageMap_[key].resize(divCnt.x * divCnt.y);
        if (LoadDivGraph(f_name.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, divSize.x, divSize.y, &imageMap_[key][0], true) == -1)
        {
            DebugLog("‰æ‘œ‚Ì“Ç‚İ‚İ¸”s");
        }
    }
    return imageMap_[key];
}

bool ImageMng::Relese(const std::string& key)
{
    if (!imageMap_.contains(key))
    {
        return false;
    }
    
    for (auto& img : imageMap_[key])
    {
        DeleteGraph(img);
    }
    imageMap_.erase(key);
    return true;
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
    InitGraph();
}
