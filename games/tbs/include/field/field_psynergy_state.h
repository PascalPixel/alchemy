#ifndef ALCHEMY_FIELD_PSYNERGY_STATE_H
#define ALCHEMY_FIELD_PSYNERGY_STATE_H

/// @file
/// FieldEffectState, the state of a field Psynergy.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "field/field_particle.h"

struct Sprite;
struct Actor;

/// The state of a field Psynergy: its caster and target, where it takes place,
/// the map state it restores and its 24 field effect sprites.
struct FieldEffectState {
  s32 angle;         ///< 1/65536 turns
  FIXED x;           ///< 16.16 fixed-point pixels
  FIXED y;           ///< 16.16 fixed-point pixels
  FIXED z;           ///< 16.16 fixed-point pixels
  void *caster;      ///< the caster's actor
  void *target;      ///< the target's actor, or NULL
  s16 casterId;      ///< the caster's actor id
  s16 targetId;      ///< the target's actor id, -1 for none
  u16 ability;       ///< the ability id
  s16 fieldPsynergy; ///< the ability's field kind (FIELD_PSYNERGY_)
  /// Set at the start: Force then aims in front of the caster; cleared once
  /// the target has been dispatched.
  s8 aimAhead;
  u8 mode;         ///< where it runs; ACTOR_SYSTEM_WORLD_MAP on the world map
  s8 flashCaster;  ///< set at the start: the caster flashes with a sound
  s8 announceName; ///< set at the start: the ability's name is announced
  void (*finishCallback)(void); ///< run, when set, as Ply ends
  u8 unusedAfterFinish[12];
  /// Set by some scenes before a field Psynergy; Carry, Frost, Force and Reveal
  /// treat the target differently with it.
  s8 sceneVariant;
  s8 childMode; ///< set by some scenes: Frost and Force leave the target be
  u8 paddingAfterChildMode[2]; ///< up to the word savedCallback starts on
  /// The target's update routine and script, saved at the start and restored
  /// when Move or Carry lets it go.
  ///@{
  void (*savedCallback)(struct Actor *actor);
  const s32 *savedScript;
  ///@}
  u8 unusedAfterSavedScript[4];
  u8 savedPalette; ///< the target's first sprite's param, saved likewise
  u8 lit; ///< set in two Mercury Lighthouse scenes: Douse lets particles sway
          ///< down
  s16 tileSlot; ///< resource entry holding the spark tiles
  u16 unusedAfterTileSlot;
  u16 freeBlocks; ///< 512 less the free resource blocks at the start
  /// The map's shake and its decay, restored when the field Psynergy ends.
  ///@{
  s32 savedShakeX;
  s32 savedShakeY;
  s32 savedShakeDecay;
  ///@}
  struct FieldParticle particles[24];
  u8 unusedAfterSlots[2];
  s16 arcTileSlot;    ///< resource entry of Avoid's falling arcs' tiles
  s8 messageMode;     ///< the announcement's box mode: 1, an event adds 8
  u8 paddingAtEnd[3]; ///< up to the struct's word-aligned size
};

STATIC_ASSERT(sizeof(struct FieldEffectState) == 1824);
STATIC_ASSERT(offsetof(struct FieldEffectState, caster) == 16);
STATIC_ASSERT(offsetof(struct FieldEffectState, ability) == 28);
STATIC_ASSERT(offsetof(struct FieldEffectState, savedCallback) == 56);
STATIC_ASSERT(offsetof(struct FieldEffectState, tileSlot) == 70);
STATIC_ASSERT(offsetof(struct FieldEffectState, savedShakeX) == 76);
STATIC_ASSERT(offsetof(struct FieldEffectState, particles) == 88);
STATIC_ASSERT(offsetof(struct FieldEffectState, arcTileSlot) == 1818);
STATIC_ASSERT(offsetof(struct FieldEffectState, messageMode) == 1820);

/// Returns the value step tenths of the way from `from` to `to`; the field
/// Psynergy that uses it steps from 0 to 10, one a frame.
static __inline__ s32 FieldEffect_Interpolate(s32 to, s32 from, s32 step) {
  return from + (to - from) * step / 10;
}

#endif
