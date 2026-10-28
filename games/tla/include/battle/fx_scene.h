#ifndef ALCHEMY_FX_SCENE_H
#define ALCHEMY_FX_SCENE_H

#include "types.h"
#include "static_assert.h"
#include "effect_slot.h"

struct AnimationObject;
union FieldObject;

/// A field object as an arc effect's caster: its facing, its animation records
/// and the phase and frame count of its per-frame callback.
struct ArcEffectObject {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x48];
  struct AnimationObject *records;
  u8 unk54[0x10];
  s16 phase; ///< 1 spins the caster
  s16 step;  ///< frames since the arcs started
  struct FieldActor *watchedObject;
  void (*callback)(union FieldObject *object); ///< run each frame
};

/// The work of a Psynergy effect scene: its caster and target, where it takes
/// place, the map state it restores and its 24 effect slots.
struct BattleFxScene {
  s32 angle;          ///< 1/65536 turns
  s32 x;              ///< 16.16 fixed-point pixels
  s32 y;              ///< 16.16 fixed-point pixels
  s32 z;              ///< 16.16 fixed-point pixels
  void *mainObject;   ///< the caster's object
  void *child;        ///< the target's object, or NULL
  s16 firstObjectId;  ///< the caster's object id
  s16 secondObjectId; ///< the target's object id
  u16 action;         ///< the ability id
  s16 fieldKind;      ///< the ability's field kind
  s8 enabled;         ///< cleared once the target has been dispatched
  u8 mode;
  s8 visible;
  s8 active;
  void (*finishCallback)(void); ///< run, when set, as an effect ends
  u8 unk28[0x0c];
  s8 childOption;
  s8 childMode;
  u8 unk36[2];
  void (*savedCallback)(union FieldObject *object);
  const s32 *savedScript;
  u8 unk40[4];
  u8 savedPalette;
  u8 lit;
  u16 tileSlot;
  u16 unk48;
  u16 freeBlocks;
  /// The map's shake and its decay, kept to restore after the effect.
  ///@{
  s32 savedShakeX;
  s32 savedShakeY;
  s32 savedShakeDecay;
  ///@}
  struct EffectSlot slots[24];
  u8 unk718[2];
  s16 arcTileSlot;
  s8 messageMode;
  u8 unk71d[3];
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

static __inline__ s32 BattleEffect_Interpolate(s32 to, s32 from, s32 step) {
  return from + (to - from) * step / 10;
}

#endif
