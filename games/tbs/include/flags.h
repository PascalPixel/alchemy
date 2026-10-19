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
#define FLAG_SCENE_TEMP_50                                                     \
  0x250 // Vault interiors: the leader is beside the Bone

#endif
