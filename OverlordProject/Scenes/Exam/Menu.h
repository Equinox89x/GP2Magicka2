#pragma once
#include <Prefabs/GameMenuPrefab.h>

class Menu : public GameScene
{
public:
	Menu() : GameScene(L"Menu") {};
	~Menu() override = default;
	Menu(const Menu& other) = delete;
	Menu(Menu&& other) noexcept = delete;
	Menu& operator=(const Menu& other) = delete;
	Menu& operator=(Menu&& other) noexcept = delete;
	void SetActiveMenu(MenuTypes menuType);

protected:
	void Initialize() override;

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

