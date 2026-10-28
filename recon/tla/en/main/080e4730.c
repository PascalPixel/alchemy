// Near miss, differ 163 (244 before statements reordered 2026-10-26) of 332: the native keeps resource 0x1e1 in the pool,
// hoists the picture's VRAM offset (tiles << 5) out of the frame loop and
// keeps the effect pointer and the frame counter in other registers; the
// rest follows from those.
// An event command in the heap slot 0x5c work (0x192c bytes). The
// other objects of the effect owner's group pause, the block cue callback
// stops, and resource 0x1e1's 64 particles (Func_080e45a8) start with delays
// two frames apart. The owner is raised 10 units, takes the leader's
// animation with its sprite flags reset and faces 0x4000; one frame of it is
// drawn into the work and, for 65 of 78 frames, rippled (Func_080e446c) and
// copied to sprite VRAM while 30 of the particles play (Func_080e46ac). On
// frame 66 the owner's mode returns, the effect target is shown and followed,
// cue 539 plays, the owner's motion flags and Data_02000452 are set by the
// event mode, and with no pending point the effect's event runs.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

struct PendingEvent;

extern u8 gAdventure[];
extern u8 Data_02000452;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Func_080dc954(void);
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *excludedObject,
                                    s32 groupMode, s32 action);
s32 Task_Remove(void (*callback)(void));
void Func_080e42d4(void);
void *Resource_GetTableEntry(u16 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Resource_ResetEntry(u32 slot);
void Func_080e45a8(void *sprite, void *particle, s32 index, s32 tiles,
                   s32 high);
void Func_080e4510(void *sprite, s32 tiles);
void Func_080e446c(u8 *source, u8 *destination, s32 phase);
void Func_080e45a0(void *sprite);
void Func_080e46ac(void *sprite, void *particle, s32 index, s32 tiles);
void Func_080e43a4(void *source, void *buffer, s32 size);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
void Animation_SetIndexAndInitializeObjectsFar(struct ObjectRuntime *object,
                                         s32 index);
void Animation_ApplyChildValueToFirstFar(void *animation, s32 value);
void WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(u32 actor);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
void Camera_FollowActor(s32 actor, s32 mode);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode,
                                 s32 argument);

void Func_080e4730(void) {
  u8 *work = HeapSlot_Alloc(92, 0x192c);
  u8 *effect = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 pictureEntry;
  struct ObjectRuntime *owner = *(struct ObjectRuntime **)(effect + 16);
  s32 particleEntry;
  s32 particleTiles;
  u8 *motionFlags;
  u8 *sprite;
  u8 *particle;
  u8 *animation;
  u8 *party;
  s32 ripple;
  s32 pictureTiles;
  s32 frame;
  s32 i;
  s32 delay;

  Func_080dc954();
  gAdventure[620] = 0;
  ObjectGroup_SetActionForOthers(owner, 1, 0);
  Task_Remove(Func_080e42d4);
  Resource_DecodeType01(Resource_GetTableEntry(0x1e1), work);
  particleEntry = Resource_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 0x800, work);
  particle = work + 0x1200;
  delay = 0;
  sprite = work + 0x800;
  for (i = 0; i < 64; i++) {
    Func_080e45a8(sprite, particle, i, particleTiles, *(s16 *)(event + 0xd48));
    *(s32 *)(particle + 24) = delay;
    sprite += 40;
    delay -= 2;
    particle += 28;
  }
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  motionFlags = &owner->motionFlags;
  *motionFlags = 0;
  Object_SetPositionAndResetMotionFar(owner, owner->x, owner->y + 0xa0000,
                                      owner->z);
  party = gAdventure;
  Animation_SetIndexAndInitializeObjectsFar(owner, *(s32 *)(party + 532));
  animation = owner->animation;
  animation[5] &= 63;
  animation = owner->animation;
  animation[7] = (animation[7] & 63) | 128;
  if (*(s8 *)(event + 394) == 3)
    *(s32 *)((u8 *)owner->animation + 12) = 0xc000;
  owner->angle = 0x4000;
  Object_SetMode(owner, 0);
  Animation_ApplyChildValueToFirstFar(owner->animation, 15);
  WaitFrames(1);
  Animation_ApplyChildValueToFirstFar(owner->animation, 0);
  Func_080e43a4(*(void **)((u8 *)owner->animation + 40), work, 0x400);
  Animation_ApplyChildValueToFirstFar(owner->animation, 15);
  pictureEntry = Resource_FindFreeEntry();
  pictureTiles = VramCache_Load(pictureEntry, 0x400, NULL);
  Func_080e4510(work + 0x1900, pictureTiles);
  WaitFrames(1);
  ripple = 0;
  for (frame = 0; frame < 78; frame++) {
    if (frame == 0)
      Sound_PlayFar(544);
    if (frame <= 64) {
      Func_080e446c(work, work + 0x400, 92 - ripple / 2);
      ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
          work + 0x400, 32, 32, (u8 *)0x06010000 + pictureTiles * 32);
      Func_080e45a0(work + 0x1900);
    }
    if (frame == 66) {
      Object_SetMode(owner, 1);
      Animation_ApplyChildValueToFirstFar(owner->animation, 0);
      ObjectDispatch_SetSingleChildFlagsFar(
          Object_GetById(*(s16 *)(effect + 24)), 1);
      Camera_FollowActor(*(s16 *)(effect + 24), 1);
      Sound_PlayFar(539);
      if (*(s8 *)(event + 394) == 3) {
        Data_02000452 = 1;
        *motionFlags = 2;
      } else {
        Data_02000452 = 0;
        *motionFlags = 3;
      }
      if (*(s16 *)(event + 0xd48) == 0)
        Field_RunPsynergyEvent(
            Func_080ce458(0x40000005, *(s16 *)(effect + 30),
                          Func_080ce31c(*(s16 *)(effect + 30))),
            *(s16 *)(effect + 24), -1);
    }
    particle = work + 0x1200;
    sprite = work + 0x800;
    for (i = 0; i < 30; i++) {
      Func_080e46ac(sprite, particle, i, particleTiles);
      sprite += 40;
      particle += 28;
    }
    WaitFrames(1);
    ripple += 3;
  }
  ObjectGroup_SetActionForOthers(owner, 0, 16);
  Resource_ResetEntry(pictureEntry);
  Resource_ResetEntry(particleEntry);
  HeapSlot_Release(92);
}
