#ifndef ALCHEMY_SONGS_H
#define ALCHEMY_SONGS_H

/// @file
/// Song and sound effect ids, indices into gSound_SongTable. MUS_ ids are
/// music and SE_ ids sound effects, as their sequences in sound/sequence are
/// named. Only the ids ☀️ plays are here: the code's own, the field music
/// gMap_MusicRules and gEncounter_BattleMusic choose by number, and
/// the voices and defeat sounds reached by adding to a first id. A sound whose
/// meaning is shown has a name; the others keep their number. Sound_PlaySong
/// tests the controls SONG_FADE_OUT_BGM and SONG_FADE_OUT_ACTION_SE first, then
/// the ranges: from SONG_SE_FIRST on a song table player, from
/// SONG_FANFARE_FIRST fanfares, below that music, where SONG_KEEP_BGM is
/// also a control (audio_engine_symbols.h).

/// Field music of Vale, of Vault, and of Altin once
/// FLAG_HYDROS_STATUES_DEFEATED is set.
#define MUS_000 0
/// Field music of Lunpa, the Lunpa Fortress Gates and the Vault jail; of
/// Bilibin, the Bilibin Barricade, Kolima, Kolima Bridge, Kolima Forest and the
/// Tret Tree once FLAG_KOLIMA_CURSE_LIFTED is set; of Tolbi and the Colosseum
/// entrance after the Colosso (FLAG_COLOSSO_OVER); and of Lalivero once
/// FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED is set. It also plays as Tret is
/// restored.
#define MUS_001 1
/// Field music of Bilibin, the Bilibin Barricade, Kolima, Kolima Bridge, the
/// Kolima tunnel, Imil and Lalivero, and of Vale and the Sanctum once Mt. Aleph
/// has erupted (FLAG_SOL_SANCTUM_ERUPTED). It also plays as the Wise One gives
/// the party its mission.
#define MUS_002 2
/// Field music of McCoy's Palace, Hammet Palace, the Colosseum's halls and
/// Vale's inn from one warp destination; the debug link test plays it too
/// (SerialTest_Run).
#define MUS_003 3
/// Field music of the Tret Tree and of Altin.
#define MUS_004 4
/// Field music of Fuchin Temple, Xian and Lama Temple.
#define MUS_005 5
/// Field music of Kalay, the Kalay docks and the Tolbi docks.
#define MUS_006 6
/// Field music at the start of each Colosso match, and as Tret is restored.
#define MUS_007 7
/// Field music of the Colosso matches, and as Donpa judges Dodonpa in the Lunpa
/// Fortress.
#define MUS_008 8
/// The music of Lucky Dice and Lucky Wheels, and of the world map during the
/// Djinn lesson (FLAG_DJINN_LESSON); it also plays as the Vault thieves are
/// chased, as the tour group leaves Kalay, and as Flint is met.
#define MUS_009 9
/// Field music of Tolbi before the Colosso; afterwards Tolbi plays MUS_001.
/// ★ Named from its area name: Tolbi
#define MUS_TOLBI 10
/// a song with no tracks: silence
#define MUS_SILENCE 19
/// Field music of Sol Sanctum until the eruption (FLAG_SOL_SANCTUM_ERUPTED).
/// ★ Named from its area name: Sol Sanctum
#define MUS_SOL_SANCTUM 20
/// Field music of Kolima Forest, Mogall Forest, and of the Tret Tree once
/// FLAG_TRET_DEFEATED is set; it also plays as Kolima is cursed, as Kraden
/// finds the heart of Sol Sanctum, and after the battle with Tret.
#define MUS_021 21
/// Field music of Mercury Lighthouse's aerie, the Elemental Star room, the
/// Lemurian ship, Venus Lighthouse's aerie once FLAG_MAPSEC_TEMP_51 is set
/// there, and the world map reached from one of its warp destinations.
#define MUS_022 22
/// Field music of Altin Peak and the Lunpa Fortress, and of Vale once the
/// stormy night's monster is fought (FLAG_VALE_STORM_MONSTER_FOUGHT); it also
/// plays as the Wise One rescues the party in Sol Sanctum and after Saturos and
/// Menardi's defeat.
#define MUS_023 23
/// Field music of the caves: Goma Cave, Bilibin Cave, Imil Falls Cave, Vale
/// Cave, Vault Cave, West Lunpa Cave, the Kalay Tunnel, Gondowan Cave and the
/// Suhalla Gate.
#define MUS_CAVE 24
/// Field music of the deserts: Lamakan Desert and Suhalla Desert.
#define MUS_DESERT 25
/// Field music of Suhalla, and of Imil once Saturos is beaten at Mercury
/// Lighthouse (FLAG_SATUROS_BEATEN_AT_MERCURY).
#define MUS_026 26
/// Field music of Mercury Lighthouse and the Venus Lighthouse entry.
#define MUS_027 27
/// Field music of Fuchin Falls Cave, Crossbone Isle's cave, Babi Lighthouse and
/// the Tunnel Ruins' rooms of lady statues; it also plays on the Tolbi-bound
/// ship as the party discusses launching without an anchor.
#define MUS_028 28
/// Field music of Altmiller Cave, the Tunnel Ruins, and the Tolbi-bound ship
/// once its monsters are sighted (FLAG_TOLBI_SHIP_MONSTERS_SIGHTED); it also
/// plays as Alex appears at Mercury Lighthouse, in the fight with Tret, on the
/// ship's mast, and as Saturos and Menardi are confronted at Venus Lighthouse.
#define MUS_029 29
/// Field music of Venus Lighthouse's aerie once FLAG_MAPSEC_TEMP_50 is set
/// there; it also plays as Babi meets the party in Altmiller Cave and after the
/// Colosso, and as Iodem questions the wounded soldiers in Suhalla.
#define MUS_030 30
/// Unused: nothing in ☀️ plays it; TRACKS_ALL names it among the songs of
/// nine or more tracks.
#define MUS_040 40
/// Field music of the Tolbi-bound ship, and of the world map reached from seven
/// of its warp destinations.
#define MUS_041 41
/// Field music of the world map, and of Goma Cave's entrance, the Bilibin Cave
/// entrance, the Alpine Crossing, Crossbone Isle, the Gondowan Passage and the
/// Kalay Tunnel entrance; it also plays after Flint is met.
#define MUS_WORLD_MAP 42
/// Field music of Venus Lighthouse, but for its entry and its aerie.
/// ★ Named from its area name: Venus Lighthouse
#define MUS_VENUS_LIGHTHOUSE 43
/// The music of every battle gEncounter_BattleMusic lists no other song for.
#define MUS_BATTLE 50
/// Battle music gEncounter_BattleMusic gives the Mystery Man and Mystery Woman
/// who catch Isaac at Vale Gate (ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, slot 4)
/// and Saturos at Mercury Lighthouse's aerie
/// (ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2); Battle_Run plays
/// it, then MUS_COLOSSO_BATTLE, for a battle with no cue of its own.
#define MUS_051 51
/// Battle music gEncounter_BattleMusic gives the guardians and bosses: Tret,
/// the Killer Ape, the Hydros Statue, the Manticore, the Kraken, Toadonpa, the
/// Storm and Tempest Lizards, and the monsters guarding Crossbone Isle's
/// floors; it also plays as Dodonpa pulls the lever that springs the trap in
/// the Lunpa Fortress.
/// ★ Named from what the player sees: the battles with Tret, the Kraken and
/// the Manticore (its render)
#define MUS_BOSS_BATTLE 52
/// Battle music gEncounter_BattleMusic gives Saturos and Menardi's first battle
/// at Venus Lighthouse's aerie (ENCOUNTER_ZONE_SCRIPTED, slot 1) and Deadbeard
/// (ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, slot 5).
#define MUS_053 53
/// Battle music of the Fusion Dragon, the aerie's second battle
/// (ENCOUNTER_ZONE_SCRIPTED, slot 0); it also plays as Saturos and Menardi
/// fuse at Venus Lighthouse's aerie.
/// ★ Named from the game: "Fusion Dragon" (MsgEnemyName)
#define MUS_FUSION_DRAGON_BATTLE 54
/// Played after a link battle's own cue (Battle_Run), and as the
/// boulder chases the party down Altin Peak.
#define MUS_055 55
/// Plays when a battle is won (Battle_Run), except during the Djinn
/// lesson (FLAG_DJINN_LESSON).
#define MUS_VICTORY 58
/// Plays when the whole party falls: defeated in battle, before "goes down in
/// defeat" or "party has been defeated", or felled on the field by poison or
/// hazardous ground.
#define MUS_DEFEAT 59
/// Plays as the party finds Hammet in the Lunpa Fortress and as Ivan is met in
/// Vault.
#define MUS_060 60
/// Plays as Saturos and Menardi appear: in Vale, at the gate to Mt. Aleph, in
/// the Elemental Star room, at Mercury Lighthouse's aerie and at Venus
/// Lighthouse; and as the party finds Hammet and frees the roofer in Vault.
#define MUS_061 61
/// Plays as Sheba hangs from the edge of Venus Lighthouse's aerie, from
/// Felix's cry until it fades as she slips
/// (VenusLighthouseAerie_TryToPreventFall).
#define MUS_SHEBA_HANGS_ON 62
/// Field music of Sol Sanctum once Mt. Aleph has erupted
/// (FLAG_SOL_SANCTUM_ERUPTED) and of the eruption scene; it also plays as the
/// Tolbi-bound ship rocks and as Saturos and Menardi combine.
#define MUS_063 63
/// The title screen's music as the intro holds the title, without its prompt,
/// after the prologue and before Vale three years later (Title_Run).
/// ★ Named from what the player sees: the title shown between the prologue and
/// Vale three years later (its render)
#define MUS_TITLE_AFTER_PROLOGUE 64
/// The ending's music: the Lemurian ship sailing away (LemurianShip_SetSail)
/// and the staff credits that follow (Title_Run).
/// ★ Named from what the player sees: the Lemurian ship sailing off and the
/// staff credits (its render)
#define MUS_ENDING 67
/// The title intro's music at power-on, when a save file exists, with its
/// "Press Start" prompt (Title_Run).
/// ★ Named from what the player sees: the title screen and its "Press Start"
/// prompt (its render)
#define MUS_TITLE 70
/// A row of the song table that holds the empty song (Sound_EmptySong), so
/// nothing plays; only the debug battle message viewer starts it, to silence
/// the music.
#define MUS_EMPTY 71
/// Plays once Felix has leapt after Sheba from Venus Lighthouse's aerie, as the
/// party watches and resolves to escape (VenusLighthouseAerie_PlayShebasFall).
#define MUS_FELIX_LEAPS_AFTER_SHEBA 72
/// Field music of the Sanctum and of the clear-data map.
#define MUS_074 74
/// The music of the Golden Sun logo shown after Jenna and Kraden's closing
/// scene on the world map, until a key is pressed (Title_Run's warp destination
/// 10). ★ Named from what the player sees: the Golden Sun logo after the
/// ending's last scene (its render)
#define MUS_ENDING_LOGO 75
/// Battle music gEncounter_BattleMusic gives the battles that end the Colosso
/// finals' matches (ENCOUNTER_ZONE_COLOSSO); Battle_Run plays it after
/// MUS_051 for a battle with no cue of its own.
/// ★ Named from what the player sees: the battle that ends a Colosso match
/// (its render)
#define MUS_COLOSSO_BATTLE 76
/// The fanfare as a puzzle is solved: Altmiller Cave's rock turned, the
/// Lemurian ship raised, the Vault Cave switches, and the West Lunpa Cave gate
/// frozen open.
#define MUS_PUZZLE_SOLVED 80
/// The fanfare as a Djinni or a new member joins the party, Flint included.
#define MUS_JOINED 81
/// The fanfare of Revive, used from the menu or bought at a Sanctum.
#define MUS_REVIVE 82
/// The fanfare as an item is found or given, Psynergy learned, a Colosso prize
/// taken, the Hermes' Water bottle filled, or an Elemental Star taken.
#define MUS_TREASURE_FOUND 83
/// The fanfare of Cure Poison, used from the menu or bought at a Sanctum, and
/// of Ply's field effect on its target.
/// ★ Named from what the player sees: Cure Poison and Ply curing their target
/// (its render)
#define MUS_CURE 84
/// The fanfare as a save is written, or a link battle's results are saved.
#define MUS_SAVED 85
/// The fanfare of a lost Colosso match, of the inn as the party sleeps, and in
/// the Sanctum as the party is asked to take on the quest.
#define MUS_086 86
/// The fanfare of a Sanctum's Remove Curse.
#define MUS_REMOVE_CURSE 87
/// The fanfare of a Sanctum's Exorcise.
#define MUS_EXORCISE 88
/// The fanfare as a member levels up after a battle, and as a Colosso round
/// begins.
#define MUS_LEVEL_UP 89
/// The smallest prize fanfare: two pairs in Lucky Dice, a Tolbi Spring prize
/// under 5,000 coins, and one level from the debug level editor; also an item
/// or Psynergy used from the menu whose effect has no fanfare of its own, such
/// as the stat items (Ability_PlayUseAnimation).
/// ★ Named from what the player sees: the least of the three prizes in Lucky
/// Dice and Tolbi Spring (its render)
#define MUS_FANFARE_SMALL 91
/// The middle prize fanfare: three pairs in Lucky Dice, a Tolbi Spring prize of
/// 5,000 coins or more, and a cleared Colosseum trial.
/// ★ Named from what the player sees: the middle of the three prizes in Lucky
/// Dice and Tolbi Spring (its render)
#define MUS_FANFARE_MEDIUM 92
/// The largest prize fanfare: four of a kind in Lucky Dice, a Lucky Wheels win,
/// a Tolbi Spring prize over 19,999 coins, and five levels from the debug level
/// editor.
/// ★ Named from what the player sees: the greatest of the three prizes in Lucky
/// Dice and Tolbi Spring, and a Lucky Wheels win (its render)
#define MUS_FANFARE_LARGE 93
/// The Vault bell rung, and three times as a shop repairs an item.
#define SE_100 100
/// An item bought in a shop, artifacts included.
#define SE_SHOP_PURCHASE 101
/// An item sold to a shop (Shop_SellItem).
/// ★ Named from what the player sees: an item sold at a shop's counter (its
/// render)
#define SE_SHOP_SELL 102
/// The curse sound: a cursed item equipped from the menu or in a shop; it also
/// plays as Hydros is beaten on Altin Peak, and in the lobbed-shell battle
/// effects.
#define SE_103 103
/// Plays at Kolima's bridge, at Kolima Forest's barrier, as the villagers try
/// to save Felix on the stormy night, and as objects fall in Vault Cave.
#define SE_106 106
/// Plays as the Wise One holds Sol Sanctum's breaking floor together, during
/// the boulder chase on Altin Peak, as a Colosseum trial is cleared, as
/// Psynergy is shown at the gate to Mt. Aleph, and as Hammet's caravan crosses
/// the world map.
#define SE_107 107
/// The low triple tone: Halt freezing an object event, and the link session
/// countdown.
#define SE_TRIPLE_TONE_LOW 108
/// The high triple tone: Halt ending.
#define SE_TRIPLE_TONE_HIGH 109
/// Plays on the title splash screen, as a code is entered on the file select
/// screen, as the field view finds a target, as Altmiller Cave's lights appear,
/// and as a battle command is chosen.
#define SE_110 110
/// The cursor moving in a menu or list.
#define SE_MENU_CURSOR_MOVE 111
/// A menu choice confirmed.
#define SE_MENU_CONFIRM 112
/// A menu closed or a choice cancelled.
#define SE_MENU_CANCEL 113
/// A choice that can't be made: not enough PP, no target, a pushed or lifted
/// thing that won't move.
#define SE_MENU_ERROR 114
/// Ice forming: Frost on the field, and the Ice and Frost battle effects.
/// ★ Named from what the player sees: the ice of Frost and of Ice, Ice Horn and
/// Ice Missile (its render)
#define SE_ICE 115
/// Tolbi Spring: the Toss prompt appearing, before each toss.
/// ★ Named from the game: "Toss" (MsgTolbiToss)
#define SE_TOLBI_SPRING_TOSS_PROMPT 116
/// The Remove command in the item menu (ItemMenu_ChooseCommand).
/// ★ Named from what the player sees: the item menu's Remove command (its
/// render)
#define SE_ITEM_REMOVE 117
/// Footsteps at the Mercury Lighthouse entrance, and the leader spraying sand
/// in Lamakan Desert.
#define SE_118 118
/// The guards throwing the leader out of Lunpa.
#define SE_SCUFFLE 121
/// Leaving a map through an ordinary warp.
#define SE_MAP_EXIT 123
/// A chest opening; also a Psynergy target reacting, the Fuchin Temple
/// students' lessons, the Vault mayor's urn, and the training tree in Xian.
#define SE_124 124
/// A chest closing again when its item can't be carried; also a switch flipped
/// on Altin Peak, the boulder lifted on the Alpine Crossing, Hammet's return to
/// Kalay, and McCoy's reward.
#define SE_125 125
/// HP recovered: healing in battle and from the menu, a Djinni's heal, a hot
/// spring or well searched, Growth on the field, the Psynergy Stone, and the
/// Sanctum's services.
#define SE_RECOVERY 126
/// A heavy landing: the leader thrown out of Lunpa, or hopping down a step in
/// the Tunnel Ruins.
#define SE_LANDING_THUD 127
/// Climbing stairs: an WARP_STYLE_STAIRS_UP warp, and the stairs up in Vale's
/// inn and Isaac's house.
#define SE_STAIRS_UP 128
/// Going down stairs: an WARP_STYLE_STAIRS_DOWN warp, and the stairs
/// down in Vale's inn and Isaac's house.
#define SE_STAIRS_DOWN 129
/// Field Psynergy beginning: Carry, Cloak, Force, Lift, Mind Read and Ply, and
/// a Psynergy shortcut set in the menu.
#define SE_130 130
/// Psynergy taking hold on the field: Carry, Growth, Move, Ply, Retreat and
/// Reveal; also the Thor summon and Mia's Ply at Mercury Lighthouse.
#define SE_131 131
/// Water: Catch and Douse on the field, the Fuchin Temple grotto, Xian's
/// student, and the Boreas, Ice, Douse, Frost, Thorn, Spire and Froth battle
/// effects.
#define SE_132 132
/// A blow landing: an enemy class's ordinary hit, field damage of 10 or less,
/// and poison on the field; the lighter twin of SE_HEAVY_IMPACT, heard in most
/// attacking Psynergy.
#define SE_IMPACT 133
/// A heavy blow landing: damage over 10 on the field, the heavy-hitting
/// classes' blows, and the summons and strongest Psynergy.
#define SE_HEAVY_IMPACT 134
/// A click: the leader's step blocked, a lifted thing steered, and each fifth
/// of a turn of Altmiller Cave's spinning lights.
#define SE_135 135
/// Plays as a Djinni hops into battle, with Whirlwind on the field, as pillars
/// move on Crossbone Isle, during the spinning leap in Gondowan Cave, and in
/// the Fire, Flare, Frost, Mars, Cybele, Kirin and other battle effects.
#define SE_136 136
/// Plays in the Kirin summon only, 46 frames after its shake begins.
/// ★ Named from what the player sees: the Kirin summon (its render)
#define SE_KIRIN 137
/// An item breaking, and a lifted, pushed or caught thing shattering; also the
/// Bolt and Ray battle effects, rumbling at Sol Sanctum's entrance, and Venus
/// Lighthouse's aerie.
#define SE_ITEM_BREAK 138
/// A Djinni set in the Djinn menu, and a Djinni recovering to Set as the party
/// walks (Field_ProcessStep).
/// ★ Named from what the player sees: a Djinni returning to Set (its render)
#define SE_DJINNI_SET 139
/// Plays with Avoid on the field, as Jupiter and Mars Djinn join, as a Venus
/// Djinni is caught, as the Lemurian ship rises, and in the Thor and Procne
/// summons.
#define SE_140 140
/// A rumble, mostly as the screen shakes (MapGrid_SetShake): Sol Sanctum's
/// rooms after the eruption, the stumps in Goma Cave, the Lunpa Fortress
/// trap, Mercury Lighthouse's waterfall passage, the boulders of the stormy
/// night, the wagons setting off from Kalay and Hammet's caravan, and the
/// Quake, Gaia, Judgment and other battle effects.
#define SE_RUMBLE 141
/// Plays in the Bind, Drain, Haunt, Sleep, Judgment and Neptune battle effects
/// and as stats rise, and as a statue is searched in Venus Lighthouse.
#define SE_142 142
/// Plays in the Break, Vine, Spider Web and Curse battle effects and as stats
/// fall, as a Venus Djinni's sparks land, and as Vale's roof is mended.
#define SE_143 143
/// A clatter: the rocks of Altin Peak's boulder, the crates moved in the Lunpa
/// Fortress, the Elemental Stars bagged, and the Eruption, Nereid and Prism
/// battle effects.
#define SE_144 144
/// A clatter with SE_144 as Altin Peak's boulder breaks; also the Lemurian
/// ship's flash, the Lunpa Fortress trap, the Mars Star taken, Mt. Aleph's
/// eruption, and the Boreas, Gaia, Judgment, Ragnarok, Thor and other summons.
#define SE_145 145
/// An enemy falling, for defeat-sound kind 0: each kind's sound is SE_146 plus
/// it (BattleSprite_GetDefeatSound). A mimic chest leaping up and the
/// monsters on the Tolbi-bound ship use it too.
#define SE_146 146
/// An enemy falling, for defeat-sound kind 1; also a battle starting on the
/// field, a beast scared off in Altin, the apes in Mogall Forest, and monsters
/// attacking the Tolbi-bound ship.
#define SE_147 147
/// An enemy falling, for defeat-sound kind 2; also the boulder and Hydros on
/// Altin Peak, the Manticore in Lamakan Desert, the Killer Ape in Mogall
/// Forest, and Tolbi Spring's prize.
#define SE_148 148
/// An enemy falling, for defeat-sound kind 3; also the leap into the Kraken
/// battle on the Tolbi-bound ship.
#define SE_149 149
/// An enemy falling, for defeat-sound kind 4.
#define SE_150 150
/// Altmiller Cave's lights growing in, one by one, and a Venus Djinni caught.
#define SE_151 151
/// An object event jumps: ObjectMotion_Launch plays the high jump above 5
/// pixels a frame.
#define SE_JUMP 152
/// A high jump: ObjectMotion_Launch above 5 pixels a frame, Djinn leaping, and
/// the leaps on Altin Peak, at the Kalay docks and in the Elemental Star room.
#define SE_HIGH_JUMP 153
/// The sound of a cast, in battle and on the field.
#define SE_CAST 154
/// A switch: Babi Lighthouse's truth door switch, Vault Cave's switch, Hammet
/// found in the Lunpa Fortress, and the passengers at the Kalay docks.
#define SE_155 155
/// Vault Cave's switch flipped, and the Cybele, Judgment and Ramses battle
/// effects.
#define SE_156 156
/// A gate or door sliding: Babi Lighthouse's truth door, Crossbone Isle's red
/// and blue doors, the Lunpa Fortress doors, West Lunpa Cave's gates, and the
/// Cybele and Judgment battle effects.
#define SE_GATE_MOVE 157
/// A door opening as the leader goes through.
#define SE_DOOR_OPEN 158
/// A door shutting in Lalivero; also the Fuchin Temple, the Kalay docks, Altin
/// Peak's cart, and the lady statues moved in the Tunnel Ruins.
#define SE_159 159
/// Plays as Vale's roof is mended, as Levitation is shown at the gate to Mt.
/// Aleph, as Felix is unmasked, at Venus Lighthouse's switches, as Tret rejects
/// the Hermes' Water, and as Saturos and Menardi combine.
#define SE_161 161
/// Venus Lighthouse's tile waves, and the Mercury, Judgment and Neptune battle
/// effects.
#define SE_162 162
/// Whirlwind on the field, the Lemurian ship rising, and the rumble in Vault
/// Cave.
#define SE_163 163
/// Jupiter Djinn joining, and the Neptune, Nereid, Titan Blade and Froth battle
/// effects.
#define SE_164 164
/// A map transition effect.
#define SE_MAP_TRANSITION 167
/// The ambient sound a map queues in Lama Temple, Xian and the Manticore's
/// part of Lamakan Desert (Map_QueueMusic).
#define SE_169 169
/// The ambient sound a map queues (Map_QueueMusic) with
/// the ground particles and Altin's water-spitting beasts, and in Fuchin
/// Temple, Vale, Kalay's tunnel, Mercury and Venus Lighthouse and the Goma Cave
/// entrance.
#define SE_170 170
/// Thunder: lightning on the field (Weather_UpdateLightning), the flash in
/// Sol Sanctum's Luna room as the scene opens on Kraden, the flash of a Lucky
/// Wheels win, and the Jupiter battle effect.
/// ★ Named from what the player sees: lightning flashing over the stormy night
/// (its render)
#define SE_THUNDER 171
/// The louder thunderclap lightning on the field plays in place of SE_THUNDER
/// (struct LightningWork's loud).
/// ★ Named from what the player sees: lightning flashing over the stormy night
/// (its render)
#define SE_THUNDER_LOUD 172
/// The choice sound of the item and Psynergy menus; also a Djinni beginning to
/// join or be caught.
#define SE_173 173
/// The Use command in the item and Psynergy menus; also Avoid on the field, and
/// Jupiter and Mars Djinn joining.
#define SE_174 174
/// A Djinni returning to Set in battle, HP or PP recovered or elemental power
/// raised at a turn's end, a Djinni put on standby in the Djinn menu, and an
/// item given by the debug item browser.
#define SE_175 175
/// The villager at Kolima's post jumping to it, as the drawbridge is lowered
/// (Kolima_LowerBridge).
#define SE_176 176
/// The Tunnel Ruins' balance settling, and Venus Lighthouse's floating pillars
/// lowering.
#define SE_180 180
/// Sol Sanctum's door opening, and the Kraken shaking the Tolbi-bound ship.
#define SE_181 181
/// A gem set into its socket as a way opens: the Dragon's Eye in Fuchin Falls
/// Cave's dragon and the Small Jewels at Sol Sanctum's entrance.
/// ★ Named from what the player sees: the leader setting the small gem into
/// the empty socket, and the dragon lit by its eye (its render)
#define SE_GEM_SET 182
/// A rumble: Altin Peak's statue waking and the boulder chase, Crossbone Isle's
/// doors opening, and the jewels set at Sol Sanctum's entrance.
#define SE_183 183
/// Altin Peak's cart turning or moving forward.
#define SE_184 184
/// The leader pushing what stands ahead a cell on: a statue, pillar, crate or
/// torch walked into in Bilibin, Bilibin Cave, Goma Cave, Kalay, Sol Sanctum,
/// the Tunnel Ruins, Venus Lighthouse and Altin Peak, and the maps' staged
/// pushes (PushedObject_AdvancePair).
/// ★ Named from what the player sees: the leader pushing a statue or pillar
/// ahead (its render)
#define SE_PUSH 185
/// Sol Sanctum lighting up as its statues are placed: a seal opening
/// (SolSanctumStatue_OpenSeal), and the left and right beams shining on the
/// centre statue.
#define SE_SOL_SANCTUM_LIGHT_UP 186
/// Sol Sanctum's falling lights, and Venus Lighthouse's aerie flashing white as
/// the beacon rises.
#define SE_187 187
/// A door sliding open: Lama Temple, the Sanctums of Altin and Bilibin, Vault,
/// Babi Lighthouse and Goma Cave; also the pursuers dodged on Crossbone Isle.
#define SE_188 188
/// Stone moving: Crossbone Isle's pillars, a stump pushed in Goma Cave, Mercury
/// Lighthouse's pillars, and Sol Sanctum's centre statue.
#define SE_189 189
/// Saturos and Alex warping at Mercury Lighthouse's aerie and in the Elemental
/// Star room, the party rising to that room, Hydros beaten, and Altmiller
/// Cave's rock turned.
#define SE_190 190
/// The Wise One's quest asked in the Sanctum.
#define SE_196 196
/// The villager reaching Kolima's post, after SE_176, before the drawbridge
/// comes down.
#define SE_198 198
/// The curse's sparkles at Kolima and Kolima Forest, Tret rejecting the party,
/// and Sol Sanctum's statue opening its central hole.
#define SE_200 200
/// The party rising to the Elemental Star room, and an object event raised and
/// shrunk in the eruption.
#define SE_201 201
/// The flame flaring up and rising away as smoke, on the stormy night.
#define SE_202 202
/// Landing in dust at Babi Lighthouse, the Mercury Lighthouse entrance and
/// Venus Lighthouse; the Tret Tree's cocoons and rooms; and the water drops
/// striking Vault Cave's pillars.
#define SE_204 204
/// The curse falling on Kolima and Kolima Forest, and a cocoon stepped on in
/// the Tret Tree.
#define SE_206 206
/// Bane's second escape on Crossbone Isle.
#define SE_208 208
/// Mercury Lighthouse's door opening, as the light turns white after Mia casts
/// Ply on it.
#define SE_MERCURY_LIGHTHOUSE_DOOR_OPEN 209
/// A hidden way opening: Frost on Bilibin Cave's puddles, and Lunpa's hidden
/// switch.
#define SE_HIDDEN_PASSAGE_OPEN 210
/// Mercury Lighthouse's stone doors, fountain and stair pillars moving.
#define SE_211 211
/// Plays as a Djinni returns to Set or elemental power rises in battle, as
/// field Psynergy is cast where it shows, with Frost on its target, and in the
/// Annihilation, Atalanta, Gaia, Procne, Ragnarok, Titan Blade and other battle
/// effects.
#define SE_212 212
/// A pushed log or pillar coming to rest, after SE_STOP_SLOT3 cuts
/// SE_HEAVY_MOVE: the Colosso matches, Kolima Forest's floodgate, Vale Cave,
/// and the maps' staged moves (ObjectEvent_MoveAndRedraw).
/// ★ Named from what the player sees: a pushed log or pillar stopping (its
/// render)
#define SE_HEAVY_STOP 213
/// The elemental arcs rising in the eruption, and the Tolbi-bound ship casting
/// off.
#define SE_214 214
/// Hopping across Mercury Lighthouse's pool, and the crates sinking in the
/// Lunpa Fortress.
#define SE_215 215
/// Mercury Lighthouse's randomized bursts, Tolbi Spring's rising coins, and the
/// crates dropping into holes in Vale Cave.
#define SE_216 216
/// A floor switch: Mercury Lighthouse's switches and gates, the Colosso's
/// moving walkway, and an object event jumping down a ledge.
#define SE_217 217
/// Mercury Lighthouse's fountain, and the storm carrying the party off in
/// Suhalla Desert and dropping it on the world map.
#define SE_219 219
/// Frost on its target, Retreat spinning, Bilibin Cave's puddles, Fuchin
/// Temple's Reveal lesson, Mia in Imil and at Mercury Lighthouse, the curse on
/// Kolima, and the Elemental Stars taken by Alex.
#define SE_220 220
/// An object event raised with sparks at Mercury Lighthouse.
#define SE_221 221
/// The lifts of Mercury Lighthouse and its aerie, as they carry the party.
/// ★ Named from what the player sees: the party riding Mercury Lighthouse's
/// lifts (its render)
#define SE_LIFT 223
/// A pusher and its log moving together: the Colosso, Crossbone Isle and
/// Donpa's judgment in the Lunpa Fortress.
#define SE_226 226
/// A tree toppling in Fuchin Temple, and the obstacle Force knocks down over
/// the Colosso's Board Walk.
/// ★ Named from what the player sees: a tree or obstacle falling over (its
/// render)
#define SE_TOPPLE 227
/// A sparkling passage: arriving and leaving in Mercury Lighthouse, Venus
/// Lighthouse's sparkling passage, and Xian's water carrier.
#define SE_228 228
/// A splash as Altin's water beasts hop and land, in Altin and on Altin Peak.
/// ★ Named from what the player sees: the water beasts landing in the water
/// (its render)
#define SE_SPLASH 229
/// Water draining at Altin Peak, a water switch flipped at Crossbone Isle and
/// Kolima Forest's floodgate, and Kalay's tunnel.
#define SE_230 230
/// The water level changing at Crossbone Isle and at Kolima Forest's floodgate.
/// ★ Named from what the player sees: the water rising and falling (its
/// render)
#define SE_WATER_LEVEL 231
/// The water rising or draining at Crossbone Isle and Kolima Forest's
/// floodgate, and Altin Peak's cart stopping.
#define SE_232 232
/// Altin Peak's cart rolling, on player 3: as it sets off, after each turn and
/// as a countdown runs out; SE_STOP_SLOT3 cuts it.
#define SE_CART_ROLL 233
/// Kyle and Dora turning back on the stormy night.
#define SE_234 234
/// Each of the countdown's 3, 2 and 1 before a Colosso round, before
/// SE_COLOSSO_GO.
/// ★ Named from what the player sees: the Colosso's 3, 2, 1 lettering (its
/// render)
#define SE_COLOSSO_COUNTDOWN 236
/// The Go! that starts a Colosso round, after SE_COLOSSO_COUNTDOWN's three.
/// ★ Named from what the player sees: the Colosso's Go! lettering (its render)
#define SE_COLOSSO_GO 237
/// A thing pushed one cell: the Colosso matches and Vault Cave's pillar.
#define SE_238 238
/// A pushed log, pillar or wall moving, on player 3 until SE_STOP_SLOT3 cuts
/// it: the Colosso, Kolima Forest's floodgate, Vale Cave, Venus Lighthouse's
/// sliding walls and the maps' staged moves; SE_HEAVY_STOP follows.
/// ★ Named from what the player sees: a log rolling or a pillar or wall
/// sliding (its render)
#define SE_HEAVY_MOVE 239
/// A log pushed over: Kolima Forest, Mogall Forest, Vale Cave, the Colosso,
/// Crossbone Isle, and monsters attacking the Tolbi-bound ship.
#define SE_240 240
/// Something moving in water, on player 3 until SONG_FADE_OUT_ACTION_SE fades
/// it: the logs floated across Fuchin Falls Cave, the riding log in Vale Cave,
/// the leader sinking in Mercury Lighthouse's pool and bathing in a Lamakan
/// Desert oasis.
/// ★ Named from what the player sees: logs floating and the leader in water
/// (its render)
#define SE_WATER_MOVE 241
/// Suhalla Desert's sandstorms rising, and the sandstorm catching the party on
/// the world map.
/// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
#define SE_SANDSTORM 244
/// A spring searched, Retreat, the curse dust at Kolima, Tret restored, Mercury
/// Lighthouse's Ply, Fuchin Temple's Reveal lesson, and the eruption in Sol
/// Sanctum.
#define SE_246 246
/// A Colosso match's round and result, and a Colosseum trial failed or cleared.
#define SE_247 247
/// A Colosso warrior fading in (Colosso_FadeInWarrior).
/// ★ Named from what the player sees: a Colosso warrior appearing (its render)
#define SE_COLOSSO_WARRIOR_APPEAR 252
/// The first voice blip of a message: an object event's voice is 0x100 plus its
/// sprite's value (Speaker_GetVoice), and every few letters typed play
/// it plus the letter code's low two bits (TextPrinter_Step).
#define SE_256 256
#define SE_257 257 ///< voice blip
#define SE_258 258 ///< voice blip
#define SE_259 259 ///< voice blip
#define SE_260 260 ///< voice blip
#define SE_261 261 ///< voice blip
#define SE_262 262 ///< voice blip
#define SE_263 263 ///< voice blip
#define SE_264 264 ///< voice blip
#define SE_265 265 ///< voice blip
#define SE_266 266 ///< voice blip
#define SE_267 267 ///< voice blip
#define SE_268 268 ///< voice blip
#define SE_269 269 ///< voice blip
#define SE_270 270 ///< voice blip
#define SE_271 271 ///< voice blip
#define SE_272 272 ///< voice blip
#define SE_273 273 ///< voice blip
#define SE_274 274 ///< voice blip
#define SE_275 275 ///< voice blip
#define SE_276 276 ///< voice blip
#define SE_277 277 ///< voice blip
#define SE_278 278 ///< voice blip
#define SE_279 279 ///< voice blip
#define SE_280 280 ///< voice blip
#define SE_281 281 ///< voice blip
#define SE_282 282 ///< voice blip
#define SE_283 283 ///< voice blip
#define SE_284 284 ///< voice blip
#define SE_285 285 ///< voice blip
#define SE_286 286 ///< voice blip
#define SE_287 287 ///< voice blip
/// A song with no tracks on player 3, which stops the loop playing there: every
/// map change (unless FLAG_SKIP_MAP_CHANGE_SOUND), a rolling log or cart
/// stopping, a battle starting on the field.
#define SE_STOP_SLOT3 288
/// An effect pair spawned in the eruption at Sol Sanctum.
#define SE_292 292
/// A song with no tracks on player 5, which stops what plays there; played
/// before each queued map sound (Map_QueueMusic).
#define SE_STOP_SLOT5 298
/// Tolbi Spring: 50 frames into a toss.
#define SE_300 300
/// Tolbi Spring: a tossed coin meeting one of the moving obstacles.
/// ★ Named from what the player sees: the coin striking an obstacle in the
/// spring (its render)
#define SE_TOLBI_SPRING_OBSTACLE_HIT 301
/// Lucky Dice: the dice thrown with A.
/// ★ Named from what the player sees: the Lucky Dice thrown (its render)
#define SE_LUCKY_DICE_THROW 302
/// Lucky Dice: each of the four numbers revealed, five frames apart.
/// ★ Named from what the player sees: the Lucky Dice numbers shown (its
/// render)
#define SE_LUCKY_DICE_NUMBER 303
/// Lucky Wheels: the reels starting to spin.
/// ★ Named from what the player sees: the Lucky Wheels spinning (its render)
#define SE_LUCKY_WHEELS_SPIN 304
/// Lucky Wheels: a reel held or released.
/// ★ Named from what the player sees: a Lucky Wheels reel held (its render)
#define SE_LUCKY_WHEELS_HOLD 305
/// Lucky Wheels: 16 frames after the reels start, as the stop prompt shows.
#define SE_306 306
/// Lucky Wheels: a reel stopped.
/// ★ Named from what the player sees: a Lucky Wheels reel stopping (its
/// render)
#define SE_LUCKY_WHEELS_STOP 307
/// Plays on player 3 as the Wise One appears in the Sanctum, as the eruption's
/// materialization tint fades in, and as the leader rises on Venus Lighthouse's
/// platforms.
#define SE_308 308
/// The Venus Star bouncing three times as Saturos throws it into the beacon at
/// Venus Lighthouse's aerie.
/// ★ Named from what the player sees: the Venus Star bouncing on the aerie
/// (its render)
#define SE_STAR_BOUNCE 309

#endif
