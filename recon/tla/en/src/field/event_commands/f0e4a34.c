#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "field_psynergy_work.h"
#include "main_routine_work.h"
#include "system/io_reg.h"

extern char ResourceId_DustPuffA;

struct GrabEventWork {
  u8 unk000[394];
  s8 mode;
  u8 unk18b[0xd48 - 395];
  s16 standing;
  s16 phase;
};

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Func_080dc954(void);
void Actor_SetOthersPaused(struct Actor *excludedActor, s32 groupMode,
                           s32 action);
s32 Func_080dbcd8(s32 x, s32 z, s32 terrain);
void BattleEffect_InitializeSharedScene(void);
void BattleEffect_SetObjectUpdateMode(struct Actor *actor, s32 mode);
void BattleEffect_PrepareBufferInterpolation(void);
void Task_Add(void (*func)(void), s32 order);
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
void Actor_SetPositionAndResetMotionFar(struct Actor *actor, s32 x, s32 y,
                                        s32 z);
void Animation_SetIndexAndInitializeObjectsFar(struct Actor *actor, s32 index);
void Animation_ApplyChildValueToFirstFar(void *animation, s32 value);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);
struct Actor *Actor_GetById(u32 actor);
void Actor_SetSingleChildFlagsFar(struct Actor *actor, s32 flags);
void Camera_FollowActor(s32 actor, s32 mode);

void Func_080e4a34(void) {
  u8 *work = HeapSlot_Alloc(92, 0x192c);
  struct BattleFxScene *effect = Ram_HeapSlots->effectWork;
  struct GrabEventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *owner;
  s32 pictureEntry;
  s32 particleEntry;
  s32 particleTiles;
  s32 savedY;
  u8 savedFlags;
  u8 *motionFlags;
  s32 pictureTiles;
  u8 *sprite;
  u8 *particle;
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
  Actor_SetOthersPaused(owner, 1, 0);
  if (Func_080dbcd8(owner->x, owner->z, owner->terrainId) == 0)
    event->standing = 1;
  else
    event->standing = 0;
  BattleEffect_InitializeSharedScene();
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                        work);
  particleEntry = Resource_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 0x800, work);
  i = 0;
  particle = work + 0x1200;
  delay = -12;
  sprite = work + 0x800;
  for (; i < 64; i++) {
    Func_080e45a8(sprite, particle, i, particleTiles, event->standing);
    *(s32 *)(particle + 24) = delay;
    sprite += 40;
    delay -= 2;
    particle += 28;
  }
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  BattleEffect_SetObjectUpdateMode(effect->mainObject, 0);
  Actor_SetPositionAndResetMotionFar(owner, owner->x, owner->y, owner->z);
  *motionFlags = 0;
  wait(2);
  for (frame = 0; frame < 40; frame++) {
    if (frame > 0)
      owner->y += 0x3999;
    if (frame & 31)
      Sound_PlayFar(SPLIT_CONSTANT(SPLIT_CONSTANT(0x200, 54)));
    owner->angle += 0x2000;
    wait(1);
  }
  owner->angle = 0x4000;
  wait(1);
  Func_080e43a4(*(void **)((u8 *)owner->animation + 40), work, 0x400);
  pictureEntry = Resource_FindFreeEntry();
  pictureTiles = VramCache_Load(pictureEntry, 0x400, NULL);
  Func_080e4510(work + 0x1900, pictureTiles);
  frame = 15;
  for (; frame <= 92; frame++) {
    Func_080e446c(work, work + 0x400, frame * 3 / 2);
    ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
        work + 0x400, 32, 32, (u8 *)OBJ_VRAM0 + pictureTiles * 32);
    Func_080e45a0(work + 0x1900);
    i = 0;
    particle = work + 0x1200;
    sprite = work + 0x800;
    for (; i <= 32; i++) {
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
      ((u8 *)owner->animation)[5] &= 63;
      ((u8 *)owner->animation)[7] = (((u8 *)owner->animation)[7] & 63) | 64;
      Actor_SetMode(owner, 1);
      Actor_SetSingleChildFlagsFar(Actor_GetById(effect->firstObjectId), 0);
      Camera_FollowActor(effect->firstObjectId, 1);
      owner->y = savedY;
      *motionFlags = savedFlags;
    }
    if (frame == 61)
      Animation_ApplyChildValueToFirstFar(owner->animation, 0);
    wait(1);
  }
  if (event->mode == 3)
    gMainRoutineWork.renderMode = 9;
  else
    gMainRoutineWork.renderMode = 5;
  owner->action = 0;
  owner->unk66 = 0;
  event->phase = 3;
  gMainRoutineWork.pendingThird = 1;
  if (event->standing == 0)
    Task_Add(Func_080e42d4, 0x480);
  Actor_SetOthersPaused(owner, 0, 16);
  Resource_ResetEntry(pictureEntry);
  Resource_ResetEntry(particleEntry);
  BattleEffect_PrepareBufferInterpolation();
end:
  HeapSlot_Release(92);
  if (event->standing != 0)
    Func_080e4730();
}
