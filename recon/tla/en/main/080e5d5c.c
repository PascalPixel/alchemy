// Draft, differ 452 of 639: a field Psynergy that raises a glowing object
// (object 352) before the caster or at the target: the scene sits 32 pixels
// before the caster, or on the target's ground when it is airborne. With a
// target in reach and no solid object at the spot (Func_080cda84,
// Func_080dbe80), the shine plays: the glow disc on the canvas, a glow
// sprite and 32 gold stars (FieldEffect_GoldStar) bursting from a ring, the
// object rising 24 frames, then Func_080e5a64 runs as a frame callback, the
// effect's stage-5 event running each frame and the glow drawn unless the
// event mode is 3 or the event work's flags at 0xd4c say otherwise, until it
// parks at 999; then the object bursts (Func_080dbf94). Otherwise the
// object sinks and shrinks (BattleEffect_SinkAndShrinkObject) and scatters
// sixteen particles. Cue 114 ends both.
// Same calls, offsets and control flow; left: register allocation (the
// native keeps the scene in r9 and the reach flag at sp 8, and its frame is
// 44 bytes against 48) and the order it cascades into.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "effect_sprite.h"
#include "canvas_sprite.h"
#include "iwram_call.h"

struct ShineParticle {
  s32 x;
  s32 y;
  s32 unk08;
  s32 velocityX;
  s32 velocityY;
  s32 unk14;
  s32 timer;
};

struct ShineWork {
  u8 tiles[0x1000];
  struct ShineParticle particles[32];
  struct EffectSprite sprites[32];
  struct EffectSprite glow;
  struct ObjectRuntime *object;
  s16 state;
  s16 timer;
  s8 sparkles;
  s8 fading;
};

struct FxScene {
  u16 facing;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct ObjectRuntime *mainObject;
  struct ObjectRuntime *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
  u8 unk21[0x7a3];
  s16 fading;
  s16 frame;
};

struct FxEvent {
  u8 unk00[20];
  struct ObjectRuntime *objects[80];
};

extern char ResourceId_GlowDisc, ResourceId_GoldStar;

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Func_080cda84(s32 id, struct ObjectRuntime *object, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void BattleEffect_InitializeSharedScene(void);
void Sound_PlayFar(s32 cue);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Object_SetMode(struct ObjectRuntime *object, s32 mode);
void Object_Destroy(struct ObjectRuntime *object);
void Func_080dba5c(void);
s32 BattleEffect_GetObjectPriority(struct ObjectRuntime *object);
void Camera_WorldToScreen(s32 *position);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void WaitFrames(s32 frames);
void Func_080dbed0(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_080dbf94(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
void BattleEffect_SinkAndShrinkObject(void);
struct ObjectRuntime *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
s32 Func_080ce31c(s32 kind);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void EventRuntime_RunPendingEvent(void *event, s32 a, s32 b);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e5a64(void);

void Func_080e5d5c(void) {
  struct ShineWork *work = Runtime_AllocateHeapBlock(92, 0x18b4);
  struct FxEvent *event = Ram_HeapSlots->eventWork;
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  struct ObjectRuntime *child = scene->child;
  struct ObjectRuntime *caster = scene->mainObject;
  struct ObjectRuntime *object;
  struct ObjectRuntime *shine;
  struct CanvasSpriteWork *canvas;
  struct EffectSprite *sprite;
  struct ShineParticle *particle;
  u8 *animation;
  void *pending;
  s32 pos[3];
  s32 lift;
  s32 inReach;
  s32 glowEntry;
  s32 starEntry;
  s32 starTile;
  s32 speed;
  s32 angle;
  s32 frame;
  s32 i;

  lift = 0x200000;
  if (child == NULL) {
    scene->x = caster->x;
    scene->y = caster->y;
    scene->z = caster->z;
    Vector_AddPolarOffset(lift, scene->facing, &scene->x);
  } else if (child->motionFlags & 4) {
    scene->y = child->terrainHeight;
    lift = child->y - child->terrainHeight + 0x200000;
  }
  if (scene->enabled != 0) {
    inReach = Func_080cda84(scene->fieldKind, caster, &scene->x) != -1;
    if (inReach == 1) {
      for (i = 0; i < 80; i++) {
        object = event->objects[i];
        if (object != NULL && object->script != NULL &&
            (object->collisionFlags & 1) &&
            Func_080dbe80(&scene->x, 4, &object->x, object->radius - 2) >= 0) {
          inReach = 0;
          break;
        }
      }
    }
  } else {
    inReach = 1;
    if (child != NULL && ((u8 *)child)[99] != 0)
      inReach = 0;
  }

  BattleEffect_InitializeSharedScene();
  Sound_PlayFar(138);
  shine = Object_Spawn(352, 0, 0, 0);
  work->object = shine;
  if (shine == NULL)
    goto end;
  shine->motionFlags = 0;
  if (*(u16 *)((u8 *)event + 0xd4c) & 2) {
    if ((shine->animationKind & 15) == 1) {
      animation = shine->animation;
      animation[9] = (animation[9] & ~12) | 4;
      ((u8 *)shine)[35] &= 254;
    }
  } else {
    ((u8 *)shine)[35] = 1;
  }
  shine->angle = caster->angle;
  Object_SetMode(shine, 2);
  if (!(*(u16 *)((u8 *)event + 0xd4c) & 1))
    Func_080dba5c();

  Func_080eb824();
  canvas = Ram_HeapSlots->unkec[1];
  canvas->priority = BattleEffect_GetObjectPriority(scene->mainObject);
  canvas->depth = 1;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->scaleX = 0;
  canvas->scaleY = 0;
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GlowDisc),
                        Ram_HeapSlots->battleFxWork[1]);
  canvas->command = CANVAS_UPLOAD;

  Iwram_FillWords(work, 0x800, 0);
  glowEntry = Resource_FindFreeEntry();
  sprite = &work->glow;
  Func_080eaf98(sprite, 32, 32, 0xc0000000,
                VramCache_Load(glowEntry, 0x800, work));
  sprite->oam.priority = 0;
  sprite->oam.palette256 = 0;
  sprite->oam.palette = 0;
  sprite->depth = 0;
  sprite->scaleX = sprite->scaleY = 0xcccc;
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  sprite->x = pos[0];
  sprite->y = pos[2];

  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GoldStar),
                        work);
  starEntry = Resource_FindFreeEntry();
  starTile = VramCache_Load(starEntry, 0x100, work);
  particle = work->particles;
  sprite = work->sprites;
  for (i = 0; i < 32; i++) {
    speed = Random16() * 18 / 10 + 0x28000;
    angle = i * 0x1555 + (Random16() >> 5);
    pos[0] = scene->x;
    pos[1] = scene->y;
    pos[2] = scene->z;
    Vector_AddPolarOffset(0x40000, angle, pos);
    Camera_WorldToScreen(pos);
    particle->x = pos[0];
    particle->y = pos[2];
    particle->velocityX = Iwram_MulQ16(Trig_Cos(angle), speed);
    particle->velocityY = Iwram_MulQ16(Trig_Sin(angle), speed);
    particle->timer = -i;
    Func_080eaf98(sprite, 8, 8, 0x40000000, starTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    sprite->scaleX = sprite->scaleY = 0xcccc;
    particle++;
    sprite++;
  }
  WaitFrames(1);
  Func_080dbed0(shine, scene->x, scene->y + lift, scene->z);
  Object_SetMode(shine, 1);
  WaitFrames(15);
  for (i = 24; i >= 0; i--) {
    shine->y += 0x8000;
    WaitFrames(1);
  }
  WaitFrames(15);
  work->state = 0;
  work->timer = 0;
  work->sparkles = 0;
  if (!inReach) {
    Scheduler_AddOrUpdateCallback(BattleEffect_SinkAndShrinkObject, 0x480);
    while (work->state != 999)
      WaitFrames(1);
    Scheduler_RemoveCallback(BattleEffect_SinkAndShrinkObject);
    for (i = 15; i >= 0; i--)
      BattleEffect_SpawnScatterParticle(shine->x, shine->y, shine->z);
    Sound_PlayFar(114);
    WaitFrames(1);
  } else {
    pending = Func_080ce458(0x50000005, scene->fieldKind,
                            Func_080ce31c(scene->fieldKind));
    Scheduler_AddOrUpdateCallback(Func_080e5a64, 0x480);
    frame = 0;
    while (work->state != 999) {
      WaitFrames(1);
      scene->fading = work->fading;
      scene->frame = frame;
      if (((s8 *)event)[394] != 3 && !(*(u16 *)((u8 *)event + 0xd4c) & 1))
        Func_080eb01c(&work->glow);
      EventRuntime_RunPendingEvent(pending, work->fading, frame);
      frame++;
    }
    Scheduler_RemoveCallback(Func_080e5a64);
    WaitFrames(10);
    Sound_PlayFar(114);
    Func_080dbf94(shine, shine->x, shine->y, shine->z);
  }
  Object_Destroy(shine);
  Resource_ResetEntry(starEntry);
  Resource_ResetEntry(glowEntry);
  BattleEffect_EndCanvasSprite();
end:
  BattleEffect_PrepareBufferInterpolation();
  Runtime_ReleaseHeapBlock(92);
}
