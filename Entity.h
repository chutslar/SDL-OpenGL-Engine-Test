#pragma once

namespace ecs
{
	template <typename T>
	class Entity
	{
	private:
		unsigned long long ID;
	public:
		Entity(unsigned long long id) : ID(id) {};
		const unsigned long long getID() { return ID; }
	};
}
