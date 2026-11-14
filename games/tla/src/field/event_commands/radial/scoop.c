/// @file
/// Scoop: digging, the object dropped at the effect's anchor and the item dug
/// up.
// Part of radial.c.

#include "../radial_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"
#include "field/field_work.h"
#include "battle/battle_unit.h"
#include "window.h"
#include "bg_scroll.h"

struct Work080e6400 {
  u8 tiles[0x400];
  u8 picture[0x400];
  struct EffectSprite sprites[64];
  struct EffectParticle particles[64];
  struct EffectSprite pictureSprite;
  s16 phase;
  s16 timer;
  s16 particleTile;
  s16 pictureTile;
  struct Actor *anchor;
  struct Actor *other;
  s16 mode;
};

#define NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

/// Task of the Scoop: plays the dig on the anchor phase by phase.
void Scoop_Update(void) {
  struct Work080e6400 *work;
  struct Actor *anchor;
  struct EffectSprite *sprite;
  struct EffectParticle *particle;
  u8 *vram;
  u8 *picture;
  s16 *phase;
  s32 jitter;
  s32 i;

  work = (struct Work080e6400 *)Ram_HeapSlots->battleFxWork[0];
  anchor = work->anchor;
  if (work->mode == 0) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = Q16_ONE;
      if (work->timer == 4)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SelectAnimationFar(anchor, 3);
      if (work->timer <= 15)
        anchor->y += 0x24ccc;
      if (work->timer == 28)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 25)
        work->other->scaleX = 0;
      if (work->timer == 50)
        work->phase = 999;
      break;
    }
    phase = &work->phase;
    if (*phase == 2) {
      picture = work->picture;
      vram = (u8 *)OBJ_VRAM0 + work->pictureTile * TILE_SIZE_4BPP;
      Picture_ShiftColumnsByWave(work->tiles, picture, work->timer + 14);
      ((const u8 *(*)(const u8 *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
          picture, 32, 32, vram);
      sprite = &work->pictureSprite;
      EffectSprite_Draw(sprite);
    }
    if (*phase == 2) {
      for (i = 0, particle = work->particles; i < 18; i++, particle++) {
        sprite = &work->sprites[i];
        if (particle->timer >= 0 && particle->timer < 32)
          BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                   work->particleTile);
        particle->timer++;
      }
    }
  } else if (work->mode == 3) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = Q16_ONE;
      if (work->timer == 4)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SelectAnimationFar(work->anchor, 3);
      if (work->timer <= 15)
        anchor->y += 0x24ccc;
      if (work->timer == 28)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 70)
        work->other->scaleX = 0;
      if (work->timer == 90)
        work->phase = 999;
      break;
    }
    if (work->phase == 2) {
      for (i = 0, particle = work->particles; i < 64; i++, particle++) {
        if (particle->timer >= 0 && particle->timer < 45) {
          sprite = &work->sprites[i];
          EffectSprite_DrawAtWorldPosition(sprite, particle);
          if (particle->timer < 42) {
            particle->y -= Q16_ONE;
            jitter = Random16();
            particle->x += jitter - (s32)Random16();
          }
        }
        particle->timer++;
      }
    }
  } else if (work->mode == 1) {
    switch (work->phase) {
    case 0:
      if (work->timer == 8)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SelectAnimationFar(work->anchor, 4);
      anchor->y += 0x24ccc;
      if (work->timer == 15)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 20)
        work->phase = 999;
      break;
    }
  } else {
    switch (work->phase) {
    case 0:
      if (work->timer == 8)
        NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SelectAnimationFar(work->anchor, 4);
      anchor->y += 0x24ccc;
      if (work->timer == 15)
        NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 30)
        work->phase = 999;
      break;
    }
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 20; i++, sprite++, particle++) {
      if (particle->timer >= 0 && particle->timer < 32)
        BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                 work->particleTile);
      particle->timer++;
    }
  }
  work->timer++;
}

/// Effect 20 cast on a target: runs the effect, ignoring the target.
void FieldPsynergy_ScoopOnTarget(void) { FieldPsynergy_Scoop(); }

struct RiseParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct ScoopWork {
  u8 tiles[0x400];
  u8 picture[0x400];
  struct EffectSprite sprites[64];
  struct RiseParticle particles[64];
  struct EffectSprite pictureSprite;
  s16 phase;
  s16 timer;
  s16 particleTile;
  s16 pictureTile;
  struct Actor *object;
  struct Actor *other;
  s16 mode;
};

extern char ResourceId_DustPuffA, ResourceId_DustPuffB, ResourceId_WhiteSparkle,
    ResourceId_Data1E9;

s32 MapGrid_CanScoop(s32 x, s32 z, s32 layer);
void Camera_WorldToScreen(s32 *position);
void Scoop_DigUpItem(void);

void FieldPsynergy_Scoop(void) {
  struct ScoopWork *work = HeapSlot_Alloc(92, 0x193c);
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *caster = scene->mainObject;
  struct Actor *other = NULL;
  struct Actor *object;
  struct Actor *target;
  struct EffectSprite *sprite;
  struct RiseParticle *particle;
  void *pending;
  void *data;
  s32 particleEntry = 96;
  s32 particleTile;
  s32 pictureEntry = 96;
  s32 pictureTile;
  s32 pos[3];
  s32 failed;
  s8 *alternate;
  s32 stage;
  s32 frame;
  s32 i;

  if (event->mode != 3) {
    SET_POSITION(scene, (caster->x & 0xfff00000) + 0x80000, caster->y,
                 (caster->z & 0xfff00000) + 0x80000);
    Vector_AddPolarOffset(px(16), scene->facing, &scene->x);
  }
  FieldPsynergy_FadeToCastColor();
  object = Actor_Spawn(353, 0, 0, 0);
  work->object = object;
  if (object == NULL)
    goto end;
  object->motionFlags = 0;
  if (event->unkd4c & 2) {
    if ((object->animationKind & 15) == 1) {
      ((struct EffectSprite *)object->sprite)->oam.priority = 1;
      object->priorityFlags &= 254;
    }
  } else {
    object->priorityFlags = 1;
  }
  object->facing = caster->facing;
  Actor_SelectAnimationFar(object, 0);
  alternate = &scene->alternate;
  if (*alternate != 0)
    Actor_SetSpriteColorModeFar(object, 6);
  if (*alternate == 0)
    other = Actor_Spawn(354, scene->x, scene->y, scene->z);
  else
    other = Actor_Spawn(355, scene->x, scene->y, scene->z);
  work->other = other;
  if (other == NULL)
    goto end;
  other->z += px(3);
  other->motionFlags = 0;
  other->priorityFlags = 1;
  other->scaleX = 0;
  if (!(event->unkd4c & 1))
    Func_080dba5c();
  Sound_PlayFar(SE_ITEM_BREAK);
  FieldPsynergy_GrowFromCaster(object, scene->x, scene->y + 0x200000, scene->z);
  wait(10);
  Actor_SelectAnimationFar(object, 1);
  wait(15);
  for (i = 9; i >= 0; i--) {
    object->y += -(px(5) / 2);
    wait(1);
  }
  Actor_SelectAnimationFar(object, 2);
  Sound_PlayFar(SE_WATER);

  failed = 0;
  for (i = 0; i < 80; i++) {
    target = event->actors[i];
    if (target != NULL && target->script != NULL &&
        (Actor_GetSpriteIdFar(target) == 406 || (target->collisionFlags & 1)) &&
        target != scene->mainObject && ((u8 *)target->sprite)[27] != 0 &&
        FieldPsynergy_CheckOverlap(&scene->x, 4, &target->x,
                                   target->radius - 2) >= 0) {
      failed = 1;
      break;
    }
  }
  if (MapGrid_GetCellBehavior(scene->x, scene->z, caster->terrainId) == 255)
    failed = 4;
  if (Field_MeasurePsynergyHeight(scene->fieldKind, caster, &scene->x) == -1)
    failed = 2;
  if (MapGrid_CanScoop(scene->x, scene->z, caster->terrainId) == 0)
    failed = 3;
  if (failed) {
    wait(2);
    for (i = 15; i >= 0; i--)
      BattleEffect_SpawnScatterParticle(object->x, object->y, object->z);
    Sound_PlayFar(SE_MENU_ERROR);
    wait(1);
    goto end;
  }

  if (scene->enabled != 0) {
    if (Random16() < 0x1000)
      work->mode = 1;
    else if (*alternate == 0)
      work->mode = 0;
    else
      work->mode = 3;
  } else {
    work->mode = 2;
  }

  if (work->mode == 0) {
    Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                        work);
    particleEntry = VramCache_FindFreeEntry();
    particleTile = VramCache_Load(particleEntry, 0x800, work);
    work->particleTile = particleTile;
    i = 0;
    particle = work->particles;
    for (; i < 64; i++, particle++) {
      sprite = &work->sprites[i];
      EffectSprite_Init(sprite, 8, 8, 0x40000000, particleTile);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(other);
      sprite->depth = BattleEffect_GetObjectDepth(other) + 1;
      sprite->scaleY = sprite->scaleX = 0x3333;
      SET_POSITION(particle, scene->x, scene->y, scene->z);
      Vector_AddPolarOffset(px(2), Random16(), &particle->x);
      particle->velocityX = 0;
      particle->velocityY = 0;
      particle->velocityZ = 0;
      Vector_AddPolarOffset(0x13333, Random16(), &particle->velocityX);
      particle->timer = -i * 2;
    }
  } else if (work->mode == 2) {
    if (*alternate == 0)
      data = Resource_GetTableEntry((s32)&ResourceId_DustPuffA);
    else
      data = Resource_GetTableEntry((s32)&ResourceId_DustPuffB);
    Decompress_TaggedLz(data, work);
    particleEntry = VramCache_FindFreeEntry();
    particleTile = VramCache_Load(particleEntry, 0x800, work);
    work->particleTile = particleTile;
    i = 0;
    particle = work->particles;
    for (; i < 64; i++, particle++) {
      sprite = &work->sprites[i];
      EffectSprite_Init(sprite, 8, 8, 0x40000000, particleTile);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(other);
      sprite->depth = BattleEffect_GetObjectDepth(other) + 1;
      SET_POSITION(particle, scene->x, scene->y, scene->z);
      Vector_AddPolarOffset(px(2), Random16(), &particle->x);
      particle->velocityX = 0;
      particle->velocityY = 0;
      particle->velocityZ = 0;
      Vector_AddPolarOffset(px(1), Random16(), &particle->velocityX);
      particle->timer = -i;
    }
  } else if (work->mode == 3) {
    Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhiteSparkle),
                        work);
    particleEntry = VramCache_FindFreeEntry();
    particleTile = VramCache_Load(particleEntry, 0x80, work);
    work->particleTile = particleTile;
    i = 0;
    particle = work->particles;
    sprite = work->sprites;
    for (; i < 64; i++) {
      EffectSprite_Init(sprite, 4, 4, 0, particleTile + (i & 1) * 2);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(object);
      sprite->depth = BattleEffect_GetObjectDepth(object) - 1;
      SET_POSITION(particle, scene->x, scene->y + 0x280000, scene->z);
      Vector_AddPolarOffset(Random16() * 4 + 0x30000, Random16(), &particle->x);
      particle->timer = -i * 3 / 4;
      sprite++;
      particle++;
    }
  }

  if (work->mode == 0) {
    Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_Data1E9), work);
    pictureEntry = VramCache_FindFreeEntry();
    pictureTile = VramCache_Load(pictureEntry, 0x400, NULL);
    work->pictureTile = pictureTile;
    sprite = &work->pictureSprite;
    EffectSprite_Init(sprite, 16, 0, 0x80000000, pictureTile);
    sprite->depth = BattleEffect_GetObjectDepth(scene->mainObject);
    sprite->oam.priority = BattleEffect_GetObjectPriority(scene->mainObject);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    pos[0] = object->x;
    pos[1] = object->y + 0x200000;
    pos[2] = object->z;
    Camera_WorldToScreen(pos);
    sprite->x = pos[0];
    sprite->y = pos[2];
  }
  if (work->mode == 0 || work->mode == 3) {
    wait(6);
    work->other->scaleX = Q16_ONE;
    wait(4);
  } else {
    wait(10);
  }

  work->phase = 0;
  work->timer = 0;
  {
    s32 order = 0x480;

    Task_Add(Scoop_Update, order);
  }
  if (work->mode == 1) {
    frame = 0;
    while (work->phase != 999) {
      if (frame == 24)
        Scoop_DigUpItem();
      wait(1);
      frame++;
    }
  } else {
    pending =
        Scene_FindPsynergyTrigger(0x50000005, scene->fieldKind,
                                  Scene_SelectPsynergyTarget(scene->fieldKind));
    frame = 0;
    while (work->phase != 999) {
      stage = 0;
      wait(1);
      if (frame == 30)
        stage = 1;
      if (frame == 50)
        stage = 2;
      {
        /* FAKEMATCH(barrier): frame copied through an empty asm. */
        s32 frameCopy;

        asm("" : "=r"(frameCopy) : "0"(frame));
        scene->stage = stage;
        scene->frame = frame;
        Field_RunPsynergyEvent(pending, stage, frameCopy);
      }
      frame++;
    }
    Field_RunPsynergyEvent(pending, 999, frame);
  }
  Task_Remove(Scoop_Update);
  Sound_PlayFar(SE_MENU_ERROR);
  FieldPsynergy_ShrinkToCaster(object, object->x, object->y, object->z);
  if (particleEntry != 96)
    VramCache_FreeEntry(particleEntry);
  if (pictureEntry != 96)
    VramCache_FreeEntry(pictureEntry);
end:
  Actor_ReleaseFar(object);
  Actor_ReleaseFar(other);
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(92);
}

/// Puts object id at the effect's anchor (if any) and lets it fall under
/// gravity, waits 50 frames, then cues the event with id | 0x1000.
/* VIEW(psynergy): routine BattleEffect_DropObjectAtAnchor as void
 * BattleEffect_DropObjectAtAnchor(s32 id); field/field.h has void
 * BattleEffect_DropObjectAtAnchor(u32 objectId); tried as the definition's void
 * BattleEffect_DropObjectAtAnchor(s32 id), which the object already declares,
 * it does not compile in tla-ja:
 * games/tla/src/field/event_commands/radial/scoop.c:494: conflicting types for
 * `BattleEffect_DropObjectAtAnchor'. */
void BattleEffect_DropObjectAtAnchor__psynergy(s32 id) __asm__(
    "BattleEffect_DropObjectAtAnchor");
#define BattleEffect_DropObjectAtAnchor                                        \
  BattleEffect_DropObjectAtAnchor__psynergy
void BattleEffect_DropObjectAtAnchor(s32 id) {
  struct ModeEvent *event = Ram_HeapSlots->fieldWork;
  struct Actor *anchor;
  struct Actor *object;

  anchor = ((struct AnchorWork *)Ram_HeapSlots->battleFxWork[0])->anchor;
  object = Actor_GetById(id);
  if (anchor != NULL) {
    SET_POSITION(object, anchor->x, anchor->y, anchor->z);
  }
  if (*((s8 *)event + 394) == 3) {
    object->motionFlags = 2;
    object->terrainHeight = 0;
  } else {
    object->motionFlags = 3;
  }
  object->gravity = 0x170a;
  wait(50);
  event->cue = id | 0x1000;
}

struct DropEvent {
  u8 unk000[0xd34];
  u16 item;
};

void *Unit_Get(s32 owner);
extern const u16 gData_080f0fc0[];

/// Object 79 at the anchor, fetched or spawned (kind 489), takes a random item
/// id from gData_080f0fc0's sixteen (0x8001 when PartyInventory_FindRoomFar
/// finds no room) or, past 0xc000 of the roll, a 0x8000-flagged value scaled
/// by the unit's level, and is handed to BattleEffect_DropObjectAtAnchor.
void Scoop_DigUpItem(void) {
  struct AnchorWork *work = (struct AnchorWork *)Ram_HeapSlots->battleFxWork[0];
  struct Actor *anchor;
  struct FieldWork *event;
  struct BattleUnit *unit;
  struct Actor *object;
  s32 item;
  s32 id = 79;

  anchor = work->anchor;
  event = Ram_HeapSlots->fieldWork;
  unit = Unit_Get(gMainRoutineWork.currentOwner);

  object = Actor_GetById(id);
  if (object == NULL) {
    object = Actor_CreateFar(489, anchor->x, anchor->y, anchor->z);
    if (object == NULL)
      return;
    event->actors[id] = object;
  }
  if (Random16() < 0xc000) {
    item = gData_080f0fc0[(Random16() * 16) >> 16];
    if (PartyInventory_FindRoomFar(item) == -1)
      item = 0x8001;
  } else {
    item = (((Random16() * unit->level) * 2) >> 16) + 1 | 0x8000;
  }
  SET_POSITION(object, anchor->x, anchor->y, anchor->z);
  Actor_ShowItemIconById(id, item);
  ((struct DropEvent *)event)->item = item;
  BattleEffect_DropObjectAtAnchor(id);
}
