#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "task.h"
#include "resource.h"
#include "heap_release.h"
#include "effect_sprite.h"
#include "battle.h"

// Kind 24's work (battle_fx_work[0]), as Func_080e9940's draft has it.
struct GlowWork {
  u8 unk0000[0x1000];
  struct EffectSprite sprite;
  s32 scale;
  s16 resource;
  u16 tile;
  u16 unk1030;
  u16 phase;
  s16 fading;
};

struct GlowScene {
  u8 unk00[0x10];
  struct Actor *mainObject;
  u8 unk14[4];
  s16 unk18;
  s16 unk1a;
  u8 unk1c[2];
  s16 kind;
};

struct GlowEvent {
  u8 unk000[0x14];
  struct Actor *objects[80];
  u8 unk154[0xbf0];
  s16 glow;
  s16 glowHeld;
};

extern char ResourceId_RingOrb;

void wait(s32 frames);
s32 Func_080dbcc0(s32 x, s32 z, s32 layer);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);
void Func_080e9940(void);

void Func_080e9af4(void) {
  struct GlowWork *work = HeapSlot_Alloc(92, 0x1038);
  struct GlowScene *scene = Ram_HeapSlots->effectWork;
  struct GlowEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *player = scene->mainObject;
  struct PendingEvent *pending;
  struct EffectSprite *sprite;
  struct Actor *other;
  s32 position[3];
  s32 found;
  s32 slot;
  struct Actor **list;
  s32 tile;
  s32 i;
  s32 n;

  pending = Func_080ce458(0x50000005, scene->kind, Func_080ce31c(scene->kind));
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_RingOrb), work);
  slot = Resource_FindFreeEntry();
  tile = VramCache_Load(slot, 0x800, work);
  work->resource = slot;
  work->tile = tile;
  work->scale = 0;
  sprite = &work->sprite;
  Func_080eaf98(sprite, 15, 15, 0x80000000, (s16)work->tile);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  Actor_SetPositionAndResetMotionFar(player, player->x, player->y, player->z);
  BattleEffect_SetObjectUpdateMode(player, 0);
  Actor_SetMode(player, 5);
  Sound_PlayFar(130);
  wait(1);
  work->unk1030 = 0;
  work->phase = 0;
  work->fading = 0;
  Task_Add(Func_080e9940, TASK_PASS_VBLANK);

  found = 0;
  if (Func_080dbcc0(player->x, player->z, player->terrainId) == 0) {
    found = 1;
  } else {
    for (n = 0; n < 80; n++) {
      other = event->objects[n];
      if (other != NULL && other->script != NULL &&
          (other->collisionFlags & 1) && other != scene->mainObject) {
        position[0] = other->x;
        position[1] = other->terrainHeight;
        position[2] = other->z;
        if (Func_080dbe80(&player->x, player->radius, &other->x,
                          other->radius - 2) >= 0) {
          found = 1;
          break;
        }
      }
    }
  }

  if (found) {
    wait(5);
    Sound_PlayFar(246);
    for (i = 0; i < 32; i++) {
      work->scale = i << 11;
      wait(1);
    }
    wait(10);
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, scene->unk18, scene->unk1a);
    for (i = 31; i != 0; i--) {
      work->scale = i << 11;
      wait(1);
    }
    wait(5);
    Task_Remove(Func_080e9940);
    Resource_ResetEntry(work->resource);
    HeapSlot_Release(92);
  } else {
    player->motionFlags = 0;
    player->y = player->terrainHeight;
    work->scale = 0;
    event->glowHeld = 0;
    event->glow = 100;
    for (i = 0; i < 32; i++) {
      if (i == 10)
        Sound_PlayFar(246);
      player->y += 0x10000;
      work->scale = i << 11;
      wait(1);
    }
    player->motionFlags = 4;
    player->y = player->terrainHeight + 0x248000;
    player->gravity = 0x30000;
    gMainRoutineWork.renderMode = 10;
    work->fading = 1;
    event->glowHeld = 1;
    event->glow = 112;
  }
}

// Kind 24's work (battle_fx_work[0]).

struct Actor *Actor_GetById(s32 id);

void Func_080e9db4(void) {
  struct GlowWork *work = (struct GlowWork *)Ram_HeapSlots->battleFxWork[0];
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());
  s32 i;

  Task_Remove(Func_080e9940);
  Actor_SetPositionAndResetMotionFar(object, object->x, object->y, object->z);
  object->motionFlags = 3;
  gMainRoutineWork.renderMode = 0;
  for (i = 0; i < 60; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  Sound_PlayFar(132);
  wait(1);
  for (i = 0; i < 20; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  wait(1);
  for (i = 0; i < 20; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  wait(1);
  object->bounce = 0;
  object->velocityY = 0;
  object->y = object->terrainHeight;
  object->gravity = 0x10000;
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(92);
}

#include "iwram_call.h"
#include "field_event.h"

struct SpritePictures {
  u8 unk00[5];
  u8 picture;
};

struct AnimatedSprite {
  u8 unk00[0x12];
  s16 rotation;
  u8 unk14[5];
  u8 picture;
  u8 nextPicture;
  u8 unk1b[0xd];
  struct SpritePictures *pictures;
};

/// Steps the sprite through a four-picture cycle on the actor's timer, as
/// overlay 68c's Func_02001398 does, but leaves the next picture alone on the
/// fourth step.
void Func_080e9e8c(struct FieldActor *object) {
  struct AnimatedSprite *sprite = (struct AnimatedSprite *)object->sprite;
  struct SpritePictures *pictures = sprite->pictures;

  switch (object->timer & 3) {
  case 0:
    pictures->picture = 7;
    sprite->picture = 1;
    sprite->nextPicture = 2;
    break;
  case 1:
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  case 2:
    pictures->picture = 7;
    sprite->picture = 1;
    sprite->nextPicture = 0;
    break;
  case 3:
    pictures->picture = 0;
    sprite->picture = 1;
    break;
  }
  object->timer++;
}

struct Chase {
  s32 x;
  s32 z;
  s32 unk08;
  s32 targetX;
  s32 targetZ;
};

s32 ArcTan2(s32 y, s32 x);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

/// Moves toward the target by speed, stopping on it on overshoot.
void Motion_StepTowardTarget(struct Chase *chase, s32 speed) {
  s32 dx = chase->targetX - chase->x;
  s32 dz = chase->targetZ - chase->z;
  u16 angle = ArcTan2(dz, dx);

  chase->x += Iwram_MulQ16(Trig_Cos(angle), speed);
  chase->z += Iwram_MulQ16(Trig_Sin(angle), speed);
  {
    s32 nextDx = chase->targetX - chase->x;
    s32 nextDz = chase->targetZ - chase->z;

    if ((nextDx ^ dx) & 0x80000000)
      chase->x = chase->targetX;
    if ((dz ^ nextDz) & 0x80000000)
      chase->z = chase->targetZ;
  }
}

// One mote: a position chasing its target (Motion_StepTowardTarget's struct
// Chase) and a frame count, negative before it starts.
struct Mote {
  s32 x;
  s32 z;
  s32 unk08;
  s32 targetX;
  s32 targetZ;
  s32 unk14;
  s32 timer;
};

struct MoteWork {
  u8 unk0000[0x800];
  struct Mote sparks[32];
  struct EffectSprite sparkSprites[32];
  struct Mote motes[48];
  struct EffectSprite moteSprites[48];
  s32 originX;
  s32 originZ;
  s32 angle;
  s32 radius;
  s32 spacing;
  s8 state;
  s8 moving;
  u16 sparkTile;
  s32 targetX;
  s32 targetZ;
};

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct Mote *mote);

void Func_080e9f68(void) {
  struct MoteWork *work = Ram_HeapSlots->battleFxWork[0];
  s32 position[3];
  struct Mote *mote;
  struct EffectSprite *sprite;
  s32 i;
  u16 *tile;

  if (work->moving != 0) {
    position[0] = work->originX;
    position[2] = work->originZ;
    Vector_AddPolarOffset(work->radius, work->angle, position);
    mote = work->motes;
    mote->targetX = position[0];
    mote->targetZ = position[2];
    work->angle += 0x600;
    for (i = 0; i < 48; i++, mote++) {
      if (mote->timer >= 0) {
        if (i != 0) {
          mote->targetX = position[0];
          mote->targetZ = position[2];
        }
        position[0] = mote->x;
        position[2] = mote->z;
        Vector_AddPolarOffset(work->spacing, 0, position);
      }
      mote->timer++;
    }
  }

  mote = work->motes;
  sprite = work->moteSprites;
  for (i = 47; i >= 0; i--, sprite++, mote++) {
    if (work->state == 0) {
      if (mote->timer >= 0) {
        Motion_StepTowardTarget(mote, 0x40000);
        sprite->x = mote->x;
        sprite->y = mote->z;
        Func_080eb01c(sprite);
      }
      mote->timer++;
    } else if (work->state == 2) {
      Motion_StepTowardTarget(mote, 0x60000);
      sprite->x = mote->x;
      sprite->y = mote->z;
      Func_080eb01c(sprite);
    } else if (work->state == 3) {
      if (mote->x != work->targetX || mote->z != work->targetZ) {
        Motion_StepTowardTarget(mote, 0x60000);
        sprite->x = mote->x;
        sprite->y = mote->z;
        Func_080eb01c(sprite);
      }
    }
  }

  mote = work->sparks;
  tile = &work->sparkTile;
  sprite = work->sparkSprites;
  for (i = 31; i >= 0; i--, sprite++, mote++) {
    if ((u32)mote->timer < 20) {
      sprite->oam.index = *tile + (mote->timer / 5 & 7) * 8;
      EffectSprite_DrawAtWorldPosition(sprite, mote);
      BattleEffect_IntegrateVector3(&mote->x, 63, -0x8000);
      mote->timer++;
    }
  }
}
