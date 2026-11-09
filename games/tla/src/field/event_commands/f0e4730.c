#include "types.h"
#include "actor.h"
#include "ram_buffer.h"

struct PendingEvent;

extern u8 gMainRoutineWork[];
extern u8 Data_02000452;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Func_080dc954(void);
void Actor_SetOthersPaused(struct Actor *excludedObject, s32 groupMode,
                           s32 action);
s32 Task_Remove(void (*func)(void));
void Func_080e42d4(void);
void *Resource_GetTableEntry(s32 resourceId);
extern char ResourceId_DustPuffA;
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
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x, s32 y,
                                        s32 z);
void Animation_SetIndexAndInitializeObjectsFar(struct Actor *object, s32 index);
void Animation_ApplyChildValueToFirstFar(void *animation, s32 value);
void wait(s32 frames);
struct Actor *Actor_GetById(u32 actor);
void Actor_SetSingleChildFlagsFar(struct Actor *object, s32 flags);
void Camera_FollowActor(s32 actor, s32 mode);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 argument);

void Func_080e4730(void) {
  u8 *work = HeapSlot_Alloc(92, 0x192c);
  u8 *effect = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 pictureEntry;
  struct Actor *owner = *(struct Actor **)(effect + 16);
  s32 particleEntry;
  s32 particleTiles;
  u8 *motionFlags;
  u8 *rippled;

  s32 sprite;
  u8 *particle;
  u8 *particleSprite;
  u8 *animation;
  u8 *party;
  s32 pictureOffset;
  s32 ripple;
  s32 frame;
  s32 i;
  s32 delay;
  s32 mask;

  Func_080dc954();
  gMainRoutineWork[620] = 0;
  Actor_SetOthersPaused(owner, 1, 0);
  Task_Remove(Func_080e42d4);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                        work);
  particleEntry = Resource_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 0x800, work);
  i = 0;
  particle = work + 0x1200;
  delay = 0;
  particleSprite = work + 0x800;
  for (; i < 64; i++) {
    Func_080e45a8(particleSprite, particle, i, particleTiles,
                  *(s16 *)(event + 0xd48));
    *(s32 *)(particle + 24) = delay;
    particleSprite += 40;
    delay -= 2;
    particle += 28;
  }
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  motionFlags = &owner->motionFlags;
  *motionFlags = 0;
  // Set before the move: 63 is held in a saved register across both calls.
  mask = 63;
  Actor_SetPositionAndResetMotionFar(owner, owner->x, owner->y + 0xa0000,
                                     owner->z);
  party = gMainRoutineWork;
  Animation_SetIndexAndInitializeObjectsFar(owner, *(s32 *)(party + 532));
  ((u8 *)owner->animation)[5] &= mask;
  mask &= ((u8 *)owner->animation)[7];
  mask |= 128;
  ((u8 *)owner->animation)[7] = mask;
  if (*(s8 *)(event + 394) == 3)
    *(s32 *)((u8 *)owner->animation + 12) = 0xc000;
  owner->angle = 0x4000;
  Actor_SetMode(owner, 0);
  Animation_ApplyChildValueToFirstFar(owner->animation, 15);
  wait(1);
  Animation_ApplyChildValueToFirstFar(owner->animation, 0);
  {
    void *source = *(void **)((u8 *)owner->animation + 40);

    sprite = 0x400;
    {
      /* FAKEMATCH(pin): the copy's size pinned to r2, so it is set before
       * work's copy to r1; as Func_080e43a4(source, work, sprite), 2 of 332
       * instructions differ in tla-en. */
      register s32 size asm("r2") = sprite;

      Func_080e43a4(source, work, size);
    }
  }
  Animation_ApplyChildValueToFirstFar(owner->animation, 15);
  /* FAKEMATCH(barrier): an empty asm reading party, which nothing reads after
   * the animation's set, keeps it live in r6 to here; without it, 79 of 332
   * instructions differ in tla-en. */
  asm("" : : "r"(party));
  pictureEntry = Resource_FindFreeEntry();
  sprite = VramCache_Load(pictureEntry, sprite, NULL);
  Func_080e4510(work + 0x1900, sprite);
  wait(1);

  frame = 0;
  rippled = work + 0x400;
  ripple = 0;
  pictureOffset = sprite * 32;
  for (; frame < 78; frame++) {
    if (frame == 0)
      Sound_PlayFar(544);
    if (frame <= 64) {
      Func_080e446c(work, rippled, 92 - ripple / 2);
      ((void (*)(void *, s32, s32, void *))Ram_HeapSlots->unk4c[2])(
          rippled, 32, 32, (u8 *)0x06010000 + pictureOffset);
      Func_080e45a0(work + 0x1900);
    }
    if (frame == 66) {
      Actor_SetMode(owner, 1);
      Animation_ApplyChildValueToFirstFar(owner->animation, 0);
      Actor_SetSingleChildFlagsFar(Actor_GetById(*(s16 *)(effect + 24)), 1);
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
    i = 0;
    particle = work + 0x1200;
    particleSprite = work + 0x800;
    for (; i < 30; i++) {
      Func_080e46ac(particleSprite, particle, i, particleTiles);
      particleSprite += 40;
      particle += 28;
    }
    wait(1);
    ripple += 3;
  }
  Actor_SetOthersPaused(owner, 0, 16);
  Resource_ResetEntry(pictureEntry);
  Resource_ResetEntry(particleEntry);
  HeapSlot_Release(92);
}
