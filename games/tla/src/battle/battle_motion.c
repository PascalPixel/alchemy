/* @file */
/* The units' movements: returning to their spots, running and leaping, and */
/* their positions on the screen. */
/* Part of battle.c. */

#define fdActorStartIdleStub ((void (*)(MOTION_OBJECT *))fdActorStartIdleStub)
#define fdActorSetMoveTargetStub                                               \
	((void (*)(MOTION_OBJECT *, s32, s32, s32))fdActorSetMoveTargetStub)
#define fdActorSelectAnimationStub                                             \
	((void (*)(MOTION_OBJECT *, s32))fdActorSelectAnimationStub)

/* Sends unitId back to its spot, landing exactly there (snapToTgt), and */
/* turns it to face the centre at once. */
void btMotionReturnToSpot(s32 unitId) {
	BATTLE_OBJECT *slot;
	MOTION_OBJECT *object;

	slot = btObjectGet(unitId);
	object = slot->obj;
	object->acceleration = toFIXED(2);
	object->speedLimit = toFIXED(8);
	object->velY = 0;
	object->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	object->snapToTgt = 1;
	fdActorStartIdleStub(object);
	fdActorSetMoveTargetStub(object, slot->anchorX, 0, slot->anchorZ);
	/* As btObjectMake turns a new actor. */
	object->ang =
		(s16)(syAtan(slot->anchorZ / 8, slot->anchorX) - DEGtoANG(180));
}

/* Sends unitId running off its side of the battlefield, three times as far */
/* out as its spot, with a hop, in POSE_STANDING. */
void btMotionSetupEscapeObject(s32 unitId) {
	BATTLE_OBJECT *slot;
	MOTION_OBJECT *object;

	slot = btObjectGet(unitId);
	object = slot->obj;
	object->acceleration = toFIXED(2);
	object->speedLimit = toFIXED(8);
	object->velY = toFIXED(5);
	object->verticalMotionStrength = toFIXED(47) / 100;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	fdActorStartIdleStub(object);
	fdActorSetMoveTargetStub(object, slot->anchorX * 3, 0, slot->anchorZ);
	fdActorSelectAnimationStub(object, POSE_STANDING);
}

/* Unused: sends one unit leaping progress percent of the way toward another, */
/* and turns off the colour effects. */
void btMotionLeapToward(ACTOR **startSlot, ACTOR **endSlot, s32 progress) {
	ACTOR *start = *startSlot;
	ACTOR *end = *endSlot;
	FIXED x;
	FIXED z;

	x = start->x.fixed + (end->x.fixed - start->x.fixed) * progress / 100;
	z = start->z.fixed + (end->z.fixed - start->z.fixed) * progress / 100;
	REG_BLDCNT = 0;
	start->acceleration = toFIXED(2);
	start->speedLimit = toFIXED(8);
	start->velY = toFIXED(4);
	start->gravity = BATTLE_UNIT_GRAVITY;
	start->bounce = 0;
	start->steeringFlg = 1;
	fdActorSetMoveTargetStub(start, x, 0, z);
	fdActorSelectAnimationStub(start, POSE_LEAP);
}

/* Unused: sends a unit back to its spot. */
void btMotionReturnObjectToSpot(BATTLE_OBJECT *slot) {
	MOTION_OBJECT *object = slot->obj;

	object->acceleration = toFIXED(2);
	object->speedLimit = toFIXED(8);
	object->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	fdActorSetMoveTargetStub(object, slot->anchorX, 0, slot->anchorZ);
}

/* Sends unitId hopping out to one and a half times its spot's distance from */
/* the centre. */
void btMotionHopBack(s32 unitId) {
	BATTLE_OBJECT *slot;
	MOTION_OBJECT *object;

	slot = btObjectGet(unitId);
	object = slot->obj;
	object->acceleration = FIXED_ONE;
	object->speedLimit = toFIXED(4);
	object->velY = toFIXED(3);
	object->verticalMotionStrength = toFIXED(3) / 5;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	fdActorStartIdleStub(object);
	fdActorSetMoveTargetStub(object, slot->anchorX * 3 / 2, 0, slot->anchorZ);
}

/* Unused: sends unitId leaping back to 1.3 times its spot's distance from the
 */
/* centre, in POSE_DAMAGE. */
void btMotionLeapBack(s32 unitId) {
	BATTLE_OBJECT *slot;
	MOTION_OBJECT *object;

	slot = btObjectGet(unitId);
	object = slot->obj;
	object->acceleration = FIXED_ONE;
	object->speedLimit = toFIXED(4);
	object->velY = toFIXED(4);
	object->verticalMotionStrength = toFIXED(3) / 5;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	fdActorStartIdleStub(object);
	{
		s32 (*mul)(s32 a, s32 b);
		s32 anchor = slot->anchorX;

		mul = syMulFX;
		fdActorSetMoveTargetStub(object, mul(anchor, FIXED_ONE * 13 / 10), 0,
								 slot->anchorZ);
	}
	fdActorSelectAnimationStub(object, POSE_DAMAGE);
}

/* Knocks a unit back as the knockback variant (BATTLE_KNOCKBACK) says, in
 */
/* its damage pose; the Fusion Dragon and the Doom Dragon's three forms only */
/* take the pose. */
void btMotionApplyKnockback(s32 id, s32 variant) {
	BATTLE_OBJECT *slot;
	MOTION_OBJECT *object;
	s32 x;
	s32 scale;
	s32 enemyId;

	slot = btObjectGet(id);
	object = slot->obj;
	enemyId = syUnitGet(id)->enemyId;
	if(enemyId != ENEMY_FUSION_DRAGON && enemyId != ENEMY_DOOM_DRAGON_A &&
	   enemyId != ENEMY_DOOM_DRAGON_E && enemyId != ENEMY_DOOM_DRAGON_H) {
		object->acceleration = BattleKnockback_Acceleration[variant];
		object->speedLimit = BattleKnockback_SpeedLimit[variant];
		/* A unit already in the air keeps its hop, except from KNOCKBACK_SKID
		 * on. */
		if(object->y == 0 || variant > 4)
			object->velY = BattleKnockback_VelocityY[variant];
		fdActorStartIdleStub(object);
		scale = slot->anchorX;
		x = (scale * BattleKnockback_DistancePercent[variant]) / 100;
		fdActorSetMoveTargetStub(object, x, 0, slot->anchorZ);
	}
	object->verticalMotionStrength = toFIXED(3) / 5;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	fdActorSelectAnimationStub(object, POSE_DAMAGE);
}

/* Sends one unit leaping three quarters of the way to another, arriving in */
/* travelFrames frames. */
void btMotionApproachTarget(s32 actorId, s32 targetId, s32 travelFrames,
							s32 initialVelocityY) {
	BATTLE_OBJECT *actorSlot = btObjectGet(actorId);
	BATTLE_OBJECT *targetSlot = btObjectGet(targetId);
	MOTION_OBJECT *object = actorSlot->obj;
	MOTION_OBJECT *target = targetSlot->obj;
	s32 percent = 75;
	s32 dx = target->x - object->x;
	s32 startX = object->x;
	s32 stepX = percent * dx / 100;
	s32 dz = target->z - object->z;
	s32 startZ = object->z;
	s32 stepZ = percent * dz / 100;
	s32 x = startX + stepX;
	s32 z = startZ + stepZ;
	/* The distance is taken in 1/256 pixels, so its square fits 32 bits. */
	s32 coarseX = stepX >> 8;
	s32 coarseZ = stepZ >> 8;
	s32 sum = coarseX * coarseX + coarseZ * coarseZ;
	s32 dist;

	if(sum != 0)
		dist = sySquart(sum) << 8;
	else
		dist = 0;
	dist = dist / travelFrames;
	object->acceleration = dist;
	object->speedLimit = dist;
	object->snapToTgt = 1;

	/* DEAD: the store under ACTOR_FLOATS is overwritten at once; the ROM keeps
	 */
	/* the test and its store. */
	if(object->motionFlg & ACTOR_FLOATS)
		object->velY = initialVelocityY;
	object->velY = initialVelocityY;
	object->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
	object->autoFaceMotion = 1;
	fdActorStartIdleStub(object);
	if(dist != 0)
		fdActorSetMoveTargetStub(object, x, 0, z);
	fdActorSelectAnimationStub(object, POSE_LEAP);
}

/* Unused: restarts a unit's motion in POSE_LEAP. */
void btMotionRestartLeap(s32 id) {
	MOTION_OBJECT *object;

	object = btObjectGet(id)->obj;
	fdActorStartIdleStub(object);
	fdActorSelectAnimationStub(object, POSE_LEAP);
}

#undef fdActorStartIdleStub
#undef fdActorSetMoveTargetStub
#undef fdActorSelectAnimationStub

/* Does nothing; btMotionApplyPairMidpoint, its only caller, passes it a */
/* position and a distance. */
void btMotionMidpointNoOp(void *position, s32 speed) {}

/* Unused: works out the point halfway between two units and hands it to a */
/* function that does nothing. */
void btMotionApplyPairMidpoint(s32 firstId, s32 secondId) {
	BATTLE_OBJECT *secondSlot;
	MOTION_OBJECT *first;
	MOTION_OBJECT *second;
	s32 x1;
	s32 x2;
	s32 z1;
	s32 z2;
	struct {
		s32 x;
		s32 y;
		s32 z;
	} pos;

	first = btObjectGet(firstId)->obj;
	secondSlot = btObjectGet(secondId);

	x1 = first->tgtX;
	second = secondSlot->obj;
	if(x1 == ACTOR_NO_TARGET) {
		x1 = first->x;
	}
	z1 = first->tgtZ;
	if(z1 == ACTOR_NO_TARGET) {
		z1 = first->z;
	}
	x2 = second->tgtX;
	if(x2 == ACTOR_NO_TARGET) {
		x2 = second->x;
	}
	z2 = second->tgtZ;
	if(z2 == ACTOR_NO_TARGET) {
		z2 = second->z;
	}

	SET_POSITION(&pos, (x1 + x2) / 2, 0, (z1 + z2) / 2);

	btMotionMidpointNoOp(&pos, toFIXED(1) / 16);
}

/* Unused: plays the hit flash on a unit's sprite and waits ten frames. Only a
 */
/* single-sprite actor takes it. */
void btMotionPlayHitFlash(s32 unitId) {
	BATTLE_OBJECT *battleObject = btObjectGet(unitId);
	SPR_ANIMATION *effect;

	if(battleObject->obj != NULL) {
		if((battleObject->obj->recordStorageKind & ACTOR_SPRITE_KIND_MASK) ==
		   ACTOR_SPRITE_ONE) {
			effect = (SPR_ANIMATION *)fdSprAddAnimationStub(
				battleObject->obj->records, SPRITE_STAR_BURST);
			if(effect != NULL) {
				fdSprAnimationStartStub(effect, 1);
				effect->pri = 3;
			}
			sySynch(10);
		}
	}
}

/* The pixels above a unit where its battle effects play when its class gives */
/* none: a small enemy's, and everyone else's. */
#define EFFECT_HEIGHT_SMALL 24
#define EFFECT_HEIGHT_LARGE 48

/* Projects the point above a unit where its battle effects play */
/* (btUnitGetEffectHeight) onto the screen, into screen, scaled as its */
/* sprite is. Returns 0. */
s32 btMotionProjectScaledPos(s32 unitId, s32 *screen) {
	MOTION_OBJECT *object = btObjectGet(unitId)->obj;
	SPR *record = btActorGetSpr(object, 0);
	FIXED scale;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED s32 unused[3];

	btCameraApplyTransform();
	scale = syMulFX(syConvert3Dto2D(&object->x, screen), record->scale);
	screen[1] -= syMulFX(scale, FIXEDtoINT(btUnitGetEffectHeight(unitId)));
	return 0;
}

/* Projects the point above a unit onto the screen, into screen: the target */
/* cursor's place, EFFECT_HEIGHT_SMALL up for a small enemy and */
/* EFFECT_HEIGHT_LARGE for others, scaled as the sprite is. Returns 0. */
s32 btMotionProjectCursorPoint(s32 unitId, s32 *screen) {
	MOTION_OBJECT *object = btObjectGet(unitId)->obj;
	SPR *record = btActorGetSpr(object, 0);
	FIXED scale;

	btCameraApplyTransform();
	scale = syMulFX(syConvert3Dto2D(&object->x, screen), record->scale);
	if(btSprIsSmall(syUnitGet(unitId)->enemyId) != 0)
		screen[1] -= syMulFX(scale, EFFECT_HEIGHT_SMALL);
	else
		screen[1] -= syMulFX(scale, EFFECT_HEIGHT_LARGE);
	return 0;
}

/* Returns the height above a unit where its battle effects play, in 16.16 */
/* pixels: its class's own, else EFFECT_HEIGHT_SMALL for a small enemy and */
/* EFFECT_HEIGHT_LARGE for others. */
FIXED btUnitGetEffectHeight(s32 unitId) {
	FIXED value = toFIXED((u8)btSprGetEffectHeight(syUnitGet(unitId)->enemyId));

	if(value == 0) {
		if(btSprIsSmall(syUnitGet(unitId)->enemyId) != 0)
			value = toFIXED(EFFECT_HEIGHT_SMALL);
		else
			value = toFIXED(EFFECT_HEIGHT_LARGE);
	}
	return value;
}
