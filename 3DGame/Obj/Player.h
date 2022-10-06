#pragma once
#include "ObjMng.h"
#include "../common/Light.h"
class Player :
    public ObjMng
{
public:
    Player();
    ~Player();
    bool Init(void) override;
    void Update(float delta) override;
    void Draw(void) override;
private:
    // ���f���`��
    int model_handl;
    // ��]�ƃ|�W�V����
    float angle;
    Vector3 pos_;
    // �V�F�[�_�p
    int ps, vs;
    int toonMap_;
    int tlbertType;
    // �f�B���N�V����
    DirectionLight directionLight_;
    // �萔�o�b�t�@�̊m�ۗp�ϐ�
    int cbuff;
    DirectionLight* direction_;
    //Vector3* direction_;
    //Vector3* color_;
    std::unique_ptr<Controller> controller_;
};

