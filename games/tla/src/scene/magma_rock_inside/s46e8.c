// ---- games/tla/src/scene/magma_rock_inside/s46e8.c ----
#include "../shared/routines.h"
#include "../shared/tables.h"
#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/field/field_effect.h"
#include "module/scene/magma_rock_inside.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "particle.h"
#include "resource.h"

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
/* VIEW(s46e8): routine Actor_StartScript as void Actor_StartScript(struct FieldEffect *effect, const s32 *script); local.h has void Actor_StartScript(struct Actor *object, const void *data); not tried: Actor_StartScript's bound declaration was not found. */
extern void Func_02004ca0__s46e8(struct FieldEffect *effect, const s32 *script) __asm__("Actor_StartScript");
void Actor_StartScript(struct Actor *object, const void *data);

void Func_020046e8(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16Far() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)FieldActor_Create(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Func_02004e98(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Func_02004c98(effect, 2);
      Func_02004ca0__s46e8(effect, gData_02005638);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Math_Cosine(i << 13), 0x13333);
        effect->velocity.z = Math_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16Far() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16Far() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

void Func_0200481c(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)FieldActor_Create(285, leader->x, leader->y,
                                                     leader->z);
    if (effect != 0) {
      frame = Func_02004e98(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Func_02004c98(effect, 2);
      Func_02004ca0__s46e8(effect, gData_02005668);
      effect->velocity.x = Math_Cosine(i << 12);
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sin(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16Far() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
    i++;
  } while (i <= 16);
}

// ---- games/tla/src/scene/magma_rock_inside/s4934.c ----
#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"
#include "map.h"

s32 Func_02004d20(s32 layer, s32 x, s32 z);
s32 Func_02004ef0(s32 layer, s32 shape, s32 height);

DEFINE_FUNC_02000BC0(Func_02004934, Func_02004d20, Func_02004ef0)

#include "local.h"

void Func_020049c0(s32 a, s32 b) {
  Func_02004934(a, b, gMainRoutineWork.currentOwner);
}

#include "system/ram_buffer.h"

void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

DEFINE_FUNC_0200379C(Func_020049d8, Func_02004f18)

/// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
/// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
void Func_02004a5c(s32 x, s32 y, s32 z, s32 count) {
  struct RockWork *work;
  s32 i;

  /* FAKEMATCH(barrier): an empty asm before the work is read keeps the spills
   * of x and y at the entry; as an initializer, 4 of 88 instructions differ in
   * tla-en. */
  asm("");
  work = Ram_HeapSlots->menuRuntime;
  // A goto loop carries no loop notes, so the offsets are rebuilt on every
  // pass, as the bytes have them; a for loop hoists them.
  if (count > 0) {
    i = count;
  loop:;
    {
      s32 angle = Random16Far();
      struct Rock *rock = &work->rocks[work->next];

      rock->timer = 0;
      rock->position[0] = x;
      rock->position[1] = y;
      rock->position[2] = z;
      FieldVector_AddPolarOffset((Random16Far() << 3) + 0x80000, angle,
                                 rock->position);
      rock->velocity[0] = 0;
      rock->velocity[1] = 0x50000;
      rock->velocity[2] = 0;
      FieldVector_AddPolarOffset((Random16Far() << 1) + 0x20000, angle,
                                 rock->velocity);
      work->next++;
      work->next &= 63;
    }
    i--;
    if (i != 0)
      goto loop;
  }
}

// ---- games/tla/src/scene/magma_rock_inside/s4b18.c ----
#include "system/task.h"

static __inline__ void StartTask(void (*func)(void), s32 priority) {
  /* FAKEMATCH(wrapper): Task_Add forwarded inline; as a direct call, 6 of 83
   * instructions differ in tla-en. */
  Task_Add(func, priority);
}

void Func_02004b18(void) {
  struct RockWork *work = HeapSlot_AllocFar(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Func_02004c58(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = Resource_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    Func_02004f10(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  StartTask(Func_020049d8, 0x480);
}

void Func_02004bd4(void) {
  struct MagmaRockInsideSparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Func_020049d8);
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(220);
}

#include "scene_tables.h"
#include "objcmd.h"

void Func_020005a0(void);
void Func_02000e14(void);
void Func_02000e20(void);
void Func_02001148(void);
void Func_02001ca0(void);
void Func_02001cd0(void);
void Func_0200217c(void);
void Func_020023e8(void);
void Func_02002a6c(void);
void Func_02002b2c(void);
void Func_02002bd0(void);

extern u32 gData_02004fb0[];

// clang-format off
u32 gData_02004f38[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02004f74[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02004fb0[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

DEFINE_G_DATA_0200198C(gData_02004fec)

u32 gData_0200502c[] = {
    39, 2, 0, 18, 3, 0,
    1, 38, 17,
};

u32 gData_02005050[] = {
    39, 3, 0, 23, 3, 0,
    1, 38, 17,
};

s16 gData_02005074[] = {
    4, 0, 0, 776, 0, 16, 0, 680,
    1, 0, 4, 0, 0, 744, 0, 16,
    0, 680, 1, 0, 4, 0, 0, 744,
    0, 16, 0, 632, 1, 0, 4, 0,
    0, 776, 0, 16, 0, 632, 1, 0,
    13, 0, -1, 0, 0, 0, 17, 0,
};

u32 gData_020050d4[] = {
    SC_MOVE(PIXELS(216), PIXELS(16), PIXELS(568)),
    SC_SYNC,
    SC_MOVE(PIXELS(216), PIXELS(16), PIXELS(584)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), PIXELS(16), PIXELS(584)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), PIXELS(16), PIXELS(568)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

u16 gData_02005134[] = {
    4, 0, 0, 600, 0, 16, 0, 664,
    1, 0, 4, 0, 0, 728, 0, 16,
    0, 664, 1, 0, 4, 0, 0, 728,
    0, 16, 0, 632, 1, 0, 4, 0,
    0, 760, 0, 16, 0, 632, 1, 0,
    4, 0, 0, 760, 0, 16, 0, 664,
    1, 0, 17, 0,
};

u32 gData_0200519c[] = {
    SC_MOVE(PIXELS(184), PIXELS(16), PIXELS(232)),
    SC_SYNC,
    SC_MOVE(PIXELS(280), PIXELS(16), PIXELS(232)),
    SC_SYNC,
    SC_MOVE(PIXELS(280), PIXELS(16), PIXELS(264)),
    SC_SYNC,
    SC_MOVE(PIXELS(184), PIXELS(16), PIXELS(264)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s16 gData_020051fc[] = {
    4, 0, 0, 472, 0, 16, 0, 296,
    1, 0, 4, 0, 0, 424, 0, 16,
    0, 296, 1, 0, 4, 0, 0, 424,
    0, 16, 0, 264, 1, 0, 4, 0,
    0, 472, 0, 16, 0, 264, 1, 0,
    13, 0, -1, 0, 0, 0, 17, 0,
};

s16 gData_0200525c[] = {
    4, 0, 0, 216, 0, 16, 0, 600,
    1, 0, 4, 0, 0, 280, 0, 16,
    0, 600, 1, 0, 4, 0, 0, 280,
    0, 16, 0, 664, 1, 0, 4, 0,
    0, 216, 0, 16, 0, 664, 1, 0,
    13, 0, -1, 0, 0, 0, 17, 0,
};

s16 gData_020052bc[] = {
    4, 0, 0, 312, 0, 16, 0, 664,
    1, 0, 4, 0, 0, 376, 0, 16,
    0, 664, 1, 0, 4, 0, 0, 376,
    0, 16, 0, 600, 1, 0, 4, 0,
    0, 312, 0, 16, 0, 600, 1, 0,
    13, 0, -1, 0, 0, 0, 17, 0,
};

s16 gData_0200531c[] = {
    4, 0, 0, 440, 0, 16, 0, 600,
    1, 0, 4, 0, 0, 504, 0, 16,
    0, 600, 1, 0, 4, 0, 0, 504,
    0, 16, 0, 664, 1, 0, 4, 0,
    0, 440, 0, 16, 0, 664, 1, 0,
    13, 0, -1, 0, 0, 0, 17, 0,
};

s16 gData_0200537c[] = {
    4, 0, 0, 536, 0, 16, 0, 664,
    1, 0, 4, 0, 0, 600, 0, 16,
    0, 664, 1, 0, 4, 0, 0, 600,
    0, 16, 0, 600, 1, 0, 4, 0,
    0, 536, 0, 16, 0, 600, 1, 0,
    13, 0, -1, 0, 0, 0, 17, 0,
};

s16 gData_020053dc[] = {
    4, 0, 0, 376, 0, 16, 0, 264,
    1, 0, 4, 0, 0, 376, 0, 16,
    0, 232, 1, 0, 4, 0, 0, 408,
    0, 16, 0, 232, 1, 0, 4, 0,
    0, 408, 0, 16, 0, 264, 1, 0,
    13, 0, -1, 0, 0, 0, 17, 0,
};

s16 gData_0200543c[] = {
    4, 0, 0, 216, 0, 16, 0, 680,
    1, 0, 4, 0, 0, 280, 0, 16,
    0, 680, 1, 0, 4, 0, 0, 280,
    0, 16, 0, 712, 1, 0, 4, 0,
    0, 216, 0, 16, 0, 712, 1, 0,
    13, 0, -1, 0, 0, 0, 17, 0,
};

s16 gData_0200549c[] = {
    4, 0, 0, 376, 0, 16, 0, 680,
    1, 0, 4, 0, 0, 312, 0, 16,
    0, 680, 1, 0, 4, 0, 0, 312,
    0, 16, 0, 712, 1, 0, 4, 0,
    0, 376, 0, 16, 0, 712, 1, 0,
    13, 0, -1, 0, 0, 0, 17, 0,
};

s16 gData_020054fc[] = {
    15, -1,
};

s32 gData_02005500[] = {
    0x320008, -1,
};

s16 gData_02005508[] = {
    11, -1,
};

s16 gData_0200550c[] = {
    8, 50, -1, 13, 51, -1, 14, 52,
    -1, -1,
};

s16 gData_02005520[] = {
    23, 24, 25, -1,
};

s16 gData_02005528[] = {
    9, 12, -1,
};

s16 gData_0200552e[] = {
    23, -1, 0,
};

u32 gData_02005534[] = {
    0, 6, 38, 17,
};

DEFINE_G_DATA_02005218(gData_02005544)

s32 gData_02005560[] = {
    SC_CALL(Func_02002a6c),
    SC_FREE,
    SC_END,
};

DEFINE_G_DATA_02005244(gData_02005570, Func_02002b2c, Func_02002bd0,
    Func_02002c1c)

u32 gData_02005590[] = {
    SC_MOVE_BY(0, PIXELS(24), 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 4),
    SC_END,
};

u32 gData_020055b4[] = {
    SC_MOVE_BY(0, PIXELS(16), 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 4),
    SC_END,
};

s32 gData_020055d8[] = {
    SC_SET(SW_MOTION, 0),
    SC_MOVE_BY(0, PIXELS(-24), 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 0),
    SC_END,
};

s32 gData_02005608[] = {
    SC_SET(SW_MOTION, 0),
    SC_MOVE_BY(0, PIXELS(-16), 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 3),
    SC_END,
};

u32 gData_02005638[] = {
    SC_WAIT(33),
    SC_SET(SW_HOOK, 0),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u32 gData_02005668[] = {
    SC_WAIT(44),
    SC_SET(SW_HOOK, 0),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gData_02005698[] = {
    (s32)gData_02004f38,
    (s32)gData_02004f74,
    (s32)gData_02004fb0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(226),
    SCENE_EXIT(1, 226, 4),
    SCENE_EXIT(2, 227, 1),
    SCENE_EXIT(3, 227, 5),
    SCENE_EXIT(4, 226, 1),
    SCENE_EXIT(5, 227, 2),
    SCENE_EXIT(6, 227, 3),
    SCENE_EXIT(7, 227, 4),
    SCENE_EXIT(8, 228, 1),
    SCENE_EXITS(227),
    SCENE_EXIT(1, 226, 2),
    SCENE_EXIT(2, 226, 5),
    SCENE_EXIT(3, 226, 6),
    SCENE_EXIT(4, 226, 7),
    SCENE_EXIT(5, 226, 3),
    SCENE_EXIT(6, 227, 7),
    SCENE_EXIT(7, 227, 6),
    SCENE_EXIT(8, 225, 3),
    SCENE_EXITS(228),
    SCENE_EXIT(1, 226, 8),
    SCENE_EXIT(2, 228, 6),
    SCENE_EXIT(3, 229, 1),
    SCENE_EXIT(4, 229, 2),
    SCENE_EXIT(5, 229, 3),
    SCENE_EXIT(6, 228, 2),
    SCENE_EXIT(7, 229, 4),
    SCENE_EXIT(8, 228, 9),
    SCENE_EXIT(9, 228, 8),
    SCENE_EXIT(10, 229, 5),
    SCENE_EXIT(11, 229, 6),
    SCENE_EXITS(229),
    SCENE_EXIT(1, 228, 3),
    SCENE_EXIT(2, 228, 4),
    SCENE_EXIT(3, 228, 5),
    SCENE_EXIT(4, 228, 7),
    SCENE_EXIT(5, 228, 10),
    SCENE_EXIT(6, 228, 11),
    SCENE_EXIT(7, 231, 7),
    SCENE_EXIT(8, 230, 7),
    SCENE_EXIT(9, 230, 8),
    SCENE_EXIT(10, 230, 1),
    SCENE_EXIT(11, 231, 1),
    SCENE_EXIT(12, 231, 2),
    SCENE_EXIT(13, 231, 5),
    SCENE_EXIT(14, 231, 6),
    SCENE_EXITS(230),
    SCENE_EXIT(1, 229, 10),
    SCENE_EXIT(2, 231, 3),
    SCENE_EXIT(3, 230, 9),
    SCENE_EXIT(4, 230, 10),
    SCENE_EXIT(5, 231, 11),
    SCENE_EXIT(6, 231, 4),
    SCENE_EXIT(7, 229, 8),
    SCENE_EXIT(8, 229, 9),
    SCENE_EXIT(9, 230, 3),
    SCENE_EXIT(10, 230, 4),
    SCENE_EXITS(231),
    SCENE_EXIT(1, 229, 11),
    SCENE_EXIT(2, 229, 12),
    SCENE_EXIT(3, 230, 2),
    SCENE_EXIT(4, 230, 6),
    SCENE_EXIT(5, 229, 13),
    SCENE_EXIT(6, 229, 14),
    SCENE_EXIT(7, 229, 7),
    SCENE_EXIT(8, 231, 14),
    SCENE_EXIT(9, 231, 16),
    SCENE_EXIT(10, 231, 17),
    SCENE_EXIT(11, 230, 5),
    SCENE_EXIT(12, 231, 18),
    SCENE_EXIT(13, 231, 19),
    SCENE_EXIT(14, 231, 8),
    SCENE_EXIT(15, 232, 10),
    SCENE_EXIT(16, 231, 9),
    SCENE_EXIT(17, 231, 10),
    SCENE_EXIT(18, 231, 12),
    SCENE_EXIT(19, 231, 13),
    SCENE_EXITS(232),
    SCENE_EXIT(1, 233, 1),
    SCENE_EXIT(2, 232, 5),
    SCENE_EXIT(3, 232, 6),
    SCENE_EXIT(4, 232, 7),
    SCENE_EXIT(5, 232, 2),
    SCENE_EXIT(6, 232, 3),
    SCENE_EXIT(7, 232, 4),
    SCENE_EXIT(8, 232, 9),
    SCENE_EXIT(9, 232, 8),
    SCENE_EXIT(10, 231, 15),
    SCENE_EXIT(11, 223, 2),
    SCENE_EXITS(233),
    SCENE_EXIT(1, 232, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02005840[] = {
    {319, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005870[] = {
    {377, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_WEST, 2, 2},
    {334, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {334, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 6, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 7, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 8, 0, 0, FACING_NORTH, 2, 2},
    {489, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_NORTH, 2, 2},
    {245, 0x6b, 1, PIXELS(296), 0, PIXELS(504), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005978[] = {
    {377, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_WEST, 2, 2},
    {489, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_SOUTH, 2, 3},
    {245, 0x68, 1, PIXELS(456), 0, PIXELS(600), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005a50[] = {
    {377, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_EAST, 2, 2},
    {404, 0x350, 1, 2, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 2},
    {404, 0x351, 1, 4, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 6, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 7, 0, 0, FACING_WEST, 2, 2},
    {319, CONDITION_ALWAYS, 7, 8, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 11, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 12, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 13, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 14, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 15, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 16, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 17, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 18, 0, 0, FACING_NORTH, 2, 2},
    {334, CONDITION_ALWAYS, 7, 19, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005c30[] = {
    {489, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {489, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {373, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005ca8[] = {
    {319, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005d38[] = {
    {312, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 2},
    {292, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_NORTH, 2, 2},
    {325, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_NORTH, 2, 2},
    {325, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_NORTH, 2, 3},
    {325, CONDITION_ALWAYS, 1, 8, 0, 0, FACING_NORTH, 2, 3},
    {325, CONDITION_ALWAYS, 1, 9, 0, 0, FACING_NORTH, 2, 3},
    {364, CONDITION_ALWAYS, 1, 10, 0, 0, FACING_NORTH, 2, 2},
    {364, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_NORTH, 2, 3},
    {364, CONDITION_ALWAYS, 1, 12, 0, 0, FACING_NORTH, 2, 3},
    {364, CONDITION_ALWAYS, 1, 13, 0, 0, FACING_NORTH, 2, 3},
    {365, CONDITION_ALWAYS, 1, 14, 0, 0, FACING_NORTH, 2, 2},
    {365, CONDITION_ALWAYS, 1, 15, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 16, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005ed0[] = {
    {323, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_SOUTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {365, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_NORTH, 2, 2},
    {365, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_NORTH, 2, 2},
    {5, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gData_02005f60[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {0x31, 8, CONDITION_ALWAYS, 8},
    {0x202, 70, CONDITION_ALWAYS, EVENT(Func_020011fc)},
    {0x202, 78, CONDITION_ALWAYS, EVENT(Func_020011fc)},
    {TRIGGERS_END},
};

struct Trigger gData_02005fe4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {0xc400, 17, CONDITION_ALWAYS, EVENT(Func_020005a0)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_0200099c)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000a30)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, 0x9f0, EVENT(Func_0200099c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x9f0, EVENT(Func_02000a30)},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(Func_02000e14)},
    {0x50009705, 50, CONDITION_ALWAYS, EVENT(Func_02000e20)},
    {0x9705, 50, CONDITION_ALWAYS, EVENT(Func_02001148)},
    {TRIGGERS_END},
};

struct Trigger gData_020060bc[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {0x202, 70, CONDITION_ALWAYS, EVENT(Func_020011fc)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Func_02000594)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_0200099c)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000a30)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11, 0x9f1, EVENT(Func_0200099c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x9f1, EVENT(Func_02000a30)},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(Func_02000e14)},
    {0x50009705, 50, CONDITION_ALWAYS, EVENT(Func_02000e20)},
    {0x9705, 50, CONDITION_ALWAYS, EVENT(Func_02001148)},
    {TRIGGERS_END},
};

struct Trigger gData_020061c4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x202, 70, CONDITION_ALWAYS, EVENT(Func_020011fc)},
    {0x202, 71, 0x1301, EVENT(Func_020011fc)},
    {0x202, 72, CONDITION_ALWAYS, EVENT(Func_020011fc)},
    {0xc602, 73, CONDITION_ALWAYS, EVENT(Func_020011fc)},
    {0x8602, 75, CONDITION_ALWAYS, EVENT(Func_020011fc)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_0200099c)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000a30)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 23, 0x9f2, EVENT(Func_0200099c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, 0x9f2, EVENT(Func_02000a30)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 24, 0x9f3, EVENT(Func_0200099c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 24, 0x9f3, EVENT(Func_02000a30)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 25, 0x9f4, EVENT(Func_0200099c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 25, 0x9f4, EVENT(Func_02000a30)},
    {0x50009705, 71, 0x350, EVENT(Func_02001ca0)},
    {0x50009705, 72, 0x351, EVENT(Func_02001cd0)},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(Func_02000e14)},
    {0x50009705, 50, 0x2f0, EVENT(Func_02000e20)},
    {0x50009705, 50, CONDITION_ALWAYS, EVENT(Func_02001ca0)},
    {0x9705, 50, CONDITION_ALWAYS, EVENT(Func_02001148)},
    {0x80009705, 51, CONDITION_ALWAYS, EVENT(Func_02000e14)},
    {0x50009705, 51, CONDITION_ALWAYS, EVENT(Func_02000e20)},
    {0x9705, 51, CONDITION_ALWAYS, EVENT(Func_02001148)},
    {0x80009705, 52, CONDITION_ALWAYS, EVENT(Func_02000e14)},
    {0x50009705, 52, CONDITION_ALWAYS, EVENT(Func_02000e20)},
    {0x9705, 52, CONDITION_ALWAYS, EVENT(Func_02001148)},
    {TRIGGERS_END},
};

struct Trigger gData_020063a4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, 0},
    {TRIGGERS_END},
};

struct Trigger gData_02006458[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x31, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, 19, CONDITION_ALWAYS, 19},
    {0x202, 70, 0x1302, EVENT(Func_020011fc)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_0200099c)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000a30)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, 0x9f5, EVENT(Func_0200099c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x9f5, EVENT(Func_02000a30)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, 0x9f6, EVENT(Func_0200099c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x9f6, EVENT(Func_02000a30)},
    {TRIGGERS_END},
};

struct Trigger gData_0200659c[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_020023e8)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0x21, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 30, 0x9f8, EVENT(Func_02001d64)},
    {TRIGGER_EXAMINE, 31, 0x9f9, EVENT(Func_02002364)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_0200099c)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000a30)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 23, 0x9f7, EVENT(Func_0200099c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, 0x9f7, EVENT(Func_02000a30)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(Func_02002128)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 18, CONDITION_ALWAYS, EVENT(Func_02002128)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(Func_02002128)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 20, CONDITION_ALWAYS, EVENT(Func_02002128)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 21, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 22, CONDITION_ALWAYS, EVENT(Func_0200217c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 22, 0x304, EVENT(Func_020021c0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 21, 0x303, EVENT(Func_020021c0)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 17, CONDITION_ALWAYS, EVENT(Func_02002318)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 18, CONDITION_ALWAYS, EVENT(Func_02002318)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 19, CONDITION_ALWAYS, EVENT(Func_02002318)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 20, CONDITION_ALWAYS, EVENT(Func_02002318)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 22, CONDITION_ALWAYS, EVENT(Func_02002318)},
    {TRIGGERS_END},
};

s32 gData_02006710 = -1;

struct Trigger gData_02006714[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(Func_0200257c)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(Func_02002610)},
    {0xc403, 30, CONDITION_ALWAYS, EVENT(Func_020030cc)},
    {0x4403, 30, CONDITION_ALWAYS, EVENT(Func_020032d8)},
    {0x8403, 30, CONDITION_ALWAYS, EVENT(Func_02003348)},
    {0x403, 30, CONDITION_ALWAYS, EVENT(Func_020033b0)},
    {TRIGGER_RAISED, 202, CONDITION_ALWAYS, EVENT(Func_020027d4)},
    {TRIGGER_RAISED, 203, CONDITION_ALWAYS, EVENT(Func_020027d4)},
    {TRIGGER_RAISED, 204, CONDITION_ALWAYS, EVENT(Func_020027d4)},
    {TRIGGER_RAISED, 205, CONDITION_ALWAYS, EVENT(Func_020027d4)},
    {TRIGGER_RAISED, 206, CONDITION_ALWAYS, EVENT(Func_020027d4)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, EVENT(Func_02002868)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 11, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 11, 0x305, EVENT(Func_020028ac)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 10, CONDITION_ALWAYS, EVENT(Func_020029dc)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 11, CONDITION_ALWAYS, EVENT(Func_020029dc)},
    {TRIGGERS_END},
};
