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

protected:
	void InitializeChild(const SceneContext&) override;
	void UpdateChild(const SceneContext&) override;

private:
};

