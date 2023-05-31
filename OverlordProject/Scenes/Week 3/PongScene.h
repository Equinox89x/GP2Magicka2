#pragma once
class PongScene final : public GameScene
{
public:
	PongScene();
	~PongScene() override = default;

	PongScene(const PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	GameObject* m_pObject{};
	RigidBodyComponent* m_pRigidBody{};
	FreeCamera* m_pFixedCamera{};
	GameObject* m_pObject2{};
	GameObject* m_pObject3{};
	bool m_isGoingLeft{ false };
	float m_upSpeed{ 18 };
};


