// The World Map's placed actors, which both the World Map's and the staff credits' overlays keep a
// copy of, included as text: the exit chosen by the side the leader approaches a place from, the
// event that shows the places' actors coming into view and releases those leaving it, and the
// sparks and particles about the ship.

#include "types.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "field/sprite.h"
#include "field/script_inline.h"
#include "field/actor.h"
#include "field/world_map.h"
#include "field/field_sprite.h"
#include "macros.h"
#include "field/actor_view.h"
#include "field/field_mode_script.h"
#include "field/map_grid.h"
#include "field/flag_call.h"
#include "field/field_mode.h"

extern ACTOR *WorldMapPlacedActors[];

// Unused: chooses a passage exit from the leader's east-west approach: exitBefore while the leader
// is west of the place's actor, else exitAfter.
UNUSED void   scWorldMapSelectExitByX(Sint32 cue, Sint32 exitBefore, Sint32 exitAfter)
{
	FIELD_STATE *state;
	ACTOR       *area;
	ACTOR       *leader;

	area   = WorldMapPlacedActors[cue - STEP_CUE_AREA_BASE];
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	state  = RamHeapSlots->fieldState;
	if (leader->x.fixed < area->x.fixed)
	{
		state->exit = exitBefore;
	}
	else
	{
		state->exit = exitAfter;
	}
	fdScriptPlaySound(SE_MAP_EXIT);
}

// Chooses a passage exit from the leader's north-south approach.
void scWorldMapSelectExitByZ(Sint32 cue, Sint32 exitBefore, Sint32 exitAfter)
{
	FIELD_STATE *state;
	ACTOR       *area;
	ACTOR       *leader;

	area   = WorldMapPlacedActors[cue - STEP_CUE_AREA_BASE];
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	state  = RamHeapSlots->fieldState;
	if (leader->z.fixed < area->z.fixed)
	{
		state->exit = exitBefore;
	}
	else
	{
		state->exit = exitAfter;
	}
	fdScriptPlaySound(SE_MAP_EXIT);
}

extern Sint32 WorldMapPlacedActorScript[];

// Each tick: creates the actor of each place (fdWorldMapPlacesGetTable) whose flag shows it as it
// comes within reach of the camera, releases those that leave it, and raises a place's step cue
// when the leader touches its actor; in Sand's form on the World Map it asks for Sand again, at no
// PP, instead.
void          scWorldMapUpdatePlacedActors(void)
{
	const WORLD_MAP_PLACE *placement   = fdWorldMapPlacesGetTable();
	ACTOR                 *leader      = fdActorGetById(fdPartyGetLeaderActor());
	Sint32                 leaderReach = leader->sprite->scale * leader->radius;
	Uint32                 i;
	FIXED                  left, right, top, bottom;
	ACTOR                **slot;
	FIELD_STATE           *state = RamHeapSlots->fieldState;
	MAP_STATE             *map   = RamHeapSlots->mapGrid;
	ACTOR                 *actor = state->camera;

	if (map->zoom <= toFIXED(1))
	{
		left   = actor->x.fixed - toFIXED(160);
		right  = actor->x.fixed + toFIXED(160);
		top    = actor->z.fixed - toFIXED(300);
		bottom = actor->z.fixed + toFIXED(200);
	}
	else
	{
		left   = actor->x.fixed - toFIXED(240);
		right  = actor->x.fixed + toFIXED(240);
		top    = actor->z.fixed - toFIXED(450);
		bottom = actor->z.fixed + toFIXED(300);
	}
	for (i = 0, slot = WorldMapPlacedActors; i < MAP_SCREEN_PLACE_COUNT && placement->flags != 0;
	     i++, slot++, placement++)
	{
		FIXED px, pz;

		if (placement->value == WORLD_MAP_MARKER_SHIP)
			continue;
		actor = *slot;
		px    = toFIXED(placement->x);
		pz    = toFIXED(placement->z);
		if (px > left && px < right && pz > top && pz < bottom)
		{
			Sint32 flag = placement->shownFlag;
			Sint32 shown;

			if (flag == -1)
				shown = 1;
			else if (flag & CONDITION_FLAG_SET)
				shown = syGameFlagGet(flag);
			else
				shown = syFlagIsClear(flag);
			if (shown)
			{
				if (actor == NULL)
				{
					SPR *sprite;

					actor = syImportActorCreate(placement->sprite, px, 0, pz);
					if (actor == NULL)
						continue;
					fdActorStartScript(actor, WorldMapPlacedActorScript);
					actor->motionFlags    = 0;
					actor->collisionFlags = ACTOR_SOLID;
					actor->facing         = placement->facing;
					fdActorSelectAnimation(actor, 1);
					sprite        = actor->sprite;
					sprite->scale = syMulFX(sprite->scale, toFIXED(3) / 4);
					*slot         = actor;
				}
				if (MainRoutineStateWork.ownerOverride == 0)
				{
					Sint32 scale = actor->sprite->scale;
					FIXED  dx    = ABS(actor->x.fixed - leader->x.fixed);

					if (dx + ABS(actor->z.fixed - leader->z.fixed)
					    < leaderReach + actor->radius * scale)
					{
						if (syGameFlagGet(FLAG_FIELD_HANDLING_REQUESTS) == 0)
						{
							if (MainRoutineStateWork.movementMode == LEADER_MOVEMENT_WORLD_SAND)
								state->psynergy = ACT_PACKED | ABILITY_SAND;
							else
								state->stepCue = i + STEP_CUE_AREA_BASE;
						}
					}
				}
				continue;
			}
		}
		if (actor != NULL)
		{
			fdActorRelease(actor);
			*slot = NULL;
		}
	}
}

// Unused: unless a menu has suspended the actors, queues the World Map's backgrounds lightened
// toward white.
UNUSED void scWorldMapQueueBlend(void)
{
	if (RamHeapSlots->actorSystem->suspended == 0)
	{
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
		                    BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3
		                        | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
		                    IO_WRITE_HALFWORD);
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDY, 12, IO_WRITE_HALFWORD);
	}
}

// Clears the places' actors and starts scWorldMapUpdatePlacedActors.
void scWorldMapStartPlacedActors(void)
{
	syWordsClear(WorldMapPlacedActors, MAP_SCREEN_PLACE_COUNT * sizeof(ACTOR *));
	sySetTickEvent(scWorldMapUpdatePlacedActors, EVENT_PASS_TICK);
}

// Unused: the metatile index of a point in 16.16 pixels.
UNUSED Sint32 scWorldMapGetMetatileIndex(FIXED x, FIXED z)
{
	return x / toFIXED(MAPGRID_METATILE_PIXELS)
	     + (z / toFIXED(MAPGRID_METATILE_PIXELS)) * MAPGRID_SIZE;
}

// The ticks a ship's spark rises for.
#define SPARK_RISE_TICKS 32

// A spark's rise (WorldMapSparkScript): rises a pixel a tick over the actor it watches, growing and
// shrinking again over SPARK_RISE_TICKS; returns 0 when done.
Bool scWorldMapRiseFromAnchor(ACTOR *actor)
{
	ACTOR *parent = (ACTOR *)actor->watchedActor;
	Sint32 step   = ++actor->timer;
	FIXED  scale;

	if (step >= SPARK_RISE_TICKS)
		return FALSE;
	scale           = syMulFX(syMathSin(step * (DEGtoANG(180) / SPARK_RISE_TICKS)), toFIXED(5) / 4);
	actor->scaleX   = scale;
	actor->scaleY   = scale;
	actor->x.fixed  = parent->x.fixed;
	actor->y.fixed += toFIXED(1);
	actor->z.fixed  = parent->z.fixed;
	return TRUE;
}

extern Sint32 WorldMapSparkScript[];
extern Uint32 WorldMapParticleScript[];

// Spawns a pair of rising sparks at the actor, as The Broken Seal's MercuryLighthouseSpawnSparkPair
// does at a spawn point.
void          scWorldMapSpawnSparkPair(ACTOR *actor)
{
	ACTOR     *spark;
	FIELD_SPR *sprite;

	// DEAD: a scale multiplied and dropped; the ROM keeps the call.
	syMathMulFX(actor->scaleX, toFIXED(3) / 2);
	spark = scFieldActorCreate(SPRITE_GOLDEN_RINGS, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	if (spark != NULL)
	{
		spark->groundHeight = actor->groundHeight;
		sprite              = spark->sprite;
		fdActorStartScript(spark, WorldMapSparkScript);
		spark->motionFlags  = 0;
		spark->timer        = 0;
		spark->watchedActor = (union FIELD_OBJECT *)actor;
		if (sprite != NULL)
		{
			fdSpriteStartAnimation(sprite, 2);
			sprite->flags    = 0;
			sprite->priority = 1;
		}
	}
	spark = scFieldActorCreate(SPRITE_GOLDEN_RINGS, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	if (spark != NULL)
	{
		spark->groundHeight = actor->groundHeight;
		sprite              = spark->sprite;
		fdActorStartScript(spark, WorldMapSparkScript);
		spark->motionFlags  = 0;
		spark->timer        = 0;
		spark->watchedActor = (union FIELD_OBJECT *)actor;
		spark->layer        = 1;
		if (sprite != NULL)
		{
			fdSpriteStartAnimation(sprite, 1);
			sprite->flags = 0;
		}
	}
}

// Spawns a particle at a random spot near the actor.
void scWorldMapSpawnParticle(ACTOR *actor)
{
	Sint32     pos[3];
	FIXED      spread;
	ACTOR     *particle;
	FIELD_SPR *sprite;

	// DEAD: a scale multiplied and dropped; the ROM keeps the call.
	syMulFX(actor->scaleX, toFIXED(3) / 2);
	SET_VECTOR(pos, actor->x.fixed, actor->y.fixed + toFIXED(2), actor->z.fixed);
	spread = syRandom();
	syFieldVectorAddPolarOffsetFX(spread * 20, syRandom(), pos);
	particle = (scFieldActorCreate(SPRITE_GLINT, pos[0], pos[1], pos[2]));
	if (particle != NULL)
	{
		particle->groundHeight = actor->groundHeight;
		sprite                 = particle->sprite;
		fdActorStartScript(particle, WorldMapParticleScript);
		fdSpriteStartAnimation(sprite, 2);
		particle->gravity = toFIXED(1) / 128;
		particle->scaleX  = toFIXED(4) / 5;
		particle->scaleY  = toFIXED(4) / 5;
		if (sprite != NULL)
			sprite->flags = 0;
	}
}

#undef SPARK_RISE_TICKS
