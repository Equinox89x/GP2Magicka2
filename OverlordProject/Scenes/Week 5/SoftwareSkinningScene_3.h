#pragma once
#include <Prefabs/BoneObject.h>
class SoftwareSkinningScene_3 final : public GameScene
{
public:
	SoftwareSkinningScene_3() : GameScene(L"SoftwareSkinningScene_3") {};
	~SoftwareSkinningScene_3() override = default;

	SoftwareSkinningScene_3(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3(SoftwareSkinningScene_3&& other) noexcept = delete;
	SoftwareSkinningScene_3& operator=(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3& operator=(SoftwareSkinningScene_3&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	BoneObject* m_pBone0{}, * m_pBone1{};
	float m_BoneRotation{ 0 };
	float m_BoneRotation2{ 0 };
	int m_RotationSign{ 1 };
	bool m_CanRotate{ true };

	struct VertexSoftwareSkinned {
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color, float weight0, float weight1) :
			transfornedVertex{ position, normal, color },
			originalVertex{ position, normal, color },
			blendWeight0{weight0},
			blendWeight1{weight1}

		{}

		VertexPosNormCol transfornedVertex{};
		VertexPosNormCol originalVertex{};

		float blendWeight0{};
		float blendWeight1{};
	};

	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};
};

