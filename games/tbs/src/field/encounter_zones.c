// The 104 encounter zones, by zone number. A field zone's comment gives its area name
// (AreaNameRules) and the scenes and entrances that take it (SceneEncounterRules); a World Map
// zone's, its encounter region and the battle environments that take it (WorldMapEncounterRules); a
// zone that code names, its name (ENCOUNTER_ZONE_ID). Over each zone's formations
// (battle/formations.c) are the monsters they bring, by their English names: for a zone code picks
// a slot from, what each slot fights.

#include "field/encounter_zone.h"

// clang-format off
const ENCOUNTER_ZONE EncounterZones[] = {
    // ENCOUNTER_ZONE_NONE
    {0},
    // Sol Sanctum: SceneIdSolSanctumCorridor
    {
        .rate = 37,
        .level = 1,
        // Vermin, Bat and Wild Mushroom.
        .formations = {7, 8, 9, 10},
        .weights = {4, 8, 8, 4},
    },
    // Sol Sanctum: SceneIdSolSanctumReliefs entrances 1 and 2
    {
        .rate = 35,
        .level = 1,
        // Vermin, Bat and Wild Mushroom.
        .formations = {7, 8, 9, 10},
        .weights = {8, 4, 4, 8},
    },
    // Sol Sanctum: SceneIdSolSanctumReliefs entrances 3, 6 and 7
    {
        .rate = 35,
        .level = 1,
        // Slime, Bat, Wild Mushroom and Vermin.
        .formations = {11, 12, 13, 14},
        .weights = {8, 8, 4, 4},
    },
    // Sol Sanctum: SceneIdSolSanctumMaze
    {
        .rate = 35,
        .level = 1,
        // Slime, Bat, Wild Mushroom and Vermin.
        .formations = {11, 12, 13, 14},
        .weights = {4, 4, 8, 8},
    },
    // Sol Sanctum: SceneIdSolSanctumReliefs entrance 10
    {
        .rate = 35,
        .level = 2,
        // Amaze, Slime, Vermin, Bat and Wild Mushroom.
        .formations = {15, 16, 17, 18},
        .weights = {12, 8, 4, 4},
    },
    // Sol Sanctum: SceneIdSolSanctumLunaSolRooms
    {
        .rate = 35,
        .level = 2,
        // Amaze, Slime, Vermin, Bat and Wild Mushroom.
        .formations = {15, 16, 17, 18},
        .weights = {4, 4, 12, 8},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_VALE_VAULT: beach, shallows and grassland
    {
        .rate = 25,
        .level = 3,
        // Vermin, Bat, Wild Mushroom, Slime, Amaze and Zombie.
        .formations = {20, 21, 22, 23, 24, 25, 26, 27},
        .weights = {4, 4, 4, 4, 2, 2, 2, 2},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_VALE_VAULT: forest
    {
        .rate = 9,
        .level = 3,
        // Vermin, Bat, Wild Mushroom, Slime, Amaze and Zombie.
        .formations = {20, 21, 22, 23, 24, 25, 26, 27},
        .weights = {2, 2, 2, 2, 3, 7, 2, 3},
    },
    // Goma Cave: SceneIdGomaCavePond
    {
        .rate = 30,
        .level = 4,
        // Ghost, Vermin, Wild Mushroom, Zombie, Bat, Slime and Skeleton.
        .formations = {31, 32, 33, 34, 35},
        .weights = {3, 3, 9, 3, 6},
    },
    // Goma Cave: SceneIdGomaCavePillar
    {
        .rate = 30,
        .level = 5,
        // Ghost, Vermin, Wild Mushroom, Zombie, Bat, Slime, Skeleton and Will

        // Head.
        .formations = {31, 32, 33, 34, 35, 36, 37},
        .weights = {2, 2, 2, 2, 8, 5, 2},
    },
    // Goma Cave: SceneIdGomaCaveStream
    {
        .rate = 30,
        .level = 5,
        // Ghost, Vermin, Wild Mushroom, Zombie, Bat, Slime, Skeleton and Will

        // Head.
        .formations = {31, 32, 33, 34, 35, 36, 37},
        .weights = {2, 2, 5, 2, 8, 8, 14},
    },
    // ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT
    {
        // By slot, 0: Vermin; 1: Bat; 2: Wild Mushroom; 3: Vermin and Bat; 4: Mystery Woman and Mystery Man; 5: Bandit and Thief; 6: Zombie.
        .formations = {1, 2, 3, 4, 5, 29, 28},
        .weights = {10, 10, 10, 10, 10, 10, 1},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_BILIBIN_KOLIMA: beach, shallows and grassland
    {
        .rate = 25,
        .level = 6,
        // Rat, Zombie, Skeleton, Ghost, Bat, Rat Soldier and Will Head.
        .formations = {50, 51, 52, 53, 54, 55, 56, 57},
        .weights = {4, 4, 4, 4, 4, 2, 2, 2},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_BILIBIN_KOLIMA: forest
    {
        .rate = 9,
        .level = 6,
        // Rat, Zombie, Skeleton, Ghost, Bat, Rat Soldier and Will Head.
        .formations = {50, 51, 52, 53, 54, 55, 56, 57},
        .weights = {1, 1, 1, 1, 1, 2, 2, 2},
    },
    // Kolima Tunnel: SceneIdKolimaTunnel
    {
        .rate = 35,
        .level = 6,
        // Rat, Zombie, Skeleton, Ghost, Bat, Rat Soldier and Will Head.
        .formations = {50, 51, 52, 53, 54, 55, 56, 57},
        .weights = {1, 2, 2, 1, 1, 5, 5, 5},
    },
    // Kolima Forest: SceneIdKolimaForestEntrance
    {
        .rate = 40,
        .level = 6,
        // Drone Bee, Rat, Rat Soldier, Will Head, Skeleton and Troll.
        .formations = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {10, 5, 1, 1, 1, 1},
    },
    // Kolima Forest: SceneIdKolimaForestGlade
    {
        .rate = 40,
        .level = 7,
        // Drone Bee, Rat, Rat Soldier, Will Head, Skeleton and Troll.
        .formations = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {2, 6, 2, 2, 8, 6, 2, 2},
    },
    // Kolima Forest: SceneIdKolimaForestCliff
    {
        .rate = 40,
        .level = 7,
        // Drone Bee, Rat, Rat Soldier, Will Head, Skeleton and Troll.
        .formations = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {1, 1, 2, 2, 3, 3, 8, 12},
    },
    // Kolima Forest: SceneIdKolimaForestTree
    {
        .rate = 40,
        .level = 7,
        // Drone Bee, Rat, Rat Soldier, Will Head, Skeleton and Troll.
        .formations = {59, 60, 61, 62, 63, 64, 65, 66},
        .weights = {1, 1, 2, 2, 3, 3, 5, 12},
    },
    // Tret Tree: SceneIdTretTreeBranch entrances 1 to 3
    {
        .rate = 45,
        .level = 7,
        // Spider, Rat Soldier, Rat, Drone Bee, Troll, Creeper and Gnome.
        .formations = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {8, 5, 6, 2, 1, 2, 1, 1},
    },
    // Tret Tree: SceneIdTreeRooms entrances 1, 11, 16 and 21 to 29
    {
        .rate = 45,
        .level = 7,
        // Spider, Rat Soldier, Rat, Drone Bee, Troll, Creeper and Gnome.
        .formations = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {6, 6, 6, 3, 2, 1},
    },
    // Tret Tree: SceneIdTreeRooms entrances 2 to 5, 12, 13, 31 and 32
    {
        .rate = 45,
        .level = 8,
        // Spider, Rat Soldier, Rat, Drone Bee, Troll, Creeper and Gnome.
        .formations = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {3, 5, 5, 5, 3, 2, 1, 1},
    },
    // Tret Tree: SceneIdTretTreeBranch, its other entrances
    {
        .rate = 45,
        .level = 8,
        // Spider, Rat Soldier, Rat, Drone Bee, Troll, Creeper and Gnome.
        .formations = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {2, 1, 5, 8, 1, 8, 1, 5},
    },
    // Tret Tree: SceneIdTreeRooms entrances 6 to 9, 14 and 51 to 59
    {
        .rate = 45,
        .level = 8,
        // Spider, Rat Soldier, Rat, Drone Bee, Troll, Creeper and Gnome.
        .formations = {68, 69, 70, 71, 72, 73, 74, 75},
        .weights = {1, 1, 1, 1, 2, 4, 12, 8},
    },
    // Bilibin Cave: SceneIdBilibinCaveRapier, its other entrances
    {
        .rate = 30,
        .level = 8,
        // Ooze, Ghoul, Troll, Drone Bee, Rat Soldier and Gnome.
        .formations = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {6, 6, 4, 4, 2, 2, 1, 1},
    },
    // Bilibin Cave: SceneIdBilibinCaveIce, its other entrances
    {
        .rate = 30,
        .level = 8,
        // Ooze, Ghoul, Troll, Drone Bee, Rat Soldier and Gnome.
        .formations = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {2, 2, 2, 4, 4, 5, 7, 7},
    },
    // Bilibin Cave: SceneIdBilibinCaveRapier entrances 5 and 6
    {
        .rate = 30,
        .level = 9,
        // Ooze, Ghoul, Troll, Drone Bee, Rat Soldier and Gnome.
        .formations = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {1, 1, 1, 1, 1, 2, 2, 2},
    },
    // Bilibin Cave: SceneIdBilibinCaveIce entrance 5
    {
        .rate = 30,
        .level = 9,
        // Ooze, Ghoul, Troll, Drone Bee, Rat Soldier and Gnome.
        .formations = {77, 78, 79, 80, 81, 82, 83, 84},
        .weights = {1, 1, 1, 1, 1, 2, 2, 2},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD: snowfield
    {
        .rate = 25,
        .level = 9,
        // Gnome, Ooze and Mauler.
        .formations = {86, 87, 88, 89, 90},
        .weights = {3, 3, 5, 5, 5},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD: snow forest
    {
        .rate = 9,
        .level = 9,
        // Gnome, Ooze and Mauler.
        .formations = {86, 87, 88, 89, 90},
        .weights = {2, 2, 4, 4, 4},
    },
    // Mercury Lighthouse: SceneIdMercuryLighthouseEntrance entrances 7 to 12 and 16 to 20, SceneIdMercuryLighthouseLower entrances 1 to 3
    {
        .rate = 40,
        .level = 9,
        // Ooze, Cuttle, Gnome, Lizard Man, Mauler, Ghoul and Siren.
        .formations = {92, 93, 94, 95, 96, 97, 98, 99},
        .weights = {4, 4, 4, 2, 2, 2, 2, 2},
    },
    // Mercury Lighthouse: SceneIdMercuryLighthouseLower entrances 5 to 8 and 11, SceneIdMercuryLighthouseMiddle entrances 3 and 4
    {
        .rate = 40,
        .level = 9,
        // Ooze, Cuttle, Gnome, Lizard Man, Mauler, Ghoul and Siren.
        .formations = {92, 93, 94, 95, 96, 97, 98, 99},
        .weights = {2, 2, 2, 4, 4, 2, 2, 2},
    },
    // Mercury Lighthouse: SceneIdMercuryLighthouseUpper entrances 1 to 3, SceneIdMercuryLighthouseMiddle entrances 5, 6 and 10
    {
        .rate = 40,
        .level = 10,
        // Lizard Man, Cuttle, Ghoul, Siren and Harpy.
        .formations = {97, 98, 99, 100, 101, 102, 103, 104},
        .weights = {2, 2, 2, 4, 4, 4, 2, 2},
    },
    // Mercury Lighthouse: SceneIdMercuryLighthouseExterior entrances 1 and 2, SceneIdMercuryLighthouseUpper entrances 12 and 13
    {
        .rate = 40,
        .level = 11,
        // Lizard Man, Cuttle, Ghoul, Siren and Harpy.
        .formations = {97, 98, 99, 100, 101, 102, 103, 104},
        .weights = {1, 1, 1, 2, 2, 2, 4, 4},
    },
    // Mercury Lighthouse: SceneIdMercuryLighthouseExterior entrances 3 and 4
    {
        .rate = 40,
        .level = 11,
        // Lizard Man, Cuttle, Ghoul, Siren and Harpy.
        .formations = {97, 98, 99, 100, 101, 102, 103, 104},
        .weights = {1, 1, 1, 2, 2, 2, 4, 4},
    },
    // ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY
    {
        // By slot, 0: Tret; 1: Lizard Man; 2: Saturos.
        .formations = {106, 107, 108},
        .weights = {10, 10, 10},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_FUCHIN: grassland
    {
        .rate = 20,
        .level = 11,
        // Ooze, Mauler, Lizard Man and Mole.
        .formations = {120, 121, 122, 123},
        .weights = {3, 3, 3, 3},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_FUCHIN: forest
    {
        .rate = 9,
        .level = 11,
        // Ooze, Mauler, Lizard Man and Mole.
        .formations = {120, 121, 122, 123},
        .weights = {1, 1, 1, 1},
    },
    // Fuchin Falls Cave: SceneIdFuchinFallsCaveEntrance, SceneIdFuchinFallsCaveZephyr
    {
        .rate = 38,
        .level = 11,
        // Ooze, Mauler, Lizard Man, Mole, Spirit, Dirge, Skeleton and Bone

        // Fighter.
        .formations = {120, 121, 122, 123, 124, 125, 126, 127},
        .weights = {4, 4, 4, 8, 6, 4, 2, 1},
    },
    // Fuchin Falls Cave: SceneIdFuchinFallsCaveDragonsEye
    {
        .rate = 38,
        .level = 11,
        // Ooze, Mauler, Lizard Man, Mole, Spirit, Dirge, Skeleton and Bone

        // Fighter.
        .formations = {120, 121, 122, 123, 124, 125, 126, 127},
        .weights = {1, 2, 2, 3, 6, 6, 3, 2},
    },
    // Fuchin Falls Cave: SceneIdFuchinFallsCaveDragonStatue
    {
        .rate = 38,
        .level = 11,
        // Ooze, Mauler, Lizard Man, Mole, Spirit, Dirge, Skeleton and Bone

        // Fighter.
        .formations = {120, 121, 122, 123, 124, 125, 126, 127},
        .weights = {1, 1, 2, 3, 4, 4, 6, 8},
    },
    // Mogall Forest: SceneIdMogallForestSign
    {
        .rate = 42,
        .level = 11,
        // Mole, Dirge, Spirit, Bone Fighter and Death Head.
        .formations = {129, 130, 131, 132, 133, 134, 135, 136},
        .weights = {6, 6, 6, 3, 2, 1, 1, 1},
    },
    // Mogall Forest: SceneIdMogallForestQuartz
    {
        .rate = 42,
        .level = 12,
        // Mole, Dirge, Spirit, Bone Fighter and Death Head.
        .formations = {129, 130, 131, 132, 133, 134, 135, 136},
        .weights = {1, 1, 3, 6, 6, 3, 1, 1},
    },
    // Mogall Forest: SceneIdMogallForestKillerApe
    {
        .rate = 42,
        .level = 12,
        // Mole, Dirge, Spirit, Bone Fighter and Death Head.
        .formations = {129, 130, 131, 132, 133, 134, 135, 136},
        .weights = {1, 1, 2, 2, 3, 4, 5, 5},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_XIAN_ALTIN: desert, beach, shallows and grassland
    {
        .rate = 25,
        .level = 12,
        // Mole, Dirge, Ape, Bone Fighter, Spirit and Death Head.
        .formations = {141, 142, 143, 144, 145, 146},
        .weights = {3, 3, 1, 3, 1, 1},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_XIAN_ALTIN: forest
    {
        .rate = 9,
        .level = 12,
        // Mole, Dirge, Ape, Bone Fighter, Spirit and Death Head.
        .formations = {141, 142, 143, 144, 145, 146},
        .weights = {1, 1, 2, 1, 2, 2},
    },
    // Altin Peak: SceneIdAltinPeakRailTunnel, SceneIdAltinPeakSecondWaterBeast, SceneIdAltinPeakSouthEntrance
    {
        .rate = 45,
        .level = 12,
        // Dirge, Bone Fighter, Slime Beast, Ape, Tarantula, Calamar and Rat

        // Fighter.
        .formations = {148, 149, 150, 151, 152, 153, 154, 155},
        .weights = {6, 6, 4, 2, 1, 1, 1, 1},
    },
    // Altin Peak: SceneIdAltinPeakThirdWaterBeast, SceneIdAltinPeakPool
    {
        .rate = 45,
        .level = 13,
        // Dirge, Bone Fighter, Slime Beast, Ape, Tarantula, Calamar and Rat

        // Fighter.
        .formations = {148, 149, 150, 151, 152, 153, 154, 155},
        .weights = {1, 1, 2, 4, 3, 4, 2, 2},
    },
    // Altin Peak: SceneIdAltinPeakSpritz, SceneIdAltinPeakJunction, SceneIdAltinPeakFallingRocks entrances 1 and 2, SceneIdAltinPeakWestExit
    {
        .rate = 42,
        .level = 13,
        // Dirge, Bone Fighter, Slime Beast, Ape, Tarantula, Calamar and Rat

        // Fighter.
        .formations = {148, 149, 150, 151, 152, 153, 154, 155},
        .weights = {1, 1, 2, 3, 4, 4, 8, 8},
    },
    // Lamakan Desert: SceneIdLamakanDesertEast
    {
        .rate = 40,
        .level = 13,
        // Orc, Grub, Rat Fighter, Fighter Bee and Salamander.
        .formations = {160, 161, 162, 163, 164, 165, 166, 167},
        .weights = {5, 5, 4, 3, 3, 2, 1, 1},
    },
    // Lamakan Desert: SceneIdLamakanDesertSmog
    {
        .rate = 40,
        .level = 14,
        // Orc, Grub, Rat Fighter, Fighter Bee and Salamander.
        .formations = {160, 161, 162, 163, 164, 165, 166, 167},
        .weights = {1, 2, 3, 6, 6, 4, 2, 1},
    },
    // Lamakan Desert: SceneIdLamakanDesertManticore
    {
        .rate = 40,
        .level = 14,
        // Orc, Grub, Rat Fighter, Fighter Bee and Salamander.
        .formations = {160, 161, 162, 163, 164, 165, 166, 167},
        .weights = {1, 1, 1, 2, 3, 4, 4, 5},
    },
    // ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN
    {
        // By slot, 0: Ape; 1: Killer Ape; 2: Living Statue; 3: Hydros Statue; 4: Ant Lion; 5: Manticore.
        .formations = {138, 139, 157, 158, 169, 170},
        .weights = {10, 10, 10, 10, 10, 10},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_KALAY: desert, beach, shallows and grassland
    {
        .rate = 25,
        .level = 15,
        // Mad Vermin, Fighter Bee, Death Cap and Undead.
        .formations = {180, 181, 182, 183, 184, 185, 186, 187},
        .weights = {4, 4, 4, 4, 4, 2, 2, 2},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_KALAY: forest
    {
        .rate = 9,
        .level = 15,
        // Mad Vermin, Fighter Bee, Death Cap and Undead.
        .formations = {180, 181, 182, 183, 184, 185, 186, 187},
        .weights = {2, 2, 2, 2, 3, 4, 4, 4},
    },
    // Vale Cave: SceneIdValeCaveKite entrances 1 to 4
    {
        .rate = 40,
        .level = 15,
        // Mad Vermin, Death Cap, Fighter Bee, Ravager, Undead and Ghost Mage.
        .formations = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {6, 6, 6, 4, 4, 1, 1, 1},
    },
    // Vale Cave: SceneIdValeCaveKite entrances 8 to 10
    {
        .rate = 40,
        .level = 15,
        // Mad Vermin, Death Cap, Fighter Bee, Ravager, Undead and Ghost Mage.
        .formations = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {4, 4, 4, 6, 6, 2, 2, 2},
    },
    // Vale Cave: SceneIdValeCaveLogs entrances 1 to 3
    {
        .rate = 40,
        .level = 15,
        // Mad Vermin, Death Cap, Fighter Bee, Ravager, Undead and Ghost Mage.
        .formations = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {2, 2, 2, 3, 3, 5, 5, 5},
    },
    // Vale Cave: SceneIdValeCaveLogs entrances 4 and 5
    {
        .rate = 40,
        .level = 15,
        // Mad Vermin, Death Cap, Fighter Bee, Ravager, Undead and Ghost Mage.
        .formations = {189, 190, 191, 192, 193, 194, 195, 196},
        .weights = {1, 1, 1, 2, 2, 5, 5, 5},
    },
    // Vault Cave: SceneIdVaultCavePassage
    {
        .rate = 40,
        .level = 16,
        // Fighter Bee, Death Cap, Ravager, Ghost Mage, Undead, Gargoyle and

        // Cave Troll.
        .formations = {201, 202, 203, 204, 205, 206, 207, 208},
        .weights = {8, 8, 8, 4, 4, 4, 2, 2},
    },
    // Vault Cave: SceneIdVaultCaveVambrace, SceneIdVaultCaveMouth
    {
        .rate = 40,
        .level = 16,
        // Fighter Bee, Death Cap, Ravager, Ghost Mage, Undead, Gargoyle and

        // Cave Troll.
        .formations = {201, 202, 203, 204, 205, 206, 207, 208},
        .weights = {2, 2, 2, 4, 4, 4, 8, 8},
    },
    // ENCOUNTER_ZONE_TOLBI_SHIP
    {
        .level = 17,
        // By slot, 0: Man o' War; 1: Lizard Fighter and Rabid Bat; 2: Virago and Man o' War; 3: Kraken.
        .formations = {210, 211, 212, 213},
        .weights = {10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS
    {
        .level = 17,
        // By slot, 0: Hobgoblin and Virago; 1: Grisly and Succubus; 2: Lich and

        // Fiendish Ghoul; 3: Gryphon.
        .formations = {291, 292, 293, 294},
        .weights = {10, 10, 10, 10},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_TOLBI: desert, beach, shallows and grassland
    {
        .rate = 25,
        .level = 18,
        // Mad Mole, Armored Rat, Rabid Bat, Foul Dirge and Gnome Mage.
        .formations = {220, 221, 222, 223, 224, 225, 226, 227},
        .weights = {6, 6, 6, 6, 3, 3, 3, 3},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_TOLBI: forest
    {
        .rate = 9,
        .level = 18,
        // Mad Mole, Armored Rat, Rabid Bat, Foul Dirge and Gnome Mage.
        .formations = {220, 221, 222, 223, 224, 225, 226, 227},
        .weights = {3, 3, 3, 3, 6, 6, 6, 6},
    },
    // Altmiller Cave: SceneIdAltmillerCaveEntrance
    {
        .rate = 40,
        .level = 17,
        // Armored Rat, Foul Dirge, Mad Mole, Worm, Gnome Mage, Wight, Golem, Clay Gargoyle and Dread Hound.
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {6, 6, 4, 4, 4, 2, 1, 1},
    },
    // Altmiller Cave: SceneIdAltmillerCaveBabi
    {
        .rate = 40,
        .level = 17,
        // Armored Rat, Foul Dirge, Mad Mole, Worm, Gnome Mage, Wight, Golem, Clay Gargoyle and Dread Hound.
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {4, 4, 6, 6, 4, 4, 2, 1},
    },
    // Altmiller Cave: SceneIdAltmillerCaveVialChest
    {
        .rate = 40,
        .level = 18,
        // Armored Rat, Foul Dirge, Mad Mole, Worm, Gnome Mage, Wight, Golem, Clay Gargoyle and Dread Hound.
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {3, 3, 4, 6, 6, 4, 2, 1},
    },
    // Altmiller Cave: SceneIdAltmillerCaveSquall
    {
        .rate = 40,
        .level = 18,
        // Armored Rat, Foul Dirge, Mad Mole, Worm, Gnome Mage, Wight, Golem, Clay Gargoyle and Dread Hound.
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {2, 2, 4, 4, 6, 5, 5, 3},
    },
    // Altmiller Cave: SceneIdAltmillerCaveCookieChest
    {
        .rate = 40,
        .level = 18,
        // Armored Rat, Foul Dirge, Mad Mole, Worm, Gnome Mage, Wight, Golem, Clay Gargoyle and Dread Hound.
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {2, 2, 3, 3, 4, 6, 6, 4},
    },
    // Altmiller Cave: SceneIdAltmillerCaveDraught entrances 50 and 51
    {
        .rate = 40,
        .level = 18,
        // Armored Rat, Foul Dirge, Mad Mole, Worm, Gnome Mage, Wight, Golem, Clay Gargoyle and Dread Hound.
        .formations = {229, 230, 231, 232, 233, 234, 235, 236},
        .weights = {1, 1, 1, 2, 2, 4, 6, 6},
    },
    // ENCOUNTER_ZONE_COLOSSO
    {
        .level = 19,
        // By slot, 0: Azart; 1: Azart; 2: Satrage; 3: Satrage; 4: Navampa; 5: Navampa.
        .formations = {238, 239, 240, 241, 242, 243},
        .weights = {10, 10, 10, 10, 10, 10},
    },
    // Gondowan Cave: SceneIdGondowanCave, its other entrances
    {
        .rate = 30,
        .level = 19,
        // Rat Warrior, Warrior Bee, Dirty Ape and Kobold.
        .formations = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // Gondowan Cave: SceneIdGondowanCaveRiver, its other entrances
    {
        .rate = 30,
        .level = 20,
        // Rat Warrior, Warrior Bee, Dirty Ape and Kobold.
        .formations = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // Gondowan Cave: SceneIdGondowanCaveRiver entrances 1 and 2
    {
        .rate = 30,
        .level = 20,
        // Rat Warrior, Warrior Bee, Dirty Ape and Kobold.
        .formations = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // Gondowan Cave: SceneIdGondowanCave entrances 1 to 3
    {
        .rate = 30,
        .level = 20,
        // Rat Warrior, Warrior Bee, Dirty Ape and Kobold.
        .formations = {250, 251, 252, 253, 254, 255, 256, 257},
        .weights = {3, 3, 4, 4, 5, 5, 6, 6},
    },
    // Lunpa Fortress: SceneIdLunpaFortressCave entrances 1 and 2
    {
        .rate = 35,
        .level = 21,
        // Rat Warrior, Vile Dirge and Kobold.
        .formations = {259, 260, 261, 262},
        .weights = {2, 2, 2, 2},
    },
    // Lunpa Fortress: SceneIdLunpaFortressCave, its other entrances
    {
        .rate = 35,
        .level = 21,
        // Rat Warrior, Vile Dirge and Kobold.
        .formations = {259, 260, 261, 262},
        .weights = {2, 2, 2, 2},
    },
    // Lunpa Fortress: SceneIdLunpaFortressToadonpa entrances 1 and 2
    {
        .rate = 35,
        .level = 21,
        // Rat Warrior, Vile Dirge and Kobold.
        .formations = {259, 260, 261, 262},
        .weights = {2, 2, 2, 2},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_SUHALLA: desert, beach, shallows and grassland
    {
        .rate = 25,
        .level = 22,
        // Orc Captain, Rat Warrior, Warrior Bee, Kobold, Dirty Ape, Vile Dirge and Brutal Troll.
        .formations = {268, 269, 270, 271, 272, 273, 274, 275},
        .weights = {4, 4, 3, 3, 3, 3, 3, 3},
    },
    // World Map, WORLD_MAP_ENCOUNTER_REGION_SUHALLA: forest
    {
        .rate = 9,
        .level = 22,
        // Orc Captain, Rat Warrior, Warrior Bee, Kobold, Dirty Ape, Vile Dirge and Brutal Troll.
        .formations = {268, 269, 270, 271, 272, 273, 274, 275},
        .weights = {2, 2, 3, 3, 3, 4, 4, 4},
    },
    // Suhalla Desert: SceneIdSuhallaDesertNorth
    {
        .rate = 40,
        .level = 22,
        // Acid Maggot, Kobold, Roach, Vile Dirge, Brutal Troll, Magicore, Harridan, Orc Captain and Stone Soldier.
        .formations = {277, 278, 279, 280, 281, 282, 284, 285},
        .weights = {4, 4, 4, 5, 3, 4, 2, 2},
    },
    // Suhalla Desert: SceneIdSuhallaDesertSouth
    {
        .rate = 40,
        .level = 22,
        // Acid Maggot, Kobold, Roach, Vile Dirge, Brutal Troll, Magicore, Harridan, Orc Captain and Stone Soldier.
        .formations = {277, 278, 279, 280, 281, 282, 284, 285},
        .weights = {2, 2, 2, 4, 3, 4, 4, 4},
    },
    // ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS
    {
        .level = 17,
        // By slot, 0: Lizard King, Harridan and Stone Soldier; 1: Chimera; 2: Earth Lizard; 3: Poison Toad and Thunder Lizard; 4: Cerebus; 5: Deadbeard.
        .formations = {296, 297, 298, 300, 301, 302},
        .weights = {10, 10, 10, 10, 10, 10},
    },
    // Suhalla Gate: SceneIdSuhallaGateDew entrances 1 and 2
    {
        .rate = 30,
        .level = 23,
        // Cannibal Ghoul, Horned Ghost, Gnome Wizard, Earth Golem, Goblin, Mole

        // Mage and Nightmare.
        .formations = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {6, 6, 6, 3, 3, 2, 2, 2},
    },
    // Suhalla Gate: SceneIdSuhallaGateDew, its other entrances
    {
        .rate = 30,
        .level = 24,
        // Cannibal Ghoul, Horned Ghost, Gnome Wizard, Earth Golem, Goblin, Mole

        // Mage and Nightmare.
        .formations = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {2, 3, 3, 6, 6, 3, 3, 3},
    },
    // Gateway Cave: SceneIdGatewayCave entrances 1 and 2
    {
        .rate = 30,
        .level = 24,
        // Cannibal Ghoul, Horned Ghost, Gnome Wizard, Earth Golem, Goblin, Mole

        // Mage and Nightmare.
        .formations = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {1, 1, 2, 2, 2, 3, 3, 3},
    },
    // Gateway Cave: SceneIdGatewayCave, its other entrances
    {
        .rate = 30,
        .level = 24,
        // Cannibal Ghoul, Horned Ghost, Gnome Wizard, Earth Golem, Goblin, Mole

        // Mage and Nightmare.
        .formations = {310, 311, 312, 313, 314, 315, 316, 317},
        .weights = {2, 2, 3, 3, 3, 6, 6, 6},
    },
    // Venus Lighthouse: SceneIdVenusLighthouseGroundFloor, its other entrances
    {
        .rate = 40,
        .level = 25,
        // Horned Ghost, Nightmare, Earth Golem, Gnome Wizard and Goblin.
        .formations = {319, 320, 321, 322, 323},
        .weights = {4, 4, 4, 4, 4},
    },
    // Venus Lighthouse: SceneIdVenusLighthouseGroundFloor entrances 20 to 26 and 30 to 32
    {
        .rate = 40,
        .level = 25,
        // Horned Ghost, Nightmare, Earth Golem, Gnome Wizard and Goblin.
        .formations = {319, 320, 321, 322, 323},
        .weights = {4, 4, 4, 4, 4},
    },
    // Babi Lighthouse Entry and Babi Lighthouse: SceneIdBabiLighthouseEntry, SceneIdBabiLighthouseLuff, SceneIdBabiLighthouseStoneDoor, SceneIdTunnelRuinsLadyStatues entrances 1 to 3
    {
        .rate = 35,
        .level = 25,
        // Gnome Wizard, Plated Rat, Skull Warrior, Willowisp, Orc Lord, Wild

        // Gryphon, Ice Gargoyle, Nightmare, Manticore King and Chimera Mage.
        .formations = {328, 329, 330, 331, 332, 333, 334, 335},
        .weights = {6, 6, 6, 4, 4, 4, 2, 2},
    },
    // Tunnel Ruins: SceneIdTunnelRuinsGreatStatue entrances 1, 2, 5 to 7 and 14 to 18, SceneIdTunnelRuinsLadyStatues entrances 8 to 11
    {
        .rate = 35,
        .level = 25,
        // Gnome Wizard, Plated Rat, Skull Warrior, Willowisp, Orc Lord, Wild

        // Gryphon, Ice Gargoyle, Nightmare, Manticore King and Chimera Mage.
        .formations = {328, 329, 330, 331, 332, 333, 334, 335},
        .weights = {3, 3, 4, 6, 6, 4, 3, 3},
    },
    // Tunnel Ruins: SceneIdTunnelRuinsLadyStatues entrances 12 and 15 to 17
    {
        .rate = 35,
        .level = 26,
        // Gnome Wizard, Plated Rat, Skull Warrior, Willowisp, Orc Lord, Wild

        // Gryphon, Ice Gargoyle, Nightmare, Manticore King and Chimera Mage.
        .formations = {328, 329, 330, 331, 332, 333, 334, 335},
        .weights = {3, 3, 3, 4, 4, 4, 6, 6},
    },
    // Venus Lighthouse: SceneIdVenusLighthouseFirstFloor, its other entrances
    {
        .rate = 34,
        .level = 27,
        // Wild Gryphon, Grand Golem, Manticore King, Skull Warrior, Chimera

        // Mage, Fenrir, Recluse, Willowisp, Boulder Beast, Thunder Lizard and Ice Gargoyle.
        .formations = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {4, 4, 3, 3, 3, 3, 2, 2},
    },
    // Venus Lighthouse: SceneIdVenusLighthouseFirstFloor entrances 19 and 20, SceneIdVenusLighthouseSecondFloor
    {
        .rate = 32,
        .level = 28,
        // Wild Gryphon, Grand Golem, Manticore King, Skull Warrior, Chimera

        // Mage, Fenrir, Recluse, Willowisp, Boulder Beast, Thunder Lizard and Ice Gargoyle.
        .formations = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {3, 3, 4, 4, 3, 3, 2, 2},
    },
    // Venus Lighthouse: SceneIdVenusLighthouseThirdFloor
    {
        .rate = 30,
        .level = 29,
        // Wild Gryphon, Grand Golem, Manticore King, Skull Warrior, Chimera

        // Mage, Fenrir, Recluse, Willowisp, Boulder Beast, Thunder Lizard and Ice Gargoyle.
        .formations = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {2, 2, 3, 3, 4, 4, 3, 3},
    },
    // Venus Lighthouse: SceneIdVenusLighthouseFourthFloor
    {
        .rate = 30,
        .level = 30,
        // Wild Gryphon, Grand Golem, Manticore King, Skull Warrior, Chimera

        // Mage, Fenrir, Recluse, Willowisp, Boulder Beast, Thunder Lizard and Ice Gargoyle.
        .formations = {337, 338, 339, 340, 341, 342, 343, 344},
        .weights = {2, 2, 2, 3, 3, 3, 4, 4},
    },
    // ENCOUNTER_ZONE_SCRIPTED
    {
        // By slot, 0: Fusion Dragon; 1: Menardi and Saturos; 2: Brigand; 3: Brigand; 4: Toadonpa; 5: Tornado Lizard; 6: Storm Lizard; 7: Tempest Lizard.
        .formations = {347, 346, 264, 265, 266, 287, 288, 289},
        .weights = {10, 10, 30, 10, 10, 80, 10, 10},
    },
    // ENCOUNTER_ZONE_MIMIC
    {
        // By slot, 0: Mimic; 1: Mimic; 2: Mimic; 3: Mimic; 4: Mimic; 5: Mimic; 6: Mimic; 7: Mimic.
        .formations = {350, 351, 352, 353, 354, 355, 356, 357},
        .weights = {10, 10, 10, 10, 10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_VENUS_DJINN
    {
        // By slot, 0: Venus Djinni; 1: Venus Djinni; 2: Venus Djinni; 3: Venus

        // Djinni; 4: Venus Djinni; 5: Venus Djinni; 6: Venus Djinni.
        .formations = {360, 360, 360, 361, 361, 361, 362},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_MERCURY_DJINN
    {
        // By slot, 0: Mercury Djinni; 1: Mercury Djinni; 2: Mercury Djinni; 3: Mercury Djinni; 4: Mercury Djinni; 5: Mercury Djinni; 6: Mercury Djinni.
        .formations = {365, 365, 365, 366, 367, 367, 368},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_MARS_DJINN
    {
        // By slot, 0: Mars Djinni; 1: Mars Djinni; 2: Mars Djinni; 3: Mars

        // Djinni; 4: Mars Djinni; 5: Mars Djinni; 6: Mars Djinni.
        .formations = {371, 371, 372, 372, 372, 373, 373},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
    // ENCOUNTER_ZONE_JUPITER_DJINN
    {
        // By slot, 0: Jupiter Djinni; 1: Jupiter Djinni; 2: Jupiter Djinni; 3: Jupiter Djinni; 4: Jupiter Djinni; 5: Jupiter Djinni; 6: Jupiter Djinni.
        .formations = {376, 376, 377, 377, 377, 378, 379},
        .weights = {10, 10, 10, 10, 10, 10, 10},
    },
};
// clang-format on
