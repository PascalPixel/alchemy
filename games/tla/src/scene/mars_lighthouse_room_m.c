/// @file
/// The Mars Lighthouse hall of the four inscriptions, in the rooms of this
/// overlay: the party reads the inscriptions of earth, water, fire and wind
/// (MarsLighthouseRoomM_ReadEarthInscription and its siblings) and hears the
/// dragon statue (MarsLighthouseRoomM_HearDragonStatue), and slides on its
/// ice (MarsLighthouseRoomM_SlideOnIce).

#include "shared/mars_lighthouse.h"
#include "flags.h"
#include "shared/actors.h"
#include "shared/effects.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/scene/scene.h"
#include "module/sound/sound.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "actor.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "system/iwram_call.h"
#include "system/io_write_queue.h"
#include "system/io_reg.h"
#include "call.h"
#include "version.h"
#include "system/dma.h"
#include "shared/mars_lighthouse_entrance_room.h"
#include "field/event_work.h"
#include "module/field/field.h"
#include "module/scene/dehkan_plateau.h"
#include "battle/battle_unit.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "shared/tables.h"
#include "graphics/display_transition.h"
#include "item_icon.h"
#include "map.h"
#include "field_sprite.h"
#include "macros.h"
#include "objcmd.h"

struct Actor;
struct Vector3;

void Leader_KnockBack(void);

s32 MarsLighthouseRoomM_SlideOnIce(void);
void Actor_LaunchAhead(struct Actor *self);

void Func_02001608(void);

void Func_0200055c(void);

void Func_02001324(s32 x, s32 z, s32 c, s32 d, s32 e, s32 f);

void Func_020000c0(void);

void Func_020003c4(void);

void Func_02000934(s32 entrance);

void Func_02002118(void);

void Func_0200080c(void);

void Actor_Blink(struct Actor *object);

extern u32 gData_02003304[];
extern u32 gData_02003348[];

void Func_020002c8(void);

void Func_020002e8(void);

void Func_020002fc(void);

void Func_02000abc(void);

void MarsLighthouseRoomM_ReadEarthInscription(void);
void MarsLighthouseRoomM_ReadWaterInscription(void);
void MarsLighthouseRoomM_ReadFireInscription(void);

void MarsLighthouseRoomM_ReadWindInscription(void);

void MarsLighthouseRoomM_HearDragonStatue(void);

extern s32 gData_020032c8[];

#include "text/msg_ids.h"

extern u16 gData_02004cf0[];
void Actor_Release(void *handle);
void FieldCamera_MoveToActorById(s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Teleport_PlaySparks(void);
extern u16 Data_020039bc[];
extern volatile u16 Data_020039dc[];
extern u16 Data_02003a1c[];
extern const u16 Data_0200ba70[];
extern s16 Data_02003a7c;
extern s16 gData_02003a7e;
extern s16 gData_02003a80;
extern s16 gData_02003a82;
void TileMap_ConvertBuffer(s32 scene, s32 a1);
u32 Map_ClearLayerEntryFlag(struct FieldSprite *sprite, u32 frame);

void FieldEffect_StartBlankOverlay(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

struct RideSlot {
  u16 value;
  u8 unk02[2];
};

struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[12];
  void *handle;
};

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

void Func_0200166c(void);

s32 Func_0200004c(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

void Func_02000058(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 0;
  object->y = PIXELS(14);
  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, value);
}

#ifndef TLA_MARS_LIGHTHOUSE_ROOM_M_H
#define TLA_MARS_LIGHTHOUSE_ROOM_M_H

#include "types.h"
#include "system/system.h"
#include "scene_tables.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "map.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "text/msg_ids.h"
#include "system/frame.h"

struct MarsLighthouseRoomMEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[0x10];
  s16 field166;
  u8 unk168[0x44];
  s32 field1ac;
};

struct MarsLighthouseRoomMVector {
  s32 x;
  s32 y;
  s32 z;
};

struct MarsLighthouseRoomMAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
  u8 unk0a[6];
  u8 value10;
  u8 flag11 : 1;
  u16 angle;
};

struct MarsLighthouseRoomMRideWork {
  s32 counter;
  s16 active;
  u8 unk06[6];
  u16 value0c;
  s16 value0e;
  u8 unk10[0x60];
  struct Actor *handle;
};

extern u8 gInventorySnapshot[];
extern char SceneId_MarsLighthouseRoomM1;
extern char SceneId_MarsLighthouseRoomM2;
extern char SceneId_MarsLighthouseRoomM3;
extern char SceneId_MarsLighthouseRoomM4;
extern char SceneId_MarsLighthouseRoomM5;
extern char SceneId_MarsLighthouseRoomM6;

void Actor_SelectAnimation(struct Actor *object, s32 value);
void Actor_StartScript(struct Actor *object, const void *script);

void Ride_UpdateCarried(struct Actor *object);
void Actor_ResetScripted(struct Actor *object);
void Actor_SetupScriptedPart(struct Actor *object, s32 layer);
void Ride_UpdateLanding(struct Actor *object);

extern u32 gData_020032d4[];
extern struct ActorPlacement gMarsLighthouseRoomM_Placements[];
extern struct ActorPlacement gMarsLighthouseRoomM_Room6Placements[];
extern struct ActorPlacement gMarsLighthouseRoomM_Room1Placements[];

extern u16 gData_02003918;
extern u16 gData_02003920;
extern u16 gData_02003924;
extern u16 *gData_02003928;
extern u16 gData_02003934;
extern u16 gData_02003938;
extern struct Trigger gMarsLighthouseRoomM_Triggers[];
extern struct Trigger gMarsLighthouseRoomM_Room6Triggers[];
extern struct Trigger gMarsLighthouseRoomM_Room1Triggers[];
extern struct Trigger gMarsLighthouseRoomM_Room5Triggers[];
extern s32 gData_0200387c[];
extern u32 gData_02003900[];

s32 List_CountEntries(u16 *list);
void PaletteCycle_Fade(void);
void Func_0200166c(void);
void Func_0200168c(void);
s32 Func_02001de0(struct Actor *a, struct Actor *b);
void Func_02001e38(struct Actor *object);
void Actor_SetupScripted(struct Actor *object);
void Func_020012a4(s32 x, s32 z, s32 kind);
void Func_02002ad4(struct Actor *object);
void Event_End(void);
void Teleport_PlayWhirl(void);
s32 Trig_Cos(s32 angle);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

#endif

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_MarsLighthouseRoomM6)
    return gMarsLighthouseRoomM_Room6Placements;
  if (scene == (s32)&SceneId_MarsLighthouseRoomM1)
    return gMarsLighthouseRoomM_Room1Placements;
  return gMarsLighthouseRoomM_Placements;
}

extern s32 gData_020036c4;
extern u16 gData_0200393c[];
extern u16 gData_0200397c[];

void Func_020000c0(void) {
  s32 entrance = gMainRoutineWork.entrance;
  struct Overlay6acEventWork *work = Ram_HeapSlots->eventWork;
  s32 base;
  s32 i;
  s8 busy;

  if (work->unk166 != 0)
    return;
  busy = work->unkca4;
  if (busy != 0)
    return;
  if (gData_020036c4 == -1) {
    Iwram_CopyWordsEntry(Data_020039bc, (void *)0x050001a0, 32);
    Iwram_CopyWordsEntry(gData_0200393c, (void *)0x05000100, 64);
    Iwram_CopyWordsEntry(gData_0200397c, gData_0200393c, 64);
    gData_020036c4 = 0;
  }
  base = gData_020036c4 / 8 % 6;
  Iwram_CopyWordsEntry(gData_0200397c, Data_020039bc, 32);
  if (entrance == 1 && Event_GetFlag(0xa32)) {
    for (i = 0; i <= 5; i++)
      gData_0200397c[i + 10] = gData_0200393c[(base + i) % 6 + 10];
  }
  if (entrance == 2 && Event_GetFlag(0xa33)) {
    for (i = 0; i <= 5; i++)
      gData_0200397c[i + 10] = gData_0200393c[(base + i) % 6 + 20];
  }
  if (entrance == 3 && Event_GetFlag(0xa34)) {
    for (i = 0; i <= 5; i++)
      gData_0200397c[i + 10] = gData_0200393c[(base + i) % 6 + 26];
  }
  if (entrance == 4 && Event_GetFlag(0xa35)) {
    for (i = 0; i <= 5; i++)
      gData_0200397c[i + 10] = gData_0200393c[(base + i) % 6 + 4];
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(13, 0), (u32)gData_0200397c,
                      0x84000008);
  gData_020036c4++;
}

void Func_020002c8(void) {
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(gMainRoutineWork.entrance);
}

void Func_020002e8(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  event[3384] = 1;
}

void Func_020002fc(void) {
  FieldCamera_MoveToActorById(10, 1);
  FieldCamera_WaitForArrival();
}

void Func_0200030c(void) {
  struct MarsLighthouseRoomMEventWork *work;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_WARP);
  switch (gMainRoutineWork.entrance) {
  case 1:
    Event_SetFlag(0xa32);
    break;
  case 2:
    Event_SetFlag(0xa33);
    break;
  case 3:
    Event_SetFlag(0xa34);
    break;
  case 4:
    Event_SetFlag(0xa35);
    break;
  }
  work = Ram_HeapSlots->eventWork;
  work->field166 = 0;
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(30);
  FieldEvent_Wait(10);
  Event_End();
}

extern s32 gData_02003704;
extern u16 gData_02003a5c[];

void Func_020003c4(void) {
  struct Overlay6acEventWork *work = Ram_HeapSlots->eventWork;
  s32 base;
  s32 i;
  s8 busy;

  if (work->unk166 != 0)
    return;
  busy = work->unkca4;
  if (busy != 0)
    return;
  if (gData_02003704 == -1) {
    Iwram_CopyWordsEntry(gData_02003a5c, (void *)0x05000040, 32);
    Iwram_CopyWordsEntry((void *)Data_020039dc, (void *)0x05000100, 64);
    Iwram_CopyWordsEntry(Data_02003a1c, (void *)Data_020039dc, 64);
    gData_02003704 = 0;
  }
  /* FAKEMATCH(volatile): the saved colours read as volatile. */
  base = gData_02003704 / 8 % 6;
  for (i = 0; i <= 5; i++) {
    Data_02003a1c[i + 4] = Data_020039dc[(base + i) % 6 + 4];
    Data_02003a1c[i + 10] = Data_020039dc[(base + i) % 6 + 10];
    Data_02003a1c[i + 20] = Data_020039dc[(base + i) % 6 + 20];
    Data_02003a1c[i + 26] = Data_020039dc[(base + i) % 6 + 26];
  }
  if (!Event_GetFlag(0xa32))
    Iwram_CopyWordsEntry(&Data_02003a1c[10], Data_0200ba70, 12);
  if (!Event_GetFlag(0xa33))
    Iwram_CopyWordsEntry(&Data_02003a1c[20], Data_0200ba70, 12);
  if (!Event_GetFlag(0xa34))
    Iwram_CopyWordsEntry(&Data_02003a1c[26], Data_0200ba70, 12);
  if (!Event_GetFlag(0xa35))
    Iwram_CopyWordsEntry(&Data_02003a1c[4], Data_0200ba70, 12);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(8, 0), (u32)Data_02003a1c,
                      0x84000010);
  gData_02003704++;
}

/// Scheduled by Func_02000934: steps room 1's blend (Data_02003a7c) one toward
/// its target (gData_02003a7e) each frame and queues the BLDALPHA write; as it
/// reaches 0 or 16, sets the cells the step (gData_02003a80) selects. The cell
/// calls go through Call6, as in Func_020006c8.
void Func_0200055c(void) {
  s32 step;

  if (Data_02003a7c == gData_02003a7e)
    return;
  step = gData_02003a7e - Data_02003a7c;
  if (step < 0)
    step = -1;
  else
    step = 1;
  Data_02003a7c += step;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      BLEND_ALPHA(Data_02003a7c, 16 - Data_02003a7c), 0x20000);
  if (Data_02003a7c == 0) {
    if (gData_02003a80 == 0)
      MapGrid_CopyCellAttributes(34, 8, 3, 3, 17, 8);
    if (gData_02003a80 == 1)
      MapGrid_CopyCellAttributes(30, 8, 3, 3, 8, 8);
    if (gData_02003a80 == 2)
      MapGrid_CopyCellAttributes(34, 8, 3, 3, 17, 8);
    if (gData_02003a80 == 3)
      MapGrid_CopyCellAttributes(30, 8, 3, 3, 8, 8);
  }
  if (Data_02003a7c == 16) {
    if (gData_02003a80 == 0)
      MapGrid_CopyCellAttributes(34, 0, 3, 3, 17, 8);
    if (gData_02003a80 == 1)
      MapGrid_CopyCellAttributes(30, 4, 3, 3, 8, 8);
    if (gData_02003a80 == 2)
      MapGrid_CopyCellAttributes(34, 4, 3, 3, 17, 8);
    if (gData_02003a80 == 3)
      MapGrid_CopyCellAttributes(30, 0, 3, 3, 8, 8);
  }
}

/// Moves the Func_0200080c cells for a new state: the cells of the old index
/// (gData_02003a80) are put back with (0, 122, 3, 3), gData_02003a7e becomes 0
/// for an odd state and 16 for an even one, the index becomes (state - 20) / 2
/// and its cells are set with (0, 125, 3, 3). Sound 560 plays when the low bit
/// of the state changes.
void Func_020006c8(s32 state) {
  s32 next;
  s32 odd;
  u16 *flash;

  if (state == gData_02003a82)
    return;
  if ((state ^ gData_02003a82) & 1)
    Event_PlaySound(SE_560);
  gData_02003a82 = state;
  /* FAKEMATCH(temporary): the low bit held in a local. */
  odd = state & 1;
  flash = (u16 *)&gData_02003a7e;
  next = state - 20;
  if (odd)
    *flash = 0;
  else
    *flash = 16;
  if (gData_02003a80 == 0)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 67);
  if (gData_02003a80 == 1)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 72);
  if (gData_02003a80 == 2)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 72);
  if (gData_02003a80 == 3)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 67);
  gData_02003a80 = next / 2;
  if (gData_02003a80 == 0)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 67);
  if (gData_02003a80 == 1)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 72);
  if (gData_02003a80 == 2)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 72);
  if (gData_02003a80 == 3)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 67);
}

/// Calls MapGrid_CopyCellPictureCellsRect with (0, 125, 3, 3) for four places,
/// steps gData_02003a7e through 12 and 8 to 0, then calls it with (0, 122, 3,
/// 3) for the same places.
void Func_0200080c(void) {
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 67);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 72);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 72);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 67);
  System_Wait(1);
  gData_02003a7e = 12;
  System_Wait(12);
  gData_02003a7e = 8;
  System_Wait(4);
  gData_02003a7e = 12;
  System_Wait(4);
  gData_02003a7e = 8;
  System_Wait(4);
  gData_02003a7e = 12;
  System_Wait(4);
  gData_02003a7e = 0;
  System_Wait(12);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 67);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 72);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 72);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 67);
  System_Wait(1);
}

void BlendScript_Suspend(void);

void Func_02000934(s32 entrance) {
  s32 cell = -1;
  s16 *blend;
  BlendScript_Suspend();
  blend = &Data_02003a7c;
  if (entrance == 8)
    cell = 20;
  if (entrance == 10)
    cell = 22;
  if (entrance == 12)
    cell = 24;
  if (entrance == 14)
    cell = 26;
  if (cell == -1) {
    *blend = 0;
    gData_02003a7e = 0;
    gData_02003a80 = cell;
    gData_02003a82 = cell;
  } else {
    s32 step;
    *blend = 15;
    gData_02003a7e = 16;
    step = (cell - 20) / 2;
    gData_02003a80 = step;
    gData_02003a82 = cell;
    if (step == 0)
      MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 67);
    if (gData_02003a80 == 1)
      MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 72);
    if (gData_02003a80 == 2)
      MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 72);
    if (gData_02003a80 == 3)
      MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 67);
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      Data_02003a7c | (16 - Data_02003a7c) << 8, 0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f42, 0x20000);
  Task_AddFrame(Func_0200055c, 0x480);
}

void Func_02000abc(void) {
  Func_02001608();
  Teleport_PlaySparks();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[706] = 2;
  }
  FieldEvent_SetExitRequest(1);
  FieldEvent_Wait(40);
  Func_0200166c();
  FieldTask_Remove(Func_0200055c);
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_MarsLighthouseRoomM6)
    return gMarsLighthouseRoomM_Room6Triggers;
  if (scene == (s32)&SceneId_MarsLighthouseRoomM1)
    return gMarsLighthouseRoomM_Room1Triggers;
  if (scene == (s32)&SceneId_MarsLighthouseRoomM2 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM3 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM4 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM5)
    return gMarsLighthouseRoomM_Room5Triggers;
  return gMarsLighthouseRoomM_Triggers;
}

void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b);
/* VIEW(s0b64): routine Actor_LaunchAhead as void Actor_LaunchAhead(void);
 * mars_lighthouse_room_m has void Actor_LaunchAhead(struct Actor *self); not
 * tried: Actor_LaunchAhead's bound declaration was not found. */
extern void Func_02002e6c__s0b64(void) __asm__("Actor_LaunchAhead");

extern s16 gData_0200338c[];
extern s16 gData_0200339c[];
extern s16 gData_020033c8[];
extern u32 gData_020033f4[];

s32 Scene_Initialize(void) {
  s32 entrance;
  s32 scene;

  scene = gMainRoutineWork.scene;
  entrance = gMainRoutineWork.entrance;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x100;
  Event_SetFlag(FLAG_MARS_LIGHTHOUSE_LIT);
  Call4((void (*)())Palette_SetColorsFade, (s32)gData_0200338c,
        (s32)gData_0200339c, (s32)gData_020033c8, (s32)gData_020033f4);
  if (scene != (s32)&SceneId_MarsLighthouseRoomM6 &&
      scene == (s32)&SceneId_MarsLighthouseRoomM1) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x204;
    if ((u32)(entrance - 1) <= 1 || entrance == 8 || entrance == 10 ||
        entrance == 12 || entrance == 14) {
      s32 unset = Event_GetFlag(0xa31);

      if (unset == 0 && Event_GetFlag(0xa32) && Event_GetFlag(0xa33) &&
          Event_GetFlag(0xa34) && Event_GetFlag(0xa35)) {
        struct Actor *actor;

        Event_Start();
        FieldEvent_ResolvePendingActions(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Event_PlaySound(SOUND_FADE_OUT_BGM);
        FieldCamera_SetSpeed(2);
        FieldCamera_MoveTo(224, 64, 216, 1);
        FieldCamera_WaitForArrival();
        FieldEvent_Wait(20);
        PaletteFade_ApplySource(0x10000, 0);
        PaletteFade_ApplyTarget(0x10003, 0);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        entrance = 0x500021e;
        *(u16 *)entrance = 0x7fff;
        System_Wait(1);
        EventMessage_ShowCentered((s32)MsgYouHaveProven, 0, 4);
        PaletteFade_ApplyTarget(0x10000, 0);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        FieldCamera_SetSpeed(1);
        FieldCamera_MoveTo(224, 16, 216, 1);
        FieldCamera_WaitForArrival();
        FieldEvent_Wait(40);
        actor = Actor_Get(16);
        actor->motionFlags = unset;
        actor->y = 0x100000;
        actor->terrainHeight = 0x100000;
        ((u8 *)actor)[98] = 18;
        actor->unk66 = 0x4000;
        actor->watchedObject = (struct Actor *)unset;
        Func_02002e6c__s0b64();
        System_Wait(40);
        MapGrid_SetShake(2, 2, 1);
        Func_02001324(216, 216, 65, 65, 13, 13);
        Func_02001324(232, 216, 66, 65, 14, 13);
        Func_02001324(232, 232, 66, 66, 14, 14);
        Func_02001324(216, 200, 65, 64, 13, 12);
        Func_02001324(248, 200, 67, 64, 15, 12);
        Func_02001324(248, 232, 67, 66, 15, 14);
        Func_02001324(216, 232, 65, 66, 13, 14);
        Func_02001324(200, 216, 64, 65, 12, 13);
        Func_02001324(232, 200, 66, 64, 14, 12);
        Func_02001324(248, 216, 67, 65, 15, 13);
        Func_02001324(200, 232, 64, 66, 12, 14);
        Func_02001324(200, 200, 64, 64, 12, 12);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        MapGrid_CopyCellAttributes(30, 12, 4, 3, 12, 12);
        FieldEvent_Wait(40);
        Event_SetFlag(0xa31);
        Event_PlaySound(MUS_080);
        Sound_WaitForBgmRestore();
        Scene_PlayMusic();
        Event_End();
      }
    }
  }
  return 0;
}

/* VIEW(s0b64): routine Func_02000058 as s32 Func_02000058(); the definition has
 * void Func_02000058(s32 id, s32 value); tried as the definition's void
 * Func_02000058(void), it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_m.c:696: conflicting types for
 * `Func_02000058'. */
extern s32 Func_02000058__s0b64() __asm__("Func_02000058");
/* VIEW(mars_lighthouse_room_m): routine Map_ClearLayerEntryFlag as void
 * Map_ClearLayerEntryFlag(s32 value); module/scene/scene.h has u32
 * Map_ClearLayerEntryFlag(struct FieldSprite *sprite, u32 frame); not tried:
 * Map_ClearLayerEntryFlag's bound declaration was not found. */
extern void Func_02003150__mars_lighthouse_room_m(s32 value) __asm__(
    "Map_ClearLayerEntryFlag");
void Weather_StartRain(s32 a);

static __inline__ void SetBlend(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                s32 g) {
  LineScroll_InitWave(a, b, c, d, e, f, g);
}

s32 Scene_PrepareMap(void) {
  s16 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;

  if (scene == (s32)&SceneId_MarsLighthouseRoomM6 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM2 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM3 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM4 ||
      scene == (s32)&SceneId_MarsLighthouseRoomM5) {
    Weather_StartRain(1);
    SetBlend(1, 0x10000, 0x40000, 0x20000, 0x10000, 0x8000, 0x4000);
  }
  if (scene == (s32)&SceneId_MarsLighthouseRoomM6) {
    switch (entrance) {
    case 1:
      FieldActor_SetSpriteColor(Actor_Get(8), 1);
      Func_02000058__s0b64(9, 4);
      Func_02003150__mars_lighthouse_room_m(4);
      Func_02003150__mars_lighthouse_room_m(5);
      break;
    case 2:
      FieldActor_SetSpriteColor(Actor_Get(8), 3);
      Func_02000058__s0b64(9, 5);
      Func_02003150__mars_lighthouse_room_m(6);
      Func_02003150__mars_lighthouse_room_m(7);
      break;
    case 3:
      FieldActor_SetSpriteColor(Actor_Get(8), 0);
      Func_02000058__s0b64(9, 2);
      Func_02003150__mars_lighthouse_room_m(0);
      Func_02003150__mars_lighthouse_room_m(1);
      break;
    case 4:
      FieldActor_SetSpriteColor(Actor_Get(8), 4);
      Func_02000058__s0b64(9, 3);
      Func_02003150__mars_lighthouse_room_m(2);
      Func_02003150__mars_lighthouse_room_m(3);
      break;
    }
    object = Actor_Get(10);
    object->motionFlags = 0;
    object->z = 0x680000;
    FieldTask_AddFrame(Func_020000c0, 0x480);
  } else if (scene == (s32)&SceneId_MarsLighthouseRoomM1) {
    if (entrance == 1 || entrance == 2 || entrance == 8 || entrance == 10 ||
        entrance == 12 || entrance == 14) {
      Func_02002118();
      if (Event_GetFlag(0xa31)) {
        MapGrid_CopyCellAttributes(30, 12, 4, 3, 12, 12);
        MapGrid_CopyCellsLayered(64, 64, 4, 3, 76, 12);
      }
      Func_02000934(entrance);
      {
        s32 priority = 0x480;

        FieldTask_AddFrame(Func_020003c4, priority);
      }
      Func_02000058__s0b64(12, 4);
      Func_02000058__s0b64(13, 5);
      Func_02000058__s0b64(14, 2);
      Func_02000058__s0b64(15, 3);
      if (!Event_GetFlag(0xa32))
        Actor_SetPosition(12, 0, 0);
      if (!Event_GetFlag(0xa33))
        Actor_SetPosition(13, 0, 0);
      if (!Event_GetFlag(0xa34))
        Actor_SetPosition(14, 0, 0);
      if (!Event_GetFlag(0xa35))
        Actor_SetPosition(15, 0, 0);
    }
  }
  if (Event_GetFlag(0xa30)) {
    extern char SceneId_MarsLighthouseRoomM3Second __asm__(
        "SceneId_MarsLighthouseRoomM3");

    TileMap_ConvertBuffer(
        gMainRoutineWork.map +
            ((s32)&SceneId_MarsLighthouseRoomM3Second - (s32)&SceneId_Prox2),
        0x5000000);
  }
  return 0;
}

void MarsLighthouseRoomM_ReadEarthInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered((s32)MsgMankindWithThe, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

void MarsLighthouseRoomM_ReadWaterInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered((s32)MsgFishWithCold, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

void MarsLighthouseRoomM_ReadFireInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered((s32)MsgDragonsBurningWith, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

void MarsLighthouseRoomM_ReadWindInscription(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  EventMessage_ShowCentered((s32)MsgBirdOnThe, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Event_End();
}

void Func_020012a4(s32 x, s32 z, s32 kind) {
  struct Actor *object;

  /* FAKEMATCH(temporary): z shifted ahead of the call. */
  z <<= 16;
  object = (struct Actor *)Import_ActorCreate(403, x << 16, 0, z);

  if (object != 0) {
    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, 3);
    if (kind != 0) {
      ((struct MarsLighthouseRoomMAnimation *)object->sprite)->layer = 2;
      Actor_StartScript(object, gData_0200387c);
    } else {
      ((struct MarsLighthouseRoomMAnimation *)object->sprite)->layer = 1;
      object->velocityX = (((u32)(Random16() * 8) >> 16) - 4) << 16;
      object->velocityY = PIXELS(4);
      Actor_StartScript(object, gData_02003900);
    }
  }
}

void Func_02001324(s32 x, s32 z, s32 c, s32 d, s32 e, s32 f) {
  u32 i;
  s32 px;
  s32 pz;

  Event_PlaySound(SE_CLATTER);
  Func_020012a4(x, z + 12, 1);
  for (i = 0; i < 3; i++) {
    px = x + ((u32)(Random16() * 12) >> 16) - 6;
    pz = z + ((u32)(Random16() * 12) >> 16) - 6;
    Func_020012a4(px, pz, 0);
  }
  wait(4);
  MapGrid_CopyCellsTo(c, d, e + 64, f, 1, 1);
}

/// No one holding item 222: MsgIfYouDo, once (flag 0xa2f). Someone holding it
/// (and flag 0xa30 set, from patch 1): MsgYouWhoHold and the
/// message after it, the camera moves on and Func_0200080c runs, then flag
/// 0xa2e is set. Each opens as MarsLighthouseRoomM_ReadEarthInscription does.
void MarsLighthouseRoomM_HearDragonStatue(void) {
  if (PartyInventory_FindMember(222) == -1) {
    if (!Event_GetFlag(0xa2f)) {
      u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);

      Event_Start();
      Event_ResolvePendingActions(0);
      FieldCamera_SetSpeed(2);
      FieldCamera_MoveTo(224, 64, 216, 1);
      FieldCamera_WaitForArrival();
      FieldEvent_Wait(20);
      PaletteFade_ApplySource(0x10000, 0);
      BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
      PaletteFade_StartInterpolation(20);
      wait(20);
      *color = 0x7fff;
      wait(1);
      EventMessage_ShowCentered((s32)MsgIfYouDo, 0, 4);
      BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
      PaletteFade_StartInterpolation(20);
      wait(20);
      Event_SetFlag(0xa2f);
      Event_End();
    }
  }
#if !TLA_PATCH_1
  else {
#else
  else if (Event_GetFlag(0xa30)) {
#endif
    u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveTo(224, 64, 216, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(20);
    PaletteFade_ApplySource(0x10000, 0);
    BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
    PaletteFade_StartInterpolation(20);
    wait(20);
    *color = 0x7fff;
    wait(1);
    EventMessage_ShowCentered((s32)MsgYouWhoHold, 0, 4);
    wait(10);
    EventMessage_ShowCentered((s32)MsgYouWhoHold + 1, 0, 3);
    FieldCamera_MoveTo(224, 88, 216, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(20);
    Event_PlaySound(SE_560);
    Func_0200080c();
    FieldEvent_Wait(20);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    PaletteFade_StartInterpolation(20);
    wait(40);
    Event_SetFlag(0xa2e);
    Event_End();
  }
}

extern u16 *Data_0200392c;
extern u16 *Data_02003930;
extern u16 *Data_0200391c;

void Func_0200183c(void);

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
DEFINE_PALETTE_SET_COLORS_FADE(Palette_SetColorsFade, gData_02003928,
                               Data_0200392c, Data_02003930, Data_0200391c,
                               Func_0200183c)

#define DEFINE_FUNC_02001608(name, n1, n2)                                     \
  void name(void) {                                                            \
    gData_02003938 = 0;                                                        \
    gData_02003934 = List_CountEntries(gData_02003928);                        \
    gData_02003924 = n1;                                                       \
    gData_02003920 = n2;                                                       \
    gData_02003918 = 1;                                                        \
    Task_AddFrame(PaletteCycle_Fade, 0x480);                                   \
  }

DEFINE_FUNC_02001608(Func_02001608, 15, 16)

extern u8 gInventorySnapshot[];
extern u16 gData_02004cf0[];
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Actor_Release(void *handle);
void Leader_KnockBack(void);
s32 MarsLighthouseRoomM_SlideOnIce(void);
void Actor_LaunchAhead(struct Actor *self);

void Actor_SelectAnimation(struct Actor *object, s32 value);
void FieldCamera_MoveToActorById(s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Func_02001608(void);
void Teleport_PlaySparks(void);
void Func_0200166c(void);
void Func_0200055c(void);
void PaletteCycle_Fade(void);

void Func_0200166c(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  FieldTask_Remove(PaletteCycle_Fade);
}

DEFINE_FUNC_02001608(Func_0200168c, 2, 2)

DEFINE_LIST_COUNT_ENTRIES(List_CountEntries)

extern u16 *Data_0200391c;
extern u16 *Data_0200392c;
extern u16 *Data_02003930;

DEFINE_PALETTE_CYCLE_FADE(PaletteCycle_Fade, gData_02003920, gData_02003924,
                          Data_02003930, gData_02003934, Data_0200391c,
                          gData_02003938, gData_02003928, gData_02003918,
                          Data_0200392c)

struct MarsLighthouseRoomMEventWork1ac {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Func_020017b4(void);

void Func_0200183c(void) {
  struct MarsLighthouseRoomMEventWork1ac *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  PaletteFade_ApplyTarget(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  ((struct MarsLighthouseRoomMEventWork1ac *)Ram_HeapSlots->eventWork)
      ->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200168c();
  Teleport_PlayWhirl();
  System_Wait(40);
  Func_0200166c();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

s32 Func_020018c4(struct Actor *a, struct Actor *b) {
  if ((a->x - b->x >= 0 ? a->x - b->x : b->x - a->x) < 0x80000 &&
      (a->y - b->y >= 0 ? a->y - b->y : b->y - a->y) < 0x80000 &&
      (a->z - b->z >= 0 ? a->z - b->z : b->z - a->z) < 0x80000)
    return 1;
  return 0;
}

s32 Func_02001928(struct Actor *a, struct Actor *b) {
  s32 height;

  if ((a->x - b->x >= 0 ? a->x - b->x : b->x - a->x) < 0xc0000) {
    height = a->y - b->y;
    if (height > -0x80000 && height < 0x180000) {
      if ((a->z - b->z >= 0 ? a->z - b->z : b->z - a->z) < 0xc0000)
        return 1;
    }
  }
  return 0;
}

/* VIEW(s1988): tag struct RideSlot as struct RideSlot__s1988;
 * mars_lighthouse_room_m.c has another definition; not tried: struct
 * RideSlot__s1988 was not found. */
struct RideSlot__s1988 {
  u16 id;
  u16 blocked;
};

/* VIEW(s1988): tag struct RideWork as struct RideWork__s1988;
 * mars_lighthouse_room_m.c has another definition; not tried: struct
 * RideWork__s1988 was not found. */
struct RideWork__s1988 {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot__s1988 slots[4];
  u8 unk64[4];
  u16 value68;
  u16 direction;
};

s32 Func_020018c4(struct Actor *a, struct Actor *b);
s32 Func_02001928(struct Actor *a, struct Actor *b);

DEFINE_RIDE_UPDATE_CARRIED(Ride_UpdateCarried, RideWork__s1988, Func_020018c4,
                           Func_02001928)

void Actor_SetSpriteFlags(struct Actor *object, u32 flags);

DEFINE_ACTOR_RESET_SCRIPTED(Actor_ResetScripted, gData_020032d4)

void Func_02001b1c(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct Actor *object =
      FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = value;
    ((void (*)())Actor_ResetScripted)(object, 2);
    object->update = Ride_UpdateCarried;
    if (flag != 0)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

/* VIEW(s1b64): tag struct RideSlot as struct RideSlot__s1b64;
 * mars_lighthouse_room_m.c has another definition; not tried: struct
 * RideSlot__s1b64 was not found. */
struct RideSlot__s1b64 {
  u16 id;
  u16 blocked;
};

struct RideDirection {
  u16 unk00;
  u16 unk02;
};

/* VIEW(s1b64): tag struct RideWork as struct RideWork__s1b64;
 * mars_lighthouse_room_m.c has another definition; not tried: struct
 * RideWork__s1b64 was not found. */
struct RideWork__s1b64 {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot__s1b64 slots[4];
  u8 unk64[4];
  struct RideDirection direction;
};

extern u8 gInventorySnapshot[];

s32 Func_020018c4(struct Actor *a, struct Actor *b);
s32 Func_02001928(struct Actor *a, struct Actor *b);

DEFINE_RIDE_UPDATE_LANDING(Ride_UpdateLanding, RideWork__s1b64, Func_020018c4,
                           Func_02001928)

void Actor_SetSpriteFlags(struct Actor *object, u32 flags);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_020032d4; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
DEFINE_ACTOR_SETUP_SCRIPTED_PART(Actor_SetupScriptedPart, gData_020032d4)

void Func_02001d08(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct Actor *object =
      FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -value;
    Actor_SetupScriptedPart(object, 3);
    object->update = Ride_UpdateCarried;
    if (flag != 0)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Func_02001d50(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct Actor *object =
      FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = value;
    Actor_SetupScriptedPart(object, 3);
    object->update = Ride_UpdateLanding;
    if (flag != 0)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Func_02001d98(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct Actor *object =
      FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -value;
    Actor_SetupScriptedPart(object, 3);
    object->update = Ride_UpdateLanding;
    if (flag != 0)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

s32 Func_02001de0(struct Actor *a, struct Actor *b) {
  s32 height;

  if ((a->x - b->x >= 0 ? a->x - b->x : b->x - a->x) < 0xc0000) {
    height = a->y - b->y;
    if (height > 0 && height < 0x100000) {
      if ((a->z - b->z >= 0 ? a->z - b->z : b->z - a->z) < 0x80000)
        return 1;
    }
  }
  return 0;
}

void Func_02001e38(struct Actor *object) {
  struct Actor *target = Actor_Get(gMainRoutineWork.currentOwner);
  s32 vx;
  s32 vz;
  s32 rate;
  struct MarsLighthouseRoomMAnimation *animation;

  vx = object->bounce;
  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  rate = object->speedLimit;
  object->scaleX += rate;
  animation = object->sprite;
  object->scaleY += object->acceleration;
  animation->angle += object->timer;
  if (target->y == target->terrainHeight && Func_02001de0(object, target)) {
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 2;
    object->unk4c = 0;
  }
}

DEFINE_ACTOR_SETUP_SCRIPTED(Actor_SetupScripted, Actor_SelectAnimation,
                            gData_020032d4)

void Func_02001f14(struct MarsLighthouseRoomMVector *position, s32 value) {
  struct Actor *object = FieldActor_Create(285, position->x, position->y,
                                           position->z + 0xfffe0000);
  s32 angle;

  if (object != 0) {
    object->bounce = Trig_Cos(((u32)(Random16() * 3 << 12) >> 16) + 0x3000);
    object->gravity = (u16)Random16();
    object->unk4c = ((u32)(Random16() << 17) >> 16) + value;
    /* FAKEMATCH(temporary): the angle held in an s32 local keeps the zero
     * extension and the pool word 0xffff8000. */
    angle = (u16)Random16() - 0x8000;
    object->timer = angle;
    ((void (*)())Actor_SetupScripted)(object, 2);
    object->update = Func_02001e38;
    Actor_SetBlendMode(object, 1);
  }
}

/* VIEW(s1f94): tag struct Actor as struct Actor__s1f94; actor.h has another
 * definition; not tried: struct Actor__s1f94 was not found. */
typedef struct Actor__s1f94 {
  u8 pad0[8];
  s32 x;
  s32 y;
  s32 z;
  u8 pad14[91 - 20];
  u8 flag5b;
} Actor;

typedef struct Entry {
  s16 active;
  s16 timer;
  u16 flags;
  s16 unk_6;
  Actor *actor;
  s32 unk_c;
} Entry;

typedef struct State {
  u8 pad0[6];
  s16 unk_6;
  s16 unk_8;
  u8 pada[4];
  s16 unk_e;
  void (*callback)();
  Entry entries[4];
} State;

void Func_02001b1c();
void Func_02001d08();
void Func_02001d50();
void Func_02001d98();
/* VIEW(s1f94): routine Func_02001f14 as void Func_02001f14(); the parts before
 * it has another declaration; not tried: Func_02001f14's declaration holds a
 * FAKEMATCH tag. */
extern void Func_02001f14__s1f94() __asm__("Func_02001f14");
extern u8 gInventorySnapshot[];

typedef struct Position {
  s32 x;
  s32 z;
} Position;

typedef struct Camera {
  u8 pad0[228];
  Position position;
} Camera;

typedef struct IwramGlobals {
  u8 pad0[32];
  Camera *camera;
  u8 pad24[108 - 36];
  union {
    s32 word;
    void *pointer;
  } unk_6c;
} IwramGlobals;

#define IWRAM ((IwramGlobals *)Ram_HeapSlots)

DEFINE_BEAM_DRAW_TARGETS(Beam_DrawTargets, Func_02001f14__s1f94, Func_02001d08,
                         Func_02001d98, Func_02001d50, Func_02001b1c)
#undef IWRAM

#undef gData_020036c4
#undef gData_02003704
void Actor_Release(void *handle);
s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);

void Func_02002118(void) {
  struct MarsLighthouseRoomMRideWork *work =
      (struct MarsLighthouseRoomMRideWork *)(gInventorySnapshot + 0x188);
  struct Actor *object;
  struct MarsLighthouseRoomMAnimation *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

DEFINE_RIDE_RELEASE_HANDLE(Ride_ReleaseHandle, Actor_Release)

void Func_02002180(s16 value) {
  ((struct RideWork *)(gInventorySnapshot + 0x188))->value0e = value;
}

DEFINE_BEAM_START(Beam_Start, Beam_DrawTargets)

s32 Func_020022c0(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a;
}

void Func_020022d0(s32 index, s16 value) {
  ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a = value;
}

s32 Func_020022e0(void) {
  return ((struct RideWork *)(gInventorySnapshot + 0x188))->counter;
}

DEFINE_RIDE_ADVANCE_COUNTER(Ride_AdvanceCounter)

DEFINE_RIDE_SET_ENTRY(Ride_SetEntry)

void Func_02002334(s32 index, s16 value) {
  ((struct RideWork *)(gInventorySnapshot + 0x188))->entries[index].c = value;
}

s32 Func_02002344(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->b;
}

DEFINE_RIDE_SET_SLOT(Ride_SetSlot)

struct MarsLighthouseEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u16 downCount;
  u16 standingCount;
  u16 downOwners[8];
};

/* VIEW(s2118): routine Unit_Get as struct BattleUnit *Unit_Get(s32 owner);
 * item_icon.h has void *Unit_Get(s32 owner); not tried: Unit_Get's declaration
 * holds a FAKEMATCH tag. */
struct BattleUnit *Unit_Get__s2118(s32 owner) __asm__("Unit_Get");
#define Unit_Get Unit_Get__s2118

DEFINE_PARTY_TAKE_FALL_DAMAGE(Party_TakeFallDamage)

#include "../field/field_effect/advance_spin.c"

DEFINE_ACTOR_BLINK(Actor_Blink, 10, 4, 0)

s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 terrain, s32 x, s32 z);

#define KNOCK_BACK Leader_KnockBack
#define KNOCK_BACK_TAKE_DAMAGE Party_TakeFallDamage
#define KNOCK_BACK_BLINK Actor_Blink
#define KNOCK_BACK_SET_ANIMATION Actor_SelectAnimation
#define KNOCK_BACK_CELL_ATTRIBUTE MapGrid_GetCellAttributeLowNibbleOrBump
#define KNOCK_BACK_CHECK_COLLISION MapGrid_CheckCollision
#define KNOCK_BACK_SPHERES_OVERLAP Field_CheckSpheresOverlap
#include "shared/leader_knock_back.c"
#undef KNOCK_BACK
#undef KNOCK_BACK_TAKE_DAMAGE
#undef KNOCK_BACK_BLINK
#undef KNOCK_BACK_SET_ANIMATION
#undef KNOCK_BACK_CELL_ATTRIBUTE
#undef KNOCK_BACK_CHECK_COLLISION
#undef KNOCK_BACK_SPHERES_OVERLAP

void Func_020026f0(void) {
  if (((struct RideWork *)(gInventorySnapshot + 0x188))->active)
    Leader_KnockBack();
}

#undef Unit_Get
/* VIEW(s2708): routine Party_TakeFallDamage as int Party_TakeFallDamage(),
 * called with no declaration in scope as its bytes need; the definition has
 * void Party_TakeFallDamage(void); not tried: Party_TakeFallDamage's bound
 * declaration was not found. */
s32 Func_02002368__s2708() __asm__("Party_TakeFallDamage");
#define Party_TakeFallDamage Func_02002368__s2708
s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 layer, s32 x, s32 z);
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

struct RideEvent {
  u8 unk000[0x15a];
  u16 field15a;
  u16 field15c;
  u16 field15e;
  u8 unk160[4];
  u16 field164;
  u16 field166;
  u8 unk168[0x44];
  s32 field1ac;
  u8 unk1b0[2];
  u16 started;
  s32 field1b4;
};

struct RideCar {
  u8 unk00[0x64];
  s16 stopped;
  u8 unk66[0xa];
  s32 stepX;
  s32 unk74;
  s32 stepZ;
};

struct RideState {
  s32 counter;
  s16 active;
  u8 unk06[0x66];
  struct RideCar *car;
};

s32 MarsLighthouseRoomM_SlideOnIce(void) {
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  if (event->started == 0) {
    ScreenTransition_Start(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Event_ClearFlag(FLAG_MENU_OPEN);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
  Party_TakeFallDamage(-10, 0);
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gInventorySnapshot + 0x188);
  Event_SetFlag(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_ClearFlag(FLAG_MENU_OPEN);
  leader->update = (void (*)(void))Actor_Blink;
  leader->facing = 0x4000;
  Actor_SelectAnimation(leader, 49);
  while (ride->car->stopped == 0) {
    s32 kind = MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x, leader->z);

    if (kind != 7) {
      leader->x += ride->car->stepX;
      leader->z += ride->car->stepZ;
    } else {
      if (ride->car->stepX != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      if (ride->car->stepZ != 0)
        leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = 0;
  Actor_SetSpriteColorMode(leader, 0);
  ride->active = 0;
#ifdef UBFIX
  return 0;
#endif
}

extern u16 gData_02004cf0[];
void Actor_Release(void *handle);

void FieldCamera_MoveToActorById(s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Teleport_PlaySparks(void);

DEFINE_RIDE_IF_ACTIVE(MarsLighthouseRoomM_ContinueSlide,
                      MarsLighthouseRoomM_SlideOnIce)

s32 Func_02002864(void) {
  return ((struct RideWork *)(gInventorySnapshot + 0x188))->active;
}

struct Sprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 attr2Rest : 6;
  u8 unk0a[2];
  s32 scale;
  u8 vramBlock;
  u8 unk11_0 : 1;
  u8 unk11_1 : 7;
  u16 rotation;
  u8 unk14[2];
  u8 unk16[5];
  u8 unk1b;
  u8 unk1c[12];
  void *record;
};

struct Work {
  u8 unk00[12];
  u16 vramBlock;
  u8 unk0e[98];
  struct Actor *owner;
};

#define WORK ((struct Work *)(gInventorySnapshot + 0x188))

void Func_02002870(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
  // A once-only loop: one object per call.
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16();
    b = Random16();
    x = actor->x + a * 16;
    x -= b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
    z = actor->z + d * 16 - e * 16;
    y = c * 8 + actor->y + Q16_ONE * 4;
    object = (struct Actor *)Import_ActorCreate(489, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;
      s32 velocity;

      angle = Random16();
      radius = Random16();
      sum = Random16();
      sum += Random16();
      scale = (sum >> 2) + Q16_ONE / 2;
      Actor_StartScript(object, gData_02003304);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + Q16_ONE * 2;
      radius <<= 3;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      velocity = Iwram_MulQ16(radius, Trig_Sin(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      sprite = object->sprite;
      object->velocityX = velocity;
      object->acceleration = 0;
      object->bounce = Q16_ONE / 2;
      source = work->owner->sprite;
      VramCache_FreeEntry(sprite->vramBlock);
      sprite->unk11_0 = 1;
      sprite->vramBlock = work->vramBlock;
      SpriteAnimation_Clear(sprite->record);
      sprite->record = NULL;
      sprite->unk1b = 0;
      sprite->unk14[0] = source->unk14[0];
      sprite->unk14[1] = source->unk14[1];
      sprite->shape = source->shape;
      sprite->size = source->size;

      sprite->tile = source->tile;
    }
  }
}

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
DEFINE_EMBERS_SPAWN(Embers_Spawn, gData_02003348)

void Func_02002ad4(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s2708): tag struct Work020023c4 as struct Work020023c4__s2708;
 * module/scene/scene.h has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_m.c:1666: redefinition of `struct
 * Work020023c4'. */
#define Work020023c4 Work020023c4__s2708
struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[0x6c - 0x0c];
  struct Actor *unk6c;
};

struct WorkEvent {
  u8 unk000[340];
  u16 unk154;
  u8 unk156[358 - 342];
  s16 unk166;
};

DEFINE_ACTOR_ROLL_AND_CRUMBLE(Actor_RollAndCrumble, Work020023c4,
                              MapGrid_GetCellAttributeLowNibbleOrBump,
                              Embers_Spawn, Func_02002870)

void Func_02002df4(s32 sourceId, s32 id, s32 value, s32 angle,
                   const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *object = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Actor_StartScript(object, script);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  object->watchedObject = source;
  object->timer = 0;
  object->unk66 = angle;
  ((u8 *)object)[0x62] = value;
  object->update = Func_02002ad4;
}

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Actor_RollAndCrumble as its update hook and its speed, clears its
/// scale and action, has it watch what self watches, and calls
/// Event_PlaySound(SE_104). Overlays 6aa and 6ab keep the same code.
DEFINE_ACTOR_LAUNCH_AHEAD(Actor_LaunchAhead, Actor_RollAndCrumble)

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

DEFINE_ACTOR_POP_UP_SLOW(Actor_PopUpSlow, Actor_SelectAnimation)

DEFINE_ACTOR_POP_UP(Actor_PopUp, Actor_SelectAnimation)

DEFINE_ACTOR_PARK(Actor_Park, Actor_LaunchAhead)

void Func_0200030c(void);

s32 gData_020032c8[] = {
    SC_CALL(Func_0200004c),
    SC_END,
};

// clang-format off
u32 gData_020032d4[] = {                                                               
      SC_WAIT(30), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_FREE,                                                                 
  };

u32 gData_02003304[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02003348[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gData_0200338c[] = {
    201, 202, 203, 204, 205, 206, 207, -1,
};

s16 gData_0200339c[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_020033c8[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gData_020033f4[] = {
    0xc900ce, 0xcb00ca, 0xcd00cc, 0xce00cd, 0xca00c9, 0xcc00cb,
    0xcd00cc, 0xc900ce, 0xcb00ca, 0xcc00cb, 0xce00cd, 0xca00c9,
    0xcb00ca, 0xcd00cc, 0xc900ce, 0xca00c9, 0xcc00cb, 0xce00cd,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MarsLighthouseRoomM6),
    SCENE_EXIT(1, MarsLighthouseRoomF4, 5),
    SCENE_EXIT(2, MarsLighthouseRoomF2, 8),
    SCENE_EXIT(3, MarsLighthouseRoomF1, 4),
    SCENE_EXIT(4, MarsLighthouseRoomF3, 15),
    SCENE_EXITS(MarsLighthouseRoomM1),
    SCENE_EXIT(1, MarsLighthouseAerie1, 1),
    SCENE_EXIT(2, MarsLighthouseEntrance2, 2),
    SCENE_EXIT(3, MarsLighthouseRoomM5, 1),
    SCENE_EXIT(4, MarsLighthouseRoomM2, 1),
    SCENE_EXIT(5, MarsLighthouseRoomM4, 1),
    SCENE_EXIT(6, MarsLighthouseRoomM3, 1),
    SCENE_EXIT(7, MarsLighthouseRoomM1, 8),
    SCENE_EXIT(8, MarsLighthouseRoomM1, 7),
    SCENE_EXIT(9, MarsLighthouseRoomM1, 10),
    SCENE_EXIT(10, MarsLighthouseRoomM1, 9),
    SCENE_EXIT(11, MarsLighthouseRoomM1, 12),
    SCENE_EXIT(12, MarsLighthouseRoomM1, 11),
    SCENE_EXIT(13, MarsLighthouseRoomM1, 14),
    SCENE_EXIT(14, MarsLighthouseRoomM1, 13),
    SCENE_EXITS(MarsLighthouseRoomM2),
    SCENE_EXIT(1, MarsLighthouseRoomM1, 4),
    SCENE_EXIT(2, MarsLighthouseRoomF2, 1),
    SCENE_EXITS(MarsLighthouseRoomM3),
    SCENE_EXIT(1, MarsLighthouseRoomM1, 6),
    SCENE_EXIT(2, MarsLighthouseRoomF3, 1),
    SCENE_EXITS(MarsLighthouseRoomM4),
    SCENE_EXIT(1, MarsLighthouseRoomM1, 5),
    SCENE_EXIT(2, MarsLighthouseRoomF1, 1),
    SCENE_EXITS(MarsLighthouseRoomM5),
    SCENE_EXIT(1, MarsLighthouseRoomM1, 3),
    SCENE_EXIT(2, MarsLighthouseRoomF4, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gMarsLighthouseRoomM_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseRoomM_Room6Placements[] = {
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gData_020032c8, 504, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, 7, 504, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, (s32)gData_020032c8, 504, 0, 112, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseRoomM_Room1Placements[] = {
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_WEST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(416, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_WEST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 224, 0, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gMarsLighthouseRoomM_Triggers[] = {
    {TRIGGER_TABLE_END},
};

s32 gData_020036c4 = -1;

struct Trigger gMarsLighthouseRoomM_Room6Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Func_020002c8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_020002e8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), 10, CONDITION_ALWAYS, EVENT(Func_020002fc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 10, CONDITION_ALWAYS, EVENT(Func_0200030c)},
    {TRIGGER_TABLE_END},
};

s32 gData_02003704 = -1;

struct Trigger gMarsLighthouseRoomM_Room1Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_STEP, 20, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 21, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 22, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 23, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 24, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 25, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 26, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 27, 0x1a30, EVENT(Func_020006c8)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 1, CONDITION_ALWAYS, EVENT(Func_02000abc)},
    {TRIGGER_EXAMINE, 30, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadEarthInscription)},
    {TRIGGER_EXAMINE, 31, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadWaterInscription)},
    {TRIGGER_EXAMINE, 32, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadFireInscription)},
    {TRIGGER_EXAMINE, 33, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadWindInscription)},
    {TRIGGER_STEP, 34, 0xa2e, EVENT(MarsLighthouseRoomM_HearDragonStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseRoomM_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};

s32 gData_0200387c[] = {
    23, 9, 0x3000, 23, 10, 0x3000,
    23, 6, -0x1800, 0, 1, 13,
    8, 0, (s32)0x80010000, 23, 9, -0x3000,
    23, 10, -0x3000, 23, 6, 0x1800,
    0, 1, 13, 12, (s32)0xc0010000, 3,
    0, 1, 38,
};

u32 gData_02003900[] = {
    0, 20, 3, 0, 1, 38,
};
// clang-format on

#undef Work020023c4
#undef Party_TakeFallDamage
