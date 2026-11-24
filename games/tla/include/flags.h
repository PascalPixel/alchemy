#ifndef ALCHEMY_FLAGS_H
#define ALCHEMY_FLAGS_H

/// @file
/// Game flags, named where their meaning is shown.

/// Set while the party member whose actor id is `actor` (ACTOR_FELIX to
/// ACTOR_PIERS, 0 to 3 for Isaac's party) is in the party.
#define FLAG_IN_PARTY(actor) (actor)

/// Set once the summon tablet of `summon` (SUMMON_ZAGAN to SUMMON_IRIS) has
/// taught it: a tablet's flag is its summon's number (gSceneItems).
#define FLAG_SUMMON_TABLET(summon) (summon)

/// The first of the 80 Djinn flags, DJINNI_FLINT's.
#define FLAG_DJINN_FIRST 0x30
/// Set once the party has `djinni` (djinn_ids.h), whose DjinniId counts from
/// FLAG_DJINN_FIRST.
#define FLAG_DJINNI(djinni) (FLAG_DJINN_FIRST + (djinni))
/// The first of the 80 hiding flags after the Djinn's own, DJINNI_FLINT's.
#define FLAG_DJINN_HIDING_FIRST 0x80
/// Set while `djinni` is hiding, its DjinniId counting from
/// FLAG_DJINN_HIDING_FIRST: Djinn_ResolvePending sets it after a lost or
/// fled Djinni battle.
#define FLAG_DJINNI_HIDING(djinni) (FLAG_DJINN_HIDING_FIRST + (djinni))
/// The last of the 96 flags from FLAG_DJINN_HIDING_FIRST that Scene_Arrive
/// clears on every arrival: the 80 hiding flags and the 16 after them.
#define FLAG_ARRIVAL_CLEARED_LAST 0xdf

/// Unused: The Broken Seal's flag for its prologue, while Isaac and Garet are
/// children. Nothing in this game sets or reads it; the debug battle menu
/// clears it.
#define FLAG_CHILDHOOD 0x20
/// Unused: The Broken Seal's flag for the tunic young Isaac wears. Nothing in
/// this game sets it; were it set, World Map actor placements would keep their
/// full size (Event_SpawnObjectTable).
#define FLAG_ISAAC_WEARS_TUNIC 0x21

/// Set as Isaac's party joins Felix's (Party_AddIsaacsParty); a file saved
/// after it is "Post-Reunion Data" (MsgSavePostReunion).
#define FLAG_REUNION 0x22

/// The first of the flags that mark each enemy defeated, by enemy id, 0x600 to
/// 0x7ff (battle_internal.h's ENEMY_DEFEATED_FLAG): BattleEnemy_RecordDefeat
/// sets them, and the monster battles choose only defeated enemies.
#define FLAG_ENEMY_DEFEATED_FIRST 0x600

// The Broken Seal's clear data, as "Transfer data from Golden Sun" brings it
// in by link or by password (DataTransfer_Apply).
/// Set once data was transferred by link.
#define FLAG_TRANSFERRED_BY_LINK 0x2c
/// Set once data was transferred by password.
#define FLAG_TRANSFERRED_BY_PASSWORD 0x2d
/// Set once data was transferred, either way.
#define FLAG_DATA_TRANSFERRED 0x2f

/// Set when the Clear scene's "Play on HARD level? (Monsters are stronger)"
/// is answered yes (MsgPlayOnHardLevel): outside the monster battles each
/// enemy is strengthened as a battle sets it up (BattleUnit_Strengthen).
#define FLAG_HARD_LEVEL 0x2e
/// The first of the 28 flags of The Broken Seal's Djinn, seven to an element
/// in element order: set for each Djinni the transferred data held, or for
/// each DataTransfer_GiveDefaultDjinn handed out.
#define FLAG_TRANSFERRED_DJINN_FIRST 0x800
/// The Broken Seal's Djinn of each element.
#define TRANSFERRED_DJINN_PER_ELEMENT 7
/// The six flags DataTransfer_Apply copies from The Broken Seal's
/// (gDataTransfer_Flags), named for those.
#define FLAG_TRANSFERRED_DODONPA_LOCKED_UP 0x820
#define FLAG_TRANSFERRED_WON_COLOSSO 0x821
/// The opposite of The Broken Seal's FLAG_HSU_RESCUED_WITHOUT_PARTY: set when
/// that was clear, so Isaac saved Hsu himself, whom this game's text calls
/// Ulmuch ("He helped Ulmuch. Ulmuch was trapped, but Isaac saved him."); only
/// then does Feizhi come to Champa looking for Isaac.
#define FLAG_TRANSFERRED_ISAAC_SAVED_ULMUCH 0x822
#define FLAG_TRANSFERRED_VISITED_SICK_DORA 0x824
#define FLAG_TRANSFERRED_VAULT_WARNED_OF_ESCAPED_THIEVES 0x825
/// How many flags gDataTransfer_Flags lists.
#define TRANSFERRED_FLAG_COUNT 6
/// Flags of The Broken Seal's save, by its own names and numbers:
/// DataTransfer_Apply reads them (gDataTransfer_Flags), and the Clear scene
/// sets them from a password (gClear_ImportedFlags).
#define FLAG_BROKEN_SEAL_VISITED_SICK_DORA 0x81e
#define FLAG_BROKEN_SEAL_VAULT_WARNED_OF_ESCAPED_THIEVES 0x868
#define FLAG_BROKEN_SEAL_HSU_RESCUED_WITHOUT_PARTY 0x8b3
#define FLAG_BROKEN_SEAL_CROSSBONE_ISLE_DEADBEARD_DEFEATED 0x8d1
#define FLAG_BROKEN_SEAL_DODONPA_LOCKED_UP 0x941
#define FLAG_BROKEN_SEAL_WON_COLOSSO 0x951

/// The system flags: what the field, battles and menus keep about their own
/// working, shared with the scenes that set or test them, rather than the
/// story's progress.
#define FLAG_SYSTEM_FIRST 0x100
#define FLAG_SYSTEM_LAST 0x1ff ///< the last of the 256 system flags
/// The next scene change is silent: the scene loader clears it in place of
/// playing its sound, once.
#define FLAG_SKIP_SCENE_CHANGE_SOUND 0x101
/// Set as a battle starts, cleared as a field scene starts.
#define FLAG_IN_BATTLE 0x103
/// Set while the item menu's info window holds a message ItemMenu_ShowMessage
/// left there; the item and Psynergy menus then leave the window be once and
/// clear it, in place of drawing the selection's description.
#define FLAG_INFO_WINDOW_HOLDS_MESSAGE 0x151
/// Set while the field menu is open; scenes hold their moving actors still.
#define FLAG_MENU_OPEN 0x106
/// While set, opening the field menu or pressing a shortcut raises the
/// scene's menu trigger instead.
#define FLAG_MENUS_RAISE_TRIGGER 0x107
/// Set while a field Psynergy's tint holds the palettes, from
/// Field_StartColorTransition until Field_StartPaletteTransition.
#define FLAG_PSYNERGY_TINTED 0x108
/// Set, the highest of these picks the tint a field Psynergy fades
/// toward, gFieldEffect_TargetColors[n], in place of the frame counter's pick
/// (FieldPsynergy_FadeToCastColor).
#define FLAG_PSYNERGY_TINT(n) (0x148 + (n))
/// Set while the field sequence handles its requests, and cleared while the
/// party walks (FieldSequence_Run).
#define FLAG_FIELD_HANDLING_REQUESTS 0x104
/// The next field scene starts where the party was, as after a battle: the
/// scene's flags are not reset, the actors come back from their snapshot,
/// and the music saved in gMainRoutineState.returnMusic resumes if there is
/// one (a random battle clears it, so the scene's own music plays).
/// FieldSequence_Run clears it once the scene starts.
#define FLAG_KEEP_PARTY_POSITION 0x109
/// Sunshine the blacksmith forges for the party: Yallam sets it while the party
/// carries a rusty weapon or a forging material, and keeps it while he works
/// or his wife holds what he made.
#define FLAG_FORGE_OPEN 0x10f
/// The next scene keeps the music that is playing; main_routine.c clears
/// FLAG_KEEP_MUSIC_ONCE as it does, and leaves FLAG_KEEP_MUSIC_HELD set.
#define FLAG_KEEP_MUSIC_ONCE 0x11a
/// As FLAG_KEEP_MUSIC_ONCE, for as long as it is set.
#define FLAG_KEEP_MUSIC_HELD 0x11b
#define FLAG_WORLD_MAP_LEFT 0x11c ///< the current scene is not the World Map
/// The leader stands in the corner of the World Map that the map screen
/// leaves out (MapScreen_GetLeaderPoint): "This place doesn't seem to
/// appear on the map..."
#define FLAG_LEADER_OFF_MAP_SCREEN 0x11e
/// The leader falls into the next scene from above, as after jumping down a
/// ledge into it.
#define FLAG_ARRIVE_FALLING 0x122
/// The party is in a town; the scene loader sets and clears it.
#define FLAG_IN_TOWN 0x123
/// The file was loaded to restart at its last Sanctum.
#define FLAG_SAVE_RESTARTED_AT_SANCTUM 0x13e
/// The file was recovered at its last Sanctum.
#define FLAG_SAVE_RECOVERED_AT_SANCTUM 0x13f
/// Play the kind's usual cast: the caster's glow, animation and sound
/// (FieldPsynergy_StartCast). Field_UsePsynergy sets it for each use and
/// clears it when the after trigger plays its own cast.
#define FLAG_PLAY_PSYNERGY_CAST 0x140
/// Aim the field Psynergy at the actor ahead rather than run the kind's
/// untargeted body; Field_UsePsynergy keeps it set only when a trigger
/// applies to what the leader faces.
#define FLAG_PSYNERGY_TARGET 0x141
/// The next layout keeps the palette and tiles already loaded, once
/// (MapGrid_LoadMap).
#define FLAG_KEEP_LAYER_GRAPHICS 0x170
/// With it set, holding B on the World Map moves the party at 4 pixels a
/// frame. Nothing in the game sets it.
#define FLAG_FAST_WALK 0x17f
#define FLAG_ITEM_USED_UP 0x143 ///< the item just used is removed afterwards
#define FLAG_PSYNERGY_FROM_ITEM 0x145 ///< the Psynergy comes from an item
/// Set while the flying ship hovers on the World Map: Hover cast aboard it sets
/// it, the ship takes off and the World Map announces "Hover"
/// (WorldMap_SetUpFlyingShip); taking off with the dash keys clears it.
#define FLAG_SHIP_HOVERING 0x147
/// While set, the keys that end Sand do nothing (Sand_CheckEndKeys): Ankohl
/// Ruins sets it as the great face's raft passes rows 31 to 35, and clears it
/// at row 37; Scene_Arrive clears it on every arrival.
#define FLAG_SAND_CANNOT_END 0x146
/// While set, the save screen refuses to save with "Cannot save here".
#define FLAG_CANNOT_SAVE_HERE 0x17c
/// While set, the save screen refuses to save with "This is no place for
/// that!": the scenes set it while the party rides a moving platform, lift or
/// raft.
#define FLAG_NO_PLACE_TO_SAVE 0x17d
/// Set when the cleared game's link lobby goes to the Battle Arena: there the
/// Pause menu offers only Change settings; Scene_Arrive clears it.
#define FLAG_IN_BATTLE_ARENA 0x17e
/// Set while Start is pressed in the Psynergy menu, which ends its waits.
#define FLAG_MENU_START_PRESSED 0x150
/// The leader left through an EXIT_STYLE_STAIRS_UP exit; the next scene plays
/// the leader's arrival, unless its setup clears this first.
#define FLAG_ARRIVAL_STAIRS_UP 0x120
/// As FLAG_ARRIVAL_STAIRS_UP, for an EXIT_STYLE_STAIRS_DOWN exit.
#define FLAG_ARRIVAL_STAIRS_DOWN 0x121
/// Each lighthouse's scenes set its flag on every arrival, and Scene_Arrive
/// clears all four when the party enters another area, so a flag is set only
/// while the party is inside.
#define FLAG_IN_VENUS_LIGHTHOUSE 0x110
#define FLAG_IN_MERCURY_LIGHTHOUSE 0x111
#define FLAG_IN_MARS_LIGHTHOUSE 0x112
#define FLAG_IN_JUPITER_LIGHTHOUSE 0x113
/// The scene shows its area's name once it is on screen; entering a new area
/// sets it.
#define FLAG_SHOW_AREA_NAME 0x12f
/// Encounters are rolled even where FLAG_NO_ENCOUNTERS,
/// FLAG_NO_ENCOUNTERS_IN_AREA, a zone of 0 or Cloak would stop them
/// (Encounter_CountStepsAndSelectFormation).
#define FLAG_IGNORE_ENCOUNTER_CHECKS 0x15f
/// Set before the party acts on the field; still set afterwards, nothing came
/// of it and the game says so. An event that acts clears it.
#define FLAG_NOTHING_HAPPENS 0x142
/// Retreat is blocked, whether cast or used from an item; the scenes that
/// forbid Retreat set it.
#define FLAG_RETREAT_BLOCKED 0x144
/// No random encounters while set (Encounter_CountStepsAndSelectFormation).
#define FLAG_NO_ENCOUNTERS 0x160
/// No random battles until the party enters another area; Scene_Arrive clears
/// it with a new area.
#define FLAG_NO_ENCOUNTERS_IN_AREA 0x161
#define FLAG_DEBUG_BATTLE 0x162 ///< set before every debug battle
/// In debug mode, steps on the field are not processed.
#define FLAG_DEBUG_STEPS_OFF 0x163
/// Holds the field's weather still, its particles and its next burst, while a
/// menu is open.
#define FLAG_WEATHER_HELD 0x166
/// Set while the Lure Cap is worn: random encounters come twice as fast.
#define FLAG_LURE_CAP 0x167
/// Auto battle: every living unit, party and enemies, makes a plain attack
/// each turn with no command menu; the debug battle menu sets it.
#define FLAG_AUTO_BATTLE 0x16a
#define FLAG_CHOOSING_COMMANDS 0x16b ///< set while the commands are chosen
/// The next battle is a link battle.
#define FLAG_LINK_BATTLE 0x16c
/// In debug mode, with it set, holding R as an action is planned defeats every
/// enemy, and R with Select the party (BattleActionRun_Build).
#define FLAG_DEBUG_BATTLE_KEYS 0x16d
/// The Djinn lesson: its battle and the parts of it the Djinn menu plays.
#define FLAG_DJINN_LESSON 0x16e
/// Set when Up is held as Echo's Djinn lesson begins on the World Map: the
/// lesson then takes its second set of messages, opening "You look like you
/// already know a little about us, huh?" (MsgDjinnGiveTutorialB and its kin).
#define FLAG_ALREADY_KNOWS_DJINN 0x175
/// Set while the party sails the ship on the World Map: world_map.c sets it as
/// the party boards, and the main routine keeps it while an actor other than
/// the leader is the one events control (MainRoutineState.ownerOverride). The
/// World Map takes MUS_TO_THE_OPEN_SEA while it is set.
#define FLAG_SAILING 0x176
/// With FLAG_DJINN_LESSON set, this and FLAG_DJINN_LESSON_PART_B choose the
/// part of the lesson the Djinn menu plays.
#define FLAG_DJINN_LESSON_PART_A 0x16f
#define FLAG_DJINN_LESSON_PART_B 0x171
/// A battle against monsters with formations matched to the party's level, and
/// no spoils.
#define FLAG_MONSTER_BATTLE 0x173
/// Lets the monster battles choose enemies above level 3 the party has never
/// defeated.
#define FLAG_MONSTER_BATTLE_ANY_ENEMY 0x174
/// A linked battle has been arranged in the Battle Arena, with
/// FLAG_LINK_BATTLE: only the first three party members take part.
#define FLAG_LINK_BATTLE_ARRANGED 0x172
/// Holds the field's palette blend still while a menu is open.
#define FLAG_PALETTE_BLEND_HELD 0x152

/// A scene's own flags, cleared on every ordinary entry (Scene_Arrive):
/// puzzles reset when the party walks back in.
#define FLAG_SCENE_TEMP_FIRST 0x200
#define FLAG_SCENE_TEMP_LAST 0x2ff ///< the last of the scene's 256 flags
/// An area's flags, kept while the party moves between the scenes of one area
/// and cleared when it enters another. Each area uses the numbers for its own,
/// so the scenes name them.
#define FLAG_AREA_TEMP_FIRST 0x300
#define FLAG_AREA_TEMP_LAST 0x3ff ///< the last of the area's 256 flags
// Byte flags, read and written eight at a time.
/// The party members the linked opponent sent (the link lobby).
#define FLAG_LINK_PARTY_COUNT 0x3f0
/// Monster Battles won in a row, added to the monsters' level; -1 after the
/// series ends.
#define FLAG_MONSTER_BATTLE_WINS 0x3f8

/// Set as Madra's gate guards let the party through (Madra_InspectPartyAtGate,
/// "You there! Hold your ground!"); then the worker no longer blocks the hole
/// into the catacombs and Daila skips Alex's event.
#define FLAG_MADRA_GATE_INSPECTED 0x910
/// Set at the end of the jail scene where Piers wards off his interrogator
/// with Frost (MadraRooms_InterrogatePiers); then the two witnesses of the
/// ice stand outside the jail and the lady can grant passage to Osenia.
#define FLAG_PIERS_INTERROGATED 0x911
/// Set as the lady grants passage over the drawbridge ("Then I hereby grant my
/// permission to these four travelers!"); the drawbridge guards let the party
/// by once it is set.
#define FLAG_OSENIA_PASSAGE_GRANTED 0x913
/// Set as Madra's mayor gives Felix the Cyclone Chip, the reward he promised
/// for the help with the boat.
#define FLAG_MADRA_CYCLONE_CHIP_GIVEN 0x918
/// Set while the Trial Road holds the party's equipment, from the race's start
/// (InventorySnapshot_Save) to its end (InventorySnapshot_Restore), as the
/// chests take the items the party puts in them ("place items in these
/// chests"); the Trial Road's music rule and its triggers test it.
#define FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT 0x9ff
/// The ten items of the Trial Road's stages (SCENE_TRIAL_ROAD_STAGES in
/// scene_items.c), set as each is taken; InventorySnapshot_Save clears them as
/// a race starts, so that every race finds them again.
#define FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST 0xe50
#define FLAG_TRIAL_ROAD_STAGE_ITEM_LAST 0xe59
/// Set once Sheba's Whirlwind has made the sand vanish from the purple rock
/// in Shaman Village, Moapa's first test ("The sand vanished!").
#define FLAG_SHAMAN_VILLAGE_SAND_CLEARED 0x94c
/// Set as Moapa comes out of his cabin in Shaman Village to meet the party.
#define FLAG_SHAMAN_VILLAGE_MOAPA_MET 0x94f
/// Set once the party has shown Moapa the Shaman's Rod.
#define FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN 0x9e1
/// Set as the party comes back from beating Moapa on the Trial Road; Shaman
/// Village's inn and sanctum serve the party from then on.
#define FLAG_MOAPA_BEATEN 0x9fe

/// Set once the old werewolf of Garoh's inn has asked the party whether they
/// learned Reveal; he then only sighs about his wasted years.
#define FLAG_GAROH_ASKED_ABOUT_REVEAL 0x91a
/// Set as the party wakes at Garoh's inn after the night with Maha
/// (GarohRooms_WakeAtInn): from then on it is day in Garoh and its people
/// no longer hide that they are werewolves.
#define FLAG_GAROH_SECRET_SHARED 0x91b

/// Set while it is night in Garoh: its scenes fade the palette to night and
/// play a howl every 1,024 frames, and its inn shows its night layers. The
/// party walks into the night as it climbs to the village, and the morning
/// after the night with Maha (FLAG_GAROH_SECRET_SHARED) clears it.
#define FLAG_GAROH_NIGHT 0x310

/// Set as Briggs sails from Alhafra in the boat he bought with the Ankohl
/// jewels, leaving the party on the shore; from then on Champa is fed and
/// talks of his return.
#define FLAG_BRIGGS_SAILED_FROM_ALHAFRA 0x97f

/// The first of four flags, one for each patch of overgrowth in the Apojii
/// Islands' cave that Whirlwind has blown away, by its placed actor id.
#define FLAG_APOJII_CAVE_FOLIAGE_FIRST 0x8e9
#define FLAG_APOJII_CAVE_FOLIAGE_BLOWN(actor)                                  \
  (FLAG_APOJII_CAVE_FOLIAGE_FIRST - ACTOR_FIRST_PLACED + (actor))

/// Set as the mayor of Madra arrives on the ship at Alhafra's eastern
/// harbour, or once the party has fought Briggs there.
#define FLAG_MAYOR_ARRIVED_AT_THE_SHIP 0x8aa
/// Set as Briggs is caught on the boat he bought at Alhafra; the town then
/// thanks the party and talks of pirates.
#define FLAG_BRIGGS_CAUGHT 0x8ab
/// Set as Chaucha and Eoleo leave the jail after visiting Briggs.
#define FLAG_FAMILY_VISITED_BRIGGS 0x8bd
/// Set as Felix comes upon the four Kibombo warriors in their hideout in the
/// Gondowan Cliffs; back in Madra, Felix is asked "It was them, wasn't it?"
#define FLAG_KIBOMBO_WARRIORS_FOUND 0x98f
/// Set as the crew sights Menardi's ship from the Lemurian ship's deck
/// (LemurianShipDeck_SightMenardisShip), at once there if the message of the
/// Gabomba was carried first, or as Karst leaves Madra after coming to find
/// the party there; the deck's sighting then runs no more.
#define FLAG_MENARDIS_SHIP_SEEN 0x980
/// Set as Piers tells Felix in Madra that they will sail for the Eastern Sea
/// in his ship, or as the party reaches his ship from the Lemurian ship's
/// deck; Kraden's farewell in Madra is shorter after it.
#define FLAG_PIERS_SETTING_SAIL 0x981
/// Set once the party has said its farewells in Madra and gathered to leave;
/// Karst then comes into the town, and the deck's departure runs no more.
#define FLAG_MADRA_FAREWELL_SAID 0x989
/// Set once Burst has broken the last block on the Alhafran boat's mast, so
/// the mast is ready for repairs; the town then wonders whether Briggs was a
/// pirate at all.
#define FLAG_ALHAFRA_MAST_CLEARED 0x8a7
/// Set as Alex greets the party at Alhafra's inn.
#define FLAG_ALHAFRA_MET_ALEX 0x8b2
/// Set as Alex meets the party again by Obaba's cliff in Champa ("I see you've
/// caught up with me again", MsgChampaISeeYouveCaughtUp); Alhafra's inn is
/// without him from then on ("That cute boy next door left already").
#define FLAG_CHAMPA_MET_ALEX 0x971
/// Set as the party from Madra, its mayor among them, reaches Alhafra across
/// the Yampi Desert.
#define FLAG_ALHAFRA_MADRANS_ARRIVED 0x8b3
/// Set as Alhafra's sailboat leaves its harbour, with
/// FLAG_BRIGGS_SAILED_FROM_ALHAFRA; the town then mourns its only ship.
#define FLAG_ALHAFRA_SHIP_GONE 0x90f
/// Set as Alhafra's inn tells that someone named Isaac picked Crossbone Isle
/// clean, which it tells only while FLAG_ISAAC_CLEARED_CROSSBONE_ISLE is set.
#define FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE 0x903
/// Copied from The Broken Seal's FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED by
/// DataTransfer_Apply: Isaac beat Deadbeard on the Pirate Ship. Alhafra's inn
/// tells of Isaac picking Crossbone Isle clean only while it is set.
#define FLAG_ISAAC_CLEARED_CROSSBONE_ISLE 0x823

/// Set as the Great Serpent of Mikage is slain in Gaia Rock; Izumo then
/// celebrates and takes its tables for after.
#define FLAG_SERPENT_SLAIN 0x927
/// Set once Izumo has recognized the party as the warriors who slew the
/// serpent (IzumoRooms_RecognizeTheHeroes).
#define FLAG_IZUMO_RECOGNIZED_THE_HEROES 0x928
/// Set once the party has given Lady Uzume the dancing doll of Mt. Mikage
/// (IzumoRooms_GiveUzumeTheDoll).
#define FLAG_UZUME_HAS_THE_DOLL 0x92c

/// Set as the party throws the boy Tavi the rope at the Shrine of the Sea God;
/// Daila's houses then take their later tables.
#define FLAG_TAVI_RESCUED 0x873

/// Set as Move sends the stump on the Gondowan Cliffs leaping into the stream,
/// where it stands as a step.
#define FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER 0x884
/// Set as Whirlwind blows away the weeds before the cave of Kibombo
/// warriors.
#define FLAG_GONDOWAN_CLIFFS_WEEDS_BLOWN 0x885
/// Set as Scoop digs up the geyser in the yellow sand.
#define FLAG_GONDOWAN_CLIFFS_GEYSER_DUG 0x886
/// Set once the leader takes the Lemurian Ship's tiller: the ship can sail, the
/// world map marks its mooring and the party comes back to it there.
#define FLAG_SHIP_CAN_SAIL 0x8de
/// Set as Grind breaks up the rock that blocks the way south.
#define FLAG_GONDOWAN_CLIFFS_ROCK_GROUND 0x8e6

/// Set once the pillar inside Jupiter Lighthouse's main gates stands on the
/// button beside them: the gates stay open.
#define FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED 0xa01

/// The Gabomba ceremony at Kibombo: FLAG_GABOMBA_CEREMONY_UNDER_WAY is set as
/// it begins (the music rules play its music while set) and cleared once the
/// message is carried; the others are set as each step happens.
#define FLAG_GABOMBA_CEREMONY_UNDER_WAY 0x8f2
#define FLAG_GABOMBA_RESPONDED 0x8fb
#define FLAG_GABOMBA_CEREMONY_ENDED 0x8ef
#define FLAG_AKAFUBU_OFFERED_HELP 0x8fd
/// Set as Felix carries the message of the Gabomba to Akafubu after the
/// ceremony at the Gabomba Statue; Madra, Alhafra and the Kibombo then speak of
/// the raids and the Black Crystal.
#define FLAG_GABOMBA_MESSAGE_CARRIED 0x8ff
/// Set as Felix's party first meets Piers in Kibombo (Kibombo_MeetPiers);
/// Jenna, Sheba and Kraden then stand at Gondowan Cliffs' hideout.
#define FLAG_KIBOMBO_PIERS_MET 0x8f3
/// Set as the Gabomba's charges drive the party back from the statue, cleared
/// as Akafubu ends the ceremony; while it is set the music rules
/// choose silence for their groups 31 and 32 (cue_rules.s).
#define FLAG_GABOMBA_MUSIC_SILENCED 0x8f7
/// Set as the ceremony sends the party back to Kibombo; the Gabomba Statue's
/// arrow room reads it once and clears it.
#define FLAG_GABOMBA_BACK_FROM_CEREMONY 0x8fc
/// Where Retreat takes the party from the Gabomba Statue's lower rooms: set as
/// it comes in to the pinion room from outside (FLAG_GABOMBA_RETREAT_TO_PINION)
/// or to the idol's chamber by its third entrance
/// (FLAG_GABOMBA_RETREAT_TO_IDOL), each clearing the other.
#define FLAG_GABOMBA_RETREAT_TO_PINION 0xa27
#define FLAG_GABOMBA_RETREAT_TO_IDOL 0xa28
/// Set while Kibombo is shown at its ceremony night
/// (Kibombo_InitializeCeremony), cleared by day and once the message is
/// carried: the houses' and the statue's exits lead to KibomboCeremony while it
/// is set.
#define FLAG_KIBOMBO_AT_CEREMONY 0x86b

/// Set as the party sets out on the ship Hamma gave wings to at Atteka Inlet:
/// from then on the ship flies, with no encounters at sea.
#define FLAG_SHIP_FLIES 0x8df
/// Set once the winged ship has first taken off from the World Map
/// (WorldMap_TakeOff).
#define FLAG_SHIP_TOOK_OFF 0xa78

/// Set once the pillar on the floor below Jupiter Lighthouse's aerie stands on
/// the button by its door: the door stays open, a shortcut.
#define FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED 0xa19

/// Set as Hover cast on the raised platform in Jupiter Lighthouse wakes the
/// lighthouse: its hover platforms and blocks rise in every room.
#define FLAG_JUPITER_LIGHTHOUSE_ACTIVATED 0xa00

/// Set as the left tower's archer statue shoots its arrow at the left angel on
/// Jupiter Lighthouse's aerie, which starts to lift its half of the lid.
#define FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT 0xa0a
/// The same for the right tower's archer and the right angel.
#define FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT 0xa0b
/// Set once both angels are shot and the lid is off the well on Jupiter
/// Lighthouse's aerie, where Isaac's party is found.
#define FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED 0xa1f
/// Set as the party comes out by the aerie to watch Karst and Agatio fight
/// Isaac and Ivan; cleared once the two have fallen.
#define FLAG_JUPITER_LIGHTHOUSE_FIGHT_WATCHED 0xa24
/// Set as Alex stops the party on its way to the fight and heals it; while
/// set there are no encounters and Retreat is blocked.
#define FLAG_JUPITER_LIGHTHOUSE_ALEX_HEALED_THE_PARTY 0xa25
/// Set once Isaac and Ivan have fallen to Karst and Agatio and gone up to the
/// aerie, where the two wait for Felix.
#define FLAG_ISAAC_AND_IVAN_FELL 0xa23
/// Set as the Jupiter Star lights the beacon; Contigo's people speak of it.
#define FLAG_JUPITER_BEACON_LIT 0xa21
/// Set in the Mars Lighthouse's aerie as the talk after the Doom Dragon's
/// battle ends, just before the voice asks for the Mars Star; Prox's people say
/// "only Mars Lighthouse still needs lighting" while it is clear and thank the
/// party for lighting the beacon once it is set.
#define FLAG_MARS_BEACON_LIT 0xa3d
/// Set as the party leaves the Mercury Lighthouse at the end: the beacon's
/// fire burns at the aerie from then on.
#define FLAG_MARS_BEACON_BURNING 0xa3c
/// Set in the dragon hall once the Mars Star is set in the statue's face: the
/// lighthouse turns to fire and its ice is gone, and battles there take the
/// dragon hall's backdrop A in place of B.
#define FLAG_MARS_LIGHTHOUSE_THAWED 0xa30
/// Set as the party, back in Prox, overhears the two warriors who could not
/// find Felix's parents to set them free, as Puelle ordered.
#define FLAG_HEARD_OF_PARENTS 0x9c0
/// Set once Puelle has told the clan he is going to the lighthouse and they
/// have begged him to let them go in his stead.
#define FLAG_CLAN_SPOKE_OF_LIGHTHOUSE 0x9c1
/// Set once the party has met Puelle, the chieftain of Prox.
#define FLAG_MET_PUELLE 0x9c2
/// Set after the ending, once the clan in Prox's houses has thanked the party
/// for lighting the fires of the lighthouse beacon.
#define FLAG_THANKED_FOR_BEACON 0x9c5
/// Set at the end of the reunion in Contigo, where the two parties hear what
/// happened on Mt. Aleph; the shipwrights at Atteka Inlet then wait with the
/// wings.
#define FLAG_REUNION_AT_CONTIGO 0x9d0

/// Set once Felix has woken on Idejima's shore.
#define FLAG_IDEJIMA_FELIX_WOKE 0x844
/// Set as Kraden, Jenna and Sheba, each in turn, join Felix on Idejima.
#define FLAG_IDEJIMA_KRADEN_JOINED 0x863
#define FLAG_IDEJIMA_JENNA_JOINED 0x864
#define FLAG_IDEJIMA_SHEBA_JOINED 0x865
/// Set once the party has wondered where Alex has gone.
#define FLAG_IDEJIMA_ASKED_WHERE_ALEX_WENT 0x867

/// Set once the villager at Daila's west edge has asked whether the party has
/// been there before.
#define FLAG_DAILA_ASKED_IF_BEEN_BEFORE 0x843
/// Set as Alex leaves Daila for Madra.
#define FLAG_DAILA_MET_ALEX 0x84b
/// Daila's area flag: set as Scoop digs up the Psy Crystal under its puddle,
/// and cleared once the party enters another area. That the crystal was taken
/// is kept by its gSceneItems row's flag, 0xf5e.
#define FLAG_DAILA_PSY_CRYSTAL_DUG FLAG_AREA_TEMP_00
/// Set once the healer of Daila's Sanctum has sent the party to Master Poi;
/// after that the Sanctum serves them across its counter.
#define FLAG_DAILA_HEALER_SENT_TO_POI 0x847
/// Set once the party has taken the Sea God's Tear from Daila's sanctuary.
#define FLAG_SEA_GODS_TEAR_TAKEN 0xa20
/// Set once the party has taken what Cyclone uncovers in Izumo's bushes: an
/// Antidote at each of the first two spots, a Lucky Medal at the third.
#define FLAG_IZUMO_FIRST_ANTIDOTE 0xf9f
#define FLAG_IZUMO_SECOND_ANTIDOTE 0xfa0
#define FLAG_IZUMO_LUCKY_MEDAL 0xfa1
/// Set once the party has taken the Mint Cyclone uncovers in the grass before
/// Jupiter Lighthouse.
#define FLAG_JUPITER_LIGHTHOUSE_MINT 0xfe2
/// Set once the party has taken the Burst Brooch in Tundaria Tower; after
/// that, searching the crystal round the Center Prong opens the field menu.
#define FLAG_TUNDARIA_TOWER_BURST_BROOCH 0x949
/// Set once the party has taken the Center Prong from Tundaria Tower.
#define FLAG_CENTER_PRONG_TAKEN 0x945
/// Set once the party has found the Cloud Brand in Gaia Rock.
#define FLAG_CLOUD_BRAND_FOUND 0xe41
/// Set once the party has taken the Dancing Idol from Gaia Rock's altar.
#define FLAG_DANCING_IDOL_TAKEN 0xf74
/// Set once the party has taken the Mint Cyclone uncovers in the bushes by the
/// Gabomba Catacombs' statue.
#define FLAG_GABOMBA_CATACOMBS_MINT_TAKEN 0xf53
/// Set once the party has taken the Tomegathericon from the seated idol of the
/// Gabomba Catacombs.
#define FLAG_TOMEGATHERICON_TAKEN 0xf93
/// Set once the party has taken the Bramble Seed in Contigo's bushes.
#define FLAG_CONTIGO_BRAMBLE_SEED_TAKEN 0xfc7
/// Set once the party has caught Kalt Island's Apple.
#define FLAG_KALT_ISLAND_APPLE_TAKEN 0xf5d
/// Set once the party has dug up the Hard Nut at Lemuria's fifth soft spot.
#define FLAG_LEMURIA_HARD_NUT_DUG 0xfbb
/// Set once the party has dug up the Golem Core under Loho's rock.
#define FLAG_LOHO_GOLEM_CORE_DUG 0xff7

/// Scene flags, FLAG_SCENE_TEMP_00 to FLAG_SCENE_TEMP_FF: a scene says beside
/// its uses what each one holds there.
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
#define FLAG_SCENE_TEMP_10 0x210
#define FLAG_SCENE_TEMP_11 0x211
#define FLAG_SCENE_TEMP_12 0x212
#define FLAG_SCENE_TEMP_13 0x213
#define FLAG_SCENE_TEMP_14 0x214
#define FLAG_SCENE_TEMP_15 0x215
#define FLAG_SCENE_TEMP_16 0x216
#define FLAG_SCENE_TEMP_17 0x217
#define FLAG_SCENE_TEMP_18 0x218
#define FLAG_SCENE_TEMP_19 0x219
#define FLAG_SCENE_TEMP_1A 0x21a
#define FLAG_SCENE_TEMP_1B 0x21b
#define FLAG_SCENE_TEMP_1C 0x21c
#define FLAG_SCENE_TEMP_1D 0x21d
#define FLAG_SCENE_TEMP_1E 0x21e
#define FLAG_SCENE_TEMP_1F 0x21f
#define FLAG_SCENE_TEMP_20 0x220
#define FLAG_SCENE_TEMP_21 0x221
#define FLAG_SCENE_TEMP_22 0x222
#define FLAG_SCENE_TEMP_23 0x223
#define FLAG_SCENE_TEMP_24 0x224
#define FLAG_SCENE_TEMP_25 0x225
#define FLAG_SCENE_TEMP_26 0x226
#define FLAG_SCENE_TEMP_27 0x227
#define FLAG_SCENE_TEMP_28 0x228
#define FLAG_SCENE_TEMP_29 0x229
#define FLAG_SCENE_TEMP_2A 0x22a
#define FLAG_SCENE_TEMP_2B 0x22b
#define FLAG_SCENE_TEMP_2C 0x22c
#define FLAG_SCENE_TEMP_2D 0x22d
#define FLAG_SCENE_TEMP_2E 0x22e
#define FLAG_SCENE_TEMP_2F 0x22f
#define FLAG_SCENE_TEMP_30 0x230
#define FLAG_SCENE_TEMP_31 0x231
#define FLAG_SCENE_TEMP_32 0x232
#define FLAG_SCENE_TEMP_33 0x233
#define FLAG_SCENE_TEMP_34 0x234
#define FLAG_SCENE_TEMP_35 0x235
#define FLAG_SCENE_TEMP_36 0x236
#define FLAG_SCENE_TEMP_37 0x237
#define FLAG_SCENE_TEMP_38 0x238
#define FLAG_SCENE_TEMP_39 0x239
#define FLAG_SCENE_TEMP_3A 0x23a
#define FLAG_SCENE_TEMP_3B 0x23b
#define FLAG_SCENE_TEMP_3C 0x23c
#define FLAG_SCENE_TEMP_3D 0x23d
#define FLAG_SCENE_TEMP_3E 0x23e
#define FLAG_SCENE_TEMP_3F 0x23f
#define FLAG_SCENE_TEMP_40 0x240
#define FLAG_SCENE_TEMP_41 0x241
#define FLAG_SCENE_TEMP_42 0x242
#define FLAG_SCENE_TEMP_43 0x243
#define FLAG_SCENE_TEMP_44 0x244
#define FLAG_SCENE_TEMP_45 0x245
#define FLAG_SCENE_TEMP_46 0x246
#define FLAG_SCENE_TEMP_47 0x247
#define FLAG_SCENE_TEMP_48 0x248
#define FLAG_SCENE_TEMP_49 0x249
#define FLAG_SCENE_TEMP_4A 0x24a
#define FLAG_SCENE_TEMP_4B 0x24b
#define FLAG_SCENE_TEMP_4C 0x24c
#define FLAG_SCENE_TEMP_4D 0x24d
#define FLAG_SCENE_TEMP_4E 0x24e
#define FLAG_SCENE_TEMP_4F 0x24f
#define FLAG_SCENE_TEMP_50 0x250
#define FLAG_SCENE_TEMP_51 0x251
#define FLAG_SCENE_TEMP_52 0x252
#define FLAG_SCENE_TEMP_53 0x253
#define FLAG_SCENE_TEMP_54 0x254
#define FLAG_SCENE_TEMP_55 0x255
#define FLAG_SCENE_TEMP_56 0x256
#define FLAG_SCENE_TEMP_57 0x257
#define FLAG_SCENE_TEMP_58 0x258
#define FLAG_SCENE_TEMP_59 0x259
#define FLAG_SCENE_TEMP_5A 0x25a
#define FLAG_SCENE_TEMP_5B 0x25b
#define FLAG_SCENE_TEMP_5C 0x25c
#define FLAG_SCENE_TEMP_5D 0x25d
#define FLAG_SCENE_TEMP_5E 0x25e
#define FLAG_SCENE_TEMP_5F 0x25f
#define FLAG_SCENE_TEMP_60 0x260
#define FLAG_SCENE_TEMP_61 0x261
#define FLAG_SCENE_TEMP_62 0x262
#define FLAG_SCENE_TEMP_63 0x263
#define FLAG_SCENE_TEMP_64 0x264
#define FLAG_SCENE_TEMP_65 0x265
#define FLAG_SCENE_TEMP_66 0x266
#define FLAG_SCENE_TEMP_67 0x267
#define FLAG_SCENE_TEMP_68 0x268
#define FLAG_SCENE_TEMP_69 0x269
#define FLAG_SCENE_TEMP_6A 0x26a
#define FLAG_SCENE_TEMP_6B 0x26b
#define FLAG_SCENE_TEMP_6C 0x26c
#define FLAG_SCENE_TEMP_6D 0x26d
#define FLAG_SCENE_TEMP_6E 0x26e
#define FLAG_SCENE_TEMP_6F 0x26f
#define FLAG_SCENE_TEMP_70 0x270
#define FLAG_SCENE_TEMP_71 0x271
#define FLAG_SCENE_TEMP_72 0x272
#define FLAG_SCENE_TEMP_73 0x273
#define FLAG_SCENE_TEMP_74 0x274
#define FLAG_SCENE_TEMP_75 0x275
#define FLAG_SCENE_TEMP_76 0x276
#define FLAG_SCENE_TEMP_77 0x277
#define FLAG_SCENE_TEMP_78 0x278
#define FLAG_SCENE_TEMP_79 0x279
#define FLAG_SCENE_TEMP_7A 0x27a
#define FLAG_SCENE_TEMP_7B 0x27b
#define FLAG_SCENE_TEMP_7C 0x27c
#define FLAG_SCENE_TEMP_7D 0x27d
#define FLAG_SCENE_TEMP_7E 0x27e
#define FLAG_SCENE_TEMP_7F 0x27f
#define FLAG_SCENE_TEMP_80 0x280
#define FLAG_SCENE_TEMP_81 0x281
#define FLAG_SCENE_TEMP_82 0x282
#define FLAG_SCENE_TEMP_83 0x283
#define FLAG_SCENE_TEMP_84 0x284
#define FLAG_SCENE_TEMP_85 0x285
#define FLAG_SCENE_TEMP_86 0x286
#define FLAG_SCENE_TEMP_87 0x287
#define FLAG_SCENE_TEMP_88 0x288
#define FLAG_SCENE_TEMP_89 0x289
#define FLAG_SCENE_TEMP_8A 0x28a
#define FLAG_SCENE_TEMP_8B 0x28b
#define FLAG_SCENE_TEMP_8C 0x28c
#define FLAG_SCENE_TEMP_8D 0x28d
#define FLAG_SCENE_TEMP_8E 0x28e
#define FLAG_SCENE_TEMP_8F 0x28f
#define FLAG_SCENE_TEMP_90 0x290
#define FLAG_SCENE_TEMP_91 0x291
#define FLAG_SCENE_TEMP_92 0x292
#define FLAG_SCENE_TEMP_93 0x293
#define FLAG_SCENE_TEMP_94 0x294
#define FLAG_SCENE_TEMP_95 0x295
#define FLAG_SCENE_TEMP_96 0x296
#define FLAG_SCENE_TEMP_97 0x297
#define FLAG_SCENE_TEMP_98 0x298
#define FLAG_SCENE_TEMP_99 0x299
#define FLAG_SCENE_TEMP_9A 0x29a
#define FLAG_SCENE_TEMP_9B 0x29b
#define FLAG_SCENE_TEMP_9C 0x29c
#define FLAG_SCENE_TEMP_9D 0x29d
#define FLAG_SCENE_TEMP_9E 0x29e
#define FLAG_SCENE_TEMP_9F 0x29f
#define FLAG_SCENE_TEMP_A0 0x2a0
#define FLAG_SCENE_TEMP_A1 0x2a1
#define FLAG_SCENE_TEMP_A2 0x2a2
#define FLAG_SCENE_TEMP_A3 0x2a3
#define FLAG_SCENE_TEMP_A4 0x2a4
#define FLAG_SCENE_TEMP_A5 0x2a5
#define FLAG_SCENE_TEMP_A6 0x2a6
#define FLAG_SCENE_TEMP_A7 0x2a7
#define FLAG_SCENE_TEMP_A8 0x2a8
#define FLAG_SCENE_TEMP_A9 0x2a9
#define FLAG_SCENE_TEMP_AA 0x2aa
#define FLAG_SCENE_TEMP_AB 0x2ab
#define FLAG_SCENE_TEMP_AC 0x2ac
#define FLAG_SCENE_TEMP_AD 0x2ad
#define FLAG_SCENE_TEMP_AE 0x2ae
#define FLAG_SCENE_TEMP_AF 0x2af
#define FLAG_SCENE_TEMP_B0 0x2b0
#define FLAG_SCENE_TEMP_B1 0x2b1
#define FLAG_SCENE_TEMP_B2 0x2b2
#define FLAG_SCENE_TEMP_B3 0x2b3
#define FLAG_SCENE_TEMP_B4 0x2b4
#define FLAG_SCENE_TEMP_B5 0x2b5
#define FLAG_SCENE_TEMP_B6 0x2b6
#define FLAG_SCENE_TEMP_B7 0x2b7
#define FLAG_SCENE_TEMP_B8 0x2b8
#define FLAG_SCENE_TEMP_B9 0x2b9
#define FLAG_SCENE_TEMP_BA 0x2ba
#define FLAG_SCENE_TEMP_BB 0x2bb
#define FLAG_SCENE_TEMP_BC 0x2bc
#define FLAG_SCENE_TEMP_BD 0x2bd
#define FLAG_SCENE_TEMP_BE 0x2be
#define FLAG_SCENE_TEMP_BF 0x2bf
#define FLAG_SCENE_TEMP_C0 0x2c0
#define FLAG_SCENE_TEMP_C1 0x2c1
#define FLAG_SCENE_TEMP_C2 0x2c2
#define FLAG_SCENE_TEMP_C3 0x2c3
#define FLAG_SCENE_TEMP_C4 0x2c4
#define FLAG_SCENE_TEMP_C5 0x2c5
#define FLAG_SCENE_TEMP_C6 0x2c6
#define FLAG_SCENE_TEMP_C7 0x2c7
#define FLAG_SCENE_TEMP_C8 0x2c8
#define FLAG_SCENE_TEMP_C9 0x2c9
#define FLAG_SCENE_TEMP_CA 0x2ca
#define FLAG_SCENE_TEMP_CB 0x2cb
#define FLAG_SCENE_TEMP_CC 0x2cc
#define FLAG_SCENE_TEMP_CD 0x2cd
#define FLAG_SCENE_TEMP_CE 0x2ce
#define FLAG_SCENE_TEMP_CF 0x2cf
#define FLAG_SCENE_TEMP_D0 0x2d0
#define FLAG_SCENE_TEMP_D1 0x2d1
#define FLAG_SCENE_TEMP_D2 0x2d2
#define FLAG_SCENE_TEMP_D3 0x2d3
#define FLAG_SCENE_TEMP_D4 0x2d4
#define FLAG_SCENE_TEMP_D5 0x2d5
#define FLAG_SCENE_TEMP_D6 0x2d6
#define FLAG_SCENE_TEMP_D7 0x2d7
#define FLAG_SCENE_TEMP_D8 0x2d8
#define FLAG_SCENE_TEMP_D9 0x2d9
#define FLAG_SCENE_TEMP_DA 0x2da
#define FLAG_SCENE_TEMP_DB 0x2db
#define FLAG_SCENE_TEMP_DC 0x2dc
#define FLAG_SCENE_TEMP_DD 0x2dd
#define FLAG_SCENE_TEMP_DE 0x2de
#define FLAG_SCENE_TEMP_DF 0x2df
#define FLAG_SCENE_TEMP_E0 0x2e0
#define FLAG_SCENE_TEMP_E1 0x2e1
#define FLAG_SCENE_TEMP_E2 0x2e2
#define FLAG_SCENE_TEMP_E3 0x2e3
#define FLAG_SCENE_TEMP_E4 0x2e4
#define FLAG_SCENE_TEMP_E5 0x2e5
#define FLAG_SCENE_TEMP_E6 0x2e6
#define FLAG_SCENE_TEMP_E7 0x2e7
#define FLAG_SCENE_TEMP_E8 0x2e8
#define FLAG_SCENE_TEMP_E9 0x2e9
#define FLAG_SCENE_TEMP_EA 0x2ea
#define FLAG_SCENE_TEMP_EB 0x2eb
#define FLAG_SCENE_TEMP_EC 0x2ec
#define FLAG_SCENE_TEMP_ED 0x2ed
#define FLAG_SCENE_TEMP_EE 0x2ee
#define FLAG_SCENE_TEMP_EF 0x2ef
#define FLAG_SCENE_TEMP_F0 0x2f0
#define FLAG_SCENE_TEMP_F1 0x2f1
#define FLAG_SCENE_TEMP_F2 0x2f2
#define FLAG_SCENE_TEMP_F3 0x2f3
#define FLAG_SCENE_TEMP_F4 0x2f4
#define FLAG_SCENE_TEMP_F5 0x2f5
#define FLAG_SCENE_TEMP_F6 0x2f6
#define FLAG_SCENE_TEMP_F7 0x2f7
#define FLAG_SCENE_TEMP_F8 0x2f8
#define FLAG_SCENE_TEMP_F9 0x2f9
#define FLAG_SCENE_TEMP_FA 0x2fa
#define FLAG_SCENE_TEMP_FB 0x2fb
#define FLAG_SCENE_TEMP_FC 0x2fc
#define FLAG_SCENE_TEMP_FD 0x2fd
#define FLAG_SCENE_TEMP_FE 0x2fe
#define FLAG_SCENE_TEMP_FF 0x2ff

/// Area flags, FLAG_AREA_TEMP_00 to FLAG_AREA_TEMP_FF, as the scene flags.
#define FLAG_AREA_TEMP_00 0x300
#define FLAG_AREA_TEMP_01 0x301
#define FLAG_AREA_TEMP_02 0x302
#define FLAG_AREA_TEMP_03 0x303
#define FLAG_AREA_TEMP_04 0x304
#define FLAG_AREA_TEMP_05 0x305
#define FLAG_AREA_TEMP_06 0x306
#define FLAG_AREA_TEMP_07 0x307
#define FLAG_AREA_TEMP_08 0x308
#define FLAG_AREA_TEMP_09 0x309
#define FLAG_AREA_TEMP_0A 0x30a
#define FLAG_AREA_TEMP_0B 0x30b
#define FLAG_AREA_TEMP_0C 0x30c
#define FLAG_AREA_TEMP_0D 0x30d
#define FLAG_AREA_TEMP_0E 0x30e
#define FLAG_AREA_TEMP_0F 0x30f
#define FLAG_AREA_TEMP_10 0x310
#define FLAG_AREA_TEMP_11 0x311
#define FLAG_AREA_TEMP_12 0x312
#define FLAG_AREA_TEMP_13 0x313
#define FLAG_AREA_TEMP_14 0x314
#define FLAG_AREA_TEMP_15 0x315
#define FLAG_AREA_TEMP_16 0x316
#define FLAG_AREA_TEMP_17 0x317
#define FLAG_AREA_TEMP_18 0x318
#define FLAG_AREA_TEMP_19 0x319
#define FLAG_AREA_TEMP_1A 0x31a
#define FLAG_AREA_TEMP_1B 0x31b
#define FLAG_AREA_TEMP_1C 0x31c
#define FLAG_AREA_TEMP_1D 0x31d
#define FLAG_AREA_TEMP_1E 0x31e
#define FLAG_AREA_TEMP_1F 0x31f
#define FLAG_AREA_TEMP_20 0x320
#define FLAG_AREA_TEMP_21 0x321
#define FLAG_AREA_TEMP_22 0x322
#define FLAG_AREA_TEMP_23 0x323
#define FLAG_AREA_TEMP_24 0x324
#define FLAG_AREA_TEMP_25 0x325
#define FLAG_AREA_TEMP_26 0x326
#define FLAG_AREA_TEMP_27 0x327
#define FLAG_AREA_TEMP_28 0x328
#define FLAG_AREA_TEMP_29 0x329
#define FLAG_AREA_TEMP_2A 0x32a
#define FLAG_AREA_TEMP_2B 0x32b
#define FLAG_AREA_TEMP_2C 0x32c
#define FLAG_AREA_TEMP_2D 0x32d
#define FLAG_AREA_TEMP_2E 0x32e
#define FLAG_AREA_TEMP_2F 0x32f
#define FLAG_AREA_TEMP_30 0x330
#define FLAG_AREA_TEMP_31 0x331
#define FLAG_AREA_TEMP_32 0x332
#define FLAG_AREA_TEMP_33 0x333
#define FLAG_AREA_TEMP_34 0x334
#define FLAG_AREA_TEMP_35 0x335
#define FLAG_AREA_TEMP_36 0x336
#define FLAG_AREA_TEMP_37 0x337
#define FLAG_AREA_TEMP_38 0x338
#define FLAG_AREA_TEMP_39 0x339
#define FLAG_AREA_TEMP_3A 0x33a
#define FLAG_AREA_TEMP_3B 0x33b
#define FLAG_AREA_TEMP_3C 0x33c
#define FLAG_AREA_TEMP_3D 0x33d
#define FLAG_AREA_TEMP_3E 0x33e
#define FLAG_AREA_TEMP_3F 0x33f
#define FLAG_AREA_TEMP_40 0x340
#define FLAG_AREA_TEMP_41 0x341
#define FLAG_AREA_TEMP_42 0x342
#define FLAG_AREA_TEMP_43 0x343
#define FLAG_AREA_TEMP_44 0x344
#define FLAG_AREA_TEMP_45 0x345
#define FLAG_AREA_TEMP_46 0x346
#define FLAG_AREA_TEMP_47 0x347
#define FLAG_AREA_TEMP_48 0x348
#define FLAG_AREA_TEMP_49 0x349
#define FLAG_AREA_TEMP_4A 0x34a
#define FLAG_AREA_TEMP_4B 0x34b
#define FLAG_AREA_TEMP_4C 0x34c
#define FLAG_AREA_TEMP_4D 0x34d
#define FLAG_AREA_TEMP_4E 0x34e
#define FLAG_AREA_TEMP_4F 0x34f
#define FLAG_AREA_TEMP_50 0x350
#define FLAG_AREA_TEMP_51 0x351
#define FLAG_AREA_TEMP_52 0x352
#define FLAG_AREA_TEMP_53 0x353
#define FLAG_AREA_TEMP_54 0x354
#define FLAG_AREA_TEMP_55 0x355
#define FLAG_AREA_TEMP_56 0x356
#define FLAG_AREA_TEMP_57 0x357
#define FLAG_AREA_TEMP_58 0x358
#define FLAG_AREA_TEMP_59 0x359
#define FLAG_AREA_TEMP_5A 0x35a
#define FLAG_AREA_TEMP_5B 0x35b
#define FLAG_AREA_TEMP_5C 0x35c
#define FLAG_AREA_TEMP_5D 0x35d
#define FLAG_AREA_TEMP_5E 0x35e
#define FLAG_AREA_TEMP_5F 0x35f
#define FLAG_AREA_TEMP_60 0x360
#define FLAG_AREA_TEMP_61 0x361
#define FLAG_AREA_TEMP_62 0x362
#define FLAG_AREA_TEMP_63 0x363
#define FLAG_AREA_TEMP_64 0x364
#define FLAG_AREA_TEMP_65 0x365
#define FLAG_AREA_TEMP_66 0x366
#define FLAG_AREA_TEMP_67 0x367
#define FLAG_AREA_TEMP_68 0x368
#define FLAG_AREA_TEMP_69 0x369
#define FLAG_AREA_TEMP_6A 0x36a
#define FLAG_AREA_TEMP_6B 0x36b
#define FLAG_AREA_TEMP_6C 0x36c
#define FLAG_AREA_TEMP_6D 0x36d
#define FLAG_AREA_TEMP_6E 0x36e
#define FLAG_AREA_TEMP_6F 0x36f
#define FLAG_AREA_TEMP_70 0x370
#define FLAG_AREA_TEMP_71 0x371
#define FLAG_AREA_TEMP_72 0x372
#define FLAG_AREA_TEMP_73 0x373
#define FLAG_AREA_TEMP_74 0x374
#define FLAG_AREA_TEMP_75 0x375
#define FLAG_AREA_TEMP_76 0x376
#define FLAG_AREA_TEMP_77 0x377
#define FLAG_AREA_TEMP_78 0x378
#define FLAG_AREA_TEMP_79 0x379
#define FLAG_AREA_TEMP_7A 0x37a
#define FLAG_AREA_TEMP_7B 0x37b
#define FLAG_AREA_TEMP_7C 0x37c
#define FLAG_AREA_TEMP_7D 0x37d
#define FLAG_AREA_TEMP_7E 0x37e
#define FLAG_AREA_TEMP_7F 0x37f
#define FLAG_AREA_TEMP_80 0x380
#define FLAG_AREA_TEMP_81 0x381
#define FLAG_AREA_TEMP_82 0x382
#define FLAG_AREA_TEMP_83 0x383
#define FLAG_AREA_TEMP_84 0x384
#define FLAG_AREA_TEMP_85 0x385
#define FLAG_AREA_TEMP_86 0x386
#define FLAG_AREA_TEMP_87 0x387
#define FLAG_AREA_TEMP_88 0x388
#define FLAG_AREA_TEMP_89 0x389
#define FLAG_AREA_TEMP_8A 0x38a
#define FLAG_AREA_TEMP_8B 0x38b
#define FLAG_AREA_TEMP_8C 0x38c
#define FLAG_AREA_TEMP_8D 0x38d
#define FLAG_AREA_TEMP_8E 0x38e
#define FLAG_AREA_TEMP_8F 0x38f
#define FLAG_AREA_TEMP_90 0x390
#define FLAG_AREA_TEMP_91 0x391
#define FLAG_AREA_TEMP_92 0x392
#define FLAG_AREA_TEMP_93 0x393
#define FLAG_AREA_TEMP_94 0x394
#define FLAG_AREA_TEMP_95 0x395
#define FLAG_AREA_TEMP_96 0x396
#define FLAG_AREA_TEMP_97 0x397
#define FLAG_AREA_TEMP_98 0x398
#define FLAG_AREA_TEMP_99 0x399
#define FLAG_AREA_TEMP_9A 0x39a
#define FLAG_AREA_TEMP_9B 0x39b
#define FLAG_AREA_TEMP_9C 0x39c
#define FLAG_AREA_TEMP_9D 0x39d
#define FLAG_AREA_TEMP_9E 0x39e
#define FLAG_AREA_TEMP_9F 0x39f
#define FLAG_AREA_TEMP_A0 0x3a0
#define FLAG_AREA_TEMP_A1 0x3a1
#define FLAG_AREA_TEMP_A2 0x3a2
#define FLAG_AREA_TEMP_A3 0x3a3
#define FLAG_AREA_TEMP_A4 0x3a4
#define FLAG_AREA_TEMP_A5 0x3a5
#define FLAG_AREA_TEMP_A6 0x3a6
#define FLAG_AREA_TEMP_A7 0x3a7
#define FLAG_AREA_TEMP_A8 0x3a8
#define FLAG_AREA_TEMP_A9 0x3a9
#define FLAG_AREA_TEMP_AA 0x3aa
#define FLAG_AREA_TEMP_AB 0x3ab
#define FLAG_AREA_TEMP_AC 0x3ac
#define FLAG_AREA_TEMP_AD 0x3ad
#define FLAG_AREA_TEMP_AE 0x3ae
#define FLAG_AREA_TEMP_AF 0x3af
#define FLAG_AREA_TEMP_B0 0x3b0
#define FLAG_AREA_TEMP_B1 0x3b1
#define FLAG_AREA_TEMP_B2 0x3b2
#define FLAG_AREA_TEMP_B3 0x3b3
#define FLAG_AREA_TEMP_B4 0x3b4
#define FLAG_AREA_TEMP_B5 0x3b5
#define FLAG_AREA_TEMP_B6 0x3b6
#define FLAG_AREA_TEMP_B7 0x3b7
#define FLAG_AREA_TEMP_B8 0x3b8
#define FLAG_AREA_TEMP_B9 0x3b9
#define FLAG_AREA_TEMP_BA 0x3ba
#define FLAG_AREA_TEMP_BB 0x3bb
#define FLAG_AREA_TEMP_BC 0x3bc
#define FLAG_AREA_TEMP_BD 0x3bd
#define FLAG_AREA_TEMP_BE 0x3be
#define FLAG_AREA_TEMP_BF 0x3bf
#define FLAG_AREA_TEMP_C0 0x3c0
#define FLAG_AREA_TEMP_C1 0x3c1
#define FLAG_AREA_TEMP_C2 0x3c2
#define FLAG_AREA_TEMP_C3 0x3c3
#define FLAG_AREA_TEMP_C4 0x3c4
#define FLAG_AREA_TEMP_C5 0x3c5
#define FLAG_AREA_TEMP_C6 0x3c6
#define FLAG_AREA_TEMP_C7 0x3c7
#define FLAG_AREA_TEMP_C8 0x3c8
#define FLAG_AREA_TEMP_C9 0x3c9
#define FLAG_AREA_TEMP_CA 0x3ca
#define FLAG_AREA_TEMP_CB 0x3cb
#define FLAG_AREA_TEMP_CC 0x3cc
#define FLAG_AREA_TEMP_CD 0x3cd
#define FLAG_AREA_TEMP_CE 0x3ce
#define FLAG_AREA_TEMP_CF 0x3cf
#define FLAG_AREA_TEMP_D0 0x3d0
#define FLAG_AREA_TEMP_D1 0x3d1
#define FLAG_AREA_TEMP_D2 0x3d2
#define FLAG_AREA_TEMP_D3 0x3d3
#define FLAG_AREA_TEMP_D4 0x3d4
#define FLAG_AREA_TEMP_D5 0x3d5
#define FLAG_AREA_TEMP_D6 0x3d6
#define FLAG_AREA_TEMP_D7 0x3d7
#define FLAG_AREA_TEMP_D8 0x3d8
#define FLAG_AREA_TEMP_D9 0x3d9
#define FLAG_AREA_TEMP_DA 0x3da
#define FLAG_AREA_TEMP_DB 0x3db
#define FLAG_AREA_TEMP_DC 0x3dc
#define FLAG_AREA_TEMP_DD 0x3dd
#define FLAG_AREA_TEMP_DE 0x3de
#define FLAG_AREA_TEMP_DF 0x3df
#define FLAG_AREA_TEMP_E0 0x3e0
#define FLAG_AREA_TEMP_E1 0x3e1
#define FLAG_AREA_TEMP_E2 0x3e2
#define FLAG_AREA_TEMP_E3 0x3e3
#define FLAG_AREA_TEMP_E4 0x3e4
#define FLAG_AREA_TEMP_E5 0x3e5
#define FLAG_AREA_TEMP_E6 0x3e6
#define FLAG_AREA_TEMP_E7 0x3e7
#define FLAG_AREA_TEMP_E8 0x3e8
#define FLAG_AREA_TEMP_E9 0x3e9
#define FLAG_AREA_TEMP_EA 0x3ea
#define FLAG_AREA_TEMP_EB 0x3eb
#define FLAG_AREA_TEMP_EC 0x3ec
#define FLAG_AREA_TEMP_ED 0x3ed
#define FLAG_AREA_TEMP_EE 0x3ee
#define FLAG_AREA_TEMP_EF 0x3ef
#define FLAG_AREA_TEMP_F0 0x3f0
#define FLAG_AREA_TEMP_F1 0x3f1
#define FLAG_AREA_TEMP_F2 0x3f2
#define FLAG_AREA_TEMP_F3 0x3f3
#define FLAG_AREA_TEMP_F4 0x3f4
#define FLAG_AREA_TEMP_F5 0x3f5
#define FLAG_AREA_TEMP_F6 0x3f6
#define FLAG_AREA_TEMP_F7 0x3f7
#define FLAG_AREA_TEMP_F8 0x3f8
#define FLAG_AREA_TEMP_F9 0x3f9
#define FLAG_AREA_TEMP_FA 0x3fa
#define FLAG_AREA_TEMP_FB 0x3fb
#define FLAG_AREA_TEMP_FC 0x3fc
#define FLAG_AREA_TEMP_FD 0x3fd
#define FLAG_AREA_TEMP_FE 0x3fe
#define FLAG_AREA_TEMP_FF 0x3ff

/// The flag a row of the stock table adds to: that shop's artifacts are in
/// stock, for the shop counter's Artifacts list.
#define FLAG_SHOP_ARTIFACTS_STOCKED 0x400

/// The Mad Plants hidden in bushes, one flag each, set once it is beaten, in
/// the order of their battles in ENCOUNTER_TABLE_MAD_PLANTS: the Gabomba
/// Catacombs' (0), Taopo Swamp's, Gaia Rock's, Jupiter Lighthouse's and
/// Trial Road's (4).
#define FLAG_MAD_PLANT_FIRST 0x9b5
#define FLAG_MAD_PLANT(n) (FLAG_MAD_PLANT_FIRST + (n))

/// Set as the World Map delivers Loho's cannon to the ship
/// (WorldMap_DeliverLohoCannon), and by the Northern Reaches' entrance 90;
/// Loho, Atteka Inlet and the Lemurian dock then place the cannon aboard.
#define FLAG_SHIP_CARRIES_CANNON 0x90a
/// Set as Loho's cannon, loaded with the Magma Ball, breaches the ancient wall
/// (Loho_FireCannon); the miners then give the party the cannon, and the next
/// arrival on the World Map at Loho delivers it to the ship.
#define FLAG_LOHO_WALL_BREACHED 0xa5f
/// Set as the first Lucky Medal to land in the middle of Lemuria's spring wins
/// the summon Eclipse (LemuriaSpring_GivePrize).
#define FLAG_LEMURIA_SPRING_ECLIPSE_WON 0x90b
/// Set as the ship reaches Lemuria's harbour (Lemuria's entrance).
#define FLAG_LEMURIA_REACHED 0x907
/// Set while the party last came by Lemuria's other way in: leaving Lemuria's
/// entrance by its fifth exit, or coming into Ancient Lemuria by its entrances
/// 1, 4 or 5; Lemuria's dock then leads there.
#define FLAG_LEMURIA_OTHER_WAY_IN 0x960
/// Set as Piers, now in the party, comes back aboard: "At last, I'm back on my
/// ship."; the cabin door then opens.
#define FLAG_PIERS_BACK_ON_HIS_SHIP 0x985
/// Set as Kraden, tired of waiting on the deck, goes to find a ship the party
/// can use.
#define FLAG_KRADEN_WENT_TO_FIND_A_SHIP 0x988
/// Set as the parties, reunited at Atteka Inlet (or Contigo), set out on the
/// winged ship.
#define FLAG_SET_OUT_WITH_WINGS 0x9b2
/// Set as Susa, at Gaia Rock once the Serpent is slain, hears that Kushinada is
/// safe (GaiaRock_SusaRejoices); his talk and Mind Read change.
#define FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE 0x9ab
/// Set as Jenna says "They're waiting for us. Let's go to Contigo." on the
/// World Map, once the Jupiter beacon is lit.
#define FLAG_JENNA_POINTED_TO_CONTIGO 0x9b3
/// Set as the ship comes into Atteka Inlet's harbour, cleared on landing at
/// Lemuria: while it is set the Lemurian Ship blocks retreat and its first exit
/// leads to Atteka Inlet rather than Lemuria's dock.
#define FLAG_SHIP_AT_ATTEKA_INLET 0x9b4
/// Set as the Sea of Time calms after Poseidon is beaten (SeaOfTime_Open); the
/// way to Lemuria follows it, and Naribwe's fortune teller then sees "None in
/// the eastern world can stand against the power you possess."
#define FLAG_SEA_OF_TIME_OPENED 0x940
/// Set in Eastern Alhafra once its workers have told the mayor; Naribwe's
/// fortune teller then sends the party to the village on the eastern shore.
#define FLAG_WORKERS_TOLD_MAYOR 0x905
/// Set as Piers leaves the party to visit his mother's grave
/// (LemuriaRooms_LeavePiersToMourn); Lemuria reads it too.
#define FLAG_PIERS_AT_MOTHERS_GRAVE 0x966
/// Set as Lunpa comes to meet the party at the foot of his stairs
/// (LemuriaRooms_MeetLunpa); Lemuria reads it too.
#define FLAG_MET_LUNPA 0x969
/// Set as the guard takes Felix in to King Hydros and his council; Lemuria's
/// palace door is then locked while the senate meets.
#define FLAG_MET_KING_HYDROS 0x96a
/// Set with FLAG_MET_KING_HYDROS; Lemuria's entrance clears it when Sheba asks
/// Piers his age.
#define FLAG_SHEBA_TO_ASK_PIERS_AGE 0x96b
/// Which of its three ways in from the World Map the party last took into the
/// Yampi Desert, by the first, eighth and ninth rooms (yampi_desert.c); from
/// patch 1, Retreat from the cave below the Trainer's Whip chest's room leads
/// back there.
#define FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST 0xa29
#define FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST 0xa2a
#define FLAG_YAMPI_DESERT_ENTERED_BY_HIDDEN_PSYNERGY_STONE 0xa2b
/// Yallam's hidden Nut, uncovered with Cyclone.
#define FLAG_YALLAM_NUT 0xf60
/// Yallam's hidden Antidote, uncovered with Cyclone.
#define FLAG_YALLAM_ANTIDOTE 0xf62
/// Set in Madra as Karst comes in (Madra_MeetKarst) once
/// FLAG_TRANSFERRED_VAULT_WARNED_OF_ESCAPED_THIEVES (or the flag after it,
/// which nothing sets) is; the bandit and his two thieves from Vault then wait
/// on the World Map, and their battle clears it.
#define FLAG_BANDIT_ON_WORLD_MAP 0xa76
/// Set while the Golden Boots are shown on the World Map
/// (WorldMap_ShowGoldenBoots), cleared as they are picked up.
#define FLAG_GOLDEN_BOOTS_ON_MAP 0xa77
/// Unused: nothing sets it but the debug flag editor. In the Japanese release,
/// while it is set, every line of a message but its last is spread across the
/// window (Text_MeasureDecoded keeps each line's character count); clear, the
/// count is read from what the stack held.
#define FLAG_SPREAD_LINES 0xc00

#endif
