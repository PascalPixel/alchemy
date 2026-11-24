/// @file
/// What Jupiter Lighthouse's rooms share (and Mars Lighthouse's room F, which
/// copies some of their code): the aerie's angels, archers and well lid, the
/// time the angels take to rise, and the calls the overlays make.
#ifndef ALCHEMY_JUPITER_LIGHTHOUSE_AERIE_H
#define ALCHEMY_JUPITER_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "system/main_routine_state.h"
#include "field/actor.h"
#include "system/frame.h"
#include "field/cell_target.h"
#include "field/scene_tables.h"

/// The frames the aerie's JupiterLighthouse_RaiseAngels takes to raise an
/// angel and tilt the lid, and then to lift both angels and the lid off the
/// well once both are shot.
#define ANGEL_RISE_FRAMES 64
#define LID_LIFT_FRAMES 256

/// The aerie's placed actors that its two scenes
/// (SceneId_JupiterLighthouseAerie and SceneId_JupiterLighthouseAerieBeacon)
/// number alike and JupiterLighthouse_PlaceRaisedActors moves.
enum {
  /// The angels either side of the lid over the well, and the archer statues
  /// that shoot them.
  ACTOR_LEFT_ANGEL = ACTOR_FIRST_PLACED + 1,
  ACTOR_RIGHT_ANGEL,
  ACTOR_LEFT_ARCHER,
  ACTOR_RIGHT_ARCHER,
  /// The round stone lid over the well, which the angels lift, and a second
  /// actor wearing its picture at its place, drawn zero wide: unseen, it holds
  /// the party back at the well's rim.
  ACTOR_LID = ACTOR_FIRST_PLACED + 15,
  ACTOR_UNSEEN_LID
};

struct EffectOptions;

extern u8 gDecodeFillByte;

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);
void GameFlag_SetByte(s32 flag, s32 value);
void MapGrid_CenterOnFollowedPoint(void);
/// Hangs a vine from high above an actor down to its feet (HangingVine_Place,
/// through the overlay's own import row).
void Import_HangingVinePlace(s32 id, s32 a);

#endif
