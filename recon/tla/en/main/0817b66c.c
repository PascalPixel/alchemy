// Draft (tla-en): listing 326 instructions, draft 326, common 318, differ 16.
// Effect kind 76, written from the listing (☀️'s nearest shapes are
// BattleFx_RunUndeadSword, 0.55, and BattleFx_RunEruption; neither is
// a twin). The shape matches; what is left is the arm9tdmi scheduling
// leftover.
// In this draft: BG2PA's 0x100 pinned to r3 behind a one-pass block (as
// s2614.c), so it loads from the pool before the register address as the
// native does, and the frame-80 block's "drift" temporary (alchemy
// permute's), which puts velocityX's store after velocityY's.
// Left:
// - the one-pass block's barrier depends on every earlier use of r10, so
// sched2 now prefers "add r1, r10" to the resource id's pool load before
// Resource_LoadAndDecompress (a one-pass block around or before that
// call only moves the swap). f18284c.c's empty asm instead keeps the
// start but counts four estimated bytes, and the halfword pool (range
// 64) is then placed before the loop with a jump around it.
// - frame's 0 is set between "mov r3, sp" and "adds r3, #12"; the native
// stores &texture at sp+8 first. Through a "source" local set before the
// loop the order is right but &texture takes r2 and the 0 r3.
// - the frame-0 block's setup (y, i, the zero and step) is in another
// order; the size mask's pool load comes after the stack load of
// texture.size (the native loads the pool word first, as if the load
// after the primitive's unk14 store waited a cycle more); the step
// mask's pool load comes one row late.
// The texture's size word is two byte fields written with masks: as u32
// bitfields the compiler stores bytes (differ 57).
// alchemy permute (22 minutes over three runs): differ 16 at best.
// 2026-10-15 (lane N2): rewritten with the work struct, named constants
// and comments; the same 326 instructions and differ 16.
// Resource 0x139 (BattleFx_CrossedDaggersSheet) needs its name in
// system/resource/directory.s when this is adopted.
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "types.h"

#define DAGGER_FRAMES 96
#define DAGGER_COUNT 8       // of the 32 steps set up, the first 8 are drawn
#define DAGGER_FALL_FRAME 80 // the daggers turn and fall on the target
#define DAGGER_LAST_HIT 90

// A dagger's texture: log2 of its width and height in the first word (32x32
// here), then its pixels.
struct DaggerTexture {
  u32 size;
  void *pixels;
};

// One entry of the list Func_08196a7c draws; kind 9 is a textured quad.
// Func_081969f8 allocates the list with an empty entry after it.
struct RenderPrimitive {
  s32 kind;
  s32 unk04;
  const void *faces;
  void *vertices;
  struct DaggerTexture *texture;
  s32 unk14;
};

extern char ResourceId_CrossedDaggersSheet;
extern s32 gCameraSceneParameters[];

// The dagger quad: its face list and its four corners, 32x80 pixels from
// (-16, -16) to (16, 64), the point down.
extern const u8 gData_08199450[];
extern const u8 gData_08199474[];

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Resource_LoadAndDecompress(s32 resourceId, void *target, s32 unpack,
                                s32 wait);
void Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
void *GetBattleObjectSlotFar(s32 id);
void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 id, struct EffectPosition *position);
u32 Random16(void);
void Render_ResetTransformState(void);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer, s32 frames);
void BattleFx_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct RenderPrimitive *Func_081969f8(s32 count);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void Func_080150e4(s32 angle);
void Func_08196958(const void *corners, void *vertices, s32 count);
void Func_08196a7c(struct RenderPrimitive *list);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void MemFree(void *buffer);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Func_08143000(void);
void Func_08143bb8(void);

// Crossed daggers. The canvas is moved over the target. Eight daggers rise
// spinning from below it, and the target is struck eight times, six frames
// apart from frame 20: each strike shakes the screen, flashes the target and
// knocks it up. At frame 80 the daggers turn point down in a fan above the
// target and plunge into it; the last strike, at frame 90, starts the
// result phase.
void BattleFx_RunSabreDance(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  struct DaggerTexture texture;
  struct EffectPosition target;
  struct EffectStep *dagger;
  struct RenderPrimitive *quad;
  void *vertices;
  s32 frame;
  s32 i;
  s32 strikeFrame;

  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_CrossedDaggersSheet, work->sheet,
                             1, 1);
  work->transferMode = 1;
  work->transferValue = 0;
  REG_BLDCNT = 0;
  Scheduler_AddOrUpdateCallback((s32)Func_08143000, 0xc80);

  // Centre the canvas's x 64 on the target.
  GetBattleObjectSlotFar(effect->actors[0]);
  EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &target);
  {
    /* FAKEMATCH: the native loads BG2PA's 0x100 from the pool before it
     * starts the register's address; the pinned halfword behind a one-pass
     * block does the same (plain: the address first, one reordered pair). */
    register u16 scale __asm__("r3") = 0x100;

    do {
    } while (0);
    REG_BG2PA = scale;
  }
  REG_BG2X = (64 - target.x) << 8;

  for (frame = 0; frame != DAGGER_FRAMES; frame++) {
    // The first frame lines the daggers up below the canvas, each spinning
    // the other way from its neighbour.
    if (frame == 0) {
      s32 y = 128 << 16;

      for (i = 0, dagger = work->particles; i != 32; i++, dagger++) {
        dagger->x = ((Random16() & 63) + 32) << 16;
        dagger->y = y;
        dagger->z = Random16();
        dagger->velocityX = 0;
        dagger->velocityY = -((Random16() & 31) + 32) << 10;
        dagger->velocityZ = (Random16() & 0x3ff) + 0x400;
        if (i & 1)
          dagger->velocityZ = -dagger->velocityZ;
        dagger->variant = 0;
        y += 8 << 16;
      }
    }
    Render_ResetTransformState();
    gCameraSceneParameters[4] = 104;

    // The eight strikes.
    for (i = 0, strikeFrame = 20; i != 8; i++, strikeFrame += 6) {
      if (frame == strikeFrame) {
        work->shakeFrames = 4;
        Audio_PlayCue(133);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 6);
        BattleFx_StartActorMotion(effect->actors[0], 1, 0, 0, 3 << 16, 100);
      }
    }
    if (frame == DAGGER_LAST_HIT) {
      work->shakeFrames = 4;
      Func_081180e8(134);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 10);
      BattleFx_StartActorMotion(effect->actors[0], 1, 0, 0, 3 << 16, 100);
    }

    // One textured quad, drawn once for each dagger.
    vertices = Runtime_BumpAllocateAlternatePool(32);
    quad = Func_081969f8(1);
    quad->unk14 = 0;
    texture.size = (texture.size & ~0xff) | 5;
    texture.size = (texture.size & ~0xff00) | 5 << 8;
    quad->texture = &texture;
    texture.pixels = work->sheet;
    quad->kind = 9;
    quad->faces = gData_08199450;
    quad->vertices = vertices;
    for (i = 0, dagger = work->particles; i != DAGGER_COUNT; i++, dagger++) {
      // The fall: from 48 pixels above the canvas, fanned out by angle and
      // closing on the middle.
      if (frame == DAGGER_FALL_FRAME) {
        s32 angle;
        s32 drift;

        dagger->y = -48 << 16;
        angle = (i << 10) + (Random16() & 0x3ff) - 0x1200;
        dagger->x = (angle << 10) + (64 << 16);
        dagger->z = angle;
        drift = -angle << 5;
        dagger->velocityY = 12 << 16;
        dagger->velocityX = drift;
        dagger->velocityZ = 0;
      }
      Render_ResetTransformState();
      Func_08015160(dagger->x - (64 << 16), dagger->y - (64 << 16), 0);
      Func_0801521c(0x8000);
      Func_080150e4(dagger->z);
      Func_08196958(gData_08199474, vertices, 4);
      Func_08196a7c(quad);
      if (frame >= DAGGER_FALL_FRAME)
        BattleFxKernels_IntegrateVector3(dagger, 64, 0);
      else
        BattleFxKernels_IntegrateVector3(dagger, 64, -0x1000);
    }
    MemFree(quad);
    MemFree(vertices);
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;

    Func_08158ce0(4, 4);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback((s32)Func_08143000);
  Func_08143bb8();
}
