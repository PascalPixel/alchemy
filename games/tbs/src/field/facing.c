// Which picture a sprite shows for the way it faces, and the small tables the actors read as they
// move: the direction pad's headings, the World Map camera's lean, a floating actor's bob, how long
// a footprint lasts and the order a sprite dissolves in.

#include "field/facing.h"
#include "field/actor_script.h"

// clang-format off
// Facing tables: for each ANIMATION_FACING_KIND, the picture to draw for each sector of the facing angle.
const Uint8 AnimationFacingOnePicture[16] = {
    0, 0, 0, MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0), 0,
    MIRRORED(0), MIRRORED(0), MIRRORED(0), 0, 0, 0, 0, MIRRORED(0),
};
const Uint8 AnimationFacingFrontBackSwapped[8] = {
    MIRRORED(0), MIRRORED(0), 0, 0, 1, 1, MIRRORED(1), MIRRORED(1),
};
const Uint8 AnimationFacingFrontBack[8] = {
    0, 0, MIRRORED(0), MIRRORED(0), 1, 1, MIRRORED(1), MIRRORED(1),
};
const Uint8 AnimationFacingThreePictures[16] = {
    1, 1, 0, 0, MIRRORED(0), MIRRORED(0), MIRRORED(1), MIRRORED(1),
    MIRRORED(1), MIRRORED(1), MIRRORED(2), MIRRORED(2), 2, 2, 2, 1,
};
const Uint8 AnimationFacingFivePictures[16] = {
    1, 0, 0, 3, 3, MIRRORED(0), MIRRORED(0), MIRRORED(1),
    MIRRORED(1), MIRRORED(2), MIRRORED(2), 4, 4, 2, 2, 1,
};
const Uint8 AnimationFacingEightPictures[8] = {
    2, 1, 0, 7, 6, 5, 4, 3,
};
const Uint8 AnimationFacingEightPicturesMirrored[8] = {
    1, 0, 3, MIRRORED(5), MIRRORED(6), MIRRORED(7), 4, 2,
};
const Uint8 AnimationFacingFourPictures[64] = {
    1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0),
    MIRRORED(0), MIRRORED(1), MIRRORED(1), MIRRORED(1), MIRRORED(1), MIRRORED(1), MIRRORED(1), MIRRORED(1),
    MIRRORED(1), MIRRORED(2), MIRRORED(2), MIRRORED(2), MIRRORED(2), MIRRORED(2), MIRRORED(2), MIRRORED(3),
    MIRRORED(3), MIRRORED(3), MIRRORED(3), MIRRORED(3), MIRRORED(3), MIRRORED(3), MIRRORED(3), MIRRORED(3),
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 2, 2, 2, 2, 2, 2, 1,
};
const Uint8 AnimationFacingSixPictures[64] = {
    1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 4,
    4, MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0),
    MIRRORED(0), MIRRORED(1), MIRRORED(1), MIRRORED(1), MIRRORED(1), MIRRORED(1), MIRRORED(1), MIRRORED(1),
    MIRRORED(1), MIRRORED(2), MIRRORED(2), MIRRORED(2), MIRRORED(2), MIRRORED(2), MIRRORED(2), MIRRORED(3),
    MIRRORED(3), MIRRORED(3), MIRRORED(3), MIRRORED(3), MIRRORED(3), MIRRORED(3), MIRRORED(3), 5,
    5, 3, 3, 3, 3, 3, 3, 3,
    3, 2, 2, 2, 2, 2, 2, 1,
};

// The order in which the 64 pixels of a sprite's tile in OBJ VRAM dissolve; read by fdSpriteDissolveStep (actors.c).
const Uint8 SpriteDissolvePixelOrder[64] = {
    4, 63, 12, 10, 44, 5, 1, 60, 46, 41, 3, 23, 29, 55, 51, 15,
    50, 25, 26, 57, 19, 49, 16, 61, 8, 58, 37, 17, 27, 31, 59, 53,
    11, 56, 33, 7, 30, 35, 36, 32, 42, 62, 21, 13, 2, 22, 45, 14,
    6, 48, 52, 9, 43, 28, 0, 40, 24, 34, 20, 47, 54, 39, 38, 18,
};

// Two bytes after the dissolve order; no code in The Broken Seal names them.
const Uint16 SpriteDissolvePixelOrderTail = 511;

// The fixed camera's view matrix (16.16): identity with a -0.8 shear of y by x. fdActorSystemProjectSprites applies it while battle commands are chosen.
const FIXED CameraFixedViewMatrix[12] = {
    toFIXED(1), 0, 0,
    -(toFIXED(4) / 5), toFIXED(1), 0,
    0, 0, toFIXED(1),
    0, 0, 0,
};

// One period of a triangle wave, which bobs a floating actor up and down.
const Sint32 ActorScriptCycleTable[32] = {
    1, 2, 3, 4, 5, 6, 7, 8,
    7, 6, 5, 4, 3, 2, 1, 0,
    -1, -2, -3, -4, -5, -6, -7, -8,
    -7, -6, -5, -4, -3, -2, -1, 0,
};

// The idle script an actor returns to when its own one ends: a lone SC_END, which reloads it. fdActorScriptEnd (SC_END) points an actor here.
const Sint32 ActorScriptEndScript[1] = {
    SC_END,
};

// Halfwords that follow the idle script; no code in The Broken Seal names them.
const Sint16 ActorScriptEndScriptTail[8] = {
    4096, -8192, 12288, -16384, 24576, -32768, 16384, 0,
};

// The heading for each state of the direction pad (bits 4-7 of the keys: Right, Left, Up, Down), in 1/65536 turns: Right is 0, Down DEGtoANG(90), Left DEGtoANG(180) and Up TRIG_TURN * 3 / 4; WORLD_MAP_KEY_HEADING_NONE means no direction is held, or only an opposite pair.
const Uint16 WorldMapKeyHeadings[16] = {
    WORLD_MAP_KEY_HEADING_NONE, 0, DEGtoANG(180), WORLD_MAP_KEY_HEADING_NONE,
    TRIG_TURN * 3 / 4, TRIG_TURN * 7 / 8, TRIG_TURN * 5 / 8, TRIG_TURN * 3 / 4,
    DEGtoANG(90), DEGtoANG(45), TRIG_TURN * 3 / 8, DEGtoANG(90),
    WORLD_MAP_KEY_HEADING_NONE, 0, DEGtoANG(180), WORLD_MAP_KEY_HEADING_NONE,
};

// Two scripts for the World Map's footprint and dust effects: wait 120 or 30 ticks, then release the actor.
const Sint32 WorldMapFootprintActions[6] = {
    SC_WAIT(120), SC_RELEASE,
    SC_WAIT(30), SC_RELEASE,
};

// The yaw the World Map camera leans to for each state of the direction pad, in 1/65536 turns: Right -512 and Left 512, or -384 and 384 with Up or Down but not both; 0 otherwise.
const Sint32 WorldMapKeyYaws[16] = {
    0, -512, 512, 0,
    0, -384, 384, 0,
    0, -384, 384, 0,
    0, 0, 0, 0,
};
// clang-format on
