#pragma once
#include "ExamCharacter.h"
class Projectile;
class DiffuseMaterial;

class ExamRangedCharacter : public GameObject
{
public:
	ExamRangedCharacter(const CharacterDescExtended& characterDesc, XMFLOAT3 postitionOffset);
	~ExamRangedCharacter() override = default;

	ExamRangedCharacter(const ExamRangedCharacter& other) = delete;
	ExamRangedCharacter(ExamRangedCharacter&& other) noexcept = delete;
	ExamRangedCharacter& operator=(const ExamRangedCharacter& other) = delete;
	ExamRangedCharacter& operator=(ExamRangedCharacter&& other) noexcept = delete;

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
	void SetCanShoot(bool canShoot) { CanShoot = canShoot; };
	void FireProjectile();

	ExamCharacter* m_pCharacter{ nullptr };

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	CharacterDescExtended m_CharacterDescExtended;
	//ControllerComponent* m_pControllerComponent{};
	bool WasPressed{ false };
	float m_Health{ 1500 };

	float DamageTimer, DefaultDamageTimer{ 0.5f }, ShootTimer, DefaultShootTimer{ 3.f };
	bool CanDamage{ false }, CanMove{ true }, CanDamageAoE{ false }, CanShoot{ false };
	float DamageToTake, m_AttackDamage{ 100 };

	XMFLOAT3 PostitionOffset;

	DiffuseMaterial* m_pMaterial{};
	PxMaterial* m_pDefaultMaterial{};
	std::vector<Projectile*> m_Projectiles;

};

