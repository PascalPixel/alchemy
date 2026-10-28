#ifndef ALCHEMY_SCENE_IDS_H
#define ALCHEMY_SCENE_IDS_H

/// @file
/// Scene numbers, named after their places, in the order of gScenes
/// (field/scenes.s).
///
/// Each SceneId_ is an absolute symbol whose value is the scene's row in the
/// table; C takes the number as (s32)&SceneId_Vale, and the exit tables name a
/// scene by its place (SCENE_EXIT).

/// ★ Named from the game: "Title screen" (MsgDebugAreaName)
extern char SceneId_Title;
/// The File Screen, where the player chooses a file; the debug map list calls
/// it "(Clear data)".
/// ★ Named from what the player sees: the Golden Sun logo over the start menu
/// of New Game, Continue, Copy and Erase (its render)
extern char SceneId_FileScreen;
/// ★ Named from its area name: World Map
extern char SceneId_WorldMap;
/// The World Map's map, row 1 of gMaps (field/scenes.s names it with
/// the World Map's scene row), which the field compares
/// gSequenceParentWork.mapId with.
extern char MapIndex_World;
#define MAP_INDEX_WORLD ((s32)&MapIndex_World)
/// Vale on the stormy night of the prologue, which Camelot labels PLOGV00.
/// ★ Named from the game: "PLOGV00" (MsgDebugSceneCode)
extern char SceneId_ValePrologue;
/// ★ Named from its area name: Vale
extern char SceneId_Vale;
/// ★ Named from the game: "Why's everyone hanging around the town gate?"
/// (MsgValeWhysEveryoneHanging)
extern char SceneId_ValePlaza;
/// ★ Named from the game: "You mustn't go beyond Kraden's cottage."
/// (MsgValeDontGoBeyondKradensCottage)
extern char SceneId_ValeKradensCottage;
/// A sheet of rooms drawn as cottages, all of them Vale's.
/// ★ Named from what the player sees: rooms drawn as cottages, the interiors of
/// Vale's houses and its shops, Garet's house and Jenna's grandparents' house
/// among them (its render)
extern char SceneId_CottageRoomsA;
/// A sheet of rooms drawn as cottages, of more than one place.
/// ★ Named from what the player sees: rooms drawn as cottages, Vale's inn,
/// Isaac's house, Kraden's study and the forge, with Babi's scholars' room (its
/// render)
extern char SceneId_CottageRoomsB;
/// ★ Named from its area name: Sanctum
extern char SceneId_Sanctum;
/// ★ Named from the game: "in this eruption?!?" (MsgValeMtAlephWasInactive)
extern char SceneId_ValeEruption;
/// ★ Named from the game: "Luna and Sol" (MsgSolSanctumLunaSolRooms)
extern char SceneId_SolSanctumLunaSolRooms;
/// ★ Named from the game: "So the statue puzzle was the trap..."
/// (MsgSolSanctumKradenYouFoundIt)
extern char SceneId_SolSanctumStatue;
/// ★ Named from the game: "a Psynergy Stone." (MsgGotPsynergyStone)
extern char SceneId_SolSanctumPsynergyStone;
/// ★ Named from what the player sees: a maze of narrow torchlit passages, one
/// of them ending at the chest with the second Small Jewel (its render)
extern char SceneId_SolSanctumMaze;
/// ★ Named from what the player sees: one long bending passage of blue
/// flagstones between two doors of the sanctum's rooms (its render)
extern char SceneId_SolSanctumCorridor;
/// ★ Named from what the player sees: the hall of the two minotaur reliefs that
/// take the Small Jewels (its render)
extern char SceneId_SolSanctumReliefs;
/// ★ Named from the game: "Elemental Star Room" (MsgDebugAreaName)
extern char SceneId_SolSanctumElementalStarRoom;
/// ★ Named from the game: "It's erupting..." (MsgSolSanctumReturnStarToBag)
extern char SceneId_SolSanctumEruption;
/// ★ Named from its area name: Sol Entrance
extern char SceneId_SolEntrance;
/// ★ Named from its area name: Vault
extern char SceneId_Vault;
/// A sheet of rooms drawn in logs, all of them Vault's.
/// ★ Named from what the player sees: rooms drawn in logs, the inside of
/// Vault's houses, shops and inn, with the inn's loft behind a crate (its
/// render)
extern char SceneId_LogRoomsA;
/// A sheet of rooms of more than one place, drawn in stone.
/// ★ Named from what the player sees: rooms drawn in stone, Vault's jail and
/// Lunpa's hidden room of chests (its render)
extern char SceneId_StoneRooms;
/// ★ Named from what the player sees: Vault again once the party has reached
/// Kalay, its thieves broken out of jail (its render)
extern char SceneId_VaultRevisit;
/// A sheet of rooms drawn in logs, all of them Vault's.
/// ★ Named from what the player sees: rooms drawn in logs, the inside of
/// Vault's houses, shops and inn on the later visit, after the thieves escape
/// (its render)
extern char SceneId_LogRoomsB;
/// ★ Named from its area name: Goma Cave Entrance
extern char SceneId_GomaCaveEntrance;
/// ★ Named from what the player sees: a cave room around a pond, below the room
/// the stump is pushed from (its render)
extern char SceneId_GomaCavePond;
/// ★ Named from what the player sees: the cave room of two pillars and a stump
/// that Move pushes (its render)
extern char SceneId_GomaCavePillar;
/// ★ Named from what the player sees: the Goma Cave room where a stream runs
/// its whole length beside the path (its render)
extern char SceneId_GomaCaveStream;
/// ★ Named from what the player sees: Goma Cave's way out, where a wooden
/// bridge crosses the river (its render)
extern char SceneId_GomaCaveBridge;
/// ★ Named from its area name: Bilibin
extern char SceneId_Bilibin;
/// A sheet of rooms drawn as cottages, all of them Bilibin's.
/// ★ Named from what the player sees: rooms drawn as cottages, the inside of
/// Bilibin's shops, inn and houses (its render)
extern char SceneId_CottageRoomsC;
/// ★ Named from its area name: Bilibin Treasury
extern char SceneId_BilibinTreasury;
/// ★ Named from the game: "McCoy's Palace" (MsgDebugAreaName)
extern char SceneId_McCoyPalace;
/// ★ Named from the game: "planning the new palace."
/// (MsgBilibinIShouldntHaveSaidAnythingWhen)
extern char SceneId_McCoyPalaceGrounds;
/// ★ Named from its area name: Bilibin Barricade
extern char SceneId_BilibinBarricade;
/// ★ Named from its area name: Kolima
extern char SceneId_Kolima;
/// A sheet of rooms drawn in logs, all of them Kolima's.
/// ★ Named from what the player sees: rooms drawn in logs, the inside of
/// Kolima's houses, shops and inn, their people turned to trees (its render)
extern char SceneId_LogRoomsC;
/// ★ Named from its area name: Kolima Tunnel
extern char SceneId_KolimaTunnel;
/// ★ Named from its area name: Kolima Bridge
extern char SceneId_KolimaBridge;
/// ★ Named from what the player sees: the way into Kolima Forest from the world
/// scene, a river crossed on log bridges (its render)
extern char SceneId_KolimaForestEntrance;
/// ★ Named from what the player sees: the one forest scene with no water, an
/// open grassy glade ringed by trees (its render)
extern char SceneId_KolimaForestGlade;
/// ★ Named from what the player sees: a cliff ridge runs along the whole north
/// of the scene, a river down its west (its render)
extern char SceneId_KolimaForestCliff;
/// ★ Named from the game: "Floodgate Switch" (MsgKolimaDoNotTouchFloodgate)
extern char SceneId_KolimaForestFloodgate;
/// ★ Named from the game: "The holy tree of Kolima"
/// (MsgKolimaInteriorsTheHolyTreeOfKolimaHas)
extern char SceneId_KolimaForestTree;
/// A sheet of rooms drawn inside a tree.
/// ★ Named from what the player sees: the rooms inside the trunk of Tret Tree
/// (its render)
extern char SceneId_TreeRooms;
/// ★ Named from the game: "Then those gems fell into my branches..."
/// (MsgKolimaWasIndeedAngryPeopleHad)
extern char SceneId_TretTreeBranch;
/// ★ Named from what the player sees: the Bilibin Cave room of ice pillars that
/// Frost, fires and torches freeze and melt (its render)
extern char SceneId_BilibinCaveIce;
/// ★ Named from the game: "Elven Rapier" (MsgItemName)
extern char SceneId_BilibinCaveRapier;
/// ★ Named from its area name: Bilibin Cave Entrance
extern char SceneId_BilibinCaveEntrance;
/// ★ Named from its area name: Imil
extern char SceneId_Imil;
/// A sheet of rooms drawn in ice, all of them Imil's.
/// ★ Named from what the player sees: rooms drawn in ice, the inside of Imil's
/// houses, shops, inn and Sanctum (its render)
extern char SceneId_IceRooms;
/// ★ Named from its area name: Imil Falls Cave
extern char SceneId_ImilFallsCave;
/// ★ Named from what the player sees: the lighthouse's ground floor inside its
/// door, where the Lizard Man blocks Mia (its render)
extern char SceneId_MercuryLighthouseEntrance;
/// ★ Named from what the player sees: the first rooms of the climb above the
/// ground floor, the pool crossed under rings of light and the pipes, where
/// Mia joins (its render)
extern char SceneId_MercuryLighthouseLower;
/// ★ Named from what the player sees: the middle of the climb, with the gate
/// held open by a statue, the pipe maze and Sleet's room (its render)
extern char SceneId_MercuryLighthouseMiddle;
/// ★ Named from what the player sees: the top of the climb inside, with the
/// waterfall halls, the five pillars and the stone doors (its render)
extern char SceneId_MercuryLighthouseUpper;
/// ★ Named from what the player sees: the lighthouse from outside, its foot in
/// the water with the door and fountain, and the walkways up to the waterfall
/// (its render)
extern char SceneId_MercuryLighthouseExterior;
/// ★ Named from its area name: Lighthouse Aerie
extern char SceneId_MercuryLighthouseAerie;
/// ★ Named from what the player sees: the same aerie as the one in use, empty,
/// its platforms not lowered (its render)
extern char SceneId_UnusedMercuryLighthouseAerieCopy;
/// ★ Named from what the player sees: outside the temple, its monks and the
/// stepped waterfall with the grotto beneath (its render)
extern char SceneId_FuchinTempleGrounds;
/// A sheet of rooms drawn in red lacquer, of more than one place.
/// ★ Named from what the player sees: rooms drawn in red lacquer, Nyunpa's hall
/// in Fuchin Temple, Feh's school in Xian and Hama's room in Lama Temple (its
/// render)
extern char SceneId_LacquerRoomsA;
/// ★ Named from what the player sees: the cave's mouth and only way in and out,
/// a log pool and stumps in the water (its render)
extern char SceneId_FuchinFallsCaveEntrance;
/// ★ Named from the game: "Zephyr" (MsgDjinnName)
extern char SceneId_FuchinFallsCaveZephyr;
/// ★ Named from the game: "Dragon's Eye" (MsgItemName)
extern char SceneId_FuchinFallsCaveDragonsEye;
/// ★ Named from the game: "Arctic Blade" (MsgItemName)
extern char SceneId_FuchinFallsCaveArcticBlade;
/// ★ Named from the game: "Orb of Force" (MsgItemName)
extern char SceneId_FuchinFallsCaveOrbOfForce;
/// ★ Named from the game: "It's a statue of an eyeless dragon."
/// (MsgFuchinEyelessDragon)
extern char SceneId_FuchinFallsCaveDragonStatue;
/// ★ Named from the game: "The broken sign reads:" (MsgMogallForestBrokenSign)
extern char SceneId_MogallForestSign;
/// ★ Named from the game: "Quartz" (MsgDjinnName)
extern char SceneId_MogallForestQuartz;
/// ★ Named from the game: "Killer Ape" (MsgEnemyName)
extern char SceneId_MogallForestKillerApe;
/// ★ Named from what the player sees: small identical clearings the wrong turns
/// lead into, whose far sides lead back in (its render)
extern char SceneId_MogallForestSidePaths;
/// ★ Named from its area name: Xian
extern char SceneId_Xian;
/// A sheet of rooms drawn in red lacquer, all of them Xian's.
/// ★ Named from what the player sees: rooms drawn in red lacquer, the inside of
/// Xian's houses, shops and inn, and the room behind Master Feh's school (its
/// render)
extern char SceneId_LacquerRoomsB;
/// ★ Named from its area name: Alpine Crossing
extern char SceneId_AlpineCrossing;
/// ★ Named from its area name: Altin
extern char SceneId_Altin;
/// A sheet of rooms drawn as cottages, all of them Altin's.
/// ★ Named from what the player sees: rooms drawn as cottages, the inside of
/// Altin's houses, shops and inn, where flooded-out families shelter (its
/// render)
extern char SceneId_CottageRoomsD;
/// ★ Named from the game: "The water beasts looked"
/// (MsgAltinTheWaterBeastsLookedExactlyLike)
extern char SceneId_AltinPeakFirstWaterBeast;
/// ★ Named from what the player sees: a winding tunnel with nothing in it but
/// one rail line between fences (its render)
extern char SceneId_AltinPeakRailTunnel;
/// ★ Named from the game: "The water beasts looked"
/// (MsgAltinTheWaterBeastsLookedExactlyLike)
extern char SceneId_AltinPeakSecondWaterBeast;
/// ★ Named from the game: "North: Altin Mine"
/// (MsgMountainNorthAltinMineWestLama)
extern char SceneId_AltinPeakSouthEntrance;
/// ★ Named from the game: "The water beasts looked"
/// (MsgAltinTheWaterBeastsLookedExactlyLike)
extern char SceneId_AltinPeakThirdWaterBeast;
/// ★ Named from what the player sees: a cave filled by a large pool, with rails
/// around it and a doorway above it (its render)
extern char SceneId_AltinPeakPool;
/// ★ Named from the game: "Spritz" (MsgDjinnName)
extern char SceneId_AltinPeakSpritz;
/// ★ Named from what the player sees: two tunnels where five ways meet, to
/// Altin and to three other rooms of the mine (its render)
extern char SceneId_AltinPeakJunction;
/// ★ Named from the game: "Watch for falling rocks!"
/// (MsgAltinWatchFallingRocks)
extern char SceneId_AltinPeakFallingRocks;
/// ★ Named from the game: "Hydros Statue" (MsgEnemyName)
extern char SceneId_AltinPeakHydrosStatue;
/// ★ Named from the game: "find your way west" (MsgAltinTryingFindYourWayWest)
extern char SceneId_AltinPeakWestExit;
/// ★ Named from its area name: Lama Temple
extern char SceneId_LamaTemple;
/// ★ Named from what the player sees: the desert's east end, entered from the
/// world map beside Lama Temple (its render)
extern char SceneId_LamakanDesertEast;
/// ★ Named from the game: "Smog" (MsgDjinnName)
extern char SceneId_LamakanDesertSmog;
/// ★ Named from the game: "Manticore" (MsgEnemyName)
extern char SceneId_LamakanDesertManticore;
/// ★ Named from what the player sees: dark rock tunnels lit by torches at the
/// desert's west end, with no sand (its render)
extern char SceneId_LamakanDesertCave;
/// ★ Named from what the player sees: the clearing at the cave's mouth above
/// Vale (its render)
extern char SceneId_ValeCaveMouth;
/// ★ Named from the game: "Kite" (MsgDjinnName)
extern char SceneId_ValeCaveKite;
/// ★ Named from what the player sees: the cave whose three logs the party
/// pushes (its render)
extern char SceneId_ValeCaveLogs;
/// ★ Named from the game: "a passage leading"
/// (MsgVaultCarefulSearchWillRevealPassage)
extern char SceneId_VaultCavePassage;
/// ★ Named from the game: "Vambrace" (MsgItemName)
extern char SceneId_VaultCaveVambrace;
/// ★ Named from what the player sees: the part of Vault Cave entered from the
/// cave mouth in Vault's northeast cliff (its render)
extern char SceneId_VaultCaveMouth;
/// ★ Named from its area name: Kalay
extern char SceneId_Kalay;
/// A sheet of rooms drawn in stucco, all of them Kalay's.
/// ★ Named from what the player sees: rooms drawn in stucco, Kalay's houses,
/// its weapon and armor shops and its inn with the guest rooms below (its
/// render)
extern char SceneId_StuccoRoomsA;
/// ★ Named from its area name: Kalay Sanctum
extern char SceneId_KalaySanctum;
/// ★ Named from what the player sees: the front of Hammet Palace, where two
/// soldiers guard the door above the stairs from town (its render)
extern char SceneId_HammetPalaceGrounds;
/// ★ Named from its area name: Hammet Palace
extern char SceneId_HammetPalace;
/// ★ Named from its area name: Lunpa
extern char SceneId_Lunpa;
/// A sheet of rooms cut into rock, of more than one place.
/// ★ Named from what the player sees: rooms cut into rock, the inside of
/// Lunpa's homes, shops, inn, crate room and temple, and the house at Suhalla
/// Gate (its render)
extern char SceneId_RockRooms;
/// ★ Named from its area name: West Lunpa Cave
extern char SceneId_WestLunpaCave;
/// ★ Named from its area name: Kalay Docks
extern char SceneId_KalayDocks;
/// A sheet of rooms drawn as cottages, of more than one place.
/// ★ Named from what the player sees: rooms drawn as cottages, the dock
/// buildings' rooms at Kalay Docks and Tolbi Docks, where boarding tickets are
/// sold (its render)
extern char SceneId_CottageRoomsE;
/// ★ Named from what the player sees: the deck of the Tolbi-bound Ship on its
/// voyage across the Karagol Sea (its render)
extern char SceneId_TolbiShipDeck;
/// ★ Named from the game: "You climbed up the mast"
/// (MsgTolbiShipItsMyLuckyAnchor)
extern char SceneId_TolbiShipMast;
/// A sheet of rooms drawn as a ship's cabins.
/// ★ Named from what the player sees: the cabins below the decks of the
/// Tolbi-bound Ship, its holds, rowing deck and captain's quarters (its render)
extern char SceneId_ShipRooms;
/// ★ Named from its area name: Tolbi Docks
extern char SceneId_TolbiDocks;
/// ★ Named from its area name: Crossbone Isle
extern char SceneId_CrossboneIsle;
/// ★ Named from what the player sees: the winding path lit by torches that
/// leads down from the island into the cave (its render)
extern char SceneId_CrossboneIsleCaveEntrance;
/// ★ Named from what the player sees: raised stone posts over a pit, which the
/// party hops across to reach four chests (its render)
extern char SceneId_CrossboneIsleSteppingStones;
/// ★ Named from what the player sees: pillars of ice, some standing and some
/// lying as walkways, across a floor of spikes (its render)
extern char SceneId_CrossboneIsleIcePillars;
/// ★ Named from the game: "Blue Key" (MsgItemName)
extern char SceneId_CrossboneIsleKeys;
/// ★ Named from what the player sees: a dark hall lit by braziers, where five
/// statues of a robed woman step out after the leader (its render)
extern char SceneId_CrossboneIsleStatues;
/// ★ Named from what the player sees: a cave with a stream and a floodgate
/// across it, whose switch floats seven logs (its render)
extern char SceneId_CrossboneIsleFloodgate;
/// ★ Named from the game: "Bane" (MsgDjinnName)
extern char SceneId_CrossboneIsleBane;
/// ★ Named from what the player sees: a cave of cliffs and ladders, where
/// carved stone pillars are lifted onto one another (its render)
extern char SceneId_CrossboneIsleStackedPillars;
/// ★ Named from what the player sees: an icy cave with a coiled, winged dragon
/// statue beside three puddles (its render)
extern char SceneId_CrossboneIsleDragonStatue;
/// ★ Named from what the player sees: two pools of green water, with logs
/// floating on them that carry the leader (its render)
extern char SceneId_CrossboneIsleFloatingLogs;
/// ★ Named from what the player sees: the ledge at the end of the cave, where a
/// wooden pier leads out to the Pirate Ship (its render)
extern char SceneId_CrossboneIslePier;
/// ★ Named from its area name: Pirate Ship
extern char SceneId_PirateShip;
/// ★ Named from what the player sees: the first of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char SceneId_CrossboneIsleFirstFloor;
/// ★ Named from what the player sees: the second of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char SceneId_CrossboneIsleSecondFloor;
/// ★ Named from what the player sees: the third of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char SceneId_CrossboneIsleThirdFloor;
/// ★ Named from what the player sees: the fourth of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char SceneId_CrossboneIsleFourthFloor;
/// ★ Named from what the player sees: the fifth of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char SceneId_CrossboneIsleFifthFloor;
/// ★ Named from what the player sees: the sixth of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char SceneId_CrossboneIsleSixthFloor;
/// ★ Named from what the player sees: the seventh of nine halls that look
/// alike, each a flight of stairs lower than the last (its render)
extern char SceneId_CrossboneIsleSeventhFloor;
/// ★ Named from what the player sees: the eighth of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char SceneId_CrossboneIsleEighthFloor;
/// ★ Named from what the player sees: the last of nine halls that look alike,
/// which leads on to the pier (its render)
extern char SceneId_CrossboneIsleNinthFloor;
/// ★ Named from its area name: Tolbi
extern char SceneId_Tolbi;
/// A sheet of rooms drawn in stucco, all of them Tolbi's.
/// ★ Named from what the player sees: rooms drawn in stucco, the inside of
/// Tolbi's houses, inns and shops, and Tolbi Sanctum (its render)
extern char SceneId_StuccoRoomsB;
/// A sheet of rooms drawn in stucco, both of them Tolbi's.
/// ★ Named from what the player sees: rooms drawn in stucco, the two small
/// rooms in Tolbi where Lucky Dice and Lucky Wheels are played (its render)
extern char SceneId_StuccoRoomsC;
/// ★ Named from its area name: Babi's Palace
extern char SceneId_BabiPalace;
/// ★ Named from what the player sees: the way up from Tolbi to the palace door,
/// where two guards stand (its render)
extern char SceneId_BabiPalaceEntrance;
/// ★ Named from its area name: Colosseum
extern char SceneId_Colosseum;
/// ★ Named from its area name: Colosseum Tunnel
extern char SceneId_ColosseumTunnel;
/// ★ Named from its area name: Colosseum Entrance
extern char SceneId_ColosseumEntrance;
/// ★ Named from the game: "Azart" (MsgEnemyName)
extern char SceneId_ColosseumAzartMatch;
/// ★ Named from the game: "Satrage" (MsgEnemyName)
extern char SceneId_ColosseumSatrageMatch;
/// ★ Named from the game: "Navampa" (MsgEnemyName)
extern char SceneId_ColosseumNavampaMatch;
/// ★ Named from what the player sees: the dark winding passage in from the cave
/// mouth and the small chamber beyond it (its render)
extern char SceneId_AltmillerCaveEntrance;
/// ★ Named from the game: "Babi!" (MsgAltmillerBabi)
extern char SceneId_AltmillerCaveBabi;
/// ★ Named from the game: "Vial" (MsgItemName)
extern char SceneId_AltmillerCaveVialChest;
/// ★ Named from the game: "Squall" (MsgDjinnName)
extern char SceneId_AltmillerCaveSquall;
/// ★ Named from the game: "Cookie" (MsgItemName)
extern char SceneId_AltmillerCaveCookieChest;
/// ★ Named from the game: "Mystic Draught" (MsgItemName)
extern char SceneId_AltmillerCaveDraught;
/// ★ Named from its area name: Gondowan Passage
extern char SceneId_GondowanPassage;
/// ★ Named from its area name: Kalay Tunnel Entrance
extern char SceneId_KalayTunnelEntrance;
/// ★ Named from the game: "the cave up ahead" (MsgKalayDoWantGoCaveUp)
extern char SceneId_KalayTunnelCave;
/// ★ Named from the game: "underground passage"
/// (MsgHammetPalaceILeftTheDoorToThe)
extern char SceneId_KalayTunnelPassage;
/// ★ Named from the game: "Hammet's treasure room"
/// (MsgHammetPalaceTheKeyToTheSecretPassage)
extern char SceneId_KalayTunnelTreasureRoom;
/// ★ Named from what the player sees: the underground river crossed on stepping
/// pads, with two chests, between the cave's two halves (its render)
extern char SceneId_GondowanCaveRiver;
/// ★ Named from its area name: Gondowan Cave
extern char SceneId_GondowanCave;
/// ★ Named from its area name: Lunpa Fortress Gates
extern char SceneId_LunpaFortressGates;
/// ★ Named from what the player sees: the fortress floor inside its door, whose
/// staircases climb to the floor above (its render)
extern char SceneId_LunpaFortressGroundFloor;
/// ★ Named from what the player sees: the floor reached up the fortress's two
/// staircases, where Donpa sleeps (its render)
extern char SceneId_LunpaFortressUpperFloor;
/// ★ Named from the game: "Hammet's down in that treacherous cave."
/// (MsgLunpaHammetInCave)
extern char SceneId_LunpaFortressCave;
/// ★ Named from the game: "Toadonpa" (MsgEnemyName)
extern char SceneId_LunpaFortressToadonpa;
/// ★ Named from what the player sees: the northern half of the desert, entered
/// from the world map just south of Suhalla (its render)
extern char SceneId_SuhallaDesertNorth;
/// ★ Named from what the player sees: the southern half of the desert, whose
/// way out leads on toward Suhalla Gate (its render)
extern char SceneId_SuhallaDesertSouth;
/// ★ Named from what the player sees: two caves under the desert, reached
/// through its two cave mouths (its render)
extern char SceneId_SuhallaDesertCaves;
/// ★ Named from its area name: Suhalla
extern char SceneId_Suhalla;
/// A sheet of rooms drawn in timber, all of them Suhalla's.
/// ★ Named from what the player sees: rooms drawn in timber, the inside of
/// Suhalla's houses, shop and inn, and Suhalla Sanctum (its render)
extern char SceneId_TimberRoomsA;
/// ★ Named from the game: "Only those with travel papers may pass through."
/// (MsgSuhallaTravelPapersRequired)
extern char SceneId_SuhallaGateCheckpoint;
/// ★ Named from the game: "Dew" (MsgDjinnName)
extern char SceneId_SuhallaGateDew;
/// ★ Named from its area name: Gateway Cave
extern char SceneId_GatewayCave;
/// ★ Named from the game: "Ye who seek to climb Venus Lighthouse must seek the
/// lady's leave." (MsgTunnelRuinsStatueSpeaksIsaacSoulYe)
extern char SceneId_TunnelRuinsGreatStatue;
/// ★ Named from what the player sees: five coloured statues of a woman standing
/// before a door (its render)
extern char SceneId_TunnelRuinsLadyStatues;
/// ★ Named from the game: "stone door" (MsgBabiYoureSureTheyWentThrough)
extern char SceneId_BabiLighthouseStoneDoor;
/// ★ Named from the game: "Luff" (MsgDjinnName)
extern char SceneId_BabiLighthouseLuff;
/// ★ Named from its area name: Babi Lighthouse Entry
extern char SceneId_BabiLighthouseEntry;
/// ★ Named from its area name: Road to Babi Lighthouse
extern char SceneId_RoadToBabiLighthouse;
/// ★ Named from its area name: Lalivero
extern char SceneId_Lalivero;
/// The first of two sheets of Lalivero's rooms, drawn in timber.
/// ★ Named from what the player sees: rooms drawn in timber, the interiors of
/// Lalivero's weapon and armor shop, item shop and Sanctum, with three houses
/// (its render)
extern char SceneId_TimberRoomsB;
/// The second of two sheets of Lalivero's rooms, drawn in timber.
/// ★ Named from what the player sees: rooms drawn in timber, the interiors of
/// Faran's house, Lalivero's inn and the way out to the wharf (its render)
extern char SceneId_TimberRoomsC;
/// ★ Named from its area name: Venus Lighthouse Entry
extern char SceneId_VenusLighthouseEntry;
/// ★ Named from what the player sees: the floor the front door opens onto, with
/// the great statue and the reliefs (its render)
extern char SceneId_VenusLighthouseGroundFloor;
/// ★ Named from what the player sees: the first floor of the climb, where the
/// Tunnel Ruins come up (its render)
extern char SceneId_VenusLighthouseFirstFloor;
/// ★ Named from what the player sees: the second floor of the climb, with the
/// sparkling passage and the gate (its render)
extern char SceneId_VenusLighthouseSecondFloor;
/// ★ Named from what the player sees: the third floor of the climb, with three
/// sparkling passages and three pillar sockets (its render)
extern char SceneId_VenusLighthouseThirdFloor;
/// ★ Named from what the player sees: the fourth floor of the climb, below the
/// Lighthouse Aerie (its render)
extern char SceneId_VenusLighthouseFourthFloor;
/// ★ Named from the game: "Lighthouse Aerie" (MsgDebugAreaName)
extern char SceneId_VenusLighthouseAerie;
/// ★ Named from its area name: Lemurian ship
extern char SceneId_LemurianShip;
/// ★ Named from its area name: Tolbi Spring
extern char SceneId_TolbiSpring;
/// ★ Named from the game: "Lobby" (MsgDebugAreaName)
extern char SceneId_BattleArena;
/// ★ Named from the game: "Test 0" (MsgDebugAreaName)
extern char SceneId_Test0;
/// ★ Named from the game: "Test 1" (MsgDebugAreaName)
extern char SceneId_Test1;
/// ★ Named from the game: "Test 2" (MsgDebugAreaName)
extern char SceneId_Test2;
/// ★ Named from the game: "Test 3" (MsgDebugAreaName)
extern char SceneId_Test3;
/// ★ Named from the game: "Test 4" (MsgDebugAreaName)
extern char SceneId_Test4;
/// ★ Named from the game: "Test 5" (MsgDebugAreaName)
extern char SceneId_Test5;
/// ★ Named from the game: "Test 6" (MsgDebugAreaName)
extern char SceneId_Test6;
/// ★ Named from the game: "Test 7" (MsgDebugAreaName)
extern char SceneId_Test7;
/// ★ Named from its area name: Increase item and level
extern char SceneId_IncreaseItemAndLevel;
/// ★ Named from its area name: Menu test
extern char SceneId_MenuTest;

#endif
