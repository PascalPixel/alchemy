#ifndef ALCHEMY_FIELD_SERVICES_INTERNAL_H
#define ALCHEMY_FIELD_SERVICES_INTERNAL_H

/// @file
/// What every part of field_services.c shares: its includes and the macros
/// and types more than one part uses. field_services.c includes it before the
/// parts, and each part includes it too, so that a part read on its own
/// resolves. It places no bytes.

#include "vram_cache.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "battle_type.h"
#include "save_block.h"
#include "scene_header.h"
#include "battle_system.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "task.h"
#include "flags.h"
#include "iwram_call.h"
#include "field_work.h"
#include "resource_ids.h"
#include "speaker.h"
#include "resource.h"
#include "scene_tables.h"
#include "map_grid.h"
#include "map_grid_scroll.h"
#include "ram_buffer.h"
#include "heap_slots.h"
#include "object.h"
#include "animspr.h"
#include "actor.h"
#include "object_dispatch.h"
#include "script_layer.h"
#include "heap.h"
#include "keystate.h"
#include "unit.h"
#include "party_state.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "fixed_math.h"
#include "item_gift.h"
#include "battle_calc.h"
#include "display_transition.h"
#include "text/msg_ids.h"
#include "objdisp.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "static_assert.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "battle_party.h"
#include "edition.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "field_coordinate.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_work.h"
#include "field_effect_sprite.h"
#include "object_effect_scripts.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/scroll_initialize_hblank_dma.h"
#include "macros.h"
#include "ability_ids.h"
#include "sprite_ids.h"

/// The field effect sprites of a Psynergy effect
/// (gFieldEffectWork->sprites).
#define PSYNERGY_EFFECT_SLOT_COUNT 24

/// gSaveBlock.pendingDjinni of Djinni 0 (requests.c, djinni.c): a Djinni
/// battle's event is 300 plus the Djinni's number (element * 20 + index),
/// with DJINN_NO_HOP (0x800) beside it.
#define DJINNI_BATTLE_EVENT_FIRST 300

/// Per-frame update of an afterimage: closes half the distance to the object
/// it is linked to, on all three axes.
static inline void
FieldEffect_MoveAfterimageHalfway(union ActorOrEffect *fieldObject) {
  struct Actor *afterimage = (struct Actor *)fieldObject;
  struct Actor *target = afterimage->linkedObject;

  afterimage->x.fixed += (target->x.fixed - afterimage->x.fixed) / 2;
  afterimage->y.fixed += (target->y.fixed - afterimage->y.fixed) / 2;
  afterimage->z.fixed += (target->z.fixed - afterimage->z.fixed) / 2;
}

/// Runs the update of each of the 24 field effect sprites. The tasks
/// FieldEffect_UpdateAllEffectSlots (setup.c) and FieldEffect_UpdateAllSlots
/// (djinni.c) run it once a frame.
static __inline__ void FieldEffect_UpdateEffectSlots(void) {
  struct FieldEffectSprite *effect;
  s32 remainingSlots;

  effect = ((struct FieldEffectWork *)gFieldEffectWork)->sprites;
  remainingSlots = PSYNERGY_EFFECT_SLOT_COUNT - 1;
  do {
    remainingSlots -= 1;
    FieldEffectSprite_Update(effect);
    effect++;
  } while (remainingSlots >= 0);
}

/// Defines a per-frame update that spins an object 1/8 turn a frame while its
/// scale changes by step, releasing the object once the scale falls below
/// 0x3000 (3/16 of full size). step is negative: from full size the object
/// lasts 0xd000 / -step frames, rounded down, plus one.
#define DEFINE_SHRINK_AND_DESTROY(name, step)                                  \
  void name(union ActorOrEffect *fieldObject) {                                \
    void *obj = fieldObject;                                                   \
    s32 scale;                                                                 \
                                                                               \
    scale = ((struct Object *)obj)->scaleX + step;                             \
    ((struct Object *)obj)->scaleY = ((struct Object *)obj)->scaleY + step;    \
    ((struct Object *)obj)->facing = ((struct Object *)obj)->facing + 0x2000;  \
    ((struct Object *)obj)->scaleX = scale;                                    \
    if (scale < 0x3000) {                                                      \
      Object_ReleaseFar(obj);                                                  \
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
  u8 attr3ToDirty[0x1c];  ///< image.attr3 to the sprite's dirty flag
  u8 flags;               ///< 0 drops shadow and outline
};

/// Move's hand and Carry shed the same short-lived falling particle: it falls
/// under a gravity of 0.1 pixel a frame, each frame, and is freed after 12
/// frames.
static inline void
FieldEffect_InitializeFallingFragment(struct Object *fragment) {
  // Spawn and choose its position at the call site: combining the spawn here
  // moves the kind load after the coordinate loads in the native English build.
  if (fragment != NULL) {
    fragment->motionFlags = ACTOR_FALLS;
    fragment->gravity = 0x1999;
    Object_SelectAnimationFar(fragment, 0);
    fragment->wait = 12;
    Object_InitializeFar(fragment, gFieldEffect_CommonParticleScript);
  }
}

/// The shards DEFINE_ITEM_BREAK_EFFECT's effects scatter.
#define ITEM_BREAK_SHARD_MAX 24

/// Defines an effect cast with no target: spawn makes a hand 16 pixels above
/// the effect's position and 32 pixels to either side, each facing the
/// middle; each moves 24 pixels inward, then 24 shards scatter 8 to 32
/// pixels. Carry's and Lift's no-target effects are made with it.
#define DEFINE_ITEM_BREAK_EFFECT(name, spawn)                                  \
  void name(void) {                                                            \
    struct FieldEffectWork *effectWork;                                        \
    struct Object *obj;                                                        \
    struct Object *pair[2];                                                    \
    struct Vector3 pos;                                                        \
    struct Object **walk;                                                      \
    s32 n;                                                                     \
    s32 dist;                                                                  \
                                                                               \
    effectWork = gFieldEffectWork;                                             \
    FieldEffect_FadeToEffectColor();                                           \
    pos.x = effectWork->x;                                                     \
    pos.y = effectWork->y + 0x100000;                                          \
    pos.z = effectWork->z;                                                     \
    pair[0] = spawn(pos.x + 0x200000, pos.y, pos.z, 0x8000);                   \
    pair[1] = spawn(pos.x - 0x200000, pos.y, pos.z, 0);                        \
                                                                               \
    WaitFrames(15);                                                            \
    walk = pair;                                                               \
    for (n = 0; n < 2; n++) {                                                  \
      obj = *walk++;                                                           \
      if (obj != 0)                                                            \
        ObjectMotion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000,        \
                                                         obj->facing);         \
    }                                                                          \
                                                                               \
    Object_WaitForMoveFar(pair[0]);                                            \
    Sound_PlaySongFar(SE_HEAVY_IMPACT);                                        \
    for (n = 0; n < ITEM_BREAK_SHARD_MAX; n++) {                               \
      pos.x = effectWork->x;                                                   \
      pos.y = effectWork->y + 0x100000;                                        \
      pos.z = effectWork->z;                                                   \
      obj =                                                                    \
          (struct Object *)Object_Spawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);  \
      if (obj != 0) {                                                          \
        Object_InitializeFar((struct Object *)obj,                             \
                             gFieldEffect_FragmentScript);                     \
        obj->speedLimit = Random16() + 0x20000;                                \
        obj->acceleration = 0x20000;                                           \
        obj->motionFlags = 0;                                                  \
        dist = Random16() * 24 + 0x80000;                                      \
        ObjectMotion_SetTargetPositionFromMagnitudeAngle((struct Object *)obj, \
                                                         dist, Random16());    \
      }                                                                        \
    }                                                                          \
                                                                               \
    Object_ReleaseFar((struct Object *)pair[0]);                               \
    Object_ReleaseFar((struct Object *)pair[1]);                               \
    FieldEffect_FadeFromEffectColor();                                         \
  }

#define ABOVE (16 << 16) ///< 16 pixels up

/// The states of a particle that flies out and back: Halt's,
/// Force's and Move's (FieldEffect_UpdateRadialCamera,
/// FieldEffect_UpdateRadialBurst, FieldEffect_UpdateOrbitAndReturn).
/// FieldEffectSprite_SetCallback starts it at the first; each takes the next.
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
