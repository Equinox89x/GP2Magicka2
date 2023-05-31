#include "stdafx.h"
#include "UberMaterialScene.h"
#include <Materials/UberMaterial.h>

void UberMaterialScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	m_pMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	m_pMaterial->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga", L"Textures/Skulls_Normalmap.tga");

	m_pObject = new GameObject();
	AddChild(m_pObject);

	m_pObject->AddComponent(new ModelComponent(L"Meshes/Sphere.ovm"));
	m_pObject->GetComponent<ModelComponent>()->SetMaterial(m_pMaterial);
	m_pObject->GetTransform()->Scale(10);

}

void UberMaterialScene::Update()
{
}

void UberMaterialScene::Draw()
{
}

void UberMaterialScene::OnGUI()
{
	m_pMaterial->DrawImGui();
}
