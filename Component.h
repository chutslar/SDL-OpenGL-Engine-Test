#pragma once
#include "Image.h"
#include "Utility.h"

namespace ecs
{
	struct Transform2DComponent
	{
		glm::vec3 position;
		float rotation;
	};

	struct DrawComponent
	{
		glm::vec3 scale;
		image::Texture *texture;
	};

	template <typename... Components>
	struct ComponentBucket;

	template <>
	struct ComponentBucket<Transform2DComponent> {
		Transform2DComponent transform;
	};

	template <>
	struct ComponentBucket<Transform2DComponent, DrawComponent> {
		Transform2DComponent transform;
		DrawComponent drawComponent;
	};

	using Object = ComponentBucket<Transform2DComponent>;
	using Drawable = ComponentBucket<Transform2DComponent, DrawComponent>;
}