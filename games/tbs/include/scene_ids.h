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
extern char SceneId_AltinPeak1;
extern char SceneId_AltinPeak2;
extern char SceneId_AltinPeak3;
extern char SceneId_AltinPeak4;
extern char SceneId_AltinPeak5;
extern char SceneId_AltinPeak6;
extern char SceneId_AltinPeak7;
extern char SceneId_AltinPeak8;
extern char SceneId_AltinPeak9;
extern char SceneId_AltinPeak10;
extern char SceneId_AltinPeak11;
extern char SceneId_LamaTemple;
extern char SceneId_LamakanDesert1;
extern char SceneId_LamakanDesert2;
extern char SceneId_LamakanDesert3;
extern char SceneId_LamakanDesert4;
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
extern char SceneId_Kalay1;
extern char SceneId_KalayInteriors1;
extern char SceneId_KalaySanctum;
extern char SceneId_Kalay2;
extern char SceneId_HammetPalace;
extern char SceneId_Lunpa;
extern char SceneId_LunpaSuhalla;
extern char SceneId_WestLunpaCave;
extern char SceneId_KalayDocks;
extern char SceneId_KalayTolbiDocks2;
extern char SceneId_TolbiShipDeck;
extern char SceneId_TolbiShipMast;
extern char SceneId_TolbiShipInteriors;
extern char SceneId_TolbiDocks;
extern char SceneId_CrossboneIsle;
extern char SceneId_CrossboneIsle2;
extern char SceneId_CrossboneIslePillarRooms1;
extern char SceneId_CrossboneIslePillarRooms2;
extern char SceneId_CrossboneIsleKeys;
extern char SceneId_CrossboneIslePlatformRooms2;
extern char SceneId_CrossboneIsleFloodgate;
extern char SceneId_CrossboneIsleBane;
extern char SceneId_CrossboneIslePillarRooms4;
extern char SceneId_CrossboneIslePillarRooms5;
extern char SceneId_CrossboneIsle3;
extern char SceneId_CrossboneIsle4;
extern char SceneId_PirateShip;
extern char SceneId_CrossboneIsle6;
extern char SceneId_CrossboneIsle7;
extern char SceneId_CrossboneIsle8;
extern char SceneId_CrossboneIsle9;
extern char SceneId_CrossboneIsle10;
extern char SceneId_CrossboneIsle11;
extern char SceneId_CrossboneIsle12;
extern char SceneId_CrossboneIsle13;
extern char SceneId_CrossboneIsle14;
extern char SceneId_Tolbi;
extern char SceneId_TolbiInteriors;
extern char SceneId_TolbiSpring1;
extern char SceneId_BabiPalace1;
extern char SceneId_BabiPalace2;
extern char SceneId_Colosseum;
extern char SceneId_ColosseumTunnel;
extern char SceneId_ColosseumEntrance;
extern char SceneId_ColosseumFinal1;
extern char SceneId_ColosseumFinal2;
extern char SceneId_ColosseumFinal3;
extern char SceneId_AltmillerCave1;
extern char SceneId_AltmillerCave2;
extern char SceneId_AltmillerCave3;
extern char SceneId_AltmillerCave4;
extern char SceneId_AltmillerCave5;
extern char SceneId_AltmillerCave6;
extern char SceneId_GondowanPassage;
extern char SceneId_KalayTunnelEntrance;
extern char SceneId_KalayTunnel1;
extern char SceneId_KalayTunnel2;
extern char SceneId_KalayTunnel3;
extern char SceneId_GondowanCave1;
extern char SceneId_GondowanCave2;
extern char SceneId_LunpaFortressGates;
extern char SceneId_LunpaFortress1;
extern char SceneId_LunpaFortress2;
extern char SceneId_LunpaFortress3;
extern char SceneId_LunpaFortress4;
extern char SceneId_SuhallaDesert1;
extern char SceneId_SuhallaDesert2;
extern char SceneId_SuhallaDesert3;
extern char SceneId_Suhalla;
extern char SceneId_SuhallaInteriors;
extern char SceneId_SuhallaGate1;
extern char SceneId_SuhallaGate2;
extern char SceneId_GatewayCave;
extern char SceneId_TunnelRuins1;
extern char SceneId_TunnelRuins2;
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
extern char SceneId_TolbiSpring2;
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
