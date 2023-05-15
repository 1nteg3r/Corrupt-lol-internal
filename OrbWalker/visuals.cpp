#include "pch.h"
#include "renderer.h"
#include "overlay.h"



namespace Visuals {
	Object* me{};
	float AttackRange;
	float BR;
	ObjList* heroes{};
	ObjList* turrets{};

	void initialize() {
		me = *(Object**)offsets.oLocalPlayer;
		BR = Functions::GetRadius(me);
		heroes = *(ObjList**)offsets.oHeroList;
		turrets = *(ObjList**)offsets.oTurretList;
	}
	auto GetObjectList(const ObjList* const list) {
		return std::span(*(Object***)((uintptr_t)list + 0x8), *(int*)((uintptr_t)list + 0x10));
	}
	void range()
	{
		auto turretlist = GetObjectList(turrets);
		if (Functions::IsAlive(me))
		{
			overlay.Circle(me->position(), me->attackrange() + BR, ImColor(0.f, 1.f, 0.f, 1.f));
		}
		for (auto turret : turretlist) {
			if (turret->EnemyTo(me))
				overlay.Circle(turret->position(), 875, ImColor(1.f, 0.f, 0.f, 1.f));
		}
	}
	
	void drawCircles() {
		auto herolist = GetObjectList(heroes);
		
		for (auto hero : herolist) {
			if(hero->AttackableFor(me) && Functions::IsAlive(hero) && me->InRangeOf(hero))
				overlay.Circle(hero->position(), hero->attackrange()+ BR, ImColor(1.f, 0.f, 0.f, 1.f));
			else if (hero->AttackableFor(me) && Functions::IsAlive(hero) && !me->InRangeOf(hero))
				overlay.Circle(hero->position(), hero->attackrange() + BR, ImColor(1.f, 1.f, 0.f, 1.f));
		}
		
	}

	
}
