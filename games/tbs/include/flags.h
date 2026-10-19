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
#define FLAG_SCENE_TEMP_50                                                     \
  0x250 // Vault interiors: the leader is beside the Bone

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
// Crossbone Isle's pursuer room: a pursuer is after the leader.
#define FLAG_SCENE_TEMP_20 0x220

#endif
