/// @file
/// The room of gaps and logs in Jupiter Lighthouse: the cracked tiles' steps
/// and the fall to the floor below.

#include "../shared/teleporter.h"
#include "../shared/tables.h"
#include "types.h"
#include "cell_target.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "system.h"
#include "frame.h"
void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"

extern u16 gPushKinds[];

/// The cells Teleporters_Step watches, and the cell it copies over the one the
/// leader steps on.
extern struct CellTarget gTeleporters;

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

DEFINE_TELEPORTER_DROP_IN(Teleporter_DropIn, FieldEffect_Spawn)

void Teleporter_Uncover();
void Teleporter_SinkEvent();

DEFINE_TELEPORTERS_STEP(Teleporters_Step, gTeleporters, Teleporter_Uncover,
                        Teleporter_SinkEvent)

DEFINE_TELEPORTER_PLACE_ARRIVAL(Teleporter_PlaceArrival, gTeleporters,
                                Teleporter_DropIn)
