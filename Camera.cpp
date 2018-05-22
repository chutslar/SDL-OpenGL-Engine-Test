#include "Camera.h"

camera::PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 direction) : Camera(position), dir(direction)
{
	
}

const glm::vec3 camera::PerspectiveCamera::right()
{
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	return glm::normalize(glm::cross(up, dir));
}

const glm::vec3 camera::PerspectiveCamera::up()
{
	return glm::cross(dir, right());
}

void camera::PerspectiveCamera::lookAt(glm::vec3 target)
{
	view = glm::lookAt(pos, target, up());
}

void camera::PerspectiveCamera::targetPosition(glm::vec3 targetPos)
{
	dir = glm::normalize(pos - targetPos);
}

void camera::OrthographicCamera::updateProjection()
{
	float midX = w / 2.f;
	float midY = h / 2.f;
	projection = glm::ortho(pos.x - midX, pos.x + midX, pos.y - midY, pos.y + midY, 0.1f, viewDist);
}

camera::OrthographicCamera::OrthographicCamera(float width, float height, float viewDistance, glm::vec3 position) : 
	Camera(position), w(width), h(height), viewDist(viewDistance)
{
}

void camera::OrthographicCamera::setViewDistance(float distance)
{ 
	if (viewDist > 0)
	{
		viewDist = distance;
		updateProjection();
	}
}
