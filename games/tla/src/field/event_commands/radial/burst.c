/// @file
/// Two routines not yet understood.
// Part of radial.c.

#include "../radial_internal.h"
#include "fixed_math.h"
#include "sounds.h"

/* VIEW(f0e572c): tag struct FxScene as struct FxScene__f0e572c; f0e2c38.c has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja: games/tla/src/field/event_commands/radial.c:4366:
 * structure has no member named `facing'. */
struct FxScene__f0e572c {
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

extern const u8 Data_080f3984[];
extern char ResourceId_FieldEffectExplosion, ResourceId_GlowDisc;

void Camera_WorldToScreen(s32 *position);
s32 FieldPsynergy_GetCanvasPriority(struct Actor *object, s32 facing);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
void FieldPsynergy_UpdateBurst(void);

void FieldPsynergy_Burst(void) {
  struct GlowWork *work = HeapSlot_Alloc(92, sizeof(struct GlowWork));
  struct FxScene__f0e572c *scene = Ram_HeapSlots->effectWork;
  struct CanvasSpriteWork *canvas;
  struct EffectSprite *sprite;
  struct GlowParticle *particle;
  void *pending;
  s32 pos[3];
  s32 sparkEntry;
  s32 sparkTile;
  s32 burstEntry;
  s32 tile;
  s32 radius;
  s32 angle;
  s32 kind;
  s32 frame;
  s32 (*mul)(s32, s32);
  struct EffectSprite *burstSprite;
  s32 i;

  scene->y += 0xa0000;
  FieldPsynergy_FadeToCastColor();
  BattleEffect_StartCanvasSprite();
  canvas = Ram_HeapSlots->unkec[1];
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  canvas->priority =
      FieldPsynergy_GetCanvasPriority(scene->mainObject, scene->facing);
  canvas->depth = BattleEffect_GetObjectDepth(scene->mainObject) + 2;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->scaleX = 0;
  canvas->scaleY = 0;
  if (scene->facing == TRIG_TURN * 3 / 4)
    canvas->depth -= 4;

  sparkEntry = VramCache_FindFreeEntry();
  sparkTile = VramCache_Load(sparkEntry, 64, Data_080f3984);
  for (i = 0, particle = work->sparks, mul = Iwram_MulQ16,
      sprite = work->sparkSprites;
       i < 32;) {
    EffectSprite_Init(sprite, 4, 4, 0, sparkTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = canvas->priority;
    sprite->depth = canvas->depth + 1;
    sprite->scaleY = sprite->scaleX = Q16_ONE;
    radius = Random16() * 20 + 0x140000;
    angle = Random16();
    particle->x = mul(Trig_Cos(angle), radius);
    particle->y = mul(Trig_Sin(angle), radius);
    radius /= 12;
    angle += TRIG_TURN / 2;
    particle->velocityX = mul(Trig_Cos(angle), radius);
    particle->velocityY = mul(Trig_Sin(angle), radius);
    particle->timer = -i * 2;
    i++;
    sprite++;
    particle++;
  }

  Decompress_TaggedLz(
      Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion), work);
  burstEntry = VramCache_FindFreeEntry();
  tile = VramCache_Load(burstEntry, 0x400, work);
  work->burstTile = tile;
  particle = work->bursts;
  burstSprite = work->burstSprites;
  for (i = 0; i < 32; i++) {
    EffectSprite_Init(burstSprite, 8, 8, 0x40000000, tile);
    burstSprite->depth = canvas->depth + 1;
    burstSprite->oam.priority = canvas->priority;
    burstSprite->oam.palette256 = 0;
    burstSprite->oam.palette = 15;
    particle->timer = -1;
    burstSprite++;
    particle++;
  }
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_GlowDisc),
                      Ram_HeapSlots->battleFxWork[1]);
  canvas->command = CANVAS_UPLOAD;
  Sound_PlayFar(SE_PSYNERGY_START);
  wait(1);
  work->phase = 0;
  work->timer = 0;
  work->paletteMode = 1;
  work->nextBurst = 0;
  work->newBursts = 0;
  Task_Add(FieldPsynergy_UpdateBurst, 0x480);
  kind = scene->fieldKind;
  pending = Scene_FindPsynergyTrigger(0x50000005, kind,
                                      Scene_SelectPsynergyTarget(kind));
  frame = 0;
  while (work->phase != 999) {
    wait(1);
    if (work->phase == 999)
      frame = 999;
    Field_RunPsynergyEvent(pending, work->shake, frame);
    frame++;
  }
  Task_Remove(FieldPsynergy_UpdateBurst);
  VramCache_FreeEntry(sparkEntry);
  VramCache_FreeEntry(burstEntry);
  FieldPsynergy_FadeFromCastColor();
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(92);
}

struct ShineParticle {
  s32 x;
  s32 y;
  u8 unk08[16];
  s32 timer;
};

struct ShineSprite {
  u8 unk00[12];
  s32 x;
  s32 y;
  u8 unk14[20];
};

struct ShineWork {
  u8 unk0000[0x1000];
  struct ShineParticle particles[12];
  u8 unk1150[0x230];
  struct ShineSprite sprites[12];
  u8 unk1560[0x348];
  struct Actor *object;
  s16 state;
  s16 timer;
  s8 sparkles;
  u8 fading;
};

struct BlendWork {
  u8 unk00[160];
  u32 phase;
  u8 unka4[12];
  s32 scaleX;
  s32 scaleY;
  u8 unkb8[9];
  u8 mode;
};

void MapGrid_SetScreenShakeFar(s32 a, s32 b, s32 c);

/// Task of the Pound: its shine's colours and particles, each frame.
void FieldPsynergy_UpdatePound(void) {
  struct ShineWork *work = (struct ShineWork *)Ram_HeapSlots->battleFxWork[0];
  struct BlendWork *blend = Ram_HeapSlots->unkec[1];
  struct ShineParticle *particle;
  struct ShineSprite *sprite;
  s32 i;
  s32 base, blue;
  s32 color;
  s32 red, green;
  u32 *phase;

  work->fading = 0;
  if (work->state == 1) {
    i = 0;
    /* FAKEMATCH(temporary): the phase read through a pointer kept across the
     * loop. */
    phase = &blend->phase;
    base = 9;
    base <<= 16;
    blue = 0x7c00;
  palette:
    red = Trig_Sin(((((*phase >> 1) + i) << 16) + base) >> 5) * 12;
    green = Trig_Sin(((((*phase >> 1) + i) << 16) + base) >> 5) * 12;
    red = (red >> 16) + 16;
    green = (green >> 16) + 16;
    /* FAKEMATCH(dead): the colour in a once-only block. */
    do {
      color = (green << 5) | blue | red;
      *(u16 *)(0x050003c0 + i * 2) = color;
    } while (0);
    if (++i < 32)
      goto palette;
  }
  if (work->sparkles) {
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 12; i++, sprite++, particle++) {
      if (particle->timer >= 0 && particle->timer < 16) {
        sprite->x = particle->x;
        sprite->y = particle->y;
        EffectSprite_Draw(sprite);
        BattleEffect_IntegrateVector2(particle, 64, 0);
      }
      particle->timer++;
    }
  }
  switch (work->state) {
  case 0:
    work->object->y -= 0x38000;
    if (work->timer == 8) {
      blend->scaleX = Q16_ONE / 8;
      blend->scaleY = Q16_ONE / 8;
      MapGrid_SetScreenShakeFar(0x14ccc, 0x14ccc, Q16_ONE);
      Sound_PlayFar(SE_567);
      work->sparkles = 1;
      work->fading = 1;
    }
    if (work->timer == 8) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    blend->scaleX += Q16_ONE / 16;
    blend->scaleY = blend->scaleX;
    if (blend->scaleX > 0xcccb) {
      blend->scaleX = 0xcccc;
      blend->scaleY = 0xcccc;
      work->state++;
      work->timer = -1;
    }
    break;
  case 2:
    blend->mode = 3;
    if (work->timer == 32) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 3:
    if (work->timer == 0)
      MapGrid_SetScreenShakeFar(1, 1, 1);
    if (work->timer == 10) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 4:
    work->object->y += 0x18000;
    if (work->timer == 13)
      work->state = 999;
    break;
  }
  work->timer++;
}
