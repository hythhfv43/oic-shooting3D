/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"Player.h"
#include	"Stage.h"
#include	"Stage1.h"


//�J����
CCamera		gCamera;
//���C�g
CDirectionalLight	gLight;
//�v���C���[
CPlayer		gPlayer;
//�G
#define	ENEMY_COUNT		(20)
CEnemy		gEnemyArray[ENEMY_COUNT];
//�G�e
#define		ENEMYSHOT_COUNT		(200)
CEnemyShot	gShotArray[ENEMYSHOT_COUNT];
CMeshContainer	gEnemyShotMesh;
//�f�o�b�N�\���t���O
bool		gbDebug = false;
//�X�e�[�W
CStage		gStage;

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//���\�[�X�z�u�f�B���N�g���̐ݒ�
	CUtilities::SetCurrentDirectory("Resource");
	
	//�J����������
	gCamera.SetViewPort();
	gCamera.LookAt(Vector3(0, 6.0f, - 17.0f), Vector3(0, 0, -10), Vector3(0, 1, 0));
	gCamera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	CGraphicsUtilities::SetCamera(&gCamera);

	//���C�g������
	gLight.SetDirection(Vector3(-1, -2, 1.5f));
	gLight.SetDiffuse(MOF_COLOR_WHITE);
	gLight.SetAmbient(MOF_COLOR_HWHITE);
	gLight.SetSpeculer(MOF_COLOR_WHITE);
	CGraphicsUtilities::SetDirectionalLight(&gLight);

	//�v���C���[�̑f�ޓǂݍ���
	gPlayer.Load();
	//�X�e�[�W�̑f�ޓǂݍ���
	gStage.Load();
	//�G�e�̑f�ޓǂݍ���
	if (!gEnemyShotMesh.Load("eshot.mom")) 
	{
		return false;
	}
	//�v���C���[�̏�ԏ�����
	gPlayer.Initialize();
	//�X�e�[�W��ԏ�����
	gStage.Initialize(&gStg1EnemyStart);
	//�G�̏�����
	for (int i = 0; i < ENEMY_COUNT; i++) 
	{
		gEnemyArray[i].Initialize();
	}
	//�G�e�̏�����
	for (int i = 0; i < ENEMYSHOT_COUNT; i++) 
	{
		gShotArray[i].Initialize();
		gShotArray[i].SetMesh(&gEnemyShotMesh);
	}
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̍X�V
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//�L�[�̍X�V
	g_pInput->RefreshKey();
	//�X�e�[�W�̍X�V
	gStage.Update(gEnemyArray,ENEMY_COUNT);
	//�v���C���[�̍X�V
	gPlayer.Update();
	//�G�̍X�V
	for (int i = 0; i < ENEMY_COUNT; i++) 
	{
		gEnemyArray[i].SetTargetPos(gPlayer.GetPosition());
		gEnemyArray[i].Update(gShotArray,ENEMYSHOT_COUNT);
	}
	//�G�e�̍X�V
	for (int i = 0; i < ENEMYSHOT_COUNT; i++) 
	{
		gShotArray[i].Update();
	}
	//�G�Ƃ̓����蔻��
	for (int i = 0; i < ENEMY_COUNT; i++) 
	{
		gPlayer.CollisionEnemy(gEnemyArray[i]);
	}
	//�G�e�Ƃ̓����蔻��
	for (int i = 0; i < ENEMYSHOT_COUNT; i++) 
	{
		gPlayer.CollisionEnemyShot(gShotArray[i]);
	}
	//�f�o�b�N�\���ؑ�
	if (g_pInput->IsKeyPush(MOFKEY_F1)) 
	{
		gbDebug = ((gbDebug) ? false : true);
	}
	//�Q�[���I�[�o�[�\�����Enter�ŏ��������s��
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && gPlayer.IsDead()) 
	{
		//�Q�[�����̃I�u�W�F�N�g�����ׂď���������
		gPlayer.Initialize();
		gStage.Initialize(&gStg1EnemyStart);
		for (int i = 0; i < ENEMY_COUNT; i++) 
		{
			gEnemyArray[i].Initialize();
		}
		for (int i = 0; i < ENEMYSHOT_COUNT; i++) 
		{
			gShotArray[i].Initialize();
		}
	}
	//�v���C���[�̓����ɍ��킹�ăJ�����𓮂���
	float posX = gPlayer.GetPosition().x * 0.4f;
	CVector3 cpos = gCamera.GetViewPosition();
	CVector3 tpos = gCamera.GetTargetPosition();
	CVector3 vup = CVector3(0, 1, 0);
	cpos.x = posX;
	tpos.x = posX;
	vup.RotationZ(gPlayer.GetPosition().x / FIELD_HALF_X * MOF_ToRadian(10.0f));
	gCamera.LookAt(cpos, tpos, vup);
	gCamera.Update();
	return TRUE;
}

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̕`��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//�`�揈��
	g_pGraphics->RenderStart();
	// ��ʂ̃N���A
	g_pGraphics->ClearTarget(0.65f,0.65f,0.67f,0.0f,1.0f,0);

	//�[�x�o�b�t�@�L����
	g_pGraphics->SetDepthEnable(TRUE);

	//�X�e�[�W�`��
	gStage.Render();

	//�v���C���[�`��
	gPlayer.Render();
	//�G�̕`��
	for (int i = 0; i < ENEMY_COUNT; i++) 
	{
		gEnemyArray[i].Render();
	}
	//�G�e�̕`��
	for (int i = 0; i < ENEMYSHOT_COUNT; i++) 
	{
		gShotArray[i].RenderDebug();
	}
	//�RD�f�o�b�O�`��
	if (gbDebug) 
	{
		//�v���C���[�̃f�o�b�N�`��
		gPlayer.RenderDebug();
		//�G�̃f�o�b�O�`��
		for (int i = 0; i < ENEMY_COUNT; i++) 
		{
			gEnemyArray[i].RenderDebug();
		}
		//�ړ��\�͈͂̕\��
		CMatrix44 matWorld;
		matWorld.Scaling(FIELD_HALF_X * 2, 1, FIELD_HALF_Z * 2);
		CGraphicsUtilities::RenderPlane(matWorld, Vector4(1,1,1, 0.4f));
	}

	//�[�x�o�b�t�@������
	g_pGraphics->SetDepthEnable(FALSE);

	//2D�f�o�b�O�`��
	if (gbDebug) 
	{
		//�v���C���[�̃f�o�b�O�����`��
		gPlayer.RenderDebugText();
		//�X�e�[�W�̃f�o�b�O�����`��
		gStage.RenderDebugText();
		//�G�̃f�o�b�O�����`��
		for (int i = 0; i < ENEMY_COUNT; i++) 
		{
			gEnemyArray[i].RenderDebugText(i);
		}
	}

	//�Q�[���I�[�o�[�\��
	if (gPlayer.IsDead()) 
	{
		CGraphicsUtilities::RenderString(240, 350, MOF_COLOR_RED,
			"�Q�[���I�[�o�[		:	Enter�L�[�ł�����x�ŏ�����");
	}

	// �`��̏I��
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void){
	gPlayer.Release();
	gStage.Release();
	gEnemyShotMesh.Release();
	return TRUE;
}