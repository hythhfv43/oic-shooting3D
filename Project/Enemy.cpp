#include "Enemy.h"

//�G�̈ړ�
ANIM_DATA g_EnemyAnimPosY[2] = {
	{ 1.0f,-10.0f,EASE_LINEAR },
	{ 3.0f,0.0f,EASE_INOUT_SINE },
};
ANIM_DATA g_EnemyAnimPosZ[5] = {
	{ 0.0f,-FIELD_HALF_Z,EASE_LINEAR },
	{ 1.0f,FIELD_HALF_Z - 10.0f,EASE_LINEAR },
	{ 2.0f,FIELD_HALF_Z,EASE_OUT_SINE },
	{ 3.0f,FIELD_HALF_Z - 10.0f,EASE_IN_SINE },
	{ 5.0f,-FIELD_HALF_Z,EASE_LINEAR },
};

/**
 * �R���X�g���N�^
 *
 */
CEnemy::CEnemy():
m_pMesh(NULL),
m_Pos(0,0,0),
m_Rot(0,0,0),
m_bShow(false),
m_HP(5),
m_ShotWait(0),
m_ShotWaitSet(0),
m_TargetPos(0,0,0),
m_AnimTime(0){
}

/**
 * �f�X�g���N�^
 *
 */
CEnemy::~CEnemy(){
}

/**
 * ������
 *
 */
void CEnemy::Initialize(){
	m_Pos = Vector3(0, 0, 0);
	m_Rot = Vector3(0, 0, 0);
	m_bShow = false;
	m_AnimTime = 0;
}

/**
 * �J�n
 *
 */
void CEnemy::Start(const Vector3& p){
	m_Pos = p;
	m_Rot = Vector3(0, 0, 0);
	m_bShow = true;
	m_HP = 5;
	m_ShotWait = 0;
	m_ShotWaitSet = 40;
	m_TargetPos = Vector3(0, 0, 0);
	m_AnimTime = 0;
}

/**
 * �X�V
 *
 */
void CEnemy::Update(CEnemyShot* shot,int smax){
	//��\��
	if (!GetShow()) 
	{
		return;
	}
	//���Ԃ�i�߂�
	m_AnimTime += CUtilities::GetFrameSecond();
	m_Pos.y = InterpolationAnim(m_AnimTime, g_EnemyAnimPosY, 2);
	m_Pos.z = InterpolationAnim(m_AnimTime, g_EnemyAnimPosZ, 5);

	if (g_EnemyAnimPosY[1].Time < m_AnimTime) 
	{
		//�e�̔���
		if (m_ShotWait <= 0) 
		{
			CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
			if (newShot) 
			{
				m_ShotWait = m_ShotWaitSet;

				Vector3 direction = m_TargetPos - m_Pos;

				float distance = CVector3Utilities::Length(direction);

				if (direction > 0) 
				{
					direction /= distance;
					newShot->Fire(m_Pos, direction * 0.075f);
				}
			}
		}
		else
		{
			m_ShotWait--;
		}
	}
	//�A�j���[�V�����I���ŏ���
	if (g_EnemyAnimPosZ[4].Time < m_AnimTime) 
	{
		m_bShow = false;
	}
}

/**
 * �`��
 *
 */
void CEnemy::Render(){
	if(!GetShow())
	{
		return;
	}
	
	CMatrix44 matWorld;
	matWorld.RotationZXY(m_Rot);
	matWorld.SetTranslation(m_Pos);
	m_pMesh->Render(matWorld);
}

/**
*
* �f�o�b�N�`��
*/
void CEnemy::RenderDebug() {
	//��\��
	if (!GetShow()) 
	{
		return;
	}
	//�����蔻��̕\��
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(1, 0, 0, 0.3f));
}

/**
 * �f�o�b�O�����`��
 *
 */
void CEnemy::RenderDebugText(int i){
	//�ʒu�̕`��
	CGraphicsUtilities::RenderString(10,70 + i * 24,MOF_XRGB(0,0,0),
		"�G[%d] %s , �ʒu X : %.1f , Y : %.1f , Z : %.1f",i + 1,
		(GetShow() ? "�\��" : "��\��"),m_Pos.x,m_Pos.y,m_Pos.z);
}

/**
* �_���[�W����
* ��������HP�����炵HP���O�ȉ��ɂȂ�ΓG����������
*
* ����
* [in]			dmg			�_���[�W
*/
void CEnemy::Damage(int dmg) {
	m_HP -= dmg;
	if (m_HP <= 0) 
	{
		m_bShow = false;
	}
}