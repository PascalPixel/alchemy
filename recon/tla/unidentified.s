@ Unidentified ROM data, read from your own ROM at build time as early pret
@ projects read their base ROM. Each section shrinks as its data gains source.
	.section .unidentified.08000000,"a"
	.incbin "baserom.gba", 0x00000000, 0x000000c0
	.section .unidentified.08017878,"a"
	.incbin "baserom.gba", 0x00017878, 0x0000046c
	.section .unidentified.08017d08,"a"
	.incbin "baserom.gba", 0x00017d08, 0x000082f8
	.section .unidentified.080203a8,"a"
	.incbin "baserom.gba", 0x000203a8, 0x00000a00
	.section .unidentified.0802e89c,"a"
	.incbin "baserom.gba", 0x0002e89c, 0x00001a74
	.section .unidentified.08030320,"a"
	.incbin "baserom.gba", 0x00030320, 0x00007ce0
	.section .unidentified.0804e584,"a"
	.incbin "baserom.gba", 0x0004e584, 0x0005ea7c
	.section .unidentified.080b127c,"a"
	.incbin "baserom.gba", 0x000b127c, 0x00000cac
	.section .unidentified.080b1f2c,"a"
	.incbin "baserom.gba", 0x000b1f2c, 0x000160d4
	.section .unidentified.080c89cc,"a"
	.incbin "baserom.gba", 0x000c89cc, 0x00000a00
	.section .unidentified.080ed80c,"a"
	.incbin "baserom.gba", 0x000ed80c, 0x00002010
	.section .unidentified.080ef824,"a"
	.incbin "baserom.gba", 0x000ef824, 0x000087dc
	.section .unidentified.081055f8,"a"
	.incbin "baserom.gba", 0x001055f8, 0x00002a08
	.section .unidentified.0810c008,"a"
	.incbin "baserom.gba", 0x0010c008, 0x0000bff8
	.section .unidentified.081287c4,"a"
	.incbin "baserom.gba", 0x001287c4, 0x000046b0
	.section .unidentified.0812ce7c,"a"
	.incbin "baserom.gba", 0x0012ce7c, 0x0000b184
	.section .unidentified.08161ac4,"a"
	.incbin "baserom.gba", 0x00161ac4, 0x00001966
	.section .unidentified.08196dd8,"a"
	.incbin "baserom.gba", 0x00196dd8, 0x00000204
	.section .unidentified.081973f0,"a"
	.incbin "baserom.gba", 0x001973f0, 0x00008c10
	.section .unidentified.081a1928,"a"
	.incbin "baserom.gba", 0x001a1928, 0x000046d8
	.section .unidentified.081a8288,"a"
	.incbin "baserom.gba", 0x001a8288, 0x00003d78
	.section .unidentified.081ad348,"a"
	.incbin "baserom.gba", 0x001ad348, 0x00004cb8
	.section .unidentified.081b4888,"a"
	.incbin "baserom.gba", 0x001b4888, 0x00003778
	.section .unidentified.081ba30c,"a"
	.incbin "baserom.gba", 0x001ba30c, 0x00005cf4
	.section .unidentified.081c342e,"a"
	.incbin "baserom.gba", 0x001c342e, 0x00e3cbd2
