@ tla-it's scaffold: the base-ROM ranges its MAIN.LD places between the
@ lines it links from source, with a label where the source names a place.
	.section .rom.00000000, "a"
	.incbin "baserom.gba", 0x00000000, 0x00000428
	.global Entry
Entry:
	.incbin "baserom.gba", 0x00000428, 0x00000290
	.section .rom.000006b8, "a"
	.incbin "baserom.gba", 0x000006b8, 0x00000fa0
	.section .rom.00001658, "a"
	.incbin "baserom.gba", 0x00001658, 0x000009fc
	.global Math_Div
	.thumb_func
Math_Div:
	.incbin "baserom.gba", 0x00002054, 0x00000008
	.global Math_DivU
	.thumb_func
Math_DivU:
	.incbin "baserom.gba", 0x0000205c, 0x00000008
	.global Math_Mod
	.thumb_func
Math_Mod:
	.incbin "baserom.gba", 0x00002064, 0x00058680
	.section .rom.0005c2e4, "a"
	.incbin "baserom.gba", 0x0005c2e4, 0x0016502c
	.section .rom.001c1310, "a"
	.incbin "baserom.gba", 0x001c1310, 0x0000039e
	.global Mixer_CallViaR3
	.thumb_func
Mixer_CallViaR3:
	.incbin "baserom.gba", 0x001c16ae, 0x00000006
	.section .rom.001c16b4, "a"
	.incbin "baserom.gba", 0x001c16b4, 0x00002060
	.section .rom.001c43b0, "a"
	.incbin "baserom.gba", 0x001c43b0, 0x00000090
	.section .rom.001c44d0, "a"
	.incbin "baserom.gba", 0x001c44d0, 0x00000060
	.section .rom.002f9030, "a"
	.incbin "baserom.gba", 0x002f9030, 0x00006fd0
	.section .rom.006322b2, "a"
	.incbin "baserom.gba", 0x006322b2, 0x0004fd5e
	.section .rom.006848d0, "a"
	.incbin "baserom.gba", 0x006848d0, 0x000000c0
	.section .rom.0068a153, "a"
	.incbin "baserom.gba", 0x0068a153, 0x0000d895
	.section .rom.006a457d, "a"
	.incbin "baserom.gba", 0x006a457d, 0x00003f3f
	.section .rom.006a99e3, "a"
	.incbin "baserom.gba", 0x006a99e3, 0x00024769
	.section .rom.006d0b3a, "a"
	.incbin "baserom.gba", 0x006d0b3a, 0x00000002
	.section .rom.006d4f7e, "a"
	.incbin "baserom.gba", 0x006d4f7e, 0x00000002
	.section .rom.006e56ee, "a"
	.incbin "baserom.gba", 0x006e56ee, 0x00000002
	.section .rom.006e8cde, "a"
	.incbin "baserom.gba", 0x006e8cde, 0x00000002
	.section .rom.006f4782, "a"
	.incbin "baserom.gba", 0x006f4782, 0x00000002
	.section .rom.00704e32, "a"
	.incbin "baserom.gba", 0x00704e32, 0x00000002
	.section .rom.0070c8d2, "a"
	.incbin "baserom.gba", 0x0070c8d2, 0x00000002
	.section .rom.00710112, "a"
	.incbin "baserom.gba", 0x00710112, 0x00000002
	.section .rom.00719546, "a"
	.incbin "baserom.gba", 0x00719546, 0x00000002
	.section .rom.007206f2, "a"
	.incbin "baserom.gba", 0x007206f2, 0x00000002
	.section .rom.007285ae, "a"
	.incbin "baserom.gba", 0x007285ae, 0x00000002
	.section .rom.0072d03e, "a"
	.incbin "baserom.gba", 0x0072d03e, 0x00000002
	.section .rom.007312f2, "a"
	.incbin "baserom.gba", 0x007312f2, 0x00000002
	.section .rom.00734c72, "a"
	.incbin "baserom.gba", 0x00734c72, 0x00000002
	.section .rom.007414ae, "a"
	.incbin "baserom.gba", 0x007414ae, 0x00000002
	.section .rom.0074cbee, "a"
	.incbin "baserom.gba", 0x0074cbee, 0x00000002
	.section .rom.0074ffbe, "a"
	.incbin "baserom.gba", 0x0074ffbe, 0x00000002
	.section .rom.00761eda, "a"
	.incbin "baserom.gba", 0x00761eda, 0x00000002
	.section .rom.00765942, "a"
	.incbin "baserom.gba", 0x00765942, 0x00000002
	.section .rom.00769332, "a"
	.incbin "baserom.gba", 0x00769332, 0x00000002
	.section .rom.0077989a, "a"
	.incbin "baserom.gba", 0x0077989a, 0x00000002
	.section .rom.0077d7ca, "a"
	.incbin "baserom.gba", 0x0077d7ca, 0x00000002
	.section .rom.007811ee, "a"
	.incbin "baserom.gba", 0x007811ee, 0x00000002
	.section .rom.0078950e, "a"
	.incbin "baserom.gba", 0x0078950e, 0x00000002
	.section .rom.0079167a, "a"
	.incbin "baserom.gba", 0x0079167a, 0x00000002
	.section .rom.0079e946, "a"
	.incbin "baserom.gba", 0x0079e946, 0x00000002
	.section .rom.007a2802, "a"
	.incbin "baserom.gba", 0x007a2802, 0x00000002
	.section .rom.007a65fe, "a"
	.incbin "baserom.gba", 0x007a65fe, 0x00000002
	.section .rom.007aaace, "a"
	.incbin "baserom.gba", 0x007aaace, 0x00000002
	.section .rom.007b24aa, "a"
	.incbin "baserom.gba", 0x007b24aa, 0x00000002
	.section .rom.007b6ad6, "a"
	.incbin "baserom.gba", 0x007b6ad6, 0x00000002
	.section .rom.007be752, "a"
	.incbin "baserom.gba", 0x007be752, 0x00000002
	.section .rom.007c234e, "a"
	.incbin "baserom.gba", 0x007c234e, 0x00000002
	.section .rom.007c5cfe, "a"
	.incbin "baserom.gba", 0x007c5cfe, 0x00000002
	.section .rom.007ca14e, "a"
	.incbin "baserom.gba", 0x007ca14e, 0x00000002
	.section .rom.007cdd46, "a"
	.incbin "baserom.gba", 0x007cdd46, 0x00000002
	.section .rom.007d99a6, "a"
	.incbin "baserom.gba", 0x007d99a6, 0x00000002
	.section .rom.007dd5f6, "a"
	.incbin "baserom.gba", 0x007dd5f6, 0x00000002
	.section .rom.007e5b76, "a"
	.incbin "baserom.gba", 0x007e5b76, 0x00000002
	.section .rom.007f21c6, "a"
	.incbin "baserom.gba", 0x007f21c6, 0x00000002
	.section .rom.007f8c7a, "a"
	.incbin "baserom.gba", 0x007f8c7a, 0x00000002
	.section .rom.00801f42, "a"
	.incbin "baserom.gba", 0x00801f42, 0x00000002
	.section .rom.00809966, "a"
	.incbin "baserom.gba", 0x00809966, 0x00000002
	.section .rom.0081f5e6, "a"
	.incbin "baserom.gba", 0x0081f5e6, 0x00000002
	.section .rom.00828a2a, "a"
	.incbin "baserom.gba", 0x00828a2a, 0x00000002
	.section .rom.00831c46, "a"
	.incbin "baserom.gba", 0x00831c46, 0x00000002
	.section .rom.0083f56e, "a"
	.incbin "baserom.gba", 0x0083f56e, 0x00000002
	.section .rom.008453c2, "a"
	.incbin "baserom.gba", 0x008453c2, 0x00000002
	.section .rom.0084b106, "a"
	.incbin "baserom.gba", 0x0084b106, 0x00000002
	.section .rom.0084b889, "a"
	.incbin "baserom.gba", 0x0084b889, 0x00000003
	.section .rom.0084d1db, "a"
	.incbin "baserom.gba", 0x0084d1db, 0x00000001
	.section .rom.0084fff9, "a"
	.incbin "baserom.gba", 0x0084fff9, 0x00000003
	.section .rom.00853f0e, "a"
	.incbin "baserom.gba", 0x00853f0e, 0x00000002
	.section .rom.00856e14, "a"
	.incbin "baserom.gba", 0x00856e14, 0x000009bc
	.section .rom.00857d52, "a"
	.incbin "baserom.gba", 0x00857d52, 0x00000002
	.section .rom.00858229, "a"
	.incbin "baserom.gba", 0x00858229, 0x00000003
	.section .rom.00858773, "a"
	.incbin "baserom.gba", 0x00858773, 0x00000001
	.section .rom.00858c03, "a"
	.incbin "baserom.gba", 0x00858c03, 0x00000001
	.section .rom.008590ae, "a"
	.incbin "baserom.gba", 0x008590ae, 0x00000002
	.section .rom.0085941d, "a"
	.incbin "baserom.gba", 0x0085941d, 0x00000003
	.section .rom.00859fdb, "a"
	.incbin "baserom.gba", 0x00859fdb, 0x00000001
	.section .rom.0085a09b, "a"
	.incbin "baserom.gba", 0x0085a09b, 0x00000001
	.section .rom.0085ac2f, "a"
	.incbin "baserom.gba", 0x0085ac2f, 0x00000001
	.section .rom.0085afc3, "a"
	.incbin "baserom.gba", 0x0085afc3, 0x00000001
	.section .rom.0085b611, "a"
	.incbin "baserom.gba", 0x0085b611, 0x00000003
	.section .rom.0085b853, "a"
	.incbin "baserom.gba", 0x0085b853, 0x00000001
	.section .rom.0085def3, "a"
	.incbin "baserom.gba", 0x0085def3, 0x00000001
	.section .rom.0085e66e, "a"
	.incbin "baserom.gba", 0x0085e66e, 0x00000002
	.section .rom.0085fcb3, "a"
	.incbin "baserom.gba", 0x0085fcb3, 0x00000001
	.section .rom.008601dd, "a"
	.incbin "baserom.gba", 0x008601dd, 0x00000003
	.section .rom.008609e6, "a"
	.incbin "baserom.gba", 0x008609e6, 0x00000002
	.section .rom.00860f5f, "a"
	.incbin "baserom.gba", 0x00860f5f, 0x00000001
	.section .rom.0086269e, "a"
	.incbin "baserom.gba", 0x0086269e, 0x00000002
	.section .rom.00865567, "a"
	.incbin "baserom.gba", 0x00865567, 0x00000001
	.section .rom.00865819, "a"
	.incbin "baserom.gba", 0x00865819, 0x00000003
	.section .rom.00866bfb, "a"
	.incbin "baserom.gba", 0x00866bfb, 0x00000001
	.section .rom.0086b437, "a"
	.incbin "baserom.gba", 0x0086b437, 0x00000001
	.section .rom.0086ebdd, "a"
	.incbin "baserom.gba", 0x0086ebdd, 0x00000003
	.section .rom.00870902, "a"
	.incbin "baserom.gba", 0x00870902, 0x00000002
	.section .rom.00872801, "a"
	.incbin "baserom.gba", 0x00872801, 0x00000003
	.section .rom.008741af, "a"
	.incbin "baserom.gba", 0x008741af, 0x00000001
	.section .rom.00875727, "a"
	.incbin "baserom.gba", 0x00875727, 0x00000001
	.section .rom.0087935a, "a"
	.incbin "baserom.gba", 0x0087935a, 0x00000002
	.section .rom.00879eb8, "a"
	.incbin "baserom.gba", 0x00879eb8, 0x00003b38
	.section .rom.0087e027, "a"
	.incbin "baserom.gba", 0x0087e027, 0x00000001
	.section .rom.00880191, "a"
	.incbin "baserom.gba", 0x00880191, 0x00000003
	.section .rom.0088029c, "a"
	.incbin "baserom.gba", 0x0088029c, 0x000003d0
	.section .rom.00880f25, "a"
	.incbin "baserom.gba", 0x00880f25, 0x00000003
	.section .rom.00882b09, "a"
	.incbin "baserom.gba", 0x00882b09, 0x00000003
	.section .rom.00884666, "a"
	.incbin "baserom.gba", 0x00884666, 0x00000002
	.section .rom.00884aa5, "a"
	.incbin "baserom.gba", 0x00884aa5, 0x00000003
	.section .rom.00884eba, "a"
	.incbin "baserom.gba", 0x00884eba, 0x00001842
	.section .rom.00888c07, "a"
	.incbin "baserom.gba", 0x00888c07, 0x00000001
	.section .rom.008895c3, "a"
	.incbin "baserom.gba", 0x008895c3, 0x00000001
	.section .rom.0088a706, "a"
	.incbin "baserom.gba", 0x0088a706, 0x00001496
	.section .rom.0088d117, "a"
	.incbin "baserom.gba", 0x0088d117, 0x00000001
	.section .rom.0088d441, "a"
	.incbin "baserom.gba", 0x0088d441, 0x0000104b
	.section .rom.0088ec15, "a"
	.incbin "baserom.gba", 0x0088ec15, 0x00000623
	.section .rom.0088f861, "a"
	.incbin "baserom.gba", 0x0088f861, 0x00000003
	.section .rom.0088fc6b, "a"
	.incbin "baserom.gba", 0x0088fc6b, 0x000007d9
	.section .rom.008906ed, "a"
	.incbin "baserom.gba", 0x008906ed, 0x0000095f
	.section .rom.00891709, "a"
	.incbin "baserom.gba", 0x00891709, 0x00003c7f
	.section .rom.008959e7, "a"
	.incbin "baserom.gba", 0x008959e7, 0x00000001
	.section .rom.00896a21, "a"
	.incbin "baserom.gba", 0x00896a21, 0x00000003
	.section .rom.00897077, "a"
	.incbin "baserom.gba", 0x00897077, 0x00000001
	.section .rom.008976f5, "a"
	.incbin "baserom.gba", 0x008976f5, 0x00000003
	.section .rom.00897d15, "a"
	.incbin "baserom.gba", 0x00897d15, 0x00000003
	.section .rom.008990e2, "a"
	.incbin "baserom.gba", 0x008990e2, 0x00000002
	.section .rom.0089a126, "a"
	.incbin "baserom.gba", 0x0089a126, 0x00000002
	.section .rom.0089abaf, "a"
	.incbin "baserom.gba", 0x0089abaf, 0x000002cd
	.section .rom.0089bbb5, "a"
	.incbin "baserom.gba", 0x0089bbb5, 0x00000003
	.section .rom.0089cdf1, "a"
	.incbin "baserom.gba", 0x0089cdf1, 0x00000003
	.section .rom.0089d73b, "a"
	.incbin "baserom.gba", 0x0089d73b, 0x00006c09
	.section .rom.008a4878, "a"
	.incbin "baserom.gba", 0x008a4878, 0x000011d8
	.section .rom.008a5f3e, "a"
	.incbin "baserom.gba", 0x008a5f3e, 0x000027fe
	.section .rom.008a8d7f, "a"
	.incbin "baserom.gba", 0x008a8d7f, 0x000000a9
	.section .rom.008a913e, "a"
	.incbin "baserom.gba", 0x008a913e, 0x000018d2
	.section .rom.008ab681, "a"
	.incbin "baserom.gba", 0x008ab681, 0x00000003
	.section .rom.008abec9, "a"
	.incbin "baserom.gba", 0x008abec9, 0x00001203
	.section .rom.008adf8b, "a"
	.incbin "baserom.gba", 0x008adf8b, 0x00000001
	.section .rom.008aea67, "a"
	.incbin "baserom.gba", 0x008aea67, 0x00000001
	.section .rom.008af12a, "a"
	.incbin "baserom.gba", 0x008af12a, 0x00000002
	.section .rom.008af411, "a"
	.incbin "baserom.gba", 0x008af411, 0x00000003
	.section .rom.008b0777, "a"
	.incbin "baserom.gba", 0x008b0777, 0x00000001
	.section .rom.008b0b61, "a"
	.incbin "baserom.gba", 0x008b0b61, 0x00000003
	.section .rom.008b0f33, "a"
	.incbin "baserom.gba", 0x008b0f33, 0x00000001
	.section .rom.008b17d3, "a"
	.incbin "baserom.gba", 0x008b17d3, 0x00000001
	.section .rom.008b1c76, "a"
	.incbin "baserom.gba", 0x008b1c76, 0x00000002
	.section .rom.008b2e2f, "a"
	.incbin "baserom.gba", 0x008b2e2f, 0x00000001
	.section .rom.008b3288, "a"
	.incbin "baserom.gba", 0x008b3288, 0x00002034
	.section .rom.008b5516, "a"
	.incbin "baserom.gba", 0x008b5516, 0x00000002
	.section .rom.008b6f71, "a"
	.incbin "baserom.gba", 0x008b6f71, 0x000001b7
	.section .rom.008b74bd, "a"
	.incbin "baserom.gba", 0x008b74bd, 0x00000003
	.section .rom.008b9246, "a"
	.incbin "baserom.gba", 0x008b9246, 0x0000027a
	.section .rom.008b9986, "a"
	.incbin "baserom.gba", 0x008b9986, 0x00000002
	.section .rom.008bb55b, "a"
	.incbin "baserom.gba", 0x008bb55b, 0x00000001
	.section .rom.008bd159, "a"
	.incbin "baserom.gba", 0x008bd159, 0x00000003
	.section .rom.008bd37a, "a"
	.incbin "baserom.gba", 0x008bd37a, 0x0000043e
	.section .rom.008bd8c9, "a"
	.incbin "baserom.gba", 0x008bd8c9, 0x00000003
	.section .rom.008bdeab, "a"
	.incbin "baserom.gba", 0x008bdeab, 0x000004a1
	.section .rom.008bf05e, "a"
	.incbin "baserom.gba", 0x008bf05e, 0x00000002
	.section .rom.008bf5a0, "a"
	.incbin "baserom.gba", 0x008bf5a0, 0x00000840
	.section .rom.008c016f, "a"
	.incbin "baserom.gba", 0x008c016f, 0x00001259
	.section .rom.008c1eb6, "a"
	.incbin "baserom.gba", 0x008c1eb6, 0x00000002
	.section .rom.008c2c8d, "a"
	.incbin "baserom.gba", 0x008c2c8d, 0x00000003
	.section .rom.008c34ba, "a"
	.incbin "baserom.gba", 0x008c34ba, 0x00000002
	.section .rom.008c38ac, "a"
	.incbin "baserom.gba", 0x008c38ac, 0x000010d0
	.section .rom.008c52ed, "a"
	.incbin "baserom.gba", 0x008c52ed, 0x00000003
	.section .rom.008c569b, "a"
	.incbin "baserom.gba", 0x008c569b, 0x00000001
	.section .rom.008c763f, "a"
	.incbin "baserom.gba", 0x008c763f, 0x00000001
	.section .rom.008c83db, "a"
	.incbin "baserom.gba", 0x008c83db, 0x00000001
	.section .rom.008c85f7, "a"
	.incbin "baserom.gba", 0x008c85f7, 0x00000001
	.section .rom.008c88f3, "a"
	.incbin "baserom.gba", 0x008c88f3, 0x00000001
	.section .rom.008c8c94, "a"
	.incbin "baserom.gba", 0x008c8c94, 0x000016b0
	.section .rom.008caaa1, "a"
	.incbin "baserom.gba", 0x008caaa1, 0x00000003
	.section .rom.008cb86f, "a"
	.incbin "baserom.gba", 0x008cb86f, 0x00000c75
	.section .rom.008ccad9, "a"
	.incbin "baserom.gba", 0x008ccad9, 0x00000003
	.section .rom.008cd033, "a"
	.incbin "baserom.gba", 0x008cd033, 0x00000001
	.section .rom.008ce91d, "a"
	.incbin "baserom.gba", 0x008ce91d, 0x000008a3
	.section .rom.008cf59b, "a"
	.incbin "baserom.gba", 0x008cf59b, 0x00000001
	.section .rom.008cf825, "a"
	.incbin "baserom.gba", 0x008cf825, 0x00000003
	.section .rom.008cfbcb, "a"
	.incbin "baserom.gba", 0x008cfbcb, 0x00000001
	.section .rom.008cfe26, "a"
	.incbin "baserom.gba", 0x008cfe26, 0x00000002
	.section .rom.008d01de, "a"
	.incbin "baserom.gba", 0x008d01de, 0x00000002
	.section .rom.008d16c7, "a"
	.incbin "baserom.gba", 0x008d16c7, 0x00000001
	.section .rom.008d2c8a, "a"
	.incbin "baserom.gba", 0x008d2c8a, 0x00000a6e
	.section .rom.008d44ce, "a"
	.incbin "baserom.gba", 0x008d44ce, 0x00000002
	.section .rom.008d4851, "a"
	.incbin "baserom.gba", 0x008d4851, 0x00000003
	.section .rom.008d5501, "a"
	.incbin "baserom.gba", 0x008d5501, 0x00000003
	.section .rom.008d6049, "a"
	.incbin "baserom.gba", 0x008d6049, 0x00000a27
	.section .rom.008d763e, "a"
	.incbin "baserom.gba", 0x008d763e, 0x00000002
	.section .rom.008d7b56, "a"
	.incbin "baserom.gba", 0x008d7b56, 0x00000626
	.section .rom.008d859d, "a"
	.incbin "baserom.gba", 0x008d859d, 0x00000003
	.section .rom.008d8837, "a"
	.incbin "baserom.gba", 0x008d8837, 0x00000001
	.section .rom.008d9d38, "a"
	.incbin "baserom.gba", 0x008d9d38, 0x00001e28
	.section .rom.008dbf4a, "a"
	.incbin "baserom.gba", 0x008dbf4a, 0x00000002
	.section .rom.008ddebf, "a"
	.incbin "baserom.gba", 0x008ddebf, 0x00000001
	.section .rom.008de38f, "a"
	.incbin "baserom.gba", 0x008de38f, 0x00001cc5
	.section .rom.008e0829, "a"
	.incbin "baserom.gba", 0x008e0829, 0x00000003
	.section .rom.008e12fa, "a"
	.incbin "baserom.gba", 0x008e12fa, 0x00000002
	.section .rom.008e1e37, "a"
	.incbin "baserom.gba", 0x008e1e37, 0x00001c2d
	.section .rom.008e3dcf, "a"
	.incbin "baserom.gba", 0x008e3dcf, 0x00000001
	.section .rom.008e436d, "a"
	.incbin "baserom.gba", 0x008e436d, 0x00000207
	.section .rom.008e48ad, "a"
	.incbin "baserom.gba", 0x008e48ad, 0x00002687
	.section .rom.008e8ed2, "a"
	.incbin "baserom.gba", 0x008e8ed2, 0x00000002
	.section .rom.008e9675, "a"
	.incbin "baserom.gba", 0x008e9675, 0x00000003
	.section .rom.008ea68c, "a"
	.incbin "baserom.gba", 0x008ea68c, 0x00001878
	.section .rom.008ebf88, "a"
	.incbin "baserom.gba", 0x008ebf88, 0x000004e8
	.section .rom.008ec578, "a"
	.incbin "baserom.gba", 0x008ec578, 0x000006b8
	.section .rom.008ed542, "a"
	.incbin "baserom.gba", 0x008ed542, 0x00002b86
	.section .rom.008f116d, "a"
	.incbin "baserom.gba", 0x008f116d, 0x00000003
	.section .rom.008f136c, "a"
	.incbin "baserom.gba", 0x008f136c, 0x0004ac54
	.section .rom.0093c1ad, "a"
	.incbin "baserom.gba", 0x0093c1ad, 0x0000060f
	.section .rom.0093df26, "a"
	.incbin "baserom.gba", 0x0093df26, 0x00000002
	.section .rom.0093f45d, "a"
	.incbin "baserom.gba", 0x0093f45d, 0x00000003
	.section .rom.009412b3, "a"
	.incbin "baserom.gba", 0x009412b3, 0x00000001
	.section .rom.009433c7, "a"
	.incbin "baserom.gba", 0x009433c7, 0x00000001
	.section .rom.009435a0, "a"
	.incbin "baserom.gba", 0x009435a0, 0x000005a4
	.section .rom.009461bf, "a"
	.incbin "baserom.gba", 0x009461bf, 0x00000001
	.section .rom.00946bc7, "a"
	.incbin "baserom.gba", 0x00946bc7, 0x00000001
	.section .rom.009498e6, "a"
	.incbin "baserom.gba", 0x009498e6, 0x00000002
	.section .rom.00949ab8, "a"
	.incbin "baserom.gba", 0x00949ab8, 0x00000468
	.section .rom.0094b512, "a"
	.incbin "baserom.gba", 0x0094b512, 0x00000002
	.section .rom.0094c982, "a"
	.incbin "baserom.gba", 0x0094c982, 0x00000002
	.section .rom.0094d276, "a"
	.incbin "baserom.gba", 0x0094d276, 0x00000002
	.section .rom.0094db9d, "a"
	.incbin "baserom.gba", 0x0094db9d, 0x00000003
	.section .rom.0094ea8e, "a"
	.incbin "baserom.gba", 0x0094ea8e, 0x00000002
	.section .rom.0094f67b, "a"
	.incbin "baserom.gba", 0x0094f67b, 0x00000001
	.section .rom.0094f856, "a"
	.incbin "baserom.gba", 0x0094f856, 0x000004ca
	.section .rom.009505b6, "a"
	.incbin "baserom.gba", 0x009505b6, 0x00000002
	.section .rom.009529f1, "a"
	.incbin "baserom.gba", 0x009529f1, 0x00000003
	.section .rom.00952fc3, "a"
	.incbin "baserom.gba", 0x00952fc3, 0x00000001
	.section .rom.00953463, "a"
	.incbin "baserom.gba", 0x00953463, 0x00000001
	.section .rom.009537b6, "a"
	.incbin "baserom.gba", 0x009537b6, 0x000002fa
	.section .rom.00953b89, "a"
	.incbin "baserom.gba", 0x00953b89, 0x000002f3
	.section .rom.00953f0e, "a"
	.incbin "baserom.gba", 0x00953f0e, 0x00000002
	.section .rom.00954ac1, "a"
	.incbin "baserom.gba", 0x00954ac1, 0x00000003
	.section .rom.0095576e, "a"
	.incbin "baserom.gba", 0x0095576e, 0x00000002
	.section .rom.00956272, "a"
	.incbin "baserom.gba", 0x00956272, 0x00000002
	.section .rom.009571b1, "a"
	.incbin "baserom.gba", 0x009571b1, 0x00000003
	.section .rom.009579ed, "a"
	.incbin "baserom.gba", 0x009579ed, 0x00000003
	.section .rom.00958e92, "a"
	.incbin "baserom.gba", 0x00958e92, 0x00000002
	.section .rom.0095994a, "a"
	.incbin "baserom.gba", 0x0095994a, 0x00000002
	.section .rom.00959c75, "a"
	.incbin "baserom.gba", 0x00959c75, 0x00000003
	.section .rom.0095b0f0, "a"
	.incbin "baserom.gba", 0x0095b0f0, 0x00000400
	.section .rom.009678aa, "a"
	.incbin "baserom.gba", 0x009678aa, 0x000009fa
	.section .rom.0096870b, "a"
	.incbin "baserom.gba", 0x0096870b, 0x00000001
	.section .rom.00968917, "a"
	.incbin "baserom.gba", 0x00968917, 0x00000001
	.section .rom.00968b1b, "a"
	.incbin "baserom.gba", 0x00968b1b, 0x00000001
	.section .rom.00968baa, "a"
	.incbin "baserom.gba", 0x00968baa, 0x00000002
	.section .rom.0096908c, "a"
	.incbin "baserom.gba", 0x0096908c, 0x00000070
	.section .rom.00969157, "a"
	.incbin "baserom.gba", 0x00969157, 0x00000001
	.section .rom.0096918a, "a"
	.incbin "baserom.gba", 0x0096918a, 0x00000002
	.section .rom.00969356, "a"
	.incbin "baserom.gba", 0x00969356, 0x00000332
	.section .rom.00969741, "a"
	.incbin "baserom.gba", 0x00969741, 0x00000003
	.section .rom.00969916, "a"
	.incbin "baserom.gba", 0x00969916, 0x000000be
	.section .rom.00969b26, "a"
	.incbin "baserom.gba", 0x00969b26, 0x00000002
	.section .rom.00969c09, "a"
	.incbin "baserom.gba", 0x00969c09, 0x00000003
	.section .rom.00969cd9, "a"
	.incbin "baserom.gba", 0x00969cd9, 0x00000003
	.section .rom.00969dff, "a"
	.incbin "baserom.gba", 0x00969dff, 0x00000001
	.section .rom.00969e2e, "a"
	.incbin "baserom.gba", 0x00969e2e, 0x00000002
	.section .rom.0096a092, "a"
	.incbin "baserom.gba", 0x0096a092, 0x00000002
	.section .rom.0096a12f, "a"
	.incbin "baserom.gba", 0x0096a12f, 0x00000001
	.section .rom.0096a194, "a"
	.incbin "baserom.gba", 0x0096a194, 0x00000cf4
	.section .rom.009700bf, "a"
	.incbin "baserom.gba", 0x009700bf, 0x00000001
	.section .rom.0097300a, "a"
	.incbin "baserom.gba", 0x0097300a, 0x00000002
	.section .rom.00976c2d, "a"
	.incbin "baserom.gba", 0x00976c2d, 0x00000003
	.section .rom.00978e79, "a"
	.incbin "baserom.gba", 0x00978e79, 0x00000003
	.section .rom.0097e415, "a"
	.incbin "baserom.gba", 0x0097e415, 0x00000003
	.section .rom.00981c8f, "a"
	.incbin "baserom.gba", 0x00981c8f, 0x00000001
	.section .rom.00987af9, "a"
	.incbin "baserom.gba", 0x00987af9, 0x00000003
	.section .rom.00988bcd, "a"
	.incbin "baserom.gba", 0x00988bcd, 0x00000003
	.section .rom.00989c39, "a"
	.incbin "baserom.gba", 0x00989c39, 0x00000003
	.section .rom.0098f293, "a"
	.incbin "baserom.gba", 0x0098f293, 0x00000001
	.section .rom.009923d5, "a"
	.incbin "baserom.gba", 0x009923d5, 0x00000003
	.section .rom.00994475, "a"
	.incbin "baserom.gba", 0x00994475, 0x00000003
	.section .rom.00996795, "a"
	.incbin "baserom.gba", 0x00996795, 0x00000003
	.section .rom.0099940e, "a"
	.incbin "baserom.gba", 0x0099940e, 0x00000002
	.section .rom.0099b309, "a"
	.incbin "baserom.gba", 0x0099b309, 0x00000003
	.section .rom.009a384d, "a"
	.incbin "baserom.gba", 0x009a384d, 0x00000003
	.section .rom.009ac4cf, "a"
	.incbin "baserom.gba", 0x009ac4cf, 0x00000001
	.section .rom.009af049, "a"
	.incbin "baserom.gba", 0x009af049, 0x00000003
	.section .rom.009b5a86, "a"
	.incbin "baserom.gba", 0x009b5a86, 0x00000002
	.section .rom.009b857a, "a"
	.incbin "baserom.gba", 0x009b857a, 0x00000002
	.section .rom.009ba06a, "a"
	.incbin "baserom.gba", 0x009ba06a, 0x00000002
	.section .rom.009bc980, "a"
	.incbin "baserom.gba", 0x009bc980, 0x00003440
	.section .rom.009c4766, "a"
	.incbin "baserom.gba", 0x009c4766, 0x00000002
	.section .rom.009c5702, "a"
	.incbin "baserom.gba", 0x009c5702, 0x00000002
	.section .rom.009c7e2d, "a"
	.incbin "baserom.gba", 0x009c7e2d, 0x00000003
	.section .rom.009c9542, "a"
	.incbin "baserom.gba", 0x009c9542, 0x00000002
	.section .rom.009d1d5b, "a"
	.incbin "baserom.gba", 0x009d1d5b, 0x00000001
	.section .rom.009d6023, "a"
	.incbin "baserom.gba", 0x009d6023, 0x00000001
	.section .rom.009dd85d, "a"
	.incbin "baserom.gba", 0x009dd85d, 0x00000003
	.section .rom.009df042, "a"
	.incbin "baserom.gba", 0x009df042, 0x00000002
	.section .rom.009e1dd9, "a"
	.incbin "baserom.gba", 0x009e1dd9, 0x00000003
	.section .rom.009e2e23, "a"
	.incbin "baserom.gba", 0x009e2e23, 0x00000001
	.section .rom.009eb3b3, "a"
	.incbin "baserom.gba", 0x009eb3b3, 0x00000001
	.section .rom.009ed1ad, "a"
	.incbin "baserom.gba", 0x009ed1ad, 0x00000003
	.section .rom.009ee573, "a"
	.incbin "baserom.gba", 0x009ee573, 0x00000001
	.section .rom.009f5475, "a"
	.incbin "baserom.gba", 0x009f5475, 0x00000003
	.section .rom.009f7776, "a"
	.incbin "baserom.gba", 0x009f7776, 0x00000002
	.section .rom.009fc9c3, "a"
	.incbin "baserom.gba", 0x009fc9c3, 0x00000001
	.section .rom.00a01dc2, "a"
	.incbin "baserom.gba", 0x00a01dc2, 0x00000002
	.section .rom.00a03f76, "a"
	.incbin "baserom.gba", 0x00a03f76, 0x00000002
	.section .rom.00a080dd, "a"
	.incbin "baserom.gba", 0x00a080dd, 0x00000003
	.section .rom.00a0b302, "a"
	.incbin "baserom.gba", 0x00a0b302, 0x00000002
	.section .rom.00a0ca06, "a"
	.incbin "baserom.gba", 0x00a0ca06, 0x00000002
	.section .rom.00a135ee, "a"
	.incbin "baserom.gba", 0x00a135ee, 0x00000002
	.section .rom.00a1fbce, "a"
	.incbin "baserom.gba", 0x00a1fbce, 0x00000002
	.section .rom.00a22c62, "a"
	.incbin "baserom.gba", 0x00a22c62, 0x00000002
	.section .rom.00a254cd, "a"
	.incbin "baserom.gba", 0x00a254cd, 0x00000003
	.section .rom.00a26fbe, "a"
	.incbin "baserom.gba", 0x00a26fbe, 0x00000002
	.section .rom.00a27dd6, "a"
	.incbin "baserom.gba", 0x00a27dd6, 0x00000002
	.section .rom.00a28ac1, "a"
	.incbin "baserom.gba", 0x00a28ac1, 0x00000003
	.section .rom.00a31f7e, "a"
	.incbin "baserom.gba", 0x00a31f7e, 0x00000002
	.section .rom.00a32c89, "a"
	.incbin "baserom.gba", 0x00a32c89, 0x00000003
	.section .rom.00a33eed, "a"
	.incbin "baserom.gba", 0x00a33eed, 0x00000003
	.section .rom.00a34e1f, "a"
	.incbin "baserom.gba", 0x00a34e1f, 0x00000001
	.section .rom.00a35a8f, "a"
	.incbin "baserom.gba", 0x00a35a8f, 0x00000001
	.section .rom.00a366a7, "a"
	.incbin "baserom.gba", 0x00a366a7, 0x00000001
	.section .rom.00a36e1b, "a"
	.incbin "baserom.gba", 0x00a36e1b, 0x00000001
	.section .rom.00a386ab, "a"
	.incbin "baserom.gba", 0x00a386ab, 0x00000001
	.section .rom.00a39079, "a"
	.incbin "baserom.gba", 0x00a39079, 0x00000003
	.section .rom.00a39c97, "a"
	.incbin "baserom.gba", 0x00a39c97, 0x00000001
	.section .rom.00a3c34b, "a"
	.incbin "baserom.gba", 0x00a3c34b, 0x00000001
	.section .rom.00a3ea5e, "a"
	.incbin "baserom.gba", 0x00a3ea5e, 0x00000002
	.section .rom.00a439b3, "a"
	.incbin "baserom.gba", 0x00a439b3, 0x00000001
	.section .rom.00a47cdd, "a"
	.incbin "baserom.gba", 0x00a47cdd, 0x00000003
	.section .rom.00a488ca, "a"
	.incbin "baserom.gba", 0x00a488ca, 0x00000002
	.section .rom.00a4d47f, "a"
	.incbin "baserom.gba", 0x00a4d47f, 0x00000001
	.section .rom.00a4f7e9, "a"
	.incbin "baserom.gba", 0x00a4f7e9, 0x00000003
	.section .rom.00a5118b, "a"
	.incbin "baserom.gba", 0x00a5118b, 0x00000001
	.section .rom.00a55a4d, "a"
	.incbin "baserom.gba", 0x00a55a4d, 0x00000003
	.section .rom.00a58e5d, "a"
	.incbin "baserom.gba", 0x00a58e5d, 0x00000003
	.section .rom.00a5cf25, "a"
	.incbin "baserom.gba", 0x00a5cf25, 0x00000003
	.section .rom.00a605ee, "a"
	.incbin "baserom.gba", 0x00a605ee, 0x00000002
	.section .rom.00a685bf, "a"
	.incbin "baserom.gba", 0x00a685bf, 0x00000001
	.section .rom.00a6f8cf, "a"
	.incbin "baserom.gba", 0x00a6f8cf, 0x00000001
	.section .rom.00a7484f, "a"
	.incbin "baserom.gba", 0x00a7484f, 0x00000001
	.section .rom.00a792d1, "a"
	.incbin "baserom.gba", 0x00a792d1, 0x0000051f
	.section .rom.00a7aa4d, "a"
	.incbin "baserom.gba", 0x00a7aa4d, 0x00000003
	.section .rom.00a7ac1e, "a"
	.incbin "baserom.gba", 0x00a7ac1e, 0x00000002
	.section .rom.00a7ccbf, "a"
	.incbin "baserom.gba", 0x00a7ccbf, 0x00000001
	.section .rom.00a7dc4c, "a"
	.incbin "baserom.gba", 0x00a7dc4c, 0x000022d8
	.section .rom.00a81178, "a"
	.incbin "baserom.gba", 0x00a81178, 0x0000461c
	.section .rom.00a8589a, "a"
	.incbin "baserom.gba", 0x00a8589a, 0x00000002
	.section .rom.00a86a85, "a"
	.incbin "baserom.gba", 0x00a86a85, 0x00000003
	.section .rom.00a8870d, "a"
	.incbin "baserom.gba", 0x00a8870d, 0x00000003
	.section .rom.00a88c1b, "a"
	.incbin "baserom.gba", 0x00a88c1b, 0x00000001
	.section .rom.00a88d5b, "a"
	.incbin "baserom.gba", 0x00a88d5b, 0x00000001
	.section .rom.00a8a9de, "a"
	.incbin "baserom.gba", 0x00a8a9de, 0x00000002
	.section .rom.00a8d3b6, "a"
	.incbin "baserom.gba", 0x00a8d3b6, 0x00000002
	.section .rom.00a8fb7f, "a"
	.incbin "baserom.gba", 0x00a8fb7f, 0x00000001
	.section .rom.00a9109f, "a"
	.incbin "baserom.gba", 0x00a9109f, 0x00000001
	.section .rom.00a929e3, "a"
	.incbin "baserom.gba", 0x00a929e3, 0x00000001
	.section .rom.00a94491, "a"
	.incbin "baserom.gba", 0x00a94491, 0x00000003
	.section .rom.00a94605, "a"
	.incbin "baserom.gba", 0x00a94605, 0x00000003
	.section .rom.00a973e9, "a"
	.incbin "baserom.gba", 0x00a973e9, 0x00000003
	.section .rom.00a99c93, "a"
	.incbin "baserom.gba", 0x00a99c93, 0x00000001
	.section .rom.00a9e12a, "a"
	.incbin "baserom.gba", 0x00a9e12a, 0x00000002
	.section .rom.00aa14a2, "a"
	.incbin "baserom.gba", 0x00aa14a2, 0x00000002
	.section .rom.00aa3f9f, "a"
	.incbin "baserom.gba", 0x00aa3f9f, 0x00000001
	.section .rom.00aa607d, "a"
	.incbin "baserom.gba", 0x00aa607d, 0x00000003
	.section .rom.00aa82c9, "a"
	.incbin "baserom.gba", 0x00aa82c9, 0x00000003
	.section .rom.00aa840b, "a"
	.incbin "baserom.gba", 0x00aa840b, 0x00000001
	.section .rom.00aa9ade, "a"
	.incbin "baserom.gba", 0x00aa9ade, 0x00000002
	.section .rom.00aa9bde, "a"
	.incbin "baserom.gba", 0x00aa9bde, 0x00000002
	.section .rom.00aabad1, "a"
	.incbin "baserom.gba", 0x00aabad1, 0x00000003
	.section .rom.00aad4a9, "a"
	.incbin "baserom.gba", 0x00aad4a9, 0x00000003
	.section .rom.00aafeda, "a"
	.incbin "baserom.gba", 0x00aafeda, 0x00000002
	.section .rom.00ab512b, "a"
	.incbin "baserom.gba", 0x00ab512b, 0x00000001
	.section .rom.00ab7a6f, "a"
	.incbin "baserom.gba", 0x00ab7a6f, 0x00000001
	.section .rom.00ab8f42, "a"
	.incbin "baserom.gba", 0x00ab8f42, 0x00000002
	.section .rom.00abdd0d, "a"
	.incbin "baserom.gba", 0x00abdd0d, 0x00000003
	.section .rom.00ac09b7, "a"
	.incbin "baserom.gba", 0x00ac09b7, 0x00000001
	.section .rom.00ac3bef, "a"
	.incbin "baserom.gba", 0x00ac3bef, 0x00000001
	.section .rom.00ac3d87, "a"
	.incbin "baserom.gba", 0x00ac3d87, 0x00000001
	.section .rom.00ac6b87, "a"
	.incbin "baserom.gba", 0x00ac6b87, 0x00000001
	.section .rom.00ac833b, "a"
	.incbin "baserom.gba", 0x00ac833b, 0x00000001
	.section .rom.00ac92d6, "a"
	.incbin "baserom.gba", 0x00ac92d6, 0x00000002
	.section .rom.00acb937, "a"
	.incbin "baserom.gba", 0x00acb937, 0x00000001
	.section .rom.00acbade, "a"
	.incbin "baserom.gba", 0x00acbade, 0x00000002
	.section .rom.00acea97, "a"
	.incbin "baserom.gba", 0x00acea97, 0x00000001
	.section .rom.00acf73d, "a"
	.incbin "baserom.gba", 0x00acf73d, 0x00000003
	.section .rom.00ad0d0d, "a"
	.incbin "baserom.gba", 0x00ad0d0d, 0x00000003
	.section .rom.00ad8cd9, "a"
	.incbin "baserom.gba", 0x00ad8cd9, 0x00000003
	.section .rom.00ada7bf, "a"
	.incbin "baserom.gba", 0x00ada7bf, 0x00000001
	.section .rom.00adbca9, "a"
	.incbin "baserom.gba", 0x00adbca9, 0x00000003
	.section .rom.00ae1ed3, "a"
	.incbin "baserom.gba", 0x00ae1ed3, 0x00000001
	.section .rom.00ae23a5, "a"
	.incbin "baserom.gba", 0x00ae23a5, 0x00000003
	.section .rom.00ae34ed, "a"
	.incbin "baserom.gba", 0x00ae34ed, 0x00000003
	.section .rom.00ae369e, "a"
	.incbin "baserom.gba", 0x00ae369e, 0x00000002
	.section .rom.00aef38b, "a"
	.incbin "baserom.gba", 0x00aef38b, 0x00000001
	.section .rom.00aef4ab, "a"
	.incbin "baserom.gba", 0x00aef4ab, 0x00000001
	.section .rom.00af184b, "a"
	.incbin "baserom.gba", 0x00af184b, 0x00000001
	.section .rom.00af2a97, "a"
	.incbin "baserom.gba", 0x00af2a97, 0x00000001
	.section .rom.00af4e27, "a"
	.incbin "baserom.gba", 0x00af4e27, 0x00000001
	.section .rom.00af6725, "a"
	.incbin "baserom.gba", 0x00af6725, 0x00000003
	.section .rom.00af7716, "a"
	.incbin "baserom.gba", 0x00af7716, 0x00000002
	.section .rom.00af9cb2, "a"
	.incbin "baserom.gba", 0x00af9cb2, 0x00000002
	.section .rom.00afe2da, "a"
	.incbin "baserom.gba", 0x00afe2da, 0x00000002
	.section .rom.00afe99f, "a"
	.incbin "baserom.gba", 0x00afe99f, 0x00000001
	.section .rom.00b01435, "a"
	.incbin "baserom.gba", 0x00b01435, 0x00000003
	.section .rom.00b01586, "a"
	.incbin "baserom.gba", 0x00b01586, 0x00000002
	.section .rom.00b03996, "a"
	.incbin "baserom.gba", 0x00b03996, 0x00000002
	.section .rom.00b05b17, "a"
	.incbin "baserom.gba", 0x00b05b17, 0x00000001
	.section .rom.00b05c57, "a"
	.incbin "baserom.gba", 0x00b05c57, 0x00000001
	.section .rom.00b086f6, "a"
	.incbin "baserom.gba", 0x00b086f6, 0x00000002
	.section .rom.00b08847, "a"
	.incbin "baserom.gba", 0x00b08847, 0x00000001
	.section .rom.00b0ac56, "a"
	.incbin "baserom.gba", 0x00b0ac56, 0x00000002
	.section .rom.00b0cdd7, "a"
	.incbin "baserom.gba", 0x00b0cdd7, 0x00000001
	.section .rom.00b0cf17, "a"
	.incbin "baserom.gba", 0x00b0cf17, 0x00000001
	.section .rom.00b10546, "a"
	.incbin "baserom.gba", 0x00b10546, 0x00000002
	.section .rom.00b12aaa, "a"
	.incbin "baserom.gba", 0x00b12aaa, 0x00000002
	.section .rom.00b14c2b, "a"
	.incbin "baserom.gba", 0x00b14c2b, 0x00000001
	.section .rom.00b14d6b, "a"
	.incbin "baserom.gba", 0x00b14d6b, 0x00000001
	.section .rom.00b1621b, "a"
	.incbin "baserom.gba", 0x00b1621b, 0x00000001
	.section .rom.00b16326, "a"
	.incbin "baserom.gba", 0x00b16326, 0x00000002
	.section .rom.00b17e7e, "a"
	.incbin "baserom.gba", 0x00b17e7e, 0x00000002
	.section .rom.00b19521, "a"
	.incbin "baserom.gba", 0x00b19521, 0x00000003
	.section .rom.00b196e2, "a"
	.incbin "baserom.gba", 0x00b196e2, 0x00000d9e
	.section .rom.00b1c0d6, "a"
	.incbin "baserom.gba", 0x00b1c0d6, 0x00000002
	.section .rom.00b1d92d, "a"
	.incbin "baserom.gba", 0x00b1d92d, 0x00000003
	.section .rom.00b1daee, "a"
	.incbin "baserom.gba", 0x00b1daee, 0x00000002
	.section .rom.00b21d69, "a"
	.incbin "baserom.gba", 0x00b21d69, 0x00000003
	.section .rom.00b21f2a, "a"
	.incbin "baserom.gba", 0x00b21f2a, 0x00000002
	.section .rom.00b229a1, "a"
	.incbin "baserom.gba", 0x00b229a1, 0x00000003
	.section .rom.00b22aa9, "a"
	.incbin "baserom.gba", 0x00b22aa9, 0x00000003
	.section .rom.00b2476a, "a"
	.incbin "baserom.gba", 0x00b2476a, 0x00000002
	.section .rom.00b25ef7, "a"
	.incbin "baserom.gba", 0x00b25ef7, 0x00000001
	.section .rom.00b261ad, "a"
	.incbin "baserom.gba", 0x00b261ad, 0x00000003
	.section .rom.00b27cbd, "a"
	.incbin "baserom.gba", 0x00b27cbd, 0x00000003
	.section .rom.00b2a54a, "a"
	.incbin "baserom.gba", 0x00b2a54a, 0x00000002
	.section .rom.00b2cd3a, "a"
	.incbin "baserom.gba", 0x00b2cd3a, 0x00000002
	.section .rom.00b2dcda, "a"
	.incbin "baserom.gba", 0x00b2dcda, 0x00000002
	.section .rom.00b2fe2d, "a"
	.incbin "baserom.gba", 0x00b2fe2d, 0x00000003
	.section .rom.00b334ba, "a"
	.incbin "baserom.gba", 0x00b334ba, 0x00000002
	.section .rom.00b3524f, "a"
	.incbin "baserom.gba", 0x00b3524f, 0x00000001
	.section .rom.00b3706d, "a"
	.incbin "baserom.gba", 0x00b3706d, 0x00000003
	.section .rom.00b38b49, "a"
	.incbin "baserom.gba", 0x00b38b49, 0x00000003
	.section .rom.00b3a132, "a"
	.incbin "baserom.gba", 0x00b3a132, 0x00000002
	.section .rom.00b3c71a, "a"
	.incbin "baserom.gba", 0x00b3c71a, 0x00000002
	.section .rom.00b3c893, "a"
	.incbin "baserom.gba", 0x00b3c893, 0x00000001
	.section .rom.00b3dd52, "a"
	.incbin "baserom.gba", 0x00b3dd52, 0x00000002
	.section .rom.00b3f556, "a"
	.incbin "baserom.gba", 0x00b3f556, 0x00000002
	.section .rom.00b40ece, "a"
	.incbin "baserom.gba", 0x00b40ece, 0x00000002
	.section .rom.00b41dee, "a"
	.incbin "baserom.gba", 0x00b41dee, 0x00000002
	.section .rom.00b4389a, "a"
	.incbin "baserom.gba", 0x00b4389a, 0x00000002
	.section .rom.00b439a7, "a"
	.incbin "baserom.gba", 0x00b439a7, 0x00000001
	.section .rom.00b45ad7, "a"
	.incbin "baserom.gba", 0x00b45ad7, 0x00000001
	.section .rom.00b476ad, "a"
	.incbin "baserom.gba", 0x00b476ad, 0x00000003
	.section .rom.00b47cbf, "a"
	.incbin "baserom.gba", 0x00b47cbf, 0x00000001
	.section .rom.00b47dff, "a"
	.incbin "baserom.gba", 0x00b47dff, 0x00000001
	.section .rom.00b4a4ff, "a"
	.incbin "baserom.gba", 0x00b4a4ff, 0x00000001
	.section .rom.00b4bd5e, "a"
	.incbin "baserom.gba", 0x00b4bd5e, 0x00000002
	.section .rom.00b4c36f, "a"
	.incbin "baserom.gba", 0x00b4c36f, 0x00000001
	.section .rom.00b4c4af, "a"
	.incbin "baserom.gba", 0x00b4c4af, 0x00000001
	.section .rom.00b4d32a, "a"
	.incbin "baserom.gba", 0x00b4d32a, 0x00000002
	.section .rom.00b4d441, "a"
	.incbin "baserom.gba", 0x00b4d441, 0x00000003
	.section .rom.00b4f717, "a"
	.incbin "baserom.gba", 0x00b4f717, 0x00000001
	.section .rom.00b5136a, "a"
	.incbin "baserom.gba", 0x00b5136a, 0x00000002
	.section .rom.00b519af, "a"
	.incbin "baserom.gba", 0x00b519af, 0x00000001
	.section .rom.00b51aef, "a"
	.incbin "baserom.gba", 0x00b51aef, 0x00000001
	.section .rom.00b526f5, "a"
	.incbin "baserom.gba", 0x00b526f5, 0x00000003
	.section .rom.00b54ca7, "a"
	.incbin "baserom.gba", 0x00b54ca7, 0x00000001
	.section .rom.00b56966, "a"
	.incbin "baserom.gba", 0x00b56966, 0x00000002
	.section .rom.00b58432, "a"
	.incbin "baserom.gba", 0x00b58432, 0x00000002
	.section .rom.00b59506, "a"
	.incbin "baserom.gba", 0x00b59506, 0x00000002
	.section .rom.00b5966d, "a"
	.incbin "baserom.gba", 0x00b5966d, 0x00000003
	.section .rom.00b5a886, "a"
	.incbin "baserom.gba", 0x00b5a886, 0x00000002
	.section .rom.00b5b465, "a"
	.incbin "baserom.gba", 0x00b5b465, 0x00000003
	.section .rom.00b5b5d2, "a"
	.incbin "baserom.gba", 0x00b5b5d2, 0x00000002
	.section .rom.00b5e423, "a"
	.incbin "baserom.gba", 0x00b5e423, 0x00000001
	.section .rom.00b60be2, "a"
	.incbin "baserom.gba", 0x00b60be2, 0x00000002
	.section .rom.00b66c93, "a"
	.incbin "baserom.gba", 0x00b66c93, 0x00000001
	.section .rom.00b687c3, "a"
	.incbin "baserom.gba", 0x00b687c3, 0x00000001
	.section .rom.00b6aafa, "a"
	.incbin "baserom.gba", 0x00b6aafa, 0x00000002
	.section .rom.00b6bcab, "a"
	.incbin "baserom.gba", 0x00b6bcab, 0x00000001
	.section .rom.00b6d5cd, "a"
	.incbin "baserom.gba", 0x00b6d5cd, 0x00000003
	.section .rom.00b6f459, "a"
	.incbin "baserom.gba", 0x00b6f459, 0x00000003
	.section .rom.00b7163a, "a"
	.incbin "baserom.gba", 0x00b7163a, 0x00000002
	.section .rom.00b75001, "a"
	.incbin "baserom.gba", 0x00b75001, 0x00000003
	.section .rom.00b750ed, "a"
	.incbin "baserom.gba", 0x00b750ed, 0x00000003
	.section .rom.00b78326, "a"
	.incbin "baserom.gba", 0x00b78326, 0x00000002
	.section .rom.00b7899d, "a"
	.incbin "baserom.gba", 0x00b7899d, 0x00000003
	.section .rom.00b7c75f, "a"
	.incbin "baserom.gba", 0x00b7c75f, 0x00000001
	.section .rom.00b7e9f1, "a"
	.incbin "baserom.gba", 0x00b7e9f1, 0x00000003
	.section .rom.00b8040e, "a"
	.incbin "baserom.gba", 0x00b8040e, 0x00000002
	.section .rom.00b8151b, "a"
	.incbin "baserom.gba", 0x00b8151b, 0x00000001
	.section .rom.00b846ba, "a"
	.incbin "baserom.gba", 0x00b846ba, 0x00000002
	.section .rom.00b858b2, "a"
	.incbin "baserom.gba", 0x00b858b2, 0x00000002
	.section .rom.00b86775, "a"
	.incbin "baserom.gba", 0x00b86775, 0x00000003
	.section .rom.00b88816, "a"
	.incbin "baserom.gba", 0x00b88816, 0x00000002
	.section .rom.00b8a26b, "a"
	.incbin "baserom.gba", 0x00b8a26b, 0x00000001
	.section .rom.00b8b736, "a"
	.incbin "baserom.gba", 0x00b8b736, 0x00000002
	.section .rom.00b8d93a, "a"
	.incbin "baserom.gba", 0x00b8d93a, 0x00000002
	.section .rom.00b8da2f, "a"
	.incbin "baserom.gba", 0x00b8da2f, 0x00000001
	.section .rom.00b8f002, "a"
	.incbin "baserom.gba", 0x00b8f002, 0x00000002
	.section .rom.00b8f221, "a"
	.incbin "baserom.gba", 0x00b8f221, 0x00000003
	.section .rom.00b91305, "a"
	.incbin "baserom.gba", 0x00b91305, 0x00000003
	.section .rom.00b914ca, "a"
	.incbin "baserom.gba", 0x00b914ca, 0x00000002
	.section .rom.00b9358d, "a"
	.incbin "baserom.gba", 0x00b9358d, 0x00000003
	.section .rom.00b936bd, "a"
	.incbin "baserom.gba", 0x00b936bd, 0x00000003
	.section .rom.00b96176, "a"
	.incbin "baserom.gba", 0x00b96176, 0x00000002
	.section .rom.00b97cce, "a"
	.incbin "baserom.gba", 0x00b97cce, 0x00000002
	.section .rom.00b98c15, "a"
	.incbin "baserom.gba", 0x00b98c15, 0x00000003
	.section .rom.00b9a61e, "a"
	.incbin "baserom.gba", 0x00b9a61e, 0x00000002
	.section .rom.00baa373, "a"
	.incbin "baserom.gba", 0x00baa373, 0x00000001
	.section .rom.00baa4c3, "a"
	.incbin "baserom.gba", 0x00baa4c3, 0x00000001
	.section .rom.00bacfce, "a"
	.incbin "baserom.gba", 0x00bacfce, 0x00000002
	.section .rom.00baebd7, "a"
	.incbin "baserom.gba", 0x00baebd7, 0x00000001
	.section .rom.00bb05de, "a"
	.incbin "baserom.gba", 0x00bb05de, 0x00000002
	.section .rom.00bb22f9, "a"
	.incbin "baserom.gba", 0x00bb22f9, 0x00000003
	.section .rom.00bb244b, "a"
	.incbin "baserom.gba", 0x00bb244b, 0x00000001
	.section .rom.00bb3e52, "a"
	.incbin "baserom.gba", 0x00bb3e52, 0x00000002
	.section .rom.00bb7bda, "a"
	.incbin "baserom.gba", 0x00bb7bda, 0x00000002
	.section .rom.00bb7d6d, "a"
	.incbin "baserom.gba", 0x00bb7d6d, 0x00000003
	.section .rom.00bbcd77, "a"
	.incbin "baserom.gba", 0x00bbcd77, 0x00000001
	.section .rom.00bbf3f9, "a"
	.incbin "baserom.gba", 0x00bbf3f9, 0x00000003
	.section .rom.00bbf72f, "a"
	.incbin "baserom.gba", 0x00bbf72f, 0x00000001
	.section .rom.00bc5b85, "a"
	.incbin "baserom.gba", 0x00bc5b85, 0x00000003
	.section .rom.00bc5cd2, "a"
	.incbin "baserom.gba", 0x00bc5cd2, 0x00000002
	.section .rom.00bc7b57, "a"
	.incbin "baserom.gba", 0x00bc7b57, 0x00000001
	.section .rom.00bc90fe, "a"
	.incbin "baserom.gba", 0x00bc90fe, 0x00000002
	.section .rom.00bcb0a9, "a"
	.incbin "baserom.gba", 0x00bcb0a9, 0x00000003
	.section .rom.00bcc01a, "a"
	.incbin "baserom.gba", 0x00bcc01a, 0x00000002
	.section .rom.00bd7623, "a"
	.incbin "baserom.gba", 0x00bd7623, 0x00000001
	.section .rom.00bd76df, "a"
	.incbin "baserom.gba", 0x00bd76df, 0x00000001
	.section .rom.00bd841e, "a"
	.incbin "baserom.gba", 0x00bd841e, 0x00000002
	.section .rom.00bd8dfb, "a"
	.incbin "baserom.gba", 0x00bd8dfb, 0x00000001
	.section .rom.00bd9a13, "a"
	.incbin "baserom.gba", 0x00bd9a13, 0x00000001
	.section .rom.00bdb8ce, "a"
	.incbin "baserom.gba", 0x00bdb8ce, 0x00000002
	.section .rom.00bdb9ea, "a"
	.incbin "baserom.gba", 0x00bdb9ea, 0x00000002
	.section .rom.00bde26b, "a"
	.incbin "baserom.gba", 0x00bde26b, 0x00000001
	.section .rom.00bdf9f6, "a"
	.incbin "baserom.gba", 0x00bdf9f6, 0x00000002
	.section .rom.00be3926, "a"
	.incbin "baserom.gba", 0x00be3926, 0x00000002
	.section .rom.00be3a6f, "a"
	.incbin "baserom.gba", 0x00be3a6f, 0x00000001
	.section .rom.00be611f, "a"
	.incbin "baserom.gba", 0x00be611f, 0x00000001
	.section .rom.00be88c2, "a"
	.incbin "baserom.gba", 0x00be88c2, 0x00000002
	.section .rom.00be964d, "a"
	.incbin "baserom.gba", 0x00be964d, 0x00000003
	.section .rom.00bec48e, "a"
	.incbin "baserom.gba", 0x00bec48e, 0x00000002
	.section .rom.00beeab5, "a"
	.incbin "baserom.gba", 0x00beeab5, 0x00000003
	.section .rom.00bf07a1, "a"
	.incbin "baserom.gba", 0x00bf07a1, 0x00000003
	.section .rom.00bf1ea1, "a"
	.incbin "baserom.gba", 0x00bf1ea1, 0x00000003
	.section .rom.00bf341d, "a"
	.incbin "baserom.gba", 0x00bf341d, 0x00000003
	.section .rom.00bf4b8f, "a"
	.incbin "baserom.gba", 0x00bf4b8f, 0x00000001
	.section .rom.00bf781a, "a"
	.incbin "baserom.gba", 0x00bf781a, 0x00000002
	.section .rom.00bf8866, "a"
	.incbin "baserom.gba", 0x00bf8866, 0x00000002
	.section .rom.00bf9963, "a"
	.incbin "baserom.gba", 0x00bf9963, 0x00000001
	.section .rom.00bfb137, "a"
	.incbin "baserom.gba", 0x00bfb137, 0x00000001
	.section .rom.00bfc5fb, "a"
	.incbin "baserom.gba", 0x00bfc5fb, 0x00000001
	.section .rom.00bfeb2e, "a"
	.incbin "baserom.gba", 0x00bfeb2e, 0x00000002
	.section .rom.00bfec62, "a"
	.incbin "baserom.gba", 0x00bfec62, 0x00000002
	.section .rom.00c013ce, "a"
	.incbin "baserom.gba", 0x00c013ce, 0x00000002
	.section .rom.00c031a1, "a"
	.incbin "baserom.gba", 0x00c031a1, 0x00000003
	.section .rom.00c06a85, "a"
	.incbin "baserom.gba", 0x00c06a85, 0x00000003
	.section .rom.00c091eb, "a"
	.incbin "baserom.gba", 0x00c091eb, 0x00000001
	.section .rom.00c0ab55, "a"
	.incbin "baserom.gba", 0x00c0ab55, 0x00000003
	.section .rom.00c0d37e, "a"
	.incbin "baserom.gba", 0x00c0d37e, 0x00000002
	.section .rom.00c1175d, "a"
	.incbin "baserom.gba", 0x00c1175d, 0x00000003
	.section .rom.00c14b07, "a"
	.incbin "baserom.gba", 0x00c14b07, 0x00000001
	.section .rom.00c158fa, "a"
	.incbin "baserom.gba", 0x00c158fa, 0x00000002
	.section .rom.00c16672, "a"
	.incbin "baserom.gba", 0x00c16672, 0x00000002
	.section .rom.00c1e79a, "a"
	.incbin "baserom.gba", 0x00c1e79a, 0x00000002
	.section .rom.00c24b19, "a"
	.incbin "baserom.gba", 0x00c24b19, 0x00000003
	.section .rom.00c2551f, "a"
	.incbin "baserom.gba", 0x00c2551f, 0x00000001
	.section .rom.00c26fcd, "a"
	.incbin "baserom.gba", 0x00c26fcd, 0x00001493
	.section .rom.00c288c7, "a"
	.incbin "baserom.gba", 0x00c288c7, 0x00000001
	.section .rom.00c28a86, "a"
	.incbin "baserom.gba", 0x00c28a86, 0x00000002
	.section .rom.00c2b28e, "a"
	.incbin "baserom.gba", 0x00c2b28e, 0x00000002
	.section .rom.00c2b3e6, "a"
	.incbin "baserom.gba", 0x00c2b3e6, 0x00000002
	.section .rom.00c2dd2d, "a"
	.incbin "baserom.gba", 0x00c2dd2d, 0x00000003
	.section .rom.00c2ff51, "a"
	.incbin "baserom.gba", 0x00c2ff51, 0x00000003
	.section .rom.00c31446, "a"
	.incbin "baserom.gba", 0x00c31446, 0x00000002
	.section .rom.00c336a9, "a"
	.incbin "baserom.gba", 0x00c336a9, 0x00000003
	.section .rom.00c35f57, "a"
	.incbin "baserom.gba", 0x00c35f57, 0x00000001
	.section .rom.00c37eea, "a"
	.incbin "baserom.gba", 0x00c37eea, 0x00000002
	.section .rom.00c38f0b, "a"
	.incbin "baserom.gba", 0x00c38f0b, 0x00000001
	.section .rom.00c3904b, "a"
	.incbin "baserom.gba", 0x00c3904b, 0x00000001
	.section .rom.00c3bc6a, "a"
	.incbin "baserom.gba", 0x00c3bc6a, 0x00000002
	.section .rom.00c3e0ee, "a"
	.incbin "baserom.gba", 0x00c3e0ee, 0x00000002
	.section .rom.00c401d5, "a"
	.incbin "baserom.gba", 0x00c401d5, 0x00000003
	.section .rom.00c41a39, "a"
	.incbin "baserom.gba", 0x00c41a39, 0x00000003
	.section .rom.00c43f9f, "a"
	.incbin "baserom.gba", 0x00c43f9f, 0x00000001
	.section .rom.00c44105, "a"
	.incbin "baserom.gba", 0x00c44105, 0x00000003
	.section .rom.00c468ff, "a"
	.incbin "baserom.gba", 0x00c468ff, 0x00000001
	.section .rom.00c48933, "a"
	.incbin "baserom.gba", 0x00c48933, 0x00000001
	.section .rom.00c4a55a, "a"
	.incbin "baserom.gba", 0x00c4a55a, 0x00000002
	.section .rom.00c4cf7a, "a"
	.incbin "baserom.gba", 0x00c4cf7a, 0x00000002
	.section .rom.00c4dae5, "a"
	.incbin "baserom.gba", 0x00c4dae5, 0x00000003
	.section .rom.00c4dbcb, "a"
	.incbin "baserom.gba", 0x00c4dbcb, 0x00000001
	.section .rom.00c4f515, "a"
	.incbin "baserom.gba", 0x00c4f515, 0x00000003
	.section .rom.00c50e7b, "a"
	.incbin "baserom.gba", 0x00c50e7b, 0x00000001
	.section .rom.00c52249, "a"
	.incbin "baserom.gba", 0x00c52249, 0x00000003
	.section .rom.00c5232f, "a"
	.incbin "baserom.gba", 0x00c5232f, 0x00000001
	.section .rom.00c52e9d, "a"
	.incbin "baserom.gba", 0x00c52e9d, 0x00000003
	.section .rom.00c52f83, "a"
	.incbin "baserom.gba", 0x00c52f83, 0x00000001
	.section .rom.00c53ce7, "a"
	.incbin "baserom.gba", 0x00c53ce7, 0x00000001
	.section .rom.00c53dcb, "a"
	.incbin "baserom.gba", 0x00c53dcb, 0x00000001
	.section .rom.00c545d1, "a"
	.incbin "baserom.gba", 0x00c545d1, 0x00000003
	.section .rom.00c546b7, "a"
	.incbin "baserom.gba", 0x00c546b7, 0x00000001
	.section .rom.00c55217, "a"
	.incbin "baserom.gba", 0x00c55217, 0x00000001
	.section .rom.00c55a95, "a"
	.incbin "baserom.gba", 0x00c55a95, 0x00000003
	.section .rom.00c55b92, "a"
	.incbin "baserom.gba", 0x00c55b92, 0x00000002
	.section .rom.00c579eb, "a"
	.incbin "baserom.gba", 0x00c579eb, 0x00000001
	.section .rom.00c587f9, "a"
	.incbin "baserom.gba", 0x00c587f9, 0x00000003
	.section .rom.00c59a8d, "a"
	.incbin "baserom.gba", 0x00c59a8d, 0x00000003
	.section .rom.00c5aa69, "a"
	.incbin "baserom.gba", 0x00c5aa69, 0x00000003
	.section .rom.00c5ce6f, "a"
	.incbin "baserom.gba", 0x00c5ce6f, 0x00000001
	.section .rom.00c5cfaf, "a"
	.incbin "baserom.gba", 0x00c5cfaf, 0x00000001
	.section .rom.00c5d67d, "a"
	.incbin "baserom.gba", 0x00c5d67d, 0x00000003
	.section .rom.00c5d753, "a"
	.incbin "baserom.gba", 0x00c5d753, 0x00000001
	.section .rom.00c5dfc9, "a"
	.incbin "baserom.gba", 0x00c5dfc9, 0x00000003
	.section .rom.00c5e47a, "a"
	.incbin "baserom.gba", 0x00c5e47a, 0x00000002
	.section .rom.00c5f8f5, "a"
	.incbin "baserom.gba", 0x00c5f8f5, 0x00000003
	.section .rom.00c6010b, "a"
	.incbin "baserom.gba", 0x00c6010b, 0x00000001
	.section .rom.00c60da1, "a"
	.incbin "baserom.gba", 0x00c60da1, 0x00000003
	.section .rom.00c60f46, "a"
	.incbin "baserom.gba", 0x00c60f46, 0x00000002
	.section .rom.00c63e0a, "a"
	.incbin "baserom.gba", 0x00c63e0a, 0x00000002
	.section .rom.00c65789, "a"
	.incbin "baserom.gba", 0x00c65789, 0x00000003
	.section .rom.00c667d6, "a"
	.incbin "baserom.gba", 0x00c667d6, 0x00000002
	.section .rom.00c6df1b, "a"
	.incbin "baserom.gba", 0x00c6df1b, 0x00000001
	.section .rom.00c7570f, "a"
	.incbin "baserom.gba", 0x00c7570f, 0x00000001
	.section .rom.00c7bd09, "a"
	.incbin "baserom.gba", 0x00c7bd09, 0x00000003
	.section .rom.00c7e742, "a"
	.incbin "baserom.gba", 0x00c7e742, 0x00000002
	.section .rom.00c81013, "a"
	.incbin "baserom.gba", 0x00c81013, 0x00000001
	.section .rom.00c8585f, "a"
	.incbin "baserom.gba", 0x00c8585f, 0x00000001
	.section .rom.00c8685d, "a"
	.incbin "baserom.gba", 0x00c8685d, 0x00000003
	.section .rom.00c86976, "a"
	.incbin "baserom.gba", 0x00c86976, 0x00000002
	.section .rom.00c87f1e, "a"
	.incbin "baserom.gba", 0x00c87f1e, 0x00000002
	.section .rom.00c8997b, "a"
	.incbin "baserom.gba", 0x00c8997b, 0x00000001
	.section .rom.00c89ab6, "a"
	.incbin "baserom.gba", 0x00c89ab6, 0x00000002
	.section .rom.00c8afbd, "a"
	.incbin "baserom.gba", 0x00c8afbd, 0x00000003
	.section .rom.00c8e246, "a"
	.incbin "baserom.gba", 0x00c8e246, 0x00000002
	.section .rom.00c8f68d, "a"
	.incbin "baserom.gba", 0x00c8f68d, 0x00000003
	.section .rom.00c92385, "a"
	.incbin "baserom.gba", 0x00c92385, 0x00000003
	.section .rom.00c947bb, "a"
	.incbin "baserom.gba", 0x00c947bb, 0x00000001
	.section .rom.00c94997, "a"
	.incbin "baserom.gba", 0x00c94997, 0x00000001
	.section .rom.00c97aab, "a"
	.incbin "baserom.gba", 0x00c97aab, 0x00000001
	.section .rom.00c98f2e, "a"
	.incbin "baserom.gba", 0x00c98f2e, 0x00000002
	.section .rom.00c99f7d, "a"
	.incbin "baserom.gba", 0x00c99f7d, 0x00000003
	.section .rom.00c9bde2, "a"
	.incbin "baserom.gba", 0x00c9bde2, 0x00000002
	.section .rom.00c9bf7d, "a"
	.incbin "baserom.gba", 0x00c9bf7d, 0x00000003
	.section .rom.00c9c0db, "a"
	.incbin "baserom.gba", 0x00c9c0db, 0x00000001
	.section .rom.00c9d05a, "a"
	.incbin "baserom.gba", 0x00c9d05a, 0x00000002
	.section .rom.00c9d172, "a"
	.incbin "baserom.gba", 0x00c9d172, 0x00000002
	.section .rom.00c9f205, "a"
	.incbin "baserom.gba", 0x00c9f205, 0x00000003
	.section .rom.00ca1087, "a"
	.incbin "baserom.gba", 0x00ca1087, 0x00000001
	.section .rom.00ca35ad, "a"
	.incbin "baserom.gba", 0x00ca35ad, 0x00000003
	.section .rom.00ca36ba, "a"
	.incbin "baserom.gba", 0x00ca36ba, 0x00000002
	.section .rom.00ca574d, "a"
	.incbin "baserom.gba", 0x00ca574d, 0x00000003
	.section .rom.00ca8c23, "a"
	.incbin "baserom.gba", 0x00ca8c23, 0x00000001
	.section .rom.00ca970d, "a"
	.incbin "baserom.gba", 0x00ca970d, 0x00000003
	.section .rom.00ca987d, "a"
	.incbin "baserom.gba", 0x00ca987d, 0x00000003
	.section .rom.00cac797, "a"
	.incbin "baserom.gba", 0x00cac797, 0x00000001
	.section .rom.00cae722, "a"
	.incbin "baserom.gba", 0x00cae722, 0x00000002
	.section .rom.00cb008d, "a"
	.incbin "baserom.gba", 0x00cb008d, 0x00000003
	.section .rom.00cb5b7a, "a"
	.incbin "baserom.gba", 0x00cb5b7a, 0x00000002
	.section .rom.00cb5d2a, "a"
	.incbin "baserom.gba", 0x00cb5d2a, 0x00000002
	.section .rom.00cba6a2, "a"
	.incbin "baserom.gba", 0x00cba6a2, 0x00000002
	.section .rom.00cba7fa, "a"
	.incbin "baserom.gba", 0x00cba7fa, 0x00000002
	.section .rom.00cbddc7, "a"
	.incbin "baserom.gba", 0x00cbddc7, 0x00000001
	.section .rom.00cbf649, "a"
	.incbin "baserom.gba", 0x00cbf649, 0x00000003
	.section .rom.00cc1371, "a"
	.incbin "baserom.gba", 0x00cc1371, 0x00000003
	.section .rom.00cc4a92, "a"
	.incbin "baserom.gba", 0x00cc4a92, 0x00000002
	.section .rom.00cc9c1b, "a"
	.incbin "baserom.gba", 0x00cc9c1b, 0x00000001
	.section .rom.00ccc1e9, "a"
	.incbin "baserom.gba", 0x00ccc1e9, 0x00000003
	.section .rom.00ccdb75, "a"
	.incbin "baserom.gba", 0x00ccdb75, 0x00000003
	.section .rom.00cceca2, "a"
	.incbin "baserom.gba", 0x00cceca2, 0x00000002
	.section .rom.00ccf852, "a"
	.incbin "baserom.gba", 0x00ccf852, 0x00000002
	.section .rom.00cd121a, "a"
	.incbin "baserom.gba", 0x00cd121a, 0x00000002
	.section .rom.00cd12fe, "a"
	.incbin "baserom.gba", 0x00cd12fe, 0x00000002
	.section .rom.00cd375b, "a"
	.incbin "baserom.gba", 0x00cd375b, 0x00000001
	.section .rom.00cd389b, "a"
	.incbin "baserom.gba", 0x00cd389b, 0x00000001
	.section .rom.00cd6f11, "a"
	.incbin "baserom.gba", 0x00cd6f11, 0x00000003
	.section .rom.00cd7075, "a"
	.incbin "baserom.gba", 0x00cd7075, 0x00000003
	.section .rom.00cd9755, "a"
	.incbin "baserom.gba", 0x00cd9755, 0x00000003
	.section .rom.00cdc605, "a"
	.incbin "baserom.gba", 0x00cdc605, 0x00000003
	.section .rom.00cddb7a, "a"
	.incbin "baserom.gba", 0x00cddb7a, 0x00000002
	.section .rom.00ce1c76, "a"
	.incbin "baserom.gba", 0x00ce1c76, 0x00000002
	.section .rom.00ce3efb, "a"
	.incbin "baserom.gba", 0x00ce3efb, 0x00000001
	.section .rom.00ce5adf, "a"
	.incbin "baserom.gba", 0x00ce5adf, 0x00000001
	.section .rom.00ce77d5, "a"
	.incbin "baserom.gba", 0x00ce77d5, 0x00000003
	.section .rom.00cf4df9, "a"
	.incbin "baserom.gba", 0x00cf4df9, 0x00000003
	.section .rom.00cf4f21, "a"
	.incbin "baserom.gba", 0x00cf4f21, 0x00000003
	.section .rom.00cf7132, "a"
	.incbin "baserom.gba", 0x00cf7132, 0x00000002
	.section .rom.00cf72c3, "a"
	.incbin "baserom.gba", 0x00cf72c3, 0x00000001
	.section .rom.00cfe775, "a"
	.incbin "baserom.gba", 0x00cfe775, 0x00000003
	.section .rom.00d00f4b, "a"
	.incbin "baserom.gba", 0x00d00f4b, 0x00000001
	.section .rom.00d036a6, "a"
	.incbin "baserom.gba", 0x00d036a6, 0x00000002
	.section .rom.00d03859, "a"
	.incbin "baserom.gba", 0x00d03859, 0x00000003
	.section .rom.00d04f5d, "a"
	.incbin "baserom.gba", 0x00d04f5d, 0x00000003
	.section .rom.00d06fb7, "a"
	.incbin "baserom.gba", 0x00d06fb7, 0x00000001
	.section .rom.00d08301, "a"
	.incbin "baserom.gba", 0x00d08301, 0x00000003
	.section .rom.00d09d4b, "a"
	.incbin "baserom.gba", 0x00d09d4b, 0x00000001
	.section .rom.00d09e6f, "a"
	.incbin "baserom.gba", 0x00d09e6f, 0x00000001
	.section .rom.00d0a341, "a"
	.incbin "baserom.gba", 0x00d0a341, 0x00000003
	.section .rom.00d0c525, "a"
	.incbin "baserom.gba", 0x00d0c525, 0x00000003
	.section .rom.00d0c9f9, "a"
	.incbin "baserom.gba", 0x00d0c9f9, 0x00000003
	.section .rom.00d0ef2f, "a"
	.incbin "baserom.gba", 0x00d0ef2f, 0x00000001
	.section .rom.00d0f0b7, "a"
	.incbin "baserom.gba", 0x00d0f0b7, 0x00000001
	.section .rom.00d13959, "a"
	.incbin "baserom.gba", 0x00d13959, 0x00000003
	.section .rom.00d13a8f, "a"
	.incbin "baserom.gba", 0x00d13a8f, 0x00000001
	.section .rom.00d156fb, "a"
	.incbin "baserom.gba", 0x00d156fb, 0x00000001
	.section .rom.00d168b3, "a"
	.incbin "baserom.gba", 0x00d168b3, 0x00000001
	.section .rom.00d17601, "a"
	.incbin "baserom.gba", 0x00d17601, 0x00000003
	.section .rom.00d1878a, "a"
	.incbin "baserom.gba", 0x00d1878a, 0x00000002
	.section .rom.00d1a283, "a"
	.incbin "baserom.gba", 0x00d1a283, 0x00000001
	.section .rom.00d1b68f, "a"
	.incbin "baserom.gba", 0x00d1b68f, 0x00000001
	.section .rom.00d1bbe3, "a"
	.incbin "baserom.gba", 0x00d1bbe3, 0x00000001
	.section .rom.00d1c21d, "a"
	.incbin "baserom.gba", 0x00d1c21d, 0x00000003
	.section .rom.00d2156d, "a"
	.incbin "baserom.gba", 0x00d2156d, 0x00000003
	.section .rom.00d23bce, "a"
	.incbin "baserom.gba", 0x00d23bce, 0x00000002
	.section .rom.00d23d63, "a"
	.incbin "baserom.gba", 0x00d23d63, 0x00000001
	.section .rom.00d25917, "a"
	.incbin "baserom.gba", 0x00d25917, 0x00000001
	.section .rom.00d25a77, "a"
	.incbin "baserom.gba", 0x00d25a77, 0x00000001
	.section .rom.00d2851b, "a"
	.incbin "baserom.gba", 0x00d2851b, 0x000021b1
	.section .rom.00d2c542, "a"
	.incbin "baserom.gba", 0x00d2c542, 0x00000002
	.section .rom.00d2c683, "a"
	.incbin "baserom.gba", 0x00d2c683, 0x00000001
	.section .rom.00d335fb, "a"
	.incbin "baserom.gba", 0x00d335fb, 0x00000001
	.section .rom.00d33739, "a"
	.incbin "baserom.gba", 0x00d33739, 0x00000003
	.section .rom.00d3570d, "a"
	.incbin "baserom.gba", 0x00d3570d, 0x00000003
	.section .rom.00d35801, "a"
	.incbin "baserom.gba", 0x00d35801, 0x00000003
	.section .rom.00d368e1, "a"
	.incbin "baserom.gba", 0x00d368e1, 0x00000003
	.section .rom.00d387bb, "a"
	.incbin "baserom.gba", 0x00d387bb, 0x00000001
	.section .rom.00d39773, "a"
	.incbin "baserom.gba", 0x00d39773, 0x00000001
	.section .rom.00d3b6ba, "a"
	.incbin "baserom.gba", 0x00d3b6ba, 0x00000002
	.section .rom.00d3d2c5, "a"
	.incbin "baserom.gba", 0x00d3d2c5, 0x00000003
	.section .rom.00d3d411, "a"
	.incbin "baserom.gba", 0x00d3d411, 0x00000003
	.section .rom.00d3f4cf, "a"
	.incbin "baserom.gba", 0x00d3f4cf, 0x00000001
	.section .rom.00d435be, "a"
	.incbin "baserom.gba", 0x00d435be, 0x00000002
	.section .rom.00d47370, "a"
	.incbin "baserom.gba", 0x00d47370, 0x00007aa8
	.section .rom.00d50565, "a"
	.incbin "baserom.gba", 0x00d50565, 0x00000003
	.section .rom.00d50692, "a"
	.incbin "baserom.gba", 0x00d50692, 0x00000002
	.section .rom.00d57605, "a"
	.incbin "baserom.gba", 0x00d57605, 0x00000003
	.section .rom.00d5770a, "a"
	.incbin "baserom.gba", 0x00d5770a, 0x00000002
	.section .rom.00d5784a, "a"
	.incbin "baserom.gba", 0x00d5784a, 0x00000002
	.section .rom.00d59216, "a"
	.incbin "baserom.gba", 0x00d59216, 0x00000002
	.section .rom.00d5930d, "a"
	.incbin "baserom.gba", 0x00d5930d, 0x00000003
	.section .rom.00d5c161, "a"
	.incbin "baserom.gba", 0x00d5c161, 0x00000003
	.section .rom.00d5c30a, "a"
	.incbin "baserom.gba", 0x00d5c30a, 0x00000002
	.section .rom.00d5ef26, "a"
	.incbin "baserom.gba", 0x00d5ef26, 0x00000002
	.section .rom.00d61c59, "a"
	.incbin "baserom.gba", 0x00d61c59, 0x00000003
	.section .rom.00d63943, "a"
	.incbin "baserom.gba", 0x00d63943, 0x00000001
	.section .rom.00d671bd, "a"
	.incbin "baserom.gba", 0x00d671bd, 0x00000003
	.section .rom.00d67315, "a"
	.incbin "baserom.gba", 0x00d67315, 0x00000003
	.section .rom.00d69859, "a"
	.incbin "baserom.gba", 0x00d69859, 0x00000003
	.section .rom.00d6d71e, "a"
	.incbin "baserom.gba", 0x00d6d71e, 0x00000002
	.section .rom.00d6f84e, "a"
	.incbin "baserom.gba", 0x00d6f84e, 0x00000002
	.section .rom.00d73a5a, "a"
	.incbin "baserom.gba", 0x00d73a5a, 0x00000002
	.section .rom.00d73c2e, "a"
	.incbin "baserom.gba", 0x00d73c2e, 0x00000002
	.section .rom.00d75c6e, "a"
	.incbin "baserom.gba", 0x00d75c6e, 0x00000002
	.section .rom.00d77065, "a"
	.incbin "baserom.gba", 0x00d77065, 0x00000003
	.section .rom.00d77ba2, "a"
	.incbin "baserom.gba", 0x00d77ba2, 0x00000002
	.section .rom.00d78d0d, "a"
	.incbin "baserom.gba", 0x00d78d0d, 0x00000003
	.section .rom.00d79db9, "a"
	.incbin "baserom.gba", 0x00d79db9, 0x00000003
	.section .rom.00d79f65, "a"
	.incbin "baserom.gba", 0x00d79f65, 0x00000003
	.section .rom.00d7ba72, "a"
	.incbin "baserom.gba", 0x00d7ba72, 0x00000002
	.section .rom.00d7d48f, "a"
	.incbin "baserom.gba", 0x00d7d48f, 0x00000001
	.section .rom.00d7f961, "a"
	.incbin "baserom.gba", 0x00d7f961, 0x00000003
	.section .rom.00d80cc3, "a"
	.incbin "baserom.gba", 0x00d80cc3, 0x00000001
	.section .rom.00d81b77, "a"
	.incbin "baserom.gba", 0x00d81b77, 0x00000001
	.section .rom.00d83165, "a"
	.incbin "baserom.gba", 0x00d83165, 0x00000003
	.section .rom.00d846ab, "a"
	.incbin "baserom.gba", 0x00d846ab, 0x00000001
	.section .rom.00d85475, "a"
	.incbin "baserom.gba", 0x00d85475, 0x00000003
	.section .rom.00d855e3, "a"
	.incbin "baserom.gba", 0x00d855e3, 0x00000001
	.section .rom.00d865a2, "a"
	.incbin "baserom.gba", 0x00d865a2, 0x00000002
	.section .rom.00d8706b, "a"
	.incbin "baserom.gba", 0x00d8706b, 0x00000001
	.section .rom.00d87c05, "a"
	.incbin "baserom.gba", 0x00d87c05, 0x00000003
	.section .rom.00d87d55, "a"
	.incbin "baserom.gba", 0x00d87d55, 0x00000003
	.section .rom.00d8c003, "a"
	.incbin "baserom.gba", 0x00d8c003, 0x00000001
	.section .rom.00d8da9d, "a"
	.incbin "baserom.gba", 0x00d8da9d, 0x00000003
	.section .rom.00d8f473, "a"
	.incbin "baserom.gba", 0x00d8f473, 0x00000001
	.section .rom.00d8f5f2, "a"
	.incbin "baserom.gba", 0x00d8f5f2, 0x00000002
	.section .rom.00d93222, "a"
	.incbin "baserom.gba", 0x00d93222, 0x00000002
	.section .rom.00d94f7f, "a"
	.incbin "baserom.gba", 0x00d94f7f, 0x00000001
	.section .rom.00d985a9, "a"
	.incbin "baserom.gba", 0x00d985a9, 0x00000003
	.section .rom.00d986fa, "a"
	.incbin "baserom.gba", 0x00d986fa, 0x00000002
	.section .rom.00d9e2f6, "a"
	.incbin "baserom.gba", 0x00d9e2f6, 0x00000002
	.section .rom.00da0c33, "a"
	.incbin "baserom.gba", 0x00da0c33, 0x00000001
	.section .rom.00da2575, "a"
	.incbin "baserom.gba", 0x00da2575, 0x00000003
	.section .rom.00da26de, "a"
	.incbin "baserom.gba", 0x00da26de, 0x00000002
	.section .rom.00da94dd, "a"
	.incbin "baserom.gba", 0x00da94dd, 0x00000003
	.section .rom.00da9abb, "a"
	.incbin "baserom.gba", 0x00da9abb, 0x00000001
	.section .rom.00daa9c3, "a"
	.incbin "baserom.gba", 0x00daa9c3, 0x00000001
	.section .rom.00dad20b, "a"
	.incbin "baserom.gba", 0x00dad20b, 0x00000001
	.section .rom.00dae82a, "a"
	.incbin "baserom.gba", 0x00dae82a, 0x00000002
	.section .rom.00db018b, "a"
	.incbin "baserom.gba", 0x00db018b, 0x00000001
	.section .rom.00db2f73, "a"
	.incbin "baserom.gba", 0x00db2f73, 0x00000001
	.section .rom.00db30ee, "a"
	.incbin "baserom.gba", 0x00db30ee, 0x00000002
	.section .rom.00dc22b7, "a"
	.incbin "baserom.gba", 0x00dc22b7, 0x00000001
	.section .rom.00dc241f, "a"
	.incbin "baserom.gba", 0x00dc241f, 0x00000001
	.section .rom.00dc4a42, "a"
	.incbin "baserom.gba", 0x00dc4a42, 0x00000002
	.section .rom.00dc4b85, "a"
	.incbin "baserom.gba", 0x00dc4b85, 0x00000003
	.section .rom.00dc6442, "a"
	.incbin "baserom.gba", 0x00dc6442, 0x00000002
	.section .rom.00dc93a6, "a"
	.incbin "baserom.gba", 0x00dc93a6, 0x00000002
	.section .rom.00dcbd3e, "a"
	.incbin "baserom.gba", 0x00dcbd3e, 0x00000002
	.section .rom.00dd0696, "a"
	.incbin "baserom.gba", 0x00dd0696, 0x00000002
	.section .rom.00dd2cbf, "a"
	.incbin "baserom.gba", 0x00dd2cbf, 0x00000001
	.section .rom.00dd2e93, "a"
	.incbin "baserom.gba", 0x00dd2e93, 0x00000001
	.section .rom.00dd61b2, "a"
	.incbin "baserom.gba", 0x00dd61b2, 0x00000002
	.section .rom.00dd6389, "a"
	.incbin "baserom.gba", 0x00dd6389, 0x00000003
	.section .rom.00dd96fd, "a"
	.incbin "baserom.gba", 0x00dd96fd, 0x00000003
	.section .rom.00ddae51, "a"
	.incbin "baserom.gba", 0x00ddae51, 0x00000003
	.section .rom.00ddbddf, "a"
	.incbin "baserom.gba", 0x00ddbddf, 0x00000001
	.section .rom.00ddd755, "a"
	.incbin "baserom.gba", 0x00ddd755, 0x00000003
	.section .rom.00ddd88f, "a"
	.incbin "baserom.gba", 0x00ddd88f, 0x00000001
	.section .rom.00ddf3d1, "a"
	.incbin "baserom.gba", 0x00ddf3d1, 0x00000003
	.section .rom.00de29a9, "a"
	.incbin "baserom.gba", 0x00de29a9, 0x00000003
	.section .rom.00de3633, "a"
	.incbin "baserom.gba", 0x00de3633, 0x00000001
	.section .rom.00de3773, "a"
	.incbin "baserom.gba", 0x00de3773, 0x00000001
	.section .rom.00de5537, "a"
	.incbin "baserom.gba", 0x00de5537, 0x00000001
	.section .rom.00de56aa, "a"
	.incbin "baserom.gba", 0x00de56aa, 0x00000002
	.section .rom.00de7b12, "a"
	.incbin "baserom.gba", 0x00de7b12, 0x00000002
	.section .rom.00de94ae, "a"
	.incbin "baserom.gba", 0x00de94ae, 0x00000002
	.section .rom.00deb2e3, "a"
	.incbin "baserom.gba", 0x00deb2e3, 0x00000001
	.section .rom.00debd9a, "a"
	.incbin "baserom.gba", 0x00debd9a, 0x00000002
	.section .rom.00ded7b6, "a"
	.incbin "baserom.gba", 0x00ded7b6, 0x00000002
	.section .rom.00df0482, "a"
	.incbin "baserom.gba", 0x00df0482, 0x00000002
	.section .rom.00df065d, "a"
	.incbin "baserom.gba", 0x00df065d, 0x00000003
	.section .rom.00df2193, "a"
	.incbin "baserom.gba", 0x00df2193, 0x00000001
	.section .rom.00df525d, "a"
	.incbin "baserom.gba", 0x00df525d, 0x00000003
	.section .rom.00df6419, "a"
	.incbin "baserom.gba", 0x00df6419, 0x00000003
	.section .rom.00df6601, "a"
	.incbin "baserom.gba", 0x00df6601, 0x00000003
	.section .rom.00df6793, "a"
	.incbin "baserom.gba", 0x00df6793, 0x00000001
	.section .rom.00df751a, "a"
	.incbin "baserom.gba", 0x00df751a, 0x00000002
	.section .rom.00df76af, "a"
	.incbin "baserom.gba", 0x00df76af, 0x00000001
	.section .rom.00df9763, "a"
	.incbin "baserom.gba", 0x00df9763, 0x00000001
	.section .rom.00e017e6, "a"
	.incbin "baserom.gba", 0x00e017e6, 0x00000002
	.section .rom.00e01927, "a"
	.incbin "baserom.gba", 0x00e01927, 0x00000001
	.section .rom.00e05caf, "a"
	.incbin "baserom.gba", 0x00e05caf, 0x00000001
	.section .rom.00e063ce, "a"
	.incbin "baserom.gba", 0x00e063ce, 0x00000002
	.section .rom.00e0778d, "a"
	.incbin "baserom.gba", 0x00e0778d, 0x00000003
	.section .rom.00e0b8a7, "a"
	.incbin "baserom.gba", 0x00e0b8a7, 0x00000001
	.section .rom.00e0c927, "a"
	.incbin "baserom.gba", 0x00e0c927, 0x00000001
	.section .rom.00e0d9a9, "a"
	.incbin "baserom.gba", 0x00e0d9a9, 0x00000003
	.section .rom.00e0e51a, "a"
	.incbin "baserom.gba", 0x00e0e51a, 0x00000002
	.section .rom.00e0f46b, "a"
	.incbin "baserom.gba", 0x00e0f46b, 0x00000001
	.section .rom.00e0f5e9, "a"
	.incbin "baserom.gba", 0x00e0f5e9, 0x00000003
	.section .rom.00e12b43, "a"
	.incbin "baserom.gba", 0x00e12b43, 0x00000001
	.section .rom.00e14176, "a"
	.incbin "baserom.gba", 0x00e14176, 0x00000002
	.section .rom.00e142b7, "a"
	.incbin "baserom.gba", 0x00e142b7, 0x00000001
	.section .rom.00e16c02, "a"
	.incbin "baserom.gba", 0x00e16c02, 0x00000002
	.section .rom.00e1f5aa, "a"
	.incbin "baserom.gba", 0x00e1f5aa, 0x00000fde
	.section .rom.00e22155, "a"
	.incbin "baserom.gba", 0x00e22155, 0x00000003
	.section .rom.00e24095, "a"
	.incbin "baserom.gba", 0x00e24095, 0x00000003
	.section .rom.00e25509, "a"
	.incbin "baserom.gba", 0x00e25509, 0x00000003
	.section .rom.00e271c3, "a"
	.incbin "baserom.gba", 0x00e271c3, 0x00000001
	.section .rom.00e34272, "a"
	.incbin "baserom.gba", 0x00e34272, 0x00000002
	.section .rom.00e343b1, "a"
	.incbin "baserom.gba", 0x00e343b1, 0x00000003
	.section .rom.00e362d6, "a"
	.incbin "baserom.gba", 0x00e362d6, 0x00000002
	.section .rom.00e363ff, "a"
	.incbin "baserom.gba", 0x00e363ff, 0x00000001
	.section .rom.00e385cf, "a"
	.incbin "baserom.gba", 0x00e385cf, 0x00000001
	.section .rom.00e3ab9f, "a"
	.incbin "baserom.gba", 0x00e3ab9f, 0x00000001
	.section .rom.00e3ceb1, "a"
	.incbin "baserom.gba", 0x00e3ceb1, 0x00000003
	.section .rom.00e3d725, "a"
	.incbin "baserom.gba", 0x00e3d725, 0x00000003
	.section .rom.00e3d867, "a"
	.incbin "baserom.gba", 0x00e3d867, 0x00000001
	.section .rom.00e4091d, "a"
	.incbin "baserom.gba", 0x00e4091d, 0x00000003
	.section .rom.00e42003, "a"
	.incbin "baserom.gba", 0x00e42003, 0x00000001
	.section .rom.00e43bcb, "a"
	.incbin "baserom.gba", 0x00e43bcb, 0x00000001
	.section .rom.00e45746, "a"
	.incbin "baserom.gba", 0x00e45746, 0x00000002
	.section .rom.00e45b91, "a"
	.incbin "baserom.gba", 0x00e45b91, 0x00000003
	.section .rom.00e47cf2, "a"
	.incbin "baserom.gba", 0x00e47cf2, 0x00000002
	.section .rom.00e47e0b, "a"
	.incbin "baserom.gba", 0x00e47e0b, 0x00000001
	.section .rom.00e48c35, "a"
	.incbin "baserom.gba", 0x00e48c35, 0x00000003
	.section .rom.00e48d97, "a"
	.incbin "baserom.gba", 0x00e48d97, 0x00000001
	.section .rom.00e4d5ff, "a"
	.incbin "baserom.gba", 0x00e4d5ff, 0x00000001
	.section .rom.00e4fcbd, "a"
	.incbin "baserom.gba", 0x00e4fcbd, 0x00000003
	.section .rom.00e5029b, "a"
	.incbin "baserom.gba", 0x00e5029b, 0x00000001
	.section .rom.00e5222e, "a"
	.incbin "baserom.gba", 0x00e5222e, 0x00000002
	.section .rom.00e53a0e, "a"
	.incbin "baserom.gba", 0x00e53a0e, 0x00000002
	.section .rom.00e53b72, "a"
	.incbin "baserom.gba", 0x00e53b72, 0x00000002
	.section .rom.00e5636a, "a"
	.incbin "baserom.gba", 0x00e5636a, 0x00000002
	.section .rom.00e56523, "a"
	.incbin "baserom.gba", 0x00e56523, 0x00000001
	.section .rom.00e566ab, "a"
	.incbin "baserom.gba", 0x00e566ab, 0x00000001
	.section .rom.00e57ecd, "a"
	.incbin "baserom.gba", 0x00e57ecd, 0x00000003
	.section .rom.00e58ab6, "a"
	.incbin "baserom.gba", 0x00e58ab6, 0x00000002
	.section .rom.00e59fef, "a"
	.incbin "baserom.gba", 0x00e59fef, 0x00000001
	.section .rom.00e5a18a, "a"
	.incbin "baserom.gba", 0x00e5a18a, 0x00000002
	.section .rom.00e5c436, "a"
	.incbin "baserom.gba", 0x00e5c436, 0x00000002
	.section .rom.00e5e5fd, "a"
	.incbin "baserom.gba", 0x00e5e5fd, 0x00000003
	.section .rom.00e5fcfb, "a"
	.incbin "baserom.gba", 0x00e5fcfb, 0x00000001
	.section .rom.00e6081f, "a"
	.incbin "baserom.gba", 0x00e6081f, 0x00000001
	.section .rom.00e656c2, "a"
	.incbin "baserom.gba", 0x00e656c2, 0x00000002
	.section .rom.00e67a69, "a"
	.incbin "baserom.gba", 0x00e67a69, 0x00000003
	.section .rom.00e68152, "a"
	.incbin "baserom.gba", 0x00e68152, 0x00000002
	.section .rom.00e68d3e, "a"
	.incbin "baserom.gba", 0x00e68d3e, 0x00000ac6
	.section .rom.00e6adb3, "a"
	.incbin "baserom.gba", 0x00e6adb3, 0x00000001
	.section .rom.00e6af66, "a"
	.incbin "baserom.gba", 0x00e6af66, 0x00000002
	.section .rom.00e6d1a5, "a"
	.incbin "baserom.gba", 0x00e6d1a5, 0x00000003
	.section .rom.00e6dccb, "a"
	.incbin "baserom.gba", 0x00e6dccb, 0x00000001
	.section .rom.00e7875c, "a"
	.incbin "baserom.gba", 0x00e7875c, 0x000007d0
	.section .rom.00e7bbf5, "a"
	.incbin "baserom.gba", 0x00e7bbf5, 0x00000003
	.section .rom.00e7bd6f, "a"
	.incbin "baserom.gba", 0x00e7bd6f, 0x00000001
	.section .rom.00e7bee6, "a"
	.incbin "baserom.gba", 0x00e7bee6, 0x00000002
	.section .rom.00e7c07e, "a"
	.incbin "baserom.gba", 0x00e7c07e, 0x00000002
	.section .rom.00e7c20f, "a"
	.incbin "baserom.gba", 0x00e7c20f, 0x00000001
	.section .rom.00e7e4bb, "a"
	.incbin "baserom.gba", 0x00e7e4bb, 0x00000001
	.section .rom.00e7e5d3, "a"
	.incbin "baserom.gba", 0x00e7e5d3, 0x00000001
	.section .rom.00e803d5, "a"
	.incbin "baserom.gba", 0x00e803d5, 0x00000003
	.section .rom.00e81e9a, "a"
	.incbin "baserom.gba", 0x00e81e9a, 0x00000002
	.section .rom.00e825c2, "a"
	.incbin "baserom.gba", 0x00e825c2, 0x00000002
	.section .rom.00e839be, "a"
	.incbin "baserom.gba", 0x00e839be, 0x00000002
	.section .rom.00e84892, "a"
	.incbin "baserom.gba", 0x00e84892, 0x00000002
	.section .rom.00e849d6, "a"
	.incbin "baserom.gba", 0x00e849d6, 0x00000002
	.section .rom.00e86d87, "a"
	.incbin "baserom.gba", 0x00e86d87, 0x00000001
	.section .rom.00e895b5, "a"
	.incbin "baserom.gba", 0x00e895b5, 0x00000003
	.section .rom.00e899b2, "a"
	.incbin "baserom.gba", 0x00e899b2, 0x00000002
	.section .rom.00e8ae52, "a"
	.incbin "baserom.gba", 0x00e8ae52, 0x00000002
	.section .rom.00e8afa6, "a"
	.incbin "baserom.gba", 0x00e8afa6, 0x00000002
	.section .rom.00e8e8f3, "a"
	.incbin "baserom.gba", 0x00e8e8f3, 0x00000001
	.section .rom.00e8fd5e, "a"
	.incbin "baserom.gba", 0x00e8fd5e, 0x00000002
	.section .rom.00e91237, "a"
	.incbin "baserom.gba", 0x00e91237, 0x00000001
	.section .rom.00e9138a, "a"
	.incbin "baserom.gba", 0x00e9138a, 0x00000002
	.section .rom.00e94d97, "a"
	.incbin "baserom.gba", 0x00e94d97, 0x00000001
	.section .rom.00e95f6f, "a"
	.incbin "baserom.gba", 0x00e95f6f, 0x00000001
	.section .rom.00e9777a, "a"
	.incbin "baserom.gba", 0x00e9777a, 0x00000002
	.section .rom.00e978ca, "a"
	.incbin "baserom.gba", 0x00e978ca, 0x00000002
	.section .rom.00e99fd7, "a"
	.incbin "baserom.gba", 0x00e99fd7, 0x00000001
	.section .rom.00e9a13e, "a"
	.incbin "baserom.gba", 0x00e9a13e, 0x00000002
	.section .rom.00e9d0ff, "a"
	.incbin "baserom.gba", 0x00e9d0ff, 0x00000001
	.section .rom.00e9e36d, "a"
	.incbin "baserom.gba", 0x00e9e36d, 0x00000003
	.section .rom.00ea0efa, "a"
	.incbin "baserom.gba", 0x00ea0efa, 0x00000002
	.section .rom.00ea18e1, "a"
	.incbin "baserom.gba", 0x00ea18e1, 0x00000003
	.section .rom.00ea2282, "a"
	.incbin "baserom.gba", 0x00ea2282, 0x00000002
	.section .rom.00ea23f6, "a"
	.incbin "baserom.gba", 0x00ea23f6, 0x00000002
	.section .rom.00ea3f07, "a"
	.incbin "baserom.gba", 0x00ea3f07, 0x00000001
	.section .rom.00ea4029, "a"
	.incbin "baserom.gba", 0x00ea4029, 0x00000003
	.section .rom.00ea6b27, "a"
	.incbin "baserom.gba", 0x00ea6b27, 0x00000001
	.section .rom.00ea791e, "a"
	.incbin "baserom.gba", 0x00ea791e, 0x00000002
	.section .rom.00ea98bd, "a"
	.incbin "baserom.gba", 0x00ea98bd, 0x00000003
	.section .rom.00eaabed, "a"
	.incbin "baserom.gba", 0x00eaabed, 0x00000003
	.section .rom.00eaad41, "a"
	.incbin "baserom.gba", 0x00eaad41, 0x00000003
	.section .rom.00eab8f3, "a"
	.incbin "baserom.gba", 0x00eab8f3, 0x00000001
	.section .rom.00ead0cd, "a"
	.incbin "baserom.gba", 0x00ead0cd, 0x00000003
	.section .rom.00eae22d, "a"
	.incbin "baserom.gba", 0x00eae22d, 0x00000003
	.section .rom.00eaf34b, "a"
	.incbin "baserom.gba", 0x00eaf34b, 0x00000001
	.section .rom.00eaf552, "a"
	.incbin "baserom.gba", 0x00eaf552, 0x00000002
	.section .rom.00eb0247, "a"
	.incbin "baserom.gba", 0x00eb0247, 0x00000001
	.section .rom.00eb29e9, "a"
	.incbin "baserom.gba", 0x00eb29e9, 0x00000003
	.section .rom.00eb415e, "a"
	.incbin "baserom.gba", 0x00eb415e, 0x00000002
	.section .rom.00eb541b, "a"
	.incbin "baserom.gba", 0x00eb541b, 0x00000001
	.section .rom.00eb5c5e, "a"
	.incbin "baserom.gba", 0x00eb5c5e, 0x0000060e
	.section .rom.00eb67e3, "a"
	.incbin "baserom.gba", 0x00eb67e3, 0x00000001
	.section .rom.00eb6923, "a"
	.incbin "baserom.gba", 0x00eb6923, 0x00000001
	.section .rom.00eb6a63, "a"
	.incbin "baserom.gba", 0x00eb6a63, 0x00000001
	.section .rom.00eb75c5, "a"
	.incbin "baserom.gba", 0x00eb75c5, 0x00000003
	.section .rom.00eb9d69, "a"
	.incbin "baserom.gba", 0x00eb9d69, 0x00000003
	.section .rom.00ebb4de, "a"
	.incbin "baserom.gba", 0x00ebb4de, 0x00000002
	.section .rom.00ebc79b, "a"
	.incbin "baserom.gba", 0x00ebc79b, 0x00000001
	.section .rom.00ebcfde, "a"
	.incbin "baserom.gba", 0x00ebcfde, 0x00000002
	.section .rom.00ebd57d, "a"
	.incbin "baserom.gba", 0x00ebd57d, 0x0000051f
	.section .rom.00ebe1b1, "a"
	.incbin "baserom.gba", 0x00ebe1b1, 0x0000051f
	.section .rom.00ebf291, "a"
	.incbin "baserom.gba", 0x00ebf291, 0x0000051f
	.section .rom.00ebfd7d, "a"
	.incbin "baserom.gba", 0x00ebfd7d, 0x0000051f
	.section .rom.00ec0c19, "a"
	.incbin "baserom.gba", 0x00ec0c19, 0x0000051f
	.section .rom.00ec1561, "a"
	.incbin "baserom.gba", 0x00ec1561, 0x0000051f
	.section .rom.00ec20a9, "a"
	.incbin "baserom.gba", 0x00ec20a9, 0x0000051f
	.section .rom.00ec2cf1, "a"
	.incbin "baserom.gba", 0x00ec2cf1, 0x0013d30f
