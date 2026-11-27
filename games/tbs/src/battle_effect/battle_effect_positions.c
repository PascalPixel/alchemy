/* Effect positions, projection and id dispatch helpers. Part of
   battle_effect.c. */

/* Particle motion and projection. A particle moves a tick by its velocity,
   gravity on its vertical speed, and a damping out of DAMPING_SCALE; the
   projection helpers return a point 16 pixels above where a unit stands, where
   effects anchor on it. */

/* Moves a particle one tick in three dimensions. */
void btParticleAdvanceWithGravity3D(
	PARTICLE *particle, Sint32 damping, Sint32 gravity)
{
	particle->x += particle->velX;
	particle->y += particle->velY;
	particle->z += particle->velZ;
	particle->velY += gravity;
	SET_VELOCITY(particle, particle->velX * damping / DAMPING_SCALE,
		particle->velY * damping / DAMPING_SCALE,
		particle->velZ * damping / DAMPING_SCALE);
}

/* Moves a particle one tick on the screen. */
void btParticleAdvanceWithGravity2D(
	PARTICLE *particle, Sint32 damping, Sint32 gravity)
{
	particle->x += particle->velX;
	particle->y += particle->velY;
	particle->velY += gravity;
	particle->velX = particle->velX * damping / DAMPING_SCALE;
	particle->velY = particle->velY * damping / DAMPING_SCALE;
}

/* Projects a point in the current transform onto the screen, 16 pixels higher,
   and returns the projection's scale. */
Sint32 btProjectionPoint(const void *point, SCREEN_POINT *pos)
{
	Sint32 result = syConvert3Dto2D(point, syScreenPointCoordinates(pos));
	pos->y -= 16;
	return result;
}

/* The screen position of a unit's sprite, at its scaled height, 16 pixels
   higher. */
void btProjectionSpr(Sint32 id, SCREEN_POINT *pos)
{
	btUnitProjectToScreen(id, syScreenPointCoordinates(pos));
	pos->y -= 16;
}

/* The screen position of a unit, scaled for the battle view, 16 pixels
   higher. */
void btProjectionScaledUnit(Sint32 id, SCREEN_POINT *pos)
{
	btMotionProjectScaledPosStub(id, syScreenPointCoordinates(pos));
	pos->y -= 16;
}

/* The screen position of a unit, 16 pixels higher. */
void btProjectionUnit(Sint32 id, SCREEN_POINT *pos)
{
	btMotionProjectPosStub(id, syScreenPointCoordinates(pos));
	pos->y -= 16;
}

/* Projects a unit onto the screen, raised by half its height, from the battle
   camera. */
Sint32 btUnitProjectToScreen(Sint32 unit, Sint32 *screen)
{
	BATTLE_CAMERA *camera = BattleCameraWork;
	ACTOR		  *actor;
	SPR			  *spr;
	FIXED		   scale;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint32 unused[3];

	actor = btObjectGetStub(unit)->actor;
	spr = btActorGetSprStub(actor, 0);
	EFFECT_VIEW_FROM_CAMERA(camera);
	scale = syMulFX(syConvert3Dto2D(&actor->x.fixed, screen), spr->scale);
	screen[1] -=
		syMulFX(scale, FIXEDtoINT(btUnitGetEffectHeightStub(unit)) >> 1);
	return 0;
}

/* Unused: reads the held keys when the first target is an enemy and does
   nothing with them. */
UNUSED void btEffectReadKeysForEnemyTarget(void)
{
	/* XXX (no evidence): what the keys were for, perhaps choosing an effect for
	   enemy targets; nothing calls this in either game. */
	BATTLE_EFFECT_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	BATTLE_EFFECT_PARM	*args = state->args;

	if(args->tgts[0] >= FIRST_ENEMY_UNIT)
		(void)PerData;
}

/* Runs a battle effect from the second range of kinds: the casting impacts, the
   beams and the ring and spark effects. */
void btEffectRunByIdRange(BATTLE_EFFECT_PARM *args)
{
	Sint32 kind;

	syHeapSlotAllocEWRAM(HEAP_SLOT_BATTLE_SHEET, GLOW_SHEET_ROOM);
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(BATTLE_EFFECT_STATE));
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	kind = args->kind;
	if(kind >= EFFECT_RANGE_CASTING_IMPACT && kind < EFFECT_RANGE_BEAM) {
		btEffectRunCastingImpact(args, kind - EFFECT_RANGE_CASTING_IMPACT);
	} else if(kind >= EFFECT_RANGE_RING_AND_SPARK) {
		btEffectRunCriticalHit(args);
	} else {
		btEffectRunAttackSlash(args);
	}
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
}
