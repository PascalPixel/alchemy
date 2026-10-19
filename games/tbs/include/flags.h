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

// Vale
// The prologue's stormy night: lightning over Vale, the rockslide and the
// monsters loose in the village.
#define FLAG_VALE_STORM 0x834
// Set whenever Kalay is entered; Vale's villagers then talk of the party's
// journey.
#define FLAG_REACHED_KALAY 0x87a

// Vale, Kraden's cottage
#define FLAG_OVERHEARD_KRADEN 0x800 // Kraden wondered aloud about the strangers
// The man hurt in the rockslide believes he will die.
#define FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP 0x82f
// He found he was unhurt and ran to the plaza.
#define FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED 0x839
#define FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT 0x87d
#define FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT 0x87e
// A monster was fought on the stormy night: Vale's music changes (song 23).
#define FLAG_VALE_STORM_MONSTER_FOUGHT 0x900

// Vale, the plaza
// "Do you need to go past here? You must be going to Kraden's cottage..."
#define FLAG_VALE_ASKED_IF_GOING_TO_KRADEN 0x806
// A villager's first outburst at Garet ("Garet, you make me so mad
// sometimes!").
#define FLAG_VALE_ANGRY_VILLAGER_ARGUED 0x807
// Back from Kalay, a villager asked whether Jenna is back yet.
#define FLAG_VALE_ASKED_IF_JENNA_IS_BACK 0x81f
// A villager stopped Isaac: "You're not planning on sneaking up to Mt.
// Aleph..."
#define FLAG_VALE_WARNED_AGAINST_MT_ALEPH 0x823

// Vale on the stormy night, the paths outside
// The four rocks that fall across the paths (actors 11 to 14).
#define FLAG_VALE_STORM_FIRST_ROCK_FELL 0x830
#define FLAG_VALE_STORM_SECOND_ROCK_FELL 0x831
#define FLAG_VALE_STORM_THIRD_ROCK_FELL 0x832
#define FLAG_VALE_STORM_FOURTH_ROCK_FELL 0x833
// A villager running out of his house shoved Isaac ("Huh? Isaac?").
#define FLAG_VALE_STORM_BUMPED_INTO_VILLAGER 0x835
// Isaac heard Garet struggling with his things ("Ugh... Hrnghhh...").
#define FLAG_VALE_STORM_HEARD_GARET 0x836
#define FLAG_VALE_STORM_GARET_JOINED 0x837
#define FLAG_VALE_BOULDER_FELL 0x838
// Isaac saw the villagers trying to pull Felix from the river.
#define FLAG_VALE_STORM_SAW_RIVER_RESCUE 0x83a
// Jenna came back to the river ("Mom! Dad! We're back!") and the scene there
// ended with ValeStorm_ShowBoulderSweepingDownriver.
#define FLAG_VALE_STORM_RIVER_GATHERING_SEEN 0x841
// Garet ran off ("Isaac!"), leaving Isaac alone.
#define FLAG_VALE_STORM_GARET_LEFT 0x842
// Dora and Kyle sent Isaac to the plaza.
#define FLAG_VALE_STORM_SENT_TO_PLAZA 0x87b
#define FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT 0x87c
#define FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT 0x87f

// Vale, the village
// On the stormy night, the villagers sent Isaac to aid the elders holding
// back the Boulder.
#define FLAG_VALE_STORM_SENT_TO_AID_ELDERS 0x840
// Whirlwind cleared the three targets (actors 8 to 10) in the village.
#define FLAG_VALE_FIRST_WHIRLWIND_TARGET_CLEARED 0x90b
#define FLAG_VALE_SECOND_WHIRLWIND_TARGET_CLEARED 0x90c
#define FLAG_VALE_THIRD_WHIRLWIND_TARGET_CLEARED 0x90d

// Vale, around Isaac's house
// Back from Kalay, Mind Read showed that Dora is ill.
#define FLAG_READ_THOUGHTS_ABOUT_DORA 0x81c
// Isaac visited Dora in her sickbed and she got up.
#define FLAG_VISITED_SICK_DORA 0x81e

// Vale, houses and shops
// The item shopkeeper gave Isaac a Herb as a farewell gift.
#define FLAG_VALE_FAREWELL_HERB_RECEIVED 0x81b
// Back from Kalay, Garet surprised his family at home.
#define FLAG_GARET_CAME_HOME 0x81d

// Vale during the eruption
// Isaac and Garet came down Mt. Aleph and followed the elders into the sanctum.
#define FLAG_VALE_RETURNED_FROM_MT_ALEPH 0x879

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
#define FLAG_SCENE_TEMP_41 0x241 // Vale: the plaza's watching villager paused
#define FLAG_SCENE_TEMP_42 0x242 // Vale: Kraden's gate stands open
#define FLAG_SCENE_TEMP_50                                                     \
  0x250 // Vault interiors: the leader is beside the Bone

#endif
