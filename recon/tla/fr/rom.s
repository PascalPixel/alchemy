@ tla-fr's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.incbin "baserom.gba", 0x00002064, 0x00058784
	.section .rom.0005c3e8, "a"
	.incbin "baserom.gba", 0x0005c3e8, 0x00164f28
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
	.section .rom.0068a0c0, "a"
	.incbin "baserom.gba", 0x0068a0c0, 0x0000d7f8
	.section .rom.006a444d, "a"
	.incbin "baserom.gba", 0x006a444d, 0x00004027
	.section .rom.006a99ab, "a"
	.incbin "baserom.gba", 0x006a99ab, 0x00024769
	.section .rom.006d0b02, "a"
	.incbin "baserom.gba", 0x006d0b02, 0x00000002
	.section .rom.006d4f46, "a"
	.incbin "baserom.gba", 0x006d4f46, 0x00000002
	.section .rom.006e56b6, "a"
	.incbin "baserom.gba", 0x006e56b6, 0x00000002
	.section .rom.006e8ca6, "a"
	.incbin "baserom.gba", 0x006e8ca6, 0x00000002
	.section .rom.006f474a, "a"
	.incbin "baserom.gba", 0x006f474a, 0x00000002
	.section .rom.00704dfa, "a"
	.incbin "baserom.gba", 0x00704dfa, 0x00000002
	.section .rom.0070c89a, "a"
	.incbin "baserom.gba", 0x0070c89a, 0x00000002
	.section .rom.007100da, "a"
	.incbin "baserom.gba", 0x007100da, 0x00000002
	.section .rom.0071950e, "a"
	.incbin "baserom.gba", 0x0071950e, 0x00000002
	.section .rom.007206ba, "a"
	.incbin "baserom.gba", 0x007206ba, 0x00000002
	.section .rom.00728576, "a"
	.incbin "baserom.gba", 0x00728576, 0x00000002
	.section .rom.0072d006, "a"
	.incbin "baserom.gba", 0x0072d006, 0x00000002
	.section .rom.007312ba, "a"
	.incbin "baserom.gba", 0x007312ba, 0x00000002
	.section .rom.00734c3a, "a"
	.incbin "baserom.gba", 0x00734c3a, 0x00000002
	.section .rom.00741476, "a"
	.incbin "baserom.gba", 0x00741476, 0x00000002
	.section .rom.0074cbb6, "a"
	.incbin "baserom.gba", 0x0074cbb6, 0x00000002
	.section .rom.0074ff86, "a"
	.incbin "baserom.gba", 0x0074ff86, 0x00000002
	.section .rom.00761ea2, "a"
	.incbin "baserom.gba", 0x00761ea2, 0x00000002
	.section .rom.0076590a, "a"
	.incbin "baserom.gba", 0x0076590a, 0x00000002
	.section .rom.007692fa, "a"
	.incbin "baserom.gba", 0x007692fa, 0x00000002
	.section .rom.00779862, "a"
	.incbin "baserom.gba", 0x00779862, 0x00000002
	.section .rom.0077d792, "a"
	.incbin "baserom.gba", 0x0077d792, 0x00000002
	.section .rom.007811b6, "a"
	.incbin "baserom.gba", 0x007811b6, 0x00000002
	.section .rom.007894d6, "a"
	.incbin "baserom.gba", 0x007894d6, 0x00000002
	.section .rom.00791642, "a"
	.incbin "baserom.gba", 0x00791642, 0x00000002
	.section .rom.0079e90e, "a"
	.incbin "baserom.gba", 0x0079e90e, 0x00000002
	.section .rom.007a27ca, "a"
	.incbin "baserom.gba", 0x007a27ca, 0x00000002
	.section .rom.007a65c6, "a"
	.incbin "baserom.gba", 0x007a65c6, 0x00000002
	.section .rom.007aaa96, "a"
	.incbin "baserom.gba", 0x007aaa96, 0x00000002
	.section .rom.007b2472, "a"
	.incbin "baserom.gba", 0x007b2472, 0x00000002
	.section .rom.007b6a9e, "a"
	.incbin "baserom.gba", 0x007b6a9e, 0x00000002
	.section .rom.007be71a, "a"
	.incbin "baserom.gba", 0x007be71a, 0x00000002
	.section .rom.007c2316, "a"
	.incbin "baserom.gba", 0x007c2316, 0x00000002
	.section .rom.007c5cc6, "a"
	.incbin "baserom.gba", 0x007c5cc6, 0x00000002
	.section .rom.007ca116, "a"
	.incbin "baserom.gba", 0x007ca116, 0x00000002
	.section .rom.007cdd0e, "a"
	.incbin "baserom.gba", 0x007cdd0e, 0x00000002
	.section .rom.007d996e, "a"
	.incbin "baserom.gba", 0x007d996e, 0x00000002
	.section .rom.007dd5be, "a"
	.incbin "baserom.gba", 0x007dd5be, 0x00000002
	.section .rom.007e5b3e, "a"
	.incbin "baserom.gba", 0x007e5b3e, 0x00000002
	.section .rom.007f218e, "a"
	.incbin "baserom.gba", 0x007f218e, 0x00000002
	.section .rom.007f8c42, "a"
	.incbin "baserom.gba", 0x007f8c42, 0x00000002
	.section .rom.00801f0a, "a"
	.incbin "baserom.gba", 0x00801f0a, 0x00000002
	.section .rom.0080992e, "a"
	.incbin "baserom.gba", 0x0080992e, 0x00000002
	.section .rom.0081f5ae, "a"
	.incbin "baserom.gba", 0x0081f5ae, 0x00000002
	.section .rom.008289f2, "a"
	.incbin "baserom.gba", 0x008289f2, 0x00000002
	.section .rom.00831c0e, "a"
	.incbin "baserom.gba", 0x00831c0e, 0x00000002
	.section .rom.0083f536, "a"
	.incbin "baserom.gba", 0x0083f536, 0x00000002
	.section .rom.0084538a, "a"
	.incbin "baserom.gba", 0x0084538a, 0x00000002
	.section .rom.0084b905, "a"
	.incbin "baserom.gba", 0x0084b905, 0x00000003
	.section .rom.0084d257, "a"
	.incbin "baserom.gba", 0x0084d257, 0x00000001
	.section .rom.00850075, "a"
	.incbin "baserom.gba", 0x00850075, 0x00000003
	.section .rom.00853f8a, "a"
	.incbin "baserom.gba", 0x00853f8a, 0x00000002
	.section .rom.00857133, "a"
	.incbin "baserom.gba", 0x00857133, 0x000009bd
	.section .rom.008580d7, "a"
	.incbin "baserom.gba", 0x008580d7, 0x00000001
	.section .rom.00858785, "a"
	.incbin "baserom.gba", 0x00858785, 0x00000003
	.section .rom.00858a52, "a"
	.incbin "baserom.gba", 0x00858a52, 0x00000002
	.section .rom.008595ad, "a"
	.incbin "baserom.gba", 0x008595ad, 0x00000003
	.section .rom.0085a16b, "a"
	.incbin "baserom.gba", 0x0085a16b, 0x00000001
	.section .rom.0085a25e, "a"
	.incbin "baserom.gba", 0x0085a25e, 0x00000002
	.section .rom.0085a3ef, "a"
	.incbin "baserom.gba", 0x0085a3ef, 0x00000001
	.section .rom.0085a8ce, "a"
	.incbin "baserom.gba", 0x0085a8ce, 0x00000002
	.section .rom.0085bb6b, "a"
	.incbin "baserom.gba", 0x0085bb6b, 0x00000001
	.section .rom.0085e43f, "a"
	.incbin "baserom.gba", 0x0085e43f, 0x00000001
	.section .rom.0085ebba, "a"
	.incbin "baserom.gba", 0x0085ebba, 0x00000002
	.section .rom.008601ff, "a"
	.incbin "baserom.gba", 0x008601ff, 0x00000001
	.section .rom.00860729, "a"
	.incbin "baserom.gba", 0x00860729, 0x00000003
	.section .rom.00860f32, "a"
	.incbin "baserom.gba", 0x00860f32, 0x00000002
	.section .rom.008614ab, "a"
	.incbin "baserom.gba", 0x008614ab, 0x00000001
	.section .rom.00862bea, "a"
	.incbin "baserom.gba", 0x00862bea, 0x00000002
	.section .rom.00865ab3, "a"
	.incbin "baserom.gba", 0x00865ab3, 0x00000001
	.section .rom.00865d65, "a"
	.incbin "baserom.gba", 0x00865d65, 0x00000003
	.section .rom.00867147, "a"
	.incbin "baserom.gba", 0x00867147, 0x00000001
	.section .rom.0086b983, "a"
	.incbin "baserom.gba", 0x0086b983, 0x00000001
	.section .rom.0086f129, "a"
	.incbin "baserom.gba", 0x0086f129, 0x00000003
	.section .rom.00870e4e, "a"
	.incbin "baserom.gba", 0x00870e4e, 0x00000002
	.section .rom.00872d4d, "a"
	.incbin "baserom.gba", 0x00872d4d, 0x00000003
	.section .rom.008746fb, "a"
	.incbin "baserom.gba", 0x008746fb, 0x00000001
	.section .rom.00875c73, "a"
	.incbin "baserom.gba", 0x00875c73, 0x00000001
	.section .rom.008798a6, "a"
	.incbin "baserom.gba", 0x008798a6, 0x00000002
	.section .rom.0087a404, "a"
	.incbin "baserom.gba", 0x0087a404, 0x00003b38
	.section .rom.0087e573, "a"
	.incbin "baserom.gba", 0x0087e573, 0x00000001
	.section .rom.008806dd, "a"
	.incbin "baserom.gba", 0x008806dd, 0x00000003
	.section .rom.008807e8, "a"
	.incbin "baserom.gba", 0x008807e8, 0x000003d0
	.section .rom.00881471, "a"
	.incbin "baserom.gba", 0x00881471, 0x00000003
	.section .rom.00883055, "a"
	.incbin "baserom.gba", 0x00883055, 0x00000003
	.section .rom.00884bb2, "a"
	.incbin "baserom.gba", 0x00884bb2, 0x00000002
	.section .rom.00884ff1, "a"
	.incbin "baserom.gba", 0x00884ff1, 0x00000003
	.section .rom.00885406, "a"
	.incbin "baserom.gba", 0x00885406, 0x00001842
	.section .rom.00889153, "a"
	.incbin "baserom.gba", 0x00889153, 0x00000001
	.section .rom.00889b0f, "a"
	.incbin "baserom.gba", 0x00889b0f, 0x00000001
	.section .rom.0088ac52, "a"
	.incbin "baserom.gba", 0x0088ac52, 0x00001496
	.section .rom.0088d663, "a"
	.incbin "baserom.gba", 0x0088d663, 0x00000001
	.section .rom.0088d98d, "a"
	.incbin "baserom.gba", 0x0088d98d, 0x0000104b
	.section .rom.0088f161, "a"
	.incbin "baserom.gba", 0x0088f161, 0x00000623
	.section .rom.0088fdad, "a"
	.incbin "baserom.gba", 0x0088fdad, 0x00000003
	.section .rom.008901b7, "a"
	.incbin "baserom.gba", 0x008901b7, 0x000007d9
	.section .rom.00890c39, "a"
	.incbin "baserom.gba", 0x00890c39, 0x0000095f
	.section .rom.00891c55, "a"
	.incbin "baserom.gba", 0x00891c55, 0x00003c7f
	.section .rom.00895f33, "a"
	.incbin "baserom.gba", 0x00895f33, 0x00000001
	.section .rom.00896f6d, "a"
	.incbin "baserom.gba", 0x00896f6d, 0x00000003
	.section .rom.008975c3, "a"
	.incbin "baserom.gba", 0x008975c3, 0x00000001
	.section .rom.00897c41, "a"
	.incbin "baserom.gba", 0x00897c41, 0x00000003
	.section .rom.00898261, "a"
	.incbin "baserom.gba", 0x00898261, 0x00000003
	.section .rom.0089962e, "a"
	.incbin "baserom.gba", 0x0089962e, 0x00000002
	.section .rom.0089a672, "a"
	.incbin "baserom.gba", 0x0089a672, 0x00000002
	.section .rom.0089b0fb, "a"
	.incbin "baserom.gba", 0x0089b0fb, 0x000002cd
	.section .rom.0089c101, "a"
	.incbin "baserom.gba", 0x0089c101, 0x00000003
	.section .rom.0089d33d, "a"
	.incbin "baserom.gba", 0x0089d33d, 0x00000003
	.section .rom.0089dc87, "a"
	.incbin "baserom.gba", 0x0089dc87, 0x00006c09
	.section .rom.008a4dc4, "a"
	.incbin "baserom.gba", 0x008a4dc4, 0x000011d8
	.section .rom.008a648a, "a"
	.incbin "baserom.gba", 0x008a648a, 0x000027fe
	.section .rom.008a92cb, "a"
	.incbin "baserom.gba", 0x008a92cb, 0x000000a9
	.section .rom.008a968a, "a"
	.incbin "baserom.gba", 0x008a968a, 0x000018d2
	.section .rom.008abbcd, "a"
	.incbin "baserom.gba", 0x008abbcd, 0x00000003
	.section .rom.008ac415, "a"
	.incbin "baserom.gba", 0x008ac415, 0x00001203
	.section .rom.008ae4d7, "a"
	.incbin "baserom.gba", 0x008ae4d7, 0x00000001
	.section .rom.008aefb3, "a"
	.incbin "baserom.gba", 0x008aefb3, 0x00000001
	.section .rom.008af676, "a"
	.incbin "baserom.gba", 0x008af676, 0x00000002
	.section .rom.008af95d, "a"
	.incbin "baserom.gba", 0x008af95d, 0x00000003
	.section .rom.008b0cc3, "a"
	.incbin "baserom.gba", 0x008b0cc3, 0x00000001
	.section .rom.008b10ad, "a"
	.incbin "baserom.gba", 0x008b10ad, 0x00000003
	.section .rom.008b147f, "a"
	.incbin "baserom.gba", 0x008b147f, 0x00000001
	.section .rom.008b1d1f, "a"
	.incbin "baserom.gba", 0x008b1d1f, 0x00000001
	.section .rom.008b21c2, "a"
	.incbin "baserom.gba", 0x008b21c2, 0x00000002
	.section .rom.008b337b, "a"
	.incbin "baserom.gba", 0x008b337b, 0x00000001
	.section .rom.008b37d4, "a"
	.incbin "baserom.gba", 0x008b37d4, 0x00002034
	.section .rom.008b5a62, "a"
	.incbin "baserom.gba", 0x008b5a62, 0x00000002
	.section .rom.008b74bd, "a"
	.incbin "baserom.gba", 0x008b74bd, 0x000001b7
	.section .rom.008b7a09, "a"
	.incbin "baserom.gba", 0x008b7a09, 0x00000003
	.section .rom.008b9792, "a"
	.incbin "baserom.gba", 0x008b9792, 0x0000027a
	.section .rom.008b9ed2, "a"
	.incbin "baserom.gba", 0x008b9ed2, 0x00000002
	.section .rom.008bbaa7, "a"
	.incbin "baserom.gba", 0x008bbaa7, 0x00000001
	.section .rom.008bd6a5, "a"
	.incbin "baserom.gba", 0x008bd6a5, 0x00000003
	.section .rom.008bd8c6, "a"
	.incbin "baserom.gba", 0x008bd8c6, 0x0000043e
	.section .rom.008bde15, "a"
	.incbin "baserom.gba", 0x008bde15, 0x00000003
	.section .rom.008be3f7, "a"
	.incbin "baserom.gba", 0x008be3f7, 0x000004a1
	.section .rom.008bf5aa, "a"
	.incbin "baserom.gba", 0x008bf5aa, 0x00000002
	.section .rom.008bfaec, "a"
	.incbin "baserom.gba", 0x008bfaec, 0x00000840
	.section .rom.008c06bb, "a"
	.incbin "baserom.gba", 0x008c06bb, 0x00001259
	.section .rom.008c2402, "a"
	.incbin "baserom.gba", 0x008c2402, 0x00000002
	.section .rom.008c31d9, "a"
	.incbin "baserom.gba", 0x008c31d9, 0x00000003
	.section .rom.008c3a06, "a"
	.incbin "baserom.gba", 0x008c3a06, 0x00000002
	.section .rom.008c3df8, "a"
	.incbin "baserom.gba", 0x008c3df8, 0x000010d0
	.section .rom.008c5839, "a"
	.incbin "baserom.gba", 0x008c5839, 0x00000003
	.section .rom.008c5be7, "a"
	.incbin "baserom.gba", 0x008c5be7, 0x00000001
	.section .rom.008c7b8b, "a"
	.incbin "baserom.gba", 0x008c7b8b, 0x00000001
	.section .rom.008c8927, "a"
	.incbin "baserom.gba", 0x008c8927, 0x00000001
	.section .rom.008c8b43, "a"
	.incbin "baserom.gba", 0x008c8b43, 0x00000001
	.section .rom.008c8e3f, "a"
	.incbin "baserom.gba", 0x008c8e3f, 0x00000001
	.section .rom.008c91e0, "a"
	.incbin "baserom.gba", 0x008c91e0, 0x000016b0
	.section .rom.008cafed, "a"
	.incbin "baserom.gba", 0x008cafed, 0x00000003
	.section .rom.008cbdbb, "a"
	.incbin "baserom.gba", 0x008cbdbb, 0x00000c75
	.section .rom.008cd025, "a"
	.incbin "baserom.gba", 0x008cd025, 0x00000003
	.section .rom.008cd57f, "a"
	.incbin "baserom.gba", 0x008cd57f, 0x00000001
	.section .rom.008cee69, "a"
	.incbin "baserom.gba", 0x008cee69, 0x000008a3
	.section .rom.008cfae7, "a"
	.incbin "baserom.gba", 0x008cfae7, 0x00000001
	.section .rom.008cfd71, "a"
	.incbin "baserom.gba", 0x008cfd71, 0x00000003
	.section .rom.008d0117, "a"
	.incbin "baserom.gba", 0x008d0117, 0x00000001
	.section .rom.008d0372, "a"
	.incbin "baserom.gba", 0x008d0372, 0x00000002
	.section .rom.008d072a, "a"
	.incbin "baserom.gba", 0x008d072a, 0x00000002
	.section .rom.008d1c13, "a"
	.incbin "baserom.gba", 0x008d1c13, 0x00000001
	.section .rom.008d31d6, "a"
	.incbin "baserom.gba", 0x008d31d6, 0x00000a6e
	.section .rom.008d4a1a, "a"
	.incbin "baserom.gba", 0x008d4a1a, 0x00000002
	.section .rom.008d4d9d, "a"
	.incbin "baserom.gba", 0x008d4d9d, 0x00000003
	.section .rom.008d5a4d, "a"
	.incbin "baserom.gba", 0x008d5a4d, 0x00000003
	.section .rom.008d6595, "a"
	.incbin "baserom.gba", 0x008d6595, 0x00000a27
	.section .rom.008d7b8a, "a"
	.incbin "baserom.gba", 0x008d7b8a, 0x00000002
	.section .rom.008d80a2, "a"
	.incbin "baserom.gba", 0x008d80a2, 0x00000626
	.section .rom.008d8ae9, "a"
	.incbin "baserom.gba", 0x008d8ae9, 0x00000003
	.section .rom.008d8d83, "a"
	.incbin "baserom.gba", 0x008d8d83, 0x00000001
	.section .rom.008da284, "a"
	.incbin "baserom.gba", 0x008da284, 0x00001e28
	.section .rom.008dc496, "a"
	.incbin "baserom.gba", 0x008dc496, 0x00000002
	.section .rom.008de40b, "a"
	.incbin "baserom.gba", 0x008de40b, 0x00000001
	.section .rom.008de8db, "a"
	.incbin "baserom.gba", 0x008de8db, 0x00001cc5
	.section .rom.008e0d75, "a"
	.incbin "baserom.gba", 0x008e0d75, 0x00000003
	.section .rom.008e1846, "a"
	.incbin "baserom.gba", 0x008e1846, 0x00000002
	.section .rom.008e2383, "a"
	.incbin "baserom.gba", 0x008e2383, 0x00001c2d
	.section .rom.008e431b, "a"
	.incbin "baserom.gba", 0x008e431b, 0x00000001
	.section .rom.008e48b9, "a"
	.incbin "baserom.gba", 0x008e48b9, 0x00000207
	.section .rom.008e4df9, "a"
	.incbin "baserom.gba", 0x008e4df9, 0x00002687
	.section .rom.008e941e, "a"
	.incbin "baserom.gba", 0x008e941e, 0x00000002
	.section .rom.008e9bc1, "a"
	.incbin "baserom.gba", 0x008e9bc1, 0x00000003
	.section .rom.008eabd8, "a"
	.incbin "baserom.gba", 0x008eabd8, 0x00001878
	.section .rom.008ec4d4, "a"
	.incbin "baserom.gba", 0x008ec4d4, 0x000004e8
	.section .rom.008ecac4, "a"
	.incbin "baserom.gba", 0x008ecac4, 0x000006b8
	.section .rom.008eda8e, "a"
	.incbin "baserom.gba", 0x008eda8e, 0x00002b86
	.section .rom.008f16b9, "a"
	.incbin "baserom.gba", 0x008f16b9, 0x00000003
	.section .rom.008f18b8, "a"
	.incbin "baserom.gba", 0x008f18b8, 0x0004ac54
	.section .rom.0093c6f9, "a"
	.incbin "baserom.gba", 0x0093c6f9, 0x0000060f
	.section .rom.0093e472, "a"
	.incbin "baserom.gba", 0x0093e472, 0x00000002
	.section .rom.0093f9a9, "a"
	.incbin "baserom.gba", 0x0093f9a9, 0x00000003
	.section .rom.009417ff, "a"
	.incbin "baserom.gba", 0x009417ff, 0x00000001
	.section .rom.00943913, "a"
	.incbin "baserom.gba", 0x00943913, 0x00000001
	.section .rom.00943aec, "a"
	.incbin "baserom.gba", 0x00943aec, 0x000005a4
	.section .rom.0094670b, "a"
	.incbin "baserom.gba", 0x0094670b, 0x00000001
	.section .rom.00947113, "a"
	.incbin "baserom.gba", 0x00947113, 0x00000001
	.section .rom.00949e32, "a"
	.incbin "baserom.gba", 0x00949e32, 0x00000002
	.section .rom.0094a004, "a"
	.incbin "baserom.gba", 0x0094a004, 0x00000468
	.section .rom.0094ba5e, "a"
	.incbin "baserom.gba", 0x0094ba5e, 0x00000002
	.section .rom.0094cece, "a"
	.incbin "baserom.gba", 0x0094cece, 0x00000002
	.section .rom.0094d7c2, "a"
	.incbin "baserom.gba", 0x0094d7c2, 0x00000002
	.section .rom.0094e0e9, "a"
	.incbin "baserom.gba", 0x0094e0e9, 0x00000003
	.section .rom.0094efda, "a"
	.incbin "baserom.gba", 0x0094efda, 0x00000002
	.section .rom.0094fbc7, "a"
	.incbin "baserom.gba", 0x0094fbc7, 0x00000001
	.section .rom.0094fda2, "a"
	.incbin "baserom.gba", 0x0094fda2, 0x000004ca
	.section .rom.00950b02, "a"
	.incbin "baserom.gba", 0x00950b02, 0x00000002
	.section .rom.00952f3d, "a"
	.incbin "baserom.gba", 0x00952f3d, 0x00000003
	.section .rom.0095350f, "a"
	.incbin "baserom.gba", 0x0095350f, 0x00000001
	.section .rom.009539af, "a"
	.incbin "baserom.gba", 0x009539af, 0x00000001
	.section .rom.00953d02, "a"
	.incbin "baserom.gba", 0x00953d02, 0x000002fa
	.section .rom.009540d5, "a"
	.incbin "baserom.gba", 0x009540d5, 0x000002f3
	.section .rom.0095445a, "a"
	.incbin "baserom.gba", 0x0095445a, 0x00000002
	.section .rom.0095500d, "a"
	.incbin "baserom.gba", 0x0095500d, 0x00000003
	.section .rom.00955cba, "a"
	.incbin "baserom.gba", 0x00955cba, 0x00000002
	.section .rom.009567be, "a"
	.incbin "baserom.gba", 0x009567be, 0x00000002
	.section .rom.009576fd, "a"
	.incbin "baserom.gba", 0x009576fd, 0x00000003
	.section .rom.00957f39, "a"
	.incbin "baserom.gba", 0x00957f39, 0x00000003
	.section .rom.009593de, "a"
	.incbin "baserom.gba", 0x009593de, 0x00000002
	.section .rom.00959e96, "a"
	.incbin "baserom.gba", 0x00959e96, 0x00000002
	.section .rom.0095a1c1, "a"
	.incbin "baserom.gba", 0x0095a1c1, 0x00000003
	.section .rom.0095af77, "a"
	.incbin "baserom.gba", 0x0095af77, 0x00000001
	.section .rom.0095b778, "a"
	.incbin "baserom.gba", 0x0095b778, 0x00000400
	.section .rom.00967f32, "a"
	.incbin "baserom.gba", 0x00967f32, 0x000009fa
	.section .rom.00968d93, "a"
	.incbin "baserom.gba", 0x00968d93, 0x00000001
	.section .rom.00968f9f, "a"
	.incbin "baserom.gba", 0x00968f9f, 0x00000001
	.section .rom.009691a3, "a"
	.incbin "baserom.gba", 0x009691a3, 0x00000001
	.section .rom.00969232, "a"
	.incbin "baserom.gba", 0x00969232, 0x00000002
	.section .rom.00969714, "a"
	.incbin "baserom.gba", 0x00969714, 0x00000070
	.section .rom.009697df, "a"
	.incbin "baserom.gba", 0x009697df, 0x00000001
	.section .rom.00969812, "a"
	.incbin "baserom.gba", 0x00969812, 0x00000002
	.section .rom.009699de, "a"
	.incbin "baserom.gba", 0x009699de, 0x00000332
	.section .rom.00969dc9, "a"
	.incbin "baserom.gba", 0x00969dc9, 0x00000003
	.section .rom.00969f9e, "a"
	.incbin "baserom.gba", 0x00969f9e, 0x000000be
	.section .rom.0096a1ae, "a"
	.incbin "baserom.gba", 0x0096a1ae, 0x00000002
	.section .rom.0096a291, "a"
	.incbin "baserom.gba", 0x0096a291, 0x00000003
	.section .rom.0096a361, "a"
	.incbin "baserom.gba", 0x0096a361, 0x00000003
	.section .rom.0096a487, "a"
	.incbin "baserom.gba", 0x0096a487, 0x00000001
	.section .rom.0096a4b6, "a"
	.incbin "baserom.gba", 0x0096a4b6, 0x00000002
	.section .rom.0096a71a, "a"
	.incbin "baserom.gba", 0x0096a71a, 0x00000002
	.section .rom.0096a7b7, "a"
	.incbin "baserom.gba", 0x0096a7b7, 0x00000001
	.section .rom.0096a81c, "a"
	.incbin "baserom.gba", 0x0096a81c, 0x00000cf4
	.section .rom.00970747, "a"
	.incbin "baserom.gba", 0x00970747, 0x00000001
	.section .rom.00973692, "a"
	.incbin "baserom.gba", 0x00973692, 0x00000002
	.section .rom.009772b5, "a"
	.incbin "baserom.gba", 0x009772b5, 0x00000003
	.section .rom.00979501, "a"
	.incbin "baserom.gba", 0x00979501, 0x00000003
	.section .rom.0097ea9d, "a"
	.incbin "baserom.gba", 0x0097ea9d, 0x00000003
	.section .rom.00982317, "a"
	.incbin "baserom.gba", 0x00982317, 0x00000001
	.section .rom.00988181, "a"
	.incbin "baserom.gba", 0x00988181, 0x00000003
	.section .rom.00989255, "a"
	.incbin "baserom.gba", 0x00989255, 0x00000003
	.section .rom.0098a2c1, "a"
	.incbin "baserom.gba", 0x0098a2c1, 0x00000003
	.section .rom.0098f91b, "a"
	.incbin "baserom.gba", 0x0098f91b, 0x00000001
	.section .rom.00992a5d, "a"
	.incbin "baserom.gba", 0x00992a5d, 0x00000003
	.section .rom.00994afd, "a"
	.incbin "baserom.gba", 0x00994afd, 0x00000003
	.section .rom.00996e1d, "a"
	.incbin "baserom.gba", 0x00996e1d, 0x00000003
	.section .rom.00999a96, "a"
	.incbin "baserom.gba", 0x00999a96, 0x00000002
	.section .rom.0099b991, "a"
	.incbin "baserom.gba", 0x0099b991, 0x00000003
	.section .rom.009a3ed5, "a"
	.incbin "baserom.gba", 0x009a3ed5, 0x00000003
	.section .rom.009acb57, "a"
	.incbin "baserom.gba", 0x009acb57, 0x00000001
	.section .rom.009af6d1, "a"
	.incbin "baserom.gba", 0x009af6d1, 0x00000003
	.section .rom.009b610e, "a"
	.incbin "baserom.gba", 0x009b610e, 0x00000002
	.section .rom.009b8c02, "a"
	.incbin "baserom.gba", 0x009b8c02, 0x00000002
	.section .rom.009ba6f2, "a"
	.incbin "baserom.gba", 0x009ba6f2, 0x00000002
	.section .rom.009bd008, "a"
	.incbin "baserom.gba", 0x009bd008, 0x00003440
	.section .rom.009c4dee, "a"
	.incbin "baserom.gba", 0x009c4dee, 0x00000002
	.section .rom.009c5d8a, "a"
	.incbin "baserom.gba", 0x009c5d8a, 0x00000002
	.section .rom.009c84b5, "a"
	.incbin "baserom.gba", 0x009c84b5, 0x00000003
	.section .rom.009c9bca, "a"
	.incbin "baserom.gba", 0x009c9bca, 0x00000002
	.section .rom.009d23e3, "a"
	.incbin "baserom.gba", 0x009d23e3, 0x00000001
	.section .rom.009d66ab, "a"
	.incbin "baserom.gba", 0x009d66ab, 0x00000001
	.section .rom.009ddee5, "a"
	.incbin "baserom.gba", 0x009ddee5, 0x00000003
	.section .rom.009df6ca, "a"
	.incbin "baserom.gba", 0x009df6ca, 0x00000002
	.section .rom.009e2461, "a"
	.incbin "baserom.gba", 0x009e2461, 0x00000003
	.section .rom.009e34ab, "a"
	.incbin "baserom.gba", 0x009e34ab, 0x00000001
	.section .rom.009eba3b, "a"
	.incbin "baserom.gba", 0x009eba3b, 0x00000001
	.section .rom.009ed835, "a"
	.incbin "baserom.gba", 0x009ed835, 0x00000003
	.section .rom.009eebfb, "a"
	.incbin "baserom.gba", 0x009eebfb, 0x00000001
	.section .rom.009f5afd, "a"
	.incbin "baserom.gba", 0x009f5afd, 0x00000003
	.section .rom.009f7dfe, "a"
	.incbin "baserom.gba", 0x009f7dfe, 0x00000002
	.section .rom.009fd04b, "a"
	.incbin "baserom.gba", 0x009fd04b, 0x00000001
	.section .rom.00a0244a, "a"
	.incbin "baserom.gba", 0x00a0244a, 0x00000002
	.section .rom.00a045fe, "a"
	.incbin "baserom.gba", 0x00a045fe, 0x00000002
	.section .rom.00a08765, "a"
	.incbin "baserom.gba", 0x00a08765, 0x00000003
	.section .rom.00a0b98a, "a"
	.incbin "baserom.gba", 0x00a0b98a, 0x00000002
	.section .rom.00a0d08e, "a"
	.incbin "baserom.gba", 0x00a0d08e, 0x00000002
	.section .rom.00a13c76, "a"
	.incbin "baserom.gba", 0x00a13c76, 0x00000002
	.section .rom.00a20256, "a"
	.incbin "baserom.gba", 0x00a20256, 0x00000002
	.section .rom.00a232ea, "a"
	.incbin "baserom.gba", 0x00a232ea, 0x00000002
	.section .rom.00a25b55, "a"
	.incbin "baserom.gba", 0x00a25b55, 0x00000003
	.section .rom.00a27646, "a"
	.incbin "baserom.gba", 0x00a27646, 0x00000002
	.section .rom.00a2845e, "a"
	.incbin "baserom.gba", 0x00a2845e, 0x00000002
	.section .rom.00a29149, "a"
	.incbin "baserom.gba", 0x00a29149, 0x00000003
	.section .rom.00a32606, "a"
	.incbin "baserom.gba", 0x00a32606, 0x00000002
	.section .rom.00a33311, "a"
	.incbin "baserom.gba", 0x00a33311, 0x00000003
	.section .rom.00a34575, "a"
	.incbin "baserom.gba", 0x00a34575, 0x00000003
	.section .rom.00a354a7, "a"
	.incbin "baserom.gba", 0x00a354a7, 0x00000001
	.section .rom.00a36117, "a"
	.incbin "baserom.gba", 0x00a36117, 0x00000001
	.section .rom.00a36d2f, "a"
	.incbin "baserom.gba", 0x00a36d2f, 0x00000001
	.section .rom.00a374a3, "a"
	.incbin "baserom.gba", 0x00a374a3, 0x00000001
	.section .rom.00a38d33, "a"
	.incbin "baserom.gba", 0x00a38d33, 0x00000001
	.section .rom.00a39701, "a"
	.incbin "baserom.gba", 0x00a39701, 0x00000003
	.section .rom.00a3a31f, "a"
	.incbin "baserom.gba", 0x00a3a31f, 0x00000001
	.section .rom.00a3c9d3, "a"
	.incbin "baserom.gba", 0x00a3c9d3, 0x00000001
	.section .rom.00a3f0e6, "a"
	.incbin "baserom.gba", 0x00a3f0e6, 0x00000002
	.section .rom.00a4403b, "a"
	.incbin "baserom.gba", 0x00a4403b, 0x00000001
	.section .rom.00a48365, "a"
	.incbin "baserom.gba", 0x00a48365, 0x00000003
	.section .rom.00a48f52, "a"
	.incbin "baserom.gba", 0x00a48f52, 0x00000002
	.section .rom.00a4db07, "a"
	.incbin "baserom.gba", 0x00a4db07, 0x00000001
	.section .rom.00a4fe71, "a"
	.incbin "baserom.gba", 0x00a4fe71, 0x00000003
	.section .rom.00a51813, "a"
	.incbin "baserom.gba", 0x00a51813, 0x00000001
	.section .rom.00a560d5, "a"
	.incbin "baserom.gba", 0x00a560d5, 0x00000003
	.section .rom.00a594e5, "a"
	.incbin "baserom.gba", 0x00a594e5, 0x00000003
	.section .rom.00a5d5ad, "a"
	.incbin "baserom.gba", 0x00a5d5ad, 0x00000003
	.section .rom.00a60c76, "a"
	.incbin "baserom.gba", 0x00a60c76, 0x00000002
	.section .rom.00a68c47, "a"
	.incbin "baserom.gba", 0x00a68c47, 0x00000001
	.section .rom.00a6ff57, "a"
	.incbin "baserom.gba", 0x00a6ff57, 0x00000001
	.section .rom.00a74ed7, "a"
	.incbin "baserom.gba", 0x00a74ed7, 0x00000001
	.section .rom.00a79959, "a"
	.incbin "baserom.gba", 0x00a79959, 0x0000051f
	.section .rom.00a7b0d5, "a"
	.incbin "baserom.gba", 0x00a7b0d5, 0x00000003
	.section .rom.00a7b2a6, "a"
	.incbin "baserom.gba", 0x00a7b2a6, 0x00000002
	.section .rom.00a7d347, "a"
	.incbin "baserom.gba", 0x00a7d347, 0x00000001
	.section .rom.00a7e2d4, "a"
	.incbin "baserom.gba", 0x00a7e2d4, 0x000022d8
	.section .rom.00a81800, "a"
	.incbin "baserom.gba", 0x00a81800, 0x0000461c
	.section .rom.00a85f22, "a"
	.incbin "baserom.gba", 0x00a85f22, 0x00000002
	.section .rom.00a8710d, "a"
	.incbin "baserom.gba", 0x00a8710d, 0x00000003
	.section .rom.00a88d95, "a"
	.incbin "baserom.gba", 0x00a88d95, 0x00000003
	.section .rom.00a892a3, "a"
	.incbin "baserom.gba", 0x00a892a3, 0x00000001
	.section .rom.00a893e3, "a"
	.incbin "baserom.gba", 0x00a893e3, 0x00000001
	.section .rom.00a8b066, "a"
	.incbin "baserom.gba", 0x00a8b066, 0x00000002
	.section .rom.00a8da3e, "a"
	.incbin "baserom.gba", 0x00a8da3e, 0x00000002
	.section .rom.00a90207, "a"
	.incbin "baserom.gba", 0x00a90207, 0x00000001
	.section .rom.00a91727, "a"
	.incbin "baserom.gba", 0x00a91727, 0x00000001
	.section .rom.00a9306b, "a"
	.incbin "baserom.gba", 0x00a9306b, 0x00000001
	.section .rom.00a94b19, "a"
	.incbin "baserom.gba", 0x00a94b19, 0x00000003
	.section .rom.00a94c8d, "a"
	.incbin "baserom.gba", 0x00a94c8d, 0x00000003
	.section .rom.00a97a71, "a"
	.incbin "baserom.gba", 0x00a97a71, 0x00000003
	.section .rom.00a9a31b, "a"
	.incbin "baserom.gba", 0x00a9a31b, 0x00000001
	.section .rom.00a9e7b2, "a"
	.incbin "baserom.gba", 0x00a9e7b2, 0x00000002
	.section .rom.00aa1b2a, "a"
	.incbin "baserom.gba", 0x00aa1b2a, 0x00000002
	.section .rom.00aa4627, "a"
	.incbin "baserom.gba", 0x00aa4627, 0x00000001
	.section .rom.00aa6705, "a"
	.incbin "baserom.gba", 0x00aa6705, 0x00000003
	.section .rom.00aa8951, "a"
	.incbin "baserom.gba", 0x00aa8951, 0x00000003
	.section .rom.00aa8a93, "a"
	.incbin "baserom.gba", 0x00aa8a93, 0x00000001
	.section .rom.00aaa166, "a"
	.incbin "baserom.gba", 0x00aaa166, 0x00000002
	.section .rom.00aaa266, "a"
	.incbin "baserom.gba", 0x00aaa266, 0x00000002
	.section .rom.00aac159, "a"
	.incbin "baserom.gba", 0x00aac159, 0x00000003
	.section .rom.00aadb31, "a"
	.incbin "baserom.gba", 0x00aadb31, 0x00000003
	.section .rom.00ab0562, "a"
	.incbin "baserom.gba", 0x00ab0562, 0x00000002
	.section .rom.00ab57b3, "a"
	.incbin "baserom.gba", 0x00ab57b3, 0x00000001
	.section .rom.00ab80f7, "a"
	.incbin "baserom.gba", 0x00ab80f7, 0x00000001
	.section .rom.00ab95ca, "a"
	.incbin "baserom.gba", 0x00ab95ca, 0x00000002
	.section .rom.00abe395, "a"
	.incbin "baserom.gba", 0x00abe395, 0x00000003
	.section .rom.00ac103f, "a"
	.incbin "baserom.gba", 0x00ac103f, 0x00000001
	.section .rom.00ac4277, "a"
	.incbin "baserom.gba", 0x00ac4277, 0x00000001
	.section .rom.00ac440f, "a"
	.incbin "baserom.gba", 0x00ac440f, 0x00000001
	.section .rom.00ac720f, "a"
	.incbin "baserom.gba", 0x00ac720f, 0x00000001
	.section .rom.00ac89c3, "a"
	.incbin "baserom.gba", 0x00ac89c3, 0x00000001
	.section .rom.00ac995e, "a"
	.incbin "baserom.gba", 0x00ac995e, 0x00000002
	.section .rom.00acbfbf, "a"
	.incbin "baserom.gba", 0x00acbfbf, 0x00000001
	.section .rom.00acc166, "a"
	.incbin "baserom.gba", 0x00acc166, 0x00000002
	.section .rom.00acf11f, "a"
	.incbin "baserom.gba", 0x00acf11f, 0x00000001
	.section .rom.00acfdc5, "a"
	.incbin "baserom.gba", 0x00acfdc5, 0x00000003
	.section .rom.00ad1395, "a"
	.incbin "baserom.gba", 0x00ad1395, 0x00000003
	.section .rom.00ad9361, "a"
	.incbin "baserom.gba", 0x00ad9361, 0x00000003
	.section .rom.00adae47, "a"
	.incbin "baserom.gba", 0x00adae47, 0x00000001
	.section .rom.00adc331, "a"
	.incbin "baserom.gba", 0x00adc331, 0x00000003
	.section .rom.00ae255b, "a"
	.incbin "baserom.gba", 0x00ae255b, 0x00000001
	.section .rom.00ae2a2d, "a"
	.incbin "baserom.gba", 0x00ae2a2d, 0x00000003
	.section .rom.00ae3b75, "a"
	.incbin "baserom.gba", 0x00ae3b75, 0x00000003
	.section .rom.00ae3d26, "a"
	.incbin "baserom.gba", 0x00ae3d26, 0x00000002
	.section .rom.00aefa13, "a"
	.incbin "baserom.gba", 0x00aefa13, 0x00000001
	.section .rom.00aefb33, "a"
	.incbin "baserom.gba", 0x00aefb33, 0x00000001
	.section .rom.00af1ed3, "a"
	.incbin "baserom.gba", 0x00af1ed3, 0x00000001
	.section .rom.00af311f, "a"
	.incbin "baserom.gba", 0x00af311f, 0x00000001
	.section .rom.00af54af, "a"
	.incbin "baserom.gba", 0x00af54af, 0x00000001
	.section .rom.00af6dad, "a"
	.incbin "baserom.gba", 0x00af6dad, 0x00000003
	.section .rom.00af7d9e, "a"
	.incbin "baserom.gba", 0x00af7d9e, 0x00000002
	.section .rom.00afa33a, "a"
	.incbin "baserom.gba", 0x00afa33a, 0x00000002
	.section .rom.00afe962, "a"
	.incbin "baserom.gba", 0x00afe962, 0x00000002
	.section .rom.00aff027, "a"
	.incbin "baserom.gba", 0x00aff027, 0x00000001
	.section .rom.00b01abd, "a"
	.incbin "baserom.gba", 0x00b01abd, 0x00000003
	.section .rom.00b01c0e, "a"
	.incbin "baserom.gba", 0x00b01c0e, 0x00000002
	.section .rom.00b0401e, "a"
	.incbin "baserom.gba", 0x00b0401e, 0x00000002
	.section .rom.00b0619f, "a"
	.incbin "baserom.gba", 0x00b0619f, 0x00000001
	.section .rom.00b062df, "a"
	.incbin "baserom.gba", 0x00b062df, 0x00000001
	.section .rom.00b08d7e, "a"
	.incbin "baserom.gba", 0x00b08d7e, 0x00000002
	.section .rom.00b08ecf, "a"
	.incbin "baserom.gba", 0x00b08ecf, 0x00000001
	.section .rom.00b0b2de, "a"
	.incbin "baserom.gba", 0x00b0b2de, 0x00000002
	.section .rom.00b0d45f, "a"
	.incbin "baserom.gba", 0x00b0d45f, 0x00000001
	.section .rom.00b0d59f, "a"
	.incbin "baserom.gba", 0x00b0d59f, 0x00000001
	.section .rom.00b10bce, "a"
	.incbin "baserom.gba", 0x00b10bce, 0x00000002
	.section .rom.00b13132, "a"
	.incbin "baserom.gba", 0x00b13132, 0x00000002
	.section .rom.00b152b3, "a"
	.incbin "baserom.gba", 0x00b152b3, 0x00000001
	.section .rom.00b153f3, "a"
	.incbin "baserom.gba", 0x00b153f3, 0x00000001
	.section .rom.00b168a3, "a"
	.incbin "baserom.gba", 0x00b168a3, 0x00000001
	.section .rom.00b169ae, "a"
	.incbin "baserom.gba", 0x00b169ae, 0x00000002
	.section .rom.00b18506, "a"
	.incbin "baserom.gba", 0x00b18506, 0x00000002
	.section .rom.00b19ba9, "a"
	.incbin "baserom.gba", 0x00b19ba9, 0x00000003
	.section .rom.00b19d6a, "a"
	.incbin "baserom.gba", 0x00b19d6a, 0x00000d9e
	.section .rom.00b1c75e, "a"
	.incbin "baserom.gba", 0x00b1c75e, 0x00000002
	.section .rom.00b1dfb5, "a"
	.incbin "baserom.gba", 0x00b1dfb5, 0x00000003
	.section .rom.00b1e176, "a"
	.incbin "baserom.gba", 0x00b1e176, 0x00000002
	.section .rom.00b223f1, "a"
	.incbin "baserom.gba", 0x00b223f1, 0x00000003
	.section .rom.00b225b2, "a"
	.incbin "baserom.gba", 0x00b225b2, 0x00000002
	.section .rom.00b23029, "a"
	.incbin "baserom.gba", 0x00b23029, 0x00000003
	.section .rom.00b23131, "a"
	.incbin "baserom.gba", 0x00b23131, 0x00000003
	.section .rom.00b24df2, "a"
	.incbin "baserom.gba", 0x00b24df2, 0x00000002
	.section .rom.00b2657f, "a"
	.incbin "baserom.gba", 0x00b2657f, 0x00000001
	.section .rom.00b26835, "a"
	.incbin "baserom.gba", 0x00b26835, 0x00000003
	.section .rom.00b28345, "a"
	.incbin "baserom.gba", 0x00b28345, 0x00000003
	.section .rom.00b2abd2, "a"
	.incbin "baserom.gba", 0x00b2abd2, 0x00000002
	.section .rom.00b2d3c2, "a"
	.incbin "baserom.gba", 0x00b2d3c2, 0x00000002
	.section .rom.00b2e362, "a"
	.incbin "baserom.gba", 0x00b2e362, 0x00000002
	.section .rom.00b304b5, "a"
	.incbin "baserom.gba", 0x00b304b5, 0x00000003
	.section .rom.00b33b42, "a"
	.incbin "baserom.gba", 0x00b33b42, 0x00000002
	.section .rom.00b358d7, "a"
	.incbin "baserom.gba", 0x00b358d7, 0x00000001
	.section .rom.00b376f5, "a"
	.incbin "baserom.gba", 0x00b376f5, 0x00000003
	.section .rom.00b391d1, "a"
	.incbin "baserom.gba", 0x00b391d1, 0x00000003
	.section .rom.00b3a7ba, "a"
	.incbin "baserom.gba", 0x00b3a7ba, 0x00000002
	.section .rom.00b3cda2, "a"
	.incbin "baserom.gba", 0x00b3cda2, 0x00000002
	.section .rom.00b3cf1b, "a"
	.incbin "baserom.gba", 0x00b3cf1b, 0x00000001
	.section .rom.00b3e3da, "a"
	.incbin "baserom.gba", 0x00b3e3da, 0x00000002
	.section .rom.00b3fbde, "a"
	.incbin "baserom.gba", 0x00b3fbde, 0x00000002
	.section .rom.00b41556, "a"
	.incbin "baserom.gba", 0x00b41556, 0x00000002
	.section .rom.00b42476, "a"
	.incbin "baserom.gba", 0x00b42476, 0x00000002
	.section .rom.00b43f22, "a"
	.incbin "baserom.gba", 0x00b43f22, 0x00000002
	.section .rom.00b4402f, "a"
	.incbin "baserom.gba", 0x00b4402f, 0x00000001
	.section .rom.00b4615f, "a"
	.incbin "baserom.gba", 0x00b4615f, 0x00000001
	.section .rom.00b47d35, "a"
	.incbin "baserom.gba", 0x00b47d35, 0x00000003
	.section .rom.00b48347, "a"
	.incbin "baserom.gba", 0x00b48347, 0x00000001
	.section .rom.00b48487, "a"
	.incbin "baserom.gba", 0x00b48487, 0x00000001
	.section .rom.00b4ab87, "a"
	.incbin "baserom.gba", 0x00b4ab87, 0x00000001
	.section .rom.00b4c3e6, "a"
	.incbin "baserom.gba", 0x00b4c3e6, 0x00000002
	.section .rom.00b4c9f7, "a"
	.incbin "baserom.gba", 0x00b4c9f7, 0x00000001
	.section .rom.00b4cb37, "a"
	.incbin "baserom.gba", 0x00b4cb37, 0x00000001
	.section .rom.00b4d9b2, "a"
	.incbin "baserom.gba", 0x00b4d9b2, 0x00000002
	.section .rom.00b4dac9, "a"
	.incbin "baserom.gba", 0x00b4dac9, 0x00000003
	.section .rom.00b4fd9f, "a"
	.incbin "baserom.gba", 0x00b4fd9f, 0x00000001
	.section .rom.00b519f2, "a"
	.incbin "baserom.gba", 0x00b519f2, 0x00000002
	.section .rom.00b52037, "a"
	.incbin "baserom.gba", 0x00b52037, 0x00000001
	.section .rom.00b52177, "a"
	.incbin "baserom.gba", 0x00b52177, 0x00000001
	.section .rom.00b52d7d, "a"
	.incbin "baserom.gba", 0x00b52d7d, 0x00000003
	.section .rom.00b5532f, "a"
	.incbin "baserom.gba", 0x00b5532f, 0x00000001
	.section .rom.00b56fee, "a"
	.incbin "baserom.gba", 0x00b56fee, 0x00000002
	.section .rom.00b58aba, "a"
	.incbin "baserom.gba", 0x00b58aba, 0x00000002
	.section .rom.00b59b8e, "a"
	.incbin "baserom.gba", 0x00b59b8e, 0x00000002
	.section .rom.00b59cf5, "a"
	.incbin "baserom.gba", 0x00b59cf5, 0x00000003
	.section .rom.00b5af0e, "a"
	.incbin "baserom.gba", 0x00b5af0e, 0x00000002
	.section .rom.00b5baed, "a"
	.incbin "baserom.gba", 0x00b5baed, 0x00000003
	.section .rom.00b5bc5a, "a"
	.incbin "baserom.gba", 0x00b5bc5a, 0x00000002
	.section .rom.00b5eaab, "a"
	.incbin "baserom.gba", 0x00b5eaab, 0x00000001
	.section .rom.00b6126a, "a"
	.incbin "baserom.gba", 0x00b6126a, 0x00000002
	.section .rom.00b6731b, "a"
	.incbin "baserom.gba", 0x00b6731b, 0x00000001
	.section .rom.00b68e4b, "a"
	.incbin "baserom.gba", 0x00b68e4b, 0x00000001
	.section .rom.00b6b182, "a"
	.incbin "baserom.gba", 0x00b6b182, 0x00000002
	.section .rom.00b6c333, "a"
	.incbin "baserom.gba", 0x00b6c333, 0x00000001
	.section .rom.00b6dc55, "a"
	.incbin "baserom.gba", 0x00b6dc55, 0x00000003
	.section .rom.00b6fae1, "a"
	.incbin "baserom.gba", 0x00b6fae1, 0x00000003
	.section .rom.00b71cc2, "a"
	.incbin "baserom.gba", 0x00b71cc2, 0x00000002
	.section .rom.00b75689, "a"
	.incbin "baserom.gba", 0x00b75689, 0x00000003
	.section .rom.00b75775, "a"
	.incbin "baserom.gba", 0x00b75775, 0x00000003
	.section .rom.00b789ae, "a"
	.incbin "baserom.gba", 0x00b789ae, 0x00000002
	.section .rom.00b79025, "a"
	.incbin "baserom.gba", 0x00b79025, 0x00000003
	.section .rom.00b7cde7, "a"
	.incbin "baserom.gba", 0x00b7cde7, 0x00000001
	.section .rom.00b7f079, "a"
	.incbin "baserom.gba", 0x00b7f079, 0x00000003
	.section .rom.00b80a96, "a"
	.incbin "baserom.gba", 0x00b80a96, 0x00000002
	.section .rom.00b81ba3, "a"
	.incbin "baserom.gba", 0x00b81ba3, 0x00000001
	.section .rom.00b84d42, "a"
	.incbin "baserom.gba", 0x00b84d42, 0x00000002
	.section .rom.00b85f3a, "a"
	.incbin "baserom.gba", 0x00b85f3a, 0x00000002
	.section .rom.00b86dfd, "a"
	.incbin "baserom.gba", 0x00b86dfd, 0x00000003
	.section .rom.00b88e9e, "a"
	.incbin "baserom.gba", 0x00b88e9e, 0x00000002
	.section .rom.00b8a8f3, "a"
	.incbin "baserom.gba", 0x00b8a8f3, 0x00000001
	.section .rom.00b8bdbe, "a"
	.incbin "baserom.gba", 0x00b8bdbe, 0x00000002
	.section .rom.00b8dfc2, "a"
	.incbin "baserom.gba", 0x00b8dfc2, 0x00000002
	.section .rom.00b8e0b7, "a"
	.incbin "baserom.gba", 0x00b8e0b7, 0x00000001
	.section .rom.00b8f68a, "a"
	.incbin "baserom.gba", 0x00b8f68a, 0x00000002
	.section .rom.00b8f8a9, "a"
	.incbin "baserom.gba", 0x00b8f8a9, 0x00000003
	.section .rom.00b9198d, "a"
	.incbin "baserom.gba", 0x00b9198d, 0x00000003
	.section .rom.00b91b52, "a"
	.incbin "baserom.gba", 0x00b91b52, 0x00000002
	.section .rom.00b93c15, "a"
	.incbin "baserom.gba", 0x00b93c15, 0x00000003
	.section .rom.00b93d45, "a"
	.incbin "baserom.gba", 0x00b93d45, 0x00000003
	.section .rom.00b967fe, "a"
	.incbin "baserom.gba", 0x00b967fe, 0x00000002
	.section .rom.00b98356, "a"
	.incbin "baserom.gba", 0x00b98356, 0x00000002
	.section .rom.00b9929d, "a"
	.incbin "baserom.gba", 0x00b9929d, 0x00000003
	.section .rom.00b9aca6, "a"
	.incbin "baserom.gba", 0x00b9aca6, 0x00000002
	.section .rom.00baa9fb, "a"
	.incbin "baserom.gba", 0x00baa9fb, 0x00000001
	.section .rom.00baab4b, "a"
	.incbin "baserom.gba", 0x00baab4b, 0x00000001
	.section .rom.00bad656, "a"
	.incbin "baserom.gba", 0x00bad656, 0x00000002
	.section .rom.00baf25f, "a"
	.incbin "baserom.gba", 0x00baf25f, 0x00000001
	.section .rom.00bb0c66, "a"
	.incbin "baserom.gba", 0x00bb0c66, 0x00000002
	.section .rom.00bb2981, "a"
	.incbin "baserom.gba", 0x00bb2981, 0x00000003
	.section .rom.00bb2ad3, "a"
	.incbin "baserom.gba", 0x00bb2ad3, 0x00000001
	.section .rom.00bb44da, "a"
	.incbin "baserom.gba", 0x00bb44da, 0x00000002
	.section .rom.00bb8262, "a"
	.incbin "baserom.gba", 0x00bb8262, 0x00000002
	.section .rom.00bb83f5, "a"
	.incbin "baserom.gba", 0x00bb83f5, 0x00000003
	.section .rom.00bbd3ff, "a"
	.incbin "baserom.gba", 0x00bbd3ff, 0x00000001
	.section .rom.00bbfa81, "a"
	.incbin "baserom.gba", 0x00bbfa81, 0x00000003
	.section .rom.00bbfdb7, "a"
	.incbin "baserom.gba", 0x00bbfdb7, 0x00000001
	.section .rom.00bc620d, "a"
	.incbin "baserom.gba", 0x00bc620d, 0x00000003
	.section .rom.00bc635a, "a"
	.incbin "baserom.gba", 0x00bc635a, 0x00000002
	.section .rom.00bc81df, "a"
	.incbin "baserom.gba", 0x00bc81df, 0x00000001
	.section .rom.00bc9786, "a"
	.incbin "baserom.gba", 0x00bc9786, 0x00000002
	.section .rom.00bcb731, "a"
	.incbin "baserom.gba", 0x00bcb731, 0x00000003
	.section .rom.00bcc6a2, "a"
	.incbin "baserom.gba", 0x00bcc6a2, 0x00000002
	.section .rom.00bd7cab, "a"
	.incbin "baserom.gba", 0x00bd7cab, 0x00000001
	.section .rom.00bd7d67, "a"
	.incbin "baserom.gba", 0x00bd7d67, 0x00000001
	.section .rom.00bd8aa6, "a"
	.incbin "baserom.gba", 0x00bd8aa6, 0x00000002
	.section .rom.00bd9483, "a"
	.incbin "baserom.gba", 0x00bd9483, 0x00000001
	.section .rom.00bda09b, "a"
	.incbin "baserom.gba", 0x00bda09b, 0x00000001
	.section .rom.00bdbf56, "a"
	.incbin "baserom.gba", 0x00bdbf56, 0x00000002
	.section .rom.00bdc072, "a"
	.incbin "baserom.gba", 0x00bdc072, 0x00000002
	.section .rom.00bde8f3, "a"
	.incbin "baserom.gba", 0x00bde8f3, 0x00000001
	.section .rom.00be007e, "a"
	.incbin "baserom.gba", 0x00be007e, 0x00000002
	.section .rom.00be3fae, "a"
	.incbin "baserom.gba", 0x00be3fae, 0x00000002
	.section .rom.00be40f7, "a"
	.incbin "baserom.gba", 0x00be40f7, 0x00000001
	.section .rom.00be67a7, "a"
	.incbin "baserom.gba", 0x00be67a7, 0x00000001
	.section .rom.00be8f4a, "a"
	.incbin "baserom.gba", 0x00be8f4a, 0x00000002
	.section .rom.00be9cd5, "a"
	.incbin "baserom.gba", 0x00be9cd5, 0x00000003
	.section .rom.00becb16, "a"
	.incbin "baserom.gba", 0x00becb16, 0x00000002
	.section .rom.00bef13d, "a"
	.incbin "baserom.gba", 0x00bef13d, 0x00000003
	.section .rom.00bf0e29, "a"
	.incbin "baserom.gba", 0x00bf0e29, 0x00000003
	.section .rom.00bf2529, "a"
	.incbin "baserom.gba", 0x00bf2529, 0x00000003
	.section .rom.00bf3aa5, "a"
	.incbin "baserom.gba", 0x00bf3aa5, 0x00000003
	.section .rom.00bf5217, "a"
	.incbin "baserom.gba", 0x00bf5217, 0x00000001
	.section .rom.00bf7ea2, "a"
	.incbin "baserom.gba", 0x00bf7ea2, 0x00000002
	.section .rom.00bf8eee, "a"
	.incbin "baserom.gba", 0x00bf8eee, 0x00000002
	.section .rom.00bf9feb, "a"
	.incbin "baserom.gba", 0x00bf9feb, 0x00000001
	.section .rom.00bfb7bf, "a"
	.incbin "baserom.gba", 0x00bfb7bf, 0x00000001
	.section .rom.00bfcc83, "a"
	.incbin "baserom.gba", 0x00bfcc83, 0x00000001
	.section .rom.00bff1b6, "a"
	.incbin "baserom.gba", 0x00bff1b6, 0x00000002
	.section .rom.00bff2ea, "a"
	.incbin "baserom.gba", 0x00bff2ea, 0x00000002
	.section .rom.00c01a56, "a"
	.incbin "baserom.gba", 0x00c01a56, 0x00000002
	.section .rom.00c03829, "a"
	.incbin "baserom.gba", 0x00c03829, 0x00000003
	.section .rom.00c0710d, "a"
	.incbin "baserom.gba", 0x00c0710d, 0x00000003
	.section .rom.00c09873, "a"
	.incbin "baserom.gba", 0x00c09873, 0x00000001
	.section .rom.00c0b1dd, "a"
	.incbin "baserom.gba", 0x00c0b1dd, 0x00000003
	.section .rom.00c0da06, "a"
	.incbin "baserom.gba", 0x00c0da06, 0x00000002
	.section .rom.00c11de5, "a"
	.incbin "baserom.gba", 0x00c11de5, 0x00000003
	.section .rom.00c1518f, "a"
	.incbin "baserom.gba", 0x00c1518f, 0x00000001
	.section .rom.00c15f82, "a"
	.incbin "baserom.gba", 0x00c15f82, 0x00000002
	.section .rom.00c16cfa, "a"
	.incbin "baserom.gba", 0x00c16cfa, 0x00000002
	.section .rom.00c1ee22, "a"
	.incbin "baserom.gba", 0x00c1ee22, 0x00000002
	.section .rom.00c251a1, "a"
	.incbin "baserom.gba", 0x00c251a1, 0x00000003
	.section .rom.00c25ba7, "a"
	.incbin "baserom.gba", 0x00c25ba7, 0x00000001
	.section .rom.00c27655, "a"
	.incbin "baserom.gba", 0x00c27655, 0x00001493
	.section .rom.00c28f4f, "a"
	.incbin "baserom.gba", 0x00c28f4f, 0x00000001
	.section .rom.00c2910e, "a"
	.incbin "baserom.gba", 0x00c2910e, 0x00000002
	.section .rom.00c2b916, "a"
	.incbin "baserom.gba", 0x00c2b916, 0x00000002
	.section .rom.00c2ba6e, "a"
	.incbin "baserom.gba", 0x00c2ba6e, 0x00000002
	.section .rom.00c2e3b5, "a"
	.incbin "baserom.gba", 0x00c2e3b5, 0x00000003
	.section .rom.00c305d9, "a"
	.incbin "baserom.gba", 0x00c305d9, 0x00000003
	.section .rom.00c31ace, "a"
	.incbin "baserom.gba", 0x00c31ace, 0x00000002
	.section .rom.00c33d31, "a"
	.incbin "baserom.gba", 0x00c33d31, 0x00000003
	.section .rom.00c365df, "a"
	.incbin "baserom.gba", 0x00c365df, 0x00000001
	.section .rom.00c38572, "a"
	.incbin "baserom.gba", 0x00c38572, 0x00000002
	.section .rom.00c39593, "a"
	.incbin "baserom.gba", 0x00c39593, 0x00000001
	.section .rom.00c396d3, "a"
	.incbin "baserom.gba", 0x00c396d3, 0x00000001
	.section .rom.00c3c2f2, "a"
	.incbin "baserom.gba", 0x00c3c2f2, 0x00000002
	.section .rom.00c3e776, "a"
	.incbin "baserom.gba", 0x00c3e776, 0x00000002
	.section .rom.00c4085d, "a"
	.incbin "baserom.gba", 0x00c4085d, 0x00000003
	.section .rom.00c420c1, "a"
	.incbin "baserom.gba", 0x00c420c1, 0x00000003
	.section .rom.00c44627, "a"
	.incbin "baserom.gba", 0x00c44627, 0x00000001
	.section .rom.00c4478d, "a"
	.incbin "baserom.gba", 0x00c4478d, 0x00000003
	.section .rom.00c46f87, "a"
	.incbin "baserom.gba", 0x00c46f87, 0x00000001
	.section .rom.00c48fbb, "a"
	.incbin "baserom.gba", 0x00c48fbb, 0x00000001
	.section .rom.00c4abe2, "a"
	.incbin "baserom.gba", 0x00c4abe2, 0x00000002
	.section .rom.00c4d602, "a"
	.incbin "baserom.gba", 0x00c4d602, 0x00000002
	.section .rom.00c4e16d, "a"
	.incbin "baserom.gba", 0x00c4e16d, 0x00000003
	.section .rom.00c4e253, "a"
	.incbin "baserom.gba", 0x00c4e253, 0x00000001
	.section .rom.00c4fb9d, "a"
	.incbin "baserom.gba", 0x00c4fb9d, 0x00000003
	.section .rom.00c51503, "a"
	.incbin "baserom.gba", 0x00c51503, 0x00000001
	.section .rom.00c528d1, "a"
	.incbin "baserom.gba", 0x00c528d1, 0x00000003
	.section .rom.00c529b7, "a"
	.incbin "baserom.gba", 0x00c529b7, 0x00000001
	.section .rom.00c53525, "a"
	.incbin "baserom.gba", 0x00c53525, 0x00000003
	.section .rom.00c5360b, "a"
	.incbin "baserom.gba", 0x00c5360b, 0x00000001
	.section .rom.00c5436f, "a"
	.incbin "baserom.gba", 0x00c5436f, 0x00000001
	.section .rom.00c54453, "a"
	.incbin "baserom.gba", 0x00c54453, 0x00000001
	.section .rom.00c54c59, "a"
	.incbin "baserom.gba", 0x00c54c59, 0x00000003
	.section .rom.00c54d3f, "a"
	.incbin "baserom.gba", 0x00c54d3f, 0x00000001
	.section .rom.00c5589f, "a"
	.incbin "baserom.gba", 0x00c5589f, 0x00000001
	.section .rom.00c5611d, "a"
	.incbin "baserom.gba", 0x00c5611d, 0x00000003
	.section .rom.00c5621a, "a"
	.incbin "baserom.gba", 0x00c5621a, 0x00000002
	.section .rom.00c58073, "a"
	.incbin "baserom.gba", 0x00c58073, 0x00000001
	.section .rom.00c58e81, "a"
	.incbin "baserom.gba", 0x00c58e81, 0x00000003
	.section .rom.00c5a115, "a"
	.incbin "baserom.gba", 0x00c5a115, 0x00000003
	.section .rom.00c5b0f1, "a"
	.incbin "baserom.gba", 0x00c5b0f1, 0x00000003
	.section .rom.00c5d4f7, "a"
	.incbin "baserom.gba", 0x00c5d4f7, 0x00000001
	.section .rom.00c5d637, "a"
	.incbin "baserom.gba", 0x00c5d637, 0x00000001
	.section .rom.00c5dd05, "a"
	.incbin "baserom.gba", 0x00c5dd05, 0x00000003
	.section .rom.00c5dddb, "a"
	.incbin "baserom.gba", 0x00c5dddb, 0x00000001
	.section .rom.00c5e651, "a"
	.incbin "baserom.gba", 0x00c5e651, 0x00000003
	.section .rom.00c5eb02, "a"
	.incbin "baserom.gba", 0x00c5eb02, 0x00000002
	.section .rom.00c5ff7d, "a"
	.incbin "baserom.gba", 0x00c5ff7d, 0x00000003
	.section .rom.00c60793, "a"
	.incbin "baserom.gba", 0x00c60793, 0x00000001
	.section .rom.00c61429, "a"
	.incbin "baserom.gba", 0x00c61429, 0x00000003
	.section .rom.00c615ce, "a"
	.incbin "baserom.gba", 0x00c615ce, 0x00000002
	.section .rom.00c64492, "a"
	.incbin "baserom.gba", 0x00c64492, 0x00000002
	.section .rom.00c65e11, "a"
	.incbin "baserom.gba", 0x00c65e11, 0x00000003
	.section .rom.00c66e5e, "a"
	.incbin "baserom.gba", 0x00c66e5e, 0x00000002
	.section .rom.00c6e5a3, "a"
	.incbin "baserom.gba", 0x00c6e5a3, 0x00000001
	.section .rom.00c75d97, "a"
	.incbin "baserom.gba", 0x00c75d97, 0x00000001
	.section .rom.00c7c391, "a"
	.incbin "baserom.gba", 0x00c7c391, 0x00000003
	.section .rom.00c7edca, "a"
	.incbin "baserom.gba", 0x00c7edca, 0x00000002
	.section .rom.00c8169b, "a"
	.incbin "baserom.gba", 0x00c8169b, 0x00000001
	.section .rom.00c85ee7, "a"
	.incbin "baserom.gba", 0x00c85ee7, 0x00000001
	.section .rom.00c86ee5, "a"
	.incbin "baserom.gba", 0x00c86ee5, 0x00000003
	.section .rom.00c86ffe, "a"
	.incbin "baserom.gba", 0x00c86ffe, 0x00000002
	.section .rom.00c885a6, "a"
	.incbin "baserom.gba", 0x00c885a6, 0x00000002
	.section .rom.00c8a003, "a"
	.incbin "baserom.gba", 0x00c8a003, 0x00000001
	.section .rom.00c8a13e, "a"
	.incbin "baserom.gba", 0x00c8a13e, 0x00000002
	.section .rom.00c8b645, "a"
	.incbin "baserom.gba", 0x00c8b645, 0x00000003
	.section .rom.00c8e8ce, "a"
	.incbin "baserom.gba", 0x00c8e8ce, 0x00000002
	.section .rom.00c8fd15, "a"
	.incbin "baserom.gba", 0x00c8fd15, 0x00000003
	.section .rom.00c92a0d, "a"
	.incbin "baserom.gba", 0x00c92a0d, 0x00000003
	.section .rom.00c94e43, "a"
	.incbin "baserom.gba", 0x00c94e43, 0x00000001
	.section .rom.00c9501f, "a"
	.incbin "baserom.gba", 0x00c9501f, 0x00000001
	.section .rom.00c98133, "a"
	.incbin "baserom.gba", 0x00c98133, 0x00000001
	.section .rom.00c995b6, "a"
	.incbin "baserom.gba", 0x00c995b6, 0x00000002
	.section .rom.00c9a605, "a"
	.incbin "baserom.gba", 0x00c9a605, 0x00000003
	.section .rom.00c9c46a, "a"
	.incbin "baserom.gba", 0x00c9c46a, 0x00000002
	.section .rom.00c9c605, "a"
	.incbin "baserom.gba", 0x00c9c605, 0x00000003
	.section .rom.00c9c763, "a"
	.incbin "baserom.gba", 0x00c9c763, 0x00000001
	.section .rom.00c9d6e2, "a"
	.incbin "baserom.gba", 0x00c9d6e2, 0x00000002
	.section .rom.00c9d7fa, "a"
	.incbin "baserom.gba", 0x00c9d7fa, 0x00000002
	.section .rom.00c9f88d, "a"
	.incbin "baserom.gba", 0x00c9f88d, 0x00000003
	.section .rom.00ca170f, "a"
	.incbin "baserom.gba", 0x00ca170f, 0x00000001
	.section .rom.00ca3c35, "a"
	.incbin "baserom.gba", 0x00ca3c35, 0x00000003
	.section .rom.00ca3d42, "a"
	.incbin "baserom.gba", 0x00ca3d42, 0x00000002
	.section .rom.00ca5dd5, "a"
	.incbin "baserom.gba", 0x00ca5dd5, 0x00000003
	.section .rom.00ca92ab, "a"
	.incbin "baserom.gba", 0x00ca92ab, 0x00000001
	.section .rom.00ca9d95, "a"
	.incbin "baserom.gba", 0x00ca9d95, 0x00000003
	.section .rom.00ca9f05, "a"
	.incbin "baserom.gba", 0x00ca9f05, 0x00000003
	.section .rom.00cace1f, "a"
	.incbin "baserom.gba", 0x00cace1f, 0x00000001
	.section .rom.00caedaa, "a"
	.incbin "baserom.gba", 0x00caedaa, 0x00000002
	.section .rom.00cb0715, "a"
	.incbin "baserom.gba", 0x00cb0715, 0x00000003
	.section .rom.00cb6202, "a"
	.incbin "baserom.gba", 0x00cb6202, 0x00000002
	.section .rom.00cb63b2, "a"
	.incbin "baserom.gba", 0x00cb63b2, 0x00000002
	.section .rom.00cbad2a, "a"
	.incbin "baserom.gba", 0x00cbad2a, 0x00000002
	.section .rom.00cbae82, "a"
	.incbin "baserom.gba", 0x00cbae82, 0x00000002
	.section .rom.00cbe44f, "a"
	.incbin "baserom.gba", 0x00cbe44f, 0x00000001
	.section .rom.00cbfcd1, "a"
	.incbin "baserom.gba", 0x00cbfcd1, 0x00000003
	.section .rom.00cc19f9, "a"
	.incbin "baserom.gba", 0x00cc19f9, 0x00000003
	.section .rom.00cc511a, "a"
	.incbin "baserom.gba", 0x00cc511a, 0x00000002
	.section .rom.00cca2a3, "a"
	.incbin "baserom.gba", 0x00cca2a3, 0x00000001
	.section .rom.00ccc871, "a"
	.incbin "baserom.gba", 0x00ccc871, 0x00000003
	.section .rom.00cce1fd, "a"
	.incbin "baserom.gba", 0x00cce1fd, 0x00000003
	.section .rom.00ccf32a, "a"
	.incbin "baserom.gba", 0x00ccf32a, 0x00000002
	.section .rom.00ccfeda, "a"
	.incbin "baserom.gba", 0x00ccfeda, 0x00000002
	.section .rom.00cd18a2, "a"
	.incbin "baserom.gba", 0x00cd18a2, 0x00000002
	.section .rom.00cd1986, "a"
	.incbin "baserom.gba", 0x00cd1986, 0x00000002
	.section .rom.00cd3de3, "a"
	.incbin "baserom.gba", 0x00cd3de3, 0x00000001
	.section .rom.00cd3f23, "a"
	.incbin "baserom.gba", 0x00cd3f23, 0x00000001
	.section .rom.00cd7599, "a"
	.incbin "baserom.gba", 0x00cd7599, 0x00000003
	.section .rom.00cd76fd, "a"
	.incbin "baserom.gba", 0x00cd76fd, 0x00000003
	.section .rom.00cd9ddd, "a"
	.incbin "baserom.gba", 0x00cd9ddd, 0x00000003
	.section .rom.00cdcc8d, "a"
	.incbin "baserom.gba", 0x00cdcc8d, 0x00000003
	.section .rom.00cde202, "a"
	.incbin "baserom.gba", 0x00cde202, 0x00000002
	.section .rom.00ce22fe, "a"
	.incbin "baserom.gba", 0x00ce22fe, 0x00000002
	.section .rom.00ce4583, "a"
	.incbin "baserom.gba", 0x00ce4583, 0x00000001
	.section .rom.00ce6167, "a"
	.incbin "baserom.gba", 0x00ce6167, 0x00000001
	.section .rom.00ce7e5d, "a"
	.incbin "baserom.gba", 0x00ce7e5d, 0x00000003
	.section .rom.00cf5481, "a"
	.incbin "baserom.gba", 0x00cf5481, 0x00000003
	.section .rom.00cf55a9, "a"
	.incbin "baserom.gba", 0x00cf55a9, 0x00000003
	.section .rom.00cf77ba, "a"
	.incbin "baserom.gba", 0x00cf77ba, 0x00000002
	.section .rom.00cf794b, "a"
	.incbin "baserom.gba", 0x00cf794b, 0x00000001
	.section .rom.00cfedfd, "a"
	.incbin "baserom.gba", 0x00cfedfd, 0x00000003
	.section .rom.00d015d3, "a"
	.incbin "baserom.gba", 0x00d015d3, 0x00000001
	.section .rom.00d03d2e, "a"
	.incbin "baserom.gba", 0x00d03d2e, 0x00000002
	.section .rom.00d03ee1, "a"
	.incbin "baserom.gba", 0x00d03ee1, 0x00000003
	.section .rom.00d055e5, "a"
	.incbin "baserom.gba", 0x00d055e5, 0x00000003
	.section .rom.00d0763f, "a"
	.incbin "baserom.gba", 0x00d0763f, 0x00000001
	.section .rom.00d08989, "a"
	.incbin "baserom.gba", 0x00d08989, 0x00000003
	.section .rom.00d0a3d3, "a"
	.incbin "baserom.gba", 0x00d0a3d3, 0x00000001
	.section .rom.00d0a4f7, "a"
	.incbin "baserom.gba", 0x00d0a4f7, 0x00000001
	.section .rom.00d0a9c9, "a"
	.incbin "baserom.gba", 0x00d0a9c9, 0x00000003
	.section .rom.00d0cbad, "a"
	.incbin "baserom.gba", 0x00d0cbad, 0x00000003
	.section .rom.00d0d081, "a"
	.incbin "baserom.gba", 0x00d0d081, 0x00000003
	.section .rom.00d0f5b7, "a"
	.incbin "baserom.gba", 0x00d0f5b7, 0x00000001
	.section .rom.00d0f73f, "a"
	.incbin "baserom.gba", 0x00d0f73f, 0x00000001
	.section .rom.00d13fe1, "a"
	.incbin "baserom.gba", 0x00d13fe1, 0x00000003
	.section .rom.00d14117, "a"
	.incbin "baserom.gba", 0x00d14117, 0x00000001
	.section .rom.00d15d83, "a"
	.incbin "baserom.gba", 0x00d15d83, 0x00000001
	.section .rom.00d16f3b, "a"
	.incbin "baserom.gba", 0x00d16f3b, 0x00000001
	.section .rom.00d17c89, "a"
	.incbin "baserom.gba", 0x00d17c89, 0x00000003
	.section .rom.00d18e12, "a"
	.incbin "baserom.gba", 0x00d18e12, 0x00000002
	.section .rom.00d1a90b, "a"
	.incbin "baserom.gba", 0x00d1a90b, 0x00000001
	.section .rom.00d1bd17, "a"
	.incbin "baserom.gba", 0x00d1bd17, 0x00000001
	.section .rom.00d1c26b, "a"
	.incbin "baserom.gba", 0x00d1c26b, 0x00000001
	.section .rom.00d1c8a5, "a"
	.incbin "baserom.gba", 0x00d1c8a5, 0x00000003
	.section .rom.00d21bf5, "a"
	.incbin "baserom.gba", 0x00d21bf5, 0x00000003
	.section .rom.00d24256, "a"
	.incbin "baserom.gba", 0x00d24256, 0x00000002
	.section .rom.00d243eb, "a"
	.incbin "baserom.gba", 0x00d243eb, 0x00000001
	.section .rom.00d25f9f, "a"
	.incbin "baserom.gba", 0x00d25f9f, 0x00000001
	.section .rom.00d260ff, "a"
	.incbin "baserom.gba", 0x00d260ff, 0x00000001
	.section .rom.00d28ba3, "a"
	.incbin "baserom.gba", 0x00d28ba3, 0x000021b1
	.section .rom.00d2cbca, "a"
	.incbin "baserom.gba", 0x00d2cbca, 0x00000002
	.section .rom.00d2cd0b, "a"
	.incbin "baserom.gba", 0x00d2cd0b, 0x00000001
	.section .rom.00d33c83, "a"
	.incbin "baserom.gba", 0x00d33c83, 0x00000001
	.section .rom.00d33dc1, "a"
	.incbin "baserom.gba", 0x00d33dc1, 0x00000003
	.section .rom.00d35d95, "a"
	.incbin "baserom.gba", 0x00d35d95, 0x00000003
	.section .rom.00d35e89, "a"
	.incbin "baserom.gba", 0x00d35e89, 0x00000003
	.section .rom.00d36f69, "a"
	.incbin "baserom.gba", 0x00d36f69, 0x00000003
	.section .rom.00d38e43, "a"
	.incbin "baserom.gba", 0x00d38e43, 0x00000001
	.section .rom.00d39dfb, "a"
	.incbin "baserom.gba", 0x00d39dfb, 0x00000001
	.section .rom.00d3bd42, "a"
	.incbin "baserom.gba", 0x00d3bd42, 0x00000002
	.section .rom.00d3d94d, "a"
	.incbin "baserom.gba", 0x00d3d94d, 0x00000003
	.section .rom.00d3da99, "a"
	.incbin "baserom.gba", 0x00d3da99, 0x00000003
	.section .rom.00d3fb57, "a"
	.incbin "baserom.gba", 0x00d3fb57, 0x00000001
	.section .rom.00d43c46, "a"
	.incbin "baserom.gba", 0x00d43c46, 0x00000002
	.section .rom.00d479f8, "a"
	.incbin "baserom.gba", 0x00d479f8, 0x00007aa8
	.section .rom.00d50bed, "a"
	.incbin "baserom.gba", 0x00d50bed, 0x00000003
	.section .rom.00d50d1a, "a"
	.incbin "baserom.gba", 0x00d50d1a, 0x00000002
	.section .rom.00d57c8d, "a"
	.incbin "baserom.gba", 0x00d57c8d, 0x00000003
	.section .rom.00d57d92, "a"
	.incbin "baserom.gba", 0x00d57d92, 0x00000002
	.section .rom.00d57ed2, "a"
	.incbin "baserom.gba", 0x00d57ed2, 0x00000002
	.section .rom.00d5989e, "a"
	.incbin "baserom.gba", 0x00d5989e, 0x00000002
	.section .rom.00d59995, "a"
	.incbin "baserom.gba", 0x00d59995, 0x00000003
	.section .rom.00d5c7e9, "a"
	.incbin "baserom.gba", 0x00d5c7e9, 0x00000003
	.section .rom.00d5c992, "a"
	.incbin "baserom.gba", 0x00d5c992, 0x00000002
	.section .rom.00d5f5ae, "a"
	.incbin "baserom.gba", 0x00d5f5ae, 0x00000002
	.section .rom.00d622e1, "a"
	.incbin "baserom.gba", 0x00d622e1, 0x00000003
	.section .rom.00d63fcb, "a"
	.incbin "baserom.gba", 0x00d63fcb, 0x00000001
	.section .rom.00d67845, "a"
	.incbin "baserom.gba", 0x00d67845, 0x00000003
	.section .rom.00d6799d, "a"
	.incbin "baserom.gba", 0x00d6799d, 0x00000003
	.section .rom.00d69ee1, "a"
	.incbin "baserom.gba", 0x00d69ee1, 0x00000003
	.section .rom.00d6dda6, "a"
	.incbin "baserom.gba", 0x00d6dda6, 0x00000002
	.section .rom.00d6fed6, "a"
	.incbin "baserom.gba", 0x00d6fed6, 0x00000002
	.section .rom.00d740e2, "a"
	.incbin "baserom.gba", 0x00d740e2, 0x00000002
	.section .rom.00d742b6, "a"
	.incbin "baserom.gba", 0x00d742b6, 0x00000002
	.section .rom.00d762f6, "a"
	.incbin "baserom.gba", 0x00d762f6, 0x00000002
	.section .rom.00d776ed, "a"
	.incbin "baserom.gba", 0x00d776ed, 0x00000003
	.section .rom.00d7822a, "a"
	.incbin "baserom.gba", 0x00d7822a, 0x00000002
	.section .rom.00d79395, "a"
	.incbin "baserom.gba", 0x00d79395, 0x00000003
	.section .rom.00d7a441, "a"
	.incbin "baserom.gba", 0x00d7a441, 0x00000003
	.section .rom.00d7a5ed, "a"
	.incbin "baserom.gba", 0x00d7a5ed, 0x00000003
	.section .rom.00d7c0fa, "a"
	.incbin "baserom.gba", 0x00d7c0fa, 0x00000002
	.section .rom.00d7db17, "a"
	.incbin "baserom.gba", 0x00d7db17, 0x00000001
	.section .rom.00d7ffe9, "a"
	.incbin "baserom.gba", 0x00d7ffe9, 0x00000003
	.section .rom.00d8134b, "a"
	.incbin "baserom.gba", 0x00d8134b, 0x00000001
	.section .rom.00d821ff, "a"
	.incbin "baserom.gba", 0x00d821ff, 0x00000001
	.section .rom.00d837ed, "a"
	.incbin "baserom.gba", 0x00d837ed, 0x00000003
	.section .rom.00d84d33, "a"
	.incbin "baserom.gba", 0x00d84d33, 0x00000001
	.section .rom.00d85afd, "a"
	.incbin "baserom.gba", 0x00d85afd, 0x00000003
	.section .rom.00d85c6b, "a"
	.incbin "baserom.gba", 0x00d85c6b, 0x00000001
	.section .rom.00d86c2a, "a"
	.incbin "baserom.gba", 0x00d86c2a, 0x00000002
	.section .rom.00d876f3, "a"
	.incbin "baserom.gba", 0x00d876f3, 0x00000001
	.section .rom.00d8828d, "a"
	.incbin "baserom.gba", 0x00d8828d, 0x00000003
	.section .rom.00d883dd, "a"
	.incbin "baserom.gba", 0x00d883dd, 0x00000003
	.section .rom.00d8c68b, "a"
	.incbin "baserom.gba", 0x00d8c68b, 0x00000001
	.section .rom.00d8e125, "a"
	.incbin "baserom.gba", 0x00d8e125, 0x00000003
	.section .rom.00d8fafb, "a"
	.incbin "baserom.gba", 0x00d8fafb, 0x00000001
	.section .rom.00d8fc7a, "a"
	.incbin "baserom.gba", 0x00d8fc7a, 0x00000002
	.section .rom.00d938aa, "a"
	.incbin "baserom.gba", 0x00d938aa, 0x00000002
	.section .rom.00d95607, "a"
	.incbin "baserom.gba", 0x00d95607, 0x00000001
	.section .rom.00d98c31, "a"
	.incbin "baserom.gba", 0x00d98c31, 0x00000003
	.section .rom.00d98d82, "a"
	.incbin "baserom.gba", 0x00d98d82, 0x00000002
	.section .rom.00d9e97e, "a"
	.incbin "baserom.gba", 0x00d9e97e, 0x00000002
	.section .rom.00da12bb, "a"
	.incbin "baserom.gba", 0x00da12bb, 0x00000001
	.section .rom.00da2bfd, "a"
	.incbin "baserom.gba", 0x00da2bfd, 0x00000003
	.section .rom.00da2d66, "a"
	.incbin "baserom.gba", 0x00da2d66, 0x00000002
	.section .rom.00da9b65, "a"
	.incbin "baserom.gba", 0x00da9b65, 0x00000003
	.section .rom.00daa143, "a"
	.incbin "baserom.gba", 0x00daa143, 0x00000001
	.section .rom.00dab04b, "a"
	.incbin "baserom.gba", 0x00dab04b, 0x00000001
	.section .rom.00dad893, "a"
	.incbin "baserom.gba", 0x00dad893, 0x00000001
	.section .rom.00daeeb2, "a"
	.incbin "baserom.gba", 0x00daeeb2, 0x00000002
	.section .rom.00db0813, "a"
	.incbin "baserom.gba", 0x00db0813, 0x00000001
	.section .rom.00db35fb, "a"
	.incbin "baserom.gba", 0x00db35fb, 0x00000001
	.section .rom.00db3776, "a"
	.incbin "baserom.gba", 0x00db3776, 0x00000002
	.section .rom.00dc293f, "a"
	.incbin "baserom.gba", 0x00dc293f, 0x00000001
	.section .rom.00dc2aa7, "a"
	.incbin "baserom.gba", 0x00dc2aa7, 0x00000001
	.section .rom.00dc50ca, "a"
	.incbin "baserom.gba", 0x00dc50ca, 0x00000002
	.section .rom.00dc520d, "a"
	.incbin "baserom.gba", 0x00dc520d, 0x00000003
	.section .rom.00dc6aca, "a"
	.incbin "baserom.gba", 0x00dc6aca, 0x00000002
	.section .rom.00dc9a2e, "a"
	.incbin "baserom.gba", 0x00dc9a2e, 0x00000002
	.section .rom.00dcc3c6, "a"
	.incbin "baserom.gba", 0x00dcc3c6, 0x00000002
	.section .rom.00dd0d1e, "a"
	.incbin "baserom.gba", 0x00dd0d1e, 0x00000002
	.section .rom.00dd3347, "a"
	.incbin "baserom.gba", 0x00dd3347, 0x00000001
	.section .rom.00dd351b, "a"
	.incbin "baserom.gba", 0x00dd351b, 0x00000001
	.section .rom.00dd683a, "a"
	.incbin "baserom.gba", 0x00dd683a, 0x00000002
	.section .rom.00dd6a11, "a"
	.incbin "baserom.gba", 0x00dd6a11, 0x00000003
	.section .rom.00dd9d85, "a"
	.incbin "baserom.gba", 0x00dd9d85, 0x00000003
	.section .rom.00ddb4d9, "a"
	.incbin "baserom.gba", 0x00ddb4d9, 0x00000003
	.section .rom.00ddc467, "a"
	.incbin "baserom.gba", 0x00ddc467, 0x00000001
	.section .rom.00dddddd, "a"
	.incbin "baserom.gba", 0x00dddddd, 0x00000003
	.section .rom.00dddf17, "a"
	.incbin "baserom.gba", 0x00dddf17, 0x00000001
	.section .rom.00ddfa59, "a"
	.incbin "baserom.gba", 0x00ddfa59, 0x00000003
	.section .rom.00de3031, "a"
	.incbin "baserom.gba", 0x00de3031, 0x00000003
	.section .rom.00de3cbb, "a"
	.incbin "baserom.gba", 0x00de3cbb, 0x00000001
	.section .rom.00de3dfb, "a"
	.incbin "baserom.gba", 0x00de3dfb, 0x00000001
	.section .rom.00de5bbf, "a"
	.incbin "baserom.gba", 0x00de5bbf, 0x00000001
	.section .rom.00de5d32, "a"
	.incbin "baserom.gba", 0x00de5d32, 0x00000002
	.section .rom.00de819a, "a"
	.incbin "baserom.gba", 0x00de819a, 0x00000002
	.section .rom.00de9b36, "a"
	.incbin "baserom.gba", 0x00de9b36, 0x00000002
	.section .rom.00deb96b, "a"
	.incbin "baserom.gba", 0x00deb96b, 0x00000001
	.section .rom.00dec422, "a"
	.incbin "baserom.gba", 0x00dec422, 0x00000002
	.section .rom.00dede3e, "a"
	.incbin "baserom.gba", 0x00dede3e, 0x00000002
	.section .rom.00df0b0a, "a"
	.incbin "baserom.gba", 0x00df0b0a, 0x00000002
	.section .rom.00df0ce5, "a"
	.incbin "baserom.gba", 0x00df0ce5, 0x00000003
	.section .rom.00df281b, "a"
	.incbin "baserom.gba", 0x00df281b, 0x00000001
	.section .rom.00df58e5, "a"
	.incbin "baserom.gba", 0x00df58e5, 0x00000003
	.section .rom.00df6aa1, "a"
	.incbin "baserom.gba", 0x00df6aa1, 0x00000003
	.section .rom.00df6c89, "a"
	.incbin "baserom.gba", 0x00df6c89, 0x00000003
	.section .rom.00df6e1b, "a"
	.incbin "baserom.gba", 0x00df6e1b, 0x00000001
	.section .rom.00df7ba2, "a"
	.incbin "baserom.gba", 0x00df7ba2, 0x00000002
	.section .rom.00df7d37, "a"
	.incbin "baserom.gba", 0x00df7d37, 0x00000001
	.section .rom.00df9deb, "a"
	.incbin "baserom.gba", 0x00df9deb, 0x00000001
	.section .rom.00e01e6e, "a"
	.incbin "baserom.gba", 0x00e01e6e, 0x00000002
	.section .rom.00e01faf, "a"
	.incbin "baserom.gba", 0x00e01faf, 0x00000001
	.section .rom.00e06337, "a"
	.incbin "baserom.gba", 0x00e06337, 0x00000001
	.section .rom.00e06a56, "a"
	.incbin "baserom.gba", 0x00e06a56, 0x00000002
	.section .rom.00e07e15, "a"
	.incbin "baserom.gba", 0x00e07e15, 0x00000003
	.section .rom.00e0bf2f, "a"
	.incbin "baserom.gba", 0x00e0bf2f, 0x00000001
	.section .rom.00e0cfaf, "a"
	.incbin "baserom.gba", 0x00e0cfaf, 0x00000001
	.section .rom.00e0e031, "a"
	.incbin "baserom.gba", 0x00e0e031, 0x00000003
	.section .rom.00e0eba2, "a"
	.incbin "baserom.gba", 0x00e0eba2, 0x00000002
	.section .rom.00e0faf3, "a"
	.incbin "baserom.gba", 0x00e0faf3, 0x00000001
	.section .rom.00e0fc71, "a"
	.incbin "baserom.gba", 0x00e0fc71, 0x00000003
	.section .rom.00e131cb, "a"
	.incbin "baserom.gba", 0x00e131cb, 0x00000001
	.section .rom.00e147fe, "a"
	.incbin "baserom.gba", 0x00e147fe, 0x00000002
	.section .rom.00e1493f, "a"
	.incbin "baserom.gba", 0x00e1493f, 0x00000001
	.section .rom.00e1728a, "a"
	.incbin "baserom.gba", 0x00e1728a, 0x00000002
	.section .rom.00e1fc32, "a"
	.incbin "baserom.gba", 0x00e1fc32, 0x00000fde
	.section .rom.00e227dd, "a"
	.incbin "baserom.gba", 0x00e227dd, 0x00000003
	.section .rom.00e2471d, "a"
	.incbin "baserom.gba", 0x00e2471d, 0x00000003
	.section .rom.00e25b91, "a"
	.incbin "baserom.gba", 0x00e25b91, 0x00000003
	.section .rom.00e2784b, "a"
	.incbin "baserom.gba", 0x00e2784b, 0x00000001
	.section .rom.00e348fa, "a"
	.incbin "baserom.gba", 0x00e348fa, 0x00000002
	.section .rom.00e34a39, "a"
	.incbin "baserom.gba", 0x00e34a39, 0x00000003
	.section .rom.00e3695e, "a"
	.incbin "baserom.gba", 0x00e3695e, 0x00000002
	.section .rom.00e36a87, "a"
	.incbin "baserom.gba", 0x00e36a87, 0x00000001
	.section .rom.00e38c57, "a"
	.incbin "baserom.gba", 0x00e38c57, 0x00000001
	.section .rom.00e3b227, "a"
	.incbin "baserom.gba", 0x00e3b227, 0x00000001
	.section .rom.00e3d539, "a"
	.incbin "baserom.gba", 0x00e3d539, 0x00000003
	.section .rom.00e3ddad, "a"
	.incbin "baserom.gba", 0x00e3ddad, 0x00000003
	.section .rom.00e3deef, "a"
	.incbin "baserom.gba", 0x00e3deef, 0x00000001
	.section .rom.00e40fa5, "a"
	.incbin "baserom.gba", 0x00e40fa5, 0x00000003
	.section .rom.00e4268b, "a"
	.incbin "baserom.gba", 0x00e4268b, 0x00000001
	.section .rom.00e44253, "a"
	.incbin "baserom.gba", 0x00e44253, 0x00000001
	.section .rom.00e45dce, "a"
	.incbin "baserom.gba", 0x00e45dce, 0x00000002
	.section .rom.00e46219, "a"
	.incbin "baserom.gba", 0x00e46219, 0x00000003
	.section .rom.00e4837a, "a"
	.incbin "baserom.gba", 0x00e4837a, 0x00000002
	.section .rom.00e48493, "a"
	.incbin "baserom.gba", 0x00e48493, 0x00000001
	.section .rom.00e492bd, "a"
	.incbin "baserom.gba", 0x00e492bd, 0x00000003
	.section .rom.00e4941f, "a"
	.incbin "baserom.gba", 0x00e4941f, 0x00000001
	.section .rom.00e4dc87, "a"
	.incbin "baserom.gba", 0x00e4dc87, 0x00000001
	.section .rom.00e50345, "a"
	.incbin "baserom.gba", 0x00e50345, 0x00000003
	.section .rom.00e50923, "a"
	.incbin "baserom.gba", 0x00e50923, 0x00000001
	.section .rom.00e528b6, "a"
	.incbin "baserom.gba", 0x00e528b6, 0x00000002
	.section .rom.00e54096, "a"
	.incbin "baserom.gba", 0x00e54096, 0x00000002
	.section .rom.00e541fa, "a"
	.incbin "baserom.gba", 0x00e541fa, 0x00000002
	.section .rom.00e569f2, "a"
	.incbin "baserom.gba", 0x00e569f2, 0x00000002
	.section .rom.00e56bab, "a"
	.incbin "baserom.gba", 0x00e56bab, 0x00000001
	.section .rom.00e56d33, "a"
	.incbin "baserom.gba", 0x00e56d33, 0x00000001
	.section .rom.00e58555, "a"
	.incbin "baserom.gba", 0x00e58555, 0x00000003
	.section .rom.00e5913e, "a"
	.incbin "baserom.gba", 0x00e5913e, 0x00000002
	.section .rom.00e5a677, "a"
	.incbin "baserom.gba", 0x00e5a677, 0x00000001
	.section .rom.00e5a812, "a"
	.incbin "baserom.gba", 0x00e5a812, 0x00000002
	.section .rom.00e5cabe, "a"
	.incbin "baserom.gba", 0x00e5cabe, 0x00000002
	.section .rom.00e5ec85, "a"
	.incbin "baserom.gba", 0x00e5ec85, 0x00000003
	.section .rom.00e60383, "a"
	.incbin "baserom.gba", 0x00e60383, 0x00000001
	.section .rom.00e60ea7, "a"
	.incbin "baserom.gba", 0x00e60ea7, 0x00000001
	.section .rom.00e65d4a, "a"
	.incbin "baserom.gba", 0x00e65d4a, 0x00000002
	.section .rom.00e680f1, "a"
	.incbin "baserom.gba", 0x00e680f1, 0x00000003
	.section .rom.00e687da, "a"
	.incbin "baserom.gba", 0x00e687da, 0x00000002
	.section .rom.00e693c6, "a"
	.incbin "baserom.gba", 0x00e693c6, 0x00000ac6
	.section .rom.00e6b43b, "a"
	.incbin "baserom.gba", 0x00e6b43b, 0x00000001
	.section .rom.00e6b5ee, "a"
	.incbin "baserom.gba", 0x00e6b5ee, 0x00000002
	.section .rom.00e6d82d, "a"
	.incbin "baserom.gba", 0x00e6d82d, 0x00000003
	.section .rom.00e6e353, "a"
	.incbin "baserom.gba", 0x00e6e353, 0x00000001
	.section .rom.00e78de4, "a"
	.incbin "baserom.gba", 0x00e78de4, 0x000007d0
	.section .rom.00e7c27d, "a"
	.incbin "baserom.gba", 0x00e7c27d, 0x00000003
	.section .rom.00e7c3f7, "a"
	.incbin "baserom.gba", 0x00e7c3f7, 0x00000001
	.section .rom.00e7c56e, "a"
	.incbin "baserom.gba", 0x00e7c56e, 0x00000002
	.section .rom.00e7c706, "a"
	.incbin "baserom.gba", 0x00e7c706, 0x00000002
	.section .rom.00e7c897, "a"
	.incbin "baserom.gba", 0x00e7c897, 0x00000001
	.section .rom.00e7eb43, "a"
	.incbin "baserom.gba", 0x00e7eb43, 0x00000001
	.section .rom.00e7ec5b, "a"
	.incbin "baserom.gba", 0x00e7ec5b, 0x00000001
	.section .rom.00e80a5d, "a"
	.incbin "baserom.gba", 0x00e80a5d, 0x00000003
	.section .rom.00e82522, "a"
	.incbin "baserom.gba", 0x00e82522, 0x00000002
	.section .rom.00e82c4a, "a"
	.incbin "baserom.gba", 0x00e82c4a, 0x00000002
	.section .rom.00e84046, "a"
	.incbin "baserom.gba", 0x00e84046, 0x00000002
	.section .rom.00e84f1a, "a"
	.incbin "baserom.gba", 0x00e84f1a, 0x00000002
	.section .rom.00e8505e, "a"
	.incbin "baserom.gba", 0x00e8505e, 0x00000002
	.section .rom.00e8740f, "a"
	.incbin "baserom.gba", 0x00e8740f, 0x00000001
	.section .rom.00e89c3d, "a"
	.incbin "baserom.gba", 0x00e89c3d, 0x00000003
	.section .rom.00e8a03a, "a"
	.incbin "baserom.gba", 0x00e8a03a, 0x00000002
	.section .rom.00e8b4da, "a"
	.incbin "baserom.gba", 0x00e8b4da, 0x00000002
	.section .rom.00e8b62e, "a"
	.incbin "baserom.gba", 0x00e8b62e, 0x00000002
	.section .rom.00e8ef7b, "a"
	.incbin "baserom.gba", 0x00e8ef7b, 0x00000001
	.section .rom.00e903e6, "a"
	.incbin "baserom.gba", 0x00e903e6, 0x00000002
	.section .rom.00e918bf, "a"
	.incbin "baserom.gba", 0x00e918bf, 0x00000001
	.section .rom.00e91a12, "a"
	.incbin "baserom.gba", 0x00e91a12, 0x00000002
	.section .rom.00e9541f, "a"
	.incbin "baserom.gba", 0x00e9541f, 0x00000001
	.section .rom.00e965f7, "a"
	.incbin "baserom.gba", 0x00e965f7, 0x00000001
	.section .rom.00e97e02, "a"
	.incbin "baserom.gba", 0x00e97e02, 0x00000002
	.section .rom.00e97f52, "a"
	.incbin "baserom.gba", 0x00e97f52, 0x00000002
	.section .rom.00e9a65f, "a"
	.incbin "baserom.gba", 0x00e9a65f, 0x00000001
	.section .rom.00e9a7c6, "a"
	.incbin "baserom.gba", 0x00e9a7c6, 0x00000002
	.section .rom.00e9d787, "a"
	.incbin "baserom.gba", 0x00e9d787, 0x00000001
	.section .rom.00e9e9f5, "a"
	.incbin "baserom.gba", 0x00e9e9f5, 0x00000003
	.section .rom.00ea1582, "a"
	.incbin "baserom.gba", 0x00ea1582, 0x00000002
	.section .rom.00ea1f69, "a"
	.incbin "baserom.gba", 0x00ea1f69, 0x00000003
	.section .rom.00ea290a, "a"
	.incbin "baserom.gba", 0x00ea290a, 0x00000002
	.section .rom.00ea2a7e, "a"
	.incbin "baserom.gba", 0x00ea2a7e, 0x00000002
	.section .rom.00ea458f, "a"
	.incbin "baserom.gba", 0x00ea458f, 0x00000001
	.section .rom.00ea46b1, "a"
	.incbin "baserom.gba", 0x00ea46b1, 0x00000003
	.section .rom.00ea71af, "a"
	.incbin "baserom.gba", 0x00ea71af, 0x00000001
	.section .rom.00ea7fa6, "a"
	.incbin "baserom.gba", 0x00ea7fa6, 0x00000002
	.section .rom.00ea9f45, "a"
	.incbin "baserom.gba", 0x00ea9f45, 0x00000003
	.section .rom.00eab275, "a"
	.incbin "baserom.gba", 0x00eab275, 0x00000003
	.section .rom.00eab3c9, "a"
	.incbin "baserom.gba", 0x00eab3c9, 0x00000003
	.section .rom.00eabf7b, "a"
	.incbin "baserom.gba", 0x00eabf7b, 0x00000001
	.section .rom.00ead755, "a"
	.incbin "baserom.gba", 0x00ead755, 0x00000003
	.section .rom.00eae8b5, "a"
	.incbin "baserom.gba", 0x00eae8b5, 0x00000003
	.section .rom.00eaf9d3, "a"
	.incbin "baserom.gba", 0x00eaf9d3, 0x00000001
	.section .rom.00eafbda, "a"
	.incbin "baserom.gba", 0x00eafbda, 0x00000002
	.section .rom.00eb08cf, "a"
	.incbin "baserom.gba", 0x00eb08cf, 0x00000001
	.section .rom.00eb3071, "a"
	.incbin "baserom.gba", 0x00eb3071, 0x00000003
	.section .rom.00eb47e6, "a"
	.incbin "baserom.gba", 0x00eb47e6, 0x00000002
	.section .rom.00eb5aa3, "a"
	.incbin "baserom.gba", 0x00eb5aa3, 0x00000001
	.section .rom.00eb62e6, "a"
	.incbin "baserom.gba", 0x00eb62e6, 0x0000060e
	.section .rom.00eb6e6b, "a"
	.incbin "baserom.gba", 0x00eb6e6b, 0x00000001
	.section .rom.00eb6fab, "a"
	.incbin "baserom.gba", 0x00eb6fab, 0x00000001
	.section .rom.00eb70eb, "a"
	.incbin "baserom.gba", 0x00eb70eb, 0x00000001
	.section .rom.00eb7c4d, "a"
	.incbin "baserom.gba", 0x00eb7c4d, 0x00000003
	.section .rom.00eba3f1, "a"
	.incbin "baserom.gba", 0x00eba3f1, 0x00000003
	.section .rom.00ebbb66, "a"
	.incbin "baserom.gba", 0x00ebbb66, 0x00000002
	.section .rom.00ebce23, "a"
	.incbin "baserom.gba", 0x00ebce23, 0x00000001
	.section .rom.00ebd666, "a"
	.incbin "baserom.gba", 0x00ebd666, 0x00000002
	.section .rom.00ebdc05, "a"
	.incbin "baserom.gba", 0x00ebdc05, 0x0000051f
	.section .rom.00ebe839, "a"
	.incbin "baserom.gba", 0x00ebe839, 0x0000051f
	.section .rom.00ebf919, "a"
	.incbin "baserom.gba", 0x00ebf919, 0x0000051f
	.section .rom.00ec0405, "a"
	.incbin "baserom.gba", 0x00ec0405, 0x0000051f
	.section .rom.00ec12a1, "a"
	.incbin "baserom.gba", 0x00ec12a1, 0x0000051f
	.section .rom.00ec1be9, "a"
	.incbin "baserom.gba", 0x00ec1be9, 0x0000051f
	.section .rom.00ec2731, "a"
	.incbin "baserom.gba", 0x00ec2731, 0x0000051f
	.section .rom.00ec3379, "a"
	.incbin "baserom.gba", 0x00ec3379, 0x0013cc87
