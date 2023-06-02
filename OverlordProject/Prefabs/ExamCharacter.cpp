#include "stdafx.h"
#include "ExamCharacter.h"
#include "../OverlordEngine/Prefabs/MagickaCamera.h"
#include "./Scenes/Exam/ExamTestClass.h"

ExamCharacter::ExamCharacter(const CharacterDescExtended& characterDesc) :
	m_CharacterDescExtended{ characterDesc },
	m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime),
	m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime)
{}

void ExamCharacter::Initialize(const SceneContext& /*sceneContext*/)
{
	//Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDescExtended.controller));

	//ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };
}

void ExamCharacter::Update(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.pGameTime->GetElapsed();

	//## Input Gathering (move, look, rotation)
	XMFLOAT2 look{ 0.f, 0.f };
	XMFLOAT3 newRot{ 0.f, 0.f, 0.f };
	constexpr float epsilon{ 0.01f };

	bool shiftPressed{ false };
	HandleInput(sceneContext, shiftPressed);

	ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };
	scene->ToggleUIElements(shiftPressed);

	HandleRotation(sceneContext, look, epsilon, newRot);

	HandleMove(sceneContext, newRot, epsilon);

	if (CanTakeDamage) {
		DamageTimer -= deltaTime;
		if (DamageTimer <= 0) {
			m_Health -= DamageToTake;
			std::cout << "Damage Taken: " << DamageToTake << ", Remaining Health: " << m_Health << "\n";
			DamageTimer = 1;
		}
	}
}

float MapRange(float value, float inputMin, float inputMax, float outputMin, float outputMax) {
	// Calculate the ratio of the input value within the input range
	float ratio = (value - inputMin) / (inputMax - inputMin);

	// Map the ratio to the output range
	float outputValue = outputMin + ratio * (outputMax - outputMin);

	return outputValue;
}
void ExamCharacter::HandleRotation(const SceneContext& sceneContext, DirectX::XMFLOAT2& look, const float& epsilon, DirectX::XMFLOAT3& newRot)
{
	//Only if the Left Mouse Button is Down >
	auto mousePos = sceneContext.pInput->GetMousePosition();
	auto movement = sceneContext.pInput->GetMouseMovement();

	auto originalRotation{ GetComponent<ModelComponent>()->GetTransform()->GetWorldRotation() };
	auto originalLocation{ GetComponent<ModelComponent>()->GetTransform()->GetWorldPosition() };

	float yaw{ 0 };
	if (sceneContext.pInput->IsGamepadConnected(GamepadIndex::playerOne)) {
		XMFLOAT2 look1, look2;
		look1 = sceneContext.pInput->GetThumbstickPosition(false); // right thumbstick
		look2 = sceneContext.pInput->GetThumbstickPosition(true); // left thumbstick
		// here we should change yaw to the input of the thumbstick
		if (abs(look2.x) > epsilon || abs(look2.y) > epsilon) {
			look = look2;
			yaw = -atan2(look.y, look.x);
		}
		if (abs(look1.x) > epsilon || abs(look1.y) > epsilon) {
			look = look1;
			yaw = -atan2(look.y, look.x);
		}
	}
	else{
		//rotate character to mouse pos
		//get position of mouse in world
		auto lookPosition{ sceneContext.pCamera->PickPosition() };
		//get rotation of character to mouse position in world
		XMVECTOR direction = XMVectorSubtract(XMLoadFloat3(&lookPosition), XMLoadFloat3(&originalLocation));
		yaw = std::atan2(XMVectorGetX(direction), XMVectorGetZ(direction));

	}

	//apply rotation to character
	newRot = XMFLOAT3{ originalRotation.x, yaw, originalRotation.z };
	GetComponent<ModelComponent>()->GetTransform()->Rotate(newRot, false);
	//}

}

void ExamCharacter::HandleMove(const SceneContext& sceneContext, DirectX::XMFLOAT3& /*newRot*/, const float& epsilon)
{
	//move character to mouse pos
	if (!sceneContext.pInput->IsGamepadConnected(GamepadIndex::playerOne)) {
		if (sceneContext.pInput->IsMouseButton(InputState::down, 1))
		{
			CalculateForwardMovement(epsilon);
		}
	}
	else {
		auto look = sceneContext.pInput->GetThumbstickPosition(true);
		if (abs(look.x) > epsilon || abs(look.y) > epsilon) {
			//when using controller, your left thumbstick defines rotation and moving
			//your right thumbstick defines rotation alone and takes priority over the rotation of the left stick.
			CalculateForwardMovement(epsilon);
		}
	}
}

void ExamCharacter::CalculateForwardMovement(const float& epsilon)
{
	//get forward vector
	XMFLOAT3 forward{ GetTransform()->GetForward() };
	m_CurrentDirection = forward;

	//calculate point to move to (based on forward vector coming from mouse rotation)
	XMFLOAT3 displacement;
	XMVECTOR displacementVec = XMVectorScale(XMLoadFloat3(&forward), 1.f);
	XMStoreFloat3(&displacement, displacementVec);
	GetComponent<ControllerComponent>()->Move(displacement, epsilon);
}

void ExamCharacter::HandleInput(const SceneContext& sceneContext, bool& shiftPressed)
{

	if (sceneContext.pInput->IsActionTriggered(m_CharacterDescExtended.actionId_SwitchElement)) {
		shiftPressed = true;
	}

	if (sceneContext.pInput->IsActionTriggered(m_CharacterDescExtended.actionId_ElementBottom)) {
		ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };
		scene->HandleCombobarFilling(shiftPressed ? ElementTypes::LIGHTNING : ElementTypes::WATER);
	}
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDescExtended.actionId_ElementLeft)) {
		ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };
		scene->HandleCombobarFilling(shiftPressed ? ElementTypes::EARTH : ElementTypes::SHIELD);
	}
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDescExtended.actionId_ElementTop)) {
		ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };
		scene->HandleCombobarFilling(shiftPressed ? ElementTypes::ARCANE : ElementTypes::LIFE);
	}
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDescExtended.actionId_ElementRight)) {
		ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };
		scene->HandleCombobarFilling(shiftPressed ? ElementTypes::FIRE : ElementTypes::COLD);
	}
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDescExtended.actionId_AoEAttack)) {
		ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };
		scene->ExecuteAOE();
	}
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDescExtended.actionId_SelfCast)) {
		ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };
		scene->ExecuteSelfCast();
	}
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDescExtended.actionId_SwordInput)) {
		ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };
		scene->ExecuteSword();
	}

	if (!sceneContext.pInput->IsGamepadConnected(GamepadIndex::playerOne)) {
		if (sceneContext.pInput->IsMouseButton(InputState::down, 2)) {
			ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };
			scene->ExecuteMagicCombo();
		}
		if (sceneContext.pInput->IsMouseButton(InputState::released, 2)) {
			ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };
			scene->FireProjectile();
			scene->ResetCombo();
		}
	}
	else {
		auto look = sceneContext.pInput->GetThumbstickPosition(false);
		ExamTestClass* scene{ reinterpret_cast<ExamTestClass*>(SceneManager::Get()->GetActiveScene()) };

		if (abs(look.x) > 0.01f || abs(look.y) > 0.01f) {
			scene->ExecuteMagicCombo();
			WasPressed = true;
		}
		else {
			if (WasPressed) {
				scene->ResetCombo();
				WasPressed = false;

			}
		}

	}
}

void ExamCharacter::DrawImGui()
{
	if (ImGui::CollapsingHeader("ExamCharacter"))
	{
		ImGui::Text(std::format("Move Speed: {:0.1f} m/s", m_MoveSpeed).c_str());
		ImGui::Text(std::format("Fall Speed: {:0.1f} m/s", m_TotalVelocity.y).c_str());

		ImGui::Text(std::format("Move Acceleration: {:0.1f} m/s2", m_MoveAcceleration).c_str());
		ImGui::Text(std::format("Fall Acceleration: {:0.1f} m/s2", m_FallAcceleration).c_str());

		const float jumpMaxTime = m_CharacterDescExtended.JumpSpeed / m_FallAcceleration;
		const float jumpMaxHeight = (m_CharacterDescExtended.JumpSpeed * jumpMaxTime) - (0.5f * (m_FallAcceleration * powf(jumpMaxTime, 2)));
		ImGui::Text(std::format("Jump Height: {:0.1f} m", jumpMaxHeight).c_str());

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Move Speed (m/s)", &m_CharacterDescExtended.maxMoveSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Move Acceleration Time (s)", &m_CharacterDescExtended.moveAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_MoveAcceleration = m_CharacterDescExtended.maxMoveSpeed / m_CharacterDescExtended.moveAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Fall Speed (m/s)", &m_CharacterDescExtended.maxFallSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Fall Acceleration Time (s)", &m_CharacterDescExtended.fallAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_FallAcceleration = m_CharacterDescExtended.maxFallSpeed / m_CharacterDescExtended.fallAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		ImGui::DragFloat("Jump Speed", &m_CharacterDescExtended.JumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
		ImGui::DragFloat("Rotation Speed (deg/s)", &m_CharacterDescExtended.rotationSpeed, 0.1f, 0.f, 0.f, "%.1f");

		/*bool isActive = m_pCameraComponent->IsActive();
		if (ImGui::Checkbox("ExamCharacter Camera", &isActive))
		{
			m_pCameraComponent->SetActive(isActive);
		}*/
	}
}

void ExamCharacter::DamagePlayer(bool canTakeDamage, float damage)
{
	CanTakeDamage = canTakeDamage;
	if (CanTakeDamage) {
		DamageToTake += damage;
	}
	else {
		DamageToTake -= damage;
		if (DamageToTake <= 0) DamageToTake = 0;
	}
}
