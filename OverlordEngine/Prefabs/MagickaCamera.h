#pragma once
struct Line {
	std::vector<XMFLOAT3> points;
};

class MagickaCamera final : public GameObject
{
public:
	MagickaCamera() = default;
	~MagickaCamera() override = default;
	MagickaCamera(const MagickaCamera& other) = delete;
	MagickaCamera(MagickaCamera&& other) noexcept = delete;
	MagickaCamera& operator=(const MagickaCamera& other) = delete;
	MagickaCamera& operator=(MagickaCamera&& other) noexcept = delete;



	void Draw(const SceneContext& sceneContext) override;
	std::vector<Line> m_LinePoints;

	void InitPoints(std::vector<Line> linePoints);

protected:
	void Initialize(const SceneContext&) override;

private:

	float m_TotalPitch{}, m_TotalYaw{};
	float m_MoveSpeed{ 10.f }, m_RotationSpeed{ 10.f }, m_SpeedMultiplier{ 10.f };
	CameraComponent* m_pCamera{};

};
