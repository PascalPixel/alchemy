// Draft, differ 277 of 260 (was 279, i now u32 as the native's bhi shows):
// shows the world map's placed objects that come within the view and whose
// flags allow them, hides those that leave it, and starts a placement's
// event when the leader touches it. Overlay 64c has the same code (its
// Func_02000d64). Left: the native spills the four view bounds and i to the
// stack, loads the first placement's kind and gData_02006c08 before the
// bounds' if (so in the block before it) and keeps the work pointer in r9;
// it reaches gPartyState's fields from its address plus a built offset (a
// rematerialized &gPartyState, as from a local pointer under pressure).
// Register allocation differs throughout. Tried: slot initialized at its
// declaration (281), a local party pointer (289).
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "event_work.h"
#include "iwram_call.h"

/// One of the scene's placed objects, 20 bytes; a zero kind ends the list.
struct WorldMapPlacement {
  u8 kind;
  u8 unk01; ///< 15 skips it
  s16 sprite;
  s16 x; ///< pixels
  s16 z; ///< pixels
  u16 angle;
  s16 flag; ///< -1 always shown; bit 12 shown while set, else while clear
  u8 unk0c[8];
};

struct WorldMapSprite {
  u8 unk00[0x0c];
  s32 scale;
};

extern struct ObjectRuntime *gData_02006c08[];
extern u8 gData_02006bd4[];

struct WorldMapPlacement *Func_02004e08(void);
s32 Func_02004e10(void);
struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Func_02004b30(s32 sprite, s32 x, s32 y, s32 z);
void Func_02004b20(struct ObjectRuntime *object, void *dispatch);
void Func_02004b18(struct ObjectRuntime *object, s32 mode);
void Func_02004b38(struct ObjectRuntime *object);

void Func_020036a8(void) {
  struct WorldMapPlacement *placement = Func_02004e08();
  struct ObjectRuntime *leader = Object_GetById(Func_02004e10());
  s32 leaderReach =
      leader->radius * ((struct WorldMapSprite *)leader->animation)->scale;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *center = work->viewCenter;
  struct ObjectRuntime **slot;
  s32 x = center->x;
  s32 z = center->z;
  s32 left, right, top, bottom;
  u32 i;

  if (*(s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x960) <= 0x10000) {
    left = x - 0xa00000;
    right = x + 0xa00000;
    top = z - 0x12c0000;
    bottom = z + 0xc80000;
  } else {
    left = x - 0xf00000;
    right = x + 0xf00000;
    top = z - 0x1c20000;
    bottom = z + 0x12c0000;
  }
  slot = gData_02006c08;
  for (i = 0; i < 128 && placement->kind != 0; i++, slot++, placement++) {
    struct ObjectRuntime *object;
    s32 px, pz;

    if (placement->unk01 == 15)
      continue;
    object = *slot;
    px = placement->x << 16;
    pz = placement->z << 16;
    if (px > left && px < right && pz > top && pz < bottom) {
      s32 shown = 1;

      if (placement->flag != -1) {
        if (placement->flag & 0x1000)
          shown = GameFlag_Test(placement->flag);
        else
          shown = !GameFlag_Test(placement->flag);
      }
      if (shown) {
        if (object == NULL) {
          struct WorldMapSprite *sprite;

          object = Func_02004b30(placement->sprite, px, 0, pz);
          if (object == NULL)
            continue;
          Func_02004b20(object, gData_02006bd4);
          object->motionFlags = 0;
          object->collisionFlags = 1;
          object->angle = placement->angle;
          Func_02004b18(object, 1);
          sprite = object->animation;
          sprite->scale = Iwram_MulQ16(sprite->scale, 0xc000);
          *slot = object;
        }
        if (gPartyState.ownerOverride == 0) {
          s32 scale = ((struct WorldMapSprite *)object->animation)->scale;
          s32 dx = object->x - leader->x;
          s32 reach, dz;

          if (dx < 0)
            dx = leader->x - object->x;
          reach = leaderReach + object->radius * scale;
          dz = object->z - leader->z;
          if (dz >= 0 ? dx + dz < reach
                      : dx + (leader->z - object->z) < reach) {
            if (GameFlag_Test(0x104) == 0) {
              if (gPartyState.renderMode == 9)
                work->unk166 = 0x208b;
              else
                work->exit = i + 100;
            }
          }
        }
        continue;
      }
    }
    if (object != NULL) {
      Func_02004b38(object);
      *slot = NULL;
    }
  }
}
