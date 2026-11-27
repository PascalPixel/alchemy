#ifndef ALCHEMY_EVENT_COMMANDS_INTERNAL_H
#define ALCHEMY_EVENT_COMMANDS_INTERNAL_H

/* @file */
/* What every part of event_commands.c shares: its includes and the macros */
/* and types more than one part uses. event_commands.c includes it before the */
/* parts, and each part includes it too, so that a part read on its own */
/* resolves. It places no bytes. */

#include "battle/battle.h"
#include "window/message_argument.h"
#include "field/field.h"
#include "battle/object_effect_scripts.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "field/field_sequence_event.h"
#include "field/actor.h"
#include "field/objdisp.h"
#include "field/field_sprite.h"
#include "edition.h"
#include "field/actor_view.h"
#include "types.h"
#include "graphics/resource.h"
#include "debug/debug.h"
#include "macros.h"
#include "window/window.h"
#include "field/map_grid.h"
#include "fixed_math.h"
#include "field/hanging_vine.h"
#include "graphics/display_transition.h"
#include "graphics/palette_buffers.h"
#include "field/field_blend.h"
#include "field/field_coordinate.h"
#include "battle/motobj.h"
#include "version.h"
#include "field/sprite.h"
#include "field/field_particle.h"
#include "graphics/vram_tab.h"
#include "field/field_psynergy_state.h"
#include "field/scene_tables.h"

/* The sparkles Lift's and Carry's hands scatter as they meet, as The Broken */
/* Seal names them. */
#define ITEM_BREAK_SHARD_MAX 24

/* The steps, one a frame, in which a Psynergy's actor, Douse's rain cloud */
/* among them, grows out of the caster on its way to the target or shrinks */
/* back into it: fdEffectInterpolate's tenths. */
#define GROW_STEPS 10

/* Move's hand and Carry shed the same short-lived falling particle: it falls */
/* under a gravity of 0.1 pixel a frame, each frame, and is freed after 12 */
/* frames. */
static inline void fdEffectInitFallingFragment(ACTOR *fragment) {
	/* The caller spawns the fragment and places it. */
	if(fragment != NULL) {
		fragment->motionFlg = ACTOR_FALLS;
		fragment->gravity = toFIXED(1) / 10;
		fdActorSelectAnimationStub(fragment, 0);
		fragment->wait = 12;
		fdActorStartScriptStub(fragment, FieldEffect_ReleaseScript);
	}
}

/* The states of a Venus Djinni's rising particle */
/* (fdEffectUpdateRadialSpread), which fdDjinnRunVenusJoinEffect */
/* moves on to VENUS_PARTICLE_LAST_RISE. */
typedef enum VENUS_PARTICLE_STATE {
	VENUS_PARTICLE_AIM = 0, /* starts round the leader, aimed up the screen */
	VENUS_PARTICLE_RISING = 1, /* back to VENUS_PARTICLE_AIM once it arrives */
	VENUS_PARTICLE_LAST_RISE = 2 /* freed once it arrives */
} VENUS_PARTICLE_STATE;

/* The states of a Mercury Djinni's droplet */
/* (fdEffectUpdateRandomTargetParticle), which */
/* fdDjinnRunMercuryJoinEffect moves on to MERCURY_DROPLET_LAST_FALL. */
typedef enum MERCURY_DROPLET_STATE {
	MERCURY_DROPLET_AIM =
		0, /* placed above the leader, aimed at a spot by it */
	/* Splashes where it lands, then falls again (MERCURY_DROPLET_AIM). */
	MERCURY_DROPLET_FALLING = 1,
	/* Splashes where it lands, for the last time. */
	MERCURY_DROPLET_LAST_FALL = 2,
	MERCURY_DROPLET_DONE = 3 /* frees its slot */
} MERCURY_DROPLET_STATE;

/* The states of a Mars Djinni's spark */
/* (fdEffectUpdateMarsSpark), which fdDjinnRunMarsJoinEffect */
/* moves on to MARS_SPARK_STOPPING and then MARS_SPARK_LEAVING. */
typedef enum MARS_SPARK_STATE {
	MARS_SPARK_AIM = 0,	   /* aimed 120 pixels up the screen from its origin */
	MARS_SPARK_RISING = 1, /* back to MARS_SPARK_AIM once it arrives */
	/* Stops where it arrives, hidden, and waits 40 frames. */
	MARS_SPARK_STOPPING = 2,
	MARS_SPARK_AIM_LEADER = 3, /* aimed at a point by the leader */
	MARS_SPARK_SWARMING = 4,   /* back to MARS_SPARK_AIM_LEADER on arrival */
	MARS_SPARK_LEAVING = 5	   /* freed once it arrives */
} MARS_SPARK_STATE;

/* The trailing copies of a moving Djinni or of Force's hand are drawn a little
 */
/* smaller than what they follow. */
#define AFTERIMAGE_SCALE (FIXED_ONE * 15 / 16)

/* MainRoutineStateWork.pendingDjinni's parts, as fdDjinnResolvePending reads */
/* them. bits: 15..12 kind | 11..0 event or flag. */
#define DJINN_PENDING_KIND_MASK	 0xf000
#define DJINN_PENDING_EVENT_MASK 0xfff
/* A Djinni battle's event without DJINN_NO_HOP. */
#define DJINN_PENDING_BATTLE_EVENT_MASK 0x7ff

#endif /* ALCHEMY_EVENT_COMMANDS_INTERNAL_H */
