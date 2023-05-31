#pragma once
#include <Prefabs/BoneObject.h>
class SoftwareSkinningScene_2 final : public GameScene
{
public:
	SoftwareSkinningScene_2() : GameScene(L"SoftwareSkinningScene_2") {};
	~SoftwareSkinningScene_2() override = default;

	SoftwareSkinningScene_2(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2(SoftwareSkinningScene_2&& other) noexcept = delete;
	SoftwareSkinningScene_2& operator=(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2& operator=(SoftwareSkinningScene_2&& other) noexcept = delete;

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
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color) :
			transfornedVertex{ position, normal, color },
			originalVertex{ position, normal, color } 
		{}

		VertexPosNormCol transfornedVertex{};
		VertexPosNormCol originalVertex{};
	};

	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};


};

