#include "stdafx.h"
#include "ModelTestScene.h"
#include "Prefabs/CubePrefab.h"

void ModelTestScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	//auto material = PxGetPhysics().createMaterial(.5f, .5f, .1f);
	GameSceneExt::CreatePhysXGroundPlane(*this);

	//chair
	m_pChair = new GameObject();
	AddChild(m_pChair);

	//material
	//m_pMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pMaterial->SetDiffuseTexture(L"Textures/Chair_Dark.dds");

	m_pChair->AddComponent(new ModelComponent(L"Meshes/chair.ovm"));
	m_pChair->GetComponent<ModelComponent>()->SetMaterial(m_pMaterial);


	//const PxConvexMesh* mesh{ ContentManager::Load<PxConvexMesh>(L"Meshes/chair.ovpc") };
	//mesh->getNbVertices();
	m_pChair->AddComponent(new RigidBodyComponent());
	//m_pChair->GetComponent<RigidBodyComponent>()->AddCollider(mesh, *material);

}

void ModelTestScene::Update()
{
}

void ModelTestScene::Draw()
{
}

void ModelTestScene::OnGUI()
{
	m_pMaterial->DrawImGui();
}
