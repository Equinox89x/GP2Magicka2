#pragma once
#include "ExamCharacter.h"
class Projectile;
class DiffuseMaterial;

class ExamEnemy : public GameObject
{
public:
	ExamEnemy(const CharacterDescExtended& characterDesc, XMFLOAT3 postitionOffset);
	~ExamEnemy() override = default;

	ExamEnemy(const ExamEnemy& other) = delete;
	ExamEnemy(ExamEnemy&& other) noexcept = delete;
	ExamEnemy& operator=(const ExamEnemy& other) = delete;
	ExamEnemy& operator=(ExamEnemy&& other) noexcept = delete;

	void DrawImGui();
	void DamageBeamEnter(float damage);
	void DamageBeamExit();
	void DamageAOE(float damage);
	void DamageAOE(float damage, bool canDamage);

	void SetCanDamageAoE(bool canDamage) { CanDamageAoE = canDamage; };
	bool GetCanDamage() { return CanDamage; };

	float GetHealth() { return m_Health; };
	void SetCanDamage(bool canDamage) { CanDamageAoE = canDamage; };
	void SetCanMove(bool canMove) { CanMove = canMove; };
	float GetAttackDamage() { return m_AttackDamage; };
	//void SetCanShoot(bool canShoot) { CanShoot = canShoot; };
	//void FireProjectile();

	bool GetMarkedForDestroy() { return m_MarkedForDestroy; };
	void SetMarkedForDestroy(bool isMarked) { m_MarkedForDestroy = isMarked; };

	ExamCharacter* m_pCharacter{ nullptr };

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	virtual void UpdateChild(const SceneContext& sceneContext) = 0;
	virtual void InitializeChild(const SceneContext& sceneContext) = 0;

	//DiffuseMaterial* m_pMaterial{};
	//PxMaterial* m_pDefaultMaterial{};

private:
	CharacterDescExtended m_CharacterDescExtended;
	//ControllerComponent* m_pControllerComponent{};
	bool WasPressed{ false };
	float m_Health{ 1500 };

	float DamageTimer, DefaultDamageTimer{ 0.5f }/*, ShootTimer, DefaultShootTimer{ 3.f }*/;
	bool CanDamage{ false }, CanMove{ true }, CanDamageAoE{ false }/*, CanShoot{ false }*/;
	float DamageToTake, m_AttackDamage{ 100 };
	bool m_MarkedForDestroy{ false };

	XMFLOAT3 PostitionOffset;

	//std::vector<Projectile*> m_Projectiles;

};

