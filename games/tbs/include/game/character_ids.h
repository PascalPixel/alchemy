#ifndef ALCHEMY_CHARACTER_IDS_H
#define ALCHEMY_CHARACTER_IDS_H

/// @file
/// Character numbers, with each character's Japanese name, and the portraits.

/// The characters, one for each row of gCharacter_DefinitionTable
/// (character_table.c) in order; unit ids below ENEMY_ID_FIRST are these.
enum CharacterId {
  ISAAC, ///< Robin
  GARET, ///< Jerald
  IVAN,
  MIA,   ///< Mary
  FELIX, ///< Garcia
  JENNA, ///< Jasmine
  SHEBA,
  PIERS ///< Picard
};

/// The portraits, one for each row of the portrait icons (Ui_Icons,
/// graphics/ui/icons/icons.tsv) in order; a row the code doesn't name yet keeps
/// its number as its placeholder, and rows 8 to 15 and 65 to 79 are empty. A
/// character's portrait is its character id. Icon_LoadWithPalette takes a
/// portrait number below 128 as its row, and the other speakers' portraits,
/// numbered from 128 (Portrait_GetForSpeaker), as the rows from
/// PORTRAIT_JUPITER_DJINNI on; each of those is the face of the speaker whose
/// sprite gPortrait_SpriteMap pairs with it, and takes that sprite's name.
enum PortraitId {
  PORTRAIT_ISAAC,
  PORTRAIT_GARET,
  PORTRAIT_IVAN,
  PORTRAIT_MIA,
  PORTRAIT_FELIX,
  PORTRAIT_JENNA,
  PORTRAIT_SHEBA,
  PORTRAIT_PIERS,
  PORTRAIT_08,
  PORTRAIT_09,
  PORTRAIT_10,
  PORTRAIT_11,
  PORTRAIT_12,
  PORTRAIT_13,
  PORTRAIT_14,
  PORTRAIT_15,
  PORTRAIT_JUPITER_DJINNI,
  PORTRAIT_VENUS_DJINNI,
  PORTRAIT_MARS_DJINNI,
  PORTRAIT_MERCURY_DJINNI,
  PORTRAIT_KRADEN,
  PORTRAIT_SATUROS,
  PORTRAIT_MENARDI,
  PORTRAIT_ALEX,
  PORTRAIT_DORA,
  PORTRAIT_KYLE,
  PORTRAIT_WOMAN_IN_GREEN_HEADSCARF,
  PORTRAIT_WOMAN_WITH_ORANGE_PONYTAIL,
  PORTRAIT_BOY_WITH_ORANGE_HAIR,
  PORTRAIT_MAN_IN_TEAL_SHIRT,
  PORTRAIT_MCCOY,
  PORTRAIT_WOMAN_WITH_PURPLE_HAIR,
  PORTRAIT_TRET,
  PORTRAIT_LAUREL,
  PORTRAIT_NYUNPA,
  PORTRAIT_HAMMET,
  PORTRAIT_LAYANA,
  PORTRAIT_FEH,
  PORTRAIT_FEIZHI,
  PORTRAIT_HSU,
  PORTRAIT_DONPA,
  PORTRAIT_DODONPA,
  PORTRAIT_BABI,
  PORTRAIT_IODEM,
  PORTRAIT_HAMA,
  PORTRAIT_KAJA,
  PORTRAIT_MAN_WITH_WHITE_BEARD_IN_GREEN,
  PORTRAIT_YOUNG_MAN_WITH_BLUE_HAIR,
  PORTRAIT_YOUNG_MAN_WITH_RED_HAIR,
  PORTRAIT_MAN_IN_TALL_HAT,
  PORTRAIT_BEARDED_MAN_IN_LONG_ROBE,
  PORTRAIT_BALD_MAN_IN_RED,
  PORTRAIT_YOUNG_MAN_IN_GREEN,
  PORTRAIT_WOMAN_WITH_PONYTAIL,
  PORTRAIT_WOMAN_IN_PINK_APRON,
  PORTRAIT_MAN_WITH_DARK_HAIR,
  PORTRAIT_ISAAC_CHILD,
  PORTRAIT_GARET_CHILD,
  PORTRAIT_FELIX_CHILD,
  PORTRAIT_JENNA_CHILD,
  PORTRAIT_FELIX_MASKED,
  PORTRAIT_TRET_WRATHFUL,
  /// The row gPortrait_SpriteMap pairs with SPRITE_FELIX, drawn from
  /// icons.tsv's FELIX, as PORTRAIT_FELIX is.
  PORTRAIT_FELIX_UNMASKED,
  PORTRAIT_INVISIBLE_BABI,
  PORTRAIT_FARAN,
  PORTRAIT_65,
  PORTRAIT_66,
  PORTRAIT_67,
  PORTRAIT_68,
  PORTRAIT_69,
  PORTRAIT_70,
  PORTRAIT_71,
  PORTRAIT_72,
  PORTRAIT_73,
  PORTRAIT_74,
  PORTRAIT_75,
  PORTRAIT_76,
  PORTRAIT_77,
  PORTRAIT_78,
  PORTRAIT_79
};

#endif
