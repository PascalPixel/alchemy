#ifndef ALCHEMY_ACTOR_VIEW_H
#define ALCHEMY_ACTOR_VIEW_H

/// @file
/// Starting an actor's sprite animations and setting its sprite's colour mode
/// and priority.

#include "types.h"
#include "system/system.h"
#include "field/actor.h"

struct Sprite;
struct SpriteAnimation;

struct FieldSprite;

/// Starting, timing, resetting and adding a sprite's animations and setting
/// their colour modes; a timer step of 16 is normal speed.
s32 Sprite_StartAnimation(struct Sprite *sprite, s32 animation);
void Sprite_SetAnimationSpeed(struct Sprite *sprite, s32 step);
s32 Sprite_ResetAnimations(struct Sprite *sprite);
s32 Sprite_AddAnimation(struct Sprite *sprite, s32 id);
void Sprite_SetAnimationColorModes(struct Sprite *sprite, u32 colorMode);
void Sprite_SetAnimationColorModesFar(struct Sprite *sprite, u32 colorMode);

/// Starting an actor's script and setting its sprite's flags, colour mode and
/// priority.
void Actor_StartScript(struct Actor *actor, const s32 *script);
void Actor_SetSpriteFlagsFar(struct Actor *actor, u32 flags);
void Actor_SetSpriteColorMode(struct Actor *actor, u32 colorMode);
void Actor_SetSpriteColorModeFar(struct Actor *actor, u32 colorMode);
void Actor_SetSpritePriorityDirect(struct Actor *actor, s32 priority);

extern const s32 gActor_CameraScript[];
extern const s32 gActorScript_EndScript[];

#endif
