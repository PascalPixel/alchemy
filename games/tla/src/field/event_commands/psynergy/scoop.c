/// @file
/// Field Psynergy kind 30, an object dropped at the effect's anchor, and the
/// canvas circle and image copies, with routines not yet understood.
// Part of psynergy.c.

#include "../psynergy_internal.h"

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

void Func_080e62f4(u8 *source, u8 *destination, s32 phase);
/* VIEW(f0e6400): routine BattleEffect_UpdateGrowingParticleSprite as void
 * BattleEffect_UpdateGrowingParticleSprite(struct EffectSprite *sprite, struct
 * EffectParticle *particle, s32 tiles); the definition has void
 * BattleEffect_UpdateGrowingParticleSprite(struct
 * EffectSprite__psynergy_effects_11 *sprite, struct EffectParticle *particle,
 * s32 tiles). */
extern void BattleEffect_UpdateGrowingParticleSprite__f0e6400(
    struct EffectSprite *sprite, struct EffectParticle *particle,
    s32 tiles) __asm__("BattleEffect_UpdateGrowingParticleSprite");
/* VIEW(f0e6400): routine EffectSprite_DrawAtWorldPosition as void
 * EffectSprite_DrawAtWorldPosition(struct ScreenSprite *sprite, s32 *position);
 * f0e4a34 has void EffectSprite_DrawAtWorldPosition(struct EffectSprite
 * *sprite, struct GlowParticle *particle). */
extern void EffectSprite_DrawAtWorldPosition__f0e6400(
    struct ScreenSprite *sprite,
    s32 *position) __asm__("EffectSprite_DrawAtWorldPosition");
u32 Random16(void);

#define NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

void Func_080e6400(void) {
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
        Actor_SetMode(anchor, 3);
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
      Func_080e62f4(work->tiles, picture, work->timer + 14);
      // The heap copy of Render_CopyStrided called as returning the source
      // pointer it leaves in r0, which moves picture to r0 after the width's
      // 32; called as void, 2 of 596 instructions differ in tla-en.
      ((const u8 *(*)(const u8 *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
          picture, 32, 32, vram);
      sprite = &work->pictureSprite;
      Func_080eb01c(sprite);
    }
    if (*phase == 2) {
      for (i = 0, particle = work->particles; i < 18; i++, particle++) {
        sprite = &work->sprites[i];
        if (particle->timer >= 0 && particle->timer < 32)
          BattleEffect_UpdateGrowingParticleSprite__f0e6400(sprite, particle,
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
        Actor_SetMode(work->anchor, 3);
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
          EffectSprite_DrawAtWorldPosition__f0e6400(sprite, particle);
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
        Actor_SetMode(work->anchor, 4);
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
        Actor_SetMode(work->anchor, 4);
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
        BattleEffect_UpdateGrowingParticleSprite__f0e6400(sprite, particle,
                                                          work->particleTile);
      particle->timer++;
    }
  }
  work->timer++;
}

void FieldPsynergy_Blaze(void);
void FieldPsynergy_Sand(void);
void FieldPsynergy_Burst(void);
void FieldPsynergy_Pound(void);
void FieldPsynergy_Scoop(void);
void FieldPsynergy_Parch(void);
void FieldPsynergy_Hover(void);
void Func_080e74d8(void);
void FieldPsynergy_FadeFromCastColor(void);
void wait(s32 frames);
s32 Party_GetLeaderActor(void);

/// Effect 20 cast on a target: runs the effect, ignoring the target.
void FieldPsynergy_ScoopOnTarget(void) { FieldPsynergy_Scoop(); }

#include "field/event_work.h"

struct RiseParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

/* VIEW(f0e6400): tag struct RiseWork as struct RiseWork__f0e6400; f0e2c38.c has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy.c:3058: structure has no member
 * named `other'. */
struct RiseWork__f0e6400 {
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

/* VIEW(f0e6400): tag struct FxScene as struct FxScene__f0e6400; f0e2c38.c has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy.c:3028: structure has no member
 * named `facing'. */
struct FxScene__f0e6400 {
  u16 facing;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
  u8 unk21[0x20];
  s8 alternate;
  u8 unk42[0x782];
  s16 stage;
  s16 frame;
};

// The event work with its flag word past the shared struct. Read as a member,
// the flag word takes this struct's alias set, which orders its reads after
// s32 stores but not after an Actor's u8 stores, as the listing has them.
/* VIEW(f0e6400): tag struct FxEvent as struct FxEvent__f0e6400; f0e2c38.c has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy.c:2984: structure has no member
 * named `unkd4c'. */
struct FxEvent__f0e6400 {
  struct EventWork work;
  u8 unkd48[4];
  /// Bit 0 clear runs Func_080dba5c; bit 1 set fixes the risen object's sprite
  /// priority at 1 directly, else its priority flag is set.
  u16 unkd4c;
};

STATIC_ASSERT(offsetof(struct FxEvent__f0e6400, unkd4c) == 0xd4c);

extern char ResourceId_DustPuffA, ResourceId_DustPuffB, ResourceId_WhiteSparkle,
    ResourceId_Data1E9;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void FieldPsynergy_FadeToCastColor(void);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_Destroy(struct Actor *object);
void Func_080dba5c(void);
void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_080dbf94(struct Actor *object, s32 x, s32 y, s32 z);
s32 Func_08020330(struct Actor *object);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 Func_080dbdc8(s32 x, s32 z, s32 layer);
s32 Func_080cda84(s32 id, struct Actor *object, s32 *position);
s32 Func_080dbc04(s32 x, s32 z, s32 layer);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
void *Resource_GetTableEntry(s32 resourceId);
s32 Decompress_TaggedLz(const void *source, void *destination);
s32 VramCache_FindFreeEntry(void);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Camera_WorldToScreen(s32 *position);
s32 Func_080ce31c(s32 kind);
void Func_080e716c(void);

void FieldPsynergy_Scoop(void) {
  struct RiseWork__f0e6400 *work = HeapSlot_Alloc(92, 0x193c);
  struct FxScene__f0e6400 *scene = Ram_HeapSlots->effectWork;
  struct FxEvent__f0e6400 *event = Ram_HeapSlots->eventWork;
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

  if (event->work.controlMode != 3) {
    scene->x = (caster->x & 0xfff00000) + 0x80000;
    scene->y = caster->y;
    scene->z = (caster->z & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x100000, scene->facing, &scene->x);
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
  Actor_SetMode(object, 0);
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
  other->z += 0x30000;
  other->motionFlags = 0;
  other->priorityFlags = 1;
  other->scaleX = 0;
  if (!(event->unkd4c & 1))
    Func_080dba5c();
  Sound_PlayFar(138);
  Func_080dbed0(object, scene->x, scene->y + 0x200000, scene->z);
  wait(10);
  Actor_SetMode(object, 1);
  wait(15);
  for (i = 9; i >= 0; i--) {
    object->y += -0x28000;
    wait(1);
  }
  Actor_SetMode(object, 2);
  Sound_PlayFar(132);

  failed = 0;
  for (i = 0; i < 80; i++) {
    target = event->work.objects[i];
    if (target != NULL && target->script != NULL &&
        (Func_08020330(target) == 406 || (target->collisionFlags & 1)) &&
        target != scene->mainObject && ((u8 *)target->sprite)[27] != 0 &&
        Func_080dbe80(&scene->x, 4, &target->x, target->radius - 2) >= 0) {
      failed = 1;
      break;
    }
  }
  if (Func_080dbdc8(scene->x, scene->z, caster->terrainId) == 255)
    failed = 4;
  if (Func_080cda84(scene->fieldKind, caster, &scene->x) == -1)
    failed = 2;
  if (Func_080dbc04(scene->x, scene->z, caster->terrainId) == 0)
    failed = 3;
  if (failed) {
    wait(2);
    for (i = 15; i >= 0; i--)
      BattleEffect_SpawnScatterParticle(object->x, object->y, object->z);
    Sound_PlayFar(114);
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
      Func_080eaf98(sprite, 8, 8, 0x40000000, particleTile);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(other);
      sprite->depth = BattleEffect_GetObjectDepth(other) + 1;
      sprite->scaleY = sprite->scaleX = 0x3333;
      particle->x = scene->x;
      particle->y = scene->y;
      particle->z = scene->z;
      Vector_AddPolarOffset(0x20000, Random16(), &particle->x);
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
      Func_080eaf98(sprite, 8, 8, 0x40000000, particleTile);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(other);
      sprite->depth = BattleEffect_GetObjectDepth(other) + 1;
      particle->x = scene->x;
      particle->y = scene->y;
      particle->z = scene->z;
      Vector_AddPolarOffset(0x20000, Random16(), &particle->x);
      particle->velocityX = 0;
      particle->velocityY = 0;
      particle->velocityZ = 0;
      Vector_AddPolarOffset(0x10000, Random16(), &particle->velocityX);
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
      Func_080eaf98(sprite, 4, 4, 0, particleTile + (i & 1) * 2);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(object);
      sprite->depth = BattleEffect_GetObjectDepth(object) - 1;
      particle->x = scene->x;
      particle->y = scene->y + 0x280000;
      particle->z = scene->z;
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
    Func_080eaf98(sprite, 16, 0, 0x80000000, pictureTile);
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
    work->other->scaleX = 0x10000;
    wait(4);
  } else {
    wait(10);
  }

  work->phase = 0;
  work->timer = 0;
  {
    // A local holding one constant: keeps 0x480's load where the native has it.
    s32 order = 0x480;

    Task_Add(Func_080e6400, order);
  }
  if (work->mode == 1) {
    frame = 0;
    while (work->phase != 999) {
      if (frame == 24)
        Func_080e716c();
      wait(1);
      frame++;
    }
  } else {
    pending = Scene_FindPsynergyTrigger(0x50000005, scene->fieldKind,
                                        Func_080ce31c(scene->fieldKind));
    frame = 0;
    while (work->phase != 999) {
      stage = 0;
      wait(1);
      if (frame == 30)
        stage = 1;
      if (frame == 50)
        stage = 2;
      {
        /* FAKEMATCH(barrier): frame copied through an empty asm, so its move
         * to r2 comes before pending's to r0; as
         * Field_RunPsynergyEvent(pending, stage, frame), 2 of 938 instructions
         * differ in tla-en. */
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
  Task_Remove(Func_080e6400);
  Sound_PlayFar(114);
  Func_080dbf94(object, object->x, object->y, object->z);
  if (particleEntry != 96)
    VramCache_FreeEntry(particleEntry);
  if (pictureEntry != 96)
    VramCache_FreeEntry(pictureEntry);
end:
  Actor_Destroy(object);
  Actor_Destroy(other);
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(92);
}

struct Actor *Actor_GetById(u32 objectId);

/// Puts object id at the effect's anchor (if any) and lets it fall under
/// gravity, waits 50 frames, then cues the event with id | 0x1000.
void BattleEffect_DropObjectAtAnchor(s32 id) {
  struct ModeEvent *event = Ram_HeapSlots->eventWork;
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

#include "battle_unit.h"

struct DropEvent {
  u8 unk000[0xd34];
  u16 item;
};

struct Actor *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void *Unit_Get(s32 owner);
s32 PartyInventory_FindRoomFar(s32 item);
void Actor_ShowItemIconById(s32 id, s32 item);
extern const u16 gData_080f0fc0[];

/// Object 79 at the anchor, fetched or spawned (kind 489), takes a random item
/// id from gData_080f0fc0's sixteen (0x8001 when PartyInventory_FindRoomFar
/// finds no room) or, past 0xc000 of the roll, a 0x8000-flagged value scaled
/// by the unit's level, and is handed to BattleEffect_DropObjectAtAnchor.
void Func_080e716c(void) {
  struct AnchorWork *work = (struct AnchorWork *)Ram_HeapSlots->battleFxWork[0];
  struct Actor *anchor;
  struct EventWork *event;
  struct BattleUnit *unit;
  struct Actor *object;
  s32 item;
  s32 id = 79;

  anchor = work->anchor;
  event = Ram_HeapSlots->eventWork;
  unit = Unit_Get(gMainRoutineWork.currentOwner);

  object = Actor_GetById(id);
  if (object == NULL) {
    object = Actor_CreateFar(489, anchor->x, anchor->y, anchor->z);
    if (object == NULL)
      return;
    event->objects[id] = object;
  }
  if (Random16() < 0xc000) {
    item = gData_080f0fc0[(Random16() * 16) >> 16];
    if (PartyInventory_FindRoomFar(item) == -1)
      item = 0x8001;
  } else {
    item = (((Random16() * unit->level) * 2) >> 16) + 1 | 0x8000;
  }
  object->x = anchor->x;
  object->y = anchor->y;
  object->z = anchor->z;
  Actor_ShowItemIconById(id, item);
  ((struct DropEvent *)event)->item = item;
  BattleEffect_DropObjectAtAnchor(id);
}

#define WHIRL_COUNT 100

struct WhirlSpark {
  s32 x;
  s32 y;
  s32 z;
  s32 angle;
  s32 unk10;
  s32 radius;
  s32 age;
};

// Heap block 92: the decoded picture, then the sparks and their sprites.
/* VIEW(f0e6400): tag struct WhirlWork as struct WhirlWork__f0e6400; f0e2c38.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy.c:3413: structure has no member
 * named `sparks'. */
struct WhirlWork__f0e6400 {
  u8 picture[0x400];
  struct WhirlSpark sparks[WHIRL_COUNT];
  struct EffectSprite sprites[WHIRL_COUNT];
};

extern char ResourceId_ColouredOrbs;

void Func_080dc954(void);

void Func_080e7238(void) {
  s32 position[3];
  struct WhirlWork__f0e6400 *work = HeapSlot_Alloc(92, sizeof(*work));
  struct Actor *object = ObjectTable_Get(Party_GetLeaderActor());
  struct WhirlSpark *spark;
  struct EffectSprite *sprite;
  s32 next;
  s32 state;
  s32 entry;
  s32 count;
  s32 tiles;
  s32 i;
  s32 angle;
  s32 frame;
  s32 radius;

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs),
                      work);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x400, work);
  for (i = 0, spark = work->sparks; i < WHIRL_COUNT; i++, spark++) {
    sprite = &work->sprites[i];
    Func_080eaf98(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    spark->age = -1;
  }
  Sound_PlayFar(200);
  wait(20);
  object->scaleX = 0;
  object->scaleY = 0;
  Actor_SetSingleChildFlagsFar(object, 0);

  count = 2;
  state = 0;
  next = 0;
  frame = 0;
  do {
    if (count != 0) {
      radius = 0x500000;
      i = count;
      do {
        angle = Random16();
        spark = &work->sparks[next];
        spark->age = 0;
        spark->x = object->x;
        spark->y = object->y + Random16() * 20;
        spark->z = object->z;
        Vector_AddPolarOffset(Random16() << 3, angle, &spark->x);
        next++;
        spark->angle = angle;
        spark->radius = radius;
        next %= WHIRL_COUNT;
        i--;
      } while (i != 0);
    }

    spark = work->sparks;
    sprite = work->sprites;
    for (i = 0; i < WHIRL_COUNT; i++, sprite++, spark++) {
      if (spark->age >= 0) {
        if ((u32)spark->age < 20) {
          s32 sparkAngle = spark->angle;
          s32 sparkRadius = spark->radius;
          position[0] = spark->x;
          position[1] = spark->y;
          position[2] = spark->z;
          Vector_AddPolarOffset(sparkRadius, sparkAngle, position);
          Camera_WorldToScreen(position);
          sprite->x = position[0];
          sprite->y = position[2];
          Func_080eb01c(sprite);
          spark->angle += 0x800;
          spark->radius += -0x40000;
        }
        if (++spark->age == 20)
          spark->age = -1;
      }
    }

    switch (state) {
    case 0:
      if (frame == 20)
        count = 3;
      if (frame == 30) {
        count = 5;
        state = 1;
        frame = -1;
      }
      break;
    case 1:
      object->facing += 0x2000;
      object->scaleX = (frame << 16) / 30;
      if (object->scaleX > 0x10000)
        object->scaleX = 0x10000;
      object->scaleY = object->scaleX;
      if (frame == 40) {
        state = 2;
        frame = -1;
      }
      break;
    case 2:
      if (frame == 0) {
        object->facing = 0x4000;
        Actor_SetSingleChildFlagsFar(object, 1);
        count = 3;
      }
      if (frame == 10)
        count = 1;
      if (frame == 20)
        count = 0;
      if (frame == 40)
        state = 999;
      break;
    }
    wait(1);
    frame++;
  } while (state != 999);
  VramCache_FreeEntry(entry);
  HeapSlot_Release(92);
  Func_080dc954();
}

#define SPARK_COUNT 100

struct SparkParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 age;
};

// Heap block 92: the decoded picture, then the sparks and their sprites.
struct SparkWork {
  u8 picture[0x400];
  struct SparkParticle particles[SPARK_COUNT];
  struct EffectSprite sprites[SPARK_COUNT];
};

void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_ApplyRandomChildValues(void);

void Func_080e74d8(void) {
  struct SparkWork *work = HeapSlot_Alloc(92, sizeof(*work));
  s32 owner = Party_GetLeaderActor();
  struct Actor *object = ObjectTable_Get(Party_GetLeaderActor());
  struct SparkParticle *particle;
  struct EffectSprite *sprite;
  s32 state;
  s32 entry;
  s32 count;
  s32 tiles;
  s32 i;
  s32 spin;
  s32 angle;
  s32 next;
  s32 frame;
  s32 zero = 0;
  s32 age;

  wait(20);
  FieldPsynergy_FadeToCastColor();
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs),
                      work);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x400, work);
  for (i = 0, particle = work->particles; i < SPARK_COUNT; i++, particle++) {
    sprite = &work->sprites[i];
    Func_080eaf98(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    particle->age = -1;
  }
  wait(10);
  Actor_ArmCallback(owner, 0x4000, 0);
  wait(20);
  Actor_SetMode(object, 28);
  object->update = Actor_ApplyRandomChildValues;
  Sound_PlayFar(131);
  wait(30);
  Sound_PlayFar(220);
  Actor_SetMode(object, 3);
  for (spin = 0, frame = 49; frame >= 0; frame--, spin += 60) {
    object->facing += spin + 0x1000;
    wait(1);
  }

  count = 2;
  state = 0;
  next = 0;
  frame = 0;
  do {
    for (i = 0; i < count; i++) {
      angle = Random16();
      particle = &work->particles[next];
      particle->age = zero;
      particle->x = object->x;
      particle->y = object->y + Random16() * 20;
      particle->z = object->z;
      Vector_AddPolarOffset(Random16() << 3, angle, &particle->x);
      if (next & 1) {
        particle->velocityX = zero;
        particle->velocityY = -0x40000;
        particle->velocityZ = zero;
        Vector_AddPolarOffset((Random16() << 2) + 0x40000, angle,
                              &work->particles[next].velocityX);
      } else {
        particle->velocityX = 0;
        particle->velocityY = 0x50000;
        particle->velocityZ = 0;
        Vector_AddPolarOffset((Random16() << 2) + 0x40000, angle,
                              &work->particles[next].velocityX);
      }
      next++;
      next %= SPARK_COUNT;
    }

    for (i = 0, particle = work->particles, sprite = work->sprites;
         i < SPARK_COUNT; i++, sprite++, particle++) {
      age = particle->age;
      if (age >= 0) {
        if ((u32)age < 20) {
          EffectSprite_DrawAtWorldPosition__f0e6400(sprite, particle);
          // Reread in each arm: the two calls share one bl only this way.
          if (i & 1) {
            BattleEffect_IntegrateVector3(&particle->x, 63, 0x8000);
            age = particle->age;
          } else {
            BattleEffect_IntegrateVector3(&particle->x, 63, -0xa000);
            age = particle->age;
          }
        }
        particle->age = ++age;
        if (age == 20)
          particle->age = -1;
      }
    }

    switch (state) {
    case 0:
      if (frame == 10)
        Sound_PlayFar(198);
      object->facing += 0x2000;
      if (frame == 40)
        count = 3;
      if (frame == 50)
        count = 4;
      if (frame == 60) {
        count = 5;
        state = 1;
        frame = -1;
      }
      break;
    case 1:
      object->facing += 0x2000;
      object->scaleX = 0x10000 - (frame << 16) / 30;
      if (object->scaleX < 0)
        object->scaleX = 0;
      object->scaleY = object->scaleX;
      Actor_SetSingleChildFlagsFar(object, 0);
      if (frame == 30) {
        state = 2;
        frame = -1;
      }
      break;
    case 2:
      count = 0;
      if (frame == 20)
        state = 999;
      break;
    }
    wait(1);
    frame++;
  } while (state != 999);
  VramCache_FreeEntry(entry);
  HeapSlot_Release(92);
}

/// Runs Func_080e74d8, fades the palettes back to the saved ones and waits
/// 10 frames.
void Func_080e7804(void) {
  Func_080e74d8();
  FieldPsynergy_FadeFromCastColor();
  wait(10);
}

/// Field Psynergy kind 30's targeted handler.
void FieldPsynergy_TeleportOnTarget(void) {}

/// Field Psynergy kind 30.
void FieldPsynergy_Teleport(void) {
  ObjectTable_Get(Party_GetLeaderActor());
  ((struct ControlEvent *)Ram_HeapSlots->eventWork)->cue = 0xfc89;
}

/// Field Psynergy kind 22 on a chosen target: runs kind 22's animation.
void FieldPsynergy_ParchOnTarget(void) { FieldPsynergy_Parch(); }

#include "window.h"

struct PuffParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 sway;
  s32 rise;
  s32 swayRate;
  s32 timer;
};

struct PuffWork {
  u8 tiles[0x1000];
  struct EffectSprite disc;
  u8 unk1028[0x28];
  s32 discX;
  s32 discY;
  s32 discScaleX;
  s32 discScaleY;
  struct EffectSprite sprites[64];
  struct PuffParticle particles[64];
};

/* VIEW(f0e6400): tag struct ObjectAnimation as struct ObjectAnimation__f0e6400;
 * f0e2c38.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy.c:3796: structure has no member
 * named `unk18'. */
struct ObjectAnimation__f0e6400 {
  u8 unk00[24];
  u8 unk18;
  u8 unk19[15];
  s16 *definition;
};

extern char ResourceId_GlowDisc, ResourceId_WhitePuff;

s32 Func_080dbd5c(s32 x, s32 z, s32 layer);
s32 Func_080db9a8(struct Actor *object, s32 facing);
void BattleEffect_ApplyColorToTargetBuffer(s32 packed, s32 mode);
void MapGrid_SetCellBehavior(s32 x, s32 z, s32 layer, s32 value);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 a, s32 b);
void BattleEffect_FadeToSavedPalettes(s32 frames);

void FieldPsynergy_Parch(void) {
  struct PuffWork *work = HeapSlot_Alloc(92, 0x2160);
  struct HeapSlots *slots;
  struct FxScene__f0e6400 *scene = Ram_HeapSlots->effectWork;
  struct WindowWork *vramWork = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 offset; // byte offset of the event work's 80 object pointers
  struct Actor *caster;
  struct Actor *object;
  struct ObjectAnimation__f0e6400 *animation;
  struct EffectSprite *sprite;
  struct EffectSprite *cursor;
  struct EffectSprite *fresh;
  struct PuffParticle *particle;
  s32 screen[3];
  s32 pos[3];
  s32 phase;
  s32 timer;
  s32 frame;
  s32 entry;
  s32 tile;
  s32 next;
  s32 spawn;
  s32 fade;
  s32 result;
  s32 found;
  s32 kind;
  void *pending;
  s32 scaleY;
  s32 scaleX;
  s32 y;
  s32 x;
  s32 red;
  s32 green;
  s32 blue;
  s32 add;
  s32 packed;
  s32 i;
  u8 *dst;

  caster = scene->mainObject;
  found = -1;
  fade = -1;
  pending = NULL;
  FieldPsynergy_FadeToCastColor();
  pos[0] = caster->x;
  pos[1] = caster->y;
  pos[2] = caster->z;
  Vector_AddPolarOffset(0x200000, scene->facing, pos);

  // The kind and the heap slots are locals that hold one value each, set
  // after i so the kind's constant is built where the listing builds it.
  i = 0;
  kind = 290;
  slots = Ram_HeapSlots;
  for (offset = 20; i < 80; i++, offset += 4) {
    object = *(struct Actor **)((u8 *)slots->eventWork + offset);
    animation = object->sprite;
    if (object->script != NULL && animation->definition[0] == kind &&
        animation->unk18 == 1 &&
        Func_080dbe80(pos, 4, &object->x, object->radius - 2) >= 0) {
      result = 2;
      found = i;
      Actor_GetById(i)->priorityFlags |= 2;
      goto loaded;
    }
  }
  if (Func_080cda84(scene->fieldKind, caster, pos) == -1) {
    result = 0;
    // DEAD: pending is already NULL; the ROM stores it again.
    pending = NULL;
  } else if (Func_080dbd5c(pos[0], pos[2], caster->terrainId) != 0) {
    pending = Scene_FindPsynergyTrigger(
        0x50000005, scene->fieldKind,
        Func_080dbdc8(pos[0], pos[2], caster->terrainId));
    result = 2;
    if (pending == NULL)
      result = 1;
  } else {
    result = 0;
  }
loaded:
  {
    void *source = Resource_GetTableEntry((s32)&ResourceId_GlowDisc);
    dst = (u8 *)0x06011000 + vramWork->glyphTiles * 32;
    Decompress_TaggedLz(source, work);
  }
  Dma_Set(work, dst, (DMA_ENABLE | DMA_32BIT) << 16 | 0x400, REG_DMA3);
  screen[0] = pos[0];
  screen[1] = pos[1];
  screen[2] = pos[2];
  Camera_WorldToScreen(screen);
  work->discX = screen[0];
  work->discY = screen[2];
  work->discScaleX = 0;
  work->discScaleY = 0;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhitePuff), work);
  entry = VramCache_FindFreeEntry();
  tile = VramCache_Load(entry, 0x800, work);
  particle = work->particles;
  for (i = 0; i < 64; i++) {
    fresh = &work->sprites[i];
    Func_080eaf98(fresh, 8, 15, 0x40000000, tile);
    fresh->depth = 250;
    fresh->oam.priority = 0;
    fresh->oam.palette256 = 1;
    fresh->oam.palette = 0;
    particle->timer = -1;
    particle++;
  }
  Sound_PlayFar(130);
  wait(1);
  next = 0;
  spawn = 0;
  phase = 0;
  timer = 0;
  frame = 0;
  do {
    for (i = spawn; i != 0; i--) {
      s32 slot = next & 63;
      sprite = &work->sprites[slot];
      sprite->scaleX = 0x3333;
      sprite->scaleY = 0x3333;
      particle = &work->particles[slot];
      particle->x = pos[0];
      particle->y = pos[1];
      particle->z = pos[2];
      Vector_AddPolarOffset(Random16() * 12 + 0x90000, Random16(),
                            &particle->x);
      particle->sway = (Random16() << 5) >> 16;
      particle->swayRate = ((Random16() << 9) >> 16) + 256;
      particle->rise = Random16() + 0x3333;
      particle->timer = 0;
      next = (next + 1) & 63;
    }
    particle = work->particles;
    cursor = work->sprites;
    for (i = 63; i >= 0; i--) {
      if (particle->timer >= 0) {
        s32 grown;
        /* FAKEMATCH(temporary): sprite holds a copy of cursor; as cursor used
         * directly, 57 of 871 instructions differ in tla-en. */
        sprite = cursor;
        sprite->oam.index = tile + ((particle->timer >> 2) & 7) * 8;
        grown = sprite->scaleX + 0x1999;
        // DEAD: stored again after the clamp; the ROM keeps both stores.
        sprite->scaleX = grown;
        if (grown > 0x10000)
          grown = 0x10000;
        sprite->scaleX = grown;
        sprite->scaleY = grown;
        screen[0] = particle->x;
        screen[1] = particle->y;
        screen[2] = particle->z;
        Camera_WorldToScreen(screen);
        sprite->x =
            screen[0] + (Trig_Sin(particle->sway * particle->swayRate) << 3);
        sprite->y = screen[2];
        Func_080eb01c(sprite);
        particle->sway++;
        particle->y += particle->rise;
        if (++particle->timer == 32)
          particle->timer = -1;
      }
      cursor++;
      particle++;
    }

    for (i = 0; i <= 31; i++) {
      if (fade == -1)
        add = 0;
      else
        add = fade / 4;
      blue = Trig_Sin((frame / 2 + i) << 11); // blue reused for green's sine
      green = (blue * 8 >> 16) + 23;
      blue = Trig_Sin(((frame / 4 + i) << 11) + 0x4800);
      blue = (blue * 10 >> 16) + 12;
      red = add + 28;
      if (red > 31)
        red = 31;
      green += add;
      if (green > 31)
        green = 31;
      blue += add;
      if (blue > 31)
        blue = 31;
      do { // once-only block: the listing schedules the store with it
        packed = (blue << 10) | (green << 5) | red;
        ((u16 *)0x05000200)[255 - i] = packed;
      } while (0);
    }
    if (fade != -1 && fade <= 126)
      fade++;

    x = work->discX;
    y = work->discY;
    scaleX = work->discScaleX;
    scaleY = work->discScaleY;
    packed = vramWork->glyphTiles + 128;
    sprite = &work->disc;
    Func_080eaf98(sprite, 31, 31, 0xc0000000, packed);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = Func_080db9a8(scene->mainObject, scene->facing);
    sprite->depth = 1;
    sprite->x = x;
    sprite->y = y;
    sprite->scaleX = scaleX;
    sprite->scaleY = scaleY;
    Func_080eb01c(sprite);

    if (result == 0) {
      switch (phase) {
      case 0:
        if (timer == 10)
          Sound_PlayFar(542);
        work->discScaleX += 0x300;
        work->discScaleY = work->discScaleX;
        if (work->discScaleX > 0x3fff) {
          phase = 1;
          timer = -1;
        }
        break;
      case 1:
        if (timer == 30) {
          phase = 2;
          timer = -1;
        }
        break;
      case 2:
        spawn = 0;
        work->discScaleX += -0x300;
        work->discScaleY = work->discScaleX;
        if (work->discScaleX <= 0)
          phase = 999;
        break;
      }
    }
    if (result == 1) {
      switch (phase) {
      case 0:
        if (timer == 30)
          Sound_PlayFar(140);
        work->discScaleX += 0x300;
        work->discScaleY = work->discScaleX;
        if (work->discScaleX > 0xbfff) {
          phase = 1;
          timer = -1;
        }
        break;
      case 1:
        spawn = 2;
        if (timer == 40) {
          phase = 2;
          timer = -1;
        }
        break;
      case 2:
        spawn = 1;
        if (timer == 40) {
          phase = 3;
          timer = -1;
        }
        break;
      case 3:
        spawn = 0;
        work->discScaleX += -0x400;
        work->discScaleY = work->discScaleX;
        if (work->discScaleX <= 0)
          phase = 999;
        break;
      }
    }
    if (result == 2) {
      switch (phase) {
      case 0:
        if (timer == 40)
          Sound_PlayFar(140);
        work->discScaleX += 0x300;
        work->discScaleY = work->discScaleX;
        if (work->discScaleX > 0xbfff) {
          phase = 1;
          timer = -1;
        }
        break;
      case 1:
        if (timer == 40) {
          fade = 0;
          BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
          PaletteFade_StartInterpolation(100);
        }
        spawn = 2;
        if (timer == 100) {
          phase = 2;
          timer = -1;
        }
        break;
      case 2:
        spawn = 1;
        if (timer == 80)
          phase = 999;
        break;
      }
    }
    wait(1);
    timer++;
    frame++;
  } while (phase != 999);

  if (found != -1) {
    struct Actor *marked = Actor_GetById(found);
    MapGrid_SetCellBehavior(marked->x, marked->z, marked->terrainId, 0);
    Actor_SetHorizontalPositionWithTerrain(found, 0, 0);
  }
  if (pending != NULL)
    Field_RunPsynergyEvent(pending, scene->firstObjectId,
                           scene->secondObjectId);
  VramCache_FreeEntry(entry);
  if (result == 2)
    BattleEffect_FadeToSavedPalettes(100);
  else
    FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(92);
}

void BattleEffect_PlotCanvasPixel(s32 x, s32 y, s32 color);

/// Plots one pixel of the 128x64 canvas on a circle around its bottom middle
/// (63, 63): a radius (16.16) and an angle, the cosine leftward and the sine
/// upward.
void Func_080e7f94(s32 unused, s32 radius, s32 angle, s32 color) {
  /* FAKEMATCH(pin): the angle pinned to r8, which leaves the multiply's entry
   * to r10; as angle itself, 12 of 39 instructions differ in tla-en. */
  register s32 turn asm("r8") = angle;
  s32 center = 63;
  s32 x = Iwram_MulQ16(Trig_Cos(turn), radius);
  s32 y = Iwram_MulQ16(Trig_Sin(turn), radius);

  BattleEffect_PlotCanvasPixel(center - (x >> 16), center - (y >> 16), color);
}

extern const u16 gData_080f39c4[];

/// Copy a 16x16 image to the BG map, clipping cells outside the 30x20-tile
/// screen.
void Func_080e7fec(s32 x, s32 y) {
  const u16 *tiles = gData_080f39c4;
  s32 row;
  s32 column;
  s32 sx;
  s32 sy;
  s32 (*fill)(void *, s32, u32);

  // The opening in a once-only block keeps the destination's load after the
  // fill value's 0; as plain statements, 4 of 66 instructions differ in
  // tla-en.
  do {
    x /= 8;
    y /= 8;
    fill = Iwram_FillWordsEntry;
    /* FAKEMATCH(barrier): an empty asm taking the fill routine's entry, so its
     * load comes before the size; without it, 4 of 66 instructions differ in
     * tla-en. */
    asm volatile("" : : "l"(fill));
    fill((void *)0x06002000, 0x400, 0);
  } while (0);
  for (row = 0, sy = y; row < 16; row++, sy++) {
    if (sy < 0)
      continue;
    if (sy > 19)
      break;
    for (sx = x, column = 0; column < 16; column++, sx++) {
      if (sx < 0)
        continue;
      if (sx > 29)
        break;
      ((u16 *)0x06002000)[row * 32 + column] = tiles[row * 16 + column];
    }
  }
}

#include "bg_scroll.h"

struct Unk_080e807cEntry {
  s32 x;
  s32 y;
  s32 z;
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 timer;
};

struct Unk_080e807cWork {
  u8 unk0000[0x2100];
  struct EffectSprite sprites[16];
  u8 unk2380[0x280];
  struct Unk_080e807cEntry entries[16];
  u8 unk27c0[0x1c2];
  u16 tile;
  s8 unk2984;
  s8 unk2985;
  s8 unk2986;
  u8 unk2987;
  u16 phase;
};

struct Unk_080e807cEvent {
  u8 unk000[0x166];
  u16 unk166;
  u8 unk168[0xb45];
  s8 unkcad;
  s16 count;
};

void Func_080e807c(void) {
  struct Unk_080e807cWork *work = Ram_HeapSlots->battleFxWork[0];
  struct Unk_080e807cEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 *pos = position;
  struct Unk_080e807cEntry *entry;
  struct EffectSprite *sprite;
  s32 x;
  s32 y;
  s32 i;
  s32 timer;
  u16 *phase;
  u16 *tile;
  s32 red;

  pos[0] = actor->x;
  pos[1] = actor->y + 0x100000;
  pos[2] = actor->z;
  if (work->unk2985 != 0)
    Vector_AddPolarOffset(0x20000, -(event->count << 11), pos);
  Camera_WorldToScreen(pos);
  x = pos[0] >> 16;
  gBgScroll[0].x = 64 - x;
  y = pos[2] >> 16;
  gBgScroll[0].y = 64 - y;
  Func_080e7fec(x - 64, y - 64);

  if (work->unk2986 != 0) {
    i = 0;
    phase = &work->phase;
    red = 28;
  cycle:
    {
      /* FAKEMATCH(pin): green pinned to r5, so the call's result is shifted
       * straight into it; as a plain local, 17 of 272 instructions differ in
       * tla-en. */
      register s32 green asm("r5") = Trig_Sin(((*phase >> 1) + i) << 11) * 8;
      s32 blue;
      s32 color;

      blue = Trig_Sin((((*phase >> 2) + i) << 11) + 0x4800) * 10;
      green = (green >> 16) + 23;
      blue = ((blue >> 16) + 12) << 10;
      green <<= 5;
      // An empty once-only loop keeps red's 28 after green's or; without it, 2
      // of 272 instructions differ in tla-en.
      do {
      } while (0);
      color = blue | green | red;

      ((u16 *)0x050001e0)[i] = color;
      ((u16 *)0x050003e0)[i] = color;
    }
    if (++i < 16)
      goto cycle;
    work->phase++;
  }

  if (work->unk2984 != 0) {
    entry = work->entries;
    sprite = work->sprites;
    tile = &work->tile;
    for (i = 15; i >= 0; i--, sprite++, entry++) {
      timer = entry->timer;
      if (timer == 0) {
        sprite->angle = Random16();
        pos[2] = timer;
        pos[1] = timer;
        pos[0] = timer;
        Vector_AddPolarOffset(0x360000, sprite->angle + 0x2000, pos);
        entry->x = pos[0];
        entry->y = pos[1] + 0x100000;
        entry->z = pos[2];
        timer = entry->timer;
      }
      if (timer >= 0) {
        sprite->oam.index = *tile + ((timer >> 2) & 3) * 4;
        pos[0] = actor->x + entry->x;
        pos[1] = actor->y + entry->y;
        pos[2] = actor->z + entry->z;
        EffectSprite_DrawAtWorldPosition__f0e6400(sprite, pos);
        timer = entry->timer;
      }
      if ((entry->timer = timer + 1) == 16)
        entry->timer = 0;
    }
  }

  if (event->unkcad != 0 && event->count != 0)
    event->count--;
  if (event->count == 0)
    event->unk166 = 0x209b;
}
#undef NEXT_PHASE
#undef WHIRL_COUNT
#undef SPARK_COUNT
