#include "stdafx.h"
#include "MagickaCamera.h"

void MagickaCamera::Initialize(const SceneContext&)
{
	AddComponent(new CameraComponent());
}

void MagickaCamera::InitPoints(std::vector<Line> linePoints)
{
	m_LinePoints = linePoints;
}

void MagickaCamera::Draw(const SceneContext&)
{
	for (size_t i = 0; i < m_LinePoints.size(); i++)
	{
		DebugRenderer::DrawLine(m_LinePoints[i].points[0], m_LinePoints[i].points[1], XMFLOAT4{1,0,0,1});
	}
}
