#pragma once
#include "ExamCharacter.h"

struct ResultingMagic;
class EnemyMeleeCharacter : public GameObject
{
public:
	EnemyMeleeCharacter(const CharacterDescExtended& characterDesc);
	~EnemyMeleeCharacter() override = default;

	EnemyMeleeCharacter(const EnemyMeleeCharacter& other) = delete;
	EnemyMeleeCharacter(EnemyMeleeCharacter&& other) noexcept = delete;
	EnemyMeleeCharacter& operator=(const EnemyMeleeCharacter& other) = delete;
	EnemyMeleeCharacter& operator=(EnemyMeleeCharacter&& other) noexcept = delete;

	void DrawImGui();
	void DamageBeamEnter(ResultingMagic MagicResult);
	void DamageBeamExit(ResultingMagic MagicResult);
	void DamageAOE(ResultingMagic MagicResult);

	float GetHealth() { return m_Health; };
	void SetCanDamage(bool canDamage) { CanDamageAoE = canDamage; };

	ExamCharacter* m_pCharacter{ nullptr };

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	CharacterDescExtended m_CharacterDescExtended;
	//ControllerComponent* m_pControllerComponent{};
	bool WasPressed{ false };
	float m_Health{ 1500 };

	float DamageTimer, DefaultDamageTimer{ 0.5f };
	bool CanDamage{ false };
	bool CanDamageAoE{ false };
	float DamageToTake;
};

