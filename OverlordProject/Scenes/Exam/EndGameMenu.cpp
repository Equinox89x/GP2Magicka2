#include "stdafx.h"
#include "EndGameMenu.h"

void EndGameMenu::Initialize()
{
	MenuDesc menuDesc{ };
	menuDesc.actionId_MenuDown = MenuDown;
	menuDesc.actionId_MenuUp = MenuUp;
	menuDesc.actionId_Select = Select;
	menuDesc.actionId_Exit = Exit;
	auto go{ new GameMenuPrefab(menuDesc, MenuTypes::EndMenu) };
	m_pMenu = AddChild(go);

	auto inputAction = InputAction(Select, InputState::down, VK_SPACE, -1, XINPUT_GAMEPAD_X);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MenuUp, InputState::down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MenuDown, InputState::down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Exit, InputState::down, VK_ESCAPE, -1, XINPUT_GAMEPAD_START);
	m_SceneContext.pInput->AddInputAction(inputAction);
}

void EndGameMenu::SetActiveMenu(MenuTypes menuType)
{
	m_pMenu->SetActiveMenu(menuType);
}