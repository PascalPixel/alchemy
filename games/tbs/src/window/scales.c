#include "unit/window/scales.h"
#include "types.h"
#include "character_ids.h"

// 拡大率 (256 = 1 倍)
// clang-format off
const u16 gUi_BounceScales[32] = {
    256, 256, 260, 266, 274, 284, 294, 306,
    320, 332, 344, 354, 364, 372, 378, 382,
    384, 382, 378, 372, 364, 354, 344, 332,
    320, 306, 294, 284, 274, 266, 260, 256,
};

const s8 gUi_BounceScalesTail[8] = {
    -15, -28, -36, -40, -36, -28, -15, 0,
};

const s8 gMenu_CursorBobOffsets[16] = {
    0, 1, 2, 3, 2, 1, 0, 0, 0, -1, -2, -3, -2, -1, 0, 0,
};

// 光の明滅
const u8 gPaletteGlow_WaveTable[121] = {
    31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    0, 3, 5, 8, 10, 13, 16, 18, 21, 23, 26, 28, 31, 34, 36, 39,
    41, 44, 47, 49, 52, 54, 57, 60, 62,
};

// 設定画面の絵番号
const s8 gOptionMenu_SpeedFrames[3] = {
    0x22, 0x23, 0x24,
};

const s8 gOptionMenu_SpeechFrames[2] = {
    0x25, 0x26,
};

const s8 gOptionMenu_SleepFrames[2] = {
    0x27, 0x28,
};

// 窓の色の組
const u8 gOptionMenu_FirstColorPresets[6] = {
    4, 16, 8, 21, 1, 13,
};

const u8 gOptionMenu_SecondColorPresets[6] = {
    0, 11, 13, 7, 14, 14,
};

const s8 gMenu_WorkspaceIconFrames[8] = {
    0x29, 0x2a, 0x2b, 0x0b, 0x0b, 0x0b, 0x00, 0x00,
};

// {絵の番号, 顔の番号} の組, -1 で終わり
const s16 gPortrait_CharacterMap[20] = {
    ISAAC, ISAAC,           /* ロビン */
    ISAAC_CHILD, FACE_ISAAC_CHILD,  /* ロビン(こ) */
    GARET, GARET,           /* ジェラルド */
    GARET_CHILD, FACE_GARET_CHILD,  /* ジェラルド(こ) */
    IVAN, IVAN,             /* イワン */
    MIA, MIA,           /* メアリー */
    FELIX, FELIX,       /* ガルシア */
    JENNA, JENNA,       /* ジャスミン */
    SHEBA, SHEBA,           /* シバ */
    -1, -1,
};

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
