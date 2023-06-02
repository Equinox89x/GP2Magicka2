#pragma once
#include "ExamCharacter.h"
#include "ExamEnemy.h"

struct ResultingMagic;
class EnemyMeleeCharacter : public ExamEnemy
{
public:
	EnemyMeleeCharacter(const CharacterDescExtended& characterDesc, XMFLOAT3 postitionOffset);
	~EnemyMeleeCharacter() override = default;

	EnemyMeleeCharacter(const EnemyMeleeCharacter& other) = delete;
	EnemyMeleeCharacter(EnemyMeleeCharacter&& other) noexcept = delete;
	EnemyMeleeCharacter& operator=(const EnemyMeleeCharacter& other) = delete;
	EnemyMeleeCharacter& operator=(EnemyMeleeCharacter&& other) noexcept = delete;

	/*void DrawImGui();
	void DamageBeamEnter(float damage);
	void DamageBeamExit();
	void DamageAOE(float damage);
	void DamageAOE(float damage, bool canDamage);*/

	/*float GetHealth() { return m_Health; };
	void SetCanDamageAoE(bool canDamage) { CanDamageAoE = canDamage; };
	bool GetCanDamage() { return CanDamage; };
	void SetCanMove(bool canMove) { CanMove = canMove; };
	float GetAttackDamage() { return m_AttackDamage; };*/

	//ExamCharacter* m_pCharacter{ nullptr };

protected:
	void InitializeChild(const SceneContext&) override;
	void UpdateChild(const SceneContext&) override;

private:
	//CharacterDescExtended m_CharacterDescExtended;
	//ControllerComponent* m_pControllerComponent{};
	//bool WasPressed{ false };
	//float m_Health{ 1500 };

	//float DamageTimer, DefaultDamageTimer{ 0.5f };
	//bool CanDamage{ false }, CanMove{ true }, CanDamageAoE{ false };
	//float DamageToTake, m_AttackDamage{ 100 };

	//XMFLOAT3 PostitionOffset;
};

