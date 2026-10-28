// Draft, differ 416 of 938: a field Psynergy that grows something from the
// ground before the caster (on a 16-pixel grid, unless the event mode is 3):
// object 353 rises, and a second object (354, or 355 when the scene's flag
// at 0x41 is set) waits there. It fails, scattering sixteen particles with
// cue 114, when an object stands on the spot, the ground is impassable
// (Func_080dbdc8 255), out of reach (Func_080cda84) or not ground
// (Func_080dbc04). Otherwise a mode is chosen: with a target, 1 in 16 casts
// mode 1, else mode 0, or 3 with the flag; without one, mode 2. Modes 0 and
// 2 set up 64 dust particles (FieldEffect_DustPuffA, or DustPuffB), mode 3
// 64 white sparkles, mode 0 also a picture (resource 0x1e9) over the object;
// then Func_080e6400 runs as a frame callback until it parks at 999, mode 1
// calling Func_080e716c at frame 24 and the others running the effect's
// stage-5 event each frame (stage 1 from frame 30, 2 from 50). The object
// then bursts (Func_080dbf94).
// Same calls, offsets and control flow; left: register allocation (the
// native keeps the scene in r10, the work in r9 and the first object in r11)
// and the order it cascades into.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"

struct RiseParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct RiseWork {
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

struct FxScene {
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

struct FxEvent {
  u8 unk00[20];
  struct Actor *objects[80];
};

extern char ResourceId_DustPuffA, ResourceId_DustPuffB, ResourceId_WhiteSparkle,
    ResourceId_Data1E9;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void BattleEffect_InitializeSharedScene(void);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetMode(struct Actor *object, s32 mode);
void Actor_Destroy(struct Actor *object);
void Animation_ApplyChildValuesFar(struct Actor *object, s32 value);
void Func_080dba5c(void);
void Sound_PlayFar(s32 cue);
void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_080dbf94(struct Actor *object, s32 x, s32 y, s32 z);
void wait(s32 frames);
s32 Func_08020330(struct Actor *object);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 Func_080dbdc8(s32 x, s32 z, s32 layer);
s32 Func_080cda84(s32 id, struct Actor *object, s32 *position);
s32 Func_080dbc04(s32 x, s32 z, s32 layer);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
u32 Random16(void);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Camera_WorldToScreen(s32 *position);
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
s32 Func_080ce31c(s32 kind);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void Field_RunPsynergyEvent(void *event, s32 a, s32 b);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e6400(void);
void Func_080e716c(void);

void Func_080e68d0(void) {
  struct RiseWork *work = HeapSlot_Alloc(92, 0x193c);
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *caster = scene->mainObject;
  struct FxEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *other = NULL;
  struct Actor *object;
  struct Actor *target;
  struct EffectSprite *sprite;
  struct RiseParticle *particle;
  s8 *alternate;
  u8 *animation;
  void *pending;
  s32 particleEntry = 96;
  s32 pictureEntry = 96;
  s32 particleTile;
  s32 pictureTile;
  s32 pos[3];
  s32 failed;
  s32 stage;
  s32 i;

  if (((s8 *)event)[394] != 3) {
    scene->x = (caster->x & 0xfff00000) + 0x80000;
    scene->y = caster->y;
    scene->z = (caster->z & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x100000, scene->facing, &scene->x);
  }
  BattleEffect_InitializeSharedScene();
  object = Actor_Spawn(353, 0, 0, 0);
  work->object = object;
  if (object == NULL)
    goto end;
  object->motionFlags = 0;
  if (*(u16 *)((u8 *)event + 0xd4c) & 2) {
    if ((object->animationKind & 15) == 1) {
      animation = object->animation;
      animation[9] = (animation[9] & ~12) | 4;
      ((u8 *)object)[35] &= 254;
    }
  } else {
    ((u8 *)object)[35] = 1;
  }
  object->angle = caster->angle;
  Actor_SetMode(object, 0);
  alternate = &scene->alternate;
  if (*alternate != 0)
    Animation_ApplyChildValuesFar(object, 6);
  if (*alternate == 0)
    other = Actor_Spawn(354, scene->x, scene->y, scene->z);
  else
    other = Actor_Spawn(355, scene->x, scene->y, scene->z);
  work->other = other;
  if (other == NULL)
    goto end;
  other->z += 0x30000;
  other->motionFlags = 0;
  ((u8 *)other)[35] = 1;
  other->scaleX = 0;
  if (!(*(u16 *)((u8 *)event + 0xd4c) & 1))
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
    target = event->objects[i];
    if (target != NULL && target->script != NULL &&
        (Func_08020330(target) == 406 || (target->collisionFlags & 1)) &&
        target != scene->mainObject && ((u8 *)target->animation)[27] != 0 &&
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
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                          work);
    particleEntry = Resource_FindFreeEntry();
    particleTile = VramCache_Load(particleEntry, 0x800, work);
    work->particleTile = particleTile;
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 64; i++) {
      Func_080eaf98(sprite, 8, 8, 0x40000000, particleTile);
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(other);
      sprite->depth = BattleEffect_GetObjectDepth(other) + 1;
      sprite->scaleX = sprite->scaleY = 0x3333;
      particle->x = scene->x;
      particle->y = scene->y;
      particle->z = scene->z;
      Vector_AddPolarOffset(0x20000, Random16(), &particle->x);
      particle->velocityX = 0;
      particle->velocityY = 0;
      particle->velocityZ = 0;
      Vector_AddPolarOffset(0x13333, Random16(), &particle->velocityX);
      particle->timer = -i * 2;
      sprite++;
      particle++;
    }
  } else if (work->mode == 2) {
    if (*alternate == 0)
      Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                            work);
    else
      Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffB),
                            work);
    particleEntry = Resource_FindFreeEntry();
    particleTile = VramCache_Load(particleEntry, 0x800, work);
    work->particleTile = particleTile;
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 64; i++) {
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
      sprite++;
      particle++;
    }
  } else if (work->mode == 3) {
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_WhiteSparkle),
                          work);
    particleEntry = Resource_FindFreeEntry();
    particleTile = VramCache_Load(particleEntry, 0x100, work);
    work->particleTile = particleTile;
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 64; i++) {
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
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Data1E9),
                          work);
    pictureEntry = Resource_FindFreeEntry();
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
    if (work->mode == 0)
      goto grow;
  }
  if (work->mode == 3) {
  grow:
    wait(6);
    work->other->scaleX = 0x10000;
    wait(4);
  } else {
    wait(10);
  }

  work->phase = 0;
  work->timer = 0;
  Task_Add(Func_080e6400, 0x480);
  if (work->mode == 1) {
    i = 0;
    while (work->phase != 999) {
      if (i == 24)
        Func_080e716c();
      wait(1);
      i++;
    }
  } else {
    pending = Func_080ce458(0x50000005, scene->fieldKind,
                            Func_080ce31c(scene->fieldKind));
    i = 0;
    while (work->phase != 999) {
      stage = 0;
      wait(1);
      if (i == 30)
        stage = 1;
      if (i == 50)
        stage = 2;
      scene->stage = stage;
      scene->frame = i;
      Field_RunPsynergyEvent(pending, stage, i);
      i++;
    }
    Field_RunPsynergyEvent(pending, 999, i);
  }
  Task_Remove(Func_080e6400);
  Sound_PlayFar(114);
  Func_080dbf94(object, object->x, object->y, object->z);
  if (particleEntry != 96)
    Resource_ResetEntry(particleEntry);
  if (pictureEntry != 96)
    Resource_ResetEntry(pictureEntry);
end:
  Actor_Destroy(object);
  Actor_Destroy(other);
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(92);
}
