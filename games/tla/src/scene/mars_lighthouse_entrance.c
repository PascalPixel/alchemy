#include "types.h"

void Func_02000338(void);

void Func_0200040c(void) { Func_02000338(); }

#include "mars_lighthouse_entrance/room.h"
#include "iwram_call.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

extern const u8 gData_02005394[];

void Func_02000414(s32 id) {
  struct Actor *source = Actor_Get(id);
  s32 i;

  i = 5;
next:
  {
    s32 x = source->x + (Random16Far() << 4) - (Random16Far() << 4);
    struct Actor *object =
        Func_02004bd8_(0x145, x, (Random16Far() << 3) + source->y + 0x40000,
                       source->z + (Random16Far() << 4) - (Random16Far() << 4));

    if (object != NULL) {
      s32 angle;
      s32 radius;
      s32 scale;

      angle = Random16Far();
      radius = Random16Far();
      scale = (Random16Far() >> 1) + 0x4000;
      Func_02004bd0(object, gData_02005394);
      Actor_SetSingleChildFlags(object, 0);
      /* FAKEMATCH(dead): an empty once-only block holds the order around it; as
       * no block, 2 of 113 instructions differ in tla-en. */
      do {
      } while (0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius * 8, Math_Cosine(angle));
      object->velocityX = Iwram_MulQ16(radius * 8, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = 0x8000;
      Animation_ApplyChildValues(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}

extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
extern const u8 gData_02005af4[];
extern const u8 gData_02005b0c[];
extern const u8 gData_02005b54[];
extern const u8 gData_02005bcc[];
extern const u8 gData_02005d4c[];
extern const u8 gData_02005eb4[];
extern const u8 gData_02005f74[];
extern const u8 gData_020060c4[];

void Func_02000798(void);
void Func_020019c4(void);
void Func_020046b4(void);
void Func_02004718(void);
void Func_02004dc8(void);
void Func_02004dd0(void);
void BattleEffect_PrepareBufferInterpolation(void);

void Func_0200051c(void) {
  s32 x;
  s32 count;

  Event_PlaySound(0x23c);
  x = 32;
  count = 6;
  // A goto loop, which GCC does not treat as a loop, so the stack arguments
  // are built inside it.
step:
  Func_02004c30_(x, 125, 3, 3, 39, 85);
  Func_02000414(10);
  count--;
  System_Wait(10);
  x += 3;
  if (count >= 0)
    goto step;
  Func_02004db0();
  FieldEvent_SetExitRequest(20);
}

#include "ram_buffer.h"

struct EventWork {
  u8 unk000[0x15a];
  u16 unk15a;
};

extern u8 MsgCheckedTheIce[];

void Func_02000564(void) {
  FieldMessage_Show((s32)MsgCheckedTheIce, 1);
  FieldMessage_Show((s32)MsgCheckedTheIce + 1, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->unk15a = 1;
}

#include "scene_tables.h"

void Func_02000590(void) {
  if (!Flag_IsSet(0xa36)) {
    Func_020046b4();
    Func_02004ca8();
    FieldEvent_ResolvePendingActions(0);
    Func_02004dd0();
    WORD_AT(Actor_Get(gMainRoutineWork.currentOwner), 108) = 0;
    Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
    Func_02004cd8_(gMainRoutineWork.currentOwner, PIXELS(688), PIXELS(360));
    FieldCamera_MoveTo(PIXELS(688), -1, PIXELS(360), 1);
    Func_02004d20();
    Func_02004dc8();
    FieldEvent_Wait(40);
    Func_02004718();
    BattleEffect_PrepareBufferInterpolation();
    System_Wait(10);
    Func_02004cb0();
    Flag_Set(0xa36);
  } else {
    Func_020046b4();
    Func_02004dd0();
    {
      u8 *party = (u8 *)&gMainRoutineWork;

      party[0x2c2] = 2;
    }
    FieldEvent_SetExitRequest(18);
    FieldEvent_Wait(40);
    Func_02004718();
  }
}

void Func_02000648(void) {
  Func_020046b4();
  Func_02004dd0();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(17);
  FieldEvent_Wait(40);
  Func_02004718();
}

void Func_02000678(s32 state, s32 frame) {
  if (state == 1) {
    Event_PlaySound(0x23e);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(60);
    Func_02004c30_(47, 94, 10, 6, 47, 100);
    Func_02004c28_(110, 40, 12, 4, 46, 40);
    Actor_SetPosition(18, 0, 0);
    Flag_Set(0xa3f);
  }
  if (frame > 96 && frame < 272 && frame % 30 == 0)
    Func_02000414(18);
  if (frame == 276) {
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(20);
  }
}

void Func_02001a00(void);

void Func_02000710(void) { Func_02001a00(); }

void Func_02000718(void) {
  SCENE_WORK->unk166 = 0;
  Flag_Clear(0x106);
  Flag_Clear(0x104);
  Func_02004ca8();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Camera_SetBounds(PIXELS(168), 0, PIXELS(664), 1);
  Func_02004d20();
  System_Wait(10);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  Camera_SetBounds(PIXELS(472), 0, PIXELS(664), 1);
  Func_02004d20();
  FieldEvent_Wait(90);
  Func_02004cb0();
}

void Func_02000798(void) {
  MapGrid_CopyCellAttributes(14, 55, 14, 3, 14, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 14, 104);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 14, 105);
}

void Func_02000808(void) {
  struct Actor *object = Actor_Get(20);

  if (!Flag_IsSet(0xa47)) {
    Event_PlaySound(0x23d);
    Flag_Set(0x231);
    object->x = S16_AT(object, 100) << 16;
    object->z = S16_AT(object, 102) << 16;
    Flag_Set(0xa48);
    Func_02000798();
  }
}

void Func_0200085c(s32 state, s32 frame) {
  if (state == 1) {
    Event_PlaySound(0x23e);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(60);
    MapGrid_CopyCellsLayered(28, 92, 8, 8, 28, 100);
    MapGrid_CopyCellAttributes(92, 38, 8, 8, 28, 38);
    Actor_SetPosition(20, 0, 0);
    Flag_Set(0xa47);
  }
  if (frame > 96 && frame < 272 && frame % 30 == 0)
    Func_02000414(20);
  if (frame == 276) {
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(20);
  }
}

void Func_020008f4(s32 unused, s32 id) {
  if (id == 8) {
    struct Actor *object = Actor_Get(8);

    if (object->x >> 20 == 20) {
      FieldEvent_Wait(5);
      object->z += PIXELS(1);
      BYTE_AT(object, 98) = 1;
      Flag_Set(0xa4f);
    }
  }
}

#include "event_inline.h"

void Func_02000930(void) { GameFlag_SetBit(0x144); }

void Func_0200093c(void) {
  Func_020046b4();
  Func_02004dd0();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(14);
  FieldEvent_Wait(40);
  Func_02004718();
}

/// Sends object id from column 11 to x 152, then from column 10 to x 200,
/// both at z 792, through Actor_SetMoveTarget.
void Func_0200096c(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 11) {
    s32 y;

    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
    y = object->y;
    Actor_SetMoveTarget(object, PIXELS(152), y, PIXELS(792));
  }
  if (object->x >> 20 == 10) {
    s32 y;

    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
    y = object->y;
    {
      /* FAKEMATCH(pin): the actor pinned to r0 to keep the argument order; as
       * an unpinned local, 2 of 38 instructions differ in tla-en. */
      register struct Actor *self asm("r0") = object;
      s32 x = 0xc80000;
      /* FAKEMATCH(pin): z pinned to r3 to keep the argument order; as an
       * unpinned local, 2 of 38 instructions differ in tla-en. */
      register s32 z asm("r3") = 0x3180000;

      Actor_SetMoveTarget(self, x, y, z);
    }
  }
}

void Func_020009c0(void) {
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(PIXELS(88), -1, PIXELS(696), 1);
  Func_020019c4();
}

void Func_020009e8(void) {
  SCENE_WORK->unk166 = 0;
  Flag_Clear(0x106);
  Flag_Clear(0x104);
  Func_02004ca8();
  FieldEvent_ResolvePendingActions(0);
  Func_02004d20();
  FieldEvent_Wait(120);
  Func_02004cb0();
}

void Func_02000a24(void) {
  struct Actor *object = Actor_Get(12);

  if (!Flag_IsSet(0xa4d)) {
    Event_PlaySound(0x23d);
    Flag_Set(0x232);
    object->x = S16_AT(object, 100) << 16;
    object->z = S16_AT(object, 102) << 16;
    Flag_Set(0xa4c);
  }
}

void Func_020019e0(void);

void Func_02000a74(void) { Func_020019e0(); }

void Func_02000a7c(void) {
  SCENE_WORK->unk166 = 0;
  Flag_Clear(0x106);
  Flag_Clear(0x104);
  Func_02004ca8();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Camera_SetBounds(PIXELS(872), -1, PIXELS(168), 1);
  Func_02004d20();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(264), 1);
  Func_02004d20();
  FieldEvent_Wait(120);
  Func_02004cb0();
}

void Func_02000af4(void) {
  Func_020046b4();
  Func_02004dd0();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(14);
  FieldEvent_Wait(40);
  Func_02004718();
}

void Func_02000b24(void) {
  MapGrid_CopyCells(53, 30, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 30, 4, 3, 117, 48);
}

void Func_02000b54(void) {
  MapGrid_CopyCells(53, 40, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 45, 4, 3, 117, 48);
}

void Func_02000b84(s32 state, s32 frame) {
  if (state == 1) {
    Event_PlaySound(0x23e);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(60);
    MapGrid_CopyCellsLayered(12, 95, 8, 6, 12, 101);
    MapGrid_CopyCellAttributes(76, 40, 8, 5, 12, 40);
    Actor_SetPosition(12, 0, 0);
    Flag_Set(0xa4d);
  }
  if (frame > 96 && frame < 272 && frame % 30 == 0)
    Func_02000414(12);
  if (frame == 276) {
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(20);
  }
}

void Func_02000c1c(void) {
  MapGrid_CopyCellAttributes(35, 11, 3, 6, 53, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 117, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 53, 75);
  Flag_Set(0xa49);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void MapCopy(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
  Map_CopyCellAttributeRect(a0, a1, a2, a3, a4, a5);
}

void Func_02000c68(void) { MapCopy(25, 9, 11, 3, 26, 9); }

void Func_02000c84(void) {
  Func_020046b4();
  Func_02004dd0();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(12);
  FieldEvent_Wait(40);
  Func_02004718();
}

void Func_02000cb4(void) {
  Func_020046b4();
  Func_02004dd0();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(13);
  FieldEvent_Wait(40);
  Func_02004718();
}

void Func_02000ce4(s32 unused, s32 id) {
  if (id == 8) {
    struct Actor *object = Actor_Get(8);

    if (object->x >> 20 == 5) {
      FieldEvent_Wait(5);
      object->z += PIXELS(1);
      Flag_Set(0xa4e);
    }
  }
}

#include "map.h"
#include "main_routine_work.h"
#include "actor.h"
#include "vram_tab.h"

extern u16 gData_020068a2;
extern u32 Data_020068a4[3];

void Func_02004b80(u32 *oam, s32 depth);

void Func_02000d18(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *camera = (s32 *)((u8 *)map + 228);
  s32 x = camera[0] & 0xffff0000;
  s32 cameraZ = camera[1] & 0xffff0000;
  s32 originY = map->origin[1];
  s32 tile = gResourceTableEntries[gData_020068a2].offset >> 5;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 y, z, depth, sx, sy;
  u32 *word;

  x = ((leader->x >> 20) << 20) - x;
  y = Map_GetTerrainHeight(0, leader->x, leader->z) - originY;
  z = ((leader->z >> 20) << 20) - cameraZ - originY;
  sy = z - y;
  sx = x >> 16;
  sy = (sy >> 16) + 16;
  depth = ((y + z) >> 16) + 58;
  if ((u32)(sx + 15) > 334 || sy < -15 || sy > 239)
    return;
  sx &= 0x1ff;
  sy &= 0xff;
  word = Data_020068a4;
  *word++ = 0;
  *word++ = sy | (sx << 16) | 0x80008800;
  *word = tile | 0x400;
  Func_02004b80(Data_020068a4, depth);
}

#include "task.h"

void *Heap_Alloc(s32 size);
void Heap_Release(void *buffer);
s32 Resource_FindFreeEntry(void);

/// Fills 0x100 bytes with colour 1, loads them into a VRAM block of their own
/// and starts Func_02000d18.
void Func_02000df0(void) {
  u32 *buffer = Heap_Alloc(0x100);
  u32 *word = buffer;
  s32 count = 63;

  // A goto loop: as a for loop loop.c moves the fill word out of it.
fill:
  *word++ = 0x11111111;
  if (--count >= 0)
    goto fill;
  gData_020068a2 = Resource_FindFreeEntry();
  Vram_Load(gData_020068a2, 0x100, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000d18, TASK_PASS_VBLANK);
}

#include "system/io_reg.h"
#include "field.h"

// A once-only block with a local that holds one constant, as the native
// builds the value.
#define SET_SPRITE_WINDOW_LAYERS()                                             \
  do {                                                                         \
    u16 winout = 0xf1f;                                                        \
                                                                               \
    REG_WINOUT = winout;                                                       \
  } while (0)

struct Overlay6aaEventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
extern struct MapCell gMapCellBuffer[];

/// The leader shrinks and sinks over 40 frames, then the screen transition
/// ends.
void Func_02000e3c(s32 arg) {
  struct Actor *actor;
  struct MapWork *map;
  struct MapCell *cells;
  struct MapCell *buffer;
  s32 x, z;
  s32 i;

  actor = Actor_Get(gMainRoutineWork.currentOwner);
  Func_02004ca8();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  actor->update = NULL;
  {
    s32 value = 0x100;

    FieldActor_ShowEmote(gMainRoutineWork.currentOwner, value, 0);
  }
  Func_02004bc0(actor, 27);
  Actor_SetAnimationSpeed(actor, 48);
  System_Wait(30);
  actor->facing = FACING_SOUTH;
  actor->motionFlags = 0;
  buffer = gMapCellBuffer;
  x = actor->x >> 20;
  z = actor->z >> 20;
  map = Ram_HeapSlots->mapWork;
  if (map != NULL)
    cells = map->layers[actor->terrainId & 3].cells;
  else
    cells = buffer;
  cells += x + (z << 7) + 128;
  if (!(cells->attributeB & 0x40)) {
    SET_SPRITE_WINDOW_LAYERS();
    REG_DISPCNT |= DISPCNT_OBJWIN_ON;
    Func_02000df0();
  }
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  i = 0;
next:
  if (i == 5)
    Event_PlaySound(204);
  actor->scaleX -= 0x400;
  actor->scaleY -= 0x300;
  actor->y -= 0x9999;
  System_Wait(1);
  i++;
  if (i <= 39)
    goto next;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct Overlay6aaEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(arg);
}

extern char SceneId_Ov6a8Room2;
extern char SceneId_Ov6ACRoom3;
extern const u8 gData_0200503c[];
extern const u8 gData_0200547c[];
extern const u8 gData_020054cc[];
extern const u8 gData_02005528[];
extern const u8 gData_02005578[];
extern const u8 gData_020055d4[];
extern const u8 gData_02005644[];
extern const u8 gData_0200610c[];
extern const u8 gData_02006118[];
extern const u8 gData_0200613c[];
extern const u8 gData_02006250[];
extern const u8 gData_020063b8[];
extern const u8 gData_020064fc[];
extern const u8 gData_02006628[];
extern const u8 gData_02006778[];
extern s16 gData_02006868;
extern s16 gData_020068a0;

void Func_02000fe0(const struct MapCell *cell);
void Func_020012e4(struct Actor *object);
void Func_02002f4c(s32 id, s32 a1, s32 a2, s32 a3, const void *script);
void Func_02002c2c(void);
void Func_02004c88(s32 scene, s32 a1);
void Func_02004d98(void);

void Func_02000fa4(void) {
  Event_PlaySound(0x248);
  Flag_Set(0x17d);
  Actor_SetScript(11, gData_0200503c);
  Func_0200246c(0, 3);
  Flag_Set(0x234);
}

void Func_02003564(void);
s32 Func_020029a0(void);

void Func_02000fd4(void) {
  Func_02003564();
  Func_020029a0();
}

void GameFlag_SetByte(s32 flag, s32 value);

void Func_02000fe0(const struct MapCell *cell) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 z = leader->z;
  // The leader's map column and, later, the frame count share one variable,
  // as the native's register shows.
  s32 i = leader->x >> 20;

  GameFlag_SetByte(0x340, i);
  GameFlag_SetByte(0x348, z >> 20);
  Func_02004ca8();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  leader->update = 0;
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Func_02004bc0(leader, 27);
  Actor_SetAnimationSpeed(leader, 48);
  System_Wait(30);
  leader->facing = FACING_SOUTH;
  leader->motionFlags = 0;
  if ((cell->attributeB & 64) == 0) {
    {
      /* FAKEMATCH(temporary): the window mask is built in a word register, as
       * the native builds it; as REG_WINOUT = 0xf1f, 18 of 130 instructions
       * differ in tla-en. */
      u16 winout = 0xf1f;

      REG_WINOUT = winout;
    }
    REG_DISPCNT |= 0x8000;
    Func_02000df0();
  }
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  i = 0;
next:
  if (i == 5)
    Event_PlaySound(204);
  leader->scaleX -= 0x400;
  leader->scaleY -= 0x300;
  leader->y -= 0x9999;
  System_Wait(1);
  i++;
  if (i <= 39)
    goto next;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  SCENE_WORK->unk1ac = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(11);
}

void Func_02003884(void);

void Func_0200112c(void) {
  Func_02004d40();
  Func_02003884();
}

void Func_02001138(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s32 x;
  s32 z;
  struct MapCell *cell;

  object = Actor_Get(party->currentOwner);
  x = object->x >> 20;
  z = object->z >> 20;
  cell = gMapCellBuffer;
  if (map != NULL)
    cell = map->layers[object->terrainId & 3].cells;
  cell += x + (z << 7);
  if (cell->attributeB & 64)
    Func_02000fe0(cell + 128);
}

const u8 *Func_0200119c(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom1)
    return gData_02006118;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom2)
    return gData_0200613c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom3)
    return gData_02006250;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom4)
    return gData_020063b8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom5)
    return gData_020064fc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom6)
    return gData_02006628;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom7)
    return gData_02006778;
  return gData_0200610c;
}

#include "mars_lighthouse_entrance/ride.h"

void Func_02001234(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    Actor_SetSingleChildFlags(object, 0);
    object->collisionFlags = 0;
    object->motionFlags = 0;
  }
}
