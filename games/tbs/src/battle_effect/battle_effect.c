/// @file
/// Everything drawn over the battle while an ability plays comes from this one
/// object. battle_effect_internal.h holds what every effect shares (includes,
/// macros and the frame-loop helpers); the effects are in the files under
/// parts/, included here in the order their code has in the ROM. That order
/// mixes families, Djinn, summons and shared machinery, and each part is one
/// run of it, named for what it holds. Camelot's own files are not
/// recoverable: the ROM shows one object, and these parts are ours, cut where
/// one family ends and the next begins.
///
/// To find the effect a move plays, search for the move's English name: each
/// handler's reach line, the paragraph of its comment that starts
/// "Abilities:", lists the abilities that reach it and the effect each one
/// selects, and each row of gBattleEffect_Handlers (battle_effect_table.c)
/// gives an effect kind's enumerator, the part that defines its handler and,
/// where an ability selects it, the first one that does. README.md gives the
/// words and prefixes the parts use, and ../MATCHING.md the spellings some
/// sites keep to match.
///
/// A reach line follows the sentence saying what the player sees, in a form
/// the docs build reads to put each ability's recording beside its handler:
/// `Abilities: ` and one or more groups separated by `; `, each a list of
/// ability names separated by `, `, then a space and the effect the group
/// selects in parentheses, and a full stop. A name is written as the comment
/// above its entry in abilities.c writes it, once in its group even where
/// several rows share it; where rows sharing a name select different
/// effects (the two Blasts), the group's effect says which are meant. A
/// group whose effect no row selects has `none` for its list. Names keep
/// their rows' order, groups follow their effects' values, `none` groups
/// come last, and the line reads as its lines joined by single spaces. The
/// effect is an enumerator whose prefix says how it is reached:
/// BATTLE_EFFECT_ a row of the handler table, SUMMON_EFFECT_ a row with
/// EFFECT_SUMMON, CASTING_IMPACT_ a row with EFFECT_BY_ID_RANGE and a kind from
/// EFFECT_RANGE_CASTING_IMPACT on (the enumerator being the kind less that),
/// FIELD_PSYNERGY_ the fieldPsynergy field in abilities.c. Anything else that
/// reaches a handler (an item, another effect handing over) is a sentence
/// after it; a function no row reaches, such as a runner, has no reach line and
/// says what calls it.
///
/// The shared machinery is in these parts:
///
///   Entry points    dispatch.c: BattleEffect_RunFromTable,
///                   BattleEffect_RunSummon; positions.c:
///                   BattleEffect_RunByIdRange
///   Canvas          canvas.c: BattleEffect_StartCanvasLayer,
///                   BattleEffect_EndCanvasLayer, BattleEffect_WipeCanvas,
///                   BattleEffect_ProcessPendingGraphicsTransfer
///   Blitters        blitter.c: BattleEffect_BuildBlitter; mirrored_wall.c:
///                   BattleEffect_FetchRectangleBlitters
///   Motion          positions.c: Particle_AdvanceWithGravity2D and 3D,
///                   BattleUnit_ProjectToScreen; hail.c: BattleCamera_Shake
///   Targets         dispatch.c: BattleEffect_SelectLivingTargets;
///                   helm_splitter.c: BattleEffect_PlaceFormationObjects
///
/// While an effect plays, its entry point owns the sheet
/// (HEAP_SLOT_BATTLE_SHEET), the work block (HEAP_SLOT_BATTLE_EFFECT) and the
/// canvas (HEAP_SLOT_BATTLE_CANVAS): each of the three entry points allocates
/// them before the handler runs and releases them after it returns, so a
/// handler removes every task that reads them before it returns. The two
/// blitter slots and gMapGridCellBuffer (PARTICLES) belong to the handler for
/// as long as it runs; it frees the blitters itself before it returns.
///
/// README.md in this folder explains the ideas: how a move picks its effect,
/// the frame loop, the canvas, particles, blitters and pictures.

#include "battle_effect_internal.h"

// The parts, in ROM order: the order of these includes is the order of the
// code in the ROM.
#include "parts/display.c"
#include "parts/falling.c"
#include "parts/streaks.c"
#include "parts/target_strike.c"
#include "parts/death_plunge.c"
#include "parts/spider_web.c"
#include "parts/canvas.c"
#include "parts/planet_diver.c"
#include "parts/gathering_lights.c"
#include "parts/mirrored_wall.c"
#include "parts/djinn_beams.c"
#include "parts/lobbed_shells.c"
#include "parts/condemn.c"
#include "parts/bind.c"
#include "parts/stars.c"
#include "parts/procne.c"
#include "parts/tiamat.c"
#include "parts/nereid.c"
#include "parts/vortices.c"
#include "parts/prism.c"
#include "parts/plasma.c"
#include "parts/volcano.c"
#include "parts/target_motes.c"
#include "parts/djinn_heal.c"
#include "parts/dispatch.c"
#include "parts/cybele.c"
#include "parts/boreas.c"
#include "parts/drain.c"
#include "parts/healing.c"
#include "parts/stat_change.c"
#include "parts/flare.c"
#include "parts/quake.c"
#include "parts/fire.c"
#include "parts/frost.c"
#include "parts/ray.c"
#include "parts/eruption.c"
#include "parts/burst_shower.c"
#include "parts/orbiting.c"
#include "parts/atalanta.c"
#include "parts/growth.c"
#include "parts/bolt.c"
#include "parts/projectiles.c"
#include "parts/rushing_strike.c"
#include "parts/djinn_attacks.c"
#include "parts/hail.c"
#include "parts/thor.c"
#include "parts/spire.c"
#include "parts/flurry.c"
#include "parts/gaia.c"
#include "parts/positions.c"
#include "parts/beams.c"
#include "parts/casting_impact.c"
#include "parts/torch.c"
#include "parts/helm_splitter.c"
#include "parts/particle_streams.c"
#include "parts/ramses.c"
#include "parts/dragon_cloud.c"
#include "parts/annihilation.c"
#include "parts/ragnarok.c"
#include "parts/titan_blade.c"
#include "parts/judgment.c"
#include "parts/kirin.c"
#include "parts/neptune.c"
#include "parts/thrash.c"
#include "parts/undead_sword.c"
#include "parts/blitter.c"
