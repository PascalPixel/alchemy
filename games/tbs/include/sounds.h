#ifndef ALCHEMY_SOUNDS_H
#define ALCHEMY_SOUNDS_H

/// @file
/// Sound ids, one for each row of gSongTable (sound/song_table.s) in order:
/// MUS_ ids are music and jingles, SE_ ids sound effects, as their sequences
/// in sound/songs are named. A row nothing plays by name yet keeps its number
/// as its placeholder; the named ones are the code's own, the field music
/// gScene_MusicRules and gEncounter_BattleMusic choose by number, and the
/// voices and defeat sounds reached by adding to a first id. Three rows are
/// controls no sound plays from. They are named as pret names its sounds:
/// music by the place or moment the player knows it from, or by what it sounds
/// like where it marks many moments (MUS_OMINOUS), a sound effect by what it
/// sounds like or the moment it marks, from its comment and its uses.
/// Sound_Play tests the controls SOUND_FADE_OUT_BGM and
/// SOUND_FADE_OUT_ACTION_SE first, then the ranges: from SOUND_SE_FIRST on the
/// player its gSongTable row names, from SOUND_JINGLE_FIRST jingles, below
/// that music, where SOUND_KEEP_BGM is also a control (sound_driver_symbols.h).

enum SoundId {
  /// Field music of Vale, of Vault, and of Altin once
  /// FLAG_HYDROS_STATUES_DEFEATED is set.
  MUS_VALE,
  /// Field music of Lunpa, the Lunpa Fortress Gates and the Vault jail; of
  /// Bilibin, the Bilibin Barricade, Kolima, Kolima Bridge, Kolima Forest and
  /// the Tret Tree once FLAG_KOLIMA_CURSE_LIFTED is set; of Tolbi and the
  /// Colosseum entrance after the Colosso (FLAG_COLOSSO_OVER); and of Lalivero
  /// once FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED is set. It also plays as Tret is
  /// restored.
  MUS_TOWN,
  /// Field music of Bilibin, the Bilibin Barricade, Kolima, Kolima Bridge, the
  /// Kolima tunnel, Imil and Lalivero, and of Vale and the Sanctum once Mt.
  /// Aleph has erupted (FLAG_SOL_SANCTUM_ERUPTED). It also plays as the Wise
  /// One gives the party its mission.
  MUS_TOWN_IN_TROUBLE,
  /// Field music of McCoy's Palace, Hammet Palace, the Colosseum's halls and
  /// Vale's inn from one entrance; the debug link test plays it too
  /// (SerialTest_Run).
  MUS_PALACE,
  /// Field music of the Tret Tree and of Altin.
  /// ★ Named from its area name: Tret Tree
  MUS_TRET_TREE,
  /// Field music of Fuchin Temple, Xian and Lama Temple.
  /// ★ Named from its area name: Fuchin Temple
  MUS_FUCHIN_TEMPLE,
  /// Field music of Kalay, the Kalay docks and the Tolbi docks.
  MUS_KALAY,
  /// Field music at the start of each Colosso match, and as Tret is restored.
  MUS_COLOSSO_INTRO,
  /// Field music of the Colosso matches, and as Donpa judges Dodonpa in the
  /// Lunpa Fortress.
  MUS_COLOSSO,
  /// The world map's music during the Djinn lesson (FLAG_DJINN_LESSON) and as
  /// Flint is met; it also plays as the Vault thieves are chased, as the tour
  /// group leaves Kalay, and in Lucky Dice and Lucky Wheels.
  MUS_DJINN_LESSON,
  /// Field music of Tolbi before the Colosso; afterwards Tolbi plays MUS_TOWN.
  /// ★ Named from its area name: Tolbi
  MUS_TOLBI,
  MUS_011,
  MUS_012,
  MUS_013,
  MUS_014,
  MUS_015,
  MUS_016,
  /// Not a sound: fades the music out over 112 VBlanks, once.
  SOUND_FADE_OUT_BGM,
  /// Not a sound: keeps the music that is playing.
  SOUND_KEEP_BGM,
  /// a sound with no tracks: silence
  MUS_SILENCE,
  /// Field music of Sol Sanctum until the eruption (FLAG_SOL_SANCTUM_ERUPTED).
  /// ★ Named from its area name: Sol Sanctum
  MUS_SOL_SANCTUM,
  /// Field music of Kolima Forest, Mogall Forest, and of the Tret Tree once
  /// FLAG_TRET_DEFEATED is set; it also plays as Kolima is cursed, as Kraden
  /// finds the heart of Sol Sanctum, and after the battle with Tret.
  MUS_FOREST,
  /// Field music of Sol Sanctum's Elemental Star room, Mercury Lighthouse's
  /// aerie, the Lemurian ship, Venus Lighthouse's aerie once its second battle
  /// has begun (FLAG_AERIE_SECOND_BATTLE_BEGUN in venus_lighthouse_aerie.c),
  /// and the world map reached from one of its entrances.
  MUS_ELEMENTAL_STARS,
  /// Field music of Altin Peak and the Lunpa Fortress, and of Vale once the
  /// stormy night's monster is fought (FLAG_VALE_STORM_MONSTER_FOUGHT); it also
  /// plays as the Wise One rescues the party in Sol Sanctum and after Saturos
  /// and Menardi's defeat.
  MUS_DANGER,
  /// Field music of the caves: Goma Cave, Bilibin Cave, Imil Falls Cave, Vale
  /// Cave, Vault Cave, West Lunpa Cave, the Kalay Tunnel, Gondowan Cave and the
  /// Suhalla Gate.
  MUS_CAVE,
  /// Field music of the deserts: Lamakan Desert and Suhalla Desert.
  MUS_DESERT,
  /// Field music of Suhalla, and of Imil once Saturos is beaten at Mercury
  /// Lighthouse (FLAG_SATUROS_BEATEN_AT_MERCURY).
  /// ★ Named from its area name: Suhalla
  MUS_SUHALLA,
  /// Field music of Mercury Lighthouse and the Venus Lighthouse entry.
  MUS_MERCURY_LIGHTHOUSE,
  /// Field music of Fuchin Falls Cave, Crossbone Isle's cave, Babi Lighthouse
  /// and the Tunnel Ruins' rooms of lady statues; it also plays on the
  /// Tolbi-bound ship as the party discusses launching without an anchor.
  MUS_DUNGEON,
  /// Field music of Altmiller Cave, the Tunnel Ruins, and the Tolbi-bound ship
  /// once its monsters are sighted (FLAG_TOLBI_SHIP_MONSTERS_SIGHTED); it also
  /// plays as Alex appears at Mercury Lighthouse, in the fight with Tret, on
  /// the ship's mast, and as Saturos and Menardi are confronted at Venus
  /// Lighthouse.
  MUS_CONFRONTATION,
  /// Plays as Babi is restored in Altmiller Cave and invites the party to
  /// Tolbi, and as he meets it after the Colosso; also as Iodem, Babi's
  /// general, questions the wounded soldiers in Suhalla, and as the field music
  /// of Venus Lighthouse's aerie once its first battle has begun
  /// (FLAG_AERIE_FIRST_BATTLE_BEGUN in venus_lighthouse_aerie.c).
  MUS_BABI,
  MUS_031,
  MUS_032,
  MUS_033,
  MUS_034,
  MUS_035,
  MUS_036,
  MUS_037,
  MUS_038,
  MUS_039,
  /// Unused: nothing in ☀️ plays it; TRACKS_ALL names it among the songs of
  /// nine or more tracks.
  MUS_UNUSED,
  /// Field music of the Tolbi-bound ship, and of the world map reached from
  /// seven of its entrances.
  MUS_TOLBI_BOUND_SHIP,
  /// Field music of the world map, and of Goma Cave's entrance, the Bilibin
  /// Cave entrance, the Alpine Crossing, Crossbone Isle, the Gondowan Passage
  /// and the Kalay Tunnel entrance; it also plays after Flint is met.
  MUS_WORLD_MAP,
  /// Field music of Venus Lighthouse, but for its entry and its aerie.
  /// ★ Named from its area name: Venus Lighthouse
  MUS_VENUS_LIGHTHOUSE,
  MUS_044,
  MUS_045,
  MUS_046,
  MUS_047,
  MUS_048,
  MUS_049,
  /// The music of every battle gEncounter_BattleMusic lists no other music for.
  MUS_BATTLE,
  /// Battle music gEncounter_BattleMusic gives the Mystery Man and Mystery
  /// Woman who catch Isaac in Vale's plaza (ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT,
  /// slot 4) and Saturos at Mercury Lighthouse's aerie
  /// (ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2); Battle_Run plays it, then
  /// MUS_COLOSSO_BATTLE, for a battle with no music of its own.
  MUS_SATUROS_BATTLE,
  /// Battle music gEncounter_BattleMusic gives the guardians and bosses: Tret,
  /// the Killer Ape, the Hydros Statue, the Manticore, the Kraken, Toadonpa,
  /// the Storm and Tempest Lizards, and the monsters guarding Crossbone Isle's
  /// floors; it also plays as Dodonpa pulls the lever that springs the trap in
  /// the Lunpa Fortress.
  /// ★ Named from what the player sees: the battles with Tret, the Kraken and
  /// the Manticore (its render)
  MUS_BOSS_BATTLE,
  /// Battle music gEncounter_BattleMusic gives Saturos and Menardi's first
  /// battle at Venus Lighthouse's aerie (ENCOUNTER_ZONE_SCRIPTED, slot 1) and
  /// Deadbeard (ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, slot 5).
  MUS_SATUROS_AND_MENARDI_BATTLE,
  /// Battle music of the Fusion Dragon, the aerie's second battle
  /// (ENCOUNTER_ZONE_SCRIPTED, slot 0); it also plays as Saturos and Menardi
  /// fuse at Venus Lighthouse's aerie.
  /// ★ Named from the game: "Fusion Dragon" (MsgEnemyName)
  MUS_FUSION_DRAGON_BATTLE,
  /// Plays as the boulder chases the party down Altin Peak, and after a link
  /// battle's own music (Battle_Run).
  MUS_BOULDER_CHASE,
  MUS_056,
  MUS_057,
  /// Plays when a battle is won (Battle_Run), except during the Djinn lesson
  /// (FLAG_DJINN_LESSON).
  MUS_VICTORY,
  /// Plays when the whole party falls: defeated in battle, before "goes down in
  /// defeat" or "party has been defeated", or felled on the field by poison or
  /// hazardous ground.
  MUS_DEFEAT,
  /// Plays as Ivan is met in Vault, and as the party finds Ivan's master Hammet
  /// in the Lunpa Fortress.
  MUS_IVAN,
  /// Ominous music that plays outside battle as villains appear: Saturos and
  /// Menardi in Vale's plaza, at Kraden's cottage, in the Elemental Star room
  /// and at the aeries of Mercury Lighthouse and Venus Lighthouse; the thieves
  /// in the loft of Vault's inn; and Dodonpa in the Lunpa Fortress, as he comes
  /// to free Toadonpa on the party.
  MUS_OMINOUS,
  /// Plays as Sheba hangs from the edge of Venus Lighthouse's aerie, from
  /// Felix's cry until it fades as she slips
  /// (VenusLighthouseAerie_TryToPreventFall).
  MUS_SHEBA_HANGS_ON,
  /// Field music of Sol Sanctum once Mt. Aleph has erupted
  /// (FLAG_SOL_SANCTUM_ERUPTED) and of the eruption event; it also plays as the
  /// Tolbi-bound ship rocks and as Saturos and Menardi combine.
  MUS_ERUPTION,
  /// The Title's music as the intro holds the title, without its prompt, after
  /// the prologue and before Vale three years later (Title_Run).
  /// ★ Named from what the player sees: the title shown between the prologue
  /// and Vale three years later (its render)
  MUS_TITLE_AFTER_PROLOGUE,
  MUS_065,
  MUS_066,
  /// The ending's music: the Lemurian ship sailing away (LemurianShip_SetSail)
  /// and the staff credits that follow (Title_Run).
  /// ★ Named from what the player sees: the Lemurian ship sailing off and the
  /// staff credits (its render)
  MUS_ENDING,
  MUS_068,
  MUS_069,
  /// The title intro's music at power-on, when a save file exists, with its
  /// "Press Start" prompt (Title_Run).
  /// ★ Named from what the player sees: the Title and its "Press Start" prompt
  /// (its render)
  MUS_TITLE,
  /// A row of the song table that holds the empty song (dummy_song_header), so
  /// nothing plays; only the debug battle message viewer starts it, to silence
  /// the music.
  MUS_EMPTY,
  /// Plays once Felix has leapt after Sheba from Venus Lighthouse's aerie, as
  /// the party watches and resolves to escape
  /// (VenusLighthouseAerie_PlayShebasFall).
  MUS_FELIX_LEAPS_AFTER_SHEBA,
  MUS_073,
  /// The File Screen's music, and the field music of the Sanctum.
  MUS_FILE_SCREEN,
  /// The music of the Golden Sun logo shown after Jenna and Kraden's closing
  /// event on the world map, until a key is pressed (Title_Run's entrance 10).
  /// ★ Named from what the player sees: the Golden Sun logo after the ending's
  /// last event (its render)
  MUS_ENDING_LOGO,
  /// Battle music gEncounter_BattleMusic gives the battles that end the Colosso
  /// finals' matches (ENCOUNTER_ZONE_COLOSSO); Battle_Run plays it after
  /// MUS_SATUROS_BATTLE for a battle with no music of its own.
  /// ★ Named from what the player sees: the battle that ends a Colosso match
  /// (its render)
  MUS_COLOSSO_BATTLE,
  MUS_077,
  MUS_078,
  MUS_079,
  /// The jingle as a puzzle is solved: Altmiller Cave's rock turned, the
  /// Lemurian ship raised, the Vault Cave switches, and the West Lunpa Cave
  /// gate frozen open.
  MUS_PUZZLE_SOLVED,
  /// The jingle as a Djinni or a new party member joins the party, Flint
  /// included.
  MUS_JOINED,
  /// The jingle of Revive, used from the menu or bought at a Sanctum.
  MUS_REVIVE,
  /// The jingle as an item is found or given, Psynergy learned, a Colosso prize
  /// taken, the Hermes' Water bottle filled, or an Elemental Star taken.
  MUS_TREASURE_FOUND,
  /// The jingle of Cure Poison, used from the menu or bought at a Sanctum, and
  /// of Ply's field effect on its target.
  /// ★ Named from what the player sees: Cure Poison and Ply curing their target
  /// (its render)
  MUS_CURE,
  /// The jingle as a save is written, or a link battle's results are saved.
  MUS_SAVED,
  /// The jingle of the inn as the party sleeps (Inn_PlaySleep); also a lost
  /// Colosso match, and the Sanctum as the party is asked to take on the quest.
  MUS_INN,
  /// The jingle of a Sanctum's Remove Curse.
  MUS_REMOVE_CURSE,
  /// The jingle of a Sanctum's Repel Evil.
  MUS_REPEL_EVIL,
  /// The jingle as a party member levels up after a battle, and as a Colosso
  /// round begins.
  MUS_LEVEL_UP,
  MUS_090,
  /// The smallest prize jingle: two pairs in Lucky Dice, a Tolbi Spring prize
  /// under 5,000 coins, and one level from the debug level editor; also an item
  /// or Psynergy used from the menu whose effect has no jingle of its own, such
  /// as the stat items (Ability_PlayUseAnimation).
  /// ★ Named from what the player sees: the least of the three prizes in Lucky
  /// Dice and Tolbi Spring (its render)
  MUS_JINGLE_SMALL,
  /// The middle prize jingle: three pairs in Lucky Dice, a Tolbi Spring prize
  /// of 5,000 coins or more, and a cleared Colosseum trial.
  /// ★ Named from what the player sees: the middle of the three prizes in Lucky
  /// Dice and Tolbi Spring (its render)
  MUS_JINGLE_MEDIUM,
  /// The largest prize jingle: four of a kind in Lucky Dice, a Lucky Wheels
  /// win, a Tolbi Spring prize over 19,999 coins, and five levels from the
  /// debug level editor.
  /// ★ Named from what the player sees: the greatest of the three prizes in
  /// Lucky Dice and Tolbi Spring, and a Lucky Wheels win (its render)
  MUS_JINGLE_LARGE,
  MUS_094,
  MUS_095,
  MUS_096,
  MUS_097,
  MUS_098,
  MUS_099,
  /// The Vault bell rung, and three times as a shop repairs an item.
  SE_BELL,
  /// An item bought in a shop, artifacts included.
  SE_SHOP_PURCHASE,
  /// An item sold to a shop (Shop_SellItem).
  /// ★ Named from what the player sees: an item sold at a shop's counter (its
  /// render)
  SE_SHOP_SELL,
  /// A cursed item equipped from the menu or in a shop; it also plays as Hydros
  /// is beaten on Altin Peak, and in the lobbed-shell battle effects.
  SE_CURSE,
  SE_104,
  SE_105,
  /// Each water drop landing in Vault Cave; also the curse's sparkles meeting
  /// Mia's Psynergy at Kolima and Kolima Forest's barrier, and the villagers
  /// trying to save Felix from the river on the stormy night. It sounds like a
  /// short high plink, a tone near 1.8 kHz gone in a tenth of a second.
  SE_DRIP,
  /// Plays with a light shake of the screen: as the Wise One holds Sol
  /// Sanctum's breaking floor together, during the boulder chase on Altin Peak,
  /// as a Colosseum trial is cleared, as Psynergy is shown at the gate to Mt.
  /// Aleph, and as the ground shakes under Hammet's caravan on the world map.
  /// It sounds like a low rumble held until it is stopped.
  SE_TREMOR,
  /// The low triple tone: Halt freezing an actor, and the link session
  /// countdown.
  SE_TRIPLE_TONE_LOW,
  /// The high triple tone: Halt ending.
  SE_TRIPLE_TONE_HIGH,
  /// The lit bulb a field view shows as it finds a target (EMOTE_LIGHT_BULB);
  /// also the title splash screen, a code entered on the File Screen, Altmiller
  /// Cave's lights appearing, and a battle command chosen.
  SE_CHIME,
  /// The cursor moving in a menu or list.
  SE_MENU_CURSOR_MOVE,
  /// A menu choice confirmed.
  SE_MENU_CONFIRM,
  /// A menu closed or a choice cancelled.
  SE_MENU_CANCEL,
  /// A choice that can't be made: not enough PP, no target, a pushed or lifted
  /// thing that won't move.
  SE_MENU_ERROR,
  /// Ice forming: Frost on the field, and the Ice and Frost battle effects.
  /// ★ Named from what the player sees: the ice of Frost and of Ice, Ice Horn
  /// and Ice Missile (its render)
  SE_ICE,
  /// Tolbi Spring: the Toss prompt appearing, before each toss.
  /// ★ Named from the game: "Toss" (MsgTolbiToss)
  SE_TOLBI_SPRING_TOSS_PROMPT,
  /// The Remove command in the item menu (ItemMenu_ChooseCommand).
  /// ★ Named from what the player sees: the item menu's Remove command (its
  /// render)
  SE_ITEM_REMOVE,
  /// Footsteps at the Mercury Lighthouse entrance, and the leader spraying sand
  /// in Lamakan Desert.
  SE_FOOTSTEP,
  SE_119,
  SE_120,
  /// The guards throwing the leader out of Lunpa.
  SE_SCUFFLE,
  SE_122,
  /// Leaving a scene through an ordinary exit.
  SE_MAP_EXIT,
  /// Played as a chest's lid lifts (TreasureSpot_OpenChest); also a Psynergy
  /// target reacting, the Fuchin Temple students' lessons, the Vault mayor's
  /// urn, and the training tree in Xian.
  SE_CHEST_OPEN,
  /// Played as a chest's lid comes down again when its item can't be carried
  /// (TreasureSpot_CloseChest); also a switch flipped on Altin Peak, the
  /// boulder lifted on the Alpine Crossing, Hammet's return to Kalay, and
  /// McCoy's reward.
  SE_CHEST_CLOSE,
  /// HP recovered: healing in battle and from the menu, a Djinni's heal, a hot
  /// spring or well searched, Growth on the field, the Psynergy Stone, and the
  /// Sanctum's services.
  SE_RECOVERY,
  /// A heavy landing: the leader thrown out of Lunpa, or hopping down a step in
  /// the Tunnel Ruins.
  SE_LANDING_THUD,
  /// Climbing stairs: an EXIT_STYLE_STAIRS_UP exit, and the stairs up in Vale's
  /// inn and Isaac's house.
  SE_STAIRS_UP,
  /// Going down stairs: an EXIT_STYLE_STAIRS_DOWN exit, and the stairs down in
  /// Vale's inn and Isaac's house.
  SE_STAIRS_DOWN,
  /// Carry, Cloak, Force, Lift, Mind Read and Ply beginning on the field, and a
  /// Psynergy shortcut set in the menu.
  SE_PSYNERGY_START,
  /// The sparkles of Psynergy taking hold on the field: Carry, Growth, Move,
  /// Ply, Retreat and Reveal; also the Thor summon and Mia's Ply at Mercury
  /// Lighthouse.
  SE_PSYNERGY_SPARKLE,
  /// Catch and Douse on the field, the Fuchin Temple grotto, Xian's student,
  /// and the Boreas, Ice, Douse, Frost, Thorn, Spire and Froth battle
  /// animations.
  SE_WATER,
  /// A blow landing: an enemy's ordinary hit, field damage of 10 or less, and
  /// poison on the field; the lighter twin of SE_HEAVY_IMPACT, heard in most
  /// attacking Psynergy.
  SE_IMPACT,
  /// A heavy blow landing: damage over 10 on the field, the heavy-hitting
  /// classes' blows, and the summons and strongest Psynergy.
  SE_HEAVY_IMPACT,
  /// The leader's step blocked, a lifted thing steered, and each fifth of a
  /// turn of Altmiller Cave's spinning lights.
  SE_CLICK,
  /// Plays as a Djinni hops into battle, with Whirlwind on the field, as
  /// pillars move on Crossbone Isle, during the spinning leap in Gondowan Cave,
  /// and in the Fire, Flare, Frost, Mars, Cybele, Kirin and other battle
  /// effects.
  SE_WHOOSH,
  /// Plays in the Kirin summon only, 46 frames after its shake begins.
  /// ★ Named from what the player sees: the Kirin summon (its render)
  SE_KIRIN,
  /// An item breaking, and a lifted, pushed or caught thing shattering; also
  /// the Bolt and Ray battle effects, rumbling at Sol Sanctum's entrance, and
  /// Venus Lighthouse's aerie.
  SE_ITEM_BREAK,
  /// A Djinni set in the Djinn menu, and a Djinni recovering to Set as the
  /// party walks (Field_UpdateWalking).
  /// ★ Named from what the player sees: a Djinni returning to Set (its render)
  SE_DJINNI_SET,
  /// A Djinni spinning up as it joins or is caught, before it leaps or shrinks
  /// away (Djinn_RunJupiterJoinEffect, Djinn_RunMarsJoinEffect,
  /// FieldEffect_RunVenusDjinnCapture); also Avoid's arcs on the field, the
  /// Lemurian ship rising, and the Thor and Procne summons.
  SE_SPIN,
  /// A rumble, mostly as the screen shakes (MapGrid_SetShake): Sol Sanctum's
  /// rooms after the eruption, the stumps in Goma Cave, the Lunpa Fortress
  /// trap, Mercury Lighthouse's waterfall passage, the boulders of the stormy
  /// night, the wagons setting off from Kalay and Hammet's caravan, and the
  /// Quake, Gaia, Judgment and other battle effects.
  SE_RUMBLE,
  /// Light swirling into its target as stats rise (BattleEffect_RunStatBoost),
  /// and in the Bind, Drain, Haunt, Sleep, Judgment and Neptune battle effects;
  /// also a statue searched in Venus Lighthouse.
  SE_SWIRL,
  /// The picture breaking up as stats fall (BattleEffect_RunStatReduction), and
  /// the Break, Vine, Spider Web and Curse battle effects; also a Venus
  /// Djinni's sparks landing and Vale's roof mended. It sounds like a sharp
  /// noise crack trailing into a low buzz for half a second.
  SE_CRACK,
  /// The rocks of Altin Peak's boulder, the crates moved in the Lunpa Fortress,
  /// the Elemental Stars bagged, and the Eruption, Nereid and Prism battle
  /// animations.
  SE_CLATTER,
  /// Each rock and the boulder crashing down on the stormy night, and with
  /// SE_CLATTER as Altin Peak's boulder breaks; also the Lemurian ship's flash,
  /// the Lunpa Fortress trap, the Mars Star taken, Mt. Aleph's eruption, and
  /// the Boreas, Gaia, Judgment, Ragnarok, Thor and other summons.
  SE_CRASH,
  /// An enemy falling, for defeat-sound kind 0, which most enemies have, the
  /// Djinn included: each kind's sound is SE_ENEMY_CRY plus it
  /// (BattleSprite_GetDefeatSound). A mimic chest leaping up and the monsters
  /// on the Tolbi-bound ship use it too.
  SE_ENEMY_CRY,
  /// An enemy falling, for defeat-sound kind 1, the beasts' (rats, harpies,
  /// gargoyles, gryphons, golems, hounds and the like); also a battle starting
  /// on the field (Field_StartEncounterTransition), a water beast scared off in
  /// Altin, the apes in Mogall Forest, and monsters attacking the Tolbi-bound
  /// ship.
  SE_BEAST_CRY,
  /// An enemy falling, for defeat-sound kind 2, the Lizard King's; also the
  /// boulder and Hydros on Altin Peak, the Manticore in Lamakan Desert, the
  /// Killer Ape in Mogall Forest, and Tolbi Spring's prize.
  SE_BOSS_ROAR,
  /// An enemy falling, for defeat-sound kind 3, the Mimic's; also the Kraken
  /// rising as the party leaps into its battle on the Tolbi-bound ship.
  SE_MONSTER_ROAR,
  /// Unused: the sound of defeat-sound kind 4, which no row of gBattleSprites
  /// gives.
  SE_UNUSED_DEFEAT_CRY,
  /// Altmiller Cave's lights growing in, one by one, and a Venus Djinni caught.
  SE_LIGHT_APPEAR,
  /// An actor jumps: Actor_Jump plays the high jump above 5 pixels a frame.
  SE_JUMP,
  /// A high jump: Actor_Jump above 5 pixels a frame, Djinn leaping, and the
  /// leaps on Altin Peak, at the Kalay docks and in the Elemental Star room.
  SE_HIGH_JUMP,
  /// The sound of a cast, in battle and on the field.
  SE_CAST,
  /// Babi Lighthouse's truth door switch, Vault Cave's switch, Hammet found in
  /// the Lunpa Fortress, and the passengers at the Kalay docks.
  SE_SWITCH,
  /// The mechanism Vault Cave's switch starts, after SE_SWITCH; also the
  /// Cybele, Judgment and Ramses battle effects.
  SE_SWITCH_MECHANISM,
  /// A gate or door sliding: Babi Lighthouse's truth door, Crossbone Isle's red
  /// and blue doors, the Lunpa Fortress doors, West Lunpa Cave's gates, and the
  /// Cybele and Judgment battle effects.
  SE_GATE_MOVE,
  /// A door opening as the leader goes through.
  SE_DOOR_OPEN,
  /// The door behind the leader as an exit's arrival plays
  /// (Entrance_PlayArrival), and a door shutting in Lalivero; also the Fuchin
  /// Temple, the Kalay docks, Altin Peak's cart, and the lady statues moved in
  /// the Tunnel Ruins.
  SE_DOOR_CLOSE,
  SE_160,
  /// Plays as Felix is unmasked, as Tret rejects the Hermes' Water, as
  /// Levitation is shown at the gate to Mt. Aleph, as Vale's roof is mended, at
  /// Venus Lighthouse's switches, and as Saturos and Menardi combine. It sounds
  /// like a quick rattle: three short noise taps, low, faint and high, then
  /// middling, over a seventh of a second.
  SE_RATTLE,
  /// Venus Lighthouse's tile waves, and the Mercury, Judgment and Neptune
  /// battle animations.
  SE_WAVE,
  /// Whirlwind on the field, the Lemurian ship rising, and the rumble in Vault
  /// Cave. It sounds like a steady rushing noise held until it is stopped.
  SE_RUSH,
  /// The light particles a joining Jupiter Djinni scatters, and the Neptune,
  /// Nereid, Titan Blade and Froth battle effects.
  SE_SCATTER,
  SE_165,
  SE_166,
  /// A map transition effect.
  SE_MAP_TRANSITION,
  SE_168,
  // XXX: likely wind, because none of the three has the water SE_AMBIENT_WATER
  // plays beside; the sequence alone can't tell wind from water, so a recording
  // of one of the three scenes would settle it.
  /// The ambient sound a scene queues in Lama Temple, Xian and the Manticore's
  /// part of Lamakan Desert (Scene_QueueMusic). It sounds like a steady quiet
  /// hiss, a little darker and softer than SE_AMBIENT_WATER on the same noise
  /// voice.
  SE_AMBIENT_WIND,
  /// The ambient sound a scene queues (Scene_QueueMusic) with the rain's ground
  /// particles (Weather_StartRain) and Altin's water-spitting beasts, and in
  /// Fuchin Temple, Vale, Kalay's tunnel, Mercury and Venus Lighthouse and the
  /// Goma Cave entrance.
  SE_AMBIENT_WATER,
  /// Thunder: lightning on the field (Weather_UpdateLightning), the flash in
  /// Sol Sanctum's Luna room as the event opens on Kraden, the flash of a Lucky
  /// Wheels win, and the Jupiter battle effect.
  /// ★ Named from what the player sees: lightning flashing over the stormy
  /// night (its render)
  SE_THUNDER,
  /// The louder thunderclap lightning on the field plays in place of SE_THUNDER
  /// (struct FieldBlendWork's loud).
  /// ★ Named from what the player sees: lightning flashing over the stormy
  /// night (its render)
  SE_THUNDER_LOUD,
  /// The lowest of three rising notes, one for each bounce of a Djinni
  /// beginning to join or be caught, before SE_NOTE_MID and SE_NOTE_HIGH
  /// (Djinn_RunMarsJoinEffect); also the choice sound of the item and Psynergy
  /// menus.
  SE_NOTE_LOW,
  /// The middle of the three rising notes; also the Use command in the item and
  /// Psynergy menus, and Avoid on the field.
  SE_NOTE_MID,
  /// The highest of the three rising notes; also a Djinni returning to Set in
  /// battle, HP or PP recovered or elemental power raised at a turn's end, a
  /// Djinni put on standby in the Djinn menu, and an item given by the debug
  /// item browser.
  SE_NOTE_HIGH,
  /// The villager at Kolima's post jumping to it, as the drawbridge is lowered
  /// (Kolima_LowerBridge).
  SE_KOLIMA_BRIDGE_HOP,
  SE_177,
  SE_178,
  SE_179,
  /// The Tunnel Ruins' balance settling, and Venus Lighthouse's floating
  /// pillars lowering.
  SE_SETTLE,
  /// Sol Sanctum's door opening, and the Kraken shaking the Tolbi-bound ship.
  SE_BOOM,
  /// A gem set into its socket as a way opens: the Dragon's Eye in Fuchin Falls
  /// Cave's dragon and the Small Jewels at Sol Sanctum's entrance.
  /// ★ Named from what the player sees: the leader setting the small gem into
  /// the empty socket, and the dragon lit by its eye (its render)
  SE_GEM_SET,
  /// Altin Peak's statue waking and the boulder chase, Crossbone Isle's doors
  /// opening, and the jewels set at Sol Sanctum's entrance.
  SE_STONE_RUMBLE,
  /// Altin Peak's cart turning or moving forward.
  SE_CART_TURN,
  /// The leader pushing what stands ahead a cell on: a statue, pillar, crate or
  /// torch walked into in Bilibin, Bilibin Cave, Goma Cave, Kalay, Sol Sanctum,
  /// the Tunnel Ruins, Venus Lighthouse and Altin Peak, and the scenes' pushed
  /// actors (PushedActor_AdvancePair).
  /// ★ Named from what the player sees: the leader pushing a statue or pillar
  /// ahead (its render)
  SE_PUSH,
  /// Sol Sanctum lighting up as its statues are placed: a seal opening
  /// (SolSanctumStatue_OpenSeal), and the left and right beams shining on the
  /// centre statue.
  SE_SOL_SANCTUM_LIGHT_UP,
  /// Sol Sanctum's falling lights, and Venus Lighthouse's aerie flashing white
  /// as the beacon rises.
  SE_FALLING_LIGHT,
  /// A door sliding open: Lama Temple, the Sanctums of Altin and Bilibin,
  /// Vault, Babi Lighthouse and Goma Cave; also Venus Lighthouse's pillars set
  /// down, landings in dust, Kolima's drawbridge coming down and the pursuers
  /// dodged on Crossbone Isle. It sounds like a low buzzing thud that dies away
  /// over a second.
  SE_THUMP,
  /// Stone moving: Crossbone Isle's pillars, a stump pushed in Goma Cave,
  /// Mercury Lighthouse's pillars, and Sol Sanctum's centre statue.
  SE_STONE_GRIND,
  /// Saturos and Alex warping at Mercury Lighthouse's aerie and in the
  /// Elemental Star room, the party rising to that room, Hydros beaten, and
  /// Altmiller Cave's rock turned.
  SE_WARP,
  SE_191,
  SE_192,
  SE_193,
  SE_194,
  SE_195,
  /// The Wise One's quest asked in the Sanctum.
  SE_WISE_ONE_QUEST,
  SE_197,
  /// The villager reaching Kolima's post, after SE_KOLIMA_BRIDGE_HOP, before
  /// the drawbridge comes down.
  SE_KOLIMA_BRIDGE_POST,
  SE_199,
  /// The curse's sparkles at Kolima and Kolima Forest, Tret rejecting the
  /// party, and Sol Sanctum's statue opening its central hole.
  SE_SPARKLE_FALL,
  /// The party rising to the Elemental Star room, and an actor raised and
  /// shrunk in the eruption.
  SE_ASCEND,
  /// On the stormy night, as Isaac and Dora hurry to the plaza, a flame flares
  /// up and rises away as smoke (gCottageRoomsB_FlameRisesScript).
  SE_FLAME_FLARE,
  SE_203,
  /// Landing in dust at Babi Lighthouse, the Mercury Lighthouse entrance and
  /// Venus Lighthouse; the Tret Tree's cocoons and rooms; and the water drops
  /// striking Vault Cave's pillars.
  SE_DUST_PUFF,
  SE_205,
  /// Kolima and Kolima Forest cursed as the party watches, and a cocoon stepped
  /// on in the Tret Tree.
  SE_CURSE_FALL,
  SE_207,
  /// Played as Bane gets away a second time on Crossbone Isle.
  SE_BANE_ESCAPE,
  /// Mercury Lighthouse's door opening, as the light turns white after Mia
  /// casts Ply on it.
  SE_MERCURY_LIGHTHOUSE_DOOR_OPEN,
  /// A hidden way opening: Frost on Bilibin Cave's puddles, and Lunpa's hidden
  /// switch.
  SE_HIDDEN_PASSAGE_OPEN,
  /// Mercury Lighthouse's stone doors, fountain and stair pillars moving.
  SE_MERCURY_LIGHTHOUSE_MECHANISM,
  /// Plays as a Djinni returns to Set or elemental power rises in battle and
  /// the unit takes its casting pose, as field Psynergy is cast where it shows,
  /// with Frost on its target, and in the Annihilation, Atalanta, Gaia, Procne,
  /// Ragnarok, Titan Blade and other battle effects.
  SE_POWER_UP,
  /// A pushed log or pillar coming to rest, after SE_STOP_ACTION_SE cuts
  /// SE_HEAVY_MOVE: the Colosso matches, Kolima Forest's floodgate, Vale Cave,
  /// and the scenes' pushed actors (Actor_MoveAndRedraw).
  /// ★ Named from what the player sees: a pushed log or pillar stopping (its
  /// render)
  SE_HEAVY_STOP,
  /// The elemental arcs rising in the eruption, and the Tolbi-bound ship
  /// casting off. It sounds like bright chimes rising in three steps over high
  /// square-wave blips.
  SE_RISING_CHIMES,
  /// Hopping across Mercury Lighthouse's pool, and the crates sinking in the
  /// Lunpa Fortress.
  SE_SPLASH_SMALL,
  /// Mercury Lighthouse's randomized bursts, Tolbi Spring's rising coins, and
  /// the crates dropping into holes in Vale Cave. It sounds like a short burst
  /// of noise that trails off into a faint hiss for about two seconds.
  SE_BURST,
  /// Mercury Lighthouse's floor switches and gates, the Colosso's moving
  /// walkway, and an actor jumping down a ledge.
  SE_FLOOR_SWITCH,
  SE_218,
  /// Mercury Lighthouse's fountain, and the storm carrying the party off in
  /// Suhalla Desert and dropping it on the world map. It sounds like a rushing
  /// noise that swells in and fades over about three seconds.
  SE_GUSH,
  /// Frost on its target, Retreat spinning, Bilibin Cave's puddles, Fuchin
  /// Temple's Reveal lesson, Mia in Imil and at Mercury Lighthouse, the curse
  /// on Kolima, and the Elemental Stars taken by Alex.
  SE_SHIMMER,
  /// An actor lifted into a passage of Mercury Lighthouse amid sparks
  /// (MercuryLighthouse_RaiseActorWithSparks).
  SE_RISE_WITH_SPARKS,
  SE_222,
  /// The lifts of Mercury Lighthouse and its aerie, as they carry the party.
  /// ★ Named from what the player sees: the party riding Mercury Lighthouse's
  /// lifts (its render)
  SE_LIFT,
  SE_224,
  SE_225,
  /// A pusher and its log moving together: the Colosso, Crossbone Isle and
  /// Donpa's judgment in the Lunpa Fortress.
  SE_LOG_PUSH,
  /// A tree toppling in Fuchin Temple, and the obstacle Force knocks down over
  /// the Colosso's Board Walk.
  /// ★ Named from what the player sees: a tree or obstacle falling over (its
  /// render)
  SE_TOPPLE,
  /// Arriving and leaving through Mercury Lighthouse's passages, Venus
  /// Lighthouse's sparkling passage, and Xian's water carrier.
  SE_SPARKLING_PASSAGE,
  /// A splash as Altin's water beasts hop and land, in Altin and on Altin Peak.
  /// ★ Named from what the player sees: the water beasts landing in the water
  /// (its render)
  SE_SPLASH,
  /// Altin Peak's pools emptied, a water switch flipped at Crossbone Isle and
  /// Kolima Forest's floodgate, and Kalay's tunnel.
  SE_WATER_DRAIN,
  /// The water level changing at Crossbone Isle and at Kolima Forest's
  /// floodgate.
  /// ★ Named from what the player sees: the water rising and falling (its
  /// render)
  SE_WATER_LEVEL,
  /// The water rising or draining at Crossbone Isle and Kolima Forest's
  /// floodgate, and Altin Peak's cart stopping. It sounds like a quick crunch:
  /// a run of bright noise taps over a third of a second.
  SE_CRUNCH,
  /// Altin Peak's cart rolling, on player 3: as it sets off, after each turn
  /// and as a countdown runs out; SE_STOP_ACTION_SE cuts it.
  SE_CART_ROLL,
  /// Dora and Isaac turning to run back as Kyle heads down, on the stormy
  /// night.
  SE_RUN_BACK,
  SE_235,
  /// Each of the countdown's 3, 2 and 1 before a Colosso round, before
  /// SE_COLOSSO_GO.
  /// ★ Named from what the player sees: the Colosso's 3, 2, 1 lettering (its
  /// render)
  SE_COLOSSO_COUNTDOWN,
  /// The Go! that starts a Colosso round, after SE_COLOSSO_COUNTDOWN's three.
  /// ★ Named from what the player sees: the Colosso's Go! lettering (its
  /// render)
  SE_COLOSSO_GO,
  /// A thing pushed one cell: the Colosso matches and Vault Cave's pillar.
  SE_PUSH_STEP,
  /// A pushed log, pillar or wall moving, on player 3 until SE_STOP_ACTION_SE
  /// cuts it: the Colosso, Kolima Forest's floodgate, Vale Cave, Venus
  /// Lighthouse's sliding walls and the scenes' pushed actors; SE_HEAVY_STOP
  /// follows. ★ Named from what the player sees: a log rolling or a pillar or
  /// wall sliding (its render)
  SE_HEAVY_MOVE,
  /// A log pushed over: Kolima Forest, Mogall Forest, Vale Cave, the Colosso,
  /// Crossbone Isle, and monsters attacking the Tolbi-bound ship.
  SE_LOG_FALL,
  /// Something moving in water, on player 3 until SOUND_FADE_OUT_ACTION_SE
  /// fades it: the logs floated across Fuchin Falls Cave, the riding log in
  /// Vale Cave, the leader sinking in Mercury Lighthouse's pool and bathing in
  /// a Lamakan Desert oasis.
  /// ★ Named from what the player sees: logs floating and the leader in water
  /// (its render)
  SE_WATER_MOVE,
  SE_242,
  SE_243,
  /// Suhalla Desert's sandstorms rising, and the sandstorm catching the party
  /// on the world map.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  SE_SANDSTORM,
  SE_245,
  /// A spring searched, Retreat, the curse dust at Kolima, Tret restored,
  /// Mercury Lighthouse's Ply, Fuchin Temple's Reveal lesson, and the eruption
  /// in Sol Sanctum.
  SE_GLITTER,
  /// The lettering of a Colosso match's round and result, and of a Colosseum
  /// trial failed or cleared.
  SE_COLOSSO_LETTERING,
  SE_248,
  SE_249,
  SE_250,
  SE_251,
  /// A Colosso warrior fading in (Colosso_FadeInWarrior).
  /// ★ Named from what the player sees: a Colosso warrior appearing (its
  /// render)
  SE_COLOSSO_WARRIOR_APPEAR,
  SE_253,
  SE_254,
  SE_255,
  /// The first voice blip of a message: an actor's voice is 256 plus its
  /// sprite's value (Speaker_GetVoice), and every few letters typed play it
  /// plus the letter code's low two bits (Message_Type), so a voice is four
  /// blips. gSpeakerDefinitions gives each sprite one of eight voices; this
  /// first one is the animals': the dog, the cat, the bird and the pink winged
  /// monster.
  SE_VOICE_FIRST,
  SE_VOICE_ANIMAL_2,
  SE_VOICE_ANIMAL_3,
  SE_VOICE_ANIMAL_4,
  /// The girls' voice: Jenna as a child and the girls of the towns.
  SE_VOICE_GIRL_1,
  SE_VOICE_GIRL_2,
  SE_VOICE_GIRL_3,
  SE_VOICE_GIRL_4,
  /// The boys' voice: Isaac and Garet as children and the boys of the towns.
  SE_VOICE_BOY_1,
  SE_VOICE_BOY_2,
  SE_VOICE_BOY_3,
  SE_VOICE_BOY_4,
  /// The young women's voice: Mia, Jenna, Sheba, Feizhi and the young women of
  /// the towns.
  SE_VOICE_YOUNG_WOMAN_1,
  SE_VOICE_YOUNG_WOMAN_2,
  SE_VOICE_YOUNG_WOMAN_3,
  SE_VOICE_YOUNG_WOMAN_4,
  /// The young men's voice: Isaac, Garet and Ivan, Felix as a child, Hsu, the
  /// Djinn and the young men of the towns.
  SE_VOICE_YOUNG_MAN_1,
  SE_VOICE_YOUNG_MAN_2,
  SE_VOICE_YOUNG_MAN_3,
  SE_VOICE_YOUNG_MAN_4,
  /// The women's voice: Dora, Menardi, Layana, Laurel, Hama and the women and
  /// old women of the towns.
  SE_VOICE_WOMAN_1,
  SE_VOICE_WOMAN_2,
  SE_VOICE_WOMAN_3,
  SE_VOICE_WOMAN_4,
  /// The old men's voice: Kraden, Hammet, Nyunpa, Tret, Bunza, the Kraken and
  /// the old men of the towns.
  SE_VOICE_OLD_MAN_1,
  SE_VOICE_OLD_MAN_2,
  SE_VOICE_OLD_MAN_3,
  SE_VOICE_OLD_MAN_4,
  /// The men's voice: Felix, Saturos, Alex, Donpa, Kaja, Dodonpa, Babi, Kyle,
  /// Iodem, McCoy, the soldiers and the men of the towns.
  SE_VOICE_MAN_1,
  SE_VOICE_MAN_2,
  SE_VOICE_MAN_3,
  SE_VOICE_MAN_4,
  /// A sound with no tracks on player 3, which stops the loop playing there:
  /// every scene change (unless FLAG_SKIP_SCENE_CHANGE_SOUND), a rolling log or
  /// cart stopping, a battle starting on the field.
  SE_STOP_ACTION_SE,
  /// Not a sound: fades player 3 out over 48 VBlanks, so this row of the song
  /// table never plays.
  SOUND_FADE_OUT_ACTION_SE,
  SE_290,
  SE_291,
  /// The pair of upright effects spawned in the eruption at Sol Sanctum
  /// (SolSanctumEruption_SpawnEffectPair).
  SE_ERUPTION_COLUMNS,
  SE_293,
  SE_294,
  SE_295,
  SE_296,
  SE_297,
  /// A sound with no tracks on player 5, which stops what plays there; played
  /// before each queued scene sound (Scene_QueueMusic).
  SE_STOP_MUSIC_PLAYER_5,
  SE_299,
  /// Tolbi Spring: a coin or medal on its way, 50 frames into the toss
  /// (TolbiSpring_RunToss).
  SE_TOLBI_SPRING_TOSS,
  /// Tolbi Spring: a tossed coin meeting one of the moving obstacles.
  /// ★ Named from what the player sees: the coin striking an obstacle in the
  /// spring (its render)
  SE_TOLBI_SPRING_OBSTACLE_HIT,
  /// Lucky Dice: the dice thrown with A.
  /// ★ Named from what the player sees: the Lucky Dice thrown (its render)
  SE_LUCKY_DICE_THROW,
  /// Lucky Dice: each of the four numbers revealed, five frames apart.
  /// ★ Named from what the player sees: the Lucky Dice numbers shown (its
  /// render)
  SE_LUCKY_DICE_NUMBER,
  /// Lucky Wheels: the wheels starting to spin.
  /// ★ Named from what the player sees: the Lucky Wheels spinning (its render)
  SE_LUCKY_WHEELS_SPIN,
  /// Lucky Wheels: a wheel held or released.
  /// ★ Named from what the player sees: a Lucky Wheels wheel held (its render)
  SE_LUCKY_WHEELS_HOLD,
  /// Lucky Wheels: 16 frames after the wheels start, as the stop prompt shows.
  SE_LUCKY_WHEELS_STOP_PROMPT,
  /// Lucky Wheels: a wheel stopped.
  /// ★ Named from what the player sees: a Lucky Wheels wheel stopping (its
  /// render)
  SE_LUCKY_WHEELS_STOP,
  /// Plays on player 3 as the Wise One appears in the Sanctum, as the
  /// eruption's materialization tint fades in, and as the leader rises on Venus
  /// Lighthouse's platforms.
  SE_MATERIALIZE,
  /// The Venus Star bouncing three times as Saturos throws it into the beacon
  /// at Venus Lighthouse's aerie.
  /// ★ Named from what the player sees: the Venus Star bouncing on the aerie
  /// (its render)
  SE_STAR_BOUNCE,
  SE_310,
  SE_311
};

#endif
