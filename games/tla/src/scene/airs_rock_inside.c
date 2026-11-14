/// @file
/// Air's Rock's inside, the rooms of this overlay within the rock in Osenia:
/// halls of tightropes, pillars to Move and twisters to Whirlwind. At its heart
/// Sheba reads the tablet and learns Reveal (AirsRockInside_ReadRevealTablet,
/// walked to by AirsRockInside_WalkToRevealTablet), and a Psynergy Stone
/// restores the party's PP (AirsRockInside_CheckPsynergyStone).

#include "field/emote.h"
#include "sounds.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/frame.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct AirsRockInsideMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct AirsRockInsideMapWork {
  u8 unk000[0x108];
  struct AirsRockInsideMapLayer layers[3];
};

extern char SceneId_AirsRockInside1;
extern char SceneId_AirsRockInside2;
extern char SceneId_AirsRockInside3;
extern char SceneId_AirsRockInside5;
extern char SceneId_AirsRockInside6;
extern char SceneId_AirsRockInside7;
extern char SceneId_AirsRockInside8;
extern char SceneId_AirsRockInside9;
extern char SceneId_AirsRockInside10;
extern u8 gMapCellBuffer[];

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSpriteFlags(struct Actor *object, u32 flags);
void Actor_SetSpriteColor(struct Actor *object, s32 attribute);
void Actor_SetBlendMode(struct Actor *object, u32 value);
struct Actor *Actor_GetById(u32 objectId);
s32 GameFlag_Get(s32 flag);
void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
void FieldEvent_Wait(s32 frames);
void FieldMessage_Show(const u8 *message, s32 mode);
void Actor_SelectAnimation(struct Actor *object, s32 value);
void Actor_StartScript(struct Actor *object, const u8 *data);
void MapGrid_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Party_AddPp(s32 a, s32 b);
void Sound_Play(s32 value);

// The cell stampers keep their last cell and the block they copy.
extern s32 gData_02006c0c;
extern s32 gData_020073dc;
extern s32 gData_020073e0;
extern s32 gData_020073e4;
extern s32 gData_020073e8;

void MapGrid_CopyCellPictureCellsRect(s32 srcX, s32 src_z, s32 width,
                                      s32 height, s32 destX, s32 destZ);

static inline void CopyCells(s32 srcX, s32 srcZ, s32 width, s32 height,
                             s32 destX, s32 destZ) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline. */
  /* FAKEMATCH(wrapper): CopyCells forwarded inline. */
  MapGrid_CopyCellPictureCellsRect(srcX, srcZ, width, height, destX, destZ);
}
#include "system/system.h"
#include "actor.h"
#include "types.h"
#include "scene_tables.h"
#include "module/scene/scene.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "map.h"
#include "flags.h"
#include "module/field/field.h"
#include "module/tla.h"
#include "field_sprite.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"
#include "call.h"
#include "system/main_routine_work.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "field/field_sequence_event.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "macros.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/task.h"
#include "field/field_sequence_event.h"
#include "flag_call.h"
#include "module/field/event_commands.h"
#include "module/field/object.h"
#include "module/system/system.h"
#include "graphics/resource.h"
#include "bg_scroll.h"
#include "system/io_reg.h"
#include "system/frame.h"
#include "version.h"
#include "shared/tables.h"
#include "module/field/field_effect.h"

struct Actor;

struct AirsRockInsideVector {
  s32 x;
  s32 y;
  s32 z;
};

struct AirsRockInsideSpawn {
  s32 mode;
  s32 unk04;
  s32 scaleX;
  s32 scaleY;
  s32 speed;
  s32 acceleration;
  u16 kind;
  const u8 *script;
  s32 unk20;
  void *update;
};

struct SparkBytes {
  u8 unk00[0x62];
  u8 unk62;
  u8 unk63;
};

union SparkView {
  struct Actor actor;
  struct SparkBytes bytes;
};

extern struct ActorPlacement gAirsRockInside_Placements[];
extern struct ActorPlacement gAirsRockInside_Room1Placements[];
extern struct ActorPlacement gAirsRockInside_Room2Placements[];
extern struct ActorPlacement gAirsRockInside_Room3Placements[];
extern struct ActorPlacement gAirsRockInside_Room5Placements[];

void Func_020026c0(void);

s32 Actor_GetCellIndex(struct Actor *object);

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

void Func_02002aa8(s32 x, s32 z);
void Func_02002b58(s32 x, s32 z);

void WindowColor_Apply(s32 hue, s32 brightness);

s32 Func_02000690(u32 layer, s32 x, s32 y);

extern s16 gData_02006050[];
extern s16 gData_02006054[];
extern s16 gData_0200605a[];
extern s16 gData_02006064[];
extern s16 gData_02006068[];
extern s16 gData_02006072[];
extern s16 gData_0200607e[];

void Actor_LiftTargetBlock(const s16 *list);
void Actor_PlaceMovedBlock(void);

void Func_02002790(void);
void AirsRockInside_ReadRevealTablet(void);

extern struct Trigger gAirsRockInside_Room1Triggers[];
extern struct Trigger gAirsRockInside_Room2Triggers[];
extern struct Trigger gAirsRockInside_Room3Triggers[];
extern struct Trigger gAirsRockInside_Room4Triggers[];
extern struct Trigger gAirsRockInside_Room5Triggers[];
extern struct Trigger gAirsRockInside_Room6Triggers[];
extern struct Trigger gAirsRockInside_Room7Triggers[];
extern struct Trigger gAirsRockInside_Room8Triggers[];
extern struct Trigger gAirsRockInside_Room9Triggers[];
extern struct Trigger gAirsRockInside_Room10Triggers[];

void FieldEffect_SetAttached(u32 id, s32 mode);
void MapGrid_WaitForShakeToSettle(void);

void Func_02002770(void);
void Func_02004f3c(void);
void Func_02004eb0(void);

void Actor_PlaceBlocks(u16 *actors);

void Func_02004cc8(void);

void Scene_QueueMusic(s32 id);
void Tightrope_Initialize(void);
s32 Tightrope_Place(s32 slot, s32 a, s32 b);

void Func_02000908(u16 *list);
void Func_0200061c(void);
void Func_02000988(void);
void Func_02001828(void);

void Func_02002898(void);
void Func_02002980(void);
void Func_02002a14(void);
void Func_02002cf0(void);
void Func_02003170(void);
void Func_02003254(void);
void Func_0200332c(void);
void Func_02003654(void);
void Func_02003ac4(void);
void Func_02003b08(void);
void Func_02003cac(void);
void Func_02003d40(void);
void Func_02003dd4(void);
void Func_02003e28(void);
void AirsRockInside_WalkToRevealTablet(void);
void Func_020047b0(void);
void Func_02004818(void);
void Func_02004884(void);
void Func_020048bc(void);
void Func_020048c4(void);
void Func_02004ab8(void);
void AirsRockInside_CheckPsynergyStone(void);
void Func_02004880(void);

extern u32 gData_02005fd8[];
extern u32 gData_020061f8[];

void MapGrid_SetCellHeight(s32 x, s32 z, s32 terrain, s32 height);
struct Actor *FieldCamera_GetActor(void);
void Event_PlayCueNearFocus(struct Actor *object, s32 cue);
s32 Actor_HasArrived(struct Actor *object);

void Func_020006cc(struct Actor *object);

void Func_02000038(s16 *list) {
  s16 *entry;

  if ((u16)list[0] == 0xffff)
    return;
  entry = list;
next:
  {
    s32 id = (u16)entry[0];
    struct Actor *object = Actor_GetById(id);
    s32 kind = entry[1];
    s32 flag = entry[2];

    object->collisionFlags &= ~1;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetCellOpen(id, (u16)kind, (u16)flag, BYTE_AT(object, 34));
    entry += 3;
  }
  if ((u16)entry[0] != 0xffff)
    goto next;
}

// Finds the current target's table row and shrinks it.
void Func_020000a4(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  while (*list != 0xffff) {
    s32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        GameFlag_Set(flag);
        Actor_SetCellOpen(id, a, flag, object->terrainId);
        object->scaleY = Q16_ONE;
      }
      break;
    }
    list += 3;
  }
}

#include "shared/actor_set_cell_open.h"

#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect/move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

extern struct ActorPlacement gAirsRockInside_Room4Placements[];
extern struct ActorPlacement gAirsRockInside_Room6Placements[];
extern struct ActorPlacement gAirsRockInside_Room7Placements[];
extern struct ActorPlacement gAirsRockInside_Room8Placements[];
extern s32 gAirsRockInside_Room9Placements[];

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);

// On most frames, moves the object to a random spot: x 176 to 431 pixels, y
// and z up to 31 and 127 pixels past the leader's. Then it keeps its action as
// the previous one and sets mode 9 after action 212, mode 10 otherwise.
void Func_020004d4(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *self = (struct Actor *)object;

  if (Random16() & 0xff) {
    object->x = ((Random16() & 0xff) << 16) + (176 << 16);
    {
      s32 r = Random16();
      /* FAKEMATCH(pin): the leader's y pinned to r3. */
      register s32 y asm("r3") = leader->y;

      object->y = y + ((r & 31) << 16);
    }
    {
      s32 r = Random16();
      /* FAKEMATCH(pin): as for y, the leader's z pinned to r3. */
      register s32 z asm("r3") = leader->z;

      object->z = z + ((r & 127) << 16);
    }
  }
  self->unk66 = self->timer;
  if (self->timer == 212)
    Actor_SelectAnimation(object, 9);
  else
    Actor_SelectAnimation(object, 10);
}

void Func_02000550(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0) {
    Actor_SetAnimationSpeed(object, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  TilesetAnim_ResumeAll();
  if (BYTE_AT(object, 99) != 0) {
    if ((gFrameCount & 7) == 0)
      BYTE_AT(object, 99)--;
    return;
  }
  if (*(s16 *)((u8 *)object + 102) == 0)
    Func_020004d4(object);
  (*(s16 *)((u8 *)object + 102))--;
  /* FAKEMATCH(dead): an empty once-only block, whose loop note ends the
   * scheduling block. */
  do {
  } while (0);
  object->x += Trig_Cos(((object->z >> 14) + (object->y >> 15)) << 8) / 2;
  object->y += WORD_AT(object, 48);
}

void Func_02000610(void) { Event_ResolvePendingActions(0); }

void Func_0200061c(void) { BYTE_AT(Ram_HeapSlots->effectWork, 52) = 1; }

s32 Func_0200062c(u32 layer, s32 x, s32 y, s32 value) {
  struct AirsRockInsideMapWork *map =
      (struct AirsRockInsideMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}

#include "../field/object/wait_until_idle.c"

s32 Func_02000690(u32 layer, s32 x, s32 y) {
  struct AirsRockInsideMapWork *map =
      (struct AirsRockInsideMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  return (cell[2] << 8) | cell[3];
}

// Sets an object's cells by its flag (its action parameter): when the flag is
// set it clears the kind byte of its layer 2 cell and its layer 0 cell, sinks
// 4 pixels when not on its ground and takes sprite priority 3; otherwise it
// marks both cells (0xff00 and 0xd480), rises 4 pixels and takes priority 1;
// then it writes both cells back and moves the object's height
// (MapGrid_SetCellHeight).
void Func_020006cc(struct Actor *object) {
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 ground = Func_02000690(0, x, z);
  s32 cell = Func_02000690(2, x, z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;
  s32 set = GameFlag_Get(object->timer);
  u8 *flags = &object->priorityFlags;
  s32 px = x << 20;
  s32 pz = z << 20;

  if (set) {
    cell &= 0xffff00ff;
    ground = 0;
    if (object->terrainHeight >> 19 != height)
      height -= 4;
    // Through the field: through flags the byte, not the 2, would take the
    // result register.
    object->priorityFlags |= 2;
    Scene_SetActorSpritePriority(object, 3);
  } else {
    cell |= 0xff00;
    ground |= 0xd480;
    *flags &= ~2;
    height += 4;
    Scene_SetActorSpritePriority(object, 1);
  }
  Func_0200062c(0, x, z, ground);
  Func_0200062c(2, x, z, cell);
  MapGrid_SetCellHeight(px, pz, object->terrainId, height);
}

// One step of two paired objects trading places: the effect work's halfword
// at 26 names the first, and its partner is the other id of its even and odd
// pair. On step 0 the partner rises 28 pixels, its sprite tiles are cleared
// and the first's copied over them; on steps 1 to 5 it sinks and 0x30 words
// more are copied; through step 7 the first flattens. In mode 1 both settle,
// the first's flag is set and the partner's cleared, their animations step
// and their cells are written again (Func_020006cc).
void Func_020007b4(s32 mode, s32 step) {
  s16 *pair = (s16 *)((u8 *)Ram_HeapSlots->effectWork + 26);
  s32 even = *pair - ((*(u16 *)pair & 1) << 1);
  struct Actor *first = Actor_GetById(*pair);
  struct Actor *second = Actor_GetById(even + 1);
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramBlockCacheEntry *entry;

  if (step == 0) {
    firstSprite = first->sprite;
    secondSprite = second->sprite;
    second->motionFlags = 0;
    second->y += -px(28);
    wait(1);
    entry = &gResourceTableEntries[secondSprite->vramBlock];
    second->watchedObject = (void *)(entry->offset + OBJ_VRAM0);
    entry = &gResourceTableEntries[firstSprite->vramBlock];
    first->watchedObject = (void *)(entry->offset + OBJ_VRAM0);
    Dma_Fill32(step, second->watchedObject, entry->size);
    Dma_Set(first->watchedObject, second->watchedObject, DMA_COPY_WORDS(192),
            REG_DMA3);
    first->watchedObject = (void *)((u8 *)first->watchedObject + 0x300);
    second->watchedObject = (void *)((u8 *)second->watchedObject + 0x300);
  } else if (step <= 5) {
    second->y += 0x53333;
    Dma_Set(first->watchedObject, second->watchedObject, DMA_COPY_WORDS(48),
            REG_DMA3);
    first->watchedObject = (void *)((u8 *)first->watchedObject + 0x100);
    second->watchedObject = (void *)((u8 *)second->watchedObject + 0x100);
  }
  if (step <= 7)
    first->scaleY += -0x1f00;
  if (mode == 1) {
    first->scaleY = Q16_ONE;
    second->y = second->terrainHeight;
    GameFlag_Set(first->timer);
    GameFlag_Clear(second->timer);
    Actor_SelectAnimation(first,
                          ((struct FieldSprite *)first->sprite)->frame + 1);
    Actor_SelectAnimation(second,
                          ((struct FieldSprite *)second->sprite)->frame - 1);
    Func_020006cc(first);
    Func_020006cc(second);
  }
}

// Places the objects of a list ended by 0xffff: each entry (three halfwords)
// names an object, the first of its three script choices and the flag that
// picks between them; the object stops moving, loses its solid bit and keeps
// the flag as its action parameter.
void Func_02000908(u16 *list) {
  struct Actor *object;
  s32 flag, base;
  u16 *entry;

  if (*list == 0xffff)
    return;
  entry = list;
  do {
    object = Actor_GetById(*entry);
    base = ((s16 *)entry)[1];
    flag = ((s16 *)entry)[2];
    Actor_SetSpriteFlags(object, 0);
    {
      s32 choice = GameFlag_Get((u16)flag);

      // The two shifts stay statements: as (u16)base, base loses its rank.
      base <<= 16;
      base = (u32)base >> 16;
      base += base << 1;
      base += choice;
    }
    Actor_SelectAnimation(object, base);
    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    object->timer = flag;
    /* FAKEMATCH(wrapper): Func_020006cc called through Call1, the step behind
     * it. */
    Func_020006cc((s32)object);
    entry += 3;
  } while (*entry != 0xffff);
}

void FieldCamera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void FieldCamera_FollowActor(s32 id, s32 flag);

void Func_02000988(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *ride = leader->watchedObject;
  struct Actor *rider, *dust;
  u8 *riding, *motion, *priority;
  s32 rise, dx, dz, moved, i, height;
  u16 facing;
  s32 angle, noFlags;

  Event_Start();
  Event_ResolvePendingActions(0);
  motion = &leader->motionFlags;
  *motion = 4;
  Actor_SetSpriteFlags(leader, 0);
  GameFlag_Set(FLAG_SCENE_TEMP_50);
  Sound_Play(SE_137);
  riding = (u8 *)ride + 99;
  rise = 0;
  while (*riding != 0) {
    leader->x = ride->x;
    leader->y = ride->y + rise;
    leader->z = ride->z;
    if (rise <= 0xbffff)
      rise += 0x1999;
    leader->facing = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(SE_288);
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside1 &&
      leader->x >> 19 == 39) {
    Event_SetExitRequest(16);
    Event_End();
    return;
  }
  priority = &leader->priorityFlags;
  facing = ride->facing;
  switch (facing) {
  case 0xc000:
    rider = ride->watchedObject;
    dx = 0;
    dz = 0;
    *priority = 4;
    leader->x = (((leader->x >> 19) & ~1) + 1) << 19;
    i = 0;
    leader->y = (((ride->y >> 19) & ~1) + 3) << 19;
    leader->z = (((leader->z >> 19) & ~1) + 1) << 19;
  waiting:
    moved = 0;
    if (gInput.held & KEY_UP) {
      dx = 0;
      dz = 0xfff00000;
      leader->facing = 0xc000;
      moved = 1;
    }
    if (gInput.held & KEY_RIGHT) {
      dx = 0x100000;
      dz = 0;
      leader->facing = 0;
      moved = 1;
    }
    if (gInput.held & KEY_LEFT) {
      dx = 0xfff00000;
      leader->facing = 0x8000;
      dz = 0;
      moved = 1;
    }
    if (moved && Func_02000690(2, (ride->x + dx) >> 20,
                               (ride->z - ride->y + dz) >> 20) >>
                         8 !=
                     255)
      goto found;
    wait(1);
    i++;
    if (i != 60)
      goto waiting;
    dx = 0;
    dz = 0xfff00000;
    leader->facing = 0xc000;
    if ((u32)(Func_02000690(2, ride->x >> 20, (ride->z - ride->y + dz) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = 0x100000;
    dz = 0;
    leader->facing = 0;
    if ((u32)(Func_02000690(2, (ride->x + dx) >> 20, (ride->z - ride->y) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = 0xfff00000;
    leader->facing = 0x8000;
  found:
    height =
        Map_GetTerrainHeight(((u8 *)rider)[98], ride->x + dx, ride->z + dz);
    leader->y = height;
    leader->terrainHeight = height;
    Actor_SelectAnimation(leader, 6);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, 7);
    leader->speedLimit = px(2);
    leader->acceleration = px(1) / 2;
    leader->velocityY = px(4);
    *motion = 2;
    *priority = 1;
    leader->terrainId = ((u8 *)rider)[98];
    Actor_SetMoveTarget(leader, leader->x + dx, leader->y, leader->z + dz);
    OverlayObject_WaitUntilIdle(leader);
    Actor_WaitForMove(leader);
    Actor_SetSpriteFlags(leader, 1);
    *motion = 3;
    break;
  default:
    angle = ride->facing;
    leader->x = (((ride->x >> 19) & ~1) + (angle >>= 13) - 1) << 19;
    leader->z = (((ride->z >> 19) & ~1) - (angle & 2) + 1) << 19;
    noFlags = 0;
    Actor_SetSpriteFlags(leader, 1);
    leader->velocityY = px(6);
    *priority = 33;
    *motion = 3;
    Call4((void (*)())FieldCamera_MoveWithinMapBounds, -1, -1, -1, 0);
    dust = FieldCamera_GetActor();
    dust->speedLimit = px(8);
    dust->acceleration = px(1);
    dust->motionFlags = noFlags;
    Actor_SetMoveTarget(dust, leader->x,
                        Map_GetTerrainHeight(0, leader->x, leader->z),
                        leader->z);
    OverlayObject_WaitUntilIdle(leader);
    Actor_WaitForMove(dust);
    break;
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  GameFlag_Clear(FLAG_SCENE_TEMP_50);
  wait(10);
  Event_End();
}

#include "shared/position_flat_distance.h"

#include "shared/position_distance_above.h"

// Func_020010b4's twin for another shot: it also stops while the render mode
// is 5, and on a cell of kind 212 it ends with one of two end scripts by its
// angle; otherwise, the first time it reaches the leader (within 16 pixels on
// the same height when the leader climbs, else within 8 on the same level) and
// flag 0x104 is clear, it carries the leader along (counter 0x16a 200, render
// mode 0); it moves 1.5 pixels a frame along its angle, upward when its angle
// is 0xc000.

extern u32 gData_020061ac[];
extern u32 gData_02006194[];

s32 Trig_Sin(s32 angle);

// Func_020010b4's twin for another shot: it also stops while the render mode
// is 5, and on a cell of kind 212 it ends with one of two end scripts by its
// angle; otherwise, the first time it reaches the leader (within 16 pixels on
// the same height when the leader climbs, else within 8 on the same level) and
// flag 0x104 is clear, it carries the leader along (counter 0x16a 200, render
// mode 0); it moves 1.5 pixels a frame along its angle, upward when its angle
// is 0xc000.
void Func_02000d48(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 near;
  s32 *leaderPosition;
  s32 *shotPosition;
  s32 speed = 0x18000;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0 || gMainRoutineWork.renderMode == 5) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000690(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
    ((struct Shot *)shot)->unk63 = 0;
    shot->update = 0;
    Call2((void (*)())Event_PlayCueNearFocus, (s32)shot, 539);
    if (shot->facing == 0xc000)
      Actor_StartScript(shot, gData_020061ac);
    else
      Actor_StartScript(shot, gData_02006194);
    return;
  }
  if (((struct Shot *)shot)->unk62 == 0) {
    near = 0;
    leaderPosition = &leader->x;
    shotPosition = &shot->x;
    if (gMainRoutineWork.renderMode == 2) {
      if (Position_DistanceAbove(leaderPosition, shotPosition) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        near = 1;
    } else {
      if (Position_FlatDistance(leaderPosition, shotPosition) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        near = 1;
    }
    if (near && !GameFlag_Get(0x104)) {
      leader->facing = shot->facing;
      leader->watchedObject = shot;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->sprite)->priority =
          ((struct FieldSprite *)shot->sprite)->priority;
      work->raisedTrigger = 200;
      gMainRoutineWork.renderMode = 0;
      ((struct Shot *)shot)->unk62 = 1;
      ((struct Shot *)shot)->unk63 = 1;
    }
  }
  if (shot->facing == 0xc000) {
    shot->x += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->y -= Iwram_MulQ16(Trig_Sin(shot->facing), speed);
  } else {
    shot->x += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->z += Iwram_MulQ16(Trig_Sin(shot->facing), speed);
  }
}

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

// A launcher's update with a new shot each time: while none of the event work's
// four busy counters is set it counts frames in its action parameter, and at
// 240 launches a shot 8 pixels ahead along its angle with a dust effect below
// it.
void Func_02000f34(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0)
    return;
  if (object->timer == 240) {
    if (!GameFlag_Get(FLAG_SCENE_TEMP_50))
      Event_PlayCueNearFocus(object, 202);
    if (object->facing == 0xc000) {
      position.x = object->x;
      position.z = object->z;
    } else {
      /* FAKEMATCH(wrapper): forwarded. */
      position.x = object->x + Math_MulQ16(0x80000, Trig_Cos(object->facing));
      position.z = object->z + Math_MulQ16(0x80000, Trig_Sin(object->facing));
    }
    shot = (struct Actor *)Import_ActorCreate(280, position.x, object->y,
                                              position.z);
    Actor_SelectAnimation(shot, 2);
    Actor_SetSpriteFlags(shot, 0);
    ((struct FieldSprite *)shot->sprite)->priority = 1;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->facing = object->facing;
    shot->update = Func_02000d48;
    shot->watchedObject = object;
    object->timer = 0;
    options.priority = 1;
    options.palette = 7;
    FieldEffect_Spawn(position.x, object->y, position.z - px(6), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->timer++;
}

void Func_02001084(struct Actor *object, s16 timer, u8 value) {
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    BYTE_AT(object, 90) = 0;
    object->timer = timer;
    BYTE_AT(object, 98) = value;
    WORD_AT(object, 108) = (s32)Func_02000f34;
  }
}

extern u32 gData_020061cc[];

void Func_020010b4(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 near;
  s32 *leaderPosition;
  s32 *shotPosition;
  s32 speed = 0x18000;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0 || work->transitionActive == 0) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000690(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
    {
      s32 cue = 284;

      Event_PlayCueNearFocus(shot, cue + 255);
    }
    ((struct Shot *)shot)->unk63 = 0;
    shot->update = 0;
    Actor_StartScript(shot, gData_020061cc);
    /* FAKEMATCH(dead): an empty once-only block holds the next statement after
     * this one. */
    do {
    } while (0);
    return;
  }
  if (((struct Shot *)shot)->unk62 == 0) {
    near = 0;
    leaderPosition = &leader->x;
    shotPosition = &shot->x;
    if (gMainRoutineWork.renderMode == 2) {
      if (Position_DistanceAbove(leaderPosition, shotPosition) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        near = 1;
    } else {
      if (Position_FlatDistance(leaderPosition, shotPosition) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        near = 1;
    }
    if (near && !GameFlag_Get(0x104)) {
      leader->facing = shot->facing;
      leader->watchedObject = shot;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->sprite)->priority =
          ((struct FieldSprite *)shot->sprite)->priority;
      work->raisedTrigger = 200;
      gMainRoutineWork.renderMode = 0;
      ((struct Shot *)shot)->unk62 = 1;
      ((struct Shot *)shot)->unk63 = 1;
    }
  }
  if (shot->facing == 0xc000) {
    shot->x += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->y -= Iwram_MulQ16(Trig_Sin(shot->facing), speed);
  } else {
    shot->x += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->z += Iwram_MulQ16(Trig_Sin(shot->facing), speed);
  }
}

// An object's update: while none of the event work's four busy counters (0x15a,
// 0x15e, 0x166, 0x168) is set it counts frames in its action parameter, and at
// 240 (playing cue 202 unless flag 0x250 is set) it launches the first free one
// of objects 13 to 15 from 8 pixels ahead along its angle, with Func_020010b4
// as its update, and a dust effect below it.
void Func_02001290(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;
  s32 i;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0)
    return;
  for (i = 0; i != 3;)
    Actor_GetById(13 + i++);
  if (object->timer == 240) {
    shot = 0;
    if (!GameFlag_Get(FLAG_SCENE_TEMP_50))
      Event_PlayCueNearFocus(object, 202);
    if (object->facing == 0xc000) {
      position.x = object->x;
      position.z = object->z;
    } else {
      /* FAKEMATCH(wrapper): forwarded. */
      position.x = object->x + Math_MulQ16(0x80000, Trig_Cos(object->facing));
      position.z = object->z + Math_MulQ16(0x80000, Trig_Sin(object->facing));
    }
    for (i = 0; i != 3; i++) {
      shot = Actor_GetById(13 + i);
      if (shot->update == 0)
        break;
    }
    shot->x = position.x;
    shot->y = object->y;
    shot->z = position.z;
    Actor_SelectAnimation(shot, 2);
    Actor_SetSpriteFlags(shot, 0);
    ((struct FieldSprite *)shot->sprite)->priority = 1;
    shot->priorityFlags |= 32;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->facing = object->facing;
    shot->watchedObject = object;
    shot->update = Func_020010b4;
    object->timer = 0;
    options.priority = 1;
    options.palette = 7;
    FieldEffect_Spawn(position.x, object->y, position.z - px(6), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->timer++;
}

void Func_02001420(struct Actor *object, s16 timer, u8 value) {
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    BYTE_AT(object, 90) = 0;
    object->timer = timer;
    BYTE_AT(object, 98) = value;
    WORD_AT(object, 108) = (s32)Func_02001290;
    Actor_SetSpriteFlags(object, 0);
  }
}

void Func_02001458(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *other;
  s32 *pos;
  u8 i;

  if (*(s8 *)((u8 *)Ram_HeapSlots->block32 + 0x2a01) != 0)
    return;
  if (work->transitionActive == 0)
    return;
  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0) {
    Actor_SetAnimationSpeed(object, 0);
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  if (object->timer == 0) {
    if (object->x == 0) {
      Actor_Release(object);
    } else {
      object->z = 0;
      object->x = 0;
    }
    return;
  }
  if (Position_FlatDistance(&leader->x, pos = &object->x) <= 8 &&
      ((s16 *)&leader->y)[1] == ((s16 *)&object->y)[1]) {
    {
      s16 *counter = &work->raisedTrigger;
      s32 value = 201;

      *counter = value;
    }
    object->unk4c = 0;
    /* FAKEMATCH(barrier): an empty asm holds the next statement after this one.
     */
    asm("");
    object->speedLimit = -object->speedLimit >> 1;
    object->z = leader->z - px(8);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside3) {
    other = Actor_GetById(8);
    if (Position_FlatDistance(&other->x, pos) <= 8) {
      object->speedLimit = -object->speedLimit >> 1;
      object->unk4c = 0;
      object->z = other->z - px(8);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside8) {
    for (i = 0; i != 3; i++) {
      struct Actor *other2 = Actor_GetById(i + 12);
#define other other2
      if (Position_FlatDistance(&other->x, pos) <= 8) {
        object->speedLimit = -object->speedLimit >> 1;
        object->unk4c = 0;
        object->z = other->z - px(8);
      }
    }
#undef other
  }
  object->timer--;
  object->z += object->speedLimit;
  object->x += object->acceleration;
  object->speedLimit -= object->unk4c;
  object->scaleX += 0xccc;
  object->scaleY += 0xccc;
}

void Func_0200160c(s32 x, s32 y, s32 z) {
  struct Actor *object = Import_ActorCreate(0x11d, x, y, z);

  Actor_SelectAnimation(object, 1);
  Actor_SetSpriteFlags(object, 0);
  Actor_SetSpriteColor(object, 6);
  Actor_SetBlendMode(object, 1);
  object->timer = 27;
  object->motionFlags = 0;
  BYTE_AT(object, 90) = 0;
  WORD_AT(object, 24) = 0x8000;
  object->speedLimit = px(4);
  object->acceleration = ((Random16() & 0x7fff) << 1) + 0xffff8001;
  WORD_AT(object, 76) = 0x3800;
  WORD_AT(object, 108) = (s32)Func_02001458;
}

// Every eighth frame pair, while none of the event work's four busy counters
// is set, starts a drip (Func_0200160c) at a place chosen by scene and frame:
// in one room one of two spots by flag 0x89b, in the other one of six by the
// leader's position and flags 0x200 to 0x202. Places the two frame arms share
// are one call, reached by goto from the other arm.
void Func_02001690(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 frame;
  s32 scene;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0)
    return;
  frame = gFrameCount;
  if ((frame & 6) != 0)
    return;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_AirsRockInside3) {
    s32 x;

    if ((frame & 1) && !GameFlag_Get(0x89b))
      x = 0x1780000;
    else
      x = 0x1180000;
    Func_0200160c(x, 0, 0x2680000);
  } else if (scene == (s32)&SceneId_AirsRockInside8) {
    if (frame & 1) {
      if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 19 > 41) {
        if (Actor_GetById(gMainRoutineWork.currentOwner)->x >> 19 <= 44)
        dripA8:
          Func_0200160c(0xa80000, 0, 0x25b0000);
        else if (!GameFlag_Get(FLAG_SCENE_TEMP_01))
          Func_0200160c(0x1f80000, 0, 0x1c80000);
        else
          goto drip228;
      } else {
      drip268:
        Func_0200160c(0x2680000, -0x200000, 0x880000);
      }
    } else {
      if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 19 > 41) {
        if (Actor_GetById(gMainRoutineWork.currentOwner)->x >> 19 > 49)
        drip228:
          Func_0200160c(0x2280000, 0, 0x1c80000);
        else if (GameFlag_Get(FLAG_SCENE_TEMP_00))
          goto dripA8;
        else
          Func_0200160c(0x1080000, 0, 0x2580000);
      } else if (GameFlag_Get(FLAG_SCENE_TEMP_02)) {
        goto drip268;
      } else {
        Func_0200160c(0x2880000, -0x200000, 0x880000);
      }
    }
  }
}

void Func_020017f4(struct Actor *object) {
  struct Actor *target = (struct Actor *)WORD_AT(object, 104);

  object->priorityFlags ^= 2 << (gFrameCount & 1);
  SET_POSITION(object, target->x, target->y + (128 << 12), target->z);
}

void Func_02002bd0(struct Actor *object);
void Camera_SetSpeedParameters(s32 a, s32 b);
void MapGrid_SetScreenShake(s32 a, s32 b, s32 c);

#define TARGET(effect) ((effect)->target)

// The gust picks up an object it passes over, to drop it at cell `at`
// (x in the high half, z in the low).
#define PICK_UP(object, at)                                                    \
  do {                                                                         \
    (object)->watchedObject = gust;                                            \
    (object)->update = Func_020017f4;                                          \
    (object)->motionFlags = 0;                                                 \
    (object)->priorityFlags &= ~1;                                             \
    Actor_SetSpriteFlags(object, 0);                                           \
    ((struct FieldSprite *)(object)->sprite)->priority =                       \
        ((struct FieldSprite *)gust->sprite)->priority;                        \
    gust->watchedObject = (object);                                            \
    carried = (at);                                                            \
  } while (0)

// One shake of the walls (cue 157).
#define SHAKE()                                                                \
  (Sound_Play(SE_GATE_MOVE),                                                   \
   Call3(MapGrid_SetScreenShake, 0x50000, 0x50000, 0x10000),                   \
   Call3(MapGrid_SetScreenShake, -1, -1, 0xe666))

void Func_02001828(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectWork *effect = (struct EffectWork *)Ram_HeapSlots->effectWork;
  struct Actor *gust, *spark, *object;
  struct Vector3 velocity;
  struct EffectOptions leaves, dust;
  struct Cell dir, cell;
  s32 i, steps = 0, carried;
  s32 hit = 0;

  /* FAKEMATCH(wrapper): calls forwarded through call.h's Call2, Call3, Call4
   * and Call6. */
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_RUSH);
  gust =
      (struct Actor *)Import_ActorCreate(280, leader->x, leader->y, leader->z);
  Actor_SelectAnimation(gust, 2);
  Actor_SetSpriteFlags(gust, 0);
  gust->scaleX = 0x1b333;
  gust->scaleY = 0x3333;
  gust->motionFlags = 0;
  /* FAKEMATCH(signature): the target's word read as long, an alias set outside
   * struct Actor's. */
  ((struct FieldSprite *)gust->sprite)->priority =
      ((struct FieldSprite *)((struct Actor *)*(long *)&effect->target)->sprite)
          ->priority;
  Actor_SetMoveTarget(gust, TARGET(effect)->x, TARGET(effect)->y,
                      TARGET(effect)->z + 0xffff0000);
  gust->terrainHeight = Map_GetTerrainHeight(0, gust->x, gust->z);
  while (gust->scaleY <= 0x14ccb) {
    gust->scaleY += 0x1999;
    FieldEvent_Wait(1);
  }
  Actor_WaitForMove(gust);
  gust->z = TARGET(effect)->z + 0xffff0000;
  while (gust->scaleY <= 0x1cccb) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(
          0x2a0, TARGET(effect)->x, TARGET(effect)->y, TARGET(effect)->z);
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->terrainHeight = Map_GetTerrainHeight(0, spark->x, spark->z);
      spark->bounce = spark->x;
      spark->gravity = 0xcccc;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gData_02005fd8);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = Func_02002bd0;
    }
    if (gFrameCount & 1) {
      gust->scaleX += 655;
      gust->scaleY += 655;
      Actor_SetSpriteColor(TARGET(effect), 15);
    } else {
      Actor_SetSpriteColor(TARGET(effect), 4);
    }
    FieldEvent_Wait(1);
  }
  Actor_SetSpriteColor(TARGET(effect), 0);
  Sound_Play(SE_WHOOSH);
  cell.x = TARGET(effect)->x >> 20;
  cell.z = TARGET(effect)->z >> 20;
  if (leader->x >> 20 < cell.x) {
    dir.x = 1;
    dir.z = 0;
  } else if (leader->x >> 20 > cell.x) {
    dir.x = -1;
    dir.z = 0;
  } else if (leader->z >> 20 < cell.z) {
    dir.x = 0;
    dir.z = 1;
  } else {
    dir.x = 0;
    dir.z = -1;
  }
  do {
    hit = Func_02000690(2, cell.x, cell.z);
    hit >>= 8;
    if (Map_GetTerrainHeight(2, cell.x << 20, cell.z << 20) !=
        gust->terrainHeight)
      hit = 0;
    cell.x += dir.x;
    cell.z += dir.z;
    steps += 2;
  } while (hit == 0);
  Func_02000690(2, cell.x, cell.z);
  {
    s32 x;
    s32 z;

    steps -= 2;
    x = (TARGET(effect)->x >> 16) + ((steps * dir.x) << 3);
    z = (TARGET(effect)->z >> 16) + ((steps * dir.z) << 3);
    ((struct FieldSprite *)gust->sprite)->priority = 1;
    cell.x = x;
    cell.z = z;
    gust->speedLimit = px(4);
    gust->acceleration = px(4);
    Actor_SetMoveTarget(gust, cell.x << 16, gust->y, cell.z << 16);
    Call2((void (*)())Camera_SetSpeedParameters, 0x50000, 0x50000);
    Call4((void (*)())FieldCamera_MoveWithinMapBounds, x << 16, -1, z << 16, 1);
  }
  carried = 0;
  while (Actor_HasArrived(gust) == 0) {
    if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside6) {
      object = Actor_GetById(8);
      if (gust->x >> 19 == object->x >> 19 && carried == 0)
        PICK_UP(object, 0x270025);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside8) {
      object = Actor_GetById(9);
      if (gust->x >> 20 == object->x >> 20 &&
          gust->z >> 20 == object->z >> 20 && carried == 0)
        PICK_UP(object, 0x290069);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside9) {
      object = Actor_GetById(10);
      if (gust->x >> 20 == object->x >> 20 &&
          gust->z >> 20 == object->z >> 20 && carried == 0)
        PICK_UP(object, 0xf0033);
    }
    if ((gFrameCount & 1) == 0) {
      leaves.priority = 1;
      leaves.type = 674;
      leaves.palette = 2;
      velocity.x = Trig_Cos((gFrameCount & 0x1ff) << 12) << 1;
      velocity.y = gust->y + ((Random16() & 31) << 16);
      velocity.z = Trig_Sin((gFrameCount & 0x1ff) << 12);
      FieldEffect_Spawn(
          gust->x, velocity.y, gust->z, velocity.x, 0, velocity.z,
          EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &leaves);
    }
    FieldEvent_Wait(1);
  }
  if (carried != 0) {
    object = gust->watchedObject;
    object->x = (carried >> 16) << 19;
    object->z = carried << 19;
    object->velocityY = px(8);
    object->update = 0;
    object->motionFlags = 2;
    object->priorityFlags |= 1;
    object->priorityFlags &= ~6;
    Actor_SetSpriteFlags(object, 1);
    Scene_SetActorSpritePriority(object, 2);
    if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside6) {
      object->terrainHeight = 0xffe00000;
      Call6(Map_CopyCellAttributeRect, 19, 16, 1, 1, 19, 18);
      GameFlag_Set(0x89f);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside8) {
      Call6(Map_CopyCellAttributeRect, 19, 38, 1, 1, 20, 38);
      OverlayObject_WaitUntilIdle(object);
      wait(1);
      OverlayObject_WaitUntilIdle(object);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
      MapGrid_SetCellHeight(0x1400000, 0x3400000, 0, 4);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside9) {
      OverlayObject_WaitUntilIdle(object);
      wait(1);
      OverlayObject_WaitUntilIdle(object);
      Actor_SetSpriteFlags(object, 0);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
      MapGrid_SetCellHeight(0x780000, 0x1980000, 0, 4);
      if (GameFlag_Get(0x899))
        Func_02001084(object, 60, 0);
    }
  }
  if (hit == 255) {
    Sound_Play(SE_WHOOSH);
  } else {
    cell.x = (gust->x >> 16) + dir.x * 48;
    cell.z = (gust->z >> 16) + dir.z * 48;
    gust->speedLimit = px(1) / 2;
    gust->acceleration = px(1) / 2;
    Call4((void (*)())Actor_SetMoveTarget, (s32)(gust), (s32)(cell.x << 16),
          (s32)(gust->y), (s32)(cell.z << 16));
    hit -= 30;
    i = 0;
    while (Actor_HasArrived(gust) == 0) {
      if (hit > 4)
        goto shrink;
      if (i == 0) {
        Sound_Play(SE_235);
        i = 1;
      }
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        {
          dust.startScaleX = dust.startScaleY = (Random16() & 0xffff) + 0x10000;
        }
        velocity.x = Trig_Cos((gFrameCount & 0x1ff) << 12) << 1;
        velocity.y = px(1);
        velocity.z = Trig_Sin((gFrameCount & 0x1ff) << 12);
        FieldEffect_Spawn(gust->x, gust->y, gust->z, velocity.x, velocity.y,
                          velocity.z,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY, &dust);
      }
      if ((gFrameCount & 3) == 0) {
        if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside4) {
          GameFlag_Set(FLAG_AREA_TEMP_00);
          cell.x = (gust->x >> 20) + 64;
          cell.z = gust->z >> 20;
          MapGrid_CopyCellPictureIndicesRect(90, 25, cell.x + 1, cell.z, 1, 3);
          MapGrid_CopyCellPictureIndicesRect(21, 88, 24, 89, 1, 2);
          Map_CopyCellAttributeRect(23, 92, 2, 1, 23, 89);
          Map_CopyCellAttributeRect(18, 33, 2, 1, 23, 25);
        } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside7) {
          cell.x = gust->x >> 20;
          cell.z = ((gust->z - gust->y) >> 20) + 64;
          if (hit == 0) {
            GameFlag_Set(FLAG_AREA_TEMP_04);
            MapGrid_CopyCellPictureIndicesRect(39, 82, cell.x, cell.z, 1, 1);
            Map_CopyCellAttributeRect(39, 82, 1, 1, 38, 82);
            Call6(Map_CopyCellAttributeRect, 38, 19, 1, 1, 38, 18);
          } else {
            GameFlag_Set(FLAG_AREA_TEMP_05);
            Call6(MapGrid_CopyCellPictureIndicesRect, 39, 82, cell.x, cell.z, 1,
                  1);
            Call6(Map_CopyCellAttributeRect, 38, 98, 1, 1, 38, 99);
            Call6(Map_CopyCellAttributeRect, 38, 35, 1, 1, 38, 37);
          }
        } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside9) {
          GameFlag_Set(FLAG_AREA_TEMP_06);
          cell.x = gust->x >> 20;
          Call6(MapGrid_CopyCellPictureIndicesRect, 15, 80, cell.x, 77, 1, 3);
          Call6(Map_CopyCellAttributeRect, 15, 80, 1, 1, 15, 79);
          Call6(Map_CopyCellAttributeRect, 17, 15, 2, 1, 15, 15);
        }
        Map_CenterOnFollowedPoint();
      }
      FieldEvent_Wait(1);
    }
    Sound_Play(SE_298);
  }
shrink:
  gust->acceleration = 0;
  gust->speedLimit = 0;
  gust->targetZ = 0;
  gust->targetY = 0;
  gust->targetX = 0;
  for (i = 0; i != 8; i++) {
    gust->scaleX -= 0x2666;
    gust->scaleY -= Q16_ONE / 4;
    gust->y += 0x38000;
    FieldEvent_Wait(1);
  }
  Actor_Release(gust);
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside4) {
    if (hit == 5) {
      SHAKE();
      Call6(MapGrid_CopyCellPictureIndicesRect, 79, 0x40, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      Call6(MapGrid_CopyCellPictureIndicesRect, 69, 0x40, 29, 85, 5, 7);
      Call6(Map_CopyCellAttributeRect, 46, 20, 5, 5, 29, 21);
      Call6(Map_CopyCellAttributeRect, 0x20, 89, 1, 1, 31, 89);
      Call6(Map_CopyCellAttributeRect, 50, 86, 1, 1, 33, 87);
      GameFlag_Set(FLAG_AREA_TEMP_01);
    } else if (hit == 6) {
      SHAKE();
      Call6(MapGrid_CopyCellPictureIndicesRect, 74, 0x40, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      Call6(MapGrid_CopyCellPictureIndicesRect, 0x40, 0x40, 29, 85, 5, 7);
      Call6(Map_CopyCellAttributeRect, 52, 20, 5, 5, 29, 21);
      Call6(Map_CopyCellAttributeRect, 33, 88, 1, 1, 33, 87);
      Call6(Map_CopyCellAttributeRect, 0x30, 88, 1, 1, 31, 89);
      GameFlag_Clear(FLAG_AREA_TEMP_01);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside6) {
    if (hit == 5) {
      SHAKE();
      Call6(MapGrid_CopyCellPictureIndicesRect, 79, 0x40, 0x20, 79, 5, 7);
      if (Actor_GetById(8)->x >> 19 == 65) {
        velocity.x = Actor_GetById(8)->x + 0xfff80000;
        velocity.y = Actor_GetById(8)->y;
        velocity.z = Actor_GetById(8)->z + px(8);
        Actor_SetSpritePriority(8, 2);
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          FieldEffect_Spawn(velocity.x + ((Random16() & 15) << 16),
                            velocity.y + ((Random16() & 31) << 16), velocity.z,
                            0, 0, -0x8000, EFFECT_USE_PRIORITY, &dust);
        }
        wait(30);
        SHAKE();
        Call6(MapGrid_CopyCellPictureIndicesRect, 0x40, 0x40, 0x20, 79, 5, 7);
        Actor_GetById(8)->priorityFlags |= 1;
      } else {
        FieldEvent_Wait(30);
        SHAKE();
        Call6(MapGrid_CopyCellPictureIndicesRect, 69, 0x40, 0x20, 79, 5, 7);
        Call6(Map_CopyCellAttributeRect, 49, 15, 5, 5, 0x20, 15);
        Call6(Map_CopyCellAttributeRect, 33, 82, 1, 1, 34, 82);
        Call6(Map_CopyCellAttributeRect, 52, 81, 1, 1, 36, 81);
        GameFlag_Clear(FLAG_AREA_TEMP_02);
      }
    } else if (hit == 6) {
      SHAKE();
      if (Actor_GetById(8)->x >> 19 == 65) {
        velocity.x = Actor_GetById(8)->x + 0xfff80000;
        velocity.y = Actor_GetById(8)->y;
        velocity.z = Actor_GetById(8)->z;
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          FieldEffect_Spawn(velocity.x + ((Random16() & 15) << 16), velocity.y,
                            velocity.z - ((Random16() & 31) << 16), 0, 0,
                            -0x8000, EFFECT_USE_PRIORITY, &dust);
        }
        wait(20);
      } else {
        Call6(MapGrid_CopyCellPictureIndicesRect, 79, 0x40, 0x20, 79, 5, 7);
        Sound_Play(SE_GATE_MOVE);
        FieldEvent_Wait(30);
        SHAKE();
        Call6(MapGrid_CopyCellPictureIndicesRect, 0x40, 0x40, 0x20, 79, 5, 7);
        Call6(Map_CopyCellAttributeRect, 49, 21, 5, 5, 0x20, 15);
        Call6(Map_CopyCellAttributeRect, 35, 81, 1, 1, 36, 81);
        Call6(Map_CopyCellAttributeRect, 50, 82, 1, 1, 34, 82);
        GameFlag_Set(FLAG_AREA_TEMP_02);
      }
    }
  }
  FieldEvent_Wait(30);
  FieldCamera_MoveToActorById(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

extern struct ActorPlacement gData_02006238[];
extern s16 gData_02006268[];

const u8 *Scene_GetEntrances(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside1)
    return gData_02006268;
  return gData_02006238;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside1)
    return gAirsRockInside_Room1Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside2)
    return gAirsRockInside_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside3)
    return gAirsRockInside_Room3Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside4)
    return gAirsRockInside_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside5)
    return gAirsRockInside_Room5Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside6)
    return gAirsRockInside_Room6Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside7)
    return gAirsRockInside_Room7Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside8)
    return gAirsRockInside_Room8Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside9)
    return gAirsRockInside_Room9Placements;
  return gAirsRockInside_Placements;
}

void Func_020026c0(void) {
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;
  s32 last;
  s32 value;
  struct FieldWork *work;

  if (object->velocityX != 0 || object->velocityZ != 0)
    return;
  last = gData_02006c0c;
  if (last == -1 || key == last)
    return;
  x = last >> 16;
  z = last & 0xffff;
  gData_02006c0c = -1;
  CopyCells(73, 72, gData_020073dc, gData_020073e0, gData_020073e4,
            gData_020073e8);
  if (gMainRoutineWork.scene != (s32)&SceneId_AirsRockInside5)
    return;
  work = Ram_HeapSlots->fieldWork;
  value = Func_02000690(2, x, z) >> 8;
  if (value != 0) {
    /* FAKEMATCH(temporary): the value plus 200 held in delay before the store.
     */
    s32 delay = value + 200;
    work->raisedTrigger = delay;
  }
}

void Func_02002770(void) {
  s32 value = -1;

  gData_02006c0c = value;
  {
    s32 priority = 0x480;
    Task_Add(Func_020026c0, priority);
  }
}

void Func_02002790(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 x = actor->x >> 20;
  s32 y = actor->y >> 20;
  s32 z = actor->z >> 20;
  s32 cell = (x << 16) + z;
  s32 scene;

  if (cell == gData_02006c0c)
    return;
  gData_02006c0c = cell;
  gData_020073dc = 1;
  gData_020073e0 = 2;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_AirsRockInside1 ||
      scene == (s32)&SceneId_AirsRockInside2 ||
      scene == (s32)&SceneId_AirsRockInside3) {
    gData_020073e4 = x;
    gData_020073e8 = z - y + 64;
  } else if (scene == (s32)&SceneId_AirsRockInside5 ||
             scene == (s32)&SceneId_AirsRockInside6 ||
             scene == (s32)&SceneId_AirsRockInside7 ||
             scene == (s32)&SceneId_AirsRockInside9) {
    gData_020073e4 = x + 64;
    gData_020073e8 = z - y;
  }
  Call6((void (*)())MapGrid_CopyCellPictureCellsRect, gData_020073e4,
        gData_020073e8, gData_020073dc, gData_020073e0, 73, 72);
  Call6((void (*)())MapGrid_CopyCellPictureCellsRect, 70, 72, gData_020073dc,
        gData_020073e0, gData_020073e4, gData_020073e8);
}

void Func_02002898(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 x = actor->x >> 20;
  s32 y = actor->y >> 20;
  s32 z = actor->z >> 20;
  s32 cell = (x << 16) + z;
  s32 scene;

  if (cell == gData_02006c0c)
    return;
  gData_02006c0c = cell;
  gData_020073dc = 1;
  gData_020073e0 = 2;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_AirsRockInside1 ||
      scene == (s32)&SceneId_AirsRockInside2) {
    gData_020073e4 = x;
    gData_020073e8 = z - y + 64;
  } else if (scene == (s32)&SceneId_AirsRockInside5 ||
             scene == (s32)&SceneId_AirsRockInside6) {
    gData_020073e4 = x + 64;
    gData_020073e8 = z - y;
  }
  Call6((void (*)())MapGrid_CopyCellPictureCellsRect, gData_020073e4,
        gData_020073e8, gData_020073dc, gData_020073e0, 73, 72);
  Call6((void (*)())MapGrid_CopyCellPictureCellsRect, 71, 72, gData_020073dc,
        gData_020073e0, gData_020073e4, gData_020073e8);
}

// When the controlled object steps onto a new cell, saves the 1 by 3 cells
// below it at (73, 72) and stamps the cells kept at (72, 72) in their place.
#define DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(name, n1)                    \
  void name(void) {                                                            \
    struct Actor *object = Actor_GetById(Party_GetLeaderActor());              \
    s32 x = object->x >> 20;                                                   \
    s32 y = object->y >> 20;                                                   \
    s32 z = object->z >> 20;                                                   \
    s32 key = (x << 16) + z;                                                   \
                                                                               \
    if (key != gData_02006c0c) {                                               \
      gData_02006c0c = key;                                                    \
      gData_020073dc = 1;                                                      \
      gData_020073e0 = 3;                                                      \
      gData_020073e4 = x;                                                      \
      gData_020073e8 = z - y + n1;                                             \
      CopyCells(x, gData_020073e8, 1, 3, 73, 72);                              \
      CopyCells(72, 72, gData_020073dc, gData_020073e0, gData_020073e4,        \
                gData_020073e8);                                               \
    }                                                                          \
  }

DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(Func_02002980, 64)

// As Func_02002980, one row nearer.
DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(Func_02002a14, 63)

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void Func_02002aa8(s32 x, s32 z) {
  struct EffectOptions options;
  struct Vector3 spawn;
  s32 i;

  options.priority = 1;
  options.startScaleX = 0x20000;
  options.startScaleY = 0x20000;
  options.targetScaleX = 0x8000;
  options.targetScaleY = 0x8000;
  i = 0;
burst:
  spawn.x = x + ((Random16() & 63) << 16) - 0x200000;
  spawn.y = (Random16() & 15) << 12;
  spawn.z = z + ((Random16() & 7) << 16) - 0x100000;
  FieldEffect_Spawn(spawn.x, 0, spawn.z, 0, spawn.y, 0,
                    EFFECT_SCALE_TO_TARGET | EFFECT_USE_START_SCALE |
                        EFFECT_USE_PRIORITY | 1,
                    &options);
  i++;
  if (i != 8)
    goto burst;
  MapGrid_SetScreenShake(0x50000, 0x70000, 0x10000);
  Call3((void (*)())MapGrid_SetScreenShake, -1, -1, 0xe666);
}

void Func_02002b58(s32 x, s32 z) {
  struct AirsRockInsideSpawn spawn;
  struct AirsRockInsideVector position;
  s32 i;

  spawn.kind = 0x2a0;
  spawn.mode = 1;
  spawn.scaleX = 0xcccc;
  spawn.scaleY = Q16_ONE * 3;
  i = 0;
next:
  position.x = x + ((Random16() & 31) << 16) - 0x100000;
  position.y = px(4);
  position.z = z;
  FieldEffect_Spawn(position.x, 0, position.z, 0, position.y, 0, 0x1a0000,
                    &spawn);
  FieldEvent_Wait(5);
  i++;
  if (i != 12)
    goto next;
}

void Func_02002bd0(struct Actor *object) {
  s32 radius = ((object->y - object->terrainHeight) >> 1) + 0x100000;

  object->x = WORD_AT(object, 68) +
              Iwram_MulQ16(Trig_Cos((WORD_AT(object, 48) & 255) << 11), radius);
  object->z =
      WORD_AT(object, 76) +
      Iwram_MulQ16(Trig_Sin((WORD_AT(object, 48) & 255) << 11), radius) / 3;
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
}

// Circles the object around its centre (fields 68 and 76) on an ellipse twice
// as wide as deep, the radius growing with its height; it rises by field 72
// each frame and switches its part attribute between 0 and 9 every two frames.
void Func_02002c40(struct Actor *object) {
  s32 y = object->y;
  s32 phase;
  s32 radius;

  phase = WORD_AT(object, 48);
  radius = 0xc0000;
  /* FAKEMATCH(barrier): the radius constant is built before y >> 2. */
  __asm__("");
  radius += y >> 2;
  object->x = WORD_AT(object, 68) +
              Iwram_MulQ16(Trig_Cos((phase & 255) << 11), radius) * 2;
  object->z = WORD_AT(object, 76) +
              Iwram_MulQ16(Trig_Sin((WORD_AT(object, 48) & 255) << 11), radius);
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
  Actor_SetSpriteColor(object, ((gFrameCount & 2) >> 1) * 9);
}

void Func_02002cc0(struct Actor *object) {
  s32 bit = ((gFrameCount & 3) >> BYTE_AT(object, 99)) & 1;

  Actor_SetSpriteColor(object, (u8)(bit * BYTE_AT(object, 98)));
}

void BattleEffect_ApplyColorToSourceBuffer(s32 a, s32 b);
s32 BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);
void PaletteFade_StartInterpolation(s32 mode);
void MapGrid_CopyCellPictureIndicesRect(s32 x, s32 z, s32 width, s32 height,
                                        s32 a, s32 b);
void Actor_ConfigureSpriteColor(s32 actor, s32 a);

// The second pillar of light, Func_02003654 with object 11 glowing, the camera
// at (0x138, 0x2a8), the cells at (0x40, 78) opening and the light reaching
// object 10 (objects 8 and 9 lit with it); sets flag 0x89a.
void Func_02002cf0(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  /* FAKEMATCH(dead): BG0CNT's value in a once-only block of its own, built
   * before its address. */
  do {
    s32 control = BGCNT_SCREEN_BASE(4);

    REG_BG0CNT = control;
  } while (0);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                      buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(4), 0x800);
  buffer = Heap_Alloc(0x80);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningMap),
                      buffer);
  i = 0;
  cell = buffer;
mask:
  *cell &= 0xfcff;
  i++;
  cell++;
  if (i != 64)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(BG_SCREEN_BLOCK(4) + (i << 6)), DMA_COPY_WORDS(2),
          REG_DMA3);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  Heap_Release(buffer);
  Sound_Play(SE_GLITTER);
  ((u8 *)Actor_GetById(11))[98] = 9;
  ((u8 *)Actor_GetById(11))[99] = 1;
  Actor_GetById(11)->update = Func_02002cc0;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Import_ActorCreate(0x2a0, Actor_GetById(11)->x,
                                 Actor_GetById(11)->y, Actor_GetById(11)->z);
      {
        s32 priority = ((s8 *)Actor_GetById(11)->sprite)[9] & 12;

        /* FAKEMATCH(temporary): the sprite held in the row pointer. */
        cell = spark->sprite;
        ((struct SpriteAttr2High *)cell)->priority = priority >> 2;
      }
      ((struct ObjectPriorityByte *)spark)->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = px(3) / 2;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gData_02005fd8);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = Func_02002c40;
    }
    FieldEvent_Wait(1);
  }
  Call2((void (*)())BattleEffect_ApplyColorToSourceBuffer, 0x10000, 0);
  Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x10003, 0);
  PaletteFade_StartInterpolation(60);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Call2((void (*)())Camera_SetSpeedParameters, 0x10000, 0x2000);
  Call4((void (*)())FieldCamera_MoveWithinMapBounds, px(312), -1, px(680), 1);
  FieldCamera_WaitForArrival();
  Actor_GetById(11)->update = 0;
  Actor_SetSpriteColor(Actor_GetById(11), 6);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = 0xff98;
  gBgScroll[0].y = 42;
  wait(30);
  Sound_Play(SE_ITEM_BREAK);
  Call2((void (*)())BattleEffect_ApplyColorToSourceBuffer, 0x10003, 0);
  Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  BattleEffect_ApplyColorToSourceBuffer(0x7fff, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  PaletteFade_StartInterpolation(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellPictureIndicesRect(64, 78, 18, 105, 3, 3);
  MapGrid_SetScreenShake(0x50000, 0x70000, 0x10000);
  Call3((void (*)())MapGrid_SetScreenShake, -1, -1, 0xe666);
  i = 0;
fade:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  wait(1);
  i++;
  if (i != 32)
    goto fade;
  BattleEffect_ApplyColorToSourceBuffer(0x10003, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(60);
  wait(30);
  Sound_Play(SE_BOSS_ROAR);
  Func_02002aa8(0x1380000, 0x2d00000);
  Call6((void (*)())MapGrid_CopyCellPictureIndicesRect, 64, 78, 18, 105, 3, 5);
  Camera_SetSpeedParameters(0x18000, 0x3000);
  FieldCamera_MoveToActorById(10, 1);
  FieldCamera_WaitForArrival();
  Sound_Play(SE_ENEMY_CRY);
  Call2((void (*)())Actor_ConfigureSpriteColor, 10, 0x100);
  Func_02002b58(Actor_GetById(10)->x, Actor_GetById(10)->z);
  Actor_ConfigureSpriteColor(10, 0);
  Actor_SetSpriteColor(Actor_GetById(10), 7);
  wait(3);
  Actor_SetSpriteColor(Actor_GetById(8), 0);
  Actor_SetSpriteColor(Actor_GetById(9), 0);
  Actor_SetSpriteColor(Actor_GetById(10), 0);
  wait(40);
  Func_02001084(Actor_GetById(10), 8, 0);
  Window_LoadGraphics();
  DebugText_Clear();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                    gMainRoutineWork.paletteGlow[1]);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_Set(0x89a);
  Event_End();
}

void Actor_WalkToById(s32 actor, s32 x, s32 z);

// When layer 1's cell at the leader's x and z - 1 reads 30, runs the four
// steps 65, 66, 67 and 62 and moves the leader to (312, 792).
void Func_02003170(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 width;
  s32 height;

  if (Func_02000690(1, leader->x >> 20, (leader->z >> 20) - 1) >> 8 != 30)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_THUMP);
  width = 1;
  height = 2;
  MapGrid_CopyCellPictureIndicesRect(65, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellPictureIndicesRect(66, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellPictureIndicesRect(67, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellPictureIndicesRect(62, 76, 19, 92, width, height);
  FieldEvent_Wait(15);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_GetById(gMainRoutineWork.currentOwner)->speedLimit =
      Actor_GetById(gMainRoutineWork.currentOwner)->acceleration = px(1) / 2;
  Sound_Play(SE_MAP_EXIT);
  Actor_WalkToById(gMainRoutineWork.currentOwner, 312, 792);
  FieldEvent_Wait(5);
  Event_SetExitRequest(15);
  Event_End();
}

static inline void ClearLayer2Shape(s32 x, s32 z) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline. */
  MapGrid_SetCellHeight(x, z, 2, 0);
}

// Runs each room's list on entering it; room 7 also clears two cells' shapes
// and room 9 stops object 10's timer.
void Func_02003254(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AirsRockInside1) {
    Actor_LiftTargetBlock(gData_02006050);
  } else if (scene == (s32)&SceneId_AirsRockInside2) {
    Actor_LiftTargetBlock(gData_02006054);
  } else if (scene == (s32)&SceneId_AirsRockInside3) {
    Actor_LiftTargetBlock(gData_0200605a);
  } else if (scene == (s32)&SceneId_AirsRockInside4) {
    Actor_LiftTargetBlock(gData_02006064);
  } else if (scene == (s32)&SceneId_AirsRockInside7) {
    ClearLayer2Shape(0x2600000, 0x1500000);
    MapGrid_SetCellHeight(0x2700000, 0x1500000, 2, 0);
    Actor_LiftTargetBlock(gData_02006068);
  } else if (scene == (s32)&SceneId_AirsRockInside8) {
    Actor_LiftTargetBlock(gData_02006072);
  } else if (scene == (s32)&SceneId_AirsRockInside9) {
    Actor_GetById(10)->timer = 0;
    Actor_LiftTargetBlock(gData_0200607e);
  }
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->fieldWork)

// After the pushed object (the event work's object at 0xcbc) comes to rest:
// if it stopped above the ground it drops onto it with cue 188; then, by
// room, the place where it stopped sets the flag for that pillar's spot
// (AirsRockInside1 to 4 and 9), in AirsRockInside7 marks two cells and shows
// objects 13 and 14 by where objects 9, 10 and 11 stand, and in AirsRockInside8
// and 9 sets or clears the flags of the spots along its rows and redraws the
// pillars' cells (Func_0200062c).
void Func_0200332c(void) {
  struct Actor *pushed = *(struct Actor **)(EVENT_WORK + 0xcbc);
  s32 x, z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  if (Map_GetTerrainHeight(2, pushed->x, pushed->z) != pushed->y) {
    pushed->terrainId = 2;
    pushed->motionFlags = 3;
    OverlayObject_WaitUntilIdle(pushed);
    Sound_Play(SE_THUMP);
    OverlayObject_WaitUntilIdle(pushed);
    pushed->motionFlags = 0;
    pushed->terrainId = 0;
  }
  x = pushed->x >> 19;
  z = pushed->z >> 19;
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside1) {
    if (x == 103 && z == 41)
      GameFlag_Set(0xa8e);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside2) {
    if (x == 13 && z == 49)
      GameFlag_Set(0x898);
    else if (x == 41 && z == 49)
      GameFlag_Set(0x897);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside3) {
    if (x == 33 && z == 79)
      GameFlag_Set(0x89b);
    else if (x == 77 && z == 71)
      GameFlag_Set(0x89c);
    else if (x == 59 && z == 65)
      GameFlag_Set(0x89d);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside4) {
    if (x == 81 && z == 37)
      GameFlag_Set(0x89e);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside7) {
    Call4((void (*)())MapGrid_SetCellHeight, 0x2600000, 0x1500000, 2, -4);
    Call4((void (*)())MapGrid_SetCellHeight, 0x2700000, 0x1500000, 2, -4);
    if (x == 43 && z == 87)
      GameFlag_Set(0x8d0);
    if ((Actor_GetById(9)->x >> 19 == 73 && Actor_GetById(9)->z >> 19 == 43) ||
        (Actor_GetById(10)->x >> 19 == 73 && Actor_GetById(10)->z >> 19 == 43))
      ((u8 *)Actor_GetById(13))[99] = 1;
    else
      ((u8 *)Actor_GetById(13))[99] = 0;
    if (pushed->x == Actor_GetById(11)->x &&
        pushed->z == Actor_GetById(11)->z) {
      if (x == 83 && z == 43)
        ((u8 *)Actor_GetById(14))[99] = 1;
      else
        ((u8 *)Actor_GetById(14))[99] = 0;
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside8) {
    if (x == 35 && z == 59)
      GameFlag_Set(0x8d1);
    if (x == 45 && z == 105)
      GameFlag_Set(0x8d2);
    if (x == 23 && z == 77) {
      GameFlag_Set(FLAG_SCENE_TEMP_00);
      GameFlag_Set(0xa8f);
    }
    if (z == 59 && (x == 67 || x == 71 || x == 73))
      GameFlag_Set(FLAG_SCENE_TEMP_01);
    if (z == 19 && (x == 75 || x == 79))
      GameFlag_Set(FLAG_SCENE_TEMP_02);
    if (x == 69 && z == 59)
      GameFlag_Clear(FLAG_SCENE_TEMP_01);
    if (x == 77 && z == 19)
      GameFlag_Clear(FLAG_SCENE_TEMP_02);
    Func_0200062c(2, Actor_GetById(8)->x >> 20, Actor_GetById(8)->z >> 20, 0);
    Func_0200062c(2, Actor_GetById(9)->x >> 20, Actor_GetById(9)->z >> 20, 0);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside9) {
    if (x == 45 && z == 33)
      GameFlag_Set(0x8d3);
    Func_0200062c(2, Actor_GetById(10)->x >> 20, Actor_GetById(10)->z >> 20, 0);
  }
  Event_End();
}

#undef EVENT_WORK

// A pillar of light rises in Air's Rock: BG0 is hidden while its tiles and
// its map (the palette bits cleared, copied in 16 rows of 8 bytes) are loaded
// into VRAM; object 10 glows (Func_02002cc0) and sheds a spark (object type
// 0x2a0, Func_02002c40) every fourth frame for 45 frames; the camera moves to
// the light, BG0 is shown blended (cue 138), the light flashes, the cells at
// (68, 84) open, the blend fades over 32 frames, then (cue 148) the light
// reaches object 8, which flashes and leaves (Func_02001084); the display is
// put back and flag 0x899 set.
void Func_02003654(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  /* FAKEMATCH(dead): BG0CNT's value in a once-only block of its own, built
   * before its address. */
  do {
    s32 control = BGCNT_SCREEN_BASE(4);

    REG_BG0CNT = control;
  } while (0);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                      buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(4), 0x800);
  buffer = Heap_Alloc(0x80);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningMap),
                      buffer);
  i = 0;
  cell = buffer;
mask:
  *cell &= 0xfcff;
  i++;
  cell++;
  if (i != 64)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(BG_SCREEN_BLOCK(4) + (i << 6)), DMA_COPY_WORDS(2),
          REG_DMA3);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  Heap_Release(buffer);
  Sound_Play(SE_GLITTER);
  ((u8 *)Actor_GetById(10))[98] = 9;
  ((u8 *)Actor_GetById(10))[99] = 1;
  Actor_GetById(10)->update = Func_02002cc0;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Import_ActorCreate(0x2a0, Actor_GetById(10)->x,
                                 Actor_GetById(10)->y, Actor_GetById(10)->z);
      {
        s32 priority = ((s8 *)Actor_GetById(10)->sprite)[9] & 12;

        /* FAKEMATCH(temporary): the sprite held in the row pointer. */
        cell = spark->sprite;
        ((struct SpriteAttr2High *)cell)->priority = priority >> 2;
      }
      ((struct ObjectPriorityByte *)spark)->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = px(3) / 2;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gData_02005fd8);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = Func_02002c40;
    }
    FieldEvent_Wait(1);
  }
  Call2((void (*)())BattleEffect_ApplyColorToSourceBuffer, 0x10000, 0);
  Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x10003, 0);
  PaletteFade_StartInterpolation(60);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Call2((void (*)())Camera_SetSpeedParameters, 0x10000, 0x2000);
  Call4((void (*)())FieldCamera_MoveWithinMapBounds, px(216), -1, px(224), 1);
  FieldCamera_WaitForArrival();
  Actor_GetById(10)->update = 0;
  Actor_SetSpriteColor(Actor_GetById(10), 6);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = 0xff98;
  gBgScroll[0].y = 42;
  wait(30);
  Sound_Play(SE_ITEM_BREAK);
  Call2((void (*)())BattleEffect_ApplyColorToSourceBuffer, 0x10003, 0);
  Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  BattleEffect_ApplyColorToSourceBuffer(0x7fff, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  PaletteFade_StartInterpolation(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellPictureIndicesRect(68, 84, 12, 77, 3, 3);
  MapGrid_SetScreenShake(0x50000, 0x70000, 0x10000);
  Call3((void (*)())MapGrid_SetScreenShake, -1, -1, 0xe666);
  i = 0;
fade:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  wait(1);
  i++;
  if (i != 32)
    goto fade;
  BattleEffect_ApplyColorToSourceBuffer(0x10003, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(60);
  wait(30);
  Sound_Play(SE_BOSS_ROAR);
  Func_02002aa8(0xd80000, 0x12d0000);
  Call6((void (*)())MapGrid_CopyCellPictureIndicesRect, 68, 84, 12, 77, 3, 6);
  Camera_SetSpeedParameters(0x18000, 0x3000);
  FieldCamera_MoveToActorById(8, 1);
  FieldCamera_WaitForArrival();
  Sound_Play(SE_ENEMY_CRY);
  Call2((void (*)())Actor_ConfigureSpriteColor, 8, 0x100);
  Func_02002b58(Actor_GetById(8)->x, Actor_GetById(8)->z + px(32));
  Actor_ConfigureSpriteColor(8, 0);
  Actor_SetSpriteColor(Actor_GetById(8), 7);
  wait(3);
  Actor_SetSpriteColor(Actor_GetById(8), 0);
  wait(40);
  Func_02001084(Actor_GetById(8), 8, 0);
  Window_LoadGraphics();
  DebugText_Clear();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                    gMainRoutineWork.paletteGlow[1]);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_Set(0x899);
  Event_End();
}

void Func_02003ac4(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 286);
  Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 6, 0);
  Event_SetExitRequest(3);
  Event_End();
}

void Actor_SetSpeedParameters(u32 objectId, s32 speedLimit, s32 acceleration);
void Actor_MoveToAndWaitById(u32 objectId, s32 x, s32 z);

// While the leader is no further than z 653, moves the leader to (376, 668)
// with bit 0 of its byte 90 cleared for the move.
void Func_02003b08(void) {
  u8 *flags;
  u8 bit = 1;

  if (((s16 *)&Actor_GetById(gMainRoutineWork.currentOwner)->z)[1] > 653)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_SOL_SANCTUM_LIGHT_UP);
  BYTE_AT(Actor_GetById(gMainRoutineWork.currentOwner), 90) &= ~1;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x40000, 0x20000);
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, 376, 668);
  flags = &BYTE_AT(Actor_GetById(gMainRoutineWork.currentOwner), 90);
  *flags = bit | *flags;
  Event_End();
}

void Func_02003b7c(s32 x, s32 y) {
  struct Actor *marker;
  struct FieldSprite *sprite;
  s32 i, cellX, cellY;

  marker = Import_ActorCreate(288, x << 16, 0, y << 16);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f44;
  Scene_SetActorSpritePriority(marker, 0);
  Actor_SetSpriteFlags(marker, 0);
  sprite = marker->sprite;
  sprite->blendMode = 2;
  Event_Start();
  Event_ResolvePendingActions(0);
  REG_WINOUT = 0x3f1f;
  REG_DISPCNT |= 0x8000;
  Sound_Play(SE_DRIP);
  i = 0;
fadeIn:
  REG_BLDALPHA = (i << 9) | 16;
  wait(1);
  i++;
  if (i != 8)
    goto fadeIn;
  i = 0;
fadeOut:
  REG_BLDALPHA = (16 - i) | 0x1000;
  wait(1);
  i++;
  if (i != 16)
    goto fadeOut;
  cellX = x >> 4;
  cellY = (y >> 4) - 1;
  Call6((void (*)())MapGrid_CopyCellPictureIndicesRect, 65, 1, cellX + 64,
        cellY, 1, 2);
  Func_0200062c(0, cellX, cellY, 0xff00);
  Actor_Release(marker);
  Event_End();
  REG_BLDCNT = 0;
  REG_DISPCNT &= 0x7fff;
}

// Reads what the leader stands on (Func_02000690, kind 2): on cells 1 and 7
// it only redraws the marker when flag 0x200 is set; elsewhere it clears
// (Func_02002790), redraws the marker in the row the event work's counter
// gives, sets flag 0x200 and the counter's flag (312 on).
void Func_02003cac(void) {
  u8 *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader;
  s32 cell, row = 54;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  cell = Func_02000690(2, leader->x >> 20, leader->z >> 20);
  work += 0x16a;
  // From 54 in two steps: as one expression the constants fold into
  // 858 - 4 * counter.
  row -= (*(s16 *)work - 201) << 2;
  cell >>= 8;
  if (cell != 1 && cell != 7) {
    Func_02002790();
    Func_02003b7c(264, row * 8 + 10);
    if (!Value1((s32 (*)())GameFlag_Get, 0x200))
      GameFlag_Set(FLAG_SCENE_TEMP_00);
    GameFlag_Set(*(s16 *)work + 312);
  } else if (GameFlag_Get(FLAG_SCENE_TEMP_00)) {
    Func_02003b7c(264, row * 8 + 10);
  }
}

// As Func_02002980 with a 1 by 2 block: saves the cells 64 columns across
// from the controlled object at (73, 72) and stamps the cells kept at
// (70, 74) in their place.
void Func_02003d40(void) {
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());
  s32 x = object->x >> 20;
  s32 y = object->y >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;

  if (key != gData_02006c0c) {
    gData_02006c0c = key;
    gData_020073dc = 1;
    gData_020073e0 = 2;
    gData_020073e4 = x + 64;
    gData_020073e8 = z - y;
    CopyCells(gData_020073e4, gData_020073e8, 1, 2, 73, 72);
    CopyCells(70, 74, gData_020073dc, gData_020073e0, gData_020073e4,
              gData_020073e8);
  }
}

extern u32 gData_02006084[];

#define DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(name, n1, n2)                        \
  void name(void) {                                                            \
    Map_CopyCellAttributeRect(n1, 17, 1, 1, 8, 19);                            \
    Map_CopyCellAttributeRect(n1, 17, 1, 1, 24, 19);                           \
    Call6((void (*)())MapGrid_CopyCellPictureIndicesRect, n2, 17, 72, 19, 1,   \
          2);                                                                  \
    MapGrid_CopyCellPictureIndicesRect(n2, 17, 88, 19, 1, 2);                  \
  }

DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(Func_02003dd4, 26, 90)

DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(Func_02003e28, 25, 89)

void Func_02003e7c(struct Actor *source) {
  struct Actor *object = (struct Actor *)Value4(
      (s32 (*)())Import_ActorCreate, 0x2a0, source->x, source->y, source->z);

  WORD_AT(object, 28) = 0x6666;
  WORD_AT(object, 24) = 0x6666;
  object->motionFlags = 0;
  Actor_SetSpriteColor(object, 2);
  Scene_SetActorSpritePriority(object, 1);
  Actor_SelectAnimation(object, 1);
  Actor_SetSpriteFlags(object, 0);
  Actor_SetBlendMode(object, 1);
  Actor_StartScript(object, gData_02006084);
}

void Vector_AddPolarOffset(s32 length, s32 angle, s32 *position);

s32 Func_02003ed4(struct Orbiter *orbiter) {
  struct Actor *anchor = orbiter->anchor;

  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x;
  orbiter->y = anchor->y + px(8);
  orbiter->z = anchor->z;
  Call3((void (*)())Vector_AddPolarOffset, orbiter->radius, orbiter->angle,
        (s32)&orbiter->x);
  if (--orbiter->countdown == 0)
    return 0;
  if (orbiter->radius == 0x20000)
    return 1;
  Func_02003e7c(orbiter);
  orbiter->radius -= 0x40000;
  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x;
  orbiter->y = anchor->y + px(8);
  orbiter->z = anchor->z;
  Vector_AddPolarOffset(orbiter->radius, orbiter->angle, &orbiter->x);
  Func_02003e7c(orbiter);
  orbiter->radius -= 0x40000;
  return 1;
}

// Moves around in a circle.
s32 Orbit_Update(struct Orbit *object) {
  s32 z;

  object->angle += object->speed;
  object->x = IwramMulQ16(Trig_Cos(object->angle), object->radius);
  z = IwramMulQ16(Trig_Sin(object->angle), object->radius);
  object->x += object->centreX;
  object->z = (z >> 1) + object->centreZ;
  if (object->growth != 141 && (gFrameCount & 1)) {
    object->radius = object->growth * Trig_Sin(object->phase << 10);
    object->growth += 10;
  }
  if (object->speed == 0)
    return 0;
  if (object->growth == 141)
    object->speed -= 128;
  return 1;
}

#include "shared/actor_aim_at_watched.h"

#include "shared/actor_step_to_watched.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/actor_emit_bubbles.h"

extern u32 gData_020060e0[];
extern u32 gData_02006094[];
extern s32 gData_020060b0[];
extern u32 gData_02006104[];
extern s32 gData_020060c0[];
extern s32 gData_02006128[];
extern s32 gData_02006158[];
void Actor_SetMovementTypeById(s32 actor, const void *callback);

void Func_02004138(s32 idA, s32 idB) {
  struct Actor *a = Actor_GetById(idA);
  struct Actor *b = Actor_GetById(idB);
  s32 i;
  s16 *angle;
  s32 never;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(30);
  Camera_SetSpeedParameters(0x8000, 0x1000);
  FieldCamera_MoveWithinMapBounds(((s16 *)&a->x)[1] << 16,
                                  ((s16 *)&a->y)[1] << 16,
                                  ((s16 *)&a->z)[1] << 16, 1);
  Sound_Play(SE_RUMBLE);
  Call3(MapGrid_SetScreenShake, 0x20000, 0x20000, 0x10000);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  PaletteFade_StartInterpolation(60);
  a->motionFlags = 0;
  a->acceleration = 0x3333;
  a->speedLimit = 0x3333;
  Actor_SetMovementTypeById(idA, gData_020060e0);
  Sound_Play(SE_194);
  FieldEvent_Wait(45);
  Actor_ConfigureSpriteColor(idA, 0x100);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
first:
  {
    struct Actor *spark = Import_ActorCreate(0x2a0, a->x, a->y, a->z);
    Sound_Play(SE_GLITTER);
    spark->x += ((Random16() & 31) - 16) << 16;
    spark->y += ((Random16() & 15) - 8) << 16;
    spark->speedLimit = px(8);
    spark->acceleration = 0x3333;
    spark->motionFlags = 0;
    Actor_SetSpriteColor(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    Actor_StartScript(spark, gData_02006094);
    REG_BLDALPHA = 0x1004;
    wait(2);
    REG_BLDALPHA = 0x100a;
    wait(2);
    REG_BLDALPHA = 0x1010;
    wait(2);
    REG_BLDALPHA = 0x100a;
    wait(2);
  }
  i++;
  if (i != 16)
    goto first;
  REG_BLDALPHA = 0x1008;
  wait(30);
  /* FAKEMATCH(dead): the never-true jump into the loop. */
  never = idB << 1;
  i = 0;
  if (never & 1) {
    i = 1;
    goto tail;
  }
second:
  {
    struct Actor *spark = (struct Actor *)Value4((s32 (*)())Import_ActorCreate,
                                                 0x2a0, b->x, b->y, b->z);
    Sound_Play(SE_195);
    angle = &spark->timer;
    *angle = (Random16() & 0x7fff) + 0x8000;
    spark->unk66 = ((i & 1) * 2 - 1) << ((Random16() & 3) + 9);
    spark->motionFlags = 0;
    spark->unk4c = 0x7a0000;
    spark->watchedObject = b;
    ((union SparkView *)spark)->bytes.unk62 = (16 - i) * 8 + 15;
    Actor_SetSpriteColor(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    Call3(Vector_AddPolarOffset, spark->unk4c, *angle, (s32)&spark->x);
    Actor_StartScript(spark, gData_020060b0);
    if (i == 3) {
      Call2(Actor_ConfigureSpriteColor, idB, 0x100);
      b->motionFlags = 0;
      b->acceleration = 0x3333;
      b->speedLimit = 0x3333;
      /* FAKEMATCH(barrier): the empty asm before the call. */
      asm("");
      Actor_SetMovementTypeById(idB, gData_02006104);
    }
  tail:
    wait(8);
  }
  i++;
  if (i != 16)
    goto second;
  Sound_Play(SE_SHIMMER);
  wait(16);
  i = 2;
third:
  {
    struct Actor *spark =
        (struct Actor *)Value4((s32 (*)())Import_ActorCreate, 0x2a0, b->x,
                               b->y + i * 65536 - 0x80000, b->z);
    spark->timer = Random16();
    spark->unk66 = 0x800;
    ((union SparkView *)spark)->bytes.unk62 = i;
    ((union SparkView *)spark)->bytes.unk63 = 1;
    spark->bounce = spark->x;
    spark->unk4c = 0;
    spark->gravity = spark->z;
    spark->watchedObject = b;
    spark->motionFlags = 0;
    spark->priorityFlags = 1;
    Actor_SetSpriteColor(spark, 2);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    Actor_StartScript(spark, gData_020060c0);
  }
  i += 2;
  if (i != 32)
    goto third;
  Sound_Play(SE_SHIMMER);
  wait(50);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  PaletteFade_StartInterpolation(8);
  wait(16);
  MapGrid_SetScreenShake(0, 0, 0);
  Actor_ConfigureSpriteColor(idA, 0);
  Actor_ConfigureSpriteColor(idB, 0);
  Sound_Play(SE_574);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(80);
  Actor_SetMovementTypeById(idA, gData_02006128);
  Actor_SetMovementTypeById(idB, gData_02006158);
  b->update = Actor_EmitBubbles;
  wait(120);
  b->update = 0;
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Scene_PlayMusic();
  wait(1);
}

#include "text/msg_ids.h"

void Actor_WalkToAndWaitById(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
#define EVENT_WORK gFieldWork

s32 Actor_MoveToObject();
s32 Actor_SetHorizontalPositionWithTerrain();
void Actor_TurnToFaceEachOther(s32 actorId, s32 other, s32 frames);
void Actor_WaitForMoveById(s32 actorId);
void Actor_SetDestinationById(u32 objectId, s32 x, s32 z);

static inline void Call0(void (*f)()) {
  /* FAKEMATCH(wrapper): the call layer without arguments, through which the two
   * -1s before the call are negated in this order. */
  f();
}

// Checking the tablet shows its message; the first time (flag 0x8d4 clear)
// the scene with actor 6 follows, as Func_020030cc of Magma Rock does with
// the Blaze tablet.
void AirsRockInside_ReadRevealTablet(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show(MsgCheckedTheTablet, 1);
  if (Value1(GameFlag_Get, 0x8d4) != 0) {
    Event_End();
  } else {
    Event_SetMessageId((s32)MsgCheckedTheTablet);
    EVENT_WORK->message++;
    Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0xb333,
          0x5999);
    Call3(Actor_SetSpeedParameters, 6, 0xb333, 0x5999);
    Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
    FieldEvent_Wait(1);
    Call3(Actor_WalkToById, gMainRoutineWork.currentOwner, 256, 136);
    Actor_WalkToById(6, 272, 136);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Actor_WaitForMoveById(6);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_ArmCallback(6, FACING_WEST, 0);
    FieldEvent_Wait(10);
    Event_ShowMessageAndWait(6, 0);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(30);
    Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 256, 152);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_ArmCallback(6, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetModeById(6, 3);
    FieldEvent_Wait(30);
    Call3(Actor_ArmCallback, 6, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Call3(Actor_WalkToAndWaitById, 6, 264, 137);
    Actor_ArmCallback(6, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    FieldMessage_Show(MsgCheckedTheTablet + 2, 1);
    Call3(MapGrid_SetScreenShake, 0x50000, 0x70000, 0x10000);
    Call3(MapGrid_SetScreenShake, -1, -1, 0xe666);
    /* FAKEMATCH(wrapper): the call through Call0's pointer. */
    MapGrid_WaitForShakeToSettle();
    Func_02004138(8, 6);
    Party_LearnAbility(SHEBA, ABILITY_REVEAL);
    GameFlag_Set(0x8d4);
    Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 6, 0);
    FieldEvent_Wait(10);
    Actor_SetModeById(6, 3);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(20);
    Call3(Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
    Actor_SetModeById(6, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != 0)
      Actor_SetDestinationById(6, FIELD_AT(object, s16, 10),
                               FIELD_AT(object, s16, 18));
    Actor_WaitForMoveById(6);
    Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
    Actor_SetActionCallback(Actor_GetById(8), 9);
    Event_End();
  }
}

void AirsRockInside_WalkToRevealTablet(void) {
  if (GameFlag_Get(0x8d4)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Event_End();
  } else {
    Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 248, 107);
    Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 248, 136);
    Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    AirsRockInside_ReadRevealTablet();
  }
}

#define DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(name, n1)                          \
  void name(void) {                                                            \
    if (GameFlag_Get(0x8d4)) {                                                 \
      Event_Start();                                                           \
      Event_ResolvePendingActions(0);                                          \
      FieldMessage_Show(MsgCheckedTheTablet, 1);                               \
      Event_End();                                                             \
    } else {                                                                   \
      Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, n1, 136);         \
      Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 264, 136);        \
      Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);       \
      FieldEvent_Wait(10);                                                     \
      AirsRockInside_ReadRevealTablet();                                       \
    }                                                                          \
  }

DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(Func_020047b0, 280)

DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(Func_02004818, 248)

void Func_02004880(void) {}

/* VIEW(s2cf0): routine Func_020007b4 as void Func_020007b4(void);
 * scene/airs_rock_inside/airs_rock_inside has void Func_020007b4(s32 mode, s32
 * step) {; tried as the definition's void Func_020007b4(s32 mode, s32 step),
 * which the object already declares, it does not compile in tla-ja
 * (airs_rock_inside.c): games/tla/src/scene/airs_rock_inside.c:2701: too few
 * arguments to function `Func_020007b4'. */
void Func_020007b4__s2cf0(void) __asm__("Func_020007b4");

void Func_02004884(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Actor_GetById(8)->x >> 19 == 65)
    GameFlag_Set(FLAG_AREA_TEMP_03);
  else
    GameFlag_Clear(FLAG_AREA_TEMP_03);
  Event_End();
}

void Func_020048bc(void) { Func_020007b4__s2cf0(); }

extern u16 gData_0200621c[];
void Func_020006cc(struct Actor *object);

void Func_020048c4(void) {
  struct Actor *first = 0, *second = 0, *leader;
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramBlockCacheEntry *entry;
  u16 *pair;
  u32 id, i;
  s32 busy;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  pair = gData_0200621c;
  if (*pair == 0xffff)
    goto found;
search:
  id = *pair;
  first = Actor_GetById(id);
  if (first->x >> 20 == leader->x >> 20 && first->z >> 20 == leader->z >> 20) {
    second = Actor_GetById(id - ((id & 1) << 1) + 1);
    goto found;
  }
  pair += 3;
  if (*pair != 0xffff)
    goto search;
found:
  busy = ((u8 *)first)[0x63] | ((u8 *)second)[0x63];
  if (busy != 0)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  firstSprite = first->sprite;
  secondSprite = second->sprite;
  second->motionFlags = busy;
  second->y += -px(28);
  wait(1);
  entry = &gResourceTableEntries[secondSprite->vramBlock];
  second->watchedObject = (void *)(entry->offset + 0x6010000);
  entry = &gResourceTableEntries[firstSprite->vramBlock];
  first->watchedObject = (void *)(entry->offset + 0x6010000);
  Dma_Fill32(busy, second->watchedObject, entry->size);
  Dma_Set(first->watchedObject, second->watchedObject, DMA_COPY_WORDS(192),
          REG_DMA3);
  first->watchedObject = (void *)((u8 *)first->watchedObject + 0x300);
  second->watchedObject = (void *)((u8 *)second->watchedObject + 0x300);
  leader->motionFlags = busy;
  i = 0;
step:
  if (i <= 4) {
    second->y += 0x53333;
    Dma_Set(first->watchedObject, second->watchedObject, DMA_COPY_WORDS(48),
            REG_DMA3);
    first->watchedObject = (void *)((u8 *)first->watchedObject + 0x100);
    second->watchedObject = (void *)((u8 *)second->watchedObject + 0x100);
  }
  leader->y += -0x38000;
  leader->terrainHeight = leader->y;
  first->scaleY += -0x1f00;
  wait(1);
  i++;
  if (i != 8)
    goto step;
  leader->motionFlags = 3;
  first->scaleY = Q16_ONE;
  second->y = second->terrainHeight;
  GameFlag_Set(first->timer);
  GameFlag_Clear(second->timer);
  Actor_SelectAnimation(first,
                        ((struct FieldSprite *)first->sprite)->frame + 1);
  Actor_SelectAnimation(second,
                        ((struct FieldSprite *)second->sprite)->frame - 1);
  Func_020006cc(first);
  Func_020006cc(second);
  FieldEvent_Wait(10);
  Event_End();
}

// When the event work's warp number (0x154) is set, plays cue 186 and moves
// the leader to that warp's place (warps 50 to 55), inside an event.
void Func_02004ab8(void) {
  s16 *warp;
  s32 x, z;

  x = 0;
  z = 0;
  warp = &gFieldWork->stepTrigger;
  if (*warp == 0)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_SOL_SANCTUM_LIGHT_UP);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x40000, 0x20000);
  switch (*warp) {
  case 50:
    // XXX: the leader is looked up twice and neither result is used; likely
    // left from a position read, because the other warps give theirs as
    // constants.
    Actor_GetById(gMainRoutineWork.currentOwner);
    Actor_GetById(gMainRoutineWork.currentOwner);
    x = 148;
    z = 632;
    break;
  case 51:
    x = 264;
    z = 652;
    break;
  case 52:
    x = 504;
    z = 508;
    break;
  case 53:
    x = 552;
    z = 508;
    break;
  case 54:
    x = 616;
    z = 188;
    break;
  case 55:
    x = 648;
    z = 188;
    break;
  }
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, x, z);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Event_End();
}

void AirsRockInside_CheckPsynergyStone(void) {
  const u8 *message;

  Event_Start();
  Event_ResolvePendingActions(0);
  message = MsgCheckedThePsynergy;
  FieldMessage_Show(message, 1);
  Sound_Play(SE_RECOVERY);
  Party_AddPp(999, 0);
  FieldEvent_Wait(10);
  GameFlag_Clear(FLAG_NOTHING_HAPPENS);
  FieldMessage_Show(message + 1, 1);
  Event_End();
}

// The placement table for the current room.
const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AirsRockInside1)
    return gAirsRockInside_Room1Triggers;
  if (scene == (s32)&SceneId_AirsRockInside2)
    return gAirsRockInside_Room2Triggers;
  if (scene == (s32)&SceneId_AirsRockInside3)
    return gAirsRockInside_Room3Triggers;
  if (scene == (s32)&SceneId_AirsRockInside4)
    return gAirsRockInside_Room4Triggers;
  if (scene == (s32)&SceneId_AirsRockInside5)
    return gAirsRockInside_Room5Triggers;
  if (scene == (s32)&SceneId_AirsRockInside6)
    return gAirsRockInside_Room6Triggers;
  if (scene == (s32)&SceneId_AirsRockInside7)
    return gAirsRockInside_Room7Triggers;
  if (scene == (s32)&SceneId_AirsRockInside8)
    return gAirsRockInside_Room8Triggers;
  if (scene == (s32)&SceneId_AirsRockInside9)
    return gAirsRockInside_Room9Triggers;
  if (scene == (s32)&SceneId_AirsRockInside10)
    return gAirsRockInside_Room10Triggers;
  return gAirsRockInside_Room2Triggers;
}

void Func_02004cc8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *prepared, *lift;
#if TLA_PATCH_1
  u8 priorityFlags;
#endif
  s32 height;

  Event_Start();
  Event_ResolvePendingActions(0);
#if TLA_PATCH_1
  priorityFlags = leader->priorityFlags;
#endif
  prepared = OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 286);
  leader->motionFlags = 0;
  Actor_SetSpriteFlags(leader, 0);
  leader->speedLimit = px(3) / 2;
  leader->acceleration = px(3) / 2;
  lift = Import_ActorCreate(280, leader->x, leader->y - px(8), leader->z);
  Actor_SelectAnimation(lift, 2);
  Actor_SetSpriteFlags(lift, 0);
  lift->motionFlags = 0;
  lift->speedLimit = 0x16666;
  lift->acceleration = 0x16666;
  Actor_SetMoveTarget(leader, leader->x, leader->y + px(80), leader->z);
  Actor_SetMoveTarget(lift, lift->x, lift->y + px(72), lift->z);
  Sound_Play(SE_137);
  Event_StartScreenTransition();
  while (!Actor_HasArrived(leader)) {
    leader->facing = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(SE_288);
  FieldEvent_Wait(10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  height = Map_GetTerrainHeight(0, lift->x, lift->z - px(16));
  leader->y = height;
  leader->terrainHeight = height;
  Actor_SelectAnimation(leader, 6);
  wait(6);
  Sound_Play(SE_JUMP);
  Actor_SelectAnimation(leader, 7);
  leader->speedLimit = px(2);
  leader->acceleration = px(1) / 2;
  leader->velocityY = px(4);
  leader->motionFlags = 2;
  leader->priorityFlags = 1;
  Actor_SetMoveTarget(leader, leader->x, leader->y, leader->z - px(16));
  OverlayObject_WaitUntilIdle(leader);
  Actor_WaitForMove(leader);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_SelectAnimation(lift, 5);
  wait(12);
  Actor_Release(lift);
  leader->motionFlags = 3;
  leader->terrainHeight = leader->y;
  Actor_SetSpriteFlags(leader, 1);
#if TLA_PATCH_1
  leader->priorityFlags = priorityFlags;
#endif
  Actor_Release(prepared);
  wait(1);
  Event_WaitTransitionFrames();
  Event_End();
}

void Func_02004eb0(void) {
  struct Actor *object;
  s32 i = 0;
  struct FieldSprite *sprite;

  do {
    object = Import_ActorCreate(672, 0, 0, 0);
    sprite = object->sprite;
    sprite->priority = 1;
    ((u8 *)object)[0x63] = i << 4;
    {
      s32 zero = 0;
      object->motionFlags = zero;
      object->priorityFlags = zero;
    }
    object->acceleration = object->speedLimit = (i << 12) + 0x8000;
    object->update = Func_02000550;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetSpriteColor(object, 2);
    if (i & 1) {
      object->timer = 212;
      object->unk66 = 0;
    } else {
      object->timer = 116;
      object->unk66 = 0;
    }
    Actor_SetBlendMode(object, 1);
    i++;
  } while (i != 8);
}

extern s32 gData_020073d8;

void Func_02004f3c(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 level;

  actor->priorityFlags |= 32;
  if (actor->y + px(32) >= 0x1380000)
    level = 2;
  else if (actor->y + px(32) >= 0xd80000)
    level = 1;
  else
    level = 0;
  if (gData_020073d8 == level)
    return;
  switch (level) {
  case 2: {
    s32 a4 = 7, a5 = 39;
    Map_CopyCellAttributeRect(64, 64, 6, 4, a4, a5);
  }
    Map_CopyCellAttributeRect(70, 64, 6, 4, 26, 39);
    break;
  case 1: {
    s32 a4 = 7, a5 = 39;
    Map_CopyCellAttributeRect(64, 68, 6, 4, a4, a5);
  }
    Map_CopyCellAttributeRect(70, 68, 6, 4, 26, 39);
    break;
  case 0: {
    s32 a4 = 7, a5 = 39;
    Map_CopyCellAttributeRect(64, 72, 6, 4, a4, a5);
  }
    Map_CopyCellAttributeRect(70, 68, 6, 4, 26, 39);
    break;
  }
  gData_020073d8 = level;
}
#undef EVENT_WORK
#undef FIELD_AT

#include "../field/field_effect/advance_motion.c"

#undef EVENT_WORK
#undef DEFINE_AIRS_ROCK_INSIDE_SET_DOORS
#undef FIELD_AT
#undef DEFINE_AIRS_ROCK_INSIDE_READ_TABLET
#undef DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef other
#undef TARGET
#undef PICK_UP
#undef SHAKE
void FieldCamera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
s32 Trig_Sin(s32 angle);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void MapGrid_SetScreenShake(s32 a, s32 b, s32 c);

void Func_0200505c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Call4((void (*)())FieldCamera_MoveWithinMapBounds, -1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = px(130);
  leader->gravity = px(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (i = 0; i <= 16; i++) {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14) >> 16;
    velocity[0] += -0x6000;
    velocity[2] += (((Random16() * 3) << 13) >> 16) + -0x3000;
    FieldEffect_Spawn(
        leader->x, leader->y, leader->z, velocity[0], velocity[1], velocity[2],
        EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1,
        &options);
  }
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Call3((void (*)())MapGrid_SetScreenShake, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())MapGrid_SetScreenShake, -1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = px(1);
  leader->bounce = Q16_ONE / 4;
  gFieldWork->transition = 0x204;
  Event_End();
}

void Func_02001690(void);
void MapGrid_CopyCellPictureIndicesRect(s32 x, s32 z, s32 width, s32 height,
                                        s32 a, s32 b);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcZ, s32 width, s32 height,
                               s32 destX, s32 destZ);

s32 Scene_Initialize(void) {
  struct Actor *object;
  s32 i, z, one;

  /* FAKEMATCH(wrapper): 48 calls forwarded through call.h's Call3, Call4 and
   * Call6. */

  gFieldWork->transition = 516;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  GameFlag_Set(FLAG_AREA_TEMP_00);
  GameFlag_Set(0x89f);
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside1) {
    Func_02002770();
    // The Japanese release arrives here whatever flag 265 says.
    if (gMainRoutineWork.entrance == 16 &&
        (!TLA_PATCH_1 || !GameFlag_Get(FLAG_KEEP_PARTY_POSITION))) {
      Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 1;
      wait(8);
      Map_CenterOnFollowedPoint();
      wait(1);
    }
    if (gMainRoutineWork.entrance <= 11 || gMainRoutineWork.entrance == 16) {
      Task_Add(Func_02004f3c, 0x480);
      REG_BLDALPHA = 0x1008;
      REG_BLDCNT = 0x3f10;
      Func_02004eb0();
    }
    if (gMainRoutineWork.entrance > 13) {
      if (GameFlag_Get(0xa8e))
        Call3(Actor_SetHorizontalPositionWithTerrain, 12, px(824), px(328));
      Actor_PlaceBlocks(gData_02006050);
    }
    Actor_GetById(10)->motionFlags = 0;
    Actor_GetById(10)->y = 0;
    Actor_GetById(10)->priorityFlags = 2;
    Actor_SetSpritePriority(10, 3);
    if (!GameFlag_Get(0x89a)) {
      Actor_SetSpriteColor(Actor_GetById(10), 6);
    } else {
      Func_02001084(Actor_GetById(10), 8, 0);
      Actor_SetSpriteColor(Actor_GetById(11), 6);
      Call6(MapGrid_CopyCellPictureIndicesRect, 64, 78, 18, 105, 3, 5);
    }
    if (!GameFlag_Get(0x899)) {
      Actor_SetSpriteColor(Actor_GetById(8), 6);
      Actor_SetSpriteColor(Actor_GetById(9), 6);
      Actor_SetSpriteColor(Actor_GetById(12), 6);
    } else {
      Func_02001084(Actor_GetById(8), 180, 0);
      Func_02001084(Actor_GetById(9), 60, 0);
      Func_02001420(Actor_GetById(12), 60, 0);
    }
    Actor_SetSpriteFlags(Actor_GetById(8), 0);
    Actor_SetSpriteFlags(Actor_GetById(9), 0);
    Actor_SetSpriteFlags(Actor_GetById(12), 0);
    if (gMainRoutineWork.entrance == 11 &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
      Func_0200505c();
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside2) {
    REG_BLDALPHA = 0x1008;
    REG_BLDCNT = 0x3f10;
    Func_02002770();
    if (GameFlag_Get(0x897))
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, px(328), px(392));
    if (GameFlag_Get(0x898))
      Call3(Actor_SetHorizontalPositionWithTerrain, 11, px(104), px(392));
    Actor_PlaceBlocks(gData_02006054);
    if (!GameFlag_Get(0x899)) {
      Actor_SetSpriteColor(Actor_GetById(8), 6);
    } else {
      Func_02001084(Actor_GetById(8), 60, 0);
      Actor_SetSpriteColor(Actor_GetById(10), 6);
      Call6(MapGrid_CopyCellPictureIndicesRect, 68, 84, 12, 77, 3, 6);
    }
    Actor_SetSpriteFlags(Actor_GetById(8), 0);
    if (gMainRoutineWork.entrance == 3 &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
      Func_02004cc8();
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside3) {
    Func_02002770();
    Scene_QueueMusic(SE_245);
    REG_BLDALPHA = 0xc08;
    REG_BLDCNT = 0x3f10;
    Task_Add(Func_02001690, 0x480);
    if (GameFlag_Get(0x89b))
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, px(264), px(632));
    if (GameFlag_Get(0x89c))
      Call3(Actor_SetHorizontalPositionWithTerrain, 11, px(616), px(568));
    if (GameFlag_Get(0x89d))
      Call3(Actor_SetHorizontalPositionWithTerrain, 12, px(472), px(520));
    Actor_PlaceBlocks(gData_0200605a);
    if (!GameFlag_Get(0x899)) {
      Actor_SetSpriteColor(Actor_GetById(9), 6);
      Actor_SetSpriteColor(Actor_GetById(10), 6);
      Actor_SetSpriteColor(Actor_GetById(13), 6);
    } else {
      Func_02001084(Actor_GetById(9), 60, 0);
      Func_02001084(Actor_GetById(10), 180, 0);
      Func_02001084(Actor_GetById(13), 200, 0);
    }
    Actor_GetById(9)->actionFlags = 0;
    Actor_GetById(10)->actionFlags = 0;
    Actor_GetById(13)->actionFlags = 0;
    Actor_SetSpriteFlags(Actor_GetById(9), 0);
    Actor_SetSpriteFlags(Actor_GetById(10), 0);
    Actor_SetSpriteFlags(Actor_GetById(13), 0);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside4) {
    Tightrope_Initialize();
    Tightrope_Place(0, 11, 12);
    Tightrope_Place(1, 13, 14);
    Tightrope_Place(2, 15, 0x10);
    Tightrope_Place(3, 17, 18);
    Tightrope_Place(4, 19, 20);
    ((u8 *)Actor_GetById(18))[98] = 1;
    if (GameFlag_Get(FLAG_AREA_TEMP_01)) {
      Call6(MapGrid_CopyCellPictureIndicesRect, 69, 64, 29, 85, 5, 7);
      Call6(Map_CopyCellAttributeRect, 46, 20, 5, 5, 29, 21);
      Call6(Map_CopyCellAttributeRect, 32, 89, 1, 1, 31, 89);
      Call6(Map_CopyCellAttributeRect, 50, 86, 1, 1, 33, 87);
    } else {
      Call6(MapGrid_CopyCellPictureIndicesRect, 64, 64, 29, 85, 5, 7);
      Call6(Map_CopyCellAttributeRect, 52, 20, 5, 5, 29, 21);
      Call6(Map_CopyCellAttributeRect, 33, 88, 1, 1, 33, 87);
      Call6(Map_CopyCellAttributeRect, 48, 88, 1, 1, 31, 89);
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_00)) {
      Call6(MapGrid_CopyCellPictureIndicesRect, 90, 25, 86, 25, 4, 3);
      Call6(MapGrid_CopyCellPictureIndicesRect, 21, 88, 24, 89, 1, 2);
      Call6(Map_CopyCellAttributeRect, 23, 92, 2, 1, 23, 89);
      Call6(Map_CopyCellAttributeRect, 18, 33, 2, 1, 23, 25);
    }
    if (GameFlag_Get(0x89e))
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, px(648), px(296));
    Actor_PlaceBlocks(gData_02006064);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside5) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    Func_02002770();
    i = 0;
  loop:
    if (GameFlag_Get(514 + i)) {
      z = 24 - (i << 1);
      Call6(MapGrid_CopyCellPictureIndicesRect, 65, 1, 80, z, 1, 2);
      Call4((void (*)())Func_0200062c, 0, 16, z, 0xff00);
    }
    i++;
    if (i != 6)
      goto loop;
    Actor_GetById(8)->motionFlags = 0;
    object = Actor_GetById(8);
    Actor_GetById(8)->terrainHeight = px(16);
    object->y = px(16);
    Actor_SetActionCallback(Actor_GetById(8), 9);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside6) {
    Func_02002770();
    Tightrope_Initialize();
    Tightrope_Place(0, 11, 12);
    Tightrope_Place(1, 13, 14);
    Tightrope_Place(2, 15, 0x10);
    if (!GameFlag_Get(FLAG_AREA_TEMP_02)) {
      Call6(MapGrid_CopyCellPictureIndicesRect, 69, 64, 32, 79, 5, 7);
      Call6(Map_CopyCellAttributeRect, 49, 15, 5, 5, 32, 15);
      Call6(Map_CopyCellAttributeRect, 33, 82, 1, 1, 34, 82);
      Call6(Map_CopyCellAttributeRect, 52, 81, 1, 1, 36, 81);
    } else {
      Call6(MapGrid_CopyCellPictureIndicesRect, 64, 64, 32, 79, 5, 7);
      Call6(Map_CopyCellAttributeRect, 49, 21, 5, 5, 32, 15);
      Call6(Map_CopyCellAttributeRect, 35, 81, 1, 1, 36, 81);
      Call6(Map_CopyCellAttributeRect, 50, 82, 1, 1, 34, 82);
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_03))
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, px(520), px(296));
    if (GameFlag_Get(0x89f)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, px(312), px(296));
      Actor_SetSpritePriority(8, 2);
      Actor_GetById(8)->motionFlags = 2;
      Actor_GetById(8)->terrainHeight = 0xffe00000;
      Call6(Map_CopyCellAttributeRect, 19, 16, 1, 1, 19, 18);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside7) {
    Func_02002770();
    one = 1;
    ((u8 *)Actor_GetById(12))[98] = one;
    ((u8 *)Actor_GetById(13))[98] = one;
    ((u8 *)Actor_GetById(14))[98] = one;
    ((u8 *)Actor_GetById(15))[98] = one;
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      GameFlag_Set(FLAG_SCENE_TEMP_01);
      GameFlag_Set(FLAG_SCENE_TEMP_02);
    }
    Func_02000908(gData_0200621c);
    if (GameFlag_Get(0x8d0))
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, px(344), px(696));
    Actor_PlaceBlocks(gData_02006068);
    if (GameFlag_Get(FLAG_AREA_TEMP_04)) {
      Call6(MapGrid_CopyCellPictureIndicesRect, 39, 82, 38, 82, one, 3);
      Call6(Map_CopyCellAttributeRect, 39, 82, 1, 1, 38, 82);
      Call6(Map_CopyCellAttributeRect, 38, 19, 1, 1, 38, 18);
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_05)) {
      Call6(MapGrid_CopyCellPictureIndicesRect, 39, 82, 38, 99, one, 3);
      Call6(Map_CopyCellAttributeRect, 38, 98, 1, 1, 38, 99);
      Call6(Map_CopyCellAttributeRect, 38, 35, 1, 1, 38, 37);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside8) {
    Scene_QueueMusic(SE_245);
    REG_BLDALPHA = 0xc08;
    REG_BLDCNT = 0x3f10;
    Task_Add(Func_02001690, 0x480);
    if (GameFlag_Get(0x8d1))
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, px(280), px(472));
    if (GameFlag_Get(0x8d2))
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, px(360), px(840));
    if (GameFlag_Get(0xa8f)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 12, px(184), px(616));
      GameFlag_Set(FLAG_SCENE_TEMP_00);
    }
    Actor_SetSpritePriority(9, 2);
    Actor_PlaceBlocks(gData_02006072);
    Func_0200062c(2, Actor_GetById(8)->x >> 20, Actor_GetById(8)->z >> 20, 0);
    Func_0200062c(2, Actor_GetById(9)->x >> 20, Actor_GetById(9)->z >> 20, 0);
    if (!GameFlag_Get(0x899))
      Actor_SetSpriteColor(Actor_GetById(11), 6);
    else
      Func_02001084(Actor_GetById(11), 8, 0);
    Actor_SetSpriteFlags(Actor_GetById(11), 0);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside9) {
    Func_02002770();
    if (GameFlag_Get(0x8d3))
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, px(360), px(264));
    Actor_PlaceBlocks(gData_0200607e);
    Func_0200062c(2, Actor_GetById(10)->x >> 20, Actor_GetById(10)->z >> 20, 0);
    Actor_GetById(10)->actionFlags = 0;
    if (!GameFlag_Get(0x899))
      Actor_SetSpriteColor(Actor_GetById(10), 6);
    else
      Func_02001084(Actor_GetById(10), 60, 0);
    Actor_SetSpriteFlags(Actor_GetById(10), 0);
    if (GameFlag_Get(FLAG_AREA_TEMP_06)) {
      Call6(MapGrid_CopyCellPictureIndicesRect, 15, 80, 15, 77, 2, 3);
      Call6(Map_CopyCellAttributeRect, 15, 80, 1, 1, 15, 79);
      Call6(Map_CopyCellAttributeRect, 17, 15, 2, 1, 15, 15);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRockInside1)
    Func_02004f3c();
  return 0;
}

void Func_02000610(void);

extern u32 gData_02005f9c[];
extern u32 gData_02006014[];

// clang-format off
u32 gData_02005f9c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02005fd8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02006014[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gData_02006050[] = {
    12, -1,
};

s16 gData_02006054[] = {
    9, 11, -1,
};

s16 gData_0200605a[] = {
    8, 11, 12, 10, -1,
};

s16 gData_02006064[] = {
    9, -1,
};

s16 gData_02006068[] = {
    8, 9, 10, 11, -1,
};

s16 gData_02006072[] = {
    12, 8, 9, 13, 14, -1,
};

s16 gData_0200607e[] = {
    9, 10, -1,
};

u32 gData_02006084[] = {
    0, 6, 38, 17,
};

u32 gData_02006094[] = {
    5, 0, 0xa00000, 0, 1, 38,
    17,
};

s32 gData_020060b0[] = {                                                               
      46,                                                                      
      (s32)Func_02003ed4,                                                              
      38,                                                                      
      17,                                                                      
  };

s32 gData_020060c0[] = {                                                               
      46, (s32)Orbit_Update, 46, (s32)Actor_AimAtWatched, 46, (s32)Actor_StepToWatched, 38, 17,                  
  };

u32 gData_020060e0[] = {
    5, 0, 0x180000, 0, 1, 22,
    26, 4, 17,
};

u32 gData_02006104[] = {
    5, 0, 0x100000, 0, 1, 22,
    26, 4, 17,
};

s32 gData_02006128[] = {
    22, 26, 0, 5, 0, -0x180000,
    0, 1, 22, 26, 0, 17,
};

s32 gData_02006158[] = {
    22, 26, 0, 5, 0, -0x100000,
    0, 1, 22, 26, 3, 17,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gData_02005f9c,
    gData_02005fd8,
    gData_02006014,
};

u32 gData_02006194[] = {
    39, 5, 0, 12, 38, 17,
};

u32 gData_020061ac[] = {
    0, 60, 39, 5, 0, 12,
    38, 17,
};

u32 gData_020061cc[] = {
    39, 5, 0, 12, 2, 0,
    0, 0, 0, 1, 17,
};

u32 gData_020061f8[] = {
    22, 10, 0x18000, 22, 31, 0,
    39, 2, 17,
};

u16 gData_0200621c[] = {
    12, 1, 512, 13, 1, 513, 14, 1,
    514, 15, 1, 515, -1, 0,
};

struct ActorPlacement gData_02006238[] = {
    {0, CONDITION_ALWAYS, 320, 0x40000140, px(-1), -0x1, 0xffff, 0, 0},
    {SCENE_TABLE_END},
};

s16 gData_02006268[] = {
    1, -1, 232, 0, 120, 0x4000, 0, 64,
    16, 560, 1008, 0, 2, -1, 152, 0,
    136, 0x4000, 0, 64, 16, 560, 1008, 0,
    3, -1, 152, 0, 232, 0x4000, 0, 64,
    16, 560, 1008, 0, 4, -1, 152, 320,
    648, 0x4000, 0, 64, 16, 560, 1008, 0,
    5, -1, 152, 128, 648, 0x4000, 0, 64,
    16, 560, 1008, 0, 6, -1, 232, 0,
    632, 0x4000, 0, 64, 16, 560, 1008, 0,
    7, -1, 312, 0, 616, 0x4000, 0, 64,
    16, 560, 1008, 0, 8, -1, 472, 0,
    136, 0x4000, 0, 64, 16, 560, 1008, 0,
    9, -1, 472, 0, 232, 0x4000, 0, 64,
    16, 560, 1008, 0, 10, -1, 408, 0,
    952, 0xc000, 0, 64, 16, 560, 1008, 0,
    11, -1, 312, 0, 840, 0x4000, 0, 64,
    16, 560, 1008, 0, 12, -1, 808, 0,
    136, 0xc000, 0, 608, 16, 864, 224, 0,
    13, -1, 680, 0, 168, 0x4000, 0, 608,
    16, 864, 224, 0, 14, -1, 872, 0,
    344, 0x4000, 0, 592, 256, 976, 1008, 0,
    15, -1, 904, 0, 904, 0x4000, 0, 592,
    256, 976, 1008, 0, 16, -1, 312, 0,
    808, 0x4000, 0, 64, 16, 560, 1008, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(AirsRockInside1),
    SCENE_EXIT_UNLESS(1, AirsRockInside2, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInside3, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockInside4, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRockInside6, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AirsRockInside1, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AirsRockInside10, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AirsRockInside10, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AirsRockInside7, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AirsRockInside8, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AirsRock2, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AirsRockInside1, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AirsRockInside1, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, AirsRockInside1, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, AirsRockInside9, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, AirsRockInside5, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, AirsRockInside2, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInside2),
    SCENE_EXIT_UNLESS(1, AirsRock3, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInside1, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockInside1, 11, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInside3),
    SCENE_EXIT_UNLESS(1, AirsRockInside1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInside4, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInside4),
    SCENE_EXIT_UNLESS(1, AirsRockInside3, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInside6, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockInside9, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRockInside1, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInside5),
    SCENE_EXIT_UNLESS(1, AirsRockInside1, 16, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInside6),
    SCENE_EXIT_UNLESS(1, AirsRockInside4, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInside1, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInside7),
    SCENE_EXIT_UNLESS(1, AirsRockInside1, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInside8, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInside8),
    SCENE_EXIT_UNLESS(1, AirsRockInside1, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInside7, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInside9),
    SCENE_EXIT_UNLESS(1, AirsRockInside4, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInside1, 15, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInside10),
    SCENE_EXIT_UNLESS(1, AirsRockInside1, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInside1, 7, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gAirsRockInside_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_Room1Placements[] = {
    {377, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 0x3, 0, 0, FACING_NORTH, 2, 2},
    ACTOR_PLACEMENT(382, CONDITION_ALWAYS, 7, 312, 0, 808, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {377, CONDITION_ALWAYS, 7, 0x5, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(280, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(280, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(280, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_Room2Placements[] = {
    {377, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 2},
    {382, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_Room3Placements[] = {
    {272, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_WEST, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_WEST, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_Room4Placements[] = {
    {325, CONDITION_ALWAYS, (s32)gData_020061f8, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gData_020061f8, 0x3, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x9, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0xa, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0xb, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0xc, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0xd, 0, 0, FACING_SOUTH, 2, 3},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_Room5Placements[] = {
    {323, CONDITION_ALWAYS, 7, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_Room6Placements[] = {
    {272, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gData_020061f8, 0x2, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gData_020061f8, 0x3, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x9, 0, 0, FACING_SOUTH, 2, 3},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_Room7Placements[] = {
    {272, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 2},
    {406, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 2},
    {406, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 2},
    {406, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 2},
    {406, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gData_020061f8, 0x9, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gData_020061f8, 0xa, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_Room8Placements[] = {
    {272, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 2},
    {325, CONDITION_ALWAYS, (s32)gData_020061f8, 0x3, 0, 0, FACING_SOUTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_WEST, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

s32 gAirsRockInside_Room9Placements[] = {                                                               
      (s32)0xffff0145, (s32)gData_020061f8, 1, 0, 0, 0x2024000, (s32)0xffff0110, 1, 2, 0, 0, 0x2024000,    
      (s32)0xffff0179, 1,         3, 0, 0, 0x2020000, 0xffff,          0,  0,  0, 0, 0,     
  };

s32 gData_02006c0c = -1;

struct Trigger gAirsRockInside_Room1Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 14, CONDITION_ALWAYS, 14},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT(Func_02003170)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_02002898)},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(Func_02002790)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(Func_02002898)},
    {TRIGGER_STEP, 82, CONDITION_ALWAYS, EVENT(Func_02002980)},
    {TRIGGER_STEP, 83, CONDITION_ALWAYS, EVENT(Func_02002a14)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 11, 0x89a, EVENT(Func_02000610)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 11, 0x89a, EVENT(Func_0200061c)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, 0x89a, EVENT(Func_02002cf0)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Func_02000988)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_Room2Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, EVENT(Func_02003ac4)},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(Func_02002790)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(Func_02002898)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, 0x897, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x897, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11, 0x898, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x898, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 10, 0x899, EVENT(Func_02000610)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 10, 0x899, EVENT(Func_0200061c)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, 0x899, EVENT(Func_02003654)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Func_02000988)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(Func_02002790)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x89b, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x89b, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11, 0x89c, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x89c, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, 0x89d, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x89d, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Func_02000988)},
    {TRIGGER_RAISED, 201, CONDITION_ALWAYS, EVENT(Func_02003b08)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_Room4Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, 0x89e, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x89e, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 8, CONDITION_ALWAYS, EVENT(Func_02000610)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_0200061c)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 10, CONDITION_ALWAYS, EVENT(Func_02000610)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, EVENT(Func_0200061c)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(Func_02002790)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(Func_02002898)},
    {TRIGGER_STEP, 90, CONDITION_ALWAYS, EVENT(Func_02003d40)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT(Func_02004880)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT(AirsRockInside_ReadRevealTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT(AirsRockInside_WalkToRevealTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_WEST), 30, CONDITION_ALWAYS, EVENT(Func_020047b0)},
    {EXAMINE_TRIGGER_FACING(FACING_EAST), 30, CONDITION_ALWAYS, EVENT(Func_02004818)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_02003e28)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_02003dd4)},
    {TRIGGER_RAISED, 202, CONDITION_ALWAYS, EVENT(Func_02003cac)},
    {TRIGGER_RAISED, 203, CONDITION_ALWAYS, EVENT(Func_02003cac)},
    {TRIGGER_RAISED, 204, CONDITION_ALWAYS, EVENT(Func_02003cac)},
    {TRIGGER_RAISED, 205, CONDITION_ALWAYS, EVENT(Func_02003cac)},
    {TRIGGER_RAISED, 206, CONDITION_ALWAYS, EVENT(Func_02003cac)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_Room6Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(Func_02002790)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS, EVENT(Func_02002898)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x89f, EVENT(Func_02004884)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 9, CONDITION_ALWAYS, EVENT(Func_02000610)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 9, CONDITION_ALWAYS, EVENT(Func_0200061c)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 10, CONDITION_ALWAYS, EVENT(Func_02000610)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, EVENT(Func_0200061c)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_Room7Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(Func_02002790)},
    {TRIGGER_STEP, 212, CONDITION_ALWAYS, EVENT(Func_020048c4)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x8d0, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x8d0, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 12, 0x200, EVENT(Func_020048bc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 13, 0x201, EVENT(Func_020048bc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, 0x202, EVENT(Func_020048bc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 15, 0x203, EVENT(Func_020048bc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 16, CONDITION_ALWAYS, EVENT(Func_02000610)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 16, CONDITION_ALWAYS, EVENT(Func_0200061c)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 16, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 17, CONDITION_ALWAYS, EVENT(Func_02000610)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(Func_0200061c)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_Room8Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_RAISED, 201, CONDITION_ALWAYS, EVENT(Func_02004ab8)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Func_02000988)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, 0xa8f, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0xa8f, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x8d1, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x8d1, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, 0x8d2, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x8d2, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 10, CONDITION_ALWAYS, EVENT(Func_02000610)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, EVENT(Func_0200061c)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_Room9Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(Func_02002790)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Func_02000988)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02003254)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, 0x8d3, EVENT(Func_02003254)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x8d3, EVENT(Func_0200332c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 8, CONDITION_ALWAYS, EVENT(Func_02000610)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_0200061c)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, EVENT(Func_02001828)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_Room10Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXAMINE, 30, CONDITION_ALWAYS, EVENT(AirsRockInside_CheckPsynergyStone)},
    {TRIGGER_TABLE_END},
};

s32 gData_020073d8 = -1;
#undef Actor_SetHorizontalPositionWithTerrain
