#ifndef ALCHEMY_PSYNERGY_EFFECTS_INTERNAL_H
#define ALCHEMY_PSYNERGY_EFFECTS_INTERNAL_H

/// @file
/// What every part of psynergy_effects.c shares: its includes and the macros
/// and types more than one part uses. psynergy_effects.c includes it before the
/// parts, and each part includes it too, so that a part read on its own
/// resolves. It places no bytes.

#include "vram_tab.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "scene.h"
#include "battle_type.h"
#include "game_state.h"
#include "field_runtime.h"
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
#include "callback_scheduler.h"
#include "flags.h"
#include "iwram_call.h"
#include "event_work.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "field_obj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "heap_state.h"
#include "keystate.h"
#include "owner_value.h"
#include "party_state.h"
#include "songs.h"
#include "fixed_math.h"
#include "item_gift.h"
#include "battle_calc.h"
#include "display_transition.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "static_assert.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "battle_party.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "field_coordinate.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_effect_scripts.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/scroll_initialize_hblank_dma.h"
#include "macros.h"
#include "ability_ids.h"

/// The effect slots of a Psynergy effect's scene (gEffectWork->slots).
#define PSYNERGY_EFFECT_SLOT_COUNT 24

/// gGameState.pendingDjinnEvent of Djinni 0 (requests.c, djinni.c): a Djinni
/// battle's event is 300 plus the Djinni's number (element * 20 + index),
/// with DJINN_NO_HOP (0x800) beside it.
#define DJINNI_0 300

/// A FieldActor seen with its position as one vector, to pass it whole. The
/// gaps are FieldActor fields this view does not use.
struct Object {
  u8 pad0[6];
  u16 angle;               ///< FieldActor.facing, 1/65536 turns
  struct EffectVector pos; ///< 16.16 fixed-point pixels
  u8 pad1[14];
  u8 kind; ///< FieldActor.terrainId; not read through this view
  u8 pad2[0x2d];
  struct AnimationObject *sprite; ///< FieldActor.sprite
};

/// Per-frame update of an afterimage: closes half the distance to the object
/// it is linked to, on all three axes.
static inline void
BattleFx_MoveAfterimageHalfway(union FieldObject *fieldObject) {
  struct FieldActor *afterimage = (struct FieldActor *)fieldObject;
  struct FieldActor *target = afterimage->linkedObject;

  afterimage->x.fixed += (target->x.fixed - afterimage->x.fixed) / 2;
  afterimage->y.fixed += (target->y.fixed - afterimage->y.fixed) / 2;
  afterimage->z.fixed += (target->z.fixed - afterimage->z.fixed) / 2;
}

/// Runs the update of each of the 24 effect slots. The scheduler callbacks
/// BattleFx_UpdateAllEffectSlots (setup.c) and BattleFx_UpdateAllSlots
/// (djinni.c) run it once a frame.
static __inline__ void FieldFx_UpdateSceneSlots(void) {
  struct EffectSlot *slot;
  s32 remainingSlots;

  slot = ((struct BattleFxScene *)gEffectWork)->slots;
  remainingSlots = PSYNERGY_EFFECT_SLOT_COUNT - 1;
  do {
    remainingSlots -= 1;
    EffectSlot_Update(slot);
    slot++;
  } while (remainingSlots >= 0);
}

/// A FieldActor seen with the halfword at 0x64 unsigned and the one at 0x66
/// signed, as the bytes read them (FieldActor's own types change the code).
/// A Venus Djinni's falling motes (BattleFx_UpdateDescendingOrbitObject).
struct DescendingOrbitObject {
  u8 unk00[8];
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 unk14[80];
  u16 timer; ///< FieldActor.timer: frames left of the spiral, 100 at the start
  s16 angle; ///< FieldActor.work: its start on the circle, 1/65536 turns
};

/// A FieldActor as the Venus Djinni's joining uses the Djinni, the leader and
/// its motes and sparks; the notes give the FieldActor fields at the same
/// offsets, some under other names.
struct CaptureObject {
  u8 unk00[8];
  s32 x;     ///< 16.16 fixed-point pixels
  s32 y;     ///< 16.16 fixed-point pixels
  s32 z;     ///< 16.16 fixed-point pixels
  s32 baseY; ///< terrainHeight: a spark's ground, set to where it starts
  s32 scaleX;
  s32 scaleY;
  u8 unk20[8];
  /// velocityY, despite the name: a spark is thrown up at 5 pixels a frame.
  s32 gravity;
  u8 unk2c[4];
  s32 speed; ///< speed limit, 16.16 fixed-point pixels per frame
  u8 unk34[33];
  u8 mode; ///< motionFlags: 0 for the motes, 2 (falls) for the sparks
  u8 unk56[5];
  u8 visible; ///< paused, despite the name: 0 lets the Djinni move
  u8 unk5c[2];
  u16 lifetime; ///< wait: frames its script waits before freeing it
  u8 unk60[4];
  u16 timer; ///< as DescendingOrbitObject.timer
  u16 angle; ///< work: as DescendingOrbitObject.angle
  struct FieldActor *linkedObject;
  void (*callback)(union FieldObject *object); ///< update: run each frame
};

/// Defines a per-frame update that spins an object 1/8 turn a frame while its
/// scale changes by step, releasing the object once the scale falls below
/// 0x3000 (3/16 of full size). step is negative: from full size the object
/// lasts 0xd000 / -step frames, rounded down, plus one.
#define DEFINE_SHRINK_AND_DESTROY(name, step)                                  \
  void name(union FieldObject *fieldObject) {                                  \
    void *obj = fieldObject;                                                   \
    s32 scale;                                                                 \
                                                                               \
    scale = ((struct MotionObject *)obj)->scaleX + step;                       \
    ((struct MotionObject *)obj)->scaleY =                                     \
        ((struct MotionObject *)obj)->scaleY + step;                           \
    ((struct MotionObject *)obj)->angle =                                      \
        ((struct MotionObject *)obj)->angle + 0x2000;                          \
    ((struct MotionObject *)obj)->scaleX = scale;                              \
    if (scale < 0x3000) {                                                      \
      ObjectDispatch_ReleaseFar(obj);                                          \
    }                                                                          \
  }

/// The heap slot of the Psynergy effect's work, struct BattleFxScene
/// (gWorkSlot[56], gEffectWork). A field Psynergy holds it from
/// BattleFx_LoadActionEffectResources to BattleEffect_CleanupSceneObjects, a
/// Djinni's joining from BattleFx_InitializeSlots to
/// BattleFx_ClearActiveSlotsAndScheduleUpdates.
#define HEAP_SLOT_FX 56

/// A particle's sprite (struct AnimationObject), seen as the two fields
/// Douse and Mind Read set on it; the notes give the AnimationObject fields
/// at the same offsets.
struct WaveFxSprite {
  u32 unk00[2];
  u8 unk08;
  u32 unk09_0 : 2;
  u32 mode : 2; ///< image.priority, despite the name
  u32 unk09_4 : 4;
  u8 unk0a[0x1c];
  u8 palette; ///< flags, despite the name: 0 drops shadow and outline
};

/// A FieldActor as the particles Move's hand and Carry shed use it; the notes
/// give the FieldActor fields at the same offsets.
struct ItemBreakFragmentObject {
  u8 reserved00[72];
  s32 gravity; ///< 16.16 fixed-point pixels per frame, each frame
  u8 reserved4c[9];
  u8 motionFlags;
  u8 reserved56[8];
  u16 lifetime; ///< wait: frames its script waits before freeing it
};

/// Move's hand and Carry shed the same short-lived falling particle: it falls
/// under a gravity of 0.1 pixel a frame, each frame, and is freed after 12
/// frames.
static inline void
BattleFx_InitializeFallingFragment(struct ItemBreakFragmentObject *fragment) {
  // Spawn and choose its position at the call site: combining the spawn here
  // moves the kind load after the coordinate loads in the native English build.
  if (fragment != NULL) {
    fragment->motionFlags = ACTOR_FALLS;
    fragment->gravity = 0x1999;
    ObjectDispatch_SelectAnimationFar(fragment, 0);
    fragment->lifetime = 12;
    ObjectDispatch_InitializeFar((struct DispatchObject *)fragment,
                                 gBattleFx_CommonParticleScript);
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
    struct BattleFxScene *work;                                                \
    struct MotionObject *obj;                                                  \
    struct MotionObject *pair[2];                                              \
    struct ScatterPosition pos;                                                \
    struct MotionObject **walk;                                                \
    s32 n;                                                                     \
    s32 dist;                                                                  \
                                                                               \
    work = gEffectWork;                                                        \
    BattleEffect_InitializeSharedScene();                                      \
    pos.x = work->x;                                                           \
    pos.y = work->y + 0x100000;                                                \
    pos.z = work->z;                                                           \
    pair[0] = spawn(pos.x + 0x200000, pos.y, pos.z, 0x8000);                   \
    pair[1] = spawn(pos.x - 0x200000, pos.y, pos.z, 0);                        \
                                                                               \
    WaitFrames(15);                                                            \
    walk = pair;                                                               \
    for (n = 0; n < 2; n++) {                                                  \
      obj = *walk++;                                                           \
      if (obj != 0)                                                            \
        Motion_SetTargetPositionFromMagnitudeAngle(                            \
            (struct ObjectRuntime *)obj, 0x180000, obj->angle);                \
    }                                                                          \
                                                                               \
    FieldObject_WaitForMoveFar(pair[0]);                                       \
    AudioCommand_PlayFar(SE_HEAVY_IMPACT);                                     \
    for (n = 0; n < ITEM_BREAK_SHARD_MAX; n++) {                               \
      pos.x = work->x;                                                         \
      pos.y = work->y + 0x100000;                                              \
      pos.z = work->z;                                                         \
      obj = (struct MotionObject *)Object_Spawn(285, pos.x, pos.y, pos.z);     \
      if (obj != 0) {                                                          \
        ObjectDispatch_InitializeFar((struct DispatchObject *)obj,             \
                                     gBattleFx_FragmentScript);                \
        obj->speedLimit = Random16() + 0x20000;                                \
        obj->acceleration = 0x20000;                                           \
        obj->motionFlags = 0;                                                  \
        dist = Random16() * 24 + 0x80000;                                      \
        Motion_SetTargetPositionFromMagnitudeAngle(                            \
            (struct ObjectRuntime *)obj, dist, Random16());                    \
      }                                                                        \
    }                                                                          \
                                                                               \
    ObjectDispatch_ReleaseFar((struct DispatchObject *)pair[0]);               \
    ObjectDispatch_ReleaseFar((struct DispatchObject *)pair[1]);               \
    BattleFx_PrepareBufferInterpolation();                                     \
  }

#define ABOVE (16 << 16) ///< 16 pixels up

/// A FieldActor as Ply's spark emitter and its sparks use it (ply.c); the
/// notes give the FieldActor fields at the same offsets.
struct SparkObject {
  u8 unk00[8];
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[24];
  s32 anchorX; ///< centre of the sway (FieldActor's targetX)
  u8 unk3c[12];
  s32 gravity; ///< as in FieldActor
  u8 unk4c[9];
  u8 motionFlags; ///< as in FieldActor
  u8 unk56[8];
  u16 lifetime; ///< wait: frames its script waits before freeing it
  u8 unk60[4];
  /// timer: phase of the sway, 128 frames a cycle for a spark and 64 for the
  /// emitter, which -1 holds still.
  s16 phase;
  u8 unk66[6];
  void (*update)(union FieldObject *object); ///< run each frame
};

#endif
