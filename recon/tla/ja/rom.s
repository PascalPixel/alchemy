@ tla-ja's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.incbin "baserom.gba", 0x00002064, 0x00058868
	.section .rom.0005cdcc, "a"
	.incbin "baserom.gba", 0x0005cdcc, 0x00003778
	.section .rom.0009d0d0, "a"
	.incbin "baserom.gba", 0x0009d0d0, 0x00124240
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
	.section .rom.006322b0, "a"
	.incbin "baserom.gba", 0x006322b0, 0x0004fd60
	.section .rom.006848d0, "a"
	.incbin "baserom.gba", 0x006848d0, 0x000000c0
	.section .rom.0068a9c1, "a"
	.incbin "baserom.gba", 0x0068a9c1, 0x0000ddab
	.section .rom.006a5301, "a"
	.incbin "baserom.gba", 0x006a5301, 0x00003fb7
	.section .rom.006aa7f5, "a"
	.incbin "baserom.gba", 0x006aa7f5, 0x0002476b
	.section .rom.006d194e, "a"
	.incbin "baserom.gba", 0x006d194e, 0x00000002
	.section .rom.006d5d92, "a"
	.incbin "baserom.gba", 0x006d5d92, 0x00000002
	.section .rom.006e6502, "a"
	.incbin "baserom.gba", 0x006e6502, 0x00000002
	.section .rom.006e9af2, "a"
	.incbin "baserom.gba", 0x006e9af2, 0x00000002
	.section .rom.006f5596, "a"
	.incbin "baserom.gba", 0x006f5596, 0x00000002
	.section .rom.00705c46, "a"
	.incbin "baserom.gba", 0x00705c46, 0x00000002
	.section .rom.0070d6e6, "a"
	.incbin "baserom.gba", 0x0070d6e6, 0x00000002
	.section .rom.00710f26, "a"
	.incbin "baserom.gba", 0x00710f26, 0x00000002
	.section .rom.0071a35a, "a"
	.incbin "baserom.gba", 0x0071a35a, 0x00000002
	.section .rom.00721506, "a"
	.incbin "baserom.gba", 0x00721506, 0x00000002
	.section .rom.007293c2, "a"
	.incbin "baserom.gba", 0x007293c2, 0x00000002
	.section .rom.0072de52, "a"
	.incbin "baserom.gba", 0x0072de52, 0x00000002
	.section .rom.00732106, "a"
	.incbin "baserom.gba", 0x00732106, 0x00000002
	.section .rom.00735a86, "a"
	.incbin "baserom.gba", 0x00735a86, 0x00000002
	.section .rom.007422c2, "a"
	.incbin "baserom.gba", 0x007422c2, 0x00000002
	.section .rom.0074da02, "a"
	.incbin "baserom.gba", 0x0074da02, 0x00000002
	.section .rom.00750dd2, "a"
	.incbin "baserom.gba", 0x00750dd2, 0x00000002
	.section .rom.00762cee, "a"
	.incbin "baserom.gba", 0x00762cee, 0x00000002
	.section .rom.00766756, "a"
	.incbin "baserom.gba", 0x00766756, 0x00000002
	.section .rom.0076a146, "a"
	.incbin "baserom.gba", 0x0076a146, 0x00000002
	.section .rom.0077a6ae, "a"
	.incbin "baserom.gba", 0x0077a6ae, 0x00000002
	.section .rom.0077e5de, "a"
	.incbin "baserom.gba", 0x0077e5de, 0x00000002
	.section .rom.00782002, "a"
	.incbin "baserom.gba", 0x00782002, 0x00000002
	.section .rom.0078a322, "a"
	.incbin "baserom.gba", 0x0078a322, 0x00000002
	.section .rom.0079248e, "a"
	.incbin "baserom.gba", 0x0079248e, 0x00000002
	.section .rom.0079f75a, "a"
	.incbin "baserom.gba", 0x0079f75a, 0x00000002
	.section .rom.007a3616, "a"
	.incbin "baserom.gba", 0x007a3616, 0x00000002
	.section .rom.007a7412, "a"
	.incbin "baserom.gba", 0x007a7412, 0x00000002
	.section .rom.007ab8e2, "a"
	.incbin "baserom.gba", 0x007ab8e2, 0x00000002
	.section .rom.007b32be, "a"
	.incbin "baserom.gba", 0x007b32be, 0x00000002
	.section .rom.007b78ea, "a"
	.incbin "baserom.gba", 0x007b78ea, 0x00000002
	.section .rom.007bf566, "a"
	.incbin "baserom.gba", 0x007bf566, 0x00000002
	.section .rom.007c3162, "a"
	.incbin "baserom.gba", 0x007c3162, 0x00000002
	.section .rom.007c6b12, "a"
	.incbin "baserom.gba", 0x007c6b12, 0x00000002
	.section .rom.007caf62, "a"
	.incbin "baserom.gba", 0x007caf62, 0x00000002
	.section .rom.007ceb5a, "a"
	.incbin "baserom.gba", 0x007ceb5a, 0x00000002
	.section .rom.007da7ba, "a"
	.incbin "baserom.gba", 0x007da7ba, 0x00000002
	.section .rom.007de40a, "a"
	.incbin "baserom.gba", 0x007de40a, 0x00000002
	.section .rom.007e698a, "a"
	.incbin "baserom.gba", 0x007e698a, 0x00000002
	.section .rom.007f2fda, "a"
	.incbin "baserom.gba", 0x007f2fda, 0x00000002
	.section .rom.007f9a8e, "a"
	.incbin "baserom.gba", 0x007f9a8e, 0x00000002
	.section .rom.00802d56, "a"
	.incbin "baserom.gba", 0x00802d56, 0x00000002
	.section .rom.0080a77a, "a"
	.incbin "baserom.gba", 0x0080a77a, 0x00000002
	.section .rom.008203fa, "a"
	.incbin "baserom.gba", 0x008203fa, 0x00000002
	.section .rom.0082983e, "a"
	.incbin "baserom.gba", 0x0082983e, 0x00000002
	.section .rom.00832a5a, "a"
	.incbin "baserom.gba", 0x00832a5a, 0x00000002
	.section .rom.00840382, "a"
	.incbin "baserom.gba", 0x00840382, 0x00000002
	.section .rom.008461d6, "a"
	.incbin "baserom.gba", 0x008461d6, 0x00000002
	.section .rom.0084beaa, "a"
	.incbin "baserom.gba", 0x0084beaa, 0x00000002
	.section .rom.0084c62d, "a"
	.incbin "baserom.gba", 0x0084c62d, 0x00000003
	.section .rom.0084dfcf, "a"
	.incbin "baserom.gba", 0x0084dfcf, 0x00000001
	.section .rom.00850ded, "a"
	.incbin "baserom.gba", 0x00850ded, 0x00000003
	.section .rom.00854d02, "a"
	.incbin "baserom.gba", 0x00854d02, 0x00000002
	.section .rom.00857c10, "a"
	.incbin "baserom.gba", 0x00857c10, 0x000009bc
	.section .rom.00858a21, "a"
	.incbin "baserom.gba", 0x00858a21, 0x00000003
	.section .rom.008590e1, "a"
	.incbin "baserom.gba", 0x008590e1, 0x00000003
	.section .rom.0085931f, "a"
	.incbin "baserom.gba", 0x0085931f, 0x00000001
	.section .rom.0085974f, "a"
	.incbin "baserom.gba", 0x0085974f, 0x00000001
	.section .rom.00859a55, "a"
	.incbin "baserom.gba", 0x00859a55, 0x00000003
	.section .rom.00859dc5, "a"
	.incbin "baserom.gba", 0x00859dc5, 0x00000003
	.section .rom.0085a983, "a"
	.incbin "baserom.gba", 0x0085a983, 0x00000001
	.section .rom.0085aa61, "a"
	.incbin "baserom.gba", 0x0085aa61, 0x00000003
	.section .rom.0085af09, "a"
	.incbin "baserom.gba", 0x0085af09, 0x00000003
	.section .rom.0085b233, "a"
	.incbin "baserom.gba", 0x0085b233, 0x00000001
	.section .rom.0085b615, "a"
	.incbin "baserom.gba", 0x0085b615, 0x00000003
	.section .rom.0085ba1a, "a"
	.incbin "baserom.gba", 0x0085ba1a, 0x00000002
	.section .rom.0085bc8e, "a"
	.incbin "baserom.gba", 0x0085bc8e, 0x00000002
	.section .rom.0085e32f, "a"
	.incbin "baserom.gba", 0x0085e32f, 0x00000001
	.section .rom.0085eaaa, "a"
	.incbin "baserom.gba", 0x0085eaaa, 0x00000002
	.section .rom.008600ef, "a"
	.incbin "baserom.gba", 0x008600ef, 0x00000001
	.section .rom.00860619, "a"
	.incbin "baserom.gba", 0x00860619, 0x00000003
	.section .rom.00860e22, "a"
	.incbin "baserom.gba", 0x00860e22, 0x00000002
	.section .rom.0086139b, "a"
	.incbin "baserom.gba", 0x0086139b, 0x00000001
	.section .rom.00862ada, "a"
	.incbin "baserom.gba", 0x00862ada, 0x00000002
	.section .rom.008659a3, "a"
	.incbin "baserom.gba", 0x008659a3, 0x00000001
	.section .rom.00865c55, "a"
	.incbin "baserom.gba", 0x00865c55, 0x00000003
	.section .rom.00867037, "a"
	.incbin "baserom.gba", 0x00867037, 0x00000001
	.section .rom.0086b873, "a"
	.incbin "baserom.gba", 0x0086b873, 0x00000001
	.section .rom.0086f019, "a"
	.incbin "baserom.gba", 0x0086f019, 0x00000003
	.section .rom.00870d3e, "a"
	.incbin "baserom.gba", 0x00870d3e, 0x00000002
	.section .rom.00872c3d, "a"
	.incbin "baserom.gba", 0x00872c3d, 0x00000003
	.section .rom.008745eb, "a"
	.incbin "baserom.gba", 0x008745eb, 0x00000001
	.section .rom.00875b63, "a"
	.incbin "baserom.gba", 0x00875b63, 0x00000001
	.section .rom.00879796, "a"
	.incbin "baserom.gba", 0x00879796, 0x00000002
	.section .rom.0087a2f4, "a"
	.incbin "baserom.gba", 0x0087a2f4, 0x00003b38
	.section .rom.0087e463, "a"
	.incbin "baserom.gba", 0x0087e463, 0x00000001
	.section .rom.008805cd, "a"
	.incbin "baserom.gba", 0x008805cd, 0x00000003
	.section .rom.008806d8, "a"
	.incbin "baserom.gba", 0x008806d8, 0x000003d0
	.section .rom.00881361, "a"
	.incbin "baserom.gba", 0x00881361, 0x00000003
	.section .rom.00882f45, "a"
	.incbin "baserom.gba", 0x00882f45, 0x00000003
	.section .rom.00884aa2, "a"
	.incbin "baserom.gba", 0x00884aa2, 0x00000002
	.section .rom.00884ee1, "a"
	.incbin "baserom.gba", 0x00884ee1, 0x00000003
	.section .rom.008852f6, "a"
	.incbin "baserom.gba", 0x008852f6, 0x00001842
	.section .rom.00889043, "a"
	.incbin "baserom.gba", 0x00889043, 0x00000001
	.section .rom.008899ff, "a"
	.incbin "baserom.gba", 0x008899ff, 0x00000001
	.section .rom.0088ab42, "a"
	.incbin "baserom.gba", 0x0088ab42, 0x00001496
	.section .rom.0088d553, "a"
	.incbin "baserom.gba", 0x0088d553, 0x00000001
	.section .rom.0088d87d, "a"
	.incbin "baserom.gba", 0x0088d87d, 0x0000104b
	.section .rom.0088f051, "a"
	.incbin "baserom.gba", 0x0088f051, 0x00000623
	.section .rom.0088fc9d, "a"
	.incbin "baserom.gba", 0x0088fc9d, 0x00000003
	.section .rom.008900a7, "a"
	.incbin "baserom.gba", 0x008900a7, 0x000007d9
	.section .rom.00890b29, "a"
	.incbin "baserom.gba", 0x00890b29, 0x0000095f
	.section .rom.00891b45, "a"
	.incbin "baserom.gba", 0x00891b45, 0x00003c7f
	.section .rom.00895e23, "a"
	.incbin "baserom.gba", 0x00895e23, 0x00000001
	.section .rom.00896e5d, "a"
	.incbin "baserom.gba", 0x00896e5d, 0x00000003
	.section .rom.008974b3, "a"
	.incbin "baserom.gba", 0x008974b3, 0x00000001
	.section .rom.00897b31, "a"
	.incbin "baserom.gba", 0x00897b31, 0x00000003
	.section .rom.00898151, "a"
	.incbin "baserom.gba", 0x00898151, 0x00000003
	.section .rom.0089951e, "a"
	.incbin "baserom.gba", 0x0089951e, 0x00000002
	.section .rom.0089a562, "a"
	.incbin "baserom.gba", 0x0089a562, 0x00000002
	.section .rom.0089afeb, "a"
	.incbin "baserom.gba", 0x0089afeb, 0x000002cd
	.section .rom.0089bff1, "a"
	.incbin "baserom.gba", 0x0089bff1, 0x00000003
	.section .rom.0089d22d, "a"
	.incbin "baserom.gba", 0x0089d22d, 0x00000003
	.section .rom.0089db77, "a"
	.incbin "baserom.gba", 0x0089db77, 0x00006c09
	.section .rom.008a4cb4, "a"
	.incbin "baserom.gba", 0x008a4cb4, 0x000011d8
	.section .rom.008a637a, "a"
	.incbin "baserom.gba", 0x008a637a, 0x000027fe
	.section .rom.008a91bb, "a"
	.incbin "baserom.gba", 0x008a91bb, 0x000000a9
	.section .rom.008a957a, "a"
	.incbin "baserom.gba", 0x008a957a, 0x000018d2
	.section .rom.008ababd, "a"
	.incbin "baserom.gba", 0x008ababd, 0x00000003
	.section .rom.008ac305, "a"
	.incbin "baserom.gba", 0x008ac305, 0x00001203
	.section .rom.008ae3c7, "a"
	.incbin "baserom.gba", 0x008ae3c7, 0x00000001
	.section .rom.008aeea3, "a"
	.incbin "baserom.gba", 0x008aeea3, 0x00000001
	.section .rom.008af566, "a"
	.incbin "baserom.gba", 0x008af566, 0x00000002
	.section .rom.008af84d, "a"
	.incbin "baserom.gba", 0x008af84d, 0x00000003
	.section .rom.008b0bb3, "a"
	.incbin "baserom.gba", 0x008b0bb3, 0x00000001
	.section .rom.008b0f9d, "a"
	.incbin "baserom.gba", 0x008b0f9d, 0x00000003
	.section .rom.008b136f, "a"
	.incbin "baserom.gba", 0x008b136f, 0x00000001
	.section .rom.008b1c0f, "a"
	.incbin "baserom.gba", 0x008b1c0f, 0x00000001
	.section .rom.008b20b2, "a"
	.incbin "baserom.gba", 0x008b20b2, 0x00000002
	.section .rom.008b326b, "a"
	.incbin "baserom.gba", 0x008b326b, 0x00000001
	.section .rom.008b36c4, "a"
	.incbin "baserom.gba", 0x008b36c4, 0x00002034
	.section .rom.008b5952, "a"
	.incbin "baserom.gba", 0x008b5952, 0x00000002
	.section .rom.008b73ad, "a"
	.incbin "baserom.gba", 0x008b73ad, 0x000001b7
	.section .rom.008b78f9, "a"
	.incbin "baserom.gba", 0x008b78f9, 0x00000003
	.section .rom.008b9682, "a"
	.incbin "baserom.gba", 0x008b9682, 0x0000027a
	.section .rom.008b9dc2, "a"
	.incbin "baserom.gba", 0x008b9dc2, 0x00000002
	.section .rom.008bb997, "a"
	.incbin "baserom.gba", 0x008bb997, 0x00000001
	.section .rom.008bd595, "a"
	.incbin "baserom.gba", 0x008bd595, 0x00000003
	.section .rom.008bd7b6, "a"
	.incbin "baserom.gba", 0x008bd7b6, 0x0000043e
	.section .rom.008bdd05, "a"
	.incbin "baserom.gba", 0x008bdd05, 0x00000003
	.section .rom.008be2e7, "a"
	.incbin "baserom.gba", 0x008be2e7, 0x000004a1
	.section .rom.008bf49a, "a"
	.incbin "baserom.gba", 0x008bf49a, 0x00000002
	.section .rom.008bf9dc, "a"
	.incbin "baserom.gba", 0x008bf9dc, 0x00000840
	.section .rom.008c05ab, "a"
	.incbin "baserom.gba", 0x008c05ab, 0x00001259
	.section .rom.008c22f2, "a"
	.incbin "baserom.gba", 0x008c22f2, 0x00000002
	.section .rom.008c30c9, "a"
	.incbin "baserom.gba", 0x008c30c9, 0x00000003
	.section .rom.008c38f6, "a"
	.incbin "baserom.gba", 0x008c38f6, 0x00000002
	.section .rom.008c3ce8, "a"
	.incbin "baserom.gba", 0x008c3ce8, 0x000010d0
	.section .rom.008c5729, "a"
	.incbin "baserom.gba", 0x008c5729, 0x00000003
	.section .rom.008c5ad7, "a"
	.incbin "baserom.gba", 0x008c5ad7, 0x00000001
	.section .rom.008c7a7b, "a"
	.incbin "baserom.gba", 0x008c7a7b, 0x00000001
	.section .rom.008c8817, "a"
	.incbin "baserom.gba", 0x008c8817, 0x00000001
	.section .rom.008c8a33, "a"
	.incbin "baserom.gba", 0x008c8a33, 0x00000001
	.section .rom.008c8d2f, "a"
	.incbin "baserom.gba", 0x008c8d2f, 0x00000001
	.section .rom.008c90d0, "a"
	.incbin "baserom.gba", 0x008c90d0, 0x000016b0
	.section .rom.008caedd, "a"
	.incbin "baserom.gba", 0x008caedd, 0x00000003
	.section .rom.008cbcab, "a"
	.incbin "baserom.gba", 0x008cbcab, 0x00000c75
	.section .rom.008ccf15, "a"
	.incbin "baserom.gba", 0x008ccf15, 0x00000003
	.section .rom.008cd46f, "a"
	.incbin "baserom.gba", 0x008cd46f, 0x00000001
	.section .rom.008ced59, "a"
	.incbin "baserom.gba", 0x008ced59, 0x000008a3
	.section .rom.008cf9d7, "a"
	.incbin "baserom.gba", 0x008cf9d7, 0x00000001
	.section .rom.008cfc61, "a"
	.incbin "baserom.gba", 0x008cfc61, 0x00000003
	.section .rom.008d0007, "a"
	.incbin "baserom.gba", 0x008d0007, 0x00000001
	.section .rom.008d0262, "a"
	.incbin "baserom.gba", 0x008d0262, 0x00000002
	.section .rom.008d061a, "a"
	.incbin "baserom.gba", 0x008d061a, 0x00000002
	.section .rom.008d1b03, "a"
	.incbin "baserom.gba", 0x008d1b03, 0x00000001
	.section .rom.008d30c6, "a"
	.incbin "baserom.gba", 0x008d30c6, 0x00000a6e
	.section .rom.008d490a, "a"
	.incbin "baserom.gba", 0x008d490a, 0x00000002
	.section .rom.008d4c8d, "a"
	.incbin "baserom.gba", 0x008d4c8d, 0x00000003
	.section .rom.008d593d, "a"
	.incbin "baserom.gba", 0x008d593d, 0x00000003
	.section .rom.008d6485, "a"
	.incbin "baserom.gba", 0x008d6485, 0x00000a27
	.section .rom.008d7a7a, "a"
	.incbin "baserom.gba", 0x008d7a7a, 0x00000002
	.section .rom.008d7f92, "a"
	.incbin "baserom.gba", 0x008d7f92, 0x00000626
	.section .rom.008d89d9, "a"
	.incbin "baserom.gba", 0x008d89d9, 0x00000003
	.section .rom.008d8c73, "a"
	.incbin "baserom.gba", 0x008d8c73, 0x00000001
	.section .rom.008da174, "a"
	.incbin "baserom.gba", 0x008da174, 0x00001e28
	.section .rom.008dc386, "a"
	.incbin "baserom.gba", 0x008dc386, 0x00000002
	.section .rom.008de2fb, "a"
	.incbin "baserom.gba", 0x008de2fb, 0x00000001
	.section .rom.008de7cb, "a"
	.incbin "baserom.gba", 0x008de7cb, 0x00001cc5
	.section .rom.008e0c65, "a"
	.incbin "baserom.gba", 0x008e0c65, 0x00000003
	.section .rom.008e1736, "a"
	.incbin "baserom.gba", 0x008e1736, 0x00000002
	.section .rom.008e2273, "a"
	.incbin "baserom.gba", 0x008e2273, 0x00001c2d
	.section .rom.008e420b, "a"
	.incbin "baserom.gba", 0x008e420b, 0x00000001
	.section .rom.008e47a9, "a"
	.incbin "baserom.gba", 0x008e47a9, 0x00000207
	.section .rom.008e4ce9, "a"
	.incbin "baserom.gba", 0x008e4ce9, 0x00002687
	.section .rom.008e930e, "a"
	.incbin "baserom.gba", 0x008e930e, 0x00000002
	.section .rom.008e9ab1, "a"
	.incbin "baserom.gba", 0x008e9ab1, 0x00000003
	.section .rom.008eaac8, "a"
	.incbin "baserom.gba", 0x008eaac8, 0x00001878
	.section .rom.008ec3c4, "a"
	.incbin "baserom.gba", 0x008ec3c4, 0x000004e8
	.section .rom.008ec9b4, "a"
	.incbin "baserom.gba", 0x008ec9b4, 0x000006b8
	.section .rom.008ed97e, "a"
	.incbin "baserom.gba", 0x008ed97e, 0x00002b86
	.section .rom.008f15a9, "a"
	.incbin "baserom.gba", 0x008f15a9, 0x00000003
	.section .rom.008f17a8, "a"
	.incbin "baserom.gba", 0x008f17a8, 0x0004ac54
	.section .rom.0093c5e9, "a"
	.incbin "baserom.gba", 0x0093c5e9, 0x0000060f
	.section .rom.0093e362, "a"
	.incbin "baserom.gba", 0x0093e362, 0x00000002
	.section .rom.0093f899, "a"
	.incbin "baserom.gba", 0x0093f899, 0x00000003
	.section .rom.009416ef, "a"
	.incbin "baserom.gba", 0x009416ef, 0x00000001
	.section .rom.00943803, "a"
	.incbin "baserom.gba", 0x00943803, 0x00000001
	.section .rom.009439dc, "a"
	.incbin "baserom.gba", 0x009439dc, 0x000005a4
	.section .rom.009465fb, "a"
	.incbin "baserom.gba", 0x009465fb, 0x00000001
	.section .rom.00947003, "a"
	.incbin "baserom.gba", 0x00947003, 0x00000001
	.section .rom.00949d22, "a"
	.incbin "baserom.gba", 0x00949d22, 0x00000002
	.section .rom.00949ef4, "a"
	.incbin "baserom.gba", 0x00949ef4, 0x00000468
	.section .rom.0094b94e, "a"
	.incbin "baserom.gba", 0x0094b94e, 0x00000002
	.section .rom.0094cdbe, "a"
	.incbin "baserom.gba", 0x0094cdbe, 0x00000002
	.section .rom.0094d6b2, "a"
	.incbin "baserom.gba", 0x0094d6b2, 0x00000002
	.section .rom.0094dfd9, "a"
	.incbin "baserom.gba", 0x0094dfd9, 0x00000003
	.section .rom.0094eeca, "a"
	.incbin "baserom.gba", 0x0094eeca, 0x00000002
	.section .rom.0094fab7, "a"
	.incbin "baserom.gba", 0x0094fab7, 0x00000001
	.section .rom.0094fc92, "a"
	.incbin "baserom.gba", 0x0094fc92, 0x000004ca
	.section .rom.009509f2, "a"
	.incbin "baserom.gba", 0x009509f2, 0x00000002
	.section .rom.00952e2d, "a"
	.incbin "baserom.gba", 0x00952e2d, 0x00000003
	.section .rom.009533ff, "a"
	.incbin "baserom.gba", 0x009533ff, 0x00000001
	.section .rom.0095389f, "a"
	.incbin "baserom.gba", 0x0095389f, 0x00000001
	.section .rom.00953bf2, "a"
	.incbin "baserom.gba", 0x00953bf2, 0x000002fa
	.section .rom.00953fc5, "a"
	.incbin "baserom.gba", 0x00953fc5, 0x000002f3
	.section .rom.0095434a, "a"
	.incbin "baserom.gba", 0x0095434a, 0x00000002
	.section .rom.00954efd, "a"
	.incbin "baserom.gba", 0x00954efd, 0x00000003
	.section .rom.00955baa, "a"
	.incbin "baserom.gba", 0x00955baa, 0x00000002
	.section .rom.009566ae, "a"
	.incbin "baserom.gba", 0x009566ae, 0x00000002
	.section .rom.009575ed, "a"
	.incbin "baserom.gba", 0x009575ed, 0x00000003
	.section .rom.00957e29, "a"
	.incbin "baserom.gba", 0x00957e29, 0x00000003
	.section .rom.009592ce, "a"
	.incbin "baserom.gba", 0x009592ce, 0x00000002
	.section .rom.00959d86, "a"
	.incbin "baserom.gba", 0x00959d86, 0x00000002
	.section .rom.0095a0b1, "a"
	.incbin "baserom.gba", 0x0095a0b1, 0x00000003
	.section .rom.0095afb6, "a"
	.incbin "baserom.gba", 0x0095afb6, 0x00000002
	.section .rom.0095b7b8, "a"
	.incbin "baserom.gba", 0x0095b7b8, 0x00000400
	.section .rom.00967f72, "a"
	.incbin "baserom.gba", 0x00967f72, 0x000009fa
	.section .rom.00968dd3, "a"
	.incbin "baserom.gba", 0x00968dd3, 0x00000001
	.section .rom.00968fdf, "a"
	.incbin "baserom.gba", 0x00968fdf, 0x00000001
	.section .rom.009691e3, "a"
	.incbin "baserom.gba", 0x009691e3, 0x00000001
	.section .rom.00969272, "a"
	.incbin "baserom.gba", 0x00969272, 0x00000002
	.section .rom.00969754, "a"
	.incbin "baserom.gba", 0x00969754, 0x00000070
	.section .rom.0096981f, "a"
	.incbin "baserom.gba", 0x0096981f, 0x00000001
	.section .rom.00969852, "a"
	.incbin "baserom.gba", 0x00969852, 0x00000002
	.section .rom.00969a1e, "a"
	.incbin "baserom.gba", 0x00969a1e, 0x00000332
	.section .rom.00969e09, "a"
	.incbin "baserom.gba", 0x00969e09, 0x00000003
	.section .rom.00969fde, "a"
	.incbin "baserom.gba", 0x00969fde, 0x000000be
	.section .rom.0096a1ee, "a"
	.incbin "baserom.gba", 0x0096a1ee, 0x00000002
	.section .rom.0096a2d1, "a"
	.incbin "baserom.gba", 0x0096a2d1, 0x00000003
	.section .rom.0096a3a1, "a"
	.incbin "baserom.gba", 0x0096a3a1, 0x00000003
	.section .rom.0096a4c7, "a"
	.incbin "baserom.gba", 0x0096a4c7, 0x00000001
	.section .rom.0096a4f6, "a"
	.incbin "baserom.gba", 0x0096a4f6, 0x00000002
	.section .rom.0096a75a, "a"
	.incbin "baserom.gba", 0x0096a75a, 0x00000002
	.section .rom.0096a7f7, "a"
	.incbin "baserom.gba", 0x0096a7f7, 0x00000001
	.section .rom.0096a85c, "a"
	.incbin "baserom.gba", 0x0096a85c, 0x00000cf4
	.section .rom.00970787, "a"
	.incbin "baserom.gba", 0x00970787, 0x00000001
	.section .rom.009736d2, "a"
	.incbin "baserom.gba", 0x009736d2, 0x00000002
	.section .rom.009772f5, "a"
	.incbin "baserom.gba", 0x009772f5, 0x00000003
	.section .rom.00979541, "a"
	.incbin "baserom.gba", 0x00979541, 0x00000003
	.section .rom.0097eadd, "a"
	.incbin "baserom.gba", 0x0097eadd, 0x00000003
	.section .rom.00982357, "a"
	.incbin "baserom.gba", 0x00982357, 0x00000001
	.section .rom.009881c1, "a"
	.incbin "baserom.gba", 0x009881c1, 0x00000003
	.section .rom.00989295, "a"
	.incbin "baserom.gba", 0x00989295, 0x00000003
	.section .rom.0098a301, "a"
	.incbin "baserom.gba", 0x0098a301, 0x00000003
	.section .rom.0098f95b, "a"
	.incbin "baserom.gba", 0x0098f95b, 0x00000001
	.section .rom.00992a9d, "a"
	.incbin "baserom.gba", 0x00992a9d, 0x00000003
	.section .rom.00994b3d, "a"
	.incbin "baserom.gba", 0x00994b3d, 0x00000003
	.section .rom.00996e5d, "a"
	.incbin "baserom.gba", 0x00996e5d, 0x00000003
	.section .rom.00999ad6, "a"
	.incbin "baserom.gba", 0x00999ad6, 0x00000002
	.section .rom.0099b9d1, "a"
	.incbin "baserom.gba", 0x0099b9d1, 0x00000003
	.section .rom.009a3f15, "a"
	.incbin "baserom.gba", 0x009a3f15, 0x00000003
	.section .rom.009acb97, "a"
	.incbin "baserom.gba", 0x009acb97, 0x00000001
	.section .rom.009af711, "a"
	.incbin "baserom.gba", 0x009af711, 0x00000003
	.section .rom.009b614e, "a"
	.incbin "baserom.gba", 0x009b614e, 0x00000002
	.section .rom.009b8c42, "a"
	.incbin "baserom.gba", 0x009b8c42, 0x00000002
	.section .rom.009ba732, "a"
	.incbin "baserom.gba", 0x009ba732, 0x00000002
	.section .rom.009bd048, "a"
	.incbin "baserom.gba", 0x009bd048, 0x00003440
	.section .rom.009c4e2e, "a"
	.incbin "baserom.gba", 0x009c4e2e, 0x00000002
	.section .rom.009c5dca, "a"
	.incbin "baserom.gba", 0x009c5dca, 0x00000002
	.section .rom.009c84f5, "a"
	.incbin "baserom.gba", 0x009c84f5, 0x00000003
	.section .rom.009c9c0a, "a"
	.incbin "baserom.gba", 0x009c9c0a, 0x00000002
	.section .rom.009d2423, "a"
	.incbin "baserom.gba", 0x009d2423, 0x00000001
	.section .rom.009d66eb, "a"
	.incbin "baserom.gba", 0x009d66eb, 0x00000001
	.section .rom.009ddf25, "a"
	.incbin "baserom.gba", 0x009ddf25, 0x00000003
	.section .rom.009df70a, "a"
	.incbin "baserom.gba", 0x009df70a, 0x00000002
	.section .rom.009e24a1, "a"
	.incbin "baserom.gba", 0x009e24a1, 0x00000003
	.section .rom.009e34eb, "a"
	.incbin "baserom.gba", 0x009e34eb, 0x00000001
	.section .rom.009eba7b, "a"
	.incbin "baserom.gba", 0x009eba7b, 0x00000001
	.section .rom.009ed875, "a"
	.incbin "baserom.gba", 0x009ed875, 0x00000003
	.section .rom.009eec3b, "a"
	.incbin "baserom.gba", 0x009eec3b, 0x00000001
	.section .rom.009f5b3d, "a"
	.incbin "baserom.gba", 0x009f5b3d, 0x00000003
	.section .rom.009f7e3e, "a"
	.incbin "baserom.gba", 0x009f7e3e, 0x00000002
	.section .rom.009fd08b, "a"
	.incbin "baserom.gba", 0x009fd08b, 0x00000001
	.section .rom.00a0248a, "a"
	.incbin "baserom.gba", 0x00a0248a, 0x00000002
	.section .rom.00a0463e, "a"
	.incbin "baserom.gba", 0x00a0463e, 0x00000002
	.section .rom.00a087a5, "a"
	.incbin "baserom.gba", 0x00a087a5, 0x00000003
	.section .rom.00a0b9ca, "a"
	.incbin "baserom.gba", 0x00a0b9ca, 0x00000002
	.section .rom.00a0d0ce, "a"
	.incbin "baserom.gba", 0x00a0d0ce, 0x00000002
	.section .rom.00a13cb6, "a"
	.incbin "baserom.gba", 0x00a13cb6, 0x00000002
	.section .rom.00a20296, "a"
	.incbin "baserom.gba", 0x00a20296, 0x00000002
	.section .rom.00a2332a, "a"
	.incbin "baserom.gba", 0x00a2332a, 0x00000002
	.section .rom.00a25b95, "a"
	.incbin "baserom.gba", 0x00a25b95, 0x00000003
	.section .rom.00a27686, "a"
	.incbin "baserom.gba", 0x00a27686, 0x00000002
	.section .rom.00a2849e, "a"
	.incbin "baserom.gba", 0x00a2849e, 0x00000002
	.section .rom.00a29189, "a"
	.incbin "baserom.gba", 0x00a29189, 0x00000003
	.section .rom.00a32646, "a"
	.incbin "baserom.gba", 0x00a32646, 0x00000002
	.section .rom.00a33351, "a"
	.incbin "baserom.gba", 0x00a33351, 0x00000003
	.section .rom.00a345b5, "a"
	.incbin "baserom.gba", 0x00a345b5, 0x00000003
	.section .rom.00a354e7, "a"
	.incbin "baserom.gba", 0x00a354e7, 0x00000001
	.section .rom.00a36157, "a"
	.incbin "baserom.gba", 0x00a36157, 0x00000001
	.section .rom.00a36d6f, "a"
	.incbin "baserom.gba", 0x00a36d6f, 0x00000001
	.section .rom.00a374e3, "a"
	.incbin "baserom.gba", 0x00a374e3, 0x00000001
	.section .rom.00a38d73, "a"
	.incbin "baserom.gba", 0x00a38d73, 0x00000001
	.section .rom.00a39741, "a"
	.incbin "baserom.gba", 0x00a39741, 0x00000003
	.section .rom.00a3a35f, "a"
	.incbin "baserom.gba", 0x00a3a35f, 0x00000001
	.section .rom.00a3ca13, "a"
	.incbin "baserom.gba", 0x00a3ca13, 0x00000001
	.section .rom.00a3f126, "a"
	.incbin "baserom.gba", 0x00a3f126, 0x00000002
	.section .rom.00a4407b, "a"
	.incbin "baserom.gba", 0x00a4407b, 0x00000001
	.section .rom.00a483a5, "a"
	.incbin "baserom.gba", 0x00a483a5, 0x00000003
	.section .rom.00a48f92, "a"
	.incbin "baserom.gba", 0x00a48f92, 0x00000002
	.section .rom.00a4db47, "a"
	.incbin "baserom.gba", 0x00a4db47, 0x00000001
	.section .rom.00a4feb1, "a"
	.incbin "baserom.gba", 0x00a4feb1, 0x00000003
	.section .rom.00a51853, "a"
	.incbin "baserom.gba", 0x00a51853, 0x00000001
	.section .rom.00a56115, "a"
	.incbin "baserom.gba", 0x00a56115, 0x00000003
	.section .rom.00a59525, "a"
	.incbin "baserom.gba", 0x00a59525, 0x00000003
	.section .rom.00a5d5ed, "a"
	.incbin "baserom.gba", 0x00a5d5ed, 0x00000003
	.section .rom.00a60cb6, "a"
	.incbin "baserom.gba", 0x00a60cb6, 0x00000002
	.section .rom.00a68c87, "a"
	.incbin "baserom.gba", 0x00a68c87, 0x00000001
	.section .rom.00a6ff97, "a"
	.incbin "baserom.gba", 0x00a6ff97, 0x00000001
	.section .rom.00a74f17, "a"
	.incbin "baserom.gba", 0x00a74f17, 0x00000001
	.section .rom.00a79999, "a"
	.incbin "baserom.gba", 0x00a79999, 0x0000051f
	.section .rom.00a7b115, "a"
	.incbin "baserom.gba", 0x00a7b115, 0x00000003
	.section .rom.00a7b2e6, "a"
	.incbin "baserom.gba", 0x00a7b2e6, 0x00000002
	.section .rom.00a7d387, "a"
	.incbin "baserom.gba", 0x00a7d387, 0x00000001
	.section .rom.00a7e314, "a"
	.incbin "baserom.gba", 0x00a7e314, 0x000022d8
	.section .rom.00a81840, "a"
	.incbin "baserom.gba", 0x00a81840, 0x0000461c
	.section .rom.00a85f62, "a"
	.incbin "baserom.gba", 0x00a85f62, 0x00000002
	.section .rom.00a8714d, "a"
	.incbin "baserom.gba", 0x00a8714d, 0x00000003
	.section .rom.00a88dd5, "a"
	.incbin "baserom.gba", 0x00a88dd5, 0x00000003
	.section .rom.00a892e3, "a"
	.incbin "baserom.gba", 0x00a892e3, 0x00000001
	.section .rom.00a89423, "a"
	.incbin "baserom.gba", 0x00a89423, 0x00000001
	.section .rom.00a8b0a6, "a"
	.incbin "baserom.gba", 0x00a8b0a6, 0x00000002
	.section .rom.00a8da7e, "a"
	.incbin "baserom.gba", 0x00a8da7e, 0x00000002
	.section .rom.00a90247, "a"
	.incbin "baserom.gba", 0x00a90247, 0x00000001
	.section .rom.00a91767, "a"
	.incbin "baserom.gba", 0x00a91767, 0x00000001
	.section .rom.00a930ab, "a"
	.incbin "baserom.gba", 0x00a930ab, 0x00000001
	.section .rom.00a94b59, "a"
	.incbin "baserom.gba", 0x00a94b59, 0x00000003
	.section .rom.00a94ccd, "a"
	.incbin "baserom.gba", 0x00a94ccd, 0x00000003
	.section .rom.00a97ab1, "a"
	.incbin "baserom.gba", 0x00a97ab1, 0x00000003
	.section .rom.00a9a35b, "a"
	.incbin "baserom.gba", 0x00a9a35b, 0x00000001
	.section .rom.00a9e7f2, "a"
	.incbin "baserom.gba", 0x00a9e7f2, 0x00000002
	.section .rom.00aa1b6a, "a"
	.incbin "baserom.gba", 0x00aa1b6a, 0x00000002
	.section .rom.00aa4667, "a"
	.incbin "baserom.gba", 0x00aa4667, 0x00000001
	.section .rom.00aa6745, "a"
	.incbin "baserom.gba", 0x00aa6745, 0x00000003
	.section .rom.00aa8991, "a"
	.incbin "baserom.gba", 0x00aa8991, 0x00000003
	.section .rom.00aa8ad3, "a"
	.incbin "baserom.gba", 0x00aa8ad3, 0x00000001
	.section .rom.00aaa1a6, "a"
	.incbin "baserom.gba", 0x00aaa1a6, 0x00000002
	.section .rom.00aaa2a6, "a"
	.incbin "baserom.gba", 0x00aaa2a6, 0x00000002
	.section .rom.00aac199, "a"
	.incbin "baserom.gba", 0x00aac199, 0x00000003
	.section .rom.00aadb71, "a"
	.incbin "baserom.gba", 0x00aadb71, 0x00000003
	.section .rom.00ab05a2, "a"
	.incbin "baserom.gba", 0x00ab05a2, 0x00000002
	.section .rom.00ab57f3, "a"
	.incbin "baserom.gba", 0x00ab57f3, 0x00000001
	.section .rom.00ab8137, "a"
	.incbin "baserom.gba", 0x00ab8137, 0x00000001
	.section .rom.00ab960a, "a"
	.incbin "baserom.gba", 0x00ab960a, 0x00000002
	.section .rom.00abe3d5, "a"
	.incbin "baserom.gba", 0x00abe3d5, 0x00000003
	.section .rom.00ac107f, "a"
	.incbin "baserom.gba", 0x00ac107f, 0x00000001
	.section .rom.00ac42b7, "a"
	.incbin "baserom.gba", 0x00ac42b7, 0x00000001
	.section .rom.00ac444f, "a"
	.incbin "baserom.gba", 0x00ac444f, 0x00000001
	.section .rom.00ac724f, "a"
	.incbin "baserom.gba", 0x00ac724f, 0x00000001
	.section .rom.00ac8a03, "a"
	.incbin "baserom.gba", 0x00ac8a03, 0x00000001
	.section .rom.00ac999e, "a"
	.incbin "baserom.gba", 0x00ac999e, 0x00000002
	.section .rom.00acbfff, "a"
	.incbin "baserom.gba", 0x00acbfff, 0x00000001
	.section .rom.00acc1a6, "a"
	.incbin "baserom.gba", 0x00acc1a6, 0x00000002
	.section .rom.00acf15f, "a"
	.incbin "baserom.gba", 0x00acf15f, 0x00000001
	.section .rom.00acfe05, "a"
	.incbin "baserom.gba", 0x00acfe05, 0x00000003
	.section .rom.00ad13d5, "a"
	.incbin "baserom.gba", 0x00ad13d5, 0x00000003
	.section .rom.00ad93a1, "a"
	.incbin "baserom.gba", 0x00ad93a1, 0x00000003
	.section .rom.00adae87, "a"
	.incbin "baserom.gba", 0x00adae87, 0x00000001
	.section .rom.00adc371, "a"
	.incbin "baserom.gba", 0x00adc371, 0x00000003
	.section .rom.00ae259b, "a"
	.incbin "baserom.gba", 0x00ae259b, 0x00000001
	.section .rom.00ae2a6d, "a"
	.incbin "baserom.gba", 0x00ae2a6d, 0x00000003
	.section .rom.00ae3bb5, "a"
	.incbin "baserom.gba", 0x00ae3bb5, 0x00000003
	.section .rom.00ae3d66, "a"
	.incbin "baserom.gba", 0x00ae3d66, 0x00000002
	.section .rom.00aefa53, "a"
	.incbin "baserom.gba", 0x00aefa53, 0x00000001
	.section .rom.00aefb73, "a"
	.incbin "baserom.gba", 0x00aefb73, 0x00000001
	.section .rom.00af1f13, "a"
	.incbin "baserom.gba", 0x00af1f13, 0x00000001
	.section .rom.00af315f, "a"
	.incbin "baserom.gba", 0x00af315f, 0x00000001
	.section .rom.00af54ef, "a"
	.incbin "baserom.gba", 0x00af54ef, 0x00000001
	.section .rom.00af6ded, "a"
	.incbin "baserom.gba", 0x00af6ded, 0x00000003
	.section .rom.00af7dde, "a"
	.incbin "baserom.gba", 0x00af7dde, 0x00000002
	.section .rom.00afa37a, "a"
	.incbin "baserom.gba", 0x00afa37a, 0x00000002
	.section .rom.00afe9a2, "a"
	.incbin "baserom.gba", 0x00afe9a2, 0x00000002
	.section .rom.00aff067, "a"
	.incbin "baserom.gba", 0x00aff067, 0x00000001
	.section .rom.00b01af5, "a"
	.incbin "baserom.gba", 0x00b01af5, 0x00000003
	.section .rom.00b01c46, "a"
	.incbin "baserom.gba", 0x00b01c46, 0x00000002
	.section .rom.00b04056, "a"
	.incbin "baserom.gba", 0x00b04056, 0x00000002
	.section .rom.00b061d7, "a"
	.incbin "baserom.gba", 0x00b061d7, 0x00000001
	.section .rom.00b06317, "a"
	.incbin "baserom.gba", 0x00b06317, 0x00000001
	.section .rom.00b08db6, "a"
	.incbin "baserom.gba", 0x00b08db6, 0x00000002
	.section .rom.00b08f07, "a"
	.incbin "baserom.gba", 0x00b08f07, 0x00000001
	.section .rom.00b0b316, "a"
	.incbin "baserom.gba", 0x00b0b316, 0x00000002
	.section .rom.00b0d497, "a"
	.incbin "baserom.gba", 0x00b0d497, 0x00000001
	.section .rom.00b0d5d7, "a"
	.incbin "baserom.gba", 0x00b0d5d7, 0x00000001
	.section .rom.00b10c06, "a"
	.incbin "baserom.gba", 0x00b10c06, 0x00000002
	.section .rom.00b1316a, "a"
	.incbin "baserom.gba", 0x00b1316a, 0x00000002
	.section .rom.00b152eb, "a"
	.incbin "baserom.gba", 0x00b152eb, 0x00000001
	.section .rom.00b1542b, "a"
	.incbin "baserom.gba", 0x00b1542b, 0x00000001
	.section .rom.00b168db, "a"
	.incbin "baserom.gba", 0x00b168db, 0x00000001
	.section .rom.00b169e6, "a"
	.incbin "baserom.gba", 0x00b169e6, 0x00000002
	.section .rom.00b1853e, "a"
	.incbin "baserom.gba", 0x00b1853e, 0x00000002
	.section .rom.00b19be1, "a"
	.incbin "baserom.gba", 0x00b19be1, 0x00000003
	.section .rom.00b19da2, "a"
	.incbin "baserom.gba", 0x00b19da2, 0x00000d9e
	.section .rom.00b1c796, "a"
	.incbin "baserom.gba", 0x00b1c796, 0x00000002
	.section .rom.00b1dfed, "a"
	.incbin "baserom.gba", 0x00b1dfed, 0x00000003
	.section .rom.00b1e1ae, "a"
	.incbin "baserom.gba", 0x00b1e1ae, 0x00000002
	.section .rom.00b22429, "a"
	.incbin "baserom.gba", 0x00b22429, 0x00000003
	.section .rom.00b225ea, "a"
	.incbin "baserom.gba", 0x00b225ea, 0x00000002
	.section .rom.00b23061, "a"
	.incbin "baserom.gba", 0x00b23061, 0x00000003
	.section .rom.00b23169, "a"
	.incbin "baserom.gba", 0x00b23169, 0x00000003
	.section .rom.00b24e2a, "a"
	.incbin "baserom.gba", 0x00b24e2a, 0x00000002
	.section .rom.00b265b7, "a"
	.incbin "baserom.gba", 0x00b265b7, 0x00000001
	.section .rom.00b2686d, "a"
	.incbin "baserom.gba", 0x00b2686d, 0x00000003
	.section .rom.00b2837d, "a"
	.incbin "baserom.gba", 0x00b2837d, 0x00000003
	.section .rom.00b2ac0a, "a"
	.incbin "baserom.gba", 0x00b2ac0a, 0x00000002
	.section .rom.00b2d3fa, "a"
	.incbin "baserom.gba", 0x00b2d3fa, 0x00000002
	.section .rom.00b2e39a, "a"
	.incbin "baserom.gba", 0x00b2e39a, 0x00000002
	.section .rom.00b304ed, "a"
	.incbin "baserom.gba", 0x00b304ed, 0x00000003
	.section .rom.00b33b7a, "a"
	.incbin "baserom.gba", 0x00b33b7a, 0x00000002
	.section .rom.00b3590f, "a"
	.incbin "baserom.gba", 0x00b3590f, 0x00000001
	.section .rom.00b3772d, "a"
	.incbin "baserom.gba", 0x00b3772d, 0x00000003
	.section .rom.00b39209, "a"
	.incbin "baserom.gba", 0x00b39209, 0x00000003
	.section .rom.00b3a7f2, "a"
	.incbin "baserom.gba", 0x00b3a7f2, 0x00000002
	.section .rom.00b3cdda, "a"
	.incbin "baserom.gba", 0x00b3cdda, 0x00000002
	.section .rom.00b3cf53, "a"
	.incbin "baserom.gba", 0x00b3cf53, 0x00000001
	.section .rom.00b3e412, "a"
	.incbin "baserom.gba", 0x00b3e412, 0x00000002
	.section .rom.00b3fc16, "a"
	.incbin "baserom.gba", 0x00b3fc16, 0x00000002
	.section .rom.00b4158e, "a"
	.incbin "baserom.gba", 0x00b4158e, 0x00000002
	.section .rom.00b424ae, "a"
	.incbin "baserom.gba", 0x00b424ae, 0x00000002
	.section .rom.00b43f5a, "a"
	.incbin "baserom.gba", 0x00b43f5a, 0x00000002
	.section .rom.00b44067, "a"
	.incbin "baserom.gba", 0x00b44067, 0x00000001
	.section .rom.00b46197, "a"
	.incbin "baserom.gba", 0x00b46197, 0x00000001
	.section .rom.00b47d6d, "a"
	.incbin "baserom.gba", 0x00b47d6d, 0x00000003
	.section .rom.00b4837f, "a"
	.incbin "baserom.gba", 0x00b4837f, 0x00000001
	.section .rom.00b484bf, "a"
	.incbin "baserom.gba", 0x00b484bf, 0x00000001
	.section .rom.00b4abbf, "a"
	.incbin "baserom.gba", 0x00b4abbf, 0x00000001
	.section .rom.00b4c41e, "a"
	.incbin "baserom.gba", 0x00b4c41e, 0x00000002
	.section .rom.00b4ca2f, "a"
	.incbin "baserom.gba", 0x00b4ca2f, 0x00000001
	.section .rom.00b4cb6f, "a"
	.incbin "baserom.gba", 0x00b4cb6f, 0x00000001
	.section .rom.00b4d9ea, "a"
	.incbin "baserom.gba", 0x00b4d9ea, 0x00000002
	.section .rom.00b4db01, "a"
	.incbin "baserom.gba", 0x00b4db01, 0x00000003
	.section .rom.00b4fdd7, "a"
	.incbin "baserom.gba", 0x00b4fdd7, 0x00000001
	.section .rom.00b51a2a, "a"
	.incbin "baserom.gba", 0x00b51a2a, 0x00000002
	.section .rom.00b5206f, "a"
	.incbin "baserom.gba", 0x00b5206f, 0x00000001
	.section .rom.00b521af, "a"
	.incbin "baserom.gba", 0x00b521af, 0x00000001
	.section .rom.00b52db5, "a"
	.incbin "baserom.gba", 0x00b52db5, 0x00000003
	.section .rom.00b55367, "a"
	.incbin "baserom.gba", 0x00b55367, 0x00000001
	.section .rom.00b57026, "a"
	.incbin "baserom.gba", 0x00b57026, 0x00000002
	.section .rom.00b58af2, "a"
	.incbin "baserom.gba", 0x00b58af2, 0x00000002
	.section .rom.00b59bc6, "a"
	.incbin "baserom.gba", 0x00b59bc6, 0x00000002
	.section .rom.00b59d2d, "a"
	.incbin "baserom.gba", 0x00b59d2d, 0x00000003
	.section .rom.00b5af46, "a"
	.incbin "baserom.gba", 0x00b5af46, 0x00000002
	.section .rom.00b5bb25, "a"
	.incbin "baserom.gba", 0x00b5bb25, 0x00000003
	.section .rom.00b5bc92, "a"
	.incbin "baserom.gba", 0x00b5bc92, 0x00000002
	.section .rom.00b5eae3, "a"
	.incbin "baserom.gba", 0x00b5eae3, 0x00000001
	.section .rom.00b612a2, "a"
	.incbin "baserom.gba", 0x00b612a2, 0x00000002
	.section .rom.00b67353, "a"
	.incbin "baserom.gba", 0x00b67353, 0x00000001
	.section .rom.00b68e83, "a"
	.incbin "baserom.gba", 0x00b68e83, 0x00000001
	.section .rom.00b6b1ba, "a"
	.incbin "baserom.gba", 0x00b6b1ba, 0x00000002
	.section .rom.00b6c36b, "a"
	.incbin "baserom.gba", 0x00b6c36b, 0x00000001
	.section .rom.00b6dc8d, "a"
	.incbin "baserom.gba", 0x00b6dc8d, 0x00000003
	.section .rom.00b6fb19, "a"
	.incbin "baserom.gba", 0x00b6fb19, 0x00000003
	.section .rom.00b71cfa, "a"
	.incbin "baserom.gba", 0x00b71cfa, 0x00000002
	.section .rom.00b756c1, "a"
	.incbin "baserom.gba", 0x00b756c1, 0x00000003
	.section .rom.00b757ad, "a"
	.incbin "baserom.gba", 0x00b757ad, 0x00000003
	.section .rom.00b789e6, "a"
	.incbin "baserom.gba", 0x00b789e6, 0x00000002
	.section .rom.00b7905d, "a"
	.incbin "baserom.gba", 0x00b7905d, 0x00000003
	.section .rom.00b7ce1f, "a"
	.incbin "baserom.gba", 0x00b7ce1f, 0x00000001
	.section .rom.00b7f0b1, "a"
	.incbin "baserom.gba", 0x00b7f0b1, 0x00000003
	.section .rom.00b80ace, "a"
	.incbin "baserom.gba", 0x00b80ace, 0x00000002
	.section .rom.00b81bdb, "a"
	.incbin "baserom.gba", 0x00b81bdb, 0x00000001
	.section .rom.00b84d7a, "a"
	.incbin "baserom.gba", 0x00b84d7a, 0x00000002
	.section .rom.00b85f72, "a"
	.incbin "baserom.gba", 0x00b85f72, 0x00000002
	.section .rom.00b86e35, "a"
	.incbin "baserom.gba", 0x00b86e35, 0x00000003
	.section .rom.00b88ed6, "a"
	.incbin "baserom.gba", 0x00b88ed6, 0x00000002
	.section .rom.00b8a92b, "a"
	.incbin "baserom.gba", 0x00b8a92b, 0x00000001
	.section .rom.00b8bdf6, "a"
	.incbin "baserom.gba", 0x00b8bdf6, 0x00000002
	.section .rom.00b8dffa, "a"
	.incbin "baserom.gba", 0x00b8dffa, 0x00000002
	.section .rom.00b8e0ef, "a"
	.incbin "baserom.gba", 0x00b8e0ef, 0x00000001
	.section .rom.00b8f6c2, "a"
	.incbin "baserom.gba", 0x00b8f6c2, 0x00000002
	.section .rom.00b8f8e1, "a"
	.incbin "baserom.gba", 0x00b8f8e1, 0x00000003
	.section .rom.00b919c5, "a"
	.incbin "baserom.gba", 0x00b919c5, 0x00000003
	.section .rom.00b91b8a, "a"
	.incbin "baserom.gba", 0x00b91b8a, 0x00000002
	.section .rom.00b93c4d, "a"
	.incbin "baserom.gba", 0x00b93c4d, 0x00000003
	.section .rom.00b93d7d, "a"
	.incbin "baserom.gba", 0x00b93d7d, 0x00000003
	.section .rom.00b96836, "a"
	.incbin "baserom.gba", 0x00b96836, 0x00000002
	.section .rom.00b9838e, "a"
	.incbin "baserom.gba", 0x00b9838e, 0x00000002
	.section .rom.00b992d5, "a"
	.incbin "baserom.gba", 0x00b992d5, 0x00000003
	.section .rom.00b9acde, "a"
	.incbin "baserom.gba", 0x00b9acde, 0x00000002
	.section .rom.00baaa33, "a"
	.incbin "baserom.gba", 0x00baaa33, 0x00000001
	.section .rom.00baab83, "a"
	.incbin "baserom.gba", 0x00baab83, 0x00000001
	.section .rom.00bad68e, "a"
	.incbin "baserom.gba", 0x00bad68e, 0x00000002
	.section .rom.00baf297, "a"
	.incbin "baserom.gba", 0x00baf297, 0x00000001
	.section .rom.00bb0c9e, "a"
	.incbin "baserom.gba", 0x00bb0c9e, 0x00000002
	.section .rom.00bb29b9, "a"
	.incbin "baserom.gba", 0x00bb29b9, 0x00000003
	.section .rom.00bb2b0b, "a"
	.incbin "baserom.gba", 0x00bb2b0b, 0x00000001
	.section .rom.00bb4512, "a"
	.incbin "baserom.gba", 0x00bb4512, 0x00000002
	.section .rom.00bb829a, "a"
	.incbin "baserom.gba", 0x00bb829a, 0x00000002
	.section .rom.00bb842d, "a"
	.incbin "baserom.gba", 0x00bb842d, 0x00000003
	.section .rom.00bbd437, "a"
	.incbin "baserom.gba", 0x00bbd437, 0x00000001
	.section .rom.00bbfab9, "a"
	.incbin "baserom.gba", 0x00bbfab9, 0x00000003
	.section .rom.00bbfdef, "a"
	.incbin "baserom.gba", 0x00bbfdef, 0x00000001
	.section .rom.00bc6245, "a"
	.incbin "baserom.gba", 0x00bc6245, 0x00000003
	.section .rom.00bc6392, "a"
	.incbin "baserom.gba", 0x00bc6392, 0x00000002
	.section .rom.00bc8217, "a"
	.incbin "baserom.gba", 0x00bc8217, 0x00000001
	.section .rom.00bc97be, "a"
	.incbin "baserom.gba", 0x00bc97be, 0x00000002
	.section .rom.00bcb769, "a"
	.incbin "baserom.gba", 0x00bcb769, 0x00000003
	.section .rom.00bcc6da, "a"
	.incbin "baserom.gba", 0x00bcc6da, 0x00000002
	.section .rom.00bd7ce3, "a"
	.incbin "baserom.gba", 0x00bd7ce3, 0x00000001
	.section .rom.00bd7d9f, "a"
	.incbin "baserom.gba", 0x00bd7d9f, 0x00000001
	.section .rom.00bd8ade, "a"
	.incbin "baserom.gba", 0x00bd8ade, 0x00000002
	.section .rom.00bd94bb, "a"
	.incbin "baserom.gba", 0x00bd94bb, 0x00000001
	.section .rom.00bda0d3, "a"
	.incbin "baserom.gba", 0x00bda0d3, 0x00000001
	.section .rom.00bdbf8e, "a"
	.incbin "baserom.gba", 0x00bdbf8e, 0x00000002
	.section .rom.00bdc0aa, "a"
	.incbin "baserom.gba", 0x00bdc0aa, 0x00000002
	.section .rom.00bde92b, "a"
	.incbin "baserom.gba", 0x00bde92b, 0x00000001
	.section .rom.00be00b6, "a"
	.incbin "baserom.gba", 0x00be00b6, 0x00000002
	.section .rom.00be3fe6, "a"
	.incbin "baserom.gba", 0x00be3fe6, 0x00000002
	.section .rom.00be412f, "a"
	.incbin "baserom.gba", 0x00be412f, 0x00000001
	.section .rom.00be67df, "a"
	.incbin "baserom.gba", 0x00be67df, 0x00000001
	.section .rom.00be8f82, "a"
	.incbin "baserom.gba", 0x00be8f82, 0x00000002
	.section .rom.00be9d0d, "a"
	.incbin "baserom.gba", 0x00be9d0d, 0x00000003
	.section .rom.00becb4e, "a"
	.incbin "baserom.gba", 0x00becb4e, 0x00000002
	.section .rom.00bef175, "a"
	.incbin "baserom.gba", 0x00bef175, 0x00000003
	.section .rom.00bf0e61, "a"
	.incbin "baserom.gba", 0x00bf0e61, 0x00000003
	.section .rom.00bf2561, "a"
	.incbin "baserom.gba", 0x00bf2561, 0x00000003
	.section .rom.00bf3add, "a"
	.incbin "baserom.gba", 0x00bf3add, 0x00000003
	.section .rom.00bf524f, "a"
	.incbin "baserom.gba", 0x00bf524f, 0x00000001
	.section .rom.00bf7eda, "a"
	.incbin "baserom.gba", 0x00bf7eda, 0x00000002
	.section .rom.00bf8f26, "a"
	.incbin "baserom.gba", 0x00bf8f26, 0x00000002
	.section .rom.00bfa023, "a"
	.incbin "baserom.gba", 0x00bfa023, 0x00000001
	.section .rom.00bfb7f7, "a"
	.incbin "baserom.gba", 0x00bfb7f7, 0x00000001
	.section .rom.00bfccbb, "a"
	.incbin "baserom.gba", 0x00bfccbb, 0x00000001
	.section .rom.00bff1ee, "a"
	.incbin "baserom.gba", 0x00bff1ee, 0x00000002
	.section .rom.00bff322, "a"
	.incbin "baserom.gba", 0x00bff322, 0x00000002
	.section .rom.00c01a8e, "a"
	.incbin "baserom.gba", 0x00c01a8e, 0x00000002
	.section .rom.00c03861, "a"
	.incbin "baserom.gba", 0x00c03861, 0x00000003
	.section .rom.00c04ba3, "a"
	.incbin "baserom.gba", 0x00c04ba3, 0x00000001
	.section .rom.00c07115, "a"
	.incbin "baserom.gba", 0x00c07115, 0x00000003
	.section .rom.00c0987b, "a"
	.incbin "baserom.gba", 0x00c0987b, 0x00000001
	.section .rom.00c0b1e5, "a"
	.incbin "baserom.gba", 0x00c0b1e5, 0x00000003
	.section .rom.00c0da0e, "a"
	.incbin "baserom.gba", 0x00c0da0e, 0x00000002
	.section .rom.00c11ded, "a"
	.incbin "baserom.gba", 0x00c11ded, 0x00000003
	.section .rom.00c15197, "a"
	.incbin "baserom.gba", 0x00c15197, 0x00000001
	.section .rom.00c15f8a, "a"
	.incbin "baserom.gba", 0x00c15f8a, 0x00000002
	.section .rom.00c16d02, "a"
	.incbin "baserom.gba", 0x00c16d02, 0x00000002
	.section .rom.00c1ee2a, "a"
	.incbin "baserom.gba", 0x00c1ee2a, 0x00000002
	.section .rom.00c251a9, "a"
	.incbin "baserom.gba", 0x00c251a9, 0x00000003
	.section .rom.00c25baf, "a"
	.incbin "baserom.gba", 0x00c25baf, 0x00000001
	.section .rom.00c2765d, "a"
	.incbin "baserom.gba", 0x00c2765d, 0x00001493
	.section .rom.00c28f57, "a"
	.incbin "baserom.gba", 0x00c28f57, 0x00000001
	.section .rom.00c29116, "a"
	.incbin "baserom.gba", 0x00c29116, 0x00000002
	.section .rom.00c2b91e, "a"
	.incbin "baserom.gba", 0x00c2b91e, 0x00000002
	.section .rom.00c2ba76, "a"
	.incbin "baserom.gba", 0x00c2ba76, 0x00000002
	.section .rom.00c2e3bd, "a"
	.incbin "baserom.gba", 0x00c2e3bd, 0x00000003
	.section .rom.00c305e1, "a"
	.incbin "baserom.gba", 0x00c305e1, 0x00000003
	.section .rom.00c31ad6, "a"
	.incbin "baserom.gba", 0x00c31ad6, 0x00000002
	.section .rom.00c33d39, "a"
	.incbin "baserom.gba", 0x00c33d39, 0x00000003
	.section .rom.00c365e7, "a"
	.incbin "baserom.gba", 0x00c365e7, 0x00000001
	.section .rom.00c3857a, "a"
	.incbin "baserom.gba", 0x00c3857a, 0x00000002
	.section .rom.00c3959b, "a"
	.incbin "baserom.gba", 0x00c3959b, 0x00000001
	.section .rom.00c396db, "a"
	.incbin "baserom.gba", 0x00c396db, 0x00000001
	.section .rom.00c3c2fa, "a"
	.incbin "baserom.gba", 0x00c3c2fa, 0x00000002
	.section .rom.00c3e77e, "a"
	.incbin "baserom.gba", 0x00c3e77e, 0x00000002
	.section .rom.00c40865, "a"
	.incbin "baserom.gba", 0x00c40865, 0x00000003
	.section .rom.00c420c9, "a"
	.incbin "baserom.gba", 0x00c420c9, 0x00000003
	.section .rom.00c4461f, "a"
	.incbin "baserom.gba", 0x00c4461f, 0x00000001
	.section .rom.00c44785, "a"
	.incbin "baserom.gba", 0x00c44785, 0x00000003
	.section .rom.00c46f7f, "a"
	.incbin "baserom.gba", 0x00c46f7f, 0x00000001
	.section .rom.00c48fb3, "a"
	.incbin "baserom.gba", 0x00c48fb3, 0x00000001
	.section .rom.00c4abda, "a"
	.incbin "baserom.gba", 0x00c4abda, 0x00000002
	.section .rom.00c4d5fa, "a"
	.incbin "baserom.gba", 0x00c4d5fa, 0x00000002
	.section .rom.00c4e165, "a"
	.incbin "baserom.gba", 0x00c4e165, 0x00000003
	.section .rom.00c4e24b, "a"
	.incbin "baserom.gba", 0x00c4e24b, 0x00000001
	.section .rom.00c4fb95, "a"
	.incbin "baserom.gba", 0x00c4fb95, 0x00000003
	.section .rom.00c514fb, "a"
	.incbin "baserom.gba", 0x00c514fb, 0x00000001
	.section .rom.00c528c9, "a"
	.incbin "baserom.gba", 0x00c528c9, 0x00000003
	.section .rom.00c529af, "a"
	.incbin "baserom.gba", 0x00c529af, 0x00000001
	.section .rom.00c5351d, "a"
	.incbin "baserom.gba", 0x00c5351d, 0x00000003
	.section .rom.00c53603, "a"
	.incbin "baserom.gba", 0x00c53603, 0x00000001
	.section .rom.00c54367, "a"
	.incbin "baserom.gba", 0x00c54367, 0x00000001
	.section .rom.00c5444b, "a"
	.incbin "baserom.gba", 0x00c5444b, 0x00000001
	.section .rom.00c54c51, "a"
	.incbin "baserom.gba", 0x00c54c51, 0x00000003
	.section .rom.00c54d37, "a"
	.incbin "baserom.gba", 0x00c54d37, 0x00000001
	.section .rom.00c55897, "a"
	.incbin "baserom.gba", 0x00c55897, 0x00000001
	.section .rom.00c56115, "a"
	.incbin "baserom.gba", 0x00c56115, 0x00000003
	.section .rom.00c56212, "a"
	.incbin "baserom.gba", 0x00c56212, 0x00000002
	.section .rom.00c5806b, "a"
	.incbin "baserom.gba", 0x00c5806b, 0x00000001
	.section .rom.00c58e79, "a"
	.incbin "baserom.gba", 0x00c58e79, 0x00000003
	.section .rom.00c5a10d, "a"
	.incbin "baserom.gba", 0x00c5a10d, 0x00000003
	.section .rom.00c5b0e9, "a"
	.incbin "baserom.gba", 0x00c5b0e9, 0x00000003
	.section .rom.00c5d4ef, "a"
	.incbin "baserom.gba", 0x00c5d4ef, 0x00000001
	.section .rom.00c5d62f, "a"
	.incbin "baserom.gba", 0x00c5d62f, 0x00000001
	.section .rom.00c5dcfd, "a"
	.incbin "baserom.gba", 0x00c5dcfd, 0x00000003
	.section .rom.00c5ddd3, "a"
	.incbin "baserom.gba", 0x00c5ddd3, 0x00000001
	.section .rom.00c5e649, "a"
	.incbin "baserom.gba", 0x00c5e649, 0x00000003
	.section .rom.00c5eafa, "a"
	.incbin "baserom.gba", 0x00c5eafa, 0x00000002
	.section .rom.00c5ff75, "a"
	.incbin "baserom.gba", 0x00c5ff75, 0x00000003
	.section .rom.00c6078b, "a"
	.incbin "baserom.gba", 0x00c6078b, 0x00000001
	.section .rom.00c61421, "a"
	.incbin "baserom.gba", 0x00c61421, 0x00000003
	.section .rom.00c615c6, "a"
	.incbin "baserom.gba", 0x00c615c6, 0x00000002
	.section .rom.00c6448a, "a"
	.incbin "baserom.gba", 0x00c6448a, 0x00000002
	.section .rom.00c65e09, "a"
	.incbin "baserom.gba", 0x00c65e09, 0x00000003
	.section .rom.00c66e56, "a"
	.incbin "baserom.gba", 0x00c66e56, 0x00000002
	.section .rom.00c6e59b, "a"
	.incbin "baserom.gba", 0x00c6e59b, 0x00000001
	.section .rom.00c75d8f, "a"
	.incbin "baserom.gba", 0x00c75d8f, 0x00000001
	.section .rom.00c7c389, "a"
	.incbin "baserom.gba", 0x00c7c389, 0x00000003
	.section .rom.00c7edc2, "a"
	.incbin "baserom.gba", 0x00c7edc2, 0x00000002
	.section .rom.00c81693, "a"
	.incbin "baserom.gba", 0x00c81693, 0x00000001
	.section .rom.00c85edf, "a"
	.incbin "baserom.gba", 0x00c85edf, 0x00000001
	.section .rom.00c86edd, "a"
	.incbin "baserom.gba", 0x00c86edd, 0x00000003
	.section .rom.00c86ff6, "a"
	.incbin "baserom.gba", 0x00c86ff6, 0x00000002
	.section .rom.00c8855e, "a"
	.incbin "baserom.gba", 0x00c8855e, 0x00000002
	.section .rom.00c89fbb, "a"
	.incbin "baserom.gba", 0x00c89fbb, 0x00000001
	.section .rom.00c8a0f6, "a"
	.incbin "baserom.gba", 0x00c8a0f6, 0x00000002
	.section .rom.00c8b5fd, "a"
	.incbin "baserom.gba", 0x00c8b5fd, 0x00000003
	.section .rom.00c8e886, "a"
	.incbin "baserom.gba", 0x00c8e886, 0x00000002
	.section .rom.00c8fccd, "a"
	.incbin "baserom.gba", 0x00c8fccd, 0x00000003
	.section .rom.00c929c5, "a"
	.incbin "baserom.gba", 0x00c929c5, 0x00000003
	.section .rom.00c94dfb, "a"
	.incbin "baserom.gba", 0x00c94dfb, 0x00000001
	.section .rom.00c94fd7, "a"
	.incbin "baserom.gba", 0x00c94fd7, 0x00000001
	.section .rom.00c980eb, "a"
	.incbin "baserom.gba", 0x00c980eb, 0x00000001
	.section .rom.00c9956e, "a"
	.incbin "baserom.gba", 0x00c9956e, 0x00000002
	.section .rom.00c9a5bd, "a"
	.incbin "baserom.gba", 0x00c9a5bd, 0x00000003
	.section .rom.00c9c422, "a"
	.incbin "baserom.gba", 0x00c9c422, 0x00000002
	.section .rom.00c9c5bd, "a"
	.incbin "baserom.gba", 0x00c9c5bd, 0x00000003
	.section .rom.00c9c71b, "a"
	.incbin "baserom.gba", 0x00c9c71b, 0x00000001
	.section .rom.00c9d69a, "a"
	.incbin "baserom.gba", 0x00c9d69a, 0x00000002
	.section .rom.00c9d7b2, "a"
	.incbin "baserom.gba", 0x00c9d7b2, 0x00000002
	.section .rom.00c9f845, "a"
	.incbin "baserom.gba", 0x00c9f845, 0x00000003
	.section .rom.00ca16c7, "a"
	.incbin "baserom.gba", 0x00ca16c7, 0x00000001
	.section .rom.00ca3bed, "a"
	.incbin "baserom.gba", 0x00ca3bed, 0x00000003
	.section .rom.00ca3cfa, "a"
	.incbin "baserom.gba", 0x00ca3cfa, 0x00000002
	.section .rom.00ca5d8d, "a"
	.incbin "baserom.gba", 0x00ca5d8d, 0x00000003
	.section .rom.00ca9263, "a"
	.incbin "baserom.gba", 0x00ca9263, 0x00000001
	.section .rom.00ca9d4d, "a"
	.incbin "baserom.gba", 0x00ca9d4d, 0x00000003
	.section .rom.00ca9ebd, "a"
	.incbin "baserom.gba", 0x00ca9ebd, 0x00000003
	.section .rom.00cacdd7, "a"
	.incbin "baserom.gba", 0x00cacdd7, 0x00000001
	.section .rom.00caed62, "a"
	.incbin "baserom.gba", 0x00caed62, 0x00000002
	.section .rom.00cb06cd, "a"
	.incbin "baserom.gba", 0x00cb06cd, 0x00000003
	.section .rom.00cb61be, "a"
	.incbin "baserom.gba", 0x00cb61be, 0x00000002
	.section .rom.00cb636e, "a"
	.incbin "baserom.gba", 0x00cb636e, 0x00000002
	.section .rom.00cbacca, "a"
	.incbin "baserom.gba", 0x00cbacca, 0x00000002
	.section .rom.00cbae22, "a"
	.incbin "baserom.gba", 0x00cbae22, 0x00000002
	.section .rom.00cbe3ef, "a"
	.incbin "baserom.gba", 0x00cbe3ef, 0x00000001
	.section .rom.00cbfc71, "a"
	.incbin "baserom.gba", 0x00cbfc71, 0x00000003
	.section .rom.00cc1999, "a"
	.incbin "baserom.gba", 0x00cc1999, 0x00000003
	.section .rom.00cc50ba, "a"
	.incbin "baserom.gba", 0x00cc50ba, 0x00000002
	.section .rom.00cca243, "a"
	.incbin "baserom.gba", 0x00cca243, 0x00000001
	.section .rom.00ccc811, "a"
	.incbin "baserom.gba", 0x00ccc811, 0x00000003
	.section .rom.00cce19d, "a"
	.incbin "baserom.gba", 0x00cce19d, 0x00000003
	.section .rom.00ccf2ca, "a"
	.incbin "baserom.gba", 0x00ccf2ca, 0x00000002
	.section .rom.00ccfe7a, "a"
	.incbin "baserom.gba", 0x00ccfe7a, 0x00000002
	.section .rom.00cd1842, "a"
	.incbin "baserom.gba", 0x00cd1842, 0x00000002
	.section .rom.00cd1926, "a"
	.incbin "baserom.gba", 0x00cd1926, 0x00000002
	.section .rom.00cd3d83, "a"
	.incbin "baserom.gba", 0x00cd3d83, 0x00000001
	.section .rom.00cd3ec3, "a"
	.incbin "baserom.gba", 0x00cd3ec3, 0x00000001
	.section .rom.00cd7539, "a"
	.incbin "baserom.gba", 0x00cd7539, 0x00000003
	.section .rom.00cd769d, "a"
	.incbin "baserom.gba", 0x00cd769d, 0x00000003
	.section .rom.00cd9d7d, "a"
	.incbin "baserom.gba", 0x00cd9d7d, 0x00000003
	.section .rom.00cdcc2d, "a"
	.incbin "baserom.gba", 0x00cdcc2d, 0x00000003
	.section .rom.00cde1a2, "a"
	.incbin "baserom.gba", 0x00cde1a2, 0x00000002
	.section .rom.00ce229e, "a"
	.incbin "baserom.gba", 0x00ce229e, 0x00000002
	.section .rom.00ce4523, "a"
	.incbin "baserom.gba", 0x00ce4523, 0x00000001
	.section .rom.00ce6107, "a"
	.incbin "baserom.gba", 0x00ce6107, 0x00000001
	.section .rom.00ce7dfd, "a"
	.incbin "baserom.gba", 0x00ce7dfd, 0x00000003
	.section .rom.00cf5421, "a"
	.incbin "baserom.gba", 0x00cf5421, 0x00000003
	.section .rom.00cf5549, "a"
	.incbin "baserom.gba", 0x00cf5549, 0x00000003
	.section .rom.00cf775a, "a"
	.incbin "baserom.gba", 0x00cf775a, 0x00000002
	.section .rom.00cf78eb, "a"
	.incbin "baserom.gba", 0x00cf78eb, 0x00000001
	.section .rom.00cfed9d, "a"
	.incbin "baserom.gba", 0x00cfed9d, 0x00000003
	.section .rom.00d01573, "a"
	.incbin "baserom.gba", 0x00d01573, 0x00000001
	.section .rom.00d03cce, "a"
	.incbin "baserom.gba", 0x00d03cce, 0x00000002
	.section .rom.00d03e81, "a"
	.incbin "baserom.gba", 0x00d03e81, 0x00000003
	.section .rom.00d05585, "a"
	.incbin "baserom.gba", 0x00d05585, 0x00000003
	.section .rom.00d075df, "a"
	.incbin "baserom.gba", 0x00d075df, 0x00000001
	.section .rom.00d08929, "a"
	.incbin "baserom.gba", 0x00d08929, 0x00000003
	.section .rom.00d0a373, "a"
	.incbin "baserom.gba", 0x00d0a373, 0x00000001
	.section .rom.00d0a497, "a"
	.incbin "baserom.gba", 0x00d0a497, 0x00000001
	.section .rom.00d0a969, "a"
	.incbin "baserom.gba", 0x00d0a969, 0x00000003
	.section .rom.00d0cb4d, "a"
	.incbin "baserom.gba", 0x00d0cb4d, 0x00000003
	.section .rom.00d0d021, "a"
	.incbin "baserom.gba", 0x00d0d021, 0x00000003
	.section .rom.00d0f557, "a"
	.incbin "baserom.gba", 0x00d0f557, 0x00000001
	.section .rom.00d0f6df, "a"
	.incbin "baserom.gba", 0x00d0f6df, 0x00000001
	.section .rom.00d13f81, "a"
	.incbin "baserom.gba", 0x00d13f81, 0x00000003
	.section .rom.00d140b7, "a"
	.incbin "baserom.gba", 0x00d140b7, 0x00000001
	.section .rom.00d15d23, "a"
	.incbin "baserom.gba", 0x00d15d23, 0x00000001
	.section .rom.00d16edb, "a"
	.incbin "baserom.gba", 0x00d16edb, 0x00000001
	.section .rom.00d17c29, "a"
	.incbin "baserom.gba", 0x00d17c29, 0x00000003
	.section .rom.00d18db2, "a"
	.incbin "baserom.gba", 0x00d18db2, 0x00000002
	.section .rom.00d1a8ab, "a"
	.incbin "baserom.gba", 0x00d1a8ab, 0x00000001
	.section .rom.00d1bcb7, "a"
	.incbin "baserom.gba", 0x00d1bcb7, 0x00000001
	.section .rom.00d1c20b, "a"
	.incbin "baserom.gba", 0x00d1c20b, 0x00000001
	.section .rom.00d1c845, "a"
	.incbin "baserom.gba", 0x00d1c845, 0x00000003
	.section .rom.00d21b11, "a"
	.incbin "baserom.gba", 0x00d21b11, 0x00000003
	.section .rom.00d24172, "a"
	.incbin "baserom.gba", 0x00d24172, 0x00000002
	.section .rom.00d24307, "a"
	.incbin "baserom.gba", 0x00d24307, 0x00000001
	.section .rom.00d25ebb, "a"
	.incbin "baserom.gba", 0x00d25ebb, 0x00000001
	.section .rom.00d2601b, "a"
	.incbin "baserom.gba", 0x00d2601b, 0x00000001
	.section .rom.00d28abf, "a"
	.incbin "baserom.gba", 0x00d28abf, 0x000021b1
	.section .rom.00d2cae6, "a"
	.incbin "baserom.gba", 0x00d2cae6, 0x00000002
	.section .rom.00d2cc27, "a"
	.incbin "baserom.gba", 0x00d2cc27, 0x00000001
	.section .rom.00d33b9b, "a"
	.incbin "baserom.gba", 0x00d33b9b, 0x00000001
	.section .rom.00d33cd9, "a"
	.incbin "baserom.gba", 0x00d33cd9, 0x00000003
	.section .rom.00d35cad, "a"
	.incbin "baserom.gba", 0x00d35cad, 0x00000003
	.section .rom.00d35da1, "a"
	.incbin "baserom.gba", 0x00d35da1, 0x00000003
	.section .rom.00d36e81, "a"
	.incbin "baserom.gba", 0x00d36e81, 0x00000003
	.section .rom.00d38d5b, "a"
	.incbin "baserom.gba", 0x00d38d5b, 0x00000001
	.section .rom.00d39d13, "a"
	.incbin "baserom.gba", 0x00d39d13, 0x00000001
	.section .rom.00d3bc5a, "a"
	.incbin "baserom.gba", 0x00d3bc5a, 0x00000002
	.section .rom.00d3d865, "a"
	.incbin "baserom.gba", 0x00d3d865, 0x00000003
	.section .rom.00d3d9b1, "a"
	.incbin "baserom.gba", 0x00d3d9b1, 0x00000003
	.section .rom.00d3fa6f, "a"
	.incbin "baserom.gba", 0x00d3fa6f, 0x00000001
	.section .rom.00d43b5e, "a"
	.incbin "baserom.gba", 0x00d43b5e, 0x00000002
	.section .rom.00d47910, "a"
	.incbin "baserom.gba", 0x00d47910, 0x00007aa8
	.section .rom.00d50b05, "a"
	.incbin "baserom.gba", 0x00d50b05, 0x00000003
	.section .rom.00d50c32, "a"
	.incbin "baserom.gba", 0x00d50c32, 0x00000002
	.section .rom.00d57ba5, "a"
	.incbin "baserom.gba", 0x00d57ba5, 0x00000003
	.section .rom.00d57caa, "a"
	.incbin "baserom.gba", 0x00d57caa, 0x00000002
	.section .rom.00d57dea, "a"
	.incbin "baserom.gba", 0x00d57dea, 0x00000002
	.section .rom.00d597b6, "a"
	.incbin "baserom.gba", 0x00d597b6, 0x00000002
	.section .rom.00d598ad, "a"
	.incbin "baserom.gba", 0x00d598ad, 0x00000003
	.section .rom.00d5c701, "a"
	.incbin "baserom.gba", 0x00d5c701, 0x00000003
	.section .rom.00d5c8aa, "a"
	.incbin "baserom.gba", 0x00d5c8aa, 0x00000002
	.section .rom.00d5f4c6, "a"
	.incbin "baserom.gba", 0x00d5f4c6, 0x00000002
	.section .rom.00d621f9, "a"
	.incbin "baserom.gba", 0x00d621f9, 0x00000003
	.section .rom.00d63ee3, "a"
	.incbin "baserom.gba", 0x00d63ee3, 0x00000001
	.section .rom.00d6775d, "a"
	.incbin "baserom.gba", 0x00d6775d, 0x00000003
	.section .rom.00d678b5, "a"
	.incbin "baserom.gba", 0x00d678b5, 0x00000003
	.section .rom.00d69df9, "a"
	.incbin "baserom.gba", 0x00d69df9, 0x00000003
	.section .rom.00d6dcbe, "a"
	.incbin "baserom.gba", 0x00d6dcbe, 0x00000002
	.section .rom.00d6fdee, "a"
	.incbin "baserom.gba", 0x00d6fdee, 0x00000002
	.section .rom.00d73ffa, "a"
	.incbin "baserom.gba", 0x00d73ffa, 0x00000002
	.section .rom.00d741ce, "a"
	.incbin "baserom.gba", 0x00d741ce, 0x00000002
	.section .rom.00d7620e, "a"
	.incbin "baserom.gba", 0x00d7620e, 0x00000002
	.section .rom.00d77605, "a"
	.incbin "baserom.gba", 0x00d77605, 0x00000003
	.section .rom.00d78142, "a"
	.incbin "baserom.gba", 0x00d78142, 0x00000002
	.section .rom.00d792ad, "a"
	.incbin "baserom.gba", 0x00d792ad, 0x00000003
	.section .rom.00d7a359, "a"
	.incbin "baserom.gba", 0x00d7a359, 0x00000003
	.section .rom.00d7a505, "a"
	.incbin "baserom.gba", 0x00d7a505, 0x00000003
	.section .rom.00d7c012, "a"
	.incbin "baserom.gba", 0x00d7c012, 0x00000002
	.section .rom.00d7da2f, "a"
	.incbin "baserom.gba", 0x00d7da2f, 0x00000001
	.section .rom.00d7ff01, "a"
	.incbin "baserom.gba", 0x00d7ff01, 0x00000003
	.section .rom.00d81263, "a"
	.incbin "baserom.gba", 0x00d81263, 0x00000001
	.section .rom.00d82117, "a"
	.incbin "baserom.gba", 0x00d82117, 0x00000001
	.section .rom.00d83705, "a"
	.incbin "baserom.gba", 0x00d83705, 0x00000003
	.section .rom.00d84c4b, "a"
	.incbin "baserom.gba", 0x00d84c4b, 0x00000001
	.section .rom.00d85a15, "a"
	.incbin "baserom.gba", 0x00d85a15, 0x00000003
	.section .rom.00d85b83, "a"
	.incbin "baserom.gba", 0x00d85b83, 0x00000001
	.section .rom.00d86b42, "a"
	.incbin "baserom.gba", 0x00d86b42, 0x00000002
	.section .rom.00d8760b, "a"
	.incbin "baserom.gba", 0x00d8760b, 0x00000001
	.section .rom.00d881a5, "a"
	.incbin "baserom.gba", 0x00d881a5, 0x00000003
	.section .rom.00d882f5, "a"
	.incbin "baserom.gba", 0x00d882f5, 0x00000003
	.section .rom.00d8c5a3, "a"
	.incbin "baserom.gba", 0x00d8c5a3, 0x00000001
	.section .rom.00d8e03d, "a"
	.incbin "baserom.gba", 0x00d8e03d, 0x00000003
	.section .rom.00d8fa13, "a"
	.incbin "baserom.gba", 0x00d8fa13, 0x00000001
	.section .rom.00d8fb92, "a"
	.incbin "baserom.gba", 0x00d8fb92, 0x00000002
	.section .rom.00d937c2, "a"
	.incbin "baserom.gba", 0x00d937c2, 0x00000002
	.section .rom.00d9551f, "a"
	.incbin "baserom.gba", 0x00d9551f, 0x00000001
	.section .rom.00d98b49, "a"
	.incbin "baserom.gba", 0x00d98b49, 0x00000003
	.section .rom.00d98c9a, "a"
	.incbin "baserom.gba", 0x00d98c9a, 0x00000002
	.section .rom.00d9e896, "a"
	.incbin "baserom.gba", 0x00d9e896, 0x00000002
	.section .rom.00da11d3, "a"
	.incbin "baserom.gba", 0x00da11d3, 0x00000001
	.section .rom.00da2b15, "a"
	.incbin "baserom.gba", 0x00da2b15, 0x00000003
	.section .rom.00da2c7e, "a"
	.incbin "baserom.gba", 0x00da2c7e, 0x00000002
	.section .rom.00da9a7d, "a"
	.incbin "baserom.gba", 0x00da9a7d, 0x00000003
	.section .rom.00daa05b, "a"
	.incbin "baserom.gba", 0x00daa05b, 0x00000001
	.section .rom.00daaf63, "a"
	.incbin "baserom.gba", 0x00daaf63, 0x00000001
	.section .rom.00dad7ab, "a"
	.incbin "baserom.gba", 0x00dad7ab, 0x00000001
	.section .rom.00daedca, "a"
	.incbin "baserom.gba", 0x00daedca, 0x00000002
	.section .rom.00db072b, "a"
	.incbin "baserom.gba", 0x00db072b, 0x00000001
	.section .rom.00db3513, "a"
	.incbin "baserom.gba", 0x00db3513, 0x00000001
	.section .rom.00db368e, "a"
	.incbin "baserom.gba", 0x00db368e, 0x00000002
	.section .rom.00dc2857, "a"
	.incbin "baserom.gba", 0x00dc2857, 0x00000001
	.section .rom.00dc29bf, "a"
	.incbin "baserom.gba", 0x00dc29bf, 0x00000001
	.section .rom.00dc4fe2, "a"
	.incbin "baserom.gba", 0x00dc4fe2, 0x00000002
	.section .rom.00dc5125, "a"
	.incbin "baserom.gba", 0x00dc5125, 0x00000003
	.section .rom.00dc69e2, "a"
	.incbin "baserom.gba", 0x00dc69e2, 0x00000002
	.section .rom.00dc9946, "a"
	.incbin "baserom.gba", 0x00dc9946, 0x00000002
	.section .rom.00dcc2de, "a"
	.incbin "baserom.gba", 0x00dcc2de, 0x00000002
	.section .rom.00dd0c36, "a"
	.incbin "baserom.gba", 0x00dd0c36, 0x00000002
	.section .rom.00dd325f, "a"
	.incbin "baserom.gba", 0x00dd325f, 0x00000001
	.section .rom.00dd3433, "a"
	.incbin "baserom.gba", 0x00dd3433, 0x00000001
	.section .rom.00dd6752, "a"
	.incbin "baserom.gba", 0x00dd6752, 0x00000002
	.section .rom.00dd6929, "a"
	.incbin "baserom.gba", 0x00dd6929, 0x00000003
	.section .rom.00dd9c9d, "a"
	.incbin "baserom.gba", 0x00dd9c9d, 0x00000003
	.section .rom.00ddb3f1, "a"
	.incbin "baserom.gba", 0x00ddb3f1, 0x00000003
	.section .rom.00ddc37f, "a"
	.incbin "baserom.gba", 0x00ddc37f, 0x00000001
	.section .rom.00dddcf5, "a"
	.incbin "baserom.gba", 0x00dddcf5, 0x00000003
	.section .rom.00ddde2f, "a"
	.incbin "baserom.gba", 0x00ddde2f, 0x00000001
	.section .rom.00ddf971, "a"
	.incbin "baserom.gba", 0x00ddf971, 0x00000003
	.section .rom.00de2f49, "a"
	.incbin "baserom.gba", 0x00de2f49, 0x00000003
	.section .rom.00de3bd3, "a"
	.incbin "baserom.gba", 0x00de3bd3, 0x00000001
	.section .rom.00de3d13, "a"
	.incbin "baserom.gba", 0x00de3d13, 0x00000001
	.section .rom.00de5ad7, "a"
	.incbin "baserom.gba", 0x00de5ad7, 0x00000001
	.section .rom.00de5c4a, "a"
	.incbin "baserom.gba", 0x00de5c4a, 0x00000002
	.section .rom.00de80b2, "a"
	.incbin "baserom.gba", 0x00de80b2, 0x00000002
	.section .rom.00de9a4e, "a"
	.incbin "baserom.gba", 0x00de9a4e, 0x00000002
	.section .rom.00deb883, "a"
	.incbin "baserom.gba", 0x00deb883, 0x00000001
	.section .rom.00dec33a, "a"
	.incbin "baserom.gba", 0x00dec33a, 0x00000002
	.section .rom.00dedd56, "a"
	.incbin "baserom.gba", 0x00dedd56, 0x00000002
	.section .rom.00df0a22, "a"
	.incbin "baserom.gba", 0x00df0a22, 0x00000002
	.section .rom.00df0bfd, "a"
	.incbin "baserom.gba", 0x00df0bfd, 0x00000003
	.section .rom.00df2733, "a"
	.incbin "baserom.gba", 0x00df2733, 0x00000001
	.section .rom.00df57fd, "a"
	.incbin "baserom.gba", 0x00df57fd, 0x00000003
	.section .rom.00df69b9, "a"
	.incbin "baserom.gba", 0x00df69b9, 0x00000003
	.section .rom.00df6ba1, "a"
	.incbin "baserom.gba", 0x00df6ba1, 0x00000003
	.section .rom.00df6d33, "a"
	.incbin "baserom.gba", 0x00df6d33, 0x00000001
	.section .rom.00df7aba, "a"
	.incbin "baserom.gba", 0x00df7aba, 0x00000002
	.section .rom.00df7c4f, "a"
	.incbin "baserom.gba", 0x00df7c4f, 0x00000001
	.section .rom.00df9d03, "a"
	.incbin "baserom.gba", 0x00df9d03, 0x00000001
	.section .rom.00e01d86, "a"
	.incbin "baserom.gba", 0x00e01d86, 0x00000002
	.section .rom.00e01ec7, "a"
	.incbin "baserom.gba", 0x00e01ec7, 0x00000001
	.section .rom.00e0624f, "a"
	.incbin "baserom.gba", 0x00e0624f, 0x00000001
	.section .rom.00e0696e, "a"
	.incbin "baserom.gba", 0x00e0696e, 0x00000002
	.section .rom.00e07d2d, "a"
	.incbin "baserom.gba", 0x00e07d2d, 0x00000003
	.section .rom.00e0be47, "a"
	.incbin "baserom.gba", 0x00e0be47, 0x00000001
	.section .rom.00e0cec7, "a"
	.incbin "baserom.gba", 0x00e0cec7, 0x00000001
	.section .rom.00e0df49, "a"
	.incbin "baserom.gba", 0x00e0df49, 0x00000003
	.section .rom.00e0eaba, "a"
	.incbin "baserom.gba", 0x00e0eaba, 0x00000002
	.section .rom.00e0fa0b, "a"
	.incbin "baserom.gba", 0x00e0fa0b, 0x00000001
	.section .rom.00e0fb89, "a"
	.incbin "baserom.gba", 0x00e0fb89, 0x00000003
	.section .rom.00e130e3, "a"
	.incbin "baserom.gba", 0x00e130e3, 0x00000001
	.section .rom.00e14716, "a"
	.incbin "baserom.gba", 0x00e14716, 0x00000002
	.section .rom.00e14857, "a"
	.incbin "baserom.gba", 0x00e14857, 0x00000001
	.section .rom.00e171a2, "a"
	.incbin "baserom.gba", 0x00e171a2, 0x00000002
	.section .rom.00e1fb4a, "a"
	.incbin "baserom.gba", 0x00e1fb4a, 0x00000fde
	.section .rom.00e226f5, "a"
	.incbin "baserom.gba", 0x00e226f5, 0x00000003
	.section .rom.00e24635, "a"
	.incbin "baserom.gba", 0x00e24635, 0x00000003
	.section .rom.00e25aa9, "a"
	.incbin "baserom.gba", 0x00e25aa9, 0x00000003
	.section .rom.00e27763, "a"
	.incbin "baserom.gba", 0x00e27763, 0x00000001
	.section .rom.00e34812, "a"
	.incbin "baserom.gba", 0x00e34812, 0x00000002
	.section .rom.00e34951, "a"
	.incbin "baserom.gba", 0x00e34951, 0x00000003
	.section .rom.00e36876, "a"
	.incbin "baserom.gba", 0x00e36876, 0x00000002
	.section .rom.00e3699f, "a"
	.incbin "baserom.gba", 0x00e3699f, 0x00000001
	.section .rom.00e38b6f, "a"
	.incbin "baserom.gba", 0x00e38b6f, 0x00000001
	.section .rom.00e3b13f, "a"
	.incbin "baserom.gba", 0x00e3b13f, 0x00000001
	.section .rom.00e3d451, "a"
	.incbin "baserom.gba", 0x00e3d451, 0x00000003
	.section .rom.00e3dcc5, "a"
	.incbin "baserom.gba", 0x00e3dcc5, 0x00000003
	.section .rom.00e3de07, "a"
	.incbin "baserom.gba", 0x00e3de07, 0x00000001
	.section .rom.00e40ebd, "a"
	.incbin "baserom.gba", 0x00e40ebd, 0x00000003
	.section .rom.00e425a3, "a"
	.incbin "baserom.gba", 0x00e425a3, 0x00000001
	.section .rom.00e4416b, "a"
	.incbin "baserom.gba", 0x00e4416b, 0x00000001
	.section .rom.00e45ce6, "a"
	.incbin "baserom.gba", 0x00e45ce6, 0x00000002
	.section .rom.00e46131, "a"
	.incbin "baserom.gba", 0x00e46131, 0x00000003
	.section .rom.00e4828e, "a"
	.incbin "baserom.gba", 0x00e4828e, 0x00000002
	.section .rom.00e483a7, "a"
	.incbin "baserom.gba", 0x00e483a7, 0x00000001
	.section .rom.00e491d1, "a"
	.incbin "baserom.gba", 0x00e491d1, 0x00000003
	.section .rom.00e49333, "a"
	.incbin "baserom.gba", 0x00e49333, 0x00000001
	.section .rom.00e4db9b, "a"
	.incbin "baserom.gba", 0x00e4db9b, 0x00000001
	.section .rom.00e50259, "a"
	.incbin "baserom.gba", 0x00e50259, 0x00000003
	.section .rom.00e50837, "a"
	.incbin "baserom.gba", 0x00e50837, 0x00000001
	.section .rom.00e527ca, "a"
	.incbin "baserom.gba", 0x00e527ca, 0x00000002
	.section .rom.00e53fae, "a"
	.incbin "baserom.gba", 0x00e53fae, 0x00000002
	.section .rom.00e54112, "a"
	.incbin "baserom.gba", 0x00e54112, 0x00000002
	.section .rom.00e5690a, "a"
	.incbin "baserom.gba", 0x00e5690a, 0x00000002
	.section .rom.00e56ac3, "a"
	.incbin "baserom.gba", 0x00e56ac3, 0x00000001
	.section .rom.00e56c4b, "a"
	.incbin "baserom.gba", 0x00e56c4b, 0x00000001
	.section .rom.00e5846d, "a"
	.incbin "baserom.gba", 0x00e5846d, 0x00000003
	.section .rom.00e59056, "a"
	.incbin "baserom.gba", 0x00e59056, 0x00000002
	.section .rom.00e5a58f, "a"
	.incbin "baserom.gba", 0x00e5a58f, 0x00000001
	.section .rom.00e5a72a, "a"
	.incbin "baserom.gba", 0x00e5a72a, 0x00000002
	.section .rom.00e5c9d6, "a"
	.incbin "baserom.gba", 0x00e5c9d6, 0x00000002
	.section .rom.00e5eb9d, "a"
	.incbin "baserom.gba", 0x00e5eb9d, 0x00000003
	.section .rom.00e6029b, "a"
	.incbin "baserom.gba", 0x00e6029b, 0x00000001
	.section .rom.00e60dbf, "a"
	.incbin "baserom.gba", 0x00e60dbf, 0x00000001
	.section .rom.00e65c46, "a"
	.incbin "baserom.gba", 0x00e65c46, 0x00000002
	.section .rom.00e67fed, "a"
	.incbin "baserom.gba", 0x00e67fed, 0x00000003
	.section .rom.00e686d6, "a"
	.incbin "baserom.gba", 0x00e686d6, 0x00000002
	.section .rom.00e692c2, "a"
	.incbin "baserom.gba", 0x00e692c2, 0x00000ac6
	.section .rom.00e6b337, "a"
	.incbin "baserom.gba", 0x00e6b337, 0x00000001
	.section .rom.00e6b4ea, "a"
	.incbin "baserom.gba", 0x00e6b4ea, 0x00000002
	.section .rom.00e6d729, "a"
	.incbin "baserom.gba", 0x00e6d729, 0x00000003
	.section .rom.00e6e24f, "a"
	.incbin "baserom.gba", 0x00e6e24f, 0x00000001
	.section .rom.00e78cd8, "a"
	.incbin "baserom.gba", 0x00e78cd8, 0x000007d0
	.section .rom.00e7c171, "a"
	.incbin "baserom.gba", 0x00e7c171, 0x00000003
	.section .rom.00e7c2eb, "a"
	.incbin "baserom.gba", 0x00e7c2eb, 0x00000001
	.section .rom.00e7c462, "a"
	.incbin "baserom.gba", 0x00e7c462, 0x00000002
	.section .rom.00e7c5fa, "a"
	.incbin "baserom.gba", 0x00e7c5fa, 0x00000002
	.section .rom.00e7c78b, "a"
	.incbin "baserom.gba", 0x00e7c78b, 0x00000001
	.section .rom.00e7ea37, "a"
	.incbin "baserom.gba", 0x00e7ea37, 0x00000001
	.section .rom.00e7eb4f, "a"
	.incbin "baserom.gba", 0x00e7eb4f, 0x00000001
	.section .rom.00e80951, "a"
	.incbin "baserom.gba", 0x00e80951, 0x00000003
	.section .rom.00e82416, "a"
	.incbin "baserom.gba", 0x00e82416, 0x00000002
	.section .rom.00e82b3e, "a"
	.incbin "baserom.gba", 0x00e82b3e, 0x00000002
	.section .rom.00e83f3a, "a"
	.incbin "baserom.gba", 0x00e83f3a, 0x00000002
	.section .rom.00e84e0e, "a"
	.incbin "baserom.gba", 0x00e84e0e, 0x00000002
	.section .rom.00e84f52, "a"
	.incbin "baserom.gba", 0x00e84f52, 0x00000002
	.section .rom.00e87303, "a"
	.incbin "baserom.gba", 0x00e87303, 0x00000001
	.section .rom.00e89b31, "a"
	.incbin "baserom.gba", 0x00e89b31, 0x00000003
	.section .rom.00e89f2e, "a"
	.incbin "baserom.gba", 0x00e89f2e, 0x00000002
	.section .rom.00e8b3ce, "a"
	.incbin "baserom.gba", 0x00e8b3ce, 0x00000002
	.section .rom.00e8b522, "a"
	.incbin "baserom.gba", 0x00e8b522, 0x00000002
	.section .rom.00e8ee6f, "a"
	.incbin "baserom.gba", 0x00e8ee6f, 0x00000001
	.section .rom.00e902da, "a"
	.incbin "baserom.gba", 0x00e902da, 0x00000002
	.section .rom.00e917b3, "a"
	.incbin "baserom.gba", 0x00e917b3, 0x00000001
	.section .rom.00e91906, "a"
	.incbin "baserom.gba", 0x00e91906, 0x00000002
	.section .rom.00e95313, "a"
	.incbin "baserom.gba", 0x00e95313, 0x00000001
	.section .rom.00e964eb, "a"
	.incbin "baserom.gba", 0x00e964eb, 0x00000001
	.section .rom.00e97cf6, "a"
	.incbin "baserom.gba", 0x00e97cf6, 0x00000002
	.section .rom.00e97e46, "a"
	.incbin "baserom.gba", 0x00e97e46, 0x00000002
	.section .rom.00e9a553, "a"
	.incbin "baserom.gba", 0x00e9a553, 0x00000001
	.section .rom.00e9a6ba, "a"
	.incbin "baserom.gba", 0x00e9a6ba, 0x00000002
	.section .rom.00e9d67b, "a"
	.incbin "baserom.gba", 0x00e9d67b, 0x00000001
	.section .rom.00e9e8e9, "a"
	.incbin "baserom.gba", 0x00e9e8e9, 0x00000003
	.section .rom.00ea1476, "a"
	.incbin "baserom.gba", 0x00ea1476, 0x00000002
	.section .rom.00ea1e5d, "a"
	.incbin "baserom.gba", 0x00ea1e5d, 0x00000003
	.section .rom.00ea27fe, "a"
	.incbin "baserom.gba", 0x00ea27fe, 0x00000002
	.section .rom.00ea2972, "a"
	.incbin "baserom.gba", 0x00ea2972, 0x00000002
	.section .rom.00ea4483, "a"
	.incbin "baserom.gba", 0x00ea4483, 0x00000001
	.section .rom.00ea45a5, "a"
	.incbin "baserom.gba", 0x00ea45a5, 0x00000003
	.section .rom.00ea70a3, "a"
	.incbin "baserom.gba", 0x00ea70a3, 0x00000001
	.section .rom.00ea7e9a, "a"
	.incbin "baserom.gba", 0x00ea7e9a, 0x00000002
	.section .rom.00ea9e39, "a"
	.incbin "baserom.gba", 0x00ea9e39, 0x00000003
	.section .rom.00eab169, "a"
	.incbin "baserom.gba", 0x00eab169, 0x00000003
	.section .rom.00eab2bd, "a"
	.incbin "baserom.gba", 0x00eab2bd, 0x00000003
	.section .rom.00eabe6f, "a"
	.incbin "baserom.gba", 0x00eabe6f, 0x00000001
	.section .rom.00ead649, "a"
	.incbin "baserom.gba", 0x00ead649, 0x00000003
	.section .rom.00eae7a9, "a"
	.incbin "baserom.gba", 0x00eae7a9, 0x00000003
	.section .rom.00eaf8c7, "a"
	.incbin "baserom.gba", 0x00eaf8c7, 0x00000001
	.section .rom.00eaface, "a"
	.incbin "baserom.gba", 0x00eaface, 0x00000002
	.section .rom.00eb07c3, "a"
	.incbin "baserom.gba", 0x00eb07c3, 0x00000001
	.section .rom.00eb2f65, "a"
	.incbin "baserom.gba", 0x00eb2f65, 0x00000003
	.section .rom.00eb46da, "a"
	.incbin "baserom.gba", 0x00eb46da, 0x00000002
	.section .rom.00eb5997, "a"
	.incbin "baserom.gba", 0x00eb5997, 0x00000001
	.section .rom.00eb61da, "a"
	.incbin "baserom.gba", 0x00eb61da, 0x0000060e
	.section .rom.00eb6d5f, "a"
	.incbin "baserom.gba", 0x00eb6d5f, 0x00000001
	.section .rom.00eb6e9f, "a"
	.incbin "baserom.gba", 0x00eb6e9f, 0x00000001
	.section .rom.00eb6fdf, "a"
	.incbin "baserom.gba", 0x00eb6fdf, 0x00000001
	.section .rom.00eb7b41, "a"
	.incbin "baserom.gba", 0x00eb7b41, 0x00000003
	.section .rom.00eba2e5, "a"
	.incbin "baserom.gba", 0x00eba2e5, 0x00000003
	.section .rom.00ebba5a, "a"
	.incbin "baserom.gba", 0x00ebba5a, 0x00000002
	.section .rom.00ebcd17, "a"
	.incbin "baserom.gba", 0x00ebcd17, 0x00000001
	.section .rom.00ebd55a, "a"
	.incbin "baserom.gba", 0x00ebd55a, 0x00000002
	.section .rom.00ebdaf9, "a"
	.incbin "baserom.gba", 0x00ebdaf9, 0x0000051f
	.section .rom.00ebe72d, "a"
	.incbin "baserom.gba", 0x00ebe72d, 0x0000051f
	.section .rom.00ebf80d, "a"
	.incbin "baserom.gba", 0x00ebf80d, 0x0000051f
	.section .rom.00ec02f9, "a"
	.incbin "baserom.gba", 0x00ec02f9, 0x0000051f
	.section .rom.00ec1195, "a"
	.incbin "baserom.gba", 0x00ec1195, 0x0000051f
	.section .rom.00ec1add, "a"
	.incbin "baserom.gba", 0x00ec1add, 0x0000051f
	.section .rom.00ec2625, "a"
	.incbin "baserom.gba", 0x00ec2625, 0x0000051f
	.section .rom.00ec326d, "a"
	.incbin "baserom.gba", 0x00ec326d, 0x0013cd93
