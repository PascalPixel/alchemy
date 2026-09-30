@ tla-de's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.incbin "baserom.gba", 0x00002064, 0x00058770
	.section .rom.0005c3d4, "a"
	.incbin "baserom.gba", 0x0005c3d4, 0x00164f3c
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
	.section .rom.0068a0ca, "a"
	.incbin "baserom.gba", 0x0068a0ca, 0x0000d96e
	.section .rom.006a45cd, "a"
	.incbin "baserom.gba", 0x006a45cd, 0x00004467
	.section .rom.006a9fa2, "a"
	.incbin "baserom.gba", 0x006a9fa2, 0x0002476a
	.section .rom.006d10fa, "a"
	.incbin "baserom.gba", 0x006d10fa, 0x00000002
	.section .rom.006d553e, "a"
	.incbin "baserom.gba", 0x006d553e, 0x00000002
	.section .rom.006e5cae, "a"
	.incbin "baserom.gba", 0x006e5cae, 0x00000002
	.section .rom.006e929e, "a"
	.incbin "baserom.gba", 0x006e929e, 0x00000002
	.section .rom.006f4d42, "a"
	.incbin "baserom.gba", 0x006f4d42, 0x00000002
	.section .rom.007053f2, "a"
	.incbin "baserom.gba", 0x007053f2, 0x00000002
	.section .rom.0070ce92, "a"
	.incbin "baserom.gba", 0x0070ce92, 0x00000002
	.section .rom.007106d2, "a"
	.incbin "baserom.gba", 0x007106d2, 0x00000002
	.section .rom.00719b06, "a"
	.incbin "baserom.gba", 0x00719b06, 0x00000002
	.section .rom.00720cb2, "a"
	.incbin "baserom.gba", 0x00720cb2, 0x00000002
	.section .rom.00728b6e, "a"
	.incbin "baserom.gba", 0x00728b6e, 0x00000002
	.section .rom.0072d5fe, "a"
	.incbin "baserom.gba", 0x0072d5fe, 0x00000002
	.section .rom.007318b2, "a"
	.incbin "baserom.gba", 0x007318b2, 0x00000002
	.section .rom.00735232, "a"
	.incbin "baserom.gba", 0x00735232, 0x00000002
	.section .rom.00741a6e, "a"
	.incbin "baserom.gba", 0x00741a6e, 0x00000002
	.section .rom.0074d1ae, "a"
	.incbin "baserom.gba", 0x0074d1ae, 0x00000002
	.section .rom.0075057e, "a"
	.incbin "baserom.gba", 0x0075057e, 0x00000002
	.section .rom.0076249a, "a"
	.incbin "baserom.gba", 0x0076249a, 0x00000002
	.section .rom.00765f02, "a"
	.incbin "baserom.gba", 0x00765f02, 0x00000002
	.section .rom.007698f2, "a"
	.incbin "baserom.gba", 0x007698f2, 0x00000002
	.section .rom.00779e5a, "a"
	.incbin "baserom.gba", 0x00779e5a, 0x00000002
	.section .rom.0077dd8a, "a"
	.incbin "baserom.gba", 0x0077dd8a, 0x00000002
	.section .rom.007817ae, "a"
	.incbin "baserom.gba", 0x007817ae, 0x00000002
	.section .rom.00789ace, "a"
	.incbin "baserom.gba", 0x00789ace, 0x00000002
	.section .rom.00791c3a, "a"
	.incbin "baserom.gba", 0x00791c3a, 0x00000002
	.section .rom.0079ef06, "a"
	.incbin "baserom.gba", 0x0079ef06, 0x00000002
	.section .rom.007a2dc2, "a"
	.incbin "baserom.gba", 0x007a2dc2, 0x00000002
	.section .rom.007a6bbe, "a"
	.incbin "baserom.gba", 0x007a6bbe, 0x00000002
	.section .rom.007ab08e, "a"
	.incbin "baserom.gba", 0x007ab08e, 0x00000002
	.section .rom.007b2a6a, "a"
	.incbin "baserom.gba", 0x007b2a6a, 0x00000002
	.section .rom.007b7096, "a"
	.incbin "baserom.gba", 0x007b7096, 0x00000002
	.section .rom.007bed12, "a"
	.incbin "baserom.gba", 0x007bed12, 0x00000002
	.section .rom.007c290e, "a"
	.incbin "baserom.gba", 0x007c290e, 0x00000002
	.section .rom.007c62be, "a"
	.incbin "baserom.gba", 0x007c62be, 0x00000002
	.section .rom.007ca70e, "a"
	.incbin "baserom.gba", 0x007ca70e, 0x00000002
	.section .rom.007ce306, "a"
	.incbin "baserom.gba", 0x007ce306, 0x00000002
	.section .rom.007d9f66, "a"
	.incbin "baserom.gba", 0x007d9f66, 0x00000002
	.section .rom.007ddbb6, "a"
	.incbin "baserom.gba", 0x007ddbb6, 0x00000002
	.section .rom.007e6136, "a"
	.incbin "baserom.gba", 0x007e6136, 0x00000002
	.section .rom.007f2786, "a"
	.incbin "baserom.gba", 0x007f2786, 0x00000002
	.section .rom.007f923a, "a"
	.incbin "baserom.gba", 0x007f923a, 0x00000002
	.section .rom.00802502, "a"
	.incbin "baserom.gba", 0x00802502, 0x00000002
	.section .rom.00809f26, "a"
	.incbin "baserom.gba", 0x00809f26, 0x00000002
	.section .rom.0081fba6, "a"
	.incbin "baserom.gba", 0x0081fba6, 0x00000002
	.section .rom.00828fea, "a"
	.incbin "baserom.gba", 0x00828fea, 0x00000002
	.section .rom.00832206, "a"
	.incbin "baserom.gba", 0x00832206, 0x00000002
	.section .rom.0083fb2e, "a"
	.incbin "baserom.gba", 0x0083fb2e, 0x00000002
	.section .rom.00845982, "a"
	.incbin "baserom.gba", 0x00845982, 0x00000002
	.section .rom.0084b33a, "a"
	.incbin "baserom.gba", 0x0084b33a, 0x00000002
	.section .rom.0084babd, "a"
	.incbin "baserom.gba", 0x0084babd, 0x00000003
	.section .rom.0084d40f, "a"
	.incbin "baserom.gba", 0x0084d40f, 0x00000001
	.section .rom.0085022d, "a"
	.incbin "baserom.gba", 0x0085022d, 0x00000003
	.section .rom.00854142, "a"
	.incbin "baserom.gba", 0x00854142, 0x00000002
	.section .rom.008573f8, "a"
	.incbin "baserom.gba", 0x008573f8, 0x000009bc
	.section .rom.008583c7, "a"
	.incbin "baserom.gba", 0x008583c7, 0x00000001
	.section .rom.008587e7, "a"
	.incbin "baserom.gba", 0x008587e7, 0x00000001
	.section .rom.00858a66, "a"
	.incbin "baserom.gba", 0x00858a66, 0x00000002
	.section .rom.00858d86, "a"
	.incbin "baserom.gba", 0x00858d86, 0x00000002
	.section .rom.008591a9, "a"
	.incbin "baserom.gba", 0x008591a9, 0x00000003
	.section .rom.0085954e, "a"
	.incbin "baserom.gba", 0x0085954e, 0x00000002
	.section .rom.008598bd, "a"
	.incbin "baserom.gba", 0x008598bd, 0x00000003
	.section .rom.0085a47b, "a"
	.incbin "baserom.gba", 0x0085a47b, 0x00000001
	.section .rom.0085a587, "a"
	.incbin "baserom.gba", 0x0085a587, 0x00000001
	.section .rom.0085a725, "a"
	.incbin "baserom.gba", 0x0085a725, 0x00000003
	.section .rom.0085ab92, "a"
	.incbin "baserom.gba", 0x0085ab92, 0x00000002
	.section .rom.0085b10d, "a"
	.incbin "baserom.gba", 0x0085b10d, 0x00000003
	.section .rom.0085bc0b, "a"
	.incbin "baserom.gba", 0x0085bc0b, 0x00000001
	.section .rom.0085be0d, "a"
	.incbin "baserom.gba", 0x0085be0d, 0x00000003
	.section .rom.0085e4af, "a"
	.incbin "baserom.gba", 0x0085e4af, 0x00000001
	.section .rom.0085ec2a, "a"
	.incbin "baserom.gba", 0x0085ec2a, 0x00000002
	.section .rom.0086026f, "a"
	.incbin "baserom.gba", 0x0086026f, 0x00000001
	.section .rom.00860799, "a"
	.incbin "baserom.gba", 0x00860799, 0x00000003
	.section .rom.00860fa2, "a"
	.incbin "baserom.gba", 0x00860fa2, 0x00000002
	.section .rom.0086151b, "a"
	.incbin "baserom.gba", 0x0086151b, 0x00000001
	.section .rom.00862c5a, "a"
	.incbin "baserom.gba", 0x00862c5a, 0x00000002
	.section .rom.00865b23, "a"
	.incbin "baserom.gba", 0x00865b23, 0x00000001
	.section .rom.00865dd5, "a"
	.incbin "baserom.gba", 0x00865dd5, 0x00000003
	.section .rom.008671b7, "a"
	.incbin "baserom.gba", 0x008671b7, 0x00000001
	.section .rom.0086b9f3, "a"
	.incbin "baserom.gba", 0x0086b9f3, 0x00000001
	.section .rom.0086f199, "a"
	.incbin "baserom.gba", 0x0086f199, 0x00000003
	.section .rom.00870ebe, "a"
	.incbin "baserom.gba", 0x00870ebe, 0x00000002
	.section .rom.00872dbd, "a"
	.incbin "baserom.gba", 0x00872dbd, 0x00000003
	.section .rom.0087476b, "a"
	.incbin "baserom.gba", 0x0087476b, 0x00000001
	.section .rom.00875ce3, "a"
	.incbin "baserom.gba", 0x00875ce3, 0x00000001
	.section .rom.00879916, "a"
	.incbin "baserom.gba", 0x00879916, 0x00000002
	.section .rom.0087a474, "a"
	.incbin "baserom.gba", 0x0087a474, 0x00003b38
	.section .rom.0087e5e3, "a"
	.incbin "baserom.gba", 0x0087e5e3, 0x00000001
	.section .rom.0088074d, "a"
	.incbin "baserom.gba", 0x0088074d, 0x00000003
	.section .rom.00880858, "a"
	.incbin "baserom.gba", 0x00880858, 0x000003d0
	.section .rom.008814e1, "a"
	.incbin "baserom.gba", 0x008814e1, 0x00000003
	.section .rom.008830c5, "a"
	.incbin "baserom.gba", 0x008830c5, 0x00000003
	.section .rom.00884c22, "a"
	.incbin "baserom.gba", 0x00884c22, 0x00000002
	.section .rom.00885061, "a"
	.incbin "baserom.gba", 0x00885061, 0x00000003
	.section .rom.00885476, "a"
	.incbin "baserom.gba", 0x00885476, 0x00001842
	.section .rom.008891c3, "a"
	.incbin "baserom.gba", 0x008891c3, 0x00000001
	.section .rom.00889b7f, "a"
	.incbin "baserom.gba", 0x00889b7f, 0x00000001
	.section .rom.0088acc2, "a"
	.incbin "baserom.gba", 0x0088acc2, 0x00001496
	.section .rom.0088d6d3, "a"
	.incbin "baserom.gba", 0x0088d6d3, 0x00000001
	.section .rom.0088d9fd, "a"
	.incbin "baserom.gba", 0x0088d9fd, 0x0000104b
	.section .rom.0088f1d1, "a"
	.incbin "baserom.gba", 0x0088f1d1, 0x00000623
	.section .rom.0088fe1d, "a"
	.incbin "baserom.gba", 0x0088fe1d, 0x00000003
	.section .rom.00890227, "a"
	.incbin "baserom.gba", 0x00890227, 0x000007d9
	.section .rom.00890ca9, "a"
	.incbin "baserom.gba", 0x00890ca9, 0x0000095f
	.section .rom.00891cc5, "a"
	.incbin "baserom.gba", 0x00891cc5, 0x00003c7f
	.section .rom.00895fa3, "a"
	.incbin "baserom.gba", 0x00895fa3, 0x00000001
	.section .rom.00896fdd, "a"
	.incbin "baserom.gba", 0x00896fdd, 0x00000003
	.section .rom.00897633, "a"
	.incbin "baserom.gba", 0x00897633, 0x00000001
	.section .rom.00897cb1, "a"
	.incbin "baserom.gba", 0x00897cb1, 0x00000003
	.section .rom.008982d1, "a"
	.incbin "baserom.gba", 0x008982d1, 0x00000003
	.section .rom.0089969e, "a"
	.incbin "baserom.gba", 0x0089969e, 0x00000002
	.section .rom.0089a6e2, "a"
	.incbin "baserom.gba", 0x0089a6e2, 0x00000002
	.section .rom.0089b16b, "a"
	.incbin "baserom.gba", 0x0089b16b, 0x000002cd
	.section .rom.0089c171, "a"
	.incbin "baserom.gba", 0x0089c171, 0x00000003
	.section .rom.0089d3ad, "a"
	.incbin "baserom.gba", 0x0089d3ad, 0x00000003
	.section .rom.0089dcf7, "a"
	.incbin "baserom.gba", 0x0089dcf7, 0x00006c09
	.section .rom.008a4e34, "a"
	.incbin "baserom.gba", 0x008a4e34, 0x000011d8
	.section .rom.008a64fa, "a"
	.incbin "baserom.gba", 0x008a64fa, 0x000027fe
	.section .rom.008a933b, "a"
	.incbin "baserom.gba", 0x008a933b, 0x000000a9
	.section .rom.008a96fa, "a"
	.incbin "baserom.gba", 0x008a96fa, 0x000018d2
	.section .rom.008abc3d, "a"
	.incbin "baserom.gba", 0x008abc3d, 0x00000003
	.section .rom.008ac485, "a"
	.incbin "baserom.gba", 0x008ac485, 0x00001203
	.section .rom.008ae547, "a"
	.incbin "baserom.gba", 0x008ae547, 0x00000001
	.section .rom.008af023, "a"
	.incbin "baserom.gba", 0x008af023, 0x00000001
	.section .rom.008af6e6, "a"
	.incbin "baserom.gba", 0x008af6e6, 0x00000002
	.section .rom.008af9cd, "a"
	.incbin "baserom.gba", 0x008af9cd, 0x00000003
	.section .rom.008b0d33, "a"
	.incbin "baserom.gba", 0x008b0d33, 0x00000001
	.section .rom.008b111d, "a"
	.incbin "baserom.gba", 0x008b111d, 0x00000003
	.section .rom.008b14ef, "a"
	.incbin "baserom.gba", 0x008b14ef, 0x00000001
	.section .rom.008b1d8f, "a"
	.incbin "baserom.gba", 0x008b1d8f, 0x00000001
	.section .rom.008b2232, "a"
	.incbin "baserom.gba", 0x008b2232, 0x00000002
	.section .rom.008b33eb, "a"
	.incbin "baserom.gba", 0x008b33eb, 0x00000001
	.section .rom.008b3844, "a"
	.incbin "baserom.gba", 0x008b3844, 0x00002034
	.section .rom.008b5ad2, "a"
	.incbin "baserom.gba", 0x008b5ad2, 0x00000002
	.section .rom.008b752d, "a"
	.incbin "baserom.gba", 0x008b752d, 0x000001b7
	.section .rom.008b7a79, "a"
	.incbin "baserom.gba", 0x008b7a79, 0x00000003
	.section .rom.008b9802, "a"
	.incbin "baserom.gba", 0x008b9802, 0x0000027a
	.section .rom.008b9f42, "a"
	.incbin "baserom.gba", 0x008b9f42, 0x00000002
	.section .rom.008bbb17, "a"
	.incbin "baserom.gba", 0x008bbb17, 0x00000001
	.section .rom.008bd715, "a"
	.incbin "baserom.gba", 0x008bd715, 0x00000003
	.section .rom.008bd936, "a"
	.incbin "baserom.gba", 0x008bd936, 0x0000043e
	.section .rom.008bde85, "a"
	.incbin "baserom.gba", 0x008bde85, 0x00000003
	.section .rom.008be467, "a"
	.incbin "baserom.gba", 0x008be467, 0x000004a1
	.section .rom.008bf61a, "a"
	.incbin "baserom.gba", 0x008bf61a, 0x00000002
	.section .rom.008bfb5c, "a"
	.incbin "baserom.gba", 0x008bfb5c, 0x00000840
	.section .rom.008c072b, "a"
	.incbin "baserom.gba", 0x008c072b, 0x00001259
	.section .rom.008c2472, "a"
	.incbin "baserom.gba", 0x008c2472, 0x00000002
	.section .rom.008c3249, "a"
	.incbin "baserom.gba", 0x008c3249, 0x00000003
	.section .rom.008c3a76, "a"
	.incbin "baserom.gba", 0x008c3a76, 0x00000002
	.section .rom.008c3e68, "a"
	.incbin "baserom.gba", 0x008c3e68, 0x000010d0
	.section .rom.008c58a9, "a"
	.incbin "baserom.gba", 0x008c58a9, 0x00000003
	.section .rom.008c5c57, "a"
	.incbin "baserom.gba", 0x008c5c57, 0x00000001
	.section .rom.008c7bfb, "a"
	.incbin "baserom.gba", 0x008c7bfb, 0x00000001
	.section .rom.008c8997, "a"
	.incbin "baserom.gba", 0x008c8997, 0x00000001
	.section .rom.008c8bb3, "a"
	.incbin "baserom.gba", 0x008c8bb3, 0x00000001
	.section .rom.008c8eaf, "a"
	.incbin "baserom.gba", 0x008c8eaf, 0x00000001
	.section .rom.008c9250, "a"
	.incbin "baserom.gba", 0x008c9250, 0x000016b0
	.section .rom.008cb05d, "a"
	.incbin "baserom.gba", 0x008cb05d, 0x00000003
	.section .rom.008cbe2b, "a"
	.incbin "baserom.gba", 0x008cbe2b, 0x00000c75
	.section .rom.008cd095, "a"
	.incbin "baserom.gba", 0x008cd095, 0x00000003
	.section .rom.008cd5ef, "a"
	.incbin "baserom.gba", 0x008cd5ef, 0x00000001
	.section .rom.008ceed9, "a"
	.incbin "baserom.gba", 0x008ceed9, 0x000008a3
	.section .rom.008cfb57, "a"
	.incbin "baserom.gba", 0x008cfb57, 0x00000001
	.section .rom.008cfde1, "a"
	.incbin "baserom.gba", 0x008cfde1, 0x00000003
	.section .rom.008d0187, "a"
	.incbin "baserom.gba", 0x008d0187, 0x00000001
	.section .rom.008d03e2, "a"
	.incbin "baserom.gba", 0x008d03e2, 0x00000002
	.section .rom.008d079a, "a"
	.incbin "baserom.gba", 0x008d079a, 0x00000002
	.section .rom.008d1c83, "a"
	.incbin "baserom.gba", 0x008d1c83, 0x00000001
	.section .rom.008d3246, "a"
	.incbin "baserom.gba", 0x008d3246, 0x00000a6e
	.section .rom.008d4a8a, "a"
	.incbin "baserom.gba", 0x008d4a8a, 0x00000002
	.section .rom.008d4e0d, "a"
	.incbin "baserom.gba", 0x008d4e0d, 0x00000003
	.section .rom.008d5abd, "a"
	.incbin "baserom.gba", 0x008d5abd, 0x00000003
	.section .rom.008d6605, "a"
	.incbin "baserom.gba", 0x008d6605, 0x00000a27
	.section .rom.008d7bfa, "a"
	.incbin "baserom.gba", 0x008d7bfa, 0x00000002
	.section .rom.008d8112, "a"
	.incbin "baserom.gba", 0x008d8112, 0x00000626
	.section .rom.008d8b59, "a"
	.incbin "baserom.gba", 0x008d8b59, 0x00000003
	.section .rom.008d8df3, "a"
	.incbin "baserom.gba", 0x008d8df3, 0x00000001
	.section .rom.008da2f4, "a"
	.incbin "baserom.gba", 0x008da2f4, 0x00001e28
	.section .rom.008dc506, "a"
	.incbin "baserom.gba", 0x008dc506, 0x00000002
	.section .rom.008de47b, "a"
	.incbin "baserom.gba", 0x008de47b, 0x00000001
	.section .rom.008de94b, "a"
	.incbin "baserom.gba", 0x008de94b, 0x00001cc5
	.section .rom.008e0de5, "a"
	.incbin "baserom.gba", 0x008e0de5, 0x00000003
	.section .rom.008e18b6, "a"
	.incbin "baserom.gba", 0x008e18b6, 0x00000002
	.section .rom.008e23f3, "a"
	.incbin "baserom.gba", 0x008e23f3, 0x00001c2d
	.section .rom.008e438b, "a"
	.incbin "baserom.gba", 0x008e438b, 0x00000001
	.section .rom.008e4929, "a"
	.incbin "baserom.gba", 0x008e4929, 0x00000207
	.section .rom.008e4e69, "a"
	.incbin "baserom.gba", 0x008e4e69, 0x00002687
	.section .rom.008e948e, "a"
	.incbin "baserom.gba", 0x008e948e, 0x00000002
	.section .rom.008e9c31, "a"
	.incbin "baserom.gba", 0x008e9c31, 0x00000003
	.section .rom.008eac48, "a"
	.incbin "baserom.gba", 0x008eac48, 0x00001878
	.section .rom.008ec544, "a"
	.incbin "baserom.gba", 0x008ec544, 0x000004e8
	.section .rom.008ecb34, "a"
	.incbin "baserom.gba", 0x008ecb34, 0x000006b8
	.section .rom.008edafe, "a"
	.incbin "baserom.gba", 0x008edafe, 0x00002b86
	.section .rom.008f1729, "a"
	.incbin "baserom.gba", 0x008f1729, 0x00000003
	.section .rom.008f1928, "a"
	.incbin "baserom.gba", 0x008f1928, 0x0004ac54
	.section .rom.0093c769, "a"
	.incbin "baserom.gba", 0x0093c769, 0x0000060f
	.section .rom.0093e4e2, "a"
	.incbin "baserom.gba", 0x0093e4e2, 0x00000002
	.section .rom.0093fa19, "a"
	.incbin "baserom.gba", 0x0093fa19, 0x00000003
	.section .rom.0094186f, "a"
	.incbin "baserom.gba", 0x0094186f, 0x00000001
	.section .rom.00943983, "a"
	.incbin "baserom.gba", 0x00943983, 0x00000001
	.section .rom.00943b5c, "a"
	.incbin "baserom.gba", 0x00943b5c, 0x000005a4
	.section .rom.0094677b, "a"
	.incbin "baserom.gba", 0x0094677b, 0x00000001
	.section .rom.00947183, "a"
	.incbin "baserom.gba", 0x00947183, 0x00000001
	.section .rom.00949ea2, "a"
	.incbin "baserom.gba", 0x00949ea2, 0x00000002
	.section .rom.0094a074, "a"
	.incbin "baserom.gba", 0x0094a074, 0x00000468
	.section .rom.0094bace, "a"
	.incbin "baserom.gba", 0x0094bace, 0x00000002
	.section .rom.0094cf3e, "a"
	.incbin "baserom.gba", 0x0094cf3e, 0x00000002
	.section .rom.0094d832, "a"
	.incbin "baserom.gba", 0x0094d832, 0x00000002
	.section .rom.0094e159, "a"
	.incbin "baserom.gba", 0x0094e159, 0x00000003
	.section .rom.0094f04a, "a"
	.incbin "baserom.gba", 0x0094f04a, 0x00000002
	.section .rom.0094fc37, "a"
	.incbin "baserom.gba", 0x0094fc37, 0x00000001
	.section .rom.0094fe12, "a"
	.incbin "baserom.gba", 0x0094fe12, 0x000004ca
	.section .rom.00950b72, "a"
	.incbin "baserom.gba", 0x00950b72, 0x00000002
	.section .rom.00952fad, "a"
	.incbin "baserom.gba", 0x00952fad, 0x00000003
	.section .rom.0095357f, "a"
	.incbin "baserom.gba", 0x0095357f, 0x00000001
	.section .rom.00953a1f, "a"
	.incbin "baserom.gba", 0x00953a1f, 0x00000001
	.section .rom.00953d72, "a"
	.incbin "baserom.gba", 0x00953d72, 0x000002fa
	.section .rom.00954145, "a"
	.incbin "baserom.gba", 0x00954145, 0x000002f3
	.section .rom.009544ca, "a"
	.incbin "baserom.gba", 0x009544ca, 0x00000002
	.section .rom.0095507d, "a"
	.incbin "baserom.gba", 0x0095507d, 0x00000003
	.section .rom.00955d2a, "a"
	.incbin "baserom.gba", 0x00955d2a, 0x00000002
	.section .rom.0095682e, "a"
	.incbin "baserom.gba", 0x0095682e, 0x00000002
	.section .rom.0095776d, "a"
	.incbin "baserom.gba", 0x0095776d, 0x00000003
	.section .rom.00957fa9, "a"
	.incbin "baserom.gba", 0x00957fa9, 0x00000003
	.section .rom.0095944e, "a"
	.incbin "baserom.gba", 0x0095944e, 0x00000002
	.section .rom.00959f06, "a"
	.incbin "baserom.gba", 0x00959f06, 0x00000002
	.section .rom.0095a231, "a"
	.incbin "baserom.gba", 0x0095a231, 0x00000003
	.section .rom.0095b02e, "a"
	.incbin "baserom.gba", 0x0095b02e, 0x00000002
	.section .rom.0095b830, "a"
	.incbin "baserom.gba", 0x0095b830, 0x00000400
	.section .rom.00967fea, "a"
	.incbin "baserom.gba", 0x00967fea, 0x000009fa
	.section .rom.00968e4b, "a"
	.incbin "baserom.gba", 0x00968e4b, 0x00000001
	.section .rom.00969057, "a"
	.incbin "baserom.gba", 0x00969057, 0x00000001
	.section .rom.0096925b, "a"
	.incbin "baserom.gba", 0x0096925b, 0x00000001
	.section .rom.009692ea, "a"
	.incbin "baserom.gba", 0x009692ea, 0x00000002
	.section .rom.009697cc, "a"
	.incbin "baserom.gba", 0x009697cc, 0x00000070
	.section .rom.00969897, "a"
	.incbin "baserom.gba", 0x00969897, 0x00000001
	.section .rom.009698ca, "a"
	.incbin "baserom.gba", 0x009698ca, 0x00000002
	.section .rom.00969a96, "a"
	.incbin "baserom.gba", 0x00969a96, 0x00000332
	.section .rom.00969e81, "a"
	.incbin "baserom.gba", 0x00969e81, 0x00000003
	.section .rom.0096a056, "a"
	.incbin "baserom.gba", 0x0096a056, 0x000000be
	.section .rom.0096a266, "a"
	.incbin "baserom.gba", 0x0096a266, 0x00000002
	.section .rom.0096a349, "a"
	.incbin "baserom.gba", 0x0096a349, 0x00000003
	.section .rom.0096a419, "a"
	.incbin "baserom.gba", 0x0096a419, 0x00000003
	.section .rom.0096a53f, "a"
	.incbin "baserom.gba", 0x0096a53f, 0x00000001
	.section .rom.0096a56e, "a"
	.incbin "baserom.gba", 0x0096a56e, 0x00000002
	.section .rom.0096a7d2, "a"
	.incbin "baserom.gba", 0x0096a7d2, 0x00000002
	.section .rom.0096a86f, "a"
	.incbin "baserom.gba", 0x0096a86f, 0x00000001
	.section .rom.0096a8d4, "a"
	.incbin "baserom.gba", 0x0096a8d4, 0x00000cf4
	.section .rom.009707ff, "a"
	.incbin "baserom.gba", 0x009707ff, 0x00000001
	.section .rom.0097374a, "a"
	.incbin "baserom.gba", 0x0097374a, 0x00000002
	.section .rom.0097736d, "a"
	.incbin "baserom.gba", 0x0097736d, 0x00000003
	.section .rom.009795b9, "a"
	.incbin "baserom.gba", 0x009795b9, 0x00000003
	.section .rom.0097eb55, "a"
	.incbin "baserom.gba", 0x0097eb55, 0x00000003
	.section .rom.009823cf, "a"
	.incbin "baserom.gba", 0x009823cf, 0x00000001
	.section .rom.00988239, "a"
	.incbin "baserom.gba", 0x00988239, 0x00000003
	.section .rom.0098930d, "a"
	.incbin "baserom.gba", 0x0098930d, 0x00000003
	.section .rom.0098a379, "a"
	.incbin "baserom.gba", 0x0098a379, 0x00000003
	.section .rom.0098f9d3, "a"
	.incbin "baserom.gba", 0x0098f9d3, 0x00000001
	.section .rom.00992b15, "a"
	.incbin "baserom.gba", 0x00992b15, 0x00000003
	.section .rom.00994bb5, "a"
	.incbin "baserom.gba", 0x00994bb5, 0x00000003
	.section .rom.00996ed5, "a"
	.incbin "baserom.gba", 0x00996ed5, 0x00000003
	.section .rom.00999b4e, "a"
	.incbin "baserom.gba", 0x00999b4e, 0x00000002
	.section .rom.0099ba49, "a"
	.incbin "baserom.gba", 0x0099ba49, 0x00000003
	.section .rom.009a3f8d, "a"
	.incbin "baserom.gba", 0x009a3f8d, 0x00000003
	.section .rom.009acc0f, "a"
	.incbin "baserom.gba", 0x009acc0f, 0x00000001
	.section .rom.009af789, "a"
	.incbin "baserom.gba", 0x009af789, 0x00000003
	.section .rom.009b61c6, "a"
	.incbin "baserom.gba", 0x009b61c6, 0x00000002
	.section .rom.009b8cba, "a"
	.incbin "baserom.gba", 0x009b8cba, 0x00000002
	.section .rom.009ba7aa, "a"
	.incbin "baserom.gba", 0x009ba7aa, 0x00000002
	.section .rom.009bd0c0, "a"
	.incbin "baserom.gba", 0x009bd0c0, 0x00003440
	.section .rom.009c4ea6, "a"
	.incbin "baserom.gba", 0x009c4ea6, 0x00000002
	.section .rom.009c5e42, "a"
	.incbin "baserom.gba", 0x009c5e42, 0x00000002
	.section .rom.009c856d, "a"
	.incbin "baserom.gba", 0x009c856d, 0x00000003
	.section .rom.009c9c82, "a"
	.incbin "baserom.gba", 0x009c9c82, 0x00000002
	.section .rom.009d249b, "a"
	.incbin "baserom.gba", 0x009d249b, 0x00000001
	.section .rom.009d6763, "a"
	.incbin "baserom.gba", 0x009d6763, 0x00000001
	.section .rom.009ddf9d, "a"
	.incbin "baserom.gba", 0x009ddf9d, 0x00000003
	.section .rom.009df782, "a"
	.incbin "baserom.gba", 0x009df782, 0x00000002
	.section .rom.009e2519, "a"
	.incbin "baserom.gba", 0x009e2519, 0x00000003
	.section .rom.009e3563, "a"
	.incbin "baserom.gba", 0x009e3563, 0x00000001
	.section .rom.009ebaf3, "a"
	.incbin "baserom.gba", 0x009ebaf3, 0x00000001
	.section .rom.009ed8ed, "a"
	.incbin "baserom.gba", 0x009ed8ed, 0x00000003
	.section .rom.009eecb3, "a"
	.incbin "baserom.gba", 0x009eecb3, 0x00000001
	.section .rom.009f5bb5, "a"
	.incbin "baserom.gba", 0x009f5bb5, 0x00000003
	.section .rom.009f7eb6, "a"
	.incbin "baserom.gba", 0x009f7eb6, 0x00000002
	.section .rom.009fd103, "a"
	.incbin "baserom.gba", 0x009fd103, 0x00000001
	.section .rom.00a02502, "a"
	.incbin "baserom.gba", 0x00a02502, 0x00000002
	.section .rom.00a046b6, "a"
	.incbin "baserom.gba", 0x00a046b6, 0x00000002
	.section .rom.00a0881d, "a"
	.incbin "baserom.gba", 0x00a0881d, 0x00000003
	.section .rom.00a0ba42, "a"
	.incbin "baserom.gba", 0x00a0ba42, 0x00000002
	.section .rom.00a0d146, "a"
	.incbin "baserom.gba", 0x00a0d146, 0x00000002
	.section .rom.00a13d2e, "a"
	.incbin "baserom.gba", 0x00a13d2e, 0x00000002
	.section .rom.00a2030e, "a"
	.incbin "baserom.gba", 0x00a2030e, 0x00000002
	.section .rom.00a233a2, "a"
	.incbin "baserom.gba", 0x00a233a2, 0x00000002
	.section .rom.00a25c0d, "a"
	.incbin "baserom.gba", 0x00a25c0d, 0x00000003
	.section .rom.00a276fe, "a"
	.incbin "baserom.gba", 0x00a276fe, 0x00000002
	.section .rom.00a28516, "a"
	.incbin "baserom.gba", 0x00a28516, 0x00000002
	.section .rom.00a29201, "a"
	.incbin "baserom.gba", 0x00a29201, 0x00000003
	.section .rom.00a326be, "a"
	.incbin "baserom.gba", 0x00a326be, 0x00000002
	.section .rom.00a333c9, "a"
	.incbin "baserom.gba", 0x00a333c9, 0x00000003
	.section .rom.00a3462d, "a"
	.incbin "baserom.gba", 0x00a3462d, 0x00000003
	.section .rom.00a3555f, "a"
	.incbin "baserom.gba", 0x00a3555f, 0x00000001
	.section .rom.00a361cf, "a"
	.incbin "baserom.gba", 0x00a361cf, 0x00000001
	.section .rom.00a36de7, "a"
	.incbin "baserom.gba", 0x00a36de7, 0x00000001
	.section .rom.00a3755b, "a"
	.incbin "baserom.gba", 0x00a3755b, 0x00000001
	.section .rom.00a38deb, "a"
	.incbin "baserom.gba", 0x00a38deb, 0x00000001
	.section .rom.00a397b9, "a"
	.incbin "baserom.gba", 0x00a397b9, 0x00000003
	.section .rom.00a3a3d7, "a"
	.incbin "baserom.gba", 0x00a3a3d7, 0x00000001
	.section .rom.00a3ca8b, "a"
	.incbin "baserom.gba", 0x00a3ca8b, 0x00000001
	.section .rom.00a3f19e, "a"
	.incbin "baserom.gba", 0x00a3f19e, 0x00000002
	.section .rom.00a440f3, "a"
	.incbin "baserom.gba", 0x00a440f3, 0x00000001
	.section .rom.00a4841d, "a"
	.incbin "baserom.gba", 0x00a4841d, 0x00000003
	.section .rom.00a4900a, "a"
	.incbin "baserom.gba", 0x00a4900a, 0x00000002
	.section .rom.00a4dbbf, "a"
	.incbin "baserom.gba", 0x00a4dbbf, 0x00000001
	.section .rom.00a4ff29, "a"
	.incbin "baserom.gba", 0x00a4ff29, 0x00000003
	.section .rom.00a518cb, "a"
	.incbin "baserom.gba", 0x00a518cb, 0x00000001
	.section .rom.00a5618d, "a"
	.incbin "baserom.gba", 0x00a5618d, 0x00000003
	.section .rom.00a5959d, "a"
	.incbin "baserom.gba", 0x00a5959d, 0x00000003
	.section .rom.00a5d665, "a"
	.incbin "baserom.gba", 0x00a5d665, 0x00000003
	.section .rom.00a60d2e, "a"
	.incbin "baserom.gba", 0x00a60d2e, 0x00000002
	.section .rom.00a68cff, "a"
	.incbin "baserom.gba", 0x00a68cff, 0x00000001
	.section .rom.00a7000f, "a"
	.incbin "baserom.gba", 0x00a7000f, 0x00000001
	.section .rom.00a74f8f, "a"
	.incbin "baserom.gba", 0x00a74f8f, 0x00000001
	.section .rom.00a79a11, "a"
	.incbin "baserom.gba", 0x00a79a11, 0x0000051f
	.section .rom.00a7b18d, "a"
	.incbin "baserom.gba", 0x00a7b18d, 0x00000003
	.section .rom.00a7b35e, "a"
	.incbin "baserom.gba", 0x00a7b35e, 0x00000002
	.section .rom.00a7d3ff, "a"
	.incbin "baserom.gba", 0x00a7d3ff, 0x00000001
	.section .rom.00a7e38c, "a"
	.incbin "baserom.gba", 0x00a7e38c, 0x000022d8
	.section .rom.00a818b8, "a"
	.incbin "baserom.gba", 0x00a818b8, 0x0000461c
	.section .rom.00a85fda, "a"
	.incbin "baserom.gba", 0x00a85fda, 0x00000002
	.section .rom.00a871c5, "a"
	.incbin "baserom.gba", 0x00a871c5, 0x00000003
	.section .rom.00a88e4d, "a"
	.incbin "baserom.gba", 0x00a88e4d, 0x00000003
	.section .rom.00a8935b, "a"
	.incbin "baserom.gba", 0x00a8935b, 0x00000001
	.section .rom.00a8949b, "a"
	.incbin "baserom.gba", 0x00a8949b, 0x00000001
	.section .rom.00a8b11e, "a"
	.incbin "baserom.gba", 0x00a8b11e, 0x00000002
	.section .rom.00a8daf6, "a"
	.incbin "baserom.gba", 0x00a8daf6, 0x00000002
	.section .rom.00a902bf, "a"
	.incbin "baserom.gba", 0x00a902bf, 0x00000001
	.section .rom.00a917df, "a"
	.incbin "baserom.gba", 0x00a917df, 0x00000001
	.section .rom.00a93123, "a"
	.incbin "baserom.gba", 0x00a93123, 0x00000001
	.section .rom.00a94bd1, "a"
	.incbin "baserom.gba", 0x00a94bd1, 0x00000003
	.section .rom.00a94d45, "a"
	.incbin "baserom.gba", 0x00a94d45, 0x00000003
	.section .rom.00a97b29, "a"
	.incbin "baserom.gba", 0x00a97b29, 0x00000003
	.section .rom.00a9a3d3, "a"
	.incbin "baserom.gba", 0x00a9a3d3, 0x00000001
	.section .rom.00a9e86a, "a"
	.incbin "baserom.gba", 0x00a9e86a, 0x00000002
	.section .rom.00aa1be2, "a"
	.incbin "baserom.gba", 0x00aa1be2, 0x00000002
	.section .rom.00aa46df, "a"
	.incbin "baserom.gba", 0x00aa46df, 0x00000001
	.section .rom.00aa67bd, "a"
	.incbin "baserom.gba", 0x00aa67bd, 0x00000003
	.section .rom.00aa8a09, "a"
	.incbin "baserom.gba", 0x00aa8a09, 0x00000003
	.section .rom.00aa8b4b, "a"
	.incbin "baserom.gba", 0x00aa8b4b, 0x00000001
	.section .rom.00aaa21e, "a"
	.incbin "baserom.gba", 0x00aaa21e, 0x00000002
	.section .rom.00aaa31e, "a"
	.incbin "baserom.gba", 0x00aaa31e, 0x00000002
	.section .rom.00aac211, "a"
	.incbin "baserom.gba", 0x00aac211, 0x00000003
	.section .rom.00aadbe9, "a"
	.incbin "baserom.gba", 0x00aadbe9, 0x00000003
	.section .rom.00ab061a, "a"
	.incbin "baserom.gba", 0x00ab061a, 0x00000002
	.section .rom.00ab586b, "a"
	.incbin "baserom.gba", 0x00ab586b, 0x00000001
	.section .rom.00ab81af, "a"
	.incbin "baserom.gba", 0x00ab81af, 0x00000001
	.section .rom.00ab9682, "a"
	.incbin "baserom.gba", 0x00ab9682, 0x00000002
	.section .rom.00abe44d, "a"
	.incbin "baserom.gba", 0x00abe44d, 0x00000003
	.section .rom.00ac10f7, "a"
	.incbin "baserom.gba", 0x00ac10f7, 0x00000001
	.section .rom.00ac432f, "a"
	.incbin "baserom.gba", 0x00ac432f, 0x00000001
	.section .rom.00ac44c7, "a"
	.incbin "baserom.gba", 0x00ac44c7, 0x00000001
	.section .rom.00ac72c7, "a"
	.incbin "baserom.gba", 0x00ac72c7, 0x00000001
	.section .rom.00ac8a7b, "a"
	.incbin "baserom.gba", 0x00ac8a7b, 0x00000001
	.section .rom.00ac9a16, "a"
	.incbin "baserom.gba", 0x00ac9a16, 0x00000002
	.section .rom.00acc077, "a"
	.incbin "baserom.gba", 0x00acc077, 0x00000001
	.section .rom.00acc21e, "a"
	.incbin "baserom.gba", 0x00acc21e, 0x00000002
	.section .rom.00acf1d7, "a"
	.incbin "baserom.gba", 0x00acf1d7, 0x00000001
	.section .rom.00acfe7d, "a"
	.incbin "baserom.gba", 0x00acfe7d, 0x00000003
	.section .rom.00ad144d, "a"
	.incbin "baserom.gba", 0x00ad144d, 0x00000003
	.section .rom.00ad9419, "a"
	.incbin "baserom.gba", 0x00ad9419, 0x00000003
	.section .rom.00adaeff, "a"
	.incbin "baserom.gba", 0x00adaeff, 0x00000001
	.section .rom.00adc3e9, "a"
	.incbin "baserom.gba", 0x00adc3e9, 0x00000003
	.section .rom.00ae2613, "a"
	.incbin "baserom.gba", 0x00ae2613, 0x00000001
	.section .rom.00ae2ae5, "a"
	.incbin "baserom.gba", 0x00ae2ae5, 0x00000003
	.section .rom.00ae3c2d, "a"
	.incbin "baserom.gba", 0x00ae3c2d, 0x00000003
	.section .rom.00ae3dde, "a"
	.incbin "baserom.gba", 0x00ae3dde, 0x00000002
	.section .rom.00aefacb, "a"
	.incbin "baserom.gba", 0x00aefacb, 0x00000001
	.section .rom.00aefbeb, "a"
	.incbin "baserom.gba", 0x00aefbeb, 0x00000001
	.section .rom.00af1f8b, "a"
	.incbin "baserom.gba", 0x00af1f8b, 0x00000001
	.section .rom.00af31d7, "a"
	.incbin "baserom.gba", 0x00af31d7, 0x00000001
	.section .rom.00af5567, "a"
	.incbin "baserom.gba", 0x00af5567, 0x00000001
	.section .rom.00af6e65, "a"
	.incbin "baserom.gba", 0x00af6e65, 0x00000003
	.section .rom.00af7e56, "a"
	.incbin "baserom.gba", 0x00af7e56, 0x00000002
	.section .rom.00afa3f2, "a"
	.incbin "baserom.gba", 0x00afa3f2, 0x00000002
	.section .rom.00afea1a, "a"
	.incbin "baserom.gba", 0x00afea1a, 0x00000002
	.section .rom.00aff0df, "a"
	.incbin "baserom.gba", 0x00aff0df, 0x00000001
	.section .rom.00b01b75, "a"
	.incbin "baserom.gba", 0x00b01b75, 0x00000003
	.section .rom.00b01cc6, "a"
	.incbin "baserom.gba", 0x00b01cc6, 0x00000002
	.section .rom.00b040d6, "a"
	.incbin "baserom.gba", 0x00b040d6, 0x00000002
	.section .rom.00b06257, "a"
	.incbin "baserom.gba", 0x00b06257, 0x00000001
	.section .rom.00b06397, "a"
	.incbin "baserom.gba", 0x00b06397, 0x00000001
	.section .rom.00b08e36, "a"
	.incbin "baserom.gba", 0x00b08e36, 0x00000002
	.section .rom.00b08f87, "a"
	.incbin "baserom.gba", 0x00b08f87, 0x00000001
	.section .rom.00b0b396, "a"
	.incbin "baserom.gba", 0x00b0b396, 0x00000002
	.section .rom.00b0d517, "a"
	.incbin "baserom.gba", 0x00b0d517, 0x00000001
	.section .rom.00b0d657, "a"
	.incbin "baserom.gba", 0x00b0d657, 0x00000001
	.section .rom.00b10c86, "a"
	.incbin "baserom.gba", 0x00b10c86, 0x00000002
	.section .rom.00b131ea, "a"
	.incbin "baserom.gba", 0x00b131ea, 0x00000002
	.section .rom.00b1536b, "a"
	.incbin "baserom.gba", 0x00b1536b, 0x00000001
	.section .rom.00b154ab, "a"
	.incbin "baserom.gba", 0x00b154ab, 0x00000001
	.section .rom.00b1695b, "a"
	.incbin "baserom.gba", 0x00b1695b, 0x00000001
	.section .rom.00b16a66, "a"
	.incbin "baserom.gba", 0x00b16a66, 0x00000002
	.section .rom.00b185be, "a"
	.incbin "baserom.gba", 0x00b185be, 0x00000002
	.section .rom.00b19c61, "a"
	.incbin "baserom.gba", 0x00b19c61, 0x00000003
	.section .rom.00b19e22, "a"
	.incbin "baserom.gba", 0x00b19e22, 0x00000d9e
	.section .rom.00b1c816, "a"
	.incbin "baserom.gba", 0x00b1c816, 0x00000002
	.section .rom.00b1e06d, "a"
	.incbin "baserom.gba", 0x00b1e06d, 0x00000003
	.section .rom.00b1e22e, "a"
	.incbin "baserom.gba", 0x00b1e22e, 0x00000002
	.section .rom.00b224a9, "a"
	.incbin "baserom.gba", 0x00b224a9, 0x00000003
	.section .rom.00b2266a, "a"
	.incbin "baserom.gba", 0x00b2266a, 0x00000002
	.section .rom.00b230e1, "a"
	.incbin "baserom.gba", 0x00b230e1, 0x00000003
	.section .rom.00b231e9, "a"
	.incbin "baserom.gba", 0x00b231e9, 0x00000003
	.section .rom.00b24eaa, "a"
	.incbin "baserom.gba", 0x00b24eaa, 0x00000002
	.section .rom.00b26637, "a"
	.incbin "baserom.gba", 0x00b26637, 0x00000001
	.section .rom.00b268ed, "a"
	.incbin "baserom.gba", 0x00b268ed, 0x00000003
	.section .rom.00b283fd, "a"
	.incbin "baserom.gba", 0x00b283fd, 0x00000003
	.section .rom.00b2ac8a, "a"
	.incbin "baserom.gba", 0x00b2ac8a, 0x00000002
	.section .rom.00b2d47a, "a"
	.incbin "baserom.gba", 0x00b2d47a, 0x00000002
	.section .rom.00b2e41a, "a"
	.incbin "baserom.gba", 0x00b2e41a, 0x00000002
	.section .rom.00b3056d, "a"
	.incbin "baserom.gba", 0x00b3056d, 0x00000003
	.section .rom.00b33bfa, "a"
	.incbin "baserom.gba", 0x00b33bfa, 0x00000002
	.section .rom.00b3598f, "a"
	.incbin "baserom.gba", 0x00b3598f, 0x00000001
	.section .rom.00b377ad, "a"
	.incbin "baserom.gba", 0x00b377ad, 0x00000003
	.section .rom.00b39289, "a"
	.incbin "baserom.gba", 0x00b39289, 0x00000003
	.section .rom.00b3a872, "a"
	.incbin "baserom.gba", 0x00b3a872, 0x00000002
	.section .rom.00b3ce5a, "a"
	.incbin "baserom.gba", 0x00b3ce5a, 0x00000002
	.section .rom.00b3cfd3, "a"
	.incbin "baserom.gba", 0x00b3cfd3, 0x00000001
	.section .rom.00b3e492, "a"
	.incbin "baserom.gba", 0x00b3e492, 0x00000002
	.section .rom.00b3fc96, "a"
	.incbin "baserom.gba", 0x00b3fc96, 0x00000002
	.section .rom.00b4160e, "a"
	.incbin "baserom.gba", 0x00b4160e, 0x00000002
	.section .rom.00b4252e, "a"
	.incbin "baserom.gba", 0x00b4252e, 0x00000002
	.section .rom.00b43fda, "a"
	.incbin "baserom.gba", 0x00b43fda, 0x00000002
	.section .rom.00b440e7, "a"
	.incbin "baserom.gba", 0x00b440e7, 0x00000001
	.section .rom.00b46217, "a"
	.incbin "baserom.gba", 0x00b46217, 0x00000001
	.section .rom.00b47ded, "a"
	.incbin "baserom.gba", 0x00b47ded, 0x00000003
	.section .rom.00b483ff, "a"
	.incbin "baserom.gba", 0x00b483ff, 0x00000001
	.section .rom.00b4853f, "a"
	.incbin "baserom.gba", 0x00b4853f, 0x00000001
	.section .rom.00b4ac3f, "a"
	.incbin "baserom.gba", 0x00b4ac3f, 0x00000001
	.section .rom.00b4c49e, "a"
	.incbin "baserom.gba", 0x00b4c49e, 0x00000002
	.section .rom.00b4caaf, "a"
	.incbin "baserom.gba", 0x00b4caaf, 0x00000001
	.section .rom.00b4cbef, "a"
	.incbin "baserom.gba", 0x00b4cbef, 0x00000001
	.section .rom.00b4da6a, "a"
	.incbin "baserom.gba", 0x00b4da6a, 0x00000002
	.section .rom.00b4db81, "a"
	.incbin "baserom.gba", 0x00b4db81, 0x00000003
	.section .rom.00b4fe57, "a"
	.incbin "baserom.gba", 0x00b4fe57, 0x00000001
	.section .rom.00b51aaa, "a"
	.incbin "baserom.gba", 0x00b51aaa, 0x00000002
	.section .rom.00b520ef, "a"
	.incbin "baserom.gba", 0x00b520ef, 0x00000001
	.section .rom.00b5222f, "a"
	.incbin "baserom.gba", 0x00b5222f, 0x00000001
	.section .rom.00b52e35, "a"
	.incbin "baserom.gba", 0x00b52e35, 0x00000003
	.section .rom.00b553e7, "a"
	.incbin "baserom.gba", 0x00b553e7, 0x00000001
	.section .rom.00b570a6, "a"
	.incbin "baserom.gba", 0x00b570a6, 0x00000002
	.section .rom.00b58b72, "a"
	.incbin "baserom.gba", 0x00b58b72, 0x00000002
	.section .rom.00b59c46, "a"
	.incbin "baserom.gba", 0x00b59c46, 0x00000002
	.section .rom.00b59dad, "a"
	.incbin "baserom.gba", 0x00b59dad, 0x00000003
	.section .rom.00b5afc6, "a"
	.incbin "baserom.gba", 0x00b5afc6, 0x00000002
	.section .rom.00b5bba5, "a"
	.incbin "baserom.gba", 0x00b5bba5, 0x00000003
	.section .rom.00b5bd12, "a"
	.incbin "baserom.gba", 0x00b5bd12, 0x00000002
	.section .rom.00b5eb63, "a"
	.incbin "baserom.gba", 0x00b5eb63, 0x00000001
	.section .rom.00b61322, "a"
	.incbin "baserom.gba", 0x00b61322, 0x00000002
	.section .rom.00b673d3, "a"
	.incbin "baserom.gba", 0x00b673d3, 0x00000001
	.section .rom.00b68f03, "a"
	.incbin "baserom.gba", 0x00b68f03, 0x00000001
	.section .rom.00b6b23a, "a"
	.incbin "baserom.gba", 0x00b6b23a, 0x00000002
	.section .rom.00b6c3eb, "a"
	.incbin "baserom.gba", 0x00b6c3eb, 0x00000001
	.section .rom.00b6dd0d, "a"
	.incbin "baserom.gba", 0x00b6dd0d, 0x00000003
	.section .rom.00b6fb99, "a"
	.incbin "baserom.gba", 0x00b6fb99, 0x00000003
	.section .rom.00b71d7a, "a"
	.incbin "baserom.gba", 0x00b71d7a, 0x00000002
	.section .rom.00b75741, "a"
	.incbin "baserom.gba", 0x00b75741, 0x00000003
	.section .rom.00b7582d, "a"
	.incbin "baserom.gba", 0x00b7582d, 0x00000003
	.section .rom.00b78a66, "a"
	.incbin "baserom.gba", 0x00b78a66, 0x00000002
	.section .rom.00b790dd, "a"
	.incbin "baserom.gba", 0x00b790dd, 0x00000003
	.section .rom.00b7ce9f, "a"
	.incbin "baserom.gba", 0x00b7ce9f, 0x00000001
	.section .rom.00b7f131, "a"
	.incbin "baserom.gba", 0x00b7f131, 0x00000003
	.section .rom.00b80b4e, "a"
	.incbin "baserom.gba", 0x00b80b4e, 0x00000002
	.section .rom.00b81c5b, "a"
	.incbin "baserom.gba", 0x00b81c5b, 0x00000001
	.section .rom.00b84dfa, "a"
	.incbin "baserom.gba", 0x00b84dfa, 0x00000002
	.section .rom.00b85ff2, "a"
	.incbin "baserom.gba", 0x00b85ff2, 0x00000002
	.section .rom.00b86eb5, "a"
	.incbin "baserom.gba", 0x00b86eb5, 0x00000003
	.section .rom.00b88f56, "a"
	.incbin "baserom.gba", 0x00b88f56, 0x00000002
	.section .rom.00b8a9ab, "a"
	.incbin "baserom.gba", 0x00b8a9ab, 0x00000001
	.section .rom.00b8be76, "a"
	.incbin "baserom.gba", 0x00b8be76, 0x00000002
	.section .rom.00b8e07a, "a"
	.incbin "baserom.gba", 0x00b8e07a, 0x00000002
	.section .rom.00b8e16f, "a"
	.incbin "baserom.gba", 0x00b8e16f, 0x00000001
	.section .rom.00b8f742, "a"
	.incbin "baserom.gba", 0x00b8f742, 0x00000002
	.section .rom.00b8f961, "a"
	.incbin "baserom.gba", 0x00b8f961, 0x00000003
	.section .rom.00b91a45, "a"
	.incbin "baserom.gba", 0x00b91a45, 0x00000003
	.section .rom.00b91c0a, "a"
	.incbin "baserom.gba", 0x00b91c0a, 0x00000002
	.section .rom.00b93ccd, "a"
	.incbin "baserom.gba", 0x00b93ccd, 0x00000003
	.section .rom.00b93dfd, "a"
	.incbin "baserom.gba", 0x00b93dfd, 0x00000003
	.section .rom.00b968b6, "a"
	.incbin "baserom.gba", 0x00b968b6, 0x00000002
	.section .rom.00b9840e, "a"
	.incbin "baserom.gba", 0x00b9840e, 0x00000002
	.section .rom.00b99355, "a"
	.incbin "baserom.gba", 0x00b99355, 0x00000003
	.section .rom.00b9ad5e, "a"
	.incbin "baserom.gba", 0x00b9ad5e, 0x00000002
	.section .rom.00baaab3, "a"
	.incbin "baserom.gba", 0x00baaab3, 0x00000001
	.section .rom.00baac03, "a"
	.incbin "baserom.gba", 0x00baac03, 0x00000001
	.section .rom.00bad70e, "a"
	.incbin "baserom.gba", 0x00bad70e, 0x00000002
	.section .rom.00baf317, "a"
	.incbin "baserom.gba", 0x00baf317, 0x00000001
	.section .rom.00bb0d1e, "a"
	.incbin "baserom.gba", 0x00bb0d1e, 0x00000002
	.section .rom.00bb2a39, "a"
	.incbin "baserom.gba", 0x00bb2a39, 0x00000003
	.section .rom.00bb2b8b, "a"
	.incbin "baserom.gba", 0x00bb2b8b, 0x00000001
	.section .rom.00bb4592, "a"
	.incbin "baserom.gba", 0x00bb4592, 0x00000002
	.section .rom.00bb831a, "a"
	.incbin "baserom.gba", 0x00bb831a, 0x00000002
	.section .rom.00bb84ad, "a"
	.incbin "baserom.gba", 0x00bb84ad, 0x00000003
	.section .rom.00bbd4b7, "a"
	.incbin "baserom.gba", 0x00bbd4b7, 0x00000001
	.section .rom.00bbfb39, "a"
	.incbin "baserom.gba", 0x00bbfb39, 0x00000003
	.section .rom.00bbfe6f, "a"
	.incbin "baserom.gba", 0x00bbfe6f, 0x00000001
	.section .rom.00bc62c5, "a"
	.incbin "baserom.gba", 0x00bc62c5, 0x00000003
	.section .rom.00bc6412, "a"
	.incbin "baserom.gba", 0x00bc6412, 0x00000002
	.section .rom.00bc8297, "a"
	.incbin "baserom.gba", 0x00bc8297, 0x00000001
	.section .rom.00bc983e, "a"
	.incbin "baserom.gba", 0x00bc983e, 0x00000002
	.section .rom.00bcb7e9, "a"
	.incbin "baserom.gba", 0x00bcb7e9, 0x00000003
	.section .rom.00bcc75a, "a"
	.incbin "baserom.gba", 0x00bcc75a, 0x00000002
	.section .rom.00bd7d63, "a"
	.incbin "baserom.gba", 0x00bd7d63, 0x00000001
	.section .rom.00bd7e1f, "a"
	.incbin "baserom.gba", 0x00bd7e1f, 0x00000001
	.section .rom.00bd8b5e, "a"
	.incbin "baserom.gba", 0x00bd8b5e, 0x00000002
	.section .rom.00bd953b, "a"
	.incbin "baserom.gba", 0x00bd953b, 0x00000001
	.section .rom.00bda153, "a"
	.incbin "baserom.gba", 0x00bda153, 0x00000001
	.section .rom.00bdc00e, "a"
	.incbin "baserom.gba", 0x00bdc00e, 0x00000002
	.section .rom.00bdc12a, "a"
	.incbin "baserom.gba", 0x00bdc12a, 0x00000002
	.section .rom.00bde9ab, "a"
	.incbin "baserom.gba", 0x00bde9ab, 0x00000001
	.section .rom.00be0136, "a"
	.incbin "baserom.gba", 0x00be0136, 0x00000002
	.section .rom.00be4066, "a"
	.incbin "baserom.gba", 0x00be4066, 0x00000002
	.section .rom.00be41af, "a"
	.incbin "baserom.gba", 0x00be41af, 0x00000001
	.section .rom.00be685f, "a"
	.incbin "baserom.gba", 0x00be685f, 0x00000001
	.section .rom.00be9002, "a"
	.incbin "baserom.gba", 0x00be9002, 0x00000002
	.section .rom.00be9d8d, "a"
	.incbin "baserom.gba", 0x00be9d8d, 0x00000003
	.section .rom.00becbce, "a"
	.incbin "baserom.gba", 0x00becbce, 0x00000002
	.section .rom.00bef1f5, "a"
	.incbin "baserom.gba", 0x00bef1f5, 0x00000003
	.section .rom.00bf0ee1, "a"
	.incbin "baserom.gba", 0x00bf0ee1, 0x00000003
	.section .rom.00bf25e1, "a"
	.incbin "baserom.gba", 0x00bf25e1, 0x00000003
	.section .rom.00bf3b5d, "a"
	.incbin "baserom.gba", 0x00bf3b5d, 0x00000003
	.section .rom.00bf52cf, "a"
	.incbin "baserom.gba", 0x00bf52cf, 0x00000001
	.section .rom.00bf7f5a, "a"
	.incbin "baserom.gba", 0x00bf7f5a, 0x00000002
	.section .rom.00bf8fa6, "a"
	.incbin "baserom.gba", 0x00bf8fa6, 0x00000002
	.section .rom.00bfa0a3, "a"
	.incbin "baserom.gba", 0x00bfa0a3, 0x00000001
	.section .rom.00bfb877, "a"
	.incbin "baserom.gba", 0x00bfb877, 0x00000001
	.section .rom.00bfcd3b, "a"
	.incbin "baserom.gba", 0x00bfcd3b, 0x00000001
	.section .rom.00bff26e, "a"
	.incbin "baserom.gba", 0x00bff26e, 0x00000002
	.section .rom.00bff3a2, "a"
	.incbin "baserom.gba", 0x00bff3a2, 0x00000002
	.section .rom.00c01b0e, "a"
	.incbin "baserom.gba", 0x00c01b0e, 0x00000002
	.section .rom.00c038e1, "a"
	.incbin "baserom.gba", 0x00c038e1, 0x00000003
	.section .rom.00c071c5, "a"
	.incbin "baserom.gba", 0x00c071c5, 0x00000003
	.section .rom.00c0992b, "a"
	.incbin "baserom.gba", 0x00c0992b, 0x00000001
	.section .rom.00c0b295, "a"
	.incbin "baserom.gba", 0x00c0b295, 0x00000003
	.section .rom.00c0dabe, "a"
	.incbin "baserom.gba", 0x00c0dabe, 0x00000002
	.section .rom.00c11e9d, "a"
	.incbin "baserom.gba", 0x00c11e9d, 0x00000003
	.section .rom.00c15247, "a"
	.incbin "baserom.gba", 0x00c15247, 0x00000001
	.section .rom.00c1603a, "a"
	.incbin "baserom.gba", 0x00c1603a, 0x00000002
	.section .rom.00c16db2, "a"
	.incbin "baserom.gba", 0x00c16db2, 0x00000002
	.section .rom.00c1eeda, "a"
	.incbin "baserom.gba", 0x00c1eeda, 0x00000002
	.section .rom.00c25259, "a"
	.incbin "baserom.gba", 0x00c25259, 0x00000003
	.section .rom.00c25c5f, "a"
	.incbin "baserom.gba", 0x00c25c5f, 0x00000001
	.section .rom.00c2770d, "a"
	.incbin "baserom.gba", 0x00c2770d, 0x00001493
	.section .rom.00c29007, "a"
	.incbin "baserom.gba", 0x00c29007, 0x00000001
	.section .rom.00c291c6, "a"
	.incbin "baserom.gba", 0x00c291c6, 0x00000002
	.section .rom.00c2b9ce, "a"
	.incbin "baserom.gba", 0x00c2b9ce, 0x00000002
	.section .rom.00c2bb26, "a"
	.incbin "baserom.gba", 0x00c2bb26, 0x00000002
	.section .rom.00c2e46d, "a"
	.incbin "baserom.gba", 0x00c2e46d, 0x00000003
	.section .rom.00c30691, "a"
	.incbin "baserom.gba", 0x00c30691, 0x00000003
	.section .rom.00c31b86, "a"
	.incbin "baserom.gba", 0x00c31b86, 0x00000002
	.section .rom.00c33de9, "a"
	.incbin "baserom.gba", 0x00c33de9, 0x00000003
	.section .rom.00c36697, "a"
	.incbin "baserom.gba", 0x00c36697, 0x00000001
	.section .rom.00c3862a, "a"
	.incbin "baserom.gba", 0x00c3862a, 0x00000002
	.section .rom.00c3964b, "a"
	.incbin "baserom.gba", 0x00c3964b, 0x00000001
	.section .rom.00c3978b, "a"
	.incbin "baserom.gba", 0x00c3978b, 0x00000001
	.section .rom.00c3c3aa, "a"
	.incbin "baserom.gba", 0x00c3c3aa, 0x00000002
	.section .rom.00c3e82e, "a"
	.incbin "baserom.gba", 0x00c3e82e, 0x00000002
	.section .rom.00c40915, "a"
	.incbin "baserom.gba", 0x00c40915, 0x00000003
	.section .rom.00c42179, "a"
	.incbin "baserom.gba", 0x00c42179, 0x00000003
	.section .rom.00c446df, "a"
	.incbin "baserom.gba", 0x00c446df, 0x00000001
	.section .rom.00c44845, "a"
	.incbin "baserom.gba", 0x00c44845, 0x00000003
	.section .rom.00c4703f, "a"
	.incbin "baserom.gba", 0x00c4703f, 0x00000001
	.section .rom.00c49073, "a"
	.incbin "baserom.gba", 0x00c49073, 0x00000001
	.section .rom.00c4ac9a, "a"
	.incbin "baserom.gba", 0x00c4ac9a, 0x00000002
	.section .rom.00c4d6ba, "a"
	.incbin "baserom.gba", 0x00c4d6ba, 0x00000002
	.section .rom.00c4e225, "a"
	.incbin "baserom.gba", 0x00c4e225, 0x00000003
	.section .rom.00c4e30b, "a"
	.incbin "baserom.gba", 0x00c4e30b, 0x00000001
	.section .rom.00c4fc55, "a"
	.incbin "baserom.gba", 0x00c4fc55, 0x00000003
	.section .rom.00c515bb, "a"
	.incbin "baserom.gba", 0x00c515bb, 0x00000001
	.section .rom.00c52989, "a"
	.incbin "baserom.gba", 0x00c52989, 0x00000003
	.section .rom.00c52a6f, "a"
	.incbin "baserom.gba", 0x00c52a6f, 0x00000001
	.section .rom.00c535dd, "a"
	.incbin "baserom.gba", 0x00c535dd, 0x00000003
	.section .rom.00c536c3, "a"
	.incbin "baserom.gba", 0x00c536c3, 0x00000001
	.section .rom.00c54427, "a"
	.incbin "baserom.gba", 0x00c54427, 0x00000001
	.section .rom.00c5450b, "a"
	.incbin "baserom.gba", 0x00c5450b, 0x00000001
	.section .rom.00c54d11, "a"
	.incbin "baserom.gba", 0x00c54d11, 0x00000003
	.section .rom.00c54df7, "a"
	.incbin "baserom.gba", 0x00c54df7, 0x00000001
	.section .rom.00c55957, "a"
	.incbin "baserom.gba", 0x00c55957, 0x00000001
	.section .rom.00c561d5, "a"
	.incbin "baserom.gba", 0x00c561d5, 0x00000003
	.section .rom.00c562d2, "a"
	.incbin "baserom.gba", 0x00c562d2, 0x00000002
	.section .rom.00c5812b, "a"
	.incbin "baserom.gba", 0x00c5812b, 0x00000001
	.section .rom.00c58f39, "a"
	.incbin "baserom.gba", 0x00c58f39, 0x00000003
	.section .rom.00c5a1cd, "a"
	.incbin "baserom.gba", 0x00c5a1cd, 0x00000003
	.section .rom.00c5b1a9, "a"
	.incbin "baserom.gba", 0x00c5b1a9, 0x00000003
	.section .rom.00c5d5af, "a"
	.incbin "baserom.gba", 0x00c5d5af, 0x00000001
	.section .rom.00c5d6ef, "a"
	.incbin "baserom.gba", 0x00c5d6ef, 0x00000001
	.section .rom.00c5ddbd, "a"
	.incbin "baserom.gba", 0x00c5ddbd, 0x00000003
	.section .rom.00c5de93, "a"
	.incbin "baserom.gba", 0x00c5de93, 0x00000001
	.section .rom.00c5e709, "a"
	.incbin "baserom.gba", 0x00c5e709, 0x00000003
	.section .rom.00c5ebba, "a"
	.incbin "baserom.gba", 0x00c5ebba, 0x00000002
	.section .rom.00c60035, "a"
	.incbin "baserom.gba", 0x00c60035, 0x00000003
	.section .rom.00c6084b, "a"
	.incbin "baserom.gba", 0x00c6084b, 0x00000001
	.section .rom.00c614e1, "a"
	.incbin "baserom.gba", 0x00c614e1, 0x00000003
	.section .rom.00c61686, "a"
	.incbin "baserom.gba", 0x00c61686, 0x00000002
	.section .rom.00c6454a, "a"
	.incbin "baserom.gba", 0x00c6454a, 0x00000002
	.section .rom.00c65ec9, "a"
	.incbin "baserom.gba", 0x00c65ec9, 0x00000003
	.section .rom.00c66f16, "a"
	.incbin "baserom.gba", 0x00c66f16, 0x00000002
	.section .rom.00c6e65b, "a"
	.incbin "baserom.gba", 0x00c6e65b, 0x00000001
	.section .rom.00c75e4f, "a"
	.incbin "baserom.gba", 0x00c75e4f, 0x00000001
	.section .rom.00c7c449, "a"
	.incbin "baserom.gba", 0x00c7c449, 0x00000003
	.section .rom.00c7ee82, "a"
	.incbin "baserom.gba", 0x00c7ee82, 0x00000002
	.section .rom.00c81753, "a"
	.incbin "baserom.gba", 0x00c81753, 0x00000001
	.section .rom.00c85f9f, "a"
	.incbin "baserom.gba", 0x00c85f9f, 0x00000001
	.section .rom.00c86f9d, "a"
	.incbin "baserom.gba", 0x00c86f9d, 0x00000003
	.section .rom.00c870b6, "a"
	.incbin "baserom.gba", 0x00c870b6, 0x00000002
	.section .rom.00c8865e, "a"
	.incbin "baserom.gba", 0x00c8865e, 0x00000002
	.section .rom.00c8a0bb, "a"
	.incbin "baserom.gba", 0x00c8a0bb, 0x00000001
	.section .rom.00c8a1f6, "a"
	.incbin "baserom.gba", 0x00c8a1f6, 0x00000002
	.section .rom.00c8b6fd, "a"
	.incbin "baserom.gba", 0x00c8b6fd, 0x00000003
	.section .rom.00c8e986, "a"
	.incbin "baserom.gba", 0x00c8e986, 0x00000002
	.section .rom.00c8fdcd, "a"
	.incbin "baserom.gba", 0x00c8fdcd, 0x00000003
	.section .rom.00c92ac5, "a"
	.incbin "baserom.gba", 0x00c92ac5, 0x00000003
	.section .rom.00c94efb, "a"
	.incbin "baserom.gba", 0x00c94efb, 0x00000001
	.section .rom.00c950d7, "a"
	.incbin "baserom.gba", 0x00c950d7, 0x00000001
	.section .rom.00c981eb, "a"
	.incbin "baserom.gba", 0x00c981eb, 0x00000001
	.section .rom.00c9966e, "a"
	.incbin "baserom.gba", 0x00c9966e, 0x00000002
	.section .rom.00c9a6bd, "a"
	.incbin "baserom.gba", 0x00c9a6bd, 0x00000003
	.section .rom.00c9c522, "a"
	.incbin "baserom.gba", 0x00c9c522, 0x00000002
	.section .rom.00c9c6bd, "a"
	.incbin "baserom.gba", 0x00c9c6bd, 0x00000003
	.section .rom.00c9c81b, "a"
	.incbin "baserom.gba", 0x00c9c81b, 0x00000001
	.section .rom.00c9d79a, "a"
	.incbin "baserom.gba", 0x00c9d79a, 0x00000002
	.section .rom.00c9d8b2, "a"
	.incbin "baserom.gba", 0x00c9d8b2, 0x00000002
	.section .rom.00c9f945, "a"
	.incbin "baserom.gba", 0x00c9f945, 0x00000003
	.section .rom.00ca17c7, "a"
	.incbin "baserom.gba", 0x00ca17c7, 0x00000001
	.section .rom.00ca3ced, "a"
	.incbin "baserom.gba", 0x00ca3ced, 0x00000003
	.section .rom.00ca3dfa, "a"
	.incbin "baserom.gba", 0x00ca3dfa, 0x00000002
	.section .rom.00ca5e8d, "a"
	.incbin "baserom.gba", 0x00ca5e8d, 0x00000003
	.section .rom.00ca9363, "a"
	.incbin "baserom.gba", 0x00ca9363, 0x00000001
	.section .rom.00ca9e4d, "a"
	.incbin "baserom.gba", 0x00ca9e4d, 0x00000003
	.section .rom.00ca9fbd, "a"
	.incbin "baserom.gba", 0x00ca9fbd, 0x00000003
	.section .rom.00caced7, "a"
	.incbin "baserom.gba", 0x00caced7, 0x00000001
	.section .rom.00caee62, "a"
	.incbin "baserom.gba", 0x00caee62, 0x00000002
	.section .rom.00cb07cd, "a"
	.incbin "baserom.gba", 0x00cb07cd, 0x00000003
	.section .rom.00cb62ba, "a"
	.incbin "baserom.gba", 0x00cb62ba, 0x00000002
	.section .rom.00cb646a, "a"
	.incbin "baserom.gba", 0x00cb646a, 0x00000002
	.section .rom.00cbade2, "a"
	.incbin "baserom.gba", 0x00cbade2, 0x00000002
	.section .rom.00cbaf3a, "a"
	.incbin "baserom.gba", 0x00cbaf3a, 0x00000002
	.section .rom.00cbe507, "a"
	.incbin "baserom.gba", 0x00cbe507, 0x00000001
	.section .rom.00cbfd89, "a"
	.incbin "baserom.gba", 0x00cbfd89, 0x00000003
	.section .rom.00cc1ab1, "a"
	.incbin "baserom.gba", 0x00cc1ab1, 0x00000003
	.section .rom.00cc51d2, "a"
	.incbin "baserom.gba", 0x00cc51d2, 0x00000002
	.section .rom.00cca35b, "a"
	.incbin "baserom.gba", 0x00cca35b, 0x00000001
	.section .rom.00ccc929, "a"
	.incbin "baserom.gba", 0x00ccc929, 0x00000003
	.section .rom.00cce2b5, "a"
	.incbin "baserom.gba", 0x00cce2b5, 0x00000003
	.section .rom.00ccf3e2, "a"
	.incbin "baserom.gba", 0x00ccf3e2, 0x00000002
	.section .rom.00ccff92, "a"
	.incbin "baserom.gba", 0x00ccff92, 0x00000002
	.section .rom.00cd195a, "a"
	.incbin "baserom.gba", 0x00cd195a, 0x00000002
	.section .rom.00cd1a3e, "a"
	.incbin "baserom.gba", 0x00cd1a3e, 0x00000002
	.section .rom.00cd3e9b, "a"
	.incbin "baserom.gba", 0x00cd3e9b, 0x00000001
	.section .rom.00cd3fdb, "a"
	.incbin "baserom.gba", 0x00cd3fdb, 0x00000001
	.section .rom.00cd7651, "a"
	.incbin "baserom.gba", 0x00cd7651, 0x00000003
	.section .rom.00cd77b5, "a"
	.incbin "baserom.gba", 0x00cd77b5, 0x00000003
	.section .rom.00cd9e95, "a"
	.incbin "baserom.gba", 0x00cd9e95, 0x00000003
	.section .rom.00cdcd45, "a"
	.incbin "baserom.gba", 0x00cdcd45, 0x00000003
	.section .rom.00cde2ba, "a"
	.incbin "baserom.gba", 0x00cde2ba, 0x00000002
	.section .rom.00ce23b6, "a"
	.incbin "baserom.gba", 0x00ce23b6, 0x00000002
	.section .rom.00ce463b, "a"
	.incbin "baserom.gba", 0x00ce463b, 0x00000001
	.section .rom.00ce621f, "a"
	.incbin "baserom.gba", 0x00ce621f, 0x00000001
	.section .rom.00ce7f15, "a"
	.incbin "baserom.gba", 0x00ce7f15, 0x00000003
	.section .rom.00cf5539, "a"
	.incbin "baserom.gba", 0x00cf5539, 0x00000003
	.section .rom.00cf5661, "a"
	.incbin "baserom.gba", 0x00cf5661, 0x00000003
	.section .rom.00cf7872, "a"
	.incbin "baserom.gba", 0x00cf7872, 0x00000002
	.section .rom.00cf7a03, "a"
	.incbin "baserom.gba", 0x00cf7a03, 0x00000001
	.section .rom.00cfeeb5, "a"
	.incbin "baserom.gba", 0x00cfeeb5, 0x00000003
	.section .rom.00d0168b, "a"
	.incbin "baserom.gba", 0x00d0168b, 0x00000001
	.section .rom.00d03de6, "a"
	.incbin "baserom.gba", 0x00d03de6, 0x00000002
	.section .rom.00d03f99, "a"
	.incbin "baserom.gba", 0x00d03f99, 0x00000003
	.section .rom.00d0569d, "a"
	.incbin "baserom.gba", 0x00d0569d, 0x00000003
	.section .rom.00d076f7, "a"
	.incbin "baserom.gba", 0x00d076f7, 0x00000001
	.section .rom.00d08a41, "a"
	.incbin "baserom.gba", 0x00d08a41, 0x00000003
	.section .rom.00d0a48b, "a"
	.incbin "baserom.gba", 0x00d0a48b, 0x00000001
	.section .rom.00d0a5af, "a"
	.incbin "baserom.gba", 0x00d0a5af, 0x00000001
	.section .rom.00d0aa81, "a"
	.incbin "baserom.gba", 0x00d0aa81, 0x00000003
	.section .rom.00d0cc65, "a"
	.incbin "baserom.gba", 0x00d0cc65, 0x00000003
	.section .rom.00d0d139, "a"
	.incbin "baserom.gba", 0x00d0d139, 0x00000003
	.section .rom.00d0f66f, "a"
	.incbin "baserom.gba", 0x00d0f66f, 0x00000001
	.section .rom.00d0f7f7, "a"
	.incbin "baserom.gba", 0x00d0f7f7, 0x00000001
	.section .rom.00d14099, "a"
	.incbin "baserom.gba", 0x00d14099, 0x00000003
	.section .rom.00d141cf, "a"
	.incbin "baserom.gba", 0x00d141cf, 0x00000001
	.section .rom.00d15e3b, "a"
	.incbin "baserom.gba", 0x00d15e3b, 0x00000001
	.section .rom.00d16ff3, "a"
	.incbin "baserom.gba", 0x00d16ff3, 0x00000001
	.section .rom.00d17d41, "a"
	.incbin "baserom.gba", 0x00d17d41, 0x00000003
	.section .rom.00d18eca, "a"
	.incbin "baserom.gba", 0x00d18eca, 0x00000002
	.section .rom.00d1a9c3, "a"
	.incbin "baserom.gba", 0x00d1a9c3, 0x00000001
	.section .rom.00d1bdcf, "a"
	.incbin "baserom.gba", 0x00d1bdcf, 0x00000001
	.section .rom.00d1c323, "a"
	.incbin "baserom.gba", 0x00d1c323, 0x00000001
	.section .rom.00d1c95d, "a"
	.incbin "baserom.gba", 0x00d1c95d, 0x00000003
	.section .rom.00d21cad, "a"
	.incbin "baserom.gba", 0x00d21cad, 0x00000003
	.section .rom.00d2430e, "a"
	.incbin "baserom.gba", 0x00d2430e, 0x00000002
	.section .rom.00d244a3, "a"
	.incbin "baserom.gba", 0x00d244a3, 0x00000001
	.section .rom.00d26057, "a"
	.incbin "baserom.gba", 0x00d26057, 0x00000001
	.section .rom.00d261b7, "a"
	.incbin "baserom.gba", 0x00d261b7, 0x00000001
	.section .rom.00d28c5b, "a"
	.incbin "baserom.gba", 0x00d28c5b, 0x000021b1
	.section .rom.00d2cc82, "a"
	.incbin "baserom.gba", 0x00d2cc82, 0x00000002
	.section .rom.00d2cdc3, "a"
	.incbin "baserom.gba", 0x00d2cdc3, 0x00000001
	.section .rom.00d33d3b, "a"
	.incbin "baserom.gba", 0x00d33d3b, 0x00000001
	.section .rom.00d33e79, "a"
	.incbin "baserom.gba", 0x00d33e79, 0x00000003
	.section .rom.00d35e4d, "a"
	.incbin "baserom.gba", 0x00d35e4d, 0x00000003
	.section .rom.00d35f41, "a"
	.incbin "baserom.gba", 0x00d35f41, 0x00000003
	.section .rom.00d37021, "a"
	.incbin "baserom.gba", 0x00d37021, 0x00000003
	.section .rom.00d38efb, "a"
	.incbin "baserom.gba", 0x00d38efb, 0x00000001
	.section .rom.00d39eb3, "a"
	.incbin "baserom.gba", 0x00d39eb3, 0x00000001
	.section .rom.00d3bdfa, "a"
	.incbin "baserom.gba", 0x00d3bdfa, 0x00000002
	.section .rom.00d3da05, "a"
	.incbin "baserom.gba", 0x00d3da05, 0x00000003
	.section .rom.00d3db51, "a"
	.incbin "baserom.gba", 0x00d3db51, 0x00000003
	.section .rom.00d3fc0f, "a"
	.incbin "baserom.gba", 0x00d3fc0f, 0x00000001
	.section .rom.00d43cfe, "a"
	.incbin "baserom.gba", 0x00d43cfe, 0x00000002
	.section .rom.00d47ab0, "a"
	.incbin "baserom.gba", 0x00d47ab0, 0x00007aa8
	.section .rom.00d50ca5, "a"
	.incbin "baserom.gba", 0x00d50ca5, 0x00000003
	.section .rom.00d50dd2, "a"
	.incbin "baserom.gba", 0x00d50dd2, 0x00000002
	.section .rom.00d57d45, "a"
	.incbin "baserom.gba", 0x00d57d45, 0x00000003
	.section .rom.00d57e4a, "a"
	.incbin "baserom.gba", 0x00d57e4a, 0x00000002
	.section .rom.00d57f8a, "a"
	.incbin "baserom.gba", 0x00d57f8a, 0x00000002
	.section .rom.00d59956, "a"
	.incbin "baserom.gba", 0x00d59956, 0x00000002
	.section .rom.00d59a4d, "a"
	.incbin "baserom.gba", 0x00d59a4d, 0x00000003
	.section .rom.00d5c8a1, "a"
	.incbin "baserom.gba", 0x00d5c8a1, 0x00000003
	.section .rom.00d5ca4a, "a"
	.incbin "baserom.gba", 0x00d5ca4a, 0x00000002
	.section .rom.00d5f666, "a"
	.incbin "baserom.gba", 0x00d5f666, 0x00000002
	.section .rom.00d62399, "a"
	.incbin "baserom.gba", 0x00d62399, 0x00000003
	.section .rom.00d64083, "a"
	.incbin "baserom.gba", 0x00d64083, 0x00000001
	.section .rom.00d678fd, "a"
	.incbin "baserom.gba", 0x00d678fd, 0x00000003
	.section .rom.00d67a55, "a"
	.incbin "baserom.gba", 0x00d67a55, 0x00000003
	.section .rom.00d69f99, "a"
	.incbin "baserom.gba", 0x00d69f99, 0x00000003
	.section .rom.00d6de5e, "a"
	.incbin "baserom.gba", 0x00d6de5e, 0x00000002
	.section .rom.00d6ff8e, "a"
	.incbin "baserom.gba", 0x00d6ff8e, 0x00000002
	.section .rom.00d7419a, "a"
	.incbin "baserom.gba", 0x00d7419a, 0x00000002
	.section .rom.00d7436e, "a"
	.incbin "baserom.gba", 0x00d7436e, 0x00000002
	.section .rom.00d763ae, "a"
	.incbin "baserom.gba", 0x00d763ae, 0x00000002
	.section .rom.00d777a5, "a"
	.incbin "baserom.gba", 0x00d777a5, 0x00000003
	.section .rom.00d782e2, "a"
	.incbin "baserom.gba", 0x00d782e2, 0x00000002
	.section .rom.00d7944d, "a"
	.incbin "baserom.gba", 0x00d7944d, 0x00000003
	.section .rom.00d7a4f9, "a"
	.incbin "baserom.gba", 0x00d7a4f9, 0x00000003
	.section .rom.00d7a6a5, "a"
	.incbin "baserom.gba", 0x00d7a6a5, 0x00000003
	.section .rom.00d7c1b2, "a"
	.incbin "baserom.gba", 0x00d7c1b2, 0x00000002
	.section .rom.00d7dbcf, "a"
	.incbin "baserom.gba", 0x00d7dbcf, 0x00000001
	.section .rom.00d800a1, "a"
	.incbin "baserom.gba", 0x00d800a1, 0x00000003
	.section .rom.00d81403, "a"
	.incbin "baserom.gba", 0x00d81403, 0x00000001
	.section .rom.00d822b7, "a"
	.incbin "baserom.gba", 0x00d822b7, 0x00000001
	.section .rom.00d838a5, "a"
	.incbin "baserom.gba", 0x00d838a5, 0x00000003
	.section .rom.00d84deb, "a"
	.incbin "baserom.gba", 0x00d84deb, 0x00000001
	.section .rom.00d85bb5, "a"
	.incbin "baserom.gba", 0x00d85bb5, 0x00000003
	.section .rom.00d85d23, "a"
	.incbin "baserom.gba", 0x00d85d23, 0x00000001
	.section .rom.00d86ce2, "a"
	.incbin "baserom.gba", 0x00d86ce2, 0x00000002
	.section .rom.00d877ab, "a"
	.incbin "baserom.gba", 0x00d877ab, 0x00000001
	.section .rom.00d88345, "a"
	.incbin "baserom.gba", 0x00d88345, 0x00000003
	.section .rom.00d88495, "a"
	.incbin "baserom.gba", 0x00d88495, 0x00000003
	.section .rom.00d8c743, "a"
	.incbin "baserom.gba", 0x00d8c743, 0x00000001
	.section .rom.00d8e1dd, "a"
	.incbin "baserom.gba", 0x00d8e1dd, 0x00000003
	.section .rom.00d8fbb3, "a"
	.incbin "baserom.gba", 0x00d8fbb3, 0x00000001
	.section .rom.00d8fd32, "a"
	.incbin "baserom.gba", 0x00d8fd32, 0x00000002
	.section .rom.00d93962, "a"
	.incbin "baserom.gba", 0x00d93962, 0x00000002
	.section .rom.00d956bf, "a"
	.incbin "baserom.gba", 0x00d956bf, 0x00000001
	.section .rom.00d98ce9, "a"
	.incbin "baserom.gba", 0x00d98ce9, 0x00000003
	.section .rom.00d98e3a, "a"
	.incbin "baserom.gba", 0x00d98e3a, 0x00000002
	.section .rom.00d9ea36, "a"
	.incbin "baserom.gba", 0x00d9ea36, 0x00000002
	.section .rom.00da1373, "a"
	.incbin "baserom.gba", 0x00da1373, 0x00000001
	.section .rom.00da2cb5, "a"
	.incbin "baserom.gba", 0x00da2cb5, 0x00000003
	.section .rom.00da2e1e, "a"
	.incbin "baserom.gba", 0x00da2e1e, 0x00000002
	.section .rom.00da9c1d, "a"
	.incbin "baserom.gba", 0x00da9c1d, 0x00000003
	.section .rom.00daa1fb, "a"
	.incbin "baserom.gba", 0x00daa1fb, 0x00000001
	.section .rom.00dab103, "a"
	.incbin "baserom.gba", 0x00dab103, 0x00000001
	.section .rom.00dad94b, "a"
	.incbin "baserom.gba", 0x00dad94b, 0x00000001
	.section .rom.00daef6a, "a"
	.incbin "baserom.gba", 0x00daef6a, 0x00000002
	.section .rom.00db08cb, "a"
	.incbin "baserom.gba", 0x00db08cb, 0x00000001
	.section .rom.00db36b3, "a"
	.incbin "baserom.gba", 0x00db36b3, 0x00000001
	.section .rom.00db382e, "a"
	.incbin "baserom.gba", 0x00db382e, 0x00000002
	.section .rom.00dc29f7, "a"
	.incbin "baserom.gba", 0x00dc29f7, 0x00000001
	.section .rom.00dc2b5f, "a"
	.incbin "baserom.gba", 0x00dc2b5f, 0x00000001
	.section .rom.00dc5182, "a"
	.incbin "baserom.gba", 0x00dc5182, 0x00000002
	.section .rom.00dc52c5, "a"
	.incbin "baserom.gba", 0x00dc52c5, 0x00000003
	.section .rom.00dc6b82, "a"
	.incbin "baserom.gba", 0x00dc6b82, 0x00000002
	.section .rom.00dc9ae6, "a"
	.incbin "baserom.gba", 0x00dc9ae6, 0x00000002
	.section .rom.00dcc47e, "a"
	.incbin "baserom.gba", 0x00dcc47e, 0x00000002
	.section .rom.00dd0dd6, "a"
	.incbin "baserom.gba", 0x00dd0dd6, 0x00000002
	.section .rom.00dd33ff, "a"
	.incbin "baserom.gba", 0x00dd33ff, 0x00000001
	.section .rom.00dd35d3, "a"
	.incbin "baserom.gba", 0x00dd35d3, 0x00000001
	.section .rom.00dd68f2, "a"
	.incbin "baserom.gba", 0x00dd68f2, 0x00000002
	.section .rom.00dd6ac9, "a"
	.incbin "baserom.gba", 0x00dd6ac9, 0x00000003
	.section .rom.00dd9e3d, "a"
	.incbin "baserom.gba", 0x00dd9e3d, 0x00000003
	.section .rom.00ddb591, "a"
	.incbin "baserom.gba", 0x00ddb591, 0x00000003
	.section .rom.00ddc51f, "a"
	.incbin "baserom.gba", 0x00ddc51f, 0x00000001
	.section .rom.00ddde95, "a"
	.incbin "baserom.gba", 0x00ddde95, 0x00000003
	.section .rom.00dddfcf, "a"
	.incbin "baserom.gba", 0x00dddfcf, 0x00000001
	.section .rom.00ddfb11, "a"
	.incbin "baserom.gba", 0x00ddfb11, 0x00000003
	.section .rom.00de30e9, "a"
	.incbin "baserom.gba", 0x00de30e9, 0x00000003
	.section .rom.00de3d73, "a"
	.incbin "baserom.gba", 0x00de3d73, 0x00000001
	.section .rom.00de3eb3, "a"
	.incbin "baserom.gba", 0x00de3eb3, 0x00000001
	.section .rom.00de5c77, "a"
	.incbin "baserom.gba", 0x00de5c77, 0x00000001
	.section .rom.00de5dea, "a"
	.incbin "baserom.gba", 0x00de5dea, 0x00000002
	.section .rom.00de8252, "a"
	.incbin "baserom.gba", 0x00de8252, 0x00000002
	.section .rom.00de9bee, "a"
	.incbin "baserom.gba", 0x00de9bee, 0x00000002
	.section .rom.00deba23, "a"
	.incbin "baserom.gba", 0x00deba23, 0x00000001
	.section .rom.00dec4da, "a"
	.incbin "baserom.gba", 0x00dec4da, 0x00000002
	.section .rom.00dedef6, "a"
	.incbin "baserom.gba", 0x00dedef6, 0x00000002
	.section .rom.00df0bc2, "a"
	.incbin "baserom.gba", 0x00df0bc2, 0x00000002
	.section .rom.00df0d9d, "a"
	.incbin "baserom.gba", 0x00df0d9d, 0x00000003
	.section .rom.00df28d3, "a"
	.incbin "baserom.gba", 0x00df28d3, 0x00000001
	.section .rom.00df599d, "a"
	.incbin "baserom.gba", 0x00df599d, 0x00000003
	.section .rom.00df6b59, "a"
	.incbin "baserom.gba", 0x00df6b59, 0x00000003
	.section .rom.00df6d41, "a"
	.incbin "baserom.gba", 0x00df6d41, 0x00000003
	.section .rom.00df6ed3, "a"
	.incbin "baserom.gba", 0x00df6ed3, 0x00000001
	.section .rom.00df7c5a, "a"
	.incbin "baserom.gba", 0x00df7c5a, 0x00000002
	.section .rom.00df7def, "a"
	.incbin "baserom.gba", 0x00df7def, 0x00000001
	.section .rom.00df9ea3, "a"
	.incbin "baserom.gba", 0x00df9ea3, 0x00000001
	.section .rom.00e01f26, "a"
	.incbin "baserom.gba", 0x00e01f26, 0x00000002
	.section .rom.00e02067, "a"
	.incbin "baserom.gba", 0x00e02067, 0x00000001
	.section .rom.00e063ef, "a"
	.incbin "baserom.gba", 0x00e063ef, 0x00000001
	.section .rom.00e06b0e, "a"
	.incbin "baserom.gba", 0x00e06b0e, 0x00000002
	.section .rom.00e07ecd, "a"
	.incbin "baserom.gba", 0x00e07ecd, 0x00000003
	.section .rom.00e0bfe7, "a"
	.incbin "baserom.gba", 0x00e0bfe7, 0x00000001
	.section .rom.00e0d067, "a"
	.incbin "baserom.gba", 0x00e0d067, 0x00000001
	.section .rom.00e0e0e9, "a"
	.incbin "baserom.gba", 0x00e0e0e9, 0x00000003
	.section .rom.00e0ec5a, "a"
	.incbin "baserom.gba", 0x00e0ec5a, 0x00000002
	.section .rom.00e0fbab, "a"
	.incbin "baserom.gba", 0x00e0fbab, 0x00000001
	.section .rom.00e0fd29, "a"
	.incbin "baserom.gba", 0x00e0fd29, 0x00000003
	.section .rom.00e13283, "a"
	.incbin "baserom.gba", 0x00e13283, 0x00000001
	.section .rom.00e148b6, "a"
	.incbin "baserom.gba", 0x00e148b6, 0x00000002
	.section .rom.00e149f7, "a"
	.incbin "baserom.gba", 0x00e149f7, 0x00000001
	.section .rom.00e17342, "a"
	.incbin "baserom.gba", 0x00e17342, 0x00000002
	.section .rom.00e1fcea, "a"
	.incbin "baserom.gba", 0x00e1fcea, 0x00000fde
	.section .rom.00e22895, "a"
	.incbin "baserom.gba", 0x00e22895, 0x00000003
	.section .rom.00e247d5, "a"
	.incbin "baserom.gba", 0x00e247d5, 0x00000003
	.section .rom.00e25c49, "a"
	.incbin "baserom.gba", 0x00e25c49, 0x00000003
	.section .rom.00e27903, "a"
	.incbin "baserom.gba", 0x00e27903, 0x00000001
	.section .rom.00e349b2, "a"
	.incbin "baserom.gba", 0x00e349b2, 0x00000002
	.section .rom.00e34af1, "a"
	.incbin "baserom.gba", 0x00e34af1, 0x00000003
	.section .rom.00e36a16, "a"
	.incbin "baserom.gba", 0x00e36a16, 0x00000002
	.section .rom.00e36b3f, "a"
	.incbin "baserom.gba", 0x00e36b3f, 0x00000001
	.section .rom.00e38d0f, "a"
	.incbin "baserom.gba", 0x00e38d0f, 0x00000001
	.section .rom.00e3b2df, "a"
	.incbin "baserom.gba", 0x00e3b2df, 0x00000001
	.section .rom.00e3d5f1, "a"
	.incbin "baserom.gba", 0x00e3d5f1, 0x00000003
	.section .rom.00e3de65, "a"
	.incbin "baserom.gba", 0x00e3de65, 0x00000003
	.section .rom.00e3dfa7, "a"
	.incbin "baserom.gba", 0x00e3dfa7, 0x00000001
	.section .rom.00e4105d, "a"
	.incbin "baserom.gba", 0x00e4105d, 0x00000003
	.section .rom.00e42743, "a"
	.incbin "baserom.gba", 0x00e42743, 0x00000001
	.section .rom.00e4430b, "a"
	.incbin "baserom.gba", 0x00e4430b, 0x00000001
	.section .rom.00e45e86, "a"
	.incbin "baserom.gba", 0x00e45e86, 0x00000002
	.section .rom.00e462d1, "a"
	.incbin "baserom.gba", 0x00e462d1, 0x00000003
	.section .rom.00e48432, "a"
	.incbin "baserom.gba", 0x00e48432, 0x00000002
	.section .rom.00e4854b, "a"
	.incbin "baserom.gba", 0x00e4854b, 0x00000001
	.section .rom.00e49375, "a"
	.incbin "baserom.gba", 0x00e49375, 0x00000003
	.section .rom.00e494d7, "a"
	.incbin "baserom.gba", 0x00e494d7, 0x00000001
	.section .rom.00e4dd3f, "a"
	.incbin "baserom.gba", 0x00e4dd3f, 0x00000001
	.section .rom.00e503fd, "a"
	.incbin "baserom.gba", 0x00e503fd, 0x00000003
	.section .rom.00e509db, "a"
	.incbin "baserom.gba", 0x00e509db, 0x00000001
	.section .rom.00e5296e, "a"
	.incbin "baserom.gba", 0x00e5296e, 0x00000002
	.section .rom.00e5414e, "a"
	.incbin "baserom.gba", 0x00e5414e, 0x00000002
	.section .rom.00e542b2, "a"
	.incbin "baserom.gba", 0x00e542b2, 0x00000002
	.section .rom.00e56aaa, "a"
	.incbin "baserom.gba", 0x00e56aaa, 0x00000002
	.section .rom.00e56c63, "a"
	.incbin "baserom.gba", 0x00e56c63, 0x00000001
	.section .rom.00e56deb, "a"
	.incbin "baserom.gba", 0x00e56deb, 0x00000001
	.section .rom.00e5860d, "a"
	.incbin "baserom.gba", 0x00e5860d, 0x00000003
	.section .rom.00e591f6, "a"
	.incbin "baserom.gba", 0x00e591f6, 0x00000002
	.section .rom.00e5a72f, "a"
	.incbin "baserom.gba", 0x00e5a72f, 0x00000001
	.section .rom.00e5a8ca, "a"
	.incbin "baserom.gba", 0x00e5a8ca, 0x00000002
	.section .rom.00e5cb76, "a"
	.incbin "baserom.gba", 0x00e5cb76, 0x00000002
	.section .rom.00e5ed3d, "a"
	.incbin "baserom.gba", 0x00e5ed3d, 0x00000003
	.section .rom.00e6043b, "a"
	.incbin "baserom.gba", 0x00e6043b, 0x00000001
	.section .rom.00e60f5f, "a"
	.incbin "baserom.gba", 0x00e60f5f, 0x00000001
	.section .rom.00e65e02, "a"
	.incbin "baserom.gba", 0x00e65e02, 0x00000002
	.section .rom.00e681a9, "a"
	.incbin "baserom.gba", 0x00e681a9, 0x00000003
	.section .rom.00e68892, "a"
	.incbin "baserom.gba", 0x00e68892, 0x00000002
	.section .rom.00e6947e, "a"
	.incbin "baserom.gba", 0x00e6947e, 0x00000ac6
	.section .rom.00e6b4f3, "a"
	.incbin "baserom.gba", 0x00e6b4f3, 0x00000001
	.section .rom.00e6b6a6, "a"
	.incbin "baserom.gba", 0x00e6b6a6, 0x00000002
	.section .rom.00e6d8e5, "a"
	.incbin "baserom.gba", 0x00e6d8e5, 0x00000003
	.section .rom.00e6e40b, "a"
	.incbin "baserom.gba", 0x00e6e40b, 0x00000001
	.section .rom.00e78e9c, "a"
	.incbin "baserom.gba", 0x00e78e9c, 0x000007d0
	.section .rom.00e7c335, "a"
	.incbin "baserom.gba", 0x00e7c335, 0x00000003
	.section .rom.00e7c4af, "a"
	.incbin "baserom.gba", 0x00e7c4af, 0x00000001
	.section .rom.00e7c626, "a"
	.incbin "baserom.gba", 0x00e7c626, 0x00000002
	.section .rom.00e7c7be, "a"
	.incbin "baserom.gba", 0x00e7c7be, 0x00000002
	.section .rom.00e7c94f, "a"
	.incbin "baserom.gba", 0x00e7c94f, 0x00000001
	.section .rom.00e7ebfb, "a"
	.incbin "baserom.gba", 0x00e7ebfb, 0x00000001
	.section .rom.00e7ed13, "a"
	.incbin "baserom.gba", 0x00e7ed13, 0x00000001
	.section .rom.00e80b15, "a"
	.incbin "baserom.gba", 0x00e80b15, 0x00000003
	.section .rom.00e825da, "a"
	.incbin "baserom.gba", 0x00e825da, 0x00000002
	.section .rom.00e82d02, "a"
	.incbin "baserom.gba", 0x00e82d02, 0x00000002
	.section .rom.00e840fe, "a"
	.incbin "baserom.gba", 0x00e840fe, 0x00000002
	.section .rom.00e84fd2, "a"
	.incbin "baserom.gba", 0x00e84fd2, 0x00000002
	.section .rom.00e85116, "a"
	.incbin "baserom.gba", 0x00e85116, 0x00000002
	.section .rom.00e874c7, "a"
	.incbin "baserom.gba", 0x00e874c7, 0x00000001
	.section .rom.00e89cf5, "a"
	.incbin "baserom.gba", 0x00e89cf5, 0x00000003
	.section .rom.00e8a0f2, "a"
	.incbin "baserom.gba", 0x00e8a0f2, 0x00000002
	.section .rom.00e8b592, "a"
	.incbin "baserom.gba", 0x00e8b592, 0x00000002
	.section .rom.00e8b6e6, "a"
	.incbin "baserom.gba", 0x00e8b6e6, 0x00000002
	.section .rom.00e8f033, "a"
	.incbin "baserom.gba", 0x00e8f033, 0x00000001
	.section .rom.00e9049e, "a"
	.incbin "baserom.gba", 0x00e9049e, 0x00000002
	.section .rom.00e91977, "a"
	.incbin "baserom.gba", 0x00e91977, 0x00000001
	.section .rom.00e91aca, "a"
	.incbin "baserom.gba", 0x00e91aca, 0x00000002
	.section .rom.00e954d7, "a"
	.incbin "baserom.gba", 0x00e954d7, 0x00000001
	.section .rom.00e966af, "a"
	.incbin "baserom.gba", 0x00e966af, 0x00000001
	.section .rom.00e97eba, "a"
	.incbin "baserom.gba", 0x00e97eba, 0x00000002
	.section .rom.00e9800a, "a"
	.incbin "baserom.gba", 0x00e9800a, 0x00000002
	.section .rom.00e9a717, "a"
	.incbin "baserom.gba", 0x00e9a717, 0x00000001
	.section .rom.00e9a87e, "a"
	.incbin "baserom.gba", 0x00e9a87e, 0x00000002
	.section .rom.00e9d83f, "a"
	.incbin "baserom.gba", 0x00e9d83f, 0x00000001
	.section .rom.00e9eaad, "a"
	.incbin "baserom.gba", 0x00e9eaad, 0x00000003
	.section .rom.00ea163a, "a"
	.incbin "baserom.gba", 0x00ea163a, 0x00000002
	.section .rom.00ea2021, "a"
	.incbin "baserom.gba", 0x00ea2021, 0x00000003
	.section .rom.00ea29c2, "a"
	.incbin "baserom.gba", 0x00ea29c2, 0x00000002
	.section .rom.00ea2b36, "a"
	.incbin "baserom.gba", 0x00ea2b36, 0x00000002
	.section .rom.00ea4647, "a"
	.incbin "baserom.gba", 0x00ea4647, 0x00000001
	.section .rom.00ea4769, "a"
	.incbin "baserom.gba", 0x00ea4769, 0x00000003
	.section .rom.00ea7267, "a"
	.incbin "baserom.gba", 0x00ea7267, 0x00000001
	.section .rom.00ea805e, "a"
	.incbin "baserom.gba", 0x00ea805e, 0x00000002
	.section .rom.00ea9ffd, "a"
	.incbin "baserom.gba", 0x00ea9ffd, 0x00000003
	.section .rom.00eab32d, "a"
	.incbin "baserom.gba", 0x00eab32d, 0x00000003
	.section .rom.00eab481, "a"
	.incbin "baserom.gba", 0x00eab481, 0x00000003
	.section .rom.00eac033, "a"
	.incbin "baserom.gba", 0x00eac033, 0x00000001
	.section .rom.00ead80d, "a"
	.incbin "baserom.gba", 0x00ead80d, 0x00000003
	.section .rom.00eae96d, "a"
	.incbin "baserom.gba", 0x00eae96d, 0x00000003
	.section .rom.00eafa8b, "a"
	.incbin "baserom.gba", 0x00eafa8b, 0x00000001
	.section .rom.00eafc92, "a"
	.incbin "baserom.gba", 0x00eafc92, 0x00000002
	.section .rom.00eb0987, "a"
	.incbin "baserom.gba", 0x00eb0987, 0x00000001
	.section .rom.00eb3129, "a"
	.incbin "baserom.gba", 0x00eb3129, 0x00000003
	.section .rom.00eb489e, "a"
	.incbin "baserom.gba", 0x00eb489e, 0x00000002
	.section .rom.00eb5b5b, "a"
	.incbin "baserom.gba", 0x00eb5b5b, 0x00000001
	.section .rom.00eb639e, "a"
	.incbin "baserom.gba", 0x00eb639e, 0x0000060e
	.section .rom.00eb6f23, "a"
	.incbin "baserom.gba", 0x00eb6f23, 0x00000001
	.section .rom.00eb7063, "a"
	.incbin "baserom.gba", 0x00eb7063, 0x00000001
	.section .rom.00eb71a3, "a"
	.incbin "baserom.gba", 0x00eb71a3, 0x00000001
	.section .rom.00eb7d05, "a"
	.incbin "baserom.gba", 0x00eb7d05, 0x00000003
	.section .rom.00eba4a9, "a"
	.incbin "baserom.gba", 0x00eba4a9, 0x00000003
	.section .rom.00ebbc1e, "a"
	.incbin "baserom.gba", 0x00ebbc1e, 0x00000002
	.section .rom.00ebcedb, "a"
	.incbin "baserom.gba", 0x00ebcedb, 0x00000001
	.section .rom.00ebd71e, "a"
	.incbin "baserom.gba", 0x00ebd71e, 0x00000002
	.section .rom.00ebdcbd, "a"
	.incbin "baserom.gba", 0x00ebdcbd, 0x0000051f
	.section .rom.00ebe8f1, "a"
	.incbin "baserom.gba", 0x00ebe8f1, 0x0000051f
	.section .rom.00ebf9d1, "a"
	.incbin "baserom.gba", 0x00ebf9d1, 0x0000051f
	.section .rom.00ec04bd, "a"
	.incbin "baserom.gba", 0x00ec04bd, 0x0000051f
	.section .rom.00ec1359, "a"
	.incbin "baserom.gba", 0x00ec1359, 0x0000051f
	.section .rom.00ec1ca1, "a"
	.incbin "baserom.gba", 0x00ec1ca1, 0x0000051f
	.section .rom.00ec27e9, "a"
	.incbin "baserom.gba", 0x00ec27e9, 0x0000051f
	.section .rom.00ec3431, "a"
	.incbin "baserom.gba", 0x00ec3431, 0x0013cbcf
