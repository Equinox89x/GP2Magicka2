#include "stdafx.h"
#include "SpikyScene.h"
#include "Prefabs/SpherePrefab.h"


void SpikyScene::Initialize()
{
    //m_SceneContext.settings.showInfoOverlay = true;
   //m_SceneContext.settings.drawPhysXDebug = true;
    m_SceneContext.settings.drawGrid = false;
    m_SceneContext.settings.enableOnGUI = true;

    //model
    m_pObject = new GameObject();
    AddChild(m_pObject);

    //material
    m_pObject->AddComponent(new ModelComponent(L"Meshes/OctaSphere.ovm"));
    m_pObject->GetTransform()->Scale(XMFLOAT3{ 15.f,15.f,15.f });
    m_pObject->GetComponent<ModelComponent>()->SetMaterial(m_pMaterial);

    m_pMaterial = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();
    m_pObject->GetComponent<ModelComponent>()->SetMaterial(m_pMaterial);

    //ground
    GameSceneExt::CreatePhysXGroundPlane(*this);

}

void SpikyScene::Update()
{
}

void SpikyScene::OnGUI()
{
    m_pMaterial->DrawImGui();
}
