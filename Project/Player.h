#pragma once

#include "GameDefine.h"
#include "PlayerShot.h"
#include "Enemy.h"
// ˆÚ“®‘¬“x
#define PLAYER_SPEED		(0.1f)

#define PLAYERSHOT_COUNT		(40)

#define PLAYERSHOT_WAIT			(5)

class CPlayer{
private:
	CMeshContainer	m_Mesh;
	CVector3		m_Pos;
	float			m_RotZ;
	CMeshContainer	m_ShotMesh;
	CPlayerShot		m_ShotArray[PLAYERSHOT_COUNT];
	int				m_ShotWait;
	bool			m_bDead;
public:
	CPlayer();
	~CPlayer();
	CSphere GetSphere() { return CSphere(m_Pos, 0.4f); }
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebugText();
	void RenderDebug();
	void CollisionEnemy(CEnemy& ene);
	void CollisionEnemyShot(CEnemyShot& shot);
	bool IsDead() { return m_bDead; }
	const CVector3 GetPosition(){ return m_Pos; }
	void Release();
};