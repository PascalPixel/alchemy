// ---- games/tla/src/scene/trial_road/s15a8.c ----
#include "trial_road.h"
#include "event_inline.h"
#include "event_work.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/field/object.h"
#include "module/scene/scene.h"
#include "module/scene/trial_road.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "resource.h"
#include "serial.h"
#include "task.h"
#include "heap_slots.h"

void Func_020015a8(void) { GameFlag_SetBit(0x289); }

void Func_020015b8(void) { Func_02005a50_(68, 6, 1, 21, 13, 64, 64); }

void Func_020015d8(void) {
  Func_02005a50_(68, 6, 1, 21, 13, 64, 64);
  if (!GameFlag_Test(0x9b9)) {
    Func_02005a60(249, 72, 152);
    ((struct MainRoutineWork254 *)&gMainRoutineWork)->value = 0x19b9;
    gMainRoutineWork.battleRule24b = 2;
    Field_RequestBattleFromEncounterTable(106, 4);
  }
}

s32 Func_02001638(void) { return 1; }

void Func_0200163c(void) {
  MapGrid_CopyCells(33, 30, 1, 1, 10, 11);
  Map_CopyCellsRect(33, 30, 1, 1, 18, 7);
  Map_CopyCellsRect(33, 30, 1, 1, 18, 15);
  Map_CopyCellsRect(35, 30, 1, 1, 74, 11);
  Map_CopyCellsRect(35, 30, 1, 1, 82, 7);
  Map_CopyCellsRect(35, 30, 1, 1, 82, 15);
}

void Func_020016c8(void) {
  MapGrid_CopyCells(34, 30, 1, 1, 10, 11);
  Map_CopyCellsRect(34, 30, 1, 1, 18, 7);
  Map_CopyCellsRect(34, 30, 1, 1, 18, 15);
  Map_CopyCellsRect(36, 30, 1, 1, 74, 11);
  Map_CopyCellsRect(36, 30, 1, 1, 82, 7);
  Map_CopyCellsRect(36, 30, 1, 1, 82, 15);
}

void Func_02001754(s32 sound) {
  if (GameFlag_Test(0x9ff)) {
    if (gTrialRoadState->mirrored == 0) {
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 13, 79);
      MapGrid_CopyCellAttributes(78, 18, 1, 1, 77, 18);
    } else {
      Func_0802b450(26, 79, 1, 2, 18, 79);
      Map_CopyCellAttributeRect(81, 18, 1, 1, 82, 18);
    }
    if (sound)
      Sound_Play(138);
  }
  GameFlag_SetBit(0x283);
}

void Func_020017e0(void) {
  struct MainRoutineWork *party;
  s32 count = GameFlag_Test(0x381);

  count += GameFlag_Test(0x382);
  count += GameFlag_Test(0x383);
  count += GameFlag_Test(0x384);
  if (GameFlag_Test(0x385))
    Func_02002dec();
  else
    Func_0200322c();
  party = &gMainRoutineWork;
  party->battleRule24b = 2;
  do {
  } while (0);
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov697Room2, 4);
  Party_SetFields1f2And1f4((s32)&SceneId_Ov697Room2, 5);
  Field_RequestBattleFromEncounterTable(13, 4 - count);
}

/// When the object stands in column 8, sets flag 0x9fc, stops its motion and
/// lifts it one pixel.
void Func_0200185c(s32 a, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object->x >> 20 == 8) {
    GameFlag_SetBit(0x9fc);
    object->motionFlags = 0;
    object->y -= 0x10000;
  }
}

#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern const u8 gData_02007044[];
extern const u8 gData_02007cf8[];
extern const u8 gData_02007d40[];
extern const u8 gData_02007d94[];
extern const u8 *const gData_02007e00[];

const u8 *Scene_GetEvents(void) {
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

/* VIEW(s15a8): routine Func_02004224 as struct TrialRoadMover *Func_02004224(s32 index, s32 id, s32 value, s32 low, s32 high); the definition has struct RideEntry *Func_02004224(s32 slot, s32 id, s32 timer, s32 low, s32 high); tried as the definition's s32 Func_02004224(s32 row, s32 column, s32 direction), it does not compile in tla-ja: games/tla/src/scene/trial_road/s15a8.c:266: too many arguments to function `Func_02004224'. */
extern struct TrialRoadMover *Func_02004224__s15a8(s32 index, s32 id, s32 value, s32 low,
                                     s32 high) __asm__("Func_02004224");

/// Sets up the Trial Road room with five movers: objects 16 to 20 in slots 0
/// to 4, the first three over a range of 64 and the last two over 32, each
/// with 240, 120 and 150; objects 16 to 18 are put away (Func_0200078c) once
/// flags 0x363 to 0x365 are set and placed otherwise; then object 15.
void Func_02001c6c(void) {
  struct TrialRoadMover *mover;

  Func_02004188(0, 240);
  mover = Func_02004224__s15a8(0, 16, 0, 0, 64);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224__s15a8(1, 17, 0, 0, 64);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224__s15a8(2, 18, 0, 0, 64);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224__s15a8(3, 19, 0, 0, 32);
  mover->unk06 = 240;
  mover->unk08 = 120;
  mover->unk0a = 150;
  mover = Func_02004224__s15a8(4, 20, 0, 0, 32);
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

void Func_02001d8c(void) {
  Func_02005a08();
  Func_02005a10(0, 15, 16);
  if (!GameFlag_Test(0x374))
    MapGrid_CopyCellAttributes(18, 21, 1, 1, 19, 21);
  else
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
}

void Func_02001dd0(void) {}

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

// ---- games/tla/src/scene/trial_road/s1fc4.c ----
#include "system.h"

extern s32 gData_02007e20;
extern s32 gData_02007e24;

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Words_Copy(void *destination, const void *source,
                                  s32 size) {
  Iwram_CopyWords(destination, source, size);
}

void Func_02001fc4(s32 mode) {
  gData_02007e24 = 1;
  if (mode != 2)
    Words_Copy(gData_02007e28, (void *)&BG_PLTT_COLOR(5, 0), 32);
  if (mode == 1) {
    /* FAKEMATCH(temporary): a local holding the zero builds it in r2 and the
     * counter address in r3, as overlay 699's twin does; as
     * gData_02007e20 = 0, 6 of 25 instructions differ in tla-en. */
    s32 zero = 0;

    gData_02007e20 = zero;
    Task_Add(Func_02001f08, 0x480);
  }
}

#include "types.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "field.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02002db0(void);

void Func_0200200c(void) {
  s32 t7, t8, t10, t13, t16, p3;

  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 4) {
    p3 = FIELD_AT(&gMainRoutineWork, s16, 482);
    goto L_0200a02e;
  }
  Func_02005868();
  Func_020034b4();
  Import_EventSetExitRequest(4);
  goto L_0200a07c;
L_0200a02e:
  if (p3 != 5)
    goto L_0200a042;
  Func_02005868();
  Func_020037c8();
  Import_EventSetExitRequest(5);
  goto L_0200a07c;
L_0200a042:
  t7 = GameFlag_Test(0x9ff);
  if (t7 == 0)
    goto L_0200a07c;
  t8 = GameFlag_Test(0x283);
  if (t8 == 0)
    goto L_0200a064;
  Func_02001754(0);
L_0200a064:
  t10 = GameFlag_Test(0x385);
  if (t10 == 0)
    goto L_0200a078;
  Func_02002db0();
  goto L_0200a07c;
L_0200a078:
  Func_02002db4();
L_0200a07c:
  t13 = ((s32)Actor_GetById(gMainRoutineWork.currentOwner));
  FIELD_AT(t13, u8, 35) = (64 | FIELD_AT(t13, u8, 35));
  Func_02001fc4(1);
  Func_020006b4(10);
  t16 = GameFlag_Test(0x9fc);
  if (t16 == 0)
    goto L_0200a0bc;
  Actor_SetPosition(10, PIXELS(136), PIXELS(328));
L_0200a0bc:
}

/* VIEW(s1fc4): variable gData_02005b64 as extern s32(*const gData_02005b64[]) (void); the definition has void(*gData_02005b64[]) (void) = { Func_02001904, Func_02001974, Func_02001a70, Func_02001afc, Func_02001bdc, Func_02001bec, Func_02001c6c, ...; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/trial_road/s15a8.c:518: `gData_02005b64' undeclared (first use in this function). */
extern s32 (*const gData_02005b64__s1fc4[])(void) __asm__("gData_02005b64");

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  s32 pattern = gTrialRoadState->pattern;

  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 344) != 0)
    return 0;
  if (scene == (s32)&SceneId_Ov697Room1) {
    if (entrance <= 7) {
      GameFlag_SetBit(324);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
      gData_02005b64__s1fc4[pattern & 7]();
    } else if (entrance == 55) {
      Func_02003900();
    }
  }
  if (scene == (s32)&SceneId_Ov697Room3)
    Func_02001dd0();
  if (scene == (s32)&SceneId_Ov697Room4)
    Func_02001eb0();
  if (scene == (s32)&SceneId_Ov697Room2) {
    GameFlag_SetBit(324);
    Func_0200200c();
  }
  return 0;
}

void Func_0200217c(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  s16 row;
  s16 col;

  cells += src;
  for (row = height - 1; row >= 0; row--) {
    u32 *dstCell = dstCells + ((row << 7) + width) - 1;
    u32 *srcCell = cells + ((row << 7) + width) - 1;
    u8 *dstShape = dstShapes + (row << 7) + width - 1;
    u8 *srcShape = srcShapes + (row << 7) + width - 1;

    for (col = width - 1; col >= 0; col--) {
      *dstCell-- = *srcCell--;
      *dstShape = *srcShape;
      dstShape--;
      srcShape--;
    }
  }
}

void Func_020002a4(s32 mirrored, s32 stage);

void Func_0200223c(void) {
  struct TrialRoadStage *state = (struct TrialRoadStage *)gTrialRoadState;
  struct TrialRoadState *road = gTrialRoadState;
  s32 side;

  if (state->stage > 3)
    return;
  if (Func_020056e8() != 0) {
    if (state->timer == 0) {
      side = Func_020000fc(1) ^ 1;
      Func_020056c4(gData_02005b84[side + 8]);
      state->timer = side * 5 * 120 + 1;
    } else {
      if (state->stage >= state->best)
        state->best = state->stage + 1;
      if (++road->field16[1] <= 3) {
        s32 pattern =
            state->order[state->stage * 2 + (s16)(state->mirrored ^ 1)];

        Func_020056c4(gData_02005b84[pattern & 7]);
        Actor_GetById(8)->terrainHeight = 0;
        Actor_GetById(8)->y = 0;
      } else {
        Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
      }
      state->timer = 0;
    }
  }
  {
    u8 *kind = &Actor_GetById(8)->animationKind;

    if (state->current == state->stage)
      *kind = 1;
    else
      *kind = 0;
  }
  if (state->timer == 0)
    return;
  if (GameFlag_Test(260))
    return;
  if (state->current == state->stage) {
    switch (state->timer) {
    case 1:
    case 601:
      Sound_Play(124);
      if (state->mirrored != 0) {
        Actor_SetModeById(9, 6);
        if (state->timer > 600)
          Actor_SetModeById(10, 6);
      } else {
        Actor_SetModeById(11, 6);
        if (state->timer > 600)
          Actor_SetModeById(12, 6);
      }
      break;
    case 655:
      Sound_Play(125);
      if (state->mirrored != 0)
        Actor_SetModeById(10, 5);
      else
        Actor_SetModeById(12, 5);
      break;
    case 653:
    case 708:
      Actor_GetById(8)->facing = 0xc000;
      break;
    case 50:
    case 710:
      Sound_Play(125);
      if (state->mirrored != 0)
        Actor_SetModeById(9, 5);
      else
        Actor_SetModeById(11, 5);
      break;
    case 70:
    case 730:
      Func_020002a4(state->mirrored, state->stage);
      Sound_Play(138);
      break;
    }
  }
  road->field14++;
}

/* VIEW(s1fc4): variable gData_02005bac as extern const s16 gData_02005bac[][2]; the definition has u16 gData_02005bac[] = { 1, 48, 15, 48, 29, 48, 1, 70, 15, 70, 29, 70, 1, 92, 15, 92, }; tried as the definition's extern u32 gData_02005bac[], it does not compile in tla-ja: games/tla/src/scene/trial_road/s15a8.c:782: subscripted value is neither array nor pointer. */
extern const s16 gData_02005bac__s1fc4[][2] __asm__("gData_02005bac");
/* VIEW(s1fc4): variable gData_02005bcc as extern const s8 gData_02005bcc[]; the definition has u8 gData_02005bcc[] = { 0, 1, 2, 3, 4, 4, 4, 5, 6, 7, }; tried as the definition's extern struct ActorPlacement gData_02005bcc[], it does not compile in tla-ja: games/tla/src/scene/trial_road/s15a8.c:791: invalid operands to binary !=. */
extern const s8 gData_02005bcc__s1fc4[] __asm__("gData_02005bcc");
/* VIEW(s1fc4): variable gData_02005bd6 as extern const s8 gData_02005bd6[]; the definition has u8 gData_02005bd6[] = { 0, 1, 2, 3, 4, 4, 4, 5, 6, 7, }; tried as the definition's extern u8 gData_02005bd6[], Scene_PrepareMap moves: 1 of 868 instructions in tla-ja. */
extern const s8 gData_02005bd6__s1fc4[] __asm__("gData_02005bd6");

void *Unit_Get(s32 owner);
void Func_02005688(s32 a);

// Places `layer` `column` cells into the map: its offset (column << 20), its
// cells and terrain from that column on, and its position from the view. The
// offset takes the literal, the cells and terrain the column's variable:
// with the variable in the offset too, CSE keeps a shift of the register;
// with the literal in the cells, the buffer and offset fold into one literal;
// through gMapCellBuffer's name, the address stays in a register across calls.
#define SET_LAYER(column, x)                                                   \
  words = (s32 *)layer;                                                        \
  words[2] = (column) << 20;                                                   \
  words[3] = row;                                                              \
  x = (column);                                                                \
  layer->cells = (struct MapCell *)Ram_MapCellBuffer + x;                      \
  layer->terrainIds = gMapShapeGrid + x;                                       \
  words[0] = Iwram_MulQ16(*viewX, words[4]) + words[2];                        \
  words[1] = Iwram_MulQ16(*viewZ, words[5]) + words[3]

s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;

  if (scene == (s32)&SceneId_Ov697Room1) {
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct TrialRoadDeal *state = (struct TrialRoadDeal *)gTrialRoadState;
    struct Actor *leader;
    u8 *unit;
    s32 value;
    s32 n;
    s32 px;
    u32 i;

    if ((u32)(entrance - 31) <= 1) {
      if (!GameFlag_Test(896)) {
        unit = Unit_Get(gMainRoutineWork.currentOwner);
        Iwram_ClearWords(state, 32);
        value = 0;
        for (n = 0; n <= 13; n++)
          value += unit[n];
        Func_02002c30(value);
        GameFlag_SetBit(896);
      }
      state->mirrored = (entrance - 1) & 1;
      state->field16[0] = 0;
      state->field16[1] = 0;
      state->field1a = 0;
      state->field1c = 0;
      value = state->order[state->mirrored ^ 1];
      Func_020056c4((s32)gData_02005b84[value & 7]);
      state->pattern = state->order[state->field16[0] * 2 + state->mirrored];
      n = entrance % 10;
      Event_SetDestination((s32)&SceneId_Ov697Room1, n);
    } else if ((u32)(entrance - 33) <= 1) {
      if (!GameFlag_Test(896)) {
        unit = Unit_Get(gMainRoutineWork.currentOwner);
        Iwram_ClearWords(state, 32);
        value = 0;
        for (n = 0; n <= 13; n++)
          value += unit[n];
        Func_02002c30(value);
        GameFlag_SetBit(896);
      }
      {
        /* FAKEMATCH(temporary): side, for both the store and its flip; as
         * state->mirrored, 2 of 879 instructions differ in tla-en. */
        s16 side = (entrance - 1) & 1;

        state->mirrored = side;
        state->field16[0] = 3;
        state->field16[1] = 3;
        state->field1a = 4;
        state->field1c = 4;
        side ^= 1;
        {
          /* FAKEMATCH(temporary): index; as state->order[side + 6], 2 of 879
           * instructions differ in tla-en. */
          s32 index = side + 6;

          value = state->order[index];
        }
      }
      Func_020056c4((s32)gData_02005b84[value & 7]);
      state->pattern = state->order[state->field16[0] * 2 + state->mirrored];
      n = entrance % 10;
      Event_SetDestination((s32)&SceneId_Ov697Room1, n);
    } else if (entrance == 50) {
      unit = Unit_Get(gMainRoutineWork.currentOwner);
      Iwram_ClearWords(state, 32);
      value = 0;
      for (n = 0; n <= 13; n++)
        value += unit[n];
      Func_02002c30(value);
      state->mirrored = 0;
      state->field16[0] = 0;
      state->field16[1] = 0;
      state->field1a = 0;
      state->field1c = 0;
      state->pattern = state->order[0];
      Event_SetDestination((s32)&SceneId_Ov697Room1, 55);
    } else {
      s32 *viewX = &map->viewX;
      s32 *viewZ = &map->viewZ;
      struct MapLayer *layer;
      s32 *words;
      s32 row;

      if (entrance == 55) {
        s32 x1;
        s32 x2;

        row = 0;
        layer = &map->layers[1];
        SET_LAYER(34, x1);
        layer = &map->layers[2];
        SET_LAYER(68, x2);
        Actor_SetPartAttribute(Actor_GetById(9), 3);
        Actor_SetPartAttribute(Actor_GetById(10), 3);
        Actor_SetPartAttribute(Actor_GetById(11), 3);
        Actor_SetPartAttribute(Actor_GetById(12), 3);
        Func_02005828(0, 12, 1, 21, 0, 13);
        Func_02005828(x1, 12, 1, 21, x1, 13);
        Func_02005828(x2, 12, 1, 21, x2, 13);
        Func_02005828(0, 13, 33, 21, 1, 13);
        Func_02005828(x1, 13, 33, 21, x1 + 1, 13);
        Func_02005828(x2, 13, 33, 21, x2 + 1, 13);
        value = state->pattern;
        px = gData_02005bac__s1fc4[value][0];
        row = gData_02005bac__s1fc4[value][1];
        Func_02005828(px, row, 13, 21, 1, 13);
        Func_02005828(px + 42, row, 13, 21, x1 + 1, 13);
        Func_02005828(px + 84, row, 13, 21, x2 + 1, 13);
        // The native neither hoists the table's address nor strength-reduces
        // the index; as a for loop, 10 more instructions differ (376).
        i = 0;
      hideUnused55:
        if (gData_02005bcc__s1fc4[i] != value)
          Actor_SetHorizontalPositionWithTerrain(i + 64, 0, 0);
        i++;
        if (i <= 9)
          goto hideUnused55;
        Map_CenterOnFollowedPoint();
      } else {
        s32 x1;
        s32 x2;
        s32 destinationY;

        row = 0;
        layer = &map->layers[1];
        SET_LAYER(34, x1);
        layer = &map->layers[2];
        SET_LAYER(68, x2);
        value = 1;
        if (entrance & value) {
          Func_0200217c(0, 0, 102, 13, 9, 0);
          Func_02005828(0, 0, 10, 1, 1, 0);
          Func_02005828(0, 0, 10, 12, 0, 1);
          Func_02005828(x1, 0, 10, 12, x1, 1);
          Func_02005828(x2, 0, 10, 12, x2, 1);
          Func_0200217c(0, 34, 102, 10, 9, 34);
          Func_02005828(0, 33, 10, 10, 0, 34);
          Func_02005828(x1, 33, 10, 10, x1, 34);
          Func_02005828(x2, 33, 10, 10, x2, 34);
        } else {
          Func_02005828(10, 0, 92, 13, 0, 0);
          Func_02005828(23, 0, 10, 1, 24, 0);
          Func_02005828(23, 0, 10, 12, 23, 1);
          Func_02005828(57, 0, 10, 12, 57, 1);
          Func_02005828(91, 0, 10, 12, 91, 1);
          Func_02005828(10, 34, 92, 10, 0, 34);
          Func_02005828(24, 33, 10, 10, 24, 34);
          Func_02005828(58, 33, 10, 10, 58, 34);
          Func_02005828(92, 33, 10, 10, 92, 34);
          Actor_SetPosition(9, 160 << 14, 152 << 16);
          Actor_SetPosition(10, 192 << 13, 152 << 16);
          Actor_SetPosition(11, 184 << 16, 152 << 16);
          Actor_SetPosition(12, 200 << 16, 152 << 16);
        }
        Actor_SetPartAttribute(Actor_GetById(9), 3);
        Actor_SetPartAttribute(Actor_GetById(10), 3);
        Actor_SetPartAttribute(Actor_GetById(11), 3);
        Actor_SetPartAttribute(Actor_GetById(12), 3);
        destinationY = 13;
        value = state->pattern;
        px = gData_02005bac__s1fc4[value][0];
        row = gData_02005bac__s1fc4[value][1];
        Func_02005828(px, row, 13, 21, 10, destinationY);
        Func_02005828(px + 42, row, 13, 21, 44, destinationY);
        Func_02005828(px + 84, row, 13, 21, 78, destinationY);
        // A goto loop for the same reason as the one above.
        i = 0;
      hideUnused:
        if (gData_02005bd6__s1fc4[i] != value)
          Actor_SetHorizontalPositionWithTerrain(i + 64, 0, 0);
        i++;
        if (i <= 9)
          goto hideUnused;
        Map_CenterOnFollowedPoint();
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        if (!GameFlag_Test(265))
          leader->y = leader->terrainHeight =
              Map_GetTerrainHeight(0, leader->x, leader->z);
        if (GameFlag_Test(2559)) {
          struct TrialRoadState *road;

          GameFlag_SetBit(324);
          // A pointer of its own keeps the state's address in r5 across the
          // calls; through gTrialRoadState each test reloads it (5 more).
          road = gTrialRoadState;
          // The call's argument reads through the other struct: through the
          // same one GCSE moves the load into both predecessors (11 more of
          // gap).
          if (road->field16[0] < road->field1a || GameFlag_Test(642)) {
            Func_02000218(road->mirrored,
                          ((struct TrialRoadDeal *)road)->field16[0]);
            GameFlag_SetBit(642);
            GameFlag_SetBit(644);
          }
          if (road->field16[0] < road->field1c)
            Func_020002a4(road->mirrored, road->field16[0]);
          Func_020055c4(gMainRoutineWork.currentOwner, 8);
          Func_02005688((s16)(state->mirrored ^ 1));
          Task_Add(Func_0200223c, 0x480);
        } else {
          Func_02000218(state->mirrored, state->field16[0]);
          GameFlag_SetBit(642);
          GameFlag_SetBit(644);
          Func_020002a4(state->mirrored, state->field16[0]);
        }
      }
    }
  } else if (scene == (s32)&SceneId_Ov697Room3) {
  } else if (scene == (s32)&SceneId_Ov697Room4) {
  } else if (scene == (s32)&SceneId_Ov697Room2 && GameFlag_Test(2559) &&
             !GameFlag_Test(901)) {
    if (gTrialRoadState->mirrored != 0)
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 13, 79);
    else
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 18, 79);
  }
  return 0;
}

// Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the
// eight slots at the start of the stage state, again until both the even and
// the odd slots hold at least one of the four values in gData_02005be0; the
// random state is restored after.

/* VIEW(s1fc4): variable gData_02005be0 as extern const s8 gData_02005be0[4]; the definition has u32 gData_02005be0[] = { 0x7030200, 0x6050401, }; tried as the definition's extern struct ActorPlacement gData_02005be0[], which the object already declares, it does not compile in tla-ja: games/tla/src/scene/trial_road/s15a8.c:932: `gData_02005be0' undeclared (first use in this function). */
extern const s8 gData_02005be0__s1fc4[4] __asm__("gData_02005be0");

/// Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the
/// eight slots at the start of the stage state, again until both the even and
/// the odd slots hold at least one of the four values in gData_02005be0; the
/// random state is restored after.
void Func_02002c30(u32 seed) {
  s16 *order = (s16 *)gTrialRoadState;
  u32 saved = gRandomState;
  s32 counts[2];
  s32 i;
  s32 k;
  s32 offset;
  s8 value;

  gRandomState = seed;
  do {
    for (i = 0; i <= 7; i++)
      order[i] = -1;
    for (i = 0; i <= 7; i++) {
      do {
        offset = ((Random16Far() << 3) >> 16) * 2;
      } while (*(s16 *)((u8 *)order + offset) != -1);
      *(s16 *)((u8 *)order + offset) = i;
    }
    counts[1] = 0;
    counts[0] = 0;
    for (i = 0; i <= 7; i++) {
      for (k = 0; k <= 3; k++) {
        value = gData_02005be0__s1fc4[k];
        if (order[i] == value)
          counts[i & 1]++;
      }
    }
  } while (counts[0] == 0 || counts[1] == 0);
  gRandomState = saved;
}

s32 Func_02002ce8(s32 side) {
  struct TrialRoadState *state;

  state = gTrialRoadState;
  return state->field16[side & 1];
}

s32 Func_02002cfc(void) { return gTrialRoadState->mirrored; }

s32 Func_02002d08(s32 x) {
  if (Func_02002cfc())
    return 512 - x;
  return x;
}

s32 Func_02002d20(s32 x) {
  if (Func_02002cfc())
    return (64 - x) << 3;
  return x << 3;
}

s32 Func_02002d38(s32 x) {
  if (Func_02002cfc())
    return -x << 3;
  return x << 3;
}

s32 Func_02002d50(s32 angle) {
  if (Func_02002cfc())
    return (0x8000 - angle) & 0xffff;
  return angle & 0xffff;
}

void Func_02002d78(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      id, Func_02002d08(x) << 16, z << 16, (u16)Func_02002d50(angle));
}

void Func_02002db0(void) {}

void Func_02002db4(void) {
  Func_02002d78(8, 288, 264, 0x8000);
  Func_02002d78(13, 296, 248, 0x8000);
  Func_02002d78(14, 296, 280, 0x8000);
}

void Actor_MoveToObject(s32 actor, s32 value);

void Func_02002dec(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(0x1000000, -1, 0x1080000, 1);
  Actor_MoveToObject(5, 4);
  Actor_MoveToObject(6, 4);
  Actor_MoveToObject(7, 4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2(4, Func_02002d20(28), 272);
  Actor_WalkTo(7, Func_02002d20(28), 256);
  Actor_ResetAndSetPositionInMode2(5, Func_02002d20(27), 248);
  Actor_WalkTo(6, Func_02002d20(27), 280);
  Actor_WaitForMoveById(4);
  Actor_ArmCallback(4, Func_02002d50(0), 0);
  Actor_WaitForMoveById(7);
  Actor_ArmCallback(7, Func_02002d50(0), 0);
  Actor_WaitForMoveById(5);
  Actor_ArmCallback(5, Func_02002d50(0), 0);
  Actor_WaitForMoveById(6);
  Actor_ArmCallback(6, Func_02002d50(0), 0);
  Func_02005980();
  wait(20);
  Sound_Play(138);
  wait(20);
  Func_02002d78(8, 344, 296, 0xc000);
  object = Actor_GetById(8);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->motionFlags = 0;
  Actor_SetMoveTarget(object, object->x, object->y + 0x580000, object->z);
  Func_020057c8(object, 2);
  Script_WaitForEventTimeout(object);
  Func_020057c8(object, 1);
  object->motionFlags = 2;
  object->terrainHeight = 0x800000;
  object->velocityY = 0x40000;
  object->speedLimit = 0x30000;
  object->acceleration = 0x20000;
  Actor_SetMoveTarget(object, object->x, object->y, object->z - 0x100000);
  Script_WaitForEventTimeout(object);
  wait(12);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  FieldActor_SetMoveTarget(object, Func_02002d20(41) << 16, object->y,
                           264 << 16);
  Script_WaitForEventTimeout(object);
  wait(6);
  Sound_Play(152);
  object->velocityY = 0x60000;
  object->speedLimit = 0x30000;
  object->acceleration = 0x20000;
  Actor_SetMoveTarget(object, Func_02002d20(37) << 16, object->y, 264 << 16);
  Script_WaitForEventTimeout(object);
  wait(6);
  Actor_MoveToObject(13, 8);
  Actor_MoveToObject(14, 8);
  Actor_SetSpeedParameters(8, 0x10000, 0x8000);
  Actor_SetSpeedParameters(13, 0x10000, 0x8000);
  Actor_SetSpeedParameters(14, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2(8, Func_02002d20(36), 264);
  Actor_ResetAndSetPositionInMode2(13, Func_02002d20(37), 248);
  Actor_ResetAndSetPositionInMode2(14, Func_02002d20(37), 280);
  Actor_WaitForMoveById(8);
  Actor_ArmCallback(8, Func_02002d50(0x8000), 0);
  Actor_WaitForMoveById(13);
  Actor_ArmCallback(13, Func_02002d50(0x8000), 0);
  Actor_WaitForMoveById(14);
  Actor_ArmCallback(14, Func_02002d50(0x8000), 0);
  Event_SetMessageId((s32)MsgTrialRoadSwiftChallenge);
  Func_080d47b4(8, 256, 10);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(4, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Event_Wait(15);
  Actor_SetSpeedParameters(4, 0x20000, 0x10000);
  Actor_SetSpeed(5, 0x1e666, 0xf333);
  Actor_SetSpeed(6, 0x1b333, 0xd999);
  Actor_SetSpeed(7, 0x1cccc, 0xe666);
  Actor_SetSpeedParameters(8, 0x20000, 0x10000);
  Actor_SetSpeed(13, 0x1e666, 0xf333);
  Actor_SetSpeedParameters(14, 0x1b333, 0xd999);
  Actor_OffsetPositionAndReset(4, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(8, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(5, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(13, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(7, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(14, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(6, Func_02002d38(3), 0);
  wait(1);
  Event_Wait(6);
}

void Func_0200322c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  Camera_MoveWithinMapBounds(PIXELS(256), -1, PIXELS(264), 1);
  Actor_MoveToObject(5, 4);
  Actor_MoveToObject(6, 4);
  Actor_MoveToObject(7, 4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2(4, Func_02002d20(28), 272);
  Actor_WalkTo(7, Func_02002d20(28), 256);
  Actor_ResetAndSetPositionInMode2(5, Func_02002d20(27), 248);
  Actor_ResetAndSetPositionInMode2(6, Func_02002d20(27), 280);
  Actor_WaitForMoveById(4);
  Actor_ArmCallback(4, Func_02002d50(0), 0);
  Actor_WaitForMoveById(7);
  Actor_ArmCallback(7, Func_02002d50(0), 0);
  Actor_WaitForMoveById(5);
  Actor_ArmCallback(5, Func_02002d50(0), 0);
  Actor_WaitForMoveById(6);
  Actor_ArmCallback(6, Func_02002d50(0), 0);
  Func_02005980();
  Event_SetMessageId((s32)MsgTrialRoadSlowArrival);
  Func_080d47b4(8, 256, 10);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(4, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Event_Wait(15);
  Actor_SetSpeed(4, 0x20000, 0x10000);
  Actor_SetSpeed(5, 0x1e666, 0xf333);
  Actor_SetSpeed(6, 0x1b333, 0xd999);
  Actor_SetSpeed(7, 0x1cccc, 0xe666);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  Actor_SetSpeed(13, 0x1e666, 0xf333);
  Actor_SetSpeedParameters(14, 0x1b333, 0xd999);
  Actor_OffsetPositionAndReset(4, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(8, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(5, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(13, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(7, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(14, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(6, Func_02002d38(3), 0);
  wait(1);
  Event_Wait(6);
}

void PartyInventory_Discard(s32 cue);

void Func_020034b4(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02002d78(4, 248, 272, 0);
  Func_02002d78(7, 224, 248, 0);
  Func_02002d78(5, 232, 264, 0);
  Func_02002d78(6, 224, 280, 0);
  Func_02002d78(9, 248, 256, 0);
  Func_02002d78(8, 272, 264, 0x4000);
  Func_02002d78(13, 288, 248, 0x4000);
  Func_02002d78(14, 288, 280, 0x4000);
  Actor_SetModeById(8, 6);
  Actor_SetModeById(13, 6);
  Actor_SetModeById(14, 6);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadHeroesProven);
  Actor_StartRepeatedMotion(13, 2);
  wait(4);
  Actor_StartRepeatedMotion(14, 2);
  wait(10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(8, 7);
  Event_Wait(15);
  Actor_SetModeById(13, 7);
  Event_Wait(10);
  Actor_SetModeById(14, 7);
  Event_Wait(5);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Actor_ArmCallback(8, Func_02002d50(0x8000), 0);
  Actor_SetModeById(13, 1);
  Event_Wait(10);
  Actor_ArmCallback(13, Func_02002d50(0x8000), 0);
  Actor_SetModeById(14, 1);
  Event_Wait(5);
  Actor_ArmCallback(14, Func_02002d50(0x8000), 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetSpeedParameters(8, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2(8, Func_02002d20(32), 264);
  Actor_TurnToFaceEachOther(9, 7, 0);
  Actor_TurnToFaceEachOther(5, 6, 0);
  Actor_WaitForMoveById(8);
  Actor_SetAngleToward(9, 4, 0);
  Actor_SetAngleToward(7, 4, 0);
  Actor_SetAngleToward(6, 4, 0);
  Actor_SetAngleToward(5, 4, 0);
  Event_Wait(20);
  PartyInventory_Discard(65);
  PartyInventory_GiveItem(215, 0);
  Actor_SetAngleToward(4, 8, 0);
  OBJECT_BYTE(Actor_GetById(8), 0x5a) &= ~1;
  Actor_SetPositionAndReset(8, Func_02002d20(35), 264);
  Event_Wait(1);
  /* FAKEMATCH(signature): the byte typed as a whole-byte bit-field, as in
   * overlay 685; as a plain byte, which stores in the order of a store that
   * aliases what follows, 4 of 304 instructions differ in tla-en. */
  ((struct ActorSolid *)Actor_GetById(8))->solid |= 1;
  Event_Wait(20);
  Func_080d47b4(8, 261, 10);
  Event_ShowMessageAndWait(8, 0);
  Actor_ArmCallbackAndRefresh(8, Func_02002d50(0));
  Actor_SetAngleToward(13, 8, 0);
  Actor_SetAngleToward(14, 8, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(13, 3);
  wait(3);
  Motion_SetModeAndWaitAnimation(14, 3);
  Actor_SetAngleToward(4, 9, 0);
  Actor_SetAngleToward(7, 9, 0);
  Actor_SetAngleToward(5, 9, 0);
  Actor_SetAngleToward(6, 9, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(4, 3);
  wait(2);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  wait(1);
  Actor_SetModeById(5, 3);
  wait(30);
}

void Func_020037c8(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02002d78(8, 264, 264, 0x8000);
  Func_02002d78(13, 280, 248, 0x8000);
  Func_02002d78(14, 280, 280, 0x8000);
  Func_02002d78(4, 240, 276, 0);
  Func_02002d78(7, 216, 254, 0);
  Func_02002d78(5, 232, 244, 0);
  Func_02002d78(6, 224, 296, 0);
  Actor_SetModeById(4, 38);
  Actor_SetModeById(5, 19);
  Actor_SetModeById(6, 19);
  Actor_SetModeById(7, 19);
  Func_080d3928(7, 0x4000);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetSingleChildFlags(Actor_GetById(5), 0);
  Actor_SetSingleChildFlags(Actor_GetById(6), 0);
  Actor_SetSingleChildFlags(Actor_GetById(7), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadLeaveVillage);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_StartRepeatedMotion(4, 2);
  wait(1);
  Actor_StartRepeatedMotion(6, 2);
  wait(2);
  Actor_StartRepeatedMotion(5, 2);
  Actor_StartRepeatedMotion(7, 2);
  Event_Wait(15);
}

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.

/* VIEW(s1fc4): variable gData_02005dc8 as extern const u8 gData_02005dc8[]; the definition has u32 gData_02005dc8[] = { 22, 15, 0x18000, 22, 16, 0x10000, 39, 5, 4, 0xd80000, 0, 0xa80000, 1, 4, 0xe80000, 0, 0x980000, 1, 4, 0xe80000, 0, ...; tried as the definition's extern s16 gData_02005dc8[], which the object already declares, it does not compile in tla-ja: games/tla/src/scene/trial_road/s15a8.c:1371: `gData_02005dc8' undeclared (first use in this function). */
extern const u8 gData_02005dc8__s1fc4[] __asm__("gData_02005dc8");

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 actor, s32 dx,
                                                           s32 dz) {
  Actor_OffsetPositionAndResetMotion(actor, dx, dz);
}

void Func_02003900(void) {
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, PIXELS(1016));
  Actor_SetPosition(8, PIXELS(1016), PIXELS(1016));
  for (i = 9; i < 80; i++) {
    struct Actor *object = Actor_GetById(i);

    if (object->x != 0)
      object->x -= 0x900000;
  }
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetMovementTypeById(13, gData_02005c88);
  Actor_SetMovementTypeById(14, gData_02005be8);
  Event_StartScreenTransition();
  Event_Wait(20);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(184 << 16, -1, 600 << 16, 1);
  Func_02005980();
  FieldCamera_MoveTo(120 << 16, -1, 552 << 16, 1);
  Func_02005980();
  FieldCamera_MoveTo(120 << 16, -1, 408 << 16, 1);
  Func_02005980();
  FieldCamera_MoveTo(120 << 16, -1, 120 << 16, 1);
  Event_SetMessageId((s32)MsgTrialRoadTrapInstructions);
  Event_ShowMessageAndWait(8, 0);
  Func_02005980();
  FieldCamera_MoveTo(216 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(4, 0);
  Actor_SetMovementTypeById(13, gData_02005d28);
  Actor_RefreshSelectorById(13);
  Actor_SetModeById(13, 1);
  Event_Wait(10);
  Actor_SetSpeed(13, 0x4000, 0x2000);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_CommitPositionAndActivate(13, 0, 8);
  Event_Wait(1);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) |= 1;
  Event_Wait(20);
  Actor_SetSpeed(13, 0x20000, 0x10000);
  Actor_SetModeById(13, 5);
  Actor_OffsetPositionAndResetMotion_(13, 0, -40);
  Actor_WaitForMoveById(13);
  Sound_Play(103);
  MapGrid_SetShake(0x20000, 0x40000, 0x10000);
  Actor_GetById(13)->velocityY = 0x40000;
  MapGrid_SetShake(-1, -1, 0xe666);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_CommitPositionAndActivate(13, 0, 16);
  Event_Wait(1);
  ((struct ActorSolid *)Actor_GetById(13))->solid |= 1;
  Event_Wait(60);
  Actor_ArmCallback(13, FACING_SOUTH, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(13, 4);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(13, FACING_NORTHWEST, 0);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  Actor_WalkByAndWait(13, -16, -16);
  Actor_FaceDirection(13, FACING_WEST, 0);
  Sound_Play(124);
  Actor_SetModeById(9, 6);
  Event_Wait(30);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetPositionAndReset(13, 200, 168);
  Actor_FaceDirection(13, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(9, 5);
  Event_Wait(20);
  Sound_Play(138);
  Func_0802b450(127, 3, 1, 2, 48, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 14, 8);
  Event_Wait(20);
  Actor_SetMovementTypeById(13, gData_02005dc8__s1fc4);
  Actor_RefreshSelectorById(13);
  Sound_Play(123);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetMovementTypeById(14, gData_02005e40);
  FieldCamera_MoveTo(328 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(14, FACING_NORTHEAST, 0);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Actor_SetSpeed(14, 0x10000, 0x8000);
  Actor_WalkByAndWait(14, 16, -16);
  Actor_ArmCallback(14, 0, 0);
  Sound_Play(124);
  Actor_SetModeById(11, 6);
  Actor_SetModeById(12, 6);
  Event_Wait(30);
  Actor_WalkToAndWait(14, 350, 168);
  Actor_FaceDirection(14, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(11, 5);
  Event_Wait(40);
  Actor_WalkToAndWait(14, 364, 168);
  Actor_ArmCallback(14, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(12, 5);
  Event_Wait(20);
  Sound_Play(138);
  Func_0802b450(127, 3, 1, 2, 53, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 19, 8);
  Event_Wait(30);
  Actor_ArmCallback(14, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_ShowMessageAndWait(8, 0);
  Camera_MoveWithinMapBounds(408 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Event_ShowMessageAndWait(8, 0);
  Actor_SetMovementTypeById(14, gData_02005ed4);
  Actor_RefreshSelectorById(14);
  Event_ShowMessageAndWait(8, 0);
  Import_EventSetExitRequest(10);
}

// Updates the sixteen movers at gData_020023c4 + 0x188 each frame: at its
// upAt and downAt phases a mover calls Sound_Play (206 or 395) and moves by
// 4 or -8 a frame until it reaches its high or low offset; its phase counts
// round its period unless flag 260 is set. Each mover's shape, collision
// height and cell attribute byte follow its actor and offset, and its marker
// actor is placed over it, bobbing on a sine while the offset is not 0.

/* VIEW(s1fc4): tag struct TrialRoadMover as struct TrialRoadMover__s1fc4; s15a8.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/trial_road/s15a8.c:1464: structure has no member named `phase'. */
struct TrialRoadMover__s1fc4 {
  s16 id;
  s16 phase;
  s16 offset;
  s16 period;
  s16 upAt;
  s16 downAt;
  s16 high;
  s16 low;
  s16 unk10;
  s16 speed;
  s16 unk14;
  s16 bob;
  struct Actor *marker;
};

struct TrialRoadMovers {
  u8 unk00[2];
  s16 shape;
  struct TrialRoadMover__s1fc4 movers[16];
};

/// Updates the sixteen movers at gData_020023c4 + 0x188 each frame: at its upAt
/// and downAt phases a mover calls Sound_Play (206 or 395) and moves by 4 or -8
/// a frame until it reaches its high or low offset; its phase counts round its
/// period unless flag 260 is set. Each mover's shape, collision height and cell
/// attribute byte follow its actor and offset, and its marker actor is placed
/// over it, bobbing on a sine while the offset is not 0.
void Func_02003da0(void) {
  struct TrialRoadMovers *work =
      (struct TrialRoadMovers *)(gData_020023c4 + 0x188);
  struct TrialRoadMover__s1fc4 *mover = work->movers;
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = work->shape;
  u8 *shapes = *(u8 **)((u8 *)map + 316);
  u8 *cells = map->layers[0].cells;
  s32 *collision;
  struct Actor *actor;
  struct Actor *marker;
  s32 cell;
  u8 *attributes;
  s32 i;

  i = 0;
  collision = (s32 *)Ram_MapCollision + shape;
  for (; i <= 15; collision++, i++, mover++) {
    if (mover->id == 0)
      continue;
    actor = Actor_GetById(mover->id);
    if (actor->x == 0 && actor->z == 0)
      continue;
    if (mover->phase == mover->upAt) {
      Sound_Play(206);
      mover->speed = 4;
    }
    if (mover->phase == mover->downAt) {
      Sound_Play(395);
      mover->speed = -8;
    }
    if (mover->speed != 0) {
      mover->offset += mover->speed;
      if (mover->offset >= mover->high) {
        mover->offset = mover->high;
        mover->speed = 0;
        mover->bob = 0;
      }
      if (mover->offset <= mover->low) {
        mover->offset = mover->low;
        mover->speed = 0;
        mover->bob = 0;
      }
    }
    if (!GameFlag_Test(260)) {
      if (++mover->phase >= mover->period)
        mover->phase = 0;
    }
    cell = (actor->x >> 20) + ((actor->z >> 20) << 7);
    shapes[cell] = (u8)work->shape + i;
    *collision = ((((s16 *)&actor->y)[1] + mover->offset) / 8) << 8;
    attributes = cells + cell * 4;
    if (mover->speed != 0)
      attributes[3] = 0;
    else {
      /* FAKEMATCH(temporary): the form kept; as attributes[3] |= 128, 4 of 213
         instructions differ in tla-en. */
      s32 bits = 128;
      bits |= attributes[3];
      attributes[3] = bits;
    }
    if (mover->offset != 0) {
      /* FAKEMATCH(temporary): the form kept; as attributes[3] |= 16, 4 of 213
         instructions differ in tla-en. */
      s32 bits = 16;
      bits |= attributes[3];
      attributes[3] = bits;
    } else
      attributes[3] = 0;
    marker = mover->marker;
    if (marker != 0) {
      if (mover->offset == 0) {
        marker->y = actor->y - 0x100000;
      } else {
        s32 wave;
        mover->bob += 0x1000;
        wave = Iwram_MulQ16(0x20000, Math_Sine(mover->bob)) - 0xe0000;
        marker->y = actor->y + (mover->offset << 16) + wave;
      }
      marker->x = actor->x;
      marker->z = actor->z;
    }
  }
}
#undef FIELD_AT
#undef SET_LAYER

// ---- games/tla/src/scene/trial_road/s3f68.c ----
// Trial Road's movers' sprites, the setup of its gauge, a rising platform's
// entry, the trap table's setup, Func_02004418, the placing of actors 8, 9
// and 10, Func_02004828 and a second copy of
// Effect_AdvanceMotionAtLeaderPriority, in ROM order.
#include "frame.h"

/* VIEW(s3f68): tag struct TrialRoadMover as struct TrialRoadMover__s3f68; s15a8.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/trial_road/s15a8.c:1598: structure has no member named `offset'. */
struct TrialRoadMover__s3f68 {
  s16 id;
  s16 phase;
  s16 offset;
  s16 unk06[8];
  s16 bob;
  struct Actor *marker;
};

/* VIEW(s3f68): tag struct TrialRoadMovers as struct TrialRoadMovers__s3f68; s15a8.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/trial_road/s15a8.c:1589: structure has no member named `vramEntry'. */
struct TrialRoadMovers__s3f68 {
  s16 vramEntry;
  s16 shape;
  struct TrialRoadMover__s3f68 movers[16];
  u32 *entries;
};

/// Queues the Trial Road movers' sprites.
void Func_02003f68(void) {
  struct TrialRoadMovers__s3f68 *base =
      (struct TrialRoadMovers__s3f68 *)(gData_020023c4 + 0x188);
  struct TrialRoadMover__s3f68 *mover = base->movers;
  s32 i;
  s32 tile;
  s32 frame;
  s32 count;
  s32 viewX;
  s32 viewZ;
  s32 viewY;
  s32 rise;
  struct MapWork *map;
  s32 *view;
  struct Actor *object;
  u32 *entry;
  u32 *cursor;
  s32 lift;
  s32 x;
  s32 height;
  s32 z;
  s32 sy;
  s32 depth;
  s32 bob;

  tile = gResourceTableEntries[base->vramEntry].offset >> 5;
  entry = base->entries;
  count = 0;
  rise = -0x20000;
  map = Ram_HeapSlots->mapWork;
  view = &map->viewX;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  viewY = map->origin[1];

  for (i = 0; i < 16; i++, mover++) {
    if (mover->id == 0 || mover->offset == 0)
      continue;
    object = Actor_GetById(mover->id);
    frame = ((gFrameCount % 12) / 3) * 8 + 32;
    lift = 0;
    if (lift < mover->offset) {
    next:
      if (count > 79)
        goto skip;
      x = object->x - viewX;
      height = object->y + (lift << 16) - viewY;
      z = object->z - viewZ - viewY;
      x = (x >> 16) - 8;
      sy = z - height;
      sy = (sy >> 16) - 16;
      depth = ((height + z) >> 16) + 58;
      if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
        goto skip;
      x &= 0x1ff;
      sy &= 0xff;
      cursor = entry;
      *cursor++ = 0;
      *cursor++ = sy | (x << 16) | 0x40002000;
      *cursor = (tile + frame) | 0x800;
      Oam_AddSpriteAtDepth(entry, depth);
      entry += 3;
      count++;
    skip:
      lift += 16;
      if (lift < mover->offset)
        goto next;
    }
    if (count > 79)
      continue;
    x = object->x - viewX;
    height = object->y + (mover->offset << 16) - viewY;
    z = object->z - viewZ - viewY;
    x = (x >> 16) - 8;
    sy = z - height;
    bob = Iwram_MulQ16(rise, Math_Sine(mover->bob));
    sy = ((sy + bob + rise) >> 16) - 4;
    depth = ((height + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 0x1ff;
    sy &= 0xff;
    cursor = entry;
    *cursor++ = 0;
    *cursor++ = sy | (x << 16) | 0x40002000;
    *cursor = (tile + frame - 32) | 0x800;
    Oam_AddSpriteAtDepth(entry, depth);
    entry += 3;
    count++;
  }
}

#include "actor.h"

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void *Pool_Alloc(s32 size) { return Heap_AllocEwram(size); }

static __inline__ void Words_Clear(void *destination, s32 size) {
  Iwram_ClearWords(destination, size);
}

void Func_02004188(s32 a, s32 b) {
  struct TrialRoadGauge *gauge =
      (struct TrialRoadGauge *)(gData_020023c4 + 0x188);
  void *buffer;

  if (!GameFlag_Test(0x109))
    Words_Clear(gauge, 0x1c8);
  buffer = Pool_Alloc(0x800);
  Func_02005770(gField_Tiles64D, buffer);
  gauge->handle = Resource_FindFreeEntry();
  VramCache_Load((s16)gauge->handle, 0x800, buffer);
  Heap_Release(buffer);
  gauge->buffer = Heap_AllocEwram(0x3c0);
  gauge->field02 = b;
  Task_Add(Func_02003da0, 0x48d);
  Task_Add(Func_02003f68, 0x480);
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x40;
}

/// One rising platform and what rides it.
struct RideEntry {
  s16 id;
  s16 timer;
  s16 height;
  s16 period;
  s16 riseAt;
  s16 fallAt;
  s16 max;
  s16 min;
  s16 unk10;
  s16 step;
  s16 unk14;
  s16 phase;
  struct Actor *rider;
};

struct RideWork {
  s16 entry;
  s16 value;
  struct RideEntry entries[16];
  void *block;
};

struct RideEntry *Func_02004224(s32 slot, s32 id, s32 timer, s32 low,
                                s32 high) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct RideEntry *entry = &work->entries[slot];
  struct Actor *object = Actor_GetById(id);
  struct Actor *rider;

  Actor_GetById(id);
  Actor_SetSingleChildFlags(object, 0);
  if (!GameFlag_Test(0x109)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    entry->id = id;
    entry->height = 0;
    entry->timer = timer;
    entry->period = 360;
    entry->max = high;
    entry->riseAt = 60;
    entry->fallAt = 240;
    entry->min = low;
    entry->unk10 = 1;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    entry->step = 0;
    entry->phase = 0;
    entry->unk14 = 0;
    object->priorityFlags |= 2;
  }
  rider = ((struct Actor *)Import_ActorCreate(0x8000, object->x, object->y,
                                              object->z));
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= 128;
  entry->rider = rider;
  return entry;
}

// The trap table's setup: a trap whose second flag is set is put away; any
// other marks its cell, writes a shape for its height into the shape grid, and
// takes mode 0 when its first flag is set. Then the leader is lifted onto the
// ground if it stands below it.

/// The trap table's setup: a trap whose second flag is set is put away; any
/// other marks its cell, writes a shape for its height into the shape grid, and
/// takes mode 0 when its first flag is set. Then the leader is lifted onto the
/// ground if it stands below it.
void Func_020042f0(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  struct Actor *leader;
  s32 ground;

  if (*table == 0xffff)
    goto done;
next:
  id = table[0];
  object = Actor_GetById(id);
  flag = table[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  if (GameFlag_Test(nextFlag)) {
    Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Func_02005a18(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = Func_02005848(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Func_02005958(id);
    cell->attributeB = 128;
    *slot = Func_02005a20(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Test(flag))
      Func_020057c8(object, 0);
  }
  table += 2;
  if (*table != 0xffff)
    goto next;
done:
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ground = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < ground) {
    leader->terrainHeight = ground;
    leader->y = ground;
  }
}

void Func_02004418(const u16 *table) {
  struct Actor *leader;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Func_020059c0() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Func_02005a28(position, 0);
  if (trap != 0)
    goto test;
  return;
next:
  id = table[0];
  if (Actor_GetById(id) != trap) {
    table += 2;
    goto test;
  }
  {
    flag = table[1];
    Event_Start();
    if (!GameFlag_Test(flag)) {
      Sound_Play(125);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(2);
      Func_020057c8(trap, 0);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(id), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(id), 0);
      Func_02004a94(0);
      GameFlag_SetBit(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Test(second)) {
        leader->motionFlags = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02004a94(0);
        wait(8);
        Func_020057c8(trap, 2);
        Func_02004d6c(-2, trap->terrainId);
        Func_02004a94(1);
        wait(16);
        Func_02004d6c(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(trap, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02004bc8();
        wait(20);
        GameFlag_SetBit(second);
      }
    }
    Event_End();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}

void Func_020045ac(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *object = Actor_GetById(id);
  s32 flag;

  while (entry->actor != 0xffff) {
    if (entry->actor != id) {
      entry++;
      continue;
    }
    {
      flag = entry->flag;
      Event_Start();
      if (!GameFlag_Test(flag + 1)) {
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02004a94(0);
        wait(8);
        Func_020057c8(object, 2);
        Func_02004ce0(-4, OBJECT_BYTE(object, 34), id);
        Func_02004a94(1);
        wait(16);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        Actor_SetPositionAndResetMotion(object, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02004bc8();
        wait(20);
        GameFlag_SetBit(flag);
        GameFlag_SetBit(flag + 1);
      }
      Event_End();
      break;
    }
  }
}

void Func_0200468c(s32 id) {
  Actor_GetById(id)->unk5c[0] = 3;
  Func_02005958(id);
}

void Actor_MoveToObject(s32 id, s32 owner);
/// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
/// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
/// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
/// it copy actor 9's visual attributes.
void Func_020046a4(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  u8 *grid = gMapShapeGrid; // the shape grid, read once

  Actor_MoveToObject(8, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_0200468c(8);
  Func_0200468c(9);
  Func_0200468c(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  wait(1);
  if (*list == 0xffff)
    goto done;
next:
  id = list[0];
  object = Actor_GetById(id);
  flag = list[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->unk5c[0] = 3;
  if (GameFlag_Test(nextFlag)) {
    Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Func_02005a18(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = Func_02005848(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Func_02005958(id);
    cell->attributeB = 128;
    *slot = Func_02005a20(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Test(flag))
      ObjectVisual_CopyAttributes(id, 9);
  }
  list += 2;
  if (*list != 0xffff)
    goto next;
done:
  wait(10);
}

void Func_0200480c(struct Actor *object, struct Actor *target) {
  Actor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  Actor_SetPositionAndResetMotion(target, 0, 0, 0);
}

#include "field_effect.h"

void Func_02004828(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;
  u8 *motion;

  if (Func_020059c0() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  partner = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Func_02005a28(position, 0);
  if (trap != 0) {
    goto test;
  next:
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      goto test;
    }
    flag = table[1];
    Event_Start();
    if (!GameFlag_Test(flag)) {
      Func_020057c8(partner, 1);
      Func_0200480c(partner, trap);
      wait(1);
      Sound_Play(125);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(2);
      Func_020057c8(partner, 0);
      ObjectVisual_CopyAttributes(id, 9);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      Func_02004a94(0);
      Func_0200480c(trap, partner);
      wait(1);
      GameFlag_SetBit(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Test(second)) {
        Func_020057c8(partner, 0);
        Func_0200480c(partner, trap);
        motion = &leader->motionFlags;
        wait(1);
        *motion = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02004a94(0);
        wait(8);
        Func_020057c8(partner, 2);
        Func_02004d6c(-2, trap->terrainId);
        Func_02004a94(1);
        wait(16);
        Func_02004d6c(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *motion = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02004bc8();
        wait(20);
        GameFlag_SetBit(second);
      }
    }
    Event_End();
    return;
  test:
    if (*table != 0xffff)
      goto next;
  }
}

/// A second copy of Effect_AdvanceMotionAtLeaderPriority in this overlay:
/// Effect_AdvanceMotion, then the leader's sprite priority for the effect.
void Func_02004a14(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX = effect->velocity.x;
  s32 velocityZ;
  struct MainRoutineWork *party;
  struct FieldSprite *sprite;
  union FieldObject *leader;
  struct EffectSpriteAttributes *to;
  struct EffectSpriteAttributes *from;
  struct EffectSpriteBits source;
  struct EffectSpriteBits bits;

  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;
  effect->velocity.x = velocityX - velocityX / 18;
  effect->velocity.z = velocityZ - velocityZ / 16;
  /* FAKEMATCH(dead): an empty once-only block ends the velocity stores, so the
   * scale step's loads follow the z velocity's store; without it, 2 of 59
   * instructions differ in tla-en. */
  do {
  } while (0);
  {
    /* FAKEMATCH(temporary): the x scale rate read into a block temporary,
     * which loads it before the x scale; as effect->scaleX +=
     * effect->scaleRateX, 2 of 59 instructions differ in tla-en. */
    s32 rate = effect->scaleRateX;

    effect->scaleX += rate;
  }
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;

  party = &gMainRoutineWork;

  leader = Actor_GetById(party->currentOwner);
  to = (struct EffectSpriteAttributes *)effect->sprite;
  from = (struct EffectSpriteAttributes *)leader->effect.sprite;
  source = from->bits;
  bits = to->bits;
  bits.priority = source.priority;
  to->bits = bits;
}

// ---- games/tla/src/scene/trial_road/s4a94.c ----
#include "field_sprite.h"

extern const s32 gData_02006254[];

/* VIEW(s4a94): routine Actor_StartScript as void Actor_StartScript(struct FieldEffect *effect, const s32 *script); trial_road.h has void Actor_StartScript(struct Actor *object, const void *script); not tried: Actor_StartScript's bound declaration was not found. */
extern void Func_020057d0__s4a94(struct FieldEffect *effect, const s32 *script) __asm__("Actor_StartScript");

void Func_02004a94(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16Far() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Func_020059f0(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Func_020057c8(effect, 2);
      Func_020057d0__s4a94(effect, gData_02006254);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Math_Cosine(i << 13), 0x13333);
        effect->velocity.z = Math_Sine(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16Far() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16Far() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Func_02004a14;
    }
    i++;
  } while (i <= 7);
}

extern const s32 gData_02006284[];

void Func_02004bc8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)Import_ActorCreate(285, leader->x, leader->y,
                                                      leader->z);
    if (effect != 0) {
      frame = Func_020059f0(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Func_020057c8(effect, 2);
      Func_020057d0__s4a94(effect, gData_02006284);
      effect->velocity.x = Math_Cosine(i << 12);
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sine(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16Far() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Func_02004a14;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
    i++;
  } while (i <= 16);
}

#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"

void Func_02004ce0(s32 level, s32 layer, s32 owner) {
  struct Actor *object = Actor_GetById(owner);
  u8 *shape = Ram_MapShapeGrid + (Ram_HeapSlots->mapWork->layers[layer].cells -
                                  (struct MapCell *)Ram_MapCellBuffer);
  s32 value = Func_02005848(object->terrainId, object->x, object->z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;

  height += level;

  shape += object->x / 0x100000 + object->z / 0x100000 * 128;
  *shape = Func_02005a20(object->terrainId, value, height);
}

void Func_02004d6c(s32 a, s32 b) {
  Func_02004ce0(a, b, gMainRoutineWork.currentOwner);
}

extern const u8 gData_020063ac[];

/// Writes value as eight hexadecimal digits and a terminating null.
UNUSED void String_WriteU32AsHex(u8 *buf, u32 value) {
  s32 i;
  const u8 *digits = gData_020063ac;

  buf += 8;
  *buf = 0;
  buf--;
  i = 7;
  // A goto loop, which builds the 15 on every pass: as a for loop, the 15 is
  // moved out of it (differ 13).
next:
  *buf = digits[value & 15];
  value >>= 4;
  buf--;
  i--;
  if (i >= 0)
    goto next;
}

#include "field_event.h"

extern u8 gSceneState[];

void TrialRoad_Idle(void) {}

void TrialRoad_SetBalanceStateReady(void) {
  u16 *state = (u16 *)gSceneState;
  /* FAKEMATCH(temporary): a local holding 9 builds it with movs; as a plain
   * constant, which is a halfword pool load, 4 of 4 instructions differ in
   * tla-en. */
  u16 ready = 9;

  *state = ready;
}

void TrialRoad_WaitForBalanceState(void) {
  s16 *state = (s16 *)gSceneState;

  while (*state != 9) {
    wait(1);
  }
}

// ---- games/tla/src/scene/trial_road/s4ddc.c ----
// The Trial Road rival's per-frame update.

#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

/// Updates the Trial Road rival by the scene state's mode: 4 follows the
/// leader's position mirrored across the course; 3 places the rival and its
/// partner from the linked player's record; 1 replays the recorded path; 2
/// records the actor's position.
void Func_02004ddc(void) {
  struct TrialRoadLinkWork *work =
      *(struct TrialRoadLinkWork **)((u8 *)Ram_HeapSlots + 236);
  struct TrialRoadReplay *state = &gSceneState;
  struct Actor *rival = Actor_GetById(state->id);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x, z, turn, dx, dz;
  u16 heading;

  if (rival == 0)
    return;
  if (state->mode == 4) {
    x = leader->x;
    z = leader->z;
    if ((u32)(z - 0x1000000) > 0x1300000)
      x = 0x2100000 - (x & 0xffff0000);
    if (gMainRoutineWork.entrance & 1)
      x += 0x1300000;
    else
      x -= 0x1300000;
    if (rival->x != x || rival->z != z) {
      dx = x - rival->x;
      dz = z - rival->z;
      heading = ArcTan2(dz, dx);
      turn = (s16)(heading - rival->facing);
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      rival->facing += turn;
      rival->x = x;
      rival->z = z;
    }
    if (state->still > 2)
      Func_020057c8(rival, 1);
    else
      Func_020057c8(rival, 5);
  } else if (state->mode == 3) {
    s32 player;
    s16 *signature;
    s32 angle, link, ok, partner;
    s16 st;

    player = ((s32)GameFlag_GetByte(760)) ^ 1;
    signature = (s16 *)gLinkPeerSignatures[player];
    if (((gLinkStatus >> player) & 1) == 0)
      return;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    st = *signature++;
    link = st >> 8;
    ok = st & 0xff;
    if (GameFlag_Test(dz = 529) && link != 0) {
      state->mode = 9;
      Func_020057c8(rival, 1);
      if (work->linkX < rival->x)
        x = work->linkX + 0xc0000;
      else
        x = work->linkX - 0xc0000;
      if (!GameFlag_Test(530)) {
        z = work->linkZ + 0x100000;
        rival->timer = work->linkAngle;
      } else {
        z = work->linkZ - 0x100000;
        rival->timer = work->unke6;
      }
      rival->acceleration = 0x4000;
      rival->speedLimit = 0x10000;
      Actor_SetMoveTarget(rival, x, 0, z);
      Func_020057f0(rival);
    } else {
      x <<= 16;
      z <<= 16;
      rival->facing = angle;
      rival->x = x;
      rival->z = z;
      Func_020057c8(rival, ok);
    }
    partner = *signature++;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    ok = *signature++;
    if (partner == 0)
      return;
    rival = Actor_GetById(partner);
    if (rival == 0)
      return;
    rival->x = x << 16;
    rival->facing = angle;
    rival->z = z << 16;
    Func_020057c8(rival, ok);
  } else if (state->mode == 1) {
    if (state->resource != work->resource) {
      Func_02005770(Resource_GetTableEntry(state->resource), work->path);
      work->resource = state->resource;
    }
    x = work->path[state->point];
    z = work->path[state->point + 1];
    if (!GameFlag_Test(260))
      state->point += 2;
    if (x == 0 && z == 0) {
      state->mode = 9;
      return;
    }
    x <<= 16;
    z <<= 16;
    if (state->mirrored != 0 && (u32)(z - 0x1000000) > 0x1300000)
      x = 0x2100000 - x;
    if (gMainRoutineWork.entrance & 1)
      x += 0x1300000;
    else
      x -= 0x1300000;
    if (rival->x != x || rival->z != z) {
      dx = x - rival->x;
      dz = z - rival->z;
      heading = ArcTan2(dz, dx);
      turn = (s16)(heading - rival->facing);
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      rival->facing += turn;
      rival->x = x;
      rival->z = z;
      state->still = 0;
    } else {
      state->still++;
    }
    if (state->still > 2)
      Func_020057c8(rival, 1);
    else
      Func_020057c8(rival, 5);
  } else if (state->mode == 2) {
    x = rival->x >> 16;
    z = rival->z >> 16;
    work->path[state->point] = x;
    work->path[state->point + 1] = z;
    if (!GameFlag_Test(260))
      state->point += 2;
    if (state->point == 7198) {
      // A u16 read back as s16 at its use: the same value, the spelling that
      // matches.
      u16 next = state->point + 1;

      work->path[state->point] = 0;
      work->path[(s16)next] = 0;
      state->id = work->rival;
      state->point = 0;
      state->mode = 1;
    }
  }
}

// ---- games/tla/src/scene/trial_road/s51a0.c ----
// Scene code of overlay 0x697, in ROM order, from s51a0's offset up to the
// next object the overlay links, and the overlay's data.

#include "heap_release.h"
#include "resource.h"
#include "vram_tab.h"

/* VIEW(s51a0): tag struct TrialRoadLinkWork as struct TrialRoadLinkWork__s51a0; s15a8.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/trial_road/s15a8.c:2529: structure has no member named `vramEntry'. */
struct TrialRoadLinkWork__s51a0 {
  u32 oam[54];
  s16 vramEntry;
  s16 fade;
  s16 scrollX;
  s16 scrollY;
  s16 unke0;
  s16 a;
  s16 b;
  u8 unke6[6];
  s32 originX;
  s32 originZ;
};

extern const u8 gData_020062d4[];

s32 Func_02002ce8(s32 side);

// The picture's buffer: 576 bytes of EWRAM, the size VramCache_Load copies.
static inline void *TrialRoadLink_AllocPicture(void) {
  return Heap_AllocEwram(576);
}

// left and right are the arms' offsets, each opening with its sign, so the
// subtractions compile in the order the native has them.
#define MARKER(left, right, which, tileAdd)                                    \
  do {                                                                         \
    s32 lift;                                                                  \
    /* FAKEMATCH(pin): originZ in r6; as a plain s32 *, 304 of 497             \
       instructions differ in tla-en. */                                       \
    register s32 *originZ asm("r6");                                           \
    /* FAKEMATCH(pin): x in r9; as a plain s32, 365 of 497 instructions        \
       differ in tla-en. */                                                    \
    register s32 x asm("r9");                                                  \
    /* FAKEMATCH(pin): arg in r0; as a plain s32, 44 of 497 instructions       \
       differ in tla-en. */                                                    \
    register s32 arg asm("r0");                                                \
    s32 side = gMainRoutineWork.entrance & 1;                                  \
    if (side) {                                                                \
      x = (actor->x - work->originX) / PIXELS(16) left;                        \
      arg = which;                                                             \
    } else {                                                                   \
      x = (actor->x - work->originX) / PIXELS(16) right;                       \
      arg = which;                                                             \
    }                                                                          \
    originZ = &work->originZ;                                                  \
    lift = Func_02002ce8(arg);                                                 \
    y = (actor->z - *originZ) / PIXELS(16) + work->scrollY - lift * 36 + 4;    \
    /* Once-only: ends the block, so y + 4 is scheduled before x & 511. */     \
    do {                                                                       \
    } while (0);                                                               \
    x &= 511;                                                                  \
    y &= 255;                                                                  \
    *cursor++ = 0;                                                             \
    y |= x << 16;                                                              \
    y |= markerSize;                                                           \
    *cursor++ = y;                                                             \
    *cursor++ = (tile + (tileAdd)) | 0xe400;                                   \
    Oam_AddSpriteAtDepth(entry++, 255);                                               \
  } while (0)

/// The Trial Road link display's scheduled callback, which Func_020055c4
/// starts. Its fade state falls during events or while flag 262 is set and
/// otherwise rises to 2 (unke0 holds it at 2); rising from 0 to 1 it loads
/// the palette and picture again, and at 0 it frees its VRAM. Otherwise it
/// queues two rows of sprites, each up to four pairs 36 pixels apart down from
/// the scrolled offset, sliding in 6 pixels per fade step (the right row
/// mirrored), and on frames 5 to 15 of every 16 a marker over each of the two
/// watched actors, projected at 1/16 scale from their positions and the
/// course side.
void Func_020051a0(void) {
  struct TrialRoadLinkWork__s51a0 *work = Ram_HeapSlots->unkec[0];
  u32 *cursor = (u32 *)work;
  struct TrialRoadSprite *entry = (struct TrialRoadSprite *)work;
  s16 *vramEntry = &work->vramEntry;
  u32 row, i;
  s32 tile = gResourceTableEntries[*vramEntry].offset >> 5;
  u32 flip, word;
  s32 y;
  // One constant for its whole life: the markers' OAM size bits (attribute 1
  // size 1, 16 by 16), held in a register across both markers.
  u32 markerSize = 0x40000000;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *actor;

  if (work->unke0 != 0) {
    work->fade = 2;
  } else if (GameFlag_Test(262) || event->unk166 != 0) {
    if (work->fade > 0)
      work->fade--;
  } else if (work->fade <= 1) {
    if (++work->fade == 1) {
      void *buffer;
      Dma_Set(Data_020062b4, (void *)0x050003c0, 0x80000010, REG_DMA3);
      buffer = TrialRoadLink_AllocPicture();
      Func_02005770(gData_020062d4, buffer);
      VramCache_Load(*vramEntry, 576, buffer);
      Heap_Release(buffer);
    }
  }
  if (work->fade == 0) {
    VramCache_ReleaseVram(work->vramEntry);
    return;
  }
  for (row = 0; row < 2; row++) {
    s32 x;
    y = (work->scrollY + 8) & 255;
    if (row == 0) {
      x = work->fade * 6 + work->scrollX - 12;
      x &= 511;
      flip = 0;
    } else {
      x = 236 - work->fade * 6 - work->scrollX;
      x &= 511;
      flip = 0x10000000;
    }
    for (i = 0; i < 4 && y < 160; i++) {
      *cursor++ = 0;
      *cursor++ = y | (x << 16) | flip | 0x80008000;
      *cursor++ = tile | 0xe400;
      Oam_AddSpriteAtDepth(entry++, 255);
      word = y + 32;
      *cursor++ = 0;
      *cursor++ = word | (x << 16) | flip | 0x4000;
      *cursor++ = (tile + 8) | 0xe400;
      Oam_AddSpriteAtDepth(entry++, 255);
      y += 36;
    }
  }
  if ((gFrameCount & 15) < 5)
    return;
  actor = ObjectTable_Get(work->a);
  if (actor != NULL) {
    MARKER(+work->fade * 6 + work->scrollX - 19,
           -work->fade * 6 - work->scrollX + 232, 0, 10);
  }
  actor = ObjectTable_Get(work->b);
  if (actor == NULL || actor->x == 0)
    return;
  {
    // XXX: this side's offset adds scrollX where the first marker's subtracts
    // it; the native does so, and a render of the course's far side would show
    // whether it is a slip.
    MARKER(+work->fade * 6 + work->scrollX + 189,
           -work->fade * 6 + work->scrollX + 23, 1, 14);
  }
}

void Func_020055c4(s32 a, s32 b) {
  struct TrialRoadLink *link = HeapSlot_AllocEwram(236, 0x3934);
  void *buffer = Heap_AllocEwram(576);
  s32 zero;
  s32 owner;
  u16 *unkda;
  u16 *unke0;
  s32 *height;
  s32 *unkf0;
  s16 *vramEntry;

  link->a = a;
  link->b = b;
  owner = gMainRoutineWork.currentOwner;
  height = &link->height;
  unkf0 = &link->unkf0;
  unkda = &link->unkda;
  unke0 = &link->unke0;
  vramEntry = &link->vramEntry;
  if (a == owner) {
    *height = 0xa00000;
    *unkf0 = 0xf9a00000;
  } else {
    *height = 0x700000;
    *unkf0 = 0xf9a00000;
  }
  Actor_GetById(a);
  Actor_GetById(b);
  zero = 0;
  *unkda = zero;
  *unke0 = zero;
  Func_02005770(gData_020062d4, buffer);
  *vramEntry = Resource_FindFreeEntry();
  VramCache_Load(*vramEntry, 576, buffer);
  Task_Add(Func_020051a0, 0x476);
  Heap_Release(buffer);
}

extern u8 gSceneState[];

void Func_02005688(s32 a) {
  u8 *work = Ram_HeapSlots->unkec[0];
  struct TrialRoadControl *control = (struct TrialRoadControl *)gSceneState;

  if (GameFlag_Test(0x109) == 0) {
    /* FAKEMATCH(temporary): a local reads the work value before the three
     * stores; as the value stored directly, 4 of 24 instructions differ in
     * tla-en. */
    s32 value = *(u16 *)(work + 228);

    control->field02 = a;
    control->field04 = value;
    control->field08 = 0;
  }
  Task_Add(Func_02004ddc, 0x485);
}

void Func_020056c4(s32 a) {
  struct TrialRoadControl *control = (struct TrialRoadControl *)gSceneState;
  /* FAKEMATCH(temporary): a word local builds 0 with movs for both halfword
   * stores; as a halfword constant, which loads from the pool, 14
   * of 15 instructions differ in tla-en. */
  s32 zero = 0;

  *(u16 *)((Ram_HeapSlots->unkec[0]) + 234) = zero;
  control->field0a = a;
  control->field06 = zero;
  control->state = 1;
  Func_02004ddc();
}

s32 Func_020056e8(void) {
  s32 state = *(s16 *)gSceneState;

  return IsZero(state ^ 9);
}

void Func_02005704(void) {}

void Func_02000058(void);
void Func_02000694(void);

s32 gData_02005a70[] = {
    46,
    (s32)Func_02000058,
    17,
};

s32 gData_02005a7c[] = {
    46,
    (s32)Func_02000694,
    17,
};

u32 gData_02005a88[] = {
    0x14151213,
};

// clang-format off
u32 gData_02005a8c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02005ac8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02005b04[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17, 17,
};

u32 gData_02005b44[] = {
    23, 3, 0x1000, 0, 1, 13,
    0xffff, 0,
};

void (*gData_02005b64[])(void) = {
    Func_02001904,
    Func_02001974,
    Func_02001a70,
    Func_02001afc,
    Func_02001bdc,
    Func_02001bec,
    Func_02001c6c,
    Func_02001d8c,
};

u32 gData_02005b84[] = {
    450, 451, 452, 453, 454, 455,
    456, 457, 458, 459,
};

u16 gData_02005bac[] = {
    1, 48, 15, 48, 29, 48, 1, 70,
    15, 70, 29, 70, 1, 92, 15, 92,
};

u8 gData_02005bcc[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

u8 gData_02005bd6[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

u32 gData_02005be0[] = {
    0x7030200, 0x6050401,
};

u32 gData_02005be8[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0x1380000, 22, 7, 0x2a80000,
    39, 5, 4, 0x1380000, 0, 0x2780000,
    1, 4, 0x1580000, 0, 0x2680000, 1,
    4, 0x1980000, 0, 0x2680000, 1, 4,
    0x1a80000, 0, 0x2580000, 1, 4, 0x1a80000,
    0, 0x1c80000, 1, 17,
};

u32 gData_02005c88[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0xe80000, 22, 7, 0x2a80000,
    39, 5, 4, 0xe80000, 0, 0x2780000,
    1, 4, 0xc80000, 0, 0x2680000, 1,
    4, 0x880000, 0, 0x2680000, 1, 4,
    0x780000, 0, 0x2580000, 1, 4, 0x780000,
    0, 0x1c80000, 1, 17,
};

u32 gData_02005d28[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0x780000, 22, 7, 0x1280000,
    39, 5, 4, 0x780000, 0, 0xd80000,
    1, 4, 0x880000, 0, 0xc80000, 1,
    4, 0xd80000, 0, 0xc80000, 1, 4,
    0xe80000, 0, 0xb80000, 1, 0, 2,
    22, 3, 0xc000, 17,
};

u32 gData_02005dc8[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    39, 5, 4, 0xd80000, 0, 0xa80000,
    1, 4, 0xe80000, 0, 0x980000, 1,
    4, 0xe80000, 0, 0x880000, 1, 22,
    5, 0, 22, 7, 0, 17,
};

u32 gData_02005e40[] = {
    22, 15, 0x10000, 22, 16, 0x8000,
    22, 5, 0x1580000, 22, 7, 0xb80000,
    39, 2, 4, 0x1380000, 0, 0xa80000,
    1, 39, 1, 0, 2, 22,
    3, 0x8000, 0, 2, 22, 3,
    0x6000, 0, 2, 22, 3, 0x4000,
    17,
};

u32 gData_02005ed4[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    39, 5, 4, 0x1780000, 0, 0xc80000,
    1, 4, 0x1d80000, 0, 0xc80000, 1,
    4, 0x1e80000, 0, 0xb80000, 1, 4,
    0x1e80000, 0, 0x880000, 1, 39, 1,
    0, 2, 22, 3, 0xa000, 0,
    2, 22, 3, 0x8000, 0, 2,
    22, 3, 0x6000, 0, 2, 22,
    3, 0x4000, 17,
};
// clang-format on
#undef MARKER
