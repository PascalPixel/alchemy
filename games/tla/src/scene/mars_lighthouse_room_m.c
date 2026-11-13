// ---- games/tla/src/scene/mars_lighthouse_room_m.c ----
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
#include "event_work.h"
#include "module/scene/mars_lighthouse_room_m.h"
#include "module/scene/scene.h"
#include "module/sound/sound.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "module/scene/aqua_rock_inside.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"

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

void Animation_ApplyChildValues(struct Actor *object, s32 value);
#include "main_routine_work.h"

void Func_0200166c(void);

s32 Func_0200004c(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

void Func_02000058(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 0;
  object->y = PIXELS(14);
  Actor_SetSingleChildFlags(object, 0);
  Func_02003118(object, value);
}

#include "mars_lighthouse_room_m/mars_lighthouse_room_m.h"

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6ACRoom6)
    return gData_02003508;
  if (scene == (s32)&SceneId_Ov6ACRoom1)
    return gData_02003598;
  return gData_020034f0;
}

#include "iwram_call.h"
#include "io_write_queue.h"

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
    Iwram_CopyWords(Data_020039bc, (void *)0x050001a0, 32);
    Iwram_CopyWords(gData_0200393c, (void *)0x05000100, 64);
    Iwram_CopyWords(gData_0200397c, gData_0200393c, 64);
    gData_020036c4 = 0;
  }
  base = gData_020036c4 / 8 % 6;
  Iwram_CopyWords(gData_0200397c, Data_020039bc, 32);
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
  IO_WRITE_QUEUE_PUSH(0x050001a0, (u32)gData_0200397c, 0x84000008);
  gData_020036c4++;
}

void Func_020002c8(void) {
  Event_PlaySound(123);
  FieldEvent_SetExitRequest(gMainRoutineWork.entrance);
}

void Func_020002e8(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  event[3384] = 1;
}

void Func_020002fc(void) {
  Func_02003230(10, 1);
  Func_02003228();
}

void Func_0200030c(void) {
  struct MarsLighthouseRoomMEventWork *work;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(30);
  Event_PlaySound(190);
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
    Iwram_CopyWords(gData_02003a5c, (void *)0x05000040, 32);
    Iwram_CopyWords((void *)Data_020039dc, (void *)0x05000100, 64);
    Iwram_CopyWords(Data_02003a1c, (void *)Data_020039dc, 64);
    gData_02003704 = 0;
  }
  /* FAKEMATCH(volatile): the saved colours read as volatile; as plain u16, 2 of
   * 179 instructions differ in tla-en. */
  base = gData_02003704 / 8 % 6;
  for (i = 0; i <= 5; i++) {
    Data_02003a1c[i + 4] = Data_020039dc[(base + i) % 6 + 4];
    Data_02003a1c[i + 10] = Data_020039dc[(base + i) % 6 + 10];
    Data_02003a1c[i + 20] = Data_020039dc[(base + i) % 6 + 20];
    Data_02003a1c[i + 26] = Data_020039dc[(base + i) % 6 + 26];
  }
  if (!Event_GetFlag(0xa32))
    Iwram_CopyWords(&Data_02003a1c[10], Data_0200ba70, 12);
  if (!Event_GetFlag(0xa33))
    Iwram_CopyWords(&Data_02003a1c[20], Data_0200ba70, 12);
  if (!Event_GetFlag(0xa34))
    Iwram_CopyWords(&Data_02003a1c[26], Data_0200ba70, 12);
  if (!Event_GetFlag(0xa35))
    Iwram_CopyWords(&Data_02003a1c[4], Data_0200ba70, 12);
  IO_WRITE_QUEUE_PUSH(0x05000100, (u32)Data_02003a1c, 0x84000010);
  gData_02003704++;
}

#include "io_reg.h"

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
    Event_PlaySound(560);
  gData_02003a82 = state;
  /* FAKEMATCH(temporary): the low bit held in a local, so it is taken before
   * the state is copied for the next index, as in the native; as the test
   * itself, 12 of 141 instructions differ in tla-en. */
  odd = state & 1;
  /* FAKEMATCH(temporary): a local pointer to the flash, taken here, loads its
   * address before 20 is subtracted; as a direct store in each arm, 13 of 141
   * instructions differ in tla-en. */
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

/// Calls Func_0802b450 with (0, 125, 3, 3) for four places, steps
/// gData_02003a7e through 12 and 8 to 0, then calls it with (0, 122, 3, 3) for
/// the same places.
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

void DisplayBlend_EnableRunScript(void);

void Func_02000934(s32 entrance) {
  s32 cell = -1;
  s16 *blend;
  DisplayBlend_EnableRunScript();
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
  IO_WRITE_QUEUE_PUSH(0x04000052, Data_02003a7c | (16 - Data_02003a7c) << 8,
                      0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
  Task_AddFrame(Func_0200055c, 0x480);
}

void Func_02000abc(void) {
  Func_02001608();
  Func_020032a8();
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

  if (scene == (s32)&SceneId_Ov6ACRoom6)
    return gData_020036c8;
  if (scene == (s32)&SceneId_Ov6ACRoom1)
    return gData_02003708;
  if (scene == (s32)&SceneId_Ov6ACRoom2 || scene == (s32)&SceneId_Ov6ACRoom3 ||
      scene == (s32)&SceneId_Ov6ACRoom4 || scene == (s32)&SceneId_Ov6ACRoom5)
    return gData_02003858;
  return gData_020036b8;
}

// ---- games/tla/src/scene/mars_lighthouse_room_m/s0b64.c ----
#include "call.h"

void Func_0200154c(u16 *slots, u16 *colours, u16 *a, u16 *b);
/* VIEW(s0b64): routine Func_02002e6c as void Func_02002e6c(void); mars_lighthouse_room_m has void Func_02002e6c(struct Actor *self); tried as the definition's void Func_02002e6c(struct Actor *self), which the object already declares, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_m.c:490: too few arguments to function `Func_02002e6c'. */
extern void Func_02002e6c__s0b64(void) __asm__("Func_02002e6c");

extern u8 gData_0200338c[];
extern u8 gData_0200339c[];
extern u8 gData_020033c8[];
extern u8 gData_020033f4[];

s32 Scene_Initialize(void) {
  s32 entrance;
  s32 scene;

  scene = gMainRoutineWork.scene;
  entrance = gMainRoutineWork.entrance;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x100;
  Event_SetFlag(0x112);
  Call4((void (*)())Func_0200154c, (s32)gData_0200338c, (s32)gData_0200339c,
        (s32)gData_020033c8, (s32)gData_020033f4);
  if (scene != (s32)&SceneId_Ov6ACRoom6 && scene == (s32)&SceneId_Ov6ACRoom1) {
    Call1((void (*)())Func_02003278, 0);
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x204;
    if ((u32)(entrance - 1) <= 1 || entrance == 8 || entrance == 10 ||
        entrance == 12 || entrance == 14) {
      s32 unset = Event_GetFlag(0xa31);

      if (unset == 0 && Event_GetFlag(0xa32) &&
          Event_GetFlag(0xa33) &&
          Event_GetFlag(0xa34) &&
          Event_GetFlag(0xa35)) {
        struct Actor *actor;

        Event_Start();
        FieldEvent_ResolvePendingActions(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Event_PlaySound(78);
        FieldCamera_SetSpeed(0x20000, 0x4000);
        FieldCamera_MoveTo(0xe00000, 0x400000, 0xd80000, 1);
        Func_02003228();
        FieldEvent_Wait(20);
        PaletteFade_ApplySource(0x10000, 0);
        PaletteFade_ApplyTarget(0x10003, 0);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        /* FAKEMATCH(temporary): entrance reused for the palette address, which
         * holds its register; as a plain store, 38 of 324 instructions differ
         * in tla-en. */
        entrance = 0x500021e;
        *(u16 *)entrance = 0x7fff;
        System_Wait(1);
        FieldMessage_ShowCentered((s32)MsgYouHaveProven, 0, 4);
        PaletteFade_ApplyTarget(0x10000, 0);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        FieldCamera_SetSpeed(0x10000, 0x2000);
        FieldCamera_MoveTo(0xe00000, 0x100000, 0xd80000, 1);
        Func_02003228();
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
        MapGrid_SetShake(0x20000, 0x20000, 0x10000);
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
        MapGrid_SetShake(-1, -1, 0xe666);
        MapGrid_CopyCellAttributes(30, 12, 4, 3, 12, 12);
        FieldEvent_Wait(40);
        Event_SetFlag(0xa31);
        Event_PlaySound(80);
        Sound_WaitForBgmRestore();
        Party_PlaySavedMusic();
        Event_End();
      }
    }
  }
  return 0;
}

/* VIEW(s0b64): routine Func_02000058 as s32 Func_02000058(); the definition has void Func_02000058(s32 id, s32 value); tried as the definition's void Func_02000058(void), it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_m.c:521: conflicting types for `Func_02000058'. */
extern s32 Func_02000058__s0b64() __asm__("Func_02000058");
/* VIEW(mars_lighthouse_room_m): routine Func_02003150 as void Func_02003150(s32 value); module/scene/scene.h has u32 Func_02003150(struct FieldSprite *sprite, u32 frame); tried as the rest of the object declares it, Scene_PrepareMap moves: 8 of 237 instructions in tla-ja. */
extern void Func_02003150__mars_lighthouse_room_m(s32 value) __asm__("Func_02003150");
void Func_020032b0(s32 a);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void SetBlend(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
  DisplayScroll_InitializeHBlankDma(a, b, c, d, e, f, g);
}

s32 Scene_PrepareMap(void) {
  s16 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;

  if (scene == (s32)&SceneId_Ov6ACRoom6 || scene == (s32)&SceneId_Ov6ACRoom2 ||
      scene == (s32)&SceneId_Ov6ACRoom3 || scene == (s32)&SceneId_Ov6ACRoom4 ||
      scene == (s32)&SceneId_Ov6ACRoom5) {
    Func_020032b0(1);
    SetBlend(1, 0x10000, 0x40000, 0x20000, 0x10000, 0x8000, 0x4000);
  }
  if (scene == (s32)&SceneId_Ov6ACRoom6) {
    switch (entrance) {
    case 1:
      Actor_SetSpriteColor(Actor_Get(8), 1);
      Func_02000058__s0b64(9, 4);
      Func_02003150__mars_lighthouse_room_m(4);
      Func_02003150__mars_lighthouse_room_m(5);
      break;
    case 2:
      Actor_SetSpriteColor(Actor_Get(8), 3);
      Func_02000058__s0b64(9, 5);
      Func_02003150__mars_lighthouse_room_m(6);
      Func_02003150__mars_lighthouse_room_m(7);
      break;
    case 3:
      Actor_SetSpriteColor(Actor_Get(8), 0);
      Func_02000058__s0b64(9, 2);
      Func_02003150__mars_lighthouse_room_m(0);
      Func_02003150__mars_lighthouse_room_m(1);
      break;
    case 4:
      Actor_SetSpriteColor(Actor_Get(8), 4);
      Func_02000058__s0b64(9, 3);
      Func_02003150__mars_lighthouse_room_m(2);
      Func_02003150__mars_lighthouse_room_m(3);
      break;
    }
    object = Actor_Get(10);
    object->motionFlags = 0;
    object->z = 0x680000;
    FieldTask_AddFrame(Func_020000c0, 0x480);
  } else if (scene == (s32)&SceneId_Ov6ACRoom1) {
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
    // A second name of the one scene id: the ROM keeps a literal for each
    // use, and with one name they share one.
    extern char SceneId_Ov6ACRoom3Second __asm__("SceneId_Ov6ACRoom3");

    Func_020031b8(gMainRoutineWork.map +
                      ((s32)&SceneId_Ov6ACRoom3Second - (s32)&SceneId_Ov6a8Room2),
                  0x5000000);
  }
  return 0;
}

#include "version.h"

void Func_020010e4(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgMankindWithThe, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Event_End();
}

void Func_02001154(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgFishWithCold, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Event_End();
}

void Func_020011c4(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgDragonsBurningWith, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Event_End();
}

void Func_02001234(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Event_Start();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgBirdOnThe, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Event_End();
}

void Func_020012a4(s32 x, s32 z, s32 kind) {
  struct Actor *object;

  /* FAKEMATCH(temporary): z shifted ahead of the call; as z << 16 in its
   * arguments, 2 of 53 instructions differ in tla-en. */
  z <<= 16;
  object = (struct Actor *)Import_ActorCreate(403, x << 16, 0, z);

  if (object != 0) {
    Actor_SetSingleChildFlags(object, 0);
    Func_02003118(object, 3);
    if (kind != 0) {
      ((struct MarsLighthouseRoomMAnimation *)object->sprite)->layer = 2;
      Actor_StartScript(object, gData_0200387c);
    } else {
      ((struct MarsLighthouseRoomMAnimation *)object->sprite)->layer = 1;
      object->velocityX = (((u32)(Random16Far() * 8) >> 16) - 4) << 16;
      object->velocityY = PIXELS(4);
      Actor_StartScript(object, gData_02003900);
    }
  }
}

void Func_02001324(s32 x, s32 z, s32 c, s32 d, s32 e, s32 f) {
  u32 i;
  s32 px;
  s32 pz;

  Sound_Play(144);
  Func_020012a4(x, z + 12, 1);
  for (i = 0; i < 3; i++) {
    px = x + ((u32)(Random16Far() * 12) >> 16) - 6;
    pz = z + ((u32)(Random16Far() * 12) >> 16) - 6;
    Func_020012a4(px, pz, 0);
  }
  wait(4);
  Map_CopyCellPictureIndicesRect(c, d, e + 64, f, 1, 1);
}

/// No one holding item 222: MsgIfYouDo, once (flag 0xa2f). Someone holding it
/// (and flag 0xa30 set, from patch 1): MsgYouWhoHold and the
/// message after it, the camera moves on and Func_0200080c runs, then flag
/// 0xa2e is set. Each opens as Func_020010e4 does.
void Func_020013a0(void) {
  if (PartyInventory_FindMember(222) == -1) {
    if (!Event_GetFlag(0xa2f)) {
      u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);

      Event_Start();
      Event_ResolvePendingActions(0);
      FieldCamera_SetSpeed(0x20000, 0x4000);
      FieldCamera_MoveTo(PIXELS(224), PIXELS(64), PIXELS(216), 1);
      Func_02003228();
      FieldEvent_Wait(20);
      /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
       * call; as a direct call, 12 of 155 instructions differ in tla-en. */
      PaletteFade_ApplySource(0x10000, 0);
      BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
      BattleEffect_StartBufferInterpolation(20);
      wait(20);
      *color = 0x7fff;
      wait(1);
      Func_0803ce64((s32)MsgIfYouDo, 0, 4);
      BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
      BattleEffect_StartBufferInterpolation(20);
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
    FieldCamera_SetSpeed(0x20000, 0x4000);
    /* FAKEMATCH(wrapper): forwarded so this branch builds its own positions;
     * as a direct call, 16 of 155 instructions differ in tla-en. */
    FieldCamera_MoveTo(PIXELS(224), PIXELS(64), PIXELS(216), 1);
    Func_02003228();
    FieldEvent_Wait(20);
    /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
     * call, as in the branch above; as a direct call at this site, 22 of 155
     * instructions differ in tla-en. */
    PaletteFade_ApplySource(0x10000, 0);
    BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
    BattleEffect_StartBufferInterpolation(20);
    wait(20);
    *color = 0x7fff;
    wait(1);
    Func_0803ce64((s32)MsgYouWhoHold, 0, 4);
    wait(10);
    Func_0803ce64((s32)MsgYouWhoHold + 1, 0, 3);
    FieldCamera_MoveTo(PIXELS(224), PIXELS(88), PIXELS(216), 1);
    Func_02003228();
    FieldEvent_Wait(20);
    Sound_Play(560);
    Func_0200080c();
    FieldEvent_Wait(20);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    BattleEffect_StartBufferInterpolation(20);
    wait(40);
    Event_SetFlag(0xa2e);
    Event_End();
  }
}

#include "dma.h"

extern u16 *Data_0200392c;
extern u16 *Data_02003930;
extern u16 *Data_0200391c;

void Func_0200183c(void);

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
void Func_0200154c(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02003928 = slots;
  Data_0200392c = colours;
  Data_02003930 = a;
  Data_0200391c = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      int value;

      /* FAKEMATCH(dead): the colour's build and store in a once-only
       * block, so both lists are stepped right after their loads and the
       * next entry's marker loads before 0xffff is built; as plain
       * statements, 22 of 80 instructions differ in tla-en. */
      do {
        value = (blue << 10) | (green << 5) | red;
        ((u16 *)BG_PLTT)[index] = value;
      } while (0);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_ApplyTarget(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Func_0200183c();
}

void Func_02001608(void) {
  gData_02003938 = 0;
  gData_02003934 = Func_020016e8(gData_02003928);
  gData_02003924 = 15;
  gData_02003920 = 16;
  gData_02003918 = 1;
  Task_AddFrame(Func_0200170c, 0x480);
}

// ---- games/tla/src/scene/mars_lighthouse_room_m/stop.c ----

extern u8 gData_020023c4[];
extern u16 gData_02004cf0[];
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02003130(void *handle);
void Func_020024e4(void);
s32 Func_02002708(void);
void Func_02002e6c(struct Actor *self);

void Func_02003118(struct Actor *object, s32 value);
void Func_02003230(s32 a, s32 b);
void Func_02003228(void);
void Func_02001608(void);
void Func_020032a8(void);
void Func_0200166c(void);
void Func_0200055c(void);
void Func_0200170c(void);

void Func_0200166c(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  FieldTask_Remove(Func_0200170c);
}

void Func_0200168c(void) {
  gData_02003938 = 0;
  gData_02003934 = Func_020016e8(gData_02003928);
  gData_02003924 = 2;
  gData_02003920 = 2;
  gData_02003918 = 1;
  Task_AddFrame(Func_0200170c, 0x480);
}

s32 Func_020016e8(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
  count--;
  return count;
}

extern u16 *Data_0200391c;
extern u16 *Data_0200392c;
extern u16 *Data_02003930;

void Func_0200170c(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gData_02003920 != 0) {
    if (--gData_02003920 == 0) {
      step = 1;
      gData_02003920 = gData_02003924;
      if (gData_02003924 > 2)
        gData_02003924--;
    }
  }
  if (step == 0)
    return;
  src = Data_02003930;
  for (i = 0; i < gData_02003934; i++) {
    dst = &BG_PLTT[Data_0200391c[gData_02003938 * gData_02003934 + i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    /* FAKEMATCH(dead): the store in a once-only block, whose end holds the
     * order around it; as the plain statement, 2 of 120 instructions differ in
     * tla-en. */
    do {
      *dst = (blue << 10) | (green << 5) | red;
    } while (0);
  }
  dst = &BG_PLTT[gData_02003928[gData_02003934]];
  if (gData_02003918 != 0)
    src = Data_0200392c + gData_02003934 * 3;
  else
    src = Data_02003930 + gData_02003934 * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(dead): the last read in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_02003918 ^= 1;
  if (++gData_02003938 == gData_02003934)
    gData_02003938 = 0;
}

struct MarsLighthouseRoomMEventWork1ac {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Func_020017b4(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02003248_(s32 a, s32 b) {
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

void Func_0200183c(void) {
  struct MarsLighthouseRoomMEventWork1ac *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  PaletteFade_ApplyTarget(0x202108, 1);
  Func_02003248_(0x10000, 2);
  ((struct MarsLighthouseRoomMEventWork1ac *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200168c();
  Func_020032a0();
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

// ---- games/tla/src/scene/mars_lighthouse_room_m/s1988.c ----
#include "mars_lighthouse_entrance/room.h"
#include "field/event_work.h"

/* VIEW(s1988): tag struct RideSlot as struct RideSlot__s1988; mars_lighthouse_room_m.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_m.c:1088: structure has no member named `id'. */
struct RideSlot__s1988 {
  u16 id;
  u16 blocked;
};

/* VIEW(s1988): tag struct RideWork as struct RideWork__s1988; mars_lighthouse_room_m.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_m.c:1056: structure has no member named `value68'. */
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

void Func_02001988(struct Actor *object) {
  struct RideWork__s1988 *work = (struct RideWork__s1988 *)(gData_020023c4 + 0x188);
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  s32 direction;
  s32 id;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->sprite)->unk12 += object->timer;
  if (actor->y == actor->terrainHeight && Func_020018c4(object, actor) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->value68 = 0;
      {
        u16 *cell = &work->direction;

        if (object->unk4c > 0) {
          direction = 1;
          *cell = direction;
        } else {
          direction = 0xffff;
          *cell = direction;
        }
      }
    }
    object->priorityFlags = 2;
    {
      s32 speed = object->unk4c;

      object->unk4c = 0;
      object->z -= speed;
      object->gravity += speed >> 2;
    }
    work->landed = 1;
    event->exit = work->value0a;
  }
  for (i = 0; i < 4 && (id = work->slots[i].id) != 0; i++) {
    actor = Actor_Get(id);

    if (actor->y == actor->terrainHeight && Func_02001928(object, actor)) {
      if (work->slots[i].blocked == 0) {
        s32 speed;

        object->priorityFlags = 2;
        speed = object->unk4c;
        object->unk4c = 0;
        object->z -= speed;
        object->gravity += speed >> 2;
      } else {
        work->blocked = 1;
      }
    }
  }
}

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_02001aec(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Actor_StartScript(object, gData_020032d4);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_02001b1c(struct MarsLighthouseRoomMVector *position, s32 value, s32 flag) {
  struct Actor *object = FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = value;
    ((void (*)())Func_02001aec)(object, 2);
    object->update = Func_02001988;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_02003118(object, 2);
  }
}

// ---- games/tla/src/scene/mars_lighthouse_room_m/s1b64.c ----
#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"

/* VIEW(s1b64): tag struct RideSlot as struct RideSlot__s1b64; mars_lighthouse_room_m.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_m.c:1202: structure has no member named `id'. */
struct RideSlot__s1b64 {
  u16 id;
  u16 blocked;
};

struct RideDirection {
  u16 unk00;
  u16 unk02;
};

/* VIEW(s1b64): tag struct RideWork as struct RideWork__s1b64; mars_lighthouse_room_m.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_m.c:1180: structure has no member named `direction'. */
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

extern u8 gData_020023c4[];

s32 Func_020018c4(struct Actor *a, struct Actor *b);
s32 Func_02001928(struct Actor *a, struct Actor *b);

void Func_02001b64(struct Actor *object) {
  struct RideWork__s1b64 *work = (struct RideWork__s1b64 *)(gData_020023c4 + 0x188);
  struct Actor *other = Actor_Get(gMainRoutineWork.currentOwner);
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  struct RideDirection *direction;

  object->z += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->x += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->sprite)->unk12 += object->timer;
  if (other->y == other->terrainHeight && Func_020018c4(object, other) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->direction.unk02 = 0;
      direction = &work->direction;
      if (object->unk4c > 0)
        direction->unk00 = 1;
      else
        direction->unk00 = 0xffff;
    }
    object->priorityFlags = 2;
    object->z -= object->bounce;
    object->gravity += object->bounce >> 2;
    object->unk4c = 0;
    work->landed = 1;
    event->exit = work->value0a;
  }
  for (i = 0; i < 4; i++) {
    s32 id = work->slots[i].id;

    if (id == 0)
      break;
    other = Actor_Get(id);
    if (other->y == other->terrainHeight && Func_02001928(object, other)) {
      if (work->slots[i].blocked == 0) {
        object->priorityFlags = 2;
        object->z -= object->unk4c;
        object->gravity += object->unk4c >> 2;
        object->unk4c = 0;
      } else {
        work->blocked = 1;
      }
    }
  }
}

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_020032d4; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
void Func_02001cc4(struct Actor *object, s32 part) {
  /* FAKEMATCH(temporary): one zero for every cleared field; as six stores of
   * 0, 29 of 30 instructions differ in tla-en. */
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;

  ((struct SpriteParts *)object->sprite)->part = part;
  Actor_SetSingleChildFlags(object, 0);
  Actor_StartScript(object, gData_020032d4);
  object->timer = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}

void Func_02001d08(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct Actor *object = FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -value;
    Func_02001cc4(object, 3);
    object->update = Func_02001988;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_02003118(object, 2);
  }
}

void Func_02001d50(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct Actor *object = FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = value;
    Func_02001cc4(object, 3);
    object->update = Func_02001b64;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_02003118(object, 2);
  }
}

void Func_02001d98(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct Actor *object = FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -value;
    Func_02001cc4(object, 3);
    object->update = Func_02001b64;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_02003118(object, 2);
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

void Func_02001ed4(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_02003118(object, 2);
  Actor_StartScript(object, gData_020032d4);
  Actor_SetSpriteColor(object, 10);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_02001f14(struct MarsLighthouseRoomMVector *position, s32 value) {
  struct Actor *object =
      FieldActor_Create(285, position->x, position->y, position->z + 0xfffe0000);
  s32 angle;

  if (object != 0) {
    object->bounce =
        Math_Cosine(((u32)(Random16Far() * 3 << 12) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = ((u32)(Random16Far() << 17) >> 16) + value;
    /* FAKEMATCH(temporary): the angle held in an s32 local keeps the zero
     * extension and the pool word 0xffff8000 (a direct store narrows the add
     * to 16 bits: movs, lsls for 0x8000, no extension); as a direct store, 10
     * of 50 instructions differ in tla-en. */
    angle = (u16)Random16Far() - 0x8000;
    object->timer = angle;
    ((void (*)())Func_02001ed4)(object, 2);
    object->update = Func_02001e38;
    Animation_SetStateFlags(object, 1);
  }
}

// ---- games/tla/src/scene/mars_lighthouse_room_m/s1f94.c ----
/* VIEW(s1f94): tag struct Actor as struct Actor__s1f94; actor.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_m.c:1456: structure has no member named `flag5b'. */
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
/* VIEW(s1f94): routine Func_02001f14 as void Func_02001f14(); the parts before it has another declaration; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_m.c:1491: too many arguments to function `Func_02001f14'. */
extern void Func_02001f14__s1f94() __asm__("Func_02001f14");
extern u8 gData_020023c4[];

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

void Func_02001f94(void) {
  s32 saved[2];
  State *state;
  Entry *entry;
  Camera *camera;
  Position *position;
  s32 cameraX, cameraZ;
  s32 v;
  s32 i;

  state = (State *)(gData_020023c4 + 0x188);
  camera = IWRAM->camera;
  entry = state->entries;
  position = &camera->position;
  cameraX = position->x;
  cameraX &= 0xffff0000;
  cameraZ = position->z & 0xffff0000;
  saved[1] = (*(s32 **)camera)[1];
  saved[0] = IWRAM->unk_6c.word;
  if (state->unk_6 != 0 && state->unk_8 == 0 && state->callback != 0)
    state->callback();
  v = (u16)state->unk_6;
  state->unk_8 = v;
  state->unk_6 = 0;
  for (i = 3; i >= 0; i--, entry++) {
    Actor *actor;
    s32 flag, x, y, unk6, value, dx, dz;
    u8 *mark;

    if (entry->active == 0)
      continue;
    actor = entry->actor;
    if (actor == 0)
      continue;
    flag = GameFlag_Test(0x106);
    if (*(s16 *)(saved[0] + 358) != 0)
      flag |= 1;
    mark = &actor->flag5b;
    *mark = flag;
    if (state->unk_e != 0)
      *mark = GameFlag_Test(0x106);
    dx = actor->x;
    dz = actor->z;
    dx -= cameraX;
    /* FAKEMATCH(barrier): the empty asm on dx changes its allocation rank; as
     * no asm, 94 of 182 instructions differ in tla-en. */
    asm("" : : "r"(dx));
    y = actor->y;
    dz -= cameraZ;
    y = dz - y;
    unk6 = entry->unk_6;
    x = dx >> 16;
    y = (y >> 16) - 8;
    if (unk6 == 0) {
      if ((u32)(x + 7) > 334)
        continue;
      if (y <= -48)
        continue;
      if (y > 239)
        continue;
    }
    y = (u16)entry->timer;
    if (entry->timer <= 0) {
      y = entry->flags & 240;
      switch (y) {
      case 128:
        Func_02001f14__s1f94(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 48:
        Func_02001d08(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 32:
        Func_02001d98(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 16:
        Func_02001d50(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 0:
        Func_02001b1c(&entry->actor->x, entry->unk_c, unk6);
        break;
      }
      value = 8;
    } else {
      value = y - 1;
    }
    entry->timer = value;
  }
}
#undef IWRAM
