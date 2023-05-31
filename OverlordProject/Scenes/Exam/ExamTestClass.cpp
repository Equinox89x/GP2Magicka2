#include "stdafx.h"
#include "ExamTestClass.h"
#include <Prefabs/ExamCharacter.h>
#include <Prefabs/EnemyMeleeCharacter.h>
#include <unordered_set>

#pragma region Setup
ExamTestClass::ExamTestClass() : GameScene(L"ExamTestClass")
{
}

void ExamTestClass::Initialize()
{

	m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	CreateMagic();
	m_ComboBar.reserve(5);
	
	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_Material = PxGetPhysics().createMaterial(.5f, .5f, .5f);
	m_pMaterial->SetDiffuseTexture(L"Textures/Chair_Dark.dds");

	CreateLevel();

	CreateCharacter();

	CreateInput();

	CreateEmitters();

	auto pos{ m_pCharacter->GetTransform()->GetPosition() };
	auto pos1{ XMFLOAT3{pos.x - 50, pos.y + 10, pos.z - 450} };
	auto pos2{ XMFLOAT3{ pos.x + 100, pos.y + 10, pos.z + 250 } };
	auto pos3{ XMFLOAT3{ pos2.x + 50, pos.y + 10, pos2.z + 50 } };
	auto pos4{ XMFLOAT3{ pos3.x + 620, pos.y + 10, pos3.z + 100 } };
	auto pos5{ XMFLOAT3{ pos4.x+300, pos.y + 10, pos4.z+550 } };
	auto pos6{ XMFLOAT3{ pos5.x-100, pos.y + 10, pos5.z+550 } };

	Line line1{ {pos1, pos2} };
	Line line2{ {pos2, pos3} };
	Line line3{ {pos3, pos4} };
	Line line4{ {pos4, pos5} };
	Line line5{ {pos5, pos6} };
	std::vector<Line> lines{ line1, line2, line3, line4, line5 };

	Sphere = AddChild(new SpherePrefab{ 10,20, XMFLOAT4{1,0,0,1} });
	Sphere->GetTransform()->Translate(pos);
	m_pCamera->InitPoints(lines);

	SetStartPos();

	m_pCameraComponent->GetTransform()->Translate(300, 400, -100);

	CreateUI();
}

void ExamTestClass::SetStartPos()
{
	//calculate new position
	auto p1{ m_pCamera->m_LinePoints[0].points[0]};
	auto p2{ m_pCamera->m_LinePoints[0].points[1]};
	auto point{ m_pCharacter->GetTransform()->GetWorldPosition() };

	XMFLOAT3 lineDir = { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
	XMFLOAT3 p1ToPos1 = { point.x - p1.x, point.y - p1.y, point.z - p1.z };

	float dotProduct = lineDir.x * lineDir.x + lineDir.y * lineDir.y + lineDir.z * lineDir.z;
	float projection = (p1ToPos1.x * lineDir.x + p1ToPos1.y * lineDir.y + p1ToPos1.z * lineDir.z) / dotProduct;

	XMFLOAT3 projectedPoint = { p1.x + (projection * lineDir.x),
		p1.y + (projection * lineDir.y),
		p1.z + (projection * lineDir.z) };

	XMFLOAT3 newPos{ projectedPoint.x, p1.y, projectedPoint.z };
	Sphere->GetTransform()->Translate(newPos);

	XMFLOAT3 p{ m_pCamera->GetTransform()->GetWorldPosition() };
	p.x += projectedPoint.x;
	p.y += projectedPoint.y;
	p.z += projectedPoint.z;

	m_SceneContext.pCamera->GetTransform()->Translate(p);
	m_pCameraComponent->GetTransform()->Translate(p);

	m_pCameraComponent->GetTransform()->Rotate(47, -53, 0);
	m_SceneContext.pCamera->GetTransform()->Rotate(47, -53, 0);

}

void ExamTestClass::CreateLevel()
{
	//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//level
	m_pLevel = new GameObject();
	AddChild(m_pLevel);
	m_pLevel->AddComponent(new ModelComponent(L"Meshes/level.ovm"));
	m_pLevelMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pLevelMaterial->SetDiffuseTexture(L"Textures/level.dds");
	m_pLevel->GetComponent<ModelComponent>()->SetMaterial(m_pLevelMaterial);

	m_pLevel->AddComponent(new RigidBodyComponent());
	m_pLevel->GetComponent<RigidBodyComponent>()->SetKinematic(true);
	const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/level.ovpt");
	m_pLevel->GetComponent<RigidBodyComponent>()->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ 1.f,1.f,1.f })), *pDefaultMaterial);
	//const PxTriangleMesh* mesh{ ContentManager::Load<PxTriangleMesh>(L"Meshes/level.ovpc") };
	//m_pLevel->GetComponent<RigidBodyComponent>()->AddCollider(mesh, *pDefaultMaterial);


}

void ExamTestClass::CreateInput()
{
	//Input	
	auto inputAction = InputAction(ElementBottom, InputState::released, 'Q', -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(ElementLeft, InputState::released, 'E', -1, XINPUT_GAMEPAD_X);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(ElementTop, InputState::released, 'Z', -1, XINPUT_GAMEPAD_Y);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(ElementRight, InputState::released, 'F', -1, XINPUT_GAMEPAD_B);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(SwitchElement, InputState::down, VK_LSHIFT, -1, XINPUT_GAMEPAD_LEFT_SHOULDER);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(AoEAttack, InputState::down, VK_LCONTROL, -1, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	m_SceneContext.pInput->AddInputAction(inputAction);					  
	
	inputAction = InputAction(SwordInput, InputState::down, VK_EXECUTE, -1, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	m_SceneContext.pInput->AddInputAction(inputAction);
	
	inputAction = InputAction(SelfCast, InputState::down, VK_LSHIFT, -1, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	m_SceneContext.pInput->AddInputAction(inputAction);
}

void ExamTestClass::CreateCharacter()
{
	//Character
	CharacterDescExtended characterDesc{ m_Material };
	/*characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;*/

	characterDesc.actionId_ElementBottom = ElementBottom;
	characterDesc.actionId_ElementLeft = ElementLeft;
	characterDesc.actionId_ElementTop = ElementTop;
	characterDesc.actionId_ElementRight = ElementRight;
	characterDesc.actionId_SwitchElement = SwitchElement;

	characterDesc.actionId_AoEAttack = AoEAttack;
	characterDesc.actionId_SelfCast = SelfCast;
	characterDesc.actionId_SwordInput = SwordInput;

	characterDesc.actionId_Move = Move;
	characterDesc.actionId_Execute = Execute;

	m_pCharacter = AddChild(new ExamCharacter(characterDesc));
	m_pCharacter->GetTransform()->Translate(0.f, 5.f, 0.f);
	m_pCharacter->AddComponent(new ModelComponent(L"Meshes/wizard.ovm"));

	m_pCharacter->GetComponent<ModelComponent>()->SetMaterial(m_pMaterial);
	m_pCharacter->GetTransform()->Scale(0.5f);

	for (auto comp : m_pCharacter->GetComponents<ParticleEmitterComponent>()) {
		comp->GetTransform()->Translate(0, 20, 0);
	}

	m_pCamera = AddChild(new MagickaCamera());
	m_pCameraComponent = m_pCamera->GetComponent<CameraComponent>();
	m_pCamera->GetTransform()->Translate(0.f, characterDesc.controller.height * .5f, 0.f);
	m_SceneContext.pCamera->GetTransform()->Rotate(-45, -90, 0);

	auto enemy = AddChild(new EnemyMeleeCharacter());
	enemy->GetTransform()->Translate(0.f, 5.f, 0.f);
	enemy->AddComponent(new ModelComponent(L"Meshes/wizard.ovm"));

	enemy->GetComponent<ModelComponent>()->SetMaterial(m_pMaterial);
	enemy->GetTransform()->Scale(0.5f);
	m_pMeleeEnemies.push_back(enemy);

}

void ExamTestClass::CreateMagic()
{
	const MagicTypes earthBomb{ ElementTypes::EARTH, ProjectileTypes::PROJECTILEBOMB, 75 };
	const MagicTypes iceBomb{ ElementTypes::EARTH, ProjectileTypes::PROJECTILEBOMB, 137.5f };
	const MagicTypes arcaneBomb{ ElementTypes::EARTH, ProjectileTypes::PROJECTILEBOMB, 425 };
	const MagicTypes lifeBomb{ ElementTypes::EARTH, ProjectileTypes::PROJECTILEBOMB, 360 };
	const MagicTypes steamBomb{ ElementTypes::EARTH, ProjectileTypes::PROJECTILEBOMB, 280 };
	const MagicTypes waterBomb{ ElementTypes::EARTH, ProjectileTypes::PROJECTILEBOMB, 0 };
	const MagicTypes fireBomb{ ElementTypes::EARTH, ProjectileTypes::PROJECTILEBOMB, 60 };
	const MagicTypes coldBomb{ ElementTypes::EARTH, ProjectileTypes::PROJECTILEBOMB, 25 };

	const MagicTypes arcaneBeam{ ElementTypes::NONE, ProjectileTypes::BEAM, 106.25f };
	const MagicTypes lifeBeam{ ElementTypes::NONE, ProjectileTypes::BEAM, 89 };
	const MagicTypes steamBeam{ ElementTypes::NONE, ProjectileTypes::BEAM, 90 };
	const MagicTypes lightningBeam{ ElementTypes::NONE, ProjectileTypes::BEAM, 51.25f };
	const MagicTypes waterBeam{ ElementTypes::NONE, ProjectileTypes::BEAM, 0 };
	const MagicTypes fireBeam{ ElementTypes::NONE, ProjectileTypes::BEAM, 30 };
	const MagicTypes coldBeam{ ElementTypes::NONE, ProjectileTypes::BEAM, 12.5 };

	const MagicTypes iceBarrage{ ElementTypes::ICE, ProjectileTypes::PROJECTILE, 60 };
	const MagicTypes arcaneBarrage{ ElementTypes::ICE, ProjectileTypes::PROJECTILE, 42.5f };
	const MagicTypes lifeBarrage{ ElementTypes::ICE, ProjectileTypes::PROJECTILE, 60 };
	const MagicTypes lightningBarrage{ ElementTypes::ICE, ProjectileTypes::PROJECTILE, 42.5f };
	const MagicTypes coldBarrage{ ElementTypes::ICE, ProjectileTypes::PROJECTILE, 0 };
	const MagicTypes waterBarrage{ ElementTypes::ICE, ProjectileTypes::PROJECTILE, 0 };

	const MagicTypes steamSpray{ ElementTypes::NONE, ProjectileTypes::SPRAY, 70 };
	const MagicTypes lightningSpray{ ElementTypes::NONE, ProjectileTypes::SPRAY, 0 };
	const MagicTypes fireSpray{ ElementTypes::NONE, ProjectileTypes::SPRAY, 15 };
	const MagicTypes waterSpray{ ElementTypes::NONE, ProjectileTypes::SPRAY, 0 };

	const MagicTypes lightningSpray2{ ElementTypes::LIGHTNING, ProjectileTypes::SPRAY, 31.25f };
	const MagicTypes fireSpray2{ ElementTypes::LIGHTNING, ProjectileTypes::SPRAY, 0 };
	const MagicTypes coldSpray2{ ElementTypes::LIGHTNING, ProjectileTypes::SPRAY, 0 };

	const MagicTypes poisonSpray{ ElementTypes::NONE, ProjectileTypes::SPRAY, 0 };

	const MagicTypes shieldShield{ ElementTypes::NONE, ProjectileTypes::SHIELD, 0 };

	m_MagicTypes = { earthBomb, iceBomb, arcaneBomb, lifeBomb, steamBomb, waterBomb, fireBomb, coldBomb,
		arcaneBeam, lifeBeam, steamBeam, lightningBeam, waterBeam, fireBeam, coldBeam,
		iceBarrage, arcaneBarrage, lifeBarrage, lightningBarrage, coldBarrage, waterBarrage,
		steamSpray, lightningSpray, fireSpray, waterSpray,
		lightningSpray2, fireSpray2, coldSpray2
	};

	const MagicSpell push{ std::deque<ElementTypes>{ElementTypes::WATER, ElementTypes::SHIELD}, Spells::PUSH };
	const MagicSpell emergencyTeleport{ std::deque<ElementTypes>{ElementTypes::LIGHTNING, ElementTypes::ARCANE, ElementTypes::LIGHTNING}, Spells::EMERGENCYTELEPORT };
	const MagicSpell dispel{ std::deque<ElementTypes>{ElementTypes::ARCANE, ElementTypes::SHIELD}, Spells::DISPEL };
	const MagicSpell randomspell{ std::deque<ElementTypes>{ElementTypes::FIRE, ElementTypes::LIGHTNING, ElementTypes::LIFE}, Spells::RANDOMSPELL };
	const MagicSpell teleport{ std::deque<ElementTypes>{ElementTypes::LIGHTNING, ElementTypes::ARCANE, ElementTypes::ARCANE}, Spells::TELEPORT };
	const MagicSpell haste{ std::deque<ElementTypes>{ElementTypes::LIGHTNING, ElementTypes::ARCANE, ElementTypes::FIRE}, Spells::HASTE };
	const MagicSpell disruptor{ std::deque<ElementTypes>{ElementTypes::ARCANE, ElementTypes::EARTH, ElementTypes::SHIELD}, Spells::DISRUPTOR };
	const MagicSpell concussion{ std::deque<ElementTypes>{ElementTypes::WATER, ElementTypes::SHIELD, ElementTypes::WATER, ElementTypes::WATER}, Spells::CONCUSSION };
	const MagicSpell thunderbolt{ std::deque<ElementTypes>{ElementTypes::STEAM, ElementTypes::LIGHTNING, ElementTypes::ARCANE, ElementTypes::LIGHTNING}, Spells::THUNDERBOLT };
	const MagicSpell spikequake{ std::deque<ElementTypes>{ElementTypes::ARCANE, ElementTypes::ARCANE, ElementTypes::EARTH, ElementTypes::ARCANE, ElementTypes::ARCANE}, Spells::SPIKEQUAKE };
	const MagicSpell icetornado{ std::deque<ElementTypes>{ElementTypes::ICE, ElementTypes::EARTH, ElementTypes::COLD, ElementTypes::COLD}, Spells::ICETORNADO };
	const MagicSpell breeze{ std::deque<ElementTypes>{ElementTypes::ICE, ElementTypes::ICE, ElementTypes::ICE, ElementTypes::ICE }, Spells::BREEZE };
	const MagicSpell guardian{ std::deque<ElementTypes>{ElementTypes::LIFE, ElementTypes::LIFE, ElementTypes::LIGHTNING, ElementTypes::SHIELD }, Spells::GUARDIAN };
	const MagicSpell dragonstrike{ std::deque<ElementTypes>{ElementTypes::FIRE, ElementTypes::FIRE, ElementTypes::EARTH, ElementTypes::FIRE, ElementTypes::FIRE}, Spells::DRAGONSTRIKE };
	const MagicSpell fowl{ std::deque<ElementTypes>{ElementTypes::ARCANE, ElementTypes::EARTH, ElementTypes::ARCANE, ElementTypes::EARTH}, Spells::FOWL };
	const MagicSpell thunderhead{ std::deque<ElementTypes>{ElementTypes::FIRE, ElementTypes::FIRE, ElementTypes::ARCANE, ElementTypes::LIGHTNING, ElementTypes::LIGHTNING}, Spells::THUNDERHEAD };
	const MagicSpell summon{ std::deque<ElementTypes>{ElementTypes::ICE, ElementTypes::EARTH, ElementTypes::ARCANE, ElementTypes::COLD}, Spells::SUMMON };
	const MagicSpell sacrifice{ std::deque<ElementTypes>{ElementTypes::ARCANE, ElementTypes::ARCANE, ElementTypes::ARCANE, ElementTypes::ARCANE, ElementTypes::LIGHTNING}, Spells::SACRIFICE };
	const MagicSpell revive{ std::deque<ElementTypes>{ElementTypes::LIFE, ElementTypes::LIGHTNING}, Spells::REVIVE };
	m_MagicSpells = { push, emergencyTeleport, dispel, randomspell, teleport, haste, disruptor, concussion, thunderbolt, spikequake, icetornado, breeze, guardian, dragonstrike, fowl, thunderhead, summon, sacrifice, revive };


	const Magic water{ ElementTypes::WATER, waterSpray, std::vector<ElementTypes>{ElementTypes::LIGHTNING}, std::vector<ElementTypes>{ElementTypes::FIRE, ElementTypes::COLD, ElementTypes::ARCANE} };
	const Magic lightning{ ElementTypes::LIGHTNING, lightningSpray, std::vector<ElementTypes>{ElementTypes::WATER, ElementTypes::EARTH} };
	const Magic life{ ElementTypes::LIFE, lifeBeam, std::vector<ElementTypes>{ElementTypes::ARCANE, ElementTypes::POISON} };
	const Magic arcane{ ElementTypes::ARCANE, arcaneBeam, std::vector<ElementTypes>{ElementTypes::LIFE}, std::vector<ElementTypes>{ElementTypes::WATER} };
	const Magic shield{ ElementTypes::SHIELD, shieldShield, std::vector<ElementTypes>{ElementTypes::SHIELD} };
	const Magic earth{ ElementTypes::EARTH, earthBomb, std::vector<ElementTypes>{ElementTypes::LIGHTNING} };
	const Magic cold{ ElementTypes::COLD, coldSpray2, std::vector<ElementTypes>{ElementTypes::FIRE, ElementTypes::STEAM}, std::vector<ElementTypes>{ElementTypes::WATER} };
	const Magic fire{ ElementTypes::FIRE, fireSpray, std::vector<ElementTypes>{ElementTypes::COLD, ElementTypes::ICE}, std::vector<ElementTypes>{ElementTypes::WATER} };

	const Magic steam{ ElementTypes::STEAM, steamSpray, std::vector<ElementTypes>{}, std::vector<ElementTypes>{ElementTypes::WATER, ElementTypes::FIRE} };
	const Magic ice{ ElementTypes::ICE, iceBarrage, std::vector<ElementTypes>{}, std::vector<ElementTypes>{ElementTypes::WATER, ElementTypes::COLD} };
	const Magic poison{ ElementTypes::POISON, poisonSpray, std::vector<ElementTypes>{}, std::vector<ElementTypes>{ElementTypes::WATER, ElementTypes::ARCANE} };
	m_CombinedMagics = { steam, ice, poison };
	m_Magics = { water, lightning, life, arcane, shield, earth, cold, fire };

	for (int i = static_cast<int>(ElementTypes::NONE); i < static_cast<int>(ElementTypes::WATER); i++) {
		m_PatternOptions += static_cast<ElementTypes>(i);
	}
	
}

void ExamTestClass::CreateEmitters()
{
	//spray
	float value{ 50 };
	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,0.f, value };
	settings.originalVelocity = { 0.f,0.f, value };
	settings.modifier = { 1, 1, 1 };
	settings.minSize = 20.f;
	settings.maxSize = 20.f;
	settings.minEnergy = 1.f;
	settings.maxEnergy = 2.f;
	settings.minScale = 3.5f;
	settings.maxScale = 5.5f;
	settings.minEmitterRadius = .2f;
	settings.maxEmitterRadius = .5f;
	settings.color = { 1.f,1.f,1.f, .6f };

	m_pSprayMagicEmitter = AddChild(new GameObject());
	settings.originalVelocity = { value, 0.f, value };
	settings.modifier = { 1, 0.f, 1 };
	auto component = new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200);
	m_pSprayMagicEmitter->AddComponent(component);

	settings.originalVelocity = { value, 0.f, -value };
	settings.modifier = { 1, 0.f, 1 };
	component = new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200);
	m_pSprayMagicEmitter->AddComponent(component);

	settings.originalVelocity = { value, 0.f, value / 2 };
	settings.modifier = { 1, 0.f, 0.5 };
	component = new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200);
	m_pSprayMagicEmitter->AddComponent(component);

	settings.originalVelocity = { value, 0.f, -value / 2 };
	settings.modifier = { 1, 0.f, 0.5 };
	component = new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200);
	m_pSprayMagicEmitter->AddComponent(component);

	settings.originalVelocity = { value, 0.f, 0.f };
	settings.modifier = { 1, 0.f, 0.f };
	component = new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200);
	m_pSprayMagicEmitter->AddComponent(component);

	settings.originalVelocity = { value, 0.f, 0.f };
	settings.modifier = { 1, 0.f, 0.f };
	component = new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200);
	m_pSprayMagicEmitter->AddComponent(component);

	settings.originalVelocity = { value, 0.f, value / 4 };
	settings.modifier = { 1, 0.f, 0.f };
	component = new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200);
	m_pSprayMagicEmitter->AddComponent(component);

	settings.originalVelocity = { value, 0.f, value / 1.5f };
	settings.modifier = { 1, 0.f, 0.f };
	component = new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200);
	m_pSprayMagicEmitter->AddComponent(component);

	settings.originalVelocity = { value, 0.f, 0.f };
	settings.modifier = { 1, 0.f, 0.f };
	component = new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200);
	m_pSprayMagicEmitter->AddComponent(component);

	settings.originalVelocity = { value, 0.f, -value / 4 };
	settings.modifier = { 1, 0.f, 0.f };
	component = new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200);
	m_pSprayMagicEmitter->AddComponent(component);

	settings.originalVelocity = { value, 0.f, -value / 1.5f };
	settings.modifier = { 1, 0.f, 0.f };
	component = new ParticleEmitterComponent(L"Textures/Smoke.png", settings, 200);
	m_pSprayMagicEmitter->AddComponent(component);

	auto material = PxGetPhysics().createMaterial(.5f, .5f, .1f);
	//beam
	m_pBeamMagicEmitter = new CubePrefab(10, 10, 600);
	m_pCharacter->AddChild(m_pBeamMagicEmitter);
	m_pBeamMagicEmitter->GetTransform()->Translate(0, 0, 300);
	m_pBeamMagicEmitter->AddComponent(new RigidBodyComponent());
	m_pBeamMagicEmitter->GetComponent<RigidBodyComponent>()->SetKinematic(true);
	m_pBeamMagicEmitter->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 10, 10, 600 }, *material);
	auto colliderInfo = m_pBeamMagicEmitter->GetComponent<RigidBodyComponent>()->GetCollider(0);
	colliderInfo.SetTrigger(true);

	m_pBeamMagicEmitter->SetOnTriggerCallBack([=](GameObject* /*pTriggerObject*/, GameObject* pOtherObject, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER)
			{
				if (auto enemy{ reinterpret_cast<EnemyMeleeCharacter*>(pOtherObject) }) {
					enemy->DamageBeamEnter(MagicResult);
				}
			}
			if (action == PxTriggerAction::LEAVE)
			{
				if (auto enemy{ reinterpret_cast<EnemyMeleeCharacter*>(pOtherObject) }) {
					enemy->DamageBeamExit(MagicResult);
				}
			}
		});

	//aoe
	m_pAOEMagicEmitter = AddChild(new TorusPrefab(50.f, 50, 20.f, 50, XMFLOAT4{1,0,0,1}));
	m_pAOEMagicEmitter->AddComponent(component);
	m_pAOEMagicEmitter->GetTransform()->Rotate(90, 0, 0);

	m_pAOEMagicEmitter->AddComponent(new RigidBodyComponent());
	m_pAOEMagicEmitter->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 50,20,50 }, *material);
	m_pAOEMagicEmitter->GetComponent<RigidBodyComponent>()->SetKinematic(true);

	colliderInfo = m_pAOEMagicEmitter->GetComponent<RigidBodyComponent>()->GetCollider(0);
	colliderInfo.SetTrigger(true);

	m_pAOEMagicEmitter->SetOnTriggerCallBack([=](GameObject* /*pTriggerObject*/, GameObject* pOtherObject, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER)
			{
				if (auto enemy{ reinterpret_cast<EnemyMeleeCharacter*>(pOtherObject) }) {
					enemy->DamageAOE(MagicResult);
				}
			}
		});

	//m_pShieldMagicEmitter

	//const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	//const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Sphere.ovpt");
	//m_pBeamMagicEmitter->GetComponent<RigidBodyComponent>()->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ 10.f,10.f,10.f })), *pDefaultMaterial);


	m_pSprayMagicEmitter->SetVisibility(false);
	m_pBeamMagicEmitter->SetVisibility(false);
	m_pAOEMagicEmitter->SetVisibility(false);
}

void ExamTestClass::CreateUI()
{
	m_pUI = AddChild(new GameObject());
	m_pUI->GetTransform()->Translate(100, m_SceneContext.windowHeight - 200, 0);

	m_pUI2 = AddChild(new GameObject());
	m_pUI2->GetTransform()->Translate(112.5f, m_SceneContext.windowHeight - 140, 0);

	m_pUI3 = AddChild(new GameObject());
	m_pUI3->GetTransform()->Translate(100, m_SceneContext.windowHeight/2, 0);

	//order is important in all 3 vectors (don't change)
	auto arr{ std::vector<std::wstring>{
			L"Textures/Magicka/Q.png",
			L"Textures/Magicka/E.png",
			L"Textures/Magicka/Z.png",
			L"Textures/Magicka/F.png",
	} };

	m_Textures = std::vector<std::wstring>{
			L"Textures/Magicka/Element_water.png",
			L"Textures/Magicka/Element_shield.png",
			L"Textures/Magicka/Element_life.png",
			L"Textures/Magicka/Element_cold.png",
			L"Textures/Magicka/Element_lightning.png",
			L"Textures/Magicka/Element_earth.png",
			L"Textures/Magicka/Element_arcane.png",
			L"Textures/Magicka/Element_fire.png",
	};
	m_TransparentTextures = std::vector<std::wstring>{
			L"Textures/Magicka/Element_water_transparent.png",
			L"Textures/Magicka/Element_shield_transparent.png",
			L"Textures/Magicka/Element_life_transparent.png",
			L"Textures/Magicka/Element_cold_transparent.png",
			L"Textures/Magicka/Element_lightning_transparent.png",
			L"Textures/Magicka/Element_earth_transparent.png",
			L"Textures/Magicka/Element_arcane_transparent.png",
			L"Textures/Magicka/Element_fire_transparent.png",
	};

	float height1{ 0 };
	float width1{ 0 };
	for (size_t i = 0; i < 4; i++)
	{
		auto go{ new GameObject() };
		m_pUI2->AddChild(go);
		go->AddComponent(new SpriteComponent(arr[i]));
		go->GetTransform()->Translate(width1, height1, 0);
		go->GetTransform()->Scale(0.2f);
		width1 += 60;
	}

	float height{ 0 };
	float width{ 0 };
	int id{ 0 };
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			auto go{ new GameObject() };
			m_pUI->AddChild(go);
			go->AddComponent(new SpriteComponent(m_Textures[id]));
			go->GetTransform()->Translate(width, height, 0);
			width += 60;
			id++;
		}
		width = 0;
		height += 100;
	}

	float width2{ 0 };
	float height2{ 0 };
	for (size_t i = 0; i < 5; i++)
	{
		auto go{ new GameObject() };
		m_pUI3->AddChild(go);
		go->AddComponent(new SpriteComponent(L"Textures/Magicka/Element_water.png"));
		go->GetTransform()->Translate(width2, height2, 0);
		go->GetTransform()->Scale(0.6f);
		width2 += 35;
	}
}
#pragma endregion

void ExamTestClass::OnGUI()
{
	m_pCharacter->DrawImGui();
}

#pragma region Update
void ExamTestClass::Update()
{
	//for (auto comp : m_pCharacter->GetComponents<ParticleEmitterComponent>()) {
	//	auto pos = m_pCharacter->GetTransform()->GetPosition();
	//	//comp->GetTransform()->Translate(pos);
	//}

	XMFLOAT3 pos{ m_pCharacter->GetTransform()->GetForward() };
	XMFLOAT3 pos2{ m_pCharacter->GetTransform()->GetWorldPosition()};
	pos.x *= 400 + pos2.x;
	pos.z *= 400 + pos2.z;
	m_pAOEMagicEmitter->GetTransform()->Translate(pos2);
	//m_pBeamMagicEmitter->GetComponent<RigidBodyComponent>()->Translate(pos);

	//pos2.z -= 20;
	//pos2.x -= 20;
	XMVECTOR screenPosition = XMVector3Project(XMLoadFloat3(&pos2), 0, 0, m_SceneContext.windowWidth, m_SceneContext.windowHeight, 0.0f, 1.0f, XMLoadFloat4x4(&m_SceneContext.pCamera->GetProjection()), XMLoadFloat4x4(&m_SceneContext.pCamera->GetView()), XMMatrixIdentity());
	XMFLOAT3 screenPos;
	XMStoreFloat3(&screenPos, screenPosition);
	screenPos.x -= 90;
	screenPos.y += 30;

	m_pUI3->GetTransform()->Translate(screenPos);

	HandleCameraMovement();

	HandleMagicTransform();
}

void ExamTestClass::HandleMagicTransform()
{
	auto originalRotation{ m_pCharacter->GetComponent<ModelComponent>()->GetTransform()->GetWorldRotation() };
	auto originalLocation{ m_pCharacter->GetComponent<ModelComponent>()->GetTransform()->GetWorldPosition() };

	m_pSprayMagicEmitter->GetTransform()->Translate(originalLocation);
	//m_pBeamMagicEmitter->GetTransform()->Translate(originalLocation);
	//m_pBeamMagicEmitter->GetTransform()->Rotate(XMFLOAT3{ originalRotation.x, originalRotation.y, originalRotation.z });

	XMVECTOR rotationQuaternion = XMLoadFloat4(&originalRotation);
	rotationQuaternion = XMQuaternionMultiply(rotationQuaternion, XMQuaternionRotationRollPitchYaw(0.0f, -XM_PIDIV2, 0.0f));
	rotationQuaternion = XMQuaternionNormalize(rotationQuaternion);

	//apply rotation to the particles
	auto comps{ m_pSprayMagicEmitter->GetComponents<ParticleEmitterComponent>() };
	for (auto comp : comps) {
		auto velo{ comp->GetSettings().originalVelocity };
		XMVECTOR vectorToRotate = XMLoadFloat3(&velo);
		XMVECTOR rotatedVector = XMVector3Rotate(vectorToRotate, rotationQuaternion);
		XMFLOAT3 rot;
		XMStoreFloat3(&rot, rotatedVector);
		comp->GetSettings().velocity = rot;
	}

}

void ExamTestClass::HandleCameraMovement()
{
	auto point{ m_pCharacter->GetTransform()->GetWorldPosition() };
	auto pos1{ m_pCamera->m_LinePoints[currentLineIndex].points[0]};
	auto pos2{ m_pCamera->m_LinePoints[currentLineIndex].points[1] };

	//project on line
	XMFLOAT3 lineDir = { pos2.x - pos1.x, pos2.y - pos1.y, pos2.z - pos1.z };
	XMFLOAT3 p1ToPos1 = { point.x - pos1.x, point.y - pos1.y, point.z - pos1.z };

	float dotProduct = lineDir.x * lineDir.x + lineDir.y * lineDir.y + lineDir.z * lineDir.z;
	float projection = (p1ToPos1.x * lineDir.x + p1ToPos1.y * lineDir.y + p1ToPos1.z * lineDir.z) / dotProduct;
	XMFLOAT3 projectedPoint = { pos1.x + (projection * lineDir.x), pos1.y + (projection * lineDir.y), pos1.z + (projection * lineDir.z) };

	XMFLOAT3 newPos{ projectedPoint.x, pos1.y, projectedPoint.z };
	if (IsPointInCircle3D(newPos, pos2, 0.5f)) {
		if (CanIncrease) {
			currentLineIndex++;
			CanIncrease = false;
		}
	}	
	else if (IsPointInCircle3D(newPos, pos1, 0.5f)) {
		if (CanIncrease) {
			currentLineIndex--;
			CanIncrease = false;
		}
	}
	else {
	}

	Sphere->GetTransform()->Translate(newPos);
	auto newpos1{ m_pCamera->m_LinePoints[currentLineIndex].points[0] };
	auto newpos2{ m_pCamera->m_LinePoints[currentLineIndex].points[1] };
	
	if (CheckRange(newPos.x, newpos1.x, newpos2.x) ||
		//newpos1.y < y && y < newpos2.y ||
		CheckRange(newPos.z, newpos1.z, newpos2.z)) {
		CanIncrease = true;

	}

	//move camera with projected point
	XMFLOAT3 pos{ m_pCamera->GetTransform()->GetWorldPosition() };
	pos.x += projectedPoint.x;
	pos.y += projectedPoint.y;
	pos.z += projectedPoint.z;
	m_SceneContext.pCamera->GetTransform()->Translate(pos);
}
#pragma endregion

#pragma region Draw
void ExamTestClass::Draw()
{
	m_pCamera->Draw(SceneContext());
}

void ExamTestClass::PostDraw()
{
	//HandleCameraMovement();
}
#pragma endregion

#pragma region Execution
void ExamTestClass::ExecuteMagicCombo()
{
	//const auto spell{ std::find_if(m_MagicSpells.begin(), m_MagicSpells.end(), [&](const MagicSpell& spell) {
	//	return spell.Pattern == m_ComboPattern;
	//}) };

	//if (spell != m_MagicSpells.end()) {
	//	//cast the selected spell
	//	HandlePrint2(spell->Spell);

	//	//ResetCombo();
	//}
	//else {
	if (!IsExecutingMagic) {
		IsExecutingMagic = true;
		//set up result magic
		for (auto type : m_ComboBar) {
			//add modifiers to the result magic
			const auto magic{ std::find_if(MagicResult.Modifiers.begin(), MagicResult.Modifiers.end(), [&](const ElementTypes& magic) {
				return magic == type.ElementType;
			}) };
			if (magic == MagicResult.Modifiers.end()) {
				MagicResult.Modifiers.push_back(type.ElementType);
				MagicResult.AOETextureName.push_back(type.AOETextureName);
				MagicResult.TextureName.push_back(type.TextureName);
			}
		}


		////find most occuring character in the pattern
		//const auto count = std::unordered_multiset<char>{ m_ComboPattern.begin(), m_ComboPattern.end() };
		//const auto comparator = [count](auto a, auto b) { return count.count(a) < count.count(b); };
		//if (!count.empty()) {
		//	auto character{ *std::max_element(count.begin(), count.end(), comparator) };
		//	HandlePrint(character);

		//	const auto selectedMagic{ std::find_if(m_Magics.begin(), m_Magics.end(), [&](const Magic& magic) {
		//		return magic.ElementType == static_cast<ElementTypes>(character);
		//	}) };
		//	if (selectedMagic != m_Magics.end()) {
		//		m_pSprayMagicEmitter->SetVisibility(selectedMagic->DefaultMagicType.ProjectileType == ProjectileTypes::SPRAY);
		//		m_pBeamMagicEmitter->SetVisibility(selectedMagic->DefaultMagicType.ProjectileType == ProjectileTypes::BEAM);
		//		
		//		//change texture
		//		if (selectedMagic->DefaultMagicType.ProjectileType == ProjectileTypes::SPRAY) {
		//			for (auto comp : m_pSprayMagicEmitter->GetComponents<ParticleEmitterComponent>()) {
		//				comp->SetTexture(m_SceneContext, selectedMagic->TextureName);
		//			}
		//		}
		//	}
		//	//ResetCombo();
		//}
	//}
		if (m_ComboBar.size() > 0) {
			const auto selectedMagicShield{ std::find_if(m_ComboBar.begin(), m_ComboBar.end(), [&](const Magic& magic) {
				return magic.DefaultMagicType.ProjectileType == ProjectileTypes::SHIELD;
			}) };
			const auto selectedMagicProjectile{ std::find_if(m_ComboBar.begin(), m_ComboBar.end(), [&](const Magic& magic) {
				return magic.DefaultMagicType.ProjectileType == ProjectileTypes::PROJECTILE;
			}) };
			const auto selectedMagicBeam{ std::find_if(m_ComboBar.begin(), m_ComboBar.end(), [&](const Magic& magic) {
				return magic.DefaultMagicType.ProjectileType == ProjectileTypes::BEAM;
			}) };			
			const auto selectedMagicSpray{ std::find_if(m_ComboBar.begin(), m_ComboBar.end(), [&](const Magic& magic) {
				return magic.DefaultMagicType.ProjectileType == ProjectileTypes::SPRAY;
			}) };
			if (selectedMagicShield != m_ComboBar.end()) {
				MagicResult.Damage = selectedMagicShield->DefaultMagicType.Damage;
				MagicResult.ProjectileType = ProjectileTypes::SHIELD;
				m_pSprayMagicEmitter->SetVisibility(false);
				m_pAOEMagicEmitter->SetVisibility(false);
				m_pBeamMagicEmitter->SetVisibility(false);

			}
			else if (selectedMagicProjectile != m_ComboBar.end()) {
				MagicResult.Damage = selectedMagicProjectile->DefaultMagicType.Damage;
				MagicResult.ProjectileType = ProjectileTypes::PROJECTILE;
				m_pSprayMagicEmitter->SetVisibility(false);
				m_pAOEMagicEmitter->SetVisibility(false);
				m_pBeamMagicEmitter->SetVisibility(false);

			}
			else if (selectedMagicBeam != m_ComboBar.end()) {
				MagicResult.Damage = selectedMagicBeam->DefaultMagicType.Damage;
				MagicResult.ProjectileType = ProjectileTypes::BEAM;
				m_pSprayMagicEmitter->SetVisibility(false);
				m_pAOEMagicEmitter->SetVisibility(false);
				m_pBeamMagicEmitter->SetVisibility(true);

			}
			else {
				MagicResult.Damage = selectedMagicSpray->DefaultMagicType.Damage;
				MagicResult.ProjectileType = ProjectileTypes::SPRAY;
				m_pSprayMagicEmitter->SetVisibility(true);
				m_pBeamMagicEmitter->SetVisibility(false);
				m_pAOEMagicEmitter->SetVisibility(false);
				for (auto comp : m_pSprayMagicEmitter->GetComponents<ParticleEmitterComponent>()) {
					comp->SetTexture(m_SceneContext, MagicResult.TextureName[0]);
				}
			}
		}
	}
}

void ExamTestClass::ExecuteAOE()
{
	auto components{ m_pAOEMagicEmitter->GetComponents<ParticleEmitterComponent>() };
	MagicResult.ProjectileType = ProjectileTypes::AOE;

	for (size_t i = 0; i < m_ComboBar.size(); i++)
	{
		auto type{ m_ComboBar[i] };
		//add modifiers to the result magic
		const auto magic{ std::find_if(MagicResult.Modifiers.begin(), MagicResult.Modifiers.end(), [&](const ElementTypes& magic) {
			return magic == type.ElementType;
		}) };
		if (magic == MagicResult.Modifiers.end()) {
			MagicResult.Modifiers.push_back(type.ElementType);
		}
	}

	if (MagicResult.Modifiers.size() > 0 && m_ComboBar.size() > 0) {
		m_pAOEMagicEmitter->SetVisibility(true);
	}
}

void ExamTestClass::ExecuteSelfCast()
{
}

void ExamTestClass::ExecuteSword()
{
	const auto spell{ std::find_if(m_MagicSpells.begin(), m_MagicSpells.end(), [&](const MagicSpell& spell) {
		return spell.Pattern == m_ComboPattern;
	}) };

	if (spell != m_MagicSpells.end()) {
		//cast the selected spell
		HandlePrint2(spell->Spell);

		ResetCombo();
	}
	else {
		//sword attack
	}
}

void ExamTestClass::ResetCombo()
{
	m_ComboPattern = "";
	m_ComboBar.clear();
	m_ComboBar.reserve(5);

	m_pSprayMagicEmitter->SetVisibility(false);
	m_pBeamMagicEmitter->SetVisibility(false);
	m_pAOEMagicEmitter->SetVisibility(false);
	MagicResult.Modifiers.clear();
	MagicResult.TextureName.clear();
	MagicResult.AOETextureName.clear();
	IsExecutingMagic = false;
}
#pragma endregion

#pragma region UI
void ExamTestClass::ToggleUIElements(bool shiftPressed)
{
	auto arr{ m_pUI->GetChildren<GameObject>() };
	for (size_t i = 0; i < arr.size(); i++)
	{
		auto comp{ arr[i]->GetComponent<SpriteComponent>() };
		if (i < 4) {
			comp->SetTexture(shiftPressed ? m_TransparentTextures[i] : m_Textures[i]);
		}
		else {
			comp->SetTexture(shiftPressed ? m_Textures[i] : m_TransparentTextures[i]);
		}
	}
}
#pragma endregion

#pragma region Helpers
Magic* ExamTestClass::GetCombinedMagicType(ElementTypes elementType1, ElementTypes elementType2)
{
	const auto it{ std::find_if(m_CombinedMagics.begin(), m_CombinedMagics.end(), [&elementType1, &elementType2](const Magic& magic) {
		//safe to check like this because [0] will always be the water magic
		return magic.CombiningMagicTypes[0] == elementType1 && magic.CombiningMagicTypes[1] == elementType2;
	}) };

	if (it != m_CombinedMagics.end()) {
		return it._Ptr;
	}
	else {
		return nullptr;
	}
}

bool ExamTestClass::IsMagicCancelled(int currentIterator, const Magic* currentMagic, bool& foundCanceller)
{
	for (ElementTypes cancellingMagicType : m_ComboBar[currentIterator].CancellingMagicTypes) {
		if (cancellingMagicType == currentMagic->ElementType) {
			m_ComboBar.pop_back();
			m_ComboPattern.pop_back();
			foundCanceller = true;
			std::cout << "CANCELLED" << ", Pattern: " << m_ComboPattern << std::endl;
			return true;
		}
	}
	return false;
}
#pragma endregion

#pragma region Combo-ing
void ExamTestClass::HandleCombobarFilling(ElementTypes elementType)
{
	//find the spell of the current element type
	const auto comparator{ [&elementType](const Magic& magic) { return magic.ElementType == elementType; } };
	Magic* currentMagic{ std::find_if(m_Magics.begin(), m_Magics.end(), comparator)._Ptr };

	//get index of previous element
	const int currentIterator{ static_cast<int>(m_ComboBar.size()) - 1 };
	bool foundCanceller{ false };


	if (m_ComboBar.empty()) {
		//push the magic
		m_ComboBar.push_back(*currentMagic);
		m_ComboPattern += currentMagic->ElementType;
		HandlePrint(currentMagic->ElementType);
	}
	else if (m_ComboBar.size() >= 5) {
		//remove element from pattern if opposite and return
		if (IsMagicCancelled(currentIterator, currentMagic, foundCanceller)) return;
	}
	else if (!m_ComboBar.empty() && m_ComboBar.size() < 5) {
		//remove element from pattern if opposite and return
		if (IsMagicCancelled(currentIterator, currentMagic, foundCanceller)) return;

		//get the previous elemet to check against (for element combination)
		const Magic prevMagic{ m_ComboBar[currentIterator] };

		if (!foundCanceller) {
			//check for combined magic
			if (!prevMagic.CombiningMagicTypes.empty() && !currentMagic->CombiningMagicTypes.empty()) {

				//Water should be the first element to check
				if (auto magic{ 
					prevMagic.ElementType == ElementTypes::WATER ?
						GetCombinedMagicType(prevMagic.ElementType, currentMagic->ElementType) : 
						GetCombinedMagicType(currentMagic->ElementType, prevMagic.ElementType) 
				}) {
					m_ComboBar.pop_back();
					m_ComboPattern.pop_back();
					currentMagic = magic;
				}
			}

			//push the magic
			m_ComboBar.push_back(*currentMagic);
			m_ComboPattern += currentMagic->ElementType;
			HandlePrint(currentMagic->ElementType);
		}
	}
	m_pUI3->GetChildren<GameObject>()[m_ComboBar.size()-1]->GetComponent<SpriteComponent>()->SetTexture(currentMagic->UITextureName);
}
#pragma endregion

#pragma region Debug
void ExamTestClass::HandlePrint(const char currentMagic) const {
	std::string name;
	switch (static_cast<ElementTypes>(currentMagic))
	{
	case ElementTypes::WATER: name = "WATER"; break;
	case ElementTypes::LIGHTNING: name = "LIGHTNING"; break;
	case ElementTypes::LIFE: name = "LIFE"; break;
	case ElementTypes::ARCANE: name = "ARCANE"; break;
	case ElementTypes::SHIELD: name = "SHIELD"; break;
	case ElementTypes::EARTH: name = "EARTH"; break;
	case ElementTypes::COLD: name = "COLD"; break;
	case ElementTypes::FIRE: name = "FIRE"; break;
	case ElementTypes::STEAM: name = "STEAM"; break;
	case ElementTypes::ICE: name = "ICE"; break;
	case ElementTypes::POISON: name = "POISON"; break;
	default: break;
	}
	std::cout << "Most occuring Element: " << name << std::endl;

}

void ExamTestClass::HandlePrint(const ElementTypes currentMagic) const {
	std::string name;
	switch (currentMagic)
	{
	case ElementTypes::WATER: name = "WATER"; break;
	case ElementTypes::LIGHTNING: name = "LIGHTNING"; break;
	case ElementTypes::LIFE: name = "LIFE"; break;
	case ElementTypes::ARCANE: name = "ARCANE"; break;
	case ElementTypes::SHIELD: name = "SHIELD"; break;
	case ElementTypes::EARTH: name = "EARTH"; break;
	case ElementTypes::COLD: name = "COLD"; break;
	case ElementTypes::FIRE: name = "FIRE"; break;
	case ElementTypes::STEAM: name = "STEAM"; break;
	case ElementTypes::ICE: name = "ICE"; break;
	case ElementTypes::POISON: name = "POISON"; break;
	default: break;
	}

	std::cout << "Type: " << name << ", Size: " << m_ComboBar.size() << ", Pattern: " << m_ComboPattern << std::endl;
}

void ExamTestClass::HandlePrint2(Spells spell) const {
	std::string name;
	switch (spell)
	{
	case Spells::PUSH: name = "PUSH"; break;
	case Spells::EMERGENCYTELEPORT: name = "EMERGENCYTELEPORT"; break;
	case Spells::DISPEL: name = "DISPEL"; break;
	case Spells::RANDOMSPELL: name = "RANDOMSPELL"; break;
	case Spells::TELEPORT: name = "TELEPORT"; break;
	case Spells::HASTE: name = "HASTE"; break;
	case Spells::DISRUPTOR: name = "DISRUPTOR"; break;
	case Spells::CONCUSSION: name = "CONCUSSION"; break;
	case Spells::THUNDERBOLT: name = "THUNDERBOLT"; break;
	case Spells::SPIKEQUAKE: name = "SPIKEQUAKE"; break;
	case Spells::ICETORNADO: name = "ICETORNADO"; break;
	case Spells::BREEZE: name = "BREEZE"; break;
	case Spells::GUARDIAN: name = "GUARDIAN"; break;
	case Spells::DRAGONSTRIKE: name = "DRAGONSTRIKE"; break;
	case Spells::FOWL: name = "FOWL"; break;
	case Spells::THUNDERHEAD: name = "THUNDERHEAD"; break;
	case Spells::SUMMON: name = "SUMMON"; break;
	case Spells::SACRIFICE: name = "SACRIFICE"; break;
	case Spells::REVIVE: name = "REVIVE"; break;
	default: break;
	}

	std::cout << "Casted spell: " << name << std::endl;
}
#pragma endregion

#pragma region Math
bool ExamTestClass::IsPointNearViewport(const XMFLOAT3& point, float threshold)
{
	// Calculate the normalized device coordinates (NDC) for the point
	float ndcX = (point.x + 1.0f) / 2.0f; // Assuming viewport range: -1 to 1
	float ndcY = (point.z + 1.0f) / 2.0f; // Assuming viewport range: -1 to 1

	// Calculate the screen coordinates for the point
	int screenX = static_cast<int>(ndcX * m_SceneContext.windowHeight);
	int screenY = static_cast<int>(ndcY * m_SceneContext.windowWidth);

	// Check if the point is within the threshold distance from any border
	if (screenX < threshold || screenX >(m_SceneContext.windowHeight - threshold) ||
		screenY < threshold || screenY >(m_SceneContext.windowWidth - threshold)) {
		return true; // Point is near the viewport borders
	}
	else {
		return false; // Point is not near the viewport borders
	}
}

bool ExamTestClass::IsPointOnLine(const XMFLOAT3& point, const XMFLOAT3& linePoint1, const XMFLOAT3& linePoint2, float epsilon) {
	float slope = (linePoint2.z - linePoint1.z) / (linePoint2.x - linePoint1.x);
	float expectedY = linePoint1.z + slope * (point.x - linePoint1.x);

	return std::abs(point.z - expectedY) <= epsilon;
}

double ExamTestClass::Distance3D(XMFLOAT3 p1, XMFLOAT3 p2) {
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	double dz = p2.z - p1.z;
	return std::sqrt(dx * dx + dy * dy + dz * dz);
}

bool ExamTestClass::IsPointInCircle3D(XMFLOAT3 point, XMFLOAT3 circleCenter, double circleRadius) {
	double dist = Distance3D(point, circleCenter);
	return (dist <= circleRadius);
}

bool ExamTestClass::CheckRange(double value, double lowerBound, double upperBound) {
	return value > lowerBound && value < upperBound;
}
#pragma endregion