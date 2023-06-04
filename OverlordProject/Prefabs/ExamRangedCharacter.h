#pragma once
#include "ExamCharacter.h"
#include "ExamEnemy.h"
class Projectile;
class DiffuseMaterial_Shadow;

class ExamRangedCharacter : public ExamEnemy
{
public:
	ExamRangedCharacter(const CharacterDescExtended& characterDesc, XMFLOAT3 postitionOffset);
	~ExamRangedCharacter() override = default;

	ExamRangedCharacter(const ExamRangedCharacter& other) = delete;
	ExamRangedCharacter(ExamRangedCharacter&& other) noexcept = delete;
	ExamRangedCharacter& operator=(const ExamRangedCharacter& other) = delete;
	ExamRangedCharacter& operator=(ExamRangedCharacter&& other) noexcept = delete;

	void SetCanShoot(bool canShoot) { CanShoot = canShoot; };
	void FireProjectile();
	void SetProjectileHolder(GameObject* projectileHolder) { m_ProjectileHolder = projectileHolder; };

protected:
	void InitializeChild(const SceneContext&) override;
	void UpdateChild(const SceneContext&) override;

private:
	float ShootTimer, DefaultShootTimer{ 3.f };
	bool CanShoot{ false };

	DiffuseMaterial_Shadow* m_pMaterial{};
	PxMaterial* m_pDefaultMaterial{};
	std::vector<Projectile*> m_Projectiles;
	GameObject* m_ProjectileHolder;

	FMOD::System* SoundSystem;
	FMOD::Sound* ProjectileSound;
	FMOD::Channel* CurrentChannel;


};

