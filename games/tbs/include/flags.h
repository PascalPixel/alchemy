#ifndef ALCHEMY_FLAGS_H
#define ALCHEMY_FLAGS_H

#define FLAG_CHILDHOOD 0x20 // the prologue: Isaac and Garet are children
#define FLAG_DJINNI_0 48    // the flag of Djinni 0, set once it is caught
// Lit lighthouses: the adepts of a lit element regain PP faster
#define FLAG_VENUS_LIGHTHOUSE_LIT 0x110
#define FLAG_MERCURY_LIGHTHOUSE_LIT 0x111
#define FLAG_MARS_LIGHTHOUSE_LIT 0x112
#define FLAG_JUPITER_LIGHTHOUSE_LIT 0x113
#define FLAG_NOT_ON_MAP 0x11c // the current place is not on the world map
// Set before an action on the field; still set afterwards, nothing came of it
// and the game says "But nothing happens.". An event that acts clears it.
#define FLAG_NOTHING_HAPPENS 0x142
#define FLAG_ITEM_USED_UP 0x143 // the item just used is removed afterwards
#define FLAG_ITEM_USE 0x145     // the Psynergy comes from an item
#define FLAG_DEBUG_BATTLE 0x162 // set before every debug battle
// Set while the Lure Cap is worn: random encounters come twice as fast.
#define FLAG_LURE_CAP 0x167
#define FLAG_AUTO_BATTLE 0x16a
#define FLAG_CHOOSING_COMMANDS 0x16b // set while the commands are chosen
#define FLAG_LINK_BATTLE 0x16c
#define FLAG_DJINN_TUTORIAL 0x16e
// The link lobby's battle against a monster when no opponent is linked:
// formations matched to the party's level, and no spoils.
#define FLAG_MONSTER_BATTLE 0x173
// Byte flags, read and written eight at a time.
#define FLAG_LINK_PARTY_COUNT 0x3f0 // the members the linked opponent sent
// Monster battles won in a row, added to the monsters' level; -1 after the
// series ends.
#define FLAG_MONSTER_BATTLE_WINS 0x3f8
// Plus a row of the shop stock table: that place's artifacts are in stock.
#define FLAG_SHOP_ARTIFACTS_STOCKED 0x400
// Set while the Colosso holds the party's equipment (the inventory snapshot).
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
#define FLAG_PARTY_STAYED_IN_LUNPA 0x94f
#define FLAG_IODEM_FOLLOWING_PARTY 0x9a0
#define FLAG_GONDOWAN_CAVE_FIRST_LEAP_COMPLETE 0x9a2
#define FLAG_GONDOWAN_CAVE_SECOND_OBJECT_MOVED 0x9a5

// Alpine Crossing and Lama Temple
#define FLAG_LAMA_TEMPLE_PSYNERGY_STONE 0x350 // the temple's Psynergy Stone
// Feizhi has found the road blocked and run ahead to look for Hsu.
#define FLAG_FEIZHI_SAW_ROCKSLIDE 0x8b0
// Hama has reached Hsu and spoken with him and Feizhi.
#define FLAG_HAMA_REACHED_HSU 0x8b1
// Hsu was freed without the party: set with FLAG_HSU_RESCUED when the party
// comes back from Lama Temple after Xian has heard of the accident. While it
// is set, Xian's people do not thank the party for the rescue.
#define FLAG_HSU_RESCUED_WITHOUT_PARTY 0x8b3
#define FLAG_ALPINE_CROSSING_POWER_BREAD 0xf71 // the crossing's chest
#define FLAG_LAMA_TEMPLE_WATER_OF_LIFE 0xf7a   // the temple's chest

// Altin
#define FLAG_ALTIN_WATER_BEAST_LEFT 0x8ff // jumped back into the flood
#define FLAG_ALTIN_BLOCK_MOVED 0x906      // pushed to its place by Move
#define FLAG_ALTIN_NUT 0xf73              // a barrel in the houses
#define FLAG_ALTIN_COINS 0xf74            // a barrel in the houses
#define FLAG_ALTIN_SLEEP_BOMB 0xf75       // a bed in the houses
// Scene flags: Altin keeps its water level in them, set on entry.
#define FLAG_SCENE_TEMP_40 0x240
#define FLAG_SCENE_TEMP_41 0x241
#define FLAG_SCENE_TEMP_42 0x242

// Altin Peak, numbered by its rooms (SceneId_AltinPeak1 to 11)
// A room's floor switch has been flipped from its first position.
#define FLAG_ALTIN_PEAK_ROOM_3_SWITCH 0x323
#define FLAG_ALTIN_PEAK_ROOM_5_SWITCH 0x325
#define FLAG_ALTIN_PEAK_ROOM_6_SWITCH 0x326
#define FLAG_ALTIN_PEAK_ROOM_7_SWITCH 0x327
// Room 1's water beast has been seen hopping along its first and its second
// route.
#define FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN 0x904
#define FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN 0x905
// Room 9's boulder has fallen and rolled down after the party leader.
#define FLAG_ALTIN_PEAK_BOULDER_FALLEN 0x908
#define FLAG_ALTIN_PEAK_COOKIE 0xf76        // room 3's chest
#define FLAG_ALTIN_PEAK_DRAGON_SHIELD 0xf77 // room 5's chest
#define FLAG_ALTIN_PEAK_LUCKY_MEDAL 0xf78   // room 8's chest
#define FLAG_ALTIN_PEAK_VIAL 0xf79          // room 11's chest
#define FLAG_ALTIN_PEAK_PSY_CRYSTAL 0xf07   // room 7's chest
#define FLAG_ALTIN_PEAK_LIFTING_GEM 0xf08   // room 10's chest

// Altmiller Cave
#define FLAG_MET_INVISIBLE_BABI 0x960 // Babi has asked for his draught
// Babi has called the party back once as it made to leave without him.
#define FLAG_BABI_CALLED_PARTY_BACK 0x961
// The party has saved Babi in Altmiller Cave with his Mystic Draught.
#define FLAG_BABI_RESCUED 0x962
#define FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT 0xf30 // behind the rocks' wall
#define FLAG_ALTMILLER_CAVE_VIAL 0xf9c           // room 3's chest
#define FLAG_ALTMILLER_CAVE_COOKIE 0xf9d         // room 5's chest

// Babi Lighthouse
// The stone door that Reveal's switch opens stands open.
#define FLAG_BABI_LIGHTHOUSE_DOOR_OPEN 0x985
// Iodem and Faran have asked the party to follow Sheba's kidnappers.
#define FLAG_BABI_LIGHTHOUSE_PURSUIT_ASKED 0x988
// The party has opened the door before Iodem and Faran, who stay behind.
#define FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN 0x989
#define FLAG_BABI_LIGHTHOUSE_LUCKY_PEPPER 0xfbc // the building site's chest

// Babi Palace
#define FLAG_COLOSSO_OVER 0x950 // the Colosso finals have been fought
#define FLAG_WON_COLOSSO 0x951  // the party won the finals
// The attendant has offered the beds for the night before the finals.
#define FLAG_BABI_PALACE_FINALS_REST_OFFERED 0x966
// While set, the beds can be used during the finals.
#define FLAG_BABI_PALACE_FINALS_BEDS_OPEN 0x967
// Sheba has felt the party read her mind.
#define FLAG_SHEBA_NOTICED_MIND_READ 0x968
// While set, the beds can be used before the finals.
#define FLAG_BABI_PALACE_BEDS_OPEN 0x969
#define FLAG_BABI_PALACE_FIRST_NIGHT 0x96a // the party has slept there once
// The soldiers have come back from Altmiller Cave to Iodem without Babi.
#define FLAG_IODEM_HEARD_SOLDIERS 0x96b
// A courtyard guard has passed on Babi's word to go to the colosseum.
#define FLAG_BABI_PALACE_GUARD_GAVE_WORD 0x96c
// The other courtyard guard has thanked the party for saving Babi.
#define FLAG_BABI_PALACE_GUARD_THANKED_PARTY 0x96d
// Babi and Iodem have spoken with the party after Colosso.
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
#define FLAG_COLOSSO_OVER 0x950       // the colosseum is closed until next year
#define FLAG_COLOSSO_FINALS_DAY 0x962 // the trials are over: the finals day
// Kalay
// The inn's guests told the party about the Colosso tour to Tolbi.
#define FLAG_KALAY_HEARD_OF_COLOSSO_TOUR 0x910
#define FLAG_KALAY_COLOSSO_TOUR_LEFT 0x911 // the tour group has left town
#define FLAG_KALAY_TOWN_NUT_CAUGHT 0x916   // the Nut circling in the town

// Imil
// Mia's second apprentice told the party Mia went to the old couple's house.
#define FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE 0x82a
// Mia healed the old man, and the party saw the Mercury Lighthouse lit.
#define FLAG_IMIL_MET_MIA 0x82b
// Her apprentice told the party she can act as its healer in the sanctum.
#define FLAG_IMIL_APPRENTICE_OFFERED_HEALING 0x82d
#define FLAG_IMIL_MIA_SAID_FAREWELL 0x82e // to her two apprentices

// Fuchin Falls
// The Dragon's Eye is set in the dragon statue, which opened the hidden path.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET 0x820

// Kalay and Tolbi docks
#define FLAG_KALAY_DOCKS_TICKET_BOUGHT 0x8a5
// The father on the Kalay docks asked the party to take a message to his
// daughter in Tolbi, and the party agreed.
#define FLAG_KALAY_DOCKS_CARRYING_MESSAGE 0x8a6
#define FLAG_KALAY_DOCKS_TICKET_HANDED_OVER 0x8a7 // the party may board
#define FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED 0x8a8
#define FLAG_KALAY_DOCKS_WENT_ASHORE 0x8a9 // off the ship after boarding
// A passenger on the Tolbi docks walked up to the party as it arrived.
#define FLAG_TOLBI_DOCKS_PASSENGER_MET_PARTY 0x8aa
// The passengers confronted the tour guides and their bodyguard.
#define FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN 0x8ab
#define FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL 0x8ac
// Kolima
// The party has seen the curse fall, in Kolima or on the forest bridge.
#define FLAG_KOLIMA_CURSE_WITNESSED 0x843
// Kolima's bridge fell as the party crossed it.
#define FLAG_KOLIMA_BRIDGE_COLLAPSED 0x847

// Kolima Forest
#define FLAG_KOLIMA_FOREST_NUT_CAUGHT 0xfd3 // Catch took the clearing's Nut

// Lamakan Desert
#define FLAG_LAMAKAN_SAND_MONSTER_BEATEN 0x90a // on the third map

// Lalivero
// The party is back from Venus Lighthouse, by the aerie's exit.
#define FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE 0x9a7
// The party saw the ship from Venus Lighthouse before coming back.
#define FLAG_SAW_SHIP_FROM_VENUS_LIGHTHOUSE 0x9bf
#define FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN 0x9b8
#define FLAG_IODEM_FAREWELL_SEEN 0x9ba // Iodem gave the Black Orb
#define FLAG_LALIVERO_WHARF_OPEN 0x9bb // the guard lets the party to the ship
// Iodem asked the party to wait for him outside.
#define FLAG_IODEM_ASKED_TO_MEET 0x9bc
// Lunpa
// Set on the world map; from then on the east guard asks whether the party
// came from Kalay.
#define FLAG_GUARDS_SUSPECT_KALAY 0x85a
// The gate's left guard has recognized Hammet with the party.
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
// The party has seen Hammet's rescue up to Dodonpa's trap; a retry after
// losing the battle skips to the trap.
#define FLAG_LUNPA_FORTRESS_TRAP_SEEN 0x942
#define FLAG_LUNPA_FORTRESS_COLUMN_LOWERED 0x943
// Whirlwind has blown away what hid the first switch, the passage and the
// second switch.
#define FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNCOVERED 0x944
#define FLAG_LUNPA_FORTRESS_PASSAGE_UNCOVERED 0x945
#define FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNCOVERED 0x946
#define FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED 0x947
#define FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED 0x948
#define FLAG_LUNPA_FORTRESS_HALL_GUARD_BEATEN 0x949
#define FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN 0x94a
#define FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN 0x94b
#define FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN 0x94c
// Donpa's reward, the Djinni in his room, has joined the party.
#define FLAG_LUNPA_FORTRESS_REWARD_TAKEN 0x94e
#define FLAG_LUNPA_FORTRESS_CHEST_ELIXIR 0xfa2
#define FLAG_LUNPA_FORTRESS_CHEST_LUCKY_MEDAL 0xfa3
#define FLAG_LUNPA_FORTRESS_THIRD_CHEST 0xfa4 // item 0xaa
#define FLAG_LUNPA_FORTRESS_CHEST_COINS 0xfa5
#define FLAG_LUNPA_FORTRESS_BARREL_VIAL 0xfa6
#define FLAG_LUNPA_FORTRESS_CHEST_POWER_BREAD 0xfa7
#define FLAG_LUNPA_FORTRESS_CELL_KEY 0xf2e // caught or taken from its hook

// McCoy Palace and its garden, in Bilibin
// The gate guards have let the party into the palace.
#define FLAG_MCCOY_PALACE_ADMITTED 0x84a
#define FLAG_MCCOY_PALACE_VISITED 0x84b // set on every entry to the palace
// After McCoy turned the party away, his escort let slip that the
// barricade may not need a key.
#define FLAG_MCCOY_PALACE_BARRICADE_HINT 0x84f
// The escort has seen the party off after McCoy's reward.
#define FLAG_MCCOY_PALACE_PARTY_REWARDED 0x85e
// The party refused McCoy's job at the first audience.
#define FLAG_MCCOY_PALACE_JOB_REFUSED 0x85f
#define FLAG_MCCOY_PALACE_BARREL_ELIXIR 0xf55
#define FLAG_MCCOY_PALACE_BARREL_SMOKE_BOMB 0xf56
#define FLAG_MCCOY_PALACE_GARDEN_NUT_CAUGHT 0xfd2 // the floating nut

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
#define FLAG_SCENE_TEMP_15 0x215 // Lunpa Fortress: set with FLAG_SCENE_TEMP_14
#define FLAG_SCENE_TEMP_17 0x217 // Lunpa Fortress: the first switch is hidden
#define FLAG_SCENE_TEMP_18 0x218 // Lunpa Fortress: the second switch is hidden
#define FLAG_SCENE_TEMP_24 0x224 // Lunpa Fortress: thrown out at the entrance
#define FLAG_SCENE_TEMP_25 0x225 // Lunpa Fortress: the sentries saw the party
#define FLAG_SCENE_TEMP_26 0x226 // Lunpa Fortress: the resident keeps quiet
#define FLAG_SCENE_TEMP_27 0x227 // Lunpa Fortress: the resident was startled
#define FLAG_SCENE_TEMP_40 0x240 // Lunpa gate: the guards block the gateway
#define FLAG_SCENE_TEMP_41 0x241 // Lunpa gate: Cloak is cast
#define FLAG_SCENE_TEMP_42 0x242 // Lunpa gate: the party enters the fortress
#define FLAG_SCENE_TEMP_43 0x243 // Lunpa gate: Garet turns the leader back
#define FLAG_SCENE_TEMP_44 0x244 // Lunpa gate: the guards caught the party
#define FLAG_SCENE_TEMP_50                                                     \
  0x250 // Vault interiors: the leader is beside the Bone
#define FLAG_SCENE_TEMP_70 0x270 // Lunpa Fortress: Hammet's rescue has begun

// Goma Cave
#define FLAG_GOMA_CAVE_STUMP_FELL 0x300 // pushed over into the room below
#define FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE 0x301 // sunk into its place
#define FLAG_GOMA_CAVE_WEST_PILLAR_MOVED 0x302    // one step east of its start
#define FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED 0x860
#define FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP 0x861
#define FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS 0x862
#define FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE 0x863
#define FLAG_GOMA_CAVE_WATERWAY_STUMP_MOVED 0x864
// Ivan has blown the ivy off the stump and rejoined the party.
#define FLAG_GOMA_CAVE_IVAN_REJOINED 0x865
#define FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY 0x866
#define FLAG_GOMA_CAVE_WITNESS_LEFT 0x869 // after the party caught Forge

// Hammet's palace
#define FLAG_IVAN_REPORTED_TO_LAYANA 0x912 // that Hammet is still in Lunpa
#define FLAG_LAYANA_SCOLDED_LUNPA_VISIT 0x913
#define FLAG_HAMMET_HOMECOMING_SEEN 0x914

// Fuchin Temple and Master Feh's school
// Nyunpa is enlightened, beyond words: he has passed away.
#define FLAG_NYUNPA_ENLIGHTENED 0x88f
// The visitor from Kolima heard that the bridge is open again.
#define FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN 0x890
#define FLAG_NYUNPA_TEST_ACCEPTED 0x891 // the test of the waterfall grotto
#define FLAG_FUCHIN_FALLS_GROTTO_OPENED 0x892 // the monk stepped aside
#define FLAG_NYUNPA_TAUGHT_FORCE 0x893        // the power the monks call Ki
#define FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST 0x894
#define FLAG_FEH_WAITS_TO_SEE_CHI 0x898
#define FLAG_FEH_CHI_DEMONSTRATION_DECLINED 0x899
#define FLAG_FEH_SAW_CHI 0x89b
// Nyunpa's test was declined; the students saw the training tree fall.
#define FLAG_NYUNPA_TEST_DECLINED 0x300
#define FLAG_FEH_SCHOOL_SAW_TREE_FALL 0x301

// Crossbone Isle
#define FLAG_CROSSBONE_ISLE_THIRD_BLOCK_LOWERED 0x300
// The four blue doors in a row, then the red door.
#define FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN 0x9c0
#define FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN 0x9c4
// Bane's three escapes in the block room.
#define FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE 0x9c8
#define FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE 0x9c9
#define FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE 0x9ca
// The ivy in the first room: four flags from this one.
#define FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY 0x8c4
// The nine guarded rooms: one flag each from these, in room order.
#define FLAG_CROSSBONE_ISLE_FIRST_GUARDS_DEFEATED 0x8c8
#define FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED 0x8d2
#define FLAG_CROSSBONE_ISLE_FINAL_GUARDS_DEFEATED 0x8d1
// Crossbone Isle's pursuer room: a pursuer is after the leader.
#define FLAG_SCENE_TEMP_20 0x220
// Crossbone Isle's first room: its four plants have grown.
#define FLAG_SCENE_TEMP_40 0x240
#define FLAG_SCENE_TEMP_41 0x241
#define FLAG_SCENE_TEMP_42 0x242
#define FLAG_SCENE_TEMP_43 0x243

#endif
