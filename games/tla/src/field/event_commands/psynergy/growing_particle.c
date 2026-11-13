/// @file
/// A sprite particle that grows as it drifts, and FieldPsynergy_EndSand, which
/// sets up 64 of them as a dust puff.
// Part of psynergy.c.

#include "../psynergy_internal.h"

void Camera_WorldToScreen(s32 *position);

/// Advances one particle by a frame. For its first 32 frames its sprite
/// steps through eight pictures of eight tiles from tiles, four frames each,
/// grows by a tenth of its size a frame to full size, is placed on screen at
/// the particle's position and drawn, and the particle moves on; after that
/// only its age counts on. Reached from FieldPsynergy_EndSand and
/// FieldPsynergy_Sand.
void Func_080e46ac(struct EffectSprite *sprite, struct EffectParticle *particle,
                   s32 unused, s32 tiles) {
  s32 position[3];
  s32 scale;
  s32 picture;

  if ((u32)particle->timer <= 31) {
    picture = (particle->timer >> 2) & 7;
    sprite->oam.index = tiles + picture * 8;
    scale = sprite->scaleX + 0x1999;
    sprite->scaleX = scale;
    if (scale > Q16_ONE)
      scale = Q16_ONE;
    sprite->scaleX = scale;
    sprite->scaleY = scale;
    position[0] = particle->x;
    position[1] = particle->y;
    position[2] = particle->z;
    Camera_WorldToScreen(position);
    sprite->x = position[0];
    sprite->y = position[2];
    Func_080eb01c(sprite);
    BattleEffect_IntegrateVector2(&particle->x, 60, 0);
  }
  particle->timer++;
}

struct PendingEvent;

/* VIEW(growing_particle): variable gMainRoutineWork as extern u8
 * gMainRoutineWork[]; main_routine_work.h has extern struct MainRoutineWork
 * gMainRoutineWork; tried as the 3 declarations' extern u8 gMainRoutineWork[],
 * it does not compile in tla-ja:
 * games/tla/include/system/main_routine_work.h:123: conflicting types for
 * `gMainRoutineWork'. */
extern u8 gMainRoutineWork__growing_particle[] __asm__("gMainRoutineWork");
extern u8 Data_02000452;

void Func_080dc954(void);
void Actor_SetOthersPaused(struct Actor *excludedObject, s32 groupMode,
                           s32 action);
void Func_080e42d4(void);
void *Resource_GetTableEntry(s32 resourceId);
extern char ResourceId_DustPuffA;
s32 Decompress_TaggedLz(const void *source, void *destination);
s32 VramCache_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 VramCache_FreeEntry(u32 slot);
/* VIEW(growing_particle): routine Func_080e45a8 as void Func_080e45a8(void
 * *sprite, void *particle, s32 index, s32 tiles, s32 high); the definition has
 * void Func_080e45a8(struct EffectSprite__f0e446c *sprite, struct
 * EffectParticle *particle, s32 unused, s32 tiles, s32 high). */
extern void Func_080e45a8__growing_particle(void *sprite, void *particle,
                                            s32 index, s32 tiles,
                                            s32 high) __asm__("Func_080e45a8");
/* VIEW(growing_particle): routine Func_080e4510 as void Func_080e4510(void
 * *sprite, s32 tiles); the definition has void Func_080e4510(struct
 * EffectSprite__f0e446c *sprite, s32 tiles). */
extern void Func_080e4510__growing_particle(void *sprite,
                                            s32 tiles) __asm__("Func_080e4510");
void Func_080e446c(u8 *source, u8 *destination, s32 phase);
void Func_080e45a0(void *sprite);
void FieldCamera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x, s32 y,
                                        s32 z);
void Actor_SetFirstSpriteAnimationFar(struct Actor *object, s32 index);
void Animation_ApplyChildValueToFirstFar(void *animation, s32 value);
struct Actor *Actor_GetById(u32 objectId);
void FieldCamera_FollowActor(s32 actor, s32 mode);
s32 Func_080ce31c(s32 kind);
/* VIEW(growing_particle): routine Scene_FindPsynergyTrigger as struct
 * PendingEvent *Scene_FindPsynergyTrigger(u32 filter, s32 kind, s32 selector);
 * battle.h has struct TriggerEvent *Scene_FindPsynergyTrigger(s32 filter, s32
 * kind, s32 selector). */
extern struct PendingEvent *Func_080ce458__growing_particle(
    u32 filter, s32 kind, s32 selector) __asm__("Scene_FindPsynergyTrigger");
s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 argument);

void FieldPsynergy_EndSand(void) {
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
  gMainRoutineWork__growing_particle[620] = 0;
  Actor_SetOthersPaused(owner, 1, 0);
  Task_Remove(Func_080e42d4);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DustPuffA), work);
  particleEntry = VramCache_FindFreeEntry();
  particleTiles = VramCache_Load(particleEntry, 0x800, work);
  i = 0;
  particle = work + 0x1200;
  delay = 0;
  particleSprite = work + 0x800;
  for (; i < 64; i++) {
    Func_080e45a8__growing_particle(particleSprite, particle, i, particleTiles,
                                    *(s16 *)(event + 0xd48));
    *(s32 *)(particle + 24) = delay;
    particleSprite += 40;
    delay -= 2;
    particle += 28;
  }
  FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
  motionFlags = &owner->motionFlags;
  *motionFlags = 0;
  // Set before the move: 63 is held in a saved register across both calls.
  mask = 63;
  Actor_SetPositionAndResetMotionFar(owner, owner->x, owner->y + 0xa0000,
                                     owner->z);
  party = gMainRoutineWork__growing_particle;
  Actor_SetFirstSpriteAnimationFar(owner, *(s32 *)(party + 532));
  ((u8 *)owner->sprite)[5] &= mask;
  mask &= ((u8 *)owner->sprite)[7];
  mask |= 128;
  ((u8 *)owner->sprite)[7] = mask;
  if (*(s8 *)(event + 394) == 3)
    *(s32 *)((u8 *)owner->sprite + 12) = 0xc000;
  owner->facing = 0x4000;
  Actor_SetMode(owner, 0);
  Animation_ApplyChildValueToFirstFar(owner->sprite, 15);
  wait(1);
  Animation_ApplyChildValueToFirstFar(owner->sprite, 0);
  {
    void *source = *(void **)((u8 *)owner->sprite + 40);

    sprite = 0x400;
    {
      /* FAKEMATCH(pin): the copy's size pinned to r2, so it is set before
       * work's copy to r1; as Func_080e43a4(source, work, sprite), 2 of 332
       * instructions differ in tla-en. */
      register s32 size asm("r2") = sprite;

      Func_080e43a4(source, work, size);
    }
  }
  Animation_ApplyChildValueToFirstFar(owner->sprite, 15);
  /* FAKEMATCH(barrier): an empty asm reading party, which nothing reads after
   * the animation's set, keeps it live in r6 to here; without it, 79 of 332
   * instructions differ in tla-en. */
  asm("" : : "r"(party));
  pictureEntry = VramCache_FindFreeEntry();
  sprite = VramCache_Load(pictureEntry, sprite, NULL);
  Func_080e4510__growing_particle(work + 0x1900, sprite);
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
      Animation_ApplyChildValueToFirstFar(owner->sprite, 0);
      Actor_SetSingleChildFlagsFar(Actor_GetById(*(s16 *)(effect + 24)), 1);
      FieldCamera_FollowActor(*(s16 *)(effect + 24), 1);
      Sound_PlayFar(539);
      if (*(s8 *)(event + 394) == 3) {
        Data_02000452 = 1;
        *motionFlags = 2;
      } else {
        Data_02000452 = 0;
        *motionFlags = 3;
      }
      if (*(s16 *)(event + 0xd48) == 0)
        Field_RunPsynergyEvent(Func_080ce458__growing_particle(
                                   0x40000005, *(s16 *)(effect + 30),
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
  VramCache_FreeEntry(pictureEntry);
  VramCache_FreeEntry(particleEntry);
  HeapSlot_Release(92);
}

void FieldPsynergy_Blaze(void);
void FieldPsynergy_Sand(void);
void FieldPsynergy_Burst(void);
void FieldPsynergy_Pound(void);
void FieldPsynergy_Scoop(void);
void FieldPsynergy_Parch(void);
void FieldPsynergy_Hover(void);
void Func_080e74d8(void);
void FieldPsynergy_FadeFromCastColor(void);
s32 Party_GetLeaderActor(void);

/// Psynergy effect 23 cast on a target: runs its effect.
void FieldPsynergy_SandOnTarget(void) { FieldPsynergy_Sand(); }
