// Lookup tables of field Psynergy after Burst's glow: Parch's picture map, the sprite shapes' sizes
// and the darkening maps of the colour ramp.

#include "window/window.h"

// clang-format off

// The screen entries of Parch's 16x16-tile picture (fdBgMapDrawClipped16x16), row by row: tile numbers with their flip bits.
const Uint16 ParchPictureTiles[256] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    64, 65, 66, 67, 68, 69, 70, 71,
    8, 9, 10, 11, 12, 13, 14, 15,
    72, 73, 74, 75, 76, 77, 78, 79,
    16, 17, 18, 19, 20, 21, 22, 23,
    80, 81, 82, 83, 84, 85, 86, 87,
    24, 25, 26, 27, 28, 29, 30, 31,
    88, 89, 90, 91, 92, 93, 94, 95,
    32, 33, 34, 35, 36, 37, 38, 39,
    96, 97, 98, 99, 100, 101, 102, 103,
    40, 41, 42, 43, 44, 45, 46, 47,
    104, 105, 106, 107, 108, 109, 110, 111,
    48, 49, 50, 51, 52, 53, 54, 55,
    112, 113, 114, 115, 116, 117, 118, 119,
    56, 57, 58, 59, 60, 61, 62, 63,
    120, 121, 122, 123, 124, 125, 126, 127,
    FLIP_XY(127), FLIP_XY(126), FLIP_XY(125), FLIP_XY(124), FLIP_XY(123), FLIP_XY(122), FLIP_XY(121), FLIP_XY(120),
    FLIP_XY(63), FLIP_XY(62), FLIP_XY(61), FLIP_XY(60), FLIP_XY(59), FLIP_XY(58), FLIP_XY(57), FLIP_XY(56),
    FLIP_XY(119), FLIP_XY(118), FLIP_XY(117), FLIP_XY(116), FLIP_XY(115), FLIP_XY(114), FLIP_XY(113), FLIP_XY(112),
    FLIP_XY(55), FLIP_XY(54), FLIP_XY(53), FLIP_XY(52), FLIP_XY(51), FLIP_XY(50), FLIP_XY(49), FLIP_XY(48),
    FLIP_XY(111), FLIP_XY(110), FLIP_XY(109), FLIP_XY(108), FLIP_XY(107), FLIP_XY(106), FLIP_XY(105), FLIP_XY(104),
    FLIP_XY(47), FLIP_XY(46), FLIP_XY(45), FLIP_XY(44), FLIP_XY(43), FLIP_XY(42), FLIP_XY(41), FLIP_XY(40),
    FLIP_XY(103), FLIP_XY(102), FLIP_XY(101), FLIP_XY(100), FLIP_XY(99), FLIP_XY(98), FLIP_XY(97), FLIP_XY(96),
    FLIP_XY(39), FLIP_XY(38), FLIP_XY(37), FLIP_XY(36), FLIP_XY(35), FLIP_XY(34), FLIP_XY(33), FLIP_XY(32),
    FLIP_XY(95), FLIP_XY(94), FLIP_XY(93), FLIP_XY(92), FLIP_XY(91), FLIP_XY(90), FLIP_XY(89), FLIP_XY(88),
    FLIP_XY(31), FLIP_XY(30), FLIP_XY(29), FLIP_XY(28), FLIP_XY(27), FLIP_XY(26), FLIP_XY(25), FLIP_XY(24),
    FLIP_XY(87), FLIP_XY(86), FLIP_XY(85), FLIP_XY(84), FLIP_XY(83), FLIP_XY(82), FLIP_XY(81), FLIP_XY(80),
    FLIP_XY(23), FLIP_XY(22), FLIP_XY(21), FLIP_XY(20), FLIP_XY(19), FLIP_XY(18), FLIP_XY(17), FLIP_XY(16),
    FLIP_XY(79), FLIP_XY(78), FLIP_XY(77), FLIP_XY(76), FLIP_XY(75), FLIP_XY(74), FLIP_XY(73), FLIP_XY(72),
    FLIP_XY(15), FLIP_XY(14), FLIP_XY(13), FLIP_XY(12), FLIP_XY(11), FLIP_XY(10), FLIP_XY(9), FLIP_XY(8),
    FLIP_Y(0), FLIP_Y(1), FLIP_Y(2), FLIP_Y(3), FLIP_Y(4), FLIP_Y(5), FLIP_Y(6), FLIP_Y(7),
    FLIP_Y(64), FLIP_Y(65), FLIP_Y(66), FLIP_Y(67), FLIP_Y(68), FLIP_Y(69), FLIP_Y(70), FLIP_Y(71),
};

// {width, height} in pixels of each sprite shape and size, by shape * 4 + size: square, wide, then tall.
const Uint8 OamShapeSizes[12][2] = {
    {8, 8},   {16, 16}, {32, 32}, {64, 64},
    {16, 8},  {32, 8},  {32, 16}, {64, 32},
    {8, 16},  {8, 32},  {16, 32}, {32, 64},
};

// The colour ramp from 224 to 255 darkened by one, two and four levels: each colour's index one, two or four steps down, 0 below 224.
const Uint8 ColorBufferDarkenOneLevelMap[32] = {
    0, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
    239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
};

// As ColorBufferDarkenOneLevelMap, two steps down.
const Uint8 ColorBufferDarkenTwoLevelsMap[32] = {
    0, 0, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237,
    238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
};

// As ColorBufferDarkenOneLevelMap, four steps down.
const Uint8 ColorBufferDarkenFourLevelsMap[32] = {
    0, 0, 0, 0, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235,
    236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
};

// Unused: 41 rows of four words, each 2, two numbers and -1, after the darkening maps; nothing names them.
const Sint32 FieldPsynergyUnusedRows[41][4] = {
    {2, 2, 10, -1},
    {2, 4, 21, -1},
    {2, 5, 23, -1},
    {2, 6, 34, -1},
    {2, 7, 34, -1},
    {2, 8, 43, -1},
    {2, 9, 44, -1},
    {2, 12, 54, -1},
    {2, 13, 54, -1},
    {2, 14, 55, -1},
    {2, 15, 59, -1},
    {2, 16, 82, -1},
    {2, 19, 67, -1},
    {2, 20, 95, -1},
    {2, 21, 102, -1},
    {2, 22, 102, -1},
    {2, 23, 105, -1},
    {2, 25, 107, -1},
    {2, 26, 107, -1},
    {2, 27, 110, -1},
    {2, 28, 131, -1},
    {2, 29, 131, -1},
    {2, 30, 131, -1},
    {2, 31, 131, -1},
    {2, 31, 131, -1},
    {2, 33, 139, -1},
    {2, 35, 143, -1},
    {2, 36, 154, -1},
    {2, 37, 158, -1},
    {2, 38, 170, -1},
    {2, 39, 174, -1},
    {2, 40, 182, -1},
    {2, 41, 189, -1},
    {2, 45, 199, -1},
    {2, 46, 206, -1},
    {2, 47, 139, -1},
    {2, 48, 139, -1},
    {2, 49, 139, -1},
    {2, 50, 139, -1},
    {2, 51, 139, -1},
    {2, 19, 95, -1},
};
// clang-format on
