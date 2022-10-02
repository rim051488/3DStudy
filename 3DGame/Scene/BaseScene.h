#pragma once
#include <memory>
#include "../common/Math.h"
#include"../Input/Controller.h"

class BaseScene;
using uniqueScene = std::unique_ptr<BaseScene>;

enum class Scene
{
	Game,
	Max
};

class BaseScene
{
public:
	// �R���X�g���N�^
	BaseScene();
	virtual ~BaseScene();

	//������
	virtual bool Init(void) = 0;

	/// <summary> �X�V </summary>
	/// <param name="delta">�f���^�^�C��</param>
	/// <param name="ownScene">���݂̃V�[��</param>
	/// <returns> ���݂̃V�[�� </returns>
	virtual uniqueScene Update(float delta, uniqueScene ownScene) = 0;

	/// <summary> �`�� </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	virtual void Draw(float delta);

	/// <summary> ���݂̃V�[���̕`�� </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	virtual void DrawOwnScreen(float delta) = 0;

	/// <summary> ���݂̃V�[���̎擾 </summary>
	/// <returns> ���݂̃V�[�� </returns>
	virtual Scene GetSceneID(void) = 0;

	/// <summary> �V�[���J�ڃt���O�擾 </summary>
	/// <returns> �V�[���J��:true </returns>
	virtual bool GetFlag(void) = 0;

protected:
	// �V�[���̑J�ڃt���O
	bool SceneFlag_;

	// ��ʃT�C�Y
	Vector2I screenSize_;

	// �V�[���f�[�^�̊i�[
	int screenID_;

	// �R���g���[�����
	std::unique_ptr<Controller> controller_;

};

