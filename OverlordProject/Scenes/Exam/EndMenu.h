#pragma once
#include <Prefabs/GameMenuPrefab.h>

class EndMenu : public GameScene
{
public:
	EndMenu() : GameScene(L"EndMenu") {};
	~EndMenu() override = default;
	EndMenu(const EndMenu& other) = delete;
	EndMenu(EndMenu&& other) noexcept = delete;
	EndMenu& operator=(const EndMenu& other) = delete;
	EndMenu& operator=(EndMenu&& other) noexcept = delete;
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

