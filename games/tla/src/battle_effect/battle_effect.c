/// @file
/// The battle effect object, the bank's C in one: the display and HBlank
/// helpers, the canvas layer, the dispatch that runs summon and table effects,
/// and the effects from Ice and Douse to the battler tints, as The Broken
/// Seal's battle_effect.c holds them; then the effects from Cybele to Bolt, the
/// effects of abilities and summons from the opening every Djinn effect shares
/// to Meteor Blow, the Liquifier, and the Megiddo, Moloch and Iris summons,
/// Odyssey and the model renderers the effects draw with. Each part is named
/// for the first ability it draws. battle_effect_internal.h holds the includes
/// every part shares; the parts under battle_effect/ are included here in ROM
/// order. Where two groups of parts name a type or routine otherwise, the later
/// one sees its own through a view, defined before it and undefined after.

#include "battle_effect_internal.h"

// The parts, in ROM order.
#include "battle_effect/display.c"
#include "battle_effect/falling.c"
#include "battle_effect/streaks.c"
#include "battle_effect/target_strike.c"
#include "battle_effect/cruel_ruin.c"
#include "battle_effect/canvas.c"
#include "battle_effect/planet_diver.c"
#include "battle_effect/mirrored_wall.c"
#include "battle_effect/condemn.c"
#include "battle_effect/bind.c"
#include "battle_effect/stars.c"
#include "battle_effect/procne.c"
#include "battle_effect/nereid.c"
#include "battle_effect/vortices.c"
#include "battle_effect/prism.c"
#include "battle_effect/plasma.c"
#include "battle_effect/volcano.c"
#include "battle_effect/target_motes.c"
#include "battle_effect/djinn_heal.c"
#include "battle_effect/dispatch.c"
#include "battle_effect/battler_tint.c"

#undef PHASE_RESULTS
#undef SHARD_COUNT
#undef DROP_COUNT
#undef EFFECT_STEPS
#undef PARTICLES
#undef MOTES
#undef BOLT_COUNT
#undef STAR_COUNT
#undef FORMATION_SIZE
#undef COLUMN_WIDTH
#undef COLUMN_HEIGHT
#undef MOTE_COUNT
#undef PILLAR_FRAMES

#undef EffectSprite_CreateWithTiles
#undef EffectSprite_Acquire
#undef Render_DrawModelList
#undef SLOT_ALT
#undef OBJ_TILES
#undef CLIP
#undef RECORD_FRAMES
#undef HEAP_BLITTER_ALTERNATE
#undef SPARK_COUNT
#undef HEAP_BLITTER
#undef SLOT_BLITTER
#undef BLITTER
#undef HI

// The effects from the Cybele summon to Bolt.
#define FxGroup FxGroup__cybele
#include "battle_effect/cybele.c"
#include "battle_effect/boreas.c"
#include "battle_effect/drain.c"
#include "battle_effect/healing.c"
#include "battle_effect/stat_change.c"
#include "battle_effect/flare.c"
#include "battle_effect/quake.c"
#include "battle_effect/fire.c"
#include "battle_effect/frost.c"
#include "battle_effect/ray.c"
#include "battle_effect/eruption.c"
#include "battle_effect/burst_shower.c"
#include "battle_effect/orbiting.c"
#include "battle_effect/atalanta.c"
#include "battle_effect/growth.c"
#include "battle_effect/bolt.c"

#undef HI
#undef HEAP_BLITTER
#undef HEAP_BLITTER_ALTERNATE
#undef PARTICLES
#undef BLIT_MODE
#undef SHARDS
#undef MOTES
#undef SPARKS
#undef BLITTER_SLOT
#undef FLAME_COUNT
#undef EMBERS
#undef CHIPS
#undef CLIP
#undef SPARK_COUNT
#undef Actor_SetAnimationSpeedFar
#undef STARS
#undef ALTERNATE_BLITTER_SLOT
#undef OBJ_VRAM
#undef COLUMN_COUNT
#undef PHASE_RESULTS
#undef BLITTER_TRANSPARENT
#undef PREPARE_KIND
#undef PREPARE_ANCHOR
#undef BLITTER_MAX

#undef FxGroup
#undef EffectSprite_CreateWithTiles
#undef PUFF_COUNT

// The effects of abilities and summons from the opening every Djinn effect
// shares to Meteor Blow.

#define SpriteAttributes SpriteAttributes__ability_effects
#define FxSlot FxSlot__ability_effects
#define SpriteObject SpriteObject__ability_effects
#define Texture Texture__ability_effects
#define Particle Particle__ability_effects
#define ResourceTableEntry ResourceTableEntry__ability_effects

#include "battle_effect/projectiles.c"
#include "battle_effect/djinn_attacks.c"
#include "battle_effect/hail.c"
#include "battle_effect/thor.c"
#include "battle_effect/spire.c"
#include "battle_effect/flurry.c"
#include "battle_effect/gaia.c"
#include "battle_effect/spider_web.c"
#include "battle_effect/azul.c"
#include "battle_effect/positions.c"
#include "battle_effect/casting_impact.c"
#include "battle_effect/torch.c"
#include "battle_effect/canvas_routine.c"
#include "battle_effect/particle_streams.c"
#include "battle_effect/ramses.c"
#include "battle_effect/annihilation.c"
#include "battle_effect/ragnarok.c"
#include "battle_effect/judgment.c"
#include "battle_effect/kirin.c"
#include "battle_effect/neptune.c"
#include "battle_effect/thrash.c"
#include "battle_effect/undead_sword.c"
#include "battle_effect/crystal.c"
#include "battle_effect/geode.c"
#include "battle_effect/nirvana.c"
#include "battle_effect/heavy_divide.c"
#include "battle_effect/thunder_mine.c"
#include "battle_effect/light_surge.c"
#include "battle_effect/whorl.c"
#include "battle_effect/djinni_charge.c"
#include "battle_effect/rime.c"
#include "battle_effect/gasp.c"
#include "battle_effect/charon.c"
#include "battle_effect/rising_dragon.c"
#include "battle_effect/raging_heat.c"
#include "battle_effect/flora.c"
#include "battle_effect/fire_puppet.c"
#include "battle_effect/fume.c"
#include "battle_effect/ocean_fist.c"
#include "battle_effect/rushing_strike.c"
#include "battle_effect/glowing_lunge.c"
#include "battle_effect/daedalus.c"
#include "battle_effect/vengeance.c"
#include "battle_effect/catastrophe.c"
#include "battle_effect/haures.c"
#include "battle_effect/death_plunge.c"
#include "battle_effect/helm_splitter.c"
#include "battle_effect/dragon_cloud.c"
#include "battle_effect/legend.c"
#include "battle_effect/cards.c"
#include "battle_effect/triple_chomp.c"
#include "battle_effect/breaths.c"
#include "battle_effect/trident.c"
#include "battle_effect/meteor_blow.c"

#undef ResourceTableEntry
#undef Particle
#undef Texture
#undef SpriteObject
#undef FxSlot
#undef SpriteAttributes
#undef Render_ProjectVertices
#undef Render_DrawModelList

#include "battle_effect/liquifier.c"

#undef HI
#undef Actor_SetAnimationSpeedFar

// The Megiddo, Moloch and Iris summons, Odyssey, and the model renderers the
// effects draw with.

#define Model Model__summon_effects
#define Texture Texture__summon_effects
#define ResourceTableEntry ResourceTableEntry__summon_effects
#define BattleBackgroundId BattleBackgroundId__summon_effects
#define ModelVertex ModelVertex__summon_effects

#include "battle_effect/megiddo.c"
#include "battle_effect/odyssey.c"
#include "battle_effect/moloch.c"

#undef ModelVertex
#undef BattleBackgroundId
#undef ResourceTableEntry
#undef gResourceTableEntries
#undef EffectSprite_CreateWithTiles
#undef EffectSprite_Acquire
#undef Texture
#undef Model
