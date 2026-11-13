#ifndef SETTLEMENTS_LOCAL_H
#define SETTLEMENTS_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "scene_ids.h"
#include "event_inline.h"

s32 Trig_Cos(s32 angle);

extern u8 MsgSettlements01[];

#include "frame.h"

extern s16 gSettlements_FallsObjects[];
extern u16 gSettlements_SlideStopped;

void Settlements_SpawnSmoke(s32 x, s32 y, s32 z);
void Settlements_CycleWaterColors(void);
void Scroll_StartHBlank(void);
void Settlements_SinkPillar(void);
void Settlements_SlideCrateDownFalls(void);
void Settlements_RestoreWallCells(void);
void Settlements_DropCrate(void);
void Settlements_SlideOnIce(void);
void Leader_StopMoving(void);
void Event_ResolveAllPendingActions(void);
void Scene_QueueMusic(s32 a);
#ifdef MODERN
// The rope routine is called with no prototype in scope, as its bytes need:
// called as returning int, the two post ids load before the first argument.
// A current GCC gets it here.
void Rope_Lash(s32 a, s32 b, s32 c);
#endif
void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);
void Settlements_EnterDoor(void);
void Settlements_DriftSmoke(struct Actor *object);
void Actor_SelectAnimation(struct Actor *object, s32 a);
void Actor_Release(struct Actor *object);
extern s32 gSettlements_WaterfallLine;
extern u16 gSettlements_WaterfallScrollAbove;
extern u16 gSettlements_WaterfallScrollBelow;
void Scroll_HBlank(void);
void Layer_TrackScroll(void);
void Irq_SetHandler(s32 a, s32 b, void (*callback)(void));
void Event_End(void);
void ItemDisplay_ShowItem(s32 a, s32 b);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);

extern u32 gSettlements_Room5Regions[];
extern u32 gSettlements_Room3Regions[];

static __inline__ void Window(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

#endif
