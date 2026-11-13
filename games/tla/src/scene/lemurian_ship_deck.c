/// @file
/// The scene code of overlay 0x657 that its scene tables belong to, in ROM order; the rest of the overlay is in lemurian_ship_deck/.

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
#include "main_routine_work.h"
#include "scene_ids.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"
#include "io_reg.h"
#include "map.h"
#include "frame.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_020022cc[];
extern const u8 gData_0200235c[];

const u8 *Func_0200004c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov657Room1)
    return gData_0200235c;
  return gData_020022cc;
}

void Func_02001f5c(void);
void Func_02001f64(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02000078(void) {
  if (Event_GetFlag(0x985)) {
    u8 *work = Ram_HeapSlots->eventWork;
    u32 i;

    Func_02001f5c();
    FieldEvent_ResolvePendingActions(0);
    // The loop is written with goto so that it carries no loop notes: the bytes
    // rebuild the zero and the height on every pass, and a for loop hoists both
    // (differ 39).
    i = 8;
  loop:
    {
      struct Actor *object = Actor_Get(i);

      if (object != 0) {
        object->motionFlags = 0;
        object->y = PIXELS(32);
      }
    }
    i++;
    if (i <= 63)
      goto loop;
    if (!Event_GetFlag(0x201)) {
      Event_PlaySound(158);
      MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2);
    }
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
    Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -4);
    FieldEvent_Wait(8);
    Event_PlaySound(123);
    FieldEvent_SetExitRequest(*(s16 *)(work + 0x154));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02001f64();
  }
}

extern const u8 gData_020023ec[];
extern const u8 gData_020023f8[];

const u8 *Func_0200013c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov657Room2)
    return gData_020023f8;
  return gData_020023ec;
}

extern const u8 MsgTheDoorIsHeld[];

void Func_02000168(void) {
  Func_02001f5c();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgTheDoorIsHeld, 1);
  Func_02001f64();
}

#define OBJ_PALETTE ((volatile u16 *)&BG_PLTT_COLOR(11, 0))

void Func_02000188(void) {
  if ((gFrameCount & 7) == 0) {
    s16 last = OBJ_PALETTE[15];
    s32 i;

    // The loop is written with goto so that it carries no loop notes: the bytes
    // rebuild the palette base on every pass, and a for or do loop hoists it
    // and steps two pointers (differ 30).
    i = 15;
  loop:
    OBJ_PALETTE[i] = OBJ_PALETTE[i - 1];
    i--;
    if (i != 12)
      goto loop;
    OBJ_PALETTE[12] = last;
  }
}

void Func_020001cc(void) {
  if ((gFrameCount & 7) == 0) {
    s16 last = OBJ_PALETTE[9];
    s32 i;

    // The loop is written with goto so that it carries no loop notes: the bytes
    // rebuild the palette base on every pass, and a for or do loop hoists it
    // and steps two pointers (differ 30).
    i = 9;
  loop:
    OBJ_PALETTE[i] = OBJ_PALETTE[i - 1];
    i--;
    if (i != 4)
      goto loop;
    OBJ_PALETTE[4] = last;
  }
}

void Func_02001f34(s32 a);
void Event_StartScreenTransition(void);

void Func_02000210(void) {
  Func_02001f5c();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  FieldTask_AddFrame(Func_020001cc, 0x480);
  Event_WaitTransitionFrames();
  FieldEvent_Wait(60);
  Func_02001f34(13);
  FieldEvent_Wait(60);
  FieldEvent_Wait(60);
  Func_02001f34(14);
  Func_02001f64();
}

#define LAYER_WORD(layer, offset) (*(s32 *)((u8 *)(layer) + (offset)))

extern s32 gData_020024b4;
extern s32 gData_020024b0;

void Func_02000258(void) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x108);
  s32 phase = gData_020024b4;

  layer[2] = phase;
  phase += 0x8000;
  gData_020024b4 = phase;
  if (phase == 0x200000)
    gData_020024b4 = 0;
}

/// Bobs the object returned by Func_080d4568 and the first map layer's word 12
/// together on a sine wave, its phase advancing 0x200 a frame.
void Func_02000288(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[0];
  struct Actor *object = FieldCamera_GetActor();
  s32 offset = Math_Sin(gData_020024b0) >> 2;

  object->y += offset;
  LAYER_WORD(layer, 12) += offset;
  gData_020024b0 += 0x200;
}
