#ifndef ALCHEMY_SCENE_IDS_H
#define ALCHEMY_SCENE_IDS_H

/// @file
/// Scene numbers, named after their places, in the order of gField_SceneTable
/// (field/scenes.s).
///
/// Each SceneId_ is an absolute symbol whose value is the scene's row in the
/// table; C takes the number as (s32)&SceneId_Name, and the exit tables name a
/// scene by its place (SCENE_EXIT).

extern char SceneId_Title;
extern char SceneId_Clear;
extern char SceneId_WorldMap;
extern char SceneId_ValeStorm;
extern char SceneId_Vale;
extern char SceneId_ValeGate;
extern char SceneId_ValeKraden;
/// ★ Named from what the player sees: Vale's item shop and its weapon and armor
/// shop, with Garet's house and Jenna's grandparents' house (its render)
extern char SceneId_ValeInteriorsShops;
/// ★ Named from what the player sees: Vale's inn and its upstairs guest room,
/// with Isaac's house and Kraden's cottage (its render)
extern char SceneId_ValeInteriorsInn;
extern char SceneId_Sanctum;
extern char SceneId_ValeEruption;
extern char SceneId_SolSanctumLunaSolRooms;
extern char SceneId_SolSanctumStatue;
extern char SceneId_SolSanctumPsynergyStone;
extern char SceneId_SolSanctumMaze;
extern char SceneId_SolSanctumCorridor;
/// ★ Named from what the player sees: the hall of the two minotaur reliefs that
/// take the Small Jewels (its render)
extern char SceneId_SolSanctumReliefs;
extern char SceneId_SolSanctumElementalStarRoom;
extern char SceneId_SolSanctumEruption;
extern char SceneId_SolEntrance;
extern char SceneId_Vault;
extern char SceneId_VaultInteriors;
extern char SceneId_VaultJail;
extern char SceneId_VaultRevisit;
extern char SceneId_VaultInteriorsRevisit;
extern char SceneId_GomaCaveEntrance;
extern char SceneId_GomaCavePond;
extern char SceneId_GomaCavePillar;
/// ★ Named from what the player sees: the Goma Cave room where a stream runs
/// its whole length beside the path (its render)
extern char SceneId_GomaCaveStream;
/// ★ Named from what the player sees: Goma Cave's way out, where a wooden
/// bridge crosses the river (its render)
extern char SceneId_GomaCaveBridge;
extern char SceneId_Bilibin;
extern char SceneId_BilibinInteriors;
extern char SceneId_BilibinTreasury;
extern char SceneId_McCoyPalace;
extern char SceneId_McCoyPalaceGrounds;
extern char SceneId_BilibinBarricade;
extern char SceneId_Kolima;
extern char SceneId_KolimaInteriors;
extern char SceneId_KolimaTunnel;
extern char SceneId_KolimaBridge;
/// ★ Named from what the player sees: the way into Kolima Forest from the world
/// map, a river crossed on log bridges (its render)
extern char SceneId_KolimaForestEntrance;
/// ★ Named from what the player sees: the one forest map with no water, an open
/// grassy glade ringed by trees (its render)
extern char SceneId_KolimaForestGlade;
/// ★ Named from what the player sees: a cliff ridge runs along the whole north
/// of the map, a river down its west (its render)
extern char SceneId_KolimaForestCliff;
extern char SceneId_KolimaForestFloodgate;
extern char SceneId_KolimaForestTree;
extern char SceneId_TretTreeInteriors;
extern char SceneId_TretTreeBranch;
/// ★ Named from what the player sees: the Bilibin Cave room of ice pillars that
/// Frost, fires and torches freeze and melt (its render)
extern char SceneId_BilibinCaveIce;
/// ★ Named from the game: "Elven Rapier" (MsgItemName)
extern char SceneId_BilibinCaveRapier;
extern char SceneId_BilibinCaveEntrance;
extern char SceneId_Imil;
extern char SceneId_ImilInteriors;
extern char SceneId_ImilFallsCave;
extern char SceneId_MercuryLighthouseEntrance;
/// ★ Named from what the player sees: the first rooms of the climb above the
/// ground floor, the pool of water pillars and the pipes, where Mia joins (its
/// render)
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
/// ★ Named from what the player sees: the rooms inside, Nyunpa's hall, Feh's
/// school in Xian and Hama's room in Lama Temple (its render)
extern char SceneId_FuchinTempleInteriors;
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
extern char SceneId_Xian;
extern char SceneId_XianInteriors;
extern char SceneId_AlpineCrossing;
extern char SceneId_Altin;
extern char SceneId_AltinInteriors;
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
/// ★ Named from what the player sees: Kalay's houses, its weapon and armor
/// shops and its inn with the guest rooms below (its render)
extern char SceneId_KalayInteriors;
extern char SceneId_KalaySanctum;
/// ★ Named from what the player sees: the front of Hammet Palace, where two
/// soldiers guard the door above the stairs from town (its render)
extern char SceneId_HammetPalaceGrounds;
extern char SceneId_HammetPalace;
extern char SceneId_Lunpa;
extern char SceneId_LunpaSuhalla;
extern char SceneId_WestLunpaCave;
extern char SceneId_KalayDocks;
/// ★ Named from what the player sees: the rooms of the dock buildings at Kalay
/// Docks and Tolbi Docks, where boarding tickets are sold (its render)
extern char SceneId_KalayTolbiDocksInteriors;
extern char SceneId_TolbiShipDeck;
extern char SceneId_TolbiShipMast;
extern char SceneId_TolbiShipInteriors;
extern char SceneId_TolbiDocks;
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
extern char SceneId_CrossboneIsleKeys;
/// ★ Named from what the player sees: a dark hall lit by braziers, where five
/// statues of a robed woman step out after the leader (its render)
extern char SceneId_CrossboneIsleStatues;
extern char SceneId_CrossboneIsleFloodgate;
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
extern char SceneId_Tolbi;
extern char SceneId_TolbiInteriors;
/// ★ Named from what the player sees: the two small rooms in Tolbi where Lucky
/// Dice and Lucky Wheels are played (its render)
extern char SceneId_TolbiGameRooms;
/// ★ Named from its area name: Babi's Palace
extern char SceneId_BabiPalace;
/// ★ Named from what the player sees: the way up from Tolbi to the palace door,
/// where two guards stand (its render)
extern char SceneId_BabiPalaceEntrance;
extern char SceneId_Colosseum;
extern char SceneId_ColosseumTunnel;
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
extern char SceneId_GondowanPassage;
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
extern char SceneId_Suhalla;
extern char SceneId_SuhallaInteriors;
/// ★ Named from the game: "Only those with travel papers may pass through."
/// (MsgSuhallaTravelPapersRequired)
extern char SceneId_SuhallaGateCheckpoint;
/// ★ Named from the game: "Dew" (MsgDjinnName)
extern char SceneId_SuhallaGateDew;
extern char SceneId_GatewayCave;
/// ★ Named from the game: "Ye who seek to climb Venus Lighthouse must seek the
/// lady's leave." (MsgTunnelRuinsStatueSpeaksIsaacSoulYe)
extern char SceneId_TunnelRuinsGreatStatue;
/// ★ Named from what the player sees: five coloured statues of a woman standing
/// before a door (its render)
extern char SceneId_TunnelRuinsLadyStatues;
extern char SceneId_BabiLighthouse1;
extern char SceneId_BabiLighthouse2;
extern char SceneId_BabiLighthouseEntry;
extern char SceneId_RoadToBabiLighthouse;
extern char SceneId_Lalivero;
extern char SceneId_LaliveroInteriors1;
extern char SceneId_LaliveroInteriors2;
extern char SceneId_VenusLighthouseEntry;
extern char SceneId_VenusLighthouse1;
extern char SceneId_VenusLighthouse2;
extern char SceneId_VenusLighthouse3;
extern char SceneId_VenusLighthouse4;
extern char SceneId_VenusLighthouse5;
extern char SceneId_VenusLighthouseAerie;
extern char SceneId_LemurianShip;
/// ★ Named from its area name: Tolbi Spring
extern char SceneId_TolbiSpring;
extern char SceneId_LinkLobby;
extern char SceneId_TestRoom0;
extern char SceneId_TestRoom1;
extern char SceneId_TestRoom2;
extern char SceneId_TestRoom3;
extern char SceneId_TestRoom4;
extern char SceneId_TestRoom5;
extern char SceneId_TestRoom6;
extern char SceneId_TestRoom7;
extern char SceneId_IncreaseItemAndLevel;
extern char SceneId_MenuTest;

#endif
