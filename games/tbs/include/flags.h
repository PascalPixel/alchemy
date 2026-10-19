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

// Sol Sanctum
// Kraden has said "This is it! We've found it!" in the hall of statues.
#define FLAG_SOL_SANCTUM_FOUND 0x809
// Kraden has seen the Luna and Sol rooms and asked the party to search on.
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN 0x80a
// A statue stands on its seal and the seal's light shines.
#define FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN 0x80b
#define FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN 0x80c
#define FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN 0x80d
#define FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN 0x80e
#define FLAG_SOL_SANCTUM_ALL_SEALS_OPEN 0x80f
// Kraden has seen the picture of Luna change to Sol.
#define FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED 0x810
// Kraden has seen the symbol on the floor change.
#define FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN 0x811
// The statues' lights have flashed and fallen through the Luna room.
#define FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN 0x812
// The statues' lights have flashed as Kraden stood among them.
#define FLAG_SOL_SANCTUM_STATUE_TRAP_SPRUNG 0x813
// Mt. Aleph has erupted: the sanctum's rooms rumble and shake.
#define FLAG_SOL_SANCTUM_ERUPTED 0x814
// The statues beside the hall's centre shine their beams.
#define FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING 0x816
#define FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING 0x817
#define FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED 0x818
// The floor has opened over the way up to the Elemental Star room.
#define FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN 0x819
// Kraden has said "You found it, Isaac." after the seals' lights flashed.
#define FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT 0x822
// Kraden has seen the statues out of his reach behind the blocks.
#define FLAG_SOL_SANCTUM_INNER_STATUES_SEEN 0x825
// The light each seal shows in the Luna and Sol rooms, lit or dark.
#define FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN 0x826
#define FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN 0x827
#define FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN 0x828
#define FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN 0x829
#define FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED 0x83b
#define FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED 0x83c
#define FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED 0x83d
// The Jupiter Star is bagged and Alex holds the four Elemental Stars.
#define FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX 0x83e
#define FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED 0x83f
// Isaac agreed to look farther on while Kraden studies the rooms.
#define FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN 0x896
// The column each movable block stands in, east or west of its pair.
#define FLAG_SOL_SANCTUM_OUTER_RIGHT_BLOCK_EAST 0x300
#define FLAG_SOL_SANCTUM_OUTER_RIGHT_BLOCK_WEST 0x301
#define FLAG_SOL_SANCTUM_OUTER_LEFT_BLOCK_EAST 0x302
#define FLAG_SOL_SANCTUM_OUTER_LEFT_BLOCK_WEST 0x303
#define FLAG_SOL_SANCTUM_FLOATING_EYE_SEEN 0x309
#define FLAG_SOL_SANCTUM_STATUE_HALL_VISITED 0x30a
#define FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED 0x30b
#define FLAG_SOL_SANCTUM_INNER_BLOCK_1_EAST 0x310
#define FLAG_SOL_SANCTUM_INNER_BLOCK_1_WEST 0x311
#define FLAG_SOL_SANCTUM_INNER_BLOCK_2_EAST 0x312
#define FLAG_SOL_SANCTUM_INNER_BLOCK_2_WEST 0x313
#define FLAG_SOL_SANCTUM_INNER_BLOCK_3_EAST 0x314
#define FLAG_SOL_SANCTUM_INNER_BLOCK_3_WEST 0x315
#define FLAG_SOL_SANCTUM_INNER_BLOCK_4_EAST 0x316
#define FLAG_SOL_SANCTUM_INNER_BLOCK_4_WEST 0x317
#define FLAG_SOL_SANCTUM_INNER_BLOCK_5_EAST 0x318
#define FLAG_SOL_SANCTUM_INNER_BLOCK_5_WEST 0x319
#define FLAG_SOL_SANCTUM_INNER_BLOCK_6_EAST 0x31a
#define FLAG_SOL_SANCTUM_INNER_BLOCK_6_WEST 0x31b
#define FLAG_SOL_SANCTUM_PSYNERGY_STONE_TAKEN 0x350
// The chests: the Small Jewel for each minotaur relief, and a Herb.
#define FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND 0xf01
#define FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND 0xf02
#define FLAG_SOL_SANCTUM_HERB_FOUND 0xf40
// The entrance hall: the party has entered the inner block room, and the
// outer blocks no longer move.
#define FLAG_SCENE_TEMP_34 0x234
// The eruption: Saturos's group has gone, and the embers fall silently.
#define FLAG_SCENE_TEMP_46 0x246

// Sanctum
// Isaac accepted the Great Healer's request to go to Sol Sanctum.
#define FLAG_SANCTUM_QUEST_ACCEPTED 0x9f0

#endif
