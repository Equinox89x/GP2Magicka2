#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial() : Material<UberMaterial>(L"Effects/GP2_W3X-2_UberShader_START.fx")
{
}



void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile, const std::wstring& assetFile2)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile2);
	SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture);
	SetVariable_Texture(L"gTextureNormal", m_pNormalTexture);

}

void UberMaterial::InitializeEffectVariables()
{
	SetVariable_Scalar(L"gUseTextureDiffuse", true);
	SetVariable_Vector(L"gColorDiffuse", { 0.9f,0,0,1 });

	SetVariable_Vector(L"gColorSpecular", { 1,1,1,1 });
	SetVariable_Scalar(L"gUseTextureSpecularIntensity", false);
	SetVariable_Scalar(L"gShininess", 40);
	SetVariable_Scalar(L"gUseSpecularBlinn", true);
	SetVariable_Scalar(L"gUseSpecularPhong", true);

	SetVariable_Vector(L"gColorAmbient", { 0.842f, 0.556f, 0.387f, 1.f });
	SetVariable_Scalar(L"gAmbientIntensity", 0);

	SetVariable_Scalar(L"gFlipGreenChannel", false);
	SetVariable_Scalar(L"gUseTextureNormal", true);

	SetVariable_Scalar(L"gUseEnvironmentMapping", false);
	SetVariable_Scalar(L"gReflectionStrength", 0.6f);
	SetVariable_Scalar(L"gRefractionStrength", 0.1f);
	SetVariable_Scalar(L"gRefractionIndex", 0.3f);

	SetVariable_Scalar(L"gUseFresnelFalloff", true);
	SetVariable_Scalar(L"gFresnelPower", 15.f);
	SetVariable_Scalar(L"gFresnelHardness", 6.8f);
	SetVariable_Scalar(L"gFresnelMultiplier", 3.9f);
	SetVariable_Vector(L"gColorFresnel", {1, 0.01f, 0.8f});
}
