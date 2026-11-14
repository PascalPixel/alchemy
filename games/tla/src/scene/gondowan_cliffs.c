/// @file
/// The Gondowan Cliffs, west of Alhafra, in two scenes that share this overlay:
/// the cliffs (SceneId_GondowanCliffs) and the pass a rock blocks until Grind
/// breaks it (SceneId_GondowanCliffsPass). Kindle, a Mars Djinni, waits at the
/// end of the cliffs (GondowanCliffs_FightKindle); Frost, Whirlwind, Move and
/// Scoop each open a way (GondowanCliffs_FreezeFirstPuddle,
/// GondowanCliffs_BlowAwayFoliage, GondowanCliffs_PushPillarIntoWater,
/// GondowanCliffs_ScoopUpGeyser); and if the party heads west before helping
/// Piers recover his Black Orb, Kraden turns them back
/// (GondowanCliffs_TurnBackToHelpPiers).
///
/// The effect spawner and the cliffs' events are in gondowan_cliffs/, which a
/// shared field effect object links between, and the tables are in
/// data/gondowan_cliffs.c, which the overlay links after the field's tiles.

#include "shared/tables.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "module/scene/scene.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "objcmd.h"
#include "flags.h"
#include "map.h"
#include "field/field_sequence_event.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "module/tla.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "call.h"
#include "graphics/vram_tab.h"
#include "macros.h"

extern s16 gGondowanCliffs_Entrances[];
extern struct ActorPlacement gGondowanCliffs_PassEntrances[];

#include "shared/tile_sprite_reset.h"

void GondowanCliffs_FightKindle(void) { Field_RequestDjinnBattle(21, 49); }

const u8 *Scene_GetEntrances(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GondowanCliffsPass)
    return gGondowanCliffs_PassEntrances;
  return gGondowanCliffs_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gGondowanCliffs_ActorPlacements[];
extern struct ActorPlacement gGondowanCliffs_PassActorPlacements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GondowanCliffsPass)
    return gGondowanCliffs_PassActorPlacements;
  return gGondowanCliffs_ActorPlacements;
}

// clang-format off
u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };
// clang-format on

#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect/move.c"

// The Gondowan Cliffs' events and set-up, and the geysers Scoop digs up out of
// the yellow sand: each rises and falls on its timer, carrying the leader up
// its column of water (Geysers_Update, Geysers_Draw).

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

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
// One rising platform and what rides it.
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
void Tightrope_Place(s32 a, s32 b, s32 c);
void Rope_Initialize(void);
void Geysers_Start(s32 a, s32 b);
void Tightrope_Initialize(void);

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
    Actor_SetSpeed(id, 3);
    Actor_SetDestination(id, 1193, 530);
    actor->gravity = 0xcccc;
    actor->motionFlags = 3;
    actor->terrainId = 0;
    FieldActor_WaitForMoveById(id);
    GondowanCliffs_WaitToLand(actor, 0);
    Event_PlaySound(SE_THUMP);
    MapGrid_SetShake(5, 5, 1);
    Event_PlaySound(SE_RUMBLE);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
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
    Event_PlaySound(SE_CAST);
    Actor_SetAnimation(id, 3);
    FieldMapGrid_WaitForShakeToSettle();
    Actor_GetById(15)->priorityFlags |= 2;
    Event_SetFlag(FLAG_GONDOWAN_CLIFFS_PILLAR_IN_WATER);
  }
  Event_End();
}

#include "shared/pillar_set_moved.h"

void GondowanCliffs_FreezeSecondPuddle(void) {
  struct Actor *object = Actor_Get(13);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  object->terrainHeight = px(32);
  object->y = px(32);
  MapGrid_CopyCellAttributes(115, 35, 1, 1, 117, 35);
  Event_SetFlag(FLAG_SECOND_PUDDLE_FROZEN);
}

void GondowanCliffs_BlowAwayFoliage(void) {
  GameFlag_Set(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN);
}

void GondowanCliffs_ScoopUpGeyser(s32 unused, s32 cell) {
  struct Actor *object;

  if (cell == 30) {
    object = Actor_Get(17);
    MapGrid_CopyCellsLayered(124, 1, 1, 1, 119, 43);
    Actor_SetPosition(17, 1912, 728);
    object->terrainHeight = object->y =
        Map_GetTerrainHeight(0, object->x, object->z);
    Event_SetFlag(FLAG_GONDOWAN_CLIFFS_GEYSER_DUG);
  }
}

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
  FieldVector_AddPolarOffset(px(16), object->facing, out);
  if ((position.x >> 20) == 119 && (position.z >> 20) == 45) {
    gFieldWork->fieldMenu = 1;
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

extern struct Trigger gGondowanCliffs_PassTriggers[];
extern struct Trigger gGondowanCliffs_Triggers[];

void GondowanCliffs_GrindTheRock(s32 unused, s32 id) {
  Actor_SetPosition(id, 0, 0);
  Event_SetFlag(FLAG_GONDOWAN_CLIFFS_ROCK_GROUND);
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GondowanCliffsPass)
    return gGondowanCliffs_PassTriggers;
  return gGondowanCliffs_Triggers;
}

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

void GondowanCliffs_SetUpPass(void) {
  if (gMainRoutineWork.entrance == 99) {
    gMainRoutineWork.renderMode = 7;
    gMainRoutineWork.ownerOverride = 1;
    Event_SetDestination((s32)&SceneId_GondowanCliffsPass, 1);
  }
  Tightrope_Initialize();
  Value3((s32 (*)())Tightrope_Place, 0, 9, 10);
  Iwram_CopyWords((s32)gGondowanCliffs_SavedWaterColors, 0x05000018, 8);
  Task_AddFrame(GondowanCliffs_CycleWaterColors, 0x480);
}

void Actor_MoveToObject(s32 actor, s32 target);

// The scene that asks where the party intends to go, to help recover the black
// orb.
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
  FieldActor_ShowEmote(22, 258, 20);
  Event_ShowMessage(22, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 10);
  FieldActor_Jump(5, 6, 15);
  FieldActor_Jump(5, 6, 23);
  Event_ShowMessage(5, 0);
  FieldActor_Stop(6);
  Actor_FaceActor(6, 5, 0);
  Event_ShowMessage(6, 0);
  FieldActor_WatchAndSetScript(6, gMainRoutineWork.currentOwner);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
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
  Event_PlaySound(SE_MAP_EXIT);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 512, 488);
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(1);
}

void Rope_Place(s32 a, s32 b, s32 c, s32 d);
struct Geyser *Geyser_Add(s32 slot, s32 id, s32 timer, s32 low, s32 high);

// Gondowan Cliffs' setup: Ov66bRoom2 runs GondowanCliffs_SetUpPass and
// returns; with flag 0x8f3 set and 0x8ef clear it runs
// GondowanCliffs_TurnBackToHelpPiers; otherwise in Ov66bRoom1 it starts the
// room's lists, places or raises actor 14 by flag
// FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN, copies cells by flags
// FLAG_FIRST_PUDDLE_FROZEN and FLAG_SECOND_PUDDLE_FROZEN, records the swinging
// actor 17 (Geyser_Add) and places it by flag FLAG_GONDOWAN_CLIFFS_GEYSER_DUG,
// sets actor 16's mode, places actor 15 by flag
// FLAG_GONDOWAN_CLIFFS_PILLAR_IN_WATER, saves four palette colours, schedules
// GondowanCliffs_CycleWaterColors and sets BG layer priority. Returns 0.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 height;

  gFieldWork->transition = 513;
  if (gMainRoutineWork.scene == (s32)&SceneId_GondowanCliffsPass) {
    GondowanCliffs_SetUpPass();
    return 0;
  }
  if (GameFlag_Get(0x8f3) && !GameFlag_Get(0x8ef)) {
    GondowanCliffs_TurnBackToHelpPiers();
    return 0;
  }
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 32;
  if (gMainRoutineWork.scene != (s32)&SceneId_GondowanCliffs)
    goto done;
  Rope_Initialize();
  Rope_Place(0, 0x200, 10, 11);
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    Tightrope_Initialize();
    Tightrope_Place(0, 8, 9);
  }
  if (GameFlag_Get(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN))
    Actor_SetPosition(14, 0, 0);
  else
    Actor_GetById(14)->scaleY = Q16_ONE * 5 / 4;
  if (GameFlag_Get(FLAG_FIRST_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(79, 28, 1, 1, 81, 28);
  if (GameFlag_Get(FLAG_SECOND_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(115, 35, 1, 1, 117, 35);
  Geysers_Start(0, 240);
  Geyser_Add(0, 17, 0, 0, 64);
  if (GameFlag_Get(FLAG_GONDOWAN_CLIFFS_GEYSER_DUG)) {
    actor = Actor_GetById(17);
    MapGrid_CopyCellsLayered(124, 1, 1, 1, 119, 43);
    Actor_SetPosition(17, 1912, 728);
    height = Map_GetTerrainHeight(0, actor->x, actor->z);
    actor->y = height;
    actor->terrainHeight = height;
  }
  Actor_SetAnimation(16, 4);
  if (GameFlag_Get(FLAG_GONDOWAN_CLIFFS_PILLAR_IN_WATER)) {
    MapGrid_CopyCellAttributes(74, 31, 1, 1, 74, 33);
    Actor_SetPosition(15, 1193, 530);
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

extern s32 gGondowanCliffs_MapRects[];

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GondowanCliffs)
    *(const u8 **)Ram_HeapSlots->mapWork->unk0fc = gGondowanCliffs_MapRects;
  return 0;
}

// The whole-pixel half of a 16.16 coordinate.
#define PIXEL_PART(v) (((s16 *)&(v))[1])

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
      Sound_Play(SE_CURSE_FALL);
      entry->step = 4;
    }
    if (entry->timer == entry->fallAt) {
      Sound_Play(SOUND_FADE_OUT_SE5);
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
    if (!GameFlag_Get(0x104)) {
      if (++entry->timer >= entry->period)
        entry->timer = 0;
    }
    index = (object->x >> 20) + (object->z >> 20) * 128;
    terrain[index] = (u8)work->value + i;
    *collision = ((PIXEL_PART(object->y) + entry->height) / 8) << 8;
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
    /* FAKEMATCH(wrapper): forwarded. */
    screenY =
        ((screenY + Math_MulQ16(-0x20000, Trig_Sin(swing->phase)) + -0x20000) >>
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
    Iwram_ClearWords((s32)work, 456);
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

#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef FLAG_FIRST_PUDDLE_FROZEN
#undef FLAG_SECOND_PUDDLE_FROZEN
#undef PIXEL_PART

// The Gondowan Cliffs' tables, which the scene's getters return.

#include "text/msg_ids.h"

// Set as Frost freezes each puddle into a pillar of ice, for this visit.
#define FLAG_FIRST_PUDDLE_FROZEN 0x201
#define FLAG_SECOND_PUDDLE_FROZEN 0x202

void GondowanCliffs_FreezeSecondPuddle(void);
void GondowanCliffs_BlowAwayFoliage(void);
extern s32 gGondowanCliffs_ResetTileSpriteScript[];

void GondowanCliffs_FightKindle(void);
void GondowanCliffs_FreezeFirstPuddle(void);
void GondowanCliffs_TalkToDog(void);

LATE_DATA s32 gGondowanCliffs_ResetTileSpriteScript[] = {
    SC_CALL(TileSprite_Reset),
    SC_END,
};

// clang-format off
LATE_DATA s16 gGondowanCliffs_Entrances[] = {
    0, -1, 680, 0, 280, 0x4000, 0, -1,
    -1, -1, -1, 0, 1, -1, 536, 0,
    488, 0, 0, 512, 0, 0x800, 864, 0,
    2, -1, 0x7e8, 0, 792, 0x8000, 0, 512,
    0, 0x800, 864, 0, 3, -1, 0x628, 0,
    680, 0x4000, 0, 512, 0, 0x800, 864, 0,
    99, -1, 0x4f8, 0, 552, 0x4000, 0, 512,
    0, 0x800, 864, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

LATE_DATA struct ActorPlacement gGondowanCliffs_PassEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0x40000118, px(-1), -0x1, 0xffff, 0, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(GondowanCliffs),
    SCENE_EXIT_UNLESS(1, WorldMap, 21, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, WorldMap, 22, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GondowanCliffsHideout, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(GondowanCliffsPass),
    SCENE_EXIT_UNLESS(1, WorldMap, 85, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, WorldMap, 86, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gGondowanCliffs_ActorPlacements[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1560, 0, 760, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1624, 0, 760, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1304, 0, 456, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1880, 0, 568, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1576, 0, 672, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(273, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1232, 0, 544, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(189, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1928, 0, 728, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(83, 0x8ab, MOVEMENT_TYPE_STAND, 888, 0, 200, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(83, 0x8ab, MOVEMENT_TYPE_STAND, 888, 0, 232, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(424, FLAG_GONDOWAN_CLIFFS_ROCK_GROUND, MOVEMENT_TYPE_STAND, 816, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(245, 0x61, MOVEMENT_TYPE_STAND, 1336, 0, 648, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(5, IF_FLAG_SET(0x8f3), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(6, IF_FLAG_SET(0x8f3), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(56, IF_FLAG_SET(0x8f3), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGondowanCliffs_PassActorPlacements[] = {
    ACTOR_PLACEMENT(8, CONDITION_ALWAYS, (s32)gGondowanCliffs_ResetTileSpriteScript, 330, 0, 79, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(424, FLAG_GONDOWAN_CLIFFS_ROCK_GROUND, MOVEMENT_TYPE_STAND, 304, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

LATE_DATA struct Trigger gGondowanCliffs_PassTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 11, CONDITION_ALWAYS, EVENT(GondowanCliffs_GrindTheRock)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGondowanCliffs_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, IF_FLAG_SET(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN), 3},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(GondowanCliffs_TalkToDog)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_GONDOWAN_CLIFFS_GEYSER_DUG, MSGNO(MsgDigHereYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgIKnewIt)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgITriedTo)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgIWonderWho)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgIfTheOther)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgILongTo)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(GondowanCliffs_FightKindle)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 10, 0x200, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, FLAG_FIRST_PUDDLE_FROZEN, EVENT(GondowanCliffs_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, FLAG_SECOND_PUDDLE_FROZEN, EVENT(GondowanCliffs_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, CONDITION_ALWAYS, EVENT(GondowanCliffs_BlowAwayFoliage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, FLAG_GONDOWAN_CLIFFS_PILLAR_IN_WATER, EVENT(GondowanCliffs_PushPillarIntoWater)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, FLAG_GONDOWAN_CLIFFS_GEYSER_DUG, EVENT(GondowanCliffs_ScoopUpGeyser)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 gGondowanCliffs_MapRects[] = {
    0x2000200, 0x3600400, 0x600, 0x1601000, -1, -1,
};
// clang-format on
