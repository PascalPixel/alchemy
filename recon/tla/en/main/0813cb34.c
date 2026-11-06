// Matches: differ 0 of 477 (tla-en, 2026-11-06), plain C with no device; the
// other five editions' listings also measure differ 0. The plain arm keeps
// speed = (Random16() & 0x3ff) + 32 before the y store: the add's constant
// does not fit a three-operand add, so it is a copy and an add, and the add
// lands after the store as in the native.
// Earlier: near miss, differ 44 of 477 (tla-en, 2026-11-05) with an empty asm
// and a one-pass block on the plain arm's angle; differ 60, skeleton gap 4
// (tla-en, 2026-11-01).

// Twin: The Broken Seal's BattleEffect_RunMoteFountain, the body of two
// effects there. ⚓️ adds a third mode: Slaver's green spray, thrown up in a
// half-circle fan, falling back under gravity and bouncing on the
// ground for 96 frames, which hands over to the Impair canvas effect at its
// end. The other two modes are the twin with ⚓️'s calling conventions.

// Tried before the match (differ, as measured then):
// the twin with Slaver's mode added                         83
// the group size of 32 as a local: the native divides by
// calling __divsi3, as only a divisor in a register gives   89
// ClearWords through an inline, as The Broken Seal's
// iwram_call.h layer: the two clears keep their own size    73
// frames declared before source (the stack slots swap)       61
// speed += 32 after the y store                              82 (count right)
// the masked speed in its own local before the store         106
// speed = 0x3ff; speed &= Random16(), as the twin            113
// the plain arm as masked = Random16() & 0x3ff; mote->y = ...;
// speed = masked + 32, which gives the native's copy before the y store
// and the add after it, every pc offset and branch right, but rotates
// the scratch registers of the loop and the code after it         96

// Measured 2026-11-01, as differ and skeleton gap:
// the same masked form, in the block, in the loop or by declaration order  86, 14
// speed = 0x3ff; speed &= Random16(); y; speed += 32 (the twin's form)    93, 11
// the mask in a local before the call                                     176, 20
// the mask in a local after the call; a copy through masked               60, 4
// masked as u16 or s16, or a u16 cast, then speed += 32                  113, 9
// u16 masked; speed = masked + 32 before the y store                       64, 12
// y through a local, the add between the load and the store                89, 7
// source->y read before the calls                                         237, 19
// speed += 32 and the y store as one comma expression                      89, 7
// speed = 32; speed += Random16() & 0x3ff                                  37, 5
// the first loop walking mote++ (the source store stays late)              37, 5

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "motobj.h"
#include "battle_display.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "graphics.h"
#include "transform.h"
#include "resource.h"

typedef DrawRectangle BlitterPair[2];

// The blitters' heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BLITTER 104           // unk64[1]
#define SLOT_BLITTER_ALTERNATE 188 // unkb8[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define TASK_PASS_FRAME 0xc80

enum { POOL_SOOTHE, POOL_SLEEP, POOL_SLAVER };

// The motes live in the map cell buffer.
#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)

// The mote's cell for each size from 1 to 6 (The Broken Seal's
// gBattleEffect_MoteCells).
extern u16 gData_08197410[];

extern char ResourceId_Data134; // The Broken Seal's ParticleSpritesA
extern char ResourceId_OrangePaletteA;
extern char ResourceId_EarthWallSheet;
extern char ResourceId_GreenPalette;

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);
void Palette_StepFadeTransfer(void);
s32 Task_Add(void *func, s32 order);
s32 Task_Remove(void *func);
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
u32 Random16(void);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void BattleEffect_CountDownBattlerTints(void);
void SummonEffect_RunCanvasMode1(void *effect);
void wait(s32 frames);

// The IWRAM routines are called through one inline each, as The Broken Seal's
// iwram_call.h layer calls them: called directly, CSE shares the size
// between the two clears.
static inline s32 ClearWords(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

// Soothing Star (mode 0, effect kind 344), Sleep Star (mode 1, kind 349) and
// Slaver (mode 2, kind 395).

// 256 motes burst from 5 pixels above the user and spread in every
// direction; the first 128 are drawn, a quarter of them starting every 8
// frames, each swaying on a sine wave and sized by its depth. Soothing Star's
// burst is half as wide and sounds as it touches each target; Sleep Star's
// drifts away from the user's side. From frame 48 the targets flash one
// after another, 8 frames apart, and the first flash lets the battle show its
// results. Slaver instead throws green drops up in a half-circle fan, turned
// by the user's side, with a sound at the start; they fall back and bounce
// on the ground for 96 frames, and then Impair's canvas effect runs on the
// targets.

void Func_0813cb34(struct BattleEffectArgument *effect, s32 mode) {
  s32 point[3];
  struct EffectPosition position;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 frames;
  struct MotionObject *source;
  struct EffectStep *mote;
  s32 resource;
  s32 frame;
  s32 i;
  s32 group = 32; // motes that start together

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  if (mode == POOL_SOOTHE)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(1);

  // Motes draw at their maximum; the second blitter, mirrored top to
  // bottom, is loaded but not used.
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_Y));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == POOL_SOOTHE)
    resource = (s32)&ResourceId_OrangePaletteA;
  else if (mode == POOL_SLEEP)
    resource = (s32)&ResourceId_EarthWallSheet;
  else
    resource = (s32)&ResourceId_GreenPalette;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Launch every mote from the user in 16.16 fixed point, with a speed and
  // a direction of its own. Slaver throws them up, faster, in a half-circle
  // fan turned by the user's side, from up to 16 pixels higher.
  source = GetBattleObjectSlotFar(effect->actor)->object;
  for (i = 0; i != 256; i++) {
    s32 speed;
    s32 angle;

    mote = &gMotes[i];
    if (mode == POOL_SLAVER) {
      if (effect->side == 0)
        angle = (Random16() & 0x7fff) + 0x8000;
      else
        angle = Random16() & 0x7fff;
      speed = (Random16() & 0x3ff) + 0x800;
      mote->y = source->y + (Random16() << 4) + 0x50000;
    } else {
      angle = Random16();
      speed = (Random16() & 0x3ff) + 32;
      mote->y = source->y + 0x50000;
    }
    mote->x = source->x;
    mote->z = source->z;
    mote->velocityX = Trig_Sin(angle) * speed >> 8;
    if (mode == POOL_SLAVER) {
      mote->velocityY = ((Random16() & 255) + 128) << 10;
      mote->velocityZ = -(Trig_Cos(angle) * speed) >> 9;
    } else {
      mote->velocityY = ((Random16() & 255) - 32) << 9;
      mote->velocityZ = -(Trig_Cos(angle) * speed << 1) >> 8;
    }
    mote->variant = (Random16() & 31) + 48;
    if (mode == POOL_SOOTHE) {
      mote->velocityX /= 2;
      mote->velocityZ /= 2;
    }
  }

  frames = 96;
  if (mode != POOL_SLAVER)
    frames = 128;
  for (frame = 0; frame != frames; frame++) {
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (mode == POOL_SLAVER && frame == 0)
      Sound_PlayFar(103);

    for (i = 0; i != 128; i++) {
      mote = &gMotes[i];
      if (frame >= i / group * 8 && mote->variant >= 0) {
        s32 size;

        // Sway the mote, project it and size it by its depth: 6 pixels
        // near, 1 far.
        point[0] = mote->x + Trig_Sin((i * 4 + mote->variant) << 10) * 16;
        point[1] = mote->y;
        point[2] = mote->z;
        EffectPosition_ProjectPoint(point, &position);
        position.x >>= 1;
        if (position.depth <= 313)
          position.depth = 314;
        if (position.depth > 634)
          position.depth = 634;
        size = 6 - (position.depth - 314) / 64;
        draw[0](canvas, sheet + gData_08197410[size - 1], position.x - size / 2,
                position.y - size, size, size * 2);

        // Slaver's drops fall and bounce off the ground; the others rise
        // slowly. Sleep Star's drift away from the user's side.
        if (mode == POOL_SLAVER) {
          BattleEffectKernels_IntegrateVector3(mote, 62, -0x2000);
          if (mote->y <= 0x4ffff)
            mote->velocityY = -mote->velocityY;
        } else {
          BattleEffectKernels_IntegrateVector3(mote, 62, 0x400);
        }
        if (mode == POOL_SLEEP) {
          if (source->x < 0)
            mote->velocityX += 0x2000;
          else
            mote->velocityX -= 0x2000;
        }
        mote->variant--;
      }
    }

    // From frame 48, flash the targets one after another, 8 frames apart.
    if (mode == POOL_SLEEP) {
      for (i = 0; i != effect->count; i++) {
        if (frame == i * 8 + 48) {
          Func_081180e8(-1);
          Func_0814cd48(effect->actors[i], 7, 5, i, 8);
        }
      }
    } else if (mode == POOL_SOOTHE) {
      for (i = 0; i != effect->count; i++) {
        if (frame == i * 8 + 48) {
          Sound_PlayFar(126);
          Func_081180e8(-1);
          Func_0814cd48(effect->actors[i], 7, -1, i, 8);
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  if (mode == POOL_SLAVER) {
    // Clear the canvas and run Impair's effect on the targets.
    ClearWords((void *)0x06004000, 0x3c00);
    ClearWords(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    SummonEffect_RunCanvasMode1(effect);
  } else {
    BattleEffect_EndCanvasLayer();
  }
}
