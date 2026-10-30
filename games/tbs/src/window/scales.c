/// @file
/// Sprite scales, motion curves and lookup tables of the window and menu code.

#include "module/window/scales.h"
#include "types.h"
#include "character_ids.h"
#include "sprite_ids.h"
#include "module/window/window.h"

/// A sprite's pulse, as a scale where 256 is its normal size: it swells to
/// 1.5 times and back over 32 steps. The swelling sprites loop over it, the
/// shrinking ones take every other step of its second half
/// (WindowSprite_UpdateScaleAnimation), and the command rows' chosen icon
/// pulses by it (screens/battle_command_row.c, icon_menu.c).
// clang-format off
const u16 gWindowSprite_BounceScales[32] = {
    256, 256, 260, 266, 274, 284, 294, 306,
    320, 332, 344, 354, 364, 372, 378, 382,
    384, 382, 378, 372, 364, 354, 344, 332,
    320, 306, 294, 284, 274, 266, 260, 256,
};

/// Nothing in the game reads these eight offsets.
const s8 gWindowSprite_BounceScalesTail[8] = {
    -15, -28, -36, -40, -36, -28, -15, 0,
};

/// The selection cursor's bob: a 16-step wave of up to 3 pixels each way. The
/// selection screen's hand takes a step every 4 frames, its large icon half
/// the offset a step every 2.
const s8 gMenuCursor_BobOffsets[16] = {
    0, 1, 2, 3, 2, 1, 0, 0, 0, -1, -2, -3, -2, -1, 0, 0,
};

/// The window colour wheel: one colour channel's level (0 to 31) over 96 hue
/// steps. Red reads it at the hue, green 32 steps later and blue 64 steps
/// later (WindowColor_Apply); a channel falls from 31 over 32 steps, stays
/// 0 for 34 and climbs back. Nothing reads the 25 entries after the first 96.
const u8 gWindowColor_WaveTable[121] = {
    31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    0, 3, 5, 8, 10, 13, 16, 18, 21, 23, 26, 28, 31, 34, 36, 39,
    41, 44, 47, 49, 52, 54, 57, 60, 62,
};

/// The settings menu's command icons for each setting's choices: message
/// speed Slow, Normal and Fast; Speech Off and On; Auto-Sleep Off and On.
/// Each is a command icon number for CommandIcon_Create, in the order of the
/// setting's values.
const s8 gSettingsMenu_SpeedIcons[3] = {
    COMMAND_ICON_SPEED_SLOW,
    COMMAND_ICON_SPEED_NORMAL,
    COMMAND_ICON_SPEED_FAST,
};

const s8 gSettingsMenu_SpeechIcons[2] = {
    COMMAND_ICON_SPEECH_OFF,
    COMMAND_ICON_SPEECH_ON,
};

const s8 gSettingsMenu_SleepIcons[2] = {
    COMMAND_ICON_AUTO_SLEEP_OFF,
    COMMAND_ICON_AUTO_SLEEP_ON,
};

/// The window colour presets SELECT cycles through on the settings menu: the
/// colour (hue, 0 to 23) and the brightness (0 to 14, 7 neutral) of each.
const u8 gSettingsMenu_HuePresets[6] = {
    4, 16, 8, 21, 1, 13,
};

const u8 gSettingsMenu_BrightnessPresets[6] = {
    0, 11, 13, 7, 14, 14,
};

/// The command icon of each entry in the Pause menu (Save your quest,
/// Sleep, Change settings, and the debug entries after them). The menu shows
/// at most six; nothing reads the last two.
const s8 gPauseMenu_Icons[8] = {
    COMMAND_ICON_SAVE_QUEST, COMMAND_ICON_SLEEP, COMMAND_ICON_CHANGE_SETTINGS,
    COMMAND_ICON_DROP,       COMMAND_ICON_DROP,  COMMAND_ICON_DROP,
    0,                       0,
};

/// {character, portrait} pairs of the playable characters, ending in -1, which
/// Portrait_GetForSpeaker searches for speakers below 20.
const s16 gPortrait_CharacterMap[20] = {
    ISAAC, PORTRAIT_ISAAC,
    SPRITE_ISAAC_CHILD, PORTRAIT_ISAAC_CHILD,
    GARET, PORTRAIT_GARET,
    SPRITE_GARET_CHILD, PORTRAIT_GARET_CHILD,
    IVAN, PORTRAIT_IVAN,
    MIA, PORTRAIT_MIA,
    FELIX, PORTRAIT_FELIX,
    JENNA, PORTRAIT_JENNA,
    SHEBA, PORTRAIT_SHEBA,
    -1, -1,
};

/// {sprite, portrait} pairs of the other speakers, ending in -1: portrait n
/// of this list is portrait 128 + n, the row PORTRAIT_16 + n.
const s16 gPortrait_SpriteMap[100] = {
    SPRITE_JUPITER_DJINNI, 0,
    SPRITE_VENUS_DJINNI, 1,
    SPRITE_MARS_DJINNI, 2,
    SPRITE_MERCURY_DJINNI, 3,
    SPRITE_KRADEN, 4,
    SPRITE_SATUROS, 5,
    SPRITE_MENARDI, 6,
    SPRITE_ALEX, 7,
    SPRITE_DORA, 8,
    SPRITE_KYLE, 9,
    SPRITE_WOMAN_IN_GREEN_HEADSCARF, 10,
    SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, 11,
    SPRITE_BOY_WITH_ORANGE_HAIR, 12,
    SPRITE_MAN_IN_TEAL_SHIRT, 13,
    SPRITE_MCCOY, 14,
    SPRITE_WOMAN_WITH_PURPLE_HAIR, 15,
    SPRITE_TRET_RAISED, 16,
    SPRITE_LAUREL, 17,
    SPRITE_NYUNPA, 18,
    SPRITE_HAMMET, 19,
    SPRITE_LAYANA, 20,
    SPRITE_FEH, 21,
    SPRITE_FEIZHI, 22,
    SPRITE_HSU, 23,
    SPRITE_DONPA, 24,
    SPRITE_DODONPA, 25,
    SPRITE_BABI, 26,
    SPRITE_IODEM, 27,
    SPRITE_HAMA, 28,
    SPRITE_KAJA, 29,
    SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, 30,
    SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, 31,
    SPRITE_YOUNG_MAN_WITH_RED_HAIR, 32,
    SPRITE_MAN_IN_TALL_HAT, 33,
    SPRITE_BEARDED_MAN_IN_LONG_ROBE, 34,
    SPRITE_BALD_MAN_IN_RED, 35,
    SPRITE_YOUNG_MAN_IN_GREEN, 36,
    SPRITE_WOMAN_WITH_PONYTAIL, 37,
    SPRITE_WOMAN_IN_PINK_APRON, 38,
    SPRITE_MAN_WITH_DARK_HAIR, 39,
    SPRITE_ISAAC_CHILD, 40,
    SPRITE_GARET_CHILD, 41,
    SPRITE_FELIX_CHILD, 42,
    SPRITE_JENNA_CHILD, 43,
    SPRITE_FELIX_MASKED, 44,
    SPRITE_TRET, 45,
    SPRITE_FELIX, 46,
    SPRITE_INVISIBLE_BABI, 47,
    SPRITE_FARAN, 48,
    -1, -1,
};
// clang-format on
