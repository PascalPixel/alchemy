#ifndef ALCHEMY_SPRITE_IDS_H
#define ALCHEMY_SPRITE_IDS_H

/// @file
/// Sprite numbers of field objects, named where the code shows what they are.
/// The characters' and Djinn's sprites are in character_ids.h. A sprite's
/// picture is graphics/sprite/spr_<number>.png, its number in decimal.

/// The sprites the field, its Psynergy and the scenes' placement tables create
/// objects of by number.
enum SpriteId {
  /// A treasure chest, shut, opening and open, with the frames of the mimic
  /// some chests turn out to be.
  /// ★ Named from what the player sees: a red treasure chest (its render)
  SPRITE_CHEST = 0x14,
  /// A blank 16-pixel square: an actor that shows nothing, such as a shop
  /// counter, or that a scene draws an item's icon on, as for the Nuts that
  /// Catch takes.
  SPRITE_BLANK = 0x16,
  /// The soldiers of Babi's Tolbi, and the Colosseum's officials, drawn alike.
  /// ★ Named from what the player sees: a soldier in a helmet with a spear
  /// (its render)
  SPRITE_SOLDIER = 0x98,
  /// ★ Named from what the player sees: a carved stone statue of a figure,
  /// as Sol Sanctum's statues stand on its seals (its render)
  SPRITE_STONE_STATUE = 0xcb,
  /// A log lying north to south, one the leader can hop onto
  /// (Field_TryJumpForward) or roll, as in Kolima Forest.
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  SPRITE_LOG_NORTH_SOUTH = 0xcd,
  /// ★ Named from what the player sees: a log lying west to east (its render)
  SPRITE_LOG_WEST_EAST = 0xcf,
  /// The effect BattleFx_ConfigureLinkedObject attaches to an actor or shows
  /// once, as the leader flinches on hazard ground.
  SPRITE_ATTACHED_EFFECT = 209,
  /// What Move pushes in Goma Cave and many rooms after it, which the game
  /// calls a stone pillar ("It's just a stone pillar.").
  /// ★ Named from what the player sees: a tall stone pillar (its render)
  SPRITE_STONE_PILLAR = 0xd3,
  /// ★ Named from what the player sees: a tree stump (its render)
  SPRITE_STUMP = 0xd4,
  SPRITE_DUST_PUFF = 222, ///< a puff of dust kicked up where an actor stands
  /// The statues of Mercury Lighthouse.
  /// ★ Named from what the player sees: a blue statue of a woman holding a
  /// jar (its render)
  SPRITE_BLUE_STATUE = 0xe0,
  /// A puddle of water, which Frost freezes into a pillar of ice.
  /// ★ Named from what the player sees: a puddle of water (its render)
  SPRITE_PUDDLE = 0xe3,
  /// The sprout Growth grows.
  /// ★ Named from what the player sees: a green sprout (its render)
  SPRITE_SPROUT = 0xee,
  /// ★ Named from what the player sees: a stack of two wooden crates (its
  /// render)
  SPRITE_CRATES = 0xf1,
  /// The platforms that float in Mercury Lighthouse's aerie and that carry the
  /// leader up to Venus Lighthouse's.
  /// ★ Named from what the player sees: a round floating platform (its
  /// render)
  SPRITE_FLOATING_PLATFORM = 0xf2,
  /// ★ Named from what the player sees: an unlit torch on its stand (its
  /// render)
  SPRITE_TORCH = 0xf6,
  /// The pillars Move shifts in the Tunnel Ruins and the lighthouses.
  /// ★ Named from what the player sees: a stone pillar with a glowing yellow
  /// cap (its render)
  SPRITE_CAPPED_PILLAR = 0xfd,
  /// The platforms of Venus Lighthouse and the Tunnel Ruins that rise or sink
  /// with the weight set down on them.
  /// ★ Named from what the player sees: a small white pillar on its base
  /// (its render)
  SPRITE_BALANCE_PLATFORM = 0x101,
  /// The particles of Growth and of a Djinni's capture, and the gate effects
  /// of Venus Lighthouse.
  SPRITE_PARTICLE = 284,
  /// One sparkle: the glints of Halt and of the field's sparkles, and Kolima's
  /// curse dust.
  /// ★ Named from what the player sees: a sparkle of light that shrinks away
  /// (its render)
  SPRITE_SPARKLE = 285,
  /// The statues that chase the leader in Crossbone Isle, and those of the
  /// Tunnel Ruins and Venus Lighthouse.
  /// ★ Named from what the player sees: a statue of a woman in a long robe
  /// (its render)
  SPRITE_LADY_STATUE = 0x1f4
};

#endif
