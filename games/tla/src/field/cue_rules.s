	.include "enums/flags.inc"

	.section .rodata
@ Scene music: match scene/group and entrance (-1 matches any), then use
@ the first row whose flag is set (-1 is unconditional).
	.macro scene_cue scene:req, entrance:req, flag:req, cue:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000, \flag, \cue
	.endm
	.macro group_cue group:req, entrance:req, flag:req, cue:req
	.2byte \group, \entrance & 0x7fff, \flag, \cue
	.endm
	.global gScene_CueRuleTable
gScene_CueRuleTable:
	group_cue 1, -1, -1, 74
	group_cue 2, 64, -1, 39
	group_cue 2, 78, -1, 79
	group_cue 2, 77, -1, 742
	group_cue 2, -1, FLAG_SAILING, 41
	group_cue 2, -1, FLAG_REUNITED, 43
	group_cue 2, -1, -1, 42
	group_cue 3, -1, -1, 44
	scene_cue VenusLighthouseEntrance, -1, -1, 727
	group_cue 4, -1, -1, 743
	group_cue 5, -1, -1, 724
	group_cue 6, -1, -1, 742
	scene_cue DailaInteriorsHouses, 7, -1, 13
	group_cue 7, -1, -1, 4
	group_cue 8, -1, -1, 9
	group_cue 9, -1, -1, 3
	group_cue 10, -1, -1, 23
	scene_cue DehkanPlateauMintChest, -1, -1, 728
	scene_cue DehkanPlateauPoundCube, -1, -1, 728
	scene_cue DehkanPlateauCannon, -1, -1, 728
	group_cue 11, -1, -1, 728
	group_cue 27, -1, FLAG_REUNITED, 43
	group_cue 27, -1, -1, 42
	group_cue 14, -1, -1, 13
	group_cue 15, -1, -1, 22
	group_cue 17, -1, FLAG_REUNITED, 43
	group_cue 17, -1, -1, 42
	scene_cue MikasallaInteriorsSanctum, 4, -1, 16
	scene_cue MikasallaInteriorsSanctum, 5, -1, 5
	scene_cue MikasallaInteriorsHouses, 5, -1, 726
	scene_cue MikasallaNorth, -1, -1, 22
	group_cue 18, -1, -1, 13
	scene_cue GarohInteriorsSanctum, -1, -1, 15
	group_cue 19, -1, -1, 15
	group_cue 20, -1, -1, 725
	group_cue 21, -1, -1, 724
	group_cue 22, -1, -1, 20
	group_cue 23, -1, -1, 20
	group_cue 24, -1, -1, 2
	group_cue 25, -1, -1, 2
	scene_cue AlhafraJail, 8, -1, 2
	group_cue 26, -1, -1, 22
	group_cue 27, -1, -1, 3
	group_cue 28, -1, -1, 10
	group_cue 29, -1, -1, 10
	group_cue 30, -1, -1, 10
	group_cue 31, -1, FLAG_GABOMBA_MUSIC_SILENCED, 0
	group_cue 31, -1, FLAG_GABOMBA_CEREMONY_UNDER_WAY, 11
	group_cue 31, -1, -1, 11
	scene_cue KibomboStatueFace, -1, -1, 25
	group_cue 32, -1, FLAG_GABOMBA_MUSIC_SILENCED, 0
	group_cue 32, -1, -1, 25
	group_cue 33, -1, -1, 729
	scene_cue LemurianShipMainDeck, -1, -1, 41
	scene_cue LemurianShipTiller, -1, -1, 41
	group_cue 12, -1, -1, 41
	scene_cue IsletsTurtle, -1, -1, 3
	scene_cue IsletsPier, -1, -1, 14
	scene_cue IsletsCow, -1, -1, 13
	scene_cue IsletsPrettyStone, -1, -1, 726
	group_cue 34, -1, -1, 4
	group_cue 39, -1, -1, 22
	group_cue 40, -1, -1, 16
	group_cue 41, -1, -1, 30
	group_cue 42, -1, -1, 30
	scene_cue ApojiiIslandsShops, 6, -1, 3
	group_cue 43, -1, -1, 5
	group_cue 44, -1, -1, 22
	group_cue 45, 5, -1, 79
	group_cue 45, -1, -1, 21
	group_cue 46, -1, -1, 21
	group_cue 47, -1, FLAG_SERPENT_SLAIN, 6
	group_cue 47, -1, -1, 7
	group_cue 48, -1, -1, 7
	group_cue 49, -1, -1, 24
	group_cue 50, -1, -1, 24
	scene_cue ChampaInteriors, 8, -1, 14
	scene_cue ChampaInteriors, 9, -1, 4
	group_cue 51, -1, -1, 3
	group_cue 52, -1, -1, 31
	group_cue 53, -1, -1, 65
	group_cue 54, -1, -1, 12
	scene_cue LemuriaSpring, -1, -1, 709
	scene_cue LemuriaPalace, -1, -1, 703
	group_cue 55, -1, -1, 12
	group_cue 56, -1, -1, 29
	group_cue 57, -1, -1, 701
	group_cue 58, -1, -1, 13
	group_cue 59, -1, -1, 14
	group_cue 60, -1, -1, 5
	group_cue 61, -1, -1, 27
	group_cue 62, -1, -1, 27
	scene_cue ContigoShops, 21, FLAG_REUNITED, 43
	scene_cue ContigoShops, 21, -1, 42
	group_cue 63, -1, FLAG_REUNITED, 43
	group_cue 63, -1, -1, 42
	group_cue 64, -1, -1, 721
	group_cue 65, -1, -1, 721
	group_cue 66, -1, -1, 23
	group_cue 67, -1, -1, 37
	group_cue 68, -1, FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT, 76
	group_cue 68, -1, -1, 721
	group_cue 69, -1, -1, 721
	scene_cue JupiterLighthouseRoomM, -1, FLAG_JUPITER_LIGHTHOUSE_FIGHT_WATCHED, 34
	group_cue 70, -1, -1, 26
	group_cue 71, -1, -1, 37
	group_cue 72, -1, -1, 41
	group_cue 73, -1, FLAG_MARS_BEACON_LIT, 726
	group_cue 73, -1, -1, 62
	group_cue 74, -1, -1, 23
	group_cue 75, -1, -1, 23
	group_cue 76, -1, -1, 23
	group_cue 77, -1, -1, 23
	scene_cue MarsLighthouseAerie, 2, -1, 0
	scene_cue MarsLighthouseAerie, 3, FLAG_MARS_BEACON_LIT, 28
	scene_cue MarsLighthouseAerie, 3, -1, 38
	scene_cue MarsLighthouseAerie, 5, -1, 72
	group_cue 78, -1, -1, 28
	group_cue 81, -1, -1, 72
	group_cue 79, -1, -1, 728
	scene_cue AnemosInnerSanctumDragonSkinChest, 1, -1, 722
	scene_cue AnemosInnerSanctumDragonSkinChest, 2, -1, 722
	scene_cue AnemosInnerSanctumDragonSkinChest, 99, -1, 722
	group_cue 80, -1, -1, 720
	group_cue 81, -1, -1, 79
	group_cue 3, -1, -1, 79
	group_cue 83, -1, -1, 79
	group_cue 82, -1, -1, 0
	group_cue 84, -1, -1, 709
	.2byte 0xffff, 0xffff, 0xffff, 0x0000	@ End.
