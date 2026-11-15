/// @file
/// Four effect scripts of the field sequence bank: the found item's picture
/// growing in and pulsing, a sinking particle and a Reveal marker.

#include "fixed_math.h"
#include "types.h"
#include "field/actor_script.h"
#include "field/screen_transition.h"
#include "battle/battle.h"

/// Grows from half size to full size, a step every 2 frames.
const s32 gFieldEffect_ParticleEmitterScript[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE / 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 3 / 4),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 4),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 7 / 8),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 7 / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 15 / 16),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 15 / 16),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_END,
};

/// Pulses twice, to 1.5 times full size, then rests, over and over: the found
/// item's picture held over the leader (field_sequence_item_display.c).
const s32 gFieldEffect_RisingPulseScript[] = {
    SC_WAIT(10),
    SC_SET(SW_SCALE_X, FIXED_ONE * 11 / 8),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 11 / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 3 / 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 9 / 8),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 9 / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 11 / 8),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 11 / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 3 / 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE * 9 / 8),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 9 / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_WAIT(2),
    SC_WAIT(50),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// Sinks a quarter pixel a frame for 60 frames, then disappears.
const s32 gBattleEffect_ParticleScript[] = {
    SC_ADD(SW_Y, -toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_RELEASE,
};

/// A Reveal marker: stays while Field_IsRevealActive returns nonzero (SC_CALL
/// runs it again each frame), and releases itself once Reveal ends; the same
/// as The Broken Seal's gFieldEffect_MarkerParticleScript.
const s32 gBattleEffect_MarkerParticleScript[] = {
    SC_CALL(Field_IsRevealActive),
    SC_RELEASE,
};
