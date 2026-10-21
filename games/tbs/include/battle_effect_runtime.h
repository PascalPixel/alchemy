#ifndef ALCHEMY_BATTLE_EFFECT_RUNTIME_H
#define ALCHEMY_BATTLE_EFFECT_RUNTIME_H

/// @file
/// The battle animation state, render objects and effect entries, and the
/// palette buffer work.

#include "unit/object/object.h"
#include "object_lookup.h"
#include "types.h"
#include "static_assert.h"
#include "palbuf.h"

/// An actor's animated sprite (struct AnimationObject), as far as the pointer
/// to its sprite id.
struct BattleAnimationState {
  u8 unk00[0x28]; ///< AnimationObject's fields before its layers
  /// AnimationObject.entries[0], its first layer, whose first halfword is the
  /// sprite id it shows (AnimationEntry.animId); null for none.
  s16 *value28;
};

/// A field actor as the speaker lookup reads it: its kind (1 for an animated
/// sprite) and that sprite.
struct BattleRenderObject {
  u8 unk00[0x24];
  s32 unk24;
  u8 unk28[4];
  s32 unk2c;
  s32 unk30;
  s32 unk34;
  s32 unk38;
  u8 unk3c[4];
  s32 unk40;
  u8 unk44[0x0c];
  struct BattleAnimationState *animation; ///< ObjectRuntime.animation
  u8 kind; ///< ObjectRuntime.animationKind: 1 for one animated sprite
};

/// A speaker definition, a row of gFieldFx_DefinitionTable: a sprite, its
/// voice (0xff for none) and the flags that choose its idle line.
struct BattleEffectEntry {
  u16 id; ///< the sprite id
  /// The first of its four voice blip sounds, less 256: a message plays sound
  /// 256 + value as it types (FieldFx_GetResourceId); 0xff for none.
  u8 value;
  u8 flags; ///< choose its line when it has nothing to say (FieldFx_GetFlags)
};

/// The colour buffer, heap slot HEAP_SLOT_PALETTE_BUFFER under its own name.
extern struct BattleEffectBuffers *gPaletteBufferWork;

#endif
