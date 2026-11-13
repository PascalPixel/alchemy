/// @file
/// Routines whose twins are in The Broken Seal's field/event_commands/ply.c.
// Part of f0dcb44.c.

#include "../f0dcb44_internal.h"

struct SwayObject {
  u8 unk00[8];
  s32 y;
  u8 unk0c[44];
  s32 baseY;
  u8 unk3c[40];
  s16 phase;
};

struct OrbitVector {
  s32 x;
  s32 y;
  s32 z;
};

/// Per-frame update: sways 4 pixels either side of the anchor, once every
/// 128 frames.
void BattleEffect_SwayParticle(struct SwayObject *object) {
  s16 *phase = &object->phase;
  s32 amplitude = 0x40000;

  object->y = object->baseY + Iwram_MulQ16(amplitude, Trig_Sin(*phase << 9));
  (*phase)++;
  *phase = (*phase + 128) % 128;
}

/// The particle circles 10 pixels above the scene's position, its radius in
/// pixels the frames it has left; at 0 it runs the shared particle script.
void BattleEffect_UpdateShrinkingOrbitObject(struct Actor *object) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct OrbitVector pos;
  s16 life;
  s32 count;
  s32 radius;
  s32 lift;

  if (object != NULL) {
    count = (u16)object->timer - 1;
    object->timer = count;
    life = count;
    if (life != 0) {
      pos.x = scene->x;
      lift = 160;
      pos.y = scene->y + (lift << 12);
      radius = life << 16;

      pos.z = scene->z;
      Vector_AddPolarOffset__f0de21c(radius, (s16)object->unk66 + (life << 11), &pos);
      SET_POSITION(object, pos.x, pos.y, pos.z);
    } else {
      Actor_StartScriptFar((struct Actor *)object,
                           gBattleEffect_SharedParticleScript);
    }
  }
}

struct SparkObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[24];
  s32 anchorX;
  u8 unk3c[12];
  s32 gravity;
  u8 unk4c[9];
  u8 motionFlags;
  u8 unk56[8];
  u16 unk5e;
  u8 unk60[4];
  s16 phase;
  u8 unk66[6];
  void (*update)(struct SparkObject *);
};

void Func_080de8d0(struct SparkObject *obj) {
  struct BattleFxScene *scene;
  s16 *phase;
  struct SparkObject *spark;
  s32 pos[3];

  scene = Ram_HeapSlots->effectWork;
  phase = &obj->phase;
  if (*phase != -1) {
    s32 amplitude = 6 << 16;

    obj->x = scene->x + Iwram_MulQ16(amplitude, Trig_Sin(*phase << 10));
    obj->y = scene->y + (16 << 16);
    obj->z = scene->z;
    (*phase)++;
    *phase = (*phase + 64) % 64;
  }
  if (gFrameCount % 3 == 0) {
    pos[0] = obj->x;
    pos[1] = obj->y + (2 << 16);
    pos[2] = obj->z;
    Vector_AddPolarOffset__f0de21c(Random16() * 6, Random16(), pos);
    spark = Actor_Spawn__f0de21c(673, pos[0], pos[1], pos[2]);
    if (spark != 0) {
      spark->update = BattleEffect_SwayParticle;
      spark->scaleX = spark->scaleY = 0x9999;
      spark->motionFlags = 2;
      spark->gravity = 458;
      spark->phase = Random16() >> 9;
      spark->anchorX = spark->x;
      Animation_ApplyChildValuesFar(spark, 9);
      spark->unk5e = 72;
      Actor_StartScriptFar((struct Actor *)spark,
                           gBattleEffect_SharedParticleScript);
    }
  }
}

void FieldEffect_RunPly(void);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);

/// Effect 3 cast on a target: runs effect 3, then stops the caster's hook.
void FieldEffect_RunPlyOnTarget(void) {
  FieldEffect_RunPly();
  BattleEffect_StopMainActor();
}

/// Eight particles spiral in on the main object, one a frame; then a centre
/// object flashes for 60 frames, emits sparks and flies off.
void FieldEffect_RunPly(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *target = scene->mainObject;
  struct Actor *object;
  struct ResourceEntry *last;
  u8 marker;
  s32 i;

  BattleEffect_InitializeSharedScene();

  last = 0;
  for (i = 0; i < 8; i++) {
    object = Actor_Spawn__f0de21c(296, target->x, target->y + 0x200000, target->z);
    if (object != 0) {
      object->scaleY = 0xb333;
      object->scaleX = 0xb333;
      object->update = (void (*)(void))BattleEffect_UpdateShrinkingOrbitObject;
      object->timer = 120;
      object->unk66 = i << 13;
      object->motionFlags = 4;
      last = Sprite_ShareVramCacheEntry(object->sprite, last);
    }
    wait(1);
  }

  marker = last->vramBlock;
  Sound_PlayFar(130);
  wait(110);

  target = Actor_Spawn__f0de21c(296, 0, 0, 0);
  object = target;
  if (target != 0) {
    target->scaleY = 0xb333;
    target->scaleX = 0xb333;
    SET_POSITION(target, scene->x, scene->y + 0x100000, scene->z);
    target->motionFlags = 4;
    Animation_ApplyChildValuesFar(target, 7);
  }

  Sound_PlayFar(131);
  wait(12);
  if (target != 0) {
    for (i = 0; i < 30; i++) {
      /* FAKEMATCH(temporary): a redundant copy keeps object as a second
       * register copy of the centre object (r5 beside r6); without it, 123 of
       * 188 instructions differ in tla-en. */
      target = object;
      if (i & 3)
        Animation_ApplyChildValuesFar(object, 9);
      else
        Animation_ApplyChildValuesFar(object, 10);
      wait(2);
    }
  }

  Animation_ApplyChildValuesFar(object, 0);
  Sound_PlayFar(84);
  if (object != 0) {
    target->update = (void (*)(void))Func_080de8d0;
    target->timer = 0;
    if (scene->enabled != 0)
      wait(128);
    else
      wait(192);
  }

  if (target != 0) {
    target->timer = -1;
    target->speedLimit = 0x50000;
    target->acceleration = 0x6666;
    target->actionFlags = 0;
    Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
    Actor_CommitPosition(target);
    Actor_Destroy(target);
  }
  if (marker != 0x60)
    Resource_ResetEntry(marker);
  if (scene->finishCallback != 0)
    scene->finishCallback();
  BattleEffect_PrepareBufferInterpolation();
}

struct OwnerObject {
  u8 unk00[8];
  s32 x;
  u8 unk0c[4];
  s32 z;
  u8 unk14[14];
  u8 layer;
};

/* VIEW(f0de21c): tag struct EventWork as struct EventWork__f0de21c; event_work.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/event_commands/f0dcb44.c:2076: structure has no member named `state'. */
struct EventWork__f0de21c {
  u8 unk000[0x166];
  u16 state;
};

s32 Party_GetLeaderActor(void);
s32 Func_080dbca8(s32 x, s32 z, s32 layer);

/// A task of Cloak: when the controlled character does not
/// stand on a tile Cloak can hide on, requests CLOAK_END (0x2092).
void FieldEffect_CheckCloakTile(void) {
  struct EventWork__f0de21c *event = Ram_HeapSlots->eventWork;
  struct OwnerObject *owner = ObjectTable_Get(Party_GetLeaderActor());

  if (Func_080dbca8(owner->x, owner->z, owner->layer) == 0)
    event->state = 0x2092;
}


// ⚓️'s composite sprite (as worka0/f0daecc.c's CompositeAnimation).
struct CloakSprite {
  u8 unk00[0x19];
  u8 dirty; // redraw the parts
  u8 blend; // 2 draws the caster half transparent, 1 shows it plainly
  u8 unk1b[0x0d];
  struct AnimationEntry *entries[4];
};

struct CloakCaster {
  u8 unk00[0x50];
  struct CloakSprite *sprite;
  u8 unk54[0x18];
  void (*hook)(void);
};

#define SE_CLOAK_END 154

// The caster's palette while it flickers.
#define CLOAK_FLICKER_PALETTE 7

/// Show the Cloaked caster again: stop checking the ground under it, then
/// flicker it back into view over twenty frames and clear the cloak (The
/// Broken Seal's FieldPsynergy_EndCloak).
void Func_080debd8(void) {
  struct CloakCaster *caster =
      ((struct BattleFxScene *)Ram_HeapSlots->effectWork)->mainObject;
  struct CloakSprite *sprite = caster->sprite;
  struct AnimationEntry *entry = sprite->entries[0];
  u32 i;

  Sound_PlayFar(SE_CLOAK_END);
  Task_Remove(FieldEffect_CheckCloakTile);
  Actor_SetMode((struct Actor *)caster, 0);
  caster->hook = 0;

  // Blink between the flicker palette half transparent and solid...
  for (i = 0; i < 5; ++i) {
    entry->param = CLOAK_FLICKER_PALETTE;
    sprite->blend = 2;
    sprite->dirty = 1;
    wait(2);
    sprite->dirty = 1;
    sprite->blend = 0;
    wait(2);
  }

  // ...then between the flicker palette and the caster's own.
  for (i = 0; i < 5; ++i) {
    entry->param = CLOAK_FLICKER_PALETTE;
    sprite->blend = 0;
    sprite->dirty = 1;
    wait(2);
    entry->param = 0;
    sprite->dirty = 1;
    wait(2);
  }

  sprite->blend = 1;
  gMainRoutineWork.cloaked = 0;
}

void FieldEffect_ResumeCloakCheck(void) {
  if (gMainRoutineWork.cloaked != 0)
    Task_Add(FieldEffect_CheckCloakTile, 0x480);
}
#undef SE_CLOAK_END
#undef CLOAK_FLICKER_PALETTE
