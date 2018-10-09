#include "stdafx.h"
#include "Camera.h"

Camera::Camera(float width, float height)
	: m_Width{width}
	, m_Height(height)
	, m_Boundaries{0,0,width,height}
{
}

void Camera::SetBoundaries(const Rectf& boundaries) {
	m_Boundaries = boundaries;
}

Point2f Camera::Track(const Rectf& toTrack, float scale) {
	float left =toTrack.left + toTrack.width / 2 - m_Width / (2 * scale);
	float bottom = toTrack.bottom + toTrack.height / 2 - m_Height / (2 * scale);
	return Point2f{left, bottom};
}

void Camera::Clamp(Point2f& bottomLeftPos, float scale) {
	if (bottomLeftPos.x < m_Boundaries.left) 
		bottomLeftPos.x = m_Boundaries.left;
	if (bottomLeftPos.x > m_Boundaries.left + m_Boundaries.width - m_Width / scale)
		bottomLeftPos.x = m_Boundaries.left + m_Boundaries.width - m_Width / scale;
	if (bottomLeftPos.y < m_Boundaries.bottom)
		bottomLeftPos.y = m_Boundaries.bottom;
	if (bottomLeftPos.y > m_Boundaries.bottom + m_Boundaries.height - m_Height / scale)
		bottomLeftPos.y = m_Boundaries.bottom + m_Boundaries.height - m_Height / scale;
}

Point2f Camera::GetPosition(const Rectf& track, float scale) {
	m_Pos = Track(track, scale);
	Clamp(m_Pos, scale);
	return m_Pos;
}