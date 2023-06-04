#pragma once
#include <Prefabs/GameMenuPrefab.h>

class GameMenu : public GameScene
{
public:
	GameMenu() : GameScene(L"GameMenu") {};
	~GameMenu() override = default;
	GameMenu(const GameMenu& other) = delete;
	GameMenu(GameMenu&& other) noexcept = delete;
	GameMenu& operator=(const GameMenu& other) = delete;
	GameMenu& operator=(GameMenu&& other) noexcept = delete;
	void SetActiveMenu(MenuTypes menuType);

protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
	void Draw() override;
	void PostDraw() override;

private:
	enum MenuInputIds
	{
		Select = 0,
		MenuUp = 1,
		MenuDown = 2,
		Exit = 3,
	};

	GameMenuPrefab* m_pMenu{ nullptr };
};

