#include "System.h"


ecs::Entity<ecs::Drawable> ecs::System::addEntity(Drawable component, identity<Drawable>)
{
	drawables.push_back(component);
	return Entity<Drawable>(drawables.size()-1);
}

ecs::Drawable* ecs::System::getComponent(Entity<Drawable> entity)
{
	if (entity.getID() < drawables.size()) 
		return &drawables[entity.getID()];
	return nullptr;
}

void ecs::System::update(long long deltaTime_ms)
{

}

void ecs::System::draw(long long deltaTime_ms)
{
	for (auto &dc : drawables)
	{
		graphics.draw_image(dc.transform.position, dc.drawComponent.scale, dc.drawComponent.texture, dc.transform.rotation);
	}
}
