#include "stdafx.h"
#include "EnemyMeleeCharacter.h"
#include "../OverlordEngine/Prefabs/MagickaCamera.h"
#include <Scenes/Exam/ExamTestClass.h>

EnemyMeleeCharacter::EnemyMeleeCharacter(const CharacterDescExtended& characterDesc, XMFLOAT3 postitionOffset):
	m_CharacterDescExtended{ characterDesc },
	PostitionOffset{ postitionOffset }
{
}

void EnemyMeleeCharacter::Initialize(const SceneContext& /*sceneContext*/)
{
	//auto go{ new GameObject() };
	//AddChild(go);
	//m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDescExtended.controller));
	GetTransform()->Translate(PostitionOffset);
}

void EnemyMeleeCharacter::Update(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.pGameTime->GetElapsed();
	//constexpr float epsilon{ 0.01f };

	if (CanDamage) {
		if (DamageTimer <= 0) {
			DamageTimer = DefaultDamageTimer;
			m_Health -= DamageToTake;
			std::cout << m_Health << "\n";
		}
		else {
			DamageTimer -= deltaTime;
		}
	}

	auto originalRotation{ GetTransform()->GetWorldRotation() };
	auto originalLocation{ GetTransform()->GetWorldPosition() };

	auto lookPosition{ m_pCharacter->GetTransform()->GetWorldPosition() };
	XMVECTOR direction = XMVectorSubtract(XMLoadFloat3(&lookPosition), XMLoadFloat3(&originalLocation));
	float yaw = std::atan2(XMVectorGetX(direction), XMVectorGetZ(direction));
	XMFLOAT3 newRot = XMFLOAT3{ originalRotation.x, yaw, originalRotation.z };
	GetComponent<ModelComponent>()->GetTransform()->Rotate(newRot, false);

	if (CanMove) {
		//get forward vector
		XMFLOAT3 forward{ GetTransform()->GetForward() };
		originalLocation.x += forward.x * (deltaTime * m_CharacterDescExtended.maxMoveSpeed);
		originalLocation.z += forward.z * (deltaTime * m_CharacterDescExtended.maxMoveSpeed);

		if (!MathHelper::IsPointInCircle3D(originalLocation, lookPosition, 25)) {
			GetTransform()->Translate(originalLocation);
		}
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
	if(CanDamageAoE) m_Health -= MagicResult.Damage;
	CanDamageAoE = false;
	std::cout << m_Health << "\n";
}
