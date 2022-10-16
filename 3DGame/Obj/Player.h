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
    Vector3 Cpos_;
    Vector3 CAngle_;
    // �V�F�[�_�p
    int ps, vs;
    int toonMap_;
    // ���f���̃^�C�v�𒲂ׂ�
    int tlbertType;
    // �f�B���N�V����
    DirectionLight directionLight_;
    // �萔�o�b�t�@�̊m�ۗp�ϐ�
    int cbuff;
    DirectionLight* direction_;
    //Vector3* direction_;
    //Vector3* color_;
    std::unique_ptr<Controller> controller_;
    // �J�����̐���
    VECTOR cPos_;
    Vector3 cTarget_;
    Vector3 cRot_;
};

