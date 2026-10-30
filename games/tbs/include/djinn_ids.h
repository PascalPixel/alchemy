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
/// A Djinni's English name is the message MsgDjinnName + its number.
enum DjinniId {
  DJINNI_FLINT,
  DJINNI_GRANITE,
  DJINNI_QUARTZ,
  DJINNI_VINE,
  DJINNI_SAP,
  DJINNI_GROUND,
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
  DJINNI_FIZZ,
  DJINNI_SLEET,
  DJINNI_MIST,
  DJINNI_SPRITZ,
  DJINNI_HAIL,
  DJINNI_TONIC,
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
  DJINNI_FORGE,
  DJINNI_FEVER,
  DJINNI_CORONA,
  DJINNI_SCORCH,
  DJINNI_EMBER,
  DJINNI_FLASH,
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
  DJINNI_GUST,
  DJINNI_BREEZE,
  DJINNI_ZEPHYR,
  DJINNI_SMOG,
  DJINNI_KITE,
  DJINNI_SQUALL,
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
