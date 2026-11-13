/// @file
/// Scene code of overlay 0x6ad, in ROM order.

#include "../shared/teleporter.h"
#include "types.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"
#include "field.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/scene.h"

void Actor_StartScript(struct Actor *object, const void *script);
extern const s32 *gData_02007058[];
void Func_020035b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldEffect *party = (struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_02005480(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect, gData_02007058[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags(effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
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
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gData_02007058[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Func_02005480(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_TABLE gData_020070a8
#define CELL_SPRITE_DRAW Func_02003780
#define CELL_PUSH_SLOT_ENTRY Func_02005440
#include "../../field/map/cell_sprite.c"

#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "cell_target.h"

s32 Func_020038ac(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gData_020070a8;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *rows;
  struct CellMapCell *cell;
  s32 x0, z0, width, height, i, j;
  u32 *buffer;
  u32 event, last;
  s32 cz, cx;

  Iwram_ClearWordsEntry(table, 4);
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
    /* FAKEMATCH(dead): an always-false entry into the row loop's tail; as no
     * entry, 73 of 268 instructions differ in tla-en. */
    if (!((height << 1) & 1))
      goto row;
    j = height;
    goto rowNext;
  row:
    cell = rows + (j << 7);
    i = 0;
    if (i < width) {
      /* FAKEMATCH(dead): an always-false entry into the column loop's tail; as
       * no entry, 85 of 268 instructions differ in tla-en. */
      if (!((width << 1) & 1))
        goto column;
      i = width;
      goto columnNext;
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
      /* FAKEMATCH(dead): an always-false entry into the leader loop's tail; as
       * no entry, 236 of 268 instructions differ in tla-en. */
      if (!((leaderX << 1) & 1))
        goto leaderRow;
      k = leaderX;
      goto leaderNext;
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

    /* FAKEMATCH(dead): an always-false entry into the fill loop's tail; as no
     * entry, 10 of 268 instructions differ in tla-en. */
    if (!(((s32)buffer << 1) & 1))
      goto fill;
    count = (s32)buffer;
    goto fillNext;
  fill:
    *word++ = 0x11111111;
  fillNext:
    if (--count >= 0)
      goto fill;
  }
  table->tile = Resource_FindFreeEntry();
  Vram_Load(table->tile, 0x100, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02003780, 0x480);
  return table->count;
}

#include "scene_tables.h"

DEFINE_LEADER_CENTER_IN_CELL(Func_02003af0)

/// Map_CopyCellsRect through this overlay's import.

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Func_02003f18 calls it the first time the leader steps on
/// one of the table's cells.
DEFINE_TELEPORTER_UNCOVER(Func_02003b78, Func_02003af0)

#include "../shared/jupiter_lighthouse_room_3.h"
#include "io_reg.h"

DEFINE_TELEPORTER_SINK_EVENT(Func_02003be8, Func_02003af0)
