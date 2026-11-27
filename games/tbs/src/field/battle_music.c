// The music a battle plays: the bosses, the guards of Crossbone Isle, the Colosso finals and the
// fights with Saturos have their own, and every other battle plays the usual battle music.

#include "field/battle_music.h"
#include "sounds.h"
#include "field/field_mode.h"

// Battle music: which battles have music of their own (fdEncounterSelectBattleMusic).
const Uint16 EncounterBattleMusic[35] = {
    ENCOUNTER_BATTLE_MUSIC(MUS_BATTLE_WITH_THE_FUSION_DRAGON),
    // The Fusion Dragon, Venus Lighthouse's aerie.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_SCRIPTED, 0),

    ENCOUNTER_BATTLE_MUSIC(MUS_BATTLE_WITH_SATUROS_AND_MENARDI),
    // Saturos and Menardi, Venus Lighthouse's aerie.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_SCRIPTED, 1),
    // Deadbeard, on the Pirate Ship.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 5),

    ENCOUNTER_BATTLE_MUSIC(MUS_BATTLING_A_POWERFUL_FOE),
    // Tret, in Tret Tree.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, 0),
    // The Killer Ape, in Mogall Forest.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 1),
    // The Hydros Statue, on Altin Peak.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 3),
    // The Manticore, in Lamakan Desert.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 5),
    // The Kraken, the Tolbi-bound ship's last attack.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_TOLBI_SHIP, 3),
    // Toadonpa, in the Lunpa Fortress.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_SCRIPTED, 4),
    // The Storm Lizard, in Suhalla Desert's chasing sandstorm.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_SCRIPTED, 6),
    // The Tempest Lizard, in Suhalla Desert's fourth sandstorm.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_SCRIPTED, 7),
    // The monsters behind the east doors of Crossbone Isle Cave's floors, the first to the ninth: a
    // Hobgoblin and a Virago; a Grisly and a Succubus; a Lich and a Fiendish Ghoul; a Gryphon; a
    // Lizard King, a Harridan and a Stone Soldier; a Chimera; an Earth Lizard; a Poison Toad and a
    // Thunder Lizard; a Cerebus.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 0),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 1),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 2),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 3),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 0),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 1),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 2),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 3),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 4),

    ENCOUNTER_BATTLE_MUSIC(MUS_COLOSSEUM_FINALS),
    // The Colosso finals: Azart with and without the Iron Shield, Satrage with and without the
    // Chain Mail, Navampa with and without the Claymore.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_COLOSSO, 0),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_COLOSSO, 1),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_COLOSSO, 2),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_COLOSSO, 3),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_COLOSSO, 4),
    ENCOUNTER_KEY(ENCOUNTER_ZONE_COLOSSO, 5),

    ENCOUNTER_BATTLE_MUSIC(MUS_SWORN_ENEMIES_STAND_IN_YOUR_WAY),
    // The Mystery Man and Mystery Woman, in Vale's plaza.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 4),
    // Saturos, at Mercury Lighthouse's aerie.
    ENCOUNTER_KEY(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, 2),

    ENCOUNTER_BATTLE_MUSIC(MUS_BATTLE),
    0,
};
