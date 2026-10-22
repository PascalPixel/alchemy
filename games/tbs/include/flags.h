#ifndef ALCHEMY_FLAGS_H
#define ALCHEMY_FLAGS_H

/// @file
/// Game flags, named where their meaning is shown.

/// Set while `character` (character_ids.h) is in the party
/// (Party_AddActiveOwner).
/// ★ Named from the game: "joined your party." (MsgJoinedParty)
#define FLAG_IN_PARTY(character) (character)
/// The prologue: Isaac and Garet are children. It ends with the caption
/// "Three years later..." over Vale's repairs.
/// ★ Named from the game: "Three years later..." (MsgValeRepairCaption)
#define FLAG_CHILDHOOD 0x20
/// The young Isaac wears the tunic Dora made him take on the stormy night:
/// placed child sprites get its child part (226), and Isaac's grown sprite is
/// ISAAC_TUNIC. Party_EndChildhood clears it.
/// ★ Named from the game: "Don't go out without your tunic!"
/// (MsgValeComeOnHurry)
#define FLAG_ISAAC_WEARS_TUNIC 0x21
/// DJINNI_FLINT's flag, set once it is caught; each Djinni's is FLAG_DJINNI_0
/// plus its DjinniId, and each is named by the Djinni's name in the game's
/// table of them.
/// ★ Named from the game: "Flint" (MsgDjinnName)
#define FLAG_DJINNI_0 48
/// FLAG_DJINNI_0's hiding flag; each Djinni's is this plus its DjinniId, the
/// 80 flags after the Djinn's own. Djinn_ResolvePendingEvent sets it after a
/// lost or fled Djinni battle, and the Djinni's actor stays away until the
/// scene is entered normally again, which clears it.
/// ★ Named from the game: "It must be hiding somewhere again"
/// (MsgVaultHmmmItMustBeHidingSomewhere)
#define FLAG_DJINNI_HIDING_0 128
/// The next scene change is silent: the scene loader clears it in place of
/// playing SE_STOP_SLOT3, once. The world map sets it after its lighthouse
/// transition.
#define FLAG_SKIP_SCENE_CHANGE_SOUND 0x101
/// Set as a battle starts, cleared as a field scene starts.
#define FLAG_IN_BATTLE 0x103
/// Set while the field loop handles its requests, and cleared while the party
/// walks (Field_RunScene); the deserts' heat gauges and the Colosso's wall
/// test it.
#define FLAG_FIELD_HANDLING_REQUESTS 0x104
/// Set while the field menu is open; scenes hold their moving actors still.
#define FLAG_MENU_OPEN 0x106
/// While set, opening the field menu or the pause menu, or pressing a shortcut,
/// raises FIELD_EVENT_MENUS_RAISED instead (Field_RunScene,
/// Field_RequestShortcut); Vault's inn sets it while the party corners the
/// suspects.
#define FLAG_MENUS_RAISE_EVENT 0x107
// Lit lighthouses. Inside a lit lighthouse, the Adepts of its element regain
// PP faster (units.c). Each lighthouse's scenes set its flag on every arrival,
// and Scene_ResetFlagsOnEnter clears all four when the party enters another
// place, so a flag is set only while the party is inside.
/// The party is inside Venus Lighthouse, which is lit.
/// ★ Named from its area name: Venus Lighthouse
#define FLAG_VENUS_LIGHTHOUSE_LIT 0x110
/// The party is inside Mercury Lighthouse, which is already lit when the
/// party reaches its aerie.
/// ★ Named from the game: "The lighthouse has already been lit!"
/// (MsgMercuryLighthouseAlreadyLit)
#define FLAG_MERCURY_LIGHTHOUSE_LIT 0x111
/// Mars Lighthouse is lit; no ☀️ scene sets it.
/// ★ Named from the game: "the lighthouses aren't all lit!" (MsgMercuryGot)
#define FLAG_MARS_LIGHTHOUSE_LIT 0x112
/// Jupiter Lighthouse is lit; no ☀️ scene sets it.
/// ★ Named from the game: "the lighthouses aren't all lit!" (MsgMercuryGot)
#define FLAG_JUPITER_LIGHTHOUSE_LIT 0x113
/// The next scene keeps the music that is playing; scene.c clears
/// FLAG_KEEP_MUSIC as it does, and leaves FLAG_KEEP_MUSIC_HELD set.
#define FLAG_KEEP_MUSIC 0x11a
/// As FLAG_KEEP_MUSIC, for as long as it is set.
#define FLAG_KEEP_MUSIC_HELD 0x11b
#define FLAG_NOT_ON_MAP 0x11c ///< the current place is not on the world map
/// The leader left through an EXIT_STYLE_ANIMATED exit; the next scene plays
/// the leader's arrival (ObjectEffect_RunPendingFlagEvent), unless its setup
/// clears this first.
#define FLAG_EXIT_ARRIVAL_PENDING 0x120
/// As FLAG_EXIT_ARRIVAL_PENDING, for an EXIT_STYLE_ANIMATED_ALTERNATE exit.
#define FLAG_EXIT_ARRIVAL_ALTERNATE 0x121
/// The leader falls into the next scene from 160 pixels above
/// (ObjectEffect_RunPendingFlagEvent), as when the world map's airborne
/// travel ends.
#define FLAG_ARRIVE_FALLING 0x122
/// The party is in a town (SCENE_VARIANT_TOWN); the scene loader sets and
/// clears it, and the Colosso's log sets it as it ends.
#define FLAG_IN_TOWN 0x123
/// Play the kind's usual effect. Field_UsePsynergy sets it for each use and
/// clears it when the after event has trigger bit 0x400 and plays its own; the
/// before event's script may clear it too.
#define FLAG_PSYNERGY_EFFECT 0x140
/// Aim the effect at the actor ahead (FieldFx_DispatchRequestKind) rather
/// than run the kind's untargeted body (FieldFx_Run, where Halt still stops
/// the nearest actor when it has a talk event, and Force and Reveal run their
/// PSYNERGY_PHASE_EFFECT_ACTS event). Field_UsePsynergy keeps it set only when
/// a scene event applies to what the leader faces.
#define FLAG_PSYNERGY_TARGET 0x141
/// Set before an action on the field; still set afterwards, nothing came of it
/// and the game says so. An event that acts clears it.
/// ★ Named from the game: "But nothing happens." (MsgNothingHappens)
#define FLAG_NOTHING_HAPPENS 0x142
#define FLAG_ITEM_USED_UP 0x143 ///< the item just used is removed afterwards
/// Retreat is blocked, whether cast or used from an item, and the game says
/// "It doesn't work here." Cleared on every normal scene entrance (not a return
/// keeping the party's position); the scenes that forbid Retreat set it.
/// ★ Named from the game: "Retreat" (MsgAbilityName)
#define FLAG_RETREAT_BLOCKED 0x144
#define FLAG_ITEM_USE 0x145 ///< the Psynergy comes from an item
/// No random encounters while set (Encounter_SelectEnemyGroup), unless flag
/// 0x15f is set; debug entrances set it.
#define FLAG_NO_ENCOUNTERS 0x160
/// No random battles until the party enters another place: Venus Lighthouse
/// sets it on its Reveal path, the world map's debug entrance too, and
/// Scene_ResetFlagsOnEnter clears it with a new place group.
#define FLAG_NO_ENCOUNTERS_IN_PLACE 0x161
#define FLAG_DEBUG_BATTLE 0x162 ///< set before every debug battle
/// In debug mode, steps on the field are not processed (Field_RunScene) and
/// the world map's places do not react to the leader; debug entrances set it.
#define FLAG_DEBUG_STEPS_OFF 0x163
/// Set while the Lure Cap is worn: random encounters come twice as fast.
/// ★ Named from the game: "Lure Cap" (MsgItemName)
#define FLAG_LURE_CAP 0x167
/// Auto battle: every living unit, party and enemies, makes a plain attack
/// each round with no command menu (BattlePresentation_BuildSortedUnitEntries).
/// The debug battle menu sets it, and clears it again for its chooser.
#define FLAG_AUTO_BATTLE 0x16a
#define FLAG_CHOOSING_COMMANDS 0x16b ///< set while the commands are chosen
/// The next battle is a link battle (Battle_RunEncounter); the Battle Arena
/// sets it, and every normal scene entrance (not a return keeping the party's
/// position) clears it.
/// ★ Named from the game: "Test your skills in Linked Battles"
/// (MsgFieldWelcomeToBattleArenaWherePeople)
#define FLAG_LINK_BATTLE 0x16c
/// In debug mode, with it set, holding R as an action is planned defeats every
/// enemy, and R with Select the party (BattleCommand_BuildPlan); that is its
/// only reader. Vale Gate's debug story preset sets it, and so do the
/// Colosseum's escorts as they swap Mia in for Jenna. XXX: likely a debug
/// switch at the Colosseum too, because Jenna is never in the party there in
/// normal play; a reader outside the debug keys would say otherwise.
#define FLAG_DEBUG_BATTLE_KEYS 0x16d
/// Set, the highest of these picks the tint a field Psynergy effect fades
/// toward, gFieldFx_TargetColors[n], in place of the frame counter's pick
/// (FieldFx_FadeToEffectColor).
#define FLAG_PSYNERGY_TINT(n) (0x148 + (n))
/// In debug mode, each step beeps (SE_135) while fewer than 136 of the 512
/// resource blocks are free.
#define FLAG_DEBUG_RESOURCE_BEEP 350
/// Encounters are rolled even where FLAG_NO_ENCOUNTERS,
/// FLAG_NO_ENCOUNTERS_IN_PLACE, a zone of 0 or Cloak would stop them
/// (Encounter_SelectEnemyGroup).
#define FLAG_IGNORE_ENCOUNTER_CHECKS 351
/// Flint's lesson on Djinn, after he joins on the world map: in its battle the
/// party strikes first, two of Flint's messages follow each round, and no
/// spoils are handed out. The world map's scene sets it.
/// ★ Named from the game: "Now, I'll briefly teach you how to use Djinn."
/// (MsgWorldMapAbilityVenusDjinni)
#define FLAG_DJINN_LESSON 0x16e
/// With FLAG_DJINN_LESSON set, these two choose the part of the lesson the
/// Djinn menu plays: neither, DJINN_TUTORIAL_GIVE; only the second,
/// DJINN_TUTORIAL_POWERS; only the first, DJINN_TUTORIAL_RECOVERY; both,
/// DJINN_TUTORIAL_CLOSE (DjinnMenu_Open). With only the first, Flint says
/// "After a Djinni has been used to summon, it must spend time in recovery."
/// ★ Named from the game: "it must spend time in recovery"
/// (MsgDjinnTutorialRecovery)
#define FLAG_DJINN_LESSON_PART_A 0x16f
/// The second part flag; with only this one set, Flint begins with "Each
/// Djinni has different powers."
/// ★ Named from the game: "Each Djinni has different powers."
/// (MsgDjinnTutorialPowers)
#define FLAG_DJINN_LESSON_PART_B 0x171
/// A linked battle has been arranged in the Battle Arena, with
/// FLAG_LINK_BATTLE: only the first three party members take part.
/// ★ Named from the game: "Test your skills in Linked Battles"
/// (MsgFieldWelcomeToBattleArenaWherePeople)
#define FLAG_LINK_BATTLE_ARRANGED 0x172
/// The Battle Arena's battle against a monster when no opponent is linked:
/// formations matched to the party's level, and no spoils.
/// ★ Named from the game: "Linked Battles and Monster Battles"
/// (MsgFieldWelcomeToBattleArenaWherePeople)
#define FLAG_MONSTER_BATTLE 0x173
/// Set when the cleared game's menu goes to the Battle Arena: there Psynergy
/// has no effect and the system menu offers only Change settings.
/// ★ Named from the game: "Welcome to Battle Arena"
/// (MsgFieldWelcomeToBattleArenaWherePeople)
#define FLAG_IN_BATTLE_ARENA 0x17e
/// Holds the field's palette blend still (ColorBuffer_Update), while a
/// menu is open (Menu_SuspendFieldTasks).
#define FLAG_PALETTE_BLEND_HELD 0x152
/// Holds the field's weather still, its particles and its next burst, while a
/// menu is open (Menu_SuspendFieldTasks).
#define FLAG_WEATHER_HELD 0x166
/// With it set, holding B on the field walks the leader at 4 pixels a frame
/// (FieldObject_UpdatePlayerControl). No ☀️ code sets it.
#define FLAG_FAST_WALK 0x17f
/// The places the party has visited, one flag for each place group, set by
/// Scene_ResetFlagsOnEnter as the party enters the group; the world map screen
/// shows a marker for each.
#define FLAG_PLACE_VISITED(group) (0x180 + (group))
/// A scene's own flags, cleared on every ordinary entrance
/// (Scene_ResetFlagsOnEnter): puzzles reset when the party walks back in.
#define FLAG_SCENE_FIRST 0x200
#define FLAG_SCENE_LAST 0x2ff ///< the last of the scene's 256 flags
/// A place's flags, kept while the party moves between the scenes of one place
/// group and cleared when it enters another. Each place uses the numbers for
/// its own, so the scenes name them; the next two are the link lobby's.
#define FLAG_PLACE_FIRST 0x300
#define FLAG_PLACE_LAST 0x3ff ///< the last of the place's 256 flags
// Byte flags, read and written eight at a time.
#define FLAG_LINK_PARTY_COUNT 0x3f0 ///< the members the linked opponent sent
/// Monster Battles won in a row, added to the monsters' level; -1 after the
/// series ends.
/// ★ Named from the game: "Would you like to fight the next monster?"
/// (MsgLobbyNotBadNextMonster)
#define FLAG_MONSTER_BATTLE_WINS 0x3f8
/// Plus a row of the shop stock table: that place's artifacts are in stock,
/// for the shop counter's Artifacts list.
/// ★ Named from the game: "Artifacts" (MsgCommandYes)
#define FLAG_SHOP_ARTIFACTS_STOCKED 0x400
/// Set while the Colosso holds the party's equipment (the inventory snapshot).
/// ★ Named from the game: "Warriors enter the finals without any of their own
/// equipment." (MsgColossoWarriorsEnterFinalsWithoutAny)
#define FLAG_COLOSSO_HOLDS_EQUIPMENT 0x952
/// The next field scene starts where the party was, as after a battle: the
/// scene's flags are not reset, the actors come back from their snapshot, and
/// the music saved in gGameState.returnCue resumes if there is one (a random
/// battle clears it, so the scene's own music plays). Field_RunScene clears it
/// once the scene starts.
#define FLAG_KEEP_PARTY_POSITION 0x109
/// The scene shows its place's name once it is on screen; entering a new
/// place group sets it.
#define FLAG_SHOW_LOCATION_NAME 0x12f
/// The next layered map keeps the palette and tiles already loaded, once.
#define FLAG_KEEP_LAYER_GRAPHICS 0x170
/// Kraden has talked Isaac, Garet and Jenna into a secret look at Sol Sanctum
/// and come out of his cottage ready to go (Event_KradenSetsOff). Until then
/// Jenna stops Isaac on the path to Mt. Aleph; afterwards Kraden, Saturos and
/// Menardi are gone from outside the cottage.
/// ★ Named from the game: "Do you mean we should go into Sol Sanctum
/// ourselves?" (MsgValeJennaTheyMightBeThieves)
#define FLAG_SANCTUM_VISIT_PLANNED 0x801
/// The party has come in through Sol Sanctum's door the first time, and
/// Kraden has welcomed everyone to it; until then that welcome plays at
/// SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE.
/// ★ Named from the game: "This is the entrance to Sol Sanctum. This is your
/// first time here, right?" (MsgSolSanctumKradenFirstTimeAtSol)
#define FLAG_SOL_SANCTUM_ENTERED 0x802
/// In the hall of Sol Sanctum's first relief, Kraden has recalled the hidden
/// passage Saturos and Menardi spoke of and the four have agreed to look for
/// it together; until then his talk plays each time the party enters the hall.
/// ★ Named from the game: "Those two said there was a hidden passage leading
/// deeper into the sanctum." (MsgSolSanctumWrongKraden)
#define FLAG_SEARCHING_FOR_HIDDEN_PASSAGE 0x804
/// Isaac, Garet and Jenna have overheard the two strangers outside Kraden's
/// cottage and been let go (Event_MeetSaturosAndMenardi). Afterwards the two
/// stand watching the leader, and Vale's barking puppies are gone.
/// ★ Named from the game: "Saturos and Menardi were their names, right?"
/// (MsgValeKradenOhIsaac)
#define FLAG_MET_SATUROS_AND_MENARDI 0x808
/// The first Small Jewel is set in the empty eye of Sol Sanctum's first
/// minotaur relief, which opened the passage beside it; the relief then shows
/// both eyes sparkling, and the passage stays open on later visits.
/// ★ Named from the game: "set the small gem into the empty socket."
/// (MsgSolSanctumSetSmallGem)
#define FLAG_SOL_SANCTUM_FIRST_GEM_PLACED 0x81a
/// As FLAG_SOL_SANCTUM_FIRST_GEM_PLACED, for the second Small Jewel and the
/// second minotaur relief, whose doorway it opened.
/// ★ Named from the game: "set the small gem into the empty socket."
/// (MsgSolSanctumSetSmallGem)
#define FLAG_SOL_SANCTUM_SECOND_GEM_PLACED 0x821
/// The party has beaten the evil within Tret in Tret Tree and he has spoken
/// with it since, too weak to free Kolima. Set only once that talk ends; from
/// then on his room's passage stays open, and in Kolima Forest Hermes' Water
/// heals him instead of being thrown back.
/// ★ Named from the game: "And you defeated the evil within me?"
/// (MsgTretMmmmm)
#define FLAG_TRET_DEFEATED 0x844
/// Hermes' Water has healed Tret in Kolima Forest, and he has freed Kolima's
/// people from being trees. The forest loses its cursed colors, and the
/// people of Kolima and Bilibin switch to their lines for after the curse.
/// ★ Named from the game: "freed Kolima from the curse"
/// (MsgKolimaInteriorsBigStrongWarriorsWithGreatBeards)
#define FLAG_KOLIMA_CURSE_LIFTED 0x845
/// Laurel has spoken with the party the first time in Kolima Forest, knowing
/// them as the ones who resisted Tret's glamour and warning them to leave;
/// afterwards she only repeats the warning until the curse is lifted.
/// ★ Named from the game: "Tret and Laurel are the spirits of the holy trees
/// protecting Kolima forest." (MsgKolimaInteriorsTheresABookEntitledTheHoly)
#define FLAG_MET_LAUREL 0x84c
/// The party has taken its reward from McCoy's treasure chests.
/// ★ Named from the game: "The reward you received was indeed the greatest of
/// treasures." (MsgBilibinRewardReceivedWasIndeedGreatest)
#define FLAG_MCCOY_PALACE_REWARD_TAKEN 0x84e
/// The party has beaten the three thieves in the loft of Vault's inn and the
/// mayor has had them locked up; the stolen goods are returned and Ivan leaves
/// the party. Vault's people, its jail and its Sanctum read it, and in Goma
/// Cave Ivan comes back to rejoin the party.
/// ★ Named from the game: "Those men you captured... They're in jail now?"
/// (MsgVaultThoseMenCapturedTheyreIn)
#define FLAG_VAULT_THIEVES_JAILED 0x855
/// Vault's dog, given a Bone, has jumped where the passage underground is, and
/// its thoughts now say so.
/// ★ Named from the game: "A careful search will reveal a passage leading
/// underground nearby." (MsgVaultCarefulSearchWillRevealPassage)
#define FLAG_VAULT_DOG_SHOWN_PASSAGE 0x858
/// The girl at Vault's inn has given the party a Bone.
/// ★ Named from the game: "Bone" (MsgItemName)
#define FLAG_VAULT_BONE_RECEIVED 0x85b
/// Ringing Vault's bell has called Sap out of hiding.
/// ★ Named from the game: "Sap" (MsgDjinnName)
#define FLAG_VAULT_DJINNI_CALLED_BY_BELL 0x867
/// Douse has put out the fire between two torches in BilibinCaveRapier.
/// ★ Named from what the player sees: a small fire burning between two torches
/// before a doorway (its render)
#define FLAG_BILIBIN_CAVE_SECOND_FIRE_DOUSED 0x882
/// Douse has put out the fire beside the first puddle in BilibinCaveIce.
/// ★ Named from what the player sees: a small fire beside a puddle among the
/// ice pillars (its render)
#define FLAG_BILIBIN_CAVE_FIRST_FIRE_DOUSED 0x883
/// Feizhi has burst in on Master Hama's lesson at Lama Temple to say that Hsu
/// is trapped among the boulders, and Hama has gone to his aid. Set as the
/// lesson ends; every villager in Xian then talks of the rockslide, and Hama
/// and Feizhi wait by Hsu at the Alpine Crossing.
/// ★ Named from the game: "He was trapped under a boulder."
/// (MsgXianXianIsAPeacefulVillageBut)
#define FLAG_XIAN_HEARD_HSU_TRAPPED 0x895
/// Feizhi has stormed out of Master Feh's school, sure that Hsu is in danger,
/// to find him alone. It unlocks the school's door she came out of, and
/// places her by Xian's south exit until she reaches the Alpine Crossing.
/// ★ Named from the game: "Hsu is in danger. Feizhi is going to help."
/// (MsgXianDidDoWarrior)
#define FLAG_FEIZHI_WENT_TO_HSU 0x89a
/// Hsu is free of the boulders at the Alpine Crossing: Lift has raised the
/// boulder while Hama and Feizhi pulled him out, or he was freed while the
/// party was away (FLAG_HSU_RESCUED_WITHOUT_PARTY, set with it). The road is
/// cleared, and Lama Temple's monks give their advice for the desert.
/// ★ Named from the game: "We did it! Master Hama, Hsu is free!"
/// (MsgMountainIsaacDidLiftBoulder)
#define FLAG_HSU_RESCUED 0x8b2
/// The water in Altin Mine has drained after the first, second and third
/// water beasts' battles (AltinPeak_DrainFirstWater and the two after it).
/// ★ Named from the game: "all the water should drain away"
/// (MsgAltinIfWeCouldJustDestroyThe)
#define FLAG_ALTIN_MINE_FIRST_DRAIN 0x8fd
/// ★ Named from the game: "all the water should drain away"
/// (MsgAltinIfWeCouldJustDestroyThe)
#define FLAG_ALTIN_MINE_SECOND_DRAIN 0x8fe
/// ★ Named from the game: "all the water should drain away"
/// (MsgAltinIfWeCouldJustDestroyThe)
#define FLAG_ALTIN_MINE_THIRD_DRAIN 0x907
/// The party has beaten the Hydros statues at the bottom of Altin Peak; the
/// statue there sinks into the floor. Every villager in Altin, and the Sanctum
/// healer's thoughts, switch to their lines for after the battle.
/// ★ Named from the game: "Thank you so much for defeating the hydros statues
/// in the mine." (MsgAltinThankYouSoMuchForDefeating)
#define FLAG_HYDROS_STATUES_DEFEATED 0x909
/// In the ship's hold at Kalay, Sean and Ouranos have lost patience with the
/// delay and gone to make the captain sail. It moves the voyage on: the
/// passengers' talk changes, and the Anchor Charm can be found on the mast.
/// ★ Named from the game: "Where else? To the captain's quarters, of course."
/// (MsgTolbiShipIfWeArentGoingSet)
#define FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN 0x922
/// The crewman behind the deck's second door has been caught, and denies
/// everything.
/// ★ Named from the game: "I didn't do anything..."
/// (MsgTolbiShipDidntDoAnything)
#define FLAG_TOLBI_SHIP_CREWMAN_CAUGHT 0x923
/// The party has found the captain's Anchor Charm at the top of the mast,
/// which it can only do once the crewman on deck is caught or Sean and Ouranos
/// have gone to the captain.
/// ★ Named from the game: "Anchor Charm" (MsgItemName)
#define FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND 0x924
/// Lunpa's chef has told of the prisoner in the fortress, in words or in his
/// thoughts.
/// ★ Named from the game: "that poor man they've got locked up in the dungeon"
/// (MsgLunpaChefThinksOfPrisoner)
#define FLAG_LUNPA_HEARD_OF_PRISONER 0x940
/// Donpa has judged Dodonpa in Lunpa Fortress and locked him up; Lunpa opens
/// again, and the party may leave with Hammet.
/// ★ Named from the game: "Donpa, didn't you just say you're going to lock up
/// Dodonpa?" (MsgLunpaDad)
#define FLAG_DODONPA_LOCKED_UP 0x941
/// In West Lunpa Cave, Hammet has found Bunza, the Kalay merchant hiding
/// behind a rock, and Bunza has offered his wagon. It plays once; until then
/// Garet keeps the leader from going back up to Lunpa Fortress.
/// ★ Named from the game: "Is that you, Bunza?" (MsgLunpaMoment)
#define FLAG_LUNPA_CAVE_REUNION_SEEN 0x94d
/// Bunza's wagon has left West Lunpa Cave, with the party aboard or without.
/// ★ Named from the game: "You don't want to ride in Bunza's wagon?"
/// (MsgLunpaGaretAsksIfNotRiding)
#define FLAG_BUNZAS_WAGON_LEFT 0x94f
/// Iodem, waiting in Gondowan Passage, travels with the party toward Babi
/// Lighthouse. Turning back there clears it if he stays to wait; in the
/// Suhalla Desert he warns of the storms and asks whether to press on.
/// ★ Named from the game: "Then we should continue on our way to the
/// lighthouse." (MsgKaragolWhyGoingBackIsaacDo)
#define FLAG_IODEM_FOLLOWING_PARTY 0x9a0
/// Move has sent GondowanCave's tree stump spinning across, freeing its cell.
/// ★ Named from what the player sees: a tree stump on the ledge beside the
/// river (its render)
#define FLAG_GONDOWAN_CAVE_STUMP_MOVED 0x9a2
/// GondowanCave's stone pillar has dropped into its place for good.
/// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
#define FLAG_GONDOWAN_CAVE_PILLAR_DROPPED 0x9a5

// Alpine Crossing and Lama Temple
/// At the Alpine Crossing, Feizhi has caught up with the party, found the road
/// blocked as her vision foretold and run ahead calling for Hsu. She is gone
/// from Xian from then on.
/// ★ Named from the game: "Yahh! Silk Road! Boulders block the road!"
/// (MsgMountainYahhSilkRoadBouldersBlock)
#define FLAG_FEIZHI_SAW_ROCKSLIDE 0x8b0
/// Hama has reached Hsu and spoken with him and Feizhi.
/// ★ Named from the game: "Hsu, are you okay?" (MsgMountainHsuOkay)
#define FLAG_HAMA_REACHED_HSU 0x8b1
/// Hsu was freed without the party: set with FLAG_HSU_RESCUED when the party
/// comes back from Lama Temple after Xian has heard of the accident. While it
/// is set, Xian's people do not thank the party for the rescue.
/// ★ Named from the game: "We have Master Hama to thank for that."
/// (MsgXianInteriorsHsuWasTrappedUnderTheRocks)
#define FLAG_HSU_RESCUED_WITHOUT_PARTY 0x8b3
/// The crossing's chest.
/// ★ Named from the game: "Power Bread" (MsgItemName)
#define FLAG_ALPINE_CROSSING_POWER_BREAD 0xf71
/// The temple's chest.
/// ★ Named from the game: "Water of Life" (MsgItemName)
#define FLAG_LAMA_TEMPLE_WATER_OF_LIFE 0xf7a

// Altin
/// The water beast spitting at Altin's houses has stopped as the party came
/// near and hopped away into the flood for good; Altin's setup then removes
/// it.
#define FLAG_ALTIN_WATER_BEAST_LEFT 0x8ff
/// Altin's stone pillar stands in column 22, where Move pushes it. Checked
/// after every Move (Village_CheckPillarMoved), which clears it if the pillar
/// is anywhere else; the setup puts the pillar back there while it is set.
#define FLAG_ALTIN_PILLAR_MOVED 0x906
/// A barrel in the houses.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_ALTIN_NUT 0xf73
/// A barrel in the houses.
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_ALTIN_COINS 0xf74
/// A bed in the houses.
/// ★ Named from the game: "Sleep Bomb" (MsgItemName)
#define FLAG_ALTIN_SLEEP_BOMB 0xf75

// Altin Peak (SceneId_AltinPeakFirstWaterBeast to SceneId_AltinPeakWestExit)
/// AltinPeakFirstWaterBeast's water beast has been seen hopping along its first
/// and its second route.
/// ★ Named from the game: "We beat the last water beast!"
/// (MsgAltinWeDidIsaacWeBeat)
#define FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN 0x904
/// As FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN, for the second route.
/// ★ Named from the game: "We beat the last water beast!"
/// (MsgAltinWeDidIsaacWeBeat)
#define FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN 0x905
/// AltinPeakFallingRocks's boulder has fallen and rolled after the party leader
/// into the wall at the bottom, leaving rubble; the scene redraws the rubble
/// and the toppled log on later visits, and Force no longer topples the log.
/// ★ Named from the game: "Watch for falling rocks! Do not strike the wall!
/// Rocks may fall!" (MsgAltinWatchFallingRocks)
#define FLAG_ALTIN_PEAK_BOULDER_FALLEN 0x908
/// AltinPeakSecondWaterBeast's chest.
/// ★ Named from the game: "Cookie" (MsgItemName)
#define FLAG_ALTIN_PEAK_COOKIE 0xf76
/// AltinPeakThirdWaterBeast's chest.
/// ★ Named from the game: "Dragon Shield" (MsgItemName)
#define FLAG_ALTIN_PEAK_DRAGON_SHIELD 0xf77
/// AltinPeakJunction's chest.
/// ★ Named from the game: "Lucky Medal" (MsgItemName)
#define FLAG_ALTIN_PEAK_LUCKY_MEDAL 0xf78
/// AltinPeakWestExit's chest.
/// ★ Named from the game: "Vial" (MsgItemName)
#define FLAG_ALTIN_PEAK_VIAL 0xf79
/// AltinPeakSpritz's chest.
/// ★ Named from the game: "Psy Crystal" (MsgItemName)
#define FLAG_ALTIN_PEAK_PSY_CRYSTAL 0xf07
/// AltinPeakHydrosStatue's chest.
/// ★ Named from the game: "Lifting Gem" (MsgItemName)
#define FLAG_ALTIN_PEAK_LIFTING_GEM 0xf08

// Altmiller Cave
/// The party has spoken with the unseen voice in Altmiller Cave, who cannot
/// move and has asked for his draught from deep in the cave, telling the order
/// of the five rocks.
/// ★ Named from the game: "You're invisible!" (MsgAltmillerSee)
#define FLAG_MET_INVISIBLE_BABI 0x960
/// Babi has called the party back once as it made to leave without him.
/// ★ Named from the game: "Wait..." (MsgAltmillerWait)
#define FLAG_BABI_CALLED_PARTY_BACK 0x961
/// The party has saved Babi in Altmiller Cave with his Mystic Draught; Tolbi
/// then holds the Colosso finals, and its scenes read this as the finals day.
/// ★ Named from the game: "You have our gratitude for saving Babi's life."
/// (MsgAltmillerBabi)
#define FLAG_BABI_SAVED 0x962
/// The chest behind the rocks' wall.
/// ★ Named from the game: "Mystic Draught" (MsgItemName)
#define FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT 0xf30
/// Room 3's chest.
/// ★ Named from the game: "Vial" (MsgItemName)
#define FLAG_ALTMILLER_CAVE_VIAL 0xf9c
/// Room 5's chest.
/// ★ Named from the game: "Cookie" (MsgItemName)
#define FLAG_ALTMILLER_CAVE_COOKIE 0xf9d

// Babi Lighthouse
/// Arrival through room 2's entrance 10 changes its tiles and collision;
/// entrances 10 and 11 restore those changes on later visits.
/// ★ Named from the game: "they opened the stone door"
/// (MsgBabiYoureSureTheyWentThrough)
#define FLAG_BABI_LIGHTHOUSE_BACK_FROM_STONE_DOOR 0x980
/// The stone door that Reveal's switch opens stands open. Walking through
/// clears it as the door shuts behind the party.
/// ★ Named from the game: "The truth of this door is open to those seeing with
/// their true heart." (MsgBabiTruthDoorOpenThoseSeeing)
#define FLAG_BABI_LIGHTHOUSE_DOOR_OPEN 0x985
/// Iodem and Faran have asked the party to follow Sheba's kidnappers.
/// ★ Named from the game: "Can you follow the same paths that they took?"
/// (MsgBabiYoureSureTheyWentThrough)
#define FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW 0x988
/// Set with FLAG_BABI_LIGHTHOUSE_ASKED_TO_FOLLOW: Iodem has come to Babi
/// Lighthouse, and from then on he no longer steps out of the party to
/// question the wounded soldiers in Suhalla, or to ask in Gondowan Passage why
/// the party is turning back.
/// ★ Named from the game: "Oh! Iodem..." (MsgBabiYoureSureTheyWentThrough)
#define FLAG_IODEM_AT_BABI_LIGHTHOUSE 0x98a
/// The party has opened the stone door before Iodem and Faran, who saw it shut
/// again and stay behind, leaving Sheba's rescue to the party.
/// ★ Named from the game: "I don't believe it! The door opened!"
/// (MsgBabiTheDoor)
#define FLAG_BABI_LIGHTHOUSE_DOOR_SHOWN 0x989
/// The building site's chest.
/// ★ Named from the game: "Lucky Pepper" (MsgItemName)
#define FLAG_BABI_LIGHTHOUSE_LUCKY_PEPPER 0xfbc

// Babi Palace
/// The Colosso finals have been fought, won or lost; Babi's Palace, the
/// Colosseum, the docks and Gondowan Passage take their after-Colosso people
/// and talk.
/// ★ Named from the game: "Colosso is over and the colosseum will be closed
/// until next year." (MsgColosseumEntranceColossoIsOverAndTheColosseum)
#define FLAG_COLOSSO_OVER 0x950
/// The party won the finals.
/// ★ Named from the game: "Iodem, bring him the victor's prize."
/// (MsgTolbiWaitingCompanions)
#define FLAG_WON_COLOSSO 0x951
/// The attendant has offered the beds for the night before the finals.
/// ★ Named from the game: "You all came to rest up before the match"
/// (MsgTolbiCameRestBefore)
#define FLAG_BABI_PALACE_FINALS_REST_OFFERED 0x966
/// While set, the beds can be used during the finals.
/// ★ Named from the game: "please feel free to get some rest"
/// (MsgTolbiTiredFeelFreeRest)
#define FLAG_BABI_PALACE_FINALS_BEDS_OPEN 0x967
/// Sheba has felt the party read her mind in Babi's Palace, the first time
/// Mind Read is used on her.
/// ★ Named from the game: "Did you just..." (MsgTolbiEvenIfEscapedBabiPalace)
#define FLAG_SHEBA_NOTICED_MIND_READ 0x968
/// While set, the beds can be used before the finals.
/// ★ Named from the game: "You may use these four beds."
/// (MsgTolbiUseFourBeds)
#define FLAG_BABI_PALACE_BEDS_OPEN 0x969
/// The party has slept in Babi's Palace once, talking it over the first night;
/// later nights in its beds go straight to sleep.
#define FLAG_BABI_PALACE_FIRST_NIGHT 0x96a
/// The soldiers have come back from Altmiller Cave to Iodem without Babi, and
/// Iodem has left for the colosseum in Babi's stead; he and the soldiers are
/// gone from the palace from then on.
/// ★ Named from the game: "We went to Altmiller Cave, as you commanded."
/// (MsgTolbiIodemIodem)
#define FLAG_IODEM_HEARD_SOLDIERS 0x96b
/// A courtyard guard has passed on Babi's word to go to the colosseum.
/// ★ Named from the game: "Babi left word that he would be waiting for you"
/// (MsgTolbiArent)
#define FLAG_BABI_PALACE_GUARD_GAVE_WORD 0x96c
/// The other courtyard guard has thanked the party for saving Babi.
/// ★ Named from the game: "I'd really like to thank you for saving Babi."
/// (MsgTolbiIsaacIdReallyLikeThank)
#define FLAG_BABI_PALACE_GUARD_THANKED_PARTY 0x96d
/// Babi and Iodem have spoken with the party in the throne room after Colosso
/// and sent Iodem with it toward Venus Lighthouse. Gondowan Passage, Suhalla
/// and Suhalla Gate take their later people and events from then on.
/// ★ Named from the game: "I've been waiting here because I wanted to speak
/// with you." (MsgTolbiWaitingCompanions)
#define FLAG_BABI_SPOKE_AFTER_COLOSSO 0x96f
/// Lent by Babi after Colosso.
/// ★ Named from the game: "Cloak Ball" (MsgItemName)
#define FLAG_BABI_PALACE_CLOAK_BALL 0xf31
/// In a barrel.
/// ★ Named from the game: "Lucky Medal" (MsgItemName)
#define FLAG_BABI_PALACE_LUCKY_MEDAL 0xf9f
/// A jar in the scholars' room, which is drawn on Vale's map
/// (vale_interiors_inn.c).
/// ★ Named from the game: "Hard Nut" (MsgItemName)
#define FLAG_BABI_PALACE_HARD_NUT 0xf9e
// Bilibin
/// Move has pulled Jill, a tree in the river by Bilibin, ashore; she stays on
/// the bank, and once the curse is lifted she is a child again with the rest
/// of her gang.
/// ★ Named from the game: "Jill, are these the people who saved you?"
/// (MsgBilibinYoureGuy)
#define FLAG_BILIBIN_JILL_RESCUED 0x848
/// Her thanks, a Hard Nut.
/// ★ Named from the game: "Jill gave a special gift."
/// (MsgBilibinJillGaveIsaacSpecialGift)
#define FLAG_BILIBIN_JILL_GIFT_RECEIVED 0x849

// Colosso
/// The trial attendant in the Colosseum's stands has shown the party a warrior
/// failing his trial and explained it; the next time, he shows one clearing
/// his.
/// ★ Named from the game: "That warrior failed his trial"
/// (MsgColosseumHaveMakeThroughCountlessMatches)
#define FLAG_COLOSSO_FAILED_TRIAL_SEEN 0x8a3
/// The trial attendant has shown the party a warrior clearing his trial; from
/// then on he only talks of the techniques.
/// ★ Named from the game: "That warrior cleared this trial"
/// (MsgColosseumHavePerfectTechniquesLikeThose)
#define FLAG_COLOSSO_CLEARED_TRIAL_SEEN 0x8a4
/// Isaac has won his first match of the Colosso finals, in Colosseum Final 1;
/// the soldiers congratulate him and lead him to Colosseum Final 2.
/// ★ Named from the game: "I see you've made it through your first match."
/// (MsgColosseumSeeYouveMadeThroughYour)
#define FLAG_COLOSSO_FIRST_MATCH_WON 0x90e
/// Isaac has won his second match, in Colosseum Final 2; from then on the
/// passages east of the waiting room lead to Colosseum Final 3.
/// ★ Named from the game: "You've managed to win both of your matches."
/// (MsgColosseumWonBothMatches)
#define FLAG_COLOSSO_SECOND_MATCH_WON 0x90f
/// The officials have taken the party into the Colosseum on the finals day.
/// XXX: no code of this game reads it; a reader would say what it changes.
/// ★ Named from the game: "Putting him in the finals" (MsgColosseumHeyYouRe)
#define FLAG_COLOSSEUM_FINALS_ENTERED 0x93f
// Also FLAG_COLOSSO_OVER: the colosseum is closed until next year.
// Also FLAG_BABI_SAVED: the trials are over, the finals day.

// Kalay
/// The innkeeper or the tour's guide at Kalay's inn has told the party about
/// the Colosso tour to Tolbi; leaving the town then plays the tour group's
/// departure.
/// ★ Named from the game: "Oh, so you must be taking the Colosso tour, too..."
/// (MsgKalayGoingTolbiAlso)
#define FLAG_KALAY_HEARD_OF_COLOSSO_TOUR 0x910
/// The Colosso tour's group has left Kalay without its missing man. Its guests
/// are gone from the inn, the ticket taker at Kalay Docks starts asking for
/// tickets, and Sean and Ouranos lose patience in the ship's hold.
/// ★ Named from the game: "Our inn feels so empty, now that the Colosso tour
/// group has left." (MsgKalayOurInnFeelsEmptyNow)
#define FLAG_KALAY_COLOSSO_TOUR_LEFT 0x911
/// Catch has taken the Nut circling in the town.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_KALAY_TOWN_NUT_CAUGHT 0x916
/// Catch has taken the Nut floating at the Kalay Tunnel Entrance.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT 0xfd6

// Imil
/// Mia's second apprentice told the party Mia went to the old couple's house.
/// ★ Named from the game: "Have you visited the old couple"
/// (MsgImilHaveVisitedOldCoupleWho)
#define FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE 0x82a
/// Mia has healed the old man of Imil with Ply before the party, seen Mercury
/// Lighthouse flash and run off to it, the party following. Imil's old couple
/// and Mercury Lighthouse's door statue read it.
/// ★ Named from the game: "Mia" (MsgCharacterName)
#define FLAG_IMIL_MET_MIA 0x82b
/// The old couple have sent for Mia: the old man moaned for help and his wife
/// hurried off to fetch her, or her apprentice told the party where she went.
/// ★ Named from the game: "Please, get" (MsgMercuryOoohHelpMe)
#define FLAG_IMIL_MIA_SENT_FOR 0x82c
/// Her apprentice told the party she can act as its healer in the sanctum.
/// ★ Named from the game: "I can act as your healer."
/// (MsgImilMayOnlyStudentButCan)
#define FLAG_IMIL_APPRENTICE_OFFERED_HEALING 0x82d
/// Mia has said farewell to her two apprentices.
/// ★ Named from the game: "I'll leave the village in your hands"
/// (MsgImilMia)
#define FLAG_IMIL_MIA_SAID_FAREWELL 0x82e

// Fuchin Falls
/// The Dragon's Eye is taken from where it lies in Fuchin Falls Cave's third
/// room.
/// ★ Named from the game: "Dragon's Eye" (MsgItemName)
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND 0xf13
/// The Dragon's Eye is set in the eyeless dragon statue in Fuchin Falls Cave,
/// which lights the room and shows its hidden path; the eye is used up.
/// ★ Named from the game: "It's a statue of an eyeless dragon. What went in
/// the sockets?" (MsgFuchinEyelessDragon)
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET 0x820

// Kalay and Tolbi docks
/// The party has bought its tickets for the ship to Tolbi.
/// ★ Named from the game: "Hand in your ticket at the ship's plank."
/// (MsgKalayHandInTicketPlank)
#define FLAG_KALAY_DOCKS_TICKET_BOUGHT 0x8a5
/// The woman waiting on Kalay Docks has asked the party to tell her daughter
/// in Tolbi that the tickets are too dear, and the party agreed; the daughter
/// on Tolbi Docks then asks for the message.
/// ★ Named from the game: "please pass a message to my daughter"
/// (MsgKalayPassMessageDaughter)
#define FLAG_KALAY_DOCKS_CARRYING_MESSAGE 0x8a6
/// The party has given its Boat Ticket to the ticket taker at the ship's plank
/// on Kalay Docks, and may board.
/// ★ Named from the game: "After handing over your ticket, you may board the
/// ship." (MsgKalayAfterHandingOverTicket)
#define FLAG_KALAY_DOCKS_TICKET_HANDED_OVER 0x8a7
/// The daughter on Tolbi Docks has heard from the party why her parents could
/// not come, and thanks it from then on.
/// ★ Named from the game: "Thank you for giving me the message from my
/// parents." (MsgKalayThanksMessageParents)
#define FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED 0x8a8
/// With its ticket handed over, the party has asked the ticket taker to go
/// ashore and he has stepped aside; he then asks it to board when it is done.
/// Coming back off the ship clears it, and he takes his place at the plank.
/// ★ Named from the game: "Do you want to go ashore?" (MsgKalayWantGoAshore)
#define FLAG_KALAY_DOCKS_WENT_ASHORE 0x8a9
/// A man on Tolbi Docks has walked up to the party as it came off the ship, and
/// stays beside it. Coming off the ship again clears it, so he meets the party
/// each time.
/// ★ Named from the game: "I hear you had a scary trip."
/// (MsgKalayHearScaryTrip)
#define FLAG_TOLBI_DOCKS_ARRIVALS_GREETED 0x8aa
/// The passengers confronted the tour guides and their bodyguard.
/// ★ Named from the game: "Isn't the tour group responsible"
/// (MsgKalayReturnedTicketCost)
#define FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED 0x8ab
/// The first time the party walked onto Kalay Docks from the world map, Garet
/// ran to the water and was told the Karagol is no real ocean.
/// ★ Named from the game: "The Karagol is surrounded by land, so technically,
/// it's not a real ocean." (MsgKalayIncredibleOcean)
#define FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL 0x8ac
// Kolima
/// The party has seen the curse fall on Kolima, in Kolima or in
/// SceneId_KolimaForestEntrance, whichever it reached first: the sparkles rain
/// down, its Psynergy shields it, and Tret and Laurel speak.
/// ★ Named from the game: "His wrathful side turned the people of Kolima into
/// trees." (MsgKolimaWatchOutItsHappeningAgain)
#define FLAG_KOLIMA_CURSE_WITNESSED 0x843
/// Kolima Bridge fell apart in front of the leader the first time the party
/// stepped onto it after the curse was lifted. The bridge is drawn broken from
/// then on, and its piece of the world map changes with it.
#define FLAG_KOLIMA_BRIDGE_COLLAPSED 0x847

// Kolima Forest
/// Catch took the clearing's Nut.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_KOLIMA_FOREST_NUT_CAUGHT 0xfd3

// Lamakan Desert
/// The Manticore, on the third map, is beaten.
/// ★ Named from the game: "Manticore" (MsgEnemyName)
#define FLAG_LAMAKAN_MANTICORE_BEATEN 0x90a
// The desert's chests, set once each is opened.
/// ★ Named from the game: "Vulcan Axe" (MsgItemName)
#define FLAG_LAMAKAN_DESERT_VULCAN_AXE 0xf7b
/// ★ Named from the game: "Lucky Pepper" (MsgItemName)
#define FLAG_LAMAKAN_DESERT_LUCKY_PEPPER 0xf7c
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_LAMAKAN_DESERT_777_COINS 0xf7d
/// ★ Named from the game: "Potion" (MsgItemName)
#define FLAG_LAMAKAN_DESERT_POTION 0xf7e
/// ★ Named from the game: "Mint" (MsgItemName)
#define FLAG_LAMAKAN_DESERT_MINT 0xf7f

// Lalivero
/// The party is back from Venus Lighthouse: set at the Lighthouse Aerie after
/// Sheba's fall, as the party is taken to Lalivero. Lalivero and its buildings
/// then read their tables for after the lighthouse.
/// ★ Named from its area name: Venus Lighthouse
#define FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE 0x9a7
/// Out of Gateway Cave, Iodem saw a ship off the coast at Idejima, took it for
/// Babi's and handed over the Black Orb (WorldMap_RunBlackOrbScene). Back from
/// Venus Lighthouse, it picks the version of the report to Faran that knows
/// the ship was swept away, and Garet's guess that it was Saturos's.
/// ★ Named from the game: "I thought the ship we saw at Idejima was Babi's..."
/// (MsgLaliveroThoughtShipWeSawAt)
#define FLAG_SAW_SHIP_AT_IDEJIMA 0x9bf
/// The party has first come into Lalivero with Iodem and found more of Tolbi's
/// soldiers beaten in the street, who say Sheba's captors went to the ruins
/// beneath Babi Lighthouse. After it, Gateway Cave's Black Orb scene no longer
/// plays.
/// ★ Named from the game: "More of Tolbi's soldiers lay defeated..."
/// (MsgLaliveroMoreTolbisSoldiersLayDefeated)
#define FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN 0x9b8
/// Outside Faran's house, Iodem has told the party the misty island may be
/// Lemuria, given it the Black Orb that sails Babi's ship and said farewell.
/// ★ Named from the game: "Farewell! And bring back news of Lemuria!"
/// (MsgGotBlackOrb)
#define FLAG_IODEM_FAREWELL_SEEN 0x9ba
/// The guard at Lalivero's wharf has let the party through to Babi's ship
/// and stepped aside, where he stays.
/// ★ Named from the game: "I was told to let you in if you showed up... The
/// door is open." (MsgLaliveroWasToldLetInIf)
#define FLAG_LALIVERO_WHARF_OPEN 0x9bb
/// After the report to Faran, Iodem has asked the party to wait for him outside
/// Faran's house, where he gives it the Black Orb.
/// ★ Named from the game: "Please wait for me outside Faran's house. I wish to
/// speak with you." (MsgLaliveroPleaseWaitForMeOutside)
#define FLAG_IODEM_ASKED_TO_MEET 0x9bc
// Lunpa
/// Set on the world map as Hammet's caravan leaves Vault for Lunpa; from then
/// on Lunpa's east guard asks whether the party came from Kalay, and touching
/// Vault on the world map leads straight in.
/// ★ Named from the game: "You're not from Kalay, are you?"
/// (MsgLunpaEastGuardAsksIfFrom)
#define FLAG_GUARDS_SUSPECT_KALAY 0x85a
/// After Donpa's judgment, the left guard at the Lunpa Fortress Gates has
/// half recognized Hammet with the party, then dismissed it; his thoughts
/// afterwards say the man looks like that merchant.
/// ★ Named from the game: "I've seen that man with you before."
/// (MsgLunpaLeftGuardRecognizesHammet)
#define FLAG_LUNPA_GATE_GUARD_SAW_HAMMET 0x9af
/// The Psynergy Stone in the town.
/// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
#define FLAG_LUNPA_PSYNERGY_STONE 0xf27
/// A grave in the town.
/// ★ Named from the game: "Sleep Bomb" (MsgItemName)
#define FLAG_LUNPA_GRAVE_SLEEP_BOMB 0xf88
/// A barrel in the town.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_LUNPA_BARREL_NUT 0xf89
/// A wooden box in the town's buildings.
/// ★ Named from the game: "Lucky Medal" (MsgItemName)
#define FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL 0xf8a
/// A barrel in the town's buildings.
/// ★ Named from the game: "Vial" (MsgItemName)
#define FLAG_LUNPA_BARREL_VIAL 0xf8b
/// Catch has taken the Nut floating over the town.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_LUNPA_NUT_CAUGHT 0xfd1

// Lunpa Fortress. From FLAG_LUNPA_FORTRESS_CRATES_MOVED to
// FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN, the fortress's state is cleared
// whenever the party comes back to the gate below it.
/// The party has seen Hammet's rescue up to Dodonpa's trap; a retry after
/// losing the battle skips to the trap.
/// ★ Named from the game: "Dodonpa pulled a lever."
/// (MsgLunpaDodonpaPulledLever)
#define FLAG_LUNPA_FORTRESS_TRAP_SEEN 0x942
/// Move has pushed the stack of crates in LunpaFortressCave onto its hole,
/// where it sinks.
/// ★ Named from what the player sees: the stack of wooden crates that Move
/// pushes down into the cave's hole (its render)
#define FLAG_LUNPA_FORTRESS_CRATES_MOVED 0x943
/// Whirlwind has blown away the plant that hid the first switch.
/// ★ Named from what the player sees: a leafy plant on the cave wall, which
/// Whirlwind blows away (its render)
#define FLAG_LUNPA_FORTRESS_FIRST_SWITCH_PLANT_BLOWN_AWAY 0x944
/// Whirlwind has blown away the plant that hid the passage.
/// ★ Named from what the player sees: a leafy plant on the cave wall, which
/// Whirlwind blows away (its render)
#define FLAG_LUNPA_FORTRESS_PASSAGE_PLANT_BLOWN_AWAY 0x945
/// Whirlwind has blown away the plant that hid the second switch.
/// ★ Named from what the player sees: a leafy plant on the cave wall, which
/// Whirlwind blows away (its render)
#define FLAG_LUNPA_FORTRESS_SECOND_SWITCH_PLANT_BLOWN_AWAY 0x946
/// The first switch Reveal shows in the cave wall has been searched.
/// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
#define FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED 0x947
/// As FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED, for the second switch.
/// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
#define FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED 0x948
/// The guard in the small room in the middle of LunpaFortressUpperFloor has
/// been beaten, as a Brigand.
/// ★ Named from the game: "Brigand" (MsgEnemyName)
#define FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN 0x949
/// The three guards who fight together have been beaten.
/// ★ Named from the game: "Who are you!?" (MsgLunpaWho3)
#define FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN 0x94a
/// The east guard has been beaten, as a Brigand.
/// ★ Named from the game: "Brigand" (MsgEnemyName)
#define FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN 0x94b
/// The west guard has been beaten, as a Brigand.
/// ★ Named from the game: "Brigand" (MsgEnemyName)
#define FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN 0x94c
/// Donpa's reward, the Djinni in his room, has joined the party.
/// ★ Named from the game: "Donpa would offer a reward"
/// (MsgLunpaSomeonePunishDodonpa)
#define FLAG_LUNPA_FORTRESS_REWARD_TAKEN 0x94e
/// ★ Named from the game: "Elixir" (MsgItemName)
#define FLAG_LUNPA_FORTRESS_CHEST_ELIXIR 0xfa2
/// ★ Named from the game: "Lucky Medal" (MsgItemName)
#define FLAG_LUNPA_FORTRESS_CHEST_LUCKY_MEDAL 0xfa3
/// ★ Named from the game: "Mythril Circlet" (MsgItemName)
#define FLAG_LUNPA_FORTRESS_CHEST_MYTHRIL_CIRCLET 0xfa4
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_LUNPA_FORTRESS_CHEST_COINS 0xfa5
/// ★ Named from the game: "Vial" (MsgItemName)
#define FLAG_LUNPA_FORTRESS_BARREL_VIAL 0xfa6
/// ★ Named from the game: "Power Bread" (MsgItemName)
#define FLAG_LUNPA_FORTRESS_CHEST_POWER_BREAD 0xfa7
/// Caught or taken from its hook.
/// ★ Named from the game: "Cell Key" (MsgItemName)
#define FLAG_LUNPA_FORTRESS_CELL_KEY 0xf2e
/// Unused: the four swords LunpaFortress's unused pickups give, each once.
/// ★ Named from the game: "Arctic Blade" (MsgItemName)
#define FLAG_LUNPA_FORTRESS_ARCTIC_BLADE 0xf2a
/// ★ Named from the game: "Masamune" (MsgItemName)
#define FLAG_LUNPA_FORTRESS_MASAMUNE 0xf2b
/// ★ Named from the game: "Silver Blade" (MsgItemName)
#define FLAG_LUNPA_FORTRESS_SILVER_BLADE 0xf2c
/// ★ Named from the game: "Shamshir" (MsgItemName)
#define FLAG_LUNPA_FORTRESS_SHAMSHIR 0xf2d

// McCoy's Palace in Bilibin (McCoyPalace and McCoyPalaceGrounds)
/// McCoy's gate guards have agreed to let the party in to see their lord, and
/// the gate stands open; from then on the east guard only warns that McCoy is
/// a difficult man. The palace also sets it when the captain lets slip that
/// the barricade may not need a key.
/// ★ Named from the game: "All right. I shall let you pass."
/// (MsgBilibinLordOnlyMeet)
#define FLAG_MCCOY_PALACE_ADMITTED 0x84a
/// The party has been inside McCoy's Palace: its setup sets this on every
/// entry. Bilibin tests it with FLAG_MCCOY_PALACE_ADMITTED, so that a party
/// let in that never went in is scolded at the gate for keeping McCoy waiting.
/// ★ Named from its area name: McCoy's Palace
#define FLAG_MCCOY_PALACE_VISITED 0x84b
/// After McCoy turned the party away, his captain let slip that the
/// barricade may not need a key.
/// ★ Named from the game: "That barricade... You may not need a key."
/// (MsgBilibinNameSorryRejected)
#define FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY 0x84f
/// McCoy's captain has seen the party off after McCoy's reward.
/// ★ Named from the game: "You are always welcome in the palace"
/// (MsgBilibinAlwaysWelcomeInPalaceLord)
#define FLAG_MCCOY_PALACE_PARTY_REWARDED 0x85e
/// The party refused McCoy's job at the first audience.
/// ★ Named from the game: "Yeh cannae handle the job."
/// (MsgBilibinTooYoungForTheJob)
#define FLAG_MCCOY_PALACE_JOB_REFUSED 0x85f
/// ★ Named from the game: "Elixir" (MsgItemName)
#define FLAG_MCCOY_PALACE_BARREL_ELIXIR 0xf55
/// ★ Named from the game: "Smoke Bomb" (MsgItemName)
#define FLAG_MCCOY_PALACE_BARREL_SMOKE_BOMB 0xf56
/// Catch has taken the Nut floating outside.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_MCCOY_PALACE_GROUNDS_NUT_CAUGHT 0xfd2
// Mercury Lighthouse
/// Mia, waiting at the statue that blocks Mercury Lighthouse's door, has told
/// the party she senses someone inside, where only she and Alex can enter.
/// From then on she stands facing the door, and talking to her only gets her
/// remark that the statue blocks the entrance.
/// ★ Named from the game: "Somebody...is here."
/// (MsgImilSomebodyHere)
#define FLAG_MIA_SENSED_SOMEONE_INSIDE 0x870
/// Move has pushed aside the statue before Mercury Lighthouse's door, Mia has
/// learned the word Psynergy, and her Ply has opened the door for her to go in.
/// Afterwards she is gone from outside and the statue stays moved.
#define FLAG_MIA_OPENED_MERCURY_LIGHTHOUSE 0x871
/// Back from the battle with the Lizard Man on the lighthouse's ground floor,
/// Mia has nodded to the leader and gone on deeper into the lighthouse. Until
/// then, coming in by the door replays the Lizard Man blocking her way.
#define FLAG_MIA_ENTERED_MERCURY_LIGHTHOUSE 0x872
/// Move has pushed aside the second statue that blocked Mia, and she has
/// joined the party with Fizz, a Mercury Djinni. Afterwards she is gone from
/// that spot and the statue stays where it was pushed.
/// ★ Named from the game: "joined your party!" (MsgMercuryThoughtSo)
#define FLAG_MIA_JOINED_PARTY 0x873
/// The statue on the lighthouse's middle floor has been pushed or moved onto
/// its floor switch, and the gate it works stays open for good. Until then the
/// gate opens only while the leader stands on the switch.
#define FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN 0x874
/// Ply cast on the heart of the pool, as its inscription bids: the lights
/// cycle and the pillar routes open.
/// ★ Named from the game: "He who honors the heart of the goddess"
/// (MsgMercuryWhoHonorsHeart)
#define FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED 0x875
/// All three small stone pillars on the lighthouse's lower floor are in place,
/// the water runs, and it carries the party down to the ground floor. Moving
/// one out again clears it. The ground floor's entrances from the lower floor
/// place the party by whether it is set.
#define FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN 0x876
/// A pillar pushed along the upper floor's row has sent the blue statue on its
/// ledge sliding off to the floor below, where Move can set it on the stair
/// gate's switch (FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN). The scene
/// places the statue below from then on.
#define FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL 0x877
/// The fallen statue rests on the floor switch of MercuryLighthouseUpper and
/// holds its gate open.
/// ★ Named from what the player sees: the barred gate in the middle arch of the
/// upper floor standing open (its render)
#define FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN 0x878
/// Saturos is beaten in the Lighthouse Aerie and Alex has come for him; set
/// with FLAG_SATUROS_BEATEN_AT_MERCURY.
/// ★ Named from the game: "Alex!" (MsgMercuryGot)
#define FLAG_MERCURY_AERIE_ALEX_APPEARED 0x880
/// The party has beaten Saturos in the Lighthouse Aerie, and Alex has come for
/// him; set with FLAG_MERCURY_AERIE_ALEX_APPEARED. The Fountain of Healing at
/// the lighthouse's foot flows again and people come to drink from it, and
/// Imil and Bilibin talk of what has changed.
/// ★ Named from the game: "we got him!" (MsgMercuryGot)
#define FLAG_SATUROS_BEATEN_AT_MERCURY 0x881
/// The Psynergy Armor in the entrance's chest is taken.
/// ★ Named from the game: "Psynergy Armor" (MsgItemName)
#define FLAG_MERCURY_LIGHTHOUSE_PSYNERGY_ARMOR 0xf65
/// The mimic in the entrance's other chest is beaten.
/// ★ Named from the game: "really a Mimic!" (MsgChestWasMimic)
#define FLAG_MERCURY_LIGHTHOUSE_MIMIC_BEATEN 0xef0

// Mogall Forest
/// On the party's first arrival in Mogall Forest's first area, an ape has
/// peeked out at the leader and hidden in its stump.
/// ★ Named from the game: "Ape" (MsgEnemyName)
#define FLAG_MOGALL_FOREST_FIRST_APE_SEEN 0x89c
/// In Mogall Forest's third area, the east ape has come out again and led the
/// party south to the Killer Ape's hiding place, where the Killer Ape then
/// bursts out to fight.
/// ★ Named from the game: "Killer Ape" (MsgEnemyName)
#define FLAG_MOGALL_FOREST_LED_TO_KILLER_APE 0x89d
/// The party has come back from the Killer Ape's battle in Mogall Forest's
/// third area, so the Killer Ape no longer bursts out. With Force learned
/// (FLAG_NYUNPA_TAUGHT_FORCE), entering that area then sets
/// FLAG_NYUNPA_ENLIGHTENED.
/// ★ Named from the game: "Killer Ape" (MsgEnemyName)
#define FLAG_MOGALL_FOREST_KILLER_APE_BEATEN 0x89e
// Suhalla
/// Iodem has questioned the two wounded soldiers in a house in Suhalla, who
/// lost Sheba in a sandstorm and warn that monsters cause the storms. Stepping
/// into the house sets it even once Iodem has reached Babi Lighthouse, when
/// the scene no longer plays; Suhalla Desert and Suhalla Gate read it.
/// ★ Named from the game: "You soldiers were supposed to be heading for
/// Babi Lighthouse!" (MsgSuhallaIodem)
#define FLAG_IODEM_MET_WOUNDED_SOLDIERS 0x9b0
/// The Smoke Bomb in one of the houses' ovens is found.
/// ★ Named from the game: "Smoke Bomb" (MsgItemName)
#define FLAG_SUHALLA_INTERIORS_OVEN_SMOKE_BOMB 0xfb0

// Suhalla Gate
/// The soldier east of Suhalla Gate's passage has gasped that Venus Lighthouse
/// was attacked by strange wanderers, then flickered and vanished for good.
#define FLAG_SUHALLA_GATE_SOLDIER_VANISHED 0x897
/// Iodem has questioned the wounded guards at Suhalla Gate about the strange
/// group that beat them, and the party is sent on after the attackers. The
/// guards then speak as the wounded, and a defeat returns the party to the
/// house beside the gate.
/// ★ Named from the game: "We tried to hold them back... but they were too
/// powerful!" (MsgSuhallaMeaning)
#define FLAG_SUHALLA_GATE_ATTACK_SEEN 0x89f
/// The side the party last came through, which Retreat returns them to.
/// ★ Named from its area name: Suhalla Gate
#define FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH 0x8fb
/// ★ Named from its area name: Suhalla Gate
#define FLAG_SUHALLA_GATE_ENTERED_FROM_EAST 0x8fc

// Suhalla Desert
/// Iodem has told the party that Sheba is nowhere to be found and asked
/// whether to search the desert again or press on to Lalivero.
/// ★ Named from the game: "Should we search the desert once more or press on
/// to Lalivero?" (MsgSuhallaNowhereFound)
#define FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA 0x9b5
/// The first storm to rise in Suhalla Desert while Iodem travels with the
/// party has brought him out to warn that they must get past the storms to
/// reach Babi Lighthouse.
/// ★ Named from the game: "There'll be trouble if we get caught up in one of
/// those..." (MsgSuhallaSandstorm)
#define FLAG_SUHALLA_DESERT_IODEM_WARNED 0x9b6
/// Douse has stopped the sandstorm that chases the party in the second scene,
/// for good.
/// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
#define FLAG_SUHALLA_DESERT_CHASING_STORM_DOUSED 0x9b7
// The desert's chests, set once each is opened or its mimic beaten.
/// ★ Named from the game: "Virtuous Armlet" (MsgItemName)
#define FLAG_SUHALLA_DESERT_VIRTUOUS_ARMLET 0xfb1
/// ★ Named from the game: "Lucky Medal" (MsgItemName)
#define FLAG_SUHALLA_DESERT_LUCKY_MEDAL 0xfb2
/// ★ Named from the game: "Cookie" (MsgItemName)
#define FLAG_SUHALLA_DESERT_COOKIE 0xfb3
/// ★ Named from the game: "really a Mimic!" (MsgChestWasMimic)
#define FLAG_SUHALLA_DESERT_MIMIC_BEATEN 0xef6
// Tolbi
/// The snack vendor has given away her leftover corn.
/// ★ Named from the game: "these are leftovers, but have some!"
/// (MsgTolbiLeftovers)
#define FLAG_TOLBI_CORN_GIVEN 0x8bf
/// Babi's soldiers have shown the party his picture.
/// ★ Named from the game: "have you seen anyone who looks like this?"
/// (MsgTolbiSeenAnyoneWho)
#define FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI 0x8c0
/// The gambler has let the party go first into Tolbi Spring.
/// ★ Named from the game: "I'll give you the chance to go first."
/// (MsgTolbiWantTestLuck)
#define FLAG_TOLBI_SPRING_OPENED 0x8c1

// Tolbi interiors: the woman who finds a Lucky Medal after Colosso
/// After Colosso, on the party's first entry into the inn's northeast guest
/// room, a woman has found a Lucky Medal on the floor and hidden it behind the
/// trash pile. She then stands beside that barrel.
/// ★ Named from the game: "She just hid that thing she found behind that trash
/// pile." (MsgTolbiHehHehSheJustHid)
#define FLAG_TOLBI_INTERIORS_MEDAL_HIDDEN 0x8bc
/// The party has taken the Lucky Medal from the barrel where the woman hid it;
/// she then stops teasing and greets the party, startled.
/// ★ Named from the game: "Lucky Medal" (MsgItemName)
#define FLAG_TOLBI_INTERIORS_MEDAL_TAKEN 0x8bd
/// The medal woman has greeted the party, startled, since it took the medal.
/// ★ Named from the game: "Oh... Hello!" (MsgTolbiHello)
#define FLAG_TOLBI_INTERIORS_MEDAL_WOMAN_SAID_HELLO 0x8be

// Tolbi ship: the crossing of the Karagol Sea from Kalay to Tolbi
/// The ship is at anchor off Crossbone Isle, the rowers resting, until Kaja
/// switches the two teams and the ship sails on. The deck, the hold, the
/// lookout and the world map, where the ship waits by the island, all follow
/// it.
/// ★ Named from the game: "We've reached the island!"
/// (MsgTolbiShipReachedIslandRowers)
#define FLAG_TOLBI_SHIP_AT_ISLAND 0x8a0
/// The lookout has sighted the monsters of one of the ship's attacks, and the
/// ship's place plays MUS_029, the monsters' theme, instead of MUS_041. It is
/// cleared when the party fights them on the rowing deck.
/// ★ Named from the game: "Monsters sighted!" (MsgTolbiShipAvast)
#define FLAG_TOLBI_SHIP_MONSTERS_SIGHTED 0x8f0
/// At anchor off the island, the party has agreed to Sean and Ouranos' request
/// to scout it for monsters, so the tour group can know whether it is safe to
/// go ashore.
/// ★ Named from the game: "I'm sure no one will complain if we scout out the
/// island." (MsgTolbiShipSeeYoureGoingGoFor)
#define FLAG_TOLBI_SHIP_ISLAND_SCOUT_AGREED 0x903
/// At Kalay, the captain, Kaja and a crewman have wondered what became of the
/// replacement ship from Tolbi, on deck or below. On deck the captain then
/// resolves to sail anyway, trusting his anchor charm, and the crewman slips
/// away.
/// ★ Named from the game: "I wonder what could have happened."
/// (MsgTolbiShipWonderCouldHaveHappened)
#define FLAG_TOLBI_SHIP_CREW_WONDERED_AT_DELAY 0x920
/// In the captain's cabin at Kalay, the passengers have pressed the captain
/// and heard he decided to sail, Ouranos and Sean have offered to guard the
/// deck, and the captain has found his anchor charm gone and will not sail
/// without it.
/// ★ Named from the game: "But the captain decided to set sail just a second
/// ago." (MsgTolbiShipYoureTryingLaunchShip)
#define FLAG_TOLBI_SHIP_CAPTAIN_DECIDED_TO_SAIL 0x921
/// The party has given the captain back his Anchor Charm, found on the mast,
/// and agreed to guard the oarsmen. The passengers who promised to row then
/// stand on deck, and the lookout takes his place on the mast.
/// ★ Named from the game: "It's my lucky anchor!"
/// (MsgTolbiShipItsMyLuckyAnchor)
#define FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED 0x925
/// Kaja has told the oarsmen the ship sails without its replacement, and asked
/// the party to bring a passenger to row whenever an oarsman falls. A defeat
/// at sea clears it with the rest of the voyage's flags.
/// ★ Named from the game: "Our replacement never arrived, but we're shoving
/// off just the same." (MsgTolbiShipOurReplacementNeverArrivedBut)
#define FLAG_TOLBI_SHIP_SAILING_WITHOUT_REPLACEMENT 0x926
/// The ship is under way and the oars are moving.
/// ★ Named from the game: "Row those oars!" (MsgTolbiShipRowThoseOars)
#define FLAG_TOLBI_SHIP_ROWING 0x927
/// The monster attacks fought off. After the first an oarsman is hurt, and
/// from then on passengers the party chooses take the empty seats.
/// ★ Named from the game: "Looks like you took care of those sea monsters."
/// (MsgTolbiShipHeyAreYouOk)
#define FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF 0x928
/// ★ Named from the game: "Looks like you took care of those sea monsters."
/// (MsgTolbiShipHeyAreYouOk)
#define FLAG_TOLBI_SHIP_SECOND_ATTACK_FOUGHT_OFF 0x929
/// ★ Named from the game: "Looks like you took care of those sea monsters."
/// (MsgTolbiShipHeyAreYouOk)
#define FLAG_TOLBI_SHIP_THIRD_ATTACK_FOUGHT_OFF 0x92a
/// The party has jumped down to the deck and beaten the Kraken, and Kaja has
/// thanked Isaac. Another oarsman is lost, so the passenger chosen next takes
/// the fourth seat (FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS).
/// ★ Named from the game: "You did good against that Kraken."
/// (MsgTolbiShipThankIsaacDidGoodAgainst)
#define FLAG_TOLBI_SHIP_KRAKEN_DEFEATED 0x92b
/// The passenger chosen to row in the first empty seat: nine flags, one for
/// each passenger, in the order of TolbiShipPassenger (tolbi_ship_interiors.c).
/// Each later seat has nine of its own, from
/// FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS.
/// ★ Named from the game: "go ask one of the passengers to row"
/// (MsgTolbiShipHeyAreYouOk)
#define FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS 0x92c
/// ★ Named from the game: "the sea monsters got one of our rowers"
/// (MsgTolbiShipHeyAreYouOk)
#define FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS 0x935
/// ★ Named from the game: "the sea monsters got one of our rowers"
/// (MsgTolbiShipHeyAreYouOk)
#define FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS 0x917
/// ★ Named from the game: "the sea monsters got one of our rowers"
/// (MsgTolbiShipHeyAreYouOk)
#define FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS 0x990
/// The ship has reached Tolbi.
/// ★ Named from the game: "It is good that the ship has arrived safely."
/// (MsgTolbiShipGoodShipHasArrivedSafely)
#define FLAG_TOLBI_SHIP_REACHED_TOLBI 0x93e
/// The party lost a battle at sea and is back in the passenger hold, where the
/// voyage starts over. XXX: no code of this game reads it; a reader would say
/// what it changes.
#define FLAG_TOLBI_SHIP_DEFEATED_AT_SEA 0x902
// Tunnel Ruins
/// Whirlwind has blown away the third leafy plant in TunnelRuinsGreatStatue,
/// for good.
/// ★ Named from what the player sees: a leafy plant in a pot that Whirlwind
/// blows away (its render)
#define FLAG_TUNNEL_RUINS_PLANT_BLOWN_AWAY 0x970
/// The first of TunnelRuinsGreatStatue's two sinking pillars has gone down
/// into the floor on its column, leaving a puddle in its place.
/// ★ Named from what the player sees: the stone pillar gone into the floor
/// with a puddle where it stood (its render)
#define FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR 0x971
/// As FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR, for the second pillar, which sinks
/// only after Douse has soaked it.
/// ★ Named from what the player sees: the second stone pillar gone into the
/// floor with a puddle where it stood (its render)
#define FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR 0x972
/// The first of the two stone pillars in TunnelRuinsLadyStatues' east room is
/// pushed to the west of its row; the scene puts it back there on the next
/// visit.
#define FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST 0x974
/// The second of the two stone pillars in TunnelRuinsLadyStatues' east room
/// is pushed to the west of its row.
/// ★ Named from what the player sees: the two stone pillars side by side, the
/// second pushed west (its render)
#define FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST 0x975
/// All five colored statues of a woman in TunnelRuinsLadyStatues stand on
/// their marks, and the door before them is open; moving one off closes it
/// again. Coming back into the room puts the statues back on their marks.
#define FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN 0x984
/// The party has entered TunnelRuinsLadyStatues' room of four paths over a
/// pit. XXX: no code of this game reads it; a reader would say what it
/// records.
/// ★ Named from what the player sees: four long stone paths over a pit (its
/// render)
#define FLAG_TUNNEL_RUINS_FOUR_PATHS_ROOM_ENTERED 0x973
/// A chest in the second scene.
/// ★ Named from the game: "Oracle's Robe" (MsgItemName)
#define FLAG_TUNNEL_RUINS_ORACLES_ROBE 0xf32
/// A chest in the second scene.
/// ★ Named from the game: "Asura's Armor" (MsgItemName)
#define FLAG_TUNNEL_RUINS_ASURAS_ARMOR 0xf33
// Vale
/// The prologue's stormy night: lightning over Vale, the rockslide and the
/// monsters loose in the village. It is set when Dora wakes Isaac and on every
/// arrival in the storm's map, and cleared when the strangers leave the boys
/// lying in the storm; Vale's houses empty and their doors lead into the storm.
/// ★ Named from the game: "I just knew a storm like this would knock it loose!"
/// (MsgValeSureHelp)
#define FLAG_VALE_STORM 0x834
/// The party has reached Kalay: every Kalay scene sets it on entry. Vale's
/// villagers then talk of the journey, Garet's homecoming plays, Vault leads to
/// its later scene and the Sanctum's talk changes. XXX: the patched editions
/// also set it on beating Lamakan Desert's Manticore; why is not known.
/// ★ Named from its area name: Kalay
#define FLAG_REACHED_KALAY 0x87a
/// Catch has taken the Nut floating in Vale.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_VALE_NUT_CAUGHT 0xfd0

// Vale, Kraden's cottage
/// Kraden has wondered aloud, as the party walks up to his cottage, who the
/// strangers were.
/// ★ Named from the game: "Just who were they?" (MsgValeWho)
#define FLAG_KRADEN_WONDERED_ABOUT_STRANGERS 0x800
/// The man hurt in the rockslide believes he will die.
/// ★ Named from the game: "This is it for me. Save yourselves..."
/// (MsgValeSaveYourselves)
#define FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP 0x82f
/// He found he was unhurt and ran to the plaza.
/// ★ Named from the game: "I'm not hurt at all!"
/// (MsgValeRockslideDestroyedFence)
#define FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED 0x839
/// The monsters by the broken fence on the stormy night, each fought once.
/// ★ Named from the game: "Rockslide...destroyed fence... Monsters
/// everywhere..." (MsgValeRockslideDestroyedFence)
#define FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT 0x87d
/// ★ Named from the game: "Rockslide...destroyed fence... Monsters
/// everywhere..." (MsgValeRockslideDestroyedFence)
#define FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT 0x87e
/// A monster was fought on the stormy night: Vale's music changes to MUS_023.
/// ★ Named from the game: "Rockslide...destroyed fence... Monsters
/// everywhere..." (MsgValeRockslideDestroyedFence)
#define FLAG_VALE_STORM_MONSTER_FOUGHT 0x900
/// The jar's Elixir by the cottage; after Kalay it is searchable only from the
/// third patch.
/// ★ Named from the game: "Elixir" (MsgItemName)
#define FLAG_VALE_KRADEN_JAR_ELIXIR 0xf41

// Vale, the plaza
/// A villager asked whether Isaac is going to Kraden's cottage.
/// "Do you need to go past here? You must be going to Kraden's cottage..."
/// ★ Named from the game: "You must be going to Kraden's cottage..."
/// (MsgValeDoYouNeedToGo)
#define FLAG_VALE_ASKED_IF_GOING_TO_KRADEN 0x806
/// A villager has told Garet off once for crushing her flowers.
/// "Garet, you make me so mad sometimes!"
/// ★ Named from the game: "Your training stone crushed my flowers!"
/// (MsgValeYouMakeMeSoMad)
#define FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS 0x807
/// Back from Kalay, a young man in Vale has asked Isaac whether Jenna is back
/// yet and made him promise to bring her next time; after that he only repeats
/// the promise.
/// ★ Named from the game: "back yet?" (MsgValeIsJennaBackYet)
#define FLAG_VALE_ASKED_IF_JENNA_IS_BACK 0x81f
/// A bearded man in a long robe has jumped out in front of Isaac on the path
/// to the sacred peak and warned him off; from then on he watches Isaac and
/// asks again whenever he comes near.
/// ★ Named from the game: "You're not planning on sneaking up to Mt. Aleph,
/// are you?" (MsgValeNotSneakingUpMtAleph)
#define FLAG_VALE_WARNED_AGAINST_MT_ALEPH 0x823

// Vale on the stormy night, the paths outside
/// The first of the four rocks that fall across the paths (actors 11 to 14).
/// ★ Named from the game: "We can't get around this rock!"
/// (MsgValeCantGetAroundThisRock)
#define FLAG_VALE_STORM_FIRST_ROCK_FELL 0x830
/// The second rock to fall across the paths.
/// ★ Named from the game: "We can't get around this rock!"
/// (MsgValeCantGetAroundThisRock)
#define FLAG_VALE_STORM_SECOND_ROCK_FELL 0x831
/// The third rock to fall across the paths.
/// ★ Named from the game: "We can't get around this rock!"
/// (MsgValeCantGetAroundThisRock)
#define FLAG_VALE_STORM_THIRD_ROCK_FELL 0x832
/// The fourth rock to fall across the paths.
/// ★ Named from the game: "We can't get around this rock!"
/// (MsgValeCantGetAroundThisRock)
#define FLAG_VALE_STORM_FOURTH_ROCK_FELL 0x833
/// A villager running out of his house shoved Isaac.
/// "Huh? Isaac?"
/// ★ Named from the game: "Huh?" (MsgValeHuh)
#define FLAG_VALE_STORM_BUMPED_INTO_VILLAGER 0x835
/// On the stormy night, Isaac has heard Garet struggling to save his chest of
/// valuables. Walking away from him afterwards has Garet call after Isaac.
/// ★ Named from the game: "Ugh... Hrnghhh..." (MsgValeUghHrnghhh)
#define FLAG_VALE_STORM_HEARD_GARET 0x836
/// Garet left his things behind and joined Isaac.
/// ★ Named from the game: "OK! Let's get out of here!" (MsgValeKnowRightOk)
#define FLAG_VALE_STORM_GARET_JOINED 0x837
/// The Boulder slipped toward the village while the elders held it with their
/// Psynergy.
/// ★ Named from the game: "The Boulder! It's falling!"
/// (MsgValeTheBoulderIsFalling)
#define FLAG_VALE_BOULDER_FELL 0x838
/// Isaac has seen Felix in the river, clinging on while Kyle and Felix's
/// parents try to save him and the rope will not reach; the attempt fails
/// later, when the Boulder comes down
/// (FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY). ★ Named from the game: "Hang on,"
/// (MsgValeNoBrother)
#define FLAG_VALE_STORM_SAW_FELIX_HANGING_ON 0x83a
/// The Boulder has come down the river and swept away Felix, his parents and
/// Kyle, as Jenna came back with help (ValeStorm_ShowBoulderSweepingDownriver);
/// Dora, Jenna, Garet and the villager are left at the bank. Afterwards Dora
/// says "No... It can't be. Kyle..." and Jenna "Don't leave me all alone...".
/// ★ Named from the game: "Please... Don't leave me all alone..."
/// (MsgValeMomDadFelixPleaseDontLeave)
#define FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY 0x841
/// After the Boulder swept Kyle, Jenna's parents and Felix away, Isaac has
/// gone for help and Garet has set off after him. Arriving among the villagers
/// then plays the meeting with the two strangers.
/// ★ Named from the game: "I can't let him go alone..." (MsgValeIsaac)
#define FLAG_VALE_STORM_GARET_LEFT 0x842
/// Dora and Kyle sent Isaac to the plaza.
/// ★ Named from the game: "go south to get to the plaza" (MsgValeKnowWayGo)
#define FLAG_VALE_STORM_SENT_TO_PLAZA 0x87b
/// The first of the two monsters that attack on the stormy night's paths has
/// been fought, once; it also sets FLAG_VALE_STORM_MONSTER_FOUGHT.
#define FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT 0x87c
/// As FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT, for the second monster on the
/// paths.
#define FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT 0x87f

// Vale, the village
/// On the stormy night, in the plaza, Garet's grandfather sent two villagers
/// to aid the elders holding back the Boulder, and the boys back to the river
/// with Jenna to show the way.
/// ★ Named from the game: "Go aid the elders!" (MsgValeGoAidElders)
#define FLAG_VALE_STORM_SENT_TO_AID_ELDERS 0x840
/// Whirlwind blew away the first of the three clumps of ivy (actors 8 to 10)
/// in the village.
/// ★ Named from what the player sees: ivy growing on the cliff above a house,
/// which Whirlwind blows away (its render)
#define FLAG_VALE_FIRST_IVY_BLOWN_AWAY 0x90b
/// Whirlwind blew away the second clump of ivy.
/// ★ Named from what the player sees: ivy growing on the cliffs, which
/// Whirlwind blows away (its render)
#define FLAG_VALE_SECOND_IVY_BLOWN_AWAY 0x90c
/// Whirlwind blew away the third clump of ivy.
/// ★ Named from what the player sees: ivy growing on the cliffs, which
/// Whirlwind blows away (its render)
#define FLAG_VALE_THIRD_IVY_BLOWN_AWAY 0x90d

// Vale, around Isaac's house
/// Back from Kalay, Mind Read showed that Dora is ill.
/// ★ Named from the game: "Dora was struck with an illness?"
/// (MsgValeDoraWasStruckWithIllness)
#define FLAG_READ_THOUGHTS_ABOUT_DORA 0x81c
/// Isaac visited Dora in her sickbed and she got up.
/// ★ Named from the game: "Who told you I was ill?" (MsgValeWhoToldIll)
#define FLAG_VISITED_SICK_DORA 0x81e
// The containers, set once each is searched; the last three hold their finds
// only once the party is back from Kalay.
/// A wooden box's Herb.
/// ★ Named from the game: "Herb" (MsgItemName)
#define FLAG_VALE_WOODEN_BOX_HERB 0xf47
/// A jar's 5 coins.
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_VALE_JAR_COINS 0xf48
/// A barrel's 1 coin.
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_VALE_BARREL_COIN 0xf49
/// A jar's Vial.
/// ★ Named from the game: "Vial" (MsgItemName)
#define FLAG_VALE_JAR_VIAL 0x8ad
/// A barrel's 123 coins.
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_VALE_BARREL_COINS 0x8ae
/// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
#define FLAG_VALE_PSYNERGY_STONE 0x8af

// Vale, houses and shops
/// After the party has left Vale, the item merchant has given Isaac a Herb as
/// a farewell gift; afterwards he urges Isaac to save Jenna.
/// ★ Named from the game: "This is my farewell gift."
/// (MsgValeThisIsMyFarewellGift)
#define FLAG_VALE_FAREWELL_HERB_RECEIVED 0x81b
/// Back from Kalay, Garet has surprised his family at home on the party's
/// first visit to his house, and Ivan and Mia have introduced themselves.
/// ★ Named from the game: "Mom, Dad... I'm home..." (MsgValeHey2)
#define FLAG_GARET_CAME_HOME 0x81d
// The houses' containers, set once each is searched (not on the stormy night)
/// ★ Named from the game: "Antidote" (MsgItemName)
#define FLAG_VALE_INTERIORS_SHOPS_BARREL_ANTIDOTE 0xf42
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_VALE_INTERIORS_SHOPS_JAR_6_COINS 0xf43
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_VALE_INTERIORS_SHOPS_JAR_2_COINS 0xf44
/// ★ Named from the game: "Smoke Bomb" (MsgItemName)
#define FLAG_VALE_INTERIORS_SHOPS_JAR_SMOKE_BOMB 0xf45
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_VALE_INTERIORS_SHOPS_JAR_3_COINS 0xf46

// Vale during the eruption
/// Isaac and Garet have come back down the mountain to the villagers waiting
/// before the sanctum during the eruption, and followed the Great Healer in.
/// Sol Sanctum's entrance then changes the cells before its door.
/// ★ Named from the game: "You're safe!"
/// (MsgValeMtAlephWasInactive)
#define FLAG_VALE_RETURNED_FROM_MT_ALEPH 0x879

// Vale Cave, where Kite hides
/// The second room's chest.
/// ★ Named from the game: "Halt Gem" (MsgItemName)
#define FLAG_VALE_CAVE_HALT_GEM 0xfc2
/// The second room's other chest.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_VALE_CAVE_NUT 0xfc3
// Vault: the stolen Shaman's Rod, and its containers, set once each is
// searched
/// Set when Ivan first shows the party his strange powers at the mayor's.
/// ★ Named from the game: "I... I do possess strange powers."
/// (MsgVaultDoPossessStrangePowers)
#define FLAG_VAULT_MET_IVAN 0x850
/// Set once the mayor has thanked the party for helping Ivan.
/// ★ Named from the game: "Thank you." (MsgVaultYoureGoingHelpIvan)
#define FLAG_VAULT_MAYOR_THANKED_PARTY 0x851
/// Set when the party first approaches the two suspects at the inn.
/// ★ Named from the game: "What do you want?" (MsgVaultWant)
#define FLAG_VAULT_SUSPECTS_APPROACHED 0x852
/// Set once the mayor has given the party the Water of Life.
/// ★ Named from the game: "Water of Life" (MsgItemName)
#define FLAG_VAULT_WATER_OF_LIFE_RECEIVED 0x857
/// Set once the crate is pushed aside, opening the passage behind it.
/// ★ Named from what the player sees: a wooden crate at the foot of the
/// loft's ladder, which Move pushes aside (its render)
#define FLAG_VAULT_CRATE_MOVED 0x859
/// Set when the party agrees to help Ivan find the rod; while it is set and
/// the thieves are free, Ivan waits in Vault whenever the party leaves town.
/// ★ Named from the game: "Would you help me find the rod?"
/// (MsgVaultWouldReallyWouldHelpMe)
#define FLAG_VAULT_IVAN_JOINED_ROD_SEARCH 0x856
/// Set once Ivan has read the minds of the two cornered suspects.
/// ★ Named from the game: "What did you learn" (MsgVaultLearn)
#define FLAG_VAULT_SUSPECTS_MIND_READ 0x853
/// Set as the three thieves attack the party in the loft, before the battle.
/// ★ Named from the game: "Get 'em!!!" (MsgVaultVaultCutFree)
#define FLAG_VAULT_THIEVES_ATTACKED 0x854
/// Set when, back in Vault, the party hears that the thieves it caught have
/// broken out: from the mayor's welcome in his house, or from the old man who
/// was talking about the party with him. The clear password carries it.
/// ★ Named from the game: "Those thieves you caught have escaped from jail."
/// (MsgVaultIsaacYouveComeBackVault)
#define FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES 0x868
/// A wooden box's Nut.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_VAULT_BOX_NUT 0xf4a

// Vault interiors: their containers, set once each is searched
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_VAULT_INTERIORS_JAR_COINS 0xf4b
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_VAULT_INTERIORS_BARREL_COINS 0xf4c
/// ★ Named from the game: "Sleep Bomb" (MsgItemName)
#define FLAG_VAULT_INTERIORS_BARREL_SLEEP_BOMB 0xf4d
/// ★ Named from the game: "Mint" (MsgItemName)
#define FLAG_VAULT_INTERIORS_BOX_MINT 0xf4e

// Vault cave: its switches, and its chests, set once each is opened
/// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
#define FLAG_VAULT_CAVE_SWITCH_FLIPPED 0x9a8
/// The southern pillar rests on its switch in VaultCaveVambrace, and the way
/// is open.
/// ★ Named from what the player sees: a pink stone pillar with a flame on top,
/// pushed onto the floor switch (its render)
#define FLAG_VAULT_CAVE_SOUTH_PILLAR_ON_SWITCH 0x9a9
/// The northern pillar rests on its switch in VaultCaveMouth, unstruck by the
/// water drops, and the way is open.
/// ★ Named from what the player sees: a pink stone pillar with a flame on top,
/// resting on the floor switch (its render)
#define FLAG_VAULT_CAVE_NORTH_PILLAR_ON_SWITCH 0x9aa
/// The chest in VaultCavePassage's pool room, a Mimic (FIND_MIMIC).
/// ★ Named from the game: "really a Mimic!" (MsgChestWasMimic)
#define FLAG_VAULT_CAVE_PASSAGE_MIMIC_BEATEN 0xef3
/// ★ Named from the game: "Vambrace" (MsgItemName)
#define FLAG_VAULT_CAVE_VAMBRACE_CHEST 0xfc4

// Vault Jail: its chests, set once each is opened
/// ★ Named from the game: "Water of Life" (MsgItemName)
#define FLAG_JAIL_CHEST_WATER_OF_LIFE 0xf8c
/// ★ Named from the game: "Smoke Bomb" (MsgItemName)
#define FLAG_JAIL_CHEST_SMOKE_BOMB 0xf8d
/// ★ Named from the game: "Lucky Medal" (MsgItemName)
#define FLAG_JAIL_CHEST_LUCKY_MEDAL 0xf8e
/// ★ Named from the game: "Antidote" (MsgItemName)
#define FLAG_JAIL_CHEST_ANTIDOTE 0xf8f
/// ★ Named from the game: "coin" (MsgGotCoins)
#define FLAG_JAIL_CHEST_COINS 0xf90
/// ★ Named from the game: "Elixir" (MsgItemName)
#define FLAG_JAIL_CHEST_ELIXIR 0xf91
/// Xian: set once the jar's Lucky Medal is taken.
/// ★ Named from the game: "Lucky Medal" (MsgItemName)
#define FLAG_XIAN_LUCKY_MEDAL_JAR 0xf6e

// Xian interiors: set once each find is taken.
/// ★ Named from the game: "Elixir" (MsgItemName)
#define FLAG_XIAN_INTERIORS_ELIXIR_BARREL 0xf6f
/// ★ Named from the game: "Sleep Bomb" (MsgItemName)
#define FLAG_XIAN_INTERIORS_SLEEP_BOMB_JAR 0xf70

// Goma Cave
/// The stump at Goma Cave Entrance has leapt out of the way and lies flat.
/// ★ Named from the game: "You want to move the stump, right?"
/// (MsgGomaNoUsePsynergy)
#define FLAG_GOMA_CAVE_ENTRANCE_STUMP_MOVED 0x860
/// The west stone pillar at Goma Cave Entrance stands one or two columns on
/// from where it starts.
/// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
#define FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP 0x861
/// As FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_ONE_STEP, two columns on.
/// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
#define FLAG_GOMA_CAVE_ENTRANCE_WEST_PILLAR_TWO_STEPS 0x862
/// The east stone pillar has settled into the floor on its last column.
/// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
#define FLAG_GOMA_CAVE_ENTRANCE_EAST_PILLAR_IN_PLACE 0x863
/// The stump in Goma Cave (SceneId_GomaCaveStream) has leapt and lies flat
/// beside the stream.
/// ★ Named from what the player sees: the stump lying flat on the bank of the
/// cave's stream once it has leapt (its render)
#define FLAG_GOMA_CAVE_STREAM_STUMP_MOVED 0x864
/// Ivan has blown the ivy off the stump and joined the party again.
/// ★ Named from the game: "Please, allow me to join your quest."
/// (MsgGomaNoUsePsynergy)
#define FLAG_GOMA_CAVE_IVAN_JOINED 0x865
/// Whirlwind has blown the second ivy at Goma Cave Entrance away.
/// ★ Named from the game: "trapped in the ivy" (MsgGomaNoUsePsynergy)
#define FLAG_GOMA_CAVE_ENTRANCE_IVY_BLOWN_AWAY 0x866
/// Set once the man in Goma Cave who fought Forge has seen it join the party,
/// cheered and walked off for good; until then he stands by the Djinni.
/// ★ Named from the game: "When I returned, I saw it again..."
/// (MsgVaultDidYouSeeSomethingStrangeOver)
#define FLAG_GOMA_CAVE_WITNESS_LEFT 0x869

// Hammet's palace
/// Set once the guards have brought Ivan and the party to Lady Layana and she
/// has sent everyone back to their posts. Until then Hammet Palace's guards
/// turn the party away at the door; Kalay's scenes set it whenever
/// FLAG_DODONPA_LOCKED_UP is set.
/// ★ Named from the game: "I must report this to Lady Layana!"
/// (MsgKalayLordHammetsPalaceLordAway)
#define FLAG_IVAN_REPORTED_TO_LAYANA 0x912
/// Layana has told the party that going into Lunpa is too dangerous.
/// ★ Named from the game: "you sneaked into Lunpa? That's far too dangerous!"
/// (MsgKalayIsaacSneakedIntoLunpaThats)
#define FLAG_LAYANA_SAID_LUNPA_TOO_DANGEROUS 0x913
/// Hammet, back home, has met the party in Layana's room.
/// ★ Named from the game: "his companions... Perfect timing!"
/// (MsgKalayItsIvanHisCompanionsPerfect)
#define FLAG_HAMMET_BACK_HOME_SEEN 0x914
/// The Kalay Tunnel, the passage under the palace to Hammet's treasure, is open
/// to the party: Layana opens it in thanks once Hammet is home, and Kalay's
/// scenes set it whenever FLAG_DODONPA_LOCKED_UP is set. The palace's passage
/// door and the tunnel's way through are drawn open, and its guard turns away.
/// ★ Named from the game: "Layana ordered the door to the underground path
/// opened." (MsgHammetPalaceLayanaOrderedTheDoorToThe)
#define FLAG_KALAY_TUNNEL_OPEN 0x915

// Fuchin Temple and Master Feh's school
/// Nyunpa has died, enlightened, at Fuchin Temple. Mogall Forest sets it once
/// he has taught the party Force and the Killer Ape is beaten; from then on he
/// no longer answers, and the monks grieve.
/// ★ Named from the game: "Master Nyunpa reached enlightenment during his
/// lifetime." (MsgFuchinTempleMasterNyunpaReachedEnlightenmentDuringHis)
#define FLAG_NYUNPA_ENLIGHTENED 0x88f
/// The young man from Kolima at Fuchin Temple asked whether the bridge is open
/// again and the party said yes; from then on he skips the question.
/// ★ Named from the game: "Is the bridge open again?"
/// (MsgXianHowDidGetHereBridge)
#define FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN 0x890
/// Isaac has taken Nyunpa's test of the waterfall grotto, and Nyunpa sends him
/// to the monk beneath the falls, who will let the party in.
/// ★ Named from the game: "Will you take the test?" (MsgXianFollowThemDoNot)
#define FLAG_NYUNPA_TEST_ACCEPTED 0x891
/// The monk beneath the falls at Fuchin Temple has heard of the test, leapt
/// aside and let the party into the waterfall grotto, Fuchin Falls Cave; he
/// stands aside on every visit after.
/// ★ Named from the game: "You will be permitted to enter the grotto."
/// (MsgXianDoNotWorryWillPermitted)
#define FLAG_FUCHIN_FALLS_GROTTO_OPENED 0x892
/// Back from the grotto with the Orb of Force, Isaac has heard Nyunpa name the
/// power Ki, "force" in the western world, which makes the monsters hiding in
/// Mogall Forest show themselves. Nyunpa can die enlightened only after this.
/// ★ Named from the game: "I will teach you, as I promised."
/// (MsgXianExcellentIsaac)
#define FLAG_NYUNPA_TAUGHT_FORCE 0x893
/// Coming out of the grotto with the Orb of Force, Isaac has been asked by the
/// monk beneath the falls whether he passed, and sent on to Nyunpa; until then
/// the way back from the grotto stays closed.
/// ★ Named from the game: "Young master, did you complete the test?"
/// (MsgXianYoungMasterDidCompleteTest)
#define FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST 0x894
/// The Orb of Force's chest in Fuchin Falls Cave has been opened, the end of
/// Nyunpa's test. It brings on the monk's question and Nyunpa's lesson, and
/// ends Garet's warning about falling rocks at Altin Peak.
/// ★ Named from the game: "Orb of Force" (MsgItemName)
#define FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND 0xf14
/// Master Feh, in his school in Xian, has asked to see Isaac's Chi and Isaac
/// agreed; Feh and his students wait for it. Cleared once Feh sees the tree
/// fall from the line (FLAG_FEH_SAW_KI).
/// ★ Named from the game: "Warrior, let me see your Chi now."
/// (MsgFuchinTempleWarriorLetMeSeeYourChi)
#define FLAG_FEH_WAITS_TO_SEE_CHI 0x898
/// Isaac has twice refused to show Master Feh his Chi. Feh then asks again
/// whenever he is spoken to, and a yes sets FLAG_FEH_WAITS_TO_SEE_CHI.
/// ★ Named from the game: "Why won't you show me, warrior?"
/// (MsgFuchinTempleIWantToSeeTheChi)
#define FLAG_FEH_CHI_DEMONSTRATION_DECLINED 0x899
/// Master Feh has watched the tree in his school fall to Force from the line
/// and seen that it is not Chi but Fuchin's Ki, which releases the mind's
/// energy as Chi releases the body's. His students' lines and his place in the
/// school follow it.
/// ★ Named from the game: "Fuchin's Ki releases the mind's energy"
/// (MsgXianYaTreeFell)
#define FLAG_FEH_SAW_KI 0x89b

// Crossbone Isle
/// The four blue doors in a row, then the red door: each is set once a Blue
/// Key has opened it, four flags from this one.
/// ★ Named from the game: "Blue Key" (MsgItemName)
#define FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN 0x9c0
/// The red door beyond them, once the Red Key has opened it.
/// ★ Named from the game: "Red Key" (MsgItemName)
#define FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN 0x9c4
/// Bane's three escapes in CrossboneIsleBane: Bane has hopped away the first
/// time the party came near.
/// ★ Named from the game: "Bane" (MsgDjinnName)
#define FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE 0x9c8
/// Bane has dashed across the platforms and crashed into the far wall.
/// ★ Named from the game: "Bane" (MsgDjinnName)
#define FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE 0x9c9
/// Bane has hopped down to the room's last corner.
/// ★ Named from the game: "Bane" (MsgDjinnName)
#define FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE 0x9ca
/// The ivy on Crossbone Isle itself (CrossboneIsle): four flags from this
/// one.
/// ★ Named from the game: "trapped in the ivy" (MsgGomaNoUsePsynergy)
#define FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY 0x8c4
/// The nine floors of Crossbone Isle Cave (CrossboneIsleFirstFloor to
/// CrossboneIsleNinthFloor): one flag each from these, in room order, for the
/// monsters at the east door beaten and for the pillar sunk into the floor.
/// ★ Named from what the player sees: a Hobgoblin and two Viragos attack as
/// the party walks into the first floor's east door (its render)
#define FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED 0x8c8
/// The first floor's stone pillar has sunk into the floor, opening the way
/// on; one flag a floor from this one.
/// ★ Named from what the player sees: the pillar between the doors sinks into
/// the floor in a ring of dust once Move brings it to its place (its render)
#define FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED 0x8d2
/// Deadbeard has been beaten on the Pirate Ship.
/// ★ Named from the game: "Deadbeard" (MsgEnemyName)
#define FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED 0x8d1
/// The Nut circling over the island has been caught.
/// ★ Named from the game: "Nut" (MsgItemName)
#define FLAG_CROSSBONE_ISLE_NUT_CAUGHT 0xfd7
/// CrossboneIsleBane's chest, carried on its second pillar: a Demon Axe.
/// ★ Named from the game: "Demon Axe" (MsgItemName)
#define FLAG_CROSSBONE_ISLE_DEMON_AXE 0xed9
/// The mimic in CrossboneIsleFirstFloor's chest is beaten.
/// ★ Named from the game: "The treasure chest was really a Mimic!"
/// (MsgChestWasMimic)
#define FLAG_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC_BEATEN 0xef4
/// The chest that appears after Deadbeard is beaten.
/// ★ Named from the game: "Demon Mail" (MsgItemName)
#define FLAG_PIRATE_SHIP_DEMON_MAIL 0xee6
/// The other chest's mimic is beaten.
/// ★ Named from the game: "The treasure chest was really a Mimic!"
/// (MsgChestWasMimic)
#define FLAG_PIRATE_SHIP_MIMIC_BEATEN 0xef7
// CrossboneIsleStatues keeps FLAG_SCENE_TEMP_20 set while one of its
// pursuers is after the leader.

// Sol Sanctum
/// Kraden has found the heart of Sol Sanctum, the mark of the sun on the floor
/// of the Luna and Sol rooms, and talked the others into going a little
/// farther; Retreat is blocked from then on.
/// ★ Named from the game: "This is it! We've found it!" (MsgSolSanctumFound)
#define FLAG_SOL_SANCTUM_FOUND 0x809
/// Kraden has found the picture of Luna, wondered how the rooms of Luna and Sol
/// are connected and asked Isaac to look farther up the passage; he waits by
/// the picture from then on.
/// ★ Named from the game: "Luna and Sol." (MsgSolSanctumLunaSolRooms)
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN 0x80a
/// The northwest statue in the statue room stands on its seal, and its quarter
/// of the pictures in the Luna and Sol rooms is lit; the first of four flags.
/// Springing the trap clears all four.
/// ★ Named from what the player sees: the statue pushed onto its square
/// shines a beam of light across the room (its render)
#define FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN 0x80b
/// As FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, for the northeast statue.
/// ★ Named from what the player sees: the statue pushed onto its square
/// shines a beam of light across the room (its render)
#define FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN 0x80c
/// As FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, for the southwest statue.
/// ★ Named from what the player sees: the statue pushed onto its square
/// shines a beam of light across the room (its render)
#define FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN 0x80d
/// As FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, for the southeast statue.
/// ★ Named from what the player sees: the statue pushed onto its square
/// shines a beam of light across the room (its render)
#define FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN 0x80e
/// All four statues stand on their seals.
/// ★ Named from what the player sees: all four quarters of the pictures in
/// the Luna and Sol rooms lit (its render)
#define FLAG_SOL_SANCTUM_ALL_SEALS_OPEN 0x80f
/// With all four seals open, Kraden has seen the picture of Luna change to Sol
/// and guessed that something else must have changed too.
/// ★ Named from the game: "The picture of Luna changed to Sol!"
/// (MsgSolSanctumThePictureOfLunaChanged)
#define FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED 0x810
/// Kraden has seen the symbol on the floor change from Sol to Luna; searching
/// it then opens the way up to the Elemental Star Room.
/// ★ Named from the game: "Look! The symbol on the floor!"
/// (MsgSolSanctumLookSymbolFloor)
#define FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN 0x811
/// The trap has sprung: the first statue set before the trap was disarmed
/// flashed its light, lights fell through the Luna room, Kraden cried out, and
/// every seal closed again. After that a seal opened too soon closes at once.
/// ★ Named from what the player sees: lights falling through the Luna room
/// around Kraden as the trap springs (its render)
#define FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN 0x812
/// The statues' lights have flashed as Kraden stood among them, after the
/// trap was disarmed, and he has run off to see.
/// ★ Named from the game: "must have found something!"
/// (MsgSolSanctumWhatsHappeningAtTheTrap)
#define FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING 0x813
/// The volcano has erupted, as the Wise One sends Isaac and Garet out of the
/// collapsing Elemental Star Room. Mt. Aleph is the volcano: from then on
/// every room of Sol Sanctum rumbles and shakes, and the field music changes.
/// ★ Named from the game: "It's erupting..." (MsgSolSanctumReturnStarToBag)
#define FLAG_SOL_SANCTUM_ERUPTED 0x814
/// The statues beside the hall's centre shine their beams.
/// ★ Named from the game: "two beams of light shot out from them"
/// (MsgSolSanctumKradenYouFoundIt)
#define FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING 0x816
/// As FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING, for the right statue.
/// ★ Named from the game: "two beams of light shot out from them"
/// (MsgSolSanctumKradenYouFoundIt)
#define FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING 0x817
/// With both beams shining, the center statue has slid into the hole that
/// opened before it, disarming the trap; from then on the seals stay open.
/// ★ Named from the game: "And dropped the statue into it after that."
/// (MsgSolSanctumKradenYouFoundIt)
#define FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED 0x818
/// The floor at the symbol in the Luna and Sol rooms has opened, and Isaac,
/// then Kraden, have risen through it to the Elemental Star Room.
/// ★ Named from what the player sees: the floor at the symbol opening on the
/// way up to the Elemental Star Room (its render)
#define FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN 0x819
/// Kraden has praised Isaac after the seals' lights flashed.
/// ★ Named from the game: "You found it" (MsgSolSanctumYouFoundIt)
#define FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT 0x822
/// Kraden has seen the statues out of his reach behind the blocks.
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_MORE_STATUES_SEEN 0x825
/// The light each seal shows in the Luna and Sol rooms, lit or dark.
/// ★ Named from what the player sees: a quarter of each picture in the Luna
/// and Sol rooms lit for each statue in place (its render)
#define FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN 0x826
/// As FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, for the northeast statue.
/// ★ Named from what the player sees: a quarter of each picture in the Luna
/// and Sol rooms lit for each statue in place (its render)
#define FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN 0x827
/// As FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, for the southwest statue.
/// ★ Named from what the player sees: a quarter of each picture in the Luna
/// and Sol rooms lit for each statue in place (its render)
#define FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN 0x828
/// As FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, for the southeast statue.
/// ★ Named from what the player sees: a quarter of each picture in the Luna
/// and Sol rooms lit for each statue in place (its render)
#define FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN 0x829
/// On the first arrival in the Elemental Star Room, Kraden has explained the
/// four elements and their stars and given out the Mythril Bags.
/// ★ Named from the game: "Legend holds that the four Elemental Stars contain
/// the purified essence of each element."
/// (MsgSolSanctumKradenTheElementsAreThe)
#define FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED 0x83b
/// Isaac has put the Venus Star in a Mythril Bag, opening new crossings between
/// the pedestals.
/// ★ Named from the game: "put the Venus Star in a Mythril Bag."
/// (MsgSolSanctumVenusStarBagged)
#define FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED 0x83c
/// As FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED, for the Mercury Star.
/// ★ Named from the game: "put the Mercury Star in a Mythril Bag."
/// (MsgSolSanctumPutMercuryStar)
#define FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED 0x83d
/// The Jupiter Star is bagged, Saturos and Menardi hold Jenna and Kraden, and
/// Garet has handed Alex three Elemental Stars; Alex demands the Mars Star too.
/// The captors stand round their hostages on every visit after.
/// ★ Named from the game: "handed over the Elemental Stars."
/// (MsgSolSanctumGuessTakeElemental)
#define FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX 0x83e
/// With the Mars Star taken, the Elemental Star Room has collapsed and the Wise
/// One has sent Isaac and Garet out; set with FLAG_SOL_SANCTUM_ERUPTED, it ends
/// the search of the Mars Star's pedestal.
/// ★ Named from the game: "this chamber is collapsing."
/// (MsgSolSanctumReturnStarToBag)
#define FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED 0x83f
/// Isaac agreed to look farther up the passage while Kraden studies the rooms;
/// spoken to, Kraden then asks to hear what he finds.
/// ★ Named from the game: "would you look farther up the passage?"
/// (MsgSolSanctumMeanLookFarther)
#define FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN 0x896
/// The chests: the Small Jewel for each minotaur relief, and a Herb.
/// ★ Named from the game: "Small Jewel" (MsgItemName)
#define FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND 0xf01
/// The chest in Sol Sanctum's maze room holds the second Small Jewel, which
/// fills the second minotaur relief's empty eye.
/// ★ Named from the game: "Small Jewel" (MsgItemName)
#define FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND 0xf02
/// The Luna and Sol rooms' chest.
/// ★ Named from the game: "Herb" (MsgItemName)
#define FLAG_SOL_SANCTUM_HERB_FOUND 0xf40

// Sanctum
/// After the eruption, Isaac has accepted the Great Healer's request in Vale's
/// Sanctum to recover the Elemental Stars, and the Wise One's guidance sends
/// him and Garet off the next day. Vale's field music changes with it.
/// ★ Named from the game: "will you accept responsibility for the fate of the
/// land?" (MsgSanctumIsaacWillAcceptResponsibilityFor)
#define FLAG_SANCTUM_QUEST_ACCEPTED 0x9f0

// Venus Lighthouse
/// The wounded soldier at Venus Lighthouse Entry has told of the attack,
/// flickered and vanished; he is gone on every visit after.
/// ★ Named from the game: "attacked by strange... wanderers..."
/// (MsgFieldVenusLighthouseWasAttackedBy)
#define FLAG_VENUS_LIGHTHOUSE_ATTACK_TOLD 0x981
/// The statue on Venus Lighthouse's ground floor has opened the first of its
/// two paths. One of this and FLAG_VENUS_LIGHTHOUSE_SECOND_PATH is set once the
/// statue has spoken, and each search switches them; the Tunnel Ruins under
/// Babi Lighthouse redraw their rooms to match.
/// ★ Named from the game: "The path long closed"
/// (MsgVenusStatueSpeaksIsaacSoulYe)
#define FLAG_VENUS_LIGHTHOUSE_FIRST_PATH 0x982
/// As FLAG_VENUS_LIGHTHOUSE_FIRST_PATH, the statue's other path.
/// ★ Named from the game: "The path long closed"
/// (MsgVenusStatueSpeaksIsaacSoulYe)
#define FLAG_VENUS_LIGHTHOUSE_SECOND_PATH 0x983
/// Move has pushed the pillar on Venus Lighthouse's ground floor up to the
/// wall, and Garet has said they can push it no farther but might carry it.
/// ★ Named from what the player sees: a grey pillar carved with spirals,
/// pushed up against the wall (its render)
#define FLAG_VENUS_LIGHTHOUSE_PILLAR_AT_WALL 0x986
/// Carry has set that pillar down beyond the wall, where it stays.
/// ★ Named from what the player sees: the grey pillar carved with spirals,
/// set down on the floor beyond the wall (its render)
#define FLAG_VENUS_LIGHTHOUSE_PILLAR_CARRIED 0x987
/// A chest on the first floor.
/// ★ Named from the game: "Thunder Crown" (MsgItemName)
#define FLAG_VENUS_LIGHTHOUSE_THUNDER_CROWN 0xf34
/// A chest on the third floor.
/// ★ Named from the game: "Dragon Scales" (MsgItemName)
#define FLAG_VENUS_LIGHTHOUSE_DRAGON_SCALES 0xf35
/// A chest on the fourth floor.
/// ★ Named from the game: "Gaia Blade" (MsgItemName)
#define FLAG_VENUS_LIGHTHOUSE_GAIA_BLADE 0xf36
/// A chest on the ground floor.
/// ★ Named from the game: "Lucky Cap" (MsgItemName)
#define FLAG_VENUS_LIGHTHOUSE_LUCKY_CAP 0xf37
/// A chest on the ground floor.
/// ★ Named from the game: "Carry Stone" (MsgItemName)
#define FLAG_VENUS_LIGHTHOUSE_CARRY_STONE 0xf38

// World map
/// The Black Orb did nothing on Babi's ship and was handed back.
/// ★ Named from the game: "You returned the Black Orb." (MsgWorldMapMatter)
#define FLAG_BLACK_ORB_RETURNED 0x85d
/// Buried, found by searching.
/// ★ Named from the game: "Turtle Boots" (MsgItemName)
#define FLAG_WORLD_MAP_TURTLE_BOOTS 0xfcf

// Scene flags, FLAG_SCENE_TEMP_00 to FLAG_SCENE_TEMP_FF:
// Scene_ResetFlagsOnEnter clears them on every ordinary entrance, and each
// scene keeps its own puzzle and event states in them. A scene says beside its
// uses what each one holds there.
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

// Place flags, FLAG_PLACE_TEMP_00 to FLAG_PLACE_TEMP_FF:
// Scene_ResetFlagsOnEnter clears them whenever the party enters a different
// place (a new scene group), so they keep what should last while the party is
// anywhere inside one place, such as where Fuchin Falls Cave's logs float. A
// scene says beside its uses what each one holds there.
#define FLAG_PLACE_TEMP_00 0x300
#define FLAG_PLACE_TEMP_01 0x301
#define FLAG_PLACE_TEMP_02 0x302
#define FLAG_PLACE_TEMP_03 0x303
#define FLAG_PLACE_TEMP_04 0x304
#define FLAG_PLACE_TEMP_05 0x305
#define FLAG_PLACE_TEMP_06 0x306
#define FLAG_PLACE_TEMP_07 0x307
#define FLAG_PLACE_TEMP_08 0x308
#define FLAG_PLACE_TEMP_09 0x309
#define FLAG_PLACE_TEMP_0A 0x30a
#define FLAG_PLACE_TEMP_0B 0x30b
#define FLAG_PLACE_TEMP_0C 0x30c
#define FLAG_PLACE_TEMP_0D 0x30d
#define FLAG_PLACE_TEMP_0E 0x30e
#define FLAG_PLACE_TEMP_0F 0x30f
#define FLAG_PLACE_TEMP_10 0x310
#define FLAG_PLACE_TEMP_11 0x311
#define FLAG_PLACE_TEMP_12 0x312
#define FLAG_PLACE_TEMP_13 0x313
#define FLAG_PLACE_TEMP_14 0x314
#define FLAG_PLACE_TEMP_15 0x315
#define FLAG_PLACE_TEMP_16 0x316
#define FLAG_PLACE_TEMP_17 0x317
#define FLAG_PLACE_TEMP_18 0x318
#define FLAG_PLACE_TEMP_19 0x319
#define FLAG_PLACE_TEMP_1A 0x31a
#define FLAG_PLACE_TEMP_1B 0x31b
#define FLAG_PLACE_TEMP_1C 0x31c
#define FLAG_PLACE_TEMP_1D 0x31d
#define FLAG_PLACE_TEMP_1E 0x31e
#define FLAG_PLACE_TEMP_1F 0x31f
#define FLAG_PLACE_TEMP_20 0x320
#define FLAG_PLACE_TEMP_21 0x321
#define FLAG_PLACE_TEMP_22 0x322
#define FLAG_PLACE_TEMP_23 0x323
#define FLAG_PLACE_TEMP_24 0x324
#define FLAG_PLACE_TEMP_25 0x325
#define FLAG_PLACE_TEMP_26 0x326
#define FLAG_PLACE_TEMP_27 0x327
#define FLAG_PLACE_TEMP_28 0x328
#define FLAG_PLACE_TEMP_29 0x329
#define FLAG_PLACE_TEMP_2A 0x32a
#define FLAG_PLACE_TEMP_2B 0x32b
#define FLAG_PLACE_TEMP_2C 0x32c
#define FLAG_PLACE_TEMP_2D 0x32d
#define FLAG_PLACE_TEMP_2E 0x32e
#define FLAG_PLACE_TEMP_2F 0x32f
#define FLAG_PLACE_TEMP_30 0x330
#define FLAG_PLACE_TEMP_31 0x331
#define FLAG_PLACE_TEMP_32 0x332
#define FLAG_PLACE_TEMP_33 0x333
#define FLAG_PLACE_TEMP_34 0x334
#define FLAG_PLACE_TEMP_35 0x335
#define FLAG_PLACE_TEMP_36 0x336
#define FLAG_PLACE_TEMP_37 0x337
#define FLAG_PLACE_TEMP_38 0x338
#define FLAG_PLACE_TEMP_39 0x339
#define FLAG_PLACE_TEMP_3A 0x33a
#define FLAG_PLACE_TEMP_3B 0x33b
#define FLAG_PLACE_TEMP_3C 0x33c
#define FLAG_PLACE_TEMP_3D 0x33d
#define FLAG_PLACE_TEMP_3E 0x33e
#define FLAG_PLACE_TEMP_3F 0x33f
#define FLAG_PLACE_TEMP_40 0x340
#define FLAG_PLACE_TEMP_41 0x341
#define FLAG_PLACE_TEMP_42 0x342
#define FLAG_PLACE_TEMP_43 0x343
#define FLAG_PLACE_TEMP_44 0x344
#define FLAG_PLACE_TEMP_45 0x345
#define FLAG_PLACE_TEMP_46 0x346
#define FLAG_PLACE_TEMP_47 0x347
#define FLAG_PLACE_TEMP_48 0x348
#define FLAG_PLACE_TEMP_49 0x349
#define FLAG_PLACE_TEMP_4A 0x34a
#define FLAG_PLACE_TEMP_4B 0x34b
#define FLAG_PLACE_TEMP_4C 0x34c
#define FLAG_PLACE_TEMP_4D 0x34d
#define FLAG_PLACE_TEMP_4E 0x34e
#define FLAG_PLACE_TEMP_4F 0x34f
#define FLAG_PLACE_TEMP_50 0x350
#define FLAG_PLACE_TEMP_51 0x351
#define FLAG_PLACE_TEMP_52 0x352
#define FLAG_PLACE_TEMP_53 0x353
#define FLAG_PLACE_TEMP_54 0x354
#define FLAG_PLACE_TEMP_55 0x355
#define FLAG_PLACE_TEMP_56 0x356
#define FLAG_PLACE_TEMP_57 0x357
#define FLAG_PLACE_TEMP_58 0x358
#define FLAG_PLACE_TEMP_59 0x359
#define FLAG_PLACE_TEMP_5A 0x35a
#define FLAG_PLACE_TEMP_5B 0x35b
#define FLAG_PLACE_TEMP_5C 0x35c
#define FLAG_PLACE_TEMP_5D 0x35d
#define FLAG_PLACE_TEMP_5E 0x35e
#define FLAG_PLACE_TEMP_5F 0x35f
#define FLAG_PLACE_TEMP_60 0x360
#define FLAG_PLACE_TEMP_61 0x361
#define FLAG_PLACE_TEMP_62 0x362
#define FLAG_PLACE_TEMP_63 0x363
#define FLAG_PLACE_TEMP_64 0x364
#define FLAG_PLACE_TEMP_65 0x365
#define FLAG_PLACE_TEMP_66 0x366
#define FLAG_PLACE_TEMP_67 0x367
#define FLAG_PLACE_TEMP_68 0x368
#define FLAG_PLACE_TEMP_69 0x369
#define FLAG_PLACE_TEMP_6A 0x36a
#define FLAG_PLACE_TEMP_6B 0x36b
#define FLAG_PLACE_TEMP_6C 0x36c
#define FLAG_PLACE_TEMP_6D 0x36d
#define FLAG_PLACE_TEMP_6E 0x36e
#define FLAG_PLACE_TEMP_6F 0x36f
#define FLAG_PLACE_TEMP_70 0x370
#define FLAG_PLACE_TEMP_71 0x371
#define FLAG_PLACE_TEMP_72 0x372
#define FLAG_PLACE_TEMP_73 0x373
#define FLAG_PLACE_TEMP_74 0x374
#define FLAG_PLACE_TEMP_75 0x375
#define FLAG_PLACE_TEMP_76 0x376
#define FLAG_PLACE_TEMP_77 0x377
#define FLAG_PLACE_TEMP_78 0x378
#define FLAG_PLACE_TEMP_79 0x379
#define FLAG_PLACE_TEMP_7A 0x37a
#define FLAG_PLACE_TEMP_7B 0x37b
#define FLAG_PLACE_TEMP_7C 0x37c
#define FLAG_PLACE_TEMP_7D 0x37d
#define FLAG_PLACE_TEMP_7E 0x37e
#define FLAG_PLACE_TEMP_7F 0x37f
#define FLAG_PLACE_TEMP_80 0x380
#define FLAG_PLACE_TEMP_81 0x381
#define FLAG_PLACE_TEMP_82 0x382
#define FLAG_PLACE_TEMP_83 0x383
#define FLAG_PLACE_TEMP_84 0x384
#define FLAG_PLACE_TEMP_85 0x385
#define FLAG_PLACE_TEMP_86 0x386
#define FLAG_PLACE_TEMP_87 0x387
#define FLAG_PLACE_TEMP_88 0x388
#define FLAG_PLACE_TEMP_89 0x389
#define FLAG_PLACE_TEMP_8A 0x38a
#define FLAG_PLACE_TEMP_8B 0x38b
#define FLAG_PLACE_TEMP_8C 0x38c
#define FLAG_PLACE_TEMP_8D 0x38d
#define FLAG_PLACE_TEMP_8E 0x38e
#define FLAG_PLACE_TEMP_8F 0x38f
#define FLAG_PLACE_TEMP_90 0x390
#define FLAG_PLACE_TEMP_91 0x391
#define FLAG_PLACE_TEMP_92 0x392
#define FLAG_PLACE_TEMP_93 0x393
#define FLAG_PLACE_TEMP_94 0x394
#define FLAG_PLACE_TEMP_95 0x395
#define FLAG_PLACE_TEMP_96 0x396
#define FLAG_PLACE_TEMP_97 0x397
#define FLAG_PLACE_TEMP_98 0x398
#define FLAG_PLACE_TEMP_99 0x399
#define FLAG_PLACE_TEMP_9A 0x39a
#define FLAG_PLACE_TEMP_9B 0x39b
#define FLAG_PLACE_TEMP_9C 0x39c
#define FLAG_PLACE_TEMP_9D 0x39d
#define FLAG_PLACE_TEMP_9E 0x39e
#define FLAG_PLACE_TEMP_9F 0x39f
#define FLAG_PLACE_TEMP_A0 0x3a0
#define FLAG_PLACE_TEMP_A1 0x3a1
#define FLAG_PLACE_TEMP_A2 0x3a2
#define FLAG_PLACE_TEMP_A3 0x3a3
#define FLAG_PLACE_TEMP_A4 0x3a4
#define FLAG_PLACE_TEMP_A5 0x3a5
#define FLAG_PLACE_TEMP_A6 0x3a6
#define FLAG_PLACE_TEMP_A7 0x3a7
#define FLAG_PLACE_TEMP_A8 0x3a8
#define FLAG_PLACE_TEMP_A9 0x3a9
#define FLAG_PLACE_TEMP_AA 0x3aa
#define FLAG_PLACE_TEMP_AB 0x3ab
#define FLAG_PLACE_TEMP_AC 0x3ac
#define FLAG_PLACE_TEMP_AD 0x3ad
#define FLAG_PLACE_TEMP_AE 0x3ae
#define FLAG_PLACE_TEMP_AF 0x3af
#define FLAG_PLACE_TEMP_B0 0x3b0
#define FLAG_PLACE_TEMP_B1 0x3b1
#define FLAG_PLACE_TEMP_B2 0x3b2
#define FLAG_PLACE_TEMP_B3 0x3b3
#define FLAG_PLACE_TEMP_B4 0x3b4
#define FLAG_PLACE_TEMP_B5 0x3b5
#define FLAG_PLACE_TEMP_B6 0x3b6
#define FLAG_PLACE_TEMP_B7 0x3b7
#define FLAG_PLACE_TEMP_B8 0x3b8
#define FLAG_PLACE_TEMP_B9 0x3b9
#define FLAG_PLACE_TEMP_BA 0x3ba
#define FLAG_PLACE_TEMP_BB 0x3bb
#define FLAG_PLACE_TEMP_BC 0x3bc
#define FLAG_PLACE_TEMP_BD 0x3bd
#define FLAG_PLACE_TEMP_BE 0x3be
#define FLAG_PLACE_TEMP_BF 0x3bf
#define FLAG_PLACE_TEMP_C0 0x3c0
#define FLAG_PLACE_TEMP_C1 0x3c1
#define FLAG_PLACE_TEMP_C2 0x3c2
#define FLAG_PLACE_TEMP_C3 0x3c3
#define FLAG_PLACE_TEMP_C4 0x3c4
#define FLAG_PLACE_TEMP_C5 0x3c5
#define FLAG_PLACE_TEMP_C6 0x3c6
#define FLAG_PLACE_TEMP_C7 0x3c7
#define FLAG_PLACE_TEMP_C8 0x3c8
#define FLAG_PLACE_TEMP_C9 0x3c9
#define FLAG_PLACE_TEMP_CA 0x3ca
#define FLAG_PLACE_TEMP_CB 0x3cb
#define FLAG_PLACE_TEMP_CC 0x3cc
#define FLAG_PLACE_TEMP_CD 0x3cd
#define FLAG_PLACE_TEMP_CE 0x3ce
#define FLAG_PLACE_TEMP_CF 0x3cf
#define FLAG_PLACE_TEMP_D0 0x3d0
#define FLAG_PLACE_TEMP_D1 0x3d1
#define FLAG_PLACE_TEMP_D2 0x3d2
#define FLAG_PLACE_TEMP_D3 0x3d3
#define FLAG_PLACE_TEMP_D4 0x3d4
#define FLAG_PLACE_TEMP_D5 0x3d5
#define FLAG_PLACE_TEMP_D6 0x3d6
#define FLAG_PLACE_TEMP_D7 0x3d7
#define FLAG_PLACE_TEMP_D8 0x3d8
#define FLAG_PLACE_TEMP_D9 0x3d9
#define FLAG_PLACE_TEMP_DA 0x3da
#define FLAG_PLACE_TEMP_DB 0x3db
#define FLAG_PLACE_TEMP_DC 0x3dc
#define FLAG_PLACE_TEMP_DD 0x3dd
#define FLAG_PLACE_TEMP_DE 0x3de
#define FLAG_PLACE_TEMP_DF 0x3df
#define FLAG_PLACE_TEMP_E0 0x3e0
#define FLAG_PLACE_TEMP_E1 0x3e1
#define FLAG_PLACE_TEMP_E2 0x3e2
#define FLAG_PLACE_TEMP_E3 0x3e3
#define FLAG_PLACE_TEMP_E4 0x3e4
#define FLAG_PLACE_TEMP_E5 0x3e5
#define FLAG_PLACE_TEMP_E6 0x3e6
#define FLAG_PLACE_TEMP_E7 0x3e7
#define FLAG_PLACE_TEMP_E8 0x3e8
#define FLAG_PLACE_TEMP_E9 0x3e9
#define FLAG_PLACE_TEMP_EA 0x3ea
#define FLAG_PLACE_TEMP_EB 0x3eb
#define FLAG_PLACE_TEMP_EC 0x3ec
#define FLAG_PLACE_TEMP_ED 0x3ed
#define FLAG_PLACE_TEMP_EE 0x3ee
#define FLAG_PLACE_TEMP_EF 0x3ef
#define FLAG_PLACE_TEMP_F0 0x3f0
#define FLAG_PLACE_TEMP_F1 0x3f1
#define FLAG_PLACE_TEMP_F2 0x3f2
#define FLAG_PLACE_TEMP_F3 0x3f3
#define FLAG_PLACE_TEMP_F4 0x3f4
#define FLAG_PLACE_TEMP_F5 0x3f5
#define FLAG_PLACE_TEMP_F6 0x3f6
#define FLAG_PLACE_TEMP_F7 0x3f7
#define FLAG_PLACE_TEMP_F8 0x3f8
#define FLAG_PLACE_TEMP_F9 0x3f9
#define FLAG_PLACE_TEMP_FA 0x3fa
#define FLAG_PLACE_TEMP_FB 0x3fb
#define FLAG_PLACE_TEMP_FC 0x3fc
#define FLAG_PLACE_TEMP_FD 0x3fd
#define FLAG_PLACE_TEMP_FE 0x3fe
#define FLAG_PLACE_TEMP_FF 0x3ff

#endif
