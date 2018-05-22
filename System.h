#pragma once
#include <map>
#include "Graphics.h"
#include "Component.h"
#include "Entity.h"
#include "Utility.h"

namespace ecs
{

	class System
	{
	private:
		graphics::GraphicsManager &graphics;
		std::vector<Drawable> drawables;
	
		template<typename T>
		struct identity { typedef T type; };
	
	public:
		System(graphics::GraphicsManager &ngm) : graphics(ngm) {};

		// Create a new entity handle and return it -- an Entity 0 is null
		template <typename T>
		Entity<T> addEntity(T component) { return addEntity(component, identity<T>()); }

		Entity<Drawable> addEntity(Drawable Component, identity<Drawable>);

		Drawable* getComponent(Entity<Drawable> entity);

		void update(long long deltaTime_ms);
		void draw(long long deltaTime_ms);
	};
}
