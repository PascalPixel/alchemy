/// @file
/// Trial Road's movers' sprites, the setup of its gauge, a rising platform's
/// entry, the trap table's setup, Func_02004418, the placing of actors 8, 9
/// and 10, Func_02004828 and a second copy of
/// Effect_AdvanceMotionAtLeaderPriority, in ROM order.
#include "trial_road.h"
#include "iwram_call.h"
#include "frame.h"
#include "map.h"
#include "event_inline.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 offset;
};

struct TrialRoadMover {
  s16 id;
  s16 phase;
  s16 offset;
  s16 unk06[8];
  s16 bob;
  struct Actor *marker;
};

struct TrialRoadMovers {
  s16 vramEntry;
  s16 shape;
  struct TrialRoadMover movers[16];
  u32 *entries;
};

extern struct ResourceTableEntry gResourceTableEntries[];

void Func_02005790(void *entry, s32 priority);

/// Queues the Trial Road movers' sprites.
void Func_02003f68(void) {
  struct TrialRoadMovers *base =
      (struct TrialRoadMovers *)(gData_020023c4 + 0x188);
  struct TrialRoadMover *mover = base->movers;
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
      Func_02005790(entry, depth);
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
    Func_02005790(entry, depth);
    entry += 3;
    count++;
  }
}

#include "types.h"
#include "actor.h"
#include "field.h"

struct TrialRoadGauge {
  u16 handle;
  u16 field02;
  u8 unk004[0x1c0];
  void *buffer;
};

extern const u8 gField_Tiles64D[];

void *Heap_AllocEwram(s32 size);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02003da0(void);
void Func_02005770(const void *source, void *destination);

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
#include "ram_buffer.h"
#include "main_routine_work.h"

extern u8 gMapShapeGrid[];

s32 Func_02005a18(struct Actor *object);
s32 Func_02005848(s32 layer, s32 x, s32 z);
void Func_02005958(s32 id);
s32 Func_02005a20(s32 layer, s32 shape, s32 height);

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

struct Actor *Func_02005a28(s32 *position, s32 mode);
void Func_02005880(void);
void Func_02005888(void);
void Func_02004a94(s32 value);
void Func_02004d6c(s32 offset, s32 layer);
void Func_02004bc8(void);

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
    Func_02005880();
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
    Func_02005888();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}

struct TrapEntry {
  u16 actor;
  u16 flag;
};

void Func_02004ce0(s32 a, s32 b, s32 c);

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
      Func_02005880();
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
      Func_02005888();
      break;
    }
  }
}

void Func_0200468c(s32 id) {
  Actor_GetById(id)->unk5c[0] = 3;
  Func_02005958(id);
}

void Func_020058f0(s32 id, s32 owner);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
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

  Func_020058f0(8, gMainRoutineWork.currentOwner);
  Func_020058f0(9, gMainRoutineWork.currentOwner);
  Func_020058f0(10, gMainRoutineWork.currentOwner);
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
    Func_02005880();
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
    Func_02005888();
    return;
  test:
    if (*table != 0xffff)
      goto next;
  }
}

/// The sprite's priority bits, read as the byte that holds them.
struct EffectSpriteBits {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

/// A sprite's attribute 2, as the byte bits view reaches it.
struct EffectSpriteAttributes {
  u8 unk00[9];
  struct EffectSpriteBits bits;
};

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
