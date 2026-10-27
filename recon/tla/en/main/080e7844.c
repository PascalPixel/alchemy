// Draft, differ 730 of 871: a field Psynergy cast 32 pixels before the caster:
// a glowing disc (FieldEffect_GlowDisc, copied to the sprite VRAM the window
// work's halfword at 0x1348 names) grows there while up to 64 white puffs
// (FieldEffect_WhitePuff) rise and sway, and OBJ palette 15 shimmers in
// blue. Its result decides the script: 2 when an object of kind 290 whose
// animation byte 24 is 1 stands there (the object is marked) or when the
// ground there has a stage-5 event (Func_080dbd5c, Func_080dbdc8), 1 when
// the ground has none, 0 out of reach. Result 0 only grows and shrinks the
// disc; 1 grows it larger with puffs for 80 frames; 2 also fades the screen
// to white over 100 frames. A marked object is set on its terrain again, the
// event runs, and the palettes fade back.
// Same calls, offsets and control flow; left: register and stack allocation
// (the native frame is 100 bytes against 96, the caster in r7 and the work
// pointer at sp 72) and the order it cascades into.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "effect_sprite.h"
#include "dma.h"

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
};

struct FxEvent {
  u8 unk00[20];
  struct ObjectRuntime *objects[80];
};

struct ObjectAnimation {
  u8 unk00[24];
  u8 unk18;
  u8 unk19[15];
  s16 *definition;
};

extern char ResourceId_GlowDisc, ResourceId_WhitePuff;

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleEffect_InitializeSharedScene(void);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 Func_080cda84(s32 id, struct ObjectRuntime *object, s32 *position);
s32 Func_080dbd5c(s32 x, s32 z, s32 layer);
s32 Func_080dbdc8(s32 x, s32 z, s32 layer);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
struct ObjectRuntime *Object_GetById(s32 id);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
void Camera_WorldToScreen(s32 *position);
void Audio_PlayCue(s32 cue);
void WaitTicks(s32 ticks);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Func_080db9a8(struct ObjectRuntime *object, s32 facing);
void BattleEffect_ApplyColorToTargetBuffer(s32 color, s32 mode);
void BattleEffect_StartBufferInterpolation(s32 frames);
void Func_080dbdd4(s32 x, s32 z, s32 layer, s32 value);
void ObjectMotion_SetHorizontalPositionWithTerrain(s32 id, s32 a, s32 b);
void EventRuntime_RunPendingEvent(void *event, s32 a, s32 b);
void BattleEffect_FadeToSavedPalettes(s32 frames);
void BattleEffect_PrepareBufferInterpolation(void);

void Func_080e7844(void) {
  struct PuffWork *work = Runtime_AllocateHeapBlock(92, 0x2160);
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  u8 *vramWork = Ram_HeapSlots->windowTiles;
  struct ObjectRuntime *caster = scene->mainObject;
  struct ObjectRuntime *object;
  struct ObjectAnimation *animation;
  struct EffectSprite *sprite;
  struct PuffParticle *particle;
  void *pending;
  s32 pos[3];
  s32 screen[3];
  s32 *scale;
  s32 found;
  s32 fade;
  s32 result;
  s32 entry;
  s32 tile;
  s32 next;
  s32 spawn;
  s32 phase;
  s32 timer;
  s32 frame;
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  s32 red;
  s32 green;
  s32 blue;
  s32 add;
  s32 half;
  s32 i;
  s32 n;
  u8 *dst;

  found = -1;
  fade = -1;
  pending = NULL;
  BattleEffect_InitializeSharedScene();
  pos[0] = caster->x;
  pos[1] = caster->y;
  pos[2] = caster->z;
  Vector_AddPolarOffset(0x200000, scene->facing, pos);
  for (i = 0; i < 80; i++) {
    object = ((struct FxEvent *)Ram_HeapSlots->eventWork)->objects[i];
    animation = object->animation;
    if (object->script != NULL && animation->definition[0] == 290 &&
        animation->unk18 == 1 &&
        Func_080dbe80(pos, 4, &object->x, object->radius - 2) >= 0) {
      result = 2;
      found = i;
      ((u8 *)Object_GetById(i))[35] |= 2;
      goto loaded;
    }
  }
  if (Func_080cda84(scene->fieldKind, caster, pos) == -1) {
    result = 0;
    pending = NULL;
  } else if (Func_080dbd5c(pos[0], pos[2], caster->terrainId) != 0) {
    pending = Func_080ce458(0x50000005, scene->fieldKind,
                            Func_080dbdc8(pos[0], pos[2], caster->terrainId));
    result = 2;
    if (pending == NULL)
      result = 1;
  } else {
    result = 0;
  }
loaded:
  dst = (u8 *)0x06011000 + *(u16 *)(vramWork + 0x1348) * 32;
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GlowDisc),
                        work);
  Dma_Set(work, dst, (DMA_ENABLE | DMA_32BIT) << 16 | 0x400, REG_DMA3);
  screen[0] = pos[0];
  screen[1] = pos[1];
  screen[2] = pos[2];
  Camera_WorldToScreen(screen);
  work->discX = screen[0];
  work->discY = screen[2];
  work->discScaleX = 0;
  work->discScaleY = 0;
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_WhitePuff),
                        work);
  entry = Resource_FindFreeEntry();
  tile = VramCache_Load(entry, 0x800, work);
  particle = work->particles;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--) {
    Func_080eaf98(sprite, 8, 15, 0x40000000, tile);
    sprite->depth = 250;
    sprite->oam.palette256 = 1;
    sprite->oam.priority = 0;
    sprite->oam.palette = 0;
    particle->timer = -1;
    sprite++;
    particle++;
  }
  Audio_PlayCue(130);
  WaitTicks(1);
  scale = &work->discScaleX;
  next = 0;
  spawn = 0;
  phase = 0;
  timer = 0;
  frame = 0;
  do {
    for (n = spawn; n != 0; n--) {
      i = next & 63;
      sprite = &work->sprites[i];
      sprite->scaleX = 0x3333;
      sprite->scaleY = 0x3333;
      particle = &work->particles[i];
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
    sprite = work->sprites;
    for (i = 63; i >= 0; i--) {
      if (particle->timer >= 0) {
        sprite->oam.index = tile + ((particle->timer >> 2) & 7) * 8;
        sprite->scaleX += 0x1999;
        if (sprite->scaleX > 0x10000)
          sprite->scaleX = 0x10000;
        sprite->scaleY = sprite->scaleX;
        screen[0] = particle->x;
        screen[1] = particle->y;
        screen[2] = particle->z;
        Camera_WorldToScreen(screen);
        sprite->x =
            screen[0] + (Trig_Sin(particle->swayRate * particle->sway) << 3);
        sprite->y = screen[2];
        Func_080eb01c(sprite);
        particle->sway++;
        particle->y += particle->rise;
        if (++particle->timer == 32)
          particle->timer = -1;
      }
      sprite++;
      particle++;
    }

    half = frame / 2;
    for (i = 0; i <= 31; i++) {
      add = 0;
      if (fade != -1)
        add = fade / 4;
      green = (Trig_Sin((half + i) << 11) * 8 >> 16) + 23;
      blue = (Trig_Sin(((frame / 4 + i) << 11) + 0x4800) * 10 >> 16) + 12;
      red = add + 28;
      if (red > 31)
        red = 31;
      green += add;
      if (green > 31)
        green = 31;
      blue += add;
      if (blue > 31)
        blue = 31;
      ((volatile u16 *)0x05000200)[255 - i] = (blue << 10) | (green << 5) | red;
    }
    if (fade != -1 && fade <= 126)
      fade++;

    x = work->discX;
    y = work->discY;
    scaleX = work->discScaleX;
    scaleY = work->discScaleY;
    sprite = &work->disc;
    Func_080eaf98(sprite, 31, 31, 0xc0000000,
                  *(u16 *)(vramWork + 0x1348) + 128);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = Func_080db9a8(scene->mainObject, scene->facing);
    sprite->x = x;
    sprite->depth = 1;
    sprite->y = y;
    sprite->scaleX = scaleX;
    sprite->scaleY = scaleY;
    Func_080eb01c(sprite);

    if (result == 0) {
      switch (phase) {
      case 0:
        if (timer == 10)
          Audio_PlayCue(542);
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
          Audio_PlayCue(140);
        *scale += 0x300;
        work->discScaleY = *scale;
        if (*scale > 0xbfff) {
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
        *scale += -0x400;
        work->discScaleY = *scale;
        if (*scale <= 0)
          phase = 999;
        break;
      }
    }
    if (result == 2) {
      switch (phase) {
      case 0:
        if (timer == 40)
          Audio_PlayCue(140);
        *scale += 0x300;
        work->discScaleY = *scale;
        if (*scale > 0xbfff) {
          phase = 1;
          timer = -1;
        }
        break;
      case 1:
        if (timer == 40) {
          fade = 0;
          BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
          BattleEffect_StartBufferInterpolation(100);
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
    WaitTicks(1);
    timer++;
    frame++;
  } while (phase != 999);

  if (found != -1) {
    object = Object_GetById(found);
    Func_080dbdd4(object->x, object->z, object->terrainId, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain(found, 0, 0);
  }
  if (pending != NULL)
    EventRuntime_RunPendingEvent(pending, scene->firstObjectId,
                                 scene->secondObjectId);
  Resource_ResetEntry(entry);
  if (result == 2)
    BattleEffect_FadeToSavedPalettes(100);
  else
    BattleEffect_PrepareBufferInterpolation();
  Runtime_ReleaseHeapBlock(92);
}
