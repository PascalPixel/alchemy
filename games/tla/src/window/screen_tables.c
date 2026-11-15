/// @file
/// The screens' tables from the save summary's level label to the menu bars'
/// patterns: the name entry's marks and labels, the Djinn and party sprites,
/// the summons' abilities and the window sprites' motions.

#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "fixed_math.h"
#include "sprite_ids.h"
#include "battle/battle_unit.h"
#include "game/game.h"
#include "window/screens.h"

/// The save summary's level label; German's fills a word.
#if LANGUAGE == LANGUAGE_GERMAN
const u8 gStatusMenu_LevelLetterString[4] = "L";
#elif LANGUAGE == LANGUAGE_SPANISH
const u8 gStatusMenu_LevelLetterString[] = "Nv.";
#elif LANGUAGE == LANGUAGE_FRENCH
const u8 gStatusMenu_LevelLetterString[] = "Niv";
#else
const u8 gStatusMenu_LevelLetterString[] = "L";
#endif

/// The kana that take a dakuten, as ranges of first and last, ending at 0.
// clang-format off
const u8 gNameEntry_VoicedMarkRanges[] = {
    0xb6, 0xc4,
    0xca, 0xce,
    0x96, 0x9f,
    0xe0, 0xe4,
    0xea, 0xee,
    0x93, 0x93,
    0xb3, 0xb3,
    0x00,
};

/// The kana that take a handakuten, as gNameEntry_VoicedMarkRanges.
const u8 gNameEntry_SemivoicedMarkRanges[] = {
    0xca, 0xce,
    0xea, 0xee,
    0x00,
};
// clang-format on

/// The name entry caret's sway, in pixels by frame.
const s8 gNameEntry_CaretWave[8] = {0, 1, 2, 3, 3, 2, 1, 0};

#if LANGUAGE == LANGUAGE_GERMAN
const u8 gNameEntry_DeleteKeyLabel[] = "ENT";
const u8 gNameEntry_EndKeyLabel[] = "ENDE";
#elif LANGUAGE == LANGUAGE_SPANISH
const u8 gNameEntry_DeleteKeyLabel[] = "SUP";
const u8 gNameEntry_EndKeyLabel[] = "FIN";
#elif LANGUAGE == LANGUAGE_FRENCH
const u8 gNameEntry_DeleteKeyLabel[] = "EFF";
const u8 gNameEntry_EndKeyLabel[] = "FIN";
#endif

/// The Djinni sprite of each element.
const s16 gDjinn_ElementSprites[ELEMENT_COUNT] = {
    SPRITE_VENUS_DJINNI,
    SPRITE_MERCURY_DJINNI,
    SPRITE_MARS_DJINNI,
    SPRITE_JUPITER_DJINNI,
};

/// Each party member's sprite.
const s16 gCharacter_SpriteTable[8] = {
    SPRITE_ISAAC, SPRITE_GARET, SPRITE_IVAN,  SPRITE_MIA,
    SPRITE_FELIX, SPRITE_JENNA, SPRITE_SHEBA, SPRITE_PIERS,
};

/// The ability of each summon, by its number (Summon_ShowLearnedMessage
/// names it).
const u16 gSummon_Abilities[SUMMON_END] = {
    ABILITY_VENUS,    ABILITY_MERCURY, ABILITY_MARS,        ABILITY_JUPITER,
    ABILITY_RAMSES,   ABILITY_NEREID,  ABILITY_KIRIN,       ABILITY_ATALANTA,
    ABILITY_CYBELE,   ABILITY_NEPTUNE, ABILITY_TIAMAT,      ABILITY_PROCNE,
    ABILITY_JUDGMENT, ABILITY_BOREAS,  ABILITY_METEOR,      ABILITY_THOR,
    ABILITY_ZAGAN,    ABILITY_MEGAERA, ABILITY_FLORA,       ABILITY_MOLOCH,
    ABILITY_ULYSSES,  ABILITY_HAURES,  ABILITY_ECLIPSE,     ABILITY_COATLICUE,
    ABILITY_DAEDALUS, ABILITY_AZUL,    ABILITY_CATASTROPHE, ABILITY_CHARON,
    ABILITY_IRIS,
};

/// Unused: each summon's element, by its number, which nothing reads.
const u8 gSummon_UnusedElements[SUMMON_END] = {
    ELEMENT_VENUS, ELEMENT_MERCURY, ELEMENT_MARS,    ELEMENT_JUPITER,
    ELEMENT_VENUS, ELEMENT_MERCURY, ELEMENT_MARS,    ELEMENT_JUPITER,
    ELEMENT_VENUS, ELEMENT_MERCURY, ELEMENT_MARS,    ELEMENT_JUPITER,
    ELEMENT_VENUS, ELEMENT_MERCURY, ELEMENT_MARS,    ELEMENT_JUPITER,
    ELEMENT_VENUS, ELEMENT_MARS,    ELEMENT_JUPITER, ELEMENT_MERCURY,
    ELEMENT_MARS,  ELEMENT_VENUS,   ELEMENT_JUPITER, ELEMENT_MERCURY,
    ELEMENT_MARS,  ELEMENT_MERCURY, ELEMENT_JUPITER, ELEMENT_VENUS,
    ELEMENT_MARS,
};

/// A window sprite pair's bob, in pixels, one step every four frames.
const u8 gWindow_PairBobOffsets[8] = {0, 0, 1, 2, 3, 3, 2, 1};

/// A window sprite's pulse, its scale one step every two frames.
const s32 gWindowSprite_PulseScales[8] = {
    FIXED_ONE * 110 / 100, FIXED_ONE * 113 / 100, FIXED_ONE * 116 / 100,
    FIXED_ONE * 119 / 100, FIXED_ONE * 122 / 100, FIXED_ONE * 119 / 100,
    FIXED_ONE * 116 / 100, FIXED_ONE * 113 / 100,
};

/// 4bpp rows in colour 5 that grow by one pixel at a time.
const struct BarPatternRow gMenu_BarPatternRows[6] = {
    {0x00055000, 0x00000000}, {0x05055050, 0x00000000},
    {0x05055050, 0x00000005}, {0x05055050, 0x00000505},
    {0x05055050, 0x00050505}, {0x05055050, 0x05050505},
};
