#ifndef ALCHEMY_SONGS_H
#define ALCHEMY_SONGS_H

/// @file
/// Song and sound effect ids, indices into gSound_SongTable. MUS_ ids are
/// music and SE_ ids sound effects, as their sequences in sound/sequence are
/// named. Only the ids ☀️ plays are here: the code's own, the field music
/// gScene_InteractionRuleTable and gEncounter_BattleMusic choose by number, and
/// the voices and defeat sounds reached by adding to a first id. A sound whose
/// meaning is shown has a name; the others keep their number. AudioCommand_Play
/// tests the controls AUDIO_CUE_FADE_BGM and AUDIO_CUE_FADE_SLOT3 first, then
/// the ranges: from AUDIO_SONG_FIRST on a song table player, from
/// AUDIO_JINGLE_FIRST fanfares, below that music, where AUDIO_CUE_KEEP_BGM is
/// also a control (audio_engine_symbols.h).

/// Field music of Vale, of Vault, and of Altin once
/// FLAG_HYDROS_STATUES_DEFEATED is set.
#define MUS_000 0
/// Field music of Lunpa, the Lunpa Fortress Gates and the Vault jail; of
/// Bilibin, the Bilibin Barricade, Kolima, Kolima Bridge, Kolima Forest and the
/// Tret Tree once FLAG_KOLIMA_CURSE_LIFTED is set; of Tolbi and the Colosseum
/// entrance after the Colosso (FLAG_COLOSSO_OVER); and of Lalivero once
/// FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE is set. It also plays as Tret is
/// restored.
#define MUS_001 1
/// Field music of Bilibin, the Bilibin Barricade, Kolima, Kolima Bridge, the
/// Kolima tunnel, Imil and Lalivero, and of Vale and the Sanctum once Mt. Aleph
/// has erupted (FLAG_SOL_SANCTUM_ERUPTED). It also plays as the Wise One gives
/// the party its mission.
#define MUS_002 2
/// Field music of McCoy's Palace, Hammet Palace, the Colosseum's halls and
/// Vale's inn from one entrance; the debug link test plays it too
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
/// Field music of Tolbi before the Colosso.
#define MUS_010 10
/// not a song: fades the music out (AUDIO_CUE_FADE_BGM)
#define MUS_017 17
/// not a song: keeps the music (AUDIO_CUE_KEEP_BGM)
#define MUS_018 18
/// a song with no tracks: silence
#define MUS_SILENCE 19
/// Field music of Sol Sanctum.
#define MUS_020 20
/// Field music of Kolima Forest, Mogall Forest, and of the Tret Tree once
/// FLAG_TRET_DEFEATED is set; it also plays as Kolima is cursed, as Kraden
/// finds the heart of Sol Sanctum, and after the battle with Tret.
#define MUS_021 21
/// Field music of Mercury Lighthouse's aerie, the Elemental Star room, the
/// Lemurian ship, Venus Lighthouse's aerie once FLAG_PLACE_TEMP_51 is set
/// there, and the world map reached from one of its entrances.
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
/// Field music of Venus Lighthouse's aerie once FLAG_PLACE_TEMP_50 is set
/// there; it also plays as Babi meets the party in Altmiller Cave and after the
/// Colosso, and as Iodem questions the wounded soldiers in Suhalla.
#define MUS_030 30
/// Nothing in ☀️ plays it; it is kept for MUSIC_ALL_TRACKS, which names it among
/// the songs of nine or more tracks.
#define MUS_040 40
/// Field music of the Tolbi-bound ship, and of the world map reached from seven
/// of its entrances.
#define MUS_041 41
/// Field music of the world map, and of Goma Cave's entrance, the Bilibin Cave
/// entrance, the Alpine Crossing, Crossbone Isle, the Gondowan Passage and the
/// Kalay Tunnel entrance; it also plays after Flint is met.
#define MUS_WORLD_MAP 42
/// Field music of Venus Lighthouse.
#define MUS_043 43
/// The music of every battle gEncounter_BattleMusic lists no other song for.
#define MUS_BATTLE 50
/// Battle music gEncounter_BattleMusic gives some encounters;
/// Battle_RunEncounter plays it, then MUS_076, for a battle with no cue of its
/// own.
#define MUS_051 51
/// Battle music gEncounter_BattleMusic gives some encounters; it also plays as
/// the party finds Hammet in the Lunpa Fortress.
#define MUS_052 52
/// Battle music gEncounter_BattleMusic gives some encounters.
#define MUS_053 53
/// Battle music gEncounter_BattleMusic gives some encounters; it also plays as
/// Saturos and Menardi combine at Venus Lighthouse's aerie.
#define MUS_054 54
/// Played after a link battle's own cue (Battle_RunEncounter), and as the
/// boulder chases the party down Altin Peak.
#define MUS_055 55
/// Plays when a battle is won (Battle_RunEncounter), except during the Djinn
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
/// Plays as the party tries to stop Sheba's fall at Venus Lighthouse's aerie.
#define MUS_062 62
/// Field music of Sol Sanctum once Mt. Aleph has erupted
/// (FLAG_SOL_SANCTUM_ERUPTED) and of the eruption scene; it also plays as the
/// Tolbi-bound ship rocks and as Saturos and Menardi combine.
#define MUS_063 63
/// The title screen's music when the intro holds it without its prompt
/// (Title_Run).
#define MUS_064 64
/// The ending's music: the staff credits, and the Lemurian ship sailing away.
#define MUS_067 67
/// The title intro's music when a save file exists (Title_Run).
#define MUS_070 70
/// The empty song: the debug battle message viewer starts it, so nothing plays.
#define MUS_071 71
/// Plays as Sheba falls from Venus Lighthouse's aerie.
#define MUS_072 72
/// Field music of the Sanctum and of the clear-data scene.
#define MUS_074 74
/// The title screen's music after the title intro (Title_Run).
#define MUS_075 75
/// Battle music gEncounter_BattleMusic gives some encounters;
/// Battle_RunEncounter plays it after MUS_051 for a battle with no cue of its
/// own.
#define MUS_076 76
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
/// The fanfare of Cure Poison, used from the menu or bought at a Sanctum; Ply's
/// field effect plays it too.
#define MUS_084 84
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
/// under 5,000 coins, and one level from the debug level editor.
#define MUS_091 91
/// The middle prize fanfare: three pairs in Lucky Dice, a Tolbi Spring prize of
/// 5,000 coins or more, and a cleared Colosseum trial.
#define MUS_092 92
/// The largest prize fanfare: four of a kind in Lucky Dice, a Lucky Wheels win,
/// a Tolbi Spring prize over 19,999 coins, and five levels from the debug level
/// editor.
#define MUS_093 93
/// The Vault bell rung, and three times as a shop repairs an item.
#define SE_100 100
/// An item bought in a shop, artifacts included.
#define SE_SHOP_PURCHASE 101
/// An item sold to a shop.
#define SE_102 102
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
/// The low triple tone: Halt freezing an actor, and the link session countdown.
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
#define SE_115 115
/// Tolbi Spring: the toss prompt appearing.
#define SE_116 116
/// The Remove command in the item menu.
#define SE_117 117
/// Footsteps at the Mercury Lighthouse entrance, and the leader spraying sand
/// in Lamakan Desert.
#define SE_118 118
/// The guards throwing the leader out of Lunpa.
#define SE_SCUFFLE 121
/// Leaving a scene through an ordinary exit.
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
/// Climbing stairs: an EXIT_STYLE_ANIMATED exit, and the stairs up in Vale's
/// inn and Isaac's house.
#define SE_STAIRS_UP 128
/// Going down stairs: an EXIT_STYLE_ANIMATED_ALTERNATE exit, and the stairs
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
/// The Kirin summon.
#define SE_137 137
/// An item breaking, and a lifted, pushed or caught thing shattering; also the
/// Bolt and Ray battle effects, rumbling at Sol Sanctum's entrance, and Venus
/// Lighthouse's aerie.
#define SE_ITEM_BREAK 138
/// A Djinni set in the Djinn menu, and a Djinni recovering to Set as the party
/// walks.
#define SE_139 139
/// Plays with Avoid on the field, as Jupiter and Mars Djinn join, as a Venus
/// Djinni is caught, as the Lemurian ship rises, and in the Thor and Procne
/// summons.
#define SE_140 140
/// A rumble: Sol Sanctum's rooms after Mt. Aleph erupts, the stumps in Goma
/// Cave, the Lunpa Fortress trap, Mercury Lighthouse's waterfall passage, and
/// the Quake, Gaia, Judgment and other battle effects.
#define SE_141 141
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
/// it (BattleClass_GetDefeatSoundKind). A mimic chest leaping up and the
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
/// An actor jumps: ObjectMotion_Launch plays the high jump above 5 pixels a
/// frame.
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
/// A scene transition effect.
#define SE_SCENE_TRANSITION 167
/// The ambient sound a scene queues in Lama Temple and Xian
/// (FieldFx_SetQueuedSoundAndPlay).
#define SE_169 169
/// The ambient sound a scene queues (FieldFx_SetQueuedSoundAndPlay) with the
/// ground particles and Altin's water-spitting beasts, and in Fuchin Temple,
/// Vale, Kalay's tunnel, Mercury and Venus Lighthouse and the Goma Cave
/// entrance.
#define SE_170 170
/// Lightning on the field, a Lucky Wheels win, Kraden in Sol Sanctum's Luna
/// room, and the Jupiter battle effect.
#define SE_171 171
/// Lightning on the field, before SE_171.
#define SE_172 172
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
/// Kolima's bridge collapsing.
#define SE_176 176
/// The Tunnel Ruins' balance settling, and Venus Lighthouse's floating pillars
/// lowering.
#define SE_180 180
/// Sol Sanctum's door opening, and the Kraken shaking the Tolbi-bound ship.
#define SE_181 181
/// An eye sparkling as a way opens: the dragon in Fuchin Falls Cave and the
/// small jewels at Sol Sanctum's entrance.
#define SE_182 182
/// A rumble: Altin Peak's statue waking and the boulder chase, Crossbone Isle's
/// doors opening, and the jewels set at Sol Sanctum's entrance.
#define SE_183 183
/// Altin Peak's cart turning or moving forward.
#define SE_184 184
/// A pillar or statue pushed: Bilibin, Bilibin Cave, Goma Cave, Kalay, Sol
/// Sanctum, the Tunnel Ruins, Venus Lighthouse and Altin Peak.
#define SE_185 185
/// Sol Sanctum's statue opening the seal and shining its beams.
#define SE_186 186
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
/// Kolima's bridge collapsing, after SE_176.
#define SE_198 198
/// The curse's sparkles at Kolima and Kolima Forest, Tret rejecting the party,
/// and Sol Sanctum's statue opening its central hole.
#define SE_200 200
/// The party rising to the Elemental Star room, and an actor raised and shrunk
/// in the eruption.
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
/// Mia's Ply opening Mercury Lighthouse's entrance.
#define SE_209 209
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
/// A log or pillar starting to roll: the Colosso matches, Kolima Forest's
/// floodgate, Vale Cave, and the scenes' staged rolls.
#define SE_213 213
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
/// moving walkway, and an actor jumping down a ledge.
#define SE_217 217
/// Mercury Lighthouse's fountain, and the storm carrying the party off in
/// Suhalla Desert and dropping it on the world map.
#define SE_219 219
/// Frost on its target, Retreat spinning, Bilibin Cave's puddles, Fuchin
/// Temple's Reveal lesson, Mia in Imil and at Mercury Lighthouse, the curse on
/// Kolima, and the Elemental Stars taken by Alex.
#define SE_220 220
/// An actor raised with sparks at Mercury Lighthouse.
#define SE_221 221
/// The lifts of Mercury Lighthouse and its aerie.
#define SE_223 223
/// A pusher and its log moving together: the Colosso, Crossbone Isle and
/// Donpa's judgment in the Lunpa Fortress.
#define SE_226 226
/// A tree toppling in Fuchin Temple, and an obstacle knocked down in the
/// Colosso.
#define SE_227 227
/// A sparkling passage: arriving and leaving in Mercury Lighthouse, Venus
/// Lighthouse's sparkling passage, and Xian's water carrier.
#define SE_228 228
/// A splash as Altin's water beasts hop and land.
#define SE_229 229
/// Water draining at Altin Peak, a water switch flipped at Crossbone Isle and
/// Kolima Forest's floodgate, and Kalay's tunnel.
#define SE_230 230
/// The water level changing at Crossbone Isle and at Kolima Forest's floodgate.
#define SE_231 231
/// The water rising or draining at Crossbone Isle and Kolima Forest's
/// floodgate, and Altin Peak's cart stopping.
#define SE_232 232
/// Altin Peak's cart rolling.
#define SE_233 233
/// Kyle and Dora turning back on the stormy night.
#define SE_234 234
/// Plays three times as a Colosso round's lettering appears, before SE_237.
#define SE_236 236
/// Plays after SE_236's three as a Colosso round begins.
#define SE_237 237
/// A thing pushed one cell: the Colosso matches and Vault Cave's pillar.
#define SE_238 238
/// A log or pillar rolling (on player 3, until SE_288 cuts it): the Colosso,
/// Kolima Forest's floodgate and the scenes' staged rolls.
#define SE_239 239
/// A log pushed over: Kolima Forest, Mogall Forest, Vale Cave, the Colosso,
/// Crossbone Isle, and monsters attacking the Tolbi-bound ship.
#define SE_240 240
/// A log moved across Fuchin Falls Cave, and hopping across Mercury
/// Lighthouse's pool.
#define SE_241 241
/// Suhalla Desert's storms, and the storm catching the party on the world map.
#define SE_244 244
/// A spring searched, Retreat, the curse dust at Kolima, Tret restored, Mercury
/// Lighthouse's Ply, Fuchin Temple's Reveal lesson, and the eruption in Sol
/// Sanctum.
#define SE_246 246
/// A Colosso match's round and result, and a Colosseum trial failed or cleared.
#define SE_247 247
/// A Colosso competitor fading in.
#define SE_252 252
/// The first voice blip of a message: an actor's voice is 0x100 plus its
/// sprite's value (FieldFx_GetResourceId), and every few letters typed play it
/// plus the letter code's low two bits (UiWork_StepChannelScript).
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
/// scene change (unless FLAG_SKIP_SCENE_CHANGE_SOUND), a rolling log or cart
/// stopping, a battle starting on the field.
#define SE_288 288
/// not a sound: fades player 3 out (AUDIO_CUE_FADE_SLOT3)
#define SE_289 289
/// An effect pair spawned in the eruption at Sol Sanctum.
#define SE_292 292
/// Played before each queued scene sound (FieldFx_SetQueuedSoundAndPlay).
#define SE_298 298
/// Tolbi Spring: 50 frames into a toss.
#define SE_300 300
/// Tolbi Spring: a tossed coin meeting one of the moving obstacles.
#define SE_301 301
/// Lucky Dice: the dice thrown with A.
#define SE_302 302
/// Lucky Dice: each of the four numbers revealed, five frames apart.
#define SE_303 303
/// Lucky Wheels: the reels starting to spin.
#define SE_304 304
/// Lucky Wheels: a reel held or released.
#define SE_305 305
/// Lucky Wheels: 16 frames after the reels start, as the stop prompt shows.
#define SE_306 306
/// Lucky Wheels: a reel stopped.
#define SE_307 307
/// Plays as the Wise One appears: in the Sanctum, in the eruption, at Venus
/// Lighthouse and its aerie.
#define SE_308 308
/// Saturos and Menardi combining at Venus Lighthouse's aerie.
#define SE_309 309

#endif
