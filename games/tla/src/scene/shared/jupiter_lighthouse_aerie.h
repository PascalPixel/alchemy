#ifndef ALCHEMY_JUPITER_LIGHTHOUSE_AERIE_H
#define ALCHEMY_JUPITER_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "field/actor.h"
#include "system/frame.h"
#include "field/cell_target.h"
#include "field/scene_tables.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

/// The frames the aerie's JupiterLighthouse_RaiseAngels takes to raise an
/// angel and tilt the lid, and then to lift both angels and the lid off the
/// well once both are energized.
#define ANGEL_RISE_FRAMES 64
#define LID_LIFT_FRAMES 256

/// The aerie's placed actors that its two scenes
/// (SceneId_JupiterLighthouseAerie and SceneId_JupiterLighthouseAerieBeacon)
/// number alike and JupiterLighthouse_PlaceRaisedActors moves: the angels
/// either side of the lid over the well.
enum { ACTOR_LEFT_ANGEL = ACTOR_FIRST_PLACED + 1, ACTOR_RIGHT_ANGEL };

struct EffectOptions;

extern u8 gDecodeFillByte;

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);
void GameFlag_SetByte(s32 flag, s32 value);
void MapGrid_CenterOnFollowedPoint(void);
void Import_HangingVinePlace(s32 id, s32 a);

#endif
