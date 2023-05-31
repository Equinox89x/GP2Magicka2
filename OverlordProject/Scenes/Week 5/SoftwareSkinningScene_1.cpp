#include "stdafx.h"
#include <Materials/ColorMaterial.h>
#include "SoftwareSkinningScene_1.h"

void SoftwareSkinningScene_1::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	ColorMaterial* color = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	color->SetColor(static_cast<XMFLOAT4>(Colors::CadetBlue));

	GameObject* pRoot{ new GameObject() };

	m_pBone0 = new BoneObject{ color, 15.f };
	pRoot->AddChild(m_pBone0);

	m_pBone1 = new BoneObject{ color, 15.f };
	m_pBone0->AddBone(m_pBone1);

	AddChild(pRoot);

}

void SoftwareSkinningScene_1::Update()
{
	if (m_CanRotate) {
		float rot{ 45 * GetSceneContext().pGameTime->GetElapsed() };
		m_RotationSign == 1 ? m_BoneRotation += rot : m_BoneRotation -= rot;

		if (m_BoneRotation > 45) m_RotationSign *= -1;
		if (m_BoneRotation < -45) m_RotationSign *= -1;
	}

	m_pBone0->GetTransform()->Rotate(0, 0, m_BoneRotation);
	m_pBone1->GetTransform()->Rotate(0, 0, m_CanRotate  ? -m_BoneRotation * 2.f : m_BoneRotation2);
}

void SoftwareSkinningScene_1::OnGUI()
{
	ImGui::Checkbox("Auto Rotate", &m_CanRotate);
	ImGui::DragFloat("Rotation", &m_BoneRotation, 1.f, -45.f, 45.f, "%.3f", 1.f);
	ImGui::DragFloat("Rotation2", &m_BoneRotation2, 1.f, -45.f, 45.f, "%.3f", 1.f);
}
