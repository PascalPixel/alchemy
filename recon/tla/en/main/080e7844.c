// Matches: differ 0 (tla-en, 2026-11-08)
// Plain C, no FAKEMATCH device. Per edition, each at its own address in the
// edition's map: tla-en, tla-de (0x080f4844), tla-es (0x080f0838), tla-fr
// (0x080ec838) and tla-it (0x080e7838) differ 0; tla-ja (0x080e7800) differs
// 10 of 869, all from the window work's tile halfword, which Japanese keeps at
// 0x1148 where every other edition keeps it at 0x1348 (both reads; the shorter
// constant also moves two pool offsets and a branch). With that offset chosen
// by edition the draft measures 0 in tla-ja too; the adopted Func_080eb824
// reads the same halfword and stays off Japanese, and nothing here yet says
// whether the move is LANGUAGE or TLA_PATCH_1, so the offset is left English.
// What closed it (8 November, from 28): the heap-slot reads at the top go
// through Ram_HeapSlots and the loop reads slots->eventWork each pass through
// a slots local set just before the loop, so CSE copies the top's constant
// into it and gives it a preference for r5; the kind (290) is a local set
// after i, so it is allocated after slots, offset and caster and lands in r8;
// the caster is read before found, fade and pending, which schedules its load
// and offset = 20 ahead of their stores. Green's sine goes through blue (the
// product leaves r5 for r3). The sprite loop counts i up and takes its sprite
// as &work->sprites[i]: loop.c then reverses it in its second pass, after it
// has hoisted the -13 mask, which orders that copy before i = 63. The
// once-only block round the palette store is still needed (4 without).
// Earlier: differ 28 (2026-11-08), 44 (2026-11-05), 75 (2026-11-01).
// A field Psynergy cast 32 pixels before the caster:
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
// Forms kept from earlier rounds: the spawn count shares i with the other loops
// (i only takes caller-saved r4 that way), the slot is its own local, and the
// disc's tile goes through packed; a block-scoped sprite, a separate counter
// and a shared wave local all measured worse.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
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
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
};

struct ObjectAnimation {
  u8 unk00[24];
  u8 unk18;
  u8 unk19[15];
  s16 *definition;
};

extern char ResourceId_GlowDisc, ResourceId_WhitePuff;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void BattleEffect_InitializeSharedScene(void);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 Func_080cda84(s32 id, struct Actor *object, s32 *position);
s32 Func_080dbd5c(s32 x, s32 z, s32 layer);
s32 Func_080dbdc8(s32 x, s32 z, s32 layer);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
struct Actor *Actor_GetById(s32 id);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
void Camera_WorldToScreen(s32 *position);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Func_080db9a8(struct Actor *object, s32 facing);
void BattleEffect_ApplyColorToTargetBuffer(s32 packed, s32 mode);
void BattleEffect_StartBufferInterpolation(s32 frames);
void Func_080dbdd4(s32 x, s32 z, s32 layer, s32 value);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 a, s32 b);
void Field_RunPsynergyEvent(void *event, s32 a, s32 b);
void BattleEffect_FadeToSavedPalettes(s32 frames);
void BattleEffect_PrepareBufferInterpolation(void);

void Func_080e7844(void) {
  struct PuffWork *work = HeapSlot_Alloc(92, 0x2160);
  struct HeapSlots *slots;
  struct FxScene *scene = Ram_HeapSlots->effectWork;
  u8 *vramWork = Ram_HeapSlots->windowTiles;
  s32 offset; // byte offset of the event work's 80 object pointers
  struct Actor *caster;
  struct Actor *object;
  struct ObjectAnimation *animation;
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
  BattleEffect_InitializeSharedScene();
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
    animation = object->animation;
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
  {
    void *source = Resource_GetTableEntry((s32)&ResourceId_GlowDisc);
    dst = (u8 *)0x06011000 + *(u16 *)(vramWork + 0x1348) * 32;
    Resource_DecodeType01(source, work);
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
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_WhitePuff),
                        work);
  entry = Resource_FindFreeEntry();
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
        sprite = cursor;
        sprite->oam.index = tile + ((particle->timer >> 2) & 7) * 8;
        grown = sprite->scaleX + 0x1999;
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
    packed = *(u16 *)(vramWork + 0x1348) + 128;
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
    wait(1);
    timer++;
    frame++;
  } while (phase != 999);

  if (found != -1) {
    struct Actor *marked = Actor_GetById(found);
    Func_080dbdd4(marked->x, marked->z, marked->terrainId, 0);
    Actor_SetHorizontalPositionWithTerrain(found, 0, 0);
  }
  if (pending != NULL)
    Field_RunPsynergyEvent(pending, scene->firstObjectId,
                           scene->secondObjectId);
  Resource_ResetEntry(entry);
  if (result == 2)
    BattleEffect_FadeToSavedPalettes(100);
  else
    BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(92);
}
