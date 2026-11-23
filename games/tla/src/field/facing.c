/// @file
/// Which picture a sprite shows for the way it faces, and the small tables
/// and scripts the actors read as they move: the direction pad's headings,
/// the World Map camera's lean, a floating actor's bob, how long a footprint
/// lasts, the order a sprite dissolves in and the World Map's graphics sets.

#include "field/actor_script.h"
#include "types.h"
#include "field/object.h"
#include "fixed_math.h"
#include "resource_ids.h"

// clang-format off
// The facing tables: for each animation facing kind, the picture to draw for
// each sector of the facing angle, MIRRORED where the picture is drawn
// mirrored, as The Broken Seal's.

/// ANIM_FACING_ONE_PICTURE's table: one picture, mirrored to the left.
const u8 gAnimationFacing_OnePicture[16] = {
    0, 0, 0, MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0), 0,
    MIRRORED(0), MIRRORED(0), MIRRORED(0), 0, 0, 0, 0, MIRRORED(0),
};

/// ANIM_FACING_FRONT_BACK_SWAPPED's table: a front and a back picture, the
/// front mirrored the other way.
const u8 gAnimationFacing_FrontBackSwapped[8] = {
    MIRRORED(0), MIRRORED(0), 0, 0, 1, 1, MIRRORED(1), MIRRORED(1),
};

/// ANIM_FACING_FRONT_BACK's table: a front and a back picture.
const u8 gAnimationFacing_FrontBack[8] = {
    0, 0, MIRRORED(0), MIRRORED(0), 1, 1, MIRRORED(1), MIRRORED(1),
};

/// ANIM_FACING_THREE_PICTURES's table: front, side and back.
const u8 gAnimationFacing_ThreePictures[16] = {
    1, 1, 0, 0, MIRRORED(0), MIRRORED(0), MIRRORED(1), MIRRORED(1),
    MIRRORED(1), MIRRORED(1), MIRRORED(2), MIRRORED(2), 2, 2, 2, 1,
};

/// Unused: The Broken Seal's sixteen-sector table for its five pictures,
/// which nothing in The Lost Age reads.
const u8 gAnimationFacing_FivePicturesUnused[16] = {
    1, 0, 0, 3, 3, MIRRORED(0), MIRRORED(0), MIRRORED(1),
    MIRRORED(1), MIRRORED(2), MIRRORED(2), 4, 4, 2, 2, 1,
};

/// ANIM_FACING_FIVE_PICTURES's table: five pictures over 64 sectors, where
/// The Broken Seal's has 16.
const u8 gAnimationFacing_SixPictures[64] = {
    1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 3, 3, 3, 3,
    3, 3, 3, MIRRORED(3), MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0),
    MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(0), MIRRORED(1), MIRRORED(1), MIRRORED(1), MIRRORED(1),
    MIRRORED(1), MIRRORED(1), MIRRORED(1), MIRRORED(1), MIRRORED(2), MIRRORED(2), MIRRORED(2), MIRRORED(2),
    MIRRORED(2), MIRRORED(2), MIRRORED(2), MIRRORED(2), MIRRORED(4), 4, 4, 4,
    4, 4, 4, 4, 2, 2, 2, 2,
    2, 2, 2, 2, 1, 1, 1, 1,
};

/// ANIM_FACING_EIGHT_PICTURES's table: a picture for each compass direction.
const u8 gAnimationFacing_EightPictures[8] = {
    2, 1, 0, 7, 6, 5, 4, 3,
};

/// The order in which the 64 pixels of a sprite's tile in OBJ VRAM dissolve;
/// read by SpriteDissolve_Step.
const u8 gSpriteDissolve_PixelOrder[64] = {
    4, 63, 12, 10, 44, 5, 1, 60, 46, 41, 3, 23, 29, 55, 51, 15,
    50, 25, 26, 57, 19, 49, 16, 61, 8, 58, 37, 17, 27, 31, 59, 53,
    11, 56, 33, 7, 30, 35, 36, 32, 42, 62, 21, 13, 2, 22, 45, 14,
    6, 48, 52, 9, 43, 28, 0, 40, 24, 34, 20, 47, 54, 39, 38, 18,
};

/// Two bytes after the dissolve order; no code names them, in either game.
const u16 gSpriteDissolve_PixelOrderTail = 511;

/// An actor sprite's scale at full size, across and down.
const FIXED gActorSprite_FullScale[2] = {
    FIXED_ONE, FIXED_ONE,
};

/// The fixed camera's view matrix (16.16): identity with a -0.8 shear of y by
/// x, as The Broken Seal's.
const FIXED gCamera_FixedViewMatrix[12] = {
    FIXED_ONE, 0, 0,
    -(FIXED_ONE * 4 / 5), FIXED_ONE, 0,
    0, 0, FIXED_ONE,
    0, 0, 0,
};

/// One period of a triangle wave, which bobs a floating actor up and down.
const s32 gActorScript_CycleTable[32] = {
    1, 2, 3, 4, 5, 6, 7, 8,
    7, 6, 5, 4, 3, 2, 1, 0,
    -1, -2, -3, -4, -5, -6, -7, -8,
    -7, -6, -5, -4, -3, -2, -1, 0,
};

/// The idle script an actor returns to when its own one ends: a lone SC_END,
/// which reloads it. ActorScript_End (SC_END) points an actor here.
const s32 gScript_MainScript[1] = {
    SC_END,
};

/// Halfwords that follow the idle script; no code names them, in either game.
const s16 gActorScript_IdleScriptTail[8] = {
    4096, -8192, 12288, -16384, 24576, -32768, 16384, 0,
};

/// The heading for each state of the direction pad (bits 4-7 of the keys:
/// Right, Left, Up, Down), in 1/65536 turns: Right is 0, Down DEGtoANG(90),
/// Left DEGtoANG(180) and Up TRIG_TURN * 3 / 4; WORLD_MAP_KEY_HEADING_NONE
/// means no direction is held, or only an opposite pair.
const u16 gWorldMap_KeyHeadings[16] = {
    WORLD_MAP_KEY_HEADING_NONE, 0, DEGtoANG(180), WORLD_MAP_KEY_HEADING_NONE,
    TRIG_TURN * 3 / 4, TRIG_TURN * 7 / 8, TRIG_TURN * 5 / 8, TRIG_TURN * 3 / 4,
    DEGtoANG(90), DEGtoANG(45), TRIG_TURN * 3 / 8, DEGtoANG(90),
    WORLD_MAP_KEY_HEADING_NONE, 0, DEGtoANG(180), WORLD_MAP_KEY_HEADING_NONE,
};

/// A footprint's script: wait 120 frames, then release the actor, as The
/// Broken Seal's gWorldMap_FootprintActions.
const s32 gActor_FootprintScript[] = {
    SC_WAIT(120),
    SC_RELEASE,
};

/// A splash's script: wait 20 frames, then release the actor.
const s32 gActor_SplashScript[] = {
    SC_WAIT(20),
    SC_RELEASE,
};

/// A ripple's script: wait 32 frames, then release the actor.
const s32 gActor_RippleScript[] = {
    SC_WAIT(32),
    SC_RELEASE,
};

/// The yaw the World Map camera leans to for each state of the direction pad:
/// a 128th of a turn for Right or Left, three quarters of that with Up or Down
/// but not both; none otherwise.
const s32 gWorldMap_KeyYaws[16] = {
    0, -(TRIG_TURN / 128), TRIG_TURN / 128, 0,
    0, -(TRIG_TURN * 3 / 512), TRIG_TURN * 3 / 512, 0,
    0, -(TRIG_TURN * 3 / 512), TRIG_TURN * 3 / 512, 0,
    0, 0, 0, 0,
};
// clang-format on

/// A puff of sand the leader leaves behind as he moves (SPRITE_SAND_PUFF): its
/// riseEnabled byte counts down from 15 a step every 5 frames, 14 steps, then
/// the puff is shrunk to nothing and goes.
const s32 gActor_WakeScript[] = {
    SC_SET(SW_RISE_ENABLED, 15),
    SC_LABEL(1),
    SC_WAIT(5),
    SC_ADD(SW_RISE_ENABLED, -1),
    SC_LOOP(13, SC_TO(1)),
    SC_SET(SW_SCALE_X, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

/// A grain of the Sand's script: wait 60 frames, then release the actor.
const s32 gSand_GrainScript[] = {
    SC_WAIT(60),
    SC_RELEASE,
};

/// The World Map's graphics in four sets, by resource id: each a palette,
/// its tile animation and cell picture attributes, and six tile sets;
/// WorldMap_PreloadCurrentTileBlock chooses one by the region the party is in.
const u32 gWorldMap_GraphicsResources[4][9] = {
    {
        RESOURCE_WORLD_MAP_PALETTE_A,
        RESOURCE_DEFAULT_MAP_ANIMATION,
        RESOURCE_DEFAULT_CELL_PICTURE_ATTRIBUTES,
        RESOURCE_WORLD_MAP_TILES_A1,
        RESOURCE_WORLD_MAP_TILES_A2,
        RESOURCE_WORLD_MAP_TILES_A3,
        RESOURCE_WORLD_MAP_TILES_A4,
        RESOURCE_WORLD_MAP_TILES_A5,
        RESOURCE_WORLD_MAP_TILES_A6,
    },
    {
        RESOURCE_WORLD_MAP_PALETTE_C,
        RESOURCE_WORLD_MAP_ANIMATION_C,
        RESOURCE_WORLD_MAP_CELL_PICTURE_ATTRIBUTES_C,
        RESOURCE_WORLD_MAP_TILES_C1,
        RESOURCE_WORLD_MAP_TILES_C2,
        RESOURCE_WORLD_MAP_TILES_C3,
        RESOURCE_WORLD_MAP_TILES_C4,
        RESOURCE_WORLD_MAP_TILES_C5,
        RESOURCE_WORLD_MAP_TILES_C6,
    },
    {
        RESOURCE_WORLD_MAP_PALETTE_B,
        RESOURCE_WORLD_MAP_ANIMATION_B,
        RESOURCE_WORLD_MAP_CELL_PICTURE_ATTRIBUTES_B,
        RESOURCE_WORLD_MAP_TILES_B1,
        RESOURCE_WORLD_MAP_TILES_B2,
        RESOURCE_WORLD_MAP_TILES_B3,
        RESOURCE_WORLD_MAP_TILES_B4,
        RESOURCE_WORLD_MAP_TILES_B5,
        RESOURCE_WORLD_MAP_TILES_B6,
    },
    {
        RESOURCE_WORLD_MAP_PALETTE_D,
        RESOURCE_WORLD_MAP_ANIMATION_D,
        RESOURCE_WORLD_MAP_CELL_PICTURE_ATTRIBUTES_D,
        RESOURCE_WORLD_MAP_TILES_D1,
        RESOURCE_WORLD_MAP_TILES_D2,
        RESOURCE_WORLD_MAP_TILES_D3,
        RESOURCE_WORLD_MAP_TILES_D4,
        RESOURCE_WORLD_MAP_TILES_D5,
        RESOURCE_WORLD_MAP_TILES_D6,
    },
};
