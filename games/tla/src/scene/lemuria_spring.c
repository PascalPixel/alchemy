#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02001358();
extern u8 MsgAndTheOthers[];

void Func_02000054(void) {
  Message_ShowCentered((s32)MsgAndTheOthers, 1);
  Event_PlaySound(0x7e);
  Func_02001358(1);
}

extern const u16 gData_02001678[];

static inline void RegSet(s32 value, s32 reg) {
  /* FAKEMATCH(wrapper): the store forwarded through this inline with s32
   * parameters, so 0x3f42 is built in r2 and the register address held in r3;
   * as direct stores to REG_BLDCNT and REG_BLDALPHA, 0x3f42 is a pool word, 27
   * of 40 instructions differ in tla-en. */
  *(u16 *)reg = value;
}

void Func_02000070(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;

  Event_PlaySound(216);
  layer = (s32 *)(map + 0x178);
  i = 15;
next:
  layer[3] -= 0x10000;
  System_Wait(4);
  i--;
  if (i >= 0)
    goto next;
  i = 0;
next2:
  RegSet(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
         (s32)&REG_BLDCNT);
  RegSet(gData_02001678[i], (s32)&REG_BLDALPHA);
  System_Wait(8);
  i++;
  if (i <= 7)
    goto next2;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02001380();
s32 Func_020001e4();
s32 Func_02001388();

void Func_020000d0(void) {
  s32 t0;
  s32 actor = FIELD_AT(&gMainRoutineWork, s32, 532);

  t0 = Event_GetFlag(0x200);
  if (t0 == 0) {
    Event_SetFlag(0x200);
    Func_02000070();
  }
  Func_02001380();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(actor, 120, 152);
  Actor_FaceDirection(actor, FACING_SOUTH, 0);
  Func_020001e4();
  Func_02001388();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }
