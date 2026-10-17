	.section .rodata
@ Battle effect tables, in the order of the effect code that reads them, as
@ The Broken Seal's effect/effect_table.c. Cell offsets count bytes from the
@ start of a sheet; widths and heights are pixels. Labels keep their
@ placeholders: most readers are not C yet.
@ The Broken Seal's gBattleFx_MoteCellOffsets; read by 84 functions
	.global gData_08197410
gData_08197410:
	.2byte 0, 2, 10, 28, 60, 110, 182, 280
	.2byte 408, 570
@ The Broken Seal's gReelGame_SparkCellOffsets; read by 10 functions
	.global Data_08197424
Data_08197424:
	.2byte 0, 4, 20, 56, 120, 220, 364, 560
	.2byte 816, 1140
@ read by Func_0813fd84
	.global Data_08197438
Data_08197438:
	.2byte 0, 8, 40, 112, 240, 440, 728, 1120
	.2byte 1632, 2280
@ The Broken Seal's gBattleFx_PuffCells; read by 6 functions
	.global Data_0819744c
Data_0819744c:
	.2byte 0, 25, 74, 123, 172, 221, 246, 255
	.2byte 256
@ The Broken Seal's gBattleFx_PuffSizes; read by 6 functions
	.global Data_0819745e
Data_0819745e:
	.byte 5, 7, 7, 7, 7, 5, 3, 1, 1
@ The Broken Seal's gBattleFx_PuffArcCellWidths; read by 7 functions
	.global gData_08197467
gData_08197467:
	.byte 24, 29, 30, 27, 24, 16
@ The Broken Seal's gBattleFx_PuffArcCellHeights; read by 7 functions
	.global gData_0819746d
gData_0819746d:
	.byte 23, 32, 37, 49, 53, 52
@ The Broken Seal's gBattleFx_PuffArcCellBiasY; read by 7 functions
	.global gData_08197473
gData_08197473:
	.byte 41, 32, 26, 11, 4, 2
	.balign 2
@ The Broken Seal's gBattleFx_PuffArcCellSourceOffsets; read by 7 functions
	.global gData_0819747a
gData_0819747a:
	.2byte 0, 552, 1480, 2590, 3913, 5185
@ The Broken Seal's gBattleFx_GlintCellOffsets; read by 14 functions
	.global gData_08197486
gData_08197486:
	.2byte 0, 589, 1302, 2015, 2414, 2750
@ The Broken Seal's gBattleFx_GlintCellWidths; read by 14 functions
	.global gData_08197492
gData_08197492:
	.byte 19, 23, 23, 19, 16, 17
@ The Broken Seal's gBattleFx_GlintCellHeights; read by 14 functions
	.global gData_08197498
gData_08197498:
	.byte 31, 31, 31, 21, 21, 24
@ The Broken Seal's gSpire_ShardWidths; read by 4 functions
	.global gData_0819749e
gData_0819749e:
	.byte 5, 11, 10, 7, 7, 12, 13, 9, 19, 12, 17, 22, 15, 16, 14
@ read by 4 functions
	.global gData_081974ad
gData_081974ad:
	.byte 11, 6, 10, 11, 10, 7, 17, 21, 12, 21, 17, 13, 16, 16, 15
@ The Broken Seal's gSpire_ShardCells; read by 4 functions
	.global gData_081974bc
gData_081974bc:
	.2byte 0, 55, 121, 221, 298, 368, 452, 673
	.2byte 862, 1090, 1342, 1631, 1917, 2157, 2413, 2623
@ The Broken Seal's gPaletteRamp_ShardCells; read by SummonFx_RunCanvasEffect and Func_0818ad40
	.global Data_081974dc
Data_081974dc:
	.2byte 0, 120, 180, 258, 298, 352, 408, 471
	.2byte 567, 607, 631, 661
@ The Broken Seal's gPaletteRamp_ShardWidths; read by SummonFx_RunCanvasEffect and Func_0818ad40
	.global Data_081974f4
Data_081974f4:
	.byte 10, 5, 6, 5, 6, 7, 9, 8, 4, 6, 6, 7
@ The Broken Seal's gPaletteRamp_ShardHeights; read by SummonFx_RunCanvasEffect and Func_0818ad40
	.global Data_08197500
Data_08197500:
	.byte 12, 12, 13, 8, 9, 8, 7, 12, 10, 4, 5, 5
@ The Broken Seal's gCondemn_PanelX; read by Func_0815f16c and Func_0818ad40
	.global gData_0819750c
gData_0819750c:
	.byte 10, 30, 0, 0, 0, 6, 52, 60, 0, 0, 0, 0, 0, 0
@ read by Func_0815f16c and Func_0818ad40
	.global gData_0819751a
gData_0819751a:
	.byte 0, 20, 2, 2, 34, 64, 68
@ The Broken Seal's gIce_Counts; read by Func_0813bbd8
	.global Data_08197521
Data_08197521:
	.byte 16, 46, 20, 52, 32, 80
@ The Broken Seal's gFallingBolts_Counts; read by Func_0813c0b8
	.global Data_08197527
Data_08197527:
	.byte 6, 8, 32, 80, 8, 16, 24, 80, 32, 4, 8, 80
@ The Broken Seal's gFiveMode_Records; read by Func_0813c5f4
	.global Data_08197533
Data_08197533:
	.byte 0, 1, 0, 0, 0, 1, 4, 0, 0, 1, -1, 1, 1, 1, -1, 1
	.byte 1, 1, 4, 0
@ The Broken Seal's gFiveMode_Tremble; read by Func_0813c5f4
	.global Data_08197547
Data_08197547:
	.byte -1, -1, 1, 1, 1, 1, -1, -1
@ read by Func_0813d098
	.global Data_0819754f
Data_0819754f:
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
@ The Broken Seal's gTwelveMode_StrikeWidths; read by Func_0813d098
	.global Data_081975e2
Data_081975e2:
	.byte 38, 54, 71, 74, 77, 65
@ The Broken Seal's gTwelveMode_StrikeHeights; read by Func_0813d098
	.global Data_081975e8
Data_081975e8:
	.byte 7, 9, 11, 13, 13, 11
@ The Broken Seal's gTwelveMode_StrikeOffsets; read by Func_0813d098
	.global Data_081975ee
Data_081975ee:
	.2byte 3200, 3466, 3952, 4733, 5695, 6696
@ The Broken Seal's gTwelveMode_StrikeReach; read by Func_0813d098
	.global Data_081975fa
Data_081975fa:
	.byte 58, 42, 25, 13, 5, 0
@ The Broken Seal's gTwelveMode_ColumnStages; read by Func_0813d098
	.global Data_08197600
Data_08197600:
	.byte 0, 1, 2, 3, 2
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by Func_0813d098
	.global Data_08197605
Data_08197605:
	.byte 0, 4, 8, 12
@ The Broken Seal's gBattlePieceWidth; read by Func_0813eb70
	.global Data_08197609
Data_08197609:
	.byte 21, 20, 24, 30, 35, 69, 19, 23, 9, 22, 33, 10, 18, 9, 7, 9
	.byte 17, 55, 30, 25, 40, 29, 10, 28, 14, 5, 32, 23, 19, 32, 8, 43
	.byte 15
@ read by Func_0813eb70
	.global Data_0819762a
Data_0819762a:
	.byte 23, 47, 25, 40, 20, 53, 28, 29, 23, 14, 35, 17, 18, 14, 15, 12
	.byte 13, 37, 38, 41, 60, 41, 15, 45, 59, 41, 26, 50, 21, 35, 12, 57
	.byte 11, 0
@ The Broken Seal's gBattlePieceOffset, byte for byte
	.global Data_0819764c
Data_0819764c:
	.2byte 0, 483, 1423, 2023, 3223, 3923, 7580, 8112, 8779, 8986, 9294
	.2byte 10449, 10619, 10943, 11069, 11174, 11282, 11503, 13538, 14678, 15703, 18103
	.2byte 19292, 19442, 20702, 21528, 21733, 22565, 23715, 24114, 25234, 25330, 27781
	.2byte 27946
@ The Broken Seal's gBattlePieceStartX; read by Func_0813eb70
	.global Data_08197690
Data_08197690:
	.byte 0, 21, 28, 46, 70, 51, 0, 13, 37, 77, 87, 19, 25, 35, 44, 45
	.byte 48, 65, 0, 0, 0, 5, 33, 15, 36, 46, 47, 50, 53, 65, 72, 77
	.byte 105
@ The Broken Seal's gBattlePieceStartY; read by Func_0813eb70
	.global Data_081976b1
Data_081976b1:
	.byte 0, 0, 0, 0, 0, 0, 19, 17, 23, 28, 8, 36, 43, 43, 40, 47
	.byte 48, 37, 42, 67, 60, 79, 62, 75, 61, 62, 60, 70, 74, 85, 108, 63
	.byte 74
@ The Broken Seal's gCornerSparks_X; read by Func_0813f518
	.global Data_081976d2
Data_081976d2:
	.byte 0, 32, 0, 32
@ The Broken Seal's gCornerSparks_Y; read by Func_0813f518
	.global Data_081976d6
Data_081976d6:
	.byte 0, 0, 32, 32
@ The Broken Seal's gRenderMode_GlintFlips; read by Func_0813f518
	.global Data_081976da
Data_081976da:
	.byte 0, 4, 8, 12
	.balign 4
@ read by Func_0813fd84
	.global Data_081976e0
Data_081976e0:
	.4byte 32764, 32516, 250, 6
@ read by Func_0813fd84
	.global Data_081976f0
Data_081976f0:
	.2byte 10240, 56579, 56579, 58880
@ read by Func_0813fd84
	.global Data_081976f8
Data_081976f8:
	.2byte 39680, 42496, 32768, 22016
@ read by Func_0813fd84
	.global Data_08197700
Data_08197700:
	.byte 24, 48
@ read by Func_0813fd84
	.global Data_08197702
Data_08197702:
	.byte 24, 32
@ read by Func_0813fd84
	.global Data_08197704
Data_08197704:
	.byte 7, 37, 45, 58, 74, 80
@ read by Func_0813fd84
	.global Data_0819770a
Data_0819770a:
	.byte 96, 48, 64, 64, 96, 128
@ read by Func_0813fd84
	.global Data_08197710
Data_08197710:
	.byte 48, 48, 32, 0, 0, 24
@ read by Func_0813fd84
	.global Data_08197716
Data_08197716:
	.byte 96, 96, 96, 96, 120, 120
@ read by Func_0813fd84
	.global Data_0819771c
Data_0819771c:
	.byte 2, 26, 104, 150
@ read by Func_0813fd84
	.global Data_08197720
Data_08197720:
	.byte 32, 16, 24, 64
@ read by Func_0813fd84
	.global Data_08197724
Data_08197724:
	.byte 96, 48, 24, 16
@ read by Func_0813fd84
	.global Data_08197728
Data_08197728:
	.byte 16, 8, 12, 8
@ read by Func_0813fd84
	.global Data_0819772c
Data_0819772c:
	.byte 16, 4, 16, 24
@ read by Func_0813fd84
	.global Data_08197730
Data_08197730:
	.byte 96, 96, 96, 120
@ read by Func_0813fd84
	.global Data_08197734
Data_08197734:
	.4byte 0x9000, 0xa000, 0x9000, 0x9000
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by Func_0814153c
	.global Data_08197744
Data_08197744:
	.byte 0, 4, 8, 12
@ read by Func_0814153c
	.global Data_08197748
Data_08197748:
	.byte 8, 9, 25, 14, 15, 20, 21
@ read by Func_0814153c
	.global Data_0819774f
Data_0819774f:
	.byte 148, 160, 162, 164, 170, 174, 178, 182, 188, 194, 196, 200, 202, 206, 208, 210
	.byte 220
@ read by Func_0814153c
	.global Data_08197760
Data_08197760:
	.4byte 404232216, 269488144, 269488144, 404232216
	.4byte 741092396, 741092396, 741092396, 741092396
	.4byte 741092396, 741092396, 269488144, 269488144
	.4byte 269488144, 741092396, 741092396, 269488144
	.4byte 269488144
@ read by Func_0814153c
	.global Data_081977a4
Data_081977a4:
	.byte 0, 32, 64, 96, 160, 192, 0, 32, 64, 96, 128, 160, 192, 224, 64, 96
	.byte 128, 160, 192, 224, 64, 96, 128, 160, 192, 224, 64, 96, 128, 160, 192, 224
@ read by Func_0814153c
	.global Data_081977c4
Data_081977c4:
	.byte 0, 0, 0, 0, 0, 0, 32, 32, 32, 32, 32, 32, 32, 32, 64, 64
	.byte 64, 64, 64, 64, 96, 96, 96, 96, 96, 96, 128, 128, 128, 128, 128, 128
@ read by Func_0814153c
	.global Data_081977e4
Data_081977e4:
	.byte 4, 3, 5, 3
@ read by Func_0814153c
	.global Data_081977e8
Data_081977e8:
	.byte 16, 12, 4, -8
@ read by Func_08142944
	.global Data_081977ec
Data_081977ec:
	.byte 0, 4, 9, 10
@ read by Func_08142944
	.global Data_081977f0
Data_081977f0:
	.byte 3, 4, 3, 5
@ read by Func_08142944
	.global Data_081977f4
Data_081977f4:
	.byte 3, 4, 5, 6
@ The Broken Seal's gPrism_ShardStarts; read by Func_08149268 and Func_0815f16c
	.global gData_081977f8
gData_081977f8:
	.byte 32, 60, 33, 82, 49, 94, 41, 89, 49, 76, 32, 99, 54, 69, 34, 82
	.byte 45, 71, 31, 91, 32, 69, 26, 80, 40, 68, 41, 60, 46, 60, 36, 63
	.byte 0, 0
@ The Broken Seal's gPrism_ShardWidths; read by 5 functions
	.global gData_0819781a
gData_0819781a:
	.byte 5, 10, 11, 14, 12, 11, 19, 18, 23, 12, 22, 22
@ read by 5 functions
	.global gData_08197826
gData_08197826:
	.byte 14, 10, 7, 13, 14, 14, 32, 30, 24, 38, 20, 20, 0, 0
@ The Broken Seal's gPrism_ShardOffsets, byte for byte
	.global gData_08197834
gData_08197834:
	.4byte 0, 70, 170, 247, 429, 597
	.4byte 751, 1359, 1899, 2451, 2907, 3347
@ read by Func_08144628
	.global Data_08197864
Data_08197864:
	.byte 16, 88, 8, 72, 12, 80, 16, 88
@ The Broken Seal's gMemberBeam_MotePictures; read by Func_08144b14
	.global Data_0819786c
Data_0819786c:
	.byte 2, 1, 0, 1, 2, 3, 4, 3
@ The Broken Seal's gEarthWall_CellSourceOffsets; read by Func_08144b14
	.global Data_08197874
Data_08197874:
	.2byte 2702, 3535, 4580
@ The Broken Seal's gEarthWall_CellWidths; read by Func_08144b14
	.global Data_0819787a
Data_0819787a:
	.byte 17, 19, 20
@ The Broken Seal's gEarthWall_CellHeights; read by Func_08144b14
	.global Data_0819787d
Data_0819787d:
	.byte 49, 55, 64
@ read by Func_0814512c
	.global Data_08197880
Data_08197880:
	.byte 0, 0, 1, 72, 0, 0, 1, 72, 0, 0, 1, 72, 1, 0, 1, 40
	.byte 1, 0, 1, 56, 0, 0, 1, 56, 0, 0, 1, 72, 1, 0, 4, 52
	.byte 0, 1, 1, 72, 0, 0, 1, 72
@ The Broken Seal's gSevenMode_ShardWidths; read by Func_0814512c
	.global Data_081978a8
Data_081978a8:
	.byte 9, 10, 9, 9, 9, 11, 11, 11, 12, 6, 7, 7, 7, 6, 6, 8
	.byte 8, 8
@ The Broken Seal's gSevenMode_ShardHeights; read by Func_0814512c
	.global Data_081978ba
Data_081978ba:
	.byte 12, 12, 12, 12, 12, 12, 12, 12, 12, 8, 7, 6, 6, 8, 6, 4
	.byte 3, 3
@ The Broken Seal's gSevenMode_ShardOffsets; read by Func_0814512c
	.global Data_081978cc
Data_081978cc:
	.2byte 0, 108, 228, 336, 444, 552, 684, 816
	.2byte 948, 1092, 1140, 1189, 1231, 1273, 1321, 1357
	.2byte 1389, 1413
@ The Broken Seal's gCondemn_PanelX; read by Func_08145784
	.global Data_081978f0
Data_081978f0:
	.byte 10, 30, 0, 0, 0, 6, 52, 60, 0, 0, 0, 0, 0, 0
@ The Broken Seal's gCondemn_PanelY; read by Func_08145784
	.global Data_081978fe
Data_081978fe:
	.byte 0, 20, 2, 2, 34, 64, 68, 0, 9, 4, 4, 4, 4, 4
@ The Broken Seal's gBind_Vertex; read by Func_08145df8
	.global Data_0819790c
Data_0819790c:
	.4byte 0, 24, 0
@ The Broken Seal's gBind_Vertex; read by Func_0814627c
	.global Data_08197918
Data_08197918:
	.4byte 0, 24, 0
@ The Broken Seal's gAstralBlast_Points; read by Func_08146638
	.global gData_08197924
gData_08197924:
	.4byte 0, 0x300000, 0, 0
	.4byte 0x1c0000, 0
@ read by Func_08146a6c
	.global gData_0819793c
gData_0819793c:
	.byte 32, 16
@ The Broken Seal's gWhirlwindComets_GridX; read by Func_08146e00
	.global Data_0819793e
Data_0819793e:
	.byte 0, 32, 64, 0, 32, 64, 0, 32, 64
@ The Broken Seal's gWhirlwindComets_GridY; read by Func_08146e00
	.global Data_08197947
Data_08197947:
	.byte 0, 0, 0, 32, 32, 32, 64, 64, 64
@ The Broken Seal's gWhirlwindComets_Frames; read by Func_08146e00
	.global Data_08197950
Data_08197950:
	.2byte 334, 347, 359, 372
@ The Broken Seal's gWhirlwindComets_CellWidths; read by Func_08146e00
	.global Data_08197958
Data_08197958:
	.byte 14, 24, 48
@ The Broken Seal's gWhirlwindComets_CellHeights; read by Func_08146e00
	.global Data_0819795b
Data_0819795b:
	.byte 14, 19, 24
@ The Broken Seal's gWhirlwindComets_CellBiasY; read by Func_08146e00
	.global Data_0819795e
Data_0819795e:
	.byte 8, 12, 17
	.balign 2
@ The Broken Seal's gWhirlwindComets_CellOffsets; read by Func_08146e00
	.global Data_08197962
Data_08197962:
	.2byte 13824, 14020, 14476
@ The Broken Seal's gPaletteParticles_DropSpeeds; read by Func_08147aec
	.global Data_08197968
Data_08197968:
	.byte 0, 8, 12, 16
@ read by Func_08147aec
	.global Data_0819796c
Data_0819796c:
	.byte 32, 32
@ The Broken Seal's gPaletteParticles_DropSizes; read by Func_08147aec
	.global Data_0819796e
Data_0819796e:
	.byte 1, 1, 2, 3
@ The Broken Seal's gBattleFx_PuffArcCellWidths; read by Func_08147aec
	.global Data_08197972
Data_08197972:
	.byte 24, 29, 30, 27, 24, 16
@ The Broken Seal's gBattleFx_PuffArcCellHeights; read by Func_08147aec
	.global Data_08197978
Data_08197978:
	.byte 23, 32, 37, 49, 53, 52
@ The Broken Seal's gPaletteParticles_StreakDrops; read by Func_08147aec
	.global Data_0819797e
Data_0819797e:
	.byte 41, 32, 26, 11, 4, 2
@ The Broken Seal's gBattleFx_PuffArcCellSourceOffsets; read by Func_08147aec
	.global Data_08197984
Data_08197984:
	.2byte 0, 552, 1480, 2590, 3913, 5185
@ The Broken Seal's gNereid_Columns; read by Func_08148458
	.global Data_08197990
Data_08197990:
	.byte 16, 104, 32, 104, 48, 104, 64, 104
@ The Broken Seal's gNereid_Gravity; read by Func_08148458
	.global Data_08197998
Data_08197998:
	.4byte 0x8000, 0x4000, 0x4000, 0x2000
@ The Broken Seal's gFroth_Counts; read by Func_08148a54
	.global gData_081979a8
gData_081979a8:
	.byte 6, 140, 12, 160, 30, 200
@ The Broken Seal's gWhirlwind_Shapes; read by Func_08148e9c
	.global gData_081979ae
gData_081979ae:
	.byte 1, 16, 2, 2, 24, 2, 4, 32, 4
@ read by Func_08149268
	.global Data_081979b7
Data_081979b7:
	.byte 1, 85, 30, 4, 124, 54, 12, 166, 86
@ The Broken Seal's gPlasma_Sparks; read by Func_08149794
	.global Data_081979c0
Data_081979c0:
	.2byte 384, 2, 256, 3, 192, 4
@ The Broken Seal's gPlasma_Columns; read by Func_08149794
	.global Data_081979cc
Data_081979cc:
	.byte 32, 64, 16, 48, 72, 96, 64, 88, 56, 104, 72, 48, 24, 56
@ The Broken Seal's gPlasma_Counts; read by Func_08149794
	.global Data_081979da
Data_081979da:
	.byte 1, 3, 7
	.balign 2
@ read by BattleFx_RunSparkGroups
	.global Data_081979de
Data_081979de:
	.2byte 128, 1, 16, 0, 0, 255, 1, 16
	.2byte 0, 0, 192, 3, 0, 24, 48, 340
	.2byte 1, 16, 24, 48, 32, 1, 16, 0
	.2byte 0, 200, 1, 16, 24, 48
@ The Broken Seal's gSparkGroups_FlashCells; read by BattleFx_RunSparkGroups
	.global Data_08197a1a
Data_08197a1a:
	.byte 5, 5, 4, 3, 2, 1
@ The Broken Seal's gVolcano_Counts; read by Func_0814a264
	.global Data_08197a20
Data_08197a20:
	.byte 1, 2, 4
@ The Broken Seal's gVolcano_Depths; read by Func_0814a264
	.global Data_08197a23
Data_08197a23:
	.byte 0, 0, 0, 0, 24, -24, 0, 0, -20, 20, -60, 60
@ The Broken Seal's gVolcano_StartFrames; read by Func_0814a264
	.global Data_08197a2f
Data_08197a2f:
	.byte 8, 18, 24, 30, 34
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by BattleFx_RunTargetMotes
	.global Data_08197a34
Data_08197a34:
	.byte 0, 4, 8, 12
@ read by Func_0814bee0
	.global Data_08197a38
Data_08197a38:
	.2byte 0, 0, 0, 32, 32, 0, 32, 32
	.2byte 32, 64, 64, 0, 64, 32, 64, 64
	.2byte 96, 0, 96, 32, 96, 64
@ read by Func_0814bee0
	.global Data_08197a64
Data_08197a64:
	.2byte 0, 16, 0, 48, 32, 0, 32, 32
	.2byte 32, 64, 64, 0, 64, 32, 64, 64
	.2byte 64, 96, 96, 0, 96, 64, 96, 96
@ read by Func_0814bee0
	.global Data_08197a94
Data_08197a94:
	.byte 0, 6, 8, 9
@ read by Func_0814bee0
	.global Data_08197a98
Data_08197a98:
	.byte 5, 8, 7, 3
@ read by Func_0814bee0
	.global Data_08197a9c
Data_08197a9c:
	.byte 32, 12, -24, 0
@ The effect handlers by effect kind - 1, as The Broken Seal's
@ gBattleFx_EffectHandlers (effect/effect_table.c); 0 where a kind has none.
	.global gData_08197aa0
gData_08197aa0:
	.4byte Func_08150c74            @ 1
	.4byte Func_08159cac            @ 2
	.4byte Func_0815a9e4            @ 3
	.4byte Func_081548b8            @ 4
	.4byte Func_08154f84            @ 5
	.4byte Func_081514bc            @ 6
	.4byte Func_0813bbd8            @ 7
	.4byte Func_08149268            @ 8
	.4byte Func_0813c0b8            @ 9
	.4byte Func_08148a54            @ 10
	.4byte Func_081509b4            @ 11
	.4byte Func_08151040            @ 12
	.4byte Func_0814a264            @ 13
	.4byte Func_08151fe4            @ 14
	.4byte Func_08149b94            @ 15
	.4byte Func_0815537c            @ 16
	.4byte Func_08151b48            @ 17
	.4byte Func_08149794            @ 18
	.4byte BattleFx_RunSlash        @ 19
	.4byte Func_08148e9c            @ 20
	.4byte BattleFx_RunCure @ 21
	.4byte BattleFx_RunPly @ 22
	.4byte BattleFx_RunWish @ 23
	.4byte BattleFx_RunCurePoison @ 24
	.4byte Func_08144af0            @ 25
	.4byte Func_0814fa08            @ 26
	.4byte SummonFx_RunCanvasMode3  @ 27
	.4byte Func_0814fa14            @ 28
	.4byte SummonFx_RunCanvasMode1  @ 29
	.4byte Func_0814fa38            @ 30
	.4byte SummonFx_RunCanvasMode2  @ 31
	.4byte BattleFx_RunTaint  @ 32
	.4byte BattleFx_RunDelude     @ 33
	.4byte Func_08144368            @ 34
	.4byte BattleFx_RunHeartrender     @ 35
	.4byte BattleFx_RunParalyze     @ 36
	.4byte Func_0814aebc            @ 37
	.4byte Func_08145df8            @ 38
	.4byte Func_08144044            @ 39
	.4byte Func_08145784            @ 40
	.4byte Func_0814b160            @ 41
	.4byte Func_0814ea3c            @ 42
	.4byte Func_0814e744            @ 43
	.4byte BattleFx_RunWish @ 44
	.4byte BattleFx_RunWish @ 45
	.4byte Func_0814fa08            @ 46
	.4byte SummonFx_RunCanvasMode1  @ 47
	.4byte 0                        @ 48
	.4byte Func_08189148            @ 49
	.4byte Func_081445f8            @ 50
	.4byte Func_08188cb0            @ 51
	.4byte Func_0816a29c            @ 52
	.4byte Func_08188714            @ 53
	.4byte BattleFx_RunShuriken        @ 54
	.4byte Func_081661dc            @ 55
	.4byte Func_081548c4            @ 56
	.4byte BattleFx_RunCuttingEdge @ 57
	.4byte Func_081665a4            @ 58
	.4byte BattleFx_RunHeatWave @ 59
	.4byte Func_0818d590            @ 60
	.4byte Func_08166af8            @ 61
	.4byte Func_08146a6c            @ 62
	.4byte BattleFx_RunBlizzard @ 63
	.4byte BattleFx_RunDemonFire     @ 64
	.4byte Func_0816a2a8            @ 65
	.4byte BattleFx_RunDrown     @ 66
	.4byte Func_0814627c            @ 67
	.4byte Func_08146638            @ 68
	.4byte Func_08178ee4            @ 69
	.4byte 0                        @ 70
	.4byte BattleFx_RunHealingMotesMode11 @ 71
	.4byte Func_0817c6ac            @ 72
	.4byte Func_0818bc60            @ 73
	.4byte Func_08179394            @ 74
	.4byte Func_0816ec58            @ 75
	.4byte Func_0817b66c            @ 76
	.4byte Func_0817b9b8            @ 77
	.4byte Func_08179994            @ 78
	.4byte Func_0818a208            @ 79
	.4byte Func_0817c27c            @ 80
	.4byte Func_08171b98            @ 81
	.4byte Func_0817bd88            @ 82
	.4byte BattleFx_RunProjectileVolleyMode14WithAction @ 83
	.4byte Func_0816bc70            @ 84
	.4byte Func_0816aa08            @ 85
	.4byte Func_0816fc9c            @ 86
	.4byte Func_0818a1fc            @ 87
	.4byte Func_0818a1f0            @ 88
	.4byte Func_0818a214            @ 89
	.4byte Func_0818ad1c            @ 90
	.4byte Func_0818ad34            @ 91
	.4byte Func_0818ad10            @ 92
	.4byte Func_0818ad28            @ 93
	.4byte 0                        @ 94
	.4byte 0                        @ 95
	.4byte 0                        @ 96
	.4byte 0                        @ 97
	.4byte 0                        @ 98
	.4byte 0                        @ 99
	.4byte Func_0816d3d8            @ 100
	.4byte Func_0816e644            @ 101
	.4byte 0                        @ 102
	.4byte Func_0816f080            @ 103
	.4byte Func_08166b04            @ 104
	.4byte Func_0816f928            @ 105
	.4byte Func_0816aed4            @ 106
	.4byte Func_08171b8c            @ 107
	.4byte Func_08178710            @ 108
	.4byte Func_0816a2c0            @ 109
	.4byte Func_0816f594            @ 110
	.4byte BattleFx_RunVolleyBMode11WithAction @ 111
	.4byte BattleFx_RunProjectileVolleyMode12WithAction @ 112
	.4byte BattleFx_RunCanvasMode0  @ 113
	.4byte Func_08181eb0            @ 114
	.4byte Func_0816d724            @ 115
	.4byte BattleFx_RunVolleyBMode10WithAction @ 116
	.4byte Func_0813d064            @ 117
	.4byte Func_0816a2b4            @ 118
	.4byte Func_0816d130            @ 119
	.4byte Func_08171430            @ 120
	.4byte Func_08170dac            @ 121
	.4byte Func_08189b14            @ 122
	.4byte Func_0818eda4            @ 123
	.4byte Func_0818fefc            @ 124
	.4byte Func_08190b74            @ 125
	.4byte Func_081811d4            @ 126
	.4byte Func_0814461c            @ 127
	.4byte Func_0818d59c            @ 128
	.4byte Func_08181910            @ 129
	.4byte 0                        @ 130
	.4byte 0                        @ 131
	.4byte 0                        @ 132
	.4byte 0                        @ 133
	.4byte 0                        @ 134
	.4byte 0                        @ 135
	.4byte 0                        @ 136
	.4byte 0                        @ 137
	.4byte 0                        @ 138
	.4byte 0                        @ 139
	.4byte 0                        @ 140
	.4byte 0                        @ 141
	.4byte 0                        @ 142
	.4byte 0                        @ 143
	.4byte 0                        @ 144
	.4byte 0                        @ 145
	.4byte 0                        @ 146
	.4byte 0                        @ 147
	.4byte 0                        @ 148
	.4byte 0                        @ 149
	.4byte 0                        @ 150
	.4byte 0                        @ 151
	.4byte 0                        @ 152
	.4byte 0                        @ 153
	.4byte 0                        @ 154
	.4byte 0                        @ 155
	.4byte 0                        @ 156
	.4byte 0                        @ 157
	.4byte 0                        @ 158
	.4byte 0                        @ 159
	.4byte 0                        @ 160
	.4byte 0                        @ 161
	.4byte 0                        @ 162
	.4byte 0                        @ 163
	.4byte 0                        @ 164
	.4byte 0                        @ 165
	.4byte 0                        @ 166
	.4byte 0                        @ 167
	.4byte 0                        @ 168
	.4byte 0                        @ 169
	.4byte 0                        @ 170
	.4byte 0                        @ 171
	.4byte 0                        @ 172
	.4byte 0                        @ 173
	.4byte 0                        @ 174
	.4byte 0                        @ 175
	.4byte 0                        @ 176
	.4byte 0                        @ 177
	.4byte 0                        @ 178
	.4byte 0                        @ 179
	.4byte 0                        @ 180
	.4byte Func_0814fa98            @ 181
	.4byte BattleFx_RunHealingMotesMode13 @ 182
	.4byte 0                        @ 183
	.4byte 0                        @ 184
	.4byte 0                        @ 185
	.4byte 0                        @ 186
	.4byte 0                        @ 187
	.4byte 0                        @ 188
	.4byte 0                        @ 189
	.4byte 0                        @ 190
	.4byte 0                        @ 191
	.4byte 0                        @ 192
	.4byte 0                        @ 193
	.4byte 0                        @ 194
	.4byte 0                        @ 195
	.4byte 0                        @ 196
	.4byte 0                        @ 197
	.4byte 0                        @ 198
	.4byte 0                        @ 199
	.4byte Func_0814fa50            @ 200
	.4byte Func_08144af0            @ 201
	.4byte Func_08154d5c            @ 202
	.4byte Func_081587e4            @ 203
	.4byte BattleFx_RunPly @ 204
	.4byte BattleFx_RunSleet        @ 205
	.4byte BattleFx_RunWish @ 206
	.4byte Func_0815842c            @ 207
	.4byte BattleFx_RunTonic @ 208
	.4byte Func_08144afc            @ 209
	.4byte Func_0814fa08            @ 210
	.4byte Func_0814fa14            @ 211
	.4byte BattleFx_RunPsyCrystal @ 212
	.4byte Func_0814fa5c            @ 213
	.4byte Func_081638d0            @ 214
	.4byte Func_0814fa38            @ 215
	.4byte Func_0814fa80            @ 216
	.4byte Func_08163c00            @ 217
	.4byte Func_08145df8            @ 218
	.4byte BattleFx_RunGust @ 219
	.4byte Func_0814fa20            @ 220
	.4byte Func_0814fa44            @ 221
	.4byte Func_0814fa68            @ 222
	.4byte Func_0814fa08            @ 223
	.4byte Func_0814fa8c            @ 224
	.4byte BattleFx_RunCure @ 225
	.4byte BattleFx_RunHealingMotesMode7 @ 226
	.4byte BattleFx_RunHealingMotesMode8 @ 227
	.4byte BattleFx_RunHealingMotesMode9 @ 228
	.4byte BattleFx_RunHealingMotesMode10 @ 229
	.4byte Func_0814b9c4            @ 230
	.4byte Func_0814b9d0            @ 231
	.4byte Func_08171b74            @ 232
	.4byte Func_08171b80            @ 233
	.4byte Func_0816b6d4            @ 234
	.4byte 0                        @ 235
	.4byte Func_08172ae8            @ 236
	.4byte Func_081450c0            @ 237
	.4byte Func_0817357c            @ 238
	.4byte BattleFx_RunVolleyBMode8 @ 239
	.4byte Func_08173570            @ 240
	.4byte Func_08173a40            @ 241
	.4byte Func_081739ec            @ 242
	.4byte BattleFx_RunVolleyBMode9 @ 243
	.4byte Func_0816c274            @ 244
	.4byte BattleFx_RunHealingMotesMode12 @ 245
	.4byte Func_08172af4            @ 246
	.4byte Func_0816b6e0            @ 247
	.4byte Func_081739f8            @ 248
	.4byte Func_08173a04            @ 249
	.4byte Func_08173a1c            @ 250
	.4byte Func_08173a28            @ 251
	.4byte Func_08173a34            @ 252
	.4byte Func_0816c6f8            @ 253
	.4byte Func_0816dc50            @ 254
	.4byte Func_08144b08            @ 255
	.4byte BattleFx_RunVolleyBMode22 @ 256
	.4byte BattleFx_RunVolleyBMode24 @ 257
	.4byte 0                        @ 258
	.4byte 0                        @ 259
	.4byte 0                        @ 260
	.4byte 0                        @ 261
	.4byte 0                        @ 262
	.4byte 0                        @ 263
	.4byte 0                        @ 264
	.4byte 0                        @ 265
	.4byte 0                        @ 266
	.4byte 0                        @ 267
	.4byte 0                        @ 268
	.4byte 0                        @ 269
	.4byte 0                        @ 270
	.4byte 0                        @ 271
	.4byte 0                        @ 272
	.4byte 0                        @ 273
	.4byte 0                        @ 274
	.4byte 0                        @ 275
	.4byte 0                        @ 276
	.4byte 0                        @ 277
	.4byte 0                        @ 278
	.4byte 0                        @ 279
	.4byte Func_08157d30            @ 280
	.4byte Func_08157638            @ 281
	.4byte Func_0815806c            @ 282
	.4byte Func_081579d0            @ 283
	.4byte 0                        @ 284
	.4byte 0                        @ 285
	.4byte 0                        @ 286
	.4byte 0                        @ 287
	.4byte 0                        @ 288
	.4byte 0                        @ 289
	.4byte 0                        @ 290
	.4byte 0                        @ 291
	.4byte 0                        @ 292
	.4byte 0                        @ 293
	.4byte 0                        @ 294
	.4byte 0                        @ 295
	.4byte 0                        @ 296
	.4byte 0                        @ 297
	.4byte 0                        @ 298
	.4byte 0                        @ 299
	.4byte Func_0813c5ac            @ 300
	.4byte Func_0813c5b8            @ 301
	.4byte Func_0813c5c4            @ 302
	.4byte BattleFx_RunCrazyVoice        @ 303
	.4byte Func_0813c5d0            @ 304
	.4byte Func_0813c5dc            @ 305
	.4byte BattleFx_RunWingBeat        @ 306
	.4byte BattleFx_RunWingFlutter        @ 307
	.4byte Func_08144604            @ 308
	.4byte Func_081450d8            @ 309
	.4byte BattleFx_RunGust @ 310
	.4byte Func_0817e078            @ 311
	.4byte BattleFx_RunMysticFlame     @ 312
	.4byte Func_0813cf80            @ 313
	.4byte Func_0817e084            @ 314
	.4byte Func_081450e4            @ 315
	.4byte Func_0813cf8c            @ 316
	.4byte BattleFx_RunBearClaw @ 317
	.4byte Func_0813cf98            @ 318
	.4byte BattleFx_RunFlyingAttack @ 319
	.4byte Func_0816a730            @ 320
	.4byte BattleFx_RunRansack @ 321
	.4byte Func_081450fc            @ 322
	.4byte Func_0813cf98            @ 323
	.4byte Func_0813cfb0            @ 324
	.4byte Func_0813cfbc            @ 325
	.4byte Func_0817e090            @ 326
	.4byte Func_0813cfc8            @ 327
	.4byte BattleFx_RunBacteriaRush @ 328
	.4byte BattleFx_RunFlyingAttack @ 329
	.4byte Func_08145108            @ 330
	.4byte 0                        @ 331
	.4byte Func_0813cfd4            @ 332
	.4byte Func_0813cfe0            @ 333
	.4byte BattleFx_RunFreebiteRush @ 334
	.4byte Func_0813cfec            @ 335
	.4byte Func_0813cfa4            @ 336
	.4byte Func_0813cff8            @ 337
	.4byte Func_08149ba0            @ 338
	.4byte Func_0817e09c            @ 339
	.4byte Func_081450f0            @ 340
	.4byte 0                        @ 341
	.4byte BattleFx_RunBearClaw @ 342
	.4byte BattleFx_RunRansack @ 343
	.4byte Func_0813cb1c            @ 344
	.4byte Func_0815cbd0            @ 345
	.4byte BattleFx_RunHeartrender     @ 346
	.4byte Func_081450b4            @ 347
	.4byte Func_0817e668            @ 348
	.4byte Func_0813cb10            @ 349
	.4byte Func_081450cc            @ 350
	.4byte Func_08144610            @ 351
	.4byte BattleFx_RunCure @ 352
	.4byte Func_0817e674            @ 353
	.4byte Func_0814b160            @ 354
	.4byte 0                        @ 355
	.4byte 0                        @ 356
	.4byte 0                        @ 357
	.4byte 0                        @ 358
	.4byte 0                        @ 359
	.4byte 0                        @ 360
	.4byte 0                        @ 361
	.4byte 0                        @ 362
	.4byte 0                        @ 363
	.4byte 0                        @ 364
	.4byte 0                        @ 365
	.4byte 0                        @ 366
	.4byte 0                        @ 367
	.4byte 0                        @ 368
	.4byte 0                        @ 369
	.4byte 0                        @ 370
	.4byte 0                        @ 371
	.4byte 0                        @ 372
	.4byte 0                        @ 373
	.4byte 0                        @ 374
	.4byte 0                        @ 375
	.4byte 0                        @ 376
	.4byte 0                        @ 377
	.4byte 0                        @ 378
	.4byte 0                        @ 379
	.4byte Func_08151fd8            @ 380
	.4byte BattleFx_RunWingStroke       @ 381
	.4byte Func_0815c958            @ 382
	.4byte Func_0815c964            @ 383
	.4byte BattleFx_RunOuterSpace @ 384
	.4byte Func_08147ad4            @ 385
	.4byte Func_0813d004            @ 386
	.4byte BattleFx_RunSparkApproachVariant3 @ 387
	.4byte BattleFx_RunThrash @ 388
	.4byte Func_0817e680            @ 389
	.4byte Func_0818c7ac            @ 390
	.4byte Func_0816aee0            @ 391
	.4byte Func_08171ba4            @ 392
	.4byte BattleFx_RunProjectileVolleyMode13WithAction @ 393
	.4byte BattleFx_LongApproachThenSpark @ 394
	.4byte Func_0813cb28            @ 395
	.4byte Func_08173a10            @ 396
	.4byte BattleFx_RunVolleyBMode17 @ 397
	.4byte BattleFx_RunVolleyBMode18 @ 398
	.4byte Func_0813d010            @ 399
	.4byte BattleFx_RunVolleyBMode0 @ 400
	.4byte BattleFx_RunVolleyBMode2 @ 401
	.4byte BattleFx_RunIceBlessing       @ 402
	.4byte BattleFx_RunVolleyBMode6 @ 403
	.4byte BattleFx_RunVolleyBMode3 @ 404
	.4byte BattleFx_RunVolleyBMode5 @ 405
	.4byte BattleFx_RunVolleyBMode7 @ 406
	.4byte BattleFx_RunVolleyBMode4 @ 407
	.4byte BattleFx_RunVolleyBMode13 @ 408
	.4byte BattleFx_RunVolleyBMode14 @ 409
	.4byte BattleFx_RunVolleyBMode15 @ 410
	.4byte BattleFx_RunVolleyBMode16 @ 411
	.4byte BattleFx_RunVolleyBMode12 @ 412
	.4byte BattleFx_RunVolleyBMode20 @ 413
	.4byte Func_0818bc8c            @ 414
	.4byte BattleFx_RunVolleyBMode21 @ 415
	.4byte BattleFx_RunVolleyBMode0 @ 416
	.4byte 0                        @ 417
	.4byte 0                        @ 418
	.4byte 0                        @ 419
	.4byte 0                        @ 420
	.4byte Func_0813d028            @ 421
	.4byte EffectStep_RunAnimationMode9 @ 422
	.4byte EffectStep_RunAnimationMode10 @ 423
	.4byte EffectStep_RunAnimationMode11 @ 424
	.4byte Func_0816a2cc            @ 425
	.4byte Func_0818b868            @ 426
	.4byte EffectStep_RunAnimationMode6Or7Or8B @ 427
	.4byte BattleFx_RunTargetMotesMode1     @ 428
	.4byte BattleFx_RunDelude     @ 429
	.4byte Func_08145114            @ 430
	.4byte Func_0817e68c            @ 431
	.4byte BattleFx_RunVolleyBMode19 @ 432
	.4byte Func_0817e0a8            @ 433
	.4byte Func_0818bc6c            @ 434
	.4byte Func_08173aa0            @ 435
	.4byte Func_0814b9dc            @ 436
	.4byte Func_0818c7a0            @ 437
	.4byte Func_0813d034            @ 438
	.4byte Func_0813d040            @ 439
	.4byte Func_0813cfbc            @ 440
	.4byte Func_0813d04c            @ 441
	.4byte BattleFx_RunProjectileVolleyMode15       @ 442
	.4byte BattleFx_RunProjectileVolleyMode16       @ 443
	.4byte BattleFx_RunProjectileVolleyMode17       @ 444
	.4byte Func_08173a4c            @ 445
	.4byte Func_08173a58            @ 446
	.4byte Func_0817cc30            @ 447
	.4byte Func_08173a64            @ 448
	.4byte Func_0818bc7c            @ 449
	.4byte Func_0817d724            @ 450
	.4byte Func_0814ea4c            @ 451
	.4byte Func_0817d174            @ 452
	.4byte Func_08145120            @ 453
	.4byte EffectStep_RunAnimationMode12 @ 454
	.4byte Func_08173a70            @ 455
	.4byte Func_0813d058            @ 456
	.4byte Func_0817e0b4            @ 457
	.4byte Func_0813d01c            @ 458
	.4byte Func_0816fc90            @ 459
	.4byte Func_0814fa74            @ 460
	.4byte Func_08181ebc            @ 461
	.4byte Func_08181ec8            @ 462
	.4byte BattleFx_RunCanvasMode3WithAction @ 463
	.4byte Func_08173a88            @ 464
	.4byte Func_08182390            @ 465
	.4byte Func_0818239c            @ 466
	.4byte Func_08173a7c            @ 467
	.4byte Func_08173a94            @ 468
	.4byte Func_0816e650            @ 469
	.4byte BattleFx_RunProjectileVolleyMode18       @ 470
	.4byte BattleFx_RunVolleyBMode23 @ 471
@ The Broken Seal's gCybele_ObjectPoses; read by Func_0814ce30
	.global Data_081981fc
Data_081981fc:
	.byte 8, 10, 9, 11, 3, 4
@ The Broken Seal's gCybele_ThornOffsets; read by Func_0814ce30
	.global Data_08198202
Data_08198202:
	.2byte 3920, 3932, 3947, 3955, 3970
@ The Broken Seal's gCybele_ThornWidths; read by Func_0814ce30
	.global Data_0819820c
Data_0819820c:
	.byte 2, 5, 2, 5, 3
@ The Broken Seal's gCybele_ThornHeights; read by Func_0814ce30
	.global Data_08198211
Data_08198211:
	.byte 6, 3, 4, 3, 3
@ The Broken Seal's gCybele_LeafOffsets; read by Func_0814ce30
	.global Data_08198216
Data_08198216:
	.2byte 3456, 3712, 3856
@ The Broken Seal's gCybele_LeafSizes; read by Func_0814ce30
	.global Data_0819821c
Data_0819821c:
	.byte 16, 12, 8
	.balign 2
@ The Broken Seal's gCybele_ThornOffsets; read by Func_0814ce30
	.global Data_08198220
Data_08198220:
	.2byte 3920, 3932, 3947, 3955, 3970
@ The Broken Seal's gCybele_ThornWidths; read by Func_0814ce30
	.global Data_0819822a
Data_0819822a:
	.byte 2, 5, 2, 5, 3
@ The Broken Seal's gCybele_ThornHeights; read by Func_0814ce30
	.global Data_0819822f
Data_0819822f:
	.byte 6, 3, 4, 3, 3
@ The Broken Seal's gCybele_SparkOffsets; read by Func_0814ce30
	.global Data_08198234
Data_08198234:
	.2byte 3979, 4043, 4268, 4389, 4470
@ The Broken Seal's gCybele_SparkSizes; read by Func_0814ce30
	.global Data_0819823e
Data_0819823e:
	.byte 8, 15, 11, 9, 5
	.balign 2
@ The Broken Seal's gCybele_EmberOffsets; read by Func_0814ce30
	.global Data_08198244
Data_08198244:
	.2byte 5760, 6016, 7040, 9344, 11648, 13952, 16256
@ The Broken Seal's gCybele_EmberSizes; read by Func_0814ce30
	.global Data_08198252
Data_08198252:
	.2byte 16, 32, 48, 48, 48, 48, 48
@ The Broken Seal's gBoreas_Points; read by Func_0814db04
	.global Data_08198260
Data_08198260:
	.byte 57, 130, 75, 128, 71, 104, 101, 130, 86, 126, 85, 98, 64, 98, 98, 96
	.byte 77, 87, 84, 88, 68, 67, 81, 49, 94, 70, 70, 51, 93, 36, 81, 50
@ The Broken Seal's gBoreas_ShakeOffsets; read by Func_0814db04
	.global Data_08198280
Data_08198280:
	.byte -4, -3, -2, 0
@ The Broken Seal's gBoreas_RockCells; read by Func_0814db04
	.global Data_08198284
Data_08198284:
	.2byte 0, 1152, 2064
@ The Broken Seal's gBoreas_RockWidths; read by Func_0814db04
	.global Data_0819828a
Data_0819828a:
	.byte 24, 24, 20
@ The Broken Seal's gBoreas_RockHeights; read by Func_0814db04
	.global Data_0819828d
Data_0819828d:
	.byte 48, 38, 26
@ The Broken Seal's gBoreas_Animations; read by Func_0814db04
	.global Data_08198290
Data_08198290:
	.byte 2, 9, 10, 11
@ The Broken Seal's gBoreas_DebrisWidths; read by Func_0814db04
	.global Data_08198294
Data_08198294:
	.byte 12, 11, 8, 10, 8, 12, 5, 7, 7, 6, 7
@ The Broken Seal's gBoreas_DebrisHeights; read by Func_0814db04
	.global Data_0819829f
Data_0819829f:
	.byte 10, 16, 14, 15, 4, 4, 3, 4, 6, 12, 10
@ The Broken Seal's gBoreas_DebrisCells; read by Func_0814db04
	.global Data_081982aa
Data_081982aa:
	.2byte 2584, 2704, 2880, 2992, 5062, 5094, 5142, 5157
	.2byte 5185, 5227, 5299
@ read by Func_0814db04
	.global Data_081982c0
Data_081982c0:
	.byte 16, 16, 16
@ read by Func_0814db04
	.global Data_081982c3
Data_081982c3:
	.byte 16, 16, 16
@ The Broken Seal's gBoreas_SprayCells; read by Func_0814db04
	.global Data_081982c6
Data_081982c6:
	.2byte 4294, 4550, 4806
@ The Broken Seal's gBoreas_RockCells; read by Func_0814db04
	.global Data_081982cc
Data_081982cc:
	.2byte 0, 1152, 2064
@ The Broken Seal's gBoreas_RockWidths; read by Func_0814db04
	.global Data_081982d2
Data_081982d2:
	.byte 24, 24, 20
@ The Broken Seal's gBoreas_RockHeights; read by Func_0814db04
	.global Data_081982d5
Data_081982d5:
	.byte 48, 38, 26
@ The Broken Seal's gBoreas_RockWidths; read by Func_0814db04
	.global Data_081982d8
Data_081982d8:
	.byte 24, 24, 20
@ The Broken Seal's gBoreas_RockHeights; read by Func_0814db04
	.global Data_081982db
Data_081982db:
	.byte 48, 38, 26
@ The Broken Seal's gBoreas_RockCells; read by Func_0814db04
	.global Data_081982de
Data_081982de:
	.2byte 0, 1152, 2064
@ The Broken Seal's gBreak_Gravities; read by Func_0814e744
	.global gData_081982e4
gData_081982e4:
	.4byte 0x4000, 0x2000, 0x4000, 0x8000
@ read by Func_0814ea58
	.global Data_081982f4
Data_081982f4:
	.byte 6, 6, 10
@ The Broken Seal's gQuake_Timings; read by Func_08150c74
	.global Data_081982f7
Data_081982f7:
	.byte 80, 32, 2, 112, 64, 4, 144, 96, 8
@ The Broken Seal's gFire_Counts; read by Func_08151040
	.global Data_08198300
Data_08198300:
	.byte 16, 32, 64
@ The Broken Seal's gBattleFx_PuffArcCellWidths; read by Func_08151040
	.global Data_08198303
Data_08198303:
	.byte 24, 29, 30, 27, 24, 16
@ The Broken Seal's gBattleFx_PuffArcCellHeights; read by Func_08151040
	.global Data_08198309
Data_08198309:
	.byte 23, 32, 37, 49, 53, 52
@ The Broken Seal's gBattleFx_PuffArcCellBiasY; read by Func_08151040
	.global Data_0819830f
Data_0819830f:
	.byte 41, 32, 26, 11, 4, 2
	.balign 2
@ The Broken Seal's gBattleFx_PuffArcCellSourceOffsets; read by Func_08151040
	.global Data_08198316
Data_08198316:
	.2byte 0, 552, 1480, 2590, 3913, 5185
@ The Broken Seal's gRisingMotes_ColumnSpots; read by Func_081514bc
	.global Data_08198322
Data_08198322:
	.byte 53, 108, 73, 110, 29, 110, 73, 108, 33, 104, 96, 106, 8, 109, 62, 98
	.byte 33, 100, 87, 100, 21, 95, 53, 90, 33, 90, 72, 91, 82, 91, 77, 86
	.byte 52, 84, 18, 92, 56, 81
@ The Broken Seal's gRisingMotes_Timings; read by Func_081514bc
	.global Data_08198348
Data_08198348:
	.byte 32, 8, 85, 48, 12, 97, 64, 19, 109
@ The Broken Seal's gRisingMotes_MoteWidths; read by Func_081514bc
	.global Data_08198351
Data_08198351:
	.byte 12, 10, 7, 10, 8, 8, 8, 8
@ The Broken Seal's gRisingMotes_MoteHeights; read by Func_081514bc
	.global Data_08198359
Data_08198359:
	.byte 9, 16, 14, 15, 8, 8, 8, 8
	.balign 2
@ The Broken Seal's gRisingMotes_MoteCells; read by Func_081514bc
	.global Data_08198362
Data_08198362:
	.2byte 1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500
@ The Broken Seal's gRisingMotes_ColumnCells; read by Func_081514bc
	.global Data_08198372
Data_08198372:
	.2byte 0, 768, 1280
@ The Broken Seal's gRisingMotes_ColumnHeights; read by Func_081514bc
	.global Data_08198378
Data_08198378:
	.byte 24, 16, 16
@ The Broken Seal's gRisingMotes_MoteWidths; read by Func_081514bc
	.global Data_0819837b
Data_0819837b:
	.byte 12, 10, 7, 10, 8, 8, 8, 8
@ The Broken Seal's gRisingMotes_MoteHeights; read by Func_081514bc
	.global Data_08198383
Data_08198383:
	.byte 9, 16, 14, 15, 8, 8, 8, 8
	.balign 2
@ The Broken Seal's gRisingMotes_MoteCells; read by Func_081514bc
	.global Data_0819838c
Data_0819838c:
	.2byte 1792, 1900, 2060, 2158, 2308, 2372, 2436, 2500
@ The Broken Seal's gLightningBolts_Sparks; read by Func_08151b48
	.global Data_0819839c
Data_0819839c:
	.byte 2, 4, 2, 6, 4, 8
@ The Broken Seal's gBlastAndSmokeBomb_Counts; read by Func_08151ff0
	.global Data_081983a2
Data_081983a2:
	.byte 16, 8, 72, 32, 16, 80, 128, 32, 88
	.balign 2
@ The Broken Seal's gBlastAndSmokeBomb_PuffCells; read by Func_08151ff0
	.global Data_081983ac
Data_081983ac:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
@ The Broken Seal's gCybele_EmberSizes; read by Func_08151ff0
	.global Data_081983ba
Data_081983ba:
	.2byte 16, 32, 48, 48, 48, 48, 48
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by BattleFx_RunProjectileVolleyB
	.global Data_081983c8
Data_081983c8:
	.byte 0, 4, 8, 12
@ The Broken Seal's gWorldMap_UnitScale; read by Func_08153ebc
	.global Data_081983cc
Data_081983cc:
	.4byte 0x10000, 0x10000
@ The Broken Seal's gDualTable_VineWidths; read by Func_081548d0
	.global Data_081983d4
Data_081983d4:
	.byte 21, 28, 19
@ The Broken Seal's gDualTable_HeightLimits; read by Func_081548d0
	.global Data_081983d7
Data_081983d7:
	.byte 119, 119, 102
@ The Broken Seal's gDualTable_VineCells; read by Func_081548d0
	.global Data_081983da
Data_081983da:
	.2byte 0, 2499, 5831
@ The Broken Seal's gDualTable_PillarWidths; read by Func_081548d0
	.global Data_081983e0
Data_081983e0:
	.byte 10, 8, 9
	.balign 2
@ The Broken Seal's gDualTable_PillarCells; read by Func_081548d0
	.global Data_081983e4
Data_081983e4:
	.2byte 0, 1080, 1888
@ The Broken Seal's gDualTable_ColumnCounts; read by Func_081548d0
	.global Data_081983ea
Data_081983ea:
	.byte 4, 8, 12
@ The Broken Seal's gDualTable_ColumnX; read by Func_081548d0
	.global Data_081983ed
Data_081983ed:
	.byte 64, 72, 56, 80, 48, 88, 40, 96, 32, 104, 24, 72, 56, 80, 48, 88
@ The Broken Seal's gDualTable_PillarHeights; read by Func_081548d0
	.global Data_081983fd
Data_081983fd:
	.byte 100, 60, 88, 96, 64, 104, 82, 98
@ The Broken Seal's gDualTable_PuffWidths; read by Func_081548d0
	.global Data_08198405
Data_08198405:
	.byte 5, 7, 11, 16, 23, 27, 16
@ The Broken Seal's gDualTable_PuffHeights; read by Func_081548d0
	.global Data_0819840c
Data_0819840c:
	.byte 10, 14, 21, 32, 45, 50, 32
	.balign 2
@ The Broken Seal's gDualTable_PuffCells; read by Func_081548d0
	.global Data_08198414
Data_08198414:
	.2byte 0, 50, 148, 379, 891, 1926, 3276
@ The Broken Seal's gVine_ColumnOffsets; read by Func_08154d5c
	.global Data_08198422
Data_08198422:
	.byte 0, 14, -32, 8, 32, -48, 4, 12, -24, 24, -12, -40, -4, -16, -32, 12
@ The Broken Seal's gThorn_Kinds; read by Func_08154f84
	.global Data_08198432
Data_08198432:
	.byte 0, 3, 2, 1, 2, 3, 2, 1
@ The Broken Seal's gThorn_X; read by Func_08154f84
	.global Data_0819843a
Data_0819843a:
	.byte 24, 80, 24, 48, 88, 40, 8, 96
@ The Broken Seal's gThorn_Counts; read by Func_08154f84
	.global Data_08198442
Data_08198442:
	.byte 3, 5, 8
@ The Broken Seal's gThorn_PuffWidths; read by Func_08154f84
	.global Data_08198445
Data_08198445:
	.byte 5, 7, 11, 17, 32, 32, 19
@ The Broken Seal's gThorn_PuffHeights; read by Func_08154f84
	.global Data_0819844c
Data_0819844c:
	.byte 10, 14, 21, 34, 59, 60, 38
	.balign 2
@ The Broken Seal's gThorn_PuffCells; read by Func_08154f84
	.global Data_08198454
Data_08198454:
	.2byte 5888, 5938, 6036, 6267, 6845, 8733, 10653
@ The Broken Seal's gLightningBolts_Counts; read by Func_0815537c
	.global Data_08198462
Data_08198462:
	.byte 16, 0, 15, 1, 32, 3, 31, 1, 128, 5, 48, 3
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by Func_0815537c
	.global Data_0819846e
Data_0819846e:
	.byte 0, 4, 8, 12
@ read by Func_0815537c
	.global Data_08198472
Data_08198472:
	.byte 1, 1, 0
@ The Broken Seal's gVolley_CometHeights; read by BattleFx_RunProjectileVolley
	.global Data_08198475
Data_08198475:
	.byte 24, 36, 48
@ read by BattleFx_RunProjectileVolley
	.global Data_08198478
Data_08198478:
	.byte 6, 10, 3, 32, 4, 4, 10, 5, 32, 8, 10, 32, 3, 32, 4, 5
	.byte 10, 5, 32, 4, 10, 10, 1, 32, 4, 1, 5, 0, 32, 80, 7, 8
	.byte 2, 32, 40, 30, 24, 1, 32, 4, 6, 2, 3, 32, 4, 60, 10, 1
	.byte 32, 4, 60, 12, 1, 32, 4, 10, 10, 1, 32, 4, 7, 8, 2, 32
	.byte 40, 20, 12, 1, 32, 40, 30, 24, 2, 32, 8, 30, 16, 1, 32, 8
	.byte 30, 16, 1, 32, 8, 30, 16, 1, 32, 8, 60, 12, 1, 32, 4
@ The Broken Seal's gVolley_FlashWidths; read by BattleFx_RunProjectileVolley
	.global Data_081984d7
Data_081984d7:
	.byte 12, 18, 24, 30, 36, 39, 40
@ The Broken Seal's gVolley_FlashHeights; read by BattleFx_RunProjectileVolley
	.global Data_081984de
Data_081984de:
	.byte 10, 15, 18, 22, 27, 28, 30
@ The Broken Seal's gVolley_FlashSteps; read by BattleFx_RunProjectileVolley
	.global Data_081984e5
Data_081984e5:
	.byte 2, 3, 4, 4, 5, 6, 6
@ read by BattleFx_RunProjectileVolley
	.global Data_081984ec
Data_081984ec:
	.byte 0, 3, 5, 7, 10, 12, 12, 0
@ The Broken Seal's gVolley_FlashOffsets; read by BattleFx_RunProjectileVolley
	.global Data_081984f4
Data_081984f4:
	.2byte 8192, 8312, 8582, 9014, 9674, 10646, 11738
@ The Broken Seal's gVolley_PelletOffsets; read by BattleFx_RunProjectileVolley
	.global Data_08198502
Data_08198502:
	.2byte 2308, 2372, 2436, 2500
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by BattleFx_RunProjectileVolley
	.global Data_0819850a
Data_0819850a:
	.byte 0, 4, 8, 12
@ The Broken Seal's gMercury_LaunchFrames; read by Func_08157638
	.global Data_0819850e
Data_0819850e:
	.byte 4, 7, 10, 21, 34
@ The Broken Seal's gHail_CellWidths; read by Func_0815842c
	.global Data_08198513
Data_08198513:
	.byte 8, 8, 8, 8
@ The Broken Seal's gHail_CellHeights; read by Func_0815842c
	.global Data_08198517
Data_08198517:
	.byte 8, 8, 8, 8
	.balign 2
@ The Broken Seal's gVolley_PelletOffsets; read by Func_0815842c
	.global Data_0819851c
Data_0819851c:
	.2byte 2308, 2372, 2436, 2500
@ The Broken Seal's gGround_ChipFlips; read by Func_081587e4
	.global Data_08198524
Data_08198524:
	.byte 3, 7, 11, 15
@ The Broken Seal's gGround_ChipWidths; read by Func_081587e4
	.global Data_08198528
Data_08198528:
	.byte 24, 7, 7, 8, 6, 13, 10, 11, 7
@ The Broken Seal's gGround_ChipHeights; read by Func_081587e4
	.global Data_08198531
Data_08198531:
	.byte 48, 13, 14, 24, 24, 17, 20, 14, 20
@ The Broken Seal's gGround_ChipCells; read by Func_081587e4
	.global Data_0819853a
Data_0819853a:
	.2byte 0, 1152, 1243, 1341, 1533, 1677, 1898, 2098
	.2byte 2252
@ The Broken Seal's gGround_ChipX; read by Func_081587e4
	.global Data_0819854c
Data_0819854c:
	.byte 0, 9, 9, 11, 10, 0, 3, 13, 7
@ The Broken Seal's gGround_ChipY; read by Func_081587e4
	.global Data_08198555
Data_08198555:
	.byte 0, 18, 18, 0, 24, 20, 5, 22, 26
@ The Broken Seal's gMode12_Animations; read by Func_08158dc8
	.global Data_0819855e
Data_0819855e:
	.byte 7, 14, 10, 4
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by Func_08158dc8
	.global Data_08198562
Data_08198562:
	.byte 0, 4, 8, 12
@ The Broken Seal's gSpire_ShardOffsets, byte for byte
	.global Data_08198566
Data_08198566:
	.byte 65, 104, 68, 95, 64, 93, 61, 93, 64, 87, 72, 85, 56, 88, 51, 80
	.byte 61, 75, 72, 81, 69, 71, 66, 70, 71, 64, 49, 71, 59, 69, 53, 68
	.byte 78, 73, 55, 67, 75, 62, 61, 65, 53, 61, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
@ The Broken Seal's gSpire_RockX; read by Func_08159cac
	.global Data_081985a6
Data_081985a6:
	.byte 0, -12, 12, -8, 16
@ The Broken Seal's gSpire_DropFrames; read by Func_08159cac
	.global Data_081985ab
Data_081985ab:
	.byte 8, 16, 20, 24, 32
@ The Broken Seal's gSpire_RockCounts; read by Func_08159cac
	.global Data_081985b0
Data_081985b0:
	.byte 1, 3, 5
@ read by BattleFx_RunBurstScene
	.global Data_081985b3
Data_081985b3:
	.byte 1, 0, 5, 16, 12, 80, 0, 0, 1, 1, 7, 16, 12, 104, 0, 0
	.byte 1, 2, 9, 16, 12, 128, 0, 0, 1, 3, 1, 16, 16, 36, 0, 0
	.byte 1, 3, 2, 20, 16, 52, 0, 0, 1, 3, 3, 24, 16, 68, 0, 0
	.byte 0, 2, 1, 32, 12, 28, 1, 0, 0, 2, 2, 32, 12, 40, 1, 0
	.byte 0, 2, 3, 32, 12, 52, 1, 0, 1, 0, 3, 16, 12, 50, 0, 0
	.byte 1, 0, 1, 16, 12, 40, 0, 1, 1, 0, 1, 16, 12, 80, 0, 2
	.byte 1, 3, 1, 16, 16, 30, 0, 0
@ read by BattleFx_RunBurstScene
	.global Data_0819861b
Data_0819861b:
	.byte 0, 4, 8, 12, 0
@ The Broken Seal's gGaia_RowSheetOffsets; read by Func_0815a9e4
	.global Data_08198620
Data_08198620:
	.2byte 0, 160, 352, 640, 1024, 1504, 2080, 2752
	.2byte 3776
@ The Broken Seal's gGaia_RowWidths; read by Func_0815a9e4
	.global Data_08198632
Data_08198632:
	.byte 5, 6, 9, 12, 15, 18, 21, 32, 40
	.balign 2
@ The Broken Seal's gGaia_MoundSheetOffsets; read by Func_0815a9e4
	.global Data_0819863c
Data_0819863c:
	.2byte 10672, 11505, 12550
@ The Broken Seal's gEarthWall_CellWidths; read by Func_0815a9e4
	.global Data_08198642
Data_08198642:
	.byte 17, 19, 20
@ The Broken Seal's gEarthWall_CellHeights; read by Func_0815a9e4
	.global Data_08198645
Data_08198645:
	.byte 49, 55, 64
@ The Broken Seal's gGaia_Bg2Shifts; read by Func_0815a9e4
	.global Data_08198648
Data_08198648:
	.byte -8, 0, 12, -72, -40, -16
@ The Broken Seal's gGaia_LateRockColumns; read by Func_0815a9e4
	.global Data_0819864e
Data_0819864e:
	.byte 136, 40, 120, 56, 104, 72
@ read by Func_0815b764
	.global Data_08198654
Data_08198654:
	.byte 0, 32, 0, 32, 0, 32, 32
@ read by Func_0815b764
	.global Data_0819865b
Data_0819865b:
	.byte 8, 0, 40, 32, 72, 64, 96
@ read by Func_0815b764
	.global Data_08198662
Data_08198662:
	.byte 12, 9, 6, 5, 8, 9, 12, 9, 8, 8, 8, 10, 11, 10, 8, 6
	.byte 6, 9
@ read by Func_0815b764
	.global Data_08198674
Data_08198674:
	.byte 18, 17, 15, 12, 14, 15, 16, 13, 8, 4, 4, 6, 9, 9, 10, 10
	.byte 13, 16
@ read by Func_0815b764
	.global Data_08198686
Data_08198686:
	.2byte 0, 216, 369, 459, 519, 631, 766, 958
	.2byte 1075, 1139, 1171, 1203, 1263, 1362, 1452, 1532
	.2byte 1592, 1670
@ read by Func_0815b764
	.global Data_081986aa
Data_081986aa:
	.byte 100, 60, 64, -6, -6, 112, 44, -64, -6, 4, 96, 28, 64, -6, -5, 112
	.byte 80, -64, -6, 4, 104, 32, 64, -6, 5, 102, 68, -64, -6, 4
@ The Broken Seal's gTwoResource_CellWidths; read by Func_0815c970
	.global gData_081986c8
gData_081986c8:
	.byte 40, 112, 112, 112, 88, 64
@ The Broken Seal's gTwoResource_CellHeights; read by Func_0815c970
	.global gData_081986ce
gData_081986ce:
	.byte 56, 80, 64, 64, 80, 88
@ The Broken Seal's gTwoResource_CellSourceOffsets; read by Func_0815c970
	.global gData_081986d4
gData_081986d4:
	.2byte 0, 2240, 11200, 18368, 0, 7040
@ The Broken Seal's gTwoResource_CellX; read by Func_0815c970
	.global gData_081986e0
gData_081986e0:
	.byte 72, 0, 0, 0, 32, 64, 16, 16, 16, 16, 8, 0
@ read by Func_0815c970
	.global gData_081986ec
gData_081986ec:
	.byte 0, 4, 24, -6, -20, -28
@ read by Func_0815cde8
	.global Data_081986f2
Data_081986f2:
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
@ The Broken Seal's gWorldMap_UnitScale; read by Func_0815cde8
	.global Data_08198828
Data_08198828:
	.4byte 0x10000, 0x10000
@ read by Func_0815cde8
	.global Data_08198830
Data_08198830:
	.byte 35, 45, 14, 1, 1, 40, 9, 40, 9, 25, 39, 8
@ read by Func_0815cde8
	.global Data_0819883c
Data_0819883c:
	.byte 0, 32, 32, 64, 96, 51, 51, 83, 83, 106, 129, 129
@ The Broken Seal's gWorldMap_UnitScale; read by Func_0815cde8
	.global Data_08198848
Data_08198848:
	.4byte 0x10000, 0x10000
@ read by Func_0815cde8
	.global Data_08198850
Data_08198850:
	.byte 0, 32, 32, 64, 96, 50, 51, 82
@ read by Func_0815cde8
	.global Data_08198858
Data_08198858:
	.byte 10, 0, 32, 45, 45, 5, 36, 5
@ The Broken Seal's gTwelveMode_StrikeWidths; read by Func_0815e3ac
	.global Data_08198860
Data_08198860:
	.byte 38, 54, 71, 74, 77, 65
@ The Broken Seal's gTwelveMode_StrikeHeights; read by Func_0815e3ac
	.global Data_08198866
Data_08198866:
	.byte 7, 9, 11, 13, 13, 11
@ The Broken Seal's gBeamSequence_CellSheetOffsets; read by Func_0815e3ac
	.global Data_0819886c
Data_0819886c:
	.2byte 0, 266, 752, 1533, 2495, 3496
@ The Broken Seal's gTwelveMode_StrikeReach; read by Func_0815e3ac
	.global Data_08198878
Data_08198878:
	.byte 58, 42, 25, 13, 5, 0
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by Func_0815f16c
	.global gData_0819887e
gData_0819887e:
	.byte 0, 4, 8, 12
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by Func_0815f16c
	.global gData_08198882
gData_08198882:
	.byte 0, 4, 8, 12
@ read by Func_0815f16c
	.global gData_08198886
gData_08198886:
	.byte 0, 1, 2, 3
@ read by Func_0815f16c
	.global gData_0819888a
gData_0819888a:
	.2byte 8448, 8712, 11462, 16646, 16646, 24351, 24351, 28201
	.2byte 28201, 28201
@ read by Func_0815f16c
	.global gData_0819889e
gData_0819889e:
	.2byte 24, 55, 64, 115, 115, 70, 70, 22
	.2byte 22, 22
@ read by Func_0815f16c
	.global gData_081988b2
gData_081988b2:
	.2byte 11, 50, 81, 67, 67, 55, 55, 29
	.2byte 29, 29
@ read by Func_0815f16c
	.global gData_081988c6
gData_081988c6:
	.2byte 58, 19, 0, 6, 6, 50, 50, 100
	.2byte 100, 100
@ read by Func_0815f16c
	.global gData_081988da
gData_081988da:
	.2byte 17, 19, 24, 44, 44, 42, 42, 37
	.2byte 37, 37
@ read by Func_0815f16c
	.global gData_081988ee
gData_081988ee:
	.2byte 5700, 7128, 10414, 14089, 19152, 23148, 26752
@ read by Func_0815f16c
	.global gData_081988fc
gData_081988fc:
	.2byte 42, 62, 75, 83, 54, 68, 37
@ read by Func_0815f16c
	.global gData_0819890a
gData_0819890a:
	.2byte 34, 53, 49, 61, 74, 53, 28
@ read by Func_0815f16c
	.global gData_08198918
gData_08198918:
	.2byte 85, 65, 50, 19, 0, 0, 58
@ read by Func_0815f16c
	.global gData_08198926
gData_08198926:
	.2byte 62, 66, 78, 41, 19, 0, 5
@ read by Func_0815f16c
	.global gData_08198934
gData_08198934:
	.2byte 27788, 28464, 29724
@ read by Func_0815f16c
	.global gData_0819893a
gData_0819893a:
	.byte 26, 35, 52
@ read by Func_0815f16c
	.global gData_0819893d
gData_0819893d:
	.byte 26, 36, 48
@ read by Func_0815f16c
	.global gData_08198940
gData_08198940:
	.byte 37, 104, 19, 81, 20, 47
@ The Broken Seal's gVolley_PelletOffsets; read by Func_0815f16c
	.global gData_08198946
gData_08198946:
	.2byte 2308, 2372, 2436, 2500
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by Func_0815f16c
	.global gData_0819894e
gData_0819894e:
	.byte 0, 4, 8, 12
@ read by Func_0815f16c
	.global gData_08198952
gData_08198952:
	.byte 0, 2, 9, 10
@ read by Func_0815f16c
	.global gData_08198956
gData_08198956:
	.byte 5, 8, 10, 8
@ read by Func_0815f16c
	.global gData_0819895a
gData_0819895a:
	.byte 32, 16, 24, 32
@ read by Func_0815f16c
	.global gData_0819895e
gData_0819895e:
	.byte 0, 4, 9, 10
@ read by Func_0815f16c
	.global gData_08198962
gData_08198962:
	.byte 3, 4, 3, 5
@ read by Func_0815f16c
	.global gData_08198966
gData_08198966:
	.byte 3, 4, 5, 6
@ read by Func_08163e14
	.global gData_0819896a
gData_0819896a:
	.byte 16, 104, 32, 104, 48, 104, 64, 104, 0, 0
@ The Broken Seal's gNereid_Gravity; read by Func_08163e14
	.global gData_08198974
gData_08198974:
	.4byte 0x8000, 0x4000, 0x4000, 0x2000
@ The Broken Seal's gFormation_TwelveX; read by Func_0816442c
	.global gData_08198984
gData_08198984:
	.byte 52, 25, 57, 89, 7, 39, 71, 103, 21, 53, 85, 117
@ The Broken Seal's gFormation_TwelveZ; read by Func_0816442c
	.global gData_08198990
gData_08198990:
	.byte 0, 32, 32, 32, 64, 64, 64, 64, 96, 96, 96, 96
@ The Broken Seal's gFormation_RightEightX; read by Func_0816442c
	.global gData_0819899c
gData_0819899c:
	.byte 0, 32, 64, 16, 48, 0, 32, 64
@ The Broken Seal's gFormation_RightEightZ; read by Func_0816442c
	.global gData_081989a4
gData_081989a4:
	.byte 0, 0, 0, 32, 32, 64, 64, 64
@ read by Func_0816442c
	.global gData_081989ac
gData_081989ac:
	.byte 0, 32, 64, 0, 32, 64, 32, 64
@ read by Func_0816442c
	.global gData_081989b4
gData_081989b4:
	.byte 10, 0, 0, 42, 32, 32, 64, 64
@ The Broken Seal's gFormation_SmallEightX; read by Func_0816442c
	.global gData_081989bc
gData_081989bc:
	.byte 0, 40, 80, 0, 40, 80, 40, 80
@ The Broken Seal's gFormation_SmallEightZ; read by Func_0816442c
	.global gData_081989c4
gData_081989c4:
	.byte 13, 0, 0, 53, 40, 40, 80, 80
@ read by Func_0816442c
	.global gData_081989cc
gData_081989cc:
	.byte 16, 48, 80, 16, 48, 80, -16, 16, 48, 80, 42
@ read by Func_0816442c
	.global gData_081989d7
gData_081989d7:
	.byte 0, 0, 0, 32, 32, 32, 50, 64, 64, 64, 96
@ The Broken Seal's gImpactBurst_CellWidths; read by Func_0816467c
	.global gData_081989e2
gData_081989e2:
	.byte 48, 48, 46, 36, 36, 32, 26, 6
@ The Broken Seal's gImpactBurst_CellHeights; read by Func_0816467c
	.global gData_081989ea
gData_081989ea:
	.byte 96, 96, 90, 74, 74, 62, 52, 12
@ The Broken Seal's gImpactBurst_CellSourceOffsets; read by Func_0816467c
	.global gData_081989f2
gData_081989f2:
	.2byte 0, 4608, 9216, 13356, 16020, 18684, 20668, 22020
@ The Broken Seal's gParticleStreams_OrbitPoints; read by BattleFx_RunParticleStreams
	.global Data_08198a02
Data_08198a02:
	.2byte -64, -96, 24, -48, -120, 32, -32, -80
	.2byte -48, -16, -72, -24, 16, -80, 16, 32
	.2byte -112, -32, 48, -96, 56
@ The Broken Seal's gParticleStreams_DropPoints; read by BattleFx_RunParticleStreams
	.global Data_08198a2c
Data_08198a2c:
	.2byte 32, 32, 48, 96, 80, 80, 112, 100
	.2byte 176, 80, 196, 64, 204, 96
@ The Broken Seal's gBlastAndSmokeBomb_PuffCells; read by BattleFx_RunParticleStreams
	.global Data_08198a48
Data_08198a48:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
@ The Broken Seal's gCybele_EmberSizes; read by BattleFx_RunParticleStreams
	.global Data_08198a56
Data_08198a56:
	.2byte 16, 32, 48, 48, 48, 48, 48
@ The Broken Seal's gRamses_ObjectColumns; read by Func_08165ab4
	.global gData_08198a64
gData_08198a64:
	.byte 12, 44, 28, 60, 0, 32, 64, 0, 32
@ The Broken Seal's gRamses_ObjectRows; read by Func_08165ab4
	.global gData_08198a6d
gData_08198a6d:
	.byte 0, 0, 32, 32, 64, 64, 64, 32, 32
@ The Broken Seal's gBlastAndSmokeBomb_PuffCells; read by Func_08165ab4
	.global gData_08198a76
gData_08198a76:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
@ The Broken Seal's gCybele_EmberSizes; read by Func_08165ab4
	.global gData_08198a84
gData_08198a84:
	.2byte 16, 32, 48, 48, 48, 48, 48
@ The Broken Seal's gRagnarok_StrikeColumns; read by Func_081665a4
	.global gData_08198a92
gData_08198a92:
	.byte 32, 24, 0, 96, 104, 128
@ The Broken Seal's gSparkGroups_FlashCells; read by Func_081665a4
	.global gData_08198a98
gData_08198a98:
	.byte 5, 5, 4, 3, 2, 1
@ The Broken Seal's gSparkGroups_FlashCells; read by Func_08166b10
	.global gData_08198a9e
gData_08198a9e:
	.byte 5, 5, 4, 3, 2, 1
@ The Broken Seal's gNereid_Gravity; read by Func_08166b10
	.global gData_08198aa4
gData_08198aa4:
	.4byte 0x8000, 0x4000, 0x4000, 0x2000
@ read by Func_0816729c
	.global Data_08198ab4
Data_08198ab4:
	.byte 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 13, 14, 0
@ read by Func_0816729c
	.global Data_08198ac2
Data_08198ac2:
	.2byte 1536, 2125, 2838, 3551, 3950, 4286
@ The Broken Seal's gBattleFx_GlintCellWidths; read by Func_0816729c
	.global Data_08198ace
Data_08198ace:
	.byte 19, 23, 23, 19, 16, 17
@ The Broken Seal's gBattleFx_GlintCellHeights; read by Func_0816729c
	.global Data_08198ad4
Data_08198ad4:
	.byte 31, 31, 31, 21, 21, 24
@ The Broken Seal's gKirin_Columns; read by Func_08168a40
	.global gData_08198ada
gData_08198ada:
	.byte 8, 40, 72, 104, 8, 40, 72, 104, 0
@ The Broken Seal's gKirin_Rows; read by Func_08168a40
	.global gData_08198ae3
gData_08198ae3:
	.byte 0, 0, 0, 24, 32, 32, 32, 56, 64
@ The Broken Seal's gNereid_Gravity; read by Func_081693d0
	.global Data_08198aec
Data_08198aec:
	.4byte 0x8000, 0x4000, 0x4000, 0x2000
@ The Broken Seal's gBlastAndSmokeBomb_PuffCells; read by Func_0816a730
	.global gData_08198afc
gData_08198afc:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
@ read by Func_0816a730
	.global gData_08198b0a
gData_08198b0a:
	.2byte 16, 32, 48, 48, 48, 48, 48, 1536
	.2byte 2312, 2053, 775, 8224, 8224
@ read by Func_0816b6ec
	.global Data_08198b24
Data_08198b24:
	.byte 3, 3
@ read by Func_0816bc70
	.global Data_08198b26
Data_08198b26:
	.byte 0, 2, 4, 5
@ read by Func_0816bc70
	.global Data_08198b2a
Data_08198b2a:
	.byte 6, 7, 6, 8
@ read by Func_0816bc70
	.global Data_08198b2e
Data_08198b2e:
	.byte 32, 16, -8, -24
@ read by Func_0816bc70
	.global Data_08198b32
Data_08198b32:
	.2byte 8192, 24576, 8192, 24576, 8192, 24576, 8192, 24576
@ read by Func_0816bc70
	.global Data_08198b42
Data_08198b42:
	.2byte -2048, -2048, -2048, -2048, -2048, -2048, -2048, -2048
@ read by Func_0816c6f8
	.global Data_08198b52
Data_08198b52:
	.byte 0, 6, 8, 9
@ read by Func_0816c6f8
	.global Data_08198b56
Data_08198b56:
	.byte 5, 8, 7, 3
@ read by Func_0816c6f8
	.global Data_08198b5a
Data_08198b5a:
	.byte 32, 32, 32, 32
@ The Broken Seal's gBlastAndSmokeBomb_PuffCells; read by Func_0816d724
	.global Data_08198b5e
Data_08198b5e:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
@ The Broken Seal's gCybele_EmberSizes; read by Func_0816d724
	.global Data_08198b6c
Data_08198b6c:
	.2byte 16, 32, 48, 48, 48, 48, 48
@ read by Func_0816dc50
	.global Data_08198b7a
Data_08198b7a:
	.byte -96, -88, -72, -64, -48, -32, -8, 0
@ read by Func_0816dc50
	.global Data_08198b82
Data_08198b82:
	.byte 1, 6, 10, 12, 16, 24, 30, 32
@ read by Func_0816dc50
	.global Data_08198b8a
Data_08198b8a:
	.byte 6, 8, 10, 11
@ read by Func_0816dc50
	.global Data_08198b8e
Data_08198b8e:
	.byte 8, 7, 6, 3
@ read by Func_0816dc50
	.global Data_08198b92
Data_08198b92:
	.byte 16, 32, 56, 72
@ read by Func_0816e65c
	.global Data_08198b96
Data_08198b96:
	.byte 38, 40
@ read by Func_0816e65c
	.global Data_08198b98
Data_08198b98:
	.byte 128, 32
@ read by Func_0816e65c
	.global Data_08198b9a
Data_08198b9a:
	.byte 58, 58
@ read by Func_0816ec58
	.global Data_08198b9c
Data_08198b9c:
	.2byte 92, 72, 32, 48, 88, 56, 80, 88
	.2byte -8, 132
@ read by Func_0816ec58
	.global Data_08198bb0
Data_08198bb0:
	.byte 0, 4, 7, 10, 13, 16, 20, 23, 25, 27
@ read by Func_0816ec58
	.global Data_08198bba
Data_08198bba:
	.byte 64, 64, 48, 64, 64, 56, 62, 64, 46, 52
@ read by Func_0816ec58
	.global Data_08198bc4
Data_08198bc4:
	.byte 0, 3, 1, 2, 3, 0, 1, 2, 1, 0
@ The Broken Seal's gBlastAndSmokeBomb_PuffCells; read by Func_0816ec58
	.global Data_08198bce
Data_08198bce:
	.2byte 0, 256, 1280, 3584, 5888, 8192, 10496
@ The Broken Seal's gCybele_EmberSizes; read by Func_0816ec58
	.global Data_08198bdc
Data_08198bdc:
	.2byte 16, 32, 48, 48, 48, 48, 48
@ read by Func_0816f080
	.global Data_08198bea
Data_08198bea:
	.byte -12, 4, -8, 14
@ The Broken Seal's gRenderMode_GlintFlips; read by Func_0816fca8
	.global Data_08198bee
Data_08198bee:
	.byte 0, 4, 8, 12
	.byte 0, 0
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by Func_08170364
	.global Data_08198bf4
Data_08198bf4:
	.byte 0, 4, 8, 12
@ read by Func_08170364
	.global Data_08198bf8
Data_08198bf8:
	.byte 1, 1, 0, 0
@ read by Func_08171430
	.global Data_08198bfc
Data_08198bfc:
	.4byte 18000, 12000, 30000, 40000
@ read by Func_08171430
	.global Data_08198c0c
Data_08198c0c:
	.4byte 16578814
@ read by Func_08171430
	.global Data_08198c10
Data_08198c10:
	.byte -24, -8, 24, 48
@ read by Func_08172b00
	.global Data_08198c14
Data_08198c14:
	.byte 2, 10, 0, 0
@ read by Func_08172b00
	.global Data_08198c18
Data_08198c18:
	.4byte -0x2000, -0x1000, -2048, -1024
@ read by Func_08173aac
	.global Data_08198c28
Data_08198c28:
	.byte 0, 0, 1, 2, 4, 6, 9, 12, 16, 0, 0, 0
@ read by Func_08173aac
	.global Data_08198c34
Data_08198c34:
	.2byte 769, 2, 31, 7967, 7936, 0, 256, 2
	.2byte 0, 31, 7936, 0, 0, 0, 0, 0
	.2byte 0, 0, 0, 0, 0, 0, 0, 0
@ read by Func_08173aac
	.global Data_08198c64
Data_08198c64:
	.byte 32, 64, 255
@ read by Func_08175f74
	.global gData_08198c67
gData_08198c67:
	.byte 16, 60, 108, 0, 0
@ read by Func_0815f16c and Func_08181ed4
	.global gData_08198c6c
gData_08198c6c:
	.4byte 0x20f800, 1505302, 63519, 15398934
	.4byte 0xe1f800, 15399146, 63713, 1505514
	.4byte 0x200800, 1443862, 2079, 15337494
	.4byte 0xe10800, 15337706, 2273, 1444074
@ read by 9 functions
	.global Data_08198cac
Data_08198cac:
	.4byte 0x1ff800, 1964044, 1505302, 849949
	.4byte 63519, 16054301, 15398934, 14940172
	.4byte 0xe1f800, 14940403, 15399145, 16054498
	.4byte 63712, 915682, 1571049, 2029811
	.4byte 0x1f0800, 1902604, 1443862, 788509
	.4byte 2079, 15992861, 15337494, 14878732
	.4byte 0xe10800, 14878963, 15337705, 15993058
	.4byte 2272, 854242, 1509609, 1968371
@ read by Func_0815f16c
	.global gData_08198d2c
gData_08198d2c:
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
@ read by Func_08181ed4
	.global Data_08198df8
Data_08198df8:
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
@ read by 9 functions
	.global Data_08198ec4
Data_08198ec4:
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
@ read by Func_0816c6f8
	.global Data_08199050
Data_08199050:
	.4byte 0x40f800, 2947116, 63550, 13957164
	.4byte 0xc2f800, 13957332, 63682, 2947284
	.4byte 0x200000, 1441814, 31, 15335446
	.4byte 0xe10000, 15335658, 225, 1442026
@ read by 4 functions
	.global gData_08199090
gData_08199090:
	.4byte 0x26f800, 1767450, 63525, 15136794
	.4byte 0xdbf800, 15136998, 63707, 1767654
	.4byte 0x200000, 1441814, 31, 15335446
	.4byte 0xe10000, 15335658, 225, 1442026
@ read by 5 functions
	.global gData_081990d0
gData_081990d0:
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
@ read by Func_0816dc50 and Func_08190b74
	.global Data_0819919c
Data_0819919c:
	.4byte 0x20100, 0
@ read by Func_0816dc50 and Func_08190b74
	.global Data_081991a4
Data_081991a4:
	.4byte 16384250, 16384006, 393466
@ read by 7 functions
	.global gData_081991b0
gData_081991b0:
	.4byte 32762, 32518, 254, 2
@ read by 7 functions
	.global Data_081991c0
Data_081991c0:
	.4byte 32762, 32518, 250, 6
@ read by Func_08173aac and Func_081848ec
	.global Data_081991d0
Data_081991d0:
	.2byte 32518, 0, 32762, 0, 64518, 0, 64762, 0
@ read by 20 functions
	.global Data_081991e0
Data_081991e0:
	.2byte 57568, 0, 57376, 0, 8416, 0, 8224, 0
@ read by 4 functions
	.global gData_081991f0
gData_081991f0:
	.2byte 49376, 0, 49184, 0, 224, 0, 32, 0
@ read by Func_08189b14
	.global Data_08199200
Data_08199200:
	.4byte 12599536, 12599312, 14576, 14352
@ read by 23 functions
	.global gData_08199210
gData_08199210:
	.4byte 12583104, 12582976, 4194496, 4194368
@ read by 6 functions
	.global Data_08199220
Data_08199220:
	.2byte 769, 2, 31, 7967, 7936, 0, 256, 2
	.2byte 0, 31, 7936, 0, 0, 0, 0, 0
	.2byte 0, 0
@ read by 5 functions
	.global Data_08199244
Data_08199244:
	.2byte 256, 2, 0, 31, 7936, 0, 769, 2
	.2byte 31, 7967, 7936, 0, 0, 0, 0, 0
	.2byte 0, 0
@ read by 6 functions
	.global gData_08199268
gData_08199268:
	.2byte 769, 2, 31, 30495, 30464, 0, 256, 2
	.2byte 0, 31, 30464, 0, 0, 0, 0, 0
	.2byte 0, 0
@ read by Func_0816e65c
	.global Data_0819928c
Data_0819928c:
	.2byte 256, 2, 0, 31, 32512, 0, 769, 2
	.2byte 31, 32543, 32512, 0, 0, 0, 0, 0
	.2byte 0, 0
@ read by 6 functions
	.global Data_081992b0
Data_081992b0:
	.2byte 256, 2, 256, 287, 16128, 0, 769, 2
	.2byte 287, 16159, 16128, 0, 0, 0, 0, 0
	.2byte 0, 0, 256, 2, 0, 15, 16128, 0
	.2byte 769, 2, 15, 16143, 16128, 0, 0, 0
	.2byte 0, 0, 0, 0
@ read by Func_08186dfc
	.global Data_081992f8
Data_081992f8:
	.2byte 256, 2, 0, 63, 7936, 0, 769, 2
	.2byte 63, 7999, 7936, 0, 0, 0, 0, 0
	.2byte 0, 0
@ read by Func_081848ec
	.global Data_0819931c
Data_0819931c:
	.2byte 256, 2, 0, 31, 12032, 0, 769, 2
	.2byte 31, 12063, 12032, 0, 0, 0, 0, 0
	.2byte 0, 0
@ read by 21 functions
	.global gData_08199340
gData_08199340:
	.2byte 769, 2, 319, 16191, 16129, 0, 256, 2
	.2byte 257, 319, 16129, 0, 0, 0, 0, 0
	.2byte 0, 0
@ read by 24 functions
	.global gData_08199364
gData_08199364:
	.2byte 769, 2, 127, 32639, 32512, 0, 256, 2
	.2byte 0, 127, 32512, 0, 0, 0, 0, 0
	.2byte 0, 0
@ read by Func_08186dfc
	.global Data_08199388
Data_08199388:
	.2byte 769, 2, 47, 18223, 18176, 0, 256, 2
	.2byte 0, 47, 18176, 0, 0, 0, 0, 0
	.2byte 0, 0, 769, 2, 4143, 12079, 12048, 0
	.2byte 256, 2, 4112, 4143, 12048, 0, 0, 0
	.2byte 0, 0, 0, 0
@ read by Func_08178ee4
	.global gData_081993d0
gData_081993d0:
	.byte 32, 39, 42, 71
@ read by Func_08178ee4
	.global gData_081993d4
gData_081993d4:
	.byte 29, 32, 48, 57
@ read by Func_08178ee4
	.global gData_081993d8
gData_081993d8:
	.2byte 0, 928, 2176, 4192
@ read by Func_08178ee4
	.global gData_081993e0
gData_081993e0:
	.byte 29, 37, 48, 47, 52, 32, 22, 17
@ read by Func_08178ee4
	.global gData_081993e8
gData_081993e8:
	.byte 34, 26, 15, 14, 7, 11, 8, 6
@ read by Func_08178ee4
	.global gData_081993f0
gData_081993f0:
	.2byte 0, 928, 2112, 3648, 5152, 6816, 7840, 8544
@ read by Func_08179394
	.global Data_08199400
Data_08199400:
	.byte 8, 127, 64, 32, 12, 191, 96, 64, 16, 255, 128, 255
@ read by Func_08179994
	.global Data_0819940c
Data_0819940c:
	.byte 1, 24, 4, 64, 32, 4, 0, 2, 32, 8, 32, 64, 8, 0, 2, 48
	.byte 16, 32, 128, 16, 0
@ read by Func_08179f18
	.global Data_08199421
Data_08199421:
	.byte 4, 8, 16, 24, 0
@ read by Func_08179f18
	.global Data_08199426
Data_08199426:
	.2byte 0, 32, 160, 672
@ read by Func_08179f18
	.global Data_0819942e
Data_0819942e:
	.2byte 114, 117, 120, 250
@ read by Func_08179f18
	.global Data_08199436
Data_08199436:
	.2byte 0, 24576, -28576, 11008
@ read by Func_08179f18
	.global Data_0819943e
Data_0819943e:
	.2byte 16384, 4096, 2096, -7168
@ read by Func_08179f18
	.global Data_08199446
Data_08199446:
	.byte 16, 16, 16, -16
@ read by Func_08179f18
	.global Data_0819944a
Data_0819944a:
	.byte 0, 0, 0, 12, 0, 0
@ read by Func_0817b66c
	.global gData_08199450
gData_08199450:
	.2byte 256, 2, 6, 25, 7942, 0, 769, 2
	.2byte 25, 7961, 7942, 0, 0, 0, 0, 0
	.2byte 0, 0
@ read by Func_0817b66c
	.global gData_08199474
gData_08199474:
	.2byte 61680, 0, 61456, 0, 16624, 0, 16400, 0
@ read by Func_0817c6ac
	.global Data_08199484
Data_08199484:
	.2byte 16384, 16, 24, 96, 8, 4096, 24576, 16
	.2byte 48, 192, 16, 8192, 40768, 32, 38, 256
	.2byte 32, 16384
@ read by Func_0817cc30
	.global Data_081994a8
Data_081994a8:
	.4byte -0x2000, -0x4000, -0x4000, -0x8000
@ read by Func_0817e0c0
	.global Data_081994b8
Data_081994b8:
	.4byte 0x8000, 0x8000, 0x8000, 0x8000
	.4byte 55536, 52768
@ read by Func_0817e698
	.global gData_081994d0
gData_081994d0:
	.4byte 39768, 44152, 44152, 55536
@ read by Func_0817e698
	.global gData_081994e0
gData_081994e0:
	.4byte -8, -8, -8, 0
@ read by Func_0817ea58
	.global Data_081994f0
Data_081994f0:
	.byte 0, -1, 1, -1, 0, 2, 1, 0, -1, -1, -2, -2, -3, -3, -4, -4
	.byte -4, -3, -3, -3, -2, -2, -1, -1
@ read by Func_0817ea58
	.global Data_08199508
Data_08199508:
	.byte 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1
@ read by Func_0817ea58
	.global Data_08199514
Data_08199514:
	.byte 112, 128, 128, 112, 112, 128, 128, 112, 128, 112, 112, 112, 180, 120, 140, 120
	.byte 140, 120, 136, 120
@ read by Func_0817ea58
	.global Data_08199528
Data_08199528:
	.4byte -120000, -70000, -117000, -68000
	.4byte -120000, -70000, -117000, -68000
	.4byte -80000, -80000, -80000, -95000
	.4byte -100000, -80000, -100000, -90000
	.4byte -120000, -95000, -100000, -95000
@ read by Func_0817ea58
	.global Data_08199578
Data_08199578:
	.byte 16, 16, 40, 40, 40, 68, 40, 72, 72, 96
@ read by Func_0817ea58
	.global Data_08199582
Data_08199582:
	.2byte 16, 115, 243, 371, 499
@ read by Func_0817ea58
	.global Data_0819958c
Data_0819958c:
	.2byte 568, 5
@ read by Func_0817ea58
	.global Data_08199590
Data_08199590:
	.byte 66, 64
@ read by Func_0817ea58
	.global Data_08199592
Data_08199592:
	.byte 0, 0, 32, 0, 0, 32, 32, 32, 32, 64
@ read by Func_0817ea58
	.global Data_0819959c
Data_0819959c:
	.byte 60, 64, 58, 72, 56, 80, 54, 88, 54, 96, 56, 104, 78, 56, 78, 64
	.byte 80, 72, 82, 80, 84, 88, 84, 96, 76, 60, 74, 60, 72, 60, 70, 60
	.byte 68, 60, 66, 60, 60, 64, 61, 64, 62, 63, 63, 62, 64, 61, 66, 60
@ read by Func_0817ea58
	.global Data_081995cc
Data_081995cc:
	.2byte 0, 24, 8, 52, 16, 40, 36, 28
	.2byte 32, 4, 20, 56, 60, 12, 44, 48
@ read by Func_08180c94
	.global Data_081995ec
Data_081995ec:
	.byte 80, 84, 86, 88, 90, 92, 94
@ read by Func_081811d4
	.global Data_081995f3
Data_081995f3:
	.byte 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1
	.byte 1, 1, 1, 1, 1
@ read by Func_08181910
	.global Data_08199608
Data_08199608:
	.2byte 769, 2, 3711, 13695, 13568, 0, 256, 2
	.2byte 3584, 3711, 13568, 0, 0, 0, 0, 0
	.2byte 0, 0
@ read by Func_08181910
	.global Data_0819962c
Data_0819962c:
	.2byte 769, 2, 127, 8063, 7936, 0, 256, 2
	.2byte 0, 127, 7936, 0, 0, 0, 0, 0
	.2byte 0, 0
@ read by Func_08181910
	.global Data_08199650
Data_08199650:
	.2byte 61615, 0, 61487, 0, 4271, 0, 4143, 0
@ read by Func_08181ed4
	.global Data_08199660
Data_08199660:
	.4byte -0x4000, -0x8000, -0x8000
@ read by Func_08181ed4
	.global Data_0819966c
Data_0819966c:
	.byte 1, 4, 5
@ read by Func_08182898
	.global gData_0819966f
gData_0819966f:
	.byte 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 4, 3
@ read by Func_08182898
	.global gData_0819967e
gData_0819967e:
	.byte 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 0
@ read by Func_08182898
	.global gData_0819968e
gData_0819968e:
	.2byte 0, 512, 962, 1354, 1692, 1980, 2222, 2422
	.2byte 2584, 2712, 2810, 2882, 2932, 2964, 2988
@ read by Func_08182aa8
	.global Data_081996ac
Data_081996ac:
	.byte 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 4, 3
@ read by Func_08182aa8
	.global Data_081996bb
Data_081996bb:
	.byte 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4
@ read by Func_08182aa8
	.global Data_081996ca
Data_081996ca:
	.2byte 0, 512, 962, 1354, 1692, 1980, 2222, 2422
	.2byte 2584, 2712, 2810, 2882, 2932, 2964, 2988
@ read by Func_08182aa8
	.global Data_081996e8
Data_081996e8:
	.2byte 0, 0, 10, 78, 82, 112, 250, 254
	.2byte 284
@ read by Func_08182aa8
	.global Data_081996fa
Data_081996fa:
	.byte 32, 64, 80, 32, 16, 88, 32, 16, 88
@ read by Func_08182aa8
	.global Data_08199703
Data_08199703:
	.byte 80, 96, 120, 120, 120, 120, 120, 120, 120
@ read by Func_08182aa8
	.global Data_0819970c
Data_0819970c:
	.byte 32, 8, 32, 8, 16, 16, 8, 16, 16
@ read by Func_08182aa8
	.global Data_08199715
Data_08199715:
	.byte 16, 8, 16, 8, 12, 12, 8, 12, 12
@ read by Func_08182aa8
	.global Data_0819971e
Data_0819971e:
	.byte -24, 4, 20, 0, -2, 2, 0, -2, 2
@ read by Func_08182aa8
	.global Data_08199727
Data_08199727:
	.byte -64, -32, -64, -16, -32, -32, -16, -32, -32
@ read by Func_08182aa8
	.global Data_08199730
Data_08199730:
	.byte 1, 0, 4, -2, 1, 0, -2, 1, 0
@ read by Func_08182aa8
	.global Data_08199739
Data_08199739:
	.byte 16, 1, 12, 0, 0, 0, 0, 0, 0
@ read by Func_08182aa8
	.global Data_08199742
Data_08199742:
	.byte 3, 5, 7, 9
@ read by Func_08182aa8
	.global Data_08199746
Data_08199746:
	.2byte 81, 209, 518, 540
@ read by Func_08182aa8
	.global Data_0819974e
Data_0819974e:
	.2byte 498, 503, 508
@ read by Func_08182aa8
	.global Data_08199754
Data_08199754:
	.2byte 518
@ read by Func_08182aa8
	.global Data_08199756
Data_08199756:
	.byte 0, 82
@ read by Func_08182aa8
	.global Data_08199758
Data_08199758:
	.2byte 4096, 18384
@ read by Func_08182aa8
	.global Data_0819975c
Data_0819975c:
	.2byte 8192, 40960
@ read by Func_08182aa8
	.global Data_08199760
Data_08199760:
	.2byte 256, 572
@ read by Func_08182aa8
	.global Data_08199764
Data_08199764:
	.2byte 44160, 52720
@ read by Func_081848ec
	.global Data_08199768
Data_08199768:
	.2byte 4, 26, 30, 34, 104, 112, 116, 120
	.2byte 150, 154, 202, 214, 244, 278, 400, 402
	.2byte 404, 420
@ read by Func_081848ec
	.global Data_0819978c
Data_0819978c:
	.2byte 8, 2, 2, 16, 2, 2, 2, 10
	.2byte 2, 16, 4, 8, 10, 12, 2, 2
	.2byte 10, 4
@ read by Func_081848ec
	.global Data_081997b0
Data_081997b0:
	.byte 0, 32, 64, 96, 32, 64, 96, 128, 64, 96, 128, 160, 32, 64, 96, 128
	.byte 25, 57, 89, 121, 153, 32, 64, 96, 128, 160, 48, 96
@ read by Func_081848ec
	.global Data_081997cc
Data_081997cc:
	.byte 0, 0, 0, 0, 32, 32, 32, 32, 64, 64, 64, 64, 96, 96, 96, 96
	.byte 128, 128, 128, 128, 128, 160, 160, 160, 160, 160, 192, 192
@ read by Func_081848ec
	.global Data_081997e8
Data_081997e8:
	.byte 16, 0, 32, 0, 32, 16, 16, 16, 16, 16, 16, 0
@ read by Func_081848ec
	.global Data_081997f4
Data_081997f4:
	.2byte 0, 32, 32, 64, 64, 96, 128, 160
	.2byte 192, 224, 256
@ read by Func_081848ec
	.global Data_0819980a
Data_0819980a:
	.byte 224, 192, 160, 128, 192, 160
@ read by Func_081848ec
	.global Data_08199810
Data_08199810:
	.byte 0, 0, 0, 0, 32, 32
@ read by Func_081848ec
	.global Data_08199816
Data_08199816:
	.2byte 26, 104, 116, 216, 278, 402, 416
@ read by Func_081848ec
	.global Data_08199824
Data_08199824:
	.byte 16, 96, 20, 40, 50, 10, 96
@ read by Func_081848ec
	.global Data_0819982b
Data_0819982b:
	.byte 64, 80, 30, 40, 80, 60, 20
@ read by Func_081848ec
	.global Data_08199832
Data_08199832:
	.byte 0, 1, 0, 1, 0, 0, 1, 0
@ read by Func_081848ec
	.global Data_0819983a
Data_0819983a:
	.2byte 246, 249, 254, 258, 260, 264, 266, 269
	.2byte 272, 275, 278, 280, 282, 284, 287, 290
	.2byte 292
@ read by Func_081848ec
	.global Data_0819985c
Data_0819985c:
	.byte 71, 71, 71, 70, 70, 70, 70, 70, 70, 69, 69, 0, 67, 67, 67, 70
	.byte 67, 68, 70, 69, 70, 69, 67, 0, 69, 69, 69, 68, 67, 68, 70, 69
	.byte 69, 69, 0, 0
@ read by Func_081848ec
	.global Data_08199880
Data_08199880:
	.byte 92, 92, 92, 120, 120, 120, 154, 154, 154, 220, 200, 0, 100, 100, 100, 120
	.byte 120, 154, 154, 154, 154, 220, 240, 0, 120, 100, 100, 140, 120, 154, 154, 154
	.byte 184, 220, 0, 0
@ read by Func_081848ec
	.global Data_081998a4
Data_081998a4:
	.2byte 492, 506, 526, 528, 542, 562, 572, 578
	.2byte 598, 600, 608, 65535, 0, 504, 518, 526
	.2byte 546, 548, 554, 562, 564, 570, 590, 594
	.2byte 596, 65535, 510, 524, 544, 546, 548, 558
	.2byte 560, 580, 582, 596, 616, 65535, 0
@ read by Func_081848ec
	.global Data_081998f2
Data_081998f2:
	.byte 6, 4, 0, 8, 8, 0, 10, 20, 0, 20, 0, 0, 6, 4, 0, 8
	.byte 8, 0, 10, 20, 8, 20, 0, 0, 6, 4, 0, 8, 8, 20, 0, 0
	.byte 20, 20, 0, 0
@ read by Func_081848ec
	.global Data_08199916
Data_08199916:
	.2byte 144, 0, 0, 128, 0, 0, 180, 0
	.2byte 0, 150, 0, 0, 112, 0, 0, 0
	.2byte 112, 48, 0, 86, 0, 0, 112, 0
	.2byte 96, 0, 0, 96, 0, 112, 128, 0
	.2byte 104, 0, 0, 0
@ read by Func_081848ec
	.global Data_0819995e
Data_0819995e:
	.2byte 256, 64, 0, 712, 500, 0, 0, 0
	.2byte 0, 356, 0, 0, -356, -124, -256, 712
	.2byte -64, 256, 0, 0, -228, 356, 0, 0
	.2byte 406, 64, -256, 0, -64, 0, 128, 0
	.2byte 328, -356, 0, 0
@ read by Func_081848ec
	.global Data_081999a6
Data_081999a6:
	.2byte 0, 1, -1, 0, 1, -1, 0, 1
	.2byte -1, 0, -1, 0, 2, 1, 0, -1
	.2byte 2, 2, -1, 0, 1, -1, 2, -1
	.2byte 0, 1, -1, 0, -1, 0, 1, -1
	.2byte 0, 1, -1, 0
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by Func_081848ec
	.global Data_081999ee
Data_081999ee:
	.byte 0, 4, 8, 12
@ read by Func_081848ec
	.global Data_081999f2
Data_081999f2:
	.2byte 14797, -1097, -8198, 4962, 15494, -5489, -9298, 3744
	.2byte 11252, -9996, 15443, -4373, -15930, 15898, -13623, 3906
@ read by Func_08186dfc
	.global Data_08199a12
Data_08199a12:
	.2byte 0, 0, 0, 32, 32, 32, 64, 32
	.2byte 96, 42, 0, 64, 32, 64, 64, 64
	.2byte 96, 74, 32, 96, 64, 96
@ read by Func_08186dfc
	.global Data_08199a3e
Data_08199a3e:
	.byte 120, 88, 144, 112, 0, 0
@ The Broken Seal's gWorldMap_UnitScale; read by Func_08186dfc
	.global Data_08199a44
Data_08199a44:
	.4byte 0x10000, 0x10000
@ read by Func_08186dfc
	.global Data_08199a4c
Data_08199a4c:
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
@ read by Func_08186dfc
	.global Data_08199bec
Data_08199bec:
	.byte 27, 15, 13, 5, 14, 16, 11, 13, 8, 18, 24, 26, 100
@ read by Func_08186dfc
	.global Data_08199bf9
Data_08199bf9:
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
@ read by Func_08186dfc
	.global Data_08199d2f
Data_08199d2f:
	.byte 10, 30
@ The Broken Seal's gHelmSplitter_CellWidths; read by Func_08188cb0
	.global gData_08199d31
gData_08199d31:
	.byte 26, 46, 63, 77, 60, 39, 32
@ read by Func_08188cb0
	.global gData_08199d38
gData_08199d38:
	.byte 11, 20, 37, 65, 65, 54, 39, 0
@ The Broken Seal's gHelmSplitter_CellSourceOffsets; read by Func_08188cb0
	.global gData_08199d40
gData_08199d40:
	.2byte 0, 286, 1206, 3537, 8542, 12442, 14548
@ The Broken Seal's gHelmSplitter_CellBiasX; read by Func_08188cb0
	.global gData_08199d4e
gData_08199d4e:
	.byte 53, 36, 16, 2, 2, 2, 2
@ The Broken Seal's gHelmSplitter_CellBiasY; read by Func_08188cb0
	.global gData_08199d55
gData_08199d55:
	.byte 2, 3, 3, 3, 4, 14, 29
@ read by Func_08189148
	.global Data_08199d5c
Data_08199d5c:
	.byte 48, 40, 0, 0
@ read by Func_0818a220
	.global Data_08199d60
Data_08199d60:
	.byte 124, 140, 132, 130
@ read by Func_0818a220
	.global Data_08199d64
Data_08199d64:
	.4byte 0x2000, 62, 1, 0x2000
	.4byte 62, 1, -0x2000, 62
	.4byte 1, 2048, 64, 4
@ read by Func_0818b868
	.global gData_08199d94
gData_08199d94:
	.byte 0, 12, 24
@ read by Func_0818bc98
	.global Data_08199d97
Data_08199d97:
	.byte 90, 38, 32, 48
@ read by Func_0818bc98
	.global Data_08199d9b
Data_08199d9b:
	.byte 56, 0, 0, 0
@ read by Func_0818bc98
	.global Data_08199d9f
Data_08199d9f:
	.byte 24, 32, 32, 24
@ read by Func_0818bc98
	.global Data_08199da3
Data_08199da3:
	.byte 56, 58, 60, 61, 8, 12, 16, 20, 0, 2, 4, 5
@ read by Func_0818bc98
	.global Data_08199daf
Data_08199daf:
	.byte 5, 6, 7, 6, 10, 9, 11, 10, 5, 6, 7, 6
@ read by Func_0818bc98
	.global Data_08199dbb
Data_08199dbb:
	.byte -32, -48, -56, -72, -16, -32, -64, -72, -32, -48, -56, -72
@ read by Func_0818bc98
	.global Data_08199dc7
Data_08199dc7:
	.byte 56, 14, 6, 56
@ read by Func_0818cb74
	.global Data_08199dcb
Data_08199dcb:
	.byte 8, 10, 12, 13, 64, 67, 70, 72
@ read by Func_0818cb74
	.global Data_08199dd3
Data_08199dd3:
	.byte 8, 7, 6, 7, 8, 8, 9, 8
@ read by Func_0818cb74
	.global Data_08199ddb
Data_08199ddb:
	.byte 16, 0, -24, -40, 16, 0, -24, -40, 0
@ read by Func_0818cb74
	.global Data_08199de4
Data_08199de4:
	.4byte 5528, 488, 62272, 308
	.4byte 52392, -698, 34584, 634
	.4byte 19692, 736, 28062, 746
	.4byte 48958, -694, 1508, -576
@ read by Func_0818cb74
	.global Data_08199e24
Data_08199e24:
	.2byte 32762, 0, 32518, 0, 64762, 0, 64518, 0
	.2byte 32762, 0, 32518, 0, 250, 0, 6, 0
	.2byte 32758, 0, 32514, 0, 65526, 0, 65282, 0
@ The Broken Seal's gTwelveMode_GlintDrawFlags; read by Func_0818cb74
	.global Data_08199e54
Data_08199e54:
	.byte 0, 4, 8, 12
@ read by Func_0818d5a8
	.global Data_08199e58
Data_08199e58:
	.byte 0, 4, 8, 9
@ read by Func_0818d5a8
	.global Data_08199e5c
Data_08199e5c:
	.byte 9, 8, 9, 3
@ read by Func_0818d5a8
	.global Data_08199e60
Data_08199e60:
	.byte 32, 32, 32, 32
@ read by Func_0818d5a8
	.global Data_08199e64
Data_08199e64:
	.byte 0, 4, 12, 19
@ read by Func_0818d5a8
	.global Data_08199e68
Data_08199e68:
	.byte 7, 8, 7, 8
@ read by Func_0818d5a8
	.global Data_08199e6c
Data_08199e6c:
	.byte 32, 16, 16, 24
@ read by Func_0818df5c
	.global Data_08199e70
Data_08199e70:
	.byte 30, 16, 16, 16, 10, 8, 8
@ read by Func_0818df5c
	.global Data_08199e77
Data_08199e77:
	.byte 30, 16, 16, 16, 16, 8, 8
@ read by Func_0818df5c
	.global Data_08199e7e
Data_08199e7e:
	.2byte 0, 900, 1156, 1412, 1668, 1828, 1892
@ read by Func_0818df5c
	.global Data_08199e8c
Data_08199e8c:
	.byte 0, 4, 9, 10
@ read by Func_0818df5c
	.global Data_08199e90
Data_08199e90:
	.byte 3, 4, 3, 5
@ read by Func_0818df5c
	.global Data_08199e94
Data_08199e94:
	.byte 3, 4, 5, 6
@ read by Func_0818df5c
	.global Data_08199e98
Data_08199e98:
	.byte 0, 3, 16, 7
@ read by Func_0818df5c
	.global Data_08199e9c
Data_08199e9c:
	.byte 5, 8, 10, 8
@ read by Func_0818df5c
	.global Data_08199ea0
Data_08199ea0:
	.byte -40, 24, 24, 24
@ read by Func_0818eda4
	.global Data_08199ea4
Data_08199ea4:
	.2byte 49360, 0, 49200, 0, 1248, 0, 1056, 0
@ read by Func_0818eda4
	.global Data_08199eb4
Data_08199eb4:
	.byte 18, 21, 16, 17, 22, 19, 23, 18
@ read by Func_0818eda4
	.global Data_08199ebc
Data_08199ebc:
	.byte 40, 56, 70, 48, 64, 80, 58, 64, 72
@ read by Func_0818f620
	.global Data_08199ec5
Data_08199ec5:
	.byte 5, 5, 4, 3, 2, 1, 0
@ read by Func_0818fefc
	.global Data_08199ecc
Data_08199ecc:
	.2byte 0, -10240, 0, -10240, 10240
@ read by Func_0818fefc
	.global Data_08199ed6
Data_08199ed6:
	.2byte 0, -4864, 0, -4864, -4864
@ read by Func_0818fefc
	.global Data_08199ee0
Data_08199ee0:
	.2byte 4, 8, 171, 175, 176
@ read by Func_0818fefc
	.global Data_08199eea
Data_08199eea:
	.2byte -12544, -10496, -8704, 8704, -8448, 12800, 10752, 11264
@ read by Func_0818fefc
	.global Data_08199efa
Data_08199efa:
	.2byte 0, 11776, 0, -14336, 11264, -19968, -22016, -22272
@ read by Func_0818fefc
	.global Data_08199f0a
Data_08199f0a:
	.2byte -21504, 13312, 22224, -18944, -32768, 8448, 25600, -28928
@ read by Func_0818fefc
	.global Data_08199f1a
Data_08199f1a:
	.byte 6, 9, 6, 2, 6, 3, 6, 7
@ read by Func_0818fefc
	.global Data_08199f22
Data_08199f22:
	.byte -6, -4, -7, -9, -6, -7, -9, -7
@ read by Func_08190b74
	.global Data_08199f2a
Data_08199f2a:
	.2byte 0, -10240
@ read by Func_08190b74
	.global Data_08199f2e
Data_08199f2e:
	.2byte 0, -4864, 0
@ read by Func_08191d60
	.global Data_08199f34
Data_08199f34:
	.byte 0, 6, 8, 9
@ read by Func_08191d60
	.global Data_08199f38
Data_08199f38:
	.byte 7, 8, 7, 8
@ read by Func_08191d60
	.global Data_08199f3c
Data_08199f3c:
	.byte 24, 12, -24, 0
@ read by Func_08192c9c
	.global Data_08199f40
Data_08199f40:
	.4byte 24, 16
@ read by Func_08192c9c
	.global Data_08199f48
Data_08199f48:
	.byte 7, 5, 0, 0
@ read by Func_0819273c
	.global Data_08199f4c
Data_08199f4c:
	.2byte 256, 2, 0, 15, 3840, 0, 258, 3
	.2byte 3840, 15, 3855, 0, 770, 4, 3584, 3599
	.2byte 3840, 0, 772, 5, 3840, 3599, 3855, 0
	.2byte 0, 0, 0, 0, 0, 0
@ read by Func_08192a4c
	.global Data_08199f88
Data_08199f88:
	.2byte 63740, 0, 63492, 0, 2300, 0, 2052, 0
@ read by Func_08192a4c
	.global Data_08199f98
Data_08199f98:
	.4byte 0x20100, 0x1f0000, 0x3f00, 196866
	.4byte 0x1f3f00, 16159, 0, 0
	.4byte 0
@ read by Func_08192c9c
	.global Data_08199fbc
Data_08199fbc:
	.byte 84, 93, 108, 111, 116, 132, 135, 141, 153, 158, 171, 0
@ read by Func_08192c9c
	.global Data_08199fc8
Data_08199fc8:
	.2byte 231, 244, 259, 265, 270
@ read by Func_08192c9c
	.global Data_08199fd2
Data_08199fd2:
	.2byte 279, 287, 293, 298, 302, 307
@ read by Func_08192c9c
	.global Data_08199fde
Data_08199fde:
	.byte -10, 80, 110, -10, 76, 50, 110, 80, 40, 30, 80, 20, 50, 80, 20
@ read by Func_08192c9c
	.global Data_08199fed
Data_08199fed:
	.byte 116, 136, 128, 112, 136, 136, 128, 136, 136, 136, 136, 136, 136, 136, 136
@ read by Func_08192c9c
	.global Data_08199ffc
Data_08199ffc:
	.byte 208, 71, 24, 244, 208, 199, 0, 64, 24, 244, 232, 11, 208, 199, 24, 244
	.byte 208, 15, 160, 23, 72, 236, 136, 27, 232, 11, 24, 244, 160, 23
@ read by Func_08192c9c
	.global Data_0819a01a
Data_0819a01a:
	.2byte -406, 64, 380, -326, 64, -64, 380, 64
	.2byte -84, -64, 94, -94, -64, 64, -64
@ read by Func_08192c9c
	.global Data_0819a038
Data_0819a038:
	.byte 23, 47, 56, 63, 63, 56, 47, 23
@ read by Func_08192c9c
	.global Data_0819a040
Data_0819a040:
	.4byte 7, 5
@ read by Func_08192c9c
	.global Data_0819a048
Data_0819a048:
	.2byte 230, 238, 260, 265, 253, 258
@ read by Func_08192c9c
	.global Data_0819a054
Data_0819a054:
	.2byte 320, 328, 330, 335, 326, 332
@ read by Func_08192c9c
	.global Data_0819a060
Data_0819a060:
	.byte 34, 86, 18, 102, 8, 112
@ read by Func_08192c9c
	.global Data_0819a066
Data_0819a066:
	.byte 64, 62, 32, 32, 80, 80
@ read by Func_08192c9c
	.global Data_0819a06c
Data_0819a06c:
	.byte 0, 0, 1, 1, 0, 0
@ read by Func_08192c9c
	.global Data_0819a072
Data_0819a072:
	.byte 8, 4, 1, 2, 4, 8
@ read by Func_08192c9c
	.global Data_0819a078
Data_0819a078:
	.byte 16, 16, 16, 16, 32, 32
@ read by Func_08192c9c
	.global Data_0819a07e
Data_0819a07e:
	.byte 1, 1, 1, 1, 2, 2
@ read by Func_08192c9c
	.global Data_0819a084
Data_0819a084:
	.2byte 49402, 0, 49170, 0, 57591, 0, 57359, 0
	.2byte 243, 0, 8, 0, 8429, 0, 8447, 0
	.2byte 16612, 0, 16628, 0
@ read by Func_08192c9c
	.global Data_0819a0ac
Data_0819a0ac:
	.2byte 49187, 0, 49224, 0, 57372, 0, 57410, 0
	.2byte 18, 0, 51, 0, 8198, 0, 8225, 0
	.2byte 16628, 0, 16393, 0
@ read by Func_08192c9c
	.global Data_0819a0d4
Data_0819a0d4:
	.byte 0, 1, 2, 0, 0, 0, 31, 0, 0, 15, 0, 0, 2, 1, 3, 0
	.byte 0, 15, 31, 0, 31, 15, 0, 0, 2, 3, 4, 0, 0, 15, 31, 15
	.byte 0, 31, 0, 0, 4, 3, 5, 0, 0, 31, 31, 15, 31, 31, 0, 0
	.byte 4, 5, 6, 0, 0, 31, 31, 31, 0, 47, 0, 0, 6, 5, 7, 0
	.byte 0, 47, 31, 31, 31, 47, 0, 0, 6, 7, 8, 0, 0, 47, 31, 47
	.byte 0, 63, 0, 0, 8, 7, 9, 0, 0, 63, 31, 47, 31, 63, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
@ read by Func_08192c9c
	.global Data_0819a140
Data_0819a140:
	.2byte 55296, 0, 64763, 0, 64517, 0, 4342, 0
	.2byte 4106, 0, 8428, 0, 8212, 0, 0, 0
@ read by Func_08192c9c
	.global Data_0819a160
Data_0819a160:
	.4byte 458753, 0x1f00, 7951, 197121
	.4byte 0x1f0000, 0x1f00, 262659, 0x1f1f00
	.4byte 7967, 328707, 0x1f0000, 0x1f00
	.4byte 394245, 0x1f1f00, 7967, 131079
	.4byte 2039567, 7967
