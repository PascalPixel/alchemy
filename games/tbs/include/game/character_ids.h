#ifndef ALCHEMY_CHARACTER_IDS_H
#define ALCHEMY_CHARACTER_IDS_H

// Character numbers, with each character's Japanese name, and the portraits.

// The characters, one for each row of CharacterDefinitionTable (character_table.c) in order; unit
// ids below ENEMY_ID_FIRST are these.
typedef enum CHARACTER_ID
{
	ISAAC, // Robin
	GARET, // Jerald
	IVAN,
	MIA,   // Mary
	FELIX, // Garcia
	JENNA, // Jasmine
	SHEBA,
	PIERS  // Picard
} CHARACTER_ID;

// The portraits, one for each row of the portrait icons (UiIcons, graphics/ui/icons/icons.tsv) in
// order; rows 8 to 15 and 65 to 79 are empty and have no names. A character's portrait is its
// character id. mnIconLoadWithPalette takes a portrait number below PORTRAIT_SPRITE_MAP_FIRST as
// its row, and the other speakers' portraits, numbered from it (mnPortraitGetForSpeaker), as the
// rows from PORTRAIT_JUPITER_DJINNI on; each of those is the face of the speaker whose sprite
// PortraitSpriteMap pairs with it, and takes that sprite's name.
typedef enum PORTRAIT_ID
{
	PORTRAIT_ISAAC,
	PORTRAIT_GARET,
	PORTRAIT_IVAN,
	PORTRAIT_MIA,
	PORTRAIT_FELIX,
	PORTRAIT_JENNA,
	PORTRAIT_SHEBA,
	PORTRAIT_PIERS,
	PORTRAIT_JUPITER_DJINNI = 16,
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
	// The row PortraitSpriteMap pairs with SPRITE_FELIX, drawn from icons.tsv's FELIX, as
	// PORTRAIT_FELIX is.
	PORTRAIT_FELIX_UNMASKED,
	PORTRAIT_INVISIBLE_BABI,
	PORTRAIT_FARAN
} PORTRAIT_ID;

// The portrait number of PortraitSpriteMap's first speaker: the speakers that are no playable
// character are numbered from it.
#define PORTRAIT_SPRITE_MAP_FIRST 128

#endif
