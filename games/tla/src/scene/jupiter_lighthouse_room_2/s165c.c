/// @file
/// Scene code of overlay 0x69b, in ROM order, from s165c's offset up to the
/// next object the overlay links.

#include "../shared/routines.h"
#include "types.h"
#include "field_effect.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/scene.h"

extern const s32 *gData_02002b50[];

void Func_020021b4(struct Actor *object, s32 a);
void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);

void Func_0200165c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_020021b4(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect, gData_02002b50[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags(effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gData_02002b50[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Func_020021b4(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

// The cells Func_02001fbc watches, and the cell it copies over the one the
// leader steps on.
#define CELL_TABLE gData_02002ce0
#define CELL_SPRITE_DRAW Func_02001824
#define CELL_PUSH_SLOT_ENTRY Func_0200218c
#include "../../field/map/cell_sprite.c"

#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "scene_tables.h"
#include "cell_target.h"

s32 Func_02001950(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gData_02002ce0;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *rows;
  struct CellMapCell *cell;
  s32 x0, z0, width, height, i, j;
  u32 *buffer;
  u32 event, last;
  s32 cz, cx;

  Iwram_ClearWords(table, 4);
  table->kind = kind;
  table->firstFlag = flag;
  table->flagByte = flagByte;
  table->x = x;
  table->z = z;
  table->current = -1;
  table->count = 0;
  map = (struct CellMapWork *)Ram_HeapSlots->mapWork;
  layer = &map->layers[0];
  rows = layer->cells;
  x0 = (layer->originX + map->minX) >> 20;
  z0 = (layer->originZ + map->minZ) >> 20;
  width = (map->maxX - map->minX) >> 20;
  height = (map->maxZ - map->minZ) >> 20;
  rows += ((map->minZ >> 20) << 7) + (map->minX >> 20);
  // Goto loops: as for loops (either one) loop.c moves the 64 of the cell
  // copy's masks out of them and 321 of 268 instructions differ.
  j = 0;
  if (j < height) {
  row:
    cell = rows + (j << 7);
    i = 0;
    if (i < width) {
    column:
      event = cell->event;
      if (event != 0 && event >= kind) {
        last = kind + 1;
        if (event <= last) {
          cx = i + x0;
          entry->x = cx;
          cz = j + z0;
          entry->z = cz;
          entry->flag = flag;
          flag++;
          if (!Event_GetFlag(0x109)) {
            if (event == last)
              Event_SetFlag(entry->flag);
          } else if (Event_GetFlag(entry->flag)) {
            s32 sourceX = table->x;
            s32 sourceZ = table->z;

            cx |= (s16)(table->x & 64);
            cz |= (s16)(table->z & 64);
            MapGrid_CopyCells(sourceX, sourceZ + 1, 1, 1, cx, cz);
          }
          entry++;
          table->count++;
          /* FAKEMATCH(barrier): an empty asm on the stored count; as no asm, 2
           * of 268 instructions differ in tla-en. */
          asm("" : "=m"(table->count) : "m"(table->count) : "r3");
          /* FAKEMATCH(barrier): an empty asm on the word over the count; as no
           * asm, 2 of 268 instructions differ in tla-en. */
          asm("" : "=m"(*(s32 *)&table->z));
        }
      }
    columnNext:
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as no
       * asm, 224 of 268 instructions differ in tla-en. */
      asm("");
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as no
       * asm, 224 of 268 instructions differ in tla-en. */
      asm("");
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as no
       * asm, 224 of 268 instructions differ in tla-en. */
      asm("");
      i++;
      cell++;
      if (i < width)
        goto column;
    }
  rowNext:
    j++;
    if (j < height)
      goto row;
  }
  if (Event_GetFlag(0x109)) {
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
    s32 leaderX = leader->x >> 20;
    s32 leaderZ = leader->z >> 20;
    s32 k;

    entry = table->entries;
    k = 0;
    if (k < table->count) {
    leaderRow:
      do {
        if (entry->x == leaderX && entry->z == leaderZ)
          table->current = k;
      leaderNext:
        k++;
        entry++;
        // A cast read: gcse keeps it in the loop.
      } while (k < *(s16 *)&table->count);
    }
  }
  buffer = Heap_Alloc(0x100);
  {
    u32 *word = buffer;
    s32 count = 63;

  fill:
    *word++ = 0x11111111;
  fillNext:
    if (--count >= 0)
      goto fill;
  }
  table->tile = Resource_FindFreeEntry();
  Vram_Load(table->tile, 0x100, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001824, 0x480);
  return table->count;
}

DEFINE_FUNC_020013E4(Func_02001b94)
#include "../shared/cell_with_bit.h"

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Func_02001fbc calls it the first time the leader steps on
/// one of the table's cells.
void Func_02001c1c(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s16 sourceX;
  s32 leaderX = leader->x;
  s32 x;
  s32 z;

  /* FAKEMATCH(barrier): passes the source column through an asm reading the
   * leader's x, so the x load ties the 64 mask's copy and wins on order; as
   * (leader->x >> 20) | (s16)(table->x & 64), 2 of 49 instructions differ in
   * tla-en. */
  asm("" : "=r"(sourceX) : "0"(table->x), "r"(leaderX));
  x = (leaderX >> 20) | (s16)(sourceX & 64);
  z = CellTarget_CellWithBit(leader->z, table->z & 64);
  Func_02001b94();
  Event_PlaySound(161);
  MapGrid_CopyCells(table->x, table->z + 1, 1, 1, x, z);
  FieldEvent_Wait(12);
}
