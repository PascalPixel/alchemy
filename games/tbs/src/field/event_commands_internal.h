#ifndef ALCHEMY_EVENT_COMMANDS_INTERNAL_H
#define ALCHEMY_EVENT_COMMANDS_INTERNAL_H

/// @file
/// What every part of event_commands.c shares: its includes and the macros
/// and types more than one part uses. event_commands.c includes it before the
/// parts, and each part includes it too, so that a part read on its own
/// resolves. It places no bytes.

#include "vram_cache.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "module/system/system.h"
#include "types.h"
#include "battle_action_run.h"
#include "main_routine_work.h"
#include "scene.h"
#include "playback_queue.h"
#include "edition.h"
#include "far_call.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "heap_release.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "io_write.h"
#include "irq.h"
#include "task.h"
#include "flags.h"
#include "iwram_call.h"
#include "field_work.h"
#include "resource_ids.h"
#include "speaker.h"
#include "battle_sprite_id.h"
#include "scene_tables.h"
#include "map_grid.h"
#include "map_grid_scroll.h"
#include "ram_buffer.h"
#include "heap_slots.h"
#include "actor.h"
#include "sprite.h"
#include "actor.h"
#include "actor_builtin_scripts.h"
#include "event.h"
#include "heap.h"
#include "input.h"
#include "main_routine_work.h"
#include "sounds.h"
#include "sound_driver_symbols.h"
#include "fixed_math.h"
#include "range_falloff.h"
#include "display_transition.h"
#include "text/msg_ids.h"
#include "ground_mask.h"
#include "item.h"
#include "battle_actor.h"
#include "static_assert.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palette_buffers.h"
#include "item_ids.h"
#include "inventory.h"
#include "battle_status.h"
#include "window.h"
#include "battle_side.h"
#include "edition.h"
#include "actor_arm_view.h"
#include "tbs_edition.h"
#include "sprite_metadata.h"
#include "field_coordinate.h"
#include "line_scroll.h"
#include "field_blend.h"
#include "field_psynergy_work.h"
#include "field_particle.h"
#include "field_effect_scripts.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"
#include "module/scene/shared/field_effect_spawn.h"
#include "module/scene/shared/scroll_initialize_hblank_dma.h"
#include "macros.h"
#include "ability_ids.h"
#include "sprite_ids.h"

/// The field effect sprites of a field Psynergy
/// (gFieldEffectWork->sprites).
#define PSYNERGY_EFFECT_SLOT_COUNT 24

/// gMainRoutineWork.pendingDjinni of Djinni 0 (requests.c, djinni.c): a
/// Djinni battle's event is 300 plus the Djinni's number (element * 20 +
/// index), with DJINN_NO_HOP beside it.
#define DJINNI_BATTLE_EVENT_FIRST 300

/// gMainRoutineWork.pendingDjinni's parts, as Djinn_ResolvePending reads them.
/// bits: 15..12 kind | 11..0 event or flag. Kind 0 is a Djinni battle, its
/// event beside DJINN_NO_HOP; CONDITION_FLAG_SET is a mimic chest's flag
/// (Field_ExamineCell).
#define DJINN_PENDING_KIND_MASK 0xf000
#define DJINN_PENDING_EVENT_MASK 0xfff
/// A Djinni battle's event without DJINN_NO_HOP.
#define DJINN_PENDING_BATTLE_EVENT_MASK 0x7ff

/// Per-frame update of an afterimage: closes half the distance to the actor
/// it watches, on all three axes.
static inline void FieldEffect_MoveAfterimageHalfway(struct Actor *actor) {
  struct Actor *afterimage = (struct Actor *)actor;
  struct Actor *target = afterimage->watchedActor;

  afterimage->x.fixed += (target->x.fixed - afterimage->x.fixed) / 2;
  afterimage->y.fixed += (target->y.fixed - afterimage->y.fixed) / 2;
  afterimage->z.fixed += (target->z.fixed - afterimage->z.fixed) / 2;
}

/// Runs the update of each of the 24 field effect sprites. The tasks
/// FieldEffect_UpdateAllEffectSlots (field_psynergy_work.c) and
/// FieldEffect_UpdateAllSlots (djinni.c) run it once a frame.
static __inline__ void FieldEffect_UpdateEffectSlots(void) {
  struct FieldParticle *effect;
  s32 remainingSlots;

  effect = ((struct FieldEffectWork *)gFieldEffectWork)->particles;
  remainingSlots = PSYNERGY_EFFECT_SLOT_COUNT - 1;
  do {
    remainingSlots -= 1;
    FieldParticle_Update(effect);
    effect++;
  } while (remainingSlots >= 0);
}

/// Defines a per-frame update that spins an actor 1/8 turn a frame while its
/// scale changes by step, releasing the actor once the scale falls below
/// Q16_ONE * 3 / 16. step is negative: from full size the actor
/// lasts (Q16_ONE * 13 / 16) / -step frames, rounded down, plus one.
#define DEFINE_SHRINK_AND_DESTROY(name, step)                                  \
  void name(struct Actor *actor) {                                             \
    void *obj = actor;                                                         \
    s32 scale;                                                                 \
                                                                               \
    scale = ((struct Actor *)obj)->scaleX + step;                              \
    ((struct Actor *)obj)->scaleY = ((struct Actor *)obj)->scaleY + step;      \
    ((struct Actor *)obj)->facing =                                            \
        ((struct Actor *)obj)->facing + TRIG_TURN / 8;                         \
    ((struct Actor *)obj)->scaleX = scale;                                     \
    if (scale < Q16_ONE * 3 / 16) {                                            \
      Actor_ReleaseFar(obj);                                                   \
    }                                                                          \
  }

/// A particle's sprite (struct Sprite), seen as the two fields
/// Douse and Mind Read set on it; the notes give the Sprite fields
/// at the same offsets.
struct WaveEffectSprite {
  u32 linkAndPosition[2]; ///< image.link and its first two attributes
  u8 tileLow;             ///< image.tile's low byte
  u32 tileHigh : 2;       ///< image.tile's high bits
  u32 priority : 2;       ///< image.priority
  u32 paletteBits : 4;    ///< image.palette
  u8 attr3ToDirty[28];    ///< image.attr3 to the sprite's dirty flag
  u8 flags;               ///< 0 drops shadow and outline
};

/// Move's hand and Carry shed the same short-lived falling particle: it falls
/// under a gravity of 0.1 pixel a frame, each frame, and is freed after 12
/// frames.
static inline void
FieldEffect_InitializeFallingFragment(struct Actor *fragment) {
  // Spawn and choose its position at the call site: combining the spawn here
  // moves the kind load after the coordinate loads in the native English build.
  if (fragment != NULL) {
    fragment->motionFlags = ACTOR_FALLS;
    fragment->gravity = Q16_ONE / 10;
    Actor_SelectAnimationFar(fragment, 0);
    fragment->wait = 12;
    Actor_StartScriptFar(fragment, gFieldEffect_SharedParticleScript);
  }
}

/// The shards DEFINE_ITEM_BREAK_EFFECT's casts scatter.
#define ITEM_BREAK_SHARD_MAX 24

/// Defines a field Psynergy cast with no target: spawn makes a hand 16 pixels
/// above the cast's position and 32 pixels to either side, each facing the
/// middle; each moves 24 pixels inward, then 24 shards scatter 8 to 32
/// pixels. Carry's and Lift's no-target casts are made with it.
#define DEFINE_ITEM_BREAK_EFFECT(name, spawn)                                  \
  void name(void) {                                                            \
    struct FieldEffectWork *effectWork;                                        \
    struct Actor *obj;                                                         \
    struct Actor *pair[2];                                                     \
    struct Vector3 pos;                                                        \
    struct Actor **walk;                                                       \
    s32 n;                                                                     \
    s32 dist;                                                                  \
                                                                               \
    effectWork = gFieldEffectWork;                                             \
    FieldPsynergy_FadeToCastColor();                                           \
    pos.x = effectWork->x;                                                     \
    pos.y = effectWork->y + PIXELS(16);                                        \
    pos.z = effectWork->z;                                                     \
    pair[0] = spawn(pos.x + PIXELS(32), pos.y, pos.z, FACING_WEST);            \
    pair[1] = spawn(pos.x - PIXELS(32), pos.y, pos.z, FACING_EAST);            \
                                                                               \
    wait(15);                                                                  \
    walk = pair;                                                               \
    for (n = 0; n < 2; n++) {                                                  \
      obj = *walk++;                                                           \
      if (obj != 0)                                                            \
        Actor_SetTargetPositionFromMagnitudeAngle(obj, PIXELS(24),             \
                                                  obj->facing);                \
    }                                                                          \
                                                                               \
    Actor_WaitForMoveFar(pair[0]);                                             \
    Sound_PlayFar(SE_HEAVY_IMPACT);                                            \
    for (n = 0; n < ITEM_BREAK_SHARD_MAX; n++) {                               \
      pos.x = effectWork->x;                                                   \
      pos.y = effectWork->y + PIXELS(16);                                      \
      pos.z = effectWork->z;                                                   \
      obj = Actor_Spawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);                  \
      if (obj != 0) {                                                          \
        Actor_StartScriptFar(obj, gFieldEffect_FragmentScript);                \
        obj->speedLimit = Random16() + PIXELS(2);                              \
        obj->acceleration = PIXELS(2);                                         \
        obj->motionFlags = 0;                                                  \
        dist = Random16() * 24 + PIXELS(8);                                    \
        Actor_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());      \
      }                                                                        \
    }                                                                          \
                                                                               \
    Actor_ReleaseFar(pair[0]);                                                 \
    Actor_ReleaseFar(pair[1]);                                                 \
    FieldPsynergy_FadeFromCastColor();                                         \
  }

#define ABOVE (16 << 16) ///< 16 pixels up

/// The states of a particle that flies out and back: Halt's,
/// Force's and Move's (FieldEffect_UpdateRadialCamera,
/// FieldEffect_UpdateRadialBurst, FieldEffect_UpdateOrbitAndReturn).
/// FieldParticle_SetCallback starts it at the first; each takes the next.
enum OutAndBackState {
  OUT_AND_BACK_AIM_OUT = 0,    ///< picks the point it flies out to
  OUT_AND_BACK_FLYING_OUT = 1, ///< waits to arrive there
  OUT_AND_BACK_AIM_BACK = 2,   ///< picks the point it comes back to
  OUT_AND_BACK_FLYING_BACK = 3 ///< frees its slot once it arrives
};

/// The states of a Venus Djinni's rising particle
/// (FieldEffect_UpdateRadialSpread), which FieldEffect_RunVenusDjinnCapture
/// moves on to VENUS_PARTICLE_LAST_RISE.
enum VenusParticleState {
  VENUS_PARTICLE_AIM = 0,      ///< starts round the leader, aimed up the screen
  VENUS_PARTICLE_RISING = 1,   ///< back to VENUS_PARTICLE_AIM once it arrives
  VENUS_PARTICLE_LAST_RISE = 2 ///< freed once it arrives
};

/// The states of a Mercury Djinni's droplet
/// (FieldEffect_UpdateRandomTargetParticle), which
/// FieldEffect_RunMercuryDjinnCapture moves on to MERCURY_DROPLET_LAST_FALL.
enum MercuryDropletState {
  MERCURY_DROPLET_AIM = 0, ///< placed above the leader, aimed at a spot by it
  /// Splashes where it lands, then falls again (MERCURY_DROPLET_AIM).
  MERCURY_DROPLET_FALLING = 1,
  /// Splashes where it lands, for the last time.
  MERCURY_DROPLET_LAST_FALL = 2,
  MERCURY_DROPLET_DONE = 3 ///< frees its slot
};

/// The states of a Mars Djinni's spark
/// (FieldEffect_UpdatePhasedRadialParticle), which Djinn_RunMarsJoinEffect
/// moves on to MARS_SPARK_STOPPING and then MARS_SPARK_LEAVING.
enum MarsSparkState {
  MARS_SPARK_AIM = 0,    ///< aimed 120 pixels up the screen from its origin
  MARS_SPARK_RISING = 1, ///< back to MARS_SPARK_AIM once it arrives
  /// Stops where it arrives, hidden, and waits 40 frames.
  MARS_SPARK_STOPPING = 2,
  MARS_SPARK_AIM_LEADER = 3, ///< aimed at a point by the leader
  MARS_SPARK_SWARMING = 4,   ///< back to MARS_SPARK_AIM_LEADER on arrival
  MARS_SPARK_LEAVING = 5     ///< freed once it arrives
};

#endif
