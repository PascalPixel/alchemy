#include "trial_road.h"
#include "event_inline.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern const u8 gData_02007044[];
extern const u8 gData_02007cf8[];
extern const u8 gData_02007d40[];
extern const u8 gData_02007d94[];
extern const u8 *const gData_02007e00[];

const u8 *Func_02001890(void) {
  s32 pattern = gTrialRoadState->pattern;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room1)
    return gData_02007e00[pattern & 7];
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room3)
    return gData_02007cf8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room4)
    return gData_02007d40;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room2)
    return gData_02007d94;
  return gData_02007044;
}

void Func_02001904(void) {
  if (!GameFlag_Test(0x307))
    OBJECT_BYTE(Actor_GetById(17), 98) = 1;
  else
    Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
  Func_020006b4(15);
  Func_020006b4(16);
  Func_020006b4(17);
  Func_020006b4(18);
  Func_020006b4(19);
  Func_0200062c(16);
  Func_0200062c(17);
  Func_0200062c(18);
  Func_0200062c(19);
  Task_Add(Func_020007b4, 0x480);
}

void Func_02001974(void) {
  Func_020006b4(15);
  Func_020006b4(16);
  Func_020006b4(17);
  Func_020006b4(18);
  Func_020006b4(19);
  Func_020006b4(20);
  Func_0200062c(15);
  Func_0200062c(16);
  Func_0200062c(17);
  Func_0200062c(18);
  Func_0200062c(19);
  Func_0200062c(20);
  OBJECT_BYTE(Actor_GetById(17), 98) = 1;
  OBJECT_BYTE(Actor_GetById(18), 98) = 1;
  OBJECT_BYTE(Actor_GetById(19), 98) = 1;
  OBJECT_BYTE(Actor_GetById(20), 98) = 1;
  if (!GameFlag_Test(0x214))
    Actor_SetModeById(17, 5);
  else
    Func_0200070c(17, -16);
  if (!GameFlag_Test(0x215))
    Actor_SetModeById(18, 5);
  else
    Func_0200070c(18, -16);
  if (!GameFlag_Test(0x216))
    Actor_SetModeById(19, 5);
  else
    Func_0200070c(19, -16);
  if (!GameFlag_Test(0x217))
    Actor_SetModeById(20, 5);
  else
    Func_0200070c(20, -16);
}

void Func_02001a70(void) {
  if (!GameFlag_Test(0x327))
    OBJECT_BYTE(Actor_GetById(15), 98) = 1;
  else
    Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
  if (!GameFlag_Test(0x328))
    OBJECT_BYTE(Actor_GetById(16), 98) = 1;
  else
    Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
  Func_020006b4(15);
  Func_020006b4(16);
  Func_020006b4(17);
  Func_020006b4(18);
  Func_020006b4(19);
  Func_0200062c(15);
  Func_0200062c(16);
  Func_0200062c(17);
  Func_0200062c(18);
  Func_0200062c(19);
}

extern const u8 gData_02007734[];

void Func_02000924(s32 id);
void Func_0200095c(s32 id);
void Func_020042f0(const void *data);
void Func_02000d70(void);

void Func_02001afc(void) {
  OBJECT_BYTE(Actor_GetById(18), 98) = 1;
  OBJECT_BYTE(Actor_GetById(19), 98) = 1;
  OBJECT_BYTE(Actor_GetById(20), 98) = 1;
  OBJECT_BYTE(Actor_GetById(21), 98) = 1;
  if (!GameFlag_Test(0x339))
    OBJECT_BYTE(Actor_GetById(16), 98) = 1;
  else
    Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  Func_020006b4(15);
  Func_020006b4(16);
  Func_020006b4(17);
  Func_020006b4(18);
  Func_020006b4(19);
  Func_020006b4(20);
  Func_020006b4(21);
  Func_0200062c(15);
  Func_0200062c(16);
  Func_0200062c(17);
  if (!GameFlag_Test(0x335)) {
    Func_0200095c(18);
    Func_02000924(19);
  } else {
    Func_0200095c(19);
    Func_02000924(18);
  }
  if (!GameFlag_Test(0x337)) {
    Func_0200095c(20);
    Func_02000924(21);
  } else {
    Func_0200095c(21);
    Func_02000924(20);
  }
}

void Func_02001bdc(void) { Func_020042f0(gData_02007734); }

/// Copies two blocks of map cells, sets byte 98 of objects 16 to 18 and clears
/// their collision flags; then runs Func_02000d70, and Func_020006b4 and
/// Func_0200062c for object 15.
void Func_02001bec(void) {
  Map_CopyCellAttributeRect(11, 22, 11, 8, 11, 64);
  Map_CopyCellAttributeRect(79, 22, 11, 8, 79, 64);
  OBJECT_BYTE(Actor_GetById(16), 98) = 1;
  OBJECT_BYTE(Actor_GetById(17), 98) = 1;
  OBJECT_BYTE(Actor_GetById(18), 98) = 1;
  {
    /* FAKEMATCH(pin): the zero pinned to r6; as a plain local, which reuses r5
     * where the 1 was held, 12 of 52 instructions differ in tla-en. */
    register s32 clear asm("r6") = 0;

    Actor_GetById(16)->collisionFlags = clear;
    Actor_GetById(17)->collisionFlags = clear;
    Actor_GetById(18)->collisionFlags = clear;
  }
  Func_02000d70();
  Func_020006b4(15);
  Func_0200062c(15);
}

/// The head of a mover's slot, as Func_02004224 fills it.
struct TrialRoadMover {
  s16 id;
  s16 value;
  s16 unk04;
  s16 unk06;
  s16 unk08;
  s16 unk0a;
};

/// A view of the map work's byte 23.
struct MapWorkByte23 {
  u8 unk00[0x17];
  u8 bits0 : 2;
  // XXX: what bits 2 and 3 select; the map code that reads them would settle
  // it.
  u8 bits2 : 2;
  u8 bits4 : 4;
};

void Func_02004188(s32 a, s32 b);
struct TrialRoadMover *Func_02004224(s32 index, s32 id, s32 value, s32 low,
                                     s32 high);

/// Sets up the Trial Road room with five movers: objects 16 to 20 in slots 0
/// to 4, the first three over a range of 64 and the last two over 32, each
/// with 240, 120 and 150; objects 16 to 18 are put away (Func_0200078c) once
/// flags 0x363 to 0x365 are set and placed otherwise; then object 15.
void Func_02001c6c(void) {
  struct TrialRoadMover *mover;

  Func_02004188(0, 240);
  mover = Func_02004224(0, 16, 0, 0, 64);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224(1, 17, 0, 0, 64);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224(2, 18, 0, 0, 64);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224(3, 19, 0, 0, 32);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224(4, 20, 0, 0, 32);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  if (!GameFlag_Test(0x363))
    Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  else
    Func_0200078c(16);
  if (!GameFlag_Test(0x364))
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  else
    Func_0200078c(17);
  if (!GameFlag_Test(0x365))
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
  else
    Func_0200078c(18);
  ((struct MapWorkByte23 *)Ram_HeapSlots->mapWork)->bits2 = 1;
  Func_020006b4(15);
  Func_0200062c(15);
}

void Func_02005a08(void);
void Func_02005a10(s32 a, s32 b, s32 c);

void Func_02001d8c(void) {
  Func_02005a08();
  Func_02005a10(0, 15, 16);
  if (!GameFlag_Test(0x374))
    MapGrid_CopyCellAttributes(18, 21, 1, 1, 19, 21);
  else
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
}

void Func_02001dd0(void) {}

extern const u8 gData_02005b44[];

void Func_02001dd4(void) {
  struct Actor *partner = Actor_GetById(8);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 dx;
  s32 dz;
  s32 near;
  s32 far;
  s32 y;

  if (GameFlag_Test(648))
    return;
  dx = (leader->x - partner->x) >> 16;
  dz = (leader->z - partner->z) >> 16;
  near = dx * dx + dz * dz;
  dx = (leader->x >> 16) - 296;
  dz = (leader->z >> 16) - 248;
  far = dx * dx + dz * dz;
  if (far <= 6400 || near <= 2304)
    Actor_SetMoveTarget(partner, (296 - dx) << 16, partner->y,
                        (248 - dz) << 16);
  if (far > 15)
    return;
  Sound_Play(146);
  Sound_Play(122);
  partner->speedLimit = 0x20000;
  partner->acceleration = 0x20000;
  partner->motionFlags = 2;
  partner->terrainHeight = 0;
  partner->collisionFlags = 1;
  partner->velocityY = 0xa0000;
  Actor_SetMovementTypeById(8, gData_02005b44);
  y = partner->y;
  Actor_SetMoveTarget(partner, 344 << 16, y, 264 << 16);
  GameFlag_SetBit(648);
}

extern s32 gData_02007e24;

void Func_02001dd4(void);

/// Unless flag 123 is set, readies object 8: before flag 0x288 it raises it
/// 0x88000 with motion 5, after it grounds it with motion 3; then schedules
/// Func_02001dd4.
void Func_02001eb0(void) {
  if (!GameFlag_Test(123)) {
    struct Actor *object = Actor_GetById(8);
    s32 started = GameFlag_Test(0x288);
    u8 *flags = &object->motionFlags;

    if (!started) {
      *flags = 5;
      object->y += 0x88000;
    } else {
      *flags = 3;
      object->y = 0;
    }
    Task_Add(Func_02001dd4, 0x480);
  }
}

void Func_02001efc(s32 value) { gData_02007e24 = value; }

extern s32 gData_02007e20;
extern s32 gData_02007e24;
extern u16 gData_02007e28[];

void Func_02001f08(void) {
  u16 colors[16];
  s32 phase = (gData_02007e20 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02007e24 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  i = 0;
  offset = 4;
next:
  p = colors;
  {
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02007e28;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x050000a4, 0x84000002, REG_DMA3);
  gData_02007e20++;
}
