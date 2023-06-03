#include "stdafx.h"
#include "EnemyMeleeCharacter.h"
#include "../OverlordEngine/Prefabs/MagickaCamera.h"
#include <Scenes/Exam/ExamTestClass.h>

EnemyMeleeCharacter::EnemyMeleeCharacter(const CharacterDescExtended& characterDesc, XMFLOAT3 postitionOffset):
	ExamEnemy(characterDesc, postitionOffset)
{
}

void EnemyMeleeCharacter::InitializeChild(const SceneContext& /*sceneContext*/)
{

}

void EnemyMeleeCharacter::UpdateChild(const SceneContext& /*sceneContext*/)
{

}
