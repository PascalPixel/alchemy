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
  /// ★ Named from the game: "Vermin" (MsgEnemyName)
  ENEMY_VERMIN,
  /// Level 3: fought on the world map around Vale and Vault and in Goma Cave.
  /// ★ Named from the game: "Vermin" (MsgEnemyName)
  ENEMY_VERMIN_2,
  /// ★ Named from the game: "Mad Vermin" (MsgEnemyName)
  ENEMY_MAD_VERMIN,
  /// Level 1: fought on the stormy night in Vale (slots 1 and 3 of
  /// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT) and in Sol Sanctum.
  /// ★ Named from the game: "Bat" (MsgEnemyName)
  ENEMY_BAT,
  /// Level 3: fought on the world map and in Goma Cave and the Kolima Tunnel.
  /// ★ Named from the game: "Bat" (MsgEnemyName)
  ENEMY_BAT_2,
  /// ★ Named from the game: "Rabid Bat" (MsgEnemyName)
  ENEMY_RABID_BAT,
  /// Level 1: fought on the stormy night in Vale (slot 2 of
  /// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT) and in Sol Sanctum.
  /// ★ Named from the game: "Wild Mushroom" (MsgEnemyName)
  ENEMY_WILD_MUSHROOM,
  /// Level 3: fought on the world map around Vale and Vault and in Goma Cave.
  /// ★ Named from the game: "Wild Mushroom" (MsgEnemyName)
  ENEMY_WILD_MUSHROOM_2,
  /// ★ Named from the game: "Death Cap" (MsgEnemyName)
  ENEMY_DEATH_CAP,
  /// Level 2: fought only in Sol Sanctum (formations 11 to 14 and 16).
  /// ★ Named from its area name: Sol Sanctum
  ENEMY_SLIME_SOL_SANCTUM,
  /// Level 3: the Slime of the world map's first encounter region, around
  /// Vale and Vault (formations 23 and 27), and of Goma Cave (formation 32).
  /// Fought in both, it has no one place to be named for.
  ENEMY_SLIME_2,
  /// ★ Named from the game: "Ooze" (MsgEnemyName)
  ENEMY_OOZE,
  /// ★ Named from the game: "Slime Beast" (MsgEnemyName)
  ENEMY_SLIME_BEAST,
  /// ★ Named from the game: "Ghost" (MsgEnemyName)
  ENEMY_GHOST,
  /// ★ Named from the game: "Ghost Mage" (MsgEnemyName)
  ENEMY_GHOST_MAGE,
  /// ★ Named from the game: "Horned Ghost" (MsgEnemyName)
  ENEMY_HORNED_GHOST,
  /// ★ Named from the game: "Lich" (MsgEnemyName)
  ENEMY_LICH,
  /// Level 4: fought on the world map and in Goma Cave and the Kolima Tunnel.
  /// ★ Named from the game: "Zombie" (MsgEnemyName)
  ENEMY_ZOMBIE,
  /// ★ Named from the game: "Undead" (MsgEnemyName)
  ENEMY_UNDEAD,
  /// ★ Named from the game: "Wight" (MsgEnemyName)
  ENEMY_WIGHT,
  /// ★ Named from the game: "Bandit" (MsgEnemyName)
  ENEMY_BANDIT,
  /// ★ Named from the game: "Thief" (MsgEnemyName)
  ENEMY_THIEF,
  /// ★ Named from the game: "Brigand" (MsgEnemyName)
  ENEMY_BRIGAND,
  /// ★ Named from the game: "Skeleton" (MsgEnemyName)
  ENEMY_SKELETON,
  /// ★ Named from the game: "Bone Fighter" (MsgEnemyName)
  ENEMY_BONE_FIGHTER,
  /// ★ Named from the game: "Skull Warrior" (MsgEnemyName)
  ENEMY_SKULL_WARRIOR,
  /// ★ Named from the game: "Will Head" (MsgEnemyName)
  ENEMY_WILL_HEAD,
  /// ★ Named from the game: "Death Head" (MsgEnemyName)
  ENEMY_DEATH_HEAD,
  /// ★ Named from the game: "Willowisp" (MsgEnemyName)
  ENEMY_WILLOWISP,
  /// ★ Named from the game: "Rat" (MsgEnemyName)
  ENEMY_RAT,
  /// ★ Named from the game: "Armored Rat" (MsgEnemyName)
  ENEMY_ARMORED_RAT,
  /// ★ Named from the game: "Plated Rat" (MsgEnemyName)
  ENEMY_PLATED_RAT,
  /// ★ Named from the game: "Rat Soldier" (MsgEnemyName)
  ENEMY_RAT_SOLDIER,
  /// ★ Named from the game: "Rat Fighter" (MsgEnemyName)
  ENEMY_RAT_FIGHTER,
  /// ★ Named from the game: "Rat Warrior" (MsgEnemyName)
  ENEMY_RAT_WARRIOR,
  /// ★ Named from the game: "Drone Bee" (MsgEnemyName)
  ENEMY_DRONE_BEE,
  /// ★ Named from the game: "Fighter Bee" (MsgEnemyName)
  ENEMY_FIGHTER_BEE,
  /// ★ Named from the game: "Warrior Bee" (MsgEnemyName)
  ENEMY_WARRIOR_BEE,
  /// ★ Named from the game: "Troll" (MsgEnemyName)
  ENEMY_TROLL,
  /// ★ Named from the game: "Cave Troll" (MsgEnemyName)
  ENEMY_CAVE_TROLL,
  /// ★ Named from the game: "Brutal Troll" (MsgEnemyName)
  ENEMY_BRUTAL_TROLL,
  /// ★ Named from the game: "Spider" (MsgEnemyName)
  ENEMY_SPIDER,
  /// ★ Named from the game: "Tarantula" (MsgEnemyName)
  ENEMY_TARANTULA,
  /// ★ Named from the game: "Recluse" (MsgEnemyName)
  ENEMY_RECLUSE,
  /// ★ Named from the game: "Gnome" (MsgEnemyName)
  ENEMY_GNOME,
  /// ★ Named from the game: "Gnome Mage" (MsgEnemyName)
  ENEMY_GNOME_MAGE,
  /// ★ Named from the game: "Gnome Wizard" (MsgEnemyName)
  ENEMY_GNOME_WIZARD,
  /// ★ Named from the game: "Ghoul" (MsgEnemyName)
  ENEMY_GHOUL,
  /// ★ Named from the game: "Fiendish Ghoul" (MsgEnemyName)
  ENEMY_FIENDISH_GHOUL,
  /// ★ Named from the game: "Cannibal Ghoul" (MsgEnemyName)
  ENEMY_CANNIBAL_GHOUL,
  /// ★ Named from the game: "Mauler" (MsgEnemyName)
  ENEMY_MAULER,
  /// ★ Named from the game: "Ravager" (MsgEnemyName)
  ENEMY_RAVAGER,
  /// ★ Named from the game: "Grisly" (MsgEnemyName)
  ENEMY_GRISLY,
  /// ★ Named from the game: "Cuttle" (MsgEnemyName)
  ENEMY_CUTTLE,
  /// ★ Named from the game: "Calamar" (MsgEnemyName)
  ENEMY_CALAMAR,
  /// ★ Named from the game: "Man o' War" (MsgEnemyName)
  ENEMY_MAN_O_WAR,
  /// ★ Named from the game: "Harpy" (MsgEnemyName)
  ENEMY_HARPY,
  /// ★ Named from the game: "Virago" (MsgEnemyName)
  ENEMY_VIRAGO,
  /// ★ Named from the game: "Harridan" (MsgEnemyName)
  ENEMY_HARRIDAN,
  /// ★ Named from the game: "Siren" (MsgEnemyName)
  ENEMY_SIREN,
  /// ★ Named from the game: "Succubus" (MsgEnemyName)
  ENEMY_SUCCUBUS,
  /// ★ Named from the game: "Nightmare" (MsgEnemyName)
  ENEMY_NIGHTMARE,
  /// ★ Named from the game: "Mole" (MsgEnemyName)
  ENEMY_MOLE,
  /// ★ Named from the game: "Mad Mole" (MsgEnemyName)
  ENEMY_MAD_MOLE,
  /// ★ Named from the game: "Mole Mage" (MsgEnemyName)
  ENEMY_MOLE_MAGE,
  /// ★ Named from the game: "Dirge" (MsgEnemyName)
  ENEMY_DIRGE,
  /// ★ Named from the game: "Foul Dirge" (MsgEnemyName)
  ENEMY_FOUL_DIRGE,
  /// ★ Named from the game: "Vile Dirge" (MsgEnemyName)
  ENEMY_VILE_DIRGE,
  /// ★ Named from the game: "Ape" (MsgEnemyName)
  ENEMY_APE,
  /// ★ Named from the game: "Killer Ape" (MsgEnemyName)
  ENEMY_KILLER_APE,
  /// ★ Named from the game: "Dirty Ape" (MsgEnemyName)
  ENEMY_DIRTY_APE,
  /// ★ Named from the game: "Grub" (MsgEnemyName)
  ENEMY_GRUB,
  /// ★ Named from the game: "Worm" (MsgEnemyName)
  ENEMY_WORM,
  /// ★ Named from the game: "Acid Maggot" (MsgEnemyName)
  ENEMY_ACID_MAGGOT,
  /// ★ Named from the game: "Orc" (MsgEnemyName)
  ENEMY_ORC,
  /// ★ Named from the game: "Orc Captain" (MsgEnemyName)
  ENEMY_ORC_CAPTAIN,
  /// ★ Named from the game: "Orc Lord" (MsgEnemyName)
  ENEMY_ORC_LORD,
  /// ★ Named from the game: "Salamander" (MsgEnemyName)
  ENEMY_SALAMANDER,
  /// ★ Named from the game: "Earth Lizard" (MsgEnemyName)
  ENEMY_EARTH_LIZARD,
  /// ★ Named from the game: "Thunder Lizard" (MsgEnemyName)
  ENEMY_THUNDER_LIZARD,
  /// ★ Named from the game: "Manticore" (MsgEnemyName)
  ENEMY_MANTICORE,
  /// ★ Named from the game: "Magicore" (MsgEnemyName)
  ENEMY_MAGICORE,
  /// ★ Named from the game: "Manticore King" (MsgEnemyName)
  ENEMY_MANTICORE_KING,
  /// ★ Named from the game: "Kobold" (MsgEnemyName)
  ENEMY_KOBOLD,
  /// ★ Named from the game: "Goblin" (MsgEnemyName)
  ENEMY_GOBLIN,
  /// ★ Named from the game: "Hobgoblin" (MsgEnemyName)
  ENEMY_HOBGOBLIN,
  /// ★ Named from the game: "Gargoyle" (MsgEnemyName)
  ENEMY_GARGOYLE,
  /// ★ Named from the game: "Clay Gargoyle" (MsgEnemyName)
  ENEMY_CLAY_GARGOYLE,
  /// ★ Named from the game: "Ice Gargoyle" (MsgEnemyName)
  ENEMY_ICE_GARGOYLE,
  /// ★ Named from the game: "Gryphon" (MsgEnemyName)
  ENEMY_GRYPHON,
  /// ★ Named from the game: "Wild Gryphon" (MsgEnemyName)
  ENEMY_WILD_GRYPHON,
  /// ★ Named from the game: "Wise Gryphon" (MsgEnemyName)
  ENEMY_WISE_GRYPHON,
  /// ★ Named from the game: "Golem" (MsgEnemyName)
  ENEMY_GOLEM,
  /// ★ Named from the game: "Earth Golem" (MsgEnemyName)
  ENEMY_EARTH_GOLEM,
  /// ★ Named from the game: "Grand Golem" (MsgEnemyName)
  ENEMY_GRAND_GOLEM,
  /// ★ Named from the game: "Dread Hound" (MsgEnemyName)
  ENEMY_DREAD_HOUND,
  /// ★ Named from the game: "Cerebus" (MsgEnemyName)
  ENEMY_CEREBUS,
  /// ★ Named from the game: "Fenrir" (MsgEnemyName)
  ENEMY_FENRIR,
  /// ★ Named from the game: "Stone Soldier" (MsgEnemyName)
  ENEMY_STONE_SOLDIER,
  /// ★ Named from the game: "Boulder Beast" (MsgEnemyName)
  ENEMY_BOULDER_BEAST,
  /// ★ Named from the game: "Raging Rock" (MsgEnemyName)
  ENEMY_RAGING_ROCK,
  /// ★ Named from the game: "Chimera" (MsgEnemyName)
  ENEMY_CHIMERA,
  /// ★ Named from the game: "Chimera Mage" (MsgEnemyName)
  ENEMY_CHIMERA_MAGE,
  ENEMY_104,
  /// Level 2: fought only in Sol Sanctum.
  /// ★ Named from the game: "Amaze" (MsgEnemyName)
  ENEMY_AMAZE_SOL_SANCTUM,
  /// Level 3: fought only on the world map around Vale and Vault.
  /// ★ Named from the game: "Amaze" (MsgEnemyName)
  ENEMY_AMAZE_WORLD_MAP,
  /// ★ Named from the game: "Creeper" (MsgEnemyName)
  ENEMY_CREEPER,
  /// ★ Named from the game: "Spirit" (MsgEnemyName)
  ENEMY_SPIRIT,
  /// The battle that ends Flint's lesson on the world map
  /// (WorldMap_ShowDjinnTutorial, slot 6 of
  /// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT): ENEMY_ZOMBIE's stats but agility 1,
  /// and it leaves no coins, item or experience.
  /// ★ Named from the game: "Zombie" (MsgEnemyName)
  ENEMY_ZOMBIE_DJINN_LESSON,
  ENEMY_110,
  ENEMY_111,
  ENEMY_112,
  /// ★ Named from the game: "Kraken" (MsgEnemyName)
  ENEMY_KRAKEN,
  /// ★ Named from the game: "Mystery Man" (MsgEnemyName)
  ENEMY_MYSTERY_MAN,
  /// Level 18.
  /// ★ Named from what the player sees: Saturos, fought alone at Mercury
  /// Lighthouse (its render)
  ENEMY_SATUROS_MERCURY_LIGHTHOUSE,
  /// Level 38.
  /// ★ Named from what the player sees: Saturos, fought beside Menardi at
  /// Venus Lighthouse's aerie (its render)
  ENEMY_SATUROS_VENUS_LIGHTHOUSE,
  /// ★ Named from the game: "Mystery Woman" (MsgEnemyName)
  ENEMY_MYSTERY_WOMAN,
  /// ★ Named from the game: "Menardi" (MsgEnemyName)
  ENEMY_MENARDI,
  /// ★ Named from the game: "Lizard Man" (MsgEnemyName)
  ENEMY_LIZARD_MAN,
  /// ★ Named from the game: "Lizard Fighter" (MsgEnemyName)
  ENEMY_LIZARD_FIGHTER,
  /// ★ Named from the game: "Lizard King" (MsgEnemyName)
  ENEMY_LIZARD_KING,
  /// ★ Named from the game: "Living Statue" (MsgEnemyName)
  ENEMY_LIVING_STATUE,
  /// ★ Named from the game: "Hydros Statue" (MsgEnemyName)
  ENEMY_HYDROS_STATUE,
  /// The Colosso finalists, two rows each: whoever reaches a match's arena
  /// first takes its better prize, and the battle (ENCOUNTER_ZONE_COLOSSO,
  /// one formation a row) is with the opponent carrying what he took.
  /// Azart having reached the arena first: defense 20, against 2 without the
  /// shield.
  /// ★ Named from the game: "Iron Shield" (MsgItemName)
  ENEMY_AZART_IRON_SHIELD,
  /// Azart having reached the arena second.
  /// ★ Named from the game: "Padded Gloves" (MsgItemName)
  ENEMY_AZART_PADDED_GLOVES,
  /// Satrage having reached the arena first: defense 28, against 6 without
  /// the mail.
  /// ★ Named from the game: "Chain Mail" (MsgItemName)
  ENEMY_SATRAGE_CHAIN_MAIL,
  /// Satrage having reached the arena second.
  /// ★ Named from the game: "Cotton Shirt" (MsgItemName)
  ENEMY_SATRAGE_COTTON_SHIRT,
  /// Navampa having reached the arena first: attack 274, against 232 without
  /// the Claymore.
  /// ★ Named from the game: "Claymore" (MsgItemName)
  ENEMY_NAVAMPA_CLAYMORE,
  /// Navampa having reached the arena second.
  /// ★ Named from the game: "Hunter's Sword" (MsgItemName)
  ENEMY_NAVAMPA_HUNTERS_SWORD,
  /// ★ Named from the game: "Ant Lion" (MsgEnemyName)
  ENEMY_ANT_LION,
  /// ★ Named from the game: "Roach" (MsgEnemyName)
  ENEMY_ROACH,
  /// ★ Named from the game: "Doodle Bug" (MsgEnemyName)
  ENEMY_DOODLE_BUG,
  /// ★ Named from the game: "Toadonpa" (MsgEnemyName)
  ENEMY_TOADONPA,
  /// ★ Named from the game: "Poison Toad" (MsgEnemyName)
  ENEMY_POISON_TOAD,
  ENEMY_135,
  /// ★ Named from the game: "Tornado Lizard" (MsgEnemyName)
  ENEMY_TORNADO_LIZARD,
  /// ★ Named from the game: "Storm Lizard" (MsgEnemyName)
  ENEMY_STORM_LIZARD,
  /// ★ Named from the game: "Tempest Lizard" (MsgEnemyName)
  ENEMY_TEMPEST_LIZARD,
  /// ★ Named from the game: "Tret" (MsgEnemyName)
  ENEMY_TRET,
  /// ★ Named from the game: "Fusion Dragon" (MsgEnemyName)
  ENEMY_FUSION_DRAGON,
  /// ★ Named from the game: "Deadbeard" (MsgEnemyName)
  ENEMY_DEADBEARD,
  /// ★ Named from what the player sees: the Mimic that leaps out of the second
  /// chest of Mercury Lighthouse's entrance (FIND_MIMIC, slot 0) (its render)
  ENEMY_MIMIC_MERCURY_LIGHTHOUSE,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// in Fuchin Falls Cave's entrance (FIND_MIMIC, slot 1) (its render)
  ENEMY_MIMIC_FUCHIN_FALLS_CAVE,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// by Altin Peak's pool (FIND_MIMIC, slot 2) (its render)
  ENEMY_MIMIC_ALTIN_PEAK,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// in the Vault cave passage's pool room (FIND_MIMIC, slot 3) (its render)
  ENEMY_MIMIC_VAULT_CAVE,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// on Crossbone Isle's first floor (FIND_MIMIC, slot 4) (its render)
  ENEMY_MIMIC_CROSSBONE_ISLE,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// in Altmiller Cave (FIND_MIMIC, slot 5) (its render)
  ENEMY_MIMIC_ALTMILLER_CAVE,
  /// ★ Named from what the player sees: the Mimic that leaps out of a chest in
  /// Suhalla Desert (FIND_MIMIC, slot 6) (its render)
  ENEMY_MIMIC_SUHALLA_DESERT,
  /// ★ Named from what the player sees: the Mimic that leaps out of the second
  /// chest on Crossbone Isle's pirate ship (FIND_MIMIC, slot 7) (its render)
  ENEMY_MIMIC_PIRATE_SHIP,
  /// The Djinn the party fights, each element's in order of level;
  /// BattleUnit_Set puts them on the party's side. A Djinni's battle is a slot
  /// of its element's zone (gEncounter_AreaEntryTable), which Djinn who join
  /// without a fight share; each row is named for the one Djinni whose scene
  /// starts its battle (Field_RequestDjinnBattle).
  /// Level 12, in Mogall Forest.
  /// ★ Named from the game: "Quartz" (MsgDjinnName)
  ENEMY_VENUS_DJINNI_QUARTZ,
  /// Level 15, on the world map.
  /// ★ Named from the game: "Vine" (MsgDjinnName)
  ENEMY_VENUS_DJINNI_VINE,
  /// Unused: level 23, the battle of Bane's slot, but Bane joins on Crossbone
  /// Isle without a fight (Djinn_JoinParty), so no battle reaches this row.
  /// ★ Named from the game: "Bane" (MsgDjinnName)
  ENEMY_VENUS_DJINNI_UNUSED_BANE,
  /// Level 10, in Mercury Lighthouse.
  /// ★ Named from the game: "Sleet" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_SLEET,
  /// Level 13, on Altin Peak.
  /// ★ Named from the game: "Spritz" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_SPRITZ,
  /// Level 18, on the world map.
  /// ★ Named from the game: "Hail" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_HAIL,
  /// Level 24, at Suhalla Gate.
  /// ★ Named from the game: "Dew" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_DEW,
  /// Level 5, in Goma Cave.
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  ENEMY_MARS_DJINNI_FORGE,
  /// Level 13, on the world map.
  /// ★ Named from the game: "Corona" (MsgDjinnName)
  ENEMY_MARS_DJINNI_CORONA,
  /// Level 22, in the Suhalla Desert.
  /// ★ Named from the game: "Flash" (MsgDjinnName)
  ENEMY_MARS_DJINNI_FLASH,
  /// Level 8, in Tret Tree.
  /// ★ Named from the game: "Breeze" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_BREEZE,
  /// Level 11, in Fuchin Falls Cave.
  /// ★ Named from the game: "Zephyr" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_ZEPHYR,
  /// Level 18, in Altmiller Cave.
  /// ★ Named from the game: "Squall" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_SQUALL,
  /// Level 25, in Babi Lighthouse.
  /// ★ Named from the game: "Luff" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_LUFF,
  ENEMY_164
};

#endif
