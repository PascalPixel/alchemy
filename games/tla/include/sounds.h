#ifndef ALCHEMY_SOUNDS_H
#define ALCHEMY_SOUNDS_H

/// @file
/// Sound ids, one for each row of gSongTable (sound/song_table.s) in order, as
/// The Broken Seal's: MUS_ ids are music and jingles, SE_ ids sound effects,
/// as their sequences in sound/songs are named. A row of the empty song
/// (Sound_EmptySong), which plays nothing, has no name, and the next named
/// row's number is written after it. A sequence with no name yet keeps its
/// number as its placeholder. Four rows are controls no sound plays from.
/// Sound_Play tests the controls first, then the ranges: from SOUND_SE_FIRST to
/// SOUND_SE_END on the player its gSongTable row names, from SOUND_JINGLE_FIRST
/// jingles, music otherwise (sound_driver_symbols.h).

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
  MUS_IZUMO_CELEBRATION,
  /// The field music of Izumo and the Izumo Ruins.
  MUS_IZUMO,
  MUS_008,
  /// The field music of the Kandorean Temple's first seven rooms.
  MUS_KANDOREAN_TEMPLE,
  /// The field music of Naribwe, the Kibombo Mountains and Kibombo's first
  /// three scenes.
  MUS_NARIBWE,
  /// The field music of Kibombo's fourth and fifth scenes.
  MUS_KIBOMBO,
  /// The field music of Lemuria and Ancient Lemuria.
  MUS_LEMURIA,
  /// The field music of Madra, Mikasalla and Hesperia Settlement.
  MUS_MADRA,
  /// The field music of Kalt Island, one islet and a Champa house.
  MUS_KALT_ISLAND,
  /// The field music of Garoh.
  MUS_GAROH,
  /// The field music of Yallam and a Mikasalla house.
  MUS_YALLAM,
  MUS_017,
  MUS_018,
  /// The field music of Air's Rock.
  MUS_AIRS_ROCK = 20,
  /// The field music of Aqua Rock.
  MUS_AQUA_ROCK,
  /// The field music of the Madra Catacombs, Alhafran Cave, Islet Cave and
  /// Apojii Islands Cave.
  MUS_MADRA_CATACOMBS,
  /// The field music of the Shrine of the Sea God, Shaman Village Cave and the
  /// Indra, Osenia, Atteka and Angara Caverns.
  MUS_SHRINE_OF_THE_SEA_GOD,
  /// The field music of Gaia Rock.
  MUS_GAIA_ROCK,
  /// The field music of the Gabomba Statue.
  MUS_GABOMBA_STATUE,
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
  MUS_034 = 34,
  MUS_035,
  MUS_036,
  /// The field music of Shaman Village and Loho.
  MUS_SHAMAN_VILLAGE,
  MUS_038,
  MUS_039,
  /// The title intro's music at power-on, when a save file exists, and before
  /// a new game's intro (the title's Scene_Initialize), as The Broken Seal's
  /// MUS_TITLE.
  MUS_TITLE,
  /// The field music of the Lemurian Ship and the Northern Reaches, and of the
  /// World Map once flag 0x176 is set.
  MUS_LEMURIAN_SHIP,
  /// The field music of the World Map, Osenia Cliffs, Gondowan Cliffs and
  /// Atteka Inlet.
  MUS_WORLD_MAP,
  /// MUS_WORLD_MAP's places once FLAG_REUNITED is set.
  MUS_WORLD_MAP_REUNITED,
  /// The music of the Staff Credit scene.
  MUS_STAFF_CREDIT,
  /// The battle music when Jenna is in the front row without Felix or Isaac.
  MUS_JENNA_BATTLE = 49,
  /// The battle music: the usual one, and with Felix in the front row.
  MUS_BATTLE,
  MUS_051,
  MUS_052,
  MUS_053,
  MUS_054,
  MUS_055,
  /// The battle music at sea, while the Lemurian Ship is the actor events move
  /// (ownerOverride) and it does not fly.
  MUS_SHIP_BATTLE,
  MUS_057,
  /// The battle is won.
  MUS_VICTORY,
  /// The party falls in battle.
  MUS_DEFEAT,
  MUS_060,
  MUS_061,
  /// The field music of Prox.
  MUS_PROX,
  MUS_063,
  MUS_064,
  /// The field music of the Sea of Time.
  MUS_SEA_OF_TIME,
  MUS_067 = 67,
  /// The staff credits' music (the title's entrance 8, Credits_Run), as The
  /// Broken Seal's MUS_ENDING.
  MUS_ENDING,
  MUS_070 = 70,
  MUS_071,
  /// The field music of the Mercury Lighthouse scene and one way into the Mars
  /// Lighthouse aerie.
  MUS_MERCURY_LIGHTHOUSE,
  MUS_073,
  /// The music of the Clear scene, area "(Clear data)", where data is
  /// transferred.
  MUS_CLEAR,
  /// The title held without its prompt: after the prologue (the title's
  /// entrance 3), as The Broken Seal's MUS_TITLE_AFTER_PROLOGUE, and as the
  /// logo shows after the ending (entrance 10).
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
  MUS_JOINED,
  /// A unit is revived.
  MUS_REVIVE,
  /// Treasure is found.
  MUS_TREASURE_FOUND,
  /// A unit is cured.
  MUS_CURE,
  /// The game is saved.
  MUS_SAVED,
  /// A night at an inn.
  MUS_INN,
  /// A sanctum removes a curse.
  MUS_REMOVE_CURSE,
  /// A sanctum repels evil.
  MUS_REPEL_EVIL,
  /// A unit levels up, among a battle's spoils.
  MUS_LEVEL_UP,
  /// Lucky Dice's and Contigo's smallest prize.
  MUS_JINGLE_SMALL = 91,
  /// Lucky Dice's and Contigo's middle prize.
  MUS_JINGLE_MEDIUM,
  /// Lucky Dice's, Lucky Wheels' and Contigo's largest prize.
  MUS_JINGLE_LARGE,
  MUS_095 = 95,
  SE_BELL = 100,
  SE_SHOP_PURCHASE,
  SE_SHOP_SELL,
  SE_CURSE,
  SE_104,
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
  SE_119,
  SE_120,
  SE_SCUFFLE,
  SE_122,
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
  SE_139,
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
  SE_162,
  SE_163,
  SE_SCATTER,
  /// Played as the birds land on Venus Lighthouse's entrance and fly off.
  SE_BIRD_WINGS,
  SE_166,
  SE_MAP_TRANSITION,
  /// Played three times as the Ruin Key opens the Madra Catacombs.
  SE_UNLOCK,
  SE_169,
  SE_170,
  SE_THUNDER,
  SE_THUNDER_LOUD,
  SE_NOTE_LOW,
  SE_NOTE_MID,
  SE_NOTE_HIGH,
  SE_KOLIMA_BRIDGE_HOP,
  SE_177,
  SE_178,
  SE_179,
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
  SE_191,
  SE_192,
  SE_193,
  SE_194,
  SE_195,
  SE_WISE_ONE_QUEST,
  SE_197,
  SE_KOLIMA_BRIDGE_POST,
  SE_199,
  SE_SPARKLE_FALL,
  SE_ASCEND,
  SE_FLAME_FLARE,
  SE_203,
  SE_DUST_PUFF,
  SE_205,
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
  SE_225,
  SE_LOG_PUSH,
  SE_TOPPLE,
  SE_SPARKLING_PASSAGE,
  SE_SPLASH,
  SE_230,
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
  SE_242,
  SE_243,
  SE_SANDSTORM,
  SE_245,
  SE_GLITTER,
  SE_COLOSSO_LETTERING,
  SE_248,
  SE_249,
  SE_250,
  /// Played as the Black Crystal is set on the Lemurian Ship.
  SE_BLACK_CRYSTAL_SET,
  SE_COLOSSO_WARRIOR_APPEAR,
  SE_253,
  /// The first voice blip of a message: an actor's voice is SE_VOICE_FIRST plus
  /// four times its voice, plus the letter's low two bits, as in The Broken
  /// Seal.
  SE_VOICE_FIRST = 256,
  SE_VOICE_ANIMAL_2,
  SE_VOICE_ANIMAL_3,
  SE_VOICE_ANIMAL_4,
  SE_VOICE_GIRL_1,
  SE_VOICE_GIRL_2,
  SE_VOICE_GIRL_3,
  SE_VOICE_GIRL_4,
  SE_VOICE_BOY_1,
  SE_VOICE_BOY_2,
  SE_VOICE_BOY_3,
  SE_VOICE_BOY_4,
  SE_VOICE_YOUNG_WOMAN_1,
  SE_VOICE_YOUNG_WOMAN_2,
  SE_VOICE_YOUNG_WOMAN_3,
  SE_VOICE_YOUNG_WOMAN_4,
  SE_VOICE_YOUNG_MAN_1,
  SE_VOICE_YOUNG_MAN_2,
  SE_VOICE_YOUNG_MAN_3,
  SE_VOICE_YOUNG_MAN_4,
  SE_VOICE_WOMAN_1,
  SE_VOICE_WOMAN_2,
  SE_VOICE_WOMAN_3,
  SE_VOICE_WOMAN_4,
  SE_VOICE_OLD_MAN_1,
  SE_VOICE_OLD_MAN_2,
  SE_VOICE_OLD_MAN_3,
  SE_VOICE_OLD_MAN_4,
  SE_VOICE_MAN_1,
  SE_VOICE_MAN_2,
  SE_VOICE_MAN_3,
  SE_VOICE_MAN_4,
  SE_STOP_ACTION_SE,
  SE_290 = 290,
  /// Played as an actor takes flight.
  SE_TAKE_FLIGHT,
  SE_ERUPTION_COLUMNS,
  SE_293,
  SE_STOP_MUSIC_PLAYER_5 = 298,
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
  SE_310,
  SE_311,
  /// Not a sound: fades out what MUSIC_PLAYER_ACTION_SE and MUSIC_PLAYER_SE5
  /// play.
  SOUND_FADE_OUT_ACTION_SE = 390,
  /// Not a sound: fades out what MUSIC_PLAYER_SE5 plays.
  SOUND_FADE_OUT_SE5 = 395,
  /// The field Psynergy Lash.
  SE_LASH = 512,
  SE_520 = 520,
  SE_521,
  SE_522,
  SE_523,
  SE_524,
  /// Played as Garoh's people howl at night.
  SE_HOWL,
  SE_526,
  SE_527,
  SE_528,
  SE_529,
  SE_530,
  SE_531,
  SE_532,
  SE_533,
  SE_534,
  SE_535,
  SE_536,
  SE_538 = 538,
  SE_539,
  /// The field Psynergy Parch.
  SE_PARCH = 542,
  SE_543,
  SE_544,
  SE_545,
  SE_546,
  SE_547,
  SE_548,
  SE_549,
  /// Played as the party drops through a hole on an islet.
  SE_DROP_THROUGH_HOLE,
  SE_551,
  SE_552,
  /// Played as Magma Rock's flames grow.
  SE_FLAME_GROW = 554,
  SE_555,
  SE_556,
  SE_557,
  SE_558,
  SE_559,
  SE_560,
  SE_561,
  SE_562,
  SE_563,
  SE_564,
  SE_565,
  SE_566,
  /// The field Psynergy Pound strikes, and the like blows of Magma Rock's rocks
  /// and Treasure Isle's spiky rock.
  SE_POUND,
  SE_568,
  SE_569,
  SE_570,
  SE_571,
  SE_572,
  /// Played as Mars Lighthouse's ice sheets crack.
  SE_ICE_CRACK,
  SE_574,
  SE_575,
  SE_576,
  SE_577,
  SE_578,
  SE_579,
  SE_580,
  SE_581,
  SE_582,
  SE_583,
  /// Played as the party presses a switch or button in Mars Lighthouse, the
  /// Ankohl Ruins and the Anemos Inner Sanctum.
  SE_BUTTON_PRESS,
  SE_585,
  SE_586,
  SE_587,
  SE_588,
  SE_589,
  /// Played as Gaia Rock's statue rises under the leader.
  SE_STATUE_RISE,
  SE_591,
  SE_592,
  SE_593,
  SE_594,
  SE_595,
  SE_596,
  /// Played as the ship rides the Sea of Time's current.
  SE_CURRENT,
  SE_598,
  SE_610 = 610,
  SE_611,
  /// Played as the leader spins away.
  SE_SPIN_AWAY,
  SE_613,
  SE_614,
  SE_615,
  SE_616,
  SE_617,
  SE_618,
  SE_619,
  SE_620,
  SE_621,
  SE_622,
  /// Played as an actor wades through water.
  SE_WADE,
  MUS_700 = 700,
  /// The field music of Gondowan Settlement.
  MUS_GONDOWAN_SETTLEMENT,
  MUS_702,
  /// The field music of Lemuria's palace scene.
  MUS_LEMURIA_PALACE,
  MUS_704,
  /// The reunion of the two parties in Contigo (Contigo_TheReunion).
  MUS_REUNION,
  MUS_706,
  MUS_707,
  MUS_708,
  /// The field music of Lemuria's spring scene and the Link Lobby.
  MUS_LEMURIA_SPRING,
  MUS_710,
  /// The field music of the Anemos Inner Sanctum.
  MUS_ANEMOS_INNER_SANCTUM = 720,
  /// The field music of Contigo, the Anemos Sanctum and the Trial Road.
  MUS_CONTIGO,
  MUS_722,
  MUS_723,
  /// The field music of Suhalla Gate and the Yampi Desert Cave.
  MUS_SUHALLA_GATE,
  /// The field music of the Yampi Desert.
  MUS_YAMPI_DESERT,
  MUS_726,
  /// The field music of the Venus Lighthouse entrance scene, where the game
  /// opens.
  MUS_VENUS_LIGHTHOUSE_ENTRANCE,
  /// The field music of the Dehkan Plateau and Treasure Isle.
  MUS_DEHKAN_PLATEAU,
  /// The field music of the Gabomba Catacombs.
  MUS_GABOMBA_CATACOMBS,
  MUS_730,
  MUS_741 = 741,
  /// The field music of Idejima, and of the World Map from one entrance.
  MUS_IDEJIMA,
  /// The field music of Venus Lighthouse.
  MUS_VENUS_LIGHTHOUSE,
  /// The battle music when Isaac is in the front row and Felix is not.
  MUS_ISAAC_BATTLE = 750,
  MUS_751,
  MUS_752,
  MUS_753,
  MUS_754,
  MUS_ENDING_LOGO
};

#endif
