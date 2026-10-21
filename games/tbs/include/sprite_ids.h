#ifndef ALCHEMY_SPRITE_IDS_H
#define ALCHEMY_SPRITE_IDS_H

/// @file
/// Sprite numbers of field objects, named where the code shows what they are.
/// The characters' and Djinn's sprites are in character_ids.h.

/// The sprites the field and its Psynergy create objects of by number.
enum SpriteId {
  /// The other platform the leader may jump onto (Field_TryJumpForward).
  SPRITE_PLATFORM_2 = 205,
  SPRITE_PLATFORM = 207, ///< a platform the leader jumps onto
  /// The effect BattleFx_ConfigureLinkedObject attaches to an actor or shows
  /// once, as the leader flinches on hazard ground.
  SPRITE_ATTACHED_EFFECT = 209,
  SPRITE_DUST_PUFF = 222, ///< a puff of dust kicked up where an actor stands
  /// The particles of Growth and of a Djinni's capture.
  SPRITE_PARTICLE = 284
};

#endif
