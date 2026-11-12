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

extern u8 gData_020023c4[];
extern u16 gData_02004cf0[];
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02003130(void *handle);
void Func_020024e4(void);
s32 Func_02002708(void);
void Func_02002e6c(struct Actor *self);
#include "main_routine_work.h"

void Func_02003118(struct Actor *object, s32 value);
void Func_02003230(s32 a, s32 b);
void Func_02003228(void);
void Func_02001608(void);
void Func_020032a8(void);
void Func_0200166c(void);
void Func_0200055c(void);
void Func_0200170c(void);

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

const struct ActorPlacement *Func_02000080(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6ACRoom6)
    return gData_02003508;
  if (scene == (s32)&SceneId_Ov6ACRoom1)
    return gData_02003598;
  return gData_020034f0;
}

#include "iwram_call.h"
#include "io_write_queue.h"

struct Overlay6acEventWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0xb3c];
  u8 unkca4;
};

extern s32 gData_020036c4;
extern u16 gData_0200393c[];
extern u16 gData_0200397c[];
extern u16 Data_020039bc[];

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

  Func_020031e0();
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
  Func_020031e8();
}

extern s32 gData_02003704;
extern u16 gData_02003a5c[];
extern volatile u16 Data_020039dc[];
extern u16 Data_02003a1c[];
extern const u16 Data_0200ba70[];

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

extern s16 Data_02003a7c;
extern s16 gData_02003a7e;
extern s16 gData_02003a80;

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

extern s16 gData_02003a82;

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

void Func_02003158(void);

void Func_02000934(s32 entrance) {
  s32 cell = -1;
  s16 *blend;
  Func_02003158();
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

const struct Trigger *Func_02000af4(void) {
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
