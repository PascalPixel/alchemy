	.section .rodata
@ Battle effect tables, in the order of the effect code that reads them. Each
@ is read only by the effect its prefix names, except the gBattleEffect_
@ tables, which many effects share. Picture offsets count bytes from the start
@ of a sheet; widths and heights are pixels. A mesh is a list of triangles,
@ each three vertex numbers and their texture coordinates; corners are signed
@ x, y and z bytes.
	.global gBattleEffect_MotePictures
gBattleEffect_MotePictures:
	.2byte 0, 2, 10, 28, 60, 110, 182, 280
	.2byte 408, 570
	.global gBattleEffect_FlarePictures
gBattleEffect_FlarePictures:
	.2byte 0, 4, 20, 56, 120, 220, 364, 560
	.2byte 816, 1140
	.global gCruelRuin_CirclePictures
gCruelRuin_CirclePictures:
	.2byte 0, 8, 40, 112, 240, 440, 728, 1120
	.2byte 1632, 2280
	.global gBattleEffect_SparklePictures
gBattleEffect_SparklePictures:
	.2byte 0, 25, 74, 123, 172, 221, 246, 255
	.2byte 256
	.global gBattleEffect_SparkleSizes
gBattleEffect_SparkleSizes:
	.byte 5, 7, 7, 7, 7, 5, 3, 1, 1
	.global gBattleEffect_PuffArcPictureWidths
gBattleEffect_PuffArcPictureWidths:
	.byte 24, 29, 30, 27, 24, 16
	.global gBattleEffect_PuffArcPictureHeights
gBattleEffect_PuffArcPictureHeights:
	.byte 23, 32, 37, 49, 53, 52
	.global gBattleEffect_PuffArcPictureBiasY
gBattleEffect_PuffArcPictureBiasY:
	.byte 41, 32, 26, 11, 4, 2
	.balign 2
	.global gBattleEffect_PuffArcPictures
gBattleEffect_PuffArcPictures:
	.2byte 0, 552, 1480, 2590, 3913, 5185
	.global gBattleEffect_GlintPictures
gBattleEffect_GlintPictures:
	.2byte 0, 589, 1302, 2015, 2414, 2750
	.global gBattleEffect_GlintPictureWidths
gBattleEffect_GlintPictureWidths:
	.byte 19, 23, 23, 19, 16, 17
	.global gBattleEffect_GlintPictureHeights
gBattleEffect_GlintPictureHeights:
	.byte 31, 31, 31, 21, 21, 24
	.global gSpire_ShardWidths
gSpire_ShardWidths:
	.byte 5, 11, 10, 7, 7, 12, 13, 9, 19, 12, 17, 22, 15, 16, 14
	.global gSpire_ShardHeights
gSpire_ShardHeights:
	.byte 11, 6, 10, 11, 10, 7, 17, 21, 12, 21, 17, 13, 16, 16, 15
	.global gSpire_ShardPictures
gSpire_ShardPictures:
	.2byte 0, 55, 121, 221, 298, 368, 452, 673
	.2byte 862, 1090, 1342, 1631, 1917, 2157, 2413, 2623
	.global gStatChange_ShardPictures
gStatChange_ShardPictures:
	.2byte 0, 120, 180, 258, 298, 352, 408, 471
	.2byte 567, 607, 631, 661
	.global gStatChange_ShardWidths
gStatChange_ShardWidths:
	.byte 10, 5, 6, 5, 6, 7, 9, 8, 4, 6, 6, 7
	.global gStatChange_ShardHeights
gStatChange_ShardHeights:
	.byte 12, 12, 13, 8, 9, 8, 7, 12, 10, 4, 5, 5
	.global gCastingImpact_PictureX
gCastingImpact_PictureX:
	.byte 10, 30, 0, 0, 0, 6, 52, 60, 0, 0, 0, 0, 0, 0
	.global gCastingImpact_PictureY
gCastingImpact_PictureY:
	.byte 0, 20, 2, 2, 34, 64, 68
	.global gFalling_IceCounts
gFalling_IceCounts:
	.byte 16, 46, 20, 52, 32, 80
	.global gFalling_Counts
gFalling_Counts:
	.byte 6, 8, 32, 80, 8, 16, 24, 80, 32, 4, 8, 80
	.global gStreaks_Styles
gStreaks_Styles:
	.byte 0, 1, 0, 0, 0, 1, 4, 0, 0, 1, -1, 1, 1, 1, -1, 1
	.byte 1, 1, 4, 0
	.global gStreaks_Tremble
gStreaks_Tremble:
	.byte -1, -1, 1, 1, 1, 1, -1, -1
	.global gTargetStrike_Styles
gTargetStrike_Styles:
	.byte 1, 2, 8, 0, 6, 0, 70, 1, 1, 1, 1, 6, 1, 54, 1, 3
	.byte 1, 2, 6, 1, 96, 1, 3, 2, 3, 4, 1, 96, 1, 3, 2, 4
	.byte 4, 1, 96, 1, 3, 4, 5, 4, 0, 60, 0, 1, 4, 5, 4, 0
	.byte 38, 0, 2, 8, 5, 2, 0, 48, 0, 1, 24, 4, 2, 0, 50, 0
	.byte 1, 1, 1, 2, 0, 46, 1, 1, 1, 4, 6, 1, 54, 1, 3, 2
	.byte 4, 4, 0, 96, 0, 4, 8, 5, 2, 1, 64, 0, 8, 8, 5, 1
	.byte 0, 64, 0, 1, 44, 6, 2, 0, 50, 1, 3, 2, 4, 4, 0, 96
	.byte 0, 1, 40, 6, 2, 1, 50, 1, 1, 24, 4, 6, 0, 50, 1, 1
	.byte 40, 6, 2, 1, 50, 0, 1, 64, 4, 2, 0, 22, 1, 1, 8, 0
	.byte 6, 0, 70
	.global gTargetStrike_Widths
gTargetStrike_Widths:
	.byte 38, 54, 71, 74, 77, 65
	.global gTargetStrike_Heights
gTargetStrike_Heights:
	.byte 7, 9, 11, 13, 13, 11
	.global gTargetStrike_Pictures
gTargetStrike_Pictures:
	.2byte 3200, 3466, 3952, 4733, 5695, 6696
	.global gTargetStrike_Reach
gTargetStrike_Reach:
	.byte 58, 42, 25, 13, 5, 0
	.global gTargetStrike_BiteStages
gTargetStrike_BiteStages:
	.byte 0, 1, 2, 3, 2
	.global gTargetStrike_GlintDrawFlags
gTargetStrike_GlintDrawFlags:
	.byte 0, 4, 8, 12
	.global gDeathPlunge_PieceWidths
gDeathPlunge_PieceWidths:
	.byte 21, 20, 24, 30, 35, 69, 19, 23, 9, 22, 33, 10, 18, 9, 7, 9
	.byte 17, 55, 30, 25, 40, 29, 10, 28, 14, 5, 32, 23, 19, 32, 8, 43
	.byte 15
	.global gDeathPlunge_PieceHeights
gDeathPlunge_PieceHeights:
	.byte 23, 47, 25, 40, 20, 53, 28, 29, 23, 14, 35, 17, 18, 14, 15, 12
	.byte 13, 37, 38, 41, 60, 41, 15, 45, 59, 41, 26, 50, 21, 35, 12, 57
	.byte 11, 0
	.global gDeathPlunge_PiecePictures
gDeathPlunge_PiecePictures:
	.2byte 0, 483, 1423, 2023, 3223, 3923, 7580, 8112, 8779, 8986, 9294
	.2byte 10449, 10619, 10943, 11069, 11174, 11282, 11503, 13538, 14678, 15703, 18103
	.2byte 19292, 19442, 20702, 21528, 21733, 22565, 23715, 24114, 25234, 25330, 27781
	.2byte 27946
	.global gDeathPlunge_PieceStartX
gDeathPlunge_PieceStartX:
	.byte 0, 21, 28, 46, 70, 51, 0, 13, 37, 77, 87, 19, 25, 35, 44, 45
	.byte 48, 65, 0, 0, 0, 5, 33, 15, 36, 46, 47, 50, 53, 65, 72, 77
	.byte 105
	.global gDeathPlunge_PieceStartY
gDeathPlunge_PieceStartY:
	.byte 0, 0, 0, 0, 0, 0, 19, 17, 23, 28, 8, 36, 43, 43, 40, 47
	.byte 48, 37, 42, 67, 60, 79, 62, 75, 61, 62, 60, 70, 74, 85, 108, 63
	.byte 74
	.global gDeathPlunge_X
gDeathPlunge_X:
	.byte 0, 32, 0, 32
	.global gDeathPlunge_Y
gDeathPlunge_Y:
	.byte 0, 0, 32, 32
	.global gElementBurst_GlintFlips
gElementBurst_GlintFlips:
	.byte 0, 4, 8, 12
	.balign 4
	.global gCruelRuin_StreakCorners
gCruelRuin_StreakCorners:
	.4byte 32764, 32516, 250, 6
	.global gCruelRuin_DiscPitches
gCruelRuin_DiscPitches:
	.2byte 10240, 56579, 56579, 58880
	.global gCruelRuin_DiscYaws
gCruelRuin_DiscYaws:
	.2byte 39680, 42496, 32768, 22016
	.global gCruelRuin_PillarSways
gCruelRuin_PillarSways:
	.byte 24, 48
	.global gCruelRuin_PillarWidths
gCruelRuin_PillarWidths:
	.byte 24, 32
	.global gCruelRuin_BlastFrames
gCruelRuin_BlastFrames:
	.byte 7, 37, 45, 58, 74, 80
	.global gCruelRuin_BlastSizes
gCruelRuin_BlastSizes:
	.byte 96, 48, 64, 64, 96, 128
	.global gCruelRuin_BlastX
gCruelRuin_BlastX:
	.byte 48, 48, 32, 0, 0, 24
	.global gCruelRuin_BlastY
gCruelRuin_BlastY:
	.byte 96, 96, 96, 96, 120, 120
	.global gCruelRuin_StreakStarts
gCruelRuin_StreakStarts:
	.byte 2, 26, 104, 150
	.global gCruelRuin_StreakSizes
gCruelRuin_StreakSizes:
	.byte 32, 16, 24, 64
	.global gCruelRuin_StreakX
gCruelRuin_StreakX:
	.byte 96, 48, 24, 16
	.global gCruelRuin_StreakStepX
gCruelRuin_StreakStepX:
	.byte 16, 8, 12, 8
	.global gCruelRuin_StreakStepY
gCruelRuin_StreakStepY:
	.byte 16, 4, 16, 24
	.global gCruelRuin_StreakEndY
gCruelRuin_StreakEndY:
	.byte 96, 96, 96, 120
	.global gCruelRuin_StreakRolls
gCruelRuin_StreakRolls:
	.4byte 0x9000, 0xa000, 0x9000, 0x9000
	.global gEclipse_GlintFlips
gEclipse_GlintFlips:
	.byte 0, 4, 8, 12
	.global gEclipse_TileObjects
gEclipse_TileObjects:
	.byte 8, 9, 25, 14, 15, 20, 21
	.global gEclipse_FillFrames
gEclipse_FillFrames:
	.byte 148, 160, 162, 164, 170, 174, 178, 182, 188, 194, 196, 200, 202, 206, 208, 210
	.byte 220
	.global gEclipse_FillLevels
gEclipse_FillLevels:
	.4byte 404232216, 269488144, 269488144, 404232216
	.4byte 741092396, 741092396, 741092396, 741092396
	.4byte 741092396, 741092396, 269488144, 269488144
	.4byte 269488144, 741092396, 741092396, 269488144
	.4byte 269488144
	.global gEclipse_ObjectX
gEclipse_ObjectX:
	.byte 0, 32, 64, 96, 160, 192, 0, 32, 64, 96, 128, 160, 192, 224, 64, 96
	.byte 128, 160, 192, 224, 64, 96, 128, 160, 192, 224, 64, 96, 128, 160, 192, 224
	.global gEclipse_ObjectZ
gEclipse_ObjectZ:
	.byte 0, 0, 0, 0, 0, 0, 32, 32, 32, 32, 32, 32, 32, 32, 64, 64
	.byte 64, 64, 64, 64, 96, 96, 96, 96, 96, 96, 128, 128, 128, 128, 128, 128
	.global gEclipse_RingGrowth
gEclipse_RingGrowth:
	.byte 4, 3, 5, 3
	.global gEclipse_RingHeights
gEclipse_RingHeights:
	.byte 16, 12, 4, -8
	.global gFormChange_WaveStarts
gFormChange_WaveStarts:
	.byte 0, 4, 9, 10
	.global gFormChange_WaveGrowth
gFormChange_WaveGrowth:
	.byte 3, 4, 3, 5
	.global gFormChange_WaveStretch
gFormChange_WaveStretch:
	.byte 3, 4, 5, 6
	.global gPrism_ShardStarts
gPrism_ShardStarts:
	.byte 32, 60, 33, 82, 49, 94, 41, 89, 49, 76, 32, 99, 54, 69, 34, 82
	.byte 45, 71, 31, 91, 32, 69, 26, 80, 40, 68, 41, 60, 46, 60, 36, 63
	.byte 0, 0
	.global gPrism_ShardWidths
gPrism_ShardWidths:
	.byte 5, 10, 11, 14, 12, 11, 19, 18, 23, 12, 22, 22
	.global gPrism_ShardHeights
gPrism_ShardHeights:
	.byte 14, 10, 7, 13, 14, 14, 32, 30, 24, 38, 20, 20, 0, 0
	.global gPrism_ShardPictures
gPrism_ShardPictures:
	.4byte 0, 70, 170, 247, 429, 597
	.4byte 751, 1359, 1899, 2451, 2907, 3347
	.global gGatheringLights_Counts
gGatheringLights_Counts:
	.byte 16, 88, 8, 72, 12, 80, 16, 88
	.global gDjinnBeams_MotePictures
gDjinnBeams_MotePictures:
	.byte 2, 1, 0, 1, 2, 3, 4, 3
	.global gMirroredWall_Pictures
gMirroredWall_Pictures:
	.2byte 2702, 3535, 4580
	.global gDjinnBeams_HeadWidths
gDjinnBeams_HeadWidths:
	.byte 17, 19, 20
	.global gDjinnBeams_HeadHeights
gDjinnBeams_HeadHeights:
	.byte 49, 55, 64
	.global gLobbedShells_Styles
gLobbedShells_Styles:
	.byte 0, 0, 1, 72, 0, 0, 1, 72, 0, 0, 1, 72, 1, 0, 1, 40
	.byte 1, 0, 1, 56, 0, 0, 1, 56, 0, 0, 1, 72, 1, 0, 4, 52
	.byte 0, 1, 1, 72, 0, 0, 1, 72
	.global gLobbedShells_ShardWidths
gLobbedShells_ShardWidths:
	.byte 9, 10, 9, 9, 9, 11, 11, 11, 12, 6, 7, 7, 7, 6, 6, 8
	.byte 8, 8
	.global gLobbedShells_ShardHeights
gLobbedShells_ShardHeights:
	.byte 12, 12, 12, 12, 12, 12, 12, 12, 12, 8, 7, 6, 6, 8, 6, 4
	.byte 3, 3
	.global gLobbedShells_ShardPictures
gLobbedShells_ShardPictures:
	.2byte 0, 108, 228, 336, 444, 552, 684, 816
	.2byte 948, 1092, 1140, 1189, 1231, 1273, 1321, 1357
	.2byte 1389, 1413
	.global gCondemn_PictureX
gCondemn_PictureX:
	.byte 10, 30, 0, 0, 0, 6, 52, 60, 0, 0, 0, 0, 0, 0
	.global gCondemn_PictureY
gCondemn_PictureY:
	.byte 0, 20, 2, 2, 34, 64, 68, 0, 9, 4, 4, 4, 4, 4
	.global gBind_Vertex
gBind_Vertex:
	.4byte 0, 24, 0
	.global gBind_TriangleStrikeVertex
gBind_TriangleStrikeVertex:
	.4byte 0, 24, 0
	.global gStar_Points
gStar_Points:
	.4byte 0, 0x300000, 0, 0
	.4byte 0x1c0000, 0
	.global gStar_Radii
gStar_Radii:
	.byte 32, 16
	.global gProcne_GridX
gProcne_GridX:
	.byte 0, 32, 64, 0, 32, 64, 0, 32, 64
	.global gProcne_GridY
gProcne_GridY:
	.byte 0, 0, 0, 32, 32, 32, 64, 64, 64
	.global gProcne_Frames
gProcne_Frames:
	.2byte 334, 347, 359, 372
	.global gProcne_PictureWidths
gProcne_PictureWidths:
	.byte 14, 24, 48
	.global gProcne_PictureHeights
gProcne_PictureHeights:
	.byte 14, 19, 24
	.global gProcne_PictureBiasY
gProcne_PictureBiasY:
	.byte 8, 12, 17
	.balign 2
	.global gProcne_Pictures
gProcne_Pictures:
	.2byte 13824, 14020, 14476
	.global gTiamat_EmberSpeeds
gTiamat_EmberSpeeds:
	.byte 0, 8, 12, 16
	.global gTiamat_SparkPixels
gTiamat_SparkPixels:
	.byte 32, 32
	.global gTiamat_EmberSizes
gTiamat_EmberSizes:
	.byte 1, 1, 2, 3
	.global gTiamat_FlameWidths
gTiamat_FlameWidths:
	.byte 24, 29, 30, 27, 24, 16
	.global gTiamat_FlameHeights
gTiamat_FlameHeights:
	.byte 23, 32, 37, 49, 53, 52
	.global gTiamat_FlameDrops
gTiamat_FlameDrops:
	.byte 41, 32, 26, 11, 4, 2
	.global gTiamat_FlamePictures
gTiamat_FlamePictures:
	.2byte 0, 552, 1480, 2590, 3913, 5185
	.global gNereid_Columns
gNereid_Columns:
	.byte 16, 104, 32, 104, 48, 104, 64, 104
	.global gNereid_Gravity
gNereid_Gravity:
	.4byte 0x8000, 0x4000, 0x4000, 0x2000
	.global gVortices_Counts
gVortices_Counts:
	.byte 6, 140, 12, 160, 30, 200
	.global gVortices_Shapes
gVortices_Shapes:
	.byte 1, 16, 2, 2, 24, 2, 4, 32, 4
	.global gPrism_Counts
gPrism_Counts:
	.byte 1, 85, 30, 4, 124, 54, 12, 166, 86
	.global gPlasma_Sparks
gPlasma_Sparks:
	.2byte 384, 2, 256, 3, 192, 4
	.global gPlasma_Columns
gPlasma_Columns:
	.byte 32, 64, 16, 48, 72, 96, 64, 88, 56, 104, 72, 48, 24, 56
	.global gPlasma_Counts
gPlasma_Counts:
	.byte 1, 3, 7
	.balign 2
	.global gPlasma_Shapes
gPlasma_Shapes:
	.2byte 128, 1, 16, 0, 0, 255, 1, 16
	.2byte 0, 0, 192, 3, 0, 24, 48, 340
	.2byte 1, 16, 24, 48, 32, 1, 16, 0
	.2byte 0, 200, 1, 16, 24, 48
	.global gPlasma_FlashPictures
gPlasma_FlashPictures:
	.byte 5, 5, 4, 3, 2, 1
	.global gVolcano_Counts
gVolcano_Counts:
	.byte 1, 2, 4
	.global gVolcano_Depths
gVolcano_Depths:
	.byte 0, 0, 0, 0, 24, -24, 0, 0, -20, 20, -60, 60
	.global gVolcano_StartFrames
gVolcano_StartFrames:
	.byte 8, 18, 24, 30, 34
	.global gTargetMotes_GlintFlips
gTargetMotes_GlintFlips:
	.byte 0, 4, 8, 12
	.global gZagan_ChargePoints
gZagan_ChargePoints:
	.2byte 0, 0, 0, 32, 32, 0, 32, 32
	.2byte 32, 64, 64, 0, 64, 32, 64, 64
	.2byte 96, 0, 96, 32, 96, 64
	.global gZagan_GallopPoints
gZagan_GallopPoints:
	.2byte 0, 16, 0, 48, 32, 0, 32, 32
	.2byte 32, 64, 64, 0, 64, 32, 64, 64
	.2byte 64, 96, 96, 0, 96, 64, 96, 96
	.global gZagan_RingStarts
gZagan_RingStarts:
	.byte 0, 6, 8, 9
	.global gZagan_RingGrowth
gZagan_RingGrowth:
	.byte 5, 8, 7, 3
	.global gZagan_RingHeights
gZagan_RingHeights:
	.byte 32, 12, -24, 0
@ One handler for each battle effect, by effect less one; 0 where none.
	.global gBattleEffect_Handlers
gBattleEffect_Handlers:
	.4byte BattleEffect_RunQuake               @ 1
	.4byte BattleEffect_RunSpire               @ 2
	.4byte BattleEffect_RunGaia                @ 3
	.4byte BattleEffect_RunGrowth              @ 4
	.4byte BattleEffect_RunThorn               @ 5
	.4byte BattleEffect_RunFrost               @ 6
	.4byte BattleEffect_RunIce                 @ 7
	.4byte BattleEffect_RunPrism               @ 8
	.4byte BattleEffect_RunDouse               @ 9
	.4byte BattleEffect_RunFroth               @ 10
	.4byte BattleEffect_RunFlare               @ 11
	.4byte BattleEffect_RunFire                @ 12
	.4byte BattleEffect_RunVolcano             @ 13
	.4byte BattleEffect_RunBlast               @ 14
	.4byte BattleEffect_RunNova                @ 15
	.4byte BattleEffect_RunBolt                @ 16
	.4byte BattleEffect_RunRay                 @ 17
	.4byte BattleEffect_RunPlasma              @ 18
	.4byte BattleEffect_RunSlash               @ 19
	.4byte BattleEffect_RunWhirlwind           @ 20
	.4byte BattleEffect_RunCure                @ 21
	.4byte BattleEffect_RunPly                 @ 22
	.4byte BattleEffect_RunWish                @ 23
	.4byte BattleEffect_RunCurePoison          @ 24
	.4byte BattleEffect_RunRevive              @ 25
	.4byte BattleEffect_RunImpact              @ 26
	.4byte BattleEffect_RunDull         @ 27
	.4byte BattleEffect_RunGuard               @ 28
	.4byte BattleEffect_RunImpair         @ 29
	.4byte BattleEffect_RunWard                @ 30
	.4byte BattleEffect_RunWeaken         @ 31
	.4byte BattleEffect_RunTaint               @ 32
	.4byte BattleEffect_RunDelude              @ 33
	.4byte BattleEffect_RunUnusedOrbitingSpirals     @ 34
	.4byte BattleEffect_RunHeartrender         @ 35
	.4byte BattleEffect_RunParalyze            @ 36
	.4byte BattleEffect_RunSleep               @ 37
	.4byte BattleEffect_RunBind                @ 38
	.4byte BattleEffect_RunHaunt               @ 39
	.4byte BattleEffect_RunCondemn             @ 40
	.4byte BattleEffect_RunCurse               @ 41
	.4byte BattleEffect_RunDrain               @ 42
	.4byte BattleEffect_RunBreak               @ 43
	.4byte BattleEffect_RunWish                @ 44
	.4byte BattleEffect_RunWish                @ 45
	.4byte BattleEffect_RunImpact              @ 46
	.4byte BattleEffect_RunImpair         @ 47
	.4byte 0                               @ 48
	.4byte BattleEffect_RunDragonCloud         @ 49
	.4byte BattleEffect_RunDemonNight          @ 50
	.4byte BattleEffect_RunHelmSplitter        @ 51
	.4byte BattleEffect_RunQuickStrike         @ 52
	.4byte BattleEffect_RunDeathPlunge         @ 53
	.4byte BattleEffect_RunShuriken            @ 54
	.4byte BattleEffect_RunAnnihilation        @ 55
	.4byte BattleEffect_RunPunji               @ 56
	.4byte BattleEffect_RunCuttingEdge         @ 57
	.4byte BattleEffect_RunRagnarok            @ 58
	.4byte BattleEffect_RunHeatWave            @ 59
	.4byte BattleEffect_RunPlanetDiver         @ 60
	.4byte BattleEffect_RunTitanBlade          @ 61
	.4byte BattleEffect_RunShiningStar         @ 62
	.4byte BattleEffect_RunBlizzard            @ 63
	.4byte BattleEffect_RunDemonFire           @ 64
	.4byte BattleEffect_RunVorpalSlash         @ 65
	.4byte BattleEffect_RunDrown               @ 66
	.4byte BattleEffect_RunUnusedTriangleStrike      @ 67
	.4byte BattleEffect_RunAstralBlast         @ 68
	.4byte BattleEffect_RunHeartbreak          @ 69
	.4byte 0                               @ 70
	.4byte BattleEffect_RunAura  @ 71
	.4byte BattleEffect_RunFume                @ 72
	.4byte BattleEffect_RunBeam                @ 73
	.4byte BattleEffect_RunRagingHeat          @ 74
	.4byte BattleEffect_RunCool                @ 75
	.4byte BattleEffect_RunSabreDance          @ 76
	.4byte BattleEffect_RunBackstab            @ 77
	.4byte BattleEffect_RunJuggle              @ 78
	.4byte BattleEffect_RunFlameCard           @ 79
	.4byte BattleEffect_RunWhiplash            @ 80
	.4byte BattleEffect_RunPoisonFlow          @ 81
	.4byte BattleEffect_RunFirePuppet          @ 82
	.4byte BattleEffect_RunDiamondDust @ 83
	.4byte BattleEffect_RunDiamondBerg         @ 84
	.4byte BattleEffect_RunPlumeEdge           @ 85
	.4byte BattleEffect_RunThunderMine         @ 86
	.4byte BattleEffect_RunFrostCard           @ 87
	.4byte BattleEffect_RunBrambleCard         @ 88
	.4byte BattleEffect_RunThunderCard         @ 89
	.4byte BattleEffect_RunBaffleCard          @ 90
	.4byte BattleEffect_RunSwordCard           @ 91
	.4byte BattleEffect_RunSleepCard           @ 92
	.4byte BattleEffect_RunDeathCard           @ 93
	.4byte 0                               @ 94
	.4byte 0                               @ 95
	.4byte 0                               @ 96
	.4byte 0                               @ 97
	.4byte 0                               @ 98
	.4byte 0                               @ 99
	.4byte BattleEffect_RunStoneJustice        @ 100
	.4byte BattleEffect_RunHeavyDivide         @ 101
	.4byte 0                               @ 102
	.4byte BattleEffect_RunWyrdCurse           @ 103
	.4byte BattleEffect_RunAcheronsGrief       @ 104
	.4byte BattleEffect_RunHurricane           @ 105
	.4byte BattleEffect_RunDreamtide           @ 106
	.4byte BattleEffect_RunLetheAlbion         @ 107
	.4byte BattleEffect_RunRisingDragon        @ 108
	.4byte BattleEffect_RunBoostHack           @ 109
	.4byte BattleEffect_RunRadiantFire         @ 110
	.4byte BattleEffect_RunFireDance @ 111
	.4byte BattleEffect_RunBlazeRush @ 112
	.4byte BattleEffect_RunShred         @ 113
	.4byte BattleEffect_RunPurgatory           @ 114
	.4byte BattleEffect_RunSoulShatter         @ 115
	.4byte BattleEffect_RunAgingGas @ 116
	.4byte BattleEffect_RunMadZephyr           @ 117
	.4byte BattleEffect_RunLunarSlash          @ 118
	.4byte BattleEffect_RunNirvana             @ 119
	.4byte BattleEffect_RunLightSurge          @ 120
	.4byte BattleEffect_RunVoidBeam            @ 121
	.4byte BattleEffect_RunLegend              @ 122
	.4byte BattleEffect_RunLiquifier           @ 123
	.4byte BattleEffect_RunMegiddo             @ 124
	.4byte BattleEffect_RunOdyssey             @ 125
	.4byte BattleEffect_RunVengeance           @ 126
	.4byte BattleEffect_RunThornyGrave         @ 127
	.4byte BattleEffect_RunPlanetary           @ 128
	.4byte BattleEffect_RunLifeShear           @ 129
	.4byte 0                               @ 130
	.4byte 0                               @ 131
	.4byte 0                               @ 132
	.4byte 0                               @ 133
	.4byte 0                               @ 134
	.4byte 0                               @ 135
	.4byte 0                               @ 136
	.4byte 0                               @ 137
	.4byte 0                               @ 138
	.4byte 0                               @ 139
	.4byte 0                               @ 140
	.4byte 0                               @ 141
	.4byte 0                               @ 142
	.4byte 0                               @ 143
	.4byte 0                               @ 144
	.4byte 0                               @ 145
	.4byte 0                               @ 146
	.4byte 0                               @ 147
	.4byte 0                               @ 148
	.4byte 0                               @ 149
	.4byte 0                               @ 150
	.4byte 0                               @ 151
	.4byte 0                               @ 152
	.4byte 0                               @ 153
	.4byte 0                               @ 154
	.4byte 0                               @ 155
	.4byte 0                               @ 156
	.4byte 0                               @ 157
	.4byte 0                               @ 158
	.4byte 0                               @ 159
	.4byte 0                               @ 160
	.4byte 0                               @ 161
	.4byte 0                               @ 162
	.4byte 0                               @ 163
	.4byte 0                               @ 164
	.4byte 0                               @ 165
	.4byte 0                               @ 166
	.4byte 0                               @ 167
	.4byte 0                               @ 168
	.4byte 0                               @ 169
	.4byte 0                               @ 170
	.4byte 0                               @ 171
	.4byte 0                               @ 172
	.4byte 0                               @ 173
	.4byte 0                               @ 174
	.4byte 0                               @ 175
	.4byte 0                               @ 176
	.4byte 0                               @ 177
	.4byte 0                               @ 178
	.4byte 0                               @ 179
	.4byte 0                               @ 180
	.4byte BattleEffect_RunCrucibleFire                   @ 181
	.4byte BattleEffect_RunReverseStar  @ 182
	.4byte 0                               @ 183
	.4byte 0                               @ 184
	.4byte 0                               @ 185
	.4byte 0                               @ 186
	.4byte 0                               @ 187
	.4byte 0                               @ 188
	.4byte 0                               @ 189
	.4byte 0                               @ 190
	.4byte 0                               @ 191
	.4byte 0                               @ 192
	.4byte 0                               @ 193
	.4byte 0                               @ 194
	.4byte 0                               @ 195
	.4byte 0                               @ 196
	.4byte 0                               @ 197
	.4byte 0                               @ 198
	.4byte 0                               @ 199
	.4byte BattleEffect_RunGranite             @ 200
	.4byte BattleEffect_RunRevive              @ 201
	.4byte BattleEffect_RunVine                @ 202
	.4byte BattleEffect_RunGround              @ 203
	.4byte BattleEffect_RunPly                 @ 204
	.4byte BattleEffect_RunSleet               @ 205
	.4byte BattleEffect_RunWish                @ 206
	.4byte BattleEffect_RunHail                @ 207
	.4byte BattleEffect_RunTonic               @ 208
	.4byte BattleEffect_RunDew                 @ 209
	.4byte BattleEffect_RunImpact              @ 210
	.4byte BattleEffect_RunGuard               @ 211
	.4byte BattleEffect_RunPsyCrystal          @ 212
	.4byte BattleEffect_RunFlash               @ 213
	.4byte BattleEffect_RunTorch               @ 214
	.4byte BattleEffect_RunWard                @ 215
	.4byte BattleEffect_RunZephyr              @ 216
	.4byte BattleEffect_RunKite                @ 217
	.4byte BattleEffect_RunBind                @ 218
	.4byte BattleEffect_RunGust                @ 219
	.4byte BattleEffect_RunIron                @ 220
	.4byte BattleEffect_RunSteam               @ 221
	.4byte BattleEffect_RunShade               @ 222
	.4byte BattleEffect_RunImpact              @ 223
	.4byte BattleEffect_RunCoal                @ 224
	.4byte BattleEffect_RunCure                @ 225
	.4byte BattleEffect_RunSalt   @ 226
	.4byte BattleEffect_RunSpring   @ 227
	.4byte BattleEffect_RunBreath   @ 228
	.4byte BattleEffect_RunEther  @ 229
	.4byte BattleEffect_RunBalm                @ 230
	.4byte BattleEffect_RunTinder              @ 231
	.4byte BattleEffect_RunWhorl               @ 232
	.4byte BattleEffect_RunGale                @ 233
	.4byte BattleEffect_RunWaft                @ 234
	.4byte 0                               @ 235
	.4byte BattleEffect_RunCore                @ 236
	.4byte BattleEffect_RunMud                 @ 237
	.4byte BattleEffect_RunHaze                @ 238
	.4byte BattleEffect_RunPetra        @ 239
	.4byte BattleEffect_RunRime                @ 240
	.4byte BattleEffect_RunLull                @ 241
	.4byte BattleEffect_RunGasp                @ 242
	.4byte BattleEffect_RunFugue        @ 243
	.4byte BattleEffect_RunCrystal             @ 244
	.4byte BattleEffect_RunEddy  @ 245
	.4byte BattleEffect_RunCannon              @ 246
	.4byte BattleEffect_RunAroma               @ 247
	.4byte BattleEffect_RunReflux              @ 248
	.4byte BattleEffect_RunDjinnCounter        @ 249
	.4byte BattleEffect_RunMold                @ 250
	.4byte BattleEffect_RunMeld                @ 251
	.4byte BattleEffect_RunShine               @ 252
	.4byte BattleEffect_RunGeode               @ 253
	.4byte BattleEffect_RunSerac               @ 254
	.4byte BattleEffect_RunSpark               @ 255
	.4byte BattleEffect_RunFury       @ 256
	.4byte BattleEffect_RunChar       @ 257
	.4byte 0                               @ 258
	.4byte 0                               @ 259
	.4byte 0                               @ 260
	.4byte 0                               @ 261
	.4byte 0                               @ 262
	.4byte 0                               @ 263
	.4byte 0                               @ 264
	.4byte 0                               @ 265
	.4byte 0                               @ 266
	.4byte 0                               @ 267
	.4byte 0                               @ 268
	.4byte 0                               @ 269
	.4byte 0                               @ 270
	.4byte 0                               @ 271
	.4byte 0                               @ 272
	.4byte 0                               @ 273
	.4byte 0                               @ 274
	.4byte 0                               @ 275
	.4byte 0                               @ 276
	.4byte 0                               @ 277
	.4byte 0                               @ 278
	.4byte 0                               @ 279
	.4byte BattleEffect_RunVenus               @ 280
	.4byte BattleEffect_RunMercury             @ 281
	.4byte BattleEffect_RunMars                @ 282
	.4byte BattleEffect_RunJupiter             @ 283
	.4byte 0                               @ 284
	.4byte 0                               @ 285
	.4byte 0                               @ 286
	.4byte 0                               @ 287
	.4byte 0                               @ 288
	.4byte 0                               @ 289
	.4byte 0                               @ 290
	.4byte 0                               @ 291
	.4byte 0                               @ 292
	.4byte 0                               @ 293
	.4byte 0                               @ 294
	.4byte 0                               @ 295
	.4byte 0                               @ 296
	.4byte 0                               @ 297
	.4byte 0                               @ 298
	.4byte 0                               @ 299
	.4byte BattleEffect_RunSonicWave           @ 300
	.4byte BattleEffect_RunShriek              @ 301
	.4byte BattleEffect_RunBansheeHowl         @ 302
	.4byte BattleEffect_RunCrazyVoice          @ 303
	.4byte BattleEffect_RunWarCry              @ 304
	.4byte BattleEffect_RunWickedHowl          @ 305
	.4byte BattleEffect_RunWingBeat            @ 306
	.4byte BattleEffect_RunWingFlutter         @ 307
	.4byte BattleEffect_RunRumble              @ 308
	.4byte BattleEffect_RunBoneChiller         @ 309
	.4byte BattleEffect_RunGust                @ 310
	.4byte BattleEffect_RunBoneCharge          @ 311
	.4byte BattleEffect_RunMysticFlame         @ 312
	.4byte BattleEffect_RunNumbingSting        @ 313
	.4byte BattleEffect_RunBruteForce          @ 314
	.4byte BattleEffect_RunStickyGoo           @ 315
	.4byte BattleEffect_RunCannibalFang        @ 316
	.4byte BattleEffect_RunBearClaw            @ 317
	.4byte BattleEffect_RunPoisonousBite       @ 318
	.4byte BattleEffect_RunFlyingAttack        @ 319
	.4byte BattleEffect_RunUndeadSword         @ 320
	.4byte BattleEffect_RunRansack             @ 321
	.4byte BattleEffect_RunStickyPoison        @ 322
	.4byte BattleEffect_RunPoisonousBite       @ 323
	.4byte BattleEffect_RunElectricBite        @ 324
	.4byte BattleEffect_RunPoisonTail          @ 325
	.4byte BattleEffect_RunOnslaught           @ 326
	.4byte BattleEffect_RunVampiricFang        @ 327
	.4byte BattleEffect_RunBacteriaRush        @ 328
	.4byte BattleEffect_RunFlyingAttack        @ 329
	.4byte BattleEffect_RunRottenBlood         @ 330
	.4byte 0                               @ 331
	.4byte BattleEffect_RunDoubleFang          @ 332
	.4byte BattleEffect_RunMortalBlow          @ 333
	.4byte BattleEffect_RunFreebiteRush        @ 334
	.4byte BattleEffect_RunTwinBeaks           @ 335
	.4byte BattleEffect_RunRabidFang           @ 336
	.4byte BattleEffect_RunAcidBite            @ 337
	.4byte BattleEffect_RunDynamite            @ 338
	.4byte BattleEffect_RunHeadbutt            @ 339
	.4byte BattleEffect_RunPoisonInk           @ 340
	.4byte 0                               @ 341
	.4byte BattleEffect_RunBearClaw            @ 342
	.4byte BattleEffect_RunRansack             @ 343
	.4byte BattleEffect_RunSoothingStar        @ 344
	.4byte BattleEffect_RunSpiderWeb           @ 345
	.4byte BattleEffect_RunHeartrender         @ 346
	.4byte BattleEffect_RunMadSpatter          @ 347
	.4byte BattleEffect_RunSpasm               @ 348
	.4byte BattleEffect_RunSleepStar           @ 349
	.4byte BattleEffect_RunDecompose           @ 350
	.4byte BattleEffect_RunHaunting            @ 351
	.4byte BattleEffect_RunCure                @ 352
	.4byte BattleEffect_RunBerserk             @ 353
	.4byte BattleEffect_RunCurse               @ 354
	.4byte 0                               @ 355
	.4byte 0                               @ 356
	.4byte 0                               @ 357
	.4byte 0                               @ 358
	.4byte 0                               @ 359
	.4byte 0                               @ 360
	.4byte 0                               @ 361
	.4byte 0                               @ 362
	.4byte 0                               @ 363
	.4byte 0                               @ 364
	.4byte 0                               @ 365
	.4byte 0                               @ 366
	.4byte 0                               @ 367
	.4byte 0                               @ 368
	.4byte 0                               @ 369
	.4byte 0                               @ 370
	.4byte 0                               @ 371
	.4byte 0                               @ 372
	.4byte 0                               @ 373
	.4byte 0                               @ 374
	.4byte 0                               @ 375
	.4byte 0                               @ 376
	.4byte 0                               @ 377
	.4byte 0                               @ 378
	.4byte 0                               @ 379
	.4byte BattleEffect_RunSmokeBomb           @ 380
	.4byte BattleEffect_RunWingStroke          @ 381
	.4byte BattleEffect_RunPoisonBeat          @ 382
	.4byte BattleEffect_RunSpinningBeat        @ 383
	.4byte BattleEffect_RunOuterSpace          @ 384
	.4byte BattleEffect_RunDragonDriver        @ 385
	.4byte BattleEffect_RunDrainFang           @ 386
	.4byte BattleEffect_RunSevereBlow @ 387
	.4byte BattleEffect_RunThrash              @ 388
	.4byte BattleEffect_RunRecovery            @ 389
	.4byte BattleEffect_RunEchoCut             @ 390
	.4byte BattleEffect_RunWateryGrave         @ 391
	.4byte BattleEffect_RunRagingFlood         @ 392
	.4byte BattleEffect_RunHydroSlash @ 393
	.4byte BattleEffect_RunMightyPress  @ 394
	.4byte BattleEffect_RunSlaver              @ 395
	.4byte BattleEffect_RunCounterRush         @ 396
	.4byte BattleEffect_RunDjinnfest       @ 397
	.4byte BattleEffect_RunHeatKiss       @ 398
	.4byte BattleEffect_RunAngleSpike          @ 399
	.4byte BattleEffect_RunFireBreath        @ 400
	.4byte BattleEffect_RunWaterBreath        @ 401
	.4byte BattleEffect_RunIceBlessing         @ 402
	.4byte BattleEffect_RunDarkBreath        @ 403
	.4byte BattleEffect_RunAcidBreath        @ 404
	.4byte BattleEffect_RunStormBreath        @ 405
	.4byte BattleEffect_RunEvilBlessing        @ 406
	.4byte BattleEffect_RunDeadlyGas        @ 407
	.4byte BattleEffect_RunSandBreath       @ 408
	.4byte BattleEffect_RunDesertGasp       @ 409
	.4byte BattleEffect_RunBlackIce       @ 410
	.4byte BattleEffect_RunTyphoonBlow       @ 411
	.4byte BattleEffect_RunToxicBreath       @ 412
	.4byte BattleEffect_RunGravelBlow       @ 413
	.4byte BattleEffect_RunBlastBreath         @ 414
	.4byte BattleEffect_RunDarksolGasp       @ 415
	.4byte BattleEffect_RunFireBreath        @ 416
	.4byte 0                               @ 417
	.4byte 0                               @ 418
	.4byte 0                               @ 419
	.4byte 0                               @ 420
	.4byte BattleEffect_RunManeater            @ 421
	.4byte BattleEffect_RunUrFlash    @ 422
	.4byte BattleEffect_RunCrusherGrip   @ 423
	.4byte BattleEffect_RunVanishClaw   @ 424
	.4byte BattleEffect_RunTwinShear           @ 425
	.4byte BattleEffect_RunTripleChomp         @ 426
	.4byte BattleEffect_RunDoublestep @ 427
	.4byte BattleEffect_RunPoisonGel    @ 428
	.4byte BattleEffect_RunDelude              @ 429
	.4byte BattleEffect_RunRisingVenom         @ 430
	.4byte BattleEffect_RunSack                @ 431
	.4byte BattleEffect_RunVolleyBMode19       @ 432
	.4byte BattleEffect_RunStunMuscle          @ 433
	.4byte BattleEffect_RunRollingFlame        @ 434
	.4byte BattleEffect_RunCage                @ 435
	.4byte BattleEffect_RunPhoenix             @ 436
	.4byte BattleEffect_RunHumanHunt           @ 437
	.4byte BattleEffect_RunTargetStrikeMode17                   @ 438
	.4byte BattleEffect_RunTargetStrikeMode18                   @ 439
	.4byte BattleEffect_RunPoisonTail          @ 440
	.4byte BattleEffect_RunKillSting           @ 441
	.4byte BattleEffect_RunBeastNeedle @ 442
	.4byte BattleEffect_RunPoisonSting @ 443
	.4byte BattleEffect_RunStunSting @ 444
	.4byte BattleEffect_RunDemonEye            @ 445
	.4byte BattleEffect_RunEmu                 @ 446
	.4byte BattleEffect_RunOceanFist           @ 447
	.4byte BattleEffect_RunCallDullahan        @ 448
	.4byte BattleEffect_RunGhostSoldier        @ 449
	.4byte BattleEffect_RunRisingDragonEnemy   @ 450
	.4byte BattleEffect_RunTrueCollide         @ 451
	.4byte BattleEffect_RunStarMine            @ 452
	.4byte BattleEffect_RunFormicAcid          @ 453
	.4byte BattleEffect_RunClawAttack   @ 454
	.4byte BattleEffect_RunBeatDance           @ 455
	.4byte BattleEffect_RunTargetStrikeMode20                   @ 456
	.4byte BattleEffect_RunLivingArmor         @ 457
	.4byte BattleEffect_RunHeatStun            @ 458
	.4byte BattleEffect_RunEarthForce          @ 459
	.4byte BattleEffect_RunGuardAura           @ 460
	.4byte BattleEffect_RunDjinnBlast          @ 461
	.4byte BattleEffect_RunDjinnStorm          @ 462
	.4byte BattleEffect_RunMineBall @ 463
	.4byte BattleEffect_RunElementSwap         @ 464
	.4byte BattleEffect_RunDjinnStun           @ 465
	.4byte BattleEffect_RunMysticCall          @ 466
	.4byte BattleEffect_RunClarionCry          @ 467
	.4byte BattleEffect_RunPsyBoost            @ 468
	.4byte BattleEffect_RunStrongHit           @ 469
	.4byte BattleEffect_RunChillBreath @ 470
	.4byte BattleEffect_RunVolleyBMode23       @ 471
	.global gCybele_ObjectPoses
gCybele_ObjectPoses:
	.byte 8, 10, 9, 11, 3, 4
	.global gCybele_ThornPictures
gCybele_ThornPictures:
	.2byte 3920, 3932, 3947, 3955, 3970
	.global gCybele_ThornWidths
gCybele_ThornWidths:
	.byte 2, 5, 2, 5, 3
	.global gCybele_ThornHeights
gCybele_ThornHeights:
	.byte 6, 3, 4, 3, 3
	.global gCybele_FlowerPictures
gCybele_FlowerPictures:
	.2byte 3456, 3712, 3856
	.global gCybele_FlowerSizes
gCybele_FlowerSizes:
	.byte 16, 12, 8
	.balign 2
	.global gCybele_ChipPictures
gCybele_ChipPictures:
	.2byte 3920, 3932, 3947, 3955, 3970
	.global gCybele_ChipWidths
gCybele_ChipWidths:
	.byte 2, 5, 2, 5, 3
	.global gCybele_ChipHeights
gCybele_ChipHeights:
	.byte 6, 3, 4, 3, 3
	.global gCybele_SparkPictures
gCybele_SparkPictures:
	.2byte 3979, 4043, 4268, 4389, 4470
	.global gCybele_SparkSizes
gCybele_SparkSizes:
	.byte 8, 15, 11, 9, 5
	.balign 2
	.global gCybele_BlastPictures
gCybele_BlastPictures:
	.2byte 5760, 6016, 7040, 9344, 11648, 13952, 16256
	.global gCybele_BlastSizes
gCybele_BlastSizes:
	.2byte 16, 32, 48, 48, 48, 48, 48
	.global gBoreas_Points
gBoreas_Points:
	.byte 57, 130, 75, 128, 71, 104, 101, 130, 86, 126, 85, 98, 64, 98, 98, 96
	.byte 77, 87, 84, 88, 68, 67, 81, 49, 94, 70, 70, 51, 93, 36, 81, 50
	.global gBoreas_ShakeOffsets
gBoreas_ShakeOffsets:
	.byte -4, -3, -2, 0
	.global gBoreas_CrystalPictures
gBoreas_CrystalPictures:
	.2byte 0, 1152, 2064
	.global gBoreas_CrystalWidths
gBoreas_CrystalWidths:
	.byte 24, 24, 20
	.global gBoreas_CrystalHeights
gBoreas_CrystalHeights:
	.byte 48, 38, 26
	.global gBoreas_Animations
gBoreas_Animations:
	.byte 2, 9, 10, 11
	.global gBoreas_DebrisWidths
gBoreas_DebrisWidths:
	.byte 12, 11, 8, 10, 8, 12, 5, 7, 7, 6, 7
	.global gBoreas_DebrisHeights
gBoreas_DebrisHeights:
	.byte 10, 16, 14, 15, 4, 4, 3, 4, 6, 12, 10
	.global gBoreas_DebrisPictures
gBoreas_DebrisPictures:
	.2byte 2584, 2704, 2880, 2992, 5062, 5094, 5142, 5157
	.2byte 5185, 5227, 5299
	.global gBoreas_SprayWidths
gBoreas_SprayWidths:
	.byte 16, 16, 16
	.global gBoreas_SprayHeights
gBoreas_SprayHeights:
	.byte 16, 16, 16
	.global gBoreas_SprayPictures
gBoreas_SprayPictures:
	.2byte 4294, 4550, 4806
	.global gBoreas_RisingCrystalPictures
gBoreas_RisingCrystalPictures:
	.2byte 0, 1152, 2064
	.global gBoreas_RisingCrystalWidths
gBoreas_RisingCrystalWidths:
	.byte 24, 24, 20
	.global gBoreas_RisingCrystalHeights
gBoreas_RisingCrystalHeights:
	.byte 48, 38, 26
	.global gBoreas_FallWidths
gBoreas_FallWidths:
	.byte 24, 24, 20
	.global gBoreas_FallHeights
gBoreas_FallHeights:
	.byte 48, 38, 26
	.global gBoreas_FallPictures
gBoreas_FallPictures:
	.2byte 0, 1152, 2064
	.global gDrain_Gravities
gDrain_Gravities:
	.4byte 0x4000, 0x2000, 0x4000, 0x8000
	.global gDrainMotes_Sizes
gDrainMotes_Sizes:
	.byte 6, 6, 10
	.global gQuake_Timings
gQuake_Timings:
	.byte 80, 32, 2, 112, 64, 4, 144, 96, 8
	.global gFire_Counts
gFire_Counts:
	.byte 16, 32, 64
	.global gFire_FlareWidths
gFire_FlareWidths:
	.byte 24, 29, 30, 27, 24, 16
	.global gFire_FlareHeights
gFire_FlareHeights:
	.byte 23, 32, 37, 49, 53, 52
	.global gFire_FlareBiasY
gFire_FlareBiasY:
	.byte 41, 32, 26, 11, 4, 2
	.balign 2
	.global gFire_FlarePictures
gFire_FlarePictures:
	.2byte 0, 552, 1480, 2590, 3913, 5185
	.global gFrost_MoundSpots
gFrost_MoundSpots:
	.byte 53, 108, 73, 110, 29, 110, 73, 108, 33, 104, 96, 106, 8, 109, 62, 98
	.byte 33, 100, 87, 100, 21, 95, 53, 90, 33, 90, 72, 91, 82, 91, 77, 86
	.byte 52, 84, 18, 92, 56, 81
	.global gFrost_Timings
gFrost_Timings:
	.byte 32, 8, 85, 48, 12, 97, 64, 19, 109
	.global gFrost_MoteWidths
gFrost_MoteWidths:
	.byte 12, 10, 7, 10, 8, 8, 8, 8
	.global gFrost_MoteHeights
gFrost_MoteHeights:
	.byte 9, 16, 14, 15, 8, 8, 8, 8
	.balign 2
	.global gFrost_MoteCells
gFrost_MoteCells:
	.2byte 1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500
	.global gFrost_MoundPictures
gFrost_MoundPictures:
	.2byte 0, 768, 1280
	.global gFrost_MoundHeights
gFrost_MoundHeights:
	.byte 24, 16, 16
	.global gFrost_ChipWidths
gFrost_ChipWidths:
	.byte 12, 10, 7, 10, 8, 8, 8, 8
	.global gFrost_ChipHeights
gFrost_ChipHeights:
	.byte 9, 16, 14, 15, 8, 8, 8, 8
	.balign 2
	.global gFrost_ChipCells
gFrost_ChipCells:
	.2byte 1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500
	.global gRay_Sparks
gRay_Sparks:
	.byte 2, 4, 2, 6, 4, 8
	.global gEruption_Counts
gEruption_Counts:
	.byte 16, 8, 72, 32, 16, 80, 128, 32, 88
	.balign 2
	.global gEruption_PuffPictures
gEruption_PuffPictures:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
	.global gEruption_PuffSizes
gEruption_PuffSizes:
	.2byte 16, 32, 48, 48, 48, 48, 48
	.global gProjectileVolleyB_GlintFlips
gProjectileVolleyB_GlintFlips:
	.byte 0, 4, 8, 12
	.global gAtalanta_UnitScale
gAtalanta_UnitScale:
	.4byte 0x10000, 0x10000
	.global gGrowth_VineWidths
gGrowth_VineWidths:
	.byte 21, 28, 19
	.global gGrowth_HeightLimits
gGrowth_HeightLimits:
	.byte 119, 119, 102
	.global gGrowth_VinePictures
gGrowth_VinePictures:
	.2byte 0, 2499, 5831
	.global gGrowth_PillarWidths
gGrowth_PillarWidths:
	.byte 10, 8, 9
	.balign 2
	.global gGrowth_PillarPictures
gGrowth_PillarPictures:
	.2byte 0, 1080, 1888
	.global gGrowth_ColumnCounts
gGrowth_ColumnCounts:
	.byte 4, 8, 12
	.global gGrowth_ColumnX
gGrowth_ColumnX:
	.byte 64, 72, 56, 80, 48, 88, 40, 96, 32, 104, 24, 72, 56, 80, 48, 88
	.global gGrowth_PillarHeights
gGrowth_PillarHeights:
	.byte 100, 60, 88, 96, 64, 104, 82, 98
	.global gGrowth_PuffWidths
gGrowth_PuffWidths:
	.byte 5, 7, 11, 16, 23, 27, 16
	.global gGrowth_PuffHeights
gGrowth_PuffHeights:
	.byte 10, 14, 21, 32, 45, 50, 32
	.balign 2
	.global gGrowth_PuffPictures
gGrowth_PuffPictures:
	.2byte 0, 50, 148, 379, 891, 1926, 3276
	.global gGrowth_ColumnOffsets
gGrowth_ColumnOffsets:
	.byte 0, 14, -32, 8, 32, -48, 4, 12, -24, 24, -12, -40, -4, -16, -32, 12
	.global gGrowth_Kinds
gGrowth_Kinds:
	.byte 0, 3, 2, 1, 2, 3, 2, 1
	.global gGrowth_X
gGrowth_X:
	.byte 24, 80, 24, 48, 88, 40, 8, 96
	.global gGrowth_Counts
gGrowth_Counts:
	.byte 3, 5, 8
	.global gGrowth_ThornPuffWidths
gGrowth_ThornPuffWidths:
	.byte 5, 7, 11, 17, 32, 32, 19
	.global gGrowth_ThornPuffHeights
gGrowth_ThornPuffHeights:
	.byte 10, 14, 21, 34, 59, 60, 38
	.balign 2
	.global gGrowth_ThornPuffPictures
gGrowth_ThornPuffPictures:
	.2byte 5888, 5938, 6036, 6267, 6845, 8733, 10653
	.global gBolt_Counts
gBolt_Counts:
	.byte 16, 0, 15, 1, 32, 3, 31, 1, 128, 5, 48, 3
	.global gBolt_GlintFlips
gBolt_GlintFlips:
	.byte 0, 4, 8, 12
	.global gBolt_GlintModes
gBolt_GlintModes:
	.byte 1, 1, 0
	.global gProjectileVolley_CometHeights
gProjectileVolley_CometHeights:
	.byte 24, 36, 48
	.global gProjectileVolley_Modes
gProjectileVolley_Modes:
	.byte 6, 10, 3, 32, 4, 4, 10, 5, 32, 8, 10, 32, 3, 32, 4, 5
	.byte 10, 5, 32, 4, 10, 10, 1, 32, 4, 1, 5, 0, 32, 80, 7, 8
	.byte 2, 32, 40, 30, 24, 1, 32, 4, 6, 2, 3, 32, 4, 60, 10, 1
	.byte 32, 4, 60, 12, 1, 32, 4, 10, 10, 1, 32, 4, 7, 8, 2, 32
	.byte 40, 20, 12, 1, 32, 40, 30, 24, 2, 32, 8, 30, 16, 1, 32, 8
	.byte 30, 16, 1, 32, 8, 30, 16, 1, 32, 8, 60, 12, 1, 32, 4
	.global gProjectileVolley_FlashWidths
gProjectileVolley_FlashWidths:
	.byte 12, 18, 24, 30, 36, 39, 40
	.global gProjectileVolley_FlashHeights
gProjectileVolley_FlashHeights:
	.byte 10, 15, 18, 22, 27, 28, 30
	.global gProjectileVolley_FlashSteps
gProjectileVolley_FlashSteps:
	.byte 2, 3, 4, 4, 5, 6, 6
	.global gProjectileVolley_FlashRises
gProjectileVolley_FlashRises:
	.byte 0, 3, 5, 7, 10, 12, 12, 0
	.global gProjectileVolley_FlashPictures
gProjectileVolley_FlashPictures:
	.2byte 8192, 8312, 8582, 9014, 9674, 10646, 11738
	.global gProjectileVolley_PelletPictures
gProjectileVolley_PelletPictures:
	.2byte 2308, 2372, 2436, 2500
	.global gProjectileVolley_GlintFlips
gProjectileVolley_GlintFlips:
	.byte 0, 4, 8, 12
	.global gDjinnAttacks_LaunchFrames
gDjinnAttacks_LaunchFrames:
	.byte 4, 7, 10, 21, 34
	.global gHail_CellWidths
gHail_CellWidths:
	.byte 8, 8, 8, 8
	.global gHail_CellHeights
gHail_CellHeights:
	.byte 8, 8, 8, 8
	.balign 2
	.global gHail_Pictures
gHail_Pictures:
	.2byte 2308, 2372, 2436, 2500
	.global gHail_CrackleFlips
gHail_CrackleFlips:
	.byte 3, 7, 11, 15
	.global gHail_PortalWidths
gHail_PortalWidths:
	.byte 24, 7, 7, 8, 6, 13, 10, 11, 7
	.global gHail_PortalHeights
gHail_PortalHeights:
	.byte 48, 13, 14, 24, 24, 17, 20, 14, 20
	.global gHail_PortalPictures
gHail_PortalPictures:
	.2byte 0, 1152, 1243, 1341, 1533, 1677, 1898, 2098
	.2byte 2252
	.global gHail_PortalX
gHail_PortalX:
	.byte 0, 9, 9, 11, 10, 0, 3, 13, 7
	.global gHail_PortalY
gHail_PortalY:
	.byte 0, 18, 18, 0, 24, 20, 5, 22, 26
	.global gThor_TargetPalettes
gThor_TargetPalettes:
	.byte 7, 14, 10, 4
	.global gThor_CrackleFlips
gThor_CrackleFlips:
	.byte 0, 4, 8, 12
	.global gSpire_ShardOffsets
gSpire_ShardOffsets:
	.byte 65, 104, 68, 95, 64, 93, 61, 93, 64, 87, 72, 85, 56, 88, 51, 80
	.byte 61, 75, 72, 81, 69, 71, 66, 70, 71, 64, 49, 71, 59, 69, 53, 68
	.byte 78, 73, 55, 67, 75, 62, 61, 65, 53, 61, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.global gSpire_RockX
gSpire_RockX:
	.byte 0, -12, 12, -8, 16
	.global gSpire_DropFrames
gSpire_DropFrames:
	.byte 8, 16, 20, 24, 32
	.global gSpire_RockCounts
gSpire_RockCounts:
	.byte 1, 3, 5
	.global gBurstScene_Styles
gBurstScene_Styles:
	.byte 1, 0, 5, 16, 12, 80, 0, 0, 1, 1, 7, 16, 12, 104, 0, 0
	.byte 1, 2, 9, 16, 12, 128, 0, 0, 1, 3, 1, 16, 16, 36, 0, 0
	.byte 1, 3, 2, 20, 16, 52, 0, 0, 1, 3, 3, 24, 16, 68, 0, 0
	.byte 0, 2, 1, 32, 12, 28, 1, 0, 0, 2, 2, 32, 12, 40, 1, 0
	.byte 0, 2, 3, 32, 12, 52, 1, 0, 1, 0, 3, 16, 12, 50, 0, 0
	.byte 1, 0, 1, 16, 12, 40, 0, 1, 1, 0, 1, 16, 12, 80, 0, 2
	.byte 1, 3, 1, 16, 16, 30, 0, 0
	.global gBurstScene_GlintFlips
gBurstScene_GlintFlips:
	.byte 0, 4, 8, 12, 0
	.global gGaia_RowPictures
gGaia_RowPictures:
	.2byte 0, 160, 352, 640, 1024, 1504, 2080, 2752
	.2byte 3776
	.global gGaia_RowWidths
gGaia_RowWidths:
	.byte 5, 6, 9, 12, 15, 18, 21, 32, 40
	.balign 2
	.global gGaia_FlamePictures
gGaia_FlamePictures:
	.2byte 10672, 11505, 12550
	.global gGaia_FlameWidths
gGaia_FlameWidths:
	.byte 17, 19, 20
	.global gGaia_FlameHeights
gGaia_FlameHeights:
	.byte 49, 55, 64
	.global gGaia_Bg2Shifts
gGaia_Bg2Shifts:
	.byte -8, 0, 12, -72, -40, -16
	.global gGaia_LateRockColumns
gGaia_LateRockColumns:
	.byte 136, 40, 120, 56, 104, 72
	.global gUlysses_ObjectX
gUlysses_ObjectX:
	.byte 0, 32, 0, 32, 0, 32, 32
	.global gUlysses_ObjectZ
gUlysses_ObjectZ:
	.byte 8, 0, 40, 32, 72, 64, 96
	.global gUlysses_PictureWidths
gUlysses_PictureWidths:
	.byte 12, 9, 6, 5, 8, 9, 12, 9, 8, 8, 8, 10, 11, 10, 8, 6
	.byte 6, 9
	.global gUlysses_PictureHeights
gUlysses_PictureHeights:
	.byte 18, 17, 15, 12, 14, 15, 16, 13, 8, 4, 4, 6, 9, 9, 10, 10
	.byte 13, 16
	.global gUlysses_Pictures
gUlysses_Pictures:
	.2byte 0, 216, 369, 459, 519, 631, 766, 958
	.2byte 1075, 1139, 1171, 1203, 1263, 1362, 1452, 1532
	.2byte 1592, 1670
	.global gUlysses_ParticleStarts
gUlysses_ParticleStarts:
	.byte 100, 60, 64, -6, -6, 112, 44, -64, -6, 4, 96, 28, 64, -6, -5, 112
	.byte 80, -64, -6, 4, 104, 32, 64, -6, 5, 102, 68, -64, -6, 4
	.global gSpiderWeb_PictureWidths
gSpiderWeb_PictureWidths:
	.byte 40, 112, 112, 112, 88, 64
	.global gSpiderWeb_PictureHeights
gSpiderWeb_PictureHeights:
	.byte 56, 80, 64, 64, 80, 88
	.global gSpiderWeb_Pictures
gSpiderWeb_Pictures:
	.2byte 0, 2240, 11200, 18368, 0, 7040
	.global gSpiderWeb_PictureX
gSpiderWeb_PictureX:
	.byte 72, 0, 0, 0, 32, 64, 16, 16, 16, 16, 8, 0
	.global gSpiderWeb_PictureBiasY
gSpiderWeb_PictureBiasY:
	.byte 0, 4, 24, -6, -20, -28
	.global gAzul_PartPoints
gAzul_PartPoints:
	.byte 23, 13, 0, 0, 42, 0, 28, 41, 28, 41, 48, 28, 42, 0, 78, 31
	.byte 28, 41, 28, 41, 66, 7, 42, 0, 79, 0, 78, 31, 78, 31, 94, 8
	.byte 79, 0, 128, 0, 78, 31, 78, 31, 112, 28, 128, 0, 132, 48, 78, 31
	.byte 78, 31, 162, 12, 182, 0, 174, 42, 128, 0, 128, 0, 145, 39, 128, 0
	.byte 174, 42, 134, 71, 134, 71, 196, 12, 182, 0, 239, 0, 174, 42, 172, 42
	.byte 231, 20, 239, 0, 239, 52, 211, 16, 211, 16, 7, 33, 0, 0, 28, 41
	.byte 0, 48, 0, 48, 59, 49, 78, 31, 69, 73, 28, 41, 28, 41, 91, 51
	.byte 78, 31, 132, 48, 69, 73, 69, 73, 16, 58, 28, 41, 20, 86, 0, 48
	.byte 0, 48, 36, 70, 28, 41, 69, 73, 20, 86, 20, 86, 96, 76, 132, 48
	.byte 99, 97, 69, 73, 69, 73, 6, 80, 0, 48, 20, 86, 0, 98, 0, 98
	.byte 168, 66, 174, 42, 205, 88, 134, 71, 134, 71, 194, 48, 211, 16, 205, 88
	.byte 174, 42, 174, 42, 217, 52, 211, 16, 239, 52, 205, 88, 208, 88, 7, 114
	.byte 0, 98, 30, 119, 0, 119, 0, 119, 29, 106, 20, 86, 64, 119, 30, 119
	.byte 0, 98, 51, 93, 69, 73, 64, 119, 20, 86, 20, 86, 78, 96, 69, 73
	.byte 99, 97, 64, 119, 64, 119, 98, 112, 99, 97, 137, 104, 112, 119, 64, 119
	.byte 123, 85, 132, 48, 137, 104, 99, 97, 99, 97, 156, 88, 134, 71, 205, 88
	.byte 137, 104, 137, 104, 139, 115, 137, 104, 175, 119, 112, 119, 112, 119, 217, 52
	.byte 211, 16, 239, 52, 205, 88, 205, 88, 228, 87, 239, 52, 239, 119, 205, 88
	.byte 205, 88, 200, 108, 185, 93, 205, 88, 239, 119, 175, 119, 169, 106, 185, 93
	.byte 175, 119, 137, 104, 137, 104
	.global gAzul_UnitScale
gAzul_UnitScale:
	.4byte 0x10000, 0x10000
	.global gAzul_ObjectX
gAzul_ObjectX:
	.byte 35, 45, 14, 1, 1, 40, 9, 40, 9, 25, 39, 8
	.global gAzul_ObjectZ
gAzul_ObjectZ:
	.byte 0, 32, 32, 64, 96, 51, 51, 83, 83, 106, 129, 129
	.global gAzul_LateUnitScale
gAzul_LateUnitScale:
	.4byte 0x10000, 0x10000
	.global gAzul_LateObjectX
gAzul_LateObjectX:
	.byte 0, 32, 32, 64, 96, 50, 51, 82
	.global gAzul_LateObjectZ
gAzul_LateObjectZ:
	.byte 10, 0, 32, 45, 45, 5, 36, 5
	.global gBeam_PictureWidths
gBeam_PictureWidths:
	.byte 38, 54, 71, 74, 77, 65
	.global gBeam_PictureHeights
gBeam_PictureHeights:
	.byte 7, 9, 11, 13, 13, 11
	.global gBeam_Pictures
gBeam_Pictures:
	.2byte 0, 266, 752, 1533, 2495, 3496
	.global gBeam_PictureReaches
gBeam_PictureReaches:
	.byte 58, 42, 25, 13, 5, 0
	.global gCastingImpact_SquallGlintFlips
gCastingImpact_SquallGlintFlips:
	.byte 0, 4, 8, 12
	.global gCastingImpact_SapGlintFlips
gCastingImpact_SapGlintFlips:
	.byte 0, 4, 8, 12
	.global gCastingImpact_BitePictures
gCastingImpact_BitePictures:
	.byte 0, 1, 2, 3
	.global gCastingImpact_RaidensWrathPictures
gCastingImpact_RaidensWrathPictures:
	.2byte 8448, 8712, 11462, 16646, 16646, 24351, 24351, 28201
	.2byte 28201, 28201
	.global gCastingImpact_RaidensWrathWidths
gCastingImpact_RaidensWrathWidths:
	.2byte 24, 55, 64, 115, 115, 70, 70, 22
	.2byte 22, 22
	.global gCastingImpact_RaidensWrathHeights
gCastingImpact_RaidensWrathHeights:
	.2byte 11, 50, 81, 67, 67, 55, 55, 29
	.2byte 29, 29
	.global gCastingImpact_RaidensWrathX
gCastingImpact_RaidensWrathX:
	.2byte 58, 19, 0, 6, 6, 50, 50, 100
	.2byte 100, 100
	.global gCastingImpact_RaidensWrathY
gCastingImpact_RaidensWrathY:
	.2byte 17, 19, 24, 44, 44, 42, 42, 37
	.2byte 37, 37
	.global gCastingImpact_ApocalypsePictures
gCastingImpact_ApocalypsePictures:
	.2byte 5700, 7128, 10414, 14089, 19152, 23148, 26752
	.global gCastingImpact_ApocalypseWidths
gCastingImpact_ApocalypseWidths:
	.2byte 42, 62, 75, 83, 54, 68, 37
	.global gCastingImpact_ApocalypseHeights
gCastingImpact_ApocalypseHeights:
	.2byte 34, 53, 49, 61, 74, 53, 28
	.global gCastingImpact_ApocalypseX
gCastingImpact_ApocalypseX:
	.2byte 85, 65, 50, 19, 0, 0, 58
	.global gCastingImpact_ApocalypseY
gCastingImpact_ApocalypseY:
	.2byte 62, 66, 78, 41, 19, 0, 5
	.global gCastingImpact_ApocalypseBurstPictures
gCastingImpact_ApocalypseBurstPictures:
	.2byte 27788, 28464, 29724
	.global gCastingImpact_ApocalypseBurstWidths
gCastingImpact_ApocalypseBurstWidths:
	.byte 26, 35, 52
	.global gCastingImpact_ApocalypseBurstHeights
gCastingImpact_ApocalypseBurstHeights:
	.byte 26, 36, 48
	.global gCastingImpact_ApocalypseBurstPoints
gCastingImpact_ApocalypseBurstPoints:
	.byte 37, 104, 19, 81, 20, 47
	.global gCastingImpact_IceChipPictures
gCastingImpact_IceChipPictures:
	.2byte 2308, 2372, 2436, 2500
	.global gCastingImpact_StunBoltGlintFlips
gCastingImpact_StunBoltGlintFlips:
	.byte 0, 4, 8, 12
	.global gCastingImpact_StunJipBandStarts
gCastingImpact_StunJipBandStarts:
	.byte 0, 2, 9, 10
	.global gCastingImpact_StunJipBandGrowth
gCastingImpact_StunJipBandGrowth:
	.byte 5, 8, 10, 8
	.global gCastingImpact_StunJipBandHeights
gCastingImpact_StunJipBandHeights:
	.byte 32, 16, 24, 32
	.global gCastingImpact_ArmorCrushWaveStarts
gCastingImpact_ArmorCrushWaveStarts:
	.byte 0, 4, 9, 10
	.global gCastingImpact_ArmorCrushWaveGrowth
gCastingImpact_ArmorCrushWaveGrowth:
	.byte 3, 4, 3, 5
	.global gCastingImpact_ArmorCrushWaveStretch
gCastingImpact_ArmorCrushWaveStretch:
	.byte 3, 4, 5, 6
	.global gNereidCopy_Columns
gNereidCopy_Columns:
	.byte 16, 104, 32, 104, 48, 104, 64, 104, 0, 0
	.global gNereidCopy_Gravity
gNereidCopy_Gravity:
	.4byte 0x8000, 0x4000, 0x4000, 0x2000
	.global gHelmSplitter_TwelveX
gHelmSplitter_TwelveX:
	.byte 52, 25, 57, 89, 7, 39, 71, 103, 21, 53, 85, 117
	.global gHelmSplitter_TwelveZ
gHelmSplitter_TwelveZ:
	.byte 0, 32, 32, 32, 64, 64, 64, 64, 96, 96, 96, 96
	.global gHelmSplitter_RightEightX
gHelmSplitter_RightEightX:
	.byte 0, 32, 64, 16, 48, 0, 32, 64
	.global gHelmSplitter_RightEightZ
gHelmSplitter_RightEightZ:
	.byte 0, 0, 0, 32, 32, 64, 64, 64
	.global gFormationObjects_X
gFormationObjects_X:
	.byte 0, 32, 64, 0, 32, 64, 32, 64
	.global gFormationObjects_Z
gFormationObjects_Z:
	.byte 10, 0, 0, 42, 32, 32, 64, 64
	.global gHelmSplitter_SmallEightX
gHelmSplitter_SmallEightX:
	.byte 0, 40, 80, 0, 40, 80, 40, 80
	.global gHelmSplitter_SmallEightZ
gHelmSplitter_SmallEightZ:
	.byte 13, 0, 0, 53, 40, 40, 80, 80
	.global gFormationObjects_WideX
gFormationObjects_WideX:
	.byte 16, 48, 80, 16, 48, 80, -16, 16, 48, 80, 42
	.global gFormationObjects_WideZ
gFormationObjects_WideZ:
	.byte 0, 0, 0, 32, 32, 32, 50, 64, 64, 64, 96
	.global gHelmSplitter_ImpactBurstPictureWidths
gHelmSplitter_ImpactBurstPictureWidths:
	.byte 48, 48, 46, 36, 36, 32, 26, 6
	.global gHelmSplitter_ImpactBurstPictureHeights
gHelmSplitter_ImpactBurstPictureHeights:
	.byte 96, 96, 90, 74, 74, 62, 52, 12
	.global gHelmSplitter_ImpactBurstPictures
gHelmSplitter_ImpactBurstPictures:
	.2byte 0, 4608, 9216, 13356, 16020, 18684, 20668, 22020
	.global gParticleStreams_OrbitPoints
gParticleStreams_OrbitPoints:
	.2byte -64, -96, 24, -48, -120, 32, -32, -80
	.2byte -48, -16, -72, -24, 16, -80, 16, 32
	.2byte -112, -32, 48, -96, 56
	.global gParticleStreams_FireballPoints
gParticleStreams_FireballPoints:
	.2byte 32, 32, 48, 96, 80, 80, 112, 100
	.2byte 176, 80, 196, 64, 204, 96
	.global gParticleStreams_FlashPictures
gParticleStreams_FlashPictures:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
	.global gParticleStreams_FlashSizes
gParticleStreams_FlashSizes:
	.2byte 16, 32, 48, 48, 48, 48, 48
	.global gRamses_StatueColumns
gRamses_StatueColumns:
	.byte 12, 44, 28, 60, 0, 32, 64, 0, 32
	.global gRamses_StatueRows
gRamses_StatueRows:
	.byte 0, 0, 32, 32, 64, 64, 64, 32, 32
	.global gRamses_PuffPictures
gRamses_PuffPictures:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
	.global gRamses_PuffSizes
gRamses_PuffSizes:
	.2byte 16, 32, 48, 48, 48, 48, 48
	.global gRagnarok_StrikeColumns
gRagnarok_StrikeColumns:
	.byte 32, 24, 0, 96, 104, 128
	.global gRagnarok_FlashPictures
gRagnarok_FlashPictures:
	.byte 5, 5, 4, 3, 2, 1
	.global gTitanBlade_FlashPictures
gTitanBlade_FlashPictures:
	.byte 5, 5, 4, 3, 2, 1
	.global gTitanBlade_DustGravity
gTitanBlade_DustGravity:
	.4byte 0x8000, 0x4000, 0x4000, 0x2000
	.global gJudgment_BurstPictures
gJudgment_BurstPictures:
	.byte 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 13, 14, 0
	.global gJudgment_ShardPictures
gJudgment_ShardPictures:
	.2byte 1536, 2125, 2838, 3551, 3950, 4286
	.global gJudgment_ShardWidths
gJudgment_ShardWidths:
	.byte 19, 23, 23, 19, 16, 17
	.global gJudgment_ShardHeights
gJudgment_ShardHeights:
	.byte 31, 31, 31, 21, 21, 24
	.global gKirin_Columns
gKirin_Columns:
	.byte 8, 40, 72, 104, 8, 40, 72, 104, 0
	.global gKirin_Rows
gKirin_Rows:
	.byte 0, 0, 0, 24, 32, 32, 32, 56, 64
	.global gNeptune_Gravity
gNeptune_Gravity:
	.4byte 0x8000, 0x4000, 0x4000, 0x2000
	.global gUndeadSword_SparkPictures
gUndeadSword_SparkPictures:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
	.global gUndeadSword_SparkSizes
gUndeadSword_SparkSizes:
	.2byte 16, 32, 48, 48, 48, 48, 48, 1536
	.2byte 2312, 2053, 775, 8224, 8224
	.global gWindFunnel_Counts
gWindFunnel_Counts:
	.byte 3, 3
	.global gDiamondBerg_RingStarts
gDiamondBerg_RingStarts:
	.byte 0, 2, 4, 5
	.global gDiamondBerg_RingGrowth
gDiamondBerg_RingGrowth:
	.byte 6, 7, 6, 8
	.global gDiamondBerg_RingHeights
gDiamondBerg_RingHeights:
	.byte 32, 16, -8, -24
	.global gDiamondBerg_ShardRolls
gDiamondBerg_ShardRolls:
	.2byte 8192, 24576, 8192, 24576, 8192, 24576, 8192, 24576
	.global gDiamondBerg_ShardPitches
gDiamondBerg_ShardPitches:
	.2byte -2048, -2048, -2048, -2048, -2048, -2048, -2048, -2048
	.global gGeode_WaveStarts
gGeode_WaveStarts:
	.byte 0, 6, 8, 9
	.global gGeode_WaveGrowth
gGeode_WaveGrowth:
	.byte 5, 8, 7, 3
	.global gGeode_WaveHeights
gGeode_WaveHeights:
	.byte 32, 32, 32, 32
	.global gSoulShatter_SparkPictures
gSoulShatter_SparkPictures:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
	.global gSoulShatter_SparkSizes
gSoulShatter_SparkSizes:
	.2byte 16, 32, 48, 48, 48, 48, 48
	.global gSerac_RowHeights
gSerac_RowHeights:
	.byte -96, -88, -72, -64, -48, -32, -8, 0
	.global gSerac_RowRadii
gSerac_RowRadii:
	.byte 1, 6, 10, 12, 16, 24, 30, 32
	.global gSerac_RingStarts
gSerac_RingStarts:
	.byte 6, 8, 10, 11
	.global gSerac_RingGrowth
gSerac_RingGrowth:
	.byte 8, 7, 6, 3
	.global gSerac_RingHeights
gSerac_RingHeights:
	.byte 16, 32, 56, 72
	.global gHeavyBlow_StrikeFrames
gHeavyBlow_StrikeFrames:
	.byte 38, 40
	.global gHeavyBlow_ChipCounts
gHeavyBlow_ChipCounts:
	.byte 128, 32
	.global gHeavyBlow_ChipPixels
gHeavyBlow_ChipPixels:
	.byte 58, 58
	.global gCool_ColumnX
gCool_ColumnX:
	.2byte 92, 72, 32, 48, 88, 56, 80, 88
	.2byte -8, 132
	.global gCool_ColumnStarts
gCool_ColumnStarts:
	.byte 0, 4, 7, 10, 13, 16, 20, 23, 25, 27
	.global gCool_ColumnHeights
gCool_ColumnHeights:
	.byte 64, 64, 48, 64, 64, 56, 62, 64, 46, 52
	.global gCool_ColumnLeans
gCool_ColumnLeans:
	.byte 0, 3, 1, 2, 3, 0, 1, 2, 1, 0
	.global gCool_SparkPictures
gCool_SparkPictures:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
	.global gCool_SparkSizes
gCool_SparkSizes:
	.2byte 16, 32, 48, 48, 48, 48, 48
	.global gWyrdCurse_DropBounces
gWyrdCurse_DropBounces:
	.byte -12, 4, -8, 14
	.global gBoltTrail_GlintFlips
gBoltTrail_GlintFlips:
	.byte 0, 4, 8, 12
	.byte 0, 0
	.global gMegaera_GlintFlips
gMegaera_GlintFlips:
	.byte 0, 4, 8, 12
	.global gMegaera_GlintModes
gMegaera_GlintModes:
	.byte 1, 1, 0, 0
	.global gLightSurge_PillarSizes
gLightSurge_PillarSizes:
	.4byte 18000, 12000, 30000, 40000
	.global gLightSurge_PillarShades
gLightSurge_PillarShades:
	.4byte 16578814
	.global gLightSurge_PillarOffsets
gLightSurge_PillarOffsets:
	.byte -24, -8, 24, 48
	.global gDjinniCharge_EntranceFlags
gDjinniCharge_EntranceFlags:
	.byte 2, 10, 0, 0
	.global gDjinniCharge_DebrisGravity
gDjinniCharge_DebrisGravity:
	.4byte -0x2000, -0x1000, -2048, -1024
	.global gGaspFamily_StreakSteps
gGaspFamily_StreakSteps:
	.byte 0, 0, 1, 2, 4, 6, 9, 12, 16, 0, 0, 0
	.global gGaspFamily_StreakMesh
gGaspFamily_StreakMesh:
	.2byte 769, 2, 31, 7967, 7936, 0, 256, 2
	.2byte 0, 31, 7936, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0, 0, 0, 0
	.global gGaspFamily_Counts
gGaspFamily_Counts:
	.byte 32, 64, 255
	.global gCharon_FlashFrames
gCharon_FlashFrames:
	.byte 16, 60, 108, 0, 0
	.global gBattleEffect_OctagonBandCorners
gBattleEffect_OctagonBandCorners:
	.4byte 0x20f800, 1505302, 63519, 15398934
	.4byte 0xe1f800, 15399146, 63713, 1505514
	.4byte 0x200800, 1443862, 2079, 15337494
	.4byte 0xe10800, 15337706, 2273, 1444074
	.global gBattleEffect_CylinderCorners
gBattleEffect_CylinderCorners:
	.4byte 0x1ff800, 1964044, 1505302, 849949
	.4byte 63519, 16054301, 15398934, 14940172
	.4byte 0xe1f800, 14940403, 15399145, 16054498
	.4byte 63712, 915682, 1571049, 2029811
	.4byte 0x1f0800, 1902604, 1443862, 788509
	.4byte 2079, 15992861, 15337494, 14878732
	.4byte 0xe10800, 14878963, 15337705, 15993058
	.4byte 2272, 854242, 1509609, 1968371
	.global gCastingImpact_BandMesh
gCastingImpact_BandMesh:
	.2byte 2305, 8, 31, 3871, 3840, 0, 256, 8
	.2byte 0, 31, 3840, 0, 2562, 9, 63, 3903
	.2byte 3872, 0, 513, 9, 32, 63, 3872, 0
	.2byte 2819, 10, 95, 3935, 3904, 0, 770, 10
	.2byte 64, 95, 3904, 0, 3076, 11, 127, 3967
	.2byte 3936, 0, 1027, 11, 96, 127, 3936, 0
	.2byte 3333, 12, 31, 3871, 3840, 0, 1284, 12
	.2byte 0, 31, 3840, 0, 3590, 13, 63, 3903
	.2byte 3872, 0, 1541, 13, 32, 63, 3872, 0
	.2byte 3847, 14, 95, 3935, 3904, 0, 1798, 14
	.2byte 64, 95, 3904, 0, 2048, 15, 127, 3967
	.2byte 3936, 0, 7, 15, 96, 127, 3936, 0
	.2byte 0, 0, 0, 0, 0, 0
	.global gGeyser_Faces
gGeyser_Faces:
	.2byte 2305, 8, 31, 16159, 16128, 0, 256, 8
	.2byte 0, 31, 16128, 0, 2562, 9, 63, 16191
	.2byte 16160, 0, 513, 9, 32, 63, 16160, 0
	.2byte 2819, 10, 95, 16223, 16192, 0, 770, 10
	.2byte 64, 95, 16192, 0, 3076, 11, 127, 16255
	.2byte 16224, 0, 1027, 11, 96, 127, 16224, 0
	.2byte 3333, 12, 31, 16159, 16128, 0, 1284, 12
	.2byte 0, 31, 16128, 0, 3590, 13, 63, 16191
	.2byte 16160, 0, 1541, 13, 32, 63, 16160, 0
	.2byte 3847, 14, 95, 16223, 16192, 0, 1798, 14
	.2byte 64, 95, 16192, 0, 2048, 15, 127, 16255
	.2byte 16224, 0, 7, 15, 96, 127, 16224, 0
	.2byte 0, 0, 0, 0, 0, 0
	.global gBattleEffect_CylinderMesh
gBattleEffect_CylinderMesh:
	.2byte 4353, 16, 15, 3855, 3840, 0, 256, 16
	.2byte 0, 15, 3840, 0, 4610, 17, 31, 3871
	.2byte 3855, 0, 513, 17, 15, 31, 3855, 0
	.2byte 4867, 18, 47, 3887, 3871, 0, 770, 18
	.2byte 31, 47, 3871, 0, 5124, 19, 63, 3903
	.2byte 3887, 0, 1027, 19, 47, 63, 3887, 0
	.2byte 5381, 20, 15, 3855, 3840, 0, 1284, 20
	.2byte 0, 15, 3840, 0, 5638, 21, 31, 3871
	.2byte 3855, 0, 1541, 21, 15, 31, 3855, 0
	.2byte 5895, 22, 47, 3887, 3871, 0, 1798, 22
	.2byte 32, 47, 3871, 0, 6152, 23, 63, 3903
	.2byte 3887, 0, 2055, 23, 47, 63, 3887, 0
	.2byte 6409, 24, 79, 3919, 3904, 0, 2312, 24
	.2byte 64, 79, 3904, 0, 6666, 25, 95, 3935
	.2byte 3919, 0, 2569, 25, 79, 95, 3919, 0
	.2byte 6923, 26, 111, 3951, 3935, 0, 2826, 26
	.2byte 95, 111, 3935, 0, 7180, 27, 127, 3967
	.2byte 3951, 0, 3083, 27, 111, 127, 3951, 0
	.2byte 7437, 28, 79, 3919, 3904, 0, 3340, 28
	.2byte 64, 79, 3904, 0, 7694, 29, 95, 3935
	.2byte 3919, 0, 3597, 29, 79, 95, 3919, 0
	.2byte 7951, 30, 111, 3951, 3935, 0, 3854, 30
	.2byte 96, 111, 3935, 0, 4096, 31, 127, 3967
	.2byte 3951, 0, 15, 31, 111, 127, 3951, 0
	.2byte 0, 0, 0, 0, 0, 0
	.global gGeode_RingCorners
gGeode_RingCorners:
	.4byte 0x40f800, 2947116, 63550, 13957164
	.4byte 0xc2f800, 13957332, 63682, 2947284
	.4byte 0x200000, 1441814, 31, 15335446
	.4byte 0xe10000, 15335658, 225, 1442026
	.global gBattleEffect_RingCorners
gBattleEffect_RingCorners:
	.4byte 0x26f800, 1767450, 63525, 15136794
	.4byte 0xdbf800, 15136998, 63707, 1767654
	.4byte 0x200000, 1441814, 31, 15335446
	.4byte 0xe10000, 15335658, 225, 1442026
	.global gBattleEffect_RingMesh
gBattleEffect_RingMesh:
	.2byte 2305, 8, 31, 16159, 16128, 0, 256, 8
	.2byte 0, 31, 16128, 0, 2562, 9, 63, 16191
	.2byte 16160, 0, 513, 9, 31, 63, 16160, 0
	.2byte 2819, 10, 95, 16223, 16192, 0, 770, 10
	.2byte 63, 95, 16192, 0, 3076, 11, 127, 16255
	.2byte 16224, 0, 1027, 11, 95, 127, 16224, 0
	.2byte 3333, 12, 31, 16159, 16128, 0, 1284, 12
	.2byte 0, 31, 16128, 0, 3590, 13, 63, 16191
	.2byte 16160, 0, 1541, 13, 31, 63, 16160, 0
	.2byte 3847, 14, 95, 16223, 16192, 0, 1798, 14
	.2byte 63, 95, 16192, 0, 2048, 15, 127, 16255
	.2byte 16224, 0, 7, 15, 95, 127, 16224, 0
	.2byte 0, 0, 0, 0, 0, 0
	.global gBattleEffect_TriangleMesh
gBattleEffect_TriangleMesh:
	.4byte 0x20100, 0
	.global gBattleEffect_TriangleCorners
gBattleEffect_TriangleCorners:
	.4byte 16384250, 16384006, 393466
	.global gBattleEffect_TaperedBeamCorners
gBattleEffect_TaperedBeamCorners:
	.4byte 32762, 32518, 254, 2
	.global gBattleEffect_BeamCorners
gBattleEffect_BeamCorners:
	.4byte 32762, 32518, 250, 6
	.global gBattleEffect_LongBeamCorners
gBattleEffect_LongBeamCorners:
	.2byte 32518, 0, 32762, 0, 64518, 0, 64762, 0
	.global gBattleEffect_SquareCorners
gBattleEffect_SquareCorners:
	.2byte 57568, 0, 57376, 0, 8416, 0, 8224, 0
	.global gBattleEffect_StandingSquareCorners
gBattleEffect_StandingSquareCorners:
	.2byte 49376, 0, 49184, 0, 224, 0, 32, 0
	.global gLegend_WingCorners
gLegend_WingCorners:
	.4byte 12599536, 12599312, 14576, 14352
	.global gBattleEffect_GroundSquareCorners
gBattleEffect_GroundSquareCorners:
	.4byte 12583104, 12582976, 4194496, 4194368
	.global gBattleEffect_SmallSquareMesh
gBattleEffect_SmallSquareMesh:
	.2byte 769, 2, 31, 7967, 7936, 0, 256, 2
	.2byte 0, 31, 7936, 0, 0, 0, 0, 0
	.2byte 0, 0
	.global gBattleEffect_SmallSquareMeshReversed
gBattleEffect_SmallSquareMeshReversed:
	.2byte 256, 2, 0, 31, 7936, 0, 769, 2
	.2byte 31, 7967, 7936, 0, 0, 0, 0, 0
	.2byte 0, 0
	.global gBattleEffect_ColumnMesh
gBattleEffect_ColumnMesh:
	.2byte 769, 2, 31, 30495, 30464, 0, 256, 2
	.2byte 0, 31, 30464, 0, 0, 0, 0, 0
	.2byte 0, 0
	.global gHeavyBlow_SwordMesh
gHeavyBlow_SwordMesh:
	.2byte 256, 2, 0, 31, 32512, 0, 769, 2
	.2byte 31, 32543, 32512, 0, 0, 0, 0, 0
	.2byte 0, 0
	.global gBattleEffect_TallMesh
gBattleEffect_TallMesh:
	.2byte 256, 2, 256, 287, 16128, 0, 769, 2
	.2byte 287, 16159, 16128, 0, 0, 0, 0, 0
	.2byte 0, 0, 256, 2, 0, 15, 16128, 0
	.2byte 769, 2, 15, 16143, 16128, 0, 0, 0
	.2byte 0, 0, 0, 0
	.global gHaures_BandMesh
gHaures_BandMesh:
	.2byte 256, 2, 0, 63, 7936, 0, 769, 2
	.2byte 63, 7999, 7936, 0, 0, 0, 0, 0
	.2byte 0, 0
	.global gCatastrophe_BurstMesh
gCatastrophe_BurstMesh:
	.2byte 256, 2, 0, 31, 12032, 0, 769, 2
	.2byte 31, 12063, 12032, 0, 0, 0, 0, 0
	.2byte 0, 0
	.global gBattleEffect_SquareMesh
gBattleEffect_SquareMesh:
	.2byte 769, 2, 319, 16191, 16129, 0, 256, 2
	.2byte 257, 319, 16129, 0, 0, 0, 0, 0
	.2byte 0, 0
	.global gBattleEffect_LargeSquareMesh
gBattleEffect_LargeSquareMesh:
	.2byte 769, 2, 127, 32639, 32512, 0, 256, 2
	.2byte 0, 127, 32512, 0, 0, 0, 0, 0
	.2byte 0, 0
	.global gHaures_BurstMesh
gHaures_BurstMesh:
	.2byte 769, 2, 47, 18223, 18176, 0, 256, 2
	.2byte 0, 47, 18176, 0, 0, 0, 0, 0
	.2byte 0, 0, 769, 2, 4143, 12079, 12048, 0
	.2byte 256, 2, 4112, 4143, 12048, 0, 0, 0
	.2byte 0, 0, 0, 0
	.global gHeartbreak_SwingWidths
gHeartbreak_SwingWidths:
	.byte 32, 39, 42, 71
	.global gHeartbreak_SwingHeights
gHeartbreak_SwingHeights:
	.byte 29, 32, 48, 57
	.global gHeartbreak_SwingPictures
gHeartbreak_SwingPictures:
	.2byte 0, 928, 2176, 4192
	.global gHeartbreak_FistHeights
gHeartbreak_FistHeights:
	.byte 29, 37, 48, 47, 52, 32, 22, 17
	.global gHeartbreak_FistY
gHeartbreak_FistY:
	.byte 34, 26, 15, 14, 7, 11, 8, 6
	.global gHeartbreak_FistPictures
gHeartbreak_FistPictures:
	.2byte 0, 928, 2112, 3648, 5152, 6816, 7840, 8544
	.global gRagingHeat_Styles
gRagingHeat_Styles:
	.byte 8, 127, 64, 32, 12, 191, 96, 64, 16, 255, 128, 255
	.global gJuggle_Styles
gJuggle_Styles:
	.byte 1, 24, 4, 64, 32, 4, 0, 2, 32, 8, 32, 64, 8, 0, 2, 48
	.byte 16, 32, 128, 16, 0
	.global gFlora_PetalSizes
gFlora_PetalSizes:
	.byte 4, 8, 16, 24, 0
	.global gFlora_PetalPictures
gFlora_PetalPictures:
	.2byte 0, 32, 160, 672
	.global gFlora_BloomFrames
gFlora_BloomFrames:
	.2byte 114, 117, 120, 250
	.global gFlora_BloomRolls
gFlora_BloomRolls:
	.2byte 0, 24576, -28576, 11008
	.global gFlora_BloomPitches
gFlora_BloomPitches:
	.2byte 16384, 4096, 2096, -7168
	.global gFlora_BloomX
gFlora_BloomX:
	.byte 16, 16, 16, -16
	.global gFlora_BloomY
gFlora_BloomY:
	.byte 0, 0, 0, 12, 0, 0
	.global gSabreDance_DaggerMesh
gSabreDance_DaggerMesh:
	.2byte 256, 2, 6, 25, 7942, 0, 769, 2
	.2byte 25, 7961, 7942, 0, 0, 0, 0, 0
	.2byte 0, 0
	.global gSabreDance_DaggerCorners
gSabreDance_DaggerCorners:
	.2byte 61680, 0, 61456, 0, 16624, 0, 16400, 0
	.global gFume_Levels
gFume_Levels:
	.2byte 16384, 16, 24, 96, 8, 4096, 24576, 16
	.2byte 48, 192, 16, 8192, 40768, 32, 38, 256
	.2byte 32, 16384
	.global gOceanFist_DropGravity
gOceanFist_DropGravity:
	.4byte -0x2000, -0x4000, -0x4000, -0x8000
	.global gRushingStrike_Scales
gRushingStrike_Scales:
	.4byte 0x8000, 0x8000, 0x8000, 0x8000
	.4byte 55536, 52768
	.global gMirroredWall_BurstScales
gMirroredWall_BurstScales:
	.4byte 39768, 44152, 44152, 55536
	.global gMirroredWall_BurstShades
gMirroredWall_BurstShades:
	.4byte -8, -8, -8, 0
	.global gDaedalus_ShakeOffsets
gDaedalus_ShakeOffsets:
	.byte 0, -1, 1, -1, 0, 2, 1, 0, -1, -1, -2, -2, -3, -3, -4, -4
	.byte -4, -3, -3, -3, -2, -2, -1, -1
	.global gDaedalus_DriftPictures
gDaedalus_DriftPictures:
	.byte 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1
	.global gDaedalus_DriftStarts
gDaedalus_DriftStarts:
	.byte 112, 128, 128, 112, 112, 128, 128, 112, 128, 112, 112, 112, 180, 120, 140, 120
	.byte 140, 120, 136, 120
	.global gDaedalus_DriftSpeeds
gDaedalus_DriftSpeeds:
	.4byte -120000, -70000, -117000, -68000
	.4byte -120000, -70000, -117000, -68000
	.4byte -80000, -80000, -80000, -95000
	.4byte -100000, -80000, -100000, -90000
	.4byte -120000, -95000, -100000, -95000
	.global gDaedalus_DriftFrames
gDaedalus_DriftFrames:
	.byte 16, 16, 40, 40, 40, 68, 40, 72, 72, 96
	.global gDaedalus_ClatterFrames
gDaedalus_ClatterFrames:
	.2byte 16, 115, 243, 371, 499
	.global gDaedalus_RingStarts
gDaedalus_RingStarts:
	.2byte 568, 5
	.global gDaedalus_RingX
gDaedalus_RingX:
	.byte 66, 64
	.global gDaedalus_SpritePoints
gDaedalus_SpritePoints:
	.byte 0, 0, 32, 0, 0, 32, 32, 32, 32, 64
	.global gDaedalus_SplashPoints
gDaedalus_SplashPoints:
	.byte 60, 64, 58, 72, 56, 80, 54, 88, 54, 96, 56, 104, 78, 56, 78, 64
	.byte 80, 72, 82, 80, 84, 88, 84, 96, 76, 60, 74, 60, 72, 60, 70, 60
	.byte 68, 60, 66, 60, 60, 64, 61, 64, 62, 63, 63, 62, 64, 61, 66, 60
	.global gDaedalus_ParticleStarts
gDaedalus_ParticleStarts:
	.2byte 0, 24, 8, 52, 16, 40, 36, 28
	.2byte 32, 4, 20, 56, 60, 12, 44, 48
	.global gDaedalusAlt_StrikeFrames
gDaedalusAlt_StrikeFrames:
	.byte 80, 84, 86, 88, 90, 92, 94
	.global gVengeance_BladeStrikes
gVengeance_BladeStrikes:
	.byte 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1
	.byte 1, 1, 1, 1, 1
	.global gLifeShear_BladeMesh
gLifeShear_BladeMesh:
	.2byte 769, 2, 3711, 13695, 13568, 0, 256, 2
	.2byte 3584, 3711, 13568, 0, 0, 0, 0, 0
	.2byte 0, 0
	.global gLifeShear_HandleMesh
gLifeShear_HandleMesh:
	.2byte 769, 2, 127, 8063, 7936, 0, 256, 2
	.2byte 0, 127, 7936, 0, 0, 0, 0, 0
	.2byte 0, 0
	.global gLifeShear_ScytheCorners
gLifeShear_ScytheCorners:
	.2byte 61615, 0, 61487, 0, 4271, 0, 4143, 0
	.global gGeyser_DropGravity
gGeyser_DropGravity:
	.4byte -0x4000, -0x8000, -0x8000
	.global gGeyser_DropMinSizes
gGeyser_DropMinSizes:
	.byte 1, 4, 5
	.global gBubbleColumn_PictureWidths
gBubbleColumn_PictureWidths:
	.byte 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 4, 3
	.global gBubbleColumn_PictureHeights
gBubbleColumn_PictureHeights:
	.byte 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 0
	.global gBubbleColumn_Pictures
gBubbleColumn_Pictures:
	.2byte 0, 512, 962, 1354, 1692, 1980, 2222, 2422
	.2byte 2584, 2712, 2810, 2882, 2932, 2964, 2988
	.global gCoatlicue_PictureWidths
gCoatlicue_PictureWidths:
	.byte 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 4, 3
	.global gCoatlicue_PictureHeights
gCoatlicue_PictureHeights:
	.byte 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4
	.global gCoatlicue_Pictures
gCoatlicue_Pictures:
	.2byte 0, 512, 962, 1354, 1692, 1980, 2222, 2422
	.2byte 2584, 2712, 2810, 2882, 2932, 2964, 2988
	.global gCoatlicue_BurstFrames
gCoatlicue_BurstFrames:
	.2byte 0, 0, 10, 78, 82, 112, 250, 254
	.2byte 284
	.global gCoatlicue_BurstX
gCoatlicue_BurstX:
	.byte 32, 64, 80, 32, 16, 88, 32, 16, 88
	.global gCoatlicue_BurstY
gCoatlicue_BurstY:
	.byte 80, 96, 120, 120, 120, 120, 120, 120, 120
	.global gCoatlicue_BurstSpreadX
gCoatlicue_BurstSpreadX:
	.byte 32, 8, 32, 8, 16, 16, 8, 16, 16
	.global gCoatlicue_BurstSpacingY
gCoatlicue_BurstSpacingY:
	.byte 16, 8, 16, 8, 12, 12, 8, 12, 12
	.global gCoatlicue_BurstVelocityX
gCoatlicue_BurstVelocityX:
	.byte -24, 4, 20, 0, -2, 2, 0, -2, 2
	.global gCoatlicue_BurstVelocityY
gCoatlicue_BurstVelocityY:
	.byte -64, -32, -64, -16, -32, -32, -16, -32, -32
	.global gCoatlicue_BurstZ
gCoatlicue_BurstZ:
	.byte 1, 0, 4, -2, 1, 0, -2, 1, 0
	.global gCoatlicue_BurstVelocityZ
gCoatlicue_BurstVelocityZ:
	.byte 16, 1, 12, 0, 0, 0, 0, 0, 0
	.global gCoatlicue_PictureBias
gCoatlicue_PictureBias:
	.byte 3, 5, 7, 9
	.global gCoatlicue_RingFrames
gCoatlicue_RingFrames:
	.2byte 81, 209, 518, 540
	.global gCoatlicue_FlashFrames
gCoatlicue_FlashFrames:
	.2byte 498, 503, 508
	.global gCoatlicue_GlowFrames
gCoatlicue_GlowFrames:
	.2byte 518
	.global gCoatlicue_CoilStarts
gCoatlicue_CoilStarts:
	.byte 0, 82
	.global gCoatlicue_CoilScales
gCoatlicue_CoilScales:
	.2byte 4096, 18384
	.global gCoatlicue_CoilRolls
gCoatlicue_CoilRolls:
	.2byte 8192, 40960
	.global gCoatlicue_CoilGrowth
gCoatlicue_CoilGrowth:
	.2byte 256, 572
	.global gCoatlicue_CoilMaxScales
gCoatlicue_CoilMaxScales:
	.2byte 44160, 52720
	.global gCatastrophe_FlashFrames
gCatastrophe_FlashFrames:
	.2byte 4, 26, 30, 34, 104, 112, 116, 120
	.2byte 150, 154, 202, 214, 244, 278, 400, 402
	.2byte 404, 420
	.global gCatastrophe_FlashLengths
gCatastrophe_FlashLengths:
	.2byte 8, 2, 2, 16, 2, 2, 2, 10
	.2byte 2, 16, 4, 8, 10, 12, 2, 2
	.2byte 10, 4
	.global gCatastrophe_DemonX
gCatastrophe_DemonX:
	.byte 0, 32, 64, 96, 32, 64, 96, 128, 64, 96, 128, 160, 32, 64, 96, 128
	.byte 25, 57, 89, 121, 153, 32, 64, 96, 128, 160, 48, 96
	.global gCatastrophe_DemonZ
gCatastrophe_DemonZ:
	.byte 0, 0, 0, 0, 32, 32, 32, 32, 64, 64, 64, 64, 96, 96, 96, 96
	.byte 128, 128, 128, 128, 128, 160, 160, 160, 160, 160, 192, 192
	.global gCatastrophe_ArmourX
gCatastrophe_ArmourX:
	.byte 16, 0, 32, 0, 32, 16, 16, 16, 16, 16, 16, 0
	.global gCatastrophe_ArmourZ
gCatastrophe_ArmourZ:
	.2byte 0, 32, 32, 64, 64, 96, 128, 160
	.2byte 192, 224, 256
	.global gCatastrophe_MirroredDemonX
gCatastrophe_MirroredDemonX:
	.byte 224, 192, 160, 128, 192, 160
	.global gCatastrophe_MirroredDemonZ
gCatastrophe_MirroredDemonZ:
	.byte 0, 0, 0, 0, 32, 32
	.global gCatastrophe_BurstStarts
gCatastrophe_BurstStarts:
	.2byte 26, 104, 116, 216, 278, 402, 416
	.global gCatastrophe_BurstX
gCatastrophe_BurstX:
	.byte 16, 96, 20, 40, 50, 10, 96
	.global gCatastrophe_BurstY
gCatastrophe_BurstY:
	.byte 64, 80, 30, 40, 80, 60, 20
	.global gCatastrophe_BurstPictures
gCatastrophe_BurstPictures:
	.byte 0, 1, 0, 1, 0, 0, 1, 0
	.global gCatastrophe_CanvasFlashFrames
gCatastrophe_CanvasFlashFrames:
	.2byte 246, 249, 254, 258, 260, 264, 266, 269
	.2byte 272, 275, 278, 280, 282, 284, 287, 290
	.2byte 292
	.global gCatastrophe_StrikeX
gCatastrophe_StrikeX:
	.byte 71, 71, 71, 70, 70, 70, 70, 70, 70, 69, 69, 0, 67, 67, 67, 70
	.byte 67, 68, 70, 69, 70, 69, 67, 0, 69, 69, 69, 68, 67, 68, 70, 69
	.byte 69, 69, 0, 0
	.global gCatastrophe_StrikeY
gCatastrophe_StrikeY:
	.byte 92, 92, 92, 120, 120, 120, 154, 154, 154, 220, 200, 0, 100, 100, 100, 120
	.byte 120, 154, 154, 154, 154, 220, 240, 0, 120, 100, 100, 140, 120, 154, 154, 154
	.byte 184, 220, 0, 0
	.global gCatastrophe_StrikeFrames
gCatastrophe_StrikeFrames:
	.2byte 492, 506, 526, 528, 542, 562, 572, 578
	.2byte 598, 600, 608, 65535, 0, 504, 518, 526
	.2byte 546, 548, 554, 562, 564, 570, 590, 594
	.2byte 596, 65535, 510, 524, 544, 546, 548, 558
	.2byte 560, 580, 582, 596, 616, 65535, 0
	.global gCatastrophe_StrikeSpeedsY
gCatastrophe_StrikeSpeedsY:
	.byte 6, 4, 0, 8, 8, 0, 10, 20, 0, 20, 0, 0, 6, 4, 0, 8
	.byte 8, 0, 10, 20, 8, 20, 0, 0, 6, 4, 0, 8, 8, 20, 0, 0
	.byte 20, 20, 0, 0
	.global gCatastrophe_StrikeRolls
gCatastrophe_StrikeRolls:
	.2byte 144, 0, 0, 128, 0, 0, 180, 0
	.2byte 0, 150, 0, 0, 112, 0, 0, 0
	.2byte 112, 48, 0, 86, 0, 0, 112, 0
	.2byte 96, 0, 0, 96, 0, 112, 128, 0
	.2byte 104, 0, 0, 0
	.global gCatastrophe_StrikeTurns
gCatastrophe_StrikeTurns:
	.2byte 256, 64, 0, 712, 500, 0, 0, 0
	.2byte 0, 356, 0, 0, -356, -124, -256, 712
	.2byte -64, 256, 0, 0, -228, 356, 0, 0
	.2byte 406, 64, -256, 0, -64, 0, 128, 0
	.2byte 328, -356, 0, 0
	.global gCatastrophe_StrikePictures
gCatastrophe_StrikePictures:
	.2byte 0, 1, -1, 0, 1, -1, 0, 1
	.2byte -1, 0, -1, 0, 2, 1, 0, -1
	.2byte 2, 2, -1, 0, 1, -1, 2, -1
	.2byte 0, 1, -1, 0, -1, 0, 1, -1
	.2byte 0, 1, -1, 0
	.global gCatastrophe_GlintFlips
gCatastrophe_GlintFlips:
	.byte 0, 4, 8, 12
	.global gCatastrophe_ShardRolls
gCatastrophe_ShardRolls:
	.2byte 14797, -1097, -8198, 4962, 15494, -5489, -9298, 3744
	.2byte 11252, -9996, 15443, -4373, -15930, 15898, -13623, 3906
	.global gHaures_FeatherPoints
gHaures_FeatherPoints:
	.2byte 0, 0, 0, 32, 32, 32, 64, 32
	.2byte 96, 42, 0, 64, 32, 64, 64, 64
	.2byte 96, 74, 32, 96, 64, 96
	.global gHaures_ParticleX
gHaures_ParticleX:
	.byte 120, 88, 144, 112, 0, 0
	.global gHaures_UnitScale
gHaures_UnitScale:
	.4byte 0x10000, 0x10000
	.global gHaures_WingShapes
gHaures_WingShapes:
	.byte 0, 0, 0, 2, 6, 9, 10, 11, 13, 14, 15, 16, 17, 18, 18, 16
	.byte 16, 16, 17, 16, 15, 14, 13, 12, 11, 9, 8, 5, 0, 0, 0, 0
	.byte 0, 2, 8, 12, 13, 16, 18, 21, 24, 27, 31, 34, 36, 34, 32, 28
	.byte 18, 18, 21, 29, 35, 40, 40, 38, 37, 32, 25, 15, 10, 8, 4, 0
	.byte 0, 8, 11, 14, 17, 19, 22, 24, 28, 31, 35, 45, 58, 54, 49, 46
	.byte 47, 56, 103, 120, 118, 98, 52, 45, 49, 49, 38, 32, 26, 18, 8, 0
	.byte 0, 8, 13, 16, 20, 22, 26, 29, 31, 36, 42, 47, 52, 59, 65, 69
	.byte 70, 69, 66, 63, 59, 56, 53, 49, 46, 42, 37, 32, 27, 18, 7, 0
	.byte 0, 8, 13, 18, 22, 26, 40, 54, 59, 57, 39, 31, 32, 37, 48, 66
	.byte 73, 61, 49, 54, 48, 45, 45, 31, 32, 44, 41, 32, 24, 20, 14, 0
	.byte 0, 8, 11, 16, 22, 27, 55, 90, 87, 76, 66, 55, 45, 45, 53, 58
	.byte 58, 50, 31, 37, 42, 48, 57, 56, 37, 29, 31, 31, 32, 31, 27, 0
	.byte 0, 10, 17, 25, 32, 42, 46, 48, 51, 55, 60, 66, 72, 77, 80, 84
	.byte 85, 83, 76, 63, 53, 49, 45, 43, 39, 35, 32, 27, 22, 18, 10, 0
	.byte 0, 10, 16, 21, 26, 29, 36, 46, 58, 69, 77, 78, 74, 67, 59, 52
	.byte 52, 59, 65, 69, 71, 72, 71, 70, 59, 66, 62, 44, 26, 20, 13, 0
	.byte 0, 9, 15, 21, 27, 29, 34, 43, 47, 51, 53, 55, 58, 62, 67, 69
	.byte 72, 72, 71, 67, 63, 60, 56, 53, 51, 45, 38, 26, 17, 10, 8, 0
	.byte 0, 8, 12, 20, 27, 52, 73, 74, 68, 60, 53, 46, 41, 40, 48, 61
	.byte 72, 77, 78, 82, 88, 93, 97, 32, 35, 40, 44, 40, 28, 15, 8, 0
	.byte 0, 8, 16, 34, 44, 50, 56, 60, 65, 69, 73, 77, 79, 82, 83, 77
	.byte 72, 68, 63, 59, 54, 40, 26, 23, 26, 29, 31, 32, 26, 18, 8, 0
	.byte 0, 8, 11, 18, 30, 56, 77, 86, 92, 93, 92, 90, 88, 85, 82, 79
	.byte 76, 72, 70, 66, 62, 60, 56, 51, 47, 42, 36, 24, 15, 10, 8, 0
	.byte 0, 8, 83, 96, 104, 110, 115, 118, 120, 116, 114, 114, 115, 116, 119, 121
	.byte 120, 117, 114, 111, 108, 106, 102, 100, 97, 87, 30, 22, 15, 10, 8, 0
	.global gHaures_WingMorphSteps
gHaures_WingMorphSteps:
	.byte 27, 15, 13, 5, 14, 16, 11, 13, 8, 18, 24, 26, 100
	.global gHaures_PartPoints
gHaures_PartPoints:
	.byte 23, 13, 0, 0, 42, 0, 28, 41, 28, 41, 48, 28, 42, 0, 78, 31
	.byte 28, 41, 28, 41, 66, 7, 42, 0, 79, 0, 78, 31, 78, 31, 94, 8
	.byte 79, 0, 128, 0, 78, 31, 78, 31, 112, 28, 128, 0, 132, 48, 78, 31
	.byte 78, 31, 162, 12, 182, 0, 174, 42, 128, 0, 128, 0, 145, 39, 128, 0
	.byte 174, 42, 134, 71, 134, 71, 196, 12, 182, 0, 239, 0, 174, 42, 172, 42
	.byte 231, 20, 239, 0, 239, 52, 211, 16, 211, 16, 7, 33, 0, 0, 28, 41
	.byte 0, 48, 0, 48, 59, 49, 78, 31, 69, 73, 28, 41, 28, 41, 91, 51
	.byte 78, 31, 132, 48, 69, 73, 69, 73, 16, 58, 28, 41, 20, 86, 0, 48
	.byte 0, 48, 36, 70, 28, 41, 69, 73, 20, 86, 20, 86, 96, 76, 132, 48
	.byte 99, 97, 69, 73, 69, 73, 6, 80, 0, 48, 20, 86, 0, 98, 0, 98
	.byte 168, 66, 174, 42, 205, 88, 134, 71, 134, 71, 194, 48, 211, 16, 205, 88
	.byte 174, 42, 174, 42, 217, 52, 211, 16, 239, 52, 205, 88, 208, 88, 7, 114
	.byte 0, 98, 30, 119, 0, 119, 0, 119, 29, 106, 20, 86, 64, 119, 30, 119
	.byte 0, 98, 51, 93, 69, 73, 64, 119, 20, 86, 20, 86, 78, 96, 69, 73
	.byte 99, 97, 64, 119, 64, 119, 98, 112, 99, 97, 137, 104, 112, 119, 64, 119
	.byte 123, 85, 132, 48, 137, 104, 99, 97, 99, 97, 156, 88, 134, 71, 205, 88
	.byte 137, 104, 137, 104, 139, 115, 137, 104, 175, 119, 112, 119, 112, 119, 217, 52
	.byte 211, 16, 239, 52, 205, 88, 205, 88, 228, 87, 239, 52, 239, 119, 205, 88
	.byte 205, 88, 200, 108, 185, 93, 205, 88, 239, 119, 175, 119, 169, 106, 185, 93
	.byte 175, 119, 137, 104, 137, 104
	.global gHaures_BandStarts
gHaures_BandStarts:
	.byte 10, 30
	.global gHelmSplitter_PictureWidths
gHelmSplitter_PictureWidths:
	.byte 26, 46, 63, 77, 60, 39, 32
	.global gHelmSplitter_PictureHeights
gHelmSplitter_PictureHeights:
	.byte 11, 20, 37, 65, 65, 54, 39, 0
	.global gHelmSplitter_Pictures
gHelmSplitter_Pictures:
	.2byte 0, 286, 1206, 3537, 8542, 12442, 14548
	.global gHelmSplitter_PictureBiasX
gHelmSplitter_PictureBiasX:
	.byte 53, 36, 16, 2, 2, 2, 2
	.global gHelmSplitter_PictureBiasY
gHelmSplitter_PictureBiasY:
	.byte 2, 3, 3, 3, 4, 14, 29
	.global gDragonCloud_SparkCounts
gDragonCloud_SparkCounts:
	.byte 48, 40, 0, 0
	.global gElementCards_Frames
gElementCards_Frames:
	.byte 124, 140, 132, 130
	.global gElementCards_SparkMotions
gElementCards_SparkMotions:
	.4byte 0x2000, 62, 1, 0x2000
	.4byte 62, 1, -0x2000, 62
	.4byte 1, 2048, 64, 4
	.global gTripleChomp_BiteFrames
gTripleChomp_BiteFrames:
	.byte 0, 12, 24
	.global gBreaths_Frames
gBreaths_Frames:
	.byte 90, 38, 32, 48
	.global gBreaths_BurstFrames
gBreaths_BurstFrames:
	.byte 56, 0, 0, 0
	.global gBreaths_MaxSizes
gBreaths_MaxSizes:
	.byte 24, 32, 32, 24
	.global gBreaths_RingStarts
gBreaths_RingStarts:
	.byte 56, 58, 60, 61, 8, 12, 16, 20, 0, 2, 4, 5
	.global gBreaths_RingGrowth
gBreaths_RingGrowth:
	.byte 5, 6, 7, 6, 10, 9, 11, 10, 5, 6, 7, 6
	.global gBreaths_RingX
gBreaths_RingX:
	.byte -32, -48, -56, -72, -16, -32, -64, -72, -32, -48, -56, -72
	.global gBreaths_FlashFrames
gBreaths_FlashFrames:
	.byte 56, 14, 6, 56
	.global gTrident_RingStarts
gTrident_RingStarts:
	.byte 8, 10, 12, 13, 64, 67, 70, 72
	.global gTrident_RingGrowth
gTrident_RingGrowth:
	.byte 8, 7, 6, 7, 8, 8, 9, 8
	.global gTrident_RingHeights
gTrident_RingHeights:
	.byte 16, 0, -24, -40, 16, 0, -24, -40, 0
	.global gTrident_Spins
gTrident_Spins:
	.4byte 5528, 488, 62272, 308
	.4byte 52392, -698, 34584, 634
	.4byte 19692, 736, 28062, 746
	.4byte 48958, -694, 1508, -576
	.global gTrident_ProngCorners
gTrident_ProngCorners:
	.2byte 32762, 0, 32518, 0, 64762, 0, 64518, 0
	.2byte 32762, 0, 32518, 0, 250, 0, 6, 0
	.2byte 32758, 0, 32514, 0, 65526, 0, 65282, 0
	.global gTrident_GlintFlips
gTrident_GlintFlips:
	.byte 0, 4, 8, 12
	.global gLeapingStrike_WaveStarts
gLeapingStrike_WaveStarts:
	.byte 0, 4, 8, 9
	.global gLeapingStrike_WaveGrowth
gLeapingStrike_WaveGrowth:
	.byte 9, 8, 9, 3
	.global gLeapingStrike_WaveHeights
gLeapingStrike_WaveHeights:
	.byte 32, 32, 32, 32
	.global gLeapingStrike_RingStarts
gLeapingStrike_RingStarts:
	.byte 0, 4, 12, 19
	.global gLeapingStrike_RingGrowth
gLeapingStrike_RingGrowth:
	.byte 7, 8, 7, 8
	.global gLeapingStrike_RingHeights
gLeapingStrike_RingHeights:
	.byte 32, 16, 16, 24
	.global gMeteorBlow_PictureWidths
gMeteorBlow_PictureWidths:
	.byte 30, 16, 16, 16, 10, 8, 8
	.global gMeteorBlow_PictureHeights
gMeteorBlow_PictureHeights:
	.byte 30, 16, 16, 16, 16, 8, 8
	.global gMeteorBlow_Pictures
gMeteorBlow_Pictures:
	.2byte 0, 900, 1156, 1412, 1668, 1828, 1892
	.global gMeteorBlow_WaveStarts
gMeteorBlow_WaveStarts:
	.byte 0, 4, 9, 10
	.global gMeteorBlow_WaveGrowth
gMeteorBlow_WaveGrowth:
	.byte 3, 4, 3, 5
	.global gMeteorBlow_WaveStretch
gMeteorBlow_WaveStretch:
	.byte 3, 4, 5, 6
	.global gMeteorBlow_RingStarts
gMeteorBlow_RingStarts:
	.byte 0, 3, 16, 7
	.global gMeteorBlow_RingGrowth
gMeteorBlow_RingGrowth:
	.byte 5, 8, 10, 8
	.global gMeteorBlow_RingHeights
gMeteorBlow_RingHeights:
	.byte -40, 24, 24, 24
	.global gLiquifier_SplashCorners
gLiquifier_SplashCorners:
	.2byte 49360, 0, 49200, 0, 1248, 0, 1056, 0
	.global gLiquifier_SplashStarts
gLiquifier_SplashStarts:
	.byte 18, 21, 16, 17, 22, 19, 23, 18
	.global gLiquifier_SplashHeights
gLiquifier_SplashHeights:
	.byte 40, 56, 70, 48, 64, 80, 58, 64, 72
	.global gCanvasBurst_FlamePictures
gCanvasBurst_FlamePictures:
	.byte 5, 5, 4, 3, 2, 1, 0
	.global gMegiddo_FlashPitches
gMegiddo_FlashPitches:
	.2byte 0, -10240, 0, -10240, 10240
	.global gMegiddo_FlashYaws
gMegiddo_FlashYaws:
	.2byte 0, -4864, 0, -4864, -4864
	.global gMegiddo_FlashStarts
gMegiddo_FlashStarts:
	.2byte 4, 8, 171, 175, 176
	.global gMegiddo_RayPitches
gMegiddo_RayPitches:
	.2byte -12544, -10496, -8704, 8704, -8448, 12800, 10752, 11264
	.global gMegiddo_RayYaws
gMegiddo_RayYaws:
	.2byte 0, 11776, 0, -14336, 11264, -19968, -22016, -22272
	.global gMegiddo_RayRolls
gMegiddo_RayRolls:
	.2byte -21504, 13312, 22224, -18944, -32768, 8448, 25600, -28928
	.global gMegiddo_RayX
gMegiddo_RayX:
	.byte 6, 9, 6, 2, 6, 3, 6, 7
	.global gMegiddo_RayY
gMegiddo_RayY:
	.byte -6, -4, -7, -9, -6, -7, -9, -7
	.global gOdyssey_FlashPitches
gOdyssey_FlashPitches:
	.2byte 0, -10240
	.global gOdyssey_FlashYaws
gOdyssey_FlashYaws:
	.2byte 0, -4864, 0
	.global gMoloch_RingStarts
gMoloch_RingStarts:
	.byte 0, 6, 8, 9
	.global gMoloch_RingGrowth
gMoloch_RingGrowth:
	.byte 7, 8, 7, 8
	.global gMoloch_RingHeights
gMoloch_RingHeights:
	.byte 24, 12, -24, 0
	.global gIris_SplashRadii
gIris_SplashRadii:
	.4byte 24, 16
	.global gIris_SplashOffsetsY
gIris_SplashOffsetsY:
	.byte 7, 5, 0, 0
	.global gIris_PillarMesh
gIris_PillarMesh:
	.2byte 256, 2, 0, 15, 3840, 0, 258, 3
	.2byte 3840, 15, 3855, 0, 770, 4, 3584, 3599
	.2byte 3840, 0, 772, 5, 3840, 3599, 3855, 0
	.2byte 0, 0, 0, 0, 0, 0
	.global gIris_ParticleCorners
gIris_ParticleCorners:
	.2byte 63740, 0, 63492, 0, 2300, 0, 2052, 0
	.global gIris_ParticleMesh
gIris_ParticleMesh:
	.4byte 0x20100, 0x1f0000, 0x3f00, 196866
	.4byte 0x1f3f00, 16159, 0, 0
	.4byte 0
	.global gIris_PsynergySoundFrames
gIris_PsynergySoundFrames:
	.byte 84, 93, 108, 111, 116, 132, 135, 141, 153, 158, 171, 0
	.global gIris_DoorSoundFrames
gIris_DoorSoundFrames:
	.2byte 231, 244, 259, 265, 270
	.global gIris_PowerUpSoundFrames
gIris_PowerUpSoundFrames:
	.2byte 279, 287, 293, 298, 302, 307
	.global gIris_StreakX
gIris_StreakX:
	.byte -10, 80, 110, -10, 76, 50, 110, 80, 40, 30, 80, 20, 50, 80, 20
	.global gIris_StreakY
gIris_StreakY:
	.byte 116, 136, 128, 112, 136, 136, 128, 136, 136, 136, 136, 136, 136, 136, 136
	.global gIris_StreakAngles
gIris_StreakAngles:
	.byte 208, 71, 24, 244, 208, 199, 0, 64, 24, 244, 232, 11, 208, 199, 24, 244
	.byte 208, 15, 160, 23, 72, 236, 136, 27, 232, 11, 24, 244, 160, 23
	.global gIris_StreakSpeeds
gIris_StreakSpeeds:
	.2byte -406, 64, 380, -326, 64, -64, 380, 64
	.2byte -84, -64, 94, -94, -64, 64, -64
	.global gIris_LineShades
gIris_LineShades:
	.byte 23, 47, 56, 63, 63, 56, 47, 23
	.global gIris_MoteSizes
gIris_MoteSizes:
	.4byte 7, 5
	.global gIris_SplashStarts
gIris_SplashStarts:
	.2byte 230, 238, 260, 265, 253, 258
	.global gIris_SplashEnds
gIris_SplashEnds:
	.2byte 320, 328, 330, 335, 326, 332
	.global gIris_SplashX
gIris_SplashX:
	.byte 34, 86, 18, 102, 8, 112
	.global gIris_SplashY
gIris_SplashY:
	.byte 64, 62, 32, 32, 80, 80
	.global gIris_SplashSmall
gIris_SplashSmall:
	.byte 0, 0, 1, 1, 0, 0
	.global gIris_DropSpeeds
gIris_DropSpeeds:
	.byte 8, 4, 1, 2, 4, 8
	.global gIris_DropFrames
gIris_DropFrames:
	.byte 16, 16, 16, 16, 32, 32
	.global gIris_DropPictureSteps
gIris_DropPictureSteps:
	.byte 1, 1, 1, 1, 2, 2
	.global gIris_LeftStripCorners
gIris_LeftStripCorners:
	.2byte 49402, 0, 49170, 0, 57591, 0, 57359, 0
	.2byte 243, 0, 8, 0, 8429, 0, 8447, 0
	.2byte 16612, 0, 16628, 0
	.global gIris_RightStripCorners
gIris_RightStripCorners:
	.2byte 49187, 0, 49224, 0, 57372, 0, 57410, 0
	.2byte 18, 0, 51, 0, 8198, 0, 8225, 0
	.2byte 16628, 0, 16393, 0
	.global gIris_StripMesh
gIris_StripMesh:
	.byte 0, 1, 2, 0, 0, 0, 31, 0, 0, 15, 0, 0, 2, 1, 3, 0
	.byte 0, 15, 31, 0, 31, 15, 0, 0, 2, 3, 4, 0, 0, 15, 31, 15
	.byte 0, 31, 0, 0, 4, 3, 5, 0, 0, 31, 31, 15, 31, 31, 0, 0
	.byte 4, 5, 6, 0, 0, 31, 31, 31, 0, 47, 0, 0, 6, 5, 7, 0
	.byte 0, 47, 31, 31, 31, 47, 0, 0, 6, 7, 8, 0, 0, 47, 31, 47
	.byte 0, 63, 0, 0, 8, 7, 9, 0, 0, 63, 31, 47, 31, 63, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.global gIris_ConeCorners
gIris_ConeCorners:
	.2byte 55296, 0, 64763, 0, 64517, 0, 4342, 0
	.2byte 4106, 0, 8428, 0, 8212, 0, 0, 0
	.global gIris_ConeMesh
gIris_ConeMesh:
	.4byte 458753, 0x1f00, 7951, 197121
	.4byte 0x1f0000, 0x1f00, 262659, 0x1f1f00
	.4byte 7967, 328707, 0x1f0000, 0x1f00
	.4byte 394245, 0x1f1f00, 7967, 131079
	.4byte 2039567, 7967
