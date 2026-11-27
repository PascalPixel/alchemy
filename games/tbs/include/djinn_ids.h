#ifndef ALCHEMY_DJINN_IDS_H
#define ALCHEMY_DJINN_IDS_H

// Djinn numbers, named from the English release.

#define DJINN_PER_ELEMENT 20

// A Djinni's place among its element's Djinn, from 0, as fdDjinnJoinParty and the party members'
// Djinn lists take it.
#define DJINNI_INDEX(id)  ((id) % DJINN_PER_ELEMENT)

// Djinn, numbered as fdRequestDjinnBattle and FLAG_DJINN_FIRST count them: DJINN_PER_ELEMENT to an
// element, in the order Venus, Mercury, Mars, Jupiter. A Djinni's English name is the message
// MsgDjinnName + its number. Each element has seven; the 13 numbers after them are spares, which
// the text names "=" and whose rows of DjinnDefinitionTable hold only their abilities.
typedef enum DJINNI_ID
{
	DJINNI_FLINT,
	DJINNI_GRANITE,
	DJINNI_QUARTZ,
	DJINNI_VINE,
	DJINNI_SAP,
	DJINNI_GROUND,
	DJINNI_BANE,
	DJINNI_FIZZ = DJINN_PER_ELEMENT,
	DJINNI_SLEET,
	DJINNI_MIST,
	DJINNI_SPRITZ,
	DJINNI_HAIL,
	DJINNI_TONIC,
	DJINNI_DEW,
	DJINNI_FORGE = DJINN_PER_ELEMENT * 2,
	DJINNI_FEVER,
	DJINNI_CORONA,
	DJINNI_SCORCH,
	DJINNI_EMBER,
	DJINNI_FLASH,
	DJINNI_TORCH,
	DJINNI_GUST = DJINN_PER_ELEMENT * 3,
	DJINNI_BREEZE,
	DJINNI_ZEPHYR,
	DJINNI_SMOG,
	DJINNI_KITE,
	DJINNI_SQUALL,
	DJINNI_LUFF
} DJINNI_ID;

#endif
