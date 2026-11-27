#ifndef ALCHEMY_DJINN_IDS_H
#define ALCHEMY_DJINN_IDS_H

// Djinn numbers, named from the English release.

// The numbers each element's Djinn take, its spares among them.
#define DJINN_PER_ELEMENT  20
// The first of each element's two spares, after its eighteen Djinn (a DJINNI_INDEX): the text names
// the spares after the element ("Earth18").
#define DJINNI_SPARE_FIRST 18
// The place of each element's first Djinni this game adds (DJINNI_ECHO's), after The Broken Seal's
// seven.
#define DJINN_OWN_FIRST    7

// A Djinni's place among its element's Djinn, from 0, as fdDjinnJoinParty and the party members'
// Djinn lists take it.
#define DJINNI_INDEX(id)   ((id) % DJINN_PER_ELEMENT)

// Djinn, numbered as fdRequestDjinnBattle and FLAG_DJINN_FIRST count them: DJINN_PER_ELEMENT to an
// element, in the order Venus, Mercury, Mars, Jupiter. A Djinni's English name is the message
// MsgDjinnName + its number. Each element has eighteen; the two numbers after them are spares,
// which the text names after the element ("Earth18") and whose rows of DjinnDefinitionTable hold
// only their abilities.
typedef enum DJINNI_ID
{
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
    // The spare the text names "Earth19", which Treasure Isle places (its fifth room's placements).
    DJINNI_EARTH19 = DJINNI_CRYSTAL + 2,
    DJINNI_FIZZ    = DJINN_PER_ELEMENT,
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
    // The spare the text names "Water19", which Prox's houses place and scProxMercuryDjinniJoins
    // gives.
    DJINNI_WATER19 = DJINNI_SERAC + 2,
    DJINNI_FORGE   = DJINN_PER_ELEMENT * 2,
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
    // The spare the text names "Fire19", which Magma Rock joins to the party
    // (scMagmaRockInsideMeetDjinni).
    DJINNI_FIRE19 = DJINNI_FUGUE + 2,
    DJINNI_GUST   = DJINN_PER_ELEMENT * 3,
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
    // The spare the text names "Wind19", which the settlements join to the party
    // (scSettlementsMeetLostJupiterDjinni).
    DJINNI_WIND19 = DJINNI_GALE + 2,
} DJINNI_ID;

#endif
