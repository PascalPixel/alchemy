// Draft, differ 72 of 366: a field effect that starts Func_080e4d9c: the scene
// is raised ten pixels, the canvas sprite starts at the scene's position
// with the caster's priority and depth (four nearer when the scene faces
// 0xc000) and the glow disc (FieldEffect_GlowDisc) in its canvas; 32 sparks
// of Data_080f3984 start on a ring 20 to 40 pixels out, drifting outward,
// two frames apart, and 32 bursts of FieldEffect_Explosion wait. With cue
// 130 the callback runs until its phase reaches 999, the effect's stage-5
// event running each frame with the shake level and the frame count (999 on
// the last frame).
// Left: register choice in the two sprite loops (the native keeps the
// spark loop's counter in r10 set after r8, and the burst loop's sprite in
// r5 and its tile in r6) and a few reordered stores.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
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
  struct ObjectRuntime *mainObject;
  struct ObjectRuntime *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
};

extern const u8 Data_080f3984[];
extern char ResourceId_Explosion, ResourceId_GlowDisc;

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleEffect_InitializeSharedScene(void);
void Camera_WorldToScreen(s32 *position);
s32 Func_080db9a8(struct ObjectRuntime *object, s32 facing);
s32 BattleEffect_GetObjectDepth(struct ObjectRuntime *object);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
s32 Func_080ce31c(s32 kind);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void EventRuntime_RunPendingEvent(void *event, s32 a, s32 b);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e4d9c(void);

void Func_080e572c(void) {
  struct GlowWork *work =
      Runtime_AllocateHeapBlock(92, sizeof(struct GlowWork));
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
  particle = work->sparks;
  sprite = work->sparkSprites;
  for (i = 0; i < 32; i++) {
    Func_080eaf98(sprite, 4, 4, 0, sparkTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = canvas->priority;
    sprite->depth = canvas->depth + 1;
    sprite->scaleX = sprite->scaleY = 0x10000;
    radius = Random16() * 20 + 0x140000;
    angle = Random16();
    particle->x = Iwram_MulQ16(Trig_Cos(angle), radius);
    particle->y = Iwram_MulQ16(Trig_Sin(angle), radius);
    radius /= 12;
    angle += 0x8000;
    particle->velocityX = Iwram_MulQ16(Trig_Cos(angle), radius);
    particle->velocityY = Iwram_MulQ16(Trig_Sin(angle), radius);
    particle->timer = -i * 2;
    sprite++;
    particle++;
  }

  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Explosion),
                        work);
  burstEntry = Resource_FindFreeEntry();
  tile = VramCache_Load(burstEntry, 0x400, work);
  work->burstTile = tile;
  particle = work->bursts;
  sprite = work->burstSprites;
  for (i = 31; i >= 0; i--) {
    Func_080eaf98(sprite, 8, 8, 0x40000000, tile);
    sprite->depth = canvas->depth + 1;
    sprite->oam.priority = canvas->priority;
    sprite->oam.palette256 = 0;
    sprite->oam.palette = 15;
    particle->timer = -1;
    sprite++;
    particle++;
  }
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GlowDisc),
                        Ram_HeapSlots->battleFxWork[1]);
  canvas->command = CANVAS_UPLOAD;
  Audio_PlayCue(130);
  WaitFrames(1);
  work->phase = 0;
  work->timer = 0;
  work->paletteMode = 1;
  work->nextBurst = 0;
  work->newBursts = 0;
  Scheduler_AddOrUpdateCallback(Func_080e4d9c, 0x480);
  kind = scene->fieldKind;
  pending = Func_080ce458(0x50000005, kind, Func_080ce31c(kind));
  frame = 0;
  while (work->phase != 999) {
    WaitFrames(1);
    if (work->phase == 999)
      frame = 999;
    EventRuntime_RunPendingEvent(pending, work->shake, frame);
    frame++;
  }
  Scheduler_RemoveCallback(Func_080e4d9c);
  Resource_ResetEntry(sparkEntry);
  Resource_ResetEntry(burstEntry);
  BattleEffect_PrepareBufferInterpolation();
  BattleEffect_EndCanvasSprite();
  Runtime_ReleaseHeapBlock(92);
}
