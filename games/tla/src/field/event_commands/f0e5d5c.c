#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
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
  struct Actor *object;
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
  struct Actor *mainObject;
  struct Actor *child;
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
  struct Actor *objects[80];
};

extern char ResourceId_GlowDisc, ResourceId_GoldStar;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Func_080cda84(s32 id, struct Actor *object, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void BattleEffect_InitializeSharedScene(void);
void Sound_PlayFar(s32 cue);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetMode(struct Actor *object, s32 mode);
void Actor_Destroy(struct Actor *object);
void Func_080dba5c(void);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Camera_WorldToScreen(s32 *position);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void wait(s32 frames);
void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_080dbf94(struct Actor *object, s32 x, s32 y, s32 z);
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void BattleEffect_SinkAndShrinkObject(void);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
s32 Func_080ce31c(s32 kind);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void Field_RunPsynergyEvent(void *event, s32 a, s32 b);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e5a64(void);

void Func_080e5d5c(void) {
  struct ShineWork *work = HeapSlot_Alloc(92, 0x18b4);
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  struct FxEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *child = scene->child;
  struct Actor *caster = scene->mainObject;
  struct Actor *object;
  struct Actor *shine;
  struct CanvasSpriteWork *canvas;
  struct EffectSprite *sprite;
  struct EffectSprite *glow;
  void *pending;
  s32 pos[3];
  s32 starEntry;
  s32 starTile;
  s32 glowEntry;
  s32 inReach;
  s32 lift;
  s32 glowTile;
  s32 spot;
  s32 size = 0x800;
  s32 rise;
  s32 speed;
  s32 angle;
  s32 frame;
  s32 j;
  struct ShineParticle *particle;

  lift = 0x200000;
  if (child == NULL) {
    scene->x = caster->x;
    scene->y = caster->y;
    scene->z = caster->z;
    Vector_AddPolarOffset(lift, scene->facing, &scene->x);
  } else if (child->motionFlags & 4) {
    /* FAKEMATCH(dead): a store the line after next overwrites; its test
     * builds 300 in a reload register, the one pick before scene's reload
     * for the y store, which sends scene to r4 and the 0x200000 to r0; as
     * the function without it, 12 of 639 instructions differ in tla-en. */
    if (child->terrainHeight == 300)
      lift = 0;
    scene->y = child->terrainHeight;
    lift = child->y - child->terrainHeight + 0x200000;
  }
  if (scene->enabled != 0) {
    // As x != -1 this splits the -1 and loses the listing's mvns.
    spot = ~Func_080cda84(scene->fieldKind, caster, &scene->x);
    inReach = (u32)(-spot | spot) >> 31;
    if (inReach == 1) {
      for (j = 0; j < 80; j++) {
        object = event->objects[j];
        if (object != NULL && object->script != NULL &&
            (object->collisionFlags & 1) &&
            Func_080dbe80(&scene->x, 4, &object->x, object->radius - 2) >= 0)
          goto outOfReach;
      }
    }
  } else {
    inReach = 1;
    if (child != NULL && ((u8 *)child)[99] != 0) {
    outOfReach:
      inReach = 0;
    }
  }

  BattleEffect_InitializeSharedScene();
  Sound_PlayFar(138);
  shine = Actor_Spawn(352, 0, 0, 0);
  work->object = shine;
  if (shine == NULL)
    goto end;
  shine->motionFlags = 0;
  if (*(u16 *)((u8 *)event + 0xd4c) & 2) {
    if ((shine->animationKind & 15) == 1) {
      ((struct EffectSprite *)shine->animation)->oam.priority = 1;
      ((u8 *)shine)[35] &= 254;
    }
  } else {
    ((u8 *)shine)[35] = 1;
  }
  shine->angle = caster->angle;
  Actor_SetMode(shine, 2);
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

  // A once-only block: without it the canvas mode load is scheduled one row
  // later (6 differ).
  do {
    Iwram_FillWords(work, size, 0);
  } while (0);
  glowEntry = Resource_FindFreeEntry();
  glowTile = VramCache_Load(glowEntry, size, work);
  glow = &work->glow;
  Func_080eaf98(glow, 32, 32, 0xc0000000, glowTile);
  glow->oam.priority = 0;
  glow->oam.palette256 = 0;
  glow->oam.palette = 0;
  glow->depth = 0;
  glow->scaleY = glow->scaleX = 0xcccc;
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  glow->x = pos[0];
  glow->y = pos[2];

  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GoldStar),
                        work);
  starEntry = Resource_FindFreeEntry();
  starTile = VramCache_Load(starEntry, 0x100, work);
  for (j = 0, particle = work->particles; j < 32; j++, particle++) {
    sprite = &work->sprites[j];
    speed = Random16() * 18 / 10;
    speed += 0x28000;
    angle = j * 0x1555 + (Random16() >> 5);
    pos[0] = scene->x;
    pos[1] = scene->y;
    pos[2] = scene->z;
    Vector_AddPolarOffset(0x40000, angle, pos);
    Camera_WorldToScreen(pos);
    particle->x = pos[0];
    particle->y = pos[2];
    particle->velocityX = Iwram_MulQ16(Trig_Cos(angle), speed);
    particle->velocityY = Iwram_MulQ16(Trig_Sin(angle), speed);
    particle->timer = -j;
    Func_080eaf98(sprite, 8, 8, 0x40000000, starTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    sprite->scaleY = sprite->scaleX = 0xcccc;
  }
  wait(1);
  Func_080dbed0(shine, scene->x, scene->y + lift, scene->z);
  Actor_SetMode(shine, 1);
  wait(15);
  rise = 24;
  do {
    shine->y += 0x8000;
    rise--;
    wait(1);
  } while (rise >= 0);
  wait(15);
  work->state = 0;
  work->timer = 0;
  work->sparkles = 0;
  if (!inReach) {
    Task_Add(BattleEffect_SinkAndShrinkObject, 0x480);
    while (work->state != 999)
      wait(1);
    Task_Remove(BattleEffect_SinkAndShrinkObject);
    for (j = 15; j >= 0; j--)
      BattleEffect_SpawnScatterParticle(shine->x, shine->y, shine->z);
    Sound_PlayFar(114);
    wait(1);
  } else {
    pending = Func_080ce458(0x50000005, scene->fieldKind,
                            Func_080ce31c(scene->fieldKind));
    Task_Add(Func_080e5a64, 0x480);
    frame = 0;
    while (work->state != 999) {
      wait(1);
      scene->fading = work->fading;
      scene->frame = frame;
      if (((s8 *)event)[394] != 3 && !(*(u16 *)((u8 *)event + 0xd4c) & 1)) {
        glow = &work->glow;
        Func_080eb01c(glow);
      }
      Field_RunPsynergyEvent(pending, work->fading, frame++);
    }
    Task_Remove(Func_080e5a64);
    wait(10);
    Sound_PlayFar(114);
    Func_080dbf94(shine, shine->x, shine->y, shine->z);
  }
  Actor_Destroy(shine);
  Resource_ResetEntry(starEntry);
  Resource_ResetEntry(glowEntry);
  BattleEffect_EndCanvasSprite();
end:
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(92);
}
