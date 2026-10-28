/// @file
/// Sprite scales, motion curves and lookup tables of the window and menu code.

#include "module/window/scales.h"
#include "types.h"
#include "character_ids.h"
#include "module/window/window.h"

/// A sprite's pulse, as a scale where 256 is its normal size: it swells to
/// 1.5 times and back over 32 steps. The swelling sprites loop over it, the
/// shrinking ones take every other step of its second half
/// (WindowSprite_UpdateScaleAnimation), and the command rows' chosen icon
/// pulses by it (screens/command_row.c, icon_menu.c).
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
    ISAAC, ISAAC,
    ISAAC_CHILD, FACE_ISAAC_CHILD,
    GARET, GARET,
    GARET_CHILD, FACE_GARET_CHILD,
    IVAN, IVAN,
    MIA, MIA,
    FELIX, FELIX,
    JENNA, JENNA,
    SHEBA, SHEBA,
    -1, -1,
};

/// {sprite, portrait} pairs of the other speakers, ending in -1: portrait n
/// of this list is portrait 128 + n.
const s16 gPortrait_SpriteMap[100] = {
    93, 0,
    90, 1,
    92, 2,
    91, 3,
    30, 4,
    32, 5,
    33, 6,
    43, 7,
    31, 8,
    53, 9,
    38, 10,
    42, 11,
    37, 12,
    54, 13,
    58, 14,
    69, 15,
    71, 16,
    56, 17,
    51, 18,
    49, 19,
    45, 20,
    40, 21,
    39, 22,
    41, 23,
    44, 24,
    50, 25,
    52, 26,
    57, 27,
    61, 28,
    47, 29,
    46, 30,
    66, 31,
    67, 32,
    121, 33,
    122, 34,
    124, 35,
    125, 36,
    118, 37,
    128, 38,
    119, 39,
    18, 40,
    19, 41,
    36, 42,
    48, 43,
    34, 44,
    55, 45,
    35, 46,
    65, 47,
    70, 48,
    -1, -1,
};
// clang-format on
