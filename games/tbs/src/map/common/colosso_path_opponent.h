#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "callback_scheduler.h"
#include "colosso_gauge.h"
#include "resource.h"
#include "types.h"
#include "mem.h"
#include "script_layer.h"
#include "dma.h"
#include "vram_block.h"
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
  /// decoded from the scene's resource, or recorded (PATH_RECORDER_RECORD).
  s16 path[0x3840];
};

STATIC_ASSERT(sizeof(struct ColossoRaceWork) == 0x7170);

/// The opponent's scene callback. PATH_RECORDER_PLAY plays the path back: each
/// frame the opponent steps to the next point (mirrored across the finish when
/// asked), turning by at most a sixteenth of a turn, and stands once still for
/// three frames. At the path's end the opponent walks to the finish, beside the
/// first finisher's item actor or the second's, and sets FLAG_SCENE_TEMP_11 (a
/// racer has finished). PATH_RECORDER_RECORD records the actor's own path
/// instead, up to 0x383e halfwords, then plays it back with the opponent.
static void Colosso_UpdatePathOpponent(void) {
  struct ColossoRaceWork *work = (struct ColossoRaceWork *)gColossoWork;
  struct PathRecorder *rec = (struct PathRecorder *)gSceneState;
  struct FieldActor *obj;
  s32 x, z, d;
  u16 *itemActor;

  obj = Object_GetById(rec->actor);
  if (obj == NULL) {
    return;
  }
  if (rec->mode == PATH_RECORDER_PLAY) {
    x = work->path[(s16)rec->pathIndex++];
    z = work->path[(s16)rec->pathIndex++];
    if (x == 0 && z == 0) {
      s32 fz;
      rec->mode = PATH_RECORDER_FINISHED;
      Object_SetMode(obj, 1);
      if (work->gauge.finishX < obj->x.fixed) {
        x = work->gauge.finishX + 0xc0000;
      } else {
        x = work->gauge.finishX - 0xc0000;
      }
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_11)) {
        fz = work->gauge.finishZ + 0x100000;
        itemActor = &work->gauge.finishItemActorB;
      } else {
        fz = work->gauge.finishZ - 0x100000;
        itemActor = &work->gauge.finishItemActorA;
      }
      obj->timer = *itemActor;
      obj->acceleration = 0x4000;
      obj->speed = 0x10000;
      Engine_ObjectSetPosition(obj, x, 0, fz);
      Engine_GameFlagSet(FLAG_SCENE_TEMP_11);
      Engine_ObjectSetScript(obj, gColosso_OpponentFinishScript);
      return;
    }
    x <<= 16;
    z <<= 16;
    if (rec->mirror != 0) {
      x = work->gauge.finishX * 2 - x;
    }
    if (obj->x.fixed != x || obj->z.fixed != z) {
      {
        u16 angle = Math_Atan2(z - obj->z.fixed, x - obj->x.fixed);
        d = (s16)(angle - obj->facing);
      }
      if (d > 0x1000) {
        d = 0x1000;
      }
      if (d < -0x1000) {
        d = -0x1000;
      }
      obj->facing += d;
      obj->x.fixed = x;
      obj->z.fixed = z;
      rec->still = 0;
    } else {
      rec->still++;
    }
    if (rec->still > 2) {
      Object_SetMode(obj, 1);
    } else {
      Object_SetMode(obj, 5);
    }
  } else if (rec->mode == PATH_RECORDER_RECORD) {
    x = obj->x.part.pixel;
    z = obj->z.part.pixel;
    work->path[(s16)rec->pathIndex++] = x;
    work->path[(s16)rec->pathIndex++] = z;
    if ((s16)rec->pathIndex == 0x383e) {
      work->path[(s16)rec->pathIndex++] = 0;
      work->path[(s16)rec->pathIndex] = 0;
      rec->actor = work->gauge.opponentActor;
      rec->pathIndex = 0;
      rec->mode = PATH_RECORDER_PLAY;
    }
  }
}

extern u8 gColosso_GaugeGraphics[];

/// The race gauge's scene callback: a bar across the top of the screen, with
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
  struct FieldActor *actor;
  u32 shape;

  tile = gVramBlockCache[gauge->vramBlock].offset >> 5;
  segments = gauge->segments;
  if (gauge->hold != 0) {
    gauge->level = 2;
  } else if (Engine_GameFlagIsSet(FLAG_MENU_OPEN)) {
    if (gauge->level > 0) {
      gauge->level--;
    }
  } else if (gauge->level <= 1) {
    if (++gauge->level == 1) {
      Dma_Set(gColosso_GaugePalette, (void *)&OBJ_PLTT_COLOR(14, 0),
              (DMA_ENABLE << 16) | 0x10, REG_DMA3);
      buf = Runtime_CallAlternatePool(Runtime_BumpAllocateAlternatePool, 512);
      FieldResource_DecodeType01(gColosso_GaugeGraphics, buf);
      Engine_VramLoad(gauge->vramBlock, 512, buf);
      Runtime_BumpFree(buf);
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
  Runtime_PushSlotEntry(entry++, 255);
  for (i = 0; i < segments; i++) {
    x = 96 - i * 16;
    shape = 0x40000000;
    *dst++ = 0;
    *dst++ = (x << 16) | y | shape;
    *dst++ = (tile + 2) | 0xe400;
    Runtime_PushSlotEntry(entry++, 255);
  }
  x = 112;
  shape = 0x8000;
  *dst++ = 0;
  *dst++ = (x << 16) | y | shape;
  *dst++ = (tile + 6) | 0xe400;
  Runtime_PushSlotEntry(entry++, 255);
  x = 120;
  *dst++ = 0;
  *dst++ = (x << 16) | y | shape | 0x10000000;
  *dst++ = (tile + 6) | 0xe400;
  Runtime_PushSlotEntry(entry++, 255);
  for (i = 0; i < segments; i++) {
    x = 128 + i * 16;
    shape = 0x40000000;
    *dst++ = 0;
    *dst++ = y | (x << 16) | shape | 0x10000000;
    *dst++ = (tile + 2) | 0xe400;
    Runtime_PushSlotEntry(entry++, 255);
  }
  x = width + 128;
  shape = 0x8000;
  *dst++ = 0;
  y |= x << 16;
  y |= shape;
  y |= 0x10000000;
  *dst++ = y;
  *dst++ = tile | 0xe400;
  Runtime_PushSlotEntry(entry++, 255);

  if ((gFrameCount & 15) > 4) {
    shape = 0x40000000;
    actor = Engine_ActorLookup(gauge->opponentActor);
    if (actor != 0) {
      x = ((actor->x.fixed - gauge->finishX) / 0xe0000) + 112;
      y = ((actor->z.fixed - gauge->finishZ) / 0xe0000) + gauge->level * 6 - 4;
      y &= 255;
      *dst++ = 0;
      y |= x << 16;
      y |= shape;
      *dst++ = y;
      *dst++ = (tile + 12) | 0xe400;
      Runtime_PushSlotEntry(entry++, 255);
    }
    actor = Engine_ActorLookup(gauge->playerActor);
    if (actor != 0) {
      x = ((actor->x.fixed - gauge->finishX) / 0xe0000) + 112;
      y = ((actor->z.fixed - gauge->finishZ) / 0xe0000) + gauge->level * 6 - 4;
      y &= 255;
      *dst++ = 0;
      y |= x << 16;
      y |= shape;
      *dst++ = y;
      *dst++ = (tile + 8) | 0xe400;
      Runtime_PushSlotEntry(entry++, 255);
    }
  }
}
