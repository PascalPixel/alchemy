/// @file
/// A summon tablet's runes flying as it is read, and placing the flag blocks.
// Part of radial.c.

#include "../radial_internal.h"
#include "fixed_math.h"
#include "sounds.h"

extern const s32 gFieldEffect_TargetColors[];
extern const u8 Data_080f1040[];
extern const u8 gData_080f3614[];
extern char ResourceId_Runes, ResourceId_Pebbles;

void Field_StartColorTransition(s32 frames, s32 color);
void Camera_WorldToScreen(s32 *position);
void Actor_ArmCallback(s32 id, s32 value, s32 flags);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void SummonTablet_UpdateMotes(void);

/// Plays a summon tablet's reading (a treasure of type 132, "examined the
/// stone tablet"): the tablet's runes and pebbles fly as motes between it
/// and the leader, with their sounds, before Summon_ShowLearnedMessage.
void SummonTablet_PlayRunes(s32 targetId) {
  struct MoteWork *work = HeapSlot_Alloc(92, sizeof(struct MoteWork));
  s32 ownerId = Party_GetLeaderActor();
  struct Actor *owner = ObjectTable_Get(ownerId);
  struct Actor *target = ObjectTable_Get(targetId);
  s32 state;
  s32 moteEntry;
  s32 sparkEntry;
  s32 ownerX;
  s32 ownerZ;
  s32 targetX;
  s32 targetZ;
  s32 sparks = 0;
  s32 nextSpark = 0;
  s32 pos[3];
  struct EffectSprite *sprite;
  struct ChasingMote *mote;
  s32 moteTile;
  s32 sparkTile;
  s32 timer;
  s32 wave;
  s32 delay;
  s32 angle;
  s32 i;
  s32 startX = 130 * Q16_ONE;
  s32 startZ = 30 * Q16_ONE;

  Field_StartColorTransition(8, gFieldEffect_TargetColors[gFrameCount & 7]);
  pos[0] = owner->x;
  pos[1] = owner->y + 24 * Q16_ONE;
  pos[2] = owner->z;
  Camera_WorldToScreen(pos);
  ownerX = pos[0];
  ownerZ = pos[2];
  work->targetX = ownerX;
  work->targetZ = ownerZ;
  pos[0] = target->x;
  pos[1] = target->y;
  pos[2] = target->z;
  Camera_WorldToScreen(pos);
  targetX = pos[0];
  targetZ = pos[2];
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_Runes), work);
  moteEntry = VramCache_FindFreeEntry();
  moteTile = VramCache_Load(moteEntry, 0x800, work);
  mote = work->motes;
  i = 0;
  sprite = work->moteSprites;
  for (; i < 48; i++) {
    EffectSprite_Init(sprite, 4, 4, 0,
                      moteTile + ((Random16() << 4) >> 16) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 1;
    sprite->depth = 240;
    mote->x = targetX;
    mote->z = targetZ;
    mote->targetX = ((i % 16) * 3 << 18) + 30 * Q16_ONE;
    mote->targetZ = ((i / 16) * 3 << 19) + 30 * Q16_ONE;
    mote->timer = -i;
    sprite++;
    mote++;
  }
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_Pebbles), work);
  sparkEntry = VramCache_FindFreeEntry();
  sparkTile = VramCache_Load(sparkEntry, 0x800, work);
  work->sparkTile = sparkTile;
  mote = work->sparks;
  sprite = work->sparkSprites;
  for (i = 31; i >= 0; i--) {
    EffectSprite_Init(sprite, 8, 8, 0x40000000, sparkTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    mote->timer = -1;
    sprite++;
    mote++;
  }
  wait(1);
  Sound_PlayFar(SE_139);
  Actor_SelectAnimationFar(target, 2);
  Event_Wait(60);
  target->timer = 0;
  target->update = SummonTablet_CyclePictures;
  Sound_PlayFar(SE_SWIRL);
  Event_Wait(80);
  Actor_StartScriptFar(target, Data_080f1040);
  work->moving = 0;
  Task_Add(SummonTablet_UpdateMotes, 0x480);
  state = 0;
  timer = 0;
  do {
    switch (state) {
    case 0:
      work->state = 0;
      if (timer <= 39) {
        if ((timer & 7) == 0)
          Sound_PlayFar(SE_IMPACT);
        sparks = 1;
      }
      if (timer == 40) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        sparks = 25;
        Actor_StartIdleFar(target);
        target->scaleX = 0;
      }
      if (timer == 80) {
        state++;
        timer = -1;
      }
      break;
    case 1:
      for (i = 0; i < 48; i++) {
        struct EffectSprite *rune;
        wave = i % 16 + i / 16;
        wave = timer - wave;
        rune = &work->moteSprites[i];
        if (wave == 0)
          rune->oam.index += 32;
        if (wave == 3)
          rune->oam.index += -32;
        if (wave == 8)
          rune->oam.index += 32;
        if (wave == 10)
          rune->oam.index += -32;
        if (wave == 16)
          rune->oam.index += 32;
        if (wave == 18)
          rune->oam.index += -32;
      }
      if (timer == 0)
        Sound_PlayFar(SE_GLITTER);
      if (timer == 56) {
        state++;
        timer = -1;
      }
      break;
    case 2:
      if (timer == 0) {
        Actor_ArmCallback(ownerId, 0x4000, 0);
        break;
      }
      if (owner->script[owner->step] == 17) {
        state++;
        timer = -1;
      }
      if (timer == 60) {
        state++;
        timer = -1;
      }
      break;
    case 3:
      if (timer == 0)
        Actor_SelectAnimationFar(owner, 28);
      if (timer == 10) {
        state++;
        timer = -1;
      }
      break;
    case 4:
      if (timer == 0) {
        mote = work->motes;
        delay = 0;
        for (i = 47; i >= 0; i--) {
          mote->timer = delay;
          delay -= 3;
          mote++;
        }
        work->radius = 60 * Q16_ONE;
        work->angle = TRIG_TURN * 3 / 4;
        work->originX = startX;
        work->originZ = startZ;
        work->moving = 1;
        work->spacing = 4 * Q16_ONE;
        work->state = 2;
      }
      if (timer == 0)
        Sound_PlayFar(SE_FALLING_LIGHT);
      if (timer == 48)
        Sound_PlayFar(SE_FALLING_LIGHT);
      if (timer == 96)
        Sound_PlayFar(SE_FALLING_LIGHT);
      if (timer == 100) {
        state++;
        timer = -1;
      }
      break;
    case 5:
      work->originX = timer * (ownerX - startX) / 30 + startX;
      work->originZ = timer * (ownerZ - startZ) / 30 + startZ;
      if (timer == 30) {
        state++;
        timer = -1;
      }
      break;
    case 6:
      if (timer == 0)
        work->spacing = 0;
      work->radius += -2 * Q16_ONE;
      if (work->radius < 0) {
        work->radius = 0;
        state++;
        timer = -1;
      }
      break;
    case 7:
      if (timer == 0)
        Actor_StartScriptFar(owner, gData_080f3614);
      if ((timer & 3) == 0)
        Sound_PlayFar(SE_MAP_TRANSITION);
      work->state = 3;
      if (timer == 60)
        state = 999;
      break;
    }
    for (i = 0; i < sparks; i++) {
      angle = Random16();
      mote = &work->sparks[nextSpark];
      mote->timer = 0;
      mote->x = target->x;
      mote->z = target->y + 12 * Q16_ONE;
      mote->unk08 = target->z;
      Vector_AddPolarOffset(Random16() * 4 + 2 * Q16_ONE, angle, &mote->x);
      mote->targetX = 0;
      mote->targetZ = Q16_ONE * 9 / 2;
      mote->unk14 = 0;
      Vector_AddPolarOffset(Random16() * 2 + 2 * Q16_ONE, angle,
                            &work->sparks[nextSpark].targetX);
      nextSpark = (nextSpark + 1) & 31;
    }
    sparks = 0;
    wait(1);
    timer++;
  } while (state != 999);
  Actor_StartIdleFar(owner);
  owner->scaleX = Q16_ONE;
  owner->scaleY = Q16_ONE;
  Event_Wait(10);
  Task_Remove(SummonTablet_UpdateMotes);
  FieldPsynergy_FadeFromCastColor();
  VramCache_FreeEntry(moteEntry);
  VramCache_FreeEntry(sparkEntry);
  HeapSlot_Release(92);
}

#include "field/field.h"

struct ActorFlag {
  u16 actor;
  s16 flag;
};

struct Actor *Actor_GetById(u32 objectId);
s32 Actor_GetCellIndex(struct Actor *object);
s32 GameFlag_Get(s32 flag);
void GameFlag_Clear(s32 flag);
void FlagBlock_ApplyFlags(struct ActorFlag *list);

/// Sets up a scene's flag blocks (pairs of actor and game flag, ended by
/// actor 0xffff): each stands still on its cell, which nothing may enter, shown
/// up (animation 0) or down (1) by its flag; then FlagBlock_ApplyFlags raises
/// the cells of those up, and a leader below the ground is lifted onto it.
/// Daila, Kibombo, Madra's catacombs, the Izumo ruins, Air's Rock and the
/// Lemurian Ship call it as they start.
// XXX: what the blocks are in the game (pillars, posts?); a render of Daila's
// would settle it.
void FlagBlock_PlaceAll(struct ActorFlag *actors) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ActorFlag *list = actors;
  struct Actor *leader;
  s32 height;

  for (; actors->actor != 0xffff; actors++) {
    struct Actor *object = Actor_GetById(actors->actor);
    u16 flag = actors->flag;
    s32 slot = Actor_GetCellIndex(object);
    struct MapCell *cell = &map->layers[object->terrainId].cells[slot];

    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    Actor_SetSpriteFlagsFar(object, 0);
    cell->collisionCode = 0xff;
    if (GameFlag_Get(flag)) {
      Actor_SelectAnimationFar(object, 0);
      GameFlag_Clear(flag);
    } else {
      Actor_SelectAnimationFar(object, 1);
    }
  }
  FlagBlock_ApplyFlags(list);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = MapGrid_GetTerrainHeightFar(leader->terrainId, leader->x, leader->z);
  if (leader->y < height)
    leader->y = leader->terrainHeight = height;
}

struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

struct DescriptorEntry {
  s16 id;
  u8 unk02[6];
};

extern void *gOverlayArea[];
extern struct DescriptorEntry gData_080eff28[];

void BattleEffect_StartTiledCanvas(s32 mode);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_RunDrainMotes(void *effect, s32 mode);
void PartyStatus_CreateWindow(s32 flags);
s32 MapGrid_GetTerrainHeightAboveFar(s32 x, s32 z, s32 layer);
s32 MapGrid_GetAttributeBAt(s32 x, s32 z, s32 layer);
u8 MapGrid_SetAttributeBAt(s32 x, s32 z, s32 layer, s32 value);
void Shop_InitializeWork(void);
s32 Inn_RoomPrice(s32 innId);
void Shop_ReleaseWork(void);

/// Sets attribute bit 0x80 (raised) on the cell at (x, z) where no ground lies
/// above it.
void MapGrid_MarkRaisedIfOpen(s32 x, s32 z, s32 layer) {
  if (MapGrid_GetTerrainHeightAboveFar(x, z, layer) == 0)
    MapGrid_SetAttributeBAt(x, z, layer,
                            MapGrid_GetAttributeBAt(x, z, layer) | 0x80);
}
