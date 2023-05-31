#pragma once
#include <queue>
#include <Materials/DiffuseMaterial.h>
#include <Prefabs/MagickaCamera.h>
#include "Prefabs/SpherePrefab.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/TorusPrefab.h"

class ExamCharacter;
class EnemyMeleeCharacter;

#pragma region enums & structs
enum ElementTypes : char
{
	WATER = 'W',
	LIGHTNING = 'L',
	LIFE = 'Y',
	ARCANE = 'A',
	SHIELD = 'S',
	EARTH = 'E',
	COLD = 'C',
	FIRE = 'F',

	STEAM = 'M',
	ICE = 'I',
	POISON = 'P',
	NONE = 'N'
};

enum class ProjectileTypes {
	SPRAY,
	BEAM,
	PROJECTILE,
	PROJECTILEBOMB,
	SHIELD,
	AOE,
};

enum class Spells {
	PUSH,
	EMERGENCYTELEPORT,
	DISPEL,
	RANDOMSPELL,
	TELEPORT,
	HASTE,
	DISRUPTOR,
	CONCUSSION,
	THUNDERBOLT,
	SPIKEQUAKE,
	ICETORNADO,
	BREEZE,
	GUARDIAN,
	DRAGONSTRIKE,
	FOWL,
	THUNDERHEAD,
	SUMMON,
	SACRIFICE,
	REVIVE
};

enum class SolidType {
	SOLID,
	NOTSOLID
};

struct ResultingMagic {
	ProjectileTypes ProjectileType;
	SolidType SolidType;
	std::vector<ElementTypes> Modifiers;
	std::vector<std::wstring> TextureName;
	std::vector<std::wstring> AOETextureName;
	float Damage;
};

struct MagicTypes {
	MagicTypes(ElementTypes baseType, ProjectileTypes MagicType, float Damage) :
		BaseType{ baseType },
		ProjectileType{ MagicType },
		Damage{ Damage }
	{
		AoEDamage = Damage * 2;
	}

	ProjectileTypes ProjectileType{};
	ElementTypes BaseType{};
	float Damage{ 0 };
	float AoEDamage{ 0 };
};

struct MagicSpell {
	MagicSpell(std::deque<ElementTypes> magics, Spells spell) :
		Magics{ magics },
		Spell{ spell }
	{
		CreatePatternString();
	}

	std::deque<ElementTypes> Magics{};
	Spells Spell{};
	std::string Pattern;

	void CreatePatternString() {
		for (auto magic : Magics) {
			Pattern += magic;
		}
	}
};

struct Magic {
	Magic() :
		ElementType{ ElementTypes::ARCANE },
		CancellingMagicTypes{}
	{}

	Magic(ElementTypes elementType, MagicTypes defaultMagicType, std::vector<ElementTypes> cancellingMagicTypes, std::vector<ElementTypes> combiningMagicTypes = std::vector<ElementTypes>{}) :
		ElementType{ elementType },
		CancellingMagicTypes{ cancellingMagicTypes },
		CombiningMagicTypes{ combiningMagicTypes },
		DefaultMagicType{ defaultMagicType }
	{
		AssignName();
	}

	std::wstring TextureName{};
	std::wstring UITextureName{};
	std::wstring AOETextureName{};
	ElementTypes ElementType{ ElementTypes::ARCANE };
	std::vector<ElementTypes> CombiningMagicTypes{ };
	std::vector<ElementTypes> CancellingMagicTypes{ };
	std::vector<MagicTypes> MagicType{ };
	MagicTypes DefaultMagicType{ MagicTypes{ElementTypes::NONE, ProjectileTypes::BEAM, 0} };

	void AssignName() {
		TextureName = L"Textures/";
		UITextureName = L"Textures/Magicka/Element_";
		AOETextureName = L"Textures/Magicka/Element_aoe_";
		switch (ElementType)
		{
		case WATER:
			TextureName += L"Water.png";
			UITextureName += L"Water.png";
			AOETextureName += L"Water.png";
			break;
		case LIGHTNING:
			TextureName += L"Lightning.png";
			UITextureName += L"lightning.png";
			AOETextureName += L"lightning.png";
			break;
		case LIFE:
			UITextureName += L"life.png";
			AOETextureName += L"life.png";
			break;
		case ARCANE:
			UITextureName += L"arcane.png";
			AOETextureName += L"arcane.png";
			break;
		case SHIELD:
			UITextureName += L"shield.png";
			AOETextureName += L"shield.png";
			break;
		case EARTH:
			UITextureName += L"earth.png";
			AOETextureName += L"earth.png";
			break;
		case COLD:
			TextureName += L"Cold.png";
			UITextureName += L"cold.png";
			AOETextureName += L"cold.png";
			break;
		case FIRE:
			TextureName += L"FireBall.png";
			UITextureName += L"fire.png";
			AOETextureName += L"fire.png";
			break;
		case STEAM:
			TextureName += L"Smoke.png";
			UITextureName += L"steam.png";
			AOETextureName += L"steam.png";
			break;
		case ICE:
			UITextureName += L"ice.png";
			AOETextureName += L"ice.png";
			break;
		case POISON:
			TextureName += L"Poison.png";
			UITextureName += L"poison.png";
			AOETextureName += L"poison.png";
			break;
		case NONE:
			break;
		default:
			break;
		}
	}
};
#pragma endregion

class ExamTestClass : public GameScene
{
public:
	ExamTestClass();
	~ExamTestClass() override = default;
	ExamTestClass(const ExamTestClass& other) = delete;
	ExamTestClass(ExamTestClass&& other) noexcept = delete;
	ExamTestClass& operator=(const ExamTestClass& other) = delete;
	ExamTestClass& operator=(ExamTestClass&& other) noexcept = delete;

	void HandleCombobarFilling(ElementTypes elementType);
	void ResetCombo();
	void ToggleUIElements(bool shiftPressed);

	void ExecuteMagicCombo();
	void ExecuteAOE();
	void ExecuteSelfCast();
	void ExecuteSword();


protected:
	void Initialize() override;
	void SetStartPos();
	void OnGUI() override;
	void Update() override;
	void HandleMagicTransform();
	void HandleCameraMovement();
	void Draw() override;
	void PostDraw() override;

private:
	//setup
	PxMaterial* m_Material{};
	XMFLOAT3 m_Size{ 10,10, 10 };
	ExamCharacter* m_pCharacter{};
	std::vector<EnemyMeleeCharacter*> m_pMeleeEnemies{ };
	GameObject* m_pSprayMagicEmitter{};
	GameObject* m_pBeamMagicEmitter{ nullptr };
	GameObject* m_pAOEMagicEmitter{ nullptr };
	GameObject* m_pShieldMagicEmitter{ nullptr };
	GameObject* m_pLevel{ nullptr };
	GameObject* m_pUI{ nullptr };
	GameObject* m_pUI2{ nullptr };
	GameObject* m_pUI3{ nullptr };
	DiffuseMaterial* m_pMaterial{};
	DiffuseMaterial* m_pLevelMaterial{};
	std::vector<std::wstring> m_Textures{};
	std::vector<std::wstring> m_TransparentTextures{};
	ResultingMagic MagicResult{};

	//Magic information
	//Water must always be FIRST in the vector of combined magics
	std::vector<Magic> m_CombinedMagics{ };
	std::vector<Magic> m_Magics{ };
	std::vector<MagicSpell> m_MagicSpells{ };
	std::vector<MagicTypes> m_MagicTypes{ };

	//combo information
	std::vector<Magic> m_ComboBar{};
	std::string m_ComboPattern;
	std::string m_PatternOptions;

	SpherePrefab* Sphere;
	CameraComponent* m_pCameraComponent{};
	MagickaCamera* m_pCamera{};
	int currentLineIndex{ 0 };
	bool CanIncrease{ true };
	bool IsExecutingMagic{ true };

	//input
	enum InputIds
	{
		ElementBottom = 0,
		ElementLeft = 1,
		ElementTop = 2,
		ElementRight = 3,
		Elementcombo5 = 4,
		SwitchElement = 5,
		CharacterMoveLeft = 6,
		CharacterMoveRight = 7,
		CharacterMoveForward = 8,
		CharacterMoveBackward = 9,
		CharacterJump = 10,
		Move = 11,
		Execute = 12,
		SwordInput = 13,
		SelfCast = 14,
		AoEAttack = 15,
	};

	//setup helpers
	void CreateLevel();
	void CreateInput();
	void CreateCharacter();
	void CreateMagic();
	void CreateEmitters();
	void CreateUI();

	//magic helper functions
	Magic* GetCombinedMagicType(ElementTypes elementType1, ElementTypes elementType2);
	bool IsMagicCancelled(int currentIterator, const Magic* currentMagic, bool& foundCanceller);

	//debug
	void HandlePrint(const ElementTypes currentMagic) const;
	void HandlePrint(const char currentMagic) const;
	void HandlePrint2(const Spells spell) const;

	//helpers
	double Distance3D(XMFLOAT3 p1, XMFLOAT3 p2);
	bool IsPointInCircle3D(XMFLOAT3 point, XMFLOAT3 circleCenter, double circleRadius);
	bool CheckRange(double value, double lowerBound, double upperBound);

	bool IsPointNearViewport(const XMFLOAT3& point, float threshold);
	bool IsPointOnLine(const XMFLOAT3& point, const XMFLOAT3& linePoint1, const XMFLOAT3& linePoint2, float epsilon);


};
