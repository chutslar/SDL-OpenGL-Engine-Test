#pragma once

#include "Utility.h"

namespace camera
{
	class Camera
	{
	protected:
		glm::vec3 pos;
		glm::mat4 view;
		glm::mat4 projection;
		
	public:
		// Create new camera at position (default origin)
		Camera(glm::vec3 position = glm::vec3()) : pos(position) {};

		// Move camera to new position
		virtual void moveTo(const glm::vec3 position) { pos = position; }

		// Get camera position
		virtual const glm::vec3 getPosition() { return pos; }

		// Get camera view
		virtual const glm::mat4 getView() { return view; }

		// Get projection
		virtual const glm::mat4 getProjection() { return projection; }
	};

	class OrthographicCamera : public Camera
	{
	private:
		float w, h, viewDist;

		// Update projection due to change in variables
		void updateProjection();

	public:
		OrthographicCamera(float width, float height, float viewDistance, glm::vec3 position = glm::vec3(0, 0, 0.1));

		// Set the farthest distance at which objects can be seen
		void setViewDistance(float distance);

	};

	class PerspectiveCamera : public Camera
	{
	private:
		glm::vec3 dir;

		// Set new target
		void targetPosition(glm::vec3 targetPos);

	public:
		// Create new camera at position (default origin) facing direction (default forward)
		PerspectiveCamera(glm::vec3 position = glm::vec3(0, 0, 0.1), glm::vec3 direction = glm::vec3(0, 0, -1));
		
		// Get right normal vector
		const glm::vec3 right();

		// Get up normal vector
		const glm::vec3 up();

		void lookAt(glm::vec3 target);
	};
}
