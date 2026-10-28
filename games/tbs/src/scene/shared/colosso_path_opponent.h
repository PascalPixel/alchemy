#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "task.h"
#include "colosso_gauge.h"
#include "resource.h"
#include "types.h"
#include "mem.h"
#include "event.h"
#include "dma.h"
#include "vram_cache.h"
#include "io_reg.h"
#include "flags.h"

// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
// the opponent who races Isaac by running a recorded path, and the race gauge
// that shows how near each of them is to the finish.

/// The race's work in HEAP_SLOT_COLOSSO_RACE, allocated whole when the gauge
/// starts: the gauge, then the opponent's path.
struct ColossoRaceWork {
  struct ColossoGaugeWork gauge;
  /// Pixels, as pairs of x and z, one pair a frame, ending with (0, 0):
  /// decoded from the map's resource, or recorded (PATH_RECORDER_RECORD).
  s16 path[0x3840];
};

STATIC_ASSERT(sizeof(struct ColossoRaceWork) == 0x7170);

/// The opponent's task. PATH_RECORDER_PLAY plays the path back: each
/// frame the opponent steps to the next point (mirrored across the finish when
/// asked), turning by at most a sixteenth of a turn, and stands once still for
/// three frames. At the path's end the opponent walks to the finish, beside the
/// first finisher's item actor or the second's, and sets
/// FLAG_SCENE_TEMP_11 (a racer has finished). PATH_RECORDER_RECORD records the
/// actor's own path instead, up to 0x383e halfwords, then plays it back
/// with the opponent.
static void Colosso_UpdatePathOpponent(void) {
  struct ColossoRaceWork *work = (struct ColossoRaceWork *)gColossoWork;
  struct PathRecorder *rec = (struct PathRecorder *)gSceneLocalState;
  struct Actor *actor;
  s32 x, z, d;
  u16 *itemActorId;

  actor = Actor_GetById(rec->actorId);
  if (actor == NULL) {
    return;
  }
  if (rec->mode == PATH_RECORDER_PLAY) {
    x = work->path[(s16)rec->pathIndex++];
    z = work->path[(s16)rec->pathIndex++];
    if (x == 0 && z == 0) {
      s32 fz;
      rec->mode = PATH_RECORDER_FINISHED;
      Actor_SelectAnimation(actor, 1);
      if (work->gauge.finishX < actor->x.fixed) {
        x = work->gauge.finishX + 0xc0000;
      } else {
        x = work->gauge.finishX - 0xc0000;
      }
      if (GameFlag_Get(FLAG_SCENE_TEMP_11)) {
        fz = work->gauge.finishZ + 0x100000;
        itemActorId = &work->gauge.finishItemActorIdB;
      } else {
        fz = work->gauge.finishZ - 0x100000;
        itemActorId = &work->gauge.finishItemActorIdA;
      }
      actor->timer = *itemActorId;
      actor->acceleration = 0x4000;
      actor->speedLimit = 0x10000;
      FieldActor_SetMoveTarget(actor, x, 0, fz);
      GameFlag_Set(FLAG_SCENE_TEMP_11);
      Actor_StartScript(actor, gColosso_OpponentFinishScript);
      return;
    }
    x <<= 16;
    z <<= 16;
    if (rec->mirror != 0) {
      x = work->gauge.finishX * 2 - x;
    }
    if (actor->x.fixed != x || actor->z.fixed != z) {
      {
        u16 angle = ArcTan2(z - actor->z.fixed, x - actor->x.fixed);
        d = (s16)(angle - actor->facing);
      }
      if (d > 0x1000) {
        d = 0x1000;
      }
      if (d < -0x1000) {
        d = -0x1000;
      }
      actor->facing += d;
      actor->x.fixed = x;
      actor->z.fixed = z;
      rec->still = 0;
    } else {
      rec->still++;
    }
    if (rec->still > 2) {
      Actor_SelectAnimation(actor, 1);
    } else {
      Actor_SelectAnimation(actor, 5);
    }
  } else if (rec->mode == PATH_RECORDER_RECORD) {
    x = actor->x.part.pixel;
    z = actor->z.part.pixel;
    work->path[(s16)rec->pathIndex++] = x;
    work->path[(s16)rec->pathIndex++] = z;
    if ((s16)rec->pathIndex == 0x383e) {
      work->path[(s16)rec->pathIndex++] = 0;
      work->path[(s16)rec->pathIndex] = 0;
      rec->actorId = work->gauge.opponentActorId;
      rec->pathIndex = 0;
      rec->mode = PATH_RECORDER_PLAY;
    }
  }
}

extern u8 gColosso_GaugeGraphics[];

/// The race gauge's task: a bar across the top of the screen, with
/// the finish at its centre and blinking marks for the opponent and Isaac at
/// their distances from it (a pixel of bar for 14 of course). It slides down
/// when the race starts and back up while FLAG_MENU_OPEN is set; hold keeps it
/// down.
static void Colosso_DrawGauge(void) {
  struct ColossoGaugeWork *gauge = (struct ColossoGaugeWork *)gColossoWork;
  s32 *dst = (s32 *)gauge;
  struct ColossoGaugeSprite *entry = gauge->sprites;
  s32 tile;
  u32 segments;
  u32 i;
  s32 y;
  s32 x;
  s32 width;
  u8 *buf;
  struct Actor *actor;
  u32 shape;

  tile = gVramCacheEntries[gauge->vramBlock].offset >> 5;
  segments = gauge->segments;
  if (gauge->hold != 0) {
    gauge->level = 2;
  } else if (GameFlag_Get(FLAG_MENU_OPEN)) {
    if (gauge->level > 0) {
      gauge->level--;
    }
  } else if (gauge->level <= 1) {
    if (++gauge->level == 1) {
      Dma_Set(gColosso_GaugePalette, (void *)&OBJ_PLTT_COLOR(14, 0),
              (DMA_ENABLE << 16) | 0x10, REG_DMA3);
      buf = Heap_CallAlternatePool(Heap_AllocEwram, 512);
      FieldDecompress_TaggedLz(gColosso_GaugeGraphics, buf);
      VramCache_Load(gauge->vramBlock, 512, buf);
      Heap_Release(buf);
    }
  }
  if (gauge->level == 0) {
    VramCache_ReleaseVram(gauge->vramBlock);
    return;
  }

  y = gauge->level * 6 - 8;
  y &= 255;
  width = segments * 16;
  x = 104 - width;
  shape = 0x8000;
  *dst++ = 0;
  *dst++ = (x << 16) | y | shape;
  *dst++ = tile | 0xe400;
  Oam_AddSpriteAtDepth(entry++, 255);
  for (i = 0; i < segments; i++) {
    x = 96 - i * 16;
    shape = 0x40000000;
    *dst++ = 0;
    *dst++ = (x << 16) | y | shape;
    *dst++ = (tile + 2) | 0xe400;
    Oam_AddSpriteAtDepth(entry++, 255);
  }
  x = 112;
  shape = 0x8000;
  *dst++ = 0;
  *dst++ = (x << 16) | y | shape;
  *dst++ = (tile + 6) | 0xe400;
  Oam_AddSpriteAtDepth(entry++, 255);
  x = 120;
  *dst++ = 0;
  *dst++ = (x << 16) | y | shape | 0x10000000;
  *dst++ = (tile + 6) | 0xe400;
  Oam_AddSpriteAtDepth(entry++, 255);
  for (i = 0; i < segments; i++) {
    x = 128 + i * 16;
    shape = 0x40000000;
    *dst++ = 0;
    *dst++ = y | (x << 16) | shape | 0x10000000;
    *dst++ = (tile + 2) | 0xe400;
    Oam_AddSpriteAtDepth(entry++, 255);
  }
  x = width + 128;
  shape = 0x8000;
  *dst++ = 0;
  y |= x << 16;
  y |= shape;
  y |= 0x10000000;
  *dst++ = y;
  *dst++ = tile | 0xe400;
  Oam_AddSpriteAtDepth(entry++, 255);

  if ((gFrameCount & 15) > 4) {
    shape = 0x40000000;
    actor = Actor_Lookup(gauge->opponentActorId);
    if (actor != 0) {
      x = ((actor->x.fixed - gauge->finishX) / 0xe0000) + 112;
      y = ((actor->z.fixed - gauge->finishZ) / 0xe0000) + gauge->level * 6 - 4;
      y &= 255;
      *dst++ = 0;
      y |= x << 16;
      y |= shape;
      *dst++ = y;
      *dst++ = (tile + 12) | 0xe400;
      Oam_AddSpriteAtDepth(entry++, 255);
    }
    actor = Actor_Lookup(gauge->playerActorId);
    if (actor != 0) {
      x = ((actor->x.fixed - gauge->finishX) / 0xe0000) + 112;
      y = ((actor->z.fixed - gauge->finishZ) / 0xe0000) + gauge->level * 6 - 4;
      y &= 255;
      *dst++ = 0;
      y |= x << 16;
      y |= shape;
      *dst++ = y;
      *dst++ = (tile + 8) | 0xe400;
      Oam_AddSpriteAtDepth(entry++, 255);
    }
  }
}
