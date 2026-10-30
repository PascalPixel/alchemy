#ifndef ALCHEMY_CHARACTER_IDS_H
#define ALCHEMY_CHARACTER_IDS_H

/// @file
/// Character numbers, with each character's Japanese name, and the portraits.

/// The characters, one for each row of gCharacter_DefinitionTable
/// (character_table.c) in order; unit ids below ENEMY_ID_FIRST are these.
enum CharacterId {
  ISAAC, ///< Robin
  GARET, ///< Jerald
  IVAN,  ///< Ivan
  MIA,   ///< Mary
  FELIX, ///< Garcia
  JENNA, ///< Jasmine
  SHEBA, ///< Sheba
  PIERS  ///< Picard
};

/// The portraits, one for each row of the portrait icons (Ui_Icons,
/// graphics/ui/icons/icons.tsv) in order; a row the code doesn't name yet keeps
/// its number as its placeholder, and rows 8 to 15 and 65 to 79 are empty. A
/// character's portrait is its character id. Icon_LoadWithPalette takes a
/// portrait number below 128 as its row, and the other speakers' portraits,
/// numbered from 128 (Portrait_GetForSpeaker), as the rows from PORTRAIT_16 on.
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
  PORTRAIT_16,
  PORTRAIT_17,
  PORTRAIT_18,
  PORTRAIT_19,
  PORTRAIT_20,
  PORTRAIT_21,
  PORTRAIT_22,
  PORTRAIT_23,
  PORTRAIT_24,
  PORTRAIT_25,
  PORTRAIT_26,
  PORTRAIT_27,
  PORTRAIT_28,
  PORTRAIT_29,
  PORTRAIT_30,
  PORTRAIT_31,
  PORTRAIT_32,
  PORTRAIT_33,
  PORTRAIT_34,
  PORTRAIT_35,
  PORTRAIT_36,
  PORTRAIT_37,
  PORTRAIT_38,
  PORTRAIT_39,
  PORTRAIT_40,
  PORTRAIT_41,
  PORTRAIT_42,
  PORTRAIT_43,
  PORTRAIT_44,
  PORTRAIT_45,
  PORTRAIT_46,
  PORTRAIT_47,
  PORTRAIT_48,
  PORTRAIT_49,
  PORTRAIT_50,
  PORTRAIT_51,
  PORTRAIT_52,
  PORTRAIT_53,
  PORTRAIT_54,
  PORTRAIT_55,
  PORTRAIT_ISAAC_CHILD,
  PORTRAIT_GARET_CHILD,
  PORTRAIT_58,
  PORTRAIT_59,
  PORTRAIT_60,
  PORTRAIT_61,
  PORTRAIT_62,
  PORTRAIT_63,
  PORTRAIT_64,
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
