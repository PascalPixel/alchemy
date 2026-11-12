#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "map.h"
#include "event_work.h"
#include "actor.h"
#include "iwram_call.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct WorldCamera {
  u8 unk000[0x11e];
  u16 yaw;
};

struct Unk0802a124Actor {
  u8 unk00[0x62];
  u8 unk62;
  u8 unk63;
};

struct PlayerSprite {
  u8 unk00[9];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[0x10];
  u8 flags;
  u8 unk1b[0x11];
  u8 *entry;
};

// The heading of each direction-pad combination, -1 for none.
extern const s16 gData_0802ec5c[16];
// The camera yaw each direction-pad combination turns toward.
extern const s32 gData_0802eca0[16];
extern const s32 gData_0802ece0[];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_0802db64(struct Actor *object, struct FieldPosition *position);
void Actor_SelectAnimation(struct Actor *object, s32 argument);
s32 Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
s32 FixedSqrt(s32 value);
struct Actor *Func_08023220(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *, const s32 *);
s32 Animation_ApplyChildArgument(struct PlayerSprite *, s32);
s32 Func_08029838(struct Actor *object);

/// Script command 0x22: moves the object on the world map by the direction pad
/// at a fixed pace (faster while L repeats), trying six turned headings when
/// the way is blocked, as Func_080279b0 does; records the held keys in the
/// event work and leaves an object of kind 0x164 behind it every fifth step.
s32 Func_0802a124(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition leftPosition;
  struct FieldPosition rightPosition;
  s16 angleOffsets[22];
  s32 collision;
  s32 angle;
  u16 direction;
  s32 angleIndex;
  s32 count;
  s32 difference;
  s32 y;
  s32 terrainCollision;
  s32 speed;
  s32 targetCameraYaw;
  struct WorldCamera *camera;
  s32 yawStep;
  struct PlayerSprite *playerSprite;
  struct Actor *ripple;
  struct PlayerSprite *rippleSprite;
  struct EventWork *work;
  u8 *timer;

  collision = 0;
  count = 0;
  object->unk66 = collision;
  object->action = 1;
  object->speedLimit = 0x8000;
  object->acceleration = 0x4000;
  if (gInput.repeat & 0x200)
    object->speedLimit = 0x40000;

  angle = gData_0802ec5c[(gInput.held >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, direction, &position);
    if (gDebugMode == 0 || (gInput.held & 0x200) == 0) {
      leftPosition.x = object->x;
      leftPosition.y = object->y;
      leftPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction + 0x1555, &leftPosition);
      rightPosition.x = object->x;
      /* FAKEMATCH(temporary): y read into its own local before the block; as
       * object->y read in the block, 2 of 470 instructions differ in tla-en. */
      y = object->y;
      // The right probe's y stored in a once-only block; as the plain store,
      // 50 of 470 instructions differ in tla-en.
      do {
        rightPosition.y = y;
      } while (0);
      rightPosition.z = object->z;
      Vector_AddPolarOffset(0x80000, direction - 0x1555, &rightPosition);
      if ((Func_0802db64(object, &position) |
           Func_0802db64(object, &leftPosition) |
           Func_0802db64(object, &rightPosition)) != 0) {
        angleOffsets[count++] = direction + 0x1000;
        angleOffsets[count++] = direction - 0x1000;
        angleOffsets[count++] = direction + 0x2000;
        angleOffsets[count++] = direction - 0x2000;
        angleOffsets[count++] = direction + 0x3000;
        angleOffsets[count++] = direction - 0x3000;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          position.x = object->x;
          position.y = object->y;
          position.z = object->z;
          Vector_AddPolarOffset(0x80000, direction, &position);
          leftPosition.x = object->x;
          leftPosition.y = object->y;
          leftPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction + 0x1555, &leftPosition);
          rightPosition.x = object->x;
          rightPosition.y = object->y;
          rightPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, direction - 0x1555, &rightPosition);
          if ((Func_0802db64(object, &position) |
               Func_0802db64(object, &leftPosition) |
               Func_0802db64(object, &rightPosition)) != 0)
            continue;
          angle = (s16)direction;
          goto update_object;
        }
        collision |= 1;
      }
    }
  }

update_object:
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if ((collision & 3) != 0)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  Actor_SelectAnimation(object, 1);

  if (collision != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & 3) != 0) {
      direction = angle;
      difference = (s16)(direction - object->angle);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->angle += difference;
    }
    timer = (u8 *)object + 98;
    *timer = 0;
    object->unk66 = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&object->velocityX);
    timer = (u8 *)object + 98;
    if (*timer != 0)
      (*timer)--;
  }

  camera = (struct WorldCamera *)Ram_HeapSlots->mapWork;
  targetCameraYaw = gData_0802eca0[(gInput.held >> 4) & 15];
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > 0x200)
    yawStep = 0x200;
  if (yawStep < -0x200)
    yawStep = -0x200;
  if (yawStep > -16 && yawStep < 16)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  if (object->animationKind == 1) {
    playerSprite = object->animation;
    terrainCollision = GetWorldMapCollision((struct WorldPosition *)&object->x);
    if (terrainCollision == 9)
      playerSprite->entry[6] = 1;
    else
      playerSprite->entry[6] = 9;
  }
  if (*timer == 0 && collision == 0) {
    ripple = Func_08023220(0x164, object->x, object->y, object->z - 0x20000);
    if (ripple != 0) {
      ripple->terrainHeight = object->terrainHeight;
      rippleSprite = ripple->animation;
      Actor_StartScript(ripple, gData_0802ece0);
      ripple->priorityFlags = 8;
      ((struct Unk0802a124Actor *)ripple)->unk63 = 100;
      ripple->motionFlags = collision;
      if (rippleSprite != 0) {
        Animation_ApplyChildArgument(rippleSprite, 1);
        rippleSprite->flags = collision;
        rippleSprite->priority = 2;
        rippleSprite->priority = 1;
      }
      *timer = 5;
    }
  }
  Func_08029838(object);
  object->step++;
  return 1;
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"

void SpriteColor_RemapPixels(u8 *buf, s32 cnt) {
  s32 n;
  u8 *p;
  u8 *table;

  p = buf;
  table = gSpriteColorRemapTables;
  n = cnt - 1;
  if (n != -1) {
    do {
      u32 index = *p;

      n -= 1;
      *p = table[index];
      p += 1;
    } while (n != -1);
  }
}

// Map: unpack decoded block data into gMapBlocks.

extern u32 gMapBlocks[];
#include "heap_release.h"
#include "ram_code.h"
extern u8 gMapCellBuffer[];

/// Decoded buffer [1] selects the method; halfword data starts at [2].
/// size: length including the method byte (even means no data).
/// Method 0: copy unchanged.
/// Method 1: high bytes followed by low bytes; reconstruct by XOR with prior
/// values. Method 2: reconstruct by XOR with prior values.
void Tilemap_DecodeStagedBuffer(s32 size) {
  u16 *dst;
  u16 *src;
  s32 n;
  s32 i;

  src = (u16 *)(Ram_MapCellBuffer + 2);
  dst = (u16 *)gMapBlocks;
  n = (size - 1) / 2;
  if (size & 1) {
    switch (((u8 *)gMapCellBuffer)[1]) {
    case 0:
      for (i = 0; i < n; i++)
        *dst++ = *src++;
      break;
    case 1: {
      u32 prev = 0;
      u8 *hi = Ram_MapCellBuffer + 2;
      u8 *lo = hi + n;

      for (i = 0; i < n; i++) {
        u32 v = ((*hi++ << 8) | *lo++) ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    case 2: {
      s32 prev = 0;

      for (i = 0; i < n; i++) {
        s32 v = *src++ ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    }
  }
}

typedef void (*ConvertFn)(void *destination, void *source, void *saved);

extern u8 Render_ConvertMapCode[];
extern u8 Render_ConvertMapCodeSize[];

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): the copy called through this inline, so the map cell
   * buffer's address is loaded again for the conversion; as Iwram_CopyWords
   * called directly, which keeps the address in a saved register, 20 of 40
   * instructions of Tilemap_ConvertBuffer differ in tla-en. */
  return Iwram_CopyWordsEntry(destination, source, size);
}

/// Converts the map cell buffer's 32 KB in place with the ARM routine copied
/// to RAM, keeping a copy of the buffer for it to read.
void Tilemap_ConvertBuffer(void) {
  u8 *saved;
  ConvertFn routine;
  u32 size;

  size = 0x8000;
  saved = Heap_AllocEwram(size);
  CopyWords(saved, Ram_MapCellBuffer, size);
  RamCode_Load(routine, Render_ConvertMapCode, Render_ConvertMapCodeSize);
  routine(((u8 *)gMapCellBuffer + 0x8000), Ram_MapCellBuffer, saved);
  Heap_Release(routine);
  Heap_Release(saved);
}

#include "resource.h"
#include "dma.h"
#include "io_reg.h"

extern const u16 gData_0802f380[][6];
extern char ResourceId_Map000;

/// Decodes the palette resource of a map's header row (its second halfword)
/// and copies its 224 colors to destination, keeping the current backdrop
/// color in the first entry.
void Func_0802a650(s32 map, void *destination) {
  const u16 *entry;
  u16 *buffer;
  s16 backdrop;

  entry = gData_0802f380[map];
  buffer = Heap_Alloc(0x200);
  backdrop = *(s16 *)0x05000000;
  Resource_DecodeType01(
      Resource_GetTableEntry(entry[1] + (u32)&ResourceId_Map000), buffer);
  buffer[0] = backdrop;
  Dma_Set(buffer, destination, DMA_COPY_WORDS(112), REG_DMA3);
  Heap_Release(buffer);
}
