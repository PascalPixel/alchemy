#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"
#include "canvas_sprite.h"
#include "iwram_call.h"

struct GlowParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct GlowWork {
  u8 tiles[0x400];
  struct GlowParticle sparks[32];
  struct EffectSprite sparkSprites[32];
  struct EffectSprite burstSprites[32];
  struct GlowParticle bursts[32];
  s16 phase;
  s16 timer;
  s16 shake;
  u16 burstTile;
  s16 nextBurst;
  s16 newBursts;
  s8 paletteMode;
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

extern const u8 Data_080f3984[];
extern char ResourceId_FieldEffectExplosion, ResourceId_GlowDisc;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void BattleEffect_InitializeSharedScene(void);
void Camera_WorldToScreen(s32 *position);
s32 Func_080db9a8(struct Actor *object, s32 facing);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
s32 Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
s32 Func_080ce31c(s32 kind);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void Field_RunPsynergyEvent(void *event, s32 a, s32 b);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e4d9c(void);

void Func_080e572c(void) {
  struct GlowWork *work = HeapSlot_Alloc(92, sizeof(struct GlowWork));
  struct FxScene *scene = Ram_HeapSlots->effectWork;
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
  BattleEffect_InitializeSharedScene();
  Func_080eb824();
  canvas = Ram_HeapSlots->unkec[1];
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  canvas->priority = Func_080db9a8(scene->mainObject, scene->facing);
  canvas->depth = BattleEffect_GetObjectDepth(scene->mainObject) + 2;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->scaleX = 0;
  canvas->scaleY = 0;
  if (scene->facing == 0xc000)
    canvas->depth -= 4;

  sparkEntry = Resource_FindFreeEntry();
  sparkTile = VramCache_Load(sparkEntry, 64, Data_080f3984);
  for (i = 0, particle = work->sparks, mul = Iwram_MulQ16Entry,
      sprite = work->sparkSprites;
       i < 32;) {
    Func_080eaf98(sprite, 4, 4, 0, sparkTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = canvas->priority;
    sprite->depth = canvas->depth + 1;
    sprite->scaleY = sprite->scaleX = 0x10000;
    radius = Random16() * 20 + 0x140000;
    angle = Random16();
    particle->x = mul(Trig_Cos(angle), radius);
    particle->y = mul(Trig_Sin(angle), radius);
    radius /= 12;
    angle += 0x8000;
    particle->velocityX = mul(Trig_Cos(angle), radius);
    particle->velocityY = mul(Trig_Sin(angle), radius);
    particle->timer = -i * 2;
    i++;
    sprite++;
    particle++;
  }

  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion), work);
  burstEntry = Resource_FindFreeEntry();
  tile = VramCache_Load(burstEntry, 0x400, work);
  work->burstTile = tile;
  particle = work->bursts;
  burstSprite = work->burstSprites;
  for (i = 0; i < 32; i++) {
    Func_080eaf98(burstSprite, 8, 8, 0x40000000, tile);
    burstSprite->depth = canvas->depth + 1;
    burstSprite->oam.priority = canvas->priority;
    burstSprite->oam.palette256 = 0;
    burstSprite->oam.palette = 15;
    particle->timer = -1;
    burstSprite++;
    particle++;
  }
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GlowDisc),
                        Ram_HeapSlots->battleFxWork[1]);
  canvas->command = CANVAS_UPLOAD;
  Sound_PlayFar(130);
  wait(1);
  work->phase = 0;
  work->timer = 0;
  work->paletteMode = 1;
  work->nextBurst = 0;
  work->newBursts = 0;
  Task_Add(Func_080e4d9c, 0x480);
  kind = scene->fieldKind;
  pending = Func_080ce458(SPLIT_CONSTANT(0x50000005), kind, Func_080ce31c(kind));
  frame = 0;
  while (work->phase != 999) {
    wait(1);
    if (work->phase == 999)
      frame = 999;
    Field_RunPsynergyEvent(pending, work->shake, frame);
    frame++;
  }
  Task_Remove(Func_080e4d9c);
  Resource_ResetEntry(sparkEntry);
  Resource_ResetEntry(burstEntry);
  BattleEffect_PrepareBufferInterpolation();
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(92);
}
