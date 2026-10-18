// Near miss, differ 61 of 477.

// Twin: The Broken Seal's BattleFx_RunParticlePool, the body of two
// effects there. ⚓️ adds a third mode: Slaver's green spray, thrown up in a
// half-circle fan, falling back under gravity and bouncing on the
// ground for 96 frames, which hands over to the Impair canvas effect at its
// end. The other two modes are the twin with ⚓️'s calling conventions.

// Left: in the launch loop's plain arm the native stores the mote's y before
// it adds 32 to the speed; here the add comes first, so both arms end in the
// same store and cross-jumping merges them, one instruction short. That
// shifts the first literal pool and every pc offset and branch across it
// (about 30 of the rows). The rest are single rows where the native issues
// a load or a constant one slot earlier, and the two calls' arguments to
// EffectPosition_ApplyBaseAndYOffset set in the other order.

// Tried:
// the twin with Slaver's mode added                         83
// the group size of 32 as a local: the native divides by
// calling __divsi3, as only a divisor in a register gives   89
// ClearWords through an inline, as The Broken Seal's
// iwram_call.h layer: the two clears keep their own size    73
// frames declared before source (the stack slots swap)       61
// speed += 32 after the y store                              82 (count right)
// the masked speed in its own local before the store         106
// speed = 0x3ff; speed &= Random16(), as the twin            113

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

#define CALLBACK_ORDER 0xc80

enum { POOL_SOOTHE, POOL_SLEEP, POOL_SLAVER };

// The motes live in the map cell buffer.
#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)

// The mote's cell for each size from 1 to 6 (The Broken Seal's
// gBattleFx_MoteCellOffsets).
extern u16 gData_08197410[];

extern char ResourceId_Data134; // The Broken Seal's ParticleSpritesA
extern char ResourceId_OrangePaletteA;
extern char ResourceId_EarthWallSheet;
extern char ResourceId_GreenPalette;

void BattleFx_BeginCanvasLayer(s32 control);
void BattleFx_EndCanvasLayer(void); // BattleFx_EndCanvasLayer
s32 BattleFx_LoadBlitter(s32 slot, s32 flags);
void Palette_StepFadeTransfer(void); // Palette_StepFadeTransfer
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 order);
s32 Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase); // BattleEventRuntime_BeginPhaseFar
u32 Random16(void);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void ObjectGroup_TickMemberTimers(void);            // ObjectGroup_TickMemberTimers
void SummonFx_RunCanvasMode1(void *effect);
void WaitFrames(s32 frames);

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
    BattleFx_BeginCanvasLayer(0);
  else
    BattleFx_BeginCanvasLayer(1);

  // Motes draw at their maximum; the second blitter, mirrored top to
  // bottom, is loaded but not used.
  BattleFx_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleFx_LoadBlitter(SLOT_BLITTER_ALTERNATE,
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
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);

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
      Audio_PlayCue(103);

    for (i = 0; i != 128; i++) {
      mote = &gMotes[i];
      if (frame >= i / group * 8 && mote->variant >= 0) {
        s32 size;

        // Sway the mote, project it and size it by its depth: 6 pixels
        // near, 1 far.
        point[0] = mote->x + Trig_Sin((i * 4 + mote->variant) << 10) * 16;
        point[1] = mote->y;
        point[2] = mote->z;
        EffectPosition_ApplyBaseAndYOffset(point, &position);
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
          BattleFxKernels_IntegrateVector3(mote, 62, -0x2000);
          if (mote->y <= 0x4ffff)
            mote->velocityY = -mote->velocityY;
        } else {
          BattleFxKernels_IntegrateVector3(mote, 62, 0x400);
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
          Audio_PlayCue(126);
          Func_081180e8(-1);
          Func_0814cd48(effect->actors[i], 7, -1, i, 8);
        }
      }
    }
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  if (mode == POOL_SLAVER) {
    // Clear the canvas and run Impair's effect on the targets.
    ClearWords((void *)0x06004000, 0x3c00);
    ClearWords(canvas, 0x3c00);
    Scheduler_RemoveCallback(Palette_StepFadeTransfer);
    SummonFx_RunCanvasMode1(effect);
  } else {
    BattleFx_EndCanvasLayer();
  }
}
