#ifndef ALCHEMY_ACTOR_ANIMATION_H
#define ALCHEMY_ACTOR_ANIMATION_H

/// @file
/// The animation numbers every person's sprite shares.

/// Animation numbers of a person's sprite, for Actor_SetAnimation. A sprite
/// that is not a person (a bird, a dragon, a pillar) numbers its own.
enum ActorAnimation {
  /// Standing still: the sprite's first frame held, without the blinks and
  /// small gestures of ANIM_STAND.
  ANIM_STILL = 0,
  ANIM_STAND = 1,
  ANIM_WALK = 2,
  ANIM_NOD = 3,
  ANIM_SHAKE_HEAD = 4
};

#endif
