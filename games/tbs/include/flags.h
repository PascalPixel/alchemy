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

#endif
