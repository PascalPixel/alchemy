/// @file
/// Scene code of overlay 0x6ad, in ROM order.

// ---- games/tla/src/scene/mars_lighthouse_room_f/s3d80.c ----
#include "../shared/teleporter.h"
#include "../shared/jupiter_lighthouse.h"
#include "../shared/mars_lighthouse.h"
#include "../shared/push.h"
#include "../shared/tables.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "event_work.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "module/field/field_effect.h"
#include "module/scene/mars_lighthouse_room_f.h"
#include "module/scene/scene.h"
#include "module/system/system.h"

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveTo(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetShake(a, b, c);
}

DEFINE_TELEPORTER_DROP_IN(Func_02003d80, Func_020035b8)

#include "../mars_lighthouse_room_f.h"

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02003b78(struct WarpWork *work, struct WarpEntry *entry);
void Func_02003be8(struct WarpWork *work, struct WarpEntry *entry);

s32 Func_02003f18(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct WarpWork *work = &gData_020070a8;
  struct WarpEntry *entry = work->entries;
  s32 index;

  index = 0;
  if (index >= work->count)
    goto end;
top:
  if (entry->x == x && entry->z == z) {
    if (!Event_GetFlag(entry->flag)) {
      Func_02003b78(work, entry);
      Event_SetFlag(entry->flag);
      work->current = index;
      goto end;
    }
    if (index == work->current)
      goto end;
    work->current = index;
    Func_02003be8(work, entry);
    GameFlag_SetByte(work->positionFlag, x);
    GameFlag_SetByte(work->positionFlag + 8, z);
    return 1;
  }
  index++;
  entry++;
  if (index < work->count)
    goto top;
end:
  return 0;
}

void Func_02003fc8(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct WarpWork *work = &gData_020070a8;
  s32 x = FieldGameFlag_GetByte(work->positionFlag);
  s32 z = FieldGameFlag_GetByte(work->positionFlag + 8);

  if (x != 0 || z != 0) {
    struct Actor *other;

    GameFlag_SetByte(work->positionFlag, 0);
    GameFlag_SetByte(work->positionFlag + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    other = *(struct Actor **)(event + 460);
    other->x = object->x;
    other->z = object->z;
    Map_CenterOnFollowedPoint();
    Func_02003d80();
    WORD_AT(Ram_HeapSlots->eventWork, 428) = value;
  }
}

#include "input.h"

/* VIEW(s3d80): variable gData_02005ef8 as extern const s16 gData_02005ef8[];
 * the definition has s32 gData_02005ef8[] = { 0xffff, - 0x8000, (s32)
 * 0xe000c000, (s32) 0xc000a000, 0x20004000, 0x40006000, 0xffff, - 0x8000, };
 * tried as the definition's extern struct Trigger gData_02005ef8[], it does not
 * compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f/s3d80.c:160:
 * incompatible types in assignment. */
extern const s16 gData_02005ef8__s3d80[] __asm__("gData_02005ef8");

DEFINE_ACTOR_STEER_BY_PAD(Func_02004080, gData_02005ef8__s3d80, Func_02005480)

DEFINE_LEADER_STOP_IF_PENDING(Func_0200410c)

#include "fixed_math.h"
#include "scene_tables.h"
#include "system/iwram_call.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

/* VIEW(s3d80): variable gData_02005f38 as extern s16 gData_02005f38[]; the
 * definition has s32 gData_02005f38[] = { 0xffff, - 0x8000, 0xc000, (s32)
 * 0xc0008000, 0x4000, 0x40008000, 0xffff, - 0x8000, }; tried as the
 * definition's extern s32 gData_02005f38[], it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f/s3d80.c:243:
 * `gData_02005f38__s3d80' undeclared (first use in this function). */
extern s16 gData_02005f38__s3d80[] __asm__("gData_02005f38");

DEFINE_LEADER_SLIDE_ON_ICE(Func_02004140, Func_0200410c, gData_02005f38__s3d80,
    Func_02005480, Func_02004080, Import_FieldCheckSpheresOverlap)

#include "version.h"

#include "call.h"

#define SHARED_F02003070_NAME Func_020043ec
#define SHARED_F02003070_ID2 gData_020074c8
#define SHARED_F02003070_ID3 Func_0200410c
#define SHARED_F02003070_ID4 gData_02005f38__s3d80
#define SHARED_F02003070_ID5 Func_02005480
#define SHARED_F02003070_ID6 Func_02005530
#define SHARED_F02003070_ID7 Func_02004080
#define SHARED_F02003070_ID8 Import_FieldCheckSpheresOverlap
#include "../shared/func_02003070.c"
#undef SHARED_F02003070_NAME
#undef SHARED_F02003070_ID2
#undef SHARED_F02003070_ID3
#undef SHARED_F02003070_ID4
#undef SHARED_F02003070_ID5
#undef SHARED_F02003070_ID6
#undef SHARED_F02003070_ID7
#undef SHARED_F02003070_ID8

// ---- games/tla/src/scene/mars_lighthouse_room_f/s4700.c ----

void Func_02004700(s32 value) { gData_020074c8 = value; }

#include "system/main_routine_work.h"

DEFINE_LEADER_SLIDE_ON_ICE_AHEAD(Func_0200470c, Func_0200410c, Func_02005480, Func_02004080,
    Import_FieldCheckSpheresOverlap)

#include "field_sprite.h"

extern u16 gData_02005f58[];
/* VIEW(s4700): variable gData_02005f64 as extern s8 gData_02005f64[][4]; the
 * definition has u32 gData_02005f64[] = { 0x820f8e0, 0x2008e0f8, 0x20f0e0,
 * 0x2008e0f8, 0x820f8e0, 0x2008e0f8, }; tried as the definition's extern u32
 * gData_02005f64[], it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f/s3d80.c:668: subscripted value is
 * neither array nor pointer. */
extern s8 gData_02005f64__s4700[][4] __asm__("gData_02005f64");

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02004938(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_Get(id);
  s16 shape = *(s16 *)((struct FieldSprite *)object->sprite)->record;
  s16 i = 0;
  u32 j;
  s32 width;
  s32 height;
  s32 x;
  s32 z;
  s32 mapX;
  s32 mapZ;
  s32 toX;
  s32 toZ;

  if ((u16)shape != gData_02005f58[(u16)i]) {
  next:
    j = (u16)((i << 16) + 0x10000 >> 16);
    i = j;
    if (j <= 5 && (u16)shape != gData_02005f58[j])
      goto next;
  }
  j = (u16)i;
  if (j == 6)
    return 0;
  width =
      (ABS(gData_02005f64__s4700[j][0]) + ABS(gData_02005f64__s4700[j][2])) >>
      4;
  height =
      (ABS(gData_02005f64__s4700[j][1]) + ABS(gData_02005f64__s4700[j][3])) >>
      4;
  x = (object->x + (gData_02005f64__s4700[j][0] << 16)) >> 20;
  z = (object->z + (gData_02005f64__s4700[j][1] << 16)) >> 20;
  mapX = *(s32 *)(map + 328) >> 20;
  i = mapX;
  mapZ = *(s32 *)(map + 332) >> 20;
  toX = x + (u16)i;
  toZ = z + (u16)mapZ;
  MapGrid_CopyCellAttributes(x, z, width, height, toX, toZ);
  Func_02004a44(0, x, z, width, height, 255);
  Func_02004a44(2, x, z, width, height, 255);
  return 1;
}

void Func_02004a44(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value) {
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
  u8 *cells = map->layers[layer].cells;
  u32 row;
  u32 column;

  cells += (x + (z << 7)) << 2;
  for (row = 0; row < height; row++) {
    u8 *cell = cells + (row << 9);

    for (column = 0; column < width; column++) {
      cell[2] = value;
      cell += 4;
    }
  }
}

/* VIEW(s4700): variable gData_02005f7c as extern s32 gData_02005f7c[]; the
 * definition has s16 gData_02005f7c[] = { 0, 16, 0, 16, 0, 16, 16, 0, 16, 0,
 * 16, 0, 16, 0, 0, - 16, 0, - 16, 0, - 16, 0, - 16, - 16, 0, - 16, 0, - 16, 0,
 * - 1...; tried as the definition's extern s16 gData_02005f7c[], Func_02004a8c
 * moves: 12 of 171 instructions, Func_02004bf4 moves: 41 of 132 instructions,
 * Func_02004d10 moves: 27 of 304 instructions in tla-ja. */
extern s32 gData_02005f7c__s4700[] __asm__("gData_02005f7c");

static inline s32 Func_020054f8_(struct Actor *object, s32 *position) {
  return MapGrid_CheckCollision(object, position);
}

/// Finds the object the leader is pushing and steps its footprint, one cell
/// at a time in the leader's direction, until a cell of it is blocked
/// (MapGrid_CheckCollision returns 2). Leaves the object's resting place in target and
/// returns 1, or 0 when nothing is pushed or it cannot move at all.
DEFINE_PUSH_TARGET_FIND_SLIDE(Func_02004a8c, Func_02004bf4, gData_02005f64__s4700,
    gData_02005f7c__s4700, Func_020054f8_)

#include "event_work.h"

#undef PIXELS
/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

DEFINE_PUSH_TARGET_FIND_AHEAD(Func_02004bf4, gData_02005f58, gData_02005f7c__s4700,
    gData_02005f64__s4700)

#undef PIXELS

DEFINE_PUSH_TARGET_PUSH(Func_02004d10, gData_02005f64__s4700, Func_02004a44,
    Func_02004fc0, Func_02005480, gData_02005f7c__s4700)

DEFINE_ACTOR_STOP_AT_PUSH_TARGET(Func_02004fc0, Func_02005680, gData_02005f58,
    MapGrid_CheckCollision)
#undef ABS
#undef PIXELS
#define PIXELS(value) ((value) << 16)

// ---- games/tla/src/scene/mars_lighthouse_room_f/s5048.c ----
#define MapLayer MapLayer__s5048
#include "map.h"
#undef MapLayer
/* VIEW(s5048): tag struct MapLayer as struct MapLayer__s5048; room.h has
 * another definition; not tried: struct MapLayer__s5048 was not found. */
#include "field_event.h"
#include "vram_tab.h"

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(Func_02005048, gData_020074cc, Data_0200f510,
    gData_020074ce, gData_020074d0, gData_020075d0, gData_020075d2,
    BattleEffect_GetObjectPriority, Import_OamAddSpriteAtDepthClamped)

#include "iwram_call.h"
#include "task.h"

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(Func_02005204, gData_020074d0, gData_020074cc,
    Func_02005048, gData_020074ce, gData_020075d0, gData_020075d2)

DEFINE_LIGHT_SPRITES_INIT_B(Func_02005284, gData_020074d0, gData_020074cc,
    Func_02005048, gData_020074ce, gData_020075d0, gData_020075d2)

DEFINE_LIGHT_SPRITES_INIT_C(Func_02005304, gData_020074d0, gData_020074cc,
    Func_02005048, gData_020074ce, gData_020075d0, gData_020075d2)

DEFINE_LIGHT_SPRITES_ADD(Func_0200538c, gData_020074ce, gData_020074d0)

void Func_020053bc(s32 value) { gData_020075d2 = value; }

#include "objcmd.h"

void Func_0200004c(void);
void Func_020006f4(void);
void Func_02003478(void);
void Func_0200351c(void);

s32 gData_020056c8[] = {
    SC_CALL(Func_0200004c),
    SC_END,
};

// clang-format off
DEFINE_G_DATA_020056D4(gData_020056d4, 680, 760)

DEFINE_G_DATA_020056D4(gData_02005748, 584, 664)

u32 gData_020057bc[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_FLAG_CHECK(0x220),
    SC_IFNOT(SC_TO(1)),
    SC_LABEL(2),
    SC_MOVE(PIXELS(176), 0, PIXELS(576)),
    SC_SYNC,
    SC_MOVE(PIXELS(248), 0, PIXELS(576)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(176), 0, PIXELS(576)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(576)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(2)),
};

u32 gData_02005890[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(88), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LABEL(1),
    SC_MOVE(PIXELS(168), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(88), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

u32 gData_0200593c[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_LABEL(1),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(232), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(360), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

DEFINE_G_DATA_02004FF0(gData_02005a3c, 2)

u32 gData_02005a88[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(968), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LABEL(1),
    SC_MOVE(PIXELS(872), 0, PIXELS(80)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(872), 0, PIXELS(80)),
    SC_SYNC,
    SC_MOVE(PIXELS(968), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s16 gData_02005b5c[] = {
    24, 25, 26, 24, 25, 0,
};

u32 gData_02005b68[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005bac[] = {
    0, 42, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u16 gData_02005bf0[] = {
    8, 67, 0,
};

u16 gData_02005bf6[] = {
    10, 1, 0,
};

u16 gData_02005bfc[] = {
    18, 17, 19, 49, 20, 19, 21, 3,
    0,
};

u16 gData_02005c0e[] = {
    22, 0, 0,
};

u16 gData_02005c14[] = {
    13, 17, 14, 33, 15, 49, 16, 131,
    0,
};

u16 gData_02005c26[] = {
    19, 1, 0,
};

u16 gData_02005c2c[] = {
    13, 3, 0,
};

u16 gData_02005c32[] = {
    14, 1, 0,
};

u16 gData_02005c38[] = {
    8, 3, 9, 3, 10, 3, 0,
};

u16 gData_02005c46[] = {
    12, 0, 0,
};

u16 gData_02005c4c[] = {
    13, 3, 14, 3, 15, 3, 0,
};

u16 gData_02005c5a[] = {
    10, 0, 0,
};

u16 gData_02005c60[] = {
    24, 3, 25, 3, 26, 3, 0,
};

u16 gData_02005c6e[] = {
    18, 0, 19, 0, 20, 0, 21, 0,
    22, 0, 0,
};

DEFINE_G_DATA_020054CC(gData_02005c84, Func_02003478, 0xf80000, 0x200000,
    0x2a00000, 0x160000, 0xf80000, 0x2b00000, Func_020006f4, Func_0200351c)

DEFINE_G_DATA_020054CC(gData_02005ce0, Func_02003478, 0xb80000, 0x400000,
    0x2300000, 0x360000, 0xb80000, 0x2400000, Func_02000d80, Func_0200351c)

DEFINE_G_DATA_0200547C(gData_02005d3c, Func_02003478, 872, 128, 22, 872, 144,
    Func_0200351c)

DEFINE_G_DATA_02001540(gData_02005d8c, 30)

u32 gData_02005dbc[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005e00[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

DEFINE_G_DATA_020012A4(gData_02005e44, 22)

DEFINE_G_DATA_020012A4(gData_02005e80, 44)

DEFINE_G_DATA_020012A4(gData_02005ebc, 126)

s32 gData_02005ef8[] = {
    0xffff, -0x8000, (s32)0xe000c000, (s32)0xc000a000, 0x20004000, 0x40006000,
    0xffff, -0x8000,
};

s16 gData_02005f18[] = {
    -1, 0, -0x8000, -1, -0x4000, -1, -1, -0x4000,
    0x4000, -1, -1, 0x4000, -1, 0, -0x8000, -1,
};

s32 gData_02005f38[] = {
    0xffff, -0x8000, 0xc000, (s32)0xc0008000, 0x4000, 0x40008000,
    0xffff, -0x8000,
};

u16 gData_02005f58[] = {
    258, 259, 293, 294, 332, 331,
};

u32 gData_02005f64[] = {
    0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

DEFINE_G_DATA_0200198C(gData_02005f7c)
// clang-format on
