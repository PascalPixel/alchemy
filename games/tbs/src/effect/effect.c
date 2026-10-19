/// @file
/// The battle effect object: everything drawn over the battle while an
/// ability plays. effect_internal.h holds what every effect shares (includes,
/// macros and the frame-loop helpers); the effects are in the files under
/// parts/, included here in the order their code has in the ROM. That order
/// mixes families, Djinn, summons and shared machinery, and each part is one
/// run of it, named for what it holds. Camelot's own files are not
/// recoverable: the ROM shows one object, and these parts are ours, cut where
/// one family ends and the next begins.
///
/// To find the effect a move plays, search for the move's English name: each
/// handler's "Abilities:" comment lists the abilities that reach it, and each
/// row of gBattleFx_EffectHandlers (effect_table.c) gives its kind, its part
/// and its first ability. README.md gives the prefixes and the spellings the
/// parts use. The shared machinery:
///
///   Entry points    dispatch.c: BattleFx_RunTableEffect,
///                   BattleFx_RunSceneEffect; positions.c:
///                   BattleFx_DispatchByIdRange
///   Canvas          canvas.c: BattleFx_BeginCanvasLayer,
///                   BattleFx_EndCanvasLayer, BattleFx_WipeCanvas,
///                   BattleFx_ProcessPendingGraphicsTransfer
///   Blitters        blitter.c: BattleFx_BuildBlitter; mirrored_wall.c:
///                   BattleFx_FetchRectangleBlitters
///   Motion          positions.c: EffectStep_AdvanceWithGravity2D and 3D,
///                   BattleUnit_ProjectToScreen; hail.c: Camera_ApplyShake
///   Targets         dispatch.c: BattleFx_SelectLivingTargets;
///                   helm_splitter.c: BattleFx_PlaceFormationObjects
///
/// README.md in this folder explains the ideas: the canvas, blitters, the
/// frame loop, particles, pictures and how to change an effect.

#include "effect_internal.h"

// The effect object's code, in ROM order. Each part is compiled here, as
// part of this one object, never on its own: the order of these includes is
// the order of the code in the ROM.
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
#include "parts/burst_scene.c"
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
