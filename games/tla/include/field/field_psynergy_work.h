#ifndef ALCHEMY_FIELD_PSYNERGY_WORK_H
#define ALCHEMY_FIELD_PSYNERGY_WORK_H

/// @file
/// The field Psynergy's work (FieldEffectWork) and its particle.

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "field_particle.h"
#include "field/effect_sprite.h"

struct Sprite;
union FieldObject;

/// The frame a scene's Psynergy event is given once the cast has ended
/// (Field_RunPsynergyEvent, as Burst passes it): its last call.
#define PSYNERGY_EVENT_LAST_FRAME 999

/// A particle of a field Psynergy's effect: its place and velocity, in 16.16
/// pixels on the field or on the screen, and the frames since it began
/// (negative before it starts, or while it is free).
struct FieldPsynergyParticle {
  FIXED x;
  FIXED y;
  FIXED z;
  FIXED velocityX;
  FIXED velocityY;
  FIXED velocityZ;
  s32 timer;
};

/// The effect sprites a field Psynergy has (FieldEffectWork.particles).
#define PSYNERGY_EFFECT_SLOT_COUNT 24
/// The frame of Scoop's dig at which the ground is dug, its stage 1
/// (FieldPsynergy_Scoop).
#define SCOOP_DUG_FRAME 30

/// The particles of dust and sparkles Scoop throws up.
#define SCOOP_PARTICLE_COUNT 64
/// The earth that rises as the ground is dug, 32 pixels square in 256 colours.
#define SCOOP_PICTURE_SIZE 32

/// Scoop's work (heap slot HEAP_SLOT_BATTLE_EFFECT): the dust and sparkle
/// tiles, the picture that ripples as the ground is dug, 64 particles, the
/// sprite that digs and the one left where it dug, and the stage of the cast.
struct ScoopWork {
  u8 tiles[1024];
  u8 picture[SCOOP_PICTURE_SIZE * SCOOP_PICTURE_SIZE];
  struct EffectSprite sprites[SCOOP_PARTICLE_COUNT];
  struct FieldPsynergyParticle particles[SCOOP_PARTICLE_COUNT];
  struct EffectSprite pictureSprite;
  s16 phase; ///< the stage of the cast; FIELD_PSYNERGY_FINISHED once it ends
  s16 timer; ///< frames into the stage
  s16 particleTile;
  s16 pictureTile;
  struct Actor *actor; ///< SPRITE_SCOOP_HAND, which digs and sinks as it does
  struct Actor *other; ///< SPRITE_BROWN_BOULDER (grey where lit), where it digs
  /// The kind of dig: 2 cast on a target; cast at nothing, 1 one time in
  /// sixteen (an item turns up), else 0, or 3 where lit is set.
  s16 mode;
};

/// The work of a field Psynergy (heap slot HEAP_SLOT_FIELD_EFFECT): its caster
/// and target, where it takes place, the map state it restores, its field
/// effect sprites and the sprite and stage a lasting Psynergy keeps.
struct FieldEffectWork {
  /// The caster's facing, rounded to four directions, 1/65536 turns.
  u16 angle;
  /// The caster's facing, rounded to eight directions, 1/65536 turns.
  u16 angle8;
  FIXED x;                 ///< 16.16 fixed-point pixels
  FIXED y;                 ///< 16.16 fixed-point pixels
  FIXED z;                 ///< 16.16 fixed-point pixels
  struct Actor *mainActor; ///< the caster's actor
  struct Actor *child;     ///< the target's actor, or NULL
  s16 firstActorId;        ///< the caster's actor id
  s16 secondActorId;       ///< the target's actor id, -1 for none
  s16 action;              ///< the ability id
  s16 fieldPsynergy;       ///< the ability's field Psynergy kind
  /// Set at the start; cleared once the target has been dispatched.
  s8 enabled;
  u8 mode;    ///< where it runs; ACTOR_SYSTEM_WORLD_MAP on the World Map
  s8 visible; ///< set at the start: the caster flashes with a sound
  s8 active;  ///< set at the start: the ability's name is announced
  void (*finishCallback)(void); ///< run, when set, as the Psynergy ends
  u8 unusedAfterFinish[12];
  /// Set by some scenes before a field Psynergy; it treats the target
  /// differently with it.
  s8 childOption;
  s8 childMode; ///< set by some scenes: the Psynergy leaves the target be
  u8 paddingAfterChildMode[2];
  /// The target's update routine and script, saved at the start and restored
  /// when the Psynergy lets it go.
  ///@{
  void (*savedCallback)(union FieldObject *object);
  const s32 *savedScript;
  ///@}
  u8 savedPalette; ///< the target's first sprite animation's colour mode
  /// Set in a few scenes (Gabomba Catacombs, Tundaria Tower, Jupiter
  /// Lighthouse's aerie, Loho) before a field Psynergy.
  u8 lit;
  u16 freeBlocks; ///< 512 less the free VRAM cache blocks at the start
  /// The map's shake and its decay, restored when the field Psynergy ends.
  ///@{
  s32 savedShakeX;
  s32 savedShakeY;
  s32 savedShakeDecay;
  ///@}
  /// Its field effect sprites.
  struct FieldParticle particles[PSYNERGY_EFFECT_SLOT_COUNT];
  s16 arcTileSlot; ///< the VRAM cache entry of the falling arcs' tiles
  s8 messageMode;  ///< the announcement's box mode: 1, an event adds 8
  u8 messageShown; ///< set while the announcement's message is open
  u8 unusedAfterMessageShown[4]; ///< no code names it
  /// The caster's priority flags and sprite priority, saved at the start and
  /// restored as it ends.
  ///@{
  u8 savedPriorityFlags;
  u8 savedSpritePriority;
  ///@}
  u8 unusedAfterSavedSpritePriority[170]; ///< no code names it
  /// A lasting Psynergy's stage and frame, passed to its event as it runs.
  ///@{
  s16 stage;
  s16 frame;
  ///@}
  struct EffectSprite sprite; ///< a field Psynergy's own sprite on screen
  s16 spriteVramSlot;         ///< its VRAM cache entry
  s16 spriteActive;           ///< nonzero while sprite is drawn
};

STATIC_ASSERT(sizeof(struct FieldEffectWork) == 2036);
STATIC_ASSERT(offsetof(struct FieldEffectWork, mainActor) == 16);
STATIC_ASSERT(offsetof(struct FieldEffectWork, action) == 28);
STATIC_ASSERT(offsetof(struct FieldEffectWork, childOption) == 52);
STATIC_ASSERT(offsetof(struct FieldEffectWork, savedCallback) == 56);
STATIC_ASSERT(offsetof(struct FieldEffectWork, lit) == 65);
STATIC_ASSERT(offsetof(struct FieldEffectWork, savedShakeX) == 68);
STATIC_ASSERT(offsetof(struct FieldEffectWork, particles) == 80);
STATIC_ASSERT(offsetof(struct FieldEffectWork, messageMode) == 1810);
STATIC_ASSERT(offsetof(struct FieldEffectWork, savedPriorityFlags) == 1816);
STATIC_ASSERT(offsetof(struct FieldEffectWork, stage) == 1988);
STATIC_ASSERT(offsetof(struct FieldEffectWork, sprite) == 1992);
STATIC_ASSERT(offsetof(struct FieldEffectWork, spriteActive) == 2034);

/// Returns the value step tenths of the way from `from` to `to`; the field
/// Psynergy that uses it steps from 0 to 10, one a frame.
static __inline__ s32 FieldEffect_Interpolate(s32 to, s32 from, s32 step) {
  return from + (to - from) * step / 10;
}

#endif
