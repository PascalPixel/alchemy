@ tla-es's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.incbin "baserom.gba", 0x00002064, 0x000586fc
	.section .rom.0005c360, "a"
	.incbin "baserom.gba", 0x0005c360, 0x0000387c
	.section .rom.000a8fd8, "a"
	.incbin "baserom.gba", 0x000a8fd8, 0x00118338
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
	.section .rom.0068a170, "a"
	.incbin "baserom.gba", 0x0068a170, 0x0000da18
	.section .rom.006a471d, "a"
	.incbin "baserom.gba", 0x006a471d, 0x000042eb
	.section .rom.006a9e30, "a"
	.incbin "baserom.gba", 0x006a9e30, 0x00024768
	.section .rom.006d0f86, "a"
	.incbin "baserom.gba", 0x006d0f86, 0x00000002
	.section .rom.006d53ca, "a"
	.incbin "baserom.gba", 0x006d53ca, 0x00000002
	.section .rom.006e5b3a, "a"
	.incbin "baserom.gba", 0x006e5b3a, 0x00000002
	.section .rom.006e912a, "a"
	.incbin "baserom.gba", 0x006e912a, 0x00000002
	.section .rom.006f4bce, "a"
	.incbin "baserom.gba", 0x006f4bce, 0x00000002
	.section .rom.0070527e, "a"
	.incbin "baserom.gba", 0x0070527e, 0x00000002
	.section .rom.0070cd1e, "a"
	.incbin "baserom.gba", 0x0070cd1e, 0x00000002
	.section .rom.0071055e, "a"
	.incbin "baserom.gba", 0x0071055e, 0x00000002
	.section .rom.00719992, "a"
	.incbin "baserom.gba", 0x00719992, 0x00000002
	.section .rom.00720b3e, "a"
	.incbin "baserom.gba", 0x00720b3e, 0x00000002
	.section .rom.007289fa, "a"
	.incbin "baserom.gba", 0x007289fa, 0x00000002
	.section .rom.0072d48a, "a"
	.incbin "baserom.gba", 0x0072d48a, 0x00000002
	.section .rom.0073173e, "a"
	.incbin "baserom.gba", 0x0073173e, 0x00000002
	.section .rom.007350be, "a"
	.incbin "baserom.gba", 0x007350be, 0x00000002
	.section .rom.007418fa, "a"
	.incbin "baserom.gba", 0x007418fa, 0x00000002
	.section .rom.0074d03a, "a"
	.incbin "baserom.gba", 0x0074d03a, 0x00000002
	.section .rom.0075040a, "a"
	.incbin "baserom.gba", 0x0075040a, 0x00000002
	.section .rom.00762326, "a"
	.incbin "baserom.gba", 0x00762326, 0x00000002
	.section .rom.00765d8e, "a"
	.incbin "baserom.gba", 0x00765d8e, 0x00000002
	.section .rom.0076977e, "a"
	.incbin "baserom.gba", 0x0076977e, 0x00000002
	.section .rom.00779ce6, "a"
	.incbin "baserom.gba", 0x00779ce6, 0x00000002
	.section .rom.0077dc16, "a"
	.incbin "baserom.gba", 0x0077dc16, 0x00000002
	.section .rom.0078163a, "a"
	.incbin "baserom.gba", 0x0078163a, 0x00000002
	.section .rom.0078995a, "a"
	.incbin "baserom.gba", 0x0078995a, 0x00000002
	.section .rom.00791ac6, "a"
	.incbin "baserom.gba", 0x00791ac6, 0x00000002
	.section .rom.0079ed92, "a"
	.incbin "baserom.gba", 0x0079ed92, 0x00000002
	.section .rom.007a2c4e, "a"
	.incbin "baserom.gba", 0x007a2c4e, 0x00000002
	.section .rom.007a6a4a, "a"
	.incbin "baserom.gba", 0x007a6a4a, 0x00000002
	.section .rom.007aaf1a, "a"
	.incbin "baserom.gba", 0x007aaf1a, 0x00000002
	.section .rom.007b28f6, "a"
	.incbin "baserom.gba", 0x007b28f6, 0x00000002
	.section .rom.007b6f22, "a"
	.incbin "baserom.gba", 0x007b6f22, 0x00000002
	.section .rom.007beb9e, "a"
	.incbin "baserom.gba", 0x007beb9e, 0x00000002
	.section .rom.007c279a, "a"
	.incbin "baserom.gba", 0x007c279a, 0x00000002
	.section .rom.007c614a, "a"
	.incbin "baserom.gba", 0x007c614a, 0x00000002
	.section .rom.007ca59a, "a"
	.incbin "baserom.gba", 0x007ca59a, 0x00000002
	.section .rom.007ce192, "a"
	.incbin "baserom.gba", 0x007ce192, 0x00000002
	.section .rom.007d9df2, "a"
	.incbin "baserom.gba", 0x007d9df2, 0x00000002
	.section .rom.007dda42, "a"
	.incbin "baserom.gba", 0x007dda42, 0x00000002
	.section .rom.007e5fc2, "a"
	.incbin "baserom.gba", 0x007e5fc2, 0x00000002
	.section .rom.007f2612, "a"
	.incbin "baserom.gba", 0x007f2612, 0x00000002
	.section .rom.007f90c6, "a"
	.incbin "baserom.gba", 0x007f90c6, 0x00000002
	.section .rom.0080238e, "a"
	.incbin "baserom.gba", 0x0080238e, 0x00000002
	.section .rom.00809db2, "a"
	.incbin "baserom.gba", 0x00809db2, 0x00000002
	.section .rom.0081fa32, "a"
	.incbin "baserom.gba", 0x0081fa32, 0x00000002
	.section .rom.00828e76, "a"
	.incbin "baserom.gba", 0x00828e76, 0x00000002
	.section .rom.00832092, "a"
	.incbin "baserom.gba", 0x00832092, 0x00000002
	.section .rom.0083f9ba, "a"
	.incbin "baserom.gba", 0x0083f9ba, 0x00000002
	.section .rom.0084580e, "a"
	.incbin "baserom.gba", 0x0084580e, 0x00000002
	.section .rom.0084bd89, "a"
	.incbin "baserom.gba", 0x0084bd89, 0x00000003
	.section .rom.0084d70b, "a"
	.incbin "baserom.gba", 0x0084d70b, 0x00000001
	.section .rom.00850529, "a"
	.incbin "baserom.gba", 0x00850529, 0x00000003
	.section .rom.0085443e, "a"
	.incbin "baserom.gba", 0x0085443e, 0x00000002
	.section .rom.008573ba, "a"
	.incbin "baserom.gba", 0x008573ba, 0x000009be
	.section .rom.00858396, "a"
	.incbin "baserom.gba", 0x00858396, 0x00000002
	.section .rom.0085890f, "a"
	.incbin "baserom.gba", 0x0085890f, 0x00000001
	.section .rom.00858ec1, "a"
	.incbin "baserom.gba", 0x00858ec1, 0x00000003
	.section .rom.0085921b, "a"
	.incbin "baserom.gba", 0x0085921b, 0x00000001
	.section .rom.00859579, "a"
	.incbin "baserom.gba", 0x00859579, 0x00000003
	.section .rom.008598e9, "a"
	.incbin "baserom.gba", 0x008598e9, 0x00000003
	.section .rom.00859d21, "a"
	.incbin "baserom.gba", 0x00859d21, 0x00000003
	.section .rom.0085a457, "a"
	.incbin "baserom.gba", 0x0085a457, 0x00000001
	.section .rom.0085a521, "a"
	.incbin "baserom.gba", 0x0085a521, 0x00000003
	.section .rom.0085a686, "a"
	.incbin "baserom.gba", 0x0085a686, 0x00000002
	.section .rom.0085b229, "a"
	.incbin "baserom.gba", 0x0085b229, 0x00000003
	.section .rom.0085be62, "a"
	.incbin "baserom.gba", 0x0085be62, 0x00000002
	.section .rom.0085c09b, "a"
	.incbin "baserom.gba", 0x0085c09b, 0x00000001
	.section .rom.0085e73b, "a"
	.incbin "baserom.gba", 0x0085e73b, 0x00000001
	.section .rom.0085eeb6, "a"
	.incbin "baserom.gba", 0x0085eeb6, 0x00000002
	.section .rom.008604fb, "a"
	.incbin "baserom.gba", 0x008604fb, 0x00000001
	.section .rom.00860a25, "a"
	.incbin "baserom.gba", 0x00860a25, 0x00000003
	.section .rom.0086122e, "a"
	.incbin "baserom.gba", 0x0086122e, 0x00000002
	.section .rom.008617a7, "a"
	.incbin "baserom.gba", 0x008617a7, 0x00000001
	.section .rom.00862ee6, "a"
	.incbin "baserom.gba", 0x00862ee6, 0x00000002
	.section .rom.00865daf, "a"
	.incbin "baserom.gba", 0x00865daf, 0x00000001
	.section .rom.00866061, "a"
	.incbin "baserom.gba", 0x00866061, 0x00000003
	.section .rom.00867443, "a"
	.incbin "baserom.gba", 0x00867443, 0x00000001
	.section .rom.0086bc7f, "a"
	.incbin "baserom.gba", 0x0086bc7f, 0x00000001
	.section .rom.0086f425, "a"
	.incbin "baserom.gba", 0x0086f425, 0x00000003
	.section .rom.0087114a, "a"
	.incbin "baserom.gba", 0x0087114a, 0x00000002
	.section .rom.00873049, "a"
	.incbin "baserom.gba", 0x00873049, 0x00000003
	.section .rom.008749f7, "a"
	.incbin "baserom.gba", 0x008749f7, 0x00000001
	.section .rom.00875f6f, "a"
	.incbin "baserom.gba", 0x00875f6f, 0x00000001
	.section .rom.00879ba2, "a"
	.incbin "baserom.gba", 0x00879ba2, 0x00000002
	.section .rom.0087a700, "a"
	.incbin "baserom.gba", 0x0087a700, 0x00003b38
	.section .rom.0087e86f, "a"
	.incbin "baserom.gba", 0x0087e86f, 0x00000001
	.section .rom.008809d9, "a"
	.incbin "baserom.gba", 0x008809d9, 0x00000003
	.section .rom.00880ae4, "a"
	.incbin "baserom.gba", 0x00880ae4, 0x000003d0
	.section .rom.0088176d, "a"
	.incbin "baserom.gba", 0x0088176d, 0x00000003
	.section .rom.00883351, "a"
	.incbin "baserom.gba", 0x00883351, 0x00000003
	.section .rom.00884eae, "a"
	.incbin "baserom.gba", 0x00884eae, 0x00000002
	.section .rom.008852ed, "a"
	.incbin "baserom.gba", 0x008852ed, 0x00000003
	.section .rom.00885702, "a"
	.incbin "baserom.gba", 0x00885702, 0x00001842
	.section .rom.0088944f, "a"
	.incbin "baserom.gba", 0x0088944f, 0x00000001
	.section .rom.00889e0b, "a"
	.incbin "baserom.gba", 0x00889e0b, 0x00000001
	.section .rom.0088af4e, "a"
	.incbin "baserom.gba", 0x0088af4e, 0x00001496
	.section .rom.0088d95f, "a"
	.incbin "baserom.gba", 0x0088d95f, 0x00000001
	.section .rom.0088dc89, "a"
	.incbin "baserom.gba", 0x0088dc89, 0x0000104b
	.section .rom.0088f45d, "a"
	.incbin "baserom.gba", 0x0088f45d, 0x00000623
	.section .rom.008900a9, "a"
	.incbin "baserom.gba", 0x008900a9, 0x00000003
	.section .rom.008904b3, "a"
	.incbin "baserom.gba", 0x008904b3, 0x000007d9
	.section .rom.00890f35, "a"
	.incbin "baserom.gba", 0x00890f35, 0x0000095f
	.section .rom.00891f51, "a"
	.incbin "baserom.gba", 0x00891f51, 0x00003c7f
	.section .rom.0089622f, "a"
	.incbin "baserom.gba", 0x0089622f, 0x00000001
	.section .rom.00897269, "a"
	.incbin "baserom.gba", 0x00897269, 0x00000003
	.section .rom.008978bf, "a"
	.incbin "baserom.gba", 0x008978bf, 0x00000001
	.section .rom.00897f3d, "a"
	.incbin "baserom.gba", 0x00897f3d, 0x00000003
	.section .rom.0089855d, "a"
	.incbin "baserom.gba", 0x0089855d, 0x00000003
	.section .rom.0089992a, "a"
	.incbin "baserom.gba", 0x0089992a, 0x00000002
	.section .rom.0089a96e, "a"
	.incbin "baserom.gba", 0x0089a96e, 0x00000002
	.section .rom.0089b3f7, "a"
	.incbin "baserom.gba", 0x0089b3f7, 0x000002cd
	.section .rom.0089c3fd, "a"
	.incbin "baserom.gba", 0x0089c3fd, 0x00000003
	.section .rom.0089d639, "a"
	.incbin "baserom.gba", 0x0089d639, 0x00000003
	.section .rom.0089df83, "a"
	.incbin "baserom.gba", 0x0089df83, 0x00006c09
	.section .rom.008a50c0, "a"
	.incbin "baserom.gba", 0x008a50c0, 0x000011d8
	.section .rom.008a6786, "a"
	.incbin "baserom.gba", 0x008a6786, 0x000027fe
	.section .rom.008a95c7, "a"
	.incbin "baserom.gba", 0x008a95c7, 0x000000a9
	.section .rom.008a9986, "a"
	.incbin "baserom.gba", 0x008a9986, 0x000018d2
	.section .rom.008abec9, "a"
	.incbin "baserom.gba", 0x008abec9, 0x00000003
	.section .rom.008ac711, "a"
	.incbin "baserom.gba", 0x008ac711, 0x00001203
	.section .rom.008ae7d3, "a"
	.incbin "baserom.gba", 0x008ae7d3, 0x00000001
	.section .rom.008af2af, "a"
	.incbin "baserom.gba", 0x008af2af, 0x00000001
	.section .rom.008af972, "a"
	.incbin "baserom.gba", 0x008af972, 0x00000002
	.section .rom.008afc59, "a"
	.incbin "baserom.gba", 0x008afc59, 0x00000003
	.section .rom.008b0fbf, "a"
	.incbin "baserom.gba", 0x008b0fbf, 0x00000001
	.section .rom.008b13a9, "a"
	.incbin "baserom.gba", 0x008b13a9, 0x00000003
	.section .rom.008b177b, "a"
	.incbin "baserom.gba", 0x008b177b, 0x00000001
	.section .rom.008b201b, "a"
	.incbin "baserom.gba", 0x008b201b, 0x00000001
	.section .rom.008b24be, "a"
	.incbin "baserom.gba", 0x008b24be, 0x00000002
	.section .rom.008b3677, "a"
	.incbin "baserom.gba", 0x008b3677, 0x00000001
	.section .rom.008b3ad0, "a"
	.incbin "baserom.gba", 0x008b3ad0, 0x00002034
	.section .rom.008b5d5e, "a"
	.incbin "baserom.gba", 0x008b5d5e, 0x00000002
	.section .rom.008b77b9, "a"
	.incbin "baserom.gba", 0x008b77b9, 0x000001b7
	.section .rom.008b7d05, "a"
	.incbin "baserom.gba", 0x008b7d05, 0x00000003
	.section .rom.008b9a8e, "a"
	.incbin "baserom.gba", 0x008b9a8e, 0x0000027a
	.section .rom.008ba1ce, "a"
	.incbin "baserom.gba", 0x008ba1ce, 0x00000002
	.section .rom.008bbda3, "a"
	.incbin "baserom.gba", 0x008bbda3, 0x00000001
	.section .rom.008bd9a1, "a"
	.incbin "baserom.gba", 0x008bd9a1, 0x00000003
	.section .rom.008bdbc2, "a"
	.incbin "baserom.gba", 0x008bdbc2, 0x0000043e
	.section .rom.008be111, "a"
	.incbin "baserom.gba", 0x008be111, 0x00000003
	.section .rom.008be6f3, "a"
	.incbin "baserom.gba", 0x008be6f3, 0x000004a1
	.section .rom.008bf8a6, "a"
	.incbin "baserom.gba", 0x008bf8a6, 0x00000002
	.section .rom.008bfde8, "a"
	.incbin "baserom.gba", 0x008bfde8, 0x00000840
	.section .rom.008c09b7, "a"
	.incbin "baserom.gba", 0x008c09b7, 0x00001259
	.section .rom.008c26fe, "a"
	.incbin "baserom.gba", 0x008c26fe, 0x00000002
	.section .rom.008c34d5, "a"
	.incbin "baserom.gba", 0x008c34d5, 0x00000003
	.section .rom.008c3d02, "a"
	.incbin "baserom.gba", 0x008c3d02, 0x00000002
	.section .rom.008c40f4, "a"
	.incbin "baserom.gba", 0x008c40f4, 0x000010d0
	.section .rom.008c5b35, "a"
	.incbin "baserom.gba", 0x008c5b35, 0x00000003
	.section .rom.008c5ee3, "a"
	.incbin "baserom.gba", 0x008c5ee3, 0x00000001
	.section .rom.008c7e87, "a"
	.incbin "baserom.gba", 0x008c7e87, 0x00000001
	.section .rom.008c8c23, "a"
	.incbin "baserom.gba", 0x008c8c23, 0x00000001
	.section .rom.008c8e3f, "a"
	.incbin "baserom.gba", 0x008c8e3f, 0x00000001
	.section .rom.008c913b, "a"
	.incbin "baserom.gba", 0x008c913b, 0x00000001
	.section .rom.008c94dc, "a"
	.incbin "baserom.gba", 0x008c94dc, 0x000016b0
	.section .rom.008cb2e9, "a"
	.incbin "baserom.gba", 0x008cb2e9, 0x00000003
	.section .rom.008cc0b7, "a"
	.incbin "baserom.gba", 0x008cc0b7, 0x00000c75
	.section .rom.008cd321, "a"
	.incbin "baserom.gba", 0x008cd321, 0x00000003
	.section .rom.008cd87b, "a"
	.incbin "baserom.gba", 0x008cd87b, 0x00000001
	.section .rom.008cf165, "a"
	.incbin "baserom.gba", 0x008cf165, 0x000008a3
	.section .rom.008cfde3, "a"
	.incbin "baserom.gba", 0x008cfde3, 0x00000001
	.section .rom.008d006d, "a"
	.incbin "baserom.gba", 0x008d006d, 0x00000003
	.section .rom.008d0413, "a"
	.incbin "baserom.gba", 0x008d0413, 0x00000001
	.section .rom.008d066e, "a"
	.incbin "baserom.gba", 0x008d066e, 0x00000002
	.section .rom.008d0a26, "a"
	.incbin "baserom.gba", 0x008d0a26, 0x00000002
	.section .rom.008d1f0f, "a"
	.incbin "baserom.gba", 0x008d1f0f, 0x00000001
	.section .rom.008d34d2, "a"
	.incbin "baserom.gba", 0x008d34d2, 0x00000a6e
	.section .rom.008d4d16, "a"
	.incbin "baserom.gba", 0x008d4d16, 0x00000002
	.section .rom.008d5099, "a"
	.incbin "baserom.gba", 0x008d5099, 0x00000003
	.section .rom.008d5d49, "a"
	.incbin "baserom.gba", 0x008d5d49, 0x00000003
	.section .rom.008d6891, "a"
	.incbin "baserom.gba", 0x008d6891, 0x00000a27
	.section .rom.008d7e86, "a"
	.incbin "baserom.gba", 0x008d7e86, 0x00000002
	.section .rom.008d839e, "a"
	.incbin "baserom.gba", 0x008d839e, 0x00000626
	.section .rom.008d8de5, "a"
	.incbin "baserom.gba", 0x008d8de5, 0x00000003
	.section .rom.008d907f, "a"
	.incbin "baserom.gba", 0x008d907f, 0x00000001
	.section .rom.008da580, "a"
	.incbin "baserom.gba", 0x008da580, 0x00001e28
	.section .rom.008dc792, "a"
	.incbin "baserom.gba", 0x008dc792, 0x00000002
	.section .rom.008de707, "a"
	.incbin "baserom.gba", 0x008de707, 0x00000001
	.section .rom.008debd7, "a"
	.incbin "baserom.gba", 0x008debd7, 0x00001cc5
	.section .rom.008e1071, "a"
	.incbin "baserom.gba", 0x008e1071, 0x00000003
	.section .rom.008e1b42, "a"
	.incbin "baserom.gba", 0x008e1b42, 0x00000002
	.section .rom.008e267f, "a"
	.incbin "baserom.gba", 0x008e267f, 0x00001c2d
	.section .rom.008e4617, "a"
	.incbin "baserom.gba", 0x008e4617, 0x00000001
	.section .rom.008e4bb5, "a"
	.incbin "baserom.gba", 0x008e4bb5, 0x00000207
	.section .rom.008e50f5, "a"
	.incbin "baserom.gba", 0x008e50f5, 0x00002687
	.section .rom.008e971a, "a"
	.incbin "baserom.gba", 0x008e971a, 0x00000002
	.section .rom.008e9ebd, "a"
	.incbin "baserom.gba", 0x008e9ebd, 0x00000003
	.section .rom.008eaed4, "a"
	.incbin "baserom.gba", 0x008eaed4, 0x00001878
	.section .rom.008ec7d0, "a"
	.incbin "baserom.gba", 0x008ec7d0, 0x000004e8
	.section .rom.008ecdc0, "a"
	.incbin "baserom.gba", 0x008ecdc0, 0x000006b8
	.section .rom.008edd8a, "a"
	.incbin "baserom.gba", 0x008edd8a, 0x00002b86
	.section .rom.008f19b5, "a"
	.incbin "baserom.gba", 0x008f19b5, 0x00000003
	.section .rom.008f1bb4, "a"
	.incbin "baserom.gba", 0x008f1bb4, 0x0004ac54
	.section .rom.0093c9f5, "a"
	.incbin "baserom.gba", 0x0093c9f5, 0x0000060f
	.section .rom.0093e76e, "a"
	.incbin "baserom.gba", 0x0093e76e, 0x00000002
	.section .rom.0093fca5, "a"
	.incbin "baserom.gba", 0x0093fca5, 0x00000003
	.section .rom.00941afb, "a"
	.incbin "baserom.gba", 0x00941afb, 0x00000001
	.section .rom.00943c0f, "a"
	.incbin "baserom.gba", 0x00943c0f, 0x00000001
	.section .rom.00943de8, "a"
	.incbin "baserom.gba", 0x00943de8, 0x000005a4
	.section .rom.00946a07, "a"
	.incbin "baserom.gba", 0x00946a07, 0x00000001
	.section .rom.0094740f, "a"
	.incbin "baserom.gba", 0x0094740f, 0x00000001
	.section .rom.0094a12e, "a"
	.incbin "baserom.gba", 0x0094a12e, 0x00000002
	.section .rom.0094a300, "a"
	.incbin "baserom.gba", 0x0094a300, 0x00000468
	.section .rom.0094bd5a, "a"
	.incbin "baserom.gba", 0x0094bd5a, 0x00000002
	.section .rom.0094d1ca, "a"
	.incbin "baserom.gba", 0x0094d1ca, 0x00000002
	.section .rom.0094dabe, "a"
	.incbin "baserom.gba", 0x0094dabe, 0x00000002
	.section .rom.0094e3e5, "a"
	.incbin "baserom.gba", 0x0094e3e5, 0x00000003
	.section .rom.0094f2d6, "a"
	.incbin "baserom.gba", 0x0094f2d6, 0x00000002
	.section .rom.0094fec3, "a"
	.incbin "baserom.gba", 0x0094fec3, 0x00000001
	.section .rom.0095009e, "a"
	.incbin "baserom.gba", 0x0095009e, 0x000004ca
	.section .rom.00950dfe, "a"
	.incbin "baserom.gba", 0x00950dfe, 0x00000002
	.section .rom.00953239, "a"
	.incbin "baserom.gba", 0x00953239, 0x00000003
	.section .rom.0095380b, "a"
	.incbin "baserom.gba", 0x0095380b, 0x00000001
	.section .rom.00953cab, "a"
	.incbin "baserom.gba", 0x00953cab, 0x00000001
	.section .rom.00953ffe, "a"
	.incbin "baserom.gba", 0x00953ffe, 0x000002fa
	.section .rom.009543d1, "a"
	.incbin "baserom.gba", 0x009543d1, 0x000002f3
	.section .rom.00954756, "a"
	.incbin "baserom.gba", 0x00954756, 0x00000002
	.section .rom.00955309, "a"
	.incbin "baserom.gba", 0x00955309, 0x00000003
	.section .rom.00955fb6, "a"
	.incbin "baserom.gba", 0x00955fb6, 0x00000002
	.section .rom.00956aba, "a"
	.incbin "baserom.gba", 0x00956aba, 0x00000002
	.section .rom.009579f9, "a"
	.incbin "baserom.gba", 0x009579f9, 0x00000003
	.section .rom.00958235, "a"
	.incbin "baserom.gba", 0x00958235, 0x00000003
	.section .rom.009596da, "a"
	.incbin "baserom.gba", 0x009596da, 0x00000002
	.section .rom.0095a192, "a"
	.incbin "baserom.gba", 0x0095a192, 0x00000002
	.section .rom.0095a4bd, "a"
	.incbin "baserom.gba", 0x0095a4bd, 0x00000003
	.section .rom.0095bb28, "a"
	.incbin "baserom.gba", 0x0095bb28, 0x00000400
	.section .rom.009682e2, "a"
	.incbin "baserom.gba", 0x009682e2, 0x000009fa
	.section .rom.00969143, "a"
	.incbin "baserom.gba", 0x00969143, 0x00000001
	.section .rom.0096934f, "a"
	.incbin "baserom.gba", 0x0096934f, 0x00000001
	.section .rom.00969553, "a"
	.incbin "baserom.gba", 0x00969553, 0x00000001
	.section .rom.009695e2, "a"
	.incbin "baserom.gba", 0x009695e2, 0x00000002
	.section .rom.00969ac4, "a"
	.incbin "baserom.gba", 0x00969ac4, 0x00000070
	.section .rom.00969b8f, "a"
	.incbin "baserom.gba", 0x00969b8f, 0x00000001
	.section .rom.00969bc2, "a"
	.incbin "baserom.gba", 0x00969bc2, 0x00000002
	.section .rom.00969d8e, "a"
	.incbin "baserom.gba", 0x00969d8e, 0x00000332
	.section .rom.0096a179, "a"
	.incbin "baserom.gba", 0x0096a179, 0x00000003
	.section .rom.0096a34e, "a"
	.incbin "baserom.gba", 0x0096a34e, 0x000000be
	.section .rom.0096a55e, "a"
	.incbin "baserom.gba", 0x0096a55e, 0x00000002
	.section .rom.0096a641, "a"
	.incbin "baserom.gba", 0x0096a641, 0x00000003
	.section .rom.0096a711, "a"
	.incbin "baserom.gba", 0x0096a711, 0x00000003
	.section .rom.0096a837, "a"
	.incbin "baserom.gba", 0x0096a837, 0x00000001
	.section .rom.0096a866, "a"
	.incbin "baserom.gba", 0x0096a866, 0x00000002
	.section .rom.0096aaca, "a"
	.incbin "baserom.gba", 0x0096aaca, 0x00000002
	.section .rom.0096ab67, "a"
	.incbin "baserom.gba", 0x0096ab67, 0x00000001
	.section .rom.0096abcc, "a"
	.incbin "baserom.gba", 0x0096abcc, 0x00000cf4
	.section .rom.00970af7, "a"
	.incbin "baserom.gba", 0x00970af7, 0x00000001
	.section .rom.00973a42, "a"
	.incbin "baserom.gba", 0x00973a42, 0x00000002
	.section .rom.00977665, "a"
	.incbin "baserom.gba", 0x00977665, 0x00000003
	.section .rom.009798b1, "a"
	.incbin "baserom.gba", 0x009798b1, 0x00000003
	.section .rom.0097ee4d, "a"
	.incbin "baserom.gba", 0x0097ee4d, 0x00000003
	.section .rom.009826c7, "a"
	.incbin "baserom.gba", 0x009826c7, 0x00000001
	.section .rom.00988531, "a"
	.incbin "baserom.gba", 0x00988531, 0x00000003
	.section .rom.00989605, "a"
	.incbin "baserom.gba", 0x00989605, 0x00000003
	.section .rom.0098a671, "a"
	.incbin "baserom.gba", 0x0098a671, 0x00000003
	.section .rom.0098fccb, "a"
	.incbin "baserom.gba", 0x0098fccb, 0x00000001
	.section .rom.00992e0d, "a"
	.incbin "baserom.gba", 0x00992e0d, 0x00000003
	.section .rom.00994ead, "a"
	.incbin "baserom.gba", 0x00994ead, 0x00000003
	.section .rom.009971cd, "a"
	.incbin "baserom.gba", 0x009971cd, 0x00000003
	.section .rom.00999e46, "a"
	.incbin "baserom.gba", 0x00999e46, 0x00000002
	.section .rom.0099bd41, "a"
	.incbin "baserom.gba", 0x0099bd41, 0x00000003
	.section .rom.009a4285, "a"
	.incbin "baserom.gba", 0x009a4285, 0x00000003
	.section .rom.009acf07, "a"
	.incbin "baserom.gba", 0x009acf07, 0x00000001
	.section .rom.009afa81, "a"
	.incbin "baserom.gba", 0x009afa81, 0x00000003
	.section .rom.009b64be, "a"
	.incbin "baserom.gba", 0x009b64be, 0x00000002
	.section .rom.009b8fb2, "a"
	.incbin "baserom.gba", 0x009b8fb2, 0x00000002
	.section .rom.009baaa2, "a"
	.incbin "baserom.gba", 0x009baaa2, 0x00000002
	.section .rom.009bd3b8, "a"
	.incbin "baserom.gba", 0x009bd3b8, 0x00003440
	.section .rom.009c519e, "a"
	.incbin "baserom.gba", 0x009c519e, 0x00000002
	.section .rom.009c613a, "a"
	.incbin "baserom.gba", 0x009c613a, 0x00000002
	.section .rom.009c8865, "a"
	.incbin "baserom.gba", 0x009c8865, 0x00000003
	.section .rom.009c9f7a, "a"
	.incbin "baserom.gba", 0x009c9f7a, 0x00000002
	.section .rom.009d2793, "a"
	.incbin "baserom.gba", 0x009d2793, 0x00000001
	.section .rom.009d6a5b, "a"
	.incbin "baserom.gba", 0x009d6a5b, 0x00000001
	.section .rom.009de295, "a"
	.incbin "baserom.gba", 0x009de295, 0x00000003
	.section .rom.009dfa7a, "a"
	.incbin "baserom.gba", 0x009dfa7a, 0x00000002
	.section .rom.009e2811, "a"
	.incbin "baserom.gba", 0x009e2811, 0x00000003
	.section .rom.009e385b, "a"
	.incbin "baserom.gba", 0x009e385b, 0x00000001
	.section .rom.009ebdeb, "a"
	.incbin "baserom.gba", 0x009ebdeb, 0x00000001
	.section .rom.009edbe5, "a"
	.incbin "baserom.gba", 0x009edbe5, 0x00000003
	.section .rom.009eefab, "a"
	.incbin "baserom.gba", 0x009eefab, 0x00000001
	.section .rom.009f5ead, "a"
	.incbin "baserom.gba", 0x009f5ead, 0x00000003
	.section .rom.009f81ae, "a"
	.incbin "baserom.gba", 0x009f81ae, 0x00000002
	.section .rom.009fd3fb, "a"
	.incbin "baserom.gba", 0x009fd3fb, 0x00000001
	.section .rom.00a027fa, "a"
	.incbin "baserom.gba", 0x00a027fa, 0x00000002
	.section .rom.00a049ae, "a"
	.incbin "baserom.gba", 0x00a049ae, 0x00000002
	.section .rom.00a08b15, "a"
	.incbin "baserom.gba", 0x00a08b15, 0x00000003
	.section .rom.00a0bd3a, "a"
	.incbin "baserom.gba", 0x00a0bd3a, 0x00000002
	.section .rom.00a0d43e, "a"
	.incbin "baserom.gba", 0x00a0d43e, 0x00000002
	.section .rom.00a14026, "a"
	.incbin "baserom.gba", 0x00a14026, 0x00000002
	.section .rom.00a20606, "a"
	.incbin "baserom.gba", 0x00a20606, 0x00000002
	.section .rom.00a2369a, "a"
	.incbin "baserom.gba", 0x00a2369a, 0x00000002
	.section .rom.00a25f05, "a"
	.incbin "baserom.gba", 0x00a25f05, 0x00000003
	.section .rom.00a279f6, "a"
	.incbin "baserom.gba", 0x00a279f6, 0x00000002
	.section .rom.00a2880e, "a"
	.incbin "baserom.gba", 0x00a2880e, 0x00000002
	.section .rom.00a294f9, "a"
	.incbin "baserom.gba", 0x00a294f9, 0x00000003
	.section .rom.00a329b6, "a"
	.incbin "baserom.gba", 0x00a329b6, 0x00000002
	.section .rom.00a336c1, "a"
	.incbin "baserom.gba", 0x00a336c1, 0x00000003
	.section .rom.00a34925, "a"
	.incbin "baserom.gba", 0x00a34925, 0x00000003
	.section .rom.00a35857, "a"
	.incbin "baserom.gba", 0x00a35857, 0x00000001
	.section .rom.00a364c7, "a"
	.incbin "baserom.gba", 0x00a364c7, 0x00000001
	.section .rom.00a370df, "a"
	.incbin "baserom.gba", 0x00a370df, 0x00000001
	.section .rom.00a37853, "a"
	.incbin "baserom.gba", 0x00a37853, 0x00000001
	.section .rom.00a390e3, "a"
	.incbin "baserom.gba", 0x00a390e3, 0x00000001
	.section .rom.00a39ab1, "a"
	.incbin "baserom.gba", 0x00a39ab1, 0x00000003
	.section .rom.00a3a6cf, "a"
	.incbin "baserom.gba", 0x00a3a6cf, 0x00000001
	.section .rom.00a3cd83, "a"
	.incbin "baserom.gba", 0x00a3cd83, 0x00000001
	.section .rom.00a3f496, "a"
	.incbin "baserom.gba", 0x00a3f496, 0x00000002
	.section .rom.00a443eb, "a"
	.incbin "baserom.gba", 0x00a443eb, 0x00000001
	.section .rom.00a48715, "a"
	.incbin "baserom.gba", 0x00a48715, 0x00000003
	.section .rom.00a49302, "a"
	.incbin "baserom.gba", 0x00a49302, 0x00000002
	.section .rom.00a4deb7, "a"
	.incbin "baserom.gba", 0x00a4deb7, 0x00000001
	.section .rom.00a50221, "a"
	.incbin "baserom.gba", 0x00a50221, 0x00000003
	.section .rom.00a51bc3, "a"
	.incbin "baserom.gba", 0x00a51bc3, 0x00000001
	.section .rom.00a56485, "a"
	.incbin "baserom.gba", 0x00a56485, 0x00000003
	.section .rom.00a59895, "a"
	.incbin "baserom.gba", 0x00a59895, 0x00000003
	.section .rom.00a5d95d, "a"
	.incbin "baserom.gba", 0x00a5d95d, 0x00000003
	.section .rom.00a61026, "a"
	.incbin "baserom.gba", 0x00a61026, 0x00000002
	.section .rom.00a68ff7, "a"
	.incbin "baserom.gba", 0x00a68ff7, 0x00000001
	.section .rom.00a70307, "a"
	.incbin "baserom.gba", 0x00a70307, 0x00000001
	.section .rom.00a75287, "a"
	.incbin "baserom.gba", 0x00a75287, 0x00000001
	.section .rom.00a79d09, "a"
	.incbin "baserom.gba", 0x00a79d09, 0x0000051f
	.section .rom.00a7b485, "a"
	.incbin "baserom.gba", 0x00a7b485, 0x00000003
	.section .rom.00a7b656, "a"
	.incbin "baserom.gba", 0x00a7b656, 0x00000002
	.section .rom.00a7d6f7, "a"
	.incbin "baserom.gba", 0x00a7d6f7, 0x00000001
	.section .rom.00a7e684, "a"
	.incbin "baserom.gba", 0x00a7e684, 0x000022d8
	.section .rom.00a81bb0, "a"
	.incbin "baserom.gba", 0x00a81bb0, 0x0000461c
	.section .rom.00a862d2, "a"
	.incbin "baserom.gba", 0x00a862d2, 0x00000002
	.section .rom.00a874bd, "a"
	.incbin "baserom.gba", 0x00a874bd, 0x00000003
	.section .rom.00a89145, "a"
	.incbin "baserom.gba", 0x00a89145, 0x00000003
	.section .rom.00a89653, "a"
	.incbin "baserom.gba", 0x00a89653, 0x00000001
	.section .rom.00a89793, "a"
	.incbin "baserom.gba", 0x00a89793, 0x00000001
	.section .rom.00a8b416, "a"
	.incbin "baserom.gba", 0x00a8b416, 0x00000002
	.section .rom.00a8ddee, "a"
	.incbin "baserom.gba", 0x00a8ddee, 0x00000002
	.section .rom.00a905b7, "a"
	.incbin "baserom.gba", 0x00a905b7, 0x00000001
	.section .rom.00a91ad7, "a"
	.incbin "baserom.gba", 0x00a91ad7, 0x00000001
	.section .rom.00a9341b, "a"
	.incbin "baserom.gba", 0x00a9341b, 0x00000001
	.section .rom.00a94ec9, "a"
	.incbin "baserom.gba", 0x00a94ec9, 0x00000003
	.section .rom.00a9503d, "a"
	.incbin "baserom.gba", 0x00a9503d, 0x00000003
	.section .rom.00a97e21, "a"
	.incbin "baserom.gba", 0x00a97e21, 0x00000003
	.section .rom.00a9a6cb, "a"
	.incbin "baserom.gba", 0x00a9a6cb, 0x00000001
	.section .rom.00a9eb62, "a"
	.incbin "baserom.gba", 0x00a9eb62, 0x00000002
	.section .rom.00aa1eda, "a"
	.incbin "baserom.gba", 0x00aa1eda, 0x00000002
	.section .rom.00aa49d7, "a"
	.incbin "baserom.gba", 0x00aa49d7, 0x00000001
	.section .rom.00aa6ab5, "a"
	.incbin "baserom.gba", 0x00aa6ab5, 0x00000003
	.section .rom.00aa8d01, "a"
	.incbin "baserom.gba", 0x00aa8d01, 0x00000003
	.section .rom.00aa8e43, "a"
	.incbin "baserom.gba", 0x00aa8e43, 0x00000001
	.section .rom.00aaa516, "a"
	.incbin "baserom.gba", 0x00aaa516, 0x00000002
	.section .rom.00aaa616, "a"
	.incbin "baserom.gba", 0x00aaa616, 0x00000002
	.section .rom.00aac509, "a"
	.incbin "baserom.gba", 0x00aac509, 0x00000003
	.section .rom.00aadee1, "a"
	.incbin "baserom.gba", 0x00aadee1, 0x00000003
	.section .rom.00ab0912, "a"
	.incbin "baserom.gba", 0x00ab0912, 0x00000002
	.section .rom.00ab5b63, "a"
	.incbin "baserom.gba", 0x00ab5b63, 0x00000001
	.section .rom.00ab84a7, "a"
	.incbin "baserom.gba", 0x00ab84a7, 0x00000001
	.section .rom.00ab997a, "a"
	.incbin "baserom.gba", 0x00ab997a, 0x00000002
	.section .rom.00abe745, "a"
	.incbin "baserom.gba", 0x00abe745, 0x00000003
	.section .rom.00ac13ef, "a"
	.incbin "baserom.gba", 0x00ac13ef, 0x00000001
	.section .rom.00ac4627, "a"
	.incbin "baserom.gba", 0x00ac4627, 0x00000001
	.section .rom.00ac47bf, "a"
	.incbin "baserom.gba", 0x00ac47bf, 0x00000001
	.section .rom.00ac75bf, "a"
	.incbin "baserom.gba", 0x00ac75bf, 0x00000001
	.section .rom.00ac8d73, "a"
	.incbin "baserom.gba", 0x00ac8d73, 0x00000001
	.section .rom.00ac9d0e, "a"
	.incbin "baserom.gba", 0x00ac9d0e, 0x00000002
	.section .rom.00acc36f, "a"
	.incbin "baserom.gba", 0x00acc36f, 0x00000001
	.section .rom.00acc516, "a"
	.incbin "baserom.gba", 0x00acc516, 0x00000002
	.section .rom.00acf4cf, "a"
	.incbin "baserom.gba", 0x00acf4cf, 0x00000001
	.section .rom.00ad0175, "a"
	.incbin "baserom.gba", 0x00ad0175, 0x00000003
	.section .rom.00ad1745, "a"
	.incbin "baserom.gba", 0x00ad1745, 0x00000003
	.section .rom.00ad9711, "a"
	.incbin "baserom.gba", 0x00ad9711, 0x00000003
	.section .rom.00adb1f7, "a"
	.incbin "baserom.gba", 0x00adb1f7, 0x00000001
	.section .rom.00adc6e1, "a"
	.incbin "baserom.gba", 0x00adc6e1, 0x00000003
	.section .rom.00ae290b, "a"
	.incbin "baserom.gba", 0x00ae290b, 0x00000001
	.section .rom.00ae2ddd, "a"
	.incbin "baserom.gba", 0x00ae2ddd, 0x00000003
	.section .rom.00ae3f25, "a"
	.incbin "baserom.gba", 0x00ae3f25, 0x00000003
	.section .rom.00ae40d6, "a"
	.incbin "baserom.gba", 0x00ae40d6, 0x00000002
	.section .rom.00aefdc3, "a"
	.incbin "baserom.gba", 0x00aefdc3, 0x00000001
	.section .rom.00aefee3, "a"
	.incbin "baserom.gba", 0x00aefee3, 0x00000001
	.section .rom.00af2283, "a"
	.incbin "baserom.gba", 0x00af2283, 0x00000001
	.section .rom.00af34cf, "a"
	.incbin "baserom.gba", 0x00af34cf, 0x00000001
	.section .rom.00af585f, "a"
	.incbin "baserom.gba", 0x00af585f, 0x00000001
	.section .rom.00af715d, "a"
	.incbin "baserom.gba", 0x00af715d, 0x00000003
	.section .rom.00af814e, "a"
	.incbin "baserom.gba", 0x00af814e, 0x00000002
	.section .rom.00afa6ea, "a"
	.incbin "baserom.gba", 0x00afa6ea, 0x00000002
	.section .rom.00afed12, "a"
	.incbin "baserom.gba", 0x00afed12, 0x00000002
	.section .rom.00aff3d7, "a"
	.incbin "baserom.gba", 0x00aff3d7, 0x00000001
	.section .rom.00b01e6d, "a"
	.incbin "baserom.gba", 0x00b01e6d, 0x00000003
	.section .rom.00b01fbe, "a"
	.incbin "baserom.gba", 0x00b01fbe, 0x00000002
	.section .rom.00b043ce, "a"
	.incbin "baserom.gba", 0x00b043ce, 0x00000002
	.section .rom.00b0654f, "a"
	.incbin "baserom.gba", 0x00b0654f, 0x00000001
	.section .rom.00b0668f, "a"
	.incbin "baserom.gba", 0x00b0668f, 0x00000001
	.section .rom.00b0912e, "a"
	.incbin "baserom.gba", 0x00b0912e, 0x00000002
	.section .rom.00b0927f, "a"
	.incbin "baserom.gba", 0x00b0927f, 0x00000001
	.section .rom.00b0b68e, "a"
	.incbin "baserom.gba", 0x00b0b68e, 0x00000002
	.section .rom.00b0d80f, "a"
	.incbin "baserom.gba", 0x00b0d80f, 0x00000001
	.section .rom.00b0d94f, "a"
	.incbin "baserom.gba", 0x00b0d94f, 0x00000001
	.section .rom.00b10f7e, "a"
	.incbin "baserom.gba", 0x00b10f7e, 0x00000002
	.section .rom.00b134e2, "a"
	.incbin "baserom.gba", 0x00b134e2, 0x00000002
	.section .rom.00b15663, "a"
	.incbin "baserom.gba", 0x00b15663, 0x00000001
	.section .rom.00b157a3, "a"
	.incbin "baserom.gba", 0x00b157a3, 0x00000001
	.section .rom.00b16c53, "a"
	.incbin "baserom.gba", 0x00b16c53, 0x00000001
	.section .rom.00b16d5e, "a"
	.incbin "baserom.gba", 0x00b16d5e, 0x00000002
	.section .rom.00b188b6, "a"
	.incbin "baserom.gba", 0x00b188b6, 0x00000002
	.section .rom.00b19f59, "a"
	.incbin "baserom.gba", 0x00b19f59, 0x00000003
	.section .rom.00b1a11a, "a"
	.incbin "baserom.gba", 0x00b1a11a, 0x00000d9e
	.section .rom.00b1cb0e, "a"
	.incbin "baserom.gba", 0x00b1cb0e, 0x00000002
	.section .rom.00b1e365, "a"
	.incbin "baserom.gba", 0x00b1e365, 0x00000003
	.section .rom.00b1e526, "a"
	.incbin "baserom.gba", 0x00b1e526, 0x00000002
	.section .rom.00b227a1, "a"
	.incbin "baserom.gba", 0x00b227a1, 0x00000003
	.section .rom.00b22962, "a"
	.incbin "baserom.gba", 0x00b22962, 0x00000002
	.section .rom.00b233d9, "a"
	.incbin "baserom.gba", 0x00b233d9, 0x00000003
	.section .rom.00b234e1, "a"
	.incbin "baserom.gba", 0x00b234e1, 0x00000003
	.section .rom.00b251a2, "a"
	.incbin "baserom.gba", 0x00b251a2, 0x00000002
	.section .rom.00b2692f, "a"
	.incbin "baserom.gba", 0x00b2692f, 0x00000001
	.section .rom.00b26be5, "a"
	.incbin "baserom.gba", 0x00b26be5, 0x00000003
	.section .rom.00b286f5, "a"
	.incbin "baserom.gba", 0x00b286f5, 0x00000003
	.section .rom.00b2af82, "a"
	.incbin "baserom.gba", 0x00b2af82, 0x00000002
	.section .rom.00b2d772, "a"
	.incbin "baserom.gba", 0x00b2d772, 0x00000002
	.section .rom.00b2e712, "a"
	.incbin "baserom.gba", 0x00b2e712, 0x00000002
	.section .rom.00b30865, "a"
	.incbin "baserom.gba", 0x00b30865, 0x00000003
	.section .rom.00b33ef2, "a"
	.incbin "baserom.gba", 0x00b33ef2, 0x00000002
	.section .rom.00b35c87, "a"
	.incbin "baserom.gba", 0x00b35c87, 0x00000001
	.section .rom.00b37aa5, "a"
	.incbin "baserom.gba", 0x00b37aa5, 0x00000003
	.section .rom.00b39581, "a"
	.incbin "baserom.gba", 0x00b39581, 0x00000003
	.section .rom.00b3ab6a, "a"
	.incbin "baserom.gba", 0x00b3ab6a, 0x00000002
	.section .rom.00b3d152, "a"
	.incbin "baserom.gba", 0x00b3d152, 0x00000002
	.section .rom.00b3d2cb, "a"
	.incbin "baserom.gba", 0x00b3d2cb, 0x00000001
	.section .rom.00b3e78a, "a"
	.incbin "baserom.gba", 0x00b3e78a, 0x00000002
	.section .rom.00b3ff8e, "a"
	.incbin "baserom.gba", 0x00b3ff8e, 0x00000002
	.section .rom.00b41906, "a"
	.incbin "baserom.gba", 0x00b41906, 0x00000002
	.section .rom.00b42826, "a"
	.incbin "baserom.gba", 0x00b42826, 0x00000002
	.section .rom.00b442d2, "a"
	.incbin "baserom.gba", 0x00b442d2, 0x00000002
	.section .rom.00b443df, "a"
	.incbin "baserom.gba", 0x00b443df, 0x00000001
	.section .rom.00b4650f, "a"
	.incbin "baserom.gba", 0x00b4650f, 0x00000001
	.section .rom.00b480e5, "a"
	.incbin "baserom.gba", 0x00b480e5, 0x00000003
	.section .rom.00b486f7, "a"
	.incbin "baserom.gba", 0x00b486f7, 0x00000001
	.section .rom.00b48837, "a"
	.incbin "baserom.gba", 0x00b48837, 0x00000001
	.section .rom.00b4af37, "a"
	.incbin "baserom.gba", 0x00b4af37, 0x00000001
	.section .rom.00b4c796, "a"
	.incbin "baserom.gba", 0x00b4c796, 0x00000002
	.section .rom.00b4cda7, "a"
	.incbin "baserom.gba", 0x00b4cda7, 0x00000001
	.section .rom.00b4cee7, "a"
	.incbin "baserom.gba", 0x00b4cee7, 0x00000001
	.section .rom.00b4dd62, "a"
	.incbin "baserom.gba", 0x00b4dd62, 0x00000002
	.section .rom.00b4de79, "a"
	.incbin "baserom.gba", 0x00b4de79, 0x00000003
	.section .rom.00b5014f, "a"
	.incbin "baserom.gba", 0x00b5014f, 0x00000001
	.section .rom.00b51da2, "a"
	.incbin "baserom.gba", 0x00b51da2, 0x00000002
	.section .rom.00b523e7, "a"
	.incbin "baserom.gba", 0x00b523e7, 0x00000001
	.section .rom.00b52527, "a"
	.incbin "baserom.gba", 0x00b52527, 0x00000001
	.section .rom.00b5312d, "a"
	.incbin "baserom.gba", 0x00b5312d, 0x00000003
	.section .rom.00b556df, "a"
	.incbin "baserom.gba", 0x00b556df, 0x00000001
	.section .rom.00b5739e, "a"
	.incbin "baserom.gba", 0x00b5739e, 0x00000002
	.section .rom.00b58e6a, "a"
	.incbin "baserom.gba", 0x00b58e6a, 0x00000002
	.section .rom.00b59f3e, "a"
	.incbin "baserom.gba", 0x00b59f3e, 0x00000002
	.section .rom.00b5a0a5, "a"
	.incbin "baserom.gba", 0x00b5a0a5, 0x00000003
	.section .rom.00b5b2be, "a"
	.incbin "baserom.gba", 0x00b5b2be, 0x00000002
	.section .rom.00b5be9d, "a"
	.incbin "baserom.gba", 0x00b5be9d, 0x00000003
	.section .rom.00b5c00a, "a"
	.incbin "baserom.gba", 0x00b5c00a, 0x00000002
	.section .rom.00b5ee5b, "a"
	.incbin "baserom.gba", 0x00b5ee5b, 0x00000001
	.section .rom.00b6161a, "a"
	.incbin "baserom.gba", 0x00b6161a, 0x00000002
	.section .rom.00b676cb, "a"
	.incbin "baserom.gba", 0x00b676cb, 0x00000001
	.section .rom.00b691fb, "a"
	.incbin "baserom.gba", 0x00b691fb, 0x00000001
	.section .rom.00b6b532, "a"
	.incbin "baserom.gba", 0x00b6b532, 0x00000002
	.section .rom.00b6c6e3, "a"
	.incbin "baserom.gba", 0x00b6c6e3, 0x00000001
	.section .rom.00b6e005, "a"
	.incbin "baserom.gba", 0x00b6e005, 0x00000003
	.section .rom.00b6fe91, "a"
	.incbin "baserom.gba", 0x00b6fe91, 0x00000003
	.section .rom.00b72072, "a"
	.incbin "baserom.gba", 0x00b72072, 0x00000002
	.section .rom.00b75a39, "a"
	.incbin "baserom.gba", 0x00b75a39, 0x00000003
	.section .rom.00b75b25, "a"
	.incbin "baserom.gba", 0x00b75b25, 0x00000003
	.section .rom.00b78d5e, "a"
	.incbin "baserom.gba", 0x00b78d5e, 0x00000002
	.section .rom.00b793d5, "a"
	.incbin "baserom.gba", 0x00b793d5, 0x00000003
	.section .rom.00b7d197, "a"
	.incbin "baserom.gba", 0x00b7d197, 0x00000001
	.section .rom.00b7f429, "a"
	.incbin "baserom.gba", 0x00b7f429, 0x00000003
	.section .rom.00b80e46, "a"
	.incbin "baserom.gba", 0x00b80e46, 0x00000002
	.section .rom.00b81f53, "a"
	.incbin "baserom.gba", 0x00b81f53, 0x00000001
	.section .rom.00b850f2, "a"
	.incbin "baserom.gba", 0x00b850f2, 0x00000002
	.section .rom.00b862ea, "a"
	.incbin "baserom.gba", 0x00b862ea, 0x00000002
	.section .rom.00b871ad, "a"
	.incbin "baserom.gba", 0x00b871ad, 0x00000003
	.section .rom.00b8924e, "a"
	.incbin "baserom.gba", 0x00b8924e, 0x00000002
	.section .rom.00b8aca3, "a"
	.incbin "baserom.gba", 0x00b8aca3, 0x00000001
	.section .rom.00b8c16e, "a"
	.incbin "baserom.gba", 0x00b8c16e, 0x00000002
	.section .rom.00b8e372, "a"
	.incbin "baserom.gba", 0x00b8e372, 0x00000002
	.section .rom.00b8e467, "a"
	.incbin "baserom.gba", 0x00b8e467, 0x00000001
	.section .rom.00b8fa3a, "a"
	.incbin "baserom.gba", 0x00b8fa3a, 0x00000002
	.section .rom.00b8fc59, "a"
	.incbin "baserom.gba", 0x00b8fc59, 0x00000003
	.section .rom.00b91d3d, "a"
	.incbin "baserom.gba", 0x00b91d3d, 0x00000003
	.section .rom.00b91f02, "a"
	.incbin "baserom.gba", 0x00b91f02, 0x00000002
	.section .rom.00b93fc5, "a"
	.incbin "baserom.gba", 0x00b93fc5, 0x00000003
	.section .rom.00b940f5, "a"
	.incbin "baserom.gba", 0x00b940f5, 0x00000003
	.section .rom.00b96bae, "a"
	.incbin "baserom.gba", 0x00b96bae, 0x00000002
	.section .rom.00b98706, "a"
	.incbin "baserom.gba", 0x00b98706, 0x00000002
	.section .rom.00b9964d, "a"
	.incbin "baserom.gba", 0x00b9964d, 0x00000003
	.section .rom.00b9b056, "a"
	.incbin "baserom.gba", 0x00b9b056, 0x00000002
	.section .rom.00baadab, "a"
	.incbin "baserom.gba", 0x00baadab, 0x00000001
	.section .rom.00baaefb, "a"
	.incbin "baserom.gba", 0x00baaefb, 0x00000001
	.section .rom.00bada06, "a"
	.incbin "baserom.gba", 0x00bada06, 0x00000002
	.section .rom.00baf60f, "a"
	.incbin "baserom.gba", 0x00baf60f, 0x00000001
	.section .rom.00bb1016, "a"
	.incbin "baserom.gba", 0x00bb1016, 0x00000002
	.section .rom.00bb2d31, "a"
	.incbin "baserom.gba", 0x00bb2d31, 0x00000003
	.section .rom.00bb2e83, "a"
	.incbin "baserom.gba", 0x00bb2e83, 0x00000001
	.section .rom.00bb488a, "a"
	.incbin "baserom.gba", 0x00bb488a, 0x00000002
	.section .rom.00bb8612, "a"
	.incbin "baserom.gba", 0x00bb8612, 0x00000002
	.section .rom.00bb87a5, "a"
	.incbin "baserom.gba", 0x00bb87a5, 0x00000003
	.section .rom.00bbd7af, "a"
	.incbin "baserom.gba", 0x00bbd7af, 0x00000001
	.section .rom.00bbfe31, "a"
	.incbin "baserom.gba", 0x00bbfe31, 0x00000003
	.section .rom.00bc0167, "a"
	.incbin "baserom.gba", 0x00bc0167, 0x00000001
	.section .rom.00bc65bd, "a"
	.incbin "baserom.gba", 0x00bc65bd, 0x00000003
	.section .rom.00bc670a, "a"
	.incbin "baserom.gba", 0x00bc670a, 0x00000002
	.section .rom.00bc858f, "a"
	.incbin "baserom.gba", 0x00bc858f, 0x00000001
	.section .rom.00bc9b36, "a"
	.incbin "baserom.gba", 0x00bc9b36, 0x00000002
	.section .rom.00bcbae1, "a"
	.incbin "baserom.gba", 0x00bcbae1, 0x00000003
	.section .rom.00bcca52, "a"
	.incbin "baserom.gba", 0x00bcca52, 0x00000002
	.section .rom.00bd805b, "a"
	.incbin "baserom.gba", 0x00bd805b, 0x00000001
	.section .rom.00bd8117, "a"
	.incbin "baserom.gba", 0x00bd8117, 0x00000001
	.section .rom.00bd8e56, "a"
	.incbin "baserom.gba", 0x00bd8e56, 0x00000002
	.section .rom.00bd9833, "a"
	.incbin "baserom.gba", 0x00bd9833, 0x00000001
	.section .rom.00bda44b, "a"
	.incbin "baserom.gba", 0x00bda44b, 0x00000001
	.section .rom.00bdc306, "a"
	.incbin "baserom.gba", 0x00bdc306, 0x00000002
	.section .rom.00bdc422, "a"
	.incbin "baserom.gba", 0x00bdc422, 0x00000002
	.section .rom.00bdeca3, "a"
	.incbin "baserom.gba", 0x00bdeca3, 0x00000001
	.section .rom.00be042e, "a"
	.incbin "baserom.gba", 0x00be042e, 0x00000002
	.section .rom.00be435e, "a"
	.incbin "baserom.gba", 0x00be435e, 0x00000002
	.section .rom.00be44a7, "a"
	.incbin "baserom.gba", 0x00be44a7, 0x00000001
	.section .rom.00be6b57, "a"
	.incbin "baserom.gba", 0x00be6b57, 0x00000001
	.section .rom.00be92fa, "a"
	.incbin "baserom.gba", 0x00be92fa, 0x00000002
	.section .rom.00bea085, "a"
	.incbin "baserom.gba", 0x00bea085, 0x00000003
	.section .rom.00becec6, "a"
	.incbin "baserom.gba", 0x00becec6, 0x00000002
	.section .rom.00bef4ed, "a"
	.incbin "baserom.gba", 0x00bef4ed, 0x00000003
	.section .rom.00bf11d9, "a"
	.incbin "baserom.gba", 0x00bf11d9, 0x00000003
	.section .rom.00bf28d9, "a"
	.incbin "baserom.gba", 0x00bf28d9, 0x00000003
	.section .rom.00bf3e55, "a"
	.incbin "baserom.gba", 0x00bf3e55, 0x00000003
	.section .rom.00bf55c7, "a"
	.incbin "baserom.gba", 0x00bf55c7, 0x00000001
	.section .rom.00bf8252, "a"
	.incbin "baserom.gba", 0x00bf8252, 0x00000002
	.section .rom.00bf929e, "a"
	.incbin "baserom.gba", 0x00bf929e, 0x00000002
	.section .rom.00bfa39b, "a"
	.incbin "baserom.gba", 0x00bfa39b, 0x00000001
	.section .rom.00bfbb6f, "a"
	.incbin "baserom.gba", 0x00bfbb6f, 0x00000001
	.section .rom.00bfd033, "a"
	.incbin "baserom.gba", 0x00bfd033, 0x00000001
	.section .rom.00bff566, "a"
	.incbin "baserom.gba", 0x00bff566, 0x00000002
	.section .rom.00bff69a, "a"
	.incbin "baserom.gba", 0x00bff69a, 0x00000002
	.section .rom.00c01e06, "a"
	.incbin "baserom.gba", 0x00c01e06, 0x00000002
	.section .rom.00c03bd9, "a"
	.incbin "baserom.gba", 0x00c03bd9, 0x00000003
	.section .rom.00c074bd, "a"
	.incbin "baserom.gba", 0x00c074bd, 0x00000003
	.section .rom.00c09c23, "a"
	.incbin "baserom.gba", 0x00c09c23, 0x00000001
	.section .rom.00c0b58d, "a"
	.incbin "baserom.gba", 0x00c0b58d, 0x00000003
	.section .rom.00c0ddb6, "a"
	.incbin "baserom.gba", 0x00c0ddb6, 0x00000002
	.section .rom.00c12195, "a"
	.incbin "baserom.gba", 0x00c12195, 0x00000003
	.section .rom.00c1553f, "a"
	.incbin "baserom.gba", 0x00c1553f, 0x00000001
	.section .rom.00c16332, "a"
	.incbin "baserom.gba", 0x00c16332, 0x00000002
	.section .rom.00c170aa, "a"
	.incbin "baserom.gba", 0x00c170aa, 0x00000002
	.section .rom.00c1f1d2, "a"
	.incbin "baserom.gba", 0x00c1f1d2, 0x00000002
	.section .rom.00c25551, "a"
	.incbin "baserom.gba", 0x00c25551, 0x00000003
	.section .rom.00c25f57, "a"
	.incbin "baserom.gba", 0x00c25f57, 0x00000001
	.section .rom.00c27a05, "a"
	.incbin "baserom.gba", 0x00c27a05, 0x00001493
	.section .rom.00c292ff, "a"
	.incbin "baserom.gba", 0x00c292ff, 0x00000001
	.section .rom.00c294be, "a"
	.incbin "baserom.gba", 0x00c294be, 0x00000002
	.section .rom.00c2bcc6, "a"
	.incbin "baserom.gba", 0x00c2bcc6, 0x00000002
	.section .rom.00c2be1e, "a"
	.incbin "baserom.gba", 0x00c2be1e, 0x00000002
	.section .rom.00c2e765, "a"
	.incbin "baserom.gba", 0x00c2e765, 0x00000003
	.section .rom.00c30989, "a"
	.incbin "baserom.gba", 0x00c30989, 0x00000003
	.section .rom.00c31e7e, "a"
	.incbin "baserom.gba", 0x00c31e7e, 0x00000002
	.section .rom.00c340e1, "a"
	.incbin "baserom.gba", 0x00c340e1, 0x00000003
	.section .rom.00c3698f, "a"
	.incbin "baserom.gba", 0x00c3698f, 0x00000001
	.section .rom.00c38922, "a"
	.incbin "baserom.gba", 0x00c38922, 0x00000002
	.section .rom.00c39943, "a"
	.incbin "baserom.gba", 0x00c39943, 0x00000001
	.section .rom.00c39a83, "a"
	.incbin "baserom.gba", 0x00c39a83, 0x00000001
	.section .rom.00c3c6a2, "a"
	.incbin "baserom.gba", 0x00c3c6a2, 0x00000002
	.section .rom.00c3eb26, "a"
	.incbin "baserom.gba", 0x00c3eb26, 0x00000002
	.section .rom.00c40c0d, "a"
	.incbin "baserom.gba", 0x00c40c0d, 0x00000003
	.section .rom.00c42471, "a"
	.incbin "baserom.gba", 0x00c42471, 0x00000003
	.section .rom.00c449d7, "a"
	.incbin "baserom.gba", 0x00c449d7, 0x00000001
	.section .rom.00c44b3d, "a"
	.incbin "baserom.gba", 0x00c44b3d, 0x00000003
	.section .rom.00c47337, "a"
	.incbin "baserom.gba", 0x00c47337, 0x00000001
	.section .rom.00c4936b, "a"
	.incbin "baserom.gba", 0x00c4936b, 0x00000001
	.section .rom.00c4af92, "a"
	.incbin "baserom.gba", 0x00c4af92, 0x00000002
	.section .rom.00c4d9b2, "a"
	.incbin "baserom.gba", 0x00c4d9b2, 0x00000002
	.section .rom.00c4e51d, "a"
	.incbin "baserom.gba", 0x00c4e51d, 0x00000003
	.section .rom.00c4e603, "a"
	.incbin "baserom.gba", 0x00c4e603, 0x00000001
	.section .rom.00c4ff4d, "a"
	.incbin "baserom.gba", 0x00c4ff4d, 0x00000003
	.section .rom.00c518b3, "a"
	.incbin "baserom.gba", 0x00c518b3, 0x00000001
	.section .rom.00c52c81, "a"
	.incbin "baserom.gba", 0x00c52c81, 0x00000003
	.section .rom.00c52d67, "a"
	.incbin "baserom.gba", 0x00c52d67, 0x00000001
	.section .rom.00c538d5, "a"
	.incbin "baserom.gba", 0x00c538d5, 0x00000003
	.section .rom.00c539bb, "a"
	.incbin "baserom.gba", 0x00c539bb, 0x00000001
	.section .rom.00c5471f, "a"
	.incbin "baserom.gba", 0x00c5471f, 0x00000001
	.section .rom.00c54803, "a"
	.incbin "baserom.gba", 0x00c54803, 0x00000001
	.section .rom.00c55009, "a"
	.incbin "baserom.gba", 0x00c55009, 0x00000003
	.section .rom.00c550ef, "a"
	.incbin "baserom.gba", 0x00c550ef, 0x00000001
	.section .rom.00c55c4f, "a"
	.incbin "baserom.gba", 0x00c55c4f, 0x00000001
	.section .rom.00c564cd, "a"
	.incbin "baserom.gba", 0x00c564cd, 0x00000003
	.section .rom.00c565ca, "a"
	.incbin "baserom.gba", 0x00c565ca, 0x00000002
	.section .rom.00c58423, "a"
	.incbin "baserom.gba", 0x00c58423, 0x00000001
	.section .rom.00c59231, "a"
	.incbin "baserom.gba", 0x00c59231, 0x00000003
	.section .rom.00c5a4c5, "a"
	.incbin "baserom.gba", 0x00c5a4c5, 0x00000003
	.section .rom.00c5b4a1, "a"
	.incbin "baserom.gba", 0x00c5b4a1, 0x00000003
	.section .rom.00c5d8a7, "a"
	.incbin "baserom.gba", 0x00c5d8a7, 0x00000001
	.section .rom.00c5d9e7, "a"
	.incbin "baserom.gba", 0x00c5d9e7, 0x00000001
	.section .rom.00c5e0b5, "a"
	.incbin "baserom.gba", 0x00c5e0b5, 0x00000003
	.section .rom.00c5e18b, "a"
	.incbin "baserom.gba", 0x00c5e18b, 0x00000001
	.section .rom.00c5ea01, "a"
	.incbin "baserom.gba", 0x00c5ea01, 0x00000003
	.section .rom.00c5eeb2, "a"
	.incbin "baserom.gba", 0x00c5eeb2, 0x00000002
	.section .rom.00c6032d, "a"
	.incbin "baserom.gba", 0x00c6032d, 0x00000003
	.section .rom.00c60b43, "a"
	.incbin "baserom.gba", 0x00c60b43, 0x00000001
	.section .rom.00c617d9, "a"
	.incbin "baserom.gba", 0x00c617d9, 0x00000003
	.section .rom.00c6197e, "a"
	.incbin "baserom.gba", 0x00c6197e, 0x00000002
	.section .rom.00c64842, "a"
	.incbin "baserom.gba", 0x00c64842, 0x00000002
	.section .rom.00c661c1, "a"
	.incbin "baserom.gba", 0x00c661c1, 0x00000003
	.section .rom.00c6720e, "a"
	.incbin "baserom.gba", 0x00c6720e, 0x00000002
	.section .rom.00c6e953, "a"
	.incbin "baserom.gba", 0x00c6e953, 0x00000001
	.section .rom.00c76147, "a"
	.incbin "baserom.gba", 0x00c76147, 0x00000001
	.section .rom.00c7c741, "a"
	.incbin "baserom.gba", 0x00c7c741, 0x00000003
	.section .rom.00c7f17a, "a"
	.incbin "baserom.gba", 0x00c7f17a, 0x00000002
	.section .rom.00c81a4b, "a"
	.incbin "baserom.gba", 0x00c81a4b, 0x00000001
	.section .rom.00c86297, "a"
	.incbin "baserom.gba", 0x00c86297, 0x00000001
	.section .rom.00c87295, "a"
	.incbin "baserom.gba", 0x00c87295, 0x00000003
	.section .rom.00c873ae, "a"
	.incbin "baserom.gba", 0x00c873ae, 0x00000002
	.section .rom.00c88956, "a"
	.incbin "baserom.gba", 0x00c88956, 0x00000002
	.section .rom.00c8a3b3, "a"
	.incbin "baserom.gba", 0x00c8a3b3, 0x00000001
	.section .rom.00c8a4ee, "a"
	.incbin "baserom.gba", 0x00c8a4ee, 0x00000002
	.section .rom.00c8b9f5, "a"
	.incbin "baserom.gba", 0x00c8b9f5, 0x00000003
	.section .rom.00c8ec7e, "a"
	.incbin "baserom.gba", 0x00c8ec7e, 0x00000002
	.section .rom.00c900c5, "a"
	.incbin "baserom.gba", 0x00c900c5, 0x00000003
	.section .rom.00c92dbd, "a"
	.incbin "baserom.gba", 0x00c92dbd, 0x00000003
	.section .rom.00c951f3, "a"
	.incbin "baserom.gba", 0x00c951f3, 0x00000001
	.section .rom.00c953cf, "a"
	.incbin "baserom.gba", 0x00c953cf, 0x00000001
	.section .rom.00c984e3, "a"
	.incbin "baserom.gba", 0x00c984e3, 0x00000001
	.section .rom.00c99966, "a"
	.incbin "baserom.gba", 0x00c99966, 0x00000002
	.section .rom.00c9a9b5, "a"
	.incbin "baserom.gba", 0x00c9a9b5, 0x00000003
	.section .rom.00c9c81a, "a"
	.incbin "baserom.gba", 0x00c9c81a, 0x00000002
	.section .rom.00c9c9b5, "a"
	.incbin "baserom.gba", 0x00c9c9b5, 0x00000003
	.section .rom.00c9cb13, "a"
	.incbin "baserom.gba", 0x00c9cb13, 0x00000001
	.section .rom.00c9da92, "a"
	.incbin "baserom.gba", 0x00c9da92, 0x00000002
	.section .rom.00c9dbaa, "a"
	.incbin "baserom.gba", 0x00c9dbaa, 0x00000002
	.section .rom.00c9fc3d, "a"
	.incbin "baserom.gba", 0x00c9fc3d, 0x00000003
	.section .rom.00ca1abf, "a"
	.incbin "baserom.gba", 0x00ca1abf, 0x00000001
	.section .rom.00ca3fe5, "a"
	.incbin "baserom.gba", 0x00ca3fe5, 0x00000003
	.section .rom.00ca40f2, "a"
	.incbin "baserom.gba", 0x00ca40f2, 0x00000002
	.section .rom.00ca6185, "a"
	.incbin "baserom.gba", 0x00ca6185, 0x00000003
	.section .rom.00ca965b, "a"
	.incbin "baserom.gba", 0x00ca965b, 0x00000001
	.section .rom.00caa145, "a"
	.incbin "baserom.gba", 0x00caa145, 0x00000003
	.section .rom.00caa2b5, "a"
	.incbin "baserom.gba", 0x00caa2b5, 0x00000003
	.section .rom.00cad1cf, "a"
	.incbin "baserom.gba", 0x00cad1cf, 0x00000001
	.section .rom.00caf15a, "a"
	.incbin "baserom.gba", 0x00caf15a, 0x00000002
	.section .rom.00cb0ac5, "a"
	.incbin "baserom.gba", 0x00cb0ac5, 0x00000003
	.section .rom.00cb65b2, "a"
	.incbin "baserom.gba", 0x00cb65b2, 0x00000002
	.section .rom.00cb6762, "a"
	.incbin "baserom.gba", 0x00cb6762, 0x00000002
	.section .rom.00cbb0da, "a"
	.incbin "baserom.gba", 0x00cbb0da, 0x00000002
	.section .rom.00cbb232, "a"
	.incbin "baserom.gba", 0x00cbb232, 0x00000002
	.section .rom.00cbe7ff, "a"
	.incbin "baserom.gba", 0x00cbe7ff, 0x00000001
	.section .rom.00cc0081, "a"
	.incbin "baserom.gba", 0x00cc0081, 0x00000003
	.section .rom.00cc1da9, "a"
	.incbin "baserom.gba", 0x00cc1da9, 0x00000003
	.section .rom.00cc54ca, "a"
	.incbin "baserom.gba", 0x00cc54ca, 0x00000002
	.section .rom.00cca653, "a"
	.incbin "baserom.gba", 0x00cca653, 0x00000001
	.section .rom.00cccc21, "a"
	.incbin "baserom.gba", 0x00cccc21, 0x00000003
	.section .rom.00cce5ad, "a"
	.incbin "baserom.gba", 0x00cce5ad, 0x00000003
	.section .rom.00ccf6da, "a"
	.incbin "baserom.gba", 0x00ccf6da, 0x00000002
	.section .rom.00cd028a, "a"
	.incbin "baserom.gba", 0x00cd028a, 0x00000002
	.section .rom.00cd1c52, "a"
	.incbin "baserom.gba", 0x00cd1c52, 0x00000002
	.section .rom.00cd1d36, "a"
	.incbin "baserom.gba", 0x00cd1d36, 0x00000002
	.section .rom.00cd4193, "a"
	.incbin "baserom.gba", 0x00cd4193, 0x00000001
	.section .rom.00cd42d3, "a"
	.incbin "baserom.gba", 0x00cd42d3, 0x00000001
	.section .rom.00cd7949, "a"
	.incbin "baserom.gba", 0x00cd7949, 0x00000003
	.section .rom.00cd7aad, "a"
	.incbin "baserom.gba", 0x00cd7aad, 0x00000003
	.section .rom.00cda18d, "a"
	.incbin "baserom.gba", 0x00cda18d, 0x00000003
	.section .rom.00cdd03d, "a"
	.incbin "baserom.gba", 0x00cdd03d, 0x00000003
	.section .rom.00cde5b2, "a"
	.incbin "baserom.gba", 0x00cde5b2, 0x00000002
	.section .rom.00ce26ae, "a"
	.incbin "baserom.gba", 0x00ce26ae, 0x00000002
	.section .rom.00ce4933, "a"
	.incbin "baserom.gba", 0x00ce4933, 0x00000001
	.section .rom.00ce6517, "a"
	.incbin "baserom.gba", 0x00ce6517, 0x00000001
	.section .rom.00ce820d, "a"
	.incbin "baserom.gba", 0x00ce820d, 0x00000003
	.section .rom.00cf5831, "a"
	.incbin "baserom.gba", 0x00cf5831, 0x00000003
	.section .rom.00cf5959, "a"
	.incbin "baserom.gba", 0x00cf5959, 0x00000003
	.section .rom.00cf7b6a, "a"
	.incbin "baserom.gba", 0x00cf7b6a, 0x00000002
	.section .rom.00cf7cfb, "a"
	.incbin "baserom.gba", 0x00cf7cfb, 0x00000001
	.section .rom.00cff1ad, "a"
	.incbin "baserom.gba", 0x00cff1ad, 0x00000003
	.section .rom.00d01983, "a"
	.incbin "baserom.gba", 0x00d01983, 0x00000001
	.section .rom.00d040de, "a"
	.incbin "baserom.gba", 0x00d040de, 0x00000002
	.section .rom.00d04291, "a"
	.incbin "baserom.gba", 0x00d04291, 0x00000003
	.section .rom.00d05995, "a"
	.incbin "baserom.gba", 0x00d05995, 0x00000003
	.section .rom.00d079ef, "a"
	.incbin "baserom.gba", 0x00d079ef, 0x00000001
	.section .rom.00d08d39, "a"
	.incbin "baserom.gba", 0x00d08d39, 0x00000003
	.section .rom.00d0a783, "a"
	.incbin "baserom.gba", 0x00d0a783, 0x00000001
	.section .rom.00d0a8a7, "a"
	.incbin "baserom.gba", 0x00d0a8a7, 0x00000001
	.section .rom.00d0ad79, "a"
	.incbin "baserom.gba", 0x00d0ad79, 0x00000003
	.section .rom.00d0cf5d, "a"
	.incbin "baserom.gba", 0x00d0cf5d, 0x00000003
	.section .rom.00d0d431, "a"
	.incbin "baserom.gba", 0x00d0d431, 0x00000003
	.section .rom.00d0f967, "a"
	.incbin "baserom.gba", 0x00d0f967, 0x00000001
	.section .rom.00d0faef, "a"
	.incbin "baserom.gba", 0x00d0faef, 0x00000001
	.section .rom.00d14391, "a"
	.incbin "baserom.gba", 0x00d14391, 0x00000003
	.section .rom.00d144c7, "a"
	.incbin "baserom.gba", 0x00d144c7, 0x00000001
	.section .rom.00d16133, "a"
	.incbin "baserom.gba", 0x00d16133, 0x00000001
	.section .rom.00d172eb, "a"
	.incbin "baserom.gba", 0x00d172eb, 0x00000001
	.section .rom.00d18039, "a"
	.incbin "baserom.gba", 0x00d18039, 0x00000003
	.section .rom.00d191c2, "a"
	.incbin "baserom.gba", 0x00d191c2, 0x00000002
	.section .rom.00d1acbb, "a"
	.incbin "baserom.gba", 0x00d1acbb, 0x00000001
	.section .rom.00d1c0c7, "a"
	.incbin "baserom.gba", 0x00d1c0c7, 0x00000001
	.section .rom.00d1c61b, "a"
	.incbin "baserom.gba", 0x00d1c61b, 0x00000001
	.section .rom.00d1cc55, "a"
	.incbin "baserom.gba", 0x00d1cc55, 0x00000003
	.section .rom.00d21fa5, "a"
	.incbin "baserom.gba", 0x00d21fa5, 0x00000003
	.section .rom.00d24606, "a"
	.incbin "baserom.gba", 0x00d24606, 0x00000002
	.section .rom.00d2479b, "a"
	.incbin "baserom.gba", 0x00d2479b, 0x00000001
	.section .rom.00d2634f, "a"
	.incbin "baserom.gba", 0x00d2634f, 0x00000001
	.section .rom.00d264af, "a"
	.incbin "baserom.gba", 0x00d264af, 0x00000001
	.section .rom.00d28f53, "a"
	.incbin "baserom.gba", 0x00d28f53, 0x000021b1
	.section .rom.00d2cf7a, "a"
	.incbin "baserom.gba", 0x00d2cf7a, 0x00000002
	.section .rom.00d2d0bb, "a"
	.incbin "baserom.gba", 0x00d2d0bb, 0x00000001
	.section .rom.00d34033, "a"
	.incbin "baserom.gba", 0x00d34033, 0x00000001
	.section .rom.00d34171, "a"
	.incbin "baserom.gba", 0x00d34171, 0x00000003
	.section .rom.00d36145, "a"
	.incbin "baserom.gba", 0x00d36145, 0x00000003
	.section .rom.00d36239, "a"
	.incbin "baserom.gba", 0x00d36239, 0x00000003
	.section .rom.00d37319, "a"
	.incbin "baserom.gba", 0x00d37319, 0x00000003
	.section .rom.00d391f3, "a"
	.incbin "baserom.gba", 0x00d391f3, 0x00000001
	.section .rom.00d3a1ab, "a"
	.incbin "baserom.gba", 0x00d3a1ab, 0x00000001
	.section .rom.00d3c0f2, "a"
	.incbin "baserom.gba", 0x00d3c0f2, 0x00000002
	.section .rom.00d3dcfd, "a"
	.incbin "baserom.gba", 0x00d3dcfd, 0x00000003
	.section .rom.00d3de49, "a"
	.incbin "baserom.gba", 0x00d3de49, 0x00000003
	.section .rom.00d3ff07, "a"
	.incbin "baserom.gba", 0x00d3ff07, 0x00000001
	.section .rom.00d43ff6, "a"
	.incbin "baserom.gba", 0x00d43ff6, 0x00000002
	.section .rom.00d47da8, "a"
	.incbin "baserom.gba", 0x00d47da8, 0x00007aa8
	.section .rom.00d50f9d, "a"
	.incbin "baserom.gba", 0x00d50f9d, 0x00000003
	.section .rom.00d510ca, "a"
	.incbin "baserom.gba", 0x00d510ca, 0x00000002
	.section .rom.00d5803d, "a"
	.incbin "baserom.gba", 0x00d5803d, 0x00000003
	.section .rom.00d58142, "a"
	.incbin "baserom.gba", 0x00d58142, 0x00000002
	.section .rom.00d58282, "a"
	.incbin "baserom.gba", 0x00d58282, 0x00000002
	.section .rom.00d59c4e, "a"
	.incbin "baserom.gba", 0x00d59c4e, 0x00000002
	.section .rom.00d59d45, "a"
	.incbin "baserom.gba", 0x00d59d45, 0x00000003
	.section .rom.00d5cb99, "a"
	.incbin "baserom.gba", 0x00d5cb99, 0x00000003
	.section .rom.00d5cd42, "a"
	.incbin "baserom.gba", 0x00d5cd42, 0x00000002
	.section .rom.00d5f95e, "a"
	.incbin "baserom.gba", 0x00d5f95e, 0x00000002
	.section .rom.00d62691, "a"
	.incbin "baserom.gba", 0x00d62691, 0x00000003
	.section .rom.00d6437b, "a"
	.incbin "baserom.gba", 0x00d6437b, 0x00000001
	.section .rom.00d67bf5, "a"
	.incbin "baserom.gba", 0x00d67bf5, 0x00000003
	.section .rom.00d67d4d, "a"
	.incbin "baserom.gba", 0x00d67d4d, 0x00000003
	.section .rom.00d6a291, "a"
	.incbin "baserom.gba", 0x00d6a291, 0x00000003
	.section .rom.00d6e156, "a"
	.incbin "baserom.gba", 0x00d6e156, 0x00000002
	.section .rom.00d70286, "a"
	.incbin "baserom.gba", 0x00d70286, 0x00000002
	.section .rom.00d74492, "a"
	.incbin "baserom.gba", 0x00d74492, 0x00000002
	.section .rom.00d74666, "a"
	.incbin "baserom.gba", 0x00d74666, 0x00000002
	.section .rom.00d766a6, "a"
	.incbin "baserom.gba", 0x00d766a6, 0x00000002
	.section .rom.00d77a9d, "a"
	.incbin "baserom.gba", 0x00d77a9d, 0x00000003
	.section .rom.00d785da, "a"
	.incbin "baserom.gba", 0x00d785da, 0x00000002
	.section .rom.00d79745, "a"
	.incbin "baserom.gba", 0x00d79745, 0x00000003
	.section .rom.00d7a7f1, "a"
	.incbin "baserom.gba", 0x00d7a7f1, 0x00000003
	.section .rom.00d7a99d, "a"
	.incbin "baserom.gba", 0x00d7a99d, 0x00000003
	.section .rom.00d7c4aa, "a"
	.incbin "baserom.gba", 0x00d7c4aa, 0x00000002
	.section .rom.00d7dec7, "a"
	.incbin "baserom.gba", 0x00d7dec7, 0x00000001
	.section .rom.00d80399, "a"
	.incbin "baserom.gba", 0x00d80399, 0x00000003
	.section .rom.00d816fb, "a"
	.incbin "baserom.gba", 0x00d816fb, 0x00000001
	.section .rom.00d825af, "a"
	.incbin "baserom.gba", 0x00d825af, 0x00000001
	.section .rom.00d83b9d, "a"
	.incbin "baserom.gba", 0x00d83b9d, 0x00000003
	.section .rom.00d850e3, "a"
	.incbin "baserom.gba", 0x00d850e3, 0x00000001
	.section .rom.00d85ead, "a"
	.incbin "baserom.gba", 0x00d85ead, 0x00000003
	.section .rom.00d8601b, "a"
	.incbin "baserom.gba", 0x00d8601b, 0x00000001
	.section .rom.00d86fda, "a"
	.incbin "baserom.gba", 0x00d86fda, 0x00000002
	.section .rom.00d87aa3, "a"
	.incbin "baserom.gba", 0x00d87aa3, 0x00000001
	.section .rom.00d8863d, "a"
	.incbin "baserom.gba", 0x00d8863d, 0x00000003
	.section .rom.00d8878d, "a"
	.incbin "baserom.gba", 0x00d8878d, 0x00000003
	.section .rom.00d8ca3b, "a"
	.incbin "baserom.gba", 0x00d8ca3b, 0x00000001
	.section .rom.00d8e4d5, "a"
	.incbin "baserom.gba", 0x00d8e4d5, 0x00000003
	.section .rom.00d8feab, "a"
	.incbin "baserom.gba", 0x00d8feab, 0x00000001
	.section .rom.00d9002a, "a"
	.incbin "baserom.gba", 0x00d9002a, 0x00000002
	.section .rom.00d93c5a, "a"
	.incbin "baserom.gba", 0x00d93c5a, 0x00000002
	.section .rom.00d959b7, "a"
	.incbin "baserom.gba", 0x00d959b7, 0x00000001
	.section .rom.00d98fe1, "a"
	.incbin "baserom.gba", 0x00d98fe1, 0x00000003
	.section .rom.00d99132, "a"
	.incbin "baserom.gba", 0x00d99132, 0x00000002
	.section .rom.00d9ed2e, "a"
	.incbin "baserom.gba", 0x00d9ed2e, 0x00000002
	.section .rom.00da166b, "a"
	.incbin "baserom.gba", 0x00da166b, 0x00000001
	.section .rom.00da2fad, "a"
	.incbin "baserom.gba", 0x00da2fad, 0x00000003
	.section .rom.00da3116, "a"
	.incbin "baserom.gba", 0x00da3116, 0x00000002
	.section .rom.00da9f15, "a"
	.incbin "baserom.gba", 0x00da9f15, 0x00000003
	.section .rom.00daa4f3, "a"
	.incbin "baserom.gba", 0x00daa4f3, 0x00000001
	.section .rom.00dab3fb, "a"
	.incbin "baserom.gba", 0x00dab3fb, 0x00000001
	.section .rom.00dadc43, "a"
	.incbin "baserom.gba", 0x00dadc43, 0x00000001
	.section .rom.00daf262, "a"
	.incbin "baserom.gba", 0x00daf262, 0x00000002
	.section .rom.00db0bc3, "a"
	.incbin "baserom.gba", 0x00db0bc3, 0x00000001
	.section .rom.00db39ab, "a"
	.incbin "baserom.gba", 0x00db39ab, 0x00000001
	.section .rom.00db3b26, "a"
	.incbin "baserom.gba", 0x00db3b26, 0x00000002
	.section .rom.00dc2cef, "a"
	.incbin "baserom.gba", 0x00dc2cef, 0x00000001
	.section .rom.00dc2e57, "a"
	.incbin "baserom.gba", 0x00dc2e57, 0x00000001
	.section .rom.00dc547a, "a"
	.incbin "baserom.gba", 0x00dc547a, 0x00000002
	.section .rom.00dc55bd, "a"
	.incbin "baserom.gba", 0x00dc55bd, 0x00000003
	.section .rom.00dc6e7a, "a"
	.incbin "baserom.gba", 0x00dc6e7a, 0x00000002
	.section .rom.00dc9dde, "a"
	.incbin "baserom.gba", 0x00dc9dde, 0x00000002
	.section .rom.00dcc776, "a"
	.incbin "baserom.gba", 0x00dcc776, 0x00000002
	.section .rom.00dd10ce, "a"
	.incbin "baserom.gba", 0x00dd10ce, 0x00000002
	.section .rom.00dd36f7, "a"
	.incbin "baserom.gba", 0x00dd36f7, 0x00000001
	.section .rom.00dd38cb, "a"
	.incbin "baserom.gba", 0x00dd38cb, 0x00000001
	.section .rom.00dd6bea, "a"
	.incbin "baserom.gba", 0x00dd6bea, 0x00000002
	.section .rom.00dd6dc1, "a"
	.incbin "baserom.gba", 0x00dd6dc1, 0x00000003
	.section .rom.00dda135, "a"
	.incbin "baserom.gba", 0x00dda135, 0x00000003
	.section .rom.00ddb889, "a"
	.incbin "baserom.gba", 0x00ddb889, 0x00000003
	.section .rom.00ddc817, "a"
	.incbin "baserom.gba", 0x00ddc817, 0x00000001
	.section .rom.00dde18d, "a"
	.incbin "baserom.gba", 0x00dde18d, 0x00000003
	.section .rom.00dde2c7, "a"
	.incbin "baserom.gba", 0x00dde2c7, 0x00000001
	.section .rom.00ddfe09, "a"
	.incbin "baserom.gba", 0x00ddfe09, 0x00000003
	.section .rom.00de33e1, "a"
	.incbin "baserom.gba", 0x00de33e1, 0x00000003
	.section .rom.00de406b, "a"
	.incbin "baserom.gba", 0x00de406b, 0x00000001
	.section .rom.00de41ab, "a"
	.incbin "baserom.gba", 0x00de41ab, 0x00000001
	.section .rom.00de5f6f, "a"
	.incbin "baserom.gba", 0x00de5f6f, 0x00000001
	.section .rom.00de60e2, "a"
	.incbin "baserom.gba", 0x00de60e2, 0x00000002
	.section .rom.00de854a, "a"
	.incbin "baserom.gba", 0x00de854a, 0x00000002
	.section .rom.00de9ee6, "a"
	.incbin "baserom.gba", 0x00de9ee6, 0x00000002
	.section .rom.00debd1b, "a"
	.incbin "baserom.gba", 0x00debd1b, 0x00000001
	.section .rom.00dec7d2, "a"
	.incbin "baserom.gba", 0x00dec7d2, 0x00000002
	.section .rom.00dee1ee, "a"
	.incbin "baserom.gba", 0x00dee1ee, 0x00000002
	.section .rom.00df0eba, "a"
	.incbin "baserom.gba", 0x00df0eba, 0x00000002
	.section .rom.00df1095, "a"
	.incbin "baserom.gba", 0x00df1095, 0x00000003
	.section .rom.00df2bcb, "a"
	.incbin "baserom.gba", 0x00df2bcb, 0x00000001
	.section .rom.00df5c95, "a"
	.incbin "baserom.gba", 0x00df5c95, 0x00000003
	.section .rom.00df6e51, "a"
	.incbin "baserom.gba", 0x00df6e51, 0x00000003
	.section .rom.00df7039, "a"
	.incbin "baserom.gba", 0x00df7039, 0x00000003
	.section .rom.00df71cb, "a"
	.incbin "baserom.gba", 0x00df71cb, 0x00000001
	.section .rom.00df7f52, "a"
	.incbin "baserom.gba", 0x00df7f52, 0x00000002
	.section .rom.00df80e7, "a"
	.incbin "baserom.gba", 0x00df80e7, 0x00000001
	.section .rom.00dfa19b, "a"
	.incbin "baserom.gba", 0x00dfa19b, 0x00000001
	.section .rom.00e0221e, "a"
	.incbin "baserom.gba", 0x00e0221e, 0x00000002
	.section .rom.00e0235f, "a"
	.incbin "baserom.gba", 0x00e0235f, 0x00000001
	.section .rom.00e066e7, "a"
	.incbin "baserom.gba", 0x00e066e7, 0x00000001
	.section .rom.00e06e06, "a"
	.incbin "baserom.gba", 0x00e06e06, 0x00000002
	.section .rom.00e081c5, "a"
	.incbin "baserom.gba", 0x00e081c5, 0x00000003
	.section .rom.00e0c2df, "a"
	.incbin "baserom.gba", 0x00e0c2df, 0x00000001
	.section .rom.00e0d35f, "a"
	.incbin "baserom.gba", 0x00e0d35f, 0x00000001
	.section .rom.00e0e3e1, "a"
	.incbin "baserom.gba", 0x00e0e3e1, 0x00000003
	.section .rom.00e0ef52, "a"
	.incbin "baserom.gba", 0x00e0ef52, 0x00000002
	.section .rom.00e0fea3, "a"
	.incbin "baserom.gba", 0x00e0fea3, 0x00000001
	.section .rom.00e10021, "a"
	.incbin "baserom.gba", 0x00e10021, 0x00000003
	.section .rom.00e1357b, "a"
	.incbin "baserom.gba", 0x00e1357b, 0x00000001
	.section .rom.00e14bae, "a"
	.incbin "baserom.gba", 0x00e14bae, 0x00000002
	.section .rom.00e14cef, "a"
	.incbin "baserom.gba", 0x00e14cef, 0x00000001
	.section .rom.00e1763a, "a"
	.incbin "baserom.gba", 0x00e1763a, 0x00000002
	.section .rom.00e1ffe2, "a"
	.incbin "baserom.gba", 0x00e1ffe2, 0x00000fde
	.section .rom.00e22b8d, "a"
	.incbin "baserom.gba", 0x00e22b8d, 0x00000003
	.section .rom.00e24acd, "a"
	.incbin "baserom.gba", 0x00e24acd, 0x00000003
	.section .rom.00e25f41, "a"
	.incbin "baserom.gba", 0x00e25f41, 0x00000003
	.section .rom.00e27bfb, "a"
	.incbin "baserom.gba", 0x00e27bfb, 0x00000001
	.section .rom.00e34caa, "a"
	.incbin "baserom.gba", 0x00e34caa, 0x00000002
	.section .rom.00e34de9, "a"
	.incbin "baserom.gba", 0x00e34de9, 0x00000003
	.section .rom.00e36d0e, "a"
	.incbin "baserom.gba", 0x00e36d0e, 0x00000002
	.section .rom.00e36e37, "a"
	.incbin "baserom.gba", 0x00e36e37, 0x00000001
	.section .rom.00e39007, "a"
	.incbin "baserom.gba", 0x00e39007, 0x00000001
	.section .rom.00e3b5d7, "a"
	.incbin "baserom.gba", 0x00e3b5d7, 0x00000001
	.section .rom.00e3d8e9, "a"
	.incbin "baserom.gba", 0x00e3d8e9, 0x00000003
	.section .rom.00e3e15d, "a"
	.incbin "baserom.gba", 0x00e3e15d, 0x00000003
	.section .rom.00e3e29f, "a"
	.incbin "baserom.gba", 0x00e3e29f, 0x00000001
	.section .rom.00e41355, "a"
	.incbin "baserom.gba", 0x00e41355, 0x00000003
	.section .rom.00e42a3b, "a"
	.incbin "baserom.gba", 0x00e42a3b, 0x00000001
	.section .rom.00e44603, "a"
	.incbin "baserom.gba", 0x00e44603, 0x00000001
	.section .rom.00e4617e, "a"
	.incbin "baserom.gba", 0x00e4617e, 0x00000002
	.section .rom.00e465c9, "a"
	.incbin "baserom.gba", 0x00e465c9, 0x00000003
	.section .rom.00e4872a, "a"
	.incbin "baserom.gba", 0x00e4872a, 0x00000002
	.section .rom.00e48843, "a"
	.incbin "baserom.gba", 0x00e48843, 0x00000001
	.section .rom.00e4966d, "a"
	.incbin "baserom.gba", 0x00e4966d, 0x00000003
	.section .rom.00e497cf, "a"
	.incbin "baserom.gba", 0x00e497cf, 0x00000001
	.section .rom.00e4e037, "a"
	.incbin "baserom.gba", 0x00e4e037, 0x00000001
	.section .rom.00e506f5, "a"
	.incbin "baserom.gba", 0x00e506f5, 0x00000003
	.section .rom.00e50cd3, "a"
	.incbin "baserom.gba", 0x00e50cd3, 0x00000001
	.section .rom.00e52c66, "a"
	.incbin "baserom.gba", 0x00e52c66, 0x00000002
	.section .rom.00e54446, "a"
	.incbin "baserom.gba", 0x00e54446, 0x00000002
	.section .rom.00e545aa, "a"
	.incbin "baserom.gba", 0x00e545aa, 0x00000002
	.section .rom.00e56da2, "a"
	.incbin "baserom.gba", 0x00e56da2, 0x00000002
	.section .rom.00e56f5b, "a"
	.incbin "baserom.gba", 0x00e56f5b, 0x00000001
	.section .rom.00e570e3, "a"
	.incbin "baserom.gba", 0x00e570e3, 0x00000001
	.section .rom.00e58905, "a"
	.incbin "baserom.gba", 0x00e58905, 0x00000003
	.section .rom.00e594ee, "a"
	.incbin "baserom.gba", 0x00e594ee, 0x00000002
	.section .rom.00e5aa27, "a"
	.incbin "baserom.gba", 0x00e5aa27, 0x00000001
	.section .rom.00e5abc2, "a"
	.incbin "baserom.gba", 0x00e5abc2, 0x00000002
	.section .rom.00e5ce6e, "a"
	.incbin "baserom.gba", 0x00e5ce6e, 0x00000002
	.section .rom.00e5f035, "a"
	.incbin "baserom.gba", 0x00e5f035, 0x00000003
	.section .rom.00e60733, "a"
	.incbin "baserom.gba", 0x00e60733, 0x00000001
	.section .rom.00e61257, "a"
	.incbin "baserom.gba", 0x00e61257, 0x00000001
	.section .rom.00e660fa, "a"
	.incbin "baserom.gba", 0x00e660fa, 0x00000002
	.section .rom.00e684a1, "a"
	.incbin "baserom.gba", 0x00e684a1, 0x00000003
	.section .rom.00e68b8a, "a"
	.incbin "baserom.gba", 0x00e68b8a, 0x00000002
	.section .rom.00e69776, "a"
	.incbin "baserom.gba", 0x00e69776, 0x00000ac6
	.section .rom.00e6b7eb, "a"
	.incbin "baserom.gba", 0x00e6b7eb, 0x00000001
	.section .rom.00e6b99e, "a"
	.incbin "baserom.gba", 0x00e6b99e, 0x00000002
	.section .rom.00e6dbdd, "a"
	.incbin "baserom.gba", 0x00e6dbdd, 0x00000003
	.section .rom.00e6e703, "a"
	.incbin "baserom.gba", 0x00e6e703, 0x00000001
	.section .rom.00e79194, "a"
	.incbin "baserom.gba", 0x00e79194, 0x000007d0
	.section .rom.00e7c62d, "a"
	.incbin "baserom.gba", 0x00e7c62d, 0x00000003
	.section .rom.00e7c7a7, "a"
	.incbin "baserom.gba", 0x00e7c7a7, 0x00000001
	.section .rom.00e7c91e, "a"
	.incbin "baserom.gba", 0x00e7c91e, 0x00000002
	.section .rom.00e7cab6, "a"
	.incbin "baserom.gba", 0x00e7cab6, 0x00000002
	.section .rom.00e7cc47, "a"
	.incbin "baserom.gba", 0x00e7cc47, 0x00000001
	.section .rom.00e7eef3, "a"
	.incbin "baserom.gba", 0x00e7eef3, 0x00000001
	.section .rom.00e7f00b, "a"
	.incbin "baserom.gba", 0x00e7f00b, 0x00000001
	.section .rom.00e80e0d, "a"
	.incbin "baserom.gba", 0x00e80e0d, 0x00000003
	.section .rom.00e828d2, "a"
	.incbin "baserom.gba", 0x00e828d2, 0x00000002
	.section .rom.00e82ffa, "a"
	.incbin "baserom.gba", 0x00e82ffa, 0x00000002
	.section .rom.00e843f6, "a"
	.incbin "baserom.gba", 0x00e843f6, 0x00000002
	.section .rom.00e852ca, "a"
	.incbin "baserom.gba", 0x00e852ca, 0x00000002
	.section .rom.00e8540e, "a"
	.incbin "baserom.gba", 0x00e8540e, 0x00000002
	.section .rom.00e877bf, "a"
	.incbin "baserom.gba", 0x00e877bf, 0x00000001
	.section .rom.00e89fed, "a"
	.incbin "baserom.gba", 0x00e89fed, 0x00000003
	.section .rom.00e8a3ea, "a"
	.incbin "baserom.gba", 0x00e8a3ea, 0x00000002
	.section .rom.00e8b88a, "a"
	.incbin "baserom.gba", 0x00e8b88a, 0x00000002
	.section .rom.00e8b9de, "a"
	.incbin "baserom.gba", 0x00e8b9de, 0x00000002
	.section .rom.00e8f32b, "a"
	.incbin "baserom.gba", 0x00e8f32b, 0x00000001
	.section .rom.00e90796, "a"
	.incbin "baserom.gba", 0x00e90796, 0x00000002
	.section .rom.00e91c6f, "a"
	.incbin "baserom.gba", 0x00e91c6f, 0x00000001
	.section .rom.00e91dc2, "a"
	.incbin "baserom.gba", 0x00e91dc2, 0x00000002
	.section .rom.00e957cf, "a"
	.incbin "baserom.gba", 0x00e957cf, 0x00000001
	.section .rom.00e969a7, "a"
	.incbin "baserom.gba", 0x00e969a7, 0x00000001
	.section .rom.00e981b2, "a"
	.incbin "baserom.gba", 0x00e981b2, 0x00000002
	.section .rom.00e98302, "a"
	.incbin "baserom.gba", 0x00e98302, 0x00000002
	.section .rom.00e9aa0f, "a"
	.incbin "baserom.gba", 0x00e9aa0f, 0x00000001
	.section .rom.00e9ab76, "a"
	.incbin "baserom.gba", 0x00e9ab76, 0x00000002
	.section .rom.00e9db37, "a"
	.incbin "baserom.gba", 0x00e9db37, 0x00000001
	.section .rom.00e9eda5, "a"
	.incbin "baserom.gba", 0x00e9eda5, 0x00000003
	.section .rom.00ea1932, "a"
	.incbin "baserom.gba", 0x00ea1932, 0x00000002
	.section .rom.00ea2319, "a"
	.incbin "baserom.gba", 0x00ea2319, 0x00000003
	.section .rom.00ea2cba, "a"
	.incbin "baserom.gba", 0x00ea2cba, 0x00000002
	.section .rom.00ea2e2e, "a"
	.incbin "baserom.gba", 0x00ea2e2e, 0x00000002
	.section .rom.00ea493f, "a"
	.incbin "baserom.gba", 0x00ea493f, 0x00000001
	.section .rom.00ea4a61, "a"
	.incbin "baserom.gba", 0x00ea4a61, 0x00000003
	.section .rom.00ea755f, "a"
	.incbin "baserom.gba", 0x00ea755f, 0x00000001
	.section .rom.00ea8356, "a"
	.incbin "baserom.gba", 0x00ea8356, 0x00000002
	.section .rom.00eaa2f5, "a"
	.incbin "baserom.gba", 0x00eaa2f5, 0x00000003
	.section .rom.00eab625, "a"
	.incbin "baserom.gba", 0x00eab625, 0x00000003
	.section .rom.00eab779, "a"
	.incbin "baserom.gba", 0x00eab779, 0x00000003
	.section .rom.00eac32b, "a"
	.incbin "baserom.gba", 0x00eac32b, 0x00000001
	.section .rom.00eadb05, "a"
	.incbin "baserom.gba", 0x00eadb05, 0x00000003
	.section .rom.00eaec65, "a"
	.incbin "baserom.gba", 0x00eaec65, 0x00000003
	.section .rom.00eafd83, "a"
	.incbin "baserom.gba", 0x00eafd83, 0x00000001
	.section .rom.00eaff8a, "a"
	.incbin "baserom.gba", 0x00eaff8a, 0x00000002
	.section .rom.00eb0c7f, "a"
	.incbin "baserom.gba", 0x00eb0c7f, 0x00000001
	.section .rom.00eb3421, "a"
	.incbin "baserom.gba", 0x00eb3421, 0x00000003
	.section .rom.00eb4b96, "a"
	.incbin "baserom.gba", 0x00eb4b96, 0x00000002
	.section .rom.00eb5e53, "a"
	.incbin "baserom.gba", 0x00eb5e53, 0x00000001
	.section .rom.00eb6696, "a"
	.incbin "baserom.gba", 0x00eb6696, 0x0000060e
	.section .rom.00eb721b, "a"
	.incbin "baserom.gba", 0x00eb721b, 0x00000001
	.section .rom.00eb735b, "a"
	.incbin "baserom.gba", 0x00eb735b, 0x00000001
	.section .rom.00eb749b, "a"
	.incbin "baserom.gba", 0x00eb749b, 0x00000001
	.section .rom.00eb7ffd, "a"
	.incbin "baserom.gba", 0x00eb7ffd, 0x00000003
	.section .rom.00eba7a1, "a"
	.incbin "baserom.gba", 0x00eba7a1, 0x00000003
	.section .rom.00ebbf16, "a"
	.incbin "baserom.gba", 0x00ebbf16, 0x00000002
	.section .rom.00ebd1d3, "a"
	.incbin "baserom.gba", 0x00ebd1d3, 0x00000001
	.section .rom.00ebda16, "a"
	.incbin "baserom.gba", 0x00ebda16, 0x00000002
	.section .rom.00ebdfb5, "a"
	.incbin "baserom.gba", 0x00ebdfb5, 0x0000051f
	.section .rom.00ebebe9, "a"
	.incbin "baserom.gba", 0x00ebebe9, 0x0000051f
	.section .rom.00ebfcc9, "a"
	.incbin "baserom.gba", 0x00ebfcc9, 0x0000051f
	.section .rom.00ec07b5, "a"
	.incbin "baserom.gba", 0x00ec07b5, 0x0000051f
	.section .rom.00ec1651, "a"
	.incbin "baserom.gba", 0x00ec1651, 0x0000051f
	.section .rom.00ec1f99, "a"
	.incbin "baserom.gba", 0x00ec1f99, 0x0000051f
	.section .rom.00ec2ae1, "a"
	.incbin "baserom.gba", 0x00ec2ae1, 0x0000051f
	.section .rom.00ec3729, "a"
	.incbin "baserom.gba", 0x00ec3729, 0x0013c8d7
