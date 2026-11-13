/// @file
/// The Gondowan Cliffs' events and set-up, and the geysers Scoop digs up out of
/// the yellow sand: each rises and falls on its timer, carrying the leader up
/// its column of water (Geysers_Update, Geysers_Draw).

#include "flags.h"
#include "../shared/map_cells.h"
#include "types.h"
#include "map.h"
#include "actor.h"
#include "event_inline.h"
#include "field.h"
#include "event_work.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/scene.h"
#include "module/tla.h"

// Set as Frost freezes each puddle into a pillar of ice, for this visit.
#define FLAG_FIRST_PUDDLE_FROZEN 0x201
#define FLAG_SECOND_PUDDLE_FROZEN 0x202

struct Actor;
struct EffectOptions;
struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};
/// One rising platform and what rides it.
struct Geyser {
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

struct GeyserWork {
  s16 entry;
  s16 value;
  struct Geyser entries[16];
  void *block;
};

void GondowanCliffs_FreezeSecondPuddle(void);
void GondowanCliffs_BlowAwayFoliage(void);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void Func_02001304(s32 a, s32 b, s32 c);
void Func_020012f4(void);
void Geysers_Start(s32 a, s32 b);
void Func_020012fc(void);

void GondowanCliffs_RollPillar(struct Actor *object) {
  struct TileSprite *sprite = object->sprite;

  sprite->tile -= 0x800;
}

void GondowanCliffs_WaitToLand(struct Actor *object, s32 height) {
  s32 count = 60;

  while (count != 0) {
    System_Wait(1);
    count--;
    if (object->y <= height)
      break;
  }
}

#include "field_sprite.h"
#include "system/field_effect.h"

void GondowanCliffs_PushPillarIntoWater(s32 unk0, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 unused[3]; // Unused: gives the stack frame its 12 bytes.
  struct EffectOptions options;
  struct FieldSprite *sprite;
  u32 i;

  if (actor->x >> 20 != 76)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(74, 31, 1, 1, 74, 33);
  if (Actor_GetById(15)->x >> 20 == 76) {
    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    for (i = 0; i <= 17; i++) {
      System_Wait(1);
      sprite = actor->sprite;
      sprite->rotation -= 256;
      actor->x -= Trig_Cos(((struct FieldSprite *)actor->sprite)->rotation) / 2;
      actor->targetX = 0x80000000;
    }
    actor->update = GondowanCliffs_RollPillar;
    Actor_SetSpeed(id, 0x30000, 0x18000);
    Actor_SetDestination(id, 1193, 530);
    actor->gravity = 0xcccc;
    actor->motionFlags = 3;
    actor->terrainId = 0;
    FieldActor_WaitForMoveById(id);
    GondowanCliffs_WaitToLand(actor, 0);
    Event_PlaySound(188);
    MapGrid_SetShake(0x50000, 0x50000, 0x10000);
    Event_PlaySound(141);
    MapGrid_SetShake(-1, -1, 0xe666);
    ((struct FieldSprite *)actor->sprite)->rotation = 0x1000;
    // Cleared through a word: as a function pointer the store sinks below the
    // effect options.
    *(u32 *)&actor->update = 0;
    options.type = 276;
    options.startScaleX = 0x8000;
    options.targetScaleX = 0x18000;
    options.targetScaleY = 0x13333;
    options.startScaleY = 0xcccc;
    FieldEffect_Spawn(actor->x, actor->y, actor->z, 0, 0, 0,
                      EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                          EFFECT_SCALE_TO_TARGET,
                      &options);
    Event_PlaySound(154);
    Actor_SetAnimation(id, 3);
    FieldMapGrid_WaitForShakeToSettle();
    Actor_GetById(15)->priorityFlags |= 2;
    Event_SetFlag(FLAG_GONDOWAN_CLIFFS_PILLAR_IN_WATER);
  }
  Event_End();
}

#include "scene_tables.h"

DEFINE_PILLAR_SET_MOVED(GondowanCliffs_FreezeFirstPuddle, 12, 79, 28, 81,
                        FLAG_FIRST_PUDDLE_FROZEN)

void GondowanCliffs_FreezeSecondPuddle(void) {
  struct Actor *object = Actor_Get(13);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  object->terrainHeight = PIXELS(32);
  object->y = PIXELS(32);
  MapGrid_CopyCellAttributes(115, 35, 1, 1, 117, 35);
  Event_SetFlag(FLAG_SECOND_PUDDLE_FROZEN);
}

void GondowanCliffs_BlowAwayFoliage(void) {
  GameFlag_SetBit(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN);
}

void GondowanCliffs_ScoopUpGeyser(s32 unused, s32 cell) {
  struct Actor *object;

  if (cell == 30) {
    object = Actor_Get(17);
    MapGrid_CopyCellsLayered(124, 1, 1, 1, 119, 43);
    Actor_SetPosition(17, PIXELS(1912), PIXELS(728));
    object->terrainHeight = object->y =
        Map_GetTerrainHeight(0, object->x, object->z);
    Event_SetFlag(FLAG_GONDOWAN_CLIFFS_GEYSER_DUG);
  }
}

#include "ram_buffer.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"

void GondowanCliffs_TalkToDog(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  struct {
    s32 x, y, z;
  } position;
  s32 *out = (s32 *)&position;

  out[0] = object->x;
  out[1] = object->y;
  out[2] = object->z;
  FieldVector_AddPolarOffset(0x100000, object->facing, out);
  if ((position.x >> 20) == 119 && (position.z >> 20) == 45) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk15a = 1;
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_FaceEachOther(16, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)MsgRrruffRrruff2);
    Event_ShowMessage(16, 0);
    Actor_FaceDirection(16, FACING_WEST, 0);
    Event_End();
  }
}

#include "scene_ids.h"

extern const u8 gGondowanCliffs_PassTriggers[];
extern const u8 gGondowanCliffs_Triggers[];

void GondowanCliffs_GrindTheRock(s32 unused, s32 id) {
  Actor_SetPosition(id, 0, 0);
  Event_SetFlag(FLAG_GONDOWAN_CLIFFS_ROCK_GROUND);
}

const u8 *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GondowanCliffsPass)
    return gGondowanCliffs_PassTriggers;
  return gGondowanCliffs_Triggers;
}

#include "frame.h"
#include "io_reg.h"

extern u16 gGondowanCliffs_SavedWaterColors[];

void GondowanCliffs_CycleWaterColors(void) {
  u16 *color = (u16 *)&BG_PLTT_COLOR(0, 12);
  u32 phase = gFrameCount >> 4;
  s32 i;

  for (i = 3; i >= 0; i--) {
    u32 index = 3;

    index &= phase;
    *color++ = gGondowanCliffs_SavedWaterColors[index];
    phase++;
  }
}

#include "iwram_call.h"
#include "call.h"

void GondowanCliffs_SetUpPass(void) {
  if (gMainRoutineWork.entrance == 99) {
    gMainRoutineWork.renderMode = 7;
    gMainRoutineWork.ownerOverride = 1;
    Event_SetDestination((s32)&SceneId_GondowanCliffsPass, 1);
  }
  Func_020012fc();
  Value3((s32 (*)())Func_02001304, 0, 9, 10);
  Value3((s32 (*)())Iwram_CopyWordsEntry, (s32)gGondowanCliffs_SavedWaterColors,
         0x05000018, 8);
  Task_AddFrame(GondowanCliffs_CycleWaterColors, 0x480);
}

void Actor_MoveToObject(s32 actor, s32 target);

static inline void GondowanCliffs_Jump(s32 actor, s32 height, s32 frames) {
  Actor_ShowEmoteById(actor, height, frames);
}

/// The scene that asks where the party intends to go, to help recover the black
/// orb.
void GondowanCliffs_TurnBackToHelpPiers(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 552, 488);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  FieldActor_WatchAndSetScript(5, gMainRoutineWork.currentOwner);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 568, 480);
  FieldActor_WatchAndSetScript(6, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 584, 472);
  FieldActor_WatchAndSetScript(22, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(22, gMainRoutineWork.currentOwner);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 608, 472);
  FieldEvent_SetMessageId((s32)MsgWhereDoYou);
  GondowanCliffs_Jump(22, 258, 20);
  Event_ShowMessage(22, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 10);
  FieldActor_Jump(5, 6, 15);
  FieldActor_Jump(5, 6, 23);
  Event_ShowMessage(5, 0);
  FieldActor_Stop(6);
  Actor_FaceActor(6, 5, 0);
  Event_ShowMessage(6, 0);
  FieldActor_WatchAndSetScript(6, gMainRoutineWork.currentOwner);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(22, 3);
  Event_ShowMessage(22, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(30);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 584, 472);
  Actor_SetPosition(22, 0, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 568, 480);
  Actor_SetPosition(6, 0, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 552, 488);
  Actor_SetPosition(5, 0, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 528, 488);
  Event_FinishScreenTransition();
  Event_PlaySound(123);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 512, 488);
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(1);
}

void Func_0200130c(s32 a, s32 b, s32 c, s32 d);
struct Geyser *Geyser_Add(s32 slot, s32 id, s32 timer, s32 low, s32 high);

/// Gondowan Cliffs' setup: Ov66bRoom2 runs GondowanCliffs_SetUpPass and
/// returns; with flag 0x8f3 set and 0x8ef clear it runs
/// GondowanCliffs_TurnBackToHelpPiers; otherwise in Ov66bRoom1 it starts the
/// room's lists, places or raises actor 14 by flag
/// FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN, copies cells by flags
/// FLAG_FIRST_PUDDLE_FROZEN and FLAG_SECOND_PUDDLE_FROZEN, records the swinging
/// actor 17 (Geyser_Add) and places it by flag FLAG_GONDOWAN_CLIFFS_GEYSER_DUG,
/// sets actor 16's mode, places actor 15 by flag
/// FLAG_GONDOWAN_CLIFFS_PILLAR_IN_WATER, saves four palette colours, schedules
/// GondowanCliffs_CycleWaterColors and sets BG layer priority. Returns 0.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 height;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 513;
  if (gMainRoutineWork.scene == (s32)&SceneId_GondowanCliffsPass) {
    GondowanCliffs_SetUpPass();
    return 0;
  }
  if (GameFlag_Test(0x8f3) && !GameFlag_Test(0x8ef)) {
    GondowanCliffs_TurnBackToHelpPiers();
    return 0;
  }
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 32;
  if (gMainRoutineWork.scene != (s32)&SceneId_GondowanCliffs)
    goto done;
  Func_020012f4();
  Func_0200130c(0, 0x200, 10, 11);
  if (GameFlag_Test(0x8ab)) {
    Func_020012fc();
    Func_02001304(0, 8, 9);
  }
  if (GameFlag_Test(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN))
    Actor_SetPosition(14, 0, 0);
  else
    Actor_GetById(14)->scaleY = 0x14000;
  if (GameFlag_Test(FLAG_FIRST_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(79, 28, 1, 1, 81, 28);
  if (GameFlag_Test(FLAG_SECOND_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(115, 35, 1, 1, 117, 35);
  Geysers_Start(0, 240);
  Geyser_Add(0, 17, 0, 0, 64);
  if (GameFlag_Test(FLAG_GONDOWAN_CLIFFS_GEYSER_DUG)) {
    actor = Actor_GetById(17);
    MapGrid_CopyCellsLayered(124, 1, 1, 1, 119, 43);
    Actor_SetPosition(17, PIXELS(1912), PIXELS(728));
    height = Map_GetTerrainHeight(0, actor->x, actor->z);
    actor->y = height;
    actor->terrainHeight = height;
  }
  Actor_SetAnimation(16, 4);
  if (GameFlag_Test(FLAG_GONDOWAN_CLIFFS_PILLAR_IN_WATER)) {
    MapGrid_CopyCellAttributes(74, 31, 1, 1, 74, 33);
    Actor_SetPosition(15, PIXELS(1193), PIXELS(530));
    Actor_GetById(15)->priorityFlags |= 2;
    Actor_GetById(15)->collisionFlags = 0;
    Actor_SetAnimation(15, 2);
  } else {
    Actor_GetById(15)->collisionFlags |= 16;
  }
  Iwram_CopyWordsEntry(((u8 *)gGondowanCliffs_SavedWaterColors),
                       (void *)&BG_PLTT_COLOR(0, 12), 8);
  Task_AddFrame(GondowanCliffs_CycleWaterColors, 0x480);
  ((struct MapLayerBits *)Ram_HeapSlots->mapWork)->priority = 1;
done:
  return 0;
}

extern const u8 gGondowanCliffs_MapRects[];

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GondowanCliffs)
    *(const u8 **)Ram_HeapSlots->mapWork->unk0fc = gGondowanCliffs_MapRects;
  return 0;
}

#include "map.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

// The swinging actors' callback, the same as Yampi Desert's platforms: for each
// of the 16 entries it moves the actor on its timer, writes its height to the
// collision and cell maps, and carries its rider with a bob.
void Geysers_Update(void) {
  struct GeyserWork *work = (struct GeyserWork *)(gInventorySnapshot + 0x188);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Geyser *entry = work->entries;
  s32 value = work->value;
  u8 *terrain = map->layers[0].terrainIds;
  struct MapCell *cells = map->layers[0].cells;
  s32 i = 0;
  s32 *collision = (s32 *)Ram_MapCollision + value;

  for (; i <= 15; collision++, entry++, i++) {
    struct Actor *object;
    struct Actor *rider;
    struct MapCell *cell;
    s32 index;

    if (entry->id == 0)
      continue;
    object = Actor_GetById(entry->id);
    if (object->x == 0 && object->z == 0)
      continue;
    if (entry->timer == entry->riseAt) {
      Sound_Play(206);
      entry->step = 4;
    }
    if (entry->timer == entry->fallAt) {
      Sound_Play(395);
      entry->step = -8;
    }
    if (entry->step != 0) {
      entry->height += entry->step;
      if (entry->height >= entry->max) {
        entry->height = entry->max;
        entry->step = 0;
        entry->phase = 0;
      }
      if (entry->height <= entry->min) {
        entry->height = entry->min;
        entry->step = 0;
        entry->phase = 0;
      }
    }
    if (!GameFlag_Test(0x104)) {
      if (++entry->timer >= entry->period)
        entry->timer = 0;
    }
    index = (object->x >> 20) + (object->z >> 20) * 128;
    terrain[index] = (u8)work->value + i;
    *collision = ((PIXELS(object->y) + entry->height) / 8) << 8;
    cell = &cells[index];
    if (entry->step != 0)
      cell->attributeB = 0;
    else
      cell->attributeB |= 128;
    if (entry->height != 0)
      cell->attributeB |= 16;
    else
      cell->attributeB = 0;
    rider = entry->rider;
    if (rider == NULL)
      continue;
    if (entry->height == 0) {
      rider->y = object->y + -0x100000;
    } else {
      s32 wave;

      entry->phase += 0x1000;
      wave = Iwram_MulQ16(0x20000, Trig_Sin(entry->phase)) + -0xe0000;
      rider->y = object->y + (entry->height << 16) + wave;
    }
    rider->x = object->x;
    rider->z = object->z;
  }
}

#include "graphics/vram_tab.h"

void Oam_AddSpriteAtDepthClamped(u32 *oam, s32 depth);

// Draws the swinging actors' ropes and heads (the 16 records of
// gInventorySnapshot
// + 0x188 kept by Geyser_Add): for each raised actor, a rope segment every
// 16 pixels of its height and the bobbing head on top, at most 80 sprites,
// queued by Oam_AddSpriteAtDepthClamped.
void Geysers_Draw(void) {
  struct GeyserWork *work = (struct GeyserWork *)(gInventorySnapshot + 0x188);
  struct Geyser *swing = work->entries;
  s32 k;
  u32 tile = gResourceTableEntries[work->entry].offset >> 5;
  s32 frame;
  u32 *oam = work->block;
  s32 count = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 scrollX, scrollZ, lift;
  struct Actor *actor;
  s32 h, x, y, screenY, z, depth;
  u32 *p;

  scrollX = view[0];
  scrollX &= 0xffff0000;
  scrollZ = view[1] & 0xffff0000;
  lift = map->origin[1];
  for (k = 15; k >= 0; k--, swing++) {
    if (swing->id == 0 || swing->height == 0)
      continue;
    actor = Actor_GetById(swing->id);
    frame = (gFrameCount % 12) / 3 * 8 + 32;
    // A goto loop: the native moves nothing out of it; as a for loop, loop.c
    // hoists the sprite's third word (skeleton gap 64).
    h = 0;
    if (h >= swing->height)
      goto top;
  again:
    if (count > 79)
      goto next;
    x = actor->x - scrollX;
    y = actor->y + (h << 16) - lift;
    z = actor->z - scrollZ - lift;
    x = (x >> 16) - 8;
    screenY = z - y;
    screenY = (screenY >> 16) - 16;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
      goto next;
    x &= 511;
    screenY &= 255;
    p = oam;
    *p++ = 0;
    *p++ = screenY | x << 16 | 0x40002000;
    *p = (tile + frame) | 0x800;
    Oam_AddSpriteAtDepthClamped(oam, depth);
    oam += 3;
    count++;
  next:
    h += 16;
    if (h < swing->height)
      goto again;
  top:
    if (count > 79)
      continue;
    x = actor->x - scrollX;
    y = actor->y + (swing->height << 16) - lift;
    z = actor->z - scrollZ - lift;
    screenY = z - y;
    x = (x >> 16) - 8;
    /* FAKEMATCH(wrapper): forwarded, so -0x20000 is built for the call and
     * again for the sum; as a direct call, 276 of 252 instructions differ in
     * tla-en. */
    screenY =
        ((screenY +
          Value2((s32 (*)())Iwram_MulQ16, -0x20000, Trig_Sin(swing->phase)) +
          -0x20000) >>
         16) -
        4;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
      continue;
    x &= 511;
    screenY &= 255;
    p = oam;
    *p++ = 0;
    *p++ = screenY | x << 16 | 0x40002000;
    *p = (tile + frame - 32) | 0x800;
    Oam_AddSpriteAtDepthClamped(oam, depth);
    oam += 3;
    count++;
  }
}

void Geysers_Start(s32 unused, s32 value) {
  struct GeyserWork *work = (struct GeyserWork *)(gInventorySnapshot + 0x188);
  void *buffer;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Call2((void (*)())Iwram_ClearWordsEntry, (s32)work, 456);
  buffer = (void *)Value1((s32 (*)())Heap_AllocEwram, 0x800);
  Decompress_TaggedLz(gField_Tiles64D, buffer);
  work->entry = VramCache_FindFreeEntry();
  Vram_Load(work->entry, 0x800, buffer);
  Heap_Release(buffer);
  work->block = Heap_AllocEwram(0x3c0);
  work->value = value;
  FieldTask_AddFrame((s32)Geysers_Update, 0x48d);
  FieldTask_AddFrame((s32)Geysers_Draw, 0x480);
  Actor_Get(FieldParty_GetLeaderActor())->priorityFlags |= 0x40;
}

struct Geyser *Geyser_Add(s32 slot, s32 id, s32 timer, s32 low, s32 high) {
  struct GeyserWork *work = (struct GeyserWork *)(gInventorySnapshot + 0x188);
  struct Geyser *entry = &work->entries[slot];
  struct Actor *object = Actor_Get(id);
  struct Actor *rider;

  Actor_Get(id);
  Actor_SetSpriteFlags(object, 0);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
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
  rider = (FieldActor_Create(0x8000, object->x, object->y, object->z));
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= 128;
  entry->rider = rider;
  return entry;
}
