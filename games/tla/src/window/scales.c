/// @file
/// Sprite scales, motion curves and lookup tables of the window and menu code,
/// the sound test's labels and the debug warps' names; The Broken Seal's
/// scales.c.

#include "sprite_ids.h"
#include "types.h"
#include "game/character_ids.h"
#include "window/settings_menu.h"
#include "window/window.h"
#include "macros.h"

// clang-format off

/// A sprite's pulse, as a scale where 256 is its normal size: it swells to 1.5
/// times and back over 32 steps (window_sprites.c).
const u16 gWindowSprite_BounceScales[32] = {
    256, 256, 260, 266, 274, 284, 294, 306,
    320, 332, 344, 354, 364, 372, 378, 382,
    384, 382, 378, 372, 364, 354, 344, 332,
    320, 306, 294, 284, 274, 266, 260, 256,
};

/// Nothing in the game reads these eight offsets, as in The Broken Seal.
const s8 gWindowSprite_BounceScalesTail[8] = {
    -15, -28, -36, -40, -36, -28, -15, 0,
};

/// The selection cursor's bob: a 16-step wave of up to 3 pixels each way.
const s8 gMenu_CursorBobOffsets[16] = {
    0, 1, 2, 3, 2, 1, 0, 0, 0, -1, -2, -3, -2, -1, 0, 0,
};

/// The window colour wheel: one colour channel's level (0 to 31) by hue step,
/// fading out, resting and fading in; The Broken Seal's 25 further steps are
/// gone.
const u8 gWindowColor_WaveTable[96] = {
    31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
};

/// The settings menu's command icons for each setting's choices: message speed
/// Slow, Normal and Fast.
const s8 gSettingsMenu_SpeedIcons[3] = {
    COMMAND_ICON_SPEED_SLOW,
    COMMAND_ICON_SPEED_NORMAL,
    COMMAND_ICON_SPEED_FAST,
};

/// Speech Off and On.
const s8 gSettingsMenu_SpeechIcons[2] = {
    COMMAND_ICON_SPEECH_OFF,
    COMMAND_ICON_SPEECH_ON,
};

/// Auto-Sleep Off and On.
const s8 gSettingsMenu_SleepIcons[2] = {
    COMMAND_ICON_AUTO_SLEEP_OFF,
    COMMAND_ICON_AUTO_SLEEP_ON,
};

/// The window colour presets SELECT cycles through on the settings menu: the
/// colour (hue, 0 to 23) of each.
const u8 gSettingsMenu_HuePresets[6] = {
    4, 16, 8, 21, 1, 13,
};

/// The brightness (0 to 14, 7 neutral) of each preset.
const u8 gSettingsMenu_BrightnessPresets[6] = {
    0, 11, 13, 7, 14, 14,
};

/// The command icon of each entry in the Pause menu (Save your quest, Sleep,
/// Change settings, and the debug entries after them).
const s8 gPauseMenu_Icons[8] = {
    COMMAND_ICON_SAVE_QUEST, COMMAND_ICON_SLEEP, COMMAND_ICON_CHANGE_SETTINGS,
    COMMAND_ICON_DROP,       COMMAND_ICON_DROP,  COMMAND_ICON_DROP,
    0,                       0,
};

// clang-format on

/// The sound test's starting music, jingle, sound effect and sound preset.
const struct SoundTestValues gDebugSoundTest_StartValues = {{0, 80, 100, 2}};

/// The sound test's row labels: music, jingle, sound effect and sound preset.
const u8 SoundTest_MusicString[] ALIGNED(4) = "Music";
const u8 SoundTest_JingleString[] ALIGNED(4) = "Jingle";
const u8 SoundTest_SeString[] ALIGNED(4) = "SE";
const u8 SoundTest_EffectString[] ALIGNED(4) = "FX";

/// The debug warp menu's row names (gDebugWarp_Presets), last first.
const u8 DebugWarp_KaoruSpecial1Name[] ALIGNED(4) = "8:Kaoru Special1";
const u8 DebugWarp_KaoruSpecial2Name[] ALIGNED(4) = "7:Kaoru Special2";
const u8 DebugWarp_KaoruSpecial3Name[] ALIGNED(4) = "6:Kaoru Special3";
const u8 DebugWarp_KaoruSpecial4Name[] ALIGNED(4) = "5:Kaoru Special4";
const u8 DebugWarp_Champa2Name[] ALIGNED(4) = "4:CHAMPA2";
const u8 DebugWarp_Arafura3Name[] ALIGNED(4) = "3:Arafura3";
const u8 DebugWarp_EnergySampleName[] ALIGNED(4) = "2:Energy Sample";
const u8 DebugWarp_Arafura2Name[] ALIGNED(4) = "1:Arafura2";
const u8 DebugWarp_JupiterName[] ALIGNED(4) = "0:Jupiter";

/// An empty ability's name, which the debug menu draws.
const u8 gDebugShortcut_NoAbilityName[] ALIGNED(4) = "----";

// clang-format off

/// {character, portrait} pairs of the playable characters, ending in -1, which
/// the speaker's portrait lookup searches first.
const s16 gPortrait_CharacterMap[18] ALIGNED(4) = {
    ISAAC, PORTRAIT_ISAAC,
    GARET, PORTRAIT_GARET,
    IVAN, PORTRAIT_IVAN,
    MIA, PORTRAIT_MIA,
    FELIX, PORTRAIT_FELIX,
    JENNA, PORTRAIT_JENNA,
    SHEBA, PORTRAIT_SHEBA,
    PIERS, PORTRAIT_PIERS_SPEAKING,
    -1, -1,
};

/// {sprite, portrait} pairs of the other speakers, ending in -1.
const s16 gPortrait_SpriteMap[] = {
    SPRITE_JUPITER_DJINNI, SPRITE_PORTRAIT_JUPITER_DJINNI,
    SPRITE_VENUS_DJINNI, SPRITE_PORTRAIT_VENUS_DJINNI,
    SPRITE_MARS_DJINNI, SPRITE_PORTRAIT_MARS_DJINNI,
    SPRITE_MERCURY_DJINNI, SPRITE_PORTRAIT_MERCURY_DJINNI,
    SPRITE_HEALER, SPRITE_PORTRAIT_MAN_IN_TALL_HAT,
    SPRITE_BEARDED_MAN_IN_LONG_ROBE, SPRITE_PORTRAIT_BEARDED_MAN_IN_LONG_ROBE,
    SPRITE_BALD_MAN_IN_RED, SPRITE_PORTRAIT_BALD_MAN_IN_RED,
    SPRITE_YOUNG_MAN_IN_GREEN, SPRITE_PORTRAIT_YOUNG_MAN_IN_GREEN,
    SPRITE_WOMAN_WITH_PONYTAIL, SPRITE_PORTRAIT_WOMAN_WITH_PONYTAIL,
    SPRITE_WOMAN_IN_PINK_APRON, SPRITE_PORTRAIT_WOMAN_IN_PINK_APRON,
    SPRITE_MAN_WITH_DARK_HAIR, SPRITE_PORTRAIT_MAN_WITH_DARK_HAIR,
    SPRITE_WEAPON_MERCHANT, SPRITE_PORTRAIT_WEAPON_MERCHANT,
    SPRITE_ARMOR_MERCHANT, SPRITE_PORTRAIT_ARMOR_MERCHANT,
    SPRITE_WOMAN_WITH_TEAL_HEADBAND, SPRITE_PORTRAIT_WOMAN_WITH_TEAL_HEADBAND,
    SPRITE_INNKEEPER, SPRITE_PORTRAIT_INNKEEPER,
    SPRITE_INN_WORKER, SPRITE_PORTRAIT_INN_WORKER,
    SPRITE_WOMAN_WITH_ORANGE_HAIR, SPRITE_PORTRAIT_WOMAN_WITH_ORANGE_HAIR,
    SPRITE_ARTIFACT_MERCHANT, SPRITE_PORTRAIT_ARTIFACT_MERCHANT,
    SPRITE_KRADEN, SPRITE_PORTRAIT_KRADEN,
    SPRITE_KRADEN_SMALL, SPRITE_PORTRAIT_KRADEN,
    SPRITE_ALEX, SPRITE_PORTRAIT_ALEX,
    SPRITE_POI, SPRITE_PORTRAIT_POI,
    SPRITE_RIKI, SPRITE_PORTRAIT_RIKI,
    SPRITE_TAVI, SPRITE_PORTRAIT_TAVI,
    SPRITE_MADRAS_MAYOR, SPRITE_PORTRAIT_MADRAS_MAYOR,
    SPRITE_MADRAS_ELDER, SPRITE_PORTRAIT_MADRAS_ELDER,
    SPRITE_ALHAFRAS_MAYOR, SPRITE_PORTRAIT_ALHAFRAS_MAYOR,
    SPRITE_FORTUNE_TELLER, SPRITE_PORTRAIT_FORTUNE_TELLER,
    SPRITE_AKAFUBU, SPRITE_PORTRAIT_AKAFUBU,
    SPRITE_AKAFUBU_WITCH_DOCTOR, SPRITE_PORTRAIT_AKAFUBU,
    SPRITE_MOAPA, SPRITE_PORTRAIT_MOAPA,
    SPRITE_MAHA, SPRITE_PORTRAIT_MAHA,
    SPRITE_JENNAS_FATHER, SPRITE_PORTRAIT_JENNAS_FATHER,
    SPRITE_JENNAS_MOTHER, SPRITE_PORTRAIT_JENNAS_MOTHER,
    SPRITE_BRIGGS, SPRITE_PORTRAIT_BRIGGS,
    SPRITE_CHAUCHA, SPRITE_PORTRAIT_CHAUCHA,
    SPRITE_EOLEO, SPRITE_PORTRAIT_EOLEO,
    SPRITE_OBABA, SPRITE_PORTRAIT_OBABA,
    SPRITE_SUSA, SPRITE_PORTRAIT_SUSA,
    SPRITE_LADY_UZUME, SPRITE_PORTRAIT_LADY_UZUME,
    SPRITE_KUSHINADA, SPRITE_PORTRAIT_KUSHINADA,
    SPRITE_HYDROS, SPRITE_PORTRAIT_HYDROS,
    SPRITE_CONSERVATO, SPRITE_PORTRAIT_CONSERVATO,
    SPRITE_KARST, SPRITE_PORTRAIT_KARST,
    SPRITE_AGATIO, SPRITE_PORTRAIT_AGATIO,
    SPRITE_KYLE, SPRITE_PORTRAIT_KYLE,
    SPRITE_LUNPA, SPRITE_PORTRAIT_LUNPA,
    SPRITE_PUELLE, SPRITE_PORTRAIT_PUELLE,
    SPRITE_MASKED_SHAMAN, SPRITE_PORTRAIT_MASKED_SHAMAN,
    SPRITE_PROX_ELDER, SPRITE_PORTRAIT_PROX_ELDER,
    SPRITE_SUNSHINE, SPRITE_PORTRAIT_SUNSHINE,
    SPRITE_MAN_WITH_ORANGE_BEARD, SPRITE_PORTRAIT_MAN_WITH_ORANGE_BEARD,
    SPRITE_GIRL_WITH_LONG_BLONDE_HAIR, SPRITE_PORTRAIT_GIRL_WITH_LONG_BLONDE_HAIR,
    SPRITE_IZUMO_WOMAN_IN_CREAM, SPRITE_PORTRAIT_IZUMO_WOMAN_IN_CREAM,
    SPRITE_IZUMO_MAN_IN_RED, SPRITE_PORTRAIT_IZUMO_MAN_IN_RED,
    SPRITE_IZUMO_WOMAN_IN_ORANGE, SPRITE_PORTRAIT_IZUMO_WOMAN_IN_ORANGE,
    SPRITE_FEIZHI, SPRITE_PORTRAIT_FEIZHI,
    SPRITE_HAMMA, SPRITE_PORTRAIT_HAMMA,
    SPRITE_DORA, SPRITE_PORTRAIT_DORA,
    SPRITE_WOMAN_IN_GREEN_HEADSCARF, SPRITE_PORTRAIT_WOMAN_IN_GREEN_HEADSCARF,
    SPRITE_MAN_IN_TEAL_SHIRT, SPRITE_PORTRAIT_MAN_IN_TEAL_SHIRT,
    // The two young men speak with an old man's and an old woman's portraits,
    // and the boy with orange hair with a row that holds none.
    SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, SPRITE_PORTRAIT_OLD_MAN_WITH_GREY_BEARD,
    SPRITE_YOUNG_MAN_WITH_RED_HAIR, SPRITE_PORTRAIT_OLD_WOMAN_WITH_GREY_HAIR,
    SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, SPRITE_PORTRAIT_WOMAN_WITH_ORANGE_PONYTAIL,
    SPRITE_BOY_WITH_ORANGE_HAIR, SPRITE_PORTRAIT_MISSING,
    -1, -1,
};
// clang-format on
