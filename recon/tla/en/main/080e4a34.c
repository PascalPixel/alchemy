// Near miss, differ 40 of 387 (86 before; 2026-10-26): the event work's
// fields are read through a struct (through a cast pointer the 0xd48 flag's
// 1 and 0 came from the pool), the effect work is BattleFxScene, the party
// state's bytes are renderMode and pendingThird, and the locals are declared
// in the order that gives the native's stack slots. Left: instruction order
// (the 0x2020 compare's constant, the loops' counters set after their
// pointers here, the frame loop's opening constants, the animation byte's
// copy after the owner's reload).
// Earlier: differ 118 of 387: the sibling of Func_080e4730, in the same heap slot
// 0x5c work (0x192c bytes), run only when the owner's animation is kind
// 0x2020. The other objects of the owner's group pause; whether the owner
// stands where Func_080dbcd8 allows is kept at 0xd48 of the event work.
// Resource 0x1e1's 64 particles start with delays from -12, two frames apart;
// the owner spins and rises for 40 frames (cue 566 off the 32-frame beat),
// then faces 0x4000; one frame of it is drawn into the work and for 78 frames
// rippled (Func_080e446c) and copied to sprite VRAM while 33 particles play.
// On frame 60 it takes animation 251 with its sprite flags reset, its mode
// returns, the effect target is shown and followed and its height and motion
// flags are restored. Afterwards the adventure's renderMode becomes 9 when the
// event mode is 3, else 5, the owner's action clears, 0xd4a becomes 3,
// and Func_080e42d4 runs as a callback unless 0xd48 was set, in which case
// Func_080e4730 follows.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "fx_scene.h"
#include "adventure.h"

extern char ResourceId_DustPuffA;

struct GrabEventWork {
  u8 unk000[394];
  s8 mode;
  u8 unk18b[0xd48 - 395];
  s16 standing;
  s16 phase;
};

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_080dc954(void);
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *excludedObject,
                                    s32 groupMode, s32 action);
s32 Func_080dbcd8(s32 x, s32 z, s32 terrain);
void BattleEffect_InitializeSharedScene(void);
void BattleEffect_SetObjectUpdateMode(struct ObjectRuntime *object, s32 mode);
void BattleEffect_PrepareBufferInterpolation(void);
void Task_Add(void (*callback)(void), s32 order);
void Func_080e42d4(void);
void Func_080e4730(void);
void *Resource_GetTableEntry(s32 resourceId);
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
void Sound_PlayFar(s32 cue);
struct ObjectRuntime *Object_GetById(u32 actor);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
void Camera_FollowActor(s32 actor, s32 mode);

void Func_080e4a34(void) {
  u8 *work = Runtime_AllocateHeapBlock(92, 0x192c);
  struct BattleFxScene *effect = Ram_HeapSlots->effectWork;
  struct GrabEventWork *event = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *owner;
  s32 pictureEntry;
  s32 particleEntry;
  s32 particleTiles;
  s32 savedY;
  u8 savedFlags;
  u8 *motionFlags;
  s32 pictureTiles;
  u8 *sprite;
  u8 *particle;
  u8 *animation;
  s32 ripple;
  s32 frame;
  s32 delay;
  s32 i;

  owner = effect->mainObject;
  savedY = owner->y;
  motionFlags = &owner->motionFlags;
  savedFlags = *motionFlags;
  if (*(u16 *)((u8 *)owner->animation + 20) != 0x2020)
    goto end;
  Func_080dc954();
  ObjectGroup_SetActionForOthers(owner, 1, 0);
  if (Func_080dbcd8(owner->x, owner->z, owner->terrainId) == 0)
    event->standing = 1;
  else
    event->standing = 0;
  BattleEffect_InitializeSharedScene();
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                        work);
  particleEntry = Resource_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 0x800, work);
  particle = work + 0x1200;
  delay = -12;
  sprite = work + 0x800;
  for (i = 0; i < 64; i++) {
    Func_080e45a8(sprite, particle, i, particleTiles, event->standing);
    *(s32 *)(particle + 24) = delay;
    sprite += 40;
    delay -= 2;
    particle += 28;
  }
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  BattleEffect_SetObjectUpdateMode(effect->mainObject, 0);
  Object_SetPositionAndResetMotionFar(owner, owner->x, owner->y, owner->z);
  *motionFlags = 0;
  WaitFrames(2);
  for (frame = 0; frame < 40; frame++) {
    if (frame > 0)
      owner->y += 0x3999;
    if (frame & 31)
      Sound_PlayFar(566);
    owner->angle += 0x2000;
    WaitFrames(1);
  }
  owner->angle = 0x4000;
  WaitFrames(1);
  Func_080e43a4(*(void **)((u8 *)owner->animation + 40), work, 0x400);
  pictureEntry = Resource_FindFreeEntry();
  pictureTiles = VramCache_Load(pictureEntry, 0x400, NULL);
  Func_080e4510(work + 0x1900, pictureTiles);
  ripple = 45;
  for (frame = 15; frame <= 92; frame++) {
    Func_080e446c(work, work + 0x400, ripple / 2);
    ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
        work + 0x400, 32, 32, (u8 *)0x06010000 + pictureTiles * 32);
    Func_080e45a0(work + 0x1900);
    particle = work + 0x1200;
    sprite = work + 0x800;
    for (i = 0; i <= 32; i++) {
      Func_080e46ac(sprite, particle, i, particleTiles);
      sprite += 40;
      particle += 28;
    }
    if (frame == 16)
      Animation_ApplyChildValueToFirstFar(owner->animation, 15);
    if (frame == 45)
      Sound_PlayFar(136);
    if (frame == 60) {
      Animation_SetIndexAndInitializeObjectsFar(owner, 251);
      animation = owner->animation;
      animation[5] &= 63;
      animation = owner->animation;
      animation[7] = (animation[7] & 63) | 64;
      Object_SetMode(owner, 1);
      ObjectDispatch_SetSingleChildFlagsFar(
          Object_GetById(effect->firstObjectId), 0);
      Camera_FollowActor(effect->firstObjectId, 1);
      owner->y = savedY;
      *motionFlags = savedFlags;
    }
    if (frame == 61)
      Animation_ApplyChildValueToFirstFar(owner->animation, 0);
    WaitFrames(1);
    ripple += 3;
  }
  if (event->mode == 3)
    gAdventure.renderMode = 9;
  else
    gAdventure.renderMode = 5;
  owner->action = 0;
  owner->unk66 = 0;
  event->phase = 3;
  gAdventure.pendingThird = 1;
  if (event->standing == 0)
    Task_Add(Func_080e42d4, 0x480);
  ObjectGroup_SetActionForOthers(owner, 0, 16);
  Resource_ResetEntry(pictureEntry);
  Resource_ResetEntry(particleEntry);
  BattleEffect_PrepareBufferInterpolation();
end:
  Runtime_ReleaseHeapBlock(92);
  if (event->standing != 0)
    Func_080e4730();
}
