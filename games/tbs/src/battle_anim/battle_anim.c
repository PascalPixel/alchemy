/// @file
/// Everything drawn over the battle while an ability plays comes from this one
/// object. battle_anim_internal.h holds what every animation shares (includes,
/// macros and the frame-loop helpers); the animations are in the files under
/// parts/, included here in the order their code has in the ROM. That order
/// mixes families, Djinn, summons and shared machinery, and each part is one
/// run of it, named for what it holds. Camelot's own files are not
/// recoverable: the ROM shows one object, and these parts are ours, cut where
/// one family ends and the next begins.
///
/// To find the animation a move plays, search for the move's English name: each
/// handler's reach line, the paragraph of its comment that starts
/// "Abilities:", lists the abilities that reach it and the animation each one
/// selects, and each row of gBattleAnim_Handlers (battle_anim_table.c) gives
/// an animation kind's enumerator, the part that defines its handler and, where
/// an ability selects it, the first one that does. README.md gives the words
/// and prefixes the parts use, and ../MATCHING.md the spellings some sites
/// keep to match. The shared machinery is in these parts:
///
///   Entry points    dispatch.c: BattleAnim_RunTableEffect,
///                   BattleAnim_RunSummonEffect; positions.c:
///                   BattleAnim_DispatchByIdRange
///   Canvas          canvas.c: BattleAnim_StartCanvasLayer,
///                   BattleAnim_EndCanvasLayer, BattleAnim_WipeCanvas,
///                   BattleAnim_ProcessPendingGraphicsTransfer
///   Blitters        blitter.c: BattleAnim_BuildBlitter; mirrored_wall.c:
///                   BattleAnim_FetchRectangleBlitters
///   Motion          positions.c: Particle_AdvanceWithGravity2D and 3D,
///                   BattleUnit_ProjectToScreen; hail.c: BattleCamera_Shake
///   Targets         dispatch.c: BattleAnim_SelectLivingTargets;
///                   helm_splitter.c: BattleAnim_PlaceFormationObjects
///
/// While an animation plays, its entry point owns the sheet
/// (HEAP_SLOT_BATTLE_SHEET), the work block (HEAP_SLOT_BATTLE_ANIM) and the
/// canvas (HEAP_SLOT_BATTLE_CANVAS): each of the three entry points allocates
/// them before the handler runs and releases them after it returns, so a
/// handler removes every callback that reads them before it returns. The two
/// blitter slots and gMapGridCellBuffer (PARTICLES) belong to the handler for
/// as long as it runs; it frees the blitters itself before it returns.
///
/// README.md in this folder explains the ideas: how a move picks its animation,
/// the frame loop, the canvas, particles, blitters and pictures.

#include "battle_anim_internal.h"

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
#include "parts/burst_sequence.c"
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
