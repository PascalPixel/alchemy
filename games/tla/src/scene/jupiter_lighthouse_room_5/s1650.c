/// @file
/// Scene code of overlay 0x69e, in ROM order.

#include "../shared/jupiter_lighthouse.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "gamepak/far_call/system.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_5.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "resource.h"

struct Spark {
  u8 unk00[0x0c];
  s32 dx;
  s32 dy;
  s32 speed;
  s32 age;
};

struct SparkWork {
  u8 tiles[0x400];
  struct Spark sparks[96];
  struct SparkSprite sprites[96];
  s32 from[3];
  s32 to[3];
  s16 next;
  u16 tile;
  s16 vramEntry;
};

/// Adds count sparks to the spark ring's 96 entries in heap slot 220, each
/// with a random angle from 0x6000 across three quarters of a turn, the
/// cosine and negated sine of it at 64 pixels as its direction, a random
/// speed of 16 to 31 and an age of 0.
void Func_02001650(s32 count) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark;
  s32 angle;
  s32 i;
  s32 radius;
  u64 cosine;
  s32 speed;

  if (count > 0) {
    i = count;
  next:
    spark = &work->sparks[work->next];
    angle = ((Random16Far() * 3) >> 2) + 0x6000;
    radius = 0x400000;
    speed = ((Random16Far() << 4) >> 16) + 16;
    /* FAKEMATCH(temporary): the cosine held in a 64-bit temporary; as
     * Iwram_MulQ16(Math_Cosine(angle), radius), 14 of 92 instructions differ
     * in tla-en. */
    cosine = Math_Cosine(angle);
    spark->dx = Iwram_MulQ16(cosine, radius) >> 16;
    spark->dy = -Iwram_MulQ16(Math_Sin(angle), radius) >> 16;
    spark->speed = speed;
    spark->age = 0;
    work->next++;
    work->next %= 96;
    if (--i != 0)
      goto next;
  }
}

#include "actor.h"
#include "main_routine_work.h"

/* VIEW(s1650): routine Func_020029f0 as s32 Func_020029f0(struct Actor *object); module/scene/scene.h has void Func_020029f0(struct FieldEffect *effect, s32 mode); tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/jupiter_lighthouse_room_5/s1650.c:98: too few arguments to function `Func_020029f0'. */
extern s32 Func_020029f0__s1650(struct Actor *object) __asm__("Func_020029f0");

void Func_02001718(const s32 *from, const s32 *to) {
  struct SparkWork *work = HeapSlot_AllocFar(220, sizeof(struct SparkWork));
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct SparkSprite *sprite;
  struct Spark *spark;
  s32 entry;
  s32 tile;
  s32 i;

  work->from[0] = from[0];
  work->from[1] = from[1];
  work->from[2] = from[2];
  work->to[0] = to[0];
  work->to[1] = to[1];
  work->to[2] = to[2];
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs), work);
  entry = Resource_FindFreeEntry();
  tile = Vram_Load(entry, 0x400, work);
  work->tile = tile;
  work->vramEntry = entry;
  i = 0;
  spark = work->sparks;
  sprite = work->sprites;
  for (; i <= 95; i++, sprite++, spark++) {
    Func_020029e0(sprite, 4, 4, 0, tile + (i & 15) * 2);
    sprite->fullColor = 1;
    sprite->palette = 0;
    sprite->priority = Func_020029f0__s1650(leader) & 3;
    // Through a union, so this store may alias the age store after it.
    ((union {
       u16 v;
       s32 w;
     } *)&sprite->depth)
        ->v = Func_020029f8(leader) - 1;
    spark->age = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  Task_AddFrame(Func_02001538, 0x480);
}

#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "task.h"

void Func_0200185c(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Func_02001538);
  Resource_ResetEntry(work->vramEntry);
  HeapSlot_Release(220);
}

extern struct LightOam gData_02003488[];

void Func_020027d8(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gData_02003488,
    gLightSpriteCount, gData_02003448, gLightSpritesLarge, gLightSpritePriority,
    Func_020029f0__s1650, Func_020027d8)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gData_02003448, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gData_02003448, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gData_02003448, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount, gData_02003448)

void Func_02001bfc(s32 value) { gLightSpritePriority = value; }

#include "../../field/object/set_sprite_priority.c"
