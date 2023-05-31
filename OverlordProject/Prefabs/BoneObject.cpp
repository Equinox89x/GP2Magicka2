#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length):
	m_pMaterial{pMaterial},
	m_Length{length}
{
	Initialize(GetScene()->GetSceneContext());
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(m_Length, 0, 0);
	AddChild(pBone);
}


void BoneObject::Initialize(const SceneContext&)
{
	GameObject* pEmpty{ new GameObject() };
	AddChild(pEmpty);

	ModelComponent* pModel{ new ModelComponent(L"Meshes/Bone.ovm") };
	pModel->SetMaterial(m_pMaterial);
	pEmpty->AddComponent(pModel);

	pEmpty->GetTransform()->Rotate(0, -90, 0, true);
	pEmpty->GetTransform()->Scale(m_Length);
}

void BoneObject::CalculateBindPose()
{
	const auto world{ GetTransform()->GetWorld() };
	auto determinant{ XMVECTOR{ 0,0,0 } };

	XMFLOAT4X4 worldMatrixInverse;
	auto matrixInverse{ XMMatrixInverse(&determinant, DirectX::XMLoadFloat4x4(&world)) };
	DirectX::XMStoreFloat4x4(&worldMatrixInverse, matrixInverse);

	m_BindPose = worldMatrixInverse;
}