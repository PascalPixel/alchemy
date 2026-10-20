#ifndef ALCHEMY_FX_SCENE_H
#define ALCHEMY_FX_SCENE_H

/// @file
/// BattleFxScene, the work of a battle or field Psynergy effect scene, and its
/// arc objects.

#include "types.h"
#include "static_assert.h"
#include "effect_slot.h"

struct AnimationObject;
union FieldObject;

/// A field object as Avoid's caster: its facing, its animation records and
/// the phase and frame count of its per-frame callback.
struct ArcEffectObject {
  u8 unk00[6];                     ///< not named through this view
  u16 angle;                       ///< 1/65536 turns
  u8 unk08[0x48];                  ///< not named through this view
  struct AnimationObject *records; ///< the caster's sprite
  u8 unk54[0x10];                  ///< not named through this view
  s16 phase;                       ///< 1 spins the caster 0xc00 a frame
  s16 step;                        ///< frames since the arcs started
  /// FieldActor.linkedObject; no code reads it through this view.
  struct FieldActor *linkedObject;
  void (*callback)(union FieldObject *object); ///< run each frame
};

/// The work of a Psynergy effect scene: its caster and target, where it takes
/// place, the map state it restores and its 24 effect slots.
/// It is heap slot 56 (HEAP_SLOT_FX, read as gEffectWork), cleared when
/// allocated: BattleFx_LoadActionEffectResources takes it for a Psynergy or
/// item use, and BattleFx_InitializeSlots for the Djinn joining effects and
/// Altmiller Cave's scene. The effect's end releases it.
struct BattleFxScene {
  s32 angle;          ///< 1/65536 turns
  s32 x;              ///< 16.16 fixed-point pixels
  s32 y;              ///< 16.16 fixed-point pixels
  s32 z;              ///< 16.16 fixed-point pixels
  void *mainObject;   ///< the caster's object
  void *child;        ///< the target's object, or NULL
  s16 firstObjectId;  ///< the caster's object id
  s16 secondObjectId; ///< the target's object id, -1 for none
  u16 action;         ///< the ability id
  s16 fieldKind;      ///< the ability's field kind (FIELD_KIND_)
  /// Set at the start: Force then aims in front of the caster; cleared once
  /// the target has been dispatched.
  s8 enabled;
  u8 mode;    ///< where the effect runs; MODE_WORLD_MAP on the world map
  s8 visible; ///< set at the start: the caster flashes with a sound
  s8 active;  ///< set at the start: the ability's name is announced
  void (*finishCallback)(void); ///< run, when set, as Ply's effect ends
  u8 unk28[0x0c];               ///< not named by TBS code
  /// Set by some scenes before an effect; Carry, Frost, Force and Reveal
  /// treat the target differently with it.
  s8 childOption;
  s8 childMode; ///< set by some scenes: Frost and Force leave the target be
  u8 unk36[2];  ///< not named by TBS code
  /// The target's update hook and script, saved at the start and restored
  /// when Move or Carry lets it go.
  ///@{
  void (*savedCallback)(union FieldObject *object);
  const s32 *savedScript;
  ///@}
  u8 unk40[4];     ///< not named by TBS code
  u8 savedPalette; ///< the target's first sprite's param, saved likewise
  u8 lit;          ///< set in two Mercury Lighthouse maps; not read by name
  u16 tileSlot;    ///< resource entry holding the spark tiles
  u16 unk48;       ///< not named by TBS code
  u16 freeBlocks;  ///< 512 less the free resource blocks at the start
  /// The map's shake and its decay, restored when the effect ends.
  ///@{
  s32 savedShakeX;
  s32 savedShakeY;
  s32 savedShakeDecay;
  ///@}
  struct EffectSlot slots[24];
  u8 unk718[2];    ///< not named by TBS code
  s16 arcTileSlot; ///< resource entry of Avoid's falling arcs' tiles
  s8 messageMode;  ///< the announcement's box mode: 1, a scene adds 8
  u8 unk71d[3];    ///< not named by TBS code
};

STATIC_ASSERT(sizeof(struct BattleFxScene) == 0x720);
STATIC_ASSERT(offsetof(struct BattleFxScene, mainObject) == 0x10);
STATIC_ASSERT(offsetof(struct BattleFxScene, action) == 0x1c);
STATIC_ASSERT(offsetof(struct BattleFxScene, savedCallback) == 0x38);
STATIC_ASSERT(offsetof(struct BattleFxScene, tileSlot) == 0x46);
STATIC_ASSERT(offsetof(struct BattleFxScene, savedShakeX) == 0x4c);
STATIC_ASSERT(offsetof(struct BattleFxScene, slots) == 0x58);
STATIC_ASSERT(offsetof(struct BattleFxScene, arcTileSlot) == 0x71a);
STATIC_ASSERT(offsetof(struct BattleFxScene, messageMode) == 0x71c);

/// Returns the value step tenths of the way from `from` to `to`; the Psynergy
/// effects that use it step from 0 to 10, one a frame.
static __inline__ s32 BattleFx_Interpolate(s32 to, s32 from, s32 step) {
  return from + (to - from) * step / 10;
}

#endif
