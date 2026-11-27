#ifndef ALCHEMY_BATTLE_CAMERA_H
#define ALCHEMY_BATTLE_CAMERA_H

// The battle camera, its view state and the battle background's affine state.

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle_action.h"
#include "battle/battle_effect_params.h"
#include "stubs.h"

// The battle camera: its eye, the focus it turns about, and its pitch, yaw and distance. It lives
// in the HEAP_SLOT_CAMERA heap slot.
typedef struct BATTLE_CAMERA
{
    // The eye, 16.16 fixed-point pixels: written each tick distance behind the focus along the
    // turned view.
    FIXED   eye[3];
    FIXED   focus[3]; // the focus, 16.16 fixed-point pixels
    // When set, fdActorSystemProjectSprites faces the sprites from it instead of eye; every camera
    // setup clears it and nothing sets it.
    Sint32 *followEye;
    Sint32 *followPos;               // when set, followed instead of focus
    FIXED   distance;                // eye to focus, 16.16 fixed-point pixels
    Uint8   unusedAfterDistance[16]; // no code names it
    Sint16  pitch;                   // 1/65536 turns
    Sint16  yaw;                     // 1/65536 turns
    Uint8   unusedAfterYaw[20];      // no code names it
} BATTLE_CAMERA;

STATIC_ASSERT(sizeof(BATTLE_CAMERA) == 76);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, focus) == 12);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, followPos) == 28);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, pitch) == 52);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, yaw) == 54);

// The battle camera's distance from its focus in its default view, and the unzoomed projection's
// focal length, in 16.16 pixels (btCameraSetView).
#define BATTLE_CAMERA_DISTANCE toFIXED(510)

// The HEAP_SLOT_CAMERA heap slot during a battle: the battle camera.
#define BattleCameraWork       (*(BATTLE_CAMERA **)&HeapSlots[HEAP_SLOT_CAMERA])

// The HEAP_SLOT_BATTLE_BACKGROUND heap slot: the yaw the battle camera eases toward and the ticks
// left to ease it, and how the battle background is shown.
typedef struct BATTLE_BACKGROUND_STATE
{
    Sint32 targetYaw; // 1/65536 turns
    Sint32 ticks;     // ticks left to ease the yaw, a sixteenth a tick
    Sint32 mode;      // BATTLE_BACKGROUND_MODE
    // BATTLE_BACKGROUND_FIXED also sets BG1's scroll whatever the mode; the battle effects' canvas
    // layers set it.
    Sint32 secondMode;
    Sint32 busy; // while set, btCameraSetRange leaves BG1's scroll
    // 0 lets the camera update set the background's range each tick; 1 for a critical hit's zoom
    // and once a turn's view is set.
    Sint32 keepRange;
    // cleared when a battle starts; nothing reads them
    Sint32 unusedAfterKeepRange[2];
} BATTLE_BACKGROUND_STATE;

// A presentation step's stack state: the battle effect command it builds and runs, and the targets'
// sprite colour modes saved beside it. The presentation steps of battle_staging.c and
// battle_auto_battle.c, btTransitionRun and btElementGlowPlay keep one as a local.
typedef struct BATTLE_STAGING_STATE
{
    BATTLE_EFFECT_PARM effect;
    // Each target's sprite animations' SPR_COL_MODE (SPR_ANIMATION.colorMode), all but its last,
    // saved before the effect; no code reads them back by name.
    Uint8              savedAnimationParams[8][4];
} BATTLE_STAGING_STATE;

STATIC_ASSERT(sizeof(BATTLE_BACKGROUND_STATE) == 32);
STATIC_ASSERT(offsetof(BATTLE_BACKGROUND_STATE, mode) == 8);
STATIC_ASSERT(offsetof(BATTLE_BACKGROUND_STATE, busy) == 16);
STATIC_ASSERT(sizeof(BATTLE_STAGING_STATE) == 84);
STATIC_ASSERT(offsetof(BATTLE_STAGING_STATE, effect.targets) == 36);

// BATTLE_BACKGROUND_STATE.mode and secondMode: how the battle background is shown.
typedef enum BATTLE_BACKGROUND_MODE
{
    // Not loaded yet: the next load adds btBackgroundShowZoomedPage.
    BATTLE_BACKGROUND_NONE,
    // The picture at a fixed size on BG1 (BATTLE_BG1CNT), its scroll following the camera, BG2 left
    // to a battle effect.
    BATTLE_BACKGROUND_FIXED,
    // The picture zoomed and panned on BG2, an affine layer; HBlank DMA sets each line's BG2CNT to
    // the half of the picture it reads. The depth is drawn into the picture; the hardware only
    // zooms it.
    BATTLE_BACKGROUND_ZOOMED
} BATTLE_BACKGROUND_MODE;

// btBackgroundSetMode's fade that restores the saved palette instead of taking from each colour
// channel.
#define BATTLE_BACKGROUND_FADE_RESTORE 128

// The battle background's affine parameters and reference point, copied whole to the BG2PA
// registers.
typedef struct BATTLE_AFFINE
{
    // BG2PA to BG2PD, 8.8 fixed point; btCameraSetRange writes a plain zoom (pb and pc 0, pa and pd
    // one over the scale).
    Uint16 pa, pb, pc, pd;
    Sint32 x, y; // BG2X and BG2Y, 8.8 fixed-point pixels
} BATTLE_AFFINE;

// The battle background's scanline state, in HEAP_SLOT_BATTLE_SCANLINES: two pages of per-line
// BG2CNT values, swapped each tick, and the affine parameters.
typedef struct BATTLE_AFFINE_HDMA
{
    Uint32        page;       // the page of lines shown; the other one is built
    Uint8         unused[12]; // no code names it
    BATTLE_AFFINE affine;
    // Each scanline's BG2CNT, one page per tick: which half of the picture a line shows, or the
    // plane off, with BGCNT_WRAP at a zoom of 1 or more.
    Uint16        lines[2][160];
} BATTLE_AFFINE_HDMA;

STATIC_ASSERT(sizeof(BATTLE_AFFINE_HDMA) == 672);
STATIC_ASSERT(offsetof(BATTLE_AFFINE_HDMA, affine) == 16);
STATIC_ASSERT(offsetof(BATTLE_AFFINE_HDMA, lines) == 32);

#endif
