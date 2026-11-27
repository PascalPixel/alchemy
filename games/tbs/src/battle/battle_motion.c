/* The units' movements: returning, running, leaping and knocked back, and their
   positions on the screen. Part of battle.c. */

#include "battle_internal.h"

/* The motion values below are per tick; gravity is taken from a hop's speed
   each tick. */

/* Sends unitId back to its spot, landing exactly there (snapToTgt), and turns
   it to face the centre at once. */
void btMotionReturnToSpot(Sint32 unitId)
{
	BATTLE_OBJECT *battleObject;
	ACTOR		  *actor;

	battleObject = btObjectGet(unitId);
	actor = battleObject->actor;
	actor->acceleration = toFIXED(2);
	actor->speedLimit = toFIXED(8);
	actor->velY = 0;
	actor->gravity = BATTLE_UNIT_GRAVITY;
	actor->bounce = 0;
	actor->steeringFlg = 0;
	actor->snapToTgt = 1;
	fdActorStartIdleStub(actor);
	fdActorSetMoveTargetStub(
		actor, battleObject->anchorX, 0, battleObject->anchorZ);
	/* As btObjectMake turns a new actor. */
	actor->facing = syAtan(battleObject->anchorZ / 8, battleObject->anchorX) -
		DEGtoANG(180);
}

/* Sends unitId running off its side of the battlefield, three times as far out
   as its spot, with a hop, in POSE_STANDING. */
void btMotionSetupEscapeObject(Sint32 unitId)
{
	BATTLE_OBJECT *battleObject;
	ACTOR		  *actor;

	battleObject = btObjectGet(unitId);
	actor = battleObject->actor;
	actor->acceleration = toFIXED(2);
	actor->speedLimit = toFIXED(8);
	actor->velY = toFIXED(5);
	actor->gravity = toFIXED(47) / 100;
	actor->bounce = 0;
	actor->steeringFlg = 0;
	fdActorStartIdleStub(actor);
	fdActorSetMoveTargetStub(
		actor, battleObject->anchorX * 3, 0, battleObject->anchorZ);
	fdActorSelectAnimationStub(actor, POSE_STANDING);
}

/* Unused: sends one unit leaping progress percent of the way toward another. */
void btMotionLeapToward(
	BATTLE_OBJECT *startSlot, BATTLE_OBJECT *endSlot, Sint32 progress)
{
	FIXED  zStep;
	FIXED  startX;
	FIXED  x;
	FIXED  startZ;
	FIXED  endZ;
	ACTOR *start;
	ACTOR *end;

	start = startSlot->actor;
	end = endSlot->actor;
	startX = start->x.fixed;
	x = startX + progress * (end->x.fixed - startX) / 100;
	endZ = end->z.fixed;
	startZ = start->z.fixed;
	zStep = progress * (endZ - startZ) / 100;
	REG_BLDCNT = 0;
	start->acceleration = toFIXED(2);
	start->speedLimit = toFIXED(8);
	start->velY = toFIXED(4);
	start->gravity = BATTLE_UNIT_GRAVITY;
	start->bounce = 0;
	start->steeringFlg = 1;
	fdActorSetMoveTargetStub(start, x, 0, startZ + zStep);
	fdActorSelectAnimationStub(start, POSE_LEAP);
}

/* Unused: sends a unit's actor back to its spot. */
void btMotionReturnObjectToSpot(BATTLE_OBJECT *battleObject)
{
	ACTOR *actor = battleObject->actor;

	actor->acceleration = toFIXED(2);
	actor->speedLimit = toFIXED(8);
	actor->gravity = BATTLE_UNIT_GRAVITY;
	actor->bounce = 0;
	actor->steeringFlg = 0;
	fdActorSetMoveTargetStub(
		actor, battleObject->anchorX, 0, battleObject->anchorZ);
}

/* Sends unitId hopping out to one and a half times its spot's distance from the
   centre. */
void btMotionHopBack(Sint32 unitId)
{
	BATTLE_OBJECT *battleObject;
	ACTOR		  *actor;

	battleObject = btObjectGet(unitId);
	actor = battleObject->actor;
	actor->acceleration = toFIXED(1);
	actor->speedLimit = toFIXED(4);
	actor->velY = toFIXED(3);
	actor->gravity = toFIXED(3) / 5;
	actor->bounce = 0;
	actor->steeringFlg = 0;
	fdActorStartIdleStub(actor);
	fdActorSetMoveTargetStub(
		actor, battleObject->anchorX * 3 / 2, 0, battleObject->anchorZ);
}

/* Unused: sends a unit leaping back from the centre in its damage pose. */
void btMotionLeapBack(Sint32 unitId)
{
	BATTLE_OBJECT *battleObject = btObjectGet(unitId);
	ACTOR		  *actor = battleObject->actor;

	actor->acceleration = toFIXED(1);
	actor->speedLimit = toFIXED(4);
	actor->velY = toFIXED(4);
	actor->gravity = toFIXED(3) / 5;
	actor->bounce = 0;
	actor->steeringFlg = 0;
	fdActorStartIdleStub(actor);
	fdActorSetMoveTargetStub(actor,
		syMulFX(battleObject->anchorX, toFIXED(13) / 10), 0,
		battleObject->anchorZ);
	fdActorSelectAnimationStub(actor, POSE_DAMAGE);
}

/* Knocks a unit back as the knockback variant says, in its damage pose; the
   Fusion Dragon only takes the pose. */
void btMotionApplyKnockback(Sint32 unitId, Sint32 variant)
{
	BATTLE_OBJECT *battleObject;
	ACTOR		  *actor;
	Sint32		   tgtX;
	Sint32		   anchorX;

	battleObject = btObjectGet(unitId);
	actor = battleObject->actor;
	if(gmUnitGetStub(unitId)->id != ENEMY_FUSION_DRAGON) {
		actor->acceleration = BattleKnockbackAcceleration[variant];
		actor->speedLimit = BattleKnockbackSpeedLimit[variant];
		/* A unit already in the air keeps its hop, except from KNOCKBACK_SKID
		   on. */
		if(actor->y.fixed == 0 || variant > KNOCKBACK_HURL)
			actor->velY = BattleKnockbackVelocityY[variant];
		actor->gravity = toFIXED(3) / 5;
		actor->bounce = 0;
		actor->steeringFlg = 0;
		fdActorStartIdleStub(actor);
		anchorX = battleObject->anchorX;
		tgtX = (anchorX * BattleKnockbackDistancePercent[variant]) / 100;
		fdActorSetMoveTargetStub(actor, tgtX, 0, battleObject->anchorZ);
	}
	fdActorSelectAnimationStub(actor, POSE_DAMAGE);
}

/* Sends one unit leaping three quarters of the way to another, arriving in
   `ticks` ticks, with a hop of initVelY. */
void btMotionApproachTarget(
	Sint32 unitId, Sint32 tgtId, Sint32 ticks, Sint32 initVelY)
{
	BATTLE_OBJECT *actorSlot = btObjectGet(unitId);
	BATTLE_OBJECT *tgtSlot = btObjectGet(tgtId);
	ACTOR		  *actor = actorSlot->actor;
	ACTOR		  *tgt = tgtSlot->actor;
	Sint32		   percent = 75;
	FIXED		   dx = tgt->x.fixed - actor->x.fixed;
	FIXED		   startX = actor->x.fixed;
	FIXED		   stepX = percent * dx / 100;
	FIXED		   dz = tgt->z.fixed - actor->z.fixed;
	FIXED		   startZ = actor->z.fixed;
	FIXED		   stepZ = percent * dz / 100;
	FIXED		   x = startX + stepX;
	FIXED		   z = startZ + stepZ;
	/* The distance is taken in 1/256 pixels, so its square fits 32 bits. */
	Sint32 coarseX = stepX >> 8;
	Sint32 coarseZ = stepZ >> 8;
	Sint32 speed;

	speed = sySquartIWRAM(coarseX * coarseX + coarseZ * coarseZ);
	speed = (speed << 8) / ticks;
	actor->acceleration = speed;
	actor->speedLimit = speed;
	actor->snapToTgt = 1;

	/* DEAD: the store under ACTOR_FLOATS is overwritten at once; the ROM keeps
	   the test and its store. */
	if(actor->motionFlg & ACTOR_FLOATS)
		actor->velY = initVelY;
	actor->velY = initVelY;
	actor->gravity = BATTLE_UNIT_GRAVITY;
	actor->steeringFlg = 1;
	fdActorStartIdleStub(actor);
	fdActorSetMoveTargetStub(actor, x, 0, z);
	fdActorSelectAnimationStub(actor, POSE_LEAP);
}

/* Unused: restarts unit id's motion in POSE_LEAP. */
void btMotionRestartLeap(Sint32 unitId)
{
	ACTOR *actor;

	actor = btObjectGet(unitId)->actor;
	fdActorStartIdleStub(actor);
	fdActorSelectAnimationStub(actor, POSE_LEAP);
}

/* Does nothing; btMotionApplyPairMidpoint, its only caller, passes it a
   position and a distance. */
void btMotionMidpointNoOperation()
{
}

/* Unused: works out the point halfway between two units and hands it to a
   function that does nothing. */
void btMotionApplyPairMidpoint(Sint32 firstId, Sint32 secondId)
{
	BATTLE_OBJECT *secondSlot;
	ACTOR		  *first;
	ACTOR		  *second;
	FIXED		   firstX;
	FIXED		   secondX;
	FIXED		   firstZ;
	FIXED		   secondZ;
	POS			   pos;

	first = btObjectGet(firstId)->actor;
	secondSlot = btObjectGet(secondId);
	second = secondSlot->actor;

	firstX = first->tgtX;
	if(firstX == ACTOR_NO_TARGET) {
		firstX = first->x.fixed;
	}
	firstZ = first->tgtZ;
	if(firstZ == ACTOR_NO_TARGET) {
		firstZ = first->z.fixed;
	}
	secondX = second->tgtX;
	if(secondX == ACTOR_NO_TARGET) {
		secondX = second->x.fixed;
	}
	secondZ = second->tgtZ;
	if(secondZ == ACTOR_NO_TARGET) {
		secondZ = second->z.fixed;
	}

	pos.x = (firstX + secondX) / 2;
	pos.y = 0;
	pos.z = (firstZ + secondZ) / 2;

	btMotionMidpointNoOperation(&pos, toFIXED(1) / 16);
}

/* Unused: plays the hit flash on unit id's sprite and waits ten ticks. Only a
   single-sprite actor takes it. */
void btMotionPlayHitFlash(Sint32 unitId)
{
	ACTOR		  *actor;
	SPR_ANIMATION *effect;
	Sint32		   effectHandle;

	actor = btObjectGet(unitId)->actor;
	if((actor != NULL) &&
		((ACTOR_SPRITE_KIND_MASK & actor->sprStorageKind) ==
			ACTOR_SPRITE_ONE)) {
		effectHandle = fdSprAddAnimationStub(actor->spr, SPRITE_STAR_BURST);
		if(effectHandle != 0) {
			effect = (SPR_ANIMATION *)effectHandle;
			fdSprAnimationStartStub(effect, 1);
			effect->pri = 3;
		}
		sySynch(10);
	}
}

/* Projects the point above a unit where its battle effects play onto the
   screen, scaled as its sprite is. Returns 0. */
Sint32 btMotionProjectScaledPos(Sint32 unitId, Sint32 *projected)
{
	ACTOR *actor = btObjectGet(unitId)->actor;
	SPR	  *spr = btActorGetSpr(actor, 0);
	Sint32 projectionScale;
	FIXED  sprScale;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED SCREEN_POINT input;

	btCameraApplyTransform();
	projectionScale = syConvert3Dto2D(&actor->x.fixed, projected);
	sprScale = syMulFX(projectionScale, spr->scale);
	projectionScale =
		syMulFX(sprScale, FIXEDtoINT((Sint32)btUnitGetEffectHeight(unitId)));
	projected[1] -= projectionScale;
	return 0;
}

/* The pixels above a unit where its battle effects play when its battle sprite
   row gives none: a small enemy's, and everyone else's. */
#define EFFECT_HEIGHT_SMALL 24
#define EFFECT_HEIGHT_LARGE 48

/* Projects the point above a unit onto the screen: the target cursor's place,
   EFFECT_HEIGHT_SMALL up for a small enemy and EFFECT_HEIGHT_LARGE for others,
   scaled as the sprite is. Returns 0. */
Sint32 btMotionProjectCursorPoint(Sint32 unitId, Sint32 *projected)
{
	ACTOR *actor = btObjectGet(unitId)->actor;
	SPR	  *spr = btActorGetSpr(actor, 0);
	Sint32 projectedHeight;
	FIXED  sprScale;

	btCameraApplyTransform();
	projectedHeight = syConvert3Dto2D(&actor->x.fixed, projected);
	sprScale = syMulFX(projectedHeight, spr->scale);
	if(btSprIsSmall(gmUnitGetStub(unitId)->id) != 0)
		projectedHeight = syMulFX(sprScale, EFFECT_HEIGHT_SMALL);
	else
		projectedHeight = syMulFX(sprScale, EFFECT_HEIGHT_LARGE);
	projected[1] -= projectedHeight;
	return 0;
}

/* Returns the height above a unit where its battle effects play, in 16.16
   pixels: its battle sprite row's own, else EFFECT_HEIGHT_SMALL for a small
   enemy and EFFECT_HEIGHT_LARGE for others. */
Sint32 btUnitGetEffectHeight(Sint32 unitId)
{
	Uint32 height;
	Uint8  sprRow;

	sprRow = gmUnitGetStub(unitId)->id;
	height = toFIXED((Uint8)btSprGetEffectHeight(sprRow));
	if(height == 0) {
		sprRow = gmUnitGetStub(unitId)->id;
		if(btSprIsSmall(sprRow) != 0) {
			height = toFIXED(EFFECT_HEIGHT_SMALL);
		} else {
			height = toFIXED(EFFECT_HEIGHT_LARGE);
		}
	}
	return height;
}

#undef EFFECT_HEIGHT_SMALL
#undef EFFECT_HEIGHT_LARGE
