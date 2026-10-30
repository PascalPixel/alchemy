#ifndef ALCHEMY_DJINN_IDS_H
#define ALCHEMY_DJINN_IDS_H

/// @file
/// Djinn numbers, named from the English release.

/// The Djinn of each element.
#define DJINN_PER_ELEMENT 20

/// A Djinni's place among its element's Djinn, from 0, as Djinn_JoinParty and
/// the party members' Djinn lists take it.
#define DJINNI_INDEX(id) ((id) % DJINN_PER_ELEMENT)

/// Djinn, numbered as Field_RequestDjinnBattle and FLAG_DJINN_FIRST count them:
/// DJINN_PER_ELEMENT to an element, in the order Venus, Mercury, Mars, Jupiter.
/// A Djinni's English name is the message MsgDjinnName + its number. The
/// text names each element's last 13 numbers "=", and their rows of
/// gDjinn_DefinitionTable hold only their abilities, which the text names "="
/// too; they keep their numbers.
enum DjinniId {
  /// ★ Named from the game: "Flint" (MsgDjinnName)
  DJINNI_FLINT,
  /// ★ Named from the game: "Granite" (MsgDjinnName)
  DJINNI_GRANITE,
  /// ★ Named from the game: "Quartz" (MsgDjinnName)
  DJINNI_QUARTZ,
  /// ★ Named from the game: "Vine" (MsgDjinnName)
  DJINNI_VINE,
  /// ★ Named from the game: "Sap" (MsgDjinnName)
  DJINNI_SAP,
  /// ★ Named from the game: "Ground" (MsgDjinnName)
  DJINNI_GROUND,
  /// ★ Named from the game: "Bane" (MsgDjinnName)
  DJINNI_BANE,
  DJINNI_07,
  DJINNI_08,
  DJINNI_09,
  DJINNI_10,
  DJINNI_11,
  DJINNI_12,
  DJINNI_13,
  DJINNI_14,
  DJINNI_15,
  DJINNI_16,
  DJINNI_17,
  DJINNI_18,
  DJINNI_19,
  /// ★ Named from the game: "Fizz" (MsgDjinnName)
  DJINNI_FIZZ,
  /// ★ Named from the game: "Sleet" (MsgDjinnName)
  DJINNI_SLEET,
  /// ★ Named from the game: "Mist" (MsgDjinnName)
  DJINNI_MIST,
  /// ★ Named from the game: "Spritz" (MsgDjinnName)
  DJINNI_SPRITZ,
  /// ★ Named from the game: "Hail" (MsgDjinnName)
  DJINNI_HAIL,
  /// ★ Named from the game: "Tonic" (MsgDjinnName)
  DJINNI_TONIC,
  /// ★ Named from the game: "Dew" (MsgDjinnName)
  DJINNI_DEW,
  DJINNI_27,
  DJINNI_28,
  DJINNI_29,
  DJINNI_30,
  DJINNI_31,
  DJINNI_32,
  DJINNI_33,
  DJINNI_34,
  DJINNI_35,
  DJINNI_36,
  DJINNI_37,
  DJINNI_38,
  DJINNI_39,
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  DJINNI_FORGE,
  /// ★ Named from the game: "Fever" (MsgDjinnName)
  DJINNI_FEVER,
  /// ★ Named from the game: "Corona" (MsgDjinnName)
  DJINNI_CORONA,
  /// ★ Named from the game: "Scorch" (MsgDjinnName)
  DJINNI_SCORCH,
  /// ★ Named from the game: "Ember" (MsgDjinnName)
  DJINNI_EMBER,
  /// ★ Named from the game: "Flash" (MsgDjinnName)
  DJINNI_FLASH,
  /// ★ Named from the game: "Torch" (MsgDjinnName)
  DJINNI_TORCH,
  DJINNI_47,
  DJINNI_48,
  DJINNI_49,
  DJINNI_50,
  DJINNI_51,
  DJINNI_52,
  DJINNI_53,
  DJINNI_54,
  DJINNI_55,
  DJINNI_56,
  DJINNI_57,
  DJINNI_58,
  DJINNI_59,
  /// ★ Named from the game: "Gust" (MsgDjinnName)
  DJINNI_GUST,
  /// ★ Named from the game: "Breeze" (MsgDjinnName)
  DJINNI_BREEZE,
  /// ★ Named from the game: "Zephyr" (MsgDjinnName)
  DJINNI_ZEPHYR,
  /// ★ Named from the game: "Smog" (MsgDjinnName)
  DJINNI_SMOG,
  /// ★ Named from the game: "Kite" (MsgDjinnName)
  DJINNI_KITE,
  /// ★ Named from the game: "Squall" (MsgDjinnName)
  DJINNI_SQUALL,
  /// ★ Named from the game: "Luff" (MsgDjinnName)
  DJINNI_LUFF,
  DJINNI_67,
  DJINNI_68,
  DJINNI_69,
  DJINNI_70,
  DJINNI_71,
  DJINNI_72,
  DJINNI_73,
  DJINNI_74,
  DJINNI_75,
  DJINNI_76,
  DJINNI_77,
  DJINNI_78,
  DJINNI_79
};

#endif
