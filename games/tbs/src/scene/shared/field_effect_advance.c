/* Babi Lighthouse, Crossbone Isle, Tret's rooms, the Tunnel Ruins and Venus
   Lighthouse, and the main image's Carry (script_commands_whirlwind.c, with
   EFFECT_SPAWN_RESIDENT), whose landing ring of particles it moves. It is
   static in each overlay and global in the main image. */

#ifdef EFFECT_SPAWN_RESIDENT
#define EFFECT_ADVANCE_LINKAGE
#else
#define EFFECT_ADVANCE_LINKAGE static
#endif

/* A drifting effect loses a FIELD_EFFECT_DRAG_X-th of its x speed and a
   FIELD_EFFECT_DRAG_Z-th of its z speed each tick. */
#define FIELD_EFFECT_DRAG_X 18
#define FIELD_EFFECT_DRAG_Z 16

/* An effect's update function: move by its velocity, slowing in x and z, and
   grow and spin at its rates. */
EFFECT_ADVANCE_LINKAGE void scFieldEffectAdvanceMotion(ACTOR *actor)
{
	FIELD_EFFECT *effect = ((FIELD_EFFECT *)actor);

	effect->x += effect->vel.x;
	effect->y += effect->vel.y;
	effect->z += effect->vel.z;
	effect->vel.x -= effect->vel.x / FIELD_EFFECT_DRAG_X;
	effect->vel.z -= effect->vel.z / FIELD_EFFECT_DRAG_Z;
	effect->scaleX += effect->scaleRateX;
	effect->scaleY += effect->scaleRateY;
	effect->spr->rotation += effect->spin;
}

#undef EFFECT_ADVANCE_LINKAGE
#undef FIELD_EFFECT_DRAG_X
#undef FIELD_EFFECT_DRAG_Z
