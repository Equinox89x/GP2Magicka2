#include "stdafx.h"
#include "PongScene.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"

#define VK_W 0x57
#define VK_S 0x53
#define VK_I 0x49
#define VK_K 0x4B

PongScene::PongScene() : GameScene(L"PongScene")
{}

void PongScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	auto material = PxGetPhysics().createMaterial(.5f, .5f, .1f);

	m_pObject = new GameObject();
	AddChild(m_pObject);	
	
	m_pObject2 = new GameObject();
	AddChild(m_pObject2);

	m_pObject3 = new GameObject();
	AddChild(m_pObject3);

	#pragma region platforms
	CubePrefab* cube{ new CubePrefab(2,10,10, static_cast<XMFLOAT4>(Colors::Red)) };
	m_pObject->AddChild(cube);
	TransformComponent* comp{ cube->GetComponent<TransformComponent>() };
	comp->Translate(32, 0, 0);

	cube->AddComponent(new RigidBodyComponent());
	cube->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 1,5,5 }, *material);
	cube->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 1,5,5 }, *material);
	cube->GetComponent<RigidBodyComponent>()->SetKinematic(true);

	auto colliderInfo = cube->GetComponent<RigidBodyComponent>()->GetCollider(0);
	colliderInfo.SetTrigger(true);

	cube->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER)
			{
				m_isGoingLeft = !m_isGoingLeft;
			}
		});
	
	cube = new CubePrefab(2,10,10, static_cast<XMFLOAT4>(Colors::Red));
	m_pObject->AddChild(cube);
	comp = cube->GetComponent<TransformComponent>();
	comp->Translate(-32, 0, 0);
	
	cube->AddComponent(new RigidBodyComponent());
	cube->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 1,5,5 }, *material);
	cube->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 1,5,5 }, *material);
	cube->GetComponent<RigidBodyComponent>()->SetKinematic(true);
	colliderInfo = cube->GetComponent<RigidBodyComponent>()->GetCollider(0);
	colliderInfo.SetTrigger(true);

	cube->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER)
			{
				m_isGoingLeft = !m_isGoingLeft;
			}
		});
	#pragma endregion

	#pragma region Up Down Walls
	CubePrefab* Wall{ new CubePrefab(100,10,10, static_cast<XMFLOAT4>(Colors::Black)) };
	m_pObject2->AddChild(Wall);
	comp = Wall->GetComponent<TransformComponent>();
	comp->Translate(0, -27, 0);

	Wall->AddComponent(new RigidBodyComponent());
	Wall->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 50,5,5 }, *material);
	Wall->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 50,5,5 }, *material);
	Wall->GetComponent<RigidBodyComponent>()->SetKinematic(true);
	colliderInfo = Wall->GetComponent<RigidBodyComponent>()->GetCollider(0);
	colliderInfo.SetTrigger(true);
	
	Wall->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER) {
				m_upSpeed = 18;
			}
		});


	Wall = new CubePrefab(100,10,10, static_cast<XMFLOAT4>(Colors::Black));
	m_pObject2->AddChild(Wall);
	comp = Wall->GetComponent<TransformComponent>();
	comp->Translate(0, 35, 0);

	Wall->AddComponent(new RigidBodyComponent());
	Wall->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 50,5,5 }, *material);
	Wall->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 50,5,5 }, *material);
	Wall->GetComponent<RigidBodyComponent>()->SetKinematic(true);
	colliderInfo = Wall->GetComponent<RigidBodyComponent>()->GetCollider(0);
	colliderInfo.SetTrigger(true);

	Wall->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER) {
				m_upSpeed = -18;
			}
		});
	#pragma endregion

	#pragma region Reset Walls
	Wall = new CubePrefab(10, 100, 10, static_cast<XMFLOAT4>(Colors::Black));
	m_pObject3->AddChild(Wall);
	comp = Wall->GetComponent<TransformComponent>();
	comp->Translate(-40, 0, 0);

	Wall->AddComponent(new RigidBodyComponent());
	Wall->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 5,50,5 }, *material);
	Wall->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 5,50,5 }, *material);
	Wall->GetComponent<RigidBodyComponent>()->SetKinematic(true);
	colliderInfo = Wall->GetComponent<RigidBodyComponent>()->GetCollider(0);
	colliderInfo.SetTrigger(true);

	Wall->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER) {
				auto sphere = m_pObject->GetChild<SpherePrefab>();
				auto kids = m_pObject->GetChildren<CubePrefab>();
				sphere->GetTransform()->Translate(0, 0, 0);

				auto comp{ kids[1]->GetComponent<TransformComponent>() };
				auto comp2{ kids[0]->GetComponent<TransformComponent>() };
				comp->Translate(32, 0, 0);
				comp2->Translate(-32, 0, 0);
			}
		});


	Wall = new CubePrefab(10,100,10, static_cast<XMFLOAT4>(Colors::Black));
	m_pObject3->AddChild(Wall);
	comp = Wall->GetComponent<TransformComponent>();
	comp->Translate(40, 0, 0);

	Wall->AddComponent(new RigidBodyComponent());
	Wall->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 5,50,5 }, *material);
	Wall->GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ 5,50,5 }, *material);
	Wall->GetComponent<RigidBodyComponent>()->SetKinematic(true);
	colliderInfo = Wall->GetComponent<RigidBodyComponent>()->GetCollider(0);
	colliderInfo.SetTrigger(true);

	Wall->SetOnTriggerCallBack([=](GameObject*, GameObject*, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER) {
				auto sphere = m_pObject->GetChild<SpherePrefab>();
				auto kids = m_pObject->GetChildren<CubePrefab>();
				sphere->GetTransform()->Translate(0, 0, 0);

				auto comp{ kids[1]->GetComponent<TransformComponent>() };
				auto comp2{ kids[0]->GetComponent<TransformComponent>() };
				comp->Translate(32, 0, 0);
				comp2->Translate(-32, 0, 0);
			}

		});
	#pragma endregion

	//sphere
	SpherePrefab* sphere{ new SpherePrefab(1,30,static_cast<XMFLOAT4>(Colors::Blue)) };
	sphere->AddComponent(new RigidBodyComponent());

	sphere->GetComponent<RigidBodyComponent>()->AddCollider(PxSphereGeometry{ 1 }, *material);
	sphere->GetComponent<RigidBodyComponent>()->SetKinematic(false);

	m_pObject->AddChild(sphere);

	//camera
	m_SceneContext.pCamera->UseOrthographicProjection();
	m_SceneContext.pCamera->SetOrthoSize(40);

	//input
	InputAction inp11{ 0, InputState::down, VK_W};
	InputAction inp12{ 1, InputState::down, VK_S};
	InputAction inp21{ 2, InputState::down, VK_I};
	InputAction inp22{ 3, InputState::down, VK_K};
	
	m_SceneContext.pInput->AddInputAction(inp11);
	m_SceneContext.pInput->AddInputAction(inp12);
	m_SceneContext.pInput->AddInputAction(inp21);
	m_SceneContext.pInput->AddInputAction(inp22);
}

void PongScene::Update()
{
	float deltaTime{ m_SceneContext.pGameTime->GetElapsed() };
	float speed{ 20 };

	auto kids = m_pObject->GetChildren<CubePrefab>();
	auto comp2{ kids[1]->GetComponent<TransformComponent>() };
	auto comp{ kids[0]->GetComponent<TransformComponent>() };
	if(m_SceneContext.pInput->IsActionTriggered(0)){
		auto pos{ comp->GetTransform()->GetPosition() };
		comp->Translate(pos.x, pos.y+deltaTime * speed, pos.z);
	}
	if(m_SceneContext.pInput->IsActionTriggered(1)){
		auto pos{ comp->GetTransform()->GetPosition() };
		comp->Translate(pos.x, pos.y-(deltaTime * speed), pos.z);
	}
	if(m_SceneContext.pInput->IsActionTriggered(2)){
		auto pos{ comp2->GetTransform()->GetPosition() };
		comp2->Translate(pos.x, pos.y+deltaTime * speed, pos.z);
	}
	if (m_SceneContext.pInput->IsActionTriggered(3)) {
		auto pos{ comp2->GetTransform()->GetPosition() };
		comp2->Translate(pos.x, pos.y-(deltaTime * speed), pos.z);
	}

	auto sphere = m_pObject->GetChild<SpherePrefab>();
	sphere->GetComponent<RigidBodyComponent>()->AddForce(XMFLOAT3{ m_isGoingLeft ? 5.f : -5.f, 9.81f + m_upSpeed, 0 });

}

void PongScene::Draw()
{
}

void PongScene::OnGUI()
{
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
	ImGui::Text("Left Controls: W and S", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
	ImGui::Text("Right Controls: I and K", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}
