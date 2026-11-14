/// @file
/// The room of grey blocks and locked doors in Jupiter Lighthouse, between
/// rooms 3 and 5. The Red Key and the Blue Key open its two doors
/// (JupiterLighthouse_UseRedKey, JupiterLighthouse_UseBlueKey), Reveal shows a
/// hidden passage and its block (JupiterLighthouse_RevealThePassage), and once
/// the lighthouse is activated its blocks hover, Move setting the fifth on its
/// cell to bridge the gap (JupiterLighthouse_FinishMovingBlock). Two Cyclone
/// teleporters lead on (JupiterLighthouse_LeaveByFirstCyclone,
/// JupiterLighthouse_LeaveBySecondCyclone), and motes of light circle the room
/// (JupiterLighthouse_SpawnCirclingMotes).
///
/// The cracked tiles' code is in
/// jupiter_lighthouse_room_4/, in two parts the shared field effect objects
/// link between.

#include "shared/tables.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "field/event_inline.h"
#include "system/iwram_call.h"
#include "actor.h"
#include "field_sprite.h"
#include "macros.h"
#include "system/main_routine_work.h"
#include "field/field_sequence_event.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "call.h"
#include "system/field_effect.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "item_icon.h"
#include "field/cell_target.h"
#include "objcmd.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/frame.h"
#include "fixed_math.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "module/field/field_effect.h"
#include "text/msg_ids.h"

// Set as each locked door opens to its key.
#define FLAG_RED_DOOR_OPENED 0xa07
#define FLAG_BLUE_DOOR_OPENED 0xa08
// Set once Move has set the fifth block on its cell, where it rises to bridge
// the gap.
#define FLAG_BRIDGE_BLOCK_RAISED 0xa06

/// The grey blocks, numbering the rows of gActorPlacements.
enum JupiterLighthouseBlockId {
  /// The first of the six blocks that rise once the lighthouse is activated.
  ACTOR_FIRST_BLOCK = ACTOR_FIRST_PLACED,
  /// The block Reveal shows on the hidden passage.
  ACTOR_HIDDEN_BLOCK = ACTOR_FIRST_PLACED + 6,
  /// The block Move sets on its cell to bridge the gap.
  ACTOR_BRIDGE_BLOCK,
  /// The first of three more that rise once the lighthouse is activated.
  ACTOR_LAST_BLOCKS,
  /// An actor with no picture, raised with the blocks and given gravity.
  // XXX: what it stands for; a render of the activated room would settle it.
  ACTOR_UNSEEN = ACTOR_LAST_BLOCKS + 3
};

void Actor_RaiseFromCell(s32 id);
extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct LightOam gLightSpriteOam[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;
void JupiterLighthouse_HideThePassage(void);
void JupiterLighthouse_LeaveByFirstCyclone(void);
void JupiterLighthouse_LeaveBySecondCyclone(void);
void JupiterLighthouse_TryLockedDoor(void);
void LightSprites_InitA(void);
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void MapGrid_SetCellHeight();
extern s32 gWhirlwindRiding;

void Actor_ArriveInRing(s32 id);
void MapGrid_SetAttributeBAt();

extern u16 gPaletteCycleColors[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

static inline void SetCollisionCode(s32 x, s32 z, s32 layer, s32 value) {
  /* FAKEMATCH(wrapper): the call forwarded through this inline. */
  ((u8)MapGrid_SetCellBehavior(x, z, layer, value));
}

void JupiterLighthouse_RevealThePassage(void) {
  MapGrid_CopyCellsLayered(17, 10, 1, 4, 19, 10);
  MapGrid_CopyCellAttributes(17, 10, 1, 1, 19, 10);
  SetCollisionCode(0x1300000, 0xa00000, 0, 254);
  Actor_SetPosition(ACTOR_HIDDEN_BLOCK, 504, 168);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    Actor_RaiseFromCell(ACTOR_HIDDEN_BLOCK);
    ((u8)FieldMapGrid_SetCellBehavior(px(496), px(160), 0, 254));
  }
}

void JupiterLighthouse_HideThePassage(void) {
  MapGrid_CopyCellsLayered(16, 10, 1, 4, 19, 10);
  MapGrid_CopyCellAttributes(16, 10, 1, 1, 19, 10);
  Actor_SetPosition(ACTOR_HIDDEN_BLOCK, 0, 0);
  MapGrid_CopyCellAttributes(30, 10, 1, 1, 31, 10);
}

void JupiterLighthouse_StartMovingBlock(s32 unused, s32 id) {
  Actor_SetSpritePriority(id, 2);
}

void JupiterLighthouse_FinishMovingBlock(s32 unused, s32 id) {
  struct Actor *object;
  s32 i;

  object = Actor_Get(id);
  if (object->x >> 20 == 10 && object->z >> 20 == 32) {
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += px(2);
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(FLAG_BRIDGE_BLOCK_RAISED);
  }
}

void JupiterLighthouse_LeaveByFirstCyclone(void) {
  Import_EventSetExitRequest(12);
}

void JupiterLighthouse_LeaveBySecondCyclone(void) {
  Import_EventSetExitRequest(13);
}

void JupiterLighthouse_OpenRedDoor(void) {
  MapGrid_CopyCellsLayered(73, 18, 1, 4, 74, 18);
  MapGrid_CopyCellAttributes(10, 9, 1, 1, 10, 20);
}

void JupiterLighthouse_OpenBlueDoor(void) {
  MapGrid_CopyCellsLayered(103, 18, 1, 4, 104, 18);
  MapGrid_CopyCellAttributes(40, 9, 1, 1, 40, 20);
}

void JupiterLighthouse_UseRedKey(s32 item) {
  Event_SetFlag(FLAG_RED_DOOR_OPENED);
  Event_PlaySound(SE_HEAVY_IMPACT);
  JupiterLighthouse_OpenRedDoor();
  PartyInventory_Remove(item);
}

void JupiterLighthouse_UseBlueKey(s32 item) {
  Event_SetFlag(FLAG_BLUE_DOOR_OPENED);
  Event_PlaySound(SE_HEAVY_IMPACT);
  JupiterLighthouse_OpenBlueDoor();
  PartyInventory_Remove(item);
}

void JupiterLighthouse_TryLockedDoor(void) {
  gFieldWork->fieldMenu = 1;
  Message_ShowCentered(MsgFieldDoorLocked, 1);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// A particle's step, Jupiter Lighthouse room 2's
/// JupiterLighthouse_CircleTheMote with other numbers: it circles at its angle
/// (action) around x 232 and rises with its phase (unk66) from 120 pixels down,
/// taking mode 7 at phase 1 and mode 2 at phase 38 and starting over once above
/// the ground; drawn in front while the sine is negative and behind otherwise.
void JupiterLighthouse_CircleTheMote(struct Actor *object) {
  s16 angle = object->timer;
  s16 height = object->unk66;

  SET_POSITION(object,
               Iwram_MulQ16(Trig_Cos(angle), object->speedLimit) + (232 << 16),
               (height * 3 << 16) - (120 << 16), 130 << 18);
  if ((s16)object->unk66 == 1)
    Actor_SelectAnimation(object, 7);
  if ((s16)object->unk66 == 38)
    Actor_SelectAnimation(object, 2);
  if (object->y > 0)
    object->unk66 = 0xffff;
  if (Math_Sin(angle) < 0)
    ((struct FieldSprite *)object->sprite)->priority = 1;
  else
    ((struct FieldSprite *)object->sprite)->priority = 3;
  object->timer += object->acceleration;
  object->unk66++;
}

void JupiterLighthouse_SpawnCirclingMotes(void) {
  struct Actor *object;
  s32 i;
  s32 phase;

  i = 0;
  for (phase = 0; i != 7; i++) {
    object = (FieldActor_Create(672, 0, 0, 0));
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (Random16() << 2) + 0x60000;
    object->acceleration = ((Random16() << 11) >> 16) + 0x800;
    object->update = (void (*)(void))JupiterLighthouse_CircleTheMote;
    object->timer = Random16();
    object->unk66 = phase;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, 2);
    ((struct FieldSprite *)object->sprite)->priority = 1;
    phase += 5;
  }
}

s32 Scene_Initialize(void) {
  struct Actor *leader;
  s32 flag;
  s32 i;

  gFieldWork->transition = 0x204;
  JupiterLighthouse_HideThePassage();
  if (Event_GetFlag(FLAG_RED_DOOR_OPENED))
    JupiterLighthouse_OpenRedDoor();
  if (Event_GetFlag(FLAG_BLUE_DOOR_OPENED))
    JupiterLighthouse_OpenBlueDoor();
  JupiterLighthouse_SpawnCirclingMotes();
  flag = Event_GetFlag(FLAG_KEEP_PARTY_POSITION);
  if (flag == 0 && gMainRoutineWork.entrance == 11) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    leader->priorityFlags &= ~64;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
    Actor_SetAnimationSpeed(leader, 64);
    leader->motionFlags = flag;
    leader->y = leader->terrainHeight = -px(120);
    Event_StartScreenTransition();
    FieldEvent_Wait(10);
    for (i = 0; i <= 89 && leader->y < 0; i++) {
      leader->terrainHeight = leader->y = leader->y + px(3);
      leader->facing += 0x1800;
      System_Wait(2);
    }
    leader->terrainHeight = leader->y = 0;
    leader->facing = 0x4000;
    leader->motionFlags = 3;
    leader->velocityY = px(3);
    leader->gravity = px(1) / 2;
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    FieldEvent_Wait(40);
    leader->gravity = px(1);
    leader->priorityFlags |= 64;
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
    Event_End();
  }
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineWork.entrance == 12)
    Actor_ArriveInRing(gMainRoutineWork.currentOwner);
  return 0;
}

void JupiterLighthouse_LoadPalettes(s32 value);

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 1);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 2);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 3);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 4);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 5);
    Actor_RaiseFromCell(ACTOR_LAST_BLOCKS);
    Actor_RaiseFromCell(ACTOR_LAST_BLOCKS + 1);
    Actor_RaiseFromCell(ACTOR_LAST_BLOCKS + 2);
    Actor_RaiseFromCell(ACTOR_UNSEEN);
    Actor_Get(ACTOR_UNSEEN)->gravity = px(3);
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_UNSEEN), 0);
  if (Event_GetFlag(FLAG_BRIDGE_BLOCK_RAISED)) {
    Actor_SetPosition(ACTOR_BRIDGE_BLOCK, 168, 520);
    Actor_RaiseFromCell(ACTOR_BRIDGE_BLOCK);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

void Whirlwind_UpdateLower(struct Actor *self) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(SE_WATER_LEVEL);
  {
    s8 *to = self->sprite;
    s8 *from = leader->sprite;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (MapGrid_GetCellBehavior(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Camera_WorldToScreen(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Position_FlatDistance(&leader->x, &self->x) <= 12 &&
            distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->facing = self->facing;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            work->raisedTrigger = delay;
          }
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      {
        s32 (*multiply)(s32, s32);
        s32 cosine = Trig_Cos(self->facing);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(dead): the routine address taken inside a once-only
         * block. */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x += multiply(cosine, speed);
      }
      self->z += Iwram_MulQ16(Math_Sin(self->facing), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Actor_StartScript(self, gWhirlwindEndScript);
    Event_PlaySound(SE_SPARKLING_PASSAGE);
    gWhirlwindRiding = zero;
  }
}

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

/// Unused: every eighth frame, sends a spark up from a random spot about the
/// actor.
UNUSED s32 Actor_EmitRisingSparks(struct Actor *object) {
  struct EffectOptions options;
  s32 x;
  s32 y;
  s32 velocityY;

  if ((gFrameCount & 7) == 0) {
    options.priority = 3;
    options.startScaleX = 0xb333;
    options.startScaleY = 0xb333;
    options.palette = 14;
    x = object->x + ((((Random16() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - ((Random16() * 20) >> 16)) << 16);
    velocityY = (s32)(((((Random16() * 5) >> 16) << 16) + 0x50000)) / 10;
    FieldEffect_Spawn(x, y, object->z, 0, velocityY, 0,
                      EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_START_SCALE,
                      &options);
  }
  return 0;
}

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

/// Unused here: places the aerie's two angels and the lid between them, each
/// angel the archers have energized raised with its half of the lid, and all
/// three lifted clear once both are. The aerie's beacon floor keeps the same
/// routine.
UNUSED void JupiterLighthouse_PlaceAngelsAndLid(void) {
  struct Actor *left = Actor_Get(9);
  struct Actor *right = Actor_Get(10);
  struct Actor *rider = Actor_Get(23);
  struct FieldSprite *sprite = rider->sprite;

  Actor_SetPosition(24, 512, 248);
  Actor_SetPosition(23, 512, 248);
  Actor_SetPosition(9, 472, 256);
  Actor_SetPosition(10, 552, 256);
  Actor_Get(24)->scaleX = 0;
  rider->motionFlags = 0;
  rider->y = rider->terrainHeight;
  left->motionFlags = 0;
  left->y = left->terrainHeight;
  right->motionFlags = 0;
  right->y = right->terrainHeight;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED)) {
    left->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += -0x28f40;
    sprite->rotation += 0x800;
    Actor_SetSpriteColorMode(Actor_Get(9), 4);
    Actor_SetSpriteColorMode(Actor_Get(11), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    right->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += 0x28f40;
    sprite->rotation -= 0x800;
    Actor_SetSpriteColorMode(Actor_Get(10), 4);
    Actor_SetSpriteColorMode(Actor_Get(12), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED) &&
      Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    left->y += 0x199900;
    right->y += 0x199900;
    rider->y += 0x1b8480;
    left->motionFlags = 4;
    right->motionFlags = 4;
    rider->motionFlags = 4;
  }
}

extern struct Actor *gLightSpriteActors[];

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
void LightSprites_Draw(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gLightSpriteTile].offset >> 5;
  viewY = map->origin[1];
  oam = gLightSpriteOam;

  for (i = 0; i < gLightSpriteCount; i++) {
    struct Actor *actor = gLightSpriteActors[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gLightSpritesLarge;
      if (big == 0) {
        s32 x, sy;
        frame = actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 16;
        if ((u32)(x + 7) > 334 || oy <= -16 || oy > 239)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0x40002000;
        *word = (tile + frame * 8) | 0x800;
        if (gLightSpritePriority == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gLightSpritePriority;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      } else {
        s32 x, sy;
        frame = actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 64;
        if ((u32)(x + 23) > 350 || oy <= -64 || oy > 175)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0xc000a000;
        *word = (tile + frame * 8) | 0x800;
        if (gLightSpritePriority == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gLightSpritePriority;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      }
    }
  }
}

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

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

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoom4),
    SCENE_EXIT(1, JupiterLighthouseRoom5, 1),
    SCENE_EXIT(2, JupiterLighthouseRoom3, 2),
    SCENE_EXIT(3, JupiterLighthouseRoom4, 4),
    SCENE_EXIT(4, JupiterLighthouseRoom4, 3),
    SCENE_EXIT(5, JupiterLighthouseRoom4, 6),
    SCENE_EXIT(6, JupiterLighthouseRoom4, 5),
    SCENE_EXIT(7, JupiterLighthouseRoom4, 8),
    SCENE_EXIT(8, JupiterLighthouseRoom4, 7),
    SCENE_EXIT(9, JupiterLighthouseRoomM, 9),
    SCENE_EXIT(10, JupiterLighthouseRoomM, 10),
    SCENE_EXIT(11, JupiterLighthouseRoom4, 11),
    SCENE_EXIT(12, JupiterLighthouseRoom3, 12),
    SCENE_EXIT(13, JupiterLighthouseRoom3, 13),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 136, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 136, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 168, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 168, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 200, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 200, FACING_EAST, TALK_FACING_KEEP, 1),
    {316, CONDITION_ALWAYS, 1, -0x80000, 0, -0x80000, FACING_EAST, 2, 1},
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 520, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 520, FACING_SOUTHEAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 520, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(316, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 520, FACING_EAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 520, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXAMINE, 20, FLAG_RED_DOOR_OPENED, EVENT(JupiterLighthouse_TryLockedDoor)},
    {TRIGGER_EXAMINE, 21, FLAG_BLUE_DOOR_OPENED, EVENT(JupiterLighthouse_TryLockedDoor)},
    {USE_ITEM(243), 20, FLAG_RED_DOOR_OPENED, EVENT(JupiterLighthouse_UseRedKey)}, // the Red Key
    {USE_ITEM(244), 21, FLAG_BLUE_DOOR_OPENED, EVENT(JupiterLighthouse_UseBlueKey)}, // the Blue Key
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RevealThePassage)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_HideThePassage)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByFirstCyclone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveBySecondCyclone)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#include "../field/field_effect/move.c"

#undef FLAG_RED_DOOR_OPENED
#undef FLAG_BLUE_DOOR_OPENED
#undef FLAG_BRIDGE_BLOCK_RAISED
// The room of grey blocks and locked doors in Jupiter Lighthouse: its copy of
// the effect spawner, and the cracked tiles, found, cracked and fallen
// through.

struct CellTarget;
void Leader_CenterInCell(void);
extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SelectAnimation(struct Actor *object, s32 mode);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      ((struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner));
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
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
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
    FieldActor_SetSpriteColor(effect, options->palette);
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

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - Q16_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - Q16_ONE) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

// The cells CrackedTiles_Step watches, and the cell it copies over the one the
// leader steps on.
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

#include "shared/cracked_tiles_find.h"

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect/advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
// The room of grey blocks and locked doors in Jupiter Lighthouse: the cracked
// tiles' steps and the fall to the floor below.

struct CellTarget;
struct CellTargetEntry;
extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);
void JupiterLighthouse_HideThePassage(void);
void JupiterLighthouse_LeaveByFirstCyclone(void);
void JupiterLighthouse_LeaveBySecondCyclone(void);
void JupiterLighthouse_TryLockedDoor(void);
extern u32 gFieldEffect_LongScript[];

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

s32 CrackedTiles_Step(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *puzzle = &gCrackedTiles;
  struct CellTargetEntry *tile = puzzle->entries;
  s32 i;

  for (i = 0; i < puzzle->count; i++, tile++) {
    if (tile->x == x && tile->z == z) {
      if (!Event_GetFlag(tile->flag)) {
        CrackedTile_Crack(puzzle, tile);
        Event_SetFlag(tile->flag);
        puzzle->current = i;
        break;
      }
      if (i == puzzle->current)
        break;
      puzzle->current = i;
      CrackedTile_Fall(puzzle, tile);
      GameFlag_SetByte(puzzle->flagByte, x);
      GameFlag_SetByte(puzzle->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

void CrackedTile_PlaceLanding(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->fieldWork;
  struct CellTarget *work = &gCrackedTiles;
  s32 x = ((s32)FieldGameFlag_GetByte(work->flagByte));
  s32 z = ((s32)FieldGameFlag_GetByte(work->flagByte + 8));

  if (x != 0 || z != 0) {
    struct Actor *shadow;

    GameFlag_SetByte(work->flagByte, 0);
    GameFlag_SetByte(work->flagByte + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    shadow = *(struct Actor **)(event + 460);
    shadow->x = object->x;
    shadow->z = object->z;
    Map_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    gFieldWork->transition = value;
  }
}
