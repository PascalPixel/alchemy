/* @file */
/* The battle effects: what plays on screen when a Psynergy, Djinni, summon, */
/* unleash or monster ability is used. One object built from its parts, which */
/* are included here in ROM order, each named for the first ability it draws: */
/* first the display helpers, the canvas layer, the dispatch that runs summon */
/* and table effects and the effects from Ice and Douse to the battler tints, */
/* as The Broken Seal's battle_effect.c holds them; then the effects from */
/* Cybele to Bolt; then those from the opening every Djinn effect shares to */
/* Meteor Blow, and the Liquifier; last the Megiddo, Moloch and Iris summons, */
/* Odyssey and the model renderers the effects draw with. */
/* battle_effect_internal.h holds what every part shares. Where two groups of */
/* parts name a type or function otherwise, the later one sees its own through
 */
/* a view, defined before it and undefined after. */

#include "battle_effect_internal.h"

/* The parts, in ROM order. */
#include "battle_effect_display.c"
#include "battle_effect_falling.c"
#include "battle_effect_streaks.c"
#include "battle_effect_target_strike.c"
#include "battle_effect_cruel_ruin.c"
#include "battle_effect_canvas.c"
#include "battle_effect_planet_diver.c"
#include "battle_effect_djinn_beams.c"
#include "battle_effect_condemn.c"
#include "battle_effect_bind.c"
#include "battle_effect_stars.c"
#include "battle_effect_procne.c"
#include "battle_effect_nereid.c"
#include "battle_effect_vortices.c"
#include "battle_effect_prism.c"
#include "battle_effect_plasma.c"
#include "battle_effect_volcano.c"
#include "battle_effect_target_motes.c"
#include "battle_effect_djinn_heal.c"
#include "battle_effect_dispatch.c"
#include "battle_effect_battler_tint.c"

/* The effects from the Cybele summon to Bolt. */
#include "battle_effect_cybele.c"
#include "battle_effect_boreas.c"
#include "battle_effect_drain.c"
#include "battle_effect_healing.c"
#include "battle_effect_stat_change.c"
#include "battle_effect_flare.c"
#include "battle_effect_quake.c"
#include "battle_effect_fire.c"
#include "battle_effect_frost.c"
#include "battle_effect_ray.c"
#include "battle_effect_eruption.c"
#include "battle_effect_burst_shower.c"
#include "battle_effect_orbiting.c"
#include "battle_effect_atalanta.c"
#include "battle_effect_growth.c"
#include "battle_effect_bolt.c"

/* The effects of abilities and summons from the opening every Djinn effect */
/* shares to Meteor Blow. */

#include "battle_effect_projectiles.c"
#include "battle_effect_djinn_attacks.c"
#include "battle_effect_hail.c"
#include "battle_effect_thor.c"
#include "battle_effect_spire.c"
#include "battle_effect_flurry.c"
#include "battle_effect_gaia.c"
#include "battle_effect_spider_web.c"
#include "battle_effect_azul.c"
#include "battle_effect_positions.c"
#include "battle_effect_casting_impact.c"
#include "battle_effect_torch.c"
#include "battle_effect_canvas_wipe.c"
#include "battle_effect_particle_streams.c"
#include "battle_effect_ramses.c"
#include "battle_effect_annihilation.c"
#include "battle_effect_ragnarok.c"
#include "battle_effect_judgment.c"
#include "battle_effect_kirin.c"
#include "battle_effect_neptune.c"
#include "battle_effect_thrash.c"
#include "battle_effect_undead_sword.c"
#include "battle_effect_crystal.c"
#include "battle_effect_geode.c"
#include "battle_effect_nirvana.c"
#include "battle_effect_heavy_divide.c"
#include "battle_effect_thunder_mine.c"
#include "battle_effect_light_surge.c"
#include "battle_effect_whorl.c"
#include "battle_effect_djinni_charge.c"
#include "battle_effect_rime.c"
#include "battle_effect_gasp.c"
#include "battle_effect_charon.c"
#include "battle_effect_rising_dragon.c"
#include "battle_effect_raging_heat.c"
#include "battle_effect_flora.c"
#include "battle_effect_fire_puppet.c"
#include "battle_effect_fume.c"
#include "battle_effect_ocean_fist.c"
#include "battle_effect_star_mine.c"
#include "battle_effect_rushing_strike.c"
#include "battle_effect_daedalus.c"
#include "battle_effect_vengeance.c"
#include "battle_effect_catastrophe.c"
#include "battle_effect_haures.c"
#include "battle_effect_death_plunge.c"
#include "battle_effect_helm_splitter.c"
#include "battle_effect_dragon_cloud.c"
#include "battle_effect_legend.c"
#include "battle_effect_cards.c"
#include "battle_effect_triple_chomp.c"
#include "battle_effect_breaths.c"
#include "battle_effect_trident.c"
#include "battle_effect_meteor_blow.c"

#include "battle_effect_liquifier.c"

/* The Megiddo, Moloch and Iris summons, Odyssey, and the model renderers the */
/* effects draw with. */

#include "battle_effect_megiddo.c"
#include "battle_effect_odyssey.c"
#include "battle_effect_moloch.c"
#include "battle_effect_render.c"
