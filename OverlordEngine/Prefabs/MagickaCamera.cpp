#include "stdafx.h"
#include "MagickaCamera.h"

void MagickaCamera::Initialize(const SceneContext&)
{
	m_pCamera = new CameraComponent();
	AddComponent(m_pCamera);
}

void MagickaCamera::Update(const SceneContext& /*sceneContext*/)
{
	auto posStart{ m_LinePoints[0].points[0] };
	auto posEnd{ m_LinePoints[m_LinePoints.size() - 1].points[1] };

	auto pos1{ m_LinePoints[CurrentLineIndex].points[0] };
	auto pos2{ m_LinePoints[CurrentLineIndex].points[1] };

	//project on line
	XMFLOAT3 lineDir = { pos2.x - pos1.x, pos2.y - pos1.y, pos2.z - pos1.z };
	XMFLOAT3 p1ToPos1 = { PointToFollow.x - pos1.x, PointToFollow.y - pos1.y, PointToFollow.z - pos1.z };

	float dotProduct = lineDir.x * lineDir.x + lineDir.y * lineDir.y + lineDir.z * lineDir.z;
	float projection = (p1ToPos1.x * lineDir.x + p1ToPos1.y * lineDir.y + p1ToPos1.z * lineDir.z) / dotProduct;
	ProjectedPoint = XMFLOAT3{ pos1.x + (projection * lineDir.x), pos1.y + (projection * lineDir.y), pos1.z + (projection * lineDir.z) };
	ProjectedLinePoint = XMFLOAT3{ ProjectedPoint.x, pos1.y, ProjectedPoint.z };
	
	if (MathHelper::IsPointInCircle3D(ProjectedLinePoint, pos1, 0.5f)) {
		if (XMVector3Equal(XMLoadFloat3(&pos1), XMLoadFloat3(&posStart))) {
			IsInStart = true;
		}
		else {
			IsInStart = false;
			if (CanIncrease) {
				CurrentLineIndex--;
				CanIncrease = false;
			}
		}
	}
	else if (MathHelper::IsPointInCircle3D(ProjectedLinePoint, pos2, 0.5f)) {
		if (XMVector3Equal(XMLoadFloat3(&pos2), XMLoadFloat3(&posEnd))) {
			HasReachedEnd = true;
		}
		else {
			HasReachedEnd = false;
			if (CanIncrease) {
				CurrentLineIndex++;
				CanIncrease = false;
			}
		}
	}

	/*Sphere->GetTransform()->Translate(ProjectedLinePoint);*/
	auto newpos1{ m_LinePoints[CurrentLineIndex].points[0] };
	auto newpos2{ m_LinePoints[CurrentLineIndex].points[1] };

	if (MathHelper::CheckRange(ProjectedLinePoint.x, newpos1.x, newpos2.x) ||
		//newpos1.y < y && y < newpos2.y ||
		MathHelper::CheckRange(ProjectedLinePoint.z, newpos1.z, newpos2.z)) {
		CanIncrease = true;

	}
}

void MagickaCamera::InitPoints(const std::vector<Line>& linePoints)
{
	m_LinePoints = linePoints;
}

void MagickaCamera::CalculateStartPosition()
{
	//calculate new position
	auto p1{ m_LinePoints[0].points[0] };
	auto p2{ m_LinePoints[0].points[1] };
	auto point{ PointToFollow };

	XMFLOAT3 lineDir = { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
	XMFLOAT3 p1ToPos1 = { point.x - p1.x, point.y - p1.y, point.z - p1.z };

	float dotProduct = lineDir.x * lineDir.x + lineDir.y * lineDir.y + lineDir.z * lineDir.z;
	float projection = (p1ToPos1.x * lineDir.x + p1ToPos1.y * lineDir.y + p1ToPos1.z * lineDir.z) / dotProduct;

	XMFLOAT3 projectedPoint = { p1.x + (projection * lineDir.x),
		p1.y + (projection * lineDir.y),
		p1.z + (projection * lineDir.z) };

	XMFLOAT3 newPos{ projectedPoint.x, p1.y, projectedPoint.z };
	//Sphere->GetTransform()->Translate(newPos);
}

void MagickaCamera::Draw(const SceneContext&)
{
	for (size_t i = 0; i < m_LinePoints.size(); i++)
	{
		DebugRenderer::DrawLine(m_LinePoints[i].points[0], m_LinePoints[i].points[1], XMFLOAT4{1,0,0,1});
	}
}
