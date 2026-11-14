#ifndef ALCHEMY_DJINN_IDS_H
#define ALCHEMY_DJINN_IDS_H

/// @file
/// Djinn numbers, named from the English release, as The Broken Seal's
/// djinn_ids.h names its own.

#define DJINN_PER_ELEMENT 20

/// A Djinni's place among its element's Djinn, from 0, as Djinn_JoinParty and
/// the party members' Djinn lists take it.
#define DJINNI_INDEX(id) ((id) % DJINN_PER_ELEMENT)

/// Djinn, numbered as Field_RequestDjinnBattle and FLAG_DJINN_FIRST count them:
/// DJINN_PER_ELEMENT to an element, in the order Venus, Mercury, Mars, Jupiter.
/// A Djinni's English name is the message MsgDjinnName + its number. The
/// text names each element's last two numbers after the element ("Earth18"),
/// and they keep their numbers.
enum DjinniId {
  DJINNI_FLINT,
  DJINNI_GRANITE,
  DJINNI_QUARTZ,
  DJINNI_VINE,
  DJINNI_SAP,
  DJINNI_GROUND,
  DJINNI_BANE,
  DJINNI_ECHO,
  DJINNI_IRON,
  DJINNI_STEEL,
  DJINNI_MUD,
  DJINNI_FLOWER,
  DJINNI_MELD,
  DJINNI_PETRA,
  DJINNI_SALT,
  DJINNI_GEODE,
  DJINNI_MOLD,
  DJINNI_CRYSTAL,
  DJINNI_18,
  DJINNI_19,
  DJINNI_FIZZ,
  DJINNI_SLEET,
  DJINNI_MIST,
  DJINNI_SPRITZ,
  DJINNI_HAIL,
  DJINNI_TONIC,
  DJINNI_DEW,
  DJINNI_FOG,
  DJINNI_SOUR,
  DJINNI_SPRING,
  DJINNI_SHADE,
  DJINNI_CHILL,
  DJINNI_STEAM,
  DJINNI_RIME,
  DJINNI_GEL,
  DJINNI_EDDY,
  DJINNI_BALM,
  DJINNI_SERAC,
  DJINNI_38,
  DJINNI_39,
  DJINNI_FORGE,
  DJINNI_FEVER,
  DJINNI_CORONA,
  DJINNI_SCORCH,
  DJINNI_EMBER,
  DJINNI_FLASH,
  DJINNI_TORCH,
  DJINNI_CANNON,
  DJINNI_SPARK,
  DJINNI_KINDLE,
  DJINNI_CHAR,
  DJINNI_COAL,
  DJINNI_REFLUX,
  DJINNI_CORE,
  DJINNI_TINDER,
  DJINNI_SHINE,
  DJINNI_FURY,
  DJINNI_FUGUE,
  DJINNI_58,
  DJINNI_59,
  DJINNI_GUST,
  DJINNI_BREEZE,
  DJINNI_ZEPHYR,
  DJINNI_SMOG,
  DJINNI_KITE,
  DJINNI_SQUALL,
  DJINNI_LUFF,
  DJINNI_BREATH,
  DJINNI_BLITZ,
  DJINNI_ETHER,
  DJINNI_WAFT,
  DJINNI_HAZE,
  DJINNI_WHEEZE,
  DJINNI_AROMA,
  DJINNI_WHORL,
  DJINNI_GASP,
  DJINNI_LULL,
  DJINNI_GALE,
  DJINNI_78,
  DJINNI_79
};

#endif
