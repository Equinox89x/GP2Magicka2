#pragma once

struct ResultingMagic;
class EnemyMeleeCharacter : public GameObject
{
public:
	EnemyMeleeCharacter();
	~EnemyMeleeCharacter() override = default;

	EnemyMeleeCharacter(const EnemyMeleeCharacter& other) = delete;
	EnemyMeleeCharacter(EnemyMeleeCharacter&& other) noexcept = delete;
	EnemyMeleeCharacter& operator=(const EnemyMeleeCharacter& other) = delete;
	EnemyMeleeCharacter& operator=(EnemyMeleeCharacter&& other) noexcept = delete;

	void DrawImGui();
	void DamageBeamEnter(ResultingMagic MagicResult);
	void DamageBeamExit(ResultingMagic MagicResult);
	void DamageAOE(ResultingMagic MagicResult);


protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	bool WasPressed{ false };
	float m_Health{ 500 };

	float DamageTimer, DefaultDamageTimer{ 1 };
	bool CanDamage{ false };
	float DamageToTake;
};

