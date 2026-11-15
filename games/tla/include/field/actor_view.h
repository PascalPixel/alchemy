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
