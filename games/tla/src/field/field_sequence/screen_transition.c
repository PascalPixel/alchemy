/// @file
/// The field's screen transitions and palette fades, as The Broken Seal's
/// field_sequence.c ends them, after the particle emitters and the item raised
/// above the leader; then the fade overlay and the routines after it.
// Part of field_sequence.c.

#include "types.h"
#include "actor.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/objdisp.h"
#include "field_event.h"
#include "system/ram_buffer.h"
#include "system/heap_release.h"
#include "fixed_math.h"
#include "actor_view.h"
#include "field_sprite.h"
#include "flag_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "graphics/display_transition.h"
#include "system/iwram_call.h"
#include "map.h"
#include "system/frame.h"
#include "system/task.h"
#include "system/io_write_queue.h"
#include "system/irq.h"
#include "graphics/palette_buffers.h"
#include "debug.h"
#include "field/sprite.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "system/io_write.h"
#include "system/main_routine_work.h"
#include "macros.h"
#include "event_work.h"
#include "system/input.h"

// The parts, in ROM order.
#include "module/system/system.h"

/* VIEW(f0cf050): routine Vector_AddPolarOffset as void
 * Vector_AddPolarOffset(s32 distance, u16 angle, s32 *position);
 * module/system/system.h has void Vector_AddPolarOffset(s32 radius, s32 angle,
 * s32 *position); tried as the definition's void Vector_AddPolarOffset(s32
 * radius, s32 angle, s32 *position), which the object already declares,
 * Leader_StepAwayFromNearbyActor moves: 3 of 55 instructions in tla-ja. */
extern void
Vector_AddPolarOffset__f0cf050(s32 distance, u16 angle,
                               s32 *position) __asm__("Vector_AddPolarOffset");

/// Unused: only the far-call table refers to it. Finds the first of actors 64
/// to 79 within 16 pixels of the leader on both axes and puts the leader 20
/// pixels from it, on the line from it to the leader, with no destination.
void Leader_StepAwayFromNearbyActor(void) {
  struct Actor *leader;
  struct Actor *other;
  s32 dx;
  s32 dz;
  s32 i;

  leader = ObjectTable_Get(Party_GetLeaderActor());
  for (i = 0; i <= 15; i++) {
    other = ObjectTable_Get(i + 64);
    dx = leader->x - other->x;
    dz = leader->z - other->z;
    if (dx > -0x100000 && dx < 0x100000)
      if (dz > -0x100000 && dz < 0x100000) {
        leader->x = other->x;
        leader->z = other->z;
        Vector_AddPolarOffset__f0cf050(0x140000, ArcTan2(dz, dx), &leader->x);
        leader->targetX = 0x80000000;
        leader->targetY = 0x80000000;
        leader->targetZ = 0x80000000;
        break;
      }
  }
}
#include "module/field/screen_transition.h"
#include "module/field/field.h"
#include "module/field/object.h"

void ItemDisplay_EmitSparkles(struct ParticleEmitter *emitter) {
  struct ParticlePosition position;
  struct ParticleEffectObject *object;
  u32 randomAngle;

  if (emitter->travelOffset >= -255 && emitter->travelOffset <= 255)
    emitter->active = 0;

  if ((100 * Random16() >> 16) > 9)
    return;

  position.x = emitter->position.x;
  position.y = emitter->position.y;
  position.z = emitter->position.z;
  randomAngle = Random16();
  Vector_AddPolarOffset(randomAngle << 4, Random16(), &position);
  object = Actor_Spawn(0x2a1, position.x, position.y, position.z);
  if (object != 0) {
    s32 mask;
    u8 flags;

    Actor_StartScriptFar((struct Actor *)object,
                         (u32)gBattleEffect_ParticleScript);
    Actor_SelectAnimationFar(object, 0);
    mask = 13;
    flags = object->child->flags;
    mask = -mask;
    mask &= flags;
    mask |= 4;
    object->child->flags = mask;
  }
}

/// Returns 1 while Reveal is active, else 0.
// As ☀️'s Field_IsRevealActive.
s32 Field_IsRevealActive(struct ScriptInterpreter *interpreter) {
  s32 revealState;
  u32 isActive = 0;
  struct RevealWork *work = Ram_HeapSlots->eventWork;

  if (work != NULL) {
    revealState = work->revealActive;
    isActive = (0 - revealState) | revealState;
    isActive >>= 31;
  }
  return isActive;
}

struct Actor *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z);
/* VIEW(emitter): routine Reveal_EmitSparkles as void Reveal_EmitSparkles(void);
 * the definition has void Reveal_EmitSparkles(const struct ParticleSource
 * *source); not tried: Reveal_EmitSparkles's bound declaration was not found.
 */
extern void BattleEffect_SpawnRandomParticleAtPosition__emitter(void) __asm__(
    "Reveal_EmitSparkles");

/// Shows the picture of item above actor (in front of the controlled owner,
/// on the centre of the cell), clearing Reveal's sparkles first.
struct Actor *BattleEffect_StartRandomParticleEmitter(s32 actor, s32 item) {
  struct EmitterWork *work = Ram_HeapSlots->eventWork;
  struct Actor *picture = work->actors[actor];
  struct Actor *object;
  struct EffectVisual *visual;
  s32 position[3];
  u8 *buffer;
  s32 tile;
  s32 i;
  s32 zero;
  s32 mask;

  if (picture == NULL)
    return NULL;
  position[0] = picture->x;
  position[1] = picture->y;
  position[2] = picture->z;
  if (actor == Party_GetLeaderActor()) {
    Vector_AddPolarOffset(0x100000, picture->facing, position);
    position[0] = (position[0] & 0xfff00000) + 0x80000;
    position[2] = (position[2] & 0xfff00000) + 0x80000;
  }
  if (work->revealActive != 0) {
    object = Ram_HeapSlots->scriptObjects;
    i = 63;
    do {
      if (object->script != NULL) {
        if (object->update ==
            BattleEffect_SpawnRandomParticleAtPosition__emitter)
          Actor_ReleaseFar(object);
        if (object->script == (s32 *)gBattleEffect_ParticleScript)
          Actor_ReleaseFar(object);
      }
      i--;
      object++;
    } while (i >= 0);
  }
  wait(3);
  picture =
      Actor_CreateFar(489, position[0], position[1] + 0x100000, position[2]);
  if (picture == NULL)
    return NULL;
  Actor_StartScriptFar(picture, gFieldEffect_ParticleEmitterScript);
  visual = picture->sprite;
  zero = 0;
  visual->value1a = zero;
  visual->value1b = zero;
  visual->flags05 &= zero - 33;
  mask = ((u8 *)visual)[9] & 0x0f;
  {
    s32 clear = 13;

    clear = -clear;
    mask &= clear;
  }
  mask |= 4;
  ((u8 *)visual)[9] = mask;
  picture->motionFlags = 2;
  picture->terrainHeight = position[1];
  picture->velocityY = px(2);
  picture->gravity = px(1) / 4;
  buffer = HeapSlot_Alloc(68, 0x608);
  Icon_DrawItemFar(item);
  tile = VramCache_Load(visual->tileSlot, 128, buffer + 0x400);
  visual->tile = tile;
  HeapSlot_Release(68);
  picture->update = ItemDisplay_EmitSparkles;
  if (work->revealActive != 0) {
    struct Actor *marker = Ram_HeapSlots->scriptObjects;
    s32 j;

    for (j = 0; j <= 63; j++, marker++) {
      if (marker->script != NULL &&
          marker->script == (s32 *)gBattleEffect_MarkerParticleScript &&
          marker->x >> 20 == picture->x >> 20 &&
          marker->z >> 20 == picture->z >> 20) {
        Actor_ReleaseFar(marker);
        break;
      }
    }
  }
  return picture;
}

void Actor_ReleaseIfPresent(struct Actor *object) {
  if (object != 0)
    Actor_ReleaseFar(object);
}

/// Does nothing; nothing in the image calls it.
void ScreenTransition_ReservedNoOp(void) {}

/// Places the object 36 pixels above the controlled owner, gives it its
/// sprite priority and, three frames on, the descent script, and turns the
/// owner to face a quarter turn.
void ItemDisplay_RaiseAboveLeader(struct Actor *actor) {
  struct Actor *owner;

  if (actor == 0)
    return;

  {
    struct Actor *found = ObjectTable_Get(Party_GetLeaderActor());
    actor->acceleration = Q16_ONE;
    actor->speedLimit = px(2);
    actor->motionFlags = 0;
    owner = found;
  }
  Actor_SetMoveTargetFar(actor, owner->x, owner->y + 0x240000, owner->z);
  Actor_SetSpritePriorityDirectFar(actor, 1);
  wait(3);
  Actor_SelectAnimationFar(owner, 28);
  Actor_StartScriptFar(actor, gFieldEffect_RisingPulseScript);
  owner->facing = TRIG_TURN / 4;
}

void ItemDisplay_ShowPicture(struct Actor *object, s32 flags) {
  struct Actor *owner;

  if (object != 0) {
    owner = ObjectTable_Get(Party_GetLeaderActor());
    if (flags & 1) {
      Actor_SetSpriteFlagsFar(object, 0);
      Actor_StartScriptFar(object, gFieldEffect_ParticleEmitterScript);
      object->velocityY = px(2);
      object->gravity = px(1) / 4;
      object->update = ItemDisplay_EmitSparkles;
    }
    if (flags == 3)
      wait(60);
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader(object);
    if (flags == 3)
      wait(80);
    Actor_SelectAnimationFar(owner, 1);
  }
}

/* VIEW(rising): tag struct EffectVisual as struct EffectVisual__rising;
 * f0cf050.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/field/field_sequence.c:4492: structure has no member named
 * `flags09'. */
struct EffectVisual__rising {
  u8 unk00[5];
  u8 flags05;
  u8 unk06[3];
  u8 flags09;
  u8 unk0a[6];
  u8 tileSlot;
  u8 unk11[9];
  u8 value1a;
  u8 value1b;
};

void ItemDisplay_ShowItem(s32 item, s32 flags) {
  struct Actor *owner = ObjectTable_Get(Party_GetLeaderActor());
  u8 *buffer = HeapSlot_Alloc(68, 0x608);
  struct Actor *object =
      Actor_CreateFar(489, owner->x, owner->y + 0x240000, owner->z);

  if (object != 0) {
    struct EffectVisual__rising *visual = object->sprite;

    s32 mask;
    s32 zero = 0;

    visual->value1a = zero;
    visual->value1b = zero;
    visual->flags05 &= zero - 33;

    mask = visual->flags09 & 0x0f;
    {
      s32 clear = 13;

      clear = -clear;
      mask &= clear;
    }
    mask |= 4;
    visual->flags09 = mask;

    Icon_DrawItemFar(item);
    VramCache_Load(visual->tileSlot, 128, buffer + 0x400);
    HeapSlot_Release(68);

    if (flags & 1)
      object->update = ItemDisplay_EmitSparkles;
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader(object);

    wait(80);
    Actor_SelectAnimationFar(owner, 1);
    Actor_ReleaseFar(object);
  }
}

void Reveal_EmitSparkles(const struct ParticleSource *source) {
  struct ParticlePosition position;
  struct ParticleEffectObject *object;
  u32 random;

  if ((100 * Random16() >> 16) > 9)
    return;

  position.x = source->position.x;
  position.y = source->position.y;
  position.z = source->position.z;
  random = Random16();
  Vector_AddPolarOffset(random << 4, Random16(), &position);
  object = Actor_Spawn(0x2a1, position.x, position.y, position.z);
  if (object != 0) {
    s32 mask;
    u8 flags;

    Actor_StartScriptFar((struct Actor *)object,
                         (u32)gBattleEffect_ParticleScript);
    Actor_SelectAnimationFar(object, 0);
    mask = 13;
    flags = object->child->flags;
    mask = -mask;
    mask &= flags;
    mask |= 4;
    object->child->flags = mask;
  }
}

// Places the scene's markers from its scene item rows (a type, an event id, a
// flag and a pad): for each of the first 16 rows whose flag is clear and whose
// event has not run, a marker object goes at its event object, or at the map
// cell of a tile event, when that point is on screen.

/// Places a marker at each waiting scene item event that is on screen.
void FieldEffect_SpawnRevealMarkers(void) {
  // The row type is s16, as the rows are: with it the copied head test stays
  // apart from the bottom one.
  s16 *entry = SceneItem_GetRows();
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0] & 0xffff0000;
  s32 originZ = origin[1] & 0xffff0000;
  s32 result[3];
  /* FAKEMATCH(volatile): headerWord lives at sp+4, stored once. */
  volatile s32 headerWord;
  struct Actor *object;
  s16 type;
  s32 id;
  s32 flag;
  s32 i;
  s32 x;
  s32 y;
  s32 z;
  s32 dx;
  s32 dz;

  // DEAD: the map header's second word is never read; the ROM keeps its load
  // and store.
  headerWord = (*(s32 **)map)[1];
  if (entry == NULL)
    return;
  for (i = 0; i <= 15 && (type = *entry++) != 0 && !(type & 0x4000); i++) {
    id = entry[0];
    flag = entry[1];
    entry += 3;
    if (type != 128 && flag != -1 && GameFlag_Get(flag))
      continue;
    if (SceneItem_FindPosition(id, result))
      continue;
    object = ObjectEvent_GetObject(i + 64);
    if (object == NULL)
      continue;
    // A tile event's object holds its map cell, marked by this value.
    if (object->unk4c == 0x31415927) {
      x = object->timer << 16;
      z = (s16)object->unk66 << 16;
      y = MapGrid_GetTerrainHeightFar(0, x, z);
    } else {
      if ((u16)(type - 128) <= 1)
        continue;
      if (type == 130 || type == 132 || type == 133 || type == 131)
        continue;
      x = object->x;
      y = object->y;
      z = object->z;
    }
    dx = x - originX;
    dz = z - originZ - y;
    dx >>= 16;
    dz >>= 16;
    if ((u32)(dx + 15) > 270 || (u32)(dz + 47) > 238)
      continue;
    if (flag == -1 || GameFlag_Get(flag))
      continue;
    object = Actor_CreateFar(673, x, y, z);
    if (object != NULL) {
      object->motionFlags = 0;
      Actor_StartScriptFar((struct Actor *)object,
                           gBattleEffect_MarkerParticleScript);
      Actor_SetSpriteFlagsFar((struct Actor *)object, 0);
      Actor_SelectAnimationFar(object, 1);
      ((struct FieldSprite *)object->sprite)->priority = 1;
    }
  }
}

/// Each frame, before drawing: turns windows 0 and 1 on, writes the first four
/// values of the shown page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0
/// copy the rest into WIN0H and WIN1H one word each HBlank.
// As ☀️'s FieldEffect_StartWindowHBlankDma does.
void FieldEffect_StartWindowHBlankDma(void) {
  struct ScreenTransitionState *work;
  u16 *src;
  volatile u16 *channel;
  s32 in;
  s32 out;
  s32 other;

  work = Ram_HeapSlots->screenTransition;
  src = work->pages[work->page];
  channel = REG_DMA0;
  // Stop the previous transfer before priming the first line's registers.
  channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  channel[5] &= ~DMA_ENABLE;
  (void)channel[5];
  REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
  /* FAKEMATCH(dead): the load in a once-only block comes before the register
   * address is built. */
  do {
    in = *src;
  } while (0);
  REG_WININ = in;
  src++;
  /* FAKEMATCH(temporary): WINOUT's value read into out before the register
   * address is built. */
  out = *src;
  REG_WINOUT = out;
  src++;
  REG_WIN0H = *src;
  src++;
  /* FAKEMATCH(temporary): WIN1H's value read into other before the register
   * address is built. */
  other = *src;
  REG_WIN1H = other;
  src++;
  // other is reused for both vertical ranges; WIN_RANGE(0, 160) at each
  // store differs.
  other = WIN_RANGE(0, 160);
  REG_WIN0V = other;
  REG_WIN1V = other;
  // HBlank repeats one word: both horizontal window bounds for the next line.
  Dma_Set(
      src, (void *)&REG_WIN0H,
      (DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_32BIT | DMA_DEST_RELOAD)
              << 16 |
          1,
      (volatile u32 *)channel);
}
#include "field/event.h"
#include "gamepak/far_call/window.h"

void ScreenTransition_UpdateScanlineTable(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ScreenTransitionState *state = Ram_HeapSlots->screenTransition;
  struct TransitionSpan *span;
  struct TransitionShape *shape;
  u16 *scanlineData;
  s32 *viewInverse;
  s32 *viewRipple;
  s32 *viewLeader;
  s32 *viewLeaderLow;
  struct Actor *object;
  u32 i;
  s32 k;
  s32 lineCount;
  s32 value;
  s32 radiusSquaredQ16;
  s32 delta;
  u32 horizontalExtent;
  s32 left;
  s32 right;
  s32 centerX;
  s32 centerY;
  s32 cameraY;
  s32 cameraX;
  s32 cameraY8;
  u32 leftEdge;
  s32 oppositeEdges;

  if (state->duration != 0) {
    if (state->step >= state->duration) {
      state->duration = 0;
      if (state->nextMode == 0) {
        if (state->end == 64)
          REG_DISPCNT &= 0x81ff;
        else
          REG_DISPCNT &= 0x9fff;
        if (REG_DISPCNT & 0x8000)
          REG_WINOUT |= 0x1f;
        Task_Remove(FieldEffect_StartWindowHBlankDma);
        Task_Remove(ScreenTransition_UpdateScanlineTable);
        {
          volatile u16 *dma0 = (volatile u16 *)0x040000b0;

          dma0[5] &= ~0x3a00;
          dma0[5] &= ~0x8000;
          dma0[5];
        }
        return;
      }
      state->mode = state->nextMode;
    } else {
      state->value =
          state->start +
          Iwram_SignedDivide((state->end - state->start) * ++state->step,
                             state->duration);
    }
  }

  scanlineData = state->pages[1 ^ state->page];
  ScanlineWindow_FillSpansFar(scanlineData + 2);

  switch (state->mode) {
  case 0:
    *scanlineData++ = 0x7f7f;
    *scanlineData++ = 1;
    value = state->value;
    if (value & 32)
      value = 32 - (value & 31);
    else
      value &= 31;
    value = gScreenTransition_NoiseWidths[value];
    for (i = 0; i < 160; i++) {
      leftEdge = (Random16() * (241 - value)) >> 16;
      scanlineData[0] = (leftEdge << 8) | (leftEdge + value);
      scanlineData += 2;
    }
    break;

  case 1:
    value = state->value;
    shape = &gScreenTransition_Shapes[value & 31];
    if (value & 32) {
      *scanlineData++ = shape->first;
      *scanlineData++ = shape->second;
    } else {
      *scanlineData++ = shape->second;
      *scanlineData++ = shape->first;
    }
    for (i = 0; i < 3; i++) {
      span = &shape->spans[i];
      lineCount = span->count;
      right = span->value;
      if (lineCount != 0) {
        if (right != 0) {
          for (k = 0; k < lineCount; k++) {
            scanlineData[0] = right;
            scanlineData += 2;
          }
        } else {
          radiusSquaredQ16 = span->x0;
          left = span->y0;
          delta = span->x1;
          right = span->y1;
          for (k = 0; k < lineCount; k++) {
            scanlineData[0] =
                ((radiusSquaredQ16 + (delta - radiusSquaredQ16) * k / lineCount)
                 << 8) +
                (left + (right - left) * k / lineCount);
            scanlineData += 2;
          }
        }
      }
    }
    break;

  case 2:
    value = state->value - 1;
    if (value & 32) {
      *scanlineData++ = 1;
      *scanlineData++ = 0x7f7f;
    } else {
      *scanlineData++ = 0x7f7f;
      *scanlineData++ = 1;
    }
    value &= 31;
    for (i = 0; i < 160; i++) {
      horizontalExtent = (value << 4) + ((16 * Random16()) >> 16);
      if (horizontalExtent > 255)
        horizontalExtent = 255;
      scanlineData[0] = horizontalExtent;
      scanlineData += 2;
    }
    break;

  case 3:
    value = state->value;
    *scanlineData++ = 0x7f7f;
    *scanlineData++ = 1;
    if ((u32)value > 32) {
      *scanlineData++ = 1;
      *scanlineData++ = 0x7f7f;
      value = 64 - value;
    }
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - 80;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value) << 16)) >> 8;
      left = 120 - horizontalExtent;
      right = horizontalExtent + 120;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > 240)
        left = 240;
      if (right > 240)
        right = 240;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;

  case 4:
    value = state->value;
    if (value & 32) {
      *scanlineData++ = 1;
      *scanlineData++ = 0x7f7f;
    } else {
      *scanlineData++ = 0x7f7f;
      *scanlineData++ = 1;
    }
    value = (u32)(240 * (value & 31)) >> 5;
    for (i = 0; i < 160; i += 2) {
      oppositeEdges = ((240 - value) << 8) + 240;
      scanlineData[0] = value;
      scanlineData[2] = oppositeEdges;
      scanlineData += 4;
    }
    break;

  case 7: {
    s32 ax;

    viewInverse = &map->viewX;
    ax = viewInverse[0] & 0xffff0000;
    cameraY = viewInverse[1] & 0xffff0000;
    object = ObjectTable_Get(Party_GetLeaderActor());
    centerX = (object->x - ax) / Q16_ONE;
    centerY = ((object->z - object->y) - cameraY) / Q16_ONE - 16;
    *scanlineData++ = state->active;
    *scanlineData++ = state->mask;
    value = state->value;
    if ((value & 32) == 0)
      value = 32 - (value & 31);
    else
      value &= 31;
    if (gFrameCount & 1)
      value = 0;
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > 240)
        left = 240;
      if (right > 240)
        right = 240;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;
  }

  case 8:
    viewRipple = &map->viewX;
    cameraX = viewRipple[0] & 0xffff0000;
    cameraY8 = viewRipple[1] & 0xffff0000;
    centerX = (state->centreX - cameraX) / Q16_ONE;
    centerY = (state->centreY - cameraY8) / Q16_ONE - 16;
    k = -centerY * 2 + gFrameCount;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & 32)
      value = 32 - (value & 31);
    else
      value &= 31;
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      delta = i - centerY;
      if (delta < -120)
        delta = -120;
      if (delta > 120)
        delta = 120;
      delta *= delta;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((delta * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < right) {
        value = k & 31;
        delta = gScreenTransition_EdgeOffsets[value];
        left -= delta;
        right += delta;
        if (left >= right) {
          left = 240;
          right = 240;
        }
      }
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > 240)
        left = 240;
      if (right > 240)
        right = 240;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
      k += 2;
    }
    break;

  case 9: {
    s32 ax;

    viewLeader = &map->viewX;
    ax = viewLeader[0] & 0xffff0000;
    cameraY = viewLeader[1] & 0xffff0000;
    object = ObjectTable_Get(Party_GetLeaderActor());
    centerX = (object->x - ax) / Q16_ONE;
    centerY = ((object->z - object->y) - cameraY) / Q16_ONE - 16;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & 32)
      value = 32 - (value & 31);
    else
      value &= 31;
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > 240)
        left = 240;
      if (right > 240)
        right = 240;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;
  }

  case 77: {
    s32 ax;

    viewLeaderLow = &map->viewX;
    ax = viewLeaderLow[0] & 0xffff0000;
    cameraY = viewLeaderLow[1] & 0xffff0000;
    object = ObjectTable_Get(Party_GetLeaderActor());
    centerX = (object->x - ax) / Q16_ONE;
    centerY = ((object->z - object->y) - cameraY) / Q16_ONE - 8;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & 32)
      value = 32 - (value & 31);
    else
      value &= 31;
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 14)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > 240)
        left = 240;
      if (right > 240)
        right = 240;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;
  }
  }

  state->page ^= 1;
}

void ScreenTransition_InitializeBattleEffectState(s32 mode) {
  struct ScreenTransitionState *state =
      HeapSlot_AllocEwram(124, sizeof(*state));
  volatile u32 zero;

  zero = 0;
  Dma_Set((const void *)&zero, state,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*state) / 4),
          REG_DMA3);
  state->mode = mode;
  state->value = 0;
  state->mask = 0x3f3f;
  state->active = (REG_WINOUT & 0xff00) | 1;
  Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_VBLANK);
  Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
}

/// Unused: suspends the window transition's two tasks. Nothing calls it,
/// by name, far-call entry or table.
UNUSED void ScreenTransition_SuspendCallbacks(void) {
  Task_SuspendAll(ScreenTransition_UpdateScanlineTable);
  Task_SuspendAll(FieldEffect_StartWindowHBlankDma);
}

/// Allocates the screen transition state (heap slot 124) and clears it.
struct ScreenTransitionState *ScreenTransition_AllocateAndClearState(void) {
  struct ScreenTransitionState *state =
      HeapSlot_AllocEwram(124, sizeof(*state));
  volatile u32 zero = 0;

  Dma_Set(&zero, state,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*state) / 4),
          REG_DMA3);
  return state;
}

// Fades the screen in over frames frames, as ☀️'s ScreenTransition_Start.
void ScreenTransition_Start(s32 mode, s32 frames) {
  struct MapViewWork *display;
  s32 variant;
  s32 transitionKind;

  display = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  transitionKind = (mode >> 8) & 0xff;
  variant = mode & 0xff;
  switch (transitionKind) {
  case 0:
    ScreenFade_ToBlack(0);
    ScreenFade_FromBlack(frames);
    wait(1);
    break;
  case 1:
    BattleEffect_ApplyColorToSourceBuffer(0x8000, PLTT_BACKDROP);
    PaletteFade_StartInterpolation(frames);
    wait(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    BattleEffect_SetPrimaryBufferValue(0);
    return;
  case 2: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 0;
    state->mask = 63;
    state->active = (REG_WINOUT & 0xff00) | 1;
    Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_VBLANK);
    Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
    wait(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 3: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    ScreenTransition_FillTilemapAndSolidTile(15);
    wait(1);
    Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_VBLANK);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 4: {
    struct ScreenTransitionState *state;

    display = (struct MapViewWork *)Ram_HeapSlots->mapWork;
    state = ScreenTransition_AllocateAndClearState();
    display->windowTop = 80;
    display->windowBottom = 80;
    wait(1);
    if (variant == 0) {
      Task_Add(ScreenTransition_Update, TASK_PASS_VBLANK);
      Irq_SetHandler(1, 0, ScreenTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    } else {
      Task_Add(ScreenTransition_UpdateFromCentre, TASK_PASS_VBLANK);
      Irq_SetHandler(1, 0, ScreenTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                      0x20000);
}

/// Fades the screen out over frames frames, as The Broken Seal's
/// ScreenTransition_Finish does, with this game's callbacks and priorities.
void ScreenTransition_Finish(s32 mode, s32 frames) {
  s32 variant;
  s32 transitionKind;

  transitionKind = (mode >> 8) & 0xff;
  variant = mode & 0xff;
  switch (transitionKind) {
  case 0:
    ScreenFade_FromBlack(0);
    ScreenFade_ToBlack(frames);
    break;
  case 1:
    BattleEffect_ApplyColorToTargetBuffer(0x8000, 0);
    PaletteFade_StartInterpolation(frames);
    break;
  case 2: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    state->mask = 63;
    state->active = (REG_WINOUT & 0xff00) | 1;
    Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_VBLANK);
    Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
    wait(1);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 3: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    ScreenTransition_FillTilemapAndSolidTile(0);
    wait(1);
    Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_VBLANK);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 4: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    // One zero for both branches' stores; a literal 0 at each store differs.
    s32 zero = 0;

    ((u8 *)state)[0x53f] = 1;
    if (variant == 0) {
      Task_Add(ScreenTransition_Update, TASK_PASS_VBLANK);
      Irq_SetHandler(1, 0, ScreenTransition_UpdateScanline);
      state->start = zero;
      state->end = 80;
      state->duration = frames;
      state->step = zero;
    } else {
      Task_Add(ScreenTransition_UpdateFromCentre, TASK_PASS_VBLANK);
      Irq_SetHandler(1, 0, ScreenTransition_UpdateScanline);
      state->start = zero;
      state->end = 80;
      state->duration = frames;
      state->step = zero;
    }
    break;
  }
  }
}

/// Returns 1 while ScreenTransition_UpdateScanline is the VCount handler.
s32 ScreenTransition_IsScanlineActive(void) {
  s32 other = (s32)gIrqHandlers[1] ^ (s32)ScreenTransition_UpdateScanline;

  return 1 - ((u32)(-other | other) >> 31);
}

/// The layer flags read through a volatile pointer, which keeps them the AND's
/// second operand and builds the mask in the result register; as map->flags,
/// 24 of 31 instructions of ScreenTransition_HideBackgrounds differ in tla-en.
#define MAP_LAYER_FLAGS(map) (*(volatile u16 *)&(map)->flags)

/// Clears the flag of each map layer whose argument is nonzero.
void ScreenTransition_HideBackgrounds(s32 layer0, s32 layer1, s32 layer2) {
  struct MapLayerFlags *map;
  u16 flags;

  map = (struct MapLayerFlags *)Ram_HeapSlots->mapWork;
  if (map == 0)
    return;
  if (layer2) {
    flags = MAP_LAYER_FLAGS(map);
    map->flags = flags & ~0x200;
  }
  if (layer1) {
    flags = MAP_LAYER_FLAGS(map);
    map->flags = flags & ~0x400;
  }
  if (layer0) {
    flags = MAP_LAYER_FLAGS(map);
    map->flags = flags & ~0x800;
  }
}

/// Steps the transition's value toward its end and opens the window from the
/// top and bottom edges by that many lines.
void ScreenTransition_Update(void) {
  struct ScreenTransitionState *state;
  struct MapViewWork *window;
  s8 *duration;
  u32 value;

  state = Ram_HeapSlots->screenTransition;
  duration = &state->duration;
  window = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Task_Remove(ScreenTransition_Update);
      Irq_SetHandler(1, 0, 0);
      window->extraTiles = 3;
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 offset;

      (*step)++;
      offset = Iwram_SignedDivide(delta * *step, *duration);
      state->value = state->start + offset;
    }
  }

  value = state->value;
  if (value > 79) {
    window->windowTop = 200;
    window->windowBottom = 250;
  } else {
    window->windowTop = value;
    window->windowBottom = 159 - value;
  }
}

/// As ScreenTransition_Update, but the window opens from the centre, on
/// alternate frames only.
void ScreenTransition_UpdateFromCentre(void) {
  struct ScreenTransitionState *state;
  struct MapViewWork *window;
  s8 *duration;
  u16 value;

  state = Ram_HeapSlots->screenTransition;
  duration = &state->duration;
  window = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Task_Remove(ScreenTransition_UpdateFromCentre);
      Irq_SetHandler(1, 0, 0);
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 offset;

      (*step)++;
      offset = Iwram_SignedDivide(delta * *step, *duration);
      state->value = state->start + offset;
    }
  }

  value = state->value;
  if (value > 79) {
    window->windowTop = 200;
    window->windowBottom = 250;
  } else if (value != 0 && (gFrameCount & 1)) {
    window->windowTop = value + 80;
    window->windowBottom = 80 - value;
  } else {
    window->windowTop = 0;
    window->windowBottom = 159;
  }
}

/// The VCount interrupt: switches the display to mode 2 at the window's first
/// line and back to mode 0 at its second.
void ScreenTransition_UpdateScanline(void) {
  u32 line;
  struct MapViewWork *state;

  line = REG_VCOUNT;
  state = (struct MapViewWork *)Ram_HeapSlots->mapWork;

again:
  switch (state->irqPhase) {
  case 3:
    if (line >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & 0xFFF8) | 2;
      state->irqPhase = 9;
    }
    break;
  case 2:
    if (line >= state->irqSecondLine) {
      REG_DISPCNT &= 0xFFF8;
      state->irqPhase = 9;
    }
    break;
  case 1:
    if (line >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & 0xFFF8) | 2;
      state->irqPhase++;
      goto again;
    }
    if (line >= state->irqSecondLine) {
      REG_DISPCNT &= 0xFFF8;
      state->irqPhase = 3;
      goto again;
    }
    break;
  case 0:
    if (line <= 158) {
      state->irqPhase = 1;
      goto again;
    }
    break;
  }
}

/// The Broken Seal's ScreenTransition_UpdateDissolve: steps the transition's
/// value toward its end each frame, turning DMA0 off when it arrives, then
/// sets the two pixel nibbles the value selects and queues the tile to
/// character block 0.
void ScreenTransition_UpdateDissolve(void) {
  struct ScreenTransitionDissolveState *state;
  s32 value;
  s32 blend;
  u32 i;

  state =
      (struct ScreenTransitionDissolveState *)Ram_HeapSlots->screenTransition;
  if (state->duration != 0) {
    if (state->step >= state->duration) {
      volatile u16 *dma0;
      u16 control;

      state->duration = 0;
      Task_Remove(ScreenTransition_UpdateDissolve);
      dma0 = REG_DMA0;
      control = dma0[5];
      dma0[5] = control & 0xc5ff;
      dma0[5] &= 0x7fff;
      (void)dma0[5];
      return;
    } else {
      s32 delta = state->end - state->start;

      state->step++;
      state->value = state->start +
                     Iwram_SignedDivide(delta * state->step, state->duration);
    }
  }

  value = state->value - 1;
  state->ditherToggle ^= 1;
  blend = 0;
  if (value & 32)
    blend = 15;
  value = (value & 31) * 2;
  for (i = 0; i <= 1; i++) {
    u8 entry = gScreenTransition_DitherTable[value & 63];
    u8 *p = &state->paletteNibbles[entry >> 1];

    if (entry & 1)
      *p = (*p & 0x0f) | (blend << 4);
    else
      *p = (*p & 0xf0) | blend;
    value++;
  }

  IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->paletteNibbles,
                      DMA_COPY_WORDS(8));
}

/// The Broken Seal's ScreenTransition_FillTilemapAndSolidTile: blanks the
/// screen block at 0x06002000 and, unless color is -1, fills the first tile
/// with that colour.
void ScreenTransition_FillTilemapAndSolidTile(s32 color) {
  struct ScreenTransitionState *work = Ram_HeapSlots->screenTransition;
  Dma_Fill32(0xf000f000, (void *)BG_SCREEN_BLOCK(4), 0x500);
  if (color != -1) {
    u32 pattern = 0;
    s32 cnt;
    u32 *tile;
    for (cnt = 7; cnt >= 0; --cnt)
      pattern = (pattern << 4) | color;
    tile = work->solidTile;
    for (cnt = 7; cnt >= 0; --cnt)
      *tile++ = pattern;
    Dma_Set(work->solidTile, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(8),
            REG_DMA3);
  }
}

void ScreenTransition_InitializeState(s32 value) {
  struct ScreenTransitionState *state;

  state = HeapSlot_AllocEwram(0x7c, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  ScreenTransition_FillTilemapAndSolidTile(0);
  state->mode = value;
  state->value = 0;
  Task_Add((s32)(ScreenTransition_UpdateDissolve), TASK_PASS_VBLANK);
  wait(0x78);
}
#include "field/ground_mask.h"

void BattleEffect_InterpolateBuffers(s16 *source, s16 *target, s16 *step,
                                     s32 frames) {
  s32 index;
  s32 first;
  s32 second;
  s32 (*divide)(s32, s32);

  if (frames > 0) {
    divide = Iwram_SignedDivide;
    index = 0x53F;
    do {
      first = *source;
      second = *target;
      *step = divide(second - first, frames);
      index--;
      source++;
      target++;
      step++;
    } while (index >= 0);
  }
}

void BattlePalette_UpdateBlend(void) {
  struct BattleEffectBuffers *work = Ram_HeapSlots->block32;
  u16 *add = work->delta;
  s32 i;

  if (GameFlag_Get(0x152) != 0) {
    return;
  }
  if (work->duration == 0) {
    return;
  }
  if (++work->step < work->duration) {
    u16 *sum = work->current;
    for (i = 0; i < 0x540; i++) {
      *sum++ += *add++;
    }
  } else {
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy(work->current, work->target, sizeof(work->current));
    work->duration = 0;
  }

  {
    u16 *packed = work->packed[1 ^ work->page];
    s32 blue = 0x7c00;
    u16 *current;

    i = 0x1c0;
    current = work->current;
    for (; i != 0; i--) {
      *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) |
                  (((s16)current[2] >> 10) & 0x1f);
      current += 3;
    }
  }

  work->page ^= 1;
  {
    u16 *packed = work->packed[work->page];

    IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)packed, DMA_COPY_WORDS(112));
    IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(packed + 224),
                        DMA_COPY_WORDS(112));
  }
}

#define COLOR_MAX 448
#define COLOR_HALF 224

#define gColorBuffer_ChannelCurve Data_080f0246
#define gColorBuffer_ChannelCurveLight Data_080f0286
#define gColorBuffer_ChannelCurveContrast Data_080f02c6

void BattleEffect_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 half) {
  u32 i;
  u32 colorCount = COLOR_MAX;
  s32 red;
  s32 blue;
  u32 color;
  s32 green;

  if (mode == PALETTE_TINT_FROM_SOURCE)
    mode = *(u16 *)BG_PLTT;
  if (half == 1) {
    colorCount = COLOR_HALF;
  } else if (half == 2) {
    dst += COLOR_HALF * 3;
    src += COLOR_HALF;
    colorCount = COLOR_HALF;
  }

  // One colour: write it once, then let DMA copy it over the rest. The copy
  // reads three halfwords behind where it writes, so the colour repeats.
  if (mode < PALETTE_TINT_FROM_SOURCE) {
    *dst++ = mode & 0x7c00;
    *dst++ = (mode & 0x03e0) << 5;
    *dst++ = (mode & 0x001f) << 10;
    Dma_Set(dst - 3, dst, (((colorCount - 1) * 6) >> 1) | (DMA_ENABLE << 16),
            REG_DMA3);
  } else if (mode < PALETTE_TINT_FLAGS) {
    switch (mode) {
    case PALETTE_TINT_GRAYSCALE: // grey of brightness (2R + 4G + B) / 7
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = ((color << 11) & 0xf800);
        green = ((color << 7) & 0x1f000);
        blue = (color & 0x7c00);
        color = Iwram_SignedDivide(red + green + blue, 7);
        dst[i * 3 + 0] = color;
        dst[i * 3 + 1] = color;
        dst[i * 3 + 2] = color;
      }
      break;
    case PALETTE_TINT_DIM_GRAY: // a third of R + G + B, B three steps darker
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        color = Iwram_SignedDivide(
            red + ((color >> 5) & 31) + ((color >> 10) & 31), 3);
        red = color - 1;
        green = red;
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = color - 3;
        blue = PaletteFade_ClampComponent(blue);
        dst[0] = gColorBuffer_ChannelCurveLight[blue];
        dst[1] = gColorBuffer_ChannelCurveLight[green];
        dst[2] = gColorBuffer_ChannelCurveLight[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_SEPIA: // the brightness, red stronger, within 8 to
                             // 28
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        color = Iwram_SignedDivide(red + green + blue, 10);
        red = color * 4 + 5;
        blue = color * 3 + 5;
        green = blue;
        if (red < 8)
          red = 8;
        if (green < 8)
          green = 8;
        if (blue < 8)
          blue = 8;
        if (red > 28)
          red = 28;
        if (green > 28)
          green = 28;
        if (blue > 28)
          blue = 28;
        *dst++ = gColorBuffer_ChannelCurveLight[blue];
        *dst++ = gColorBuffer_ChannelCurveLight[green];
        *dst++ = gColorBuffer_ChannelCurveLight[red];
      }
      break;
    case PALETTE_TINT_WARM: // half R + 10, two thirds G + 8, B - 7 (black
                            // stays black)
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        if ((red | green | blue) != 0) {
          red -= (u32)red >> 1;
          green -= green / 3;
          red += 10;
          green += 8;
          blue -= 7;
          red = PaletteFade_ClampComponent(red);
          green = PaletteFade_ClampComponent(green);
          blue = PaletteFade_ClampComponent(blue);
        }
        *dst++ = gColorBuffer_ChannelCurveContrast[blue];
        *dst++ = gColorBuffer_ChannelCurveLight[green];
        *dst++ = gColorBuffer_ChannelCurve[red];
      }
      break;
    case PALETTE_TINT_MUTED: // narrow the range, G and B + 2, past the first
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        if (i != 0) {
          if (red < 8)
            red = 8;
          if (green < 14)
            green = 14;
          if (blue < 14)
            blue = 14;
          if (red > 29)
            red = 29;
          if (green > 25)
            green = 25;
          if (blue > 27)
            blue = 27;
          green += 2;
          blue += 2;
          red = PaletteFade_ClampComponent(red);
          green = PaletteFade_ClampComponent(green);
          blue = PaletteFade_ClampComponent(blue);
        }
        *dst++ = gColorBuffer_ChannelCurve[blue];
        *dst++ = gColorBuffer_ChannelCurve[green];
        *dst++ = gColorBuffer_ChannelCurve[red];
      }
      break;
    case PALETTE_TINT_PALE: // half of each, plus the mean
      for (i = 0; i < colorCount; i++) {
        s32 mean;

        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        mean = PaletteFade_ClampComponent((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        dst[0] = gColorBuffer_ChannelCurveContrast[blue];
        dst[1] = gColorBuffer_ChannelCurveContrast[green];
        dst[2] = gColorBuffer_ChannelCurveContrast[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_RED: // an eighth of G and B added to R; G and B two
                           // thirds
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        red += ((green >> 3) + (blue >> 3));
        red = PaletteFade_ClampComponent(red);
        green -= green / 3;
        blue -= blue / 3;
        dst[0] = gColorBuffer_ChannelCurve[blue];
        dst[1] = gColorBuffer_ChannelCurve[green];
        dst[2] = gColorBuffer_ChannelCurveLight[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_WARM_DARK: // half R + 6, two thirds G + 4, B - 6
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        red -= (u32)red >> 1;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        dst[0] = gColorBuffer_ChannelCurveContrast[blue];
        dst[1] = gColorBuffer_ChannelCurveLight[green];
        dst[2] = gColorBuffer_ChannelCurve[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_DIM_BRIGHT: // a fifth off each channel of a bright colour
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        if (red + green + blue > 60) {
          red -= red / 5;
          green -= green / 5;
          blue -= blue / 5;
        }
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        *dst++ = gColorBuffer_ChannelCurve[blue];
        *dst++ = gColorBuffer_ChannelCurve[green];
        *dst++ = gColorBuffer_ChannelCurve[red];
      }
      break;
    default: // unchanged
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        dst[i * 3 + 0] = color & 0x7c00;
        dst[i * 3 + 1] = (color & 0x03e0) << 5;
        dst[i * 3 + 2] = (color & 0x001f) << 10;
      }
      break;
    }
  } else if (mode & PALETTE_TINT_CUSTOM) {
    // The brightness (2R + 4G + B) times the colour in mode.
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    s32 red;
    s32 green;

    tintRed = mode;
    tintRed &= 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = mode >> 10;
    tintBlue &= 31;
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      red = ((color << 11) & 0xf800);
      green = ((color << 7) & 0x1f000);
      blue = (color & 0x7c00);
      color = Iwram_SignedDivide(red + green + blue, 96);
      red = color * tintRed;
      green = color * tintGreen;
      blue = color * tintBlue;
      red = PaletteFade_ClampShiftedComponent(red);
      green = PaletteFade_ClampShiftedComponent(green);
      blue = PaletteFade_ClampShiftedComponent(blue);
      dst[i * 3 + 0] = blue;
      dst[i * 3 + 1] = green;
      dst[i * 3 + 2] = red;
    }
  } else if (mode & PALETTE_TINT_BLEND) {
    // The brightness R + G + B shared out in the proportions of mode's colour.
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    s32 blendedRed;
    s32 blendedGreen;
    s32 blendedBlue;
    s32 clampedRed;
    s32 clampedGreen;
    s32 clampedBlue;

    tintRed = mode;
    tintRed &= 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = mode >> 10;
    tintBlue &= 31;
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      red = color & 31;
      color = Iwram_SignedDivide(
          (red + ((color >> 5) & 31) + ((color >> 10) & 31)) << 4,
          tintRed + tintGreen + tintBlue);
      blendedRed = Iwram_MulQ16(((color * tintRed) >> 4) << 16,
                                (s32)(tintRed << 16) >> 4);
      blendedGreen = Iwram_MulQ16(((color * tintGreen) >> 4) << 16,
                                  (s32)(tintGreen << 16) >> 4);
      blendedBlue = Iwram_MulQ16(((color * tintBlue) >> 4) << 16,
                                 (s32)(tintBlue << 16) >> 4);
      clampedRed = PaletteFade_ClampComponent((u32)blendedRed >> 16);
      clampedGreen = PaletteFade_ClampComponent((u32)blendedGreen >> 16);
      clampedBlue = PaletteFade_ClampComponent((u32)blendedBlue >> 16);
      dst[i * 3 + 0] = gColorBuffer_ChannelCurve[clampedBlue];
      dst[i * 3 + 1] = gColorBuffer_ChannelCurve[clampedGreen];
      dst[i * 3 + 2] = gColorBuffer_ChannelCurve[clampedRed];
    }
  } else if (mode & PALETTE_TINT_SPLIT) {
    // The colours unchanged, as the default tint writes them.
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      dst[i * 3 + 0] = color & 0x7c00;
      dst[i * 3 + 1] = (color & 0x03e0) << 5;
      dst[i * 3 + 2] = (color & 0x001f) << 10;
    }
  } else {
    // Copy a ready buffer: mode is its address, and the OBJ half starts 224
    // colours of three halfwords in.
    if (half == 2)
      mode += COLOR_HALF * 3 * sizeof(u16);
    Dma_Set((const void *)mode, dst,
            ((colorCount * 6) >> 2) | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  }
}

void BattleEffect_InitializeBuffers(void) {
  volatile s32 zero;
  struct BattleEffectBuffers *buffer;

  buffer = HeapSlot_AllocEwram(128, sizeof(*buffer));
  zero = 0;
  Dma_Set(&zero, buffer,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
              (sizeof(*buffer) / 4),
          REG_DMA3);
  Dma_Set((void *)BG_PLTT, buffer, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, (u8 *)buffer->palette + 0x1c0, DMA_COPY_WORDS(112),
          REG_DMA3);
  BattleEffect_BuildBuffer(0x10000, buffer, buffer->target, 0);
  Task_Add((s32)BattlePalette_UpdateBlend, TASK_PASS_VBLANK + 15);
}

void BattlePalette_StopBlendAndReleaseBuffer(void) {
  Task_Remove((s32)&BattlePalette_UpdateBlend);
  HeapSlot_Release(128);
}

void BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    BattleEffect_BuildBuffer(value, buffers, buffers->target, mode);
  }
}

void BattleEffect_ApplyColorToSourceBuffer(s32 value, s32 mode) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    BattleEffect_BuildBuffer(value, buffers, buffers->current, mode);
  }
}

void BattleEffect_SetPrimaryBufferValue(u32 value) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    buffers->palette[0] = value;
  }
}

// Packs the effect's target channels into its palette.

/// Packs the 448 target colours, three channels each, back into the effect's
/// palette as RGB555.
void BattleEffect_PackTargetBuffer(void) {
  struct BattleEffectBuffers *work = Ram_HeapSlots->block32;
  u16 *packed = work->palette;
  s32 blue = 0x7c00;
  u16 *current;
  s32 i;

  i = 0x1c0;
  current = work->target;
  for (; i != 0; i--) {
    *packed++ = (current[0] & blue) | ((current[1] >> 5) & 0x3e0) |
                ((current[2] >> 10) & 0x1f);
    current += 3;
  }
  work--;
}

void PaletteFade_StartInterpolation(s32 mode) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    buffers->duration = mode;
    buffers->step = 0;
    BattleEffect_InterpolateBuffers((s16 *)buffers->current,
                                    (s16 *)buffers->target,
                                    (s16 *)buffers->delta, mode);
  }
}

void BattleEffect_LoadBufferPalette(const u16 *colours) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWordsEntry;

    copy(buffers->palette, colours, 0x1c0);
    BattleEffect_BuildBuffer(0x10000, buffers, buffers->target, 1);
  }
}

// Palette: clamp RGB555 components.

/// Clamps a five-bit component to 0-31.
s32 PaletteFade_ClampComponent(s32 val) {
  if (val > 31)
    return 31;
  if (val < 0)
    val = 0;
  return val;
}

/// Clamps to an upper bound of 0x7c00 (RGB555 maximum blue), with no lower
/// bound.
s32 PaletteFade_ClampShiftedComponent(s32 val) {
  if (val > 0x7c00)
    val = 0x7c00;
  return val;
}

// Each frame, on the two cells beside the actor
// where the ground one or two cells south rises above the actor's floor,
// draws a 16-pixel-wide mark at the height of that ground.
void FadeOverlay_DrawMarks(void) {
  struct FadeOverlayWork *work = Ram_HeapSlots->unk84[3];
  s16 *camera = ((struct FadeView *)Ram_HeapSlots->mapWork)->camera;
  s32 cameraX = camera[1];
  s32 cameraZ = camera[3];
  struct AnimationSpritePart *mark = work->marks;
  struct FadeActor *actor = work->actor;
  s32 x;
  s32 z;
  s32 terrainFloor;
  u8 terrainLayer;
  u32 tile;
  s32 height;
  s32 secondHeight;
  s32 screenRow;
  s16 unused[2];

  if (actor == 0)
    return;
  z = actor->z;
  x = actor->x;
  terrainFloor = actor->floor;
  terrainLayer = actor->terrainId;
  tile = gResourceTableEntries[94].offset >> 5;
  x -= 0x80000;
  height = MapGrid_GetTerrainHeightFar(terrainLayer, x, z + 0x100000) >> 16;
  secondHeight =
      (MapGrid_GetTerrainHeightFar(terrainLayer, x, z + 0x200000) >> 16) - 16;
  if (secondHeight > height)
    height = secondHeight;
  if (height > 0 && height > terrainFloor) {
    ((u32 *)mark)[1] = 0x40000800;
    ((u32 *)mark)[2] = 0x400;
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = 1;
    mark->x = ((u32)(x >> 16) & 0xfff0) - cameraX;
    screenRow = (z >> 16) & 0xf0;
    mark->y = screenRow - cameraZ - height + 16;
    Oam_AddSpriteAtDepthClamped(mark, 0);
  }
  x += 0x100000;
  height = MapGrid_GetTerrainHeightFar(terrainLayer, x, z + 0x100000) >> 16;
  mark = &work->marks[1];
  secondHeight =
      (MapGrid_GetTerrainHeightFar(terrainLayer, x, z + 0x200000) >> 16) - 16;
  if (secondHeight > height)
    height = secondHeight;
  if (height > 0 && height > terrainFloor) {
    ((u32 *)mark)[1] = 0x40000800;
    ((u32 *)mark)[2] = 0;
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = 1;
    mark->x = ((u32)(x >> 16) & 0xfff0) - cameraX;
    screenRow = (z >> 16) & 0xf0;
    mark->y = screenRow - cameraZ - height + 16;
    Oam_AddSpriteAtDepthClamped(mark, 0);
  }
}

/// Clears the overlay tiles to colour 1, sets the blend registers and starts
/// FadeOverlay_DrawMarks at priority 0x480 on a 28-byte work block whose last
/// word is the object (the selected owner's when none is given).
void FieldEffect_StartFadeOverlay(void *object) {
  u32 *work;
  u8 *tiles;
  volatile u32 fill;

  work = HeapSlot_AllocEwram(144, 28);
  tiles = HeapSlot_Alloc(56, 0x400);
  Dma_Fill32Cell(fill, 0x11111111, tiles, 0x200);
  VramCache_Load(94, 0x200, tiles);
  HeapSlot_Release(56);
  Task_Add(FadeOverlay_DrawMarks, TASK_PASS_VBLANK);
  Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                 BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
             &REG_BLDCNT);
  Io_Write16(16, &REG_BLDALPHA);
  Io_Write16(31, &REG_BLDY);
  Dma_Fill32Cell(fill, 0, work, 28);
  if (object == NULL)
    object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  work[6] = (u32)object;
}

/// Suspends the fade overlay's drawing callback.
void FadeOverlay_Suspend(void) { Task_SuspendAll(FadeOverlay_DrawMarks); }

/// Resumes the fade overlay's drawing callback.
void FadeOverlay_Resume(void) { Task_ResumeAll(FadeOverlay_DrawMarks); }

/// The blank overlay's task, which FieldEffect_StartBlankOverlay starts: each
/// frame it draws a 16-pixel mark beside the actor where the map's third layer
/// flags a cell two rows up (left or right of the actor), and another for the
/// row one up.
void BlankOverlay_DrawMarks(void) {
  struct AnimationSpritePart *mark = Ram_HeapSlots->unk84[3];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s16 *camera = (s16 *)&map->viewX;
  s16 cameraX = camera[1];
  s16 cameraZ = camera[3];
  struct FadeActor *actor = ((struct FadeOverlayWork *)mark)->actor;
  s32 x;
  s32 row;
  u32 tile;
  struct MapCell *cells;
  struct MapCell *a;
  struct MapCell *b;
  struct AnimationSpritePart *m;

  if (actor == 0)
    return;
  x = ((s16 *)&actor->x)[1];
  row = (actor->z - actor->y) >> 16;
  tile = gResourceTableEntries[94].offset >> 5;
  cells = map->layers[2].cells;
  a = cells + (x + 8) / 16 + (row / 16 - 2) * 128;
  b = cells + (x - 8) / 16 + (row / 16 - 2) * 128;
  if ((a->attributeB & 1) || (b->attributeB & 1)) {
    m = mark;
    ((u32 *)m)[1] = 0x40000000;
    ((u32 *)m)[2] = 0x400;
    m->tile = tile;
    m->x = (u16)(x - cameraX) - 8;
    m->y = (u16)(((u16)row & 0xf0) - cameraZ) - 32;
    mark = m + 1;
    Oam_AddSpriteAtDepthClamped(m, 1);
  }
  a = cells + (x + 8) / 16 + (row / 16 - 1) * 128;
  b = cells + (x - 8) / 16 + (row / 16 - 1) * 128;
  if ((a->attributeB & 1) || (b->attributeB & 1)) {
    m = mark;
    ((u32 *)m)[1] = 0x40000000;
    ((u32 *)m)[2] = 0x400;
    m->tile = tile;
    m->x = (u16)(x - cameraX) - 8;
    m->y = (u16)(((u16)row & 0xf0) - cameraZ) - 16;
    Oam_AddSpriteAtDepthClamped(m, 1);
  }
}

/// Like FieldEffect_StartFadeOverlay with blank tiles and no blending:
/// loads 0x200 zeroed tiles to VRAM slot 94 and starts BlankOverlay_DrawMarks
/// on object (the controlled owner's when 0).
void FieldEffect_StartBlankOverlay(s32 object) {
  u8 *work = HeapSlot_AllocEwram(144, 28);
  void *buffer = HeapSlot_Alloc(56, 0x400);

  Dma_Fill32(0, buffer, 0x200);
  VramCache_Load(94, 0x200, buffer);
  Task_Add(BlankOverlay_DrawMarks, 0x480);
  HeapSlot_Release(56);
  if (object == 0)
    object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  *(s32 *)(work + 24) = object;
}

/// Suspends the BlankOverlay_DrawMarks overlay task.
void BlankOverlay_Suspend(void) { Task_SuspendAll(BlankOverlay_DrawMarks); }

/// Resumes the BlankOverlay_DrawMarks overlay task.
void BlankOverlay_Resume(void) { Task_ResumeAll(BlankOverlay_DrawMarks); }

/// Fills the overlay's 512 bytes of sprite tiles (VRAM cache slot 94) with
/// colour 1 when solid is set, else clears them.
void BattleEffect_FillOverlayTiles(s32 solid) {
  void *buffer = HeapSlot_Alloc(56, 0x400);

  if (solid == 0) {
    Dma_Fill32(0, buffer, 0x200);
  } else {
    Dma_Fill32(0x01010101, buffer, 0x200);
  }
  VramCache_Load(94, 0x200, buffer);
  HeapSlot_Release(56);
}

/* VIEW(f0cf050): tag struct EffectSprite as struct EffectSprite__f0cf050;
 * effect_sprite.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/field/field_sequence.c:6192: structure has no member named
 * `priority'. */
struct EffectSprite__f0cf050 {
  u8 unk00[4];
  u16 y : 8;
  u16 affineMode : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 palette256 : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 depth;
};

/// Sets up sprite through EffectSprite_Init with resource 94's tiles, at
/// priority 1 in palette 0.
void EffectSprite_InitFromSlot94(struct EffectSprite__f0cf050 *sprite, s32 a,
                                 s32 b, s32 c) {
  EffectSprite_Init(sprite, a, b, c, gResourceTableEntries[94].offset >> 5);
  sprite->depth = 0;
  sprite->priority = 1;
  sprite->palette256 = 0;
  sprite->palette = 0;
}

/// Projects the world position (x, y, z) to the screen, stores it as the
/// effect sprite's position and draws the sprite.
void BattleEffect_DrawSpriteAtWorldPosition(s32 *sprite, s32 x, s32 y, s32 z) {
  s32 position[3];

  SET_VECTOR(position, x, y, z);
  Camera_WorldToScreen(position);
  sprite[3] = position[0];
  sprite[4] = position[2];
  EffectSprite_Draw(sprite);
}

/// The twin of EffectSprite_InitFromSlot94 with object mode 1 and priority 0.
void EffectSprite_InitFromSlot94Blended(struct EffectSprite__f0cf050 *sprite,
                                        s32 a, s32 b, s32 c) {
  EffectSprite_Init(sprite, a, b, c, gResourceTableEntries[94].offset >> 5);
  sprite->depth = 0;
  sprite->priority = 0;
  sprite->palette256 = 0;
  sprite->palette = 0;
  sprite->objectMode = 1;
}

/// Finds the battle effect definition of id among the first 243 entries; when
/// none matches it returns entry 243.
struct BattleFxDefinition *Speaker_FindDefinition(u32 id) {
  struct BattleFxDefinition *entry = gData_080f0308;
  u32 index = 0;

  if (entry->id != id) {
    do {
      index += 1;
      entry++;
      if (index > 242)
        break;
    } while (entry->id != id);
  }
  return entry;
}

s32 Speaker_GetActorSprite(u32 objectId) {
  struct BattleRenderObject *object =
      (struct BattleRenderObject *)ObjectTable_Get(objectId);

  if (object->kind != 1 || object->animation == NULL ||
      object->animation->value28 == NULL) {
    return 0;
  }
  return *object->animation->value28;
}
#include "resource_ids.h"

/* VIEW(definition_flags): routine Speaker_GetActorSprite as s32
 * Speaker_GetActorSprite(void); the definition has s32
 * Speaker_GetActorSprite(u32 objectId); not tried: Speaker_GetActorSprite's
 * bound declaration was not found. */
extern s32 BattleEffect_GetAnimationValue__definition_flags(void) __asm__(
    "Speaker_GetActorSprite");

/// Returns the voice sound of sprite id's messages, its definition's value
/// plus 0x100, or 0 for none (0xff) or with option 3
/// (gMainRoutineWork.battleFxEnabled) off. Message_OpenWindowFar takes it in
/// the high half of its last argument.
s32 Speaker_GetVoice(s32 id) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u8 value;

  if (party->battleFxEnabled == 0 ||
      (value = Speaker_FindDefinition(id)->value) == 0xff)
    return 0;
  return value + 0x100;
}

u8 BattleEffect_GetFlags(void) {
  return Speaker_FindDefinition(
             BattleEffect_GetAnimationValue__definition_flags())
      ->flags;
}

extern const s32 gPairTable[];

/// Looks key up in gPairTable's pairs and returns its value, or -1.
s32 PairTable_Find(s32 key) {
  const s32 *entry = gPairTable;
  s32 value;

  while (1) {
    value = entry[0];
    if (value == -1)
      return value;
    if (value == key)
      return entry[1];
    entry += 2;
  }
}

/// Looks key up as PairTable_Find does and returns 0.
s32 PairTable_FindAndIgnore(s32 key) {
  PairTable_Find(key);
  return 0;
}

/// PairTable_Find again.
s32 PairTable_FindCopy(s32 key) { return PairTable_Find(key); }

/// Unused: only the far-call table refers to it. Draws heading picture
/// `heading` of the headings resource into a 26 by 5 cell
/// window and opens the window from the centre, 5 pixels a side each frame
/// for 24 frames.
void Heading_Show(s32 heading) {
  u16 tile;
  s32 *headings = Resource_GetTableEntry((s32)&ResourceId_Headings);
  u8 *tiles;
  u16 *cell;
  u32 row, column;
  s32 left, right;

  tile = 16;
  Event_WaitTransitionFrames();
  cell = (u16 *)BG_SCREEN_BLOCK(4);
  tiles = Heap_AllocEwram(0x1040);
  headings += headings[heading] / 4;
  Resource_DecodeByteLzInRam(headings, tiles);
  Dma_Set(tiles, (void *)0x06000200, 0x84000410, (volatile u32 *)0x040000d4);
  Heap_Release(tiles);
  REG_DISPCNT &= 0xfeff;
  Dma_Fill32(0xf000, cell, 0x800);
  cell = (u16 *)0x060020c0;
  for (row = 0; row <= 4; row++) {
    cell += 2;
    for (column = 0; column <= 25; column++) {
      *cell++ = (u16)(tile++) | 0xf000;
    }
    cell += 4;
  }
  REG_DISPCNT &= 0x1fff;
  REG_DISPCNT |= 0x2000;
  REG_WININ = 0x3f;
  REG_WINOUT = 0x3e;
  REG_WIN0H = 0x7878;
  REG_WIN0V = 0x1848;
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10008, 0);
  PaletteFade_StartInterpolation(24);
  REG_DISPCNT ^= 0x100;
  left = 120;
  right = 120;
  // column is reused for the window's frames; a local of its own differs.
  for (column = 0; column <= 23; column++) {
    left -= 5;
    right += 5;
    REG_WIN0H = (left << 8) | right;
    wait(1);
  }
}

/// Waits up to 120 frames, stopping early when a button is pressed.
void Event_WaitTwoSecondsOrPress(void) {
  u32 i;

  for (i = 0; i <= 119; i++) {
    wait(1);
    if (gInput.pressed != 0)
      break;
  }
}

/// Clears the left (corner & 3) + 1 pixels of halfword corner >> 2 in the
/// 4bpp tile at (x, y) of a 26-tile-wide grid at VRAM 0x06000200.
void Tile_ClearLeftPixels(s32 x, s32 y, u32 corner) {
  u16 *cell = (u16 *)(0x06000200 + ((y * 26 + x) << 5));

  cell += corner >> 2;

  switch (corner & 3) {
  case 0: {
    u32 value = *cell;
    u32 mask;

    mask = 0xfff0;
    mask &= value;
    *cell = mask;
  } break;
  case 1:
    *cell &= 0xff00;
    break;
  case 2:
    *cell &= 0xf000;
    break;
  case 3:
    *cell &= 0;
    break;
  }
}

extern s16 gHeading_WipeSteps[];

#define DMA3 REG_DMA3

/// Unused: only the far-call table refers to it. Closes Heading_Show's
/// picture window: fades the palette out while wiping
/// the picture's 26 by 5 tiles from the left by the steps in gHeading_WipeSteps
/// (runs of widths, each run ended by a negative entry and the whole by -9),
/// then turns BG0 and the window off, clears the screen block, reloads the
/// window tiles and turns BG0 back on.
void Heading_Close(void) {
  s32 step = 0;
  u32 x;
  u32 y;
  volatile u32 fill;

  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(24);
  for (;;) {
    for (; gHeading_WipeSteps[step] >= 0; step++) {
      for (x = 0; x < 26; x++) {
        for (y = 0; y < 5; y++)
          Tile_ClearLeftPixels(x, y, gHeading_WipeSteps[step]);
      }
    }
    wait(2);
    if (gHeading_WipeSteps[step] == -9)
      break;
    step++;
  }
  REG_DISPCNT ^= DISPCNT_BG0_ON | DISPCNT_WIN0_ON;
  Dma_Fill32Cell(fill, 0xf000, (void *)BG_SCREEN_BLOCK(4), 0x800);
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles),
          (void *)BG_CHAR_BLOCK(0), 0x84000800, DMA3);
  REG_DISPCNT ^= DISPCNT_BG0_ON;
}
#undef DMA3
