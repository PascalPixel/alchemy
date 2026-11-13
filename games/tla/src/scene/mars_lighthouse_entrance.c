// ---- games/tla/src/scene/mars_lighthouse_entrance/s01f8.c ----
#include "shared/routines.h"
#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "actor.h"
#include "battle.h"
#include "event_work.h"
#include "field.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "scene_ids.h"
#include "text/msg_ids.h"

s32 Func_0200012c();
s32 Map_GetTerrainHeight();
void Scene_SetActorSpritePriority();

void Func_020001f8(struct Actor *object) {
  s32 t1, t2, t3, t4, v10, v6, v7, v8, v9;
  struct Actor *v5;
  u8 *v11;

  v5 = object;
  v9 = (v5->x >> 20);
  v10 = (v5->z >> 20);
  t1 = Func_0200012c(0, v9, v10);
  v8 = t1;
  t2 = Func_0200012c(2, v9, v10);
  v6 = t2;
  v11 = &v5->terrainId;
  t3 = Map_GetTerrainHeight(v5->terrainId, v5->x, v5->z);
  v7 = (t3 >> 19);
  t4 = Event_GetFlag(v5->timer);
  if (t4 == 0)
    goto L_02008268;
  v6 = (v6 & -0xff01);
  v8 = (v8 & -129);
  if ((v5->terrainHeight >> 19) == v7)
    goto L_02008282;
  v7 = (v7 + -4);
  goto L_02008282;
L_02008268:
  v6 = (v6 & 255);
  v6 = (v6 | 0xff00);
  v8 = (v8 | 128);
  v7 = (v7 + 4);
  Scene_SetActorSpritePriority(v5, 2);
L_02008282:
  Func_02000168(0, v9, v10, v8);
  Func_02000168(2, v9, v10, v6);
  Func_02004c80((v9 << 20), (v10 << 20), *v11, v7);
}

#include "field_event.h"
#include "field.h"

void Func_02004bc0();

void Func_020002bc(u16 *table) {
  u16 *row;
  struct FieldActor *object;
  s16 flag;
  s16 animation;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  object = (struct FieldActor *)Actor_Get(row[0]);
  animation = row[1];
  flag = row[2];

  Actor_SetSingleChildFlags(object, 0);
  Func_02004bc0(object, (u16)animation + Event_GetFlag((u16)flag));
  object->motionFlags = 0;
  object->collisionFlags &= 0xfe;
  object->timer = flag;
  Func_020001f8(object);
  row += 3;
  if (row[0] != 0xffff)
    goto loop;
}

#include "mars_lighthouse_entrance/room.h"
#include "field_effect.h"

void Func_02003b30(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

/// While index is at most 7, spawns effect type 0x117 index steps below the
/// object the effect work names and shrinks the object a little; mode 1
/// restores its size, sets its flag, hands it to Func_020001f8 and clears its
/// position.
void Func_02000338(s32 mode, s32 index) {
  struct EffectHolder *holder = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_Get(holder->objectId);

  if (index <= 7) {
    struct EffectOptions options;

    options.priority = 0;
    options.type = 0x117;
    options.startScaleX = options.startScaleY = 0x10000;
    options.targetScaleX = options.targetScaleY = 0x8000;
    options.rotation = (Random16Far() & 15) << 12;
    Func_02003b30(object->x, object->y - index * 3 * 0x10000 + 0x100000,
                  object->z, ((Random16Far() & 15) - 8) << 13, 0, 0, 0x5a0000,
                  &options);
    object->scaleY -= 0x1f00;
  }
  if (mode == 1) {
    object->scaleY = 0x10000;
    Event_SetFlag(object->timer);
    Func_020001f8(Actor_Get(holder->objectId));
    object->x = object->y = object->z = 0;
  }
}

// ---- games/tla/src/scene/mars_lighthouse_entrance.c ----
/* VIEW(mars_lighthouse_entrance): routine Func_02000338 as void Func_02000338(void); the definition has void Func_02000338(s32 mode, s32 index); tried as the definition's void Func_02000338(struct Actor *object), it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:123: conflicting types for `Func_02000338'. */
extern void Func_02000338__mars_lighthouse_entrance(void) __asm__("Func_02000338");

void Func_0200040c(void) { Func_02000338__mars_lighthouse_entrance(); }

#include "iwram_call.h"

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
      Actor_StartScript(object, gData_02005394);
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

extern const u8 gData_02005af4[];
extern const u8 gData_02005b0c[];
extern const u8 gData_02005b54[];
extern const u8 gData_02005bcc[];
extern const u8 gData_02005d4c[];
extern const u8 gData_02005eb4[];
extern const u8 gData_02005f74[];
extern const u8 gData_020060c4[];

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
  ObjectTable_Snapshot();
  FieldEvent_SetExitRequest(20);
}

#include "ram_buffer.h"

void Func_02000564(void) {
  FieldMessage_Show((s32)MsgCheckedTheIce, 1);
  FieldMessage_Show((s32)MsgCheckedTheIce + 1, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->unk15a = 1;
}

#include "scene_tables.h"

void Func_02000590(void) {
  if (!Flag_IsSet(0xa36)) {
    Func_020046b4();
    Event_Start();
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
    Event_End();
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

DEFINE_FUNC_020001A0(Func_02000648, Func_020046b4, Func_02004dd0, 17,
    Func_02004718)

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

void Func_02000710(void) { Func_02001a00(); }

void Func_02000718(void) {
  SCENE_WORK->unk166 = 0;
  Flag_Clear(0x106);
  Flag_Clear(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Camera_SetBounds(PIXELS(168), 0, PIXELS(664), 1);
  Func_02004d20();
  System_Wait(10);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  Camera_SetBounds(PIXELS(472), 0, PIXELS(664), 1);
  Func_02004d20();
  FieldEvent_Wait(90);
  Event_End();
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

DEFINE_FUNC_0200085C(Func_0200085c, 28, 92, 8, 100, 92, 38, 8, 20, 0xa47)

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

void Func_02000930(void) { GameFlag_SetBit(0x144); }

DEFINE_FUNC_020001A0(Func_0200093c, Func_020046b4, Func_02004dd0, 14,
    Func_02004718)

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
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Func_02004d20();
  FieldEvent_Wait(120);
  Event_End();
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

void Func_02000a74(void) { Func_020019e0(); }

void Func_02000a7c(void) {
  SCENE_WORK->unk166 = 0;
  Flag_Clear(0x106);
  Flag_Clear(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Camera_SetBounds(PIXELS(872), -1, PIXELS(168), 1);
  Func_02004d20();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(264), 1);
  Func_02004d20();
  FieldEvent_Wait(120);
  Event_End();
}

DEFINE_FUNC_020001A0(Func_02000af4, Func_020046b4, Func_02004dd0, 14,
    Func_02004718)

void Func_02000b24(void) {
  MapGrid_CopyCells(53, 30, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 30, 4, 3, 117, 48);
}

void Func_02000b54(void) {
  MapGrid_CopyCells(53, 40, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 45, 4, 3, 117, 48);
}

DEFINE_FUNC_0200085C(Func_02000b84, 12, 95, 6, 101, 76, 40, 5, 12, 0xa4d)

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

DEFINE_FUNC_020001A0(Func_02000c84, Func_020046b4, Func_02004dd0, 12,
    Func_02004718)

DEFINE_FUNC_020001A0(Func_02000cb4, Func_020046b4, Func_02004dd0, 13,
    Func_02004718)

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
#include "vram_tab.h"

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

// A once-only block with a local that holds one constant, as the native
// builds the value.
#define SET_SPRITE_WINDOW_LAYERS()                                             \
  do {                                                                         \
    u16 winout = 0xf1f;                                                        \
                                                                               \
    REG_WINOUT = winout;                                                       \
  } while (0)

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
  Event_Start();
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

void Func_02002c2c(void);

void Func_02000fa4(void) {
  Event_PlaySound(0x248);
  Flag_Set(0x17d);
  Actor_SetScript(11, gData_0200503c);
  Func_0200246c(0, 3);
  Flag_Set(0x234);
}

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
  Event_Start();
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

const u8 *Scene_GetEvents(void) {
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

DEFINE_FUNC_020002D4(Func_02001234)
#undef SET_SPRITE_WINDOW_LAYERS

extern s16 gData_02006868;

void Func_02001254(void) {
  s16 *state = &gData_02006868;

  if (gData_020068a0 < 60) {
    if (*state == 0) {
      Func_0200246c(0, 3);
      *state = 1;
    }
  } else {
    // Read through a struct, so PRE does not merge it with the first arm's
    // read.
    if (((struct HalfwordField *)state)->value == 1) {
      Func_0200246c(0, 67);
      *state = 0;
    }
  }
  if (++gData_020068a0 > 300)
    gData_020068a0 = 0;
}

DEFINE_FUNC_020012BC(Func_020012bc, 0, 6, 10)

void Func_020012e4(struct Actor *actor) {
  struct Sprite020012e4 *sprite = actor->sprite;

  sprite->unk12 += 0x800;
  if (++actor->timer == 8)
    actor->velocityY = 0x40000;
  if (actor->timer > 9) {
    actor->x += 0x40000;
    actor->scaleX -= 0x800;
    actor->scaleY -= 0x800;
  }
  if (actor->timer == 32) {
    actor->x = 0;
    actor->z = 0;
    actor->update = NULL;
  }
}

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

void Func_02002c2c(void);

void Func_02001344(struct Actor *object) {
  if (ABS(object->x - object->watchedObject->x) > 0x200000)
    return;
  object->timer = 0;
  object->update = (void (*)(void))Func_020012e4;
}

void Func_02001378(void) {
  if (Func_020029bc())
    Func_02003878(1);
}

// ---- games/tla/src/scene/mars_lighthouse_entrance/s138c.c ----
#include "version.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02002270();
s32 Func_020022d8();
s32 Func_020022e4();
s32 Func_0200246c();
s32 Func_0200248c();
s32 Func_020045f8();
/* VIEW(s138c): routine Func_0200196c as extern u8 Func_0200196c[]; the definition has void Func_0200196c(void); tried as the definition's void Func_0200196c(s32 x, s32 z, s32 effect), which the object already declares, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1118: `Func_0200196c' undeclared (first use in this function). */
extern u8 Func_0200196c__s138c[] __asm__("Func_0200196c");
extern u8 gData_02005314[];
extern u8 gData_020053d8[];
extern u8 gData_020053e6[];
extern u8 gData_020053f8[];
extern u8 gData_020053fe[];
extern u8 gData_0200540c[];
extern u8 gData_02005412[];
extern u8 gData_02005418[];
extern u8 gData_02005438[];
extern u8 gData_0200543e[];
extern u8 gData_02005448[];
extern u8 gData_0200544e[];
extern u8 gData_02005454[];
extern u8 gData_0200586c[];
extern u8 gData_0200587c[];
extern u8 gData_020058a8[];
extern u8 gData_020058d4[];
/* VIEW(s138c): variable gMainRoutineWork as extern u8 gMainRoutineWork[]; main_routine_work.h has extern struct MainRoutineWork gMainRoutineWork; tried as the 4 declarations' extern u8 gMainRoutineWork[], it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:919: conflicting types for `gMainRoutineWork'. */
extern u8 gMainRoutineWork__s138c[] __asm__("gMainRoutineWork");

s32 Scene_Initialize(void) {
  s32 v_r8, v_r7, v_r5, t0, t1, t2, t8, t12, t17, t21;
  s32 t23, t24, t26, t27, t29, t34, t38, t40, t41, t48;
  s32 t51, t52, t55, t56, t57, t72, t73, t74, t78, t81;
  s32 t93, t96, t97, t98, t99, t100, t101, t102, t103, t104;
  s32 t105, t106, t107, t108, t109, t110, t111;

  t0 = FIELD_AT((s32)gMainRoutineWork__s138c, s16, 480);
  t1 = FIELD_AT((s32)gMainRoutineWork__s138c, s16, 482);
  *(s32 *)(s32)Data_02006878 = 0;
  t2 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t2, s32, 428) = 0x204;
  Event_SetFlag(0x112);
  Func_02002270();
  v_r7 = t1;
  v_r8 = t0;
  if (v_r8 != (s32)&SceneId_Ov6aaRoom1)
    goto L_020093dc;
  Call1((void (*)())Func_02004dd8, 1);
  goto L_020098be;
L_020093dc:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom2)
    goto L_0200944c;
  Call1((void (*)())Func_02004d88, 0);
#if !TLA_PATCH_1
  FieldActor_SetSpritePriority(11, 3);
#endif
  Call1((void (*)())Func_02001234, 10);
  t8 = Event_GetFlag(0xa30);
  if (t8 == 0)
    goto L_02009420;
  MapGrid_CopyCellsLayered(35, 41, 24, 10, 3, 53);
  MapGrid_CopyCellsLayered(35, 105, 24, 10, 3, 117);
L_02009420:
  Call1((void (*)())Func_020002bc, (s32)gData_020053d8);
  t12 = Event_GetFlag(0xa3e);
  if (t12 != 0)
    goto L_02009436;
  goto L_020098be;
L_02009436:
  MapGrid_CopyCellsLayered(50, 125, 3, 3, 39, 85);
  goto L_020098be;
L_0200944c:
  if (v_r8 == (s32)&SceneId_Ov6aaRoom3)
    goto L_02009454;
  goto L_02009680;
L_02009454:
  Call1((void (*)())Func_02004d88, 0);
  Call4((void (*)())Func_020022e4, (s32)gData_020053e6, (s32)gData_020053f8, 0,
        88);
  Call1((void (*)())Func_020002bc, (s32)gData_020053fe);
  t17 = Event_GetFlag(0xa3f);
  if (t17 == 0)
    goto L_020094ae;
  MapGrid_CopyCellsLayered(47, 94, 10, 6, 47, 100);
  MapGrid_CopyCellAttributes(110, 40, 12, 4, 46, 40);
  Actor_SetPosition(18, 0, 0);
  goto L_020094e0;
L_020094ae:
  t21 = Event_GetFlag(0xa3e);
  if (t21 == 0)
    goto L_020094d6;
  Event_SetFlag(0x230);
  t23 = (s32)Actor_Get(18);
  FIELD_AT(t23, u8, 35) |= 2;
  goto L_020094e0;
L_020094d6:
  Actor_SetPosition(18, 0, 0);
L_020094e0:
  t26 = (s32)Actor_Get(20);
  FIELD_AT(t26, u8, 35) |= 2;
  FieldActor_SetSpritePriority(20, 2);
  t29 = Event_GetFlag(0xa47);
  if (t29 == 0)
    goto L_0200953c;
  MapGrid_CopyCellsLayered(28, 92, 8, 8, 28, 100);
  MapGrid_CopyCellAttributes(92, 38, 8, 8, 28, 38);
  Func_02000798();
  Actor_SetPosition(20, 0, 0);
  goto L_02009564;
L_0200953c:
  t34 = Event_GetFlag(0xa48);
  if (t34 == 0)
    goto L_0200955a;
  Func_02000798();
  Event_SetFlag(0x231);
  goto L_02009564;
L_0200955a:
  Actor_SetPosition(20, 0, 0);
L_02009564:
  t38 = Event_GetFlag(0xa4f);
  if (t38 == 0)
    goto L_0200958c;
  Actor_SetPosition(8, 0x1480000, 0x780000);
  t40 = (s32)Actor_Get(8);
  FIELD_AT(t40, u8, 98) = 1;
L_0200958c:
  t41 = Event_GetFlag(0xa30);
  if (t41 == 0)
    goto L_020095a4;
  Actor_SetPosition(19, 0, 0);
  goto L_020095ae;
L_020095a4:
  Actor_SetPosition(15, 0, 0);
L_020095ae:
  if (v_r7 == 20)
    goto L_020095b4;
  goto L_020098be;
L_020095b4:
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  MapGrid_SetShake(0, 0x20000, 0x10000);
  t48 = FIELD_AT((s32)gMainRoutineWork__s138c, s32, 532);
  Actor_SetPosition(t48, 0, 0);
  Actor_SetPosition(18, 0, 0);
  t51 = (s32)Actor_Get(18);
  FIELD_AT(t51, u8, 35) |= 2;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  t55 = (s32)Actor_Get(18);
  t56 = FIELD_AT(t55, s16, 100);
  t57 = FIELD_AT(t55, s16, 102);
  Actor_SetPosition(18, (t56 << 16), (t57 << 16));
  Event_PlaySound(0x23d);
  v_r5 = 2;
L_0200963c:
  Call1((void (*)())Func_02000414, 18);
  System_Wait(1);
  v_r5 = (v_r5 - 1);
  if (v_r5 >= 0)
    goto L_0200963c;
  Event_SetFlag(0xa3e);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldEvent_Wait(120);
  FieldEvent_SetExitRequest(20);
  Event_SetFlag(0x109);
  goto L_020098be;
L_02009680:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom4)
    goto L_020096b0;
  Call1((void (*)())Func_02004d88, 0);
  if (v_r7 == 10)
    goto L_02009694;
  if (v_r7 != 1)
    goto L_020096a8;
L_02009694:
  Call4((void (*)())Func_020022e4, (s32)gData_0200540c, (s32)gData_02005412,
        (s32)Func_020019a4, 88);
  Call2((void (*)())Func_0200248c, 0, 1);
L_020096a8:
  Call1((void (*)())Func_020002bc, (s32)gData_02005418);
  goto L_020098be;
L_020096b0:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom5)
    goto L_02009784;
  Call1((void (*)())Func_02004d88, 0);
  t72 = (s32)Actor_Get(12);
  FIELD_AT(t72, u8, 35) |= 2;
  t74 = Event_GetFlag(0xa4d);
  if (t74 == 0)
    goto L_0200970c;
  MapGrid_CopyCellsLayered(12, 95, 8, 6, 12, 101);
  MapGrid_CopyCellAttributes(76, 40, 8, 5, 12, 40);
  Actor_SetPosition(12, 0, 0);
  goto L_0200976e;
L_0200970c:
  t78 = Event_GetFlag(0xa4c);
  if (t78 == 0)
    goto L_02009764;
  Event_SetFlag(0x232);
  goto L_0200976e;
L_02009764:
  Actor_SetPosition(12, 0, 0);
L_0200976e:
  t81 = Event_GetFlag(0xa49);
  if (t81 != 0)
    goto L_0200977e;
  goto L_020098be;
L_0200977e:
  Func_02000c1c();
  goto L_020098be;
L_02009784:
  if (v_r8 == (s32)&SceneId_Ov6aaRoom6)
    goto L_0200978c;
  goto L_020098b2;
L_0200978c:
  Call1((void (*)())Func_02004d88, 0);
  if (v_r7 == 2)
    goto L_020097a2;
  if (v_r7 == 4)
    goto L_020097a2;
  if (v_r7 == 6)
    goto L_020097a2;
  if (v_r7 != 13)
    goto L_020097b6;
L_020097a2:
  Call4((void (*)())Func_020022e4, (s32)gData_02005438, (s32)gData_0200543e,
        (s32)Func_0200196c__s138c, 88);
  Call2((void (*)())Func_0200248c, 0, 1);
L_020097b6:
  if ((u32)(v_r7 - 7) <= (u32)1)
    goto L_020097c0;
  if (v_r7 != 11)
    goto L_02009884;
L_020097c0:
  Call4((void (*)())Func_020022e4, (s32)gData_02005448, (s32)gData_0200544e,
        (s32)Func_02001988, 88);
  Call2((void (*)())Func_0200248c, 0, 1);
  if (v_r7 != 11)
    goto L_020097fa;
  Actor_SetScript(11, gData_02005314);
  Func_0200246c(0, 3);
  Task_AddFrame(Func_02001254, 0x480);
  Event_SetFlag(0x17d);
L_020097fa:
  Task_AddFrame(Func_02001378, 0x480);
  t93 = (s32)Actor_Get(11);
  FIELD_AT(t93, s32, 108) = (s32)Func_020012bc;
  Func_020022d8(1);
  Call1((void (*)())Func_020002bc, (s32)gData_02005454);
  t96 = (s32)Actor_Get(20);
  t97 = (s32)Actor_Get(13);
  FIELD_AT(t97, s32, 108) = (s32)Func_02001344;
  t98 = (s32)Actor_Get(14);
  FIELD_AT(t98, s32, 108) = (s32)Func_02001344;
  t99 = (s32)Actor_Get(15);
  FIELD_AT(t99, s32, 108) = (s32)Func_02001344;
  t100 = (s32)Actor_Get(16);
  FIELD_AT(t100, s32, 108) = (s32)Func_02001344;
  t101 = (s32)Actor_Get(17);
  FIELD_AT(t101, s32, 108) = (s32)Func_02001344;
  t102 = (s32)Actor_Get(18);
  FIELD_AT(t102, s32, 108) = (s32)Func_02001344;
  t103 = (s32)Actor_Get(13);
  FIELD_AT(t103, s32, 104) = t96;
  t104 = (s32)Actor_Get(14);
  FIELD_AT(t104, s32, 104) = t96;
  t105 = (s32)Actor_Get(15);
  FIELD_AT(t105, s32, 104) = t96;
  t106 = (s32)Actor_Get(16);
  FIELD_AT(t106, s32, 104) = t96;
  t107 = (s32)Actor_Get(17);
  FIELD_AT(t107, s32, 104) = t96;
  t108 = (s32)Actor_Get(18);
  FIELD_AT(t108, s32, 104) = t96;
L_02009884:
  t109 = (s32)Actor_Get(9);
  FIELD_AT(t109, u8, 35) |= 2;
  t111 = Event_GetFlag(0xa4e);
  if (t111 == 0)
    goto L_020098be;
  Actor_SetPosition(8, 0x580000, 0x2b80000);
  goto L_020098be;
L_020098b2:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom7)
    goto L_020098be;
  Call1((void (*)())Func_02004d88, 0);
L_020098be:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom6)
    goto L_020098d0;
  if (v_r7 != 13)
    goto L_020098d0;
  Event_ClearFlag(0x104);
L_020098d0:
  Call4((void (*)())Func_020045f8, (s32)gData_0200586c, (s32)gData_0200587c,
        (s32)gData_020058a8, (s32)gData_020058d4);
  return 0;
}
#undef FIELD_AT

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

/* VIEW(s1930): routine Func_02002f4c as s32 Func_02002f4c(s32 id, s32 a1, s32 a2, s32 a3, const void *script); s1344 has void Func_02002f4c(s32 id, s32 a1, s32 a2, s32 a3, const void *script); tried as the definition's void Func_02002f4c(s32 id, s32 target, s32 kind, s32 a3, const void *script), Func_0200196c moves: 1 of 11 instructions, Func_02001988 moves: 1 of 11 instructions, Func_020019a4 moves: 1 of 12 instructions, 3 more in tla-ja. */
extern s32 Func_02002f4c__s1930(s32 id, s32 a1, s32 a2, s32 a3, const void *script) __asm__("Func_02002f4c");
void Func_02002c2c(void);

s32 Scene_PrepareMap(void) {
  if (Flag_IsSet(0xa30))
    Func_02004c88(gMainRoutineWork.map +
                      ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                  0x5000000);
  return 0;
}

void Func_0200196c(void) { Func_02002f4c__s1930(10, 19, 20, 0, gData_0200547c); }

void Func_02001988(void) { Func_02002f4c__s1930(12, 19, 20, 0, gData_020054cc); }

void Func_020019a4(void) { Func_02002f4c__s1930(9, 18, 20, 0x4000, gData_02005528); }

void Func_020019c4(void) { Func_02002f4c__s1930(9, 13, 14, 0, gData_02005578); }

void Func_020019e0(void) { Func_02002f4c__s1930(11, 13, 14, 0x4000, gData_020055d4); }

void Func_02001a00(void) { Func_02002f4c__s1930(13, 21, 22, 0, gData_02005644); }

extern const u8 gData_020056a0[];

void Func_02001f90(struct Actor *object);

DEFINE_FUNC_02001A1C(Func_02001a1c)

DEFINE_FUNC_02001A80_117(Func_02001a80)

#include "field/event_work.h"

/* VIEW(s1930): tag struct RideSlot as struct RideSlot__s1930; ride.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1319: structure has no member named `id'. */
struct RideSlot__s1930 {
  u16 id;
  u16 blocked;
};

/* VIEW(s1930): tag struct RideWork as struct RideWork__s1930; ride.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1287: structure has no member named `value68'. */
struct RideWork__s1930 {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot__s1930 slots[4];
  u8 unk64[4];
  u16 value68;
  u16 direction;
};

DEFINE_FUNC_020015EC(Func_02001ae0, RideWork__s1930, Func_02001a1c,
    Func_02001a80)

extern const u8 gData_020056a0[];

void Func_02001f90(struct Actor *object);

DEFINE_FUNC_02001C44(Func_02001c44, gData_020056a0)

DEFINE_FUNC_02001C74(Func_02001c74, Func_02004bd8_, Func_02001c44, 2,
    Func_02001ae0, Func_02004bc0)

/* VIEW(s1c44): tag struct RideSlot as struct RideSlot__s1c44; ride.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1428: structure has no member named `id'. */
struct RideSlot__s1c44 {
  u16 id;
  u16 blocked;
};

struct RideDirection {
  u16 unk00;
  u16 unk02;
};

/* VIEW(s1c44): tag struct RideWork as struct RideWork__s1c44; ride.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1406: structure has no member named `direction'. */
struct RideWork__s1c44 {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot__s1c44 slots[4];
  u8 unk64[4];
  struct RideDirection direction;
};

DEFINE_FUNC_02001B64(Func_02001cbc, RideWork__s1c44, Func_02001a1c,
    Func_02001a80)

extern const u8 gData_020056a0[];

void Func_02001f90(struct Actor *object);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_020056a0. Overlays 6ab, 6ac and 6ad have the same function.
void Func_02001e1c(struct Actor *object, s32 part) {
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;
  ((struct SpriteParts *)object->sprite)->part = part;
  Actor_SetSingleChildFlags(object, 0);
  Actor_StartScript(object, gData_020056a0);
  object->timer = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}

DEFINE_FUNC_02001E60(Func_02001e60, Func_02004bd8_, Func_02001e1c,
    Func_02001ae0, Func_02004bc0)

DEFINE_FUNC_02001C74(Func_02001ea8, Func_02004bd8_, Func_02001e1c, 3,
    Func_02001cbc, Func_02004bc0)

DEFINE_FUNC_02001E60(Func_02001ef0, Func_02004bd8_, Func_02001e1c,
    Func_02001cbc, Func_02004bc0)

DEFINE_FUNC_02001F38_118(Func_02001f38)

DEFINE_FUNC_02001A9C(Func_02001f90, Func_02001f38)

DEFINE_FUNC_0200202C(Func_0200202c, Func_02004bc0, gData_020056a0)

DEFINE_FUNC_0200206C(Func_0200206c, Func_02004bd8_, Func_0200202c,
    Func_02001f90)

// ---- games/tla/src/scene/mars_lighthouse_entrance/s20ec.c ----
/* VIEW(s20ec): tag struct Actor as struct Actor__s20ec; actor.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1669: structure has no member named `flag5b'. */
typedef struct Actor__s20ec {
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

/* VIEW(s20ec): routine Func_0200206c as void Func_0200206c(); the parts before it has another declaration; tried as the definition's void Func_0200206c(s32 x, s32 z, s32 effect), it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1615: conflicting types for `Func_0200206c'. */
extern void Func_0200206c__s20ec() __asm__("Func_0200206c");

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

DEFINE_FUNC_02001BF8(Func_020020ec, Func_0200206c__s20ec, Func_02001e60,
    Func_02001ef0, Func_02001ea8, Func_02001c74)
#undef IWRAM
