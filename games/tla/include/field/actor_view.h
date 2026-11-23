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

s32 Sprite_StartAnimation(struct Sprite *, s32);
void Sprite_SetAnimationSpeed(struct Sprite *, s32);
s32 Sprite_ResetAnimations(struct Sprite *);
s32 Sprite_AddAnimation(struct Sprite *, s32);
void Actor_StartScript(struct Actor *, const s32 *);
void Actor_SetSpriteFlagsFar(struct Actor *actor, u32 flags);
void Sprite_SetAnimationColorModes(struct Sprite *, u32);
void Sprite_SetAnimationColorModesFar(struct Sprite *, u32);
void Actor_SetSpriteColorMode(struct Actor *, u32);
void Actor_SetSpriteColorModeFar(struct Actor *, u32);
void Actor_SetSpritePriorityDirect(struct Actor *, s32);

extern const s32 gActor_CameraScript[];
extern const s32 gActorScript_EndScript[];

#endif
