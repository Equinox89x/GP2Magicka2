#include "stdafx.h"
#include "EnemyMeleeCharacter.h"
#include "../OverlordEngine/Prefabs/MagickaCamera.h"
#include <Scenes/Exam/ExamTestClass.h>

EnemyMeleeCharacter::EnemyMeleeCharacter()
{}

void EnemyMeleeCharacter::Initialize(const SceneContext& /*sceneContext*/)
{
	
}

void EnemyMeleeCharacter::Update(const SceneContext& sceneContext)
{
	if (CanDamage) {
		float deltaTime = sceneContext.pGameTime->GetElapsed();
		if (DamageTimer <= 0) {
			DamageTimer = 1;
			m_Health -= DamageToTake;
			std::cout << m_Health << "\n";
		}
		else {
			DamageTimer -= deltaTime;
		}
	}

	if (m_Health <= 0) {
		//die
	}
}


void EnemyMeleeCharacter::DrawImGui()
{
	
}

void EnemyMeleeCharacter::DamageBeamEnter(ResultingMagic MagicResult)
{
	CanDamage = true;
	DamageTimer = 0;
	DamageToTake = MagicResult.Damage;
}

void EnemyMeleeCharacter::DamageBeamExit(ResultingMagic MagicResult)
{
	CanDamage = false;
	DamageTimer = DefaultDamageTimer;
	DamageToTake = 0;
}

void EnemyMeleeCharacter::DamageAOE(ResultingMagic MagicResult)
{
	m_Health -= MagicResult.Damage;
	std::cout << m_Health << "\n";
}
