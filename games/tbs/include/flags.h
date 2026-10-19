#ifndef ALCHEMY_FLAGS_H
#define ALCHEMY_FLAGS_H

/// Flags 0 to 7: the character with that id (character_ids.h) is in the party
/// (Party_AddActiveOwner).
#define FLAG_IN_PARTY(character) (character)
#define FLAG_CHILDHOOD 0x20 // the prologue: Isaac and Garet are children
#define FLAG_DJINNI_0 48    // the flag of Djinni 0, set once it is caught
/// Set as a battle starts, cleared as a field scene starts.
#define FLAG_IN_BATTLE 0x103
/// Set while the field menu is open; scenes hold their moving actors still.
#define FLAG_MENU_OPEN 0x106
// Lit lighthouses: the adepts of a lit element regain PP faster
#define FLAG_VENUS_LIGHTHOUSE_LIT 0x110
#define FLAG_MERCURY_LIGHTHOUSE_LIT 0x111
#define FLAG_MARS_LIGHTHOUSE_LIT 0x112
#define FLAG_JUPITER_LIGHTHOUSE_LIT 0x113
/// The next scene keeps the music that is playing; scene.c clears
/// FLAG_KEEP_MUSIC as it does, and leaves FLAG_KEEP_MUSIC_HELD set.
#define FLAG_KEEP_MUSIC 0x11a
#define FLAG_KEEP_MUSIC_HELD 0x11b
#define FLAG_NOT_ON_MAP 0x11c // the current place is not on the world map
/// Set before an action on the field; still set afterwards, nothing came of it
/// and the game says "But nothing happens.". An event that acts clears it.
#define FLAG_NOTHING_HAPPENS 0x142
#define FLAG_ITEM_USED_UP 0x143 // the item just used is removed afterwards
/// Retreat "does not work here", from Psynergy or an item. Cleared on every
/// scene entrance; the scenes that forbid Retreat set it.
#define FLAG_RETREAT_BLOCKED 0x144
#define FLAG_ITEM_USE 0x145     // the Psynergy comes from an item
#define FLAG_DEBUG_BATTLE 0x162 // set before every debug battle
/// Set while the Lure Cap is worn: random encounters come twice as fast.
#define FLAG_LURE_CAP 0x167
#define FLAG_AUTO_BATTLE 0x16a
#define FLAG_CHOOSING_COMMANDS 0x16b // set while the commands are chosen
#define FLAG_LINK_BATTLE 0x16c
#define FLAG_DJINN_TUTORIAL 0x16e
/// The link lobby's battle against a monster when no opponent is linked:
/// formations matched to the party's level, and no spoils.
#define FLAG_MONSTER_BATTLE 0x173
/// Set when the cleared game's menu goes to the link lobby: there Psynergy has
/// no effect and the system menu offers only Change settings.
#define FLAG_IN_LINK_LOBBY 0x17e
// Flags 0x300 to 0x3ff are place flags: Scene_ResetFlagsOnEnter clears them
// when the party enters another place group, and each place uses the numbers
// for its own, so the scenes name them. These two are the link lobby's.
// Byte flags, read and written eight at a time.
#define FLAG_LINK_PARTY_COUNT 0x3f0 // the members the linked opponent sent
/// Monster battles won in a row, added to the monsters' level; -1 after the
/// series ends.
#define FLAG_MONSTER_BATTLE_WINS 0x3f8
/// Plus a row of the shop stock table: that place's artifacts are in stock.
#define FLAG_SHOP_ARTIFACTS_STOCKED 0x400
/// Set while the Colosso holds the party's equipment (the inventory snapshot).
#define FLAG_INVENTORY_SNAPSHOT 0x952
#define FLAG_KEEP_PARTY_POSITION 0x109
#define FLAG_SHOW_LOCATION_NAME 0x12f
#define FLAG_KEEP_LAYER_GRAPHICS 0x170
#define FLAG_SANCTUM_VISIT_PLANNED 0x801
#define FLAG_SOL_SANCTUM_ENTERED 0x802
#define FLAG_SEARCHING_FOR_HIDDEN_PASSAGE 0x804
#define FLAG_MET_SATUROS_AND_MENARDI 0x808
#define FLAG_SOL_SANCTUM_FIRST_GEM_PLACED 0x81a
#define FLAG_SOL_SANCTUM_SECOND_GEM_PLACED 0x821
#define FLAG_TRET_DEFEATED 0x844
#define FLAG_KOLIMA_CURSE_LIFTED 0x845
#define FLAG_MET_LAUREL 0x84c
#define FLAG_MCCOY_PALACE_REWARD_TAKEN 0x84e
#define FLAG_VAULT_THIEVES_JAILED 0x855
#define FLAG_VAULT_DOG_SHOWN_PASSAGE 0x858
#define FLAG_VAULT_BONE_RECEIVED 0x85b
#define FLAG_VAULT_DJINNI_CALLED_BY_BELL 0x867
#define FLAG_BILIBIN_CAVE_SECOND_DOUSE_TARGET_CLEARED 0x882
#define FLAG_BILIBIN_CAVE_FIRST_DOUSE_TARGET_CLEARED 0x883
#define FLAG_XIAN_HEARD_HSU_TRAPPED 0x895
#define FLAG_FEIZHI_WENT_TO_HSU 0x89a
#define FLAG_HSU_RESCUED 0x8b2
#define FLAG_ALTIN_MINE_FIRST_DRAIN 0x8fd
#define FLAG_ALTIN_MINE_SECOND_DRAIN 0x8fe
#define FLAG_ALTIN_MINE_THIRD_DRAIN 0x907
#define FLAG_HYDROS_STATUES_DEFEATED 0x909
#define FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN 0x922
#define FLAG_TOLBI_SHIP_CREWMAN_CONFRONTED 0x923
#define FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND 0x924
#define FLAG_LUNPA_HEARD_OF_PRISONER 0x940
#define FLAG_LUNPA_TRADE_REOPENED 0x941
#define FLAG_LUNPA_CAVE_REUNION_SEEN 0x94d
/// Bunza's wagon has left West Lunpa Cave, with the party aboard or without.
#define FLAG_BUNZAS_WAGON_LEFT 0x94f
#define FLAG_IODEM_FOLLOWING_PARTY 0x9a0
#define FLAG_GONDOWAN_CAVE_FIRST_LEAP_COMPLETE 0x9a2
#define FLAG_GONDOWAN_CAVE_SECOND_OBJECT_MOVED 0x9a5

// Alpine Crossing and Lama Temple
/// Feizhi has found the road blocked and run ahead to look for Hsu.
#define FLAG_FEIZHI_SAW_ROCKSLIDE 0x8b0
/// Hama has reached Hsu and spoken with him and Feizhi.
#define FLAG_HAMA_REACHED_HSU 0x8b1
/// Hsu was freed without the party: set with FLAG_HSU_RESCUED when the party
/// comes back from Lama Temple after Xian has heard of the accident. While it
/// is set, Xian's people do not thank the party for the rescue.
#define FLAG_HSU_RESCUED_WITHOUT_PARTY 0x8b3
#define FLAG_ALPINE_CROSSING_POWER_BREAD 0xf71 // the crossing's chest
#define FLAG_LAMA_TEMPLE_WATER_OF_LIFE 0xf7a   // the temple's chest

// Altin
#define FLAG_ALTIN_WATER_BEAST_LEFT 0x8ff // jumped back into the flood
#define FLAG_ALTIN_BLOCK_MOVED 0x906      // pushed to its place by Move
#define FLAG_ALTIN_NUT 0xf73              // a barrel in the houses
#define FLAG_ALTIN_COINS 0xf74            // a barrel in the houses
#define FLAG_ALTIN_SLEEP_BOMB 0xf75       // a bed in the houses
/// Scene flags: Altin keeps its water level in them, set on entry.
#define FLAG_SCENE_TEMP_40 0x240
#define FLAG_SCENE_TEMP_41 0x241
#define FLAG_SCENE_TEMP_42 0x242

// Altin Peak, numbered by its rooms (SceneId_AltinPeak1 to 11)
/// Room 1's water beast has been seen hopping along its first and its second
/// route.
#define FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN 0x904
#define FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN 0x905
/// Room 9's boulder has fallen and rolled down after the party leader.
#define FLAG_ALTIN_PEAK_BOULDER_FALLEN 0x908
#define FLAG_ALTIN_PEAK_COOKIE 0xf76        // room 3's chest
#define FLAG_ALTIN_PEAK_DRAGON_SHIELD 0xf77 // room 5's chest
#define FLAG_ALTIN_PEAK_LUCKY_MEDAL 0xf78   // room 8's chest
#define FLAG_ALTIN_PEAK_VIAL 0xf79          // room 11's chest
#define FLAG_ALTIN_PEAK_PSY_CRYSTAL 0xf07   // room 7's chest
#define FLAG_ALTIN_PEAK_LIFTING_GEM 0xf08   // room 10's chest

// Altmiller Cave
#define FLAG_MET_INVISIBLE_BABI 0x960 // Babi has asked for his draught
/// Babi has called the party back once as it made to leave without him.
#define FLAG_BABI_CALLED_PARTY_BACK 0x961
/// The party has saved Babi in Altmiller Cave with his Mystic Draught; Tolbi
/// then holds the Colosso finals, and its scenes read this as the finals day.
#define FLAG_BABI_SAVED 0x962
#define FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT 0xf30 // behind the rocks' wall
#define FLAG_ALTMILLER_CAVE_VIAL 0xf9c           // room 3's chest
#define FLAG_ALTMILLER_CAVE_COOKIE 0xf9d         // room 5's chest

// Babi Lighthouse
/// The stone door that Reveal's switch opens stands open.
#define FLAG_BABI_LIGHTHOUSE_DOOR_OPEN 0x985
/// Iodem and Faran have asked the party to follow Sheba's kidnappers.
#define FLAG_BABI_LIGHTHOUSE_PURSUIT_ASKED 0x988
/// Set with it: Iodem has come to Babi Lighthouse, and from then on he no
/// longer leaves the party in Suhalla's inn or Gondowan Cave.
#define FLAG_IODEM_AT_BABI_LIGHTHOUSE 0x98a
/// The party has opened the door before Iodem and Faran, who stay behind.
#define FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN 0x989
#define FLAG_BABI_LIGHTHOUSE_LUCKY_PEPPER 0xfbc // the building site's chest

// Babi Palace
#define FLAG_COLOSSO_OVER 0x950 // the Colosso finals have been fought
#define FLAG_WON_COLOSSO 0x951  // the party won the finals
/// The attendant has offered the beds for the night before the finals.
#define FLAG_BABI_PALACE_FINALS_REST_OFFERED 0x966
/// While set, the beds can be used during the finals.
#define FLAG_BABI_PALACE_FINALS_BEDS_OPEN 0x967
/// Sheba has felt the party read her mind.
#define FLAG_SHEBA_NOTICED_MIND_READ 0x968
/// While set, the beds can be used before the finals.
#define FLAG_BABI_PALACE_BEDS_OPEN 0x969
#define FLAG_BABI_PALACE_FIRST_NIGHT 0x96a // the party has slept there once
/// The soldiers have come back from Altmiller Cave to Iodem without Babi.
#define FLAG_IODEM_HEARD_SOLDIERS 0x96b
/// A courtyard guard has passed on Babi's word to go to the colosseum.
#define FLAG_BABI_PALACE_GUARD_GAVE_WORD 0x96c
/// The other courtyard guard has thanked the party for saving Babi.
#define FLAG_BABI_PALACE_GUARD_THANKED_PARTY 0x96d
/// Babi and Iodem have spoken with the party after Colosso.
#define FLAG_BABI_SPOKE_AFTER_COLOSSO 0x96f
#define FLAG_BABI_PALACE_CLOAK_BALL 0xf31  // lent by Babi after Colosso
#define FLAG_BABI_PALACE_LUCKY_MEDAL 0xf9f // in a barrel
// Bilibin
#define FLAG_BILIBIN_JILL_RESCUED 0x848 // Jill, a tree, pulled from the river
#define FLAG_BILIBIN_JILL_GIFT_RECEIVED 0x849 // her thanks, a Hard Nut

// Colosso
#define FLAG_COLOSSO_FAILED_TRIAL_SEEN 0x8a3  // a warrior failed his trial
#define FLAG_COLOSSO_CLEARED_TRIAL_SEEN 0x8a4 // a warrior cleared his trial
#define FLAG_COLOSSO_FIRST_MATCH_WON 0x90e
#define FLAG_COLOSSO_SECOND_MATCH_WON 0x90f
// Also FLAG_COLOSSO_OVER (0x950): the colosseum is closed until next year.
// Also FLAG_BABI_SAVED (0x962): the trials are over, the finals day.

// Kalay
/// The inn's guests told the party about the Colosso tour to Tolbi.
#define FLAG_KALAY_HEARD_OF_COLOSSO_TOUR 0x910
#define FLAG_KALAY_COLOSSO_TOUR_LEFT 0x911 // the tour group has left town
#define FLAG_KALAY_TOWN_NUT_CAUGHT 0x916   // the Nut circling in the town

// Imil
/// Mia's second apprentice told the party Mia went to the old couple's house.
#define FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE 0x82a
/// Mia healed the old man, and the party saw the Mercury Lighthouse lit.
#define FLAG_IMIL_MET_MIA 0x82b
/// Her apprentice told the party she can act as its healer in the sanctum.
#define FLAG_IMIL_APPRENTICE_OFFERED_HEALING 0x82d
#define FLAG_IMIL_MIA_SAID_FAREWELL 0x82e // to her two apprentices

// Fuchin Falls
/// The Dragon's Eye is set in the dragon statue, which opened the hidden path.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET 0x820

// Kalay and Tolbi docks
#define FLAG_KALAY_DOCKS_TICKET_BOUGHT 0x8a5
/// The father on the Kalay docks asked the party to take a message to his
/// daughter in Tolbi, and the party agreed.
#define FLAG_KALAY_DOCKS_CARRYING_MESSAGE 0x8a6
#define FLAG_KALAY_DOCKS_TICKET_HANDED_OVER 0x8a7 // the party may board
#define FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED 0x8a8
#define FLAG_KALAY_DOCKS_WENT_ASHORE 0x8a9 // off the ship after boarding
/// A passenger on the Tolbi docks walked up to the party as it arrived.
#define FLAG_TOLBI_DOCKS_PASSENGER_MET_PARTY 0x8aa
/// The passengers confronted the tour guides and their bodyguard.
#define FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN 0x8ab
#define FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL 0x8ac
// Kolima
/// The party has seen the curse fall, in Kolima or on the forest bridge.
#define FLAG_KOLIMA_CURSE_WITNESSED 0x843
/// Kolima's bridge fell as the party crossed it.
#define FLAG_KOLIMA_BRIDGE_COLLAPSED 0x847

// Kolima Forest
#define FLAG_KOLIMA_FOREST_NUT_CAUGHT 0xfd3 // Catch took the clearing's Nut

// Lamakan Desert
#define FLAG_LAMAKAN_SAND_MONSTER_BEATEN 0x90a // on the third map

// Lalivero
/// The party is back from Venus Lighthouse, by the aerie's exit.
#define FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE 0x9a7
/// The party saw the ship from Venus Lighthouse before coming back.
#define FLAG_SAW_SHIP_FROM_VENUS_LIGHTHOUSE 0x9bf
#define FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN 0x9b8
#define FLAG_IODEM_FAREWELL_SEEN 0x9ba // Iodem gave the Black Orb
#define FLAG_LALIVERO_WHARF_OPEN 0x9bb // the guard lets the party to the ship
/// Iodem asked the party to wait for him outside.
#define FLAG_IODEM_ASKED_TO_MEET 0x9bc
// Lunpa
/// Set on the world map; from then on the east guard asks whether the party
/// came from Kalay.
#define FLAG_GUARDS_SUSPECT_KALAY 0x85a
/// The gate's left guard has recognized Hammet with the party.
#define FLAG_LUNPA_GATE_GUARD_SAW_HAMMET 0x9af
#define FLAG_LUNPA_PSYNERGY_STONE 0xf27
#define FLAG_LUNPA_GRAVE_SLEEP_BOMB 0xf88
#define FLAG_LUNPA_BARREL_NUT 0xf89
#define FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL 0xf8a // in the town's buildings
#define FLAG_LUNPA_BARREL_VIAL 0xf8b            // in the town's buildings
#define FLAG_LUNPA_NUT_CAUGHT 0xfd1 // the nut floating over the town

// Lunpa Fortress. From FLAG_LUNPA_FORTRESS_COLUMN_LOWERED to
// FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN, the fortress's state is cleared
// whenever the party comes back to the gate below it.
/// The party has seen Hammet's rescue up to Dodonpa's trap; a retry after
/// losing the battle skips to the trap.
#define FLAG_LUNPA_FORTRESS_TRAP_SEEN 0x942
#define FLAG_LUNPA_FORTRESS_COLUMN_LOWERED 0x943
/// Whirlwind has blown away what hid the first switch, the passage and the
/// second switch.
#define FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNCOVERED 0x944
#define FLAG_LUNPA_FORTRESS_PASSAGE_UNCOVERED 0x945
#define FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNCOVERED 0x946
#define FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED 0x947
#define FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED 0x948
#define FLAG_LUNPA_FORTRESS_HALL_GUARD_BEATEN 0x949
#define FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN 0x94a
#define FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN 0x94b
#define FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN 0x94c
/// Donpa's reward, the Djinni in his room, has joined the party.
#define FLAG_LUNPA_FORTRESS_REWARD_TAKEN 0x94e
#define FLAG_LUNPA_FORTRESS_CHEST_ELIXIR 0xfa2
#define FLAG_LUNPA_FORTRESS_CHEST_LUCKY_MEDAL 0xfa3
#define FLAG_LUNPA_FORTRESS_THIRD_CHEST 0xfa4 // item 0xaa
#define FLAG_LUNPA_FORTRESS_CHEST_COINS 0xfa5
#define FLAG_LUNPA_FORTRESS_BARREL_VIAL 0xfa6
#define FLAG_LUNPA_FORTRESS_CHEST_POWER_BREAD 0xfa7
#define FLAG_LUNPA_FORTRESS_CELL_KEY 0xf2e // caught or taken from its hook

// McCoy Palace and its garden, in Bilibin
/// The gate guards have let the party into the palace.
#define FLAG_MCCOY_PALACE_ADMITTED 0x84a
#define FLAG_MCCOY_PALACE_VISITED 0x84b // set on every entry to the palace
/// After McCoy turned the party away, his escort let slip that the
/// barricade may not need a key.
#define FLAG_MCCOY_PALACE_BARRICADE_HINT 0x84f
/// The escort has seen the party off after McCoy's reward.
#define FLAG_MCCOY_PALACE_PARTY_REWARDED 0x85e
/// The party refused McCoy's job at the first audience.
#define FLAG_MCCOY_PALACE_JOB_REFUSED 0x85f
#define FLAG_MCCOY_PALACE_BARREL_ELIXIR 0xf55
#define FLAG_MCCOY_PALACE_BARREL_SMOKE_BOMB 0xf56
#define FLAG_MCCOY_PALACE_GARDEN_NUT_CAUGHT 0xfd2 // the floating nut
// Mercury Lighthouse
/// Mia has told the party she senses someone inside the lighthouse.
#define FLAG_MIA_SENSED_SOMEONE_INSIDE 0x870
/// The statue is moved and Mia's Ply has opened the lighthouse door.
#define FLAG_MIA_OPENED_MERCURY_LIGHTHOUSE 0x871
/// Past the Lizard Man, Mia has gone on into the lighthouse.
#define FLAG_MIA_ENTERED_MERCURY_LIGHTHOUSE 0x872
#define FLAG_MIA_JOINED_PARTY 0x873 // rescued again, Mia joins with Fizz
/// The block rests on the floor switch and holds the gate open.
#define FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN 0x874
/// Ply in the entrance hall: the lights cycle and the pillar routes open.
#define FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED 0x875
/// All three channel blocks are in place and the water runs.
#define FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN 0x876
/// The sliding statue has fallen through to the floor below.
#define FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL 0x877
/// The block rests on the stair gate's floor switch and holds it open.
#define FLAG_MERCURY_LIGHTHOUSE_STAIR_GATE_HELD_OPEN 0x878
/// The aerie's confrontation with Saturos, the battle and Alex's visit are
/// over; set with FLAG_SATUROS_BEATEN_AT_MERCURY.
#define FLAG_MERCURY_AERIE_CONFRONTATION_DONE 0x880
/// Saturos is beaten at Mercury Lighthouse: the Hermes fountain at its foot
/// flows again.
#define FLAG_SATUROS_BEATEN_AT_MERCURY 0x881

// Mogall Forest
#define FLAG_MOGALL_FOREST_FIRST_APE_SEEN 0x89c // an ape hides on arrival
/// The last guiding ape has led the party to the Killer Ape's hiding place.
#define FLAG_MOGALL_FOREST_LED_TO_KILLER_APE 0x89d
#define FLAG_MOGALL_FOREST_KILLER_APE_BEATEN 0x89e
// Suhalla
/// Set at Babi's palace as his audience after Colosso begins; afterwards
/// Suhalla's people talk of the Tolbi soldiers the sandstorms brought there.
/// Also uses FLAG_BABI_SPOKE_AFTER_COLOSSO.
/// Iodem has found the wounded soldiers at Suhalla's inn and asked for Sheba.
#define FLAG_IODEM_MET_WOUNDED_SOLDIERS 0x9b0

// Suhalla Gate
#define FLAG_SUHALLA_GATE_SOLDIER_VANISHED 0x897 // after his last words
#define FLAG_SUHALLA_GATE_ATTACK_SEEN 0x89f // Iodem has questioned the guards
/// The side the party last came through, which Retreat returns them to.
#define FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH 0x8fb
#define FLAG_SUHALLA_GATE_ENTERED_FROM_EAST 0x8fc

// Suhalla Desert
#define FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA 0x9b5 // search on or go to Lalivero
#define FLAG_SUHALLA_DESERT_IODEM_WARNED 0x9b6    // "A sandstorm!"
#define FLAG_SUHALLA_DESERT_CHASING_STORM_DOUSED 0x9b7
// Tolbi
#define FLAG_TOLBI_CORN_GIVEN 0x8bf    // the snack vendor gave away her corn
#define FLAG_TOLBI_SEARCHERS_MET 0x8c0 // the soldiers asked about Babi
#define FLAG_TOLBI_SPRING_OPENED 0x8c1 // the gambler let the party go first

// Tolbi interiors: the woman who finds a Lucky Medal after Colosso
#define FLAG_TOLBI_INTERIORS_MEDAL_HIDDEN 0x8bc // she hid it in the barrel
#define FLAG_TOLBI_INTERIORS_MEDAL_TAKEN 0x8bd  // the party took it
#define FLAG_TOLBI_INTERIORS_MEDAL_MISSED 0x8be // she has greeted them since

// Tolbi ship: the crossing of the Karagol Sea from Kalay to Tolbi
#define FLAG_TOLBI_SHIP_AT_ISLAND 0x8a0 // at anchor off the island
/// The ship's place plays music 29, the monsters' theme, instead of 41.
#define FLAG_TOLBI_SHIP_MONSTERS_SIGHTED 0x8f0
#define FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED 0x903 // the party will scout it
/// The crew have wondered what became of the replacement ship.
#define FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY 0x920
/// "But the captain decided to set sail just a second ago."
#define FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL 0x921
/// The party gave back the captain's Anchor Charm and agreed to protect the
/// ship.
#define FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED 0x925
/// "Our replacement never arrived, but we're shoving off just the same."
#define FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT 0x926
#define FLAG_TOLBI_SHIP_ROWING 0x927 // the oars are moving
/// The monster attacks fought off. After the first an oarsman is hurt, and
/// from then on passengers the party chooses take the empty seats.
#define FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED 0x928
#define FLAG_TOLBI_SHIP_SECOND_ATTACK_REPELLED 0x929
#define FLAG_TOLBI_SHIP_THIRD_ATTACK_REPELLED 0x92a
#define FLAG_TOLBI_SHIP_KRAKEN_DEFEATED 0x92b
/// The passengers chosen to row: a group of nine flags for each empty seat,
/// one flag for each passenger, in the order of their actors from 8.
#define FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS 0x92c
#define FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS 0x935
#define FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS 0x917
#define FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS 0x990
#define FLAG_TOLBI_SHIP_REACHED_TOLBI 0x93e // "The ship has arrived safely"
// Tunnel Ruins
#define FLAG_TUNNEL_RUINS_BUSH_BLOWN_AWAY 0x970
#define FLAG_TUNNEL_RUINS_PILLAR_A_SUNK 0x971
#define FLAG_TUNNEL_RUINS_PILLAR_B_SUNK 0x972
#define FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST 0x974
#define FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST 0x975
#define FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN 0x984
// Vale
/// The prologue's stormy night: lightning over Vale, the rockslide and the
/// monsters loose in the village.
#define FLAG_VALE_STORM 0x834
/// Set whenever Kalay is entered; Vale's villagers then talk of the party's
/// journey.
#define FLAG_REACHED_KALAY 0x87a

// Vale, Kraden's cottage
#define FLAG_OVERHEARD_KRADEN 0x800 // Kraden wondered aloud about the strangers
/// The man hurt in the rockslide believes he will die.
#define FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP 0x82f
/// He found he was unhurt and ran to the plaza.
#define FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED 0x839
#define FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT 0x87d
#define FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT 0x87e
/// A monster was fought on the stormy night: Vale's music changes (song 23).
#define FLAG_VALE_STORM_MONSTER_FOUGHT 0x900

// Vale, the plaza
/// "Do you need to go past here? You must be going to Kraden's cottage..."
#define FLAG_VALE_ASKED_IF_GOING_TO_KRADEN 0x806
/// A villager's first outburst at Garet ("Garet, you make me so mad
/// sometimes!").
#define FLAG_VALE_ANGRY_VILLAGER_ARGUED 0x807
/// Back from Kalay, a villager asked whether Jenna is back yet.
#define FLAG_VALE_ASKED_IF_JENNA_IS_BACK 0x81f
/// A villager stopped Isaac: "You're not planning on sneaking up to Mt.
/// Aleph..."
#define FLAG_VALE_WARNED_AGAINST_MT_ALEPH 0x823

// Vale on the stormy night, the paths outside
/// The four rocks that fall across the paths (actors 11 to 14).
#define FLAG_VALE_STORM_FIRST_ROCK_FELL 0x830
#define FLAG_VALE_STORM_SECOND_ROCK_FELL 0x831
#define FLAG_VALE_STORM_THIRD_ROCK_FELL 0x832
#define FLAG_VALE_STORM_FOURTH_ROCK_FELL 0x833
/// A villager running out of his house shoved Isaac ("Huh? Isaac?").
#define FLAG_VALE_STORM_BUMPED_INTO_VILLAGER 0x835
/// Isaac heard Garet struggling with his things ("Ugh... Hrnghhh...").
#define FLAG_VALE_STORM_HEARD_GARET 0x836
#define FLAG_VALE_STORM_GARET_JOINED 0x837
#define FLAG_VALE_BOULDER_FELL 0x838
/// Isaac saw the villagers trying to pull Felix from the river.
#define FLAG_VALE_STORM_SAW_RIVER_RESCUE 0x83a
/// Jenna came back to the river ("Mom! Dad! We're back!") and the scene there
/// ended with ValeStorm_ShowBoulderSweepingDownriver.
#define FLAG_VALE_STORM_RIVER_GATHERING_SEEN 0x841
/// Garet ran off ("Isaac!"), leaving Isaac alone.
#define FLAG_VALE_STORM_GARET_LEFT 0x842
/// Dora and Kyle sent Isaac to the plaza.
#define FLAG_VALE_STORM_SENT_TO_PLAZA 0x87b
#define FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT 0x87c
#define FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT 0x87f

// Vale, the village
/// On the stormy night, the villagers sent Isaac to aid the elders holding
/// back the Boulder.
#define FLAG_VALE_STORM_SENT_TO_AID_ELDERS 0x840
/// Whirlwind cleared the three targets (actors 8 to 10) in the village.
#define FLAG_VALE_FIRST_WHIRLWIND_TARGET_CLEARED 0x90b
#define FLAG_VALE_SECOND_WHIRLWIND_TARGET_CLEARED 0x90c
#define FLAG_VALE_THIRD_WHIRLWIND_TARGET_CLEARED 0x90d

// Vale, around Isaac's house
/// Back from Kalay, Mind Read showed that Dora is ill.
#define FLAG_READ_THOUGHTS_ABOUT_DORA 0x81c
/// Isaac visited Dora in her sickbed and she got up.
#define FLAG_VISITED_SICK_DORA 0x81e

// Vale, houses and shops
/// The item shopkeeper gave Isaac a Herb as a farewell gift.
#define FLAG_VALE_FAREWELL_HERB_RECEIVED 0x81b
/// Back from Kalay, Garet surprised his family at home.
#define FLAG_GARET_CAME_HOME 0x81d

// Vale during the eruption
/// Isaac and Garet came down Mt. Aleph and followed the elders into the
/// sanctum.
#define FLAG_VALE_RETURNED_FROM_MT_ALEPH 0x879
// Vault: the stolen Shaman's Rod, and its containers, set once each is
// searched
/// Set when Ivan first shows the party his strange powers at the mayor's.
#define FLAG_VAULT_MET_IVAN 0x850
/// Set once the mayor has thanked the party for helping Ivan.
#define FLAG_VAULT_MAYOR_THANKED_PARTY 0x851
/// Set when the party first approaches the two suspects at the inn.
#define FLAG_VAULT_SUSPECTS_APPROACHED 0x852
/// Set once the mayor has given the party the Water of Life.
#define FLAG_VAULT_WATER_OF_LIFE_RECEIVED 0x857
/// Set once the crate is pushed aside, opening the passage behind it.
#define FLAG_VAULT_CRATE_MOVED 0x859
/// Set when the party agrees to help Ivan find the rod; while it is set and
/// the thieves are free, Ivan waits in Vault whenever the party leaves town.
#define FLAG_VAULT_IVAN_JOINED_ROD_SEARCH 0x856
/// Set once Ivan has read the minds of the two cornered suspects.
#define FLAG_VAULT_SUSPECTS_MIND_READ 0x853
/// Set as the three thieves are confronted in the attic, before the battle.
#define FLAG_VAULT_THIEVES_CONFRONTED 0x854
#define FLAG_VAULT_BOX_NUT 0xf4a

// Vault interiors: their containers, set once each is searched
#define FLAG_VAULT_INTERIORS_JAR_COINS 0xf4b
#define FLAG_VAULT_INTERIORS_BARREL_COINS 0xf4c
#define FLAG_VAULT_INTERIORS_BARREL_SLEEP_BOMB 0xf4d
#define FLAG_VAULT_INTERIORS_BOX_MINT 0xf4e

// Vault cave: its switches, and its chests, set once each is opened
#define FLAG_VAULT_CAVE_SWITCH_FLIPPED 0x9a8
#define FLAG_VAULT_CAVE_SOUTH_BLOCK_ON_SWITCH 0x9a9
#define FLAG_VAULT_CAVE_NORTH_BLOCK_ON_SWITCH 0x9aa
#define FLAG_VAULT_CAVE_1_CHEST 0xef3
#define FLAG_VAULT_CAVE_2_CHEST 0xfc4

// Vault Jail: its chests, set once each is opened
#define FLAG_JAIL_CHEST_WATER_OF_LIFE 0xf8c
#define FLAG_JAIL_CHEST_SMOKE_BOMB 0xf8d
#define FLAG_JAIL_CHEST_LUCKY_MEDAL 0xf8e
#define FLAG_JAIL_CHEST_ANTIDOTE 0xf8f
#define FLAG_JAIL_CHEST_COINS 0xf90
#define FLAG_JAIL_CHEST_ELIXIR 0xf91
/// Xian: set once the jar's Lucky Medal is taken.
#define FLAG_XIAN_LUCKY_MEDAL_JAR 0xf6e

// Xian interiors: set once each find is taken.
#define FLAG_XIAN_INTERIORS_ELIXIR_BARREL 0xf6f
#define FLAG_XIAN_INTERIORS_SLEEP_BOMB_JAR 0xf70

// Flags from 0x200 belong to the current scene: each scene keeps its puzzle
// and event states in the same numbers, and names the ones it uses.
#define FLAG_SCENE_TEMP_00 0x200
#define FLAG_SCENE_TEMP_01 0x201
#define FLAG_SCENE_TEMP_02 0x202
#define FLAG_SCENE_TEMP_03 0x203
#define FLAG_SCENE_TEMP_04 0x204
#define FLAG_SCENE_TEMP_05 0x205
#define FLAG_SCENE_TEMP_06 0x206
#define FLAG_SCENE_TEMP_07 0x207
#define FLAG_SCENE_TEMP_08 0x208
#define FLAG_SCENE_TEMP_09 0x209
#define FLAG_SCENE_TEMP_0A 0x20a
#define FLAG_SCENE_TEMP_0B 0x20b
#define FLAG_SCENE_TEMP_0C 0x20c
#define FLAG_SCENE_TEMP_0D 0x20d
#define FLAG_SCENE_TEMP_0E 0x20e
#define FLAG_SCENE_TEMP_0F 0x20f

#define FLAG_SCENE_TEMP_10 0x210 // Tret: lower middle east landing
#define FLAG_SCENE_TEMP_11 0x211 // Tret: second floor to west landing
#define FLAG_SCENE_TEMP_12 0x212 // Tret: lower middle southwest landing
#define FLAG_SCENE_TEMP_13 0x213 // Tret: lower middle south landing
#define FLAG_SCENE_TEMP_14 0x214 // Tret: lower middle southeast landing
#define FLAG_SCENE_TEMP_15 0x215 // Colosso log course: the second rock settled
#define FLAG_SCENE_TEMP_16 0x216 // Colosso log course: the third rock settled
#define FLAG_SCENE_TEMP_34 0x234 // Colosseum waiting room: stepped to the line
#define FLAG_SCENE_TEMP_35 0x235 // Colosseum waiting room: asked at the line
// Also FLAG_SCENE_TEMP_15: Lunpa Fortress, set with FLAG_SCENE_TEMP_14.
#define FLAG_SCENE_TEMP_17 0x217 // Lunpa Fortress: the first switch is hidden
#define FLAG_SCENE_TEMP_18 0x218 // Lunpa Fortress: the second switch is hidden
#define FLAG_SCENE_TEMP_24 0x224 // Lunpa Fortress: thrown out at the entrance
#define FLAG_SCENE_TEMP_25 0x225 // Lunpa Fortress: the sentries saw the party
#define FLAG_SCENE_TEMP_26 0x226 // Lunpa Fortress: the resident keeps quiet
#define FLAG_SCENE_TEMP_27 0x227 // Lunpa Fortress: the resident was startled
// Also FLAG_SCENE_TEMP_40: Lunpa gate, the guards block the gateway.
// Also FLAG_SCENE_TEMP_41: Lunpa gate, Cloak is cast.
// Also FLAG_SCENE_TEMP_42: Lunpa gate, the party enters the fortress.
#define FLAG_SCENE_TEMP_43 0x243 // Lunpa gate: Garet turns the leader back
#define FLAG_SCENE_TEMP_44 0x244 // Lunpa gate: the guards caught the party
/// Also FLAG_SCENE_TEMP_18: Suhalla Desert, the storm holds the party.
/// Also FLAG_SCENE_TEMP_41: Vale, the plaza's watching villager paused.
/// Also FLAG_SCENE_TEMP_42: Vale, Kraden's gate stands open.
#define FLAG_SCENE_TEMP_50                                                     \
  0x250 // Vault interiors: the leader is beside the Bone
#define FLAG_SCENE_TEMP_70 0x270 // Lunpa Fortress: Hammet's rescue has begun

// Goma Cave
#define FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED 0x860
#define FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP 0x861
#define FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS 0x862
#define FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE 0x863
#define FLAG_GOMA_CAVE_WATERWAY_STUMP_MOVED 0x864
/// Ivan has blown the ivy off the stump and rejoined the party.
#define FLAG_GOMA_CAVE_IVAN_REJOINED 0x865
#define FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY 0x866
#define FLAG_GOMA_CAVE_WITNESS_LEFT 0x869 // after the party caught Forge

// Hammet's palace
#define FLAG_IVAN_REPORTED_TO_LAYANA 0x912 // that Hammet is still in Lunpa
#define FLAG_LAYANA_SCOLDED_LUNPA_VISIT 0x913
#define FLAG_HAMMET_HOMECOMING_SEEN 0x914

// Fuchin Temple and Master Feh's school
/// Nyunpa is enlightened, beyond words: he has passed away.
#define FLAG_NYUNPA_ENLIGHTENED 0x88f
/// The visitor from Kolima heard that the bridge is open again.
#define FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN 0x890
#define FLAG_NYUNPA_TEST_ACCEPTED 0x891 // the test of the waterfall grotto
#define FLAG_FUCHIN_FALLS_GROTTO_OPENED 0x892 // the monk stepped aside
#define FLAG_NYUNPA_TAUGHT_FORCE 0x893        // the power the monks call Ki
#define FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST 0x894
/// The Orb of Force's chest in Fuchin Falls has been opened.
#define FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND 0xf14
#define FLAG_FEH_WAITS_TO_SEE_CHI 0x898
#define FLAG_FEH_CHI_DEMONSTRATION_DECLINED 0x899
#define FLAG_FEH_SAW_CHI 0x89b

// Crossbone Isle
/// The four blue doors in a row, then the red door.
#define FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN 0x9c0
#define FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN 0x9c4
/// Bane's three escapes in the block room.
#define FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE 0x9c8
#define FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE 0x9c9
#define FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE 0x9ca
/// The ivy in the first room: four flags from this one.
#define FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY 0x8c4
/// The nine guarded rooms: one flag each from these, in room order.
#define FLAG_CROSSBONE_ISLE_FIRST_GUARDS_DEFEATED 0x8c8
#define FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED 0x8d2
#define FLAG_CROSSBONE_ISLE_FINAL_GUARDS_DEFEATED 0x8d1
/// Crossbone Isle's pursuer room: a pursuer is after the leader.
#define FLAG_SCENE_TEMP_20 0x220
// Crossbone Isle's first room: its four plants have grown.
// Also uses FLAG_SCENE_TEMP_40.
// Also uses FLAG_SCENE_TEMP_41.
// Also uses FLAG_SCENE_TEMP_42.
// Also uses FLAG_SCENE_TEMP_43.

// Sol Sanctum
/// Kraden has said "This is it! We've found it!" in the hall of statues.
#define FLAG_SOL_SANCTUM_FOUND 0x809
/// Kraden has seen the Luna and Sol rooms and asked the party to search on.
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN 0x80a
/// A statue stands on its seal and the seal's light shines.
#define FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN 0x80b
#define FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN 0x80c
#define FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN 0x80d
#define FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN 0x80e
#define FLAG_SOL_SANCTUM_ALL_SEALS_OPEN 0x80f
/// Kraden has seen the picture of Luna change to Sol.
#define FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED 0x810
/// Kraden has seen the symbol on the floor change.
#define FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN 0x811
/// The statues' lights have flashed and fallen through the Luna room.
#define FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN 0x812
/// The statues' lights have flashed as Kraden stood among them.
#define FLAG_SOL_SANCTUM_STATUE_TRAP_SPRUNG 0x813
/// Mt. Aleph has erupted: the sanctum's rooms rumble and shake.
#define FLAG_SOL_SANCTUM_ERUPTED 0x814
/// The statues beside the hall's centre shine their beams.
#define FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING 0x816
#define FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING 0x817
#define FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED 0x818
/// The floor has opened over the way up to the Elemental Star room.
#define FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN 0x819
/// Kraden has said "You found it, Isaac." after the seals' lights flashed.
#define FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT 0x822
/// Kraden has seen the statues out of his reach behind the blocks.
#define FLAG_SOL_SANCTUM_INNER_STATUES_SEEN 0x825
/// The light each seal shows in the Luna and Sol rooms, lit or dark.
#define FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN 0x826
#define FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN 0x827
#define FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN 0x828
#define FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN 0x829
#define FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED 0x83b
#define FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED 0x83c
#define FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED 0x83d
/// The Jupiter Star is bagged and Alex holds the four Elemental Stars.
#define FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX 0x83e
#define FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED 0x83f
/// Isaac agreed to look farther on while Kraden studies the rooms.
#define FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN 0x896
/// The chests: the Small Jewel for each minotaur relief, and a Herb.
#define FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND 0xf01
#define FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND 0xf02
#define FLAG_SOL_SANCTUM_HERB_FOUND 0xf40
/// The entrance hall: the party has entered the inner block room, and the
/// outer blocks no longer move.
/// Also uses FLAG_SCENE_TEMP_34.
/// The eruption: Saturos's group has gone, and the embers fall silently.
#define FLAG_SCENE_TEMP_46 0x246

// Sanctum
/// Isaac accepted the Great Healer's request to go to Sol Sanctum.
#define FLAG_SANCTUM_QUEST_ACCEPTED 0x9f0
#define FLAG_SCENE_TEMP_71 0x271 // Tolbi ship deck: the first door is open
#define FLAG_SCENE_TEMP_72 0x272 // Tolbi ship deck: the second door is open

// Venus Lighthouse
/// The man at the entrance has told of the attack and is gone.
#define FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD 0x981
/// The path the statue opened: the first or the second of its two.
#define FLAG_VENUS_LIGHTHOUSE_FIRST_PATH 0x982
#define FLAG_VENUS_LIGHTHOUSE_SECOND_PATH 0x983
/// Garet has said the block by the wall can go no farther.
#define FLAG_VENUS_LIGHTHOUSE_BLOCK_AT_WALL 0x986
/// Carry has set that block down beyond the wall.
#define FLAG_VENUS_LIGHTHOUSE_BLOCK_CARRIED 0x987

// Venus Lighthouse aerie
/// Also uses FLAG_SCENE_TEMP_34.
/// Also uses FLAG_SCENE_TEMP_35.
#define FLAG_SCENE_TEMP_36 0x236
#define FLAG_SCENE_TEMP_37 0x237

// World map
/// The Black Orb did nothing on Babi's ship and was handed back.
#define FLAG_BLACK_ORB_RETURNED 0x85d
#define FLAG_SCENE_TEMP_F0 0x2f0
#define FLAG_SCENE_TEMP_F1 0x2f1
#define FLAG_SCENE_TEMP_F8 0x2f8 // a byte flag

#endif
