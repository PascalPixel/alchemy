#ifndef ALCHEMY_SOUNDS_H
#define ALCHEMY_SOUNDS_H

/// @file
/// Sound ids, one for each row of gSongTable (sound/song_table.s) in order:
/// MUS_ ids are music and jingles, SE_ ids sound effects, as their sequences in
/// sound/songs are named. Music takes the title of its track where the two are
/// matched (MUS_TO_THE_OPEN_SEA), the titles as Miketendo64's track guide
/// lists them, and otherwise the place or moment it is the music of (MUS_PROX).
/// A row of the empty song (Sound_EmptySong), which plays nothing, has no name,
/// and the next named row's number is written after it. A sequence with no name
/// yet keeps its number as its placeholder: those the game plays say where, and
/// no code, table or actor script plays the rest. Four rows are controls no
/// sound plays from. Sound_Play tests the controls first, then the ranges: from
/// SOUND_SE_FIRST to SOUND_SE_END on the player its gSongTable row names, from
/// SOUND_JINGLE_FIRST jingles, music otherwise (sound_driver_symbols.h).

enum SoundId {
  /// A song of no tracks: scenes play it to silence the music, and the music
  /// rules give it to the Epilogue, and to Kibombo and the Gabomba Statue once
  /// flag 0x8f7 is set.
  MUS_SILENCE,
  /// Super Lucky Dice's music, started with each new bet (SuperLuckyDice_Run).
  MUS_SUPER_LUCKY_DICE,
  /// The field music of Alhafra and Eastern Alhafra.
  MUS_ALHAFRA,
  /// The field music of Champa, and of the Kandorean Temple's last two rooms,
  /// an Apojii Islands house and one islet.
  MUS_CHAMPA,
  /// The field music of Daila and West Indra Islet.
  MUS_DAILA,
  /// The field music of the Apojii Islands and SW Atteka Islet.
  MUS_APOJII_ISLANDS,
  /// Izumo's field music once FLAG_SERPENT_SLAIN is set, when the town
  /// celebrates.
  MUS_IZUMO_FESTIVAL,
  /// The field music of Izumo and the Izumo Ruins.
  MUS_IZUMO,
  /// Plays as the party learns who the Doom Dragon was, at Mars Lighthouse's
  /// aerie (MarsLighthouseAerie_FindWhoTheDragonWas).
  MUS_THE_WISE_ONES_TRAP,
  /// The field music of the Kandorean Temple's first seven rooms.
  MUS_KANDOREAN_TEMPLE_CAVES,
  /// The field music of Naribwe, the Kibombo Mountains and Kibombo's first
  /// three scenes.
  MUS_KIBOMBO,
  /// The field music of Kibombo's fourth and fifth scenes.
  MUS_RITUALS,
  /// The field music of Lemuria and Ancient Lemuria.
  MUS_LEMURIA,
  /// The field music of Madra, Mikasalla and Hesperia Settlement.
  MUS_MADRA,
  /// The field music of Kalt Island, one islet and a Champa house.
  MUS_ISLANDS,
  /// The field music of Garoh.
  MUS_GAROH,
  /// The field music of Yallam and a Mikasalla house.
  MUS_YALLAM,
  /// Plays as Yallam's children start their song game ("If you want to go to
  /// the stars..."), and as Echo is met on the World Map once
  /// FLAG_ALREADY_KNOWS_DJINN is set.
  MUS_CHILDRENS_SONG,
  /// MUS_CHILDRENS_SONG's tune arranged on four tracks for its eight; only the
  /// link
  /// lobby's song request plays it.
  MUS_CHILDRENS_SONG_SHORT,
  /// The field music of Air's Rock.
  MUS_AIRS_ROCK = 20,
  /// The field music of Aqua Rock.
  MUS_AQUA_ROCK,
  /// The field music of the Madra Catacombs, Alhafran Cave, Islet Cave and
  /// Apojii Islands Cave.
  MUS_MADRA_CATACOMBS,
  /// The field music of the Shrine of the Sea God, Shaman Village Cave and the
  /// Indra, Osenia, Atteka and Angara Caverns.
  MUS_CAVERNS,
  /// The field music of Gaia Rock.
  MUS_GAIA_ROCK,
  /// The field music of the Gabomba Statue.
  MUS_INSIDE_THE_GABOMBA_STATUE,
  /// The field music of Jupiter Lighthouse.
  MUS_JUPITER_LIGHTHOUSE,
  /// The field music of Magma Rock.
  MUS_MAGMA_ROCK,
  /// The field music of Mars Lighthouse.
  MUS_MARS_LIGHTHOUSE,
  /// The field music of Tundaria Tower.
  MUS_TUNDARIA_TOWER,
  /// The field music of Taopo Swamp.
  MUS_TAOPO_SWAMP,
  /// The field music of the Ankohl Ruins.
  MUS_ANKOHL_RUINS,
  /// Plays as Karst and Agatio face the party at Jupiter Lighthouse, and as
  /// Agatio wakes in the Mars Star room.
  MUS_KARST_AND_AGATIO = 34,
  /// Plays as Briggs steals the ship in Eastern Alhafra, and as Echo is met on
  /// the World Map before FLAG_ALREADY_KNOWS_DJINN is set.
  MUS_BRIGGS_STEALS_SHIP,
  /// Plays as a foe confronts the party: Karst in Madra, the challenger before
  /// the Avimander in the Champa cliff caves, the Jupiter Star cast at the
  /// beacon, and the Doom Dragon.
  MUS_CONFRONTATION,
  /// The field music of Shaman Village and Loho.
  MUS_SHAMAN_VILLAGE,
  /// Plays on the World Map as Sheba tells what she hoped to learn at Jupiter
  /// Lighthouse.
  MUS_SHEBAS_PAST,
  /// Plays as Alex appears: in Champa, at Jupiter Lighthouse's beacon and
  /// fight, and before the Doom Dragon.
  MUS_ALEX,
  /// The title intro's music at power-on, when a save file exists, and before a
  /// new game's intro (the title's Scene_Initialize).
  MUS_MAIN_THEME_TITLE,
  /// The field music of the Lemurian Ship and the Northern Reaches, and of the
  /// World Map once FLAG_SAILING is set.
  MUS_TO_THE_OPEN_SEA,
  /// The field music of the World Map, Osenia Cliffs, Gondowan Cliffs and
  /// Atteka Inlet.
  MUS_A_JOURNEY_AROUND_THE_WORLD,
  /// MUS_A_JOURNEY_AROUND_THE_WORLD's places once FLAG_REUNION is set.
  MUS_JOURNEY_TO_SAVE_THE_WORLD,
  /// The music of the Staff Credit scene.
  MUS_STAFF_CREDIT,
  /// The battle music when Jenna is in the front row without Felix or Isaac.
  MUS_BATTLE_JENNA = 49,
  /// The battle music: the usual one, and with Felix in the front row.
  MUS_BATTLE_THE_LOST_AGE,
  /// Battle music gEncounter_BattleMusic gives Briggs's battle on his ship at
  /// Eastern Alhafra (ENCOUNTER_ZONE_SCRIPTED_YAMPI_DESERT_ALHAFRA, slot 1) and
  /// ENCOUNTER_TABLE_MOAPA's slots 0 to 4.
  MUS_BRIGGS_BATTLE,
  /// Plays as the Aqua Hydra rises before the Lemurian Ship
  /// (LemurianShip_FightAquaHydra).
  MUS_AQUA_HYDRA,
  /// Plays as Karst and Agatio wake as the Flame Dragons in the Mars Star room.
  MUS_BATTLE_WITH_FLAME_DRAGON,
  /// Battle music of the Doom Dragon at Mars Lighthouse's aerie
  /// (ENCOUNTER_ZONE_SCRIPTED, slot 1).
  MUS_BATTLE_WITH_DOOM_DRAGON,
  /// The Broken Seal's MUS_TEST_YOUR_SKILLS, note for note: after a link
  /// battle's
  /// own music (Battle_Run), and at Super Lucky Dice's results past the second
  /// kind (SuperLuckyDice_Run).
  MUS_TEST_YOUR_SKILLS,
  /// The battle music at sea, while the Lemurian Ship is the actor events move
  /// (ownerOverride) and it does not fly.
  MUS_BATTLE_ON_THE_SEA,
  /// Battle music of Karst and Agatio at Jupiter Lighthouse's beacon (encounter
  /// table 13, slot 7, and slots 5 and 6); its tune is MUS_KARST_AND_AGATIO's.
  MUS_BATTLE_WITH_KARST_AND_AGATIO,
  /// The battle is won.
  MUS_VICTORY,
  /// The party falls in battle.
  MUS_DEFEATED,
  /// The Broken Seal's MUS_WIND_ADEPTS, note for note.
  MUS_WIND_ADEPTS,
  /// Plays as the ship drifts north of the continent on the World Map
  /// (WorldMap_DriftNorthOfContinent).
  MUS_FATED_CONNECTION_WITH_SWORN_ENEMIES,
  /// The field music of Prox.
  MUS_PROX,
  /// Plays as the Jupiter Star lights Jupiter Lighthouse's beacon, and as the
  /// Golden Sun rises at Mars Lighthouse's aerie.
  MUS_ALCHEMY_UNLEASHED,
  /// Unused: The Broken Seal's MUS_THE_PASSAGE_OF_TIME, note for note.
  MUS_UNUSED_TITLE_AFTER_PROLOGUE,
  /// The field music of the Sea of Time.
  MUS_SEA_OF_TIME,
  /// The Epilogue's music after its first lines.
  MUS_EPILOGUE = 67,
  /// The staff credits' music (the title's entrance 8, Credits_Run).
  MUS_THE_BROKEN_SEAL,
  /// Unused: The Broken Seal's MUS_MAIN_THEME_TITLE, note for note.
  MUS_UNUSED_TITLE = 70,
  /// Plays as a password is entered in the Clear scene (Clear_EnterPassword).
  MUS_PASSWORD_ENTRY,
  /// The field music of the Mercury Lighthouse scene and one way into the Mars
  /// Lighthouse aerie.
  MUS_MERCURY_LIGHTHOUSE,
  /// Plays in the Epilogue as the villagers' picture shows.
  MUS_EPILOGUE_VILLAGERS,
  /// The music of the Clear scene, area "(Clear data)", where data is
  /// transferred.
  MUS_THE_ADVENTURE_RESUMES,
  /// The title held without its prompt: after the prologue (the title's
  /// entrance 3), and as the logo shows after the ending (entrance 10).
  MUS_TITLE_AFTER_PROLOGUE,
  /// The Trial Road race: Moapa's challenge, the rules and the countdown in
  /// Shaman Village, and the Trial Road's music while
  /// FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT is set.
  MUS_TRIAL_ROAD,
  /// Not a sound: fades the music out.
  SOUND_FADE_OUT_BGM = 78,
  /// Not a sound: keeps the music that is playing.
  SOUND_KEEP_BGM,
  /// A puzzle is solved, such as Aqua Rock's and the lighthouses'.
  MUS_PUZZLE_SOLVED,
  /// Someone joins the party.
  MUS_A_NEW_ALLY,
  /// A unit is revived.
  MUS_RECOVERING_AFTER_COMBAT,
  /// Treasure is found.
  MUS_TREASURE_FOUND,
  /// A unit is cured.
  MUS_POISON_CURED,
  /// The game is saved.
  MUS_SAVED,
  /// A night at an inn.
  MUS_OVERNIGHT_AT_THE_INN,
  /// A sanctum removes a curse.
  MUS_CURSE_LIFTED,
  /// A sanctum repels evil.
  MUS_EXORCISED,
  /// A unit levels up, among a battle's spoils.
  MUS_LEVEL_UP,
  /// Lucky Dice's and Contigo's smallest prize.
  MUS_LUCKY = 91,
  /// Lucky Dice's and Contigo's middle prize.
  MUS_VERY_LUCKY,
  /// Lucky Dice's, Lucky Wheels' and Contigo's largest prize.
  MUS_SUPER_LUCKY,
  /// MUS_SILENCE's song of no tracks, on the jingle player; nothing plays it.
  MUS_UNUSED_SILENCE_JINGLE = 95,
  SE_BELL = 100,
  SE_SHOP_PURCHASE,
  SE_SHOP_SELL,
  SE_CURSE,
  /// Something launched: the Stone Justice boulders thrown up, a Mars
  /// Lighthouse fireball launched to roll ahead (Actor_LaunchAhead), and the
  /// Azul, Charon, Megiddo, Flora, Moloch and other battle effects; The Broken
  /// Seal's row 104 note for note.
  SE_LAUNCH,
  /// Super Lucky Dice's winnings counted into the party's coins.
  SE_COIN_PAYOUT,
  SE_DRIP,
  SE_TREMOR,
  SE_TRIPLE_TONE_LOW,
  SE_TRIPLE_TONE_HIGH,
  SE_CHIME,
  SE_MENU_CURSOR_MOVE,
  SE_MENU_CONFIRM,
  SE_MENU_CANCEL,
  SE_MENU_ERROR,
  SE_ICE,
  SE_TOLBI_SPRING_TOSS_PROMPT,
  SE_ITEM_REMOVE,
  SE_FOOTSTEP,
  /// SE_FOOTSTEP's two notes far lower on its voice; nothing plays it.
  SE_UNUSED_FOOTSTEP_LOW,
  SE_120,
  SE_SCUFFLE,
  /// Gasp fleeing the party on the Trial Road (TrialRoad_GaspFlees), after
  /// SE_ENEMY_CRY.
  SE_DJINNI_FLEES,
  SE_MAP_EXIT,
  SE_CHEST_OPEN,
  SE_CHEST_CLOSE,
  SE_RECOVERY,
  SE_LANDING_THUD,
  SE_STAIRS_UP,
  SE_STAIRS_DOWN,
  SE_PSYNERGY_START,
  SE_PSYNERGY_SPARKLE,
  SE_WATER,
  SE_IMPACT,
  SE_HEAVY_IMPACT,
  SE_CLICK,
  SE_WHOOSH,
  /// Plays in the Kirin summon, 46 frames into its charge; the field plays it
  /// too, as rain starts and in Air's Rock.
  SE_KIRIN,
  SE_ITEM_BREAK,
  /// A Djinni set in the Djinn menu, and a Djinni recovering to Set as the
  /// party walks (Field_UpdateWalking); also the summon tablets' runes,
  /// Champa's embers and the Serac, Eclipse, Cruel Ruin and other battle
  /// effects.
  SE_DJINNI_SET,
  SE_SPIN,
  SE_RUMBLE,
  SE_SWIRL,
  SE_CRACK,
  SE_CLATTER,
  SE_CRASH,
  SE_ENEMY_CRY,
  SE_BEAST_CRY,
  SE_BOSS_ROAR,
  SE_MONSTER_ROAR,
  SE_UNUSED_DEFEAT_CRY,
  SE_LIGHT_APPEAR,
  SE_JUMP,
  SE_HIGH_JUMP,
  SE_CAST,
  SE_SWITCH,
  SE_SWITCH_MECHANISM,
  SE_GATE_MOVE,
  SE_DOOR_OPEN,
  SE_DOOR_CLOSE,
  SE_160,
  SE_RATTLE,
  /// The Mercury, Judgment and Neptune battle effects; also Charon and Meteor
  /// Blow, the Aquarius Stone in Aqua Rock and the Kandorean Temple's steam
  /// vents and lifts.
  SE_WAVE,
  /// SE_WAVE's notes thirteen semitones higher: the tidal wave that carries
  /// Idejima off on the World Map, the whirlwinds clearing Air's Rock's sand
  /// wall and fog, and the Meteor Blow, Megaera and Daedalus battle effects;
  /// The Broken Seal's row 163 note for note.
  SE_WAVE_HIGH,
  SE_SCATTER,
  /// Played as the birds land on Venus Lighthouse's entrance and fly off.
  SE_BIRD_WINGS,
  /// The Broken Seal's SE_UNUSED_AMBIENT_WIND_LOW note for note: one soft note
  /// held on noise voice 42 until stopped; nothing plays it.
  SE_UNUSED_AMBIENT_WIND_LOW,
  SE_MAP_TRANSITION,
  /// Played three times as the Ruin Key opens the Madra Catacombs.
  SE_UNLOCK,
  /// Nothing in this game plays it.
  SE_AMBIENT_WIND,
  /// The steam vents and waterfalls: Taopo Swamp's and Aqua Rock's vents, the
  /// settlements' waterfall, the Kandorean Temple and the Apojii Islands.
  SE_AMBIENT_WATER,
  SE_THUNDER,
  SE_THUNDER_LOUD,
  SE_NOTE_LOW,
  SE_NOTE_MID,
  SE_NOTE_HIGH,
  SE_KOLIMA_BRIDGE_HOP,
  /// The Thunder card striking in Legend's element cards
  /// (BattleEffect_RunElementCards); The Broken Seal's row 177 note for note.
  SE_THUNDER_CARD,
  /// Power gathering round someone: the leader lighting Jupiter Lighthouse, and
  /// Alex's arc of energy at Mt. Aleph and Venus Lighthouse; The Broken Seal's
  /// row 178 note for note.
  SE_GATHERING_POWER,
  /// The wind funnel battle effect forming (BattleEffect_RunWindFunnel); The
  /// Broken Seal's row 179 note for note.
  SE_WIND_FUNNEL,
  SE_SETTLE,
  SE_BOOM,
  SE_GEM_SET,
  SE_STONE_RUMBLE,
  /// Played as an actor pops up and as Aqua Rock's moai steps aside.
  SE_POP_UP,
  SE_PUSH,
  SE_SOL_SANCTUM_LIGHT_UP,
  SE_FALLING_LIGHT,
  SE_THUMP,
  SE_STONE_GRIND,
  SE_WARP,
  /// Played as a face over one of Gaia Rock's altars shakes and slides aside,
  /// at Zagan's frame 32 as it gallops in its blaze (BattleEffect_RunZagan),
  /// and as Megiddo's sun starts to fall (BattleEffect_RunMegiddo):
  /// SE_TABLET_RISE's note nine semitones higher.
  SE_TABLET_RISE_HIGH,
  /// Shriek's and Harpy's cry as their streaks set out
  /// (BattleEffect_RunStreakVolley, mode 1): SE_SPARKS's notes on voice 25; The
  /// Broken Seal's row 192 note for note.
  SE_SHRIEK,
  /// A doll placed on one of Gaia Rock's altars ("Felix placed a doll on the
  /// altar.").
  SE_DOLL_PLACED,
  /// A Psynergy Stone's tablet rising in Air's, Aqua, Gaia and Magma Rock as
  /// Reveal, Parch, Sand and Blaze are learned; also Obaba forging the Trident
  /// in the Champa cliff caves.
  SE_TABLET_RISE,
  /// The sparks that circle the leader as Reveal, Parch, Sand and Blaze are
  /// learned, and the Anemos Inner Sanctum's pillar sparks.
  SE_SPARKS,
  SE_WISE_ONE_QUEST,
  /// The Great Gabomba speaking to Kibombo's witch doctor
  /// (GabombaStatueChamber_GabombaNamesWitchDoctor): one note bent down and
  /// down; The Broken Seal's row 197 note for note.
  SE_GABOMBA_VOICE,
  SE_KOLIMA_BRIDGE_POST,
  /// A water column rising (BattleEffect_RunWaterColumn); The Broken Seal's row
  /// 199 note for note.
  SE_WATER_COLUMN,
  SE_SPARKLE_FALL,
  SE_ASCEND,
  SE_FLAME_FLARE,
  SE_203,
  SE_DUST_PUFF,
  /// SE_AMBIENT_STREAM ten semitones higher on a noise voice and not held;
  /// nothing plays it.
  SE_UNUSED_AMBIENT_STREAM_HIGH,
  SE_CURSE_FALL,
  SE_RUSH,
  SE_BANE_ESCAPE,
  SE_MERCURY_LIGHTHOUSE_DOOR_OPEN,
  SE_HIDDEN_PASSAGE_OPEN,
  SE_MERCURY_LIGHTHOUSE_MECHANISM,
  SE_POWER_UP,
  SE_HEAVY_STOP,
  SE_RISING_CHIMES,
  SE_SPLASH_SMALL,
  SE_BURST,
  SE_FLOOR_SWITCH,
  SE_218,
  SE_GUSH,
  SE_SHIMMER,
  SE_RISE_WITH_SPARKS,
  /// Played as the whirlwinds of Air's Rock and the gusts of Jupiter Lighthouse
  /// appear.
  SE_WHIRLWIND,
  SE_LIFT,
  SE_224,
  /// Played as Moloch's white cloud starts billowing up from the ground under
  /// the falling flakes, at its frame 90 (BattleEffect_RunMoloch).
  SE_CLOUD_BILLOWS,
  SE_LOG_PUSH,
  SE_TOPPLE,
  SE_SPARKLING_PASSAGE,
  SE_SPLASH,
  /// The Broken Seal's SE_WATER_DRAIN, note for note: in the Judgment battle
  /// effect.
  SE_WATER_DRAIN,
  SE_WATER_LEVEL,
  SE_CRUNCH,
  SE_CART_ROLL,
  SE_RUN_BACK,
  /// Played as Air's Rock blows its gusts.
  SE_GUST,
  SE_COLOSSO_COUNTDOWN,
  SE_COLOSSO_GO,
  SE_PUSH_STEP,
  SE_HEAVY_MOVE,
  SE_LOG_FALL,
  SE_WATER_MOVE,
  /// Two notes held until stopped on the Curse sample, doubled on noise voice
  /// 28 and on the Rumble sample; nothing plays it.
  SE_UNUSED_CURSE_RUMBLE,
  /// A lift of the Lemurian Ship stepped on (LemurianShip_StepOnLift).
  SE_LEMURIAN_SHIP_LIFT,
  SE_SANDSTORM,
  /// Held while a room's air or water streams: queued with Air's Rock's inner
  /// rooms and the Kandorean Temple's lift room, and played as the Shrine of
  /// the Sea God's water falls; The Broken Seal's row 245 note for note.
  SE_AMBIENT_STREAM,
  SE_GLITTER,
  SE_COLOSSO_LETTERING,
  /// An arrow of the Gabomba Statue raised or lowered once its statues are
  /// ready.
  SE_GABOMBA_STATUE_ARROW,
  SE_249,
  /// SE_BLACK_CRYSTAL_SET seven semitones lower; nothing plays it.
  SE_UNUSED_BLACK_CRYSTAL_SET_LOW,
  /// Played as the Black Crystal is set on the Lemurian Ship.
  SE_BLACK_CRYSTAL_SET,
  SE_COLOSSO_WARRIOR_APPEAR,
  /// SE_COLOSSO_WARRIOR_APPEAR's rising note on noise voice 20, doubled an
  /// octave lower on the Roar sample; nothing plays it.
  SE_UNUSED_COLOSSO_WARRIOR_ROAR,
  /// The voice blips of a message: an actor's voice is SE_VOICE_FIRST plus four
  /// times its voice, and the letters typed play it plus the letter's low two
  /// bits, so a voice is four blips, _A to _D.
  SE_VOICE_ANIMAL_A = 256,
  SE_VOICE_ANIMAL_B,
  SE_VOICE_ANIMAL_C,
  SE_VOICE_ANIMAL_D,
  SE_VOICE_GIRL_A,
  SE_VOICE_GIRL_B,
  SE_VOICE_GIRL_C,
  SE_VOICE_GIRL_D,
  SE_VOICE_BOY_A,
  SE_VOICE_BOY_B,
  SE_VOICE_BOY_C,
  SE_VOICE_BOY_D,
  SE_VOICE_YOUNG_WOMAN_A,
  SE_VOICE_YOUNG_WOMAN_B,
  SE_VOICE_YOUNG_WOMAN_C,
  SE_VOICE_YOUNG_WOMAN_D,
  SE_VOICE_YOUNG_MAN_A,
  SE_VOICE_YOUNG_MAN_B,
  SE_VOICE_YOUNG_MAN_C,
  SE_VOICE_YOUNG_MAN_D,
  SE_VOICE_WOMAN_A,
  SE_VOICE_WOMAN_B,
  SE_VOICE_WOMAN_C,
  SE_VOICE_WOMAN_D,
  SE_VOICE_OLD_MAN_A,
  SE_VOICE_OLD_MAN_B,
  SE_VOICE_OLD_MAN_C,
  SE_VOICE_OLD_MAN_D,
  SE_VOICE_MAN_A,
  SE_VOICE_MAN_B,
  SE_VOICE_MAN_C,
  SE_VOICE_MAN_D,
  SE_STOP_ACTION_SE,
  /// SE_TAKE_FLIGHT higher, its first track six semitones up without the
  /// chord's low note and its second four; nothing plays it.
  SE_UNUSED_TAKE_FLIGHT_HIGH = 290,
  /// Played as an actor takes flight.
  SE_TAKE_FLIGHT,
  SE_ERUPTION_COLUMNS,
  /// SE_ERUPTION_COLUMNS four semitones lower; nothing plays it.
  SE_UNUSED_ERUPTION_COLUMNS_LOW,
  SE_STOP_EXIT_SE = 298,
  SE_TOLBI_SPRING_TOSS = 300,
  SE_TOLBI_SPRING_OBSTACLE_HIT,
  SE_LUCKY_DICE_THROW,
  SE_LUCKY_DICE_NUMBER,
  SE_LUCKY_WHEELS_SPIN,
  SE_LUCKY_WHEELS_HOLD,
  SE_LUCKY_WHEELS_STOP_PROMPT,
  SE_LUCKY_WHEELS_STOP,
  SE_MATERIALIZE,
  SE_STAR_BOUNCE,
  /// The leader dropping down a ledge or through a hole, a block or flower
  /// knocked off a ledge, and Jupiter Lighthouse's floor opening.
  SE_FALL,
  /// A chord held until stopped on the Colosso lettering's sample, under rising
  /// runs on noise voice 23 and the Sparks sample and a note of Psynergy
  /// sparkle; nothing plays it.
  SE_UNUSED_SPARKLING_LETTERING,
  /// Not a sound: fades out what MUSIC_PLAYER_ACTION_SE and MUSIC_PLAYER_SE5
  /// play.
  SOUND_FADE_OUT_ACTION_SE = 390,
  /// Not a sound: fades out what MUSIC_PLAYER_SE5 plays.
  SOUND_FADE_OUT_EXIT_SE = 395,
  /// The field Psynergy Lash.
  SE_LASH = 512,
  /// SE_CAST's notes an octave lower on voice 99, against a falling line on
  /// voice 98; nothing plays it.
  SE_UNUSED_CAST_LOW = 520,
  /// SE_SPIN_AWAY's voice on one long note, where SE_SPIN_AWAY plays a falling
  /// run of eleven short ones; nothing plays it.
  SE_UNUSED_SPIN_AWAY_SINGLE,
  /// The beams of Jupiter Lighthouse's aerie flickering as its angels are
  /// shot.
  SE_JUPITER_LIGHTHOUSE_BEAMS,
  /// SE_MARS_LIGHTHOUSE_DOORWAY's notes on voice 70 at tempo 100; nothing plays
  /// it.
  SE_UNUSED_MARS_LIGHTHOUSE_DOORWAY_A,
  /// SE_528's first eight notes and times on noise voice 21 at SE_529's tempo,
  /// the seventh 82 for 63; nothing plays it.
  SE_524,
  /// Played as Garoh's people howl at night.
  SE_HOWL,
  /// SE_HOWL's note and length on voice 115 at a slower tempo; nothing plays
  /// it.
  SE_UNUSED_HOWL_SLOW,
  /// SE_GABOMBA_JAWS_OPEN's voice on one note above its two, held and bent up
  /// and down until stopped; nothing plays it.
  SE_UNUSED_GABOMBA_JAWS_OPEN_HELD,
  SE_528,
  /// SE_528's notes at a faster tempo, their loudness rising and falling;
  /// nothing plays either.
  SE_529,
  /// The Sea of Time opening a way (SeaOfTime_Open).
  SE_SEA_OF_TIME_OPENS,
  /// SE_SANDSTORM's second track alone, six semitones lower and played once;
  /// nothing plays it.
  SE_UNUSED_SANDSTORM_LOW,
  SE_532,
  SE_533,
  SE_534,
  /// The Madra Catacombs' chest sliding with its bookshelf.
  SE_BOOKSHELF_SLIDE,
  SE_536,
  /// SE_536's notes six semitones lower and more slowly; nothing plays
  /// either.
  SE_538 = 538,
  /// The leader emerging as Sand ends (FieldPsynergy_EndSand).
  SE_SAND_EMERGE,
  /// The field Psynergy Parch.
  SE_PARCH = 542,
  /// SE_PARCH's notes fourteen semitones higher on its first voice, one more
  /// among them and nine after, doubled softly on noise voice 21; nothing plays
  /// it.
  SE_UNUSED_PARCH_HIGH,
  /// The sand mound sinking away as Sand ends (FieldPsynergy_EndSand).
  SE_SAND_END,
  /// SE_UNUSED_CURSE_RUMBLE on its voices with lower notes, louder; nothing
  /// plays it.
  SE_UNUSED_CURSE_RUMBLE_LOW,
  /// SE_SETTLE's notes played once instead of held, with voice 93 in its
  /// first track; nothing plays it.
  SE_UNUSED_SETTLE_SHORT,
  /// SE_TABLET_RISE two semitones lower and softer; nothing plays it.
  SE_UNUSED_TABLET_RISE_LOW,
  /// The first seven notes of SE_538's second track seventeen semitones lower,
  /// louder and at half its tempo; nothing plays either.
  SE_548,
  /// SE_BOOKSHELF_SLIDE with its first note three semitones higher; nothing
  /// plays it.
  SE_UNUSED_BOOKSHELF_SLIDE_HIGH,
  /// Played as the party drops through a hole on an islet.
  SE_DROP_THROUGH_HOLE,
  SE_551,
  /// SE_551's five note times and three of its notes on voice 92, at a faster
  /// tempo; nothing plays either.
  SE_552,
  /// Played as Magma Rock's flames grow.
  SE_FLAME_GROW = 554,
  /// SE_FLAME_GROW's falling run fourteen semitones lower on voice 99, its last
  /// note two higher; nothing plays it.
  SE_UNUSED_FLAME_GROW_LOW,
  /// SE_RISE_WITH_SPARKS two semitones higher, held until stopped: the Ankohl
  /// Ruins' sand rising, Aqua Rock's pillars rising as its stones are Doused,
  /// Jupiter Lighthouse's angels lifted, and the Sea of Time's waves stilled.
  SE_RISE_WITH_SPARKS_HIGH,
  /// SE_RISE_WITH_SPARKS three semitones lower, held until stopped: Aqua Rock's
  /// spring and pillars sinking, Magma Rock's flames growing, and the Sea of
  /// Time's waves starting and the sea opening.
  SE_RISE_WITH_SPARKS_LOW,
  /// A run of ten short notes on the Thunder sample; nothing plays it.
  SE_UNUSED_THUNDER_RUN,
  /// SE_UNUSED_COIN_PAYOUT_C an octave lower on SE_ICE_CRACK's voice, with four
  /// notes more; nothing plays it.
  SE_UNUSED_COIN_PAYOUT_LOW,
  /// A doorway of Mars Lighthouse's dragon-statue room flashing as it is
  /// stepped toward.
  SE_MARS_LIGHTHOUSE_DOORWAY,
  /// SE_MARS_LIGHTHOUSE_DOORWAY's notes on voice 94 at tempo 110; nothing plays
  /// it.
  SE_UNUSED_MARS_LIGHTHOUSE_DOORWAY_B,
  SE_562,
  /// SE_COIN_PAYOUT's notes with voice 93 in its first track; nothing plays it.
  SE_UNUSED_COIN_PAYOUT_A,
  /// SE_COIN_PAYOUT's notes on voices 92 and 90: a stone face's eyes glowing as
  /// its piece is set, in the Ankohl Ruins and at Gaia Rock's altars.
  SE_EYES_GLOW,
  /// SE_LUCKY_WHEELS_HOLD five semitones lower, on voice 92; nothing plays it.
  SE_UNUSED_LUCKY_WHEELS_HOLD_LOW,
  /// The leader spinning as Sand begins (FieldPsynergy_Sand), and as the party
  /// rides Aqua Rock's falls.
  SE_SPIRAL,
  /// The field Psynergy Pound strikes, and the like blows of Magma Rock's rocks
  /// and Treasure Isle's spiky rock.
  SE_POUND,
  /// SE_SPIN_AWAY's notes with voice 97 in its first track; nothing plays it.
  SE_UNUSED_SPIN_AWAY_B,
  /// SE_SPIN_AWAY an octave higher and faster, on voice 93; nothing plays it.
  SE_UNUSED_SPIN_AWAY_HIGH_A,
  /// SE_CURRENT's rhythm on its voice with other notes, the last two held long;
  /// nothing plays it.
  SE_UNUSED_CURRENT_HELD,
  /// SE_CURRENT at volume 100 for its 40; nothing plays it, nor its copy
  /// SE_UNUSED_LOUD_CURRENT_B.
  SE_UNUSED_LOUD_CURRENT_A,
  /// A rolling fireball breaking a tall ice crystal beside its path
  /// (Actor_RollAndCrumble), Mars Lighthouse's ice ground away, and the Doom
  /// Dragon faced.
  SE_CRUMBLE,
  /// Played as Mars Lighthouse's ice sheets crack, and as a rolling fireball
  /// breaks a small ice crystal beside its path (Actor_RollAndCrumble).
  SE_ICE_CRACK,
  /// The white flash: as Psynergy is learned in the four rocks, as a pillar of
  /// the Anemos Inner Sanctum lights, and as Burst breaks Mars Lighthouse's
  /// ice.
  SE_FLASH,
  /// A face piece or pillar set in place in the Ankohl Ruins.
  SE_ANKOHL_RUINS_PIECE,
  /// A statue stirring: Gaia Rock's faces turning to the doll set before them,
  /// the Great Gabomba answering Kibombo's offerings, and Gabomba bestowing its
  /// power in the catacombs.
  SE_STATUE_STIRS,
  /// The Great Gabomba's jaws opening as it sees Kibombo's jewel.
  SE_GABOMBA_JAWS_OPEN,
  /// SE_UNUSED_LOUD_CURRENT_A's notes again; nothing plays it.
  SE_UNUSED_LOUD_CURRENT_B,
  SE_579,
  /// SE_GABOMBA_STATUE_HOLE's first three pairs of notes, the first two an
  /// octave higher and the third two; nothing plays it.
  SE_UNUSED_GABOMBA_STATUE_HOLE_HIGH,
  /// The leader jumping down a hole of the Gabomba Statue.
  SE_GABOMBA_STATUE_HOLE,
  /// SE_LEMURIAN_SHIP_LIFT's notes with the second pair sooner, louder and at
  /// a faster tempo, on voice 113 for its 21: an Anemos Inner Sanctum lift
  /// turning its arrow up and floating once the room's seven round switches are
  /// pressed (AnemosInnerSanctum_PressRoundSwitch), and the Gabomba Statue's
  /// flashing cog dropping into place (GabombaStatue_CogsTurnBack).
  SE_LEMURIAN_SHIP_LIFT_FAST,
  /// SE_ICE_CRACK's second note alone, held longer; nothing plays it.
  SE_UNUSED_ICE_CRACK_SINGLE,
  /// Played as the party presses a switch or button in Mars Lighthouse, the
  /// Ankohl Ruins and the Anemos Inner Sanctum.
  SE_BUTTON_PRESS,
  /// One short note on SE_SPARKS's sample; nothing plays it.
  SE_UNUSED_SPARK,
  /// The Gabomba Statue's cogs turning back.
  SE_GABOMBA_STATUE_COGS,
  SE_587,
  SE_588,
  /// SE_588's times and loudness, a note of each of its chords, with other
  /// notes falling on the Surf sample (voice 108) at a slower tempo; nothing
  /// plays either.
  SE_UNUSED_SURF_FALL,
  /// Played as Gaia Rock's statue rises under the leader.
  SE_STATUE_RISE,
  /// Two short rising notes on SE_STATUE_STIRS's sample and on voice 96;
  /// nothing plays it.
  SE_UNUSED_STATUE_STIRS_SHORT,
  /// SE_IMPACT's rhythm and most of its notes, doubled by a second track on
  /// voice 90; nothing plays it.
  SE_UNUSED_IMPACT_DOUBLED,
  SE_593,
  /// SE_UNUSED_CURSE_RUMBLE_LOW's noise and Rumble tracks alone; nothing plays
  /// it.
  SE_UNUSED_RUMBLE_LOW,
  /// The Doom Dragon flickering into the people it was, at Mars Lighthouse's
  /// aerie.
  SE_DRAGON_FLICKER,
  /// The leader moving as sand (Actor_MoveAsSand), and the scorpion rumbling
  /// under the Yampi Desert.
  SE_SAND_MOVE,
  /// Played as the ship rides the Sea of Time's current.
  SE_CURRENT,
  /// Rings of light rising: Contigo's sparkle, Mars Lighthouse's aerie, and the
  /// Psynergy test room.
  SE_RISING_RINGS,
  /// A held note on the Crash sample under two chords on SE_SPIN_AWAY's sample;
  /// nothing plays it.
  SE_UNUSED_SPIN_AWAY_CRASH = 610,
  /// SE_COIN_PAYOUT's notes with voice 119 in its first track; nothing plays
  /// it.
  SE_UNUSED_COIN_PAYOUT_B,
  /// Played as the leader spins away.
  SE_SPIN_AWAY,
  /// SE_SPIN_AWAY's first track alone, two octaves higher; nothing plays it.
  SE_UNUSED_SPIN_AWAY_HIGH_B,
  /// SE_UNUSED_SPIN_AWAY_HIGH_B's rhythm on its voice, six of its eleven notes
  /// changed, at tempo 65; nothing plays it.
  SE_UNUSED_SPIN_AWAY_HIGH_C,
  /// SE_KOLIMA_BRIDGE_HOP's voice and rhythm with three of its five notes;
  /// nothing plays it.
  SE_UNUSED_KOLIMA_BRIDGE_HOP,
  /// SE_WHIRLWIND an octave lower; nothing plays it.
  SE_UNUSED_WHIRLWIND_LOW_A,
  /// SE_WHIRLWIND an octave lower, on voice 99; nothing plays it.
  SE_UNUSED_WHIRLWIND_LOW_B,
  /// SE_RISE_WITH_SPARKS's notes ending with the last one instead of held;
  /// nothing plays it.
  SE_UNUSED_RISE_WITH_SPARKS_SHORT,
  /// One long note: Piers's uncle calling the bird that carries his letter to
  /// Lunpa in Lemuria, and the Gasp battle effect's burst.
  SE_BIRD_CALL,
  /// SE_COIN_PAYOUT's second track alone, on voice 100; nothing plays it.
  SE_UNUSED_COIN_PAYOUT_C,
  /// SE_UNUSED_COIN_PAYOUT_C an octave higher and slower, on voice 118; nothing
  /// plays it.
  SE_UNUSED_COIN_PAYOUT_HIGH,
  /// SE_WADE's voice and tempo, louder and on two shorter notes, a soft 60 and
  /// then 84; nothing plays it.
  SE_UNUSED_WADE_SHORT,
  /// Played as an actor wades through water.
  SE_WADE,
  /// The Broken Seal's MUS_VILLAGE_THEME, note for note.
  MUS_VILLAGE_THEME = 700,
  /// The field music of Gondowan Settlement.
  MUS_GONDOWAN_SETTLEMENT,
  /// The Broken Seal's MUS_AN_UNSETTLING_FEELING, note for note.
  MUS_AN_UNSETTLING_FEELING,
  /// The field music of Lemuria's palace scene.
  MUS_LEMURIAN_PALACE,
  /// The Broken Seal's MUS_SINKING_INTO_SORROW, note for note.
  MUS_SINKING_INTO_SORROW,
  /// The reunion of the two parties in Contigo (Contigo_TheReunion).
  MUS_REUNITING_WITH_HAMA,
  /// The Broken Seal's MUS_PORT_TOWN_THEME, note for note.
  MUS_PORT_TOWN_THEME,
  /// The Broken Seal's MUS_TRIAL_ROAD, note for note; MUS_TRIAL_ROAD here is
  /// another tune, so this one keeps its older name.
  MUS_COLOSSO_INTRO,
  /// The Broken Seal's MUS_COLOSSEUM, note for note.
  MUS_COLOSSEUM,
  /// The field music of Lemuria's spring scene and the Link Lobby.
  MUS_TRY_YOUR_LUCK,
  /// The Broken Seal's MUS_TOLBI, note for note.
  MUS_TOLBI,
  /// The field music of the Anemos Inner Sanctum.
  MUS_ANEMOS_INNER_SANCTUM = 720,
  /// The field music of Contigo, the Anemos Sanctum and the Trial Road.
  MUS_CONTIGO,
  /// The Broken Seal's MUS_ELEMENTAL_STARS, note for note; Mt. Aleph's summit
  /// plays it.
  MUS_ELEMENTAL_STARS,
  /// The Broken Seal's MUS_DUNGEON_THEME; only the link lobby's song request
  /// plays it.
  MUS_DUNGEON_THEME,
  /// The field music of Suhalla Gate and the Yampi Desert Cave.
  MUS_SUHALLA_GATE,
  /// The field music of the Yampi Desert.
  MUS_YAMPI_DESERT,
  /// The Broken Seal's MUS_REMOTE_VILLAGE, note for note.
  MUS_REMOTE_VILLAGE,
  /// The field music of the Venus Lighthouse entrance scene, where the game
  /// opens.
  MUS_VENUS_LIGHTHOUSE_ENTRANCE,
  /// The field music of the Dehkan Plateau and Treasure Isle.
  MUS_DEHKAN_PLATEAU,
  /// The field music of the Gabomba Catacombs.
  MUS_GABOMBA_CATACOMBS,
  /// The Broken Seal's MUS_BABIS_THEME, note for note.
  MUS_BABIS_THEME,
  /// The Broken Seal's MUS_SET_SAIL_ON_THE_KARAGOL_SEA, note for note.
  MUS_SET_SAIL_ON_THE_KARAGOL_SEA = 741,
  /// The field music of Idejima, and of the World Map from one entrance.
  MUS_IDEJIMA,
  /// The field music of Venus Lighthouse.
  MUS_VENUS_LIGHTHOUSE,
  /// The battle music when Isaac is in the front row and Felix is not.
  MUS_BATTLE_THE_BROKEN_SEAL = 750,
  /// The Broken Seal's MUS_SWORN_ENEMIES_STAND_IN_YOUR_WAY, the battle music
  /// gEncounter_BattleMusic gives ENCOUNTER_TABLE_HIDDEN_BOSSES's slot 0.
  MUS_BATTLE_WITH_STAR_MAGICIAN,
  /// The Broken Seal's MUS_BATTLING_A_POWERFUL_FOE, note for note.
  MUS_BATTLE_WITH_VALUKAR,
  /// The Broken Seal's MUS_BATTLE_WITH_SATUROS_AND_MENARDI, the battle music
  /// gEncounter_BattleMusic gives ENCOUNTER_TABLE_HIDDEN_BOSSES's slot 2.
  MUS_BATTLE_WITH_SENTINEL,
  /// The Broken Seal's MUS_BATTLE_WITH_THE_FUSION_DRAGON, the battle music
  /// gEncounter_BattleMusic gives the Shadow's battle at the Anemos Inner
  /// Sanctum's entrance (ENCOUNTER_TABLE_HIDDEN_BOSSES, slot 3).
  MUS_BATTLE_WITH_DULLAHAN,
  /// The Broken Seal's MUS_TO_BE_CONTINUED, note for note, on the song
  /// table's last row; nothing in this game plays it.
  MUS_TO_BE_CONTINUED = 799
};

/// The first voice: the first blip of the animals'.
#define SE_VOICE_FIRST SE_VOICE_ANIMAL_A

#endif
