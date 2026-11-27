// The screens' tables from the save summary's level label to the range icons' patterns: the name
// entry's marks and labels, the Djinn and party sprites, the summons' abilities and the window
// sprites' motions.

#include "ability_ids.h"
#include "sprite_ids.h"
#include "game/game.h"
#include "window/screens.h"

// The save summary's level label; German's fills a word.
#if LANGUAGE == LANGUAGE_GERMAN
const Uint8 StatusMenuLevelLetterString[4] = "L";
#elif LANGUAGE == LANGUAGE_SPANISH
const Uint8 StatusMenuLevelLetterString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 StatusMenuLevelLetterString[] = "Niv";
#else
const Uint8 StatusMenuLevelLetterString[] = "L";
#endif

// clang-format off
// The kana that take a dakuten, as ranges of first and last font code

// (charmap_tla_ja.tsv), ending at 0.
const Uint8 NameEntryVoicedMarkRanges[] = {
    182, 196, // カ to ト
    202, 206, // ハ to ホ
    150, 159, // か to そ
    224, 228, // た to と
    234, 238, // は to ほ
    147, 147, // う
    179, 179, // ウ
    0,
};

// The kana that take a handakuten, as NameEntryVoicedMarkRanges.
const Uint8 NameEntrySemivoicedMarkRanges[] = {
    202, 206, // ハ to ホ
    234, 238, // は to ほ
    0,
};
// clang-format on

// The name entry caret's sway, in pixels by tick.
const Sint8 NameEntryCaretWave[8] = {0, 1, 2, 3, 3, 2, 1, 0};

// The name entry's labels for its delete and end keys in German, Spanish and French.
#if LANGUAGE == LANGUAGE_GERMAN
const Uint8 NameEntryDeleteKeyLabel[] = "ENT";
const Uint8 NameEntryEndKeyLabel[]    = "ENDE";
#elif LANGUAGE == LANGUAGE_SPANISH
const Uint8 NameEntryDeleteKeyLabel[] = "SUP";
const Uint8 NameEntryEndKeyLabel[]    = "FIN";
#elif LANGUAGE == LANGUAGE_FRENCH
const Uint8 NameEntryDeleteKeyLabel[] = "EFF";
const Uint8 NameEntryEndKeyLabel[]    = "FIN";
#endif

// The Djinni sprite of each element.
const Sint16 DjinnElementSprites[ELEMENT_COUNT] = {
    SPRITE_VENUS_DJINNI,
    SPRITE_MERCURY_DJINNI,
    SPRITE_MARS_DJINNI,
    SPRITE_JUPITER_DJINNI,
};

// Each party member's sprite.
const Sint16 CharacterSpriteTable[8] = {
    SPRITE_ISAAC,
    SPRITE_GARET,
    SPRITE_IVAN,
    SPRITE_MIA,
    SPRITE_FELIX,
    SPRITE_JENNA,
    SPRITE_SHEBA,
    SPRITE_PIERS,
};

// The ability of each summon, by its number (mnSummonShowLearnedMessage names it).
const Uint16 SummonAbilities[SUMMON_END] = {
    ABILITY_VENUS,   ABILITY_MERCURY,     ABILITY_MARS,     ABILITY_JUPITER,   ABILITY_RAMSES,
    ABILITY_NEREID,  ABILITY_KIRIN,       ABILITY_ATALANTA, ABILITY_CYBELE,    ABILITY_NEPTUNE,
    ABILITY_TIAMAT,  ABILITY_PROCNE,      ABILITY_JUDGMENT, ABILITY_BOREAS,    ABILITY_METEOR,
    ABILITY_THOR,    ABILITY_ZAGAN,       ABILITY_MEGAERA,  ABILITY_FLORA,     ABILITY_MOLOCH,
    ABILITY_ULYSSES, ABILITY_HAURES,      ABILITY_ECLIPSE,  ABILITY_COATLICUE, ABILITY_DAEDALUS,
    ABILITY_AZUL,    ABILITY_CATASTROPHE, ABILITY_CHARON,   ABILITY_IRIS,
};

// Unused: each summon's element, by its number, which nothing reads.
const Uint8 SummonUnusedElements[SUMMON_END] = {
    ELEMENT_VENUS,   ELEMENT_MERCURY, ELEMENT_MARS,    ELEMENT_JUPITER, ELEMENT_VENUS,
    ELEMENT_MERCURY, ELEMENT_MARS,    ELEMENT_JUPITER, ELEMENT_VENUS,   ELEMENT_MERCURY,
    ELEMENT_MARS,    ELEMENT_JUPITER, ELEMENT_VENUS,   ELEMENT_MERCURY, ELEMENT_MARS,
    ELEMENT_JUPITER, ELEMENT_VENUS,   ELEMENT_MARS,    ELEMENT_JUPITER, ELEMENT_MERCURY,
    ELEMENT_MARS,    ELEMENT_VENUS,   ELEMENT_JUPITER, ELEMENT_MERCURY, ELEMENT_MARS,
    ELEMENT_MERCURY, ELEMENT_JUPITER, ELEMENT_VENUS,   ELEMENT_MARS,
};

// A window sprite pair's bob, in pixels, one step every four ticks.
const Uint8 MenuTitleBobOffsets[8]     = {0, 0, 1, 2, 3, 3, 2, 1};

// A window sprite's pulse, its scale one step every two ticks: about 1.10 up to 1.22 and back.
const FIXED WindowSpritePulseScales[8] = {
    toFIXED(1.1),
    toFIXED(1.13),
    toFIXED(1.16),
    toFIXED(1.19),
    toFIXED(1.22),
    toFIXED(1.19),
    toFIXED(1.16),
    toFIXED(1.13),
};

// A row of 4-bit pixels for the centre tile and one for the end tile of a Psynergy range icon,
// colour 5 where the pattern marks (mnRangeIconBuildTiles); a word's lowest nibble is its leftmost
// pixel.
const BAR_PATTERN_ROW MenuBarPatternRows[6] = {
    {0x00055000, 0x00000000},
    {0x05055050, 0x00000000},
    {0x05055050, 0x00000005},
    {0x05055050, 0x00000505},
    {0x05055050, 0x00050505},
    {0x05055050, 0x05050505},
};
