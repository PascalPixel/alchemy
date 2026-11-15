#ifndef ALCHEMY_ACTOR_VIEW_H
#define ALCHEMY_ACTOR_VIEW_H

#include "types.h"
#include "system/system.h"
#include "field/actor.h"

struct Sprite;
struct SpriteAnimation;

/// An actor's sprite as the actor setters write it: the same
/// bytes as FieldSprite.
struct ActorSpriteView {
  u8 unused[5]; ///< no code names it
  u8 unk05Low : 2;
  u8 stateFlags : 2; ///< the image's OAM object mode (FieldSprite.blendMode)
  u8 unk05High : 4;
  u8 unusedB[0x0b]; ///< no code names it
  u8 unk11Low : 1;
  u8 displayFlag : 1; ///< the own-scale bit in the projection draft's layout
  u8 unk11High : 6;
  s16 value12; ///< FieldSprite.rotation, 1/65536 turns
  u8 unk14[5];
  u8 dirty;                  ///< nonzero redraws its pixels
  u8 value1a;                ///< the sprite flags (FieldSprite.nextFrame)
  u8 count;                  ///< layers in use
  u8 unusedAfterCount[0x0c]; ///< no code names it
  struct SpriteAnimation *entries[4]; ///< its layers; empty ones null
};

struct FieldSprite;

/// struct Actor as FieldPsynergy_UpdateHaltedActor and its restorer
/// read it (age_and_cue.c): as struct Actor, its update hook's type
/// disagrees with the routine it stores there.
struct ActorView {
  u32 value00;      ///< the object script's address
  s16 value04;      ///< the script cursor, in words
  u8 unused[0x1d];  ///< facing, position, scales and radius; not named here
  u8 priorityFlags; ///< Actor.priorityFlags
  u8 unusedAfterPriorityFlags[0x0c]; ///< the velocity; not named here
  s32 value30;      ///< speed limit, 16.16 fixed-point pixels per frame
  s32 value34;      ///< acceleration, 16.16 fixed point
  u8 unusedB[0x18]; ///< the target, bounce, gravity and a spare word
  union {
    struct ActorSpriteView *child;     ///< kind 1
    struct ActorSpriteView **children; ///< kind 2
    struct FieldSprite *sprite;        ///< kind 1
    struct Sprite *animation;
  } target;
  u8 kind;               ///< 1 one sprite, 2 a group of sprites
  u8 unusedAfterKind[2]; ///< motion flags and arrival axis; not named here
  u8 value57;            ///< cleared with a new script
  u8 unusedC[3];         ///< snap, collision and steering flags; not named here
  u8 value5b; ///< nonzero pauses the object; cleared with a new script
  u8 unk5c;
  u8 value5d;    ///< cleared with a new script
  u8 unusedD[6]; ///< no code names it
  s16 value64;   ///< the action script's parameter; the camera clears it
  u8 unusedE[2]; ///< no code names it
  s32 argument;  ///< the watched object a script approaches or follows
  void (*update)(struct ActorView *object); ///< per-frame hook
};

s32 Sprite_StartAnimation(struct Sprite *, s32);
void Sprite_SetAnimationSpeed(struct Sprite *, s32);
s32 Sprite_ResetAnimations(struct Sprite *);
s32 Sprite_AddAnimation(struct Sprite *, s32);
void Actor_StartScript(struct Actor *, const s32 *);
void Actor_SetSpriteFlagsFar(struct Actor *object, u32 flags);
void Animation_ApplyChildValuesToRecord(struct Sprite *, u32);
void Animation_ApplyChildValuesToRecordFar(struct Sprite *, u32);
void Actor_SetSpriteColorMode(struct Actor *, u32);
void Actor_SetSpriteColorModeFar(struct Actor *, u32);
void Actor_SetSpritePriorityDirect(struct Actor *, s32);

extern const s32 gActor_CameraScript[];
extern const s32 gActorScript_EndScript[];

#endif
