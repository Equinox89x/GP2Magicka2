//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "MinionScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include <Prefabs/TorusPrefab.h>


MinionScene::MinionScene() :
	GameScene(L"MinionScene"){}

void MinionScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	Logger::LogInfo(L"Welcome, humble Minion!");

	m_pObject = new GameObject();

	TorusPrefab* pref{ new TorusPrefab(10,30,1,25, static_cast<XMFLOAT4>(Colors::DarkRed))};
	TorusPrefab* pref2{ new TorusPrefab(7,30,1,25, static_cast<XMFLOAT4>(Colors::BlanchedAlmond))};
	TorusPrefab* pref3{ new TorusPrefab(4,30,1,25, static_cast<XMFLOAT4>(Colors::BlueViolet))};

	pref->AddComponent(new RigidBodyComponent(true));
	pref2->AddComponent(new RigidBodyComponent(true));
	pref3->AddComponent(new RigidBodyComponent(true));

	TransformComponent* comp{ pref->GetComponent<TransformComponent>() };
	TransformComponent* comp2{ pref2->GetComponent<TransformComponent>() };
	TransformComponent* comp3{ pref3->GetComponent<TransformComponent>() };

	comp->Rotate(45, 0, 0);
	comp2->Rotate(0, 45, 0);
	comp3->Rotate(45, 45, 0);

	comp->Translate(0, 20, 0);
	comp2->Translate(0, 20, 0);
	comp3->Translate(0, 20, 0);


	m_pObject->AddChild(pref);
	m_pObject->AddChild(pref2);
	m_pObject->AddChild(pref3);
	AddChild(m_pObject);
}

void MinionScene::Update()
{
	////Optional
	float deltaTime{ m_SceneContext.pGameTime->GetElapsed() };
	const float speed{ 1 };

	for (TorusPrefab* child : m_pObject->GetChildren<TorusPrefab>())
	{
		TransformComponent* comp{ child->GetComponent<TransformComponent>() };
		auto rot{ comp->GetRotation() };
		comp->Rotate(XMFLOAT3{ rot.x + deltaTime * speed, rot.y, rot.z });
	}

}

void MinionScene::Draw()
{
	//Optional
}

void MinionScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}
