#pragma once
class Camera
{
public:
	explicit Camera(float width, float height);
	void SetBoundaries(const Rectf& boundaries);

	Point2f GetPosition(const Rectf& track, float scale);
private:
	Point2f m_Pos;
	float m_Width;
	float m_Height;
	Rectf m_Boundaries;

	Point2f Track(const Rectf& toTrack, float scale);
	void Clamp(Point2f& bottomLeftPos, float scale);
};

