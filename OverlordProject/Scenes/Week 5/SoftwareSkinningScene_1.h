#pragma once
#include <Prefabs/BoneObject.h>
class SoftwareSkinningScene_1 : public GameScene
{

public:
	SoftwareSkinningScene_1() : GameScene(L"SoftwareSkinningScene_1") {};
	~SoftwareSkinningScene_1() override = default;

	SoftwareSkinningScene_1(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1(SoftwareSkinningScene_1&& other) noexcept = delete;
	SoftwareSkinningScene_1& operator=(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1& operator=(SoftwareSkinningScene_1&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	BoneObject* m_pBone0{}, * m_pBone1{};

	float m_BoneRotation{5};
	float m_BoneRotation2{5};
	int m_RotationSign{ 1 };
	bool m_CanRotate{ 1 };
};

