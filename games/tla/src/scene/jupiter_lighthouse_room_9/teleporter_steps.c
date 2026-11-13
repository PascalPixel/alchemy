/// @file
/// The hover-tile hall of Jupiter Lighthouse's right tower: the teleporters'
/// steps and arrivals.

#include "../shared/tables.h"
#include "../shared/teleporter.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "cell_target.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"

#include "field/cell_target.h"
struct CellTarget;
struct CellTargetEntry;
void Teleporter_PlaceArrival(s32 x, s32 z, s32 effect);
extern struct CellTarget gTeleporters;
void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);
void JupiterLighthouse_StopHovering(void);
void Whirlwind_CarryLeader(void);
extern u32 gFieldEffect_ShortLifeScript[];
extern u32 gFieldEffect_MediumLifeScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_TELEPORTER_DROP_IN(Teleporter_DropIn, FieldEffect_Spawn)

void GameFlag_SetByte(s32 flag, s32 value);
void Teleporter_Sink(struct CellTarget *work, struct CellTargetEntry *entry);

DEFINE_TELEPORTERS_STEP(Teleporters_Step, gTeleporters, Teleporter_Uncover,
                        Teleporter_Sink)

DEFINE_TELEPORTER_PLACE_ARRIVAL(Teleporter_PlaceArrival, gTeleporters,
                                Teleporter_DropIn)
