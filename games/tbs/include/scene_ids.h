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
extern char SceneId_KolimaForest1;
extern char SceneId_KolimaForest2;
extern char SceneId_KolimaForest3;
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
extern char SceneId_MercuryLighthouse1;
extern char SceneId_MercuryLighthouse2;
extern char SceneId_MercuryLighthouse3;
extern char SceneId_MercuryLighthouse4;
extern char SceneId_MercuryLighthouseAerie1;
extern char SceneId_UnusedMercuryLighthouseAerie2;
extern char SceneId_FuchinTemple1;
extern char SceneId_FuchinTemple2;
extern char SceneId_FuchinFallsCave1;
extern char SceneId_FuchinFallsCave2;
extern char SceneId_FuchinFallsCave3;
extern char SceneId_FuchinFallsCave4;
extern char SceneId_FuchinFallsCave5;
extern char SceneId_FuchinFallsCave6;
extern char SceneId_MogallForest1;
extern char SceneId_MogallForest2;
extern char SceneId_MogallForest3;
extern char SceneId_MogallForest4;
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
