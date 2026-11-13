	.section .rodata
@ Three 8x8 glints that Func_080e1678 draws into its 64x64 canvas, the
@ first two in turn; drawn in the basic colour palette and the ramp from
@ blue to white that FieldPsynergy_Unnamed29 writes to OBJ colours 0xe0 to 0xef
	.global Data_080eda0c
Data_080eda0c:
	.incbin "games/tla/src/field/event_commands/glint.8bpp"
@ 110 enemy encounter groups.
	.global gEncounter_EnemyGroupTable
gEncounter_EnemyGroupTable:
	.incbin "games/tla/src/field/event_commands/encounter.table"
	.section .rodata.a
@ A copy of The Broken Seal's gScreenTransition_NoiseWidths
	.global gScreenTransition_NoiseWidths
gScreenTransition_NoiseWidths:
	.byte 0, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 7, 8, 9, 11, 13
	.byte 16, 18, 22, 26, 30, 36, 43, 51, 60, 72, 85, 101, 120, 143, 170, 202, 240
@ A copy of The Broken Seal's gScreenTransition_EdgeOffsets
	.global gScreenTransition_EdgeOffsets
gScreenTransition_EdgeOffsets:
	.byte -4, -3, -3, -3, -2, -2, -1, 0, 0, 0, 1, 2, 2, 3, 3, 3
	.byte 4, 3, 3, 3, 2, 2, 1, 0, 0, 0, -1, -2, -2, -3, -3, -3
@ A copy of The Broken Seal's gScreenTransition_DitherTable
	.global gScreenTransition_DitherTable
gScreenTransition_DitherTable:
	.byte 0, 12, 35, 3, 29, 39, 6, 7, 50, 59, 4, 11, 18, 40, 46, 31
	.byte 16, 42, 47, 51, 20, 23, 43, 32, 8, 52, 17, 41, 54, 15, 63, 24
	.byte 58, 5, 2, 26, 36, 61, 9, 34, 56, 27, 10, 48, 53, 30, 28, 22
	.byte 62, 45, 19, 60, 57, 49, 25, 55, 38, 14, 44, 13, 33, 37, 1, 21
@ A copy of The Broken Seal's gColorBuffer_ChannelCurve
	.global Data_080f0246
Data_080f0246:
	.2byte 0x0000, 0x020c, 0x042f, 0x066b, 0x08c0, 0x0b2f, 0x0db9, 0x105f
	.2byte 0x1322, 0x1603, 0x1903, 0x1c22, 0x1f63, 0x22c5, 0x264b, 0x29f4
	.2byte 0x2dc3, 0x31b9, 0x35d7, 0x3a1d, 0x3e8e, 0x432b, 0x47f6, 0x4cee
	.2byte 0x5217, 0x5772, 0x5d00, 0x62c3, 0x68bc, 0x6eed, 0x7559, 0x7c00
@ A copy of The Broken Seal's gColorBuffer_ChannelCurveLight
	.global Data_080f0286
Data_080f0286:
	.2byte 0x0000, 0x0265, 0x04df, 0x076e, 0x0a14, 0x0cd1, 0x0fa5, 0x1291
	.2byte 0x1596, 0x18b4, 0x1bec, 0x1f3f, 0x22ac, 0x2636, 0x29dc, 0x2da0
	.2byte 0x3182, 0x3583, 0x39a3, 0x3de3, 0x4245, 0x46c9, 0x4b70, 0x503a
	.2byte 0x5529, 0x5a3d, 0x5f78, 0x64da, 0x6a65, 0x7019, 0x75f7, 0x7c00
@ A copy of The Broken Seal's gColorBuffer_ChannelCurveContrast
	.global Data_080f02c6
Data_080f02c6:
	.2byte 0x0000, 0x0168, 0x02f7, 0x04b1, 0x0699, 0x08b2, 0x0b01, 0x0d8a
	.2byte 0x1052, 0x135d, 0x16b0, 0x1a52, 0x1e48, 0x2298, 0x274a, 0x2c65
	.2byte 0x31f0, 0x37f5, 0x3e7c, 0x458e, 0x4d37, 0x5580, 0x5e77, 0x6826
	.2byte 0x729c, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00
	.section .rodata.b
@ A copy of The Broken Seal's gBattleEffect_CyclePatternWords
	.global Data_080f0890
Data_080f0890:
	.2byte 0xffff, 0x0000, 0x8000, 0xffff, 0xc000, 0xffff, 0xffff, 0xc000
	.2byte 0x4000, 0xffff, 0xffff, 0x4000, 0xffff, 0x0000, 0x8000, 0xffff
@ A copy of The Broken Seal's gDisplayScroll_WaveSine
	.global gLineScroll_WaveSine
gLineScroll_WaveSine:
	.2byte 0, 100, 200, 301, 401, 501, 601, 700
	.2byte 799, 897, 995, 1092, 1189, 1284, 1379, 1474
	.2byte 1567, 1659, 1751, 1841, 1930, 2018, 2105, 2191
	.2byte 2275, 2358, 2439, 2519, 2598, 2675, 2750, 2824
	.2byte 2896, 2966, 3034, 3101, 3166, 3229, 3289, 3348
	.2byte 3405, 3460, 3513, 3563, 3612, 3658, 3702, 3744
	.2byte 3784, 3821, 3856, 3889, 3919, 3947, 3973, 3996
	.2byte 4017, 4035, 4051, 4065, 4076, 4084, 4091, 4094
	.2byte 4096, 4094, 4091, 4084, 4076, 4065, 4051, 4035
	.2byte 4017, 3996, 3973, 3947, 3919, 3889, 3856, 3821
	.2byte 3784, 3744, 3702, 3658, 3612, 3563, 3513, 3460
	.2byte 3405, 3348, 3289, 3229, 3166, 3101, 3034, 2966
	.2byte 2896, 2824, 2750, 2675, 2598, 2519, 2439, 2358
	.2byte 2275, 2191, 2105, 2018, 1930, 1841, 1751, 1659
	.2byte 1567, 1474, 1379, 1284, 1189, 1092, 995, 897
	.2byte 799, 700, 601, 501, 401, 301, 200, 100
	.2byte 0, -100, -200, -301, -401, -501, -601, -700
	.2byte -799, -897, -995, -1092, -1189, -1284, -1379, -1474
	.2byte -1567, -1659, -1751, -1841, -1930, -2018, -2105, -2191
	.2byte -2275, -2358, -2439, -2519, -2598, -2675, -2750, -2824
	.2byte -2896, -2966, -3034, -3101, -3166, -3229, -3289, -3348
	.2byte -3405, -3460, -3513, -3563, -3612, -3658, -3702, -3744
	.2byte -3784, -3821, -3856, -3889, -3919, -3947, -3973, -3996
	.2byte -4017, -4035, -4051, -4065, -4076, -4084, -4091, -4094
	.2byte -4096, -4094, -4091, -4084, -4076, -4065, -4051, -4035
	.2byte -4017, -3996, -3973, -3947, -3919, -3889, -3856, -3821
	.2byte -3784, -3744, -3702, -3658, -3612, -3563, -3513, -3460
	.2byte -3405, -3348, -3289, -3229, -3166, -3101, -3034, -2966
	.2byte -2896, -2824, -2750, -2675, -2598, -2519, -2439, -2358
	.2byte -2275, -2191, -2105, -2018, -1930, -1841, -1751, -1659
	.2byte -1567, -1474, -1379, -1284, -1189, -1092, -995, -897
	.2byte -799, -700, -601, -501, -401, -301, -200, -100
@ A copy of The Broken Seal's gFieldEffect_GroundParticleFrames
	.global gFieldEffect_GroundParticleFrames
gFieldEffect_GroundParticleFrames:
	.2byte -8, -12, 20, 0, 1
	.2byte -8, -12, 20, 0, 1
	.2byte -8, -12, 20, 0, 1
	.2byte -8, -12, 16, 0, 1
	.2byte -8, -12, 16, 0, 1
	.2byte -8, -12, 12, 0, 1
	.2byte -8, -12, 8, 0, 1
	.2byte 0, -32, 0, 2, 2
	.2byte 8, -48, 0, 2, 2
	.2byte 16, -64, 0, 2, 2
	.2byte 24, -80, 0, 2, 2
	.2byte 32, -96, 0, 2, 2
	.2byte 40, -112, 0, 2, 2
	.2byte 48, -128, 0, 2, 2
	.2byte 56, -144, 0, 2, 2
	.2byte 64, -160, 0, 2, 2
@ {x, y} screen offsets of a ground particle, in pixels, indexed by its
@ remaining frame count; read by Weather_UpdateRain (The Broken Seal's
@ FieldEffect_UpdateGroundParticles), which has no such table.
	.global Data_080f0b50
Data_080f0b50:
	.2byte -32, -16
	.2byte -48, -23
	.2byte -64, -34
	.2byte -80, -43
	.2byte -96, -52
	.2byte -112, -61
	.2byte -128, -70
	.2byte -144, -77
	.2byte -160, -84
	.2byte -176, -91
	.2byte -192, -98
@ A copy of The Broken Seal's gFieldEffect_SparklePictures
	.global gFieldEffect_SparklePictures
gFieldEffect_SparklePictures:
	.2byte -2, 12
	.2byte -4, 12
	.2byte -6, 12
	.2byte -8, 8
	.2byte -10, 8
	.2byte -12, 8
	.2byte -14, 8
	.2byte -16, 8
	.2byte -19, 4
	.2byte -21, 4
	.2byte -24, 4
	.2byte -27, 4
	.2byte -30, 4
	.2byte -33, 4
	.2byte -36, 4
	.2byte -40, 4
	.2byte -44, 4
	.2byte -48, 0
	.2byte -52, 0
	.2byte -56, 0
	.2byte -60, 0
	.2byte -64, 0
	.2byte -68, 0
	.2byte -72, 0
	.2byte -76, 0
	.2byte -80, 0
	.2byte -85, 0
	.2byte -90, 0
	.2byte -95, 0
	.2byte -100, 0
	.2byte -110, 0
	.2byte -120, 0
@ The pulse scales, the rope, vine and chain sprites and the particle
@ scripts after gFieldEffect_SparklePictures, in the order of The Broken Seal's
@ field_sequence_tables.c, which has no sprites here. Object scripts are words in
@ this game's command numbers: 0x00 wait, 0x01 sync, 0x06 face the link,
@ 0x0d loop, 0x16 set, 0x26 free, 0x27 argument.

@ The Broken Seal's gBattleEffect_PulseScales without its unread third word:
@ two scales in 16.16 fixed point that alternate every four frames
	.global gData_080f0bfc
gData_080f0bfc:
	.4byte 0x10000, 0xf851

@ A rope tile Rope_Initialize and Tightrope_Initialize unpack into VRAM
	.global gRope_Tiles
gRope_Tiles:
	.incbin "games/tla/src/field/event_commands/rope.8bpp.lz"

@ Func_080da9a8 unpacks four 16x16 sprites, top first, into one VRAM block:
@ the vine's when its argument is 372, else the chain's, whose middle
@ sprite it unpacks twice
	.global Data_080f0c1c
Data_080f0c1c:
	.incbin "games/tla/src/field/event_commands/vine_0.8bpp16x16.lz"
	.global Data_080f0c63
Data_080f0c63:
	.incbin "games/tla/src/field/event_commands/vine_1.8bpp16x16.lz"
	.global Data_080f0cb8
Data_080f0cb8:
	.incbin "games/tla/src/field/event_commands/vine_2.8bpp16x16.lz"
	.global Data_080f0d07
Data_080f0d07:
	.incbin "games/tla/src/field/event_commands/vine_3.8bpp16x16.lz"
	.global Data_080f0d5e
Data_080f0d5e:
	.incbin "games/tla/src/field/event_commands/chain_0.8bpp16x16.lz"
	.global Data_080f0dbb
Data_080f0dbb:
	.incbin "games/tla/src/field/event_commands/chain_1.8bpp16x16.lz"
	.global Data_080f0e00
Data_080f0e00:
	.incbin "games/tla/src/field/event_commands/chain_2.8bpp16x16.lz"
	.balign 4

@ One word, 0x11, which no code names
	.4byte 0x11

@ The Broken Seal's gBattleEffect_SharedParticleScript: free; and from its
@ second word, sync and free
	.global gBattleEffect_SharedParticleScript
gBattleEffect_SharedParticleScript:
	.4byte 0x26
	.global gData_080f0e58
gData_080f0e58:
	.4byte 0x01
	.4byte 0x26

@ The Broken Seal's gBattleEffect_SourceHoldScript: face the link, wait a frame,
@ loop for ever to the top
	.global gFieldEffect_SourceHoldScript
gFieldEffect_SourceHoldScript:
	.4byte 0x06
	.4byte 0x00, 1
	.4byte 0x0d, 0xffff, 0

@ The Broken Seal's gBattleEffect_FragmentScript: sync, set the speed (work
@ 0x0f) to 0x1000, argument 0, wait 10 frames, free
	.global gFieldEffect_FragmentScript
gFieldEffect_FragmentScript:
	.4byte 0x01
	.4byte 0x16, 0x0f, 0x1000
	.4byte 0x27, 0
	.4byte 0x00, 10
	.4byte 0x26

@ A copy of The Broken Seal's gAnim_ChildPaletteCycle
	.global gData_080f0e9c
gData_080f0e9c:
	.byte 1, 2, 3, 4, 3, 2, 1, 0
@ A copy of The Broken Seal's gBattleEffect_CyclePatternWords
	.global gBattleEffect_CyclePatternWords
gBattleEffect_CyclePatternWords:
	.2byte 0xffff, 0x0000, 0x8000, 0xffff, 0xc000, 0xffff, 0xffff, 0xc000
	.2byte 0x4000, 0xffff, 0xffff, 0x4000, 0xffff, 0x0000, 0x8000, 0xffff
	.section .rodata.c
@ An 8x8 dot of 256 colours, shaded from its rim (colour 0xe9) to its centre
@ (0xef), that FieldPsynergy_Burst loads into VRAM for its 32 particles. Its
@ callback Func_080e4d9c writes OBJ colours 0xe0 to 0xff, computed from the
@ sine table, each frame; the picture carries the shared OBJ palette and
@ those colours of the first frame.
	.global Data_080f3984
Data_080f3984:
	.incbin "games/tla/src/field/event_commands/glow_spark.8bpp"
@ 256 tile index permutations.
	.global gData_080f39c4
gData_080f39c4:
	.2byte 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007
	.2byte 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047
	.2byte 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f
	.2byte 0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f
	.2byte 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017
	.2byte 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057
	.2byte 0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f
	.2byte 0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f
	.2byte 0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027
	.2byte 0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067
	.2byte 0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f
	.2byte 0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f
	.2byte 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037
	.2byte 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077
	.2byte 0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f
	.2byte 0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x007f
	.2byte 0x0c7f, 0x0c7e, 0x0c7d, 0x0c7c, 0x0c7b, 0x0c7a, 0x0c79, 0x0c78
	.2byte 0x0c3f, 0x0c3e, 0x0c3d, 0x0c3c, 0x0c3b, 0x0c3a, 0x0c39, 0x0c38
	.2byte 0x0c77, 0x0c76, 0x0c75, 0x0c74, 0x0c73, 0x0c72, 0x0c71, 0x0c70
	.2byte 0x0c37, 0x0c36, 0x0c35, 0x0c34, 0x0c33, 0x0c32, 0x0c31, 0x0c30
	.2byte 0x0c6f, 0x0c6e, 0x0c6d, 0x0c6c, 0x0c6b, 0x0c6a, 0x0c69, 0x0c68
	.2byte 0x0c2f, 0x0c2e, 0x0c2d, 0x0c2c, 0x0c2b, 0x0c2a, 0x0c29, 0x0c28
	.2byte 0x0c67, 0x0c66, 0x0c65, 0x0c64, 0x0c63, 0x0c62, 0x0c61, 0x0c60
	.2byte 0x0c27, 0x0c26, 0x0c25, 0x0c24, 0x0c23, 0x0c22, 0x0c21, 0x0c20
	.2byte 0x0c5f, 0x0c5e, 0x0c5d, 0x0c5c, 0x0c5b, 0x0c5a, 0x0c59, 0x0c58
	.2byte 0x0c1f, 0x0c1e, 0x0c1d, 0x0c1c, 0x0c1b, 0x0c1a, 0x0c19, 0x0c18
	.2byte 0x0c57, 0x0c56, 0x0c55, 0x0c54, 0x0c53, 0x0c52, 0x0c51, 0x0c50
	.2byte 0x0c17, 0x0c16, 0x0c15, 0x0c14, 0x0c13, 0x0c12, 0x0c11, 0x0c10
	.2byte 0x0c4f, 0x0c4e, 0x0c4d, 0x0c4c, 0x0c4b, 0x0c4a, 0x0c49, 0x0c48
	.2byte 0x0c0f, 0x0c0e, 0x0c0d, 0x0c0c, 0x0c0b, 0x0c0a, 0x0c09, 0x0c08
	.2byte 0x0800, 0x0801, 0x0802, 0x0803, 0x0804, 0x0805, 0x0806, 0x0807
	.2byte 0x0840, 0x0841, 0x0842, 0x0843, 0x0844, 0x0845, 0x0846, 0x0847
@ 12 width/height pairs.
	.global gData_080f3bc4
gData_080f3bc4:
	.byte 8, 8
	.byte 16, 16
	.byte 32, 32
	.byte 64, 64
	.byte 16, 8
	.byte 32, 8
	.byte 32, 16
	.byte 64, 32
	.byte 8, 16
	.byte 8, 32
	.byte 16, 32
	.byte 32, 64
@ Color mappings for darkening by 1, 2, and 4 levels (0xe0-0xfe).
	.global gColorBuffer_FadeStep1Map
gColorBuffer_FadeStep1Map:
	.byte 0x00, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee
	.byte 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe
	.global gColorBuffer_FadeStep2Map
gColorBuffer_FadeStep2Map:
	.byte 0x00, 0x00, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed
	.byte 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd
	.global gColorBuffer_FadeStep3Map
gColorBuffer_FadeStep3Map:
	.byte 0x00, 0x00, 0x00, 0x00, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb
	.byte 0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb
@ 41 four-word rows (consumer still unknown).
	.4byte 2, 2, 10, -1
	.4byte 2, 4, 21, -1
	.4byte 2, 5, 23, -1
	.4byte 2, 6, 34, -1
	.4byte 2, 7, 34, -1
	.4byte 2, 8, 43, -1
	.4byte 2, 9, 44, -1
	.4byte 2, 12, 54, -1
	.4byte 2, 13, 54, -1
	.4byte 2, 14, 55, -1
	.4byte 2, 15, 59, -1
	.4byte 2, 16, 82, -1
	.4byte 2, 19, 67, -1
	.4byte 2, 20, 95, -1
	.4byte 2, 21, 102, -1
	.4byte 2, 22, 102, -1
	.4byte 2, 23, 105, -1
	.4byte 2, 25, 107, -1
	.4byte 2, 26, 107, -1
	.4byte 2, 27, 110, -1
	.4byte 2, 28, 131, -1
	.4byte 2, 29, 131, -1
	.4byte 2, 30, 131, -1
	.4byte 2, 31, 131, -1
	.4byte 2, 31, 131, -1
	.4byte 2, 33, 139, -1
	.4byte 2, 35, 143, -1
	.4byte 2, 36, 154, -1
	.4byte 2, 37, 158, -1
	.4byte 2, 38, 170, -1
	.4byte 2, 39, 174, -1
	.4byte 2, 40, 182, -1
	.4byte 2, 41, 189, -1
	.4byte 2, 45, 199, -1
	.4byte 2, 46, 206, -1
	.4byte 2, 47, 139, -1
	.4byte 2, 48, 139, -1
	.4byte 2, 49, 139, -1
	.4byte 2, 50, 139, -1
	.4byte 2, 51, 139, -1
	.4byte 2, 19, 95, -1
