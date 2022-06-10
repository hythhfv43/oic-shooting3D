#pragma once

#include "GameDefine.h"
#include "EnemyShot.h"

class CEnemy{
private:
	CMeshContainer*		m_pMesh;
	Vector3				m_Pos;
	Vector3				m_Rot;
	Vector3				m_TargetPos;
	bool				m_bShow;
	int					m_HP;
	int					m_ShotWait;
	int					m_ShotWaitSet;
	float				m_AnimTime;

public:
	CEnemy();
	~CEnemy();
	void Initialize();
	void Start(const Vector3& p);
	void Update(CEnemyShot* shot,int smax);
	void Render();
	void SetTargetPos(const Vector3& t) { m_TargetPos = t; }
	void RenderDebug();
	CSphere GetSphere() { return CSphere(m_Pos, 0.5f); }
	void RenderDebugText(int i);
	void SetMesh(CMeshContainer* pm){ m_pMesh = pm; }
	void Damage(int dmg);
	bool GetShow(){ return m_bShow; }
};