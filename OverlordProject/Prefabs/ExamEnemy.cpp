#include "stdafx.h"
#include "ExamEnemy.h"
#include "../OverlordEngine/Prefabs/MagickaCamera.h"
#include <Scenes/Exam/ExamTestClass.h>
#include "ExamCharacter.h"

ExamEnemy::ExamEnemy(const CharacterDescExtended& characterDesc, XMFLOAT3 postitionOffset) :
	m_CharacterDescExtended{ characterDesc },
	PostitionOffset{ postitionOffset }
{
}

void ExamEnemy::Initialize(const SceneContext& sceneContext)
{
	//auto go{ new GameObject() };
	//AddChild(go);
	//m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDescExtended.controller));
	GetTransform()->Translate(PostitionOffset);

	/*m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pMaterial->SetDiffuseTexture(L"Textures/Chair_Dark.dds");
	m_pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);*/
	InitializeChild(sceneContext);
}

void ExamEnemy::Update(const SceneContext& sceneContext)
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
	//if (m_Health <= 0) {
	//	RemoveChild(GetChild<GameObject>(), true);
	//}

	/*if (CanShoot) {
		if (ShootTimer <= 0) {
			FireProjectile();
			ShootTimer = DefaultShootTimer;
		}
		else {
			ShootTimer -= deltaTime;
		}
	}*/

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

	for (auto projectile : GetChildren<Projectile>()) {
		if (projectile->IsMarkedForDelete()) {
			//m_Projectiles.erase(std::remove(m_Projectiles.begin(), m_Projectiles.end(), projectile));
			RemoveChild(projectile, true);
		}
	}

	UpdateChild(sceneContext);
}

//void ExamEnemy::FireProjectile()
//{
//	//m_Projectiles.push_back(AddChild(new Projectile(L"Meshes/Rock.ovm", GetTransform()->GetForward(), XMFLOAT3{0,0,0}, 1000, m_pMaterial, m_pDefaultMaterial)));
//	AddChild(new Projectile(L"Meshes/Rock.ovm", GetTransform()->GetForward(), XMFLOAT3{ 0,0,0 }, 500, 1, m_pMaterial, m_pDefaultMaterial));
//}

void ExamEnemy::DrawImGui()
{

}

void ExamEnemy::DamageBeamEnter(float damage)
{
	CanDamage = true;
	DamageTimer = 0;
	DamageToTake = damage;
}

void ExamEnemy::DamageBeamExit()
{
	CanDamage = false;
	DamageTimer = DefaultDamageTimer;
	DamageToTake = 0;
}

void ExamEnemy::DamageAOE(float damage)
{
	if (CanDamageAoE) m_Health -= damage;
	CanDamageAoE = false;
	std::cout << m_Health << "\n";
}

void ExamEnemy::DamageAOE(float damage, bool canDamage)
{
	CanDamageAoE = canDamage;
	if (CanDamageAoE) m_Health -= damage;
	CanDamageAoE = false;
	std::cout << m_Health << "\n";
}
