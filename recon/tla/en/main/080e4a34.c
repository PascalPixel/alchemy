// Draft, differ 86 (104 before statements reordered 2026-10-26).
// Draft, differ 118 of 387: the sibling of Func_080e4730, in the same heap slot
// 0x5c work (0x192c bytes), run only when the owner's animation is kind
// 0x2020. The other objects of the owner's group pause; whether the owner
// stands where Func_080dbcd8 allows is kept at 0xd48 of the event work.
// Resource 0x1e1's 64 particles start with delays from -12, two frames apart;
// the owner spins and rises for 40 frames (cue 566 off the 32-frame beat),
// then faces 0x4000; one frame of it is drawn into the work and for 78 frames
// rippled (Func_080e446c) and copied to sprite VRAM while 33 particles play.
// On frame 60 it takes animation 251 with its sprite flags reset, its mode
// returns, the effect target is shown and followed and its height and motion
// flags are restored. Afterwards the party state's byte 530 becomes 9 (or 5
// unless the event mode is 3), the owner's action clears, 0xd4a becomes 3,
// and Func_080e42d4 runs as a callback unless 0xd48 was set, in which case
// Func_080e4730 follows.
// Left: the stack slots (the native keeps the saved height at sp 24, the
// saved flags at sp 20 and the flag pointer at sp 16, the entries at sp 32
// and 36), the 0xd48 flag's 1, which the native builds with movs where
// this loads it from the pool, and the frame loop's constants.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

extern u8 gPartyState[];
extern char ResourceId_DustPuffA;

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_080dc954(void);
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *excludedObject,
                                    s32 groupMode, s32 action);
s32 Func_080dbcd8(s32 x, s32 z, s32 terrain);
void BattleEffect_InitializeSharedScene(void);
void BattleEffect_SetObjectTickMode(struct ObjectRuntime *object, s32 mode);
void BattleEffect_PrepareBufferInterpolation(void);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
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
void Audio_PlayCue(s32 cue);
struct ObjectRuntime *Object_GetById(u32 actor);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
void Camera_FollowActor(s32 actor, s32 mode);

void Func_080e4a34(void) {
  u8 *work = Runtime_AllocateHeapBlock(92, 0x192c);
  u8 *effect = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *owner = *(struct ObjectRuntime **)(effect + 16);
  s32 savedY = owner->y;
  s32 particleEntry;
  s32 particleTiles;
  u8 *motionFlags = &owner->motionFlags;
  u8 savedFlags = *motionFlags;
  s32 pictureEntry;
  s32 pictureTiles;
  u8 *sprite;
  u8 *particle;
  u8 *animation;
  s32 ripple;
  s32 frame;
  s32 delay;
  s32 i;

  if (*(u16 *)((u8 *)owner->animation + 20) != 0x2020)
    goto end;
  Func_080dc954();
  ObjectGroup_SetActionForOthers(owner, 1, 0);
  if (Func_080dbcd8(owner->x, owner->z, owner->terrainId) == 0)
    *(s16 *)(event + 0xd48) = 1;
  else
    *(s16 *)(event + 0xd48) = 0;
  BattleEffect_InitializeSharedScene();
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                        work);
  particleEntry = Resource_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 0x800, work);
  particle = work + 0x1200;
  delay = -12;
  sprite = work + 0x800;
  for (i = 0; i < 64; i++) {
    Func_080e45a8(sprite, particle, i, particleTiles, *(s16 *)(event + 0xd48));
    *(s32 *)(particle + 24) = delay;
    sprite += 40;
    delay -= 2;
    particle += 28;
  }
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  BattleEffect_SetObjectTickMode(*(struct ObjectRuntime **)(effect + 16), 0);
  Object_SetPositionAndResetMotionFar(owner, owner->x, owner->y, owner->z);
  *motionFlags = 0;
  WaitFrames(2);
  for (frame = 0; frame < 40; frame++) {
    if (frame > 0)
      owner->y += 0x3999;
    if (frame & 31)
      Audio_PlayCue(566);
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
      Audio_PlayCue(136);
    if (frame == 60) {
      Animation_SetIndexAndInitializeObjectsFar(owner, 251);
      animation = owner->animation;
      animation[5] &= 63;
      animation = owner->animation;
      animation[7] = (animation[7] & 63) | 64;
      Object_SetMode(owner, 1);
      ObjectDispatch_SetSingleChildFlagsFar(
          Object_GetById(*(s16 *)(effect + 24)), 0);
      Camera_FollowActor(*(s16 *)(effect + 24), 1);
      owner->y = savedY;
      *motionFlags = savedFlags;
    }
    if (frame == 61)
      Animation_ApplyChildValueToFirstFar(owner->animation, 0);
    WaitFrames(1);
    ripple += 3;
  }
  if (*(s8 *)(event + 394) == 3)
    gPartyState[530] = 9;
  else
    gPartyState[530] = 5;
  owner->action = 0;
  owner->unk66 = 0;
  *(s16 *)(event + 0xd4a) = 3;
  gPartyState[620] = 1;
  if (*(s16 *)(event + 0xd48) == 0)
    Scheduler_AddOrUpdateCallback(Func_080e42d4, 0x480);
  ObjectGroup_SetActionForOthers(owner, 0, 16);
  Resource_ResetEntry(pictureEntry);
  Resource_ResetEntry(particleEntry);
  BattleEffect_PrepareBufferInterpolation();
end:
  Runtime_ReleaseHeapBlock(92);
  if (*(s16 *)(event + 0xd48) != 0)
    Func_080e4730();
}
