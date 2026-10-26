#ifndef ALCHEMY_FX_WORK_H
#define ALCHEMY_FX_WORK_H

/// @file
/// FieldEffectWork, the work of a field Psynergy effect.

#include "types.h"
#include "static_assert.h"
#include "field_effect_sprite.h"

struct Sprite;
union ActorOrEffect;

/// The work of a Psynergy effect: its caster and target, where it takes
/// place, the map state it restores and its 24 field effect sprites.
/// It is the HEAP_SLOT_FIELD_EFFECT heap slot, read as gFieldEffectWork,
/// cleared when allocated: FieldEffect_LoadActionEffectResources takes it for a
/// Psynergy or item use, and FieldEffect_InitializeSlots for the Djinn joining
/// effects and Altmiller Cave's map. The effect's end releases it.
struct FieldEffectWork {
  s32 angle;          ///< 1/65536 turns
  s32 x;              ///< 16.16 fixed-point pixels
  s32 y;              ///< 16.16 fixed-point pixels
  s32 z;              ///< 16.16 fixed-point pixels
  void *mainObject;   ///< the caster's object
  void *child;        ///< the target's object, or NULL
  s16 firstObjectId;  ///< the caster's object id
  s16 secondObjectId; ///< the target's object id, -1 for none
  u16 action;         ///< the ability id
  s16 fieldPsynergy;  ///< the ability's field kind (FIELD_PSYNERGY_)
  /// Set at the start: Force then aims in front of the caster; cleared once
  /// the target has been dispatched.
  s8 enabled;
  u8 mode; ///< where the effect runs; OBJECT_SYSTEM_WORLD_MAP on the world map
  s8 visible; ///< set at the start: the caster flashes with a sound
  s8 active;  ///< set at the start: the ability's name is announced
  void (*finishCallback)(void); ///< run, when set, as Ply's effect ends
  u8 unusedAfterFinish[0x0c];   ///< no code names it
  /// Set by some maps before an effect; Carry, Frost, Force and Reveal
  /// treat the target differently with it.
  s8 childOption;
  s8 childMode; ///< set by some maps: Frost and Force leave the target be
  u8 paddingAfterChildMode[2]; ///< up to the word savedCallback starts on
  /// The target's update hook and script, saved at the start and restored
  /// when Move or Carry lets it go.
  ///@{
  void (*savedCallback)(union ActorOrEffect *fieldObject);
  const s32 *savedScript;
  ///@}
  u8 unusedAfterSavedScript[4]; ///< no code names it
  u8 savedPalette; ///< the target's first sprite's param, saved likewise
  u8 lit;          ///< set in two Mercury Lighthouse maps; not read by name
  u16 tileSlot;    ///< resource entry holding the spark tiles
  u16 unusedAfterTileSlot; ///< no code names it
  u16 freeBlocks;          ///< 512 less the free resource blocks at the start
  /// The map's shake and its decay, restored when the effect ends.
  ///@{
  s32 savedShakeX;
  s32 savedShakeY;
  s32 savedShakeDecay;
  ///@}
  struct FieldEffectSprite sprites[24];
  u8 unusedAfterSlots[2]; ///< no code names it
  s16 arcTileSlot;        ///< resource entry of Avoid's falling arcs' tiles
  s8 messageMode;         ///< the announcement's box mode: 1, a script adds 8
  u8 paddingAtEnd[3];     ///< up to the struct's word-aligned size
};

STATIC_ASSERT(sizeof(struct FieldEffectWork) == 0x720);
STATIC_ASSERT(offsetof(struct FieldEffectWork, mainObject) == 0x10);
STATIC_ASSERT(offsetof(struct FieldEffectWork, action) == 0x1c);
STATIC_ASSERT(offsetof(struct FieldEffectWork, savedCallback) == 0x38);
STATIC_ASSERT(offsetof(struct FieldEffectWork, tileSlot) == 0x46);
STATIC_ASSERT(offsetof(struct FieldEffectWork, savedShakeX) == 0x4c);
STATIC_ASSERT(offsetof(struct FieldEffectWork, sprites) == 0x58);
STATIC_ASSERT(offsetof(struct FieldEffectWork, arcTileSlot) == 0x71a);
STATIC_ASSERT(offsetof(struct FieldEffectWork, messageMode) == 0x71c);

/// Returns the value step tenths of the way from `from` to `to`; the Psynergy
/// effects that use it step from 0 to 10, one a frame.
static __inline__ s32 FieldEffect_Interpolate(s32 to, s32 from, s32 step) {
  return from + (to - from) * step / 10;
}

#endif
