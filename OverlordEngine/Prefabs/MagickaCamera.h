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

	void SetFollowPoint(XMFLOAT3 followPoint) { PointToFollow = followPoint; };
	XMFLOAT3 GetPointToFollow() { return PointToFollow; };
	XMFLOAT3 GetProjectedLinePoint() { return ProjectedLinePoint; };
	XMFLOAT3 GetProjectedPoint() { return ProjectedPoint; };

	void Draw(const SceneContext& sceneContext) override;

	void InitPoints(const std::vector<Line>& linePoints);
	void CalculateStartPosition();
	std::vector<Line> m_LinePoints;
	bool GetReachedEnd() { return HasReachedEnd; };
	void SetReachedEnd(bool hasReachedEnd) { HasReachedEnd = hasReachedEnd; };
	bool GetIsInStart() { return IsInStart; };

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:

	float m_TotalPitch{}, m_TotalYaw{};
	float m_MoveSpeed{ 10.f }, m_RotationSpeed{ 10.f }, m_SpeedMultiplier{ 10.f };
	CameraComponent* m_pCamera{};
	int CurrentLineIndex{ 0 };
	bool CanIncrease{ false }, HasReachedEnd{ false }, IsInStart{ false };
	XMFLOAT3 PointToFollow{};
	XMFLOAT3 ProjectedLinePoint{};
	XMFLOAT3 ProjectedPoint{};
};
