#ifndef ALCHEMY_ENEMY_IDS_H
#define ALCHEMY_ENEMY_IDS_H

/// @file
/// Enemy ids, named from the English release.

/// The first enemy id: enemy id `ENEMY_ID_FIRST + n` is the enemy in
/// row n of enemy_table.c (Enemy_GetDefinition); the ids below it are the
/// party's characters. A formation lists its members by row.
#define ENEMY_ID_FIRST 8

/// Enemy id `id`'s row of enemy_table.c, as a formation lists its members.
#define ENEMY_ROW(id) ((id) - ENEMY_ID_FIRST)

/// Enemy ids, one for each row of enemy_table.c in order: an enemy id is the
/// row plus ENEMY_ID_FIRST, and a row the code doesn't name yet keeps its row
/// number as its placeholder. Where several rows share a name, each takes what
/// tells its battle apart: the one place it is fought (its formation in
/// formations.c, chosen from a zone of encounter_zones.c), the prize a Colosso
/// finalist carries, or the Djinni a Djinni battle is with; a row fought in
/// several places takes `_2` onwards in row order.
enum EnemyId {
  ENEMY_000 = ENEMY_ID_FIRST,
  /// Level 1: fought on the stormy night in Vale (slots 0 and 3 of
  /// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT) and in Sol Sanctum.
  ENEMY_VERMIN,
  /// Level 3: fought on the world map around Vale and Vault and in Goma Cave.
  ENEMY_VERMIN_2,
  ENEMY_MAD_VERMIN,
  /// Level 1: fought on the stormy night in Vale (slots 1 and 3 of
  /// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT) and in Sol Sanctum.
  ENEMY_BAT,
  /// Level 3: fought on the world map and in Goma Cave and the Kolima Tunnel.
  ENEMY_BAT_2,
  ENEMY_RABID_BAT,
  /// Level 1: fought on the stormy night in Vale (slot 2 of
  /// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT) and in Sol Sanctum.
  ENEMY_WILD_MUSHROOM,
  /// Level 3: fought on the world map around Vale and Vault and in Goma Cave.
  ENEMY_WILD_MUSHROOM_2,
  ENEMY_DEATH_CAP,
  /// Level 2: fought only in Sol Sanctum (formations 11 to 14 and 16).
  ENEMY_SLIME_SOL_SANCTUM,
  /// Level 3: the Slime of the world map's first encounter region, around
  /// Vale and Vault (formations 23 and 27), and of Goma Cave (formation 32).
  /// Fought in both, it has no one place to be named for.
  ENEMY_SLIME_2,
  ENEMY_OOZE,
  ENEMY_SLIME_BEAST,
  ENEMY_GHOST,
  ENEMY_GHOST_MAGE,
  ENEMY_HORNED_GHOST,
  ENEMY_LICH,
  /// Level 4: fought on the world map and in Goma Cave and the Kolima Tunnel.
  ENEMY_ZOMBIE,
  ENEMY_UNDEAD,
  ENEMY_WIGHT,
  ENEMY_BANDIT,
  ENEMY_THIEF,
  ENEMY_BRIGAND,
  ENEMY_SKELETON,
  ENEMY_BONE_FIGHTER,
  ENEMY_SKULL_WARRIOR,
  ENEMY_WILL_HEAD,
  ENEMY_DEATH_HEAD,
  ENEMY_WILLOWISP,
  ENEMY_RAT,
  ENEMY_ARMORED_RAT,
  ENEMY_PLATED_RAT,
  ENEMY_RAT_SOLDIER,
  ENEMY_RAT_FIGHTER,
  ENEMY_RAT_WARRIOR,
  ENEMY_DRONE_BEE,
  ENEMY_FIGHTER_BEE,
  ENEMY_WARRIOR_BEE,
  ENEMY_TROLL,
  ENEMY_CAVE_TROLL,
  ENEMY_BRUTAL_TROLL,
  ENEMY_SPIDER,
  ENEMY_TARANTULA,
  ENEMY_RECLUSE,
  ENEMY_GNOME,
  ENEMY_GNOME_MAGE,
  ENEMY_GNOME_WIZARD,
  ENEMY_GHOUL,
  ENEMY_FIENDISH_GHOUL,
  ENEMY_CANNIBAL_GHOUL,
  ENEMY_MAULER,
  ENEMY_RAVAGER,
  ENEMY_GRISLY,
  ENEMY_CUTTLE,
  ENEMY_CALAMAR,
  ENEMY_MAN_O_WAR,
  ENEMY_HARPY,
  ENEMY_VIRAGO,
  ENEMY_HARRIDAN,
  ENEMY_SIREN,
  ENEMY_SUCCUBUS,
  ENEMY_NIGHTMARE,
  ENEMY_MOLE,
  ENEMY_MAD_MOLE,
  ENEMY_MOLE_MAGE,
  ENEMY_DIRGE,
  ENEMY_FOUL_DIRGE,
  ENEMY_VILE_DIRGE,
  ENEMY_APE,
  ENEMY_KILLER_APE,
  ENEMY_DIRTY_APE,
  ENEMY_GRUB,
  ENEMY_WORM,
  ENEMY_ACID_MAGGOT,
  ENEMY_ORC,
  ENEMY_ORC_CAPTAIN,
  ENEMY_ORC_LORD,
  ENEMY_SALAMANDER,
  ENEMY_EARTH_LIZARD,
  ENEMY_THUNDER_LIZARD,
  ENEMY_MANTICORE,
  ENEMY_MAGICORE,
  ENEMY_MANTICORE_KING,
  ENEMY_KOBOLD,
  ENEMY_GOBLIN,
  ENEMY_HOBGOBLIN,
  ENEMY_GARGOYLE,
  ENEMY_CLAY_GARGOYLE,
  ENEMY_ICE_GARGOYLE,
  ENEMY_GRYPHON,
  ENEMY_WILD_GRYPHON,
  ENEMY_WISE_GRYPHON,
  ENEMY_GOLEM,
  ENEMY_EARTH_GOLEM,
  ENEMY_GRAND_GOLEM,
  ENEMY_DREAD_HOUND,
  ENEMY_CEREBUS,
  ENEMY_FENRIR,
  ENEMY_STONE_SOLDIER,
  ENEMY_BOULDER_BEAST,
  ENEMY_RAGING_ROCK,
  ENEMY_CHIMERA,
  ENEMY_CHIMERA_MAGE,
  ENEMY_104,
  /// Level 2: fought only in Sol Sanctum.
  ENEMY_AMAZE_SOL_SANCTUM,
  /// Level 3: fought only on the world map around Vale and Vault.
  ENEMY_AMAZE_WORLD_MAP,
  ENEMY_CREEPER,
  ENEMY_SPIRIT,
  /// The battle that ends Flint's lesson on the world map
  /// (WorldMap_ShowDjinnTutorial, slot 6 of
  /// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT): ENEMY_ZOMBIE's stats but agility 1,
  /// and it leaves no coins, item or experience.
  ENEMY_ZOMBIE_DJINN_LESSON,
  ENEMY_110,
  ENEMY_111,
  ENEMY_112,
  ENEMY_KRAKEN,
  ENEMY_MYSTERY_MAN,
  /// Level 18.
  ENEMY_SATUROS_MERCURY_LIGHTHOUSE,
  /// Level 38.
  ENEMY_SATUROS_VENUS_LIGHTHOUSE,
  ENEMY_MYSTERY_WOMAN,
  ENEMY_MENARDI,
  ENEMY_LIZARD_MAN,
  ENEMY_LIZARD_FIGHTER,
  ENEMY_LIZARD_KING,
  ENEMY_LIVING_STATUE,
  ENEMY_HYDROS_STATUE,
  /// The Colosso finalists, two rows each: whoever reaches a match's arena
  /// first takes its better prize, and the battle (ENCOUNTER_ZONE_COLOSSO,
  /// one formation a row) is with the opponent carrying what he took.
  /// Azart having reached the arena first: defense 20, against 2 without the
  /// shield.
  ENEMY_AZART_IRON_SHIELD,
  /// Azart having reached the arena second.
  ENEMY_AZART_PADDED_GLOVES,
  /// Satrage having reached the arena first: defense 28, against 6 without
  /// the mail.
  ENEMY_SATRAGE_CHAIN_MAIL,
  /// Satrage having reached the arena second.
  ENEMY_SATRAGE_COTTON_SHIRT,
  /// Navampa having reached the arena first: attack 274, against 232 without
  /// the Claymore.
  ENEMY_NAVAMPA_CLAYMORE,
  /// Navampa having reached the arena second.
  ENEMY_NAVAMPA_HUNTERS_SWORD,
  ENEMY_ANT_LION,
  ENEMY_ROACH,
  ENEMY_DOODLE_BUG,
  ENEMY_TOADONPA,
  ENEMY_POISON_TOAD,
  ENEMY_135,
  ENEMY_TORNADO_LIZARD,
  ENEMY_STORM_LIZARD,
  ENEMY_TEMPEST_LIZARD,
  ENEMY_TRET,
  ENEMY_FUSION_DRAGON,
  ENEMY_DEADBEARD,
  ENEMY_MIMIC_MERCURY_LIGHTHOUSE,
  ENEMY_MIMIC_FUCHIN_FALLS_CAVE,
  ENEMY_MIMIC_ALTIN_PEAK,
  ENEMY_MIMIC_VAULT_CAVE,
  ENEMY_MIMIC_CROSSBONE_ISLE,
  ENEMY_MIMIC_ALTMILLER_CAVE,
  ENEMY_MIMIC_SUHALLA_DESERT,
  ENEMY_MIMIC_PIRATE_SHIP,
  /// The Djinn the party fights, each element's in order of level;
  /// BattleUnit_Set puts them on the party's side. A Djinni's battle is a slot
  /// of its element's zone (gEncounter_DjinnBattles), which Djinn who join
  /// without a fight share; each row is named for the one Djinni whose scene
  /// starts its battle (Field_RequestDjinnBattle).
  /// Level 12, in Mogall Forest.
  ENEMY_VENUS_DJINNI_QUARTZ,
  /// Level 15, on the world map.
  ENEMY_VENUS_DJINNI_VINE,
  /// Unused: level 23, the battle of Bane's slot, but Bane joins on Crossbone
  /// Isle without a fight (Djinn_JoinParty), so no battle reaches this row.
  ENEMY_VENUS_DJINNI_UNUSED_BANE,
  /// Level 10, in Mercury Lighthouse.
  ENEMY_MERCURY_DJINNI_SLEET,
  /// Level 13, on Altin Peak.
  ENEMY_MERCURY_DJINNI_SPRITZ,
  /// Level 18, on the world map.
  ENEMY_MERCURY_DJINNI_HAIL,
  /// Level 24, at Suhalla Gate.
  ENEMY_MERCURY_DJINNI_DEW,
  /// Level 5, in Goma Cave.
  ENEMY_MARS_DJINNI_FORGE,
  /// Level 13, on the world map.
  ENEMY_MARS_DJINNI_CORONA,
  /// Level 22, in the Suhalla Desert.
  ENEMY_MARS_DJINNI_FLASH,
  /// Level 8, in Tret Tree.
  ENEMY_JUPITER_DJINNI_BREEZE,
  /// Level 11, in Fuchin Falls Cave.
  ENEMY_JUPITER_DJINNI_ZEPHYR,
  /// Level 18, in Altmiller Cave.
  ENEMY_JUPITER_DJINNI_SQUALL,
  /// Level 25, in Babi Lighthouse.
  ENEMY_JUPITER_DJINNI_LUFF,
  ENEMY_164
};

#endif
