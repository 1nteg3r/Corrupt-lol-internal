#include "pch.h"
#include "renderer.h"
#include "overlay.h"



namespace Visuals {
	Object* me{};
	float AttackRange;
	float BR;


	void initialize() {
		me = *(Object**)offsets.oLocalPlayer;
		BR = Functions::GetRadius(me);
	}

	void range()
	{
		overlay.Circle(me->position(), me->attackrange() + BR, ImColor(1.f, 1.f, 1.f, 0.4f));
	}

	
}
