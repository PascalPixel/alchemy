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
const s8 gMenu_WorkspaceIcons[8] = {
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

/// The debug warp presets' names (window/keyboard.s), last first.
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
    ISAAC, 0,
    GARET, 1,
    IVAN, 2,
    MIA, 3,
    FELIX, 4,
    JENNA, 5,
    SHEBA, 6,
    PIERS, 8,
    -1, -1,
};

/// {sprite, portrait} pairs of the other speakers, ending in -1.
const s16 gPortrait_SpriteMap[] = {
    SPRITE_JUPITER_DJINNI, 0,
    SPRITE_VENUS_DJINNI, 1,
    SPRITE_MARS_DJINNI, 2,
    SPRITE_MERCURY_DJINNI, 3,
    SPRITE_HEALER, 14,
    SPRITE_BEARDED_MAN_IN_LONG_ROBE, 15,
    SPRITE_BALD_MAN_IN_RED, 16,
    SPRITE_YOUNG_MAN_IN_GREEN, 17,
    SPRITE_WOMAN_WITH_PONYTAIL, 18,
    SPRITE_WOMAN_IN_PINK_APRON, 19,
    SPRITE_MAN_WITH_DARK_HAIR, 20,
    SPRITE_WEAPON_MERCHANT, 54,
    SPRITE_ARMOR_MERCHANT, 55,
    SPRITE_WOMAN_WITH_TEAL_HEADBAND, 56,
    SPRITE_INNKEEPER, 57,
    SPRITE_INN_WORKER, 58,
    SPRITE_WOMAN_WITH_ORANGE_HAIR, 59,
    SPRITE_ARTIFACT_MERCHANT, 60,
    SPRITE_KRADEN, 4,
    SPRITE_KRADEN_SMALL, 4,
    SPRITE_ALEX, 7,
    SPRITE_POI, 30,
    SPRITE_RIKI, 31,
    SPRITE_TAVI, 32,
    SPRITE_MADRAS_MAYOR, 33,
    SPRITE_MADRAS_ELDER, 34,
    SPRITE_ALHAFRAS_MAYOR, 35,
    SPRITE_FORTUNE_TELLER, 36,
    SPRITE_AKAFUBU, 37,
    SPRITE_AKAFUBU_WITCH_DOCTOR, 37,
    SPRITE_MOAPA, 38,
    SPRITE_MAHA, 39,
    SPRITE_JENNAS_FATHER, 40,
    SPRITE_JENNAS_MOTHER, 41,
    SPRITE_BRIGGS, 42,
    SPRITE_CHAUCHA, 43,
    SPRITE_EOLEO, 44,
    SPRITE_OBABA, 45,
    SPRITE_SUSA, 46,
    SPRITE_LADY_UZUME, 47,
    SPRITE_KUSHINADA, 48,
    SPRITE_HYDROS, 49,
    SPRITE_CONSERVATO, 50,
    SPRITE_KARST, 51,
    SPRITE_AGATIO, 52,
    SPRITE_KYLE, 9,
    SPRITE_LUNPA, 61,
    SPRITE_PUELLE, 62,
    SPRITE_MASKED_SHAMAN, 63,
    SPRITE_PROX_ELDER, 64,
    SPRITE_SUNSHINE, 65,
    SPRITE_MAN_WITH_ORANGE_BEARD, 66,
    SPRITE_GIRL_WITH_LONG_BLONDE_HAIR, 67,
    SPRITE_IZUMO_WOMAN_IN_CREAM, 68,
    SPRITE_IZUMO_MAN_IN_RED, 69,
    SPRITE_IZUMO_WOMAN_IN_ORANGE, 70,
    SPRITE_FEIZHI, 73,
    SPRITE_HAMMA, 74,
    SPRITE_DORA, 8,
    SPRITE_WOMAN_IN_GREEN_HEADSCARF, 10,
    SPRITE_MAN_IN_TEAL_SHIRT, 13,
    SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, 71,
    SPRITE_YOUNG_MAN_WITH_RED_HAIR, 72,
    SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, 11,
    SPRITE_BOY_WITH_ORANGE_HAIR, 76,
    -1, -1,
};
// clang-format on
