	.section .rodata
@ 調べた床の文章番号の差
	.global Field_TileMessageOffsets
Field_TileMessageOffsets:
	.byte 2, 3, 13, 8, 30, 31
@ {方式, 範囲 (画素)}, -1 で終わり
	.global BattleFx_TargetRangeByMode
BattleFx_TargetRangeByMode:
	.2byte 3, 16
	.2byte 6, 16
	.2byte 7, 16
	.2byte 11, 16
	.2byte 12, 16
	.2byte 2, 32
	.2byte 1, 32
	.2byte 14, 32
	.2byte 5, 32
	.2byte 9, 32
	.2byte 4, 48
	.2byte 13, 48
	.2byte -1
	.global Animation_ChildPaletteCycle
Animation_ChildPaletteCycle:
	.byte 1, 2, 3, 4, 3, 2, 1, 0
	.balign 4
	.global BattleFx_ParticleEmitterScript
BattleFx_ParticleEmitterScript:
	.4byte 21, 9, 32768, 21, 10, 32768, 0, 2
	.4byte 21, 9, 49152, 21, 10, 49152, 0, 2
	.4byte 21, 9, 57344, 21, 10, 57344, 0, 2
	.4byte 21, 9, 61440, 21, 10, 61440, 0, 2
	.4byte 21, 9, 65536, 21, 10, 65536, 16
	.global BattleFx_RisingPulseScript
BattleFx_RisingPulseScript:
	.4byte 0, 10, 21, 9, 90112, 21, 10, 90112
	.4byte 0, 2, 21, 9, 98304, 21, 10, 98304
	.4byte 0, 2, 21, 9, 73728, 21, 10, 73728
	.4byte 0, 2, 21, 9, 65536, 21, 10, 65536
	.4byte 0, 2, 21, 9, 90112, 21, 10, 90112
	.4byte 0, 2, 21, 9, 98304, 21, 10, 98304
	.4byte 0, 2, 21, 9, 73728, 21, 10, 73728
	.4byte 0, 2, 21, 9, 65536, 21, 10, 65536
	.4byte 0, 2, 0, 50, 12, 65535, 0, 16
	.global BattleFx_ParticleScript
BattleFx_ParticleScript:
	.4byte 22, 6, 0xffffc000, 0, 1, 12, 60, 0
	.4byte 27
	.global BattleFx_MarkerParticleScript
BattleFx_MarkerParticleScript:
	.4byte 34, EffectRuntime_IsActive, 27
	.global DisplayTransition_NoiseWidths
DisplayTransition_NoiseWidths:
	.byte 0, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 7, 8, 9, 11, 13
	.byte 16, 18, 22, 26, 30, 36, 43, 51, 60, 72, 85, 101, 120, 143, 170, 202, 240
	.global DisplayTransition_EdgeOffsets
DisplayTransition_EdgeOffsets:
	.byte -4, -3, -3, -3, -2, -2, -1, 0, 0, 0, 1, 2, 2, 3, 3, 3
	.byte 4, 3, 3, 3, 2, 2, 1, 0, 0, 0, -1, -2, -2, -3, -3, -3
	.global DisplayTransition_DitherTable
DisplayTransition_DitherTable:
	.byte 0, 12, 35, 3, 29, 39, 6, 7, 50, 59, 4, 11, 18, 40, 46, 31
	.byte 16, 42, 47, 51, 20, 23, 43, 32, 8, 52, 17, 41, 54, 15, 63, 24
	.byte 58, 5, 2, 26, 36, 61, 9, 34, 56, 27, 10, 48, 53, 30, 28, 22
	.byte 62, 45, 19, 60, 57, 49, 25, 55, 38, 14, 44, 13, 33, 37, 1, 21
	.balign 2
@ 色成分の曲線
	.global BattlePalette_ChannelCurve
BattlePalette_ChannelCurve:
	.2byte 0x0000, 0x020c, 0x042f, 0x066b, 0x08c0, 0x0b2f, 0x0db9, 0x105f
	.2byte 0x1322, 0x1603, 0x1903, 0x1c22, 0x1f63, 0x22c5, 0x264b, 0x29f4
	.2byte 0x2dc3, 0x31b9, 0x35d7, 0x3a1d, 0x3e8e, 0x432b, 0x47f6, 0x4cee
	.2byte 0x5217, 0x5772, 0x5d00, 0x62c3, 0x68bc, 0x6eed, 0x7559, 0x7c00
	.global BattlePalette_ChannelCurveLight
BattlePalette_ChannelCurveLight:
	.2byte 0x0000, 0x0265, 0x04df, 0x076e, 0x0a14, 0x0cd1, 0x0fa5, 0x1291
	.2byte 0x1596, 0x18b4, 0x1bec, 0x1f3f, 0x22ac, 0x2636, 0x29dc, 0x2da0
	.2byte 0x3182, 0x3583, 0x39a3, 0x3de3, 0x4245, 0x46c9, 0x4b70, 0x503a
	.2byte 0x5529, 0x5a3d, 0x5f78, 0x64da, 0x6a65, 0x7019, 0x75f7, 0x7c00
	.global BattlePalette_ChannelCurveContrast
BattlePalette_ChannelCurveContrast:
	.2byte 0x0000, 0x0168, 0x02f7, 0x04b1, 0x0699, 0x08b2, 0x0b01, 0x0d8a
	.2byte 0x1052, 0x135d, 0x16b0, 0x1a52, 0x1e48, 0x2298, 0x274a, 0x2c65
	.2byte 0x31f0, 0x37f5, 0x3e7c, 0x458e, 0x4d37, 0x5580, 0x5e77, 0x6826
	.2byte 0x729c, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00, 0x7c00
	.balign 4
@ {番号, 値, フラグ}
	.global BattleFx_DefinitionTable
BattleFx_DefinitionTable:
	.2byte 0
	.byte 16, 0x00
	.2byte 1
	.byte 16, 0x00
	.2byte 2
	.byte 16, 0x00
	.2byte 3
	.byte 12, 0x00
	.2byte 4
	.byte 28, 0x00
	.2byte 5
	.byte 12, 0x00
	.2byte 6
	.byte 12, 0x00
	.2byte 18
	.byte 8, 0x00
	.2byte 19
	.byte 8, 0x00
	.2byte 30
	.byte 24, 0x00
	.2byte 31
	.byte 20, 0x00
	.2byte 32
	.byte 28, 0x00
	.2byte 33
	.byte 20, 0x00
	.2byte 34
	.byte 28, 0x00
	.2byte 35
	.byte 28, 0x00
	.2byte 36
	.byte 16, 0x00
	.2byte 37
	.byte 8, 0x00
	.2byte 38
	.byte 20, 0x00
	.2byte 39
	.byte 12, 0x00
	.2byte 40
	.byte 28, 0x00
	.2byte 41
	.byte 16, 0x00
	.2byte 42
	.byte 12, 0x00
	.2byte 43
	.byte 28, 0x00
	.2byte 44
	.byte 28, 0x00
	.2byte 45
	.byte 20, 0x00
	.2byte 46
	.byte 28, 0x00
	.2byte 47
	.byte 28, 0x00
	.2byte 48
	.byte 4, 0x00
	.2byte 49
	.byte 24, 0x00
	.2byte 50
	.byte 28, 0x00
	.2byte 51
	.byte 24, 0x00
	.2byte 52
	.byte 28, 0x00
	.2byte 53
	.byte 28, 0x00
	.2byte 54
	.byte 28, 0x00
	.2byte 55
	.byte 24, 0x00
	.2byte 71
	.byte 28, 0x00
	.2byte 56
	.byte 20, 0x00
	.2byte 57
	.byte 28, 0x00
	.2byte 58
	.byte 28, 0x00
	.2byte 59
	.byte 28, 0x00
	.2byte 60
	.byte 20, 0x00
	.2byte 61
	.byte 20, 0x00
	.2byte 62
	.byte 28, 0x00
	.2byte 63
	.byte 28, 0x00
	.2byte 64
	.byte 28, 0x00
	.2byte 65
	.byte 24, 0x00
	.2byte 66
	.byte 16, 0x00
	.2byte 67
	.byte 28, 0x00
	.2byte 68
	.byte 24, 0x00
	.2byte 69
	.byte 28, 0x00
	.2byte 93
	.byte 16, 0x00
	.2byte 90
	.byte 16, 0x00
	.2byte 92
	.byte 16, 0x00
	.2byte 91
	.byte 16, 0x00
	.2byte 101
	.byte 28, 0x00
	.2byte 102
	.byte 4, 0x00
	.2byte 103
	.byte 24, 0x00
	.2byte 104
	.byte 0, 0x00
	.2byte 106
	.byte 12, 0x00
	.2byte 107
	.byte 8, 0x00
	.2byte 108
	.byte 16, 0x00
	.2byte 109
	.byte 28, 0x00
	.2byte 111
	.byte 20, 0x00
	.2byte 114
	.byte 28, 0x00
	.2byte 115
	.byte 20, 0x00
	.2byte 116
	.byte 28, 0x00
	.2byte 117
	.byte 24, 0x00
	.2byte 118
	.byte 12, 0x00
	.2byte 119
	.byte 28, 0x00
	.2byte 120
	.byte 20, 0x00
	.2byte 121
	.byte 24, 0x00
	.2byte 122
	.byte 28, 0x00
	.2byte 123
	.byte 28, 0x00
	.2byte 124
	.byte 28, 0x00
	.2byte 125
	.byte 28, 0x00
	.2byte 128
	.byte 20, 0x00
	.2byte 129
	.byte 16, 0x00
	.2byte 130
	.byte 16, 0x00
	.2byte 131
	.byte 12, 0x00
	.2byte 132
	.byte 28, 0x00
	.2byte 133
	.byte 16, 0x00
	.2byte 134
	.byte 28, 0x00
	.2byte 135
	.byte 28, 0x00
	.2byte 136
	.byte 0, 0x00
	.2byte 144
	.byte 28, 0x00
	.2byte 145
	.byte 28, 0x00
	.2byte 146
	.byte 28, 0x00
	.2byte 147
	.byte 28, 0x00
	.2byte 148
	.byte 12, 0x00
	.2byte 149
	.byte 16, 0x00
	.2byte 150
	.byte 28, 0x00
	.2byte 151
	.byte 8, 0x00
	.2byte 152
	.byte 28, 0x00
	.2byte 153
	.byte 28, 0x00
	.2byte 155
	.byte 0, 0x00
	.2byte 156
	.byte 16, 0x00
	.2byte 157
	.byte 28, 0x00
	.2byte 158
	.byte 28, 0x00
	.2byte 159
	.byte 16, 0x00
	.2byte 160
	.byte 28, 0x00
	.2byte 162
	.byte 20, 0x00
	.2byte 163
	.byte 28, 0x00
	.2byte 164
	.byte 24, 0x00
	.2byte 165
	.byte 16, 0x00
	.2byte 166
	.byte 20, 0x00
	.2byte 167
	.byte 24, 0x00
	.2byte 168
	.byte 8, 0x00
	.2byte 169
	.byte 4, 0x00
	.2byte 170
	.byte 28, 0x00
	.2byte 171
	.byte 28, 0x00
	.2byte 187
	.byte 28, 0xff
	.2byte 189
	.byte 20, 0xff
	.2byte 190
	.byte 28, 0xff
	.2byte 191
	.byte 28, 0xff
	.2byte 192
	.byte 16, 0xff
	.2byte 193
	.byte 16, 0xff
	.2byte 194
	.byte 24, 0xff
	.2byte 195
	.byte 28, 0xff
	.2byte 196
	.byte 0, 0xff
	.2byte 197
	.byte 12, 0xff
	.2byte 198
	.byte 28, 0xff
	.2byte 199
	.byte 24, 0xff
	.2byte 20
	.byte 255, 0xff
	.2byte 200
	.byte 255, 0xff
	.2byte 105
	.byte 255, 0xff
	.2byte 110
	.byte 255, 0xff
	.2byte 274
	.byte 255, 0xff
	.2byte 231
	.byte 255, 0xff
	.2byte 293
	.byte 255, 0xff
	.2byte 22
	.byte 255, 0xff
	.2byte 0
	.byte 16, 0x00
	.global ObjectMotion_VariantScripts
ObjectMotion_VariantScripts:
	.4byte 22, 9, 0xffffc000, 22, 10, 8192, 0, 2
	.4byte 22, 9, 8192, 22, 10, 0xfffff000, 0, 2
	.4byte 22, 9, 4096, 22, 10, 0xfffff800, 0, 2
	.4byte 22, 9, 4096, 22, 10, 0xfffff800, 0, 4
	.4byte 22, 9, 0xffffc000, 22, 10, 8192, 0, 2
	.4byte 22, 9, 8192, 22, 10, 0xfffff000, 0, 2
	.4byte 22, 9, 4096, 22, 10, 0xfffff800, 0, 2
	.4byte 22, 9, 4096, 22, 10, 0xfffff800, 0, 4
	.4byte 22, 9, 0xffffc000, 22, 10, 8192, 0, 2
	.4byte 22, 9, 8192, 22, 10, 0xfffff000, 0, 2
	.4byte 22, 9, 4096, 22, 10, 0xfffff800, 0, 2
	.4byte 22, 9, 4096, 22, 10, 0xfffff800, 0, 4
	.4byte 16
	.global ObjectGroup_BlinkChildValues
ObjectGroup_BlinkChildValues:
	.byte 7, 0, 0, 0
@ 正弦 (4096 倍)
	.global DisplayScroll_WaveSine
DisplayScroll_WaveSine:
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
@ {dx, dy, タイル, 形, 大きさ}
	.global FieldFx_GroundParticleFrames
FieldFx_GroundParticleFrames:
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
@ {dy, タイル}
	.global FieldFx_SparkleFrames
FieldFx_SparkleFrames:
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
	.global BattleFx_PulseScales
BattleFx_PulseScales:
	.4byte 65536, 63569
	.4byte 16
	.global BattleFx_CommonParticleScript
BattleFx_CommonParticleScript:
	.4byte 27, 1, 27
	.global BattleFx_SourceHoldScript
BattleFx_SourceHoldScript:
	.4byte 5, 0, 1, 12, 65535, 0
	.global BattleFx_FragmentScript
BattleFx_FragmentScript:
	.4byte 1, 21, 15, 4096, 28, 0, 0, 10
	.4byte 27
	.global BattleFx_CyclePatternWords
BattleFx_CyclePatternWords:
	.2byte 0xffff, 0x0000, 0x8000, 0xffff, 0xc000, 0xffff, 0xffff, 0xc000
	.2byte 0x4000, 0xffff, 0xffff, 0x4000, 0xffff, 0x0000, 0x8000, 0xffff
	.global BattleFx_QuadrantModes
BattleFx_QuadrantModes:
	.byte 3, 1, 2, 1
	.global BattleFx_BurstParticleObjectScript
BattleFx_BurstParticleObjectScript:
	.4byte 0, 60, 27
	.byte 3, 1, 2, 1
	.global BattleFx_HeavyImpactScript
BattleFx_HeavyImpactScript:
	.4byte 1, 34, SpawnHeavyImpactEffect, 27
	.global BattleFx_ShortEffectScript
BattleFx_ShortEffectScript:
	.4byte 0, 24, 27
	.global BattleFx_MediumEffectScript
BattleFx_MediumEffectScript:
	.4byte 0, 46, 27
	.global BattleFx_LongEffectScript
BattleFx_LongEffectScript:
	.4byte 0, 128, 27
	.global BattleFx_RandomChildValues
BattleFx_RandomChildValues:
	.byte 0, 0, 1, 2, 3, 4, 5, 6
	.global WorldMap_MarkerBlendCycle
WorldMap_MarkerBlendCycle:
	.byte 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
	.byte 16, 15, 15, 14, 12, 10, 9, 9, 8, 9, 9, 10, 12, 14, 14, 15
	.global WorldMap_CursorDirectionAngles
WorldMap_CursorDirectionAngles:
	.2byte 0xffff, 0x0000, 0x8000, 0xffff, 0xc000, 0xe000, 0xa000, 0xc000
	.2byte 0x4000, 0x2000, 0x6000, 0x4000, 0xffff, 0x0000, 0x8000, 0xffff
