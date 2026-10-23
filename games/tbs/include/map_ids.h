#ifndef ALCHEMY_MAP_IDS_H
#define ALCHEMY_MAP_IDS_H

/// @file
/// Map numbers, named after their places, in the order of gMapHeaders
/// (field/map_headers.s).
///
/// Each MapId_ is an absolute symbol whose value is the map's row in the
/// table; C takes the number as (s32)&MapId_Name, and the warp tables name a
/// map by its place (MAP_WARP).

/// ★ Named from the game: "Title screen" (MsgDebugEntryName)
extern char MapId_Title;
/// The main menu, where the player chooses a file; the debug map list calls
/// it "Clear data" (MsgDebugEntryName).
extern char MapId_MainMenu;
/// ★ Named from its area name: World Map
extern char MapId_WorldMap;
/// ★ Named from the game: "I just knew a storm like this would knock it loose!"
/// (MsgValeSureHelp)
extern char MapId_ValeStorm;
/// ★ Named from its area name: Vale
extern char MapId_Vale;
/// ★ Named from the game: "Why's everyone hanging around the town gate?"
/// (MsgValeWhysEveryoneHanging)
extern char MapId_ValeGate;
/// ★ Named from the game: "You mustn't go beyond Kraden's cottage."
/// (MsgValeDontGoBeyondKradensCottage)
extern char MapId_ValeKraden;
/// ★ Named from what the player sees: Vale's item shop and its weapon and armor
/// shop, with Garet's house and Jenna's grandparents' house (its render)
extern char MapId_ValeInteriorsShops;
/// ★ Named from what the player sees: Vale's inn and its upstairs guest room,
/// with Isaac's house and Kraden's cottage (its render)
extern char MapId_ValeInteriorsInn;
/// ★ Named from its area name: Sanctum
extern char MapId_Sanctum;
/// ★ Named from the game: "in this eruption?!?" (MsgValeMtAlephWasInactive)
extern char MapId_ValeEruption;
/// ★ Named from the game: "Luna and Sol" (MsgSolSanctumLunaSolRooms)
extern char MapId_SolSanctumLunaSolRooms;
/// ★ Named from the game: "So the statue puzzle was the trap..."
/// (MsgSolSanctumKradenYouFoundIt)
extern char MapId_SolSanctumStatue;
/// ★ Named from the game: "a Psynergy Stone." (MsgGotPsynergyStone)
extern char MapId_SolSanctumPsynergyStone;
/// ★ Named from what the player sees: a maze of narrow torchlit passages, one
/// of them ending at the chest with the second Small Jewel (its render)
extern char MapId_SolSanctumMaze;
/// ★ Named from what the player sees: one long bending passage of blue
/// flagstones between two doors of the sanctum's rooms (its render)
extern char MapId_SolSanctumCorridor;
/// ★ Named from what the player sees: the hall of the two minotaur reliefs that
/// take the Small Jewels (its render)
extern char MapId_SolSanctumReliefs;
/// ★ Named from the game: "Elemental Star Room" (MsgDebugEntryName)
extern char MapId_SolSanctumElementalStarRoom;
/// ★ Named from the game: "It's erupting..." (MsgSolSanctumReturnStarToBag)
extern char MapId_SolSanctumEruption;
/// ★ Named from its area name: Sol Entrance
extern char MapId_SolEntrance;
/// ★ Named from its area name: Vault
extern char MapId_Vault;
/// ★ Named from what the player sees: the inside of Vault's houses, shops and
/// inn, with the inn's loft behind a crate (its render)
extern char MapId_VaultInteriors;
/// ★ Named from the game: "check out the jail?" (MsgVaultGuysCheckJail)
extern char MapId_VaultJail;
/// ★ Named from what the player sees: Vault again once the party has reached
/// Kalay, its thieves broken out of jail (its render)
extern char MapId_VaultRevisit;
/// ★ Named from what the player sees: the inside of Vault's houses, shops and
/// inn on the later visit, after the thieves escape (its render)
extern char MapId_VaultInteriorsRevisit;
/// ★ Named from its area name: Goma Cave Entrance
extern char MapId_GomaCaveEntrance;
/// ★ Named from what the player sees: a cave room around a pond, below the room
/// the stump is pushed from (its render)
extern char MapId_GomaCavePond;
/// ★ Named from what the player sees: the cave room of two pillars and a stump
/// that Move pushes (its render)
extern char MapId_GomaCavePillar;
/// ★ Named from what the player sees: the Goma Cave room where a stream runs
/// its whole length beside the path (its render)
extern char MapId_GomaCaveStream;
/// ★ Named from what the player sees: Goma Cave's way out, where a wooden
/// bridge crosses the river (its render)
extern char MapId_GomaCaveBridge;
/// ★ Named from its area name: Bilibin
extern char MapId_Bilibin;
/// ★ Named from what the player sees: the inside of Bilibin's shops, inn and
/// houses (its render)
extern char MapId_BilibinInteriors;
/// ★ Named from its area name: Bilibin Treasury
extern char MapId_BilibinTreasury;
/// ★ Named from the game: "McCoy's Palace" (MsgDebugEntryName)
extern char MapId_McCoyPalace;
/// ★ Named from the game: "planning the new palace."
/// (MsgBilibinIShouldntHaveSaidAnythingWhen)
extern char MapId_McCoyPalaceGrounds;
/// ★ Named from its area name: Bilibin Barricade
extern char MapId_BilibinBarricade;
/// ★ Named from its area name: Kolima
extern char MapId_Kolima;
/// ★ Named from what the player sees: the inside of Kolima's houses, shops and
/// inn, their people turned to trees (its render)
extern char MapId_KolimaInteriors;
/// ★ Named from its area name: Kolima Tunnel
extern char MapId_KolimaTunnel;
/// ★ Named from its area name: Kolima Bridge
extern char MapId_KolimaBridge;
/// ★ Named from what the player sees: the way into Kolima Forest from the world
/// map, a river crossed on log bridges (its render)
extern char MapId_KolimaForestEntrance;
/// ★ Named from what the player sees: the one forest map with no water, an open
/// grassy glade ringed by trees (its render)
extern char MapId_KolimaForestGlade;
/// ★ Named from what the player sees: a cliff ridge runs along the whole north
/// of the map, a river down its west (its render)
extern char MapId_KolimaForestCliff;
/// ★ Named from the game: "Floodgate Switch" (MsgKolimaDoNotTouchFloodgate)
extern char MapId_KolimaForestFloodgate;
/// ★ Named from the game: "The holy tree of Kolima"
/// (MsgKolimaInteriorsTheHolyTreeOfKolimaHas)
extern char MapId_KolimaForestTree;
/// ★ Named from what the player sees: the rooms inside the trunk of Tret Tree
/// (its render)
extern char MapId_TretTreeInteriors;
/// ★ Named from the game: "Then those gems fell into my branches..."
/// (MsgKolimaWasIndeedAngryPeopleHad)
extern char MapId_TretTreeBranch;
/// ★ Named from what the player sees: the Bilibin Cave room of ice pillars that
/// Frost, fires and torches freeze and melt (its render)
extern char MapId_BilibinCaveIce;
/// ★ Named from the game: "Elven Rapier" (MsgItemName)
extern char MapId_BilibinCaveRapier;
/// ★ Named from its area name: Bilibin Cave Entrance
extern char MapId_BilibinCaveEntrance;
/// ★ Named from its area name: Imil
extern char MapId_Imil;
/// ★ Named from what the player sees: the inside of Imil's houses, shops, inn
/// and Sanctum (its render)
extern char MapId_ImilInteriors;
/// ★ Named from its area name: Imil Falls Cave
extern char MapId_ImilFallsCave;
/// ★ Named from what the player sees: the lighthouse's ground floor inside its
/// door, where the Lizard Man blocks Mia (its render)
extern char MapId_MercuryLighthouseEntrance;
/// ★ Named from what the player sees: the first rooms of the climb above the
/// ground floor, the pool crossed under rings of light and the pipes, where
/// Mia joins (its render)
extern char MapId_MercuryLighthouseLower;
/// ★ Named from what the player sees: the middle of the climb, with the gate
/// held open by a statue, the pipe maze and Sleet's room (its render)
extern char MapId_MercuryLighthouseMiddle;
/// ★ Named from what the player sees: the top of the climb inside, with the
/// waterfall halls, the five pillars and the stone doors (its render)
extern char MapId_MercuryLighthouseUpper;
/// ★ Named from what the player sees: the lighthouse from outside, its foot in
/// the water with the door and fountain, and the walkways up to the waterfall
/// (its render)
extern char MapId_MercuryLighthouseExterior;
/// ★ Named from its area name: Lighthouse Aerie
extern char MapId_MercuryLighthouseAerie;
/// ★ Named from what the player sees: the same aerie as the one in use, empty,
/// its platforms not lowered (its render)
extern char MapId_UnusedMercuryLighthouseAerieCopy;
/// ★ Named from what the player sees: outside the temple, its monks and the
/// stepped waterfall with the grotto beneath (its render)
extern char MapId_FuchinTempleGrounds;
/// ★ Named from what the player sees: the rooms inside, Nyunpa's hall, Feh's
/// school in Xian and Hama's room in Lama Temple (its render)
extern char MapId_FuchinTempleInteriors;
/// ★ Named from what the player sees: the cave's mouth and only way in and out,
/// a log pool and stumps in the water (its render)
extern char MapId_FuchinFallsCaveEntrance;
/// ★ Named from the game: "Zephyr" (MsgDjinnName)
extern char MapId_FuchinFallsCaveZephyr;
/// ★ Named from the game: "Dragon's Eye" (MsgItemName)
extern char MapId_FuchinFallsCaveDragonsEye;
/// ★ Named from the game: "Arctic Blade" (MsgItemName)
extern char MapId_FuchinFallsCaveArcticBlade;
/// ★ Named from the game: "Orb of Force" (MsgItemName)
extern char MapId_FuchinFallsCaveOrbOfForce;
/// ★ Named from the game: "It's a statue of an eyeless dragon."
/// (MsgFuchinEyelessDragon)
extern char MapId_FuchinFallsCaveDragonStatue;
/// ★ Named from the game: "The broken sign reads:" (MsgMogallForestBrokenSign)
extern char MapId_MogallForestSign;
/// ★ Named from the game: "Quartz" (MsgDjinnName)
extern char MapId_MogallForestQuartz;
/// ★ Named from the game: "Killer Ape" (MsgEnemyName)
extern char MapId_MogallForestKillerApe;
/// ★ Named from what the player sees: small identical clearings the wrong turns
/// lead into, whose far sides lead back in (its render)
extern char MapId_MogallForestSidePaths;
/// ★ Named from its area name: Xian
extern char MapId_Xian;
/// ★ Named from what the player sees: the inside of Xian's houses, shops and
/// inn, and the room behind Master Feh's school (its render)
extern char MapId_XianInteriors;
/// ★ Named from its area name: Alpine Crossing
extern char MapId_AlpineCrossing;
/// ★ Named from its area name: Altin
extern char MapId_Altin;
/// ★ Named from what the player sees: the inside of Altin's houses, shops and
/// inn, where flooded-out families shelter (its render)
extern char MapId_AltinInteriors;
/// ★ Named from the game: "The water beasts looked"
/// (MsgAltinTheWaterBeastsLookedExactlyLike)
extern char MapId_AltinPeakFirstWaterBeast;
/// ★ Named from what the player sees: a winding tunnel with nothing in it but
/// one rail line between fences (its render)
extern char MapId_AltinPeakRailTunnel;
/// ★ Named from the game: "The water beasts looked"
/// (MsgAltinTheWaterBeastsLookedExactlyLike)
extern char MapId_AltinPeakSecondWaterBeast;
/// ★ Named from the game: "North: Altin Mine"
/// (MsgMountainNorthAltinMineWestLama)
extern char MapId_AltinPeakSouthEntrance;
/// ★ Named from the game: "The water beasts looked"
/// (MsgAltinTheWaterBeastsLookedExactlyLike)
extern char MapId_AltinPeakThirdWaterBeast;
/// ★ Named from what the player sees: a cave filled by a large pool, with rails
/// around it and a doorway above it (its render)
extern char MapId_AltinPeakPool;
/// ★ Named from the game: "Spritz" (MsgDjinnName)
extern char MapId_AltinPeakSpritz;
/// ★ Named from what the player sees: two tunnels where five ways meet, to
/// Altin and to three other rooms of the mine (its render)
extern char MapId_AltinPeakJunction;
/// ★ Named from the game: "Watch for falling rocks!"
/// (MsgAltinWatchFallingRocks)
extern char MapId_AltinPeakFallingRocks;
/// ★ Named from the game: "Hydros Statue" (MsgEnemyName)
extern char MapId_AltinPeakHydrosStatue;
/// ★ Named from the game: "find your way west" (MsgAltinTryingFindYourWayWest)
extern char MapId_AltinPeakWestExit;
/// ★ Named from its area name: Lama Temple
extern char MapId_LamaTemple;
/// ★ Named from what the player sees: the desert's east end, entered from the
/// world map beside Lama Temple (its render)
extern char MapId_LamakanDesertEast;
/// ★ Named from the game: "Smog" (MsgDjinnName)
extern char MapId_LamakanDesertSmog;
/// ★ Named from the game: "Manticore" (MsgEnemyName)
extern char MapId_LamakanDesertManticore;
/// ★ Named from what the player sees: dark rock tunnels lit by torches at the
/// desert's west end, with no sand (its render)
extern char MapId_LamakanDesertCave;
/// ★ Named from what the player sees: the clearing at the cave's mouth above
/// Vale (its render)
extern char MapId_ValeCaveMouth;
/// ★ Named from the game: "Kite" (MsgDjinnName)
extern char MapId_ValeCaveKite;
/// ★ Named from what the player sees: the cave whose three logs the party
/// pushes (its render)
extern char MapId_ValeCaveLogs;
/// ★ Named from the game: "a passage leading"
/// (MsgVaultCarefulSearchWillRevealPassage)
extern char MapId_VaultCavePassage;
/// ★ Named from the game: "Vambrace" (MsgItemName)
extern char MapId_VaultCaveVambrace;
/// ★ Named from what the player sees: the part of Vault Cave entered from the
/// cave mouth in Vault's northeast cliff (its render)
extern char MapId_VaultCaveMouth;
/// ★ Named from its area name: Kalay
extern char MapId_Kalay;
/// ★ Named from what the player sees: Kalay's houses, its weapon and armor
/// shops and its inn with the guest rooms below (its render)
extern char MapId_KalayInteriors;
/// ★ Named from its area name: Kalay Sanctum
extern char MapId_KalaySanctum;
/// ★ Named from what the player sees: the front of Hammet Palace, where two
/// soldiers guard the door above the stairs from town (its render)
extern char MapId_HammetPalaceGrounds;
/// ★ Named from its area name: Hammet Palace
extern char MapId_HammetPalace;
/// ★ Named from its area name: Lunpa
extern char MapId_Lunpa;
/// ★ Named from what the player sees: the inside of Lunpa's homes, shops, inn,
/// crate room and temple, and the house at Suhalla Gate (its render)
extern char MapId_LunpaInteriors;
/// ★ Named from its area name: West Lunpa Cave
extern char MapId_WestLunpaCave;
/// ★ Named from its area name: Kalay Docks
extern char MapId_KalayDocks;
/// ★ Named from what the player sees: the rooms of the dock buildings at Kalay
/// Docks and Tolbi Docks, where boarding tickets are sold (its render)
extern char MapId_KalayTolbiDocksInteriors;
/// ★ Named from what the player sees: the deck of the Tolbi-bound Ship on its
/// voyage across the Karagol Sea (its render)
extern char MapId_TolbiShipDeck;
/// ★ Named from the game: "You climbed up the mast"
/// (MsgTolbiShipItsMyLuckyAnchor)
extern char MapId_TolbiShipMast;
/// ★ Named from the game: "Monsters belowdecks!"
/// (MsgTolbiShipMonstersBelowdecks)
extern char MapId_TolbiShipInteriors;
/// ★ Named from its area name: Tolbi Docks
extern char MapId_TolbiDocks;
/// ★ Named from its area name: Crossbone Isle
extern char MapId_CrossboneIsle;
/// ★ Named from what the player sees: the winding path lit by torches that
/// leads down from the island into the cave (its render)
extern char MapId_CrossboneIsleCaveEntrance;
/// ★ Named from what the player sees: raised stone posts over a pit, which the
/// party hops across to reach four chests (its render)
extern char MapId_CrossboneIsleSteppingStones;
/// ★ Named from what the player sees: pillars of ice, some standing and some
/// lying as walkways, across a floor of spikes (its render)
extern char MapId_CrossboneIsleIcePillars;
/// ★ Named from the game: "Blue Key" (MsgItemName)
extern char MapId_CrossboneIsleKeys;
/// ★ Named from what the player sees: a dark hall lit by braziers, where five
/// statues of a robed woman step out after the leader (its render)
extern char MapId_CrossboneIsleStatues;
/// ★ Named from what the player sees: a cave with a stream and a floodgate
/// across it, whose switch floats seven logs (its render)
extern char MapId_CrossboneIsleFloodgate;
/// ★ Named from the game: "Bane" (MsgDjinnName)
extern char MapId_CrossboneIsleBane;
/// ★ Named from what the player sees: a cave of cliffs and ladders, where
/// carved stone pillars are lifted onto one another (its render)
extern char MapId_CrossboneIsleStackedPillars;
/// ★ Named from what the player sees: an icy cave with a coiled, winged dragon
/// statue beside three puddles (its render)
extern char MapId_CrossboneIsleDragonStatue;
/// ★ Named from what the player sees: two pools of green water, with logs
/// floating on them that carry the leader (its render)
extern char MapId_CrossboneIsleFloatingLogs;
/// ★ Named from what the player sees: the ledge at the end of the cave, where a
/// wooden pier leads out to the Pirate Ship (its render)
extern char MapId_CrossboneIslePier;
/// ★ Named from its area name: Pirate Ship
extern char MapId_PirateShip;
/// ★ Named from what the player sees: the first of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char MapId_CrossboneIsleFirstFloor;
/// ★ Named from what the player sees: the second of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char MapId_CrossboneIsleSecondFloor;
/// ★ Named from what the player sees: the third of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char MapId_CrossboneIsleThirdFloor;
/// ★ Named from what the player sees: the fourth of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char MapId_CrossboneIsleFourthFloor;
/// ★ Named from what the player sees: the fifth of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char MapId_CrossboneIsleFifthFloor;
/// ★ Named from what the player sees: the sixth of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char MapId_CrossboneIsleSixthFloor;
/// ★ Named from what the player sees: the seventh of nine halls that look
/// alike, each a flight of stairs lower than the last (its render)
extern char MapId_CrossboneIsleSeventhFloor;
/// ★ Named from what the player sees: the eighth of nine halls that look alike,
/// each a flight of stairs lower than the last (its render)
extern char MapId_CrossboneIsleEighthFloor;
/// ★ Named from what the player sees: the last of nine halls that look alike,
/// which leads on to the pier (its render)
extern char MapId_CrossboneIsleNinthFloor;
/// ★ Named from its area name: Tolbi
extern char MapId_Tolbi;
/// ★ Named from what the player sees: the inside of Tolbi's houses, inns and
/// shops, and Tolbi Sanctum (its render)
extern char MapId_TolbiInteriors;
/// ★ Named from what the player sees: the two small rooms in Tolbi where Lucky
/// Dice and Lucky Wheels are played (its render)
extern char MapId_TolbiGameRooms;
/// ★ Named from its area name: Babi's Palace
extern char MapId_BabiPalace;
/// ★ Named from what the player sees: the way up from Tolbi to the palace door,
/// where two guards stand (its render)
extern char MapId_BabiPalaceEntrance;
/// ★ Named from its area name: Colosseum
extern char MapId_Colosseum;
/// ★ Named from its area name: Colosseum Tunnel
extern char MapId_ColosseumTunnel;
/// ★ Named from its area name: Colosseum Entrance
extern char MapId_ColosseumEntrance;
/// ★ Named from the game: "Azart" (MsgEnemyName)
extern char MapId_ColosseumAzartMatch;
/// ★ Named from the game: "Satrage" (MsgEnemyName)
extern char MapId_ColosseumSatrageMatch;
/// ★ Named from the game: "Navampa" (MsgEnemyName)
extern char MapId_ColosseumNavampaMatch;
/// ★ Named from what the player sees: the dark winding passage in from the cave
/// mouth and the small chamber beyond it (its render)
extern char MapId_AltmillerCaveEntrance;
/// ★ Named from the game: "Babi!" (MsgAltmillerBabi)
extern char MapId_AltmillerCaveBabi;
/// ★ Named from the game: "Vial" (MsgItemName)
extern char MapId_AltmillerCaveVialChest;
/// ★ Named from the game: "Squall" (MsgDjinnName)
extern char MapId_AltmillerCaveSquall;
/// ★ Named from the game: "Cookie" (MsgItemName)
extern char MapId_AltmillerCaveCookieChest;
/// ★ Named from the game: "Mystic Draught" (MsgItemName)
extern char MapId_AltmillerCaveDraught;
/// ★ Named from its area name: Gondowan Passage
extern char MapId_GondowanPassage;
/// ★ Named from its area name: Kalay Tunnel Entrance
extern char MapId_KalayTunnelEntrance;
/// ★ Named from the game: "the cave up ahead" (MsgKalayDoWantGoCaveUp)
extern char MapId_KalayTunnelCave;
/// ★ Named from the game: "underground passage"
/// (MsgHammetPalaceILeftTheDoorToThe)
extern char MapId_KalayTunnelPassage;
/// ★ Named from the game: "Hammet's treasure room"
/// (MsgHammetPalaceTheKeyToTheSecretPassage)
extern char MapId_KalayTunnelTreasureRoom;
/// ★ Named from what the player sees: the underground river crossed on stepping
/// pads, with two chests, between the cave's two halves (its render)
extern char MapId_GondowanCaveRiver;
/// ★ Named from its area name: Gondowan Cave
extern char MapId_GondowanCave;
/// ★ Named from its area name: Lunpa Fortress Gates
extern char MapId_LunpaFortressGates;
/// ★ Named from what the player sees: the fortress floor inside its door, whose
/// staircases climb to the floor above (its render)
extern char MapId_LunpaFortressGroundFloor;
/// ★ Named from what the player sees: the floor reached up the fortress's two
/// staircases, where Donpa sleeps (its render)
extern char MapId_LunpaFortressUpperFloor;
/// ★ Named from the game: "Hammet's down in that treacherous cave."
/// (MsgLunpaHammetInCave)
extern char MapId_LunpaFortressCave;
/// ★ Named from the game: "Toadonpa" (MsgEnemyName)
extern char MapId_LunpaFortressToadonpa;
/// ★ Named from what the player sees: the northern half of the desert, entered
/// from the world map just south of Suhalla (its render)
extern char MapId_SuhallaDesertNorth;
/// ★ Named from what the player sees: the southern half of the desert, whose
/// way out leads on toward Suhalla Gate (its render)
extern char MapId_SuhallaDesertSouth;
/// ★ Named from what the player sees: two caves under the desert, reached
/// through its two cave mouths (its render)
extern char MapId_SuhallaDesertCaves;
/// ★ Named from its area name: Suhalla
extern char MapId_Suhalla;
/// ★ Named from what the player sees: the inside of Suhalla's houses, shop and
/// inn, and Suhalla Sanctum (its render)
extern char MapId_SuhallaInteriors;
/// ★ Named from the game: "Only those with travel papers may pass through."
/// (MsgSuhallaTravelPapersRequired)
extern char MapId_SuhallaGateCheckpoint;
/// ★ Named from the game: "Dew" (MsgDjinnName)
extern char MapId_SuhallaGateDew;
/// ★ Named from its area name: Gateway Cave
extern char MapId_GatewayCave;
/// ★ Named from the game: "Ye who seek to climb Venus Lighthouse must seek the
/// lady's leave." (MsgTunnelRuinsStatueSpeaksIsaacSoulYe)
extern char MapId_TunnelRuinsGreatStatue;
/// ★ Named from what the player sees: five coloured statues of a woman standing
/// before a door (its render)
extern char MapId_TunnelRuinsLadyStatues;
/// ★ Named from the game: "stone door" (MsgBabiYoureSureTheyWentThrough)
extern char MapId_BabiLighthouseStoneDoor;
/// ★ Named from the game: "Luff" (MsgDjinnName)
extern char MapId_BabiLighthouseLuff;
/// ★ Named from its area name: Babi Lighthouse Entry
extern char MapId_BabiLighthouseEntry;
/// ★ Named from its area name: Road to Babi Lighthouse
extern char MapId_RoadToBabiLighthouse;
/// ★ Named from its area name: Lalivero
extern char MapId_Lalivero;
/// ★ Named from what the player sees: Lalivero's weapon and armor shop, item
/// shop and Sanctum, with three houses (its render)
extern char MapId_LaliveroShops;
/// ★ Named from the game: "Faran's house" (MsgLaliveroPleaseWaitForMeOutside)
extern char MapId_LaliveroFaransHouse;
/// ★ Named from its area name: Venus Lighthouse Entry
extern char MapId_VenusLighthouseEntry;
/// ★ Named from what the player sees: the floor the front door opens onto, with
/// the great statue and the reliefs (its render)
extern char MapId_VenusLighthouseGroundFloor;
/// ★ Named from what the player sees: the first floor of the climb, where the
/// Tunnel Ruins come up (its render)
extern char MapId_VenusLighthouseFirstFloor;
/// ★ Named from what the player sees: the second floor of the climb, with the
/// sparkling passage and the gate (its render)
extern char MapId_VenusLighthouseSecondFloor;
/// ★ Named from what the player sees: the third floor of the climb, with three
/// sparkling passages and three pillar sockets (its render)
extern char MapId_VenusLighthouseThirdFloor;
/// ★ Named from what the player sees: the fourth floor of the climb, below the
/// Lighthouse Aerie (its render)
extern char MapId_VenusLighthouseFourthFloor;
/// ★ Named from the game: "Lighthouse Aerie" (MsgDebugEntryName)
extern char MapId_VenusLighthouseAerie;
/// ★ Named from its area name: Lemurian ship
extern char MapId_LemurianShip;
/// ★ Named from its area name: Tolbi Spring
extern char MapId_TolbiSpring;
/// ★ Named from the game: "Lobby" (MsgDebugEntryName)
extern char MapId_BattleArena;
/// ★ Named from the game: "Test 0" (MsgDebugEntryName)
extern char MapId_TestRoom0;
/// ★ Named from the game: "Test 1" (MsgDebugEntryName)
extern char MapId_TestRoom1;
/// ★ Named from the game: "Test 2" (MsgDebugEntryName)
extern char MapId_TestRoom2;
/// ★ Named from the game: "Test 3" (MsgDebugEntryName)
extern char MapId_TestRoom3;
/// ★ Named from the game: "Test 4" (MsgDebugEntryName)
extern char MapId_TestRoom4;
/// ★ Named from the game: "Test 5" (MsgDebugEntryName)
extern char MapId_TestRoom5;
/// ★ Named from the game: "Test 6" (MsgDebugEntryName)
extern char MapId_TestRoom6;
/// ★ Named from the game: "Test 7" (MsgDebugEntryName)
extern char MapId_TestRoom7;
/// ★ Named from its area name: Increase item and level
extern char MapId_IncreaseItemAndLevel;
/// ★ Named from its area name: Menu test
extern char MapId_MenuTest;

#endif
