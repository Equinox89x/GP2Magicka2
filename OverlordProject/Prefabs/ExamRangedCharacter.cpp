#include "stdafx.h"
#include "ExamRangedCharacter.h"
#include "../OverlordEngine/Prefabs/MagickaCamera.h"
#include <Scenes/Exam/ExamTestClass.h>
#include "ExamCharacter.h"

ExamRangedCharacter::ExamRangedCharacter(const CharacterDescExtended& characterDesc, XMFLOAT3 postitionOffset) :
	ExamEnemy(characterDesc, postitionOffset)
{
}

void ExamRangedCharacter::InitializeChild(const SceneContext& /*sceneContext*/)
{
	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	m_pMaterial->SetDiffuseTexture(L"Textures/Chair_Dark.dds");
	m_pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	SoundSystem = SoundManager::Get()->GetSystem();
	auto file = ContentManager::GetFullAssetPath(L"Sound/projectile.wav");
	SoundSystem->createSound(file.string().c_str(), FMOD_DEFAULT, nullptr, &ProjectileSound);
}

void ExamRangedCharacter::UpdateChild(const SceneContext& sceneContext)
{
	
	float deltaTime = sceneContext.pGameTime->GetElapsed();
	if (CanShoot) {
		if (ShootTimer <= 0) {
			FireProjectile();
			ShootTimer = DefaultShootTimer;
		}
		else {
			ShootTimer -= deltaTime;
		}
	}

	for (auto projectile : GetChildren<Projectile>()) {
		if (projectile->IsMarkedForDelete()) {
			RemoveChild(projectile, true);
		}
	}
}

void ExamRangedCharacter::FireProjectile()
{
	auto trans{ GetTransform() };
	auto forward{ trans->GetForward() };
	auto originalPosition{ trans->GetWorldPosition() };
	auto rotation{ trans->GetWorldRotation() };

	auto go{ m_ProjectileHolder->AddChild(new Projectile(L"Meshes/Rock.ovm", forward, originalPosition, 200, 0, m_pMaterial, m_pDefaultMaterial)) };
	go->SetDamageToGive(GetAttackDamage());

	auto newRot = MathHelper::GetRotationTowardsPoint(originalPosition, forward, rotation, true);
	go->GetTransform()->Rotate(newRot, false);

	SoundSystem->playSound(ProjectileSound, nullptr, false, &CurrentChannel);
}