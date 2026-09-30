@ tbs-it's scaffold: the base-ROM ranges its MAIN.LD places between the
@ lines it links from source, with a label where the source names a place.
	.section .rom.00000000, "a"
	.incbin "baserom.gba", 0x00000000, 0x000003c0
	.global Entry_080003c0
Entry_080003c0:
	.incbin "baserom.gba", 0x000003c0, 0x000003b0
	.section .rom.00000770, "a"
	.incbin "baserom.gba", 0x00000770, 0x00001400
	.section .rom.00001b70, "a"
	.incbin "baserom.gba", 0x00001b70, 0x0000077c
	.global IwramSignedDivideEntry
	.thumb_func
IwramSignedDivideEntry:
	.incbin "baserom.gba", 0x000022ec, 0x00000008
	.global IwramUnsignedDivideEntry
	.thumb_func
IwramUnsignedDivideEntry:
	.incbin "baserom.gba", 0x000022f4, 0x00000008
	.global IwramSignedRemainderEntry
	.thumb_func
IwramSignedRemainderEntry:
	.incbin "baserom.gba", 0x000022fc, 0x00000008
	.global IwramUnsignedRemainderEntry
	.thumb_func
IwramUnsignedRemainderEntry:
	.incbin "baserom.gba", 0x00002304, 0x00000504
	.global Resource_Decode2
Resource_Decode2:
	.incbin "baserom.gba", 0x00002808, 0x000004ec
	.global Resource_Decode2End
Resource_Decode2End:
	.incbin "baserom.gba", 0x00002cf4, 0x0000638c
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	.incbin "baserom.gba", 0x00009080, 0x00000018
	.global ObjectDispatch_InitializeFar
	.thumb_func
ObjectDispatch_InitializeFar:
	.incbin "baserom.gba", 0x00009098, 0x00000030
	.global Object_CreateFar
	.thumb_func
Object_CreateFar:
	.incbin "baserom.gba", 0x000090c8, 0x00000af0
	.global Render_DecodeFrame
Render_DecodeFrame:
	.incbin "baserom.gba", 0x00009bb8, 0x000002c4
	.global Render_DecodeFrameEnd
Render_DecodeFrameEnd:
	.incbin "baserom.gba", 0x00009e7c, 0x0000bc5c
	.global Tile_Decompress4bpp
Tile_Decompress4bpp:
	.incbin "baserom.gba", 0x00015ad8, 0x00000278
	.global Tile_Decompress4bppEnd
Tile_Decompress4bppEnd:
	.global Tile_ExpandMasked
Tile_ExpandMasked:
	.incbin "baserom.gba", 0x00015d50, 0x0000009c
	.global Tile_ExpandMaskedEnd
Tile_ExpandMaskedEnd:
	.global Tile_ExpandOpaque
Tile_ExpandOpaque:
	.incbin "baserom.gba", 0x00015dec, 0x0000007c
	.global Tile_ExpandOpaqueEnd
Tile_ExpandOpaqueEnd:
	.incbin "baserom.gba", 0x00015e68, 0x0001bb28
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00031990, 0x000009dc
	.section .rom.00033f6c, "a"
	.incbin "baserom.gba", 0x00033f6c, 0x0005e174
	.global Object_GetById
	.thumb_func
Object_GetById:
	.incbin "baserom.gba", 0x000920e0, 0x0005df44
	.global SentouKouka_Tenkai
SentouKouka_Tenkai:
	.incbin "baserom.gba", 0x000f0024, 0x00000230
	.global SentouKouka_TenkaiEnd
SentouKouka_TenkaiEnd:
	.incbin "baserom.gba", 0x000f0254, 0x00009420
	.section .rom.000f9674, "a"
	.incbin "baserom.gba", 0x000f9674, 0x0000039e
	.global Mixer_CallViaR3
	.thumb_func
Mixer_CallViaR3:
	.incbin "baserom.gba", 0x000f9a12, 0x00000006
	.section .rom.000f9a18, "a"
	.incbin "baserom.gba", 0x000f9a18, 0x00002060
	.section .rom.000fc504, "a"
	.incbin "baserom.gba", 0x000fc504, 0x00000090
	.section .rom.000fc624, "a"
	.incbin "baserom.gba", 0x000fc624, 0x00000060
	.section .rom.00184698, "a"
	.incbin "baserom.gba", 0x00184698, 0x0000098c
	.section .rom.0031efe0, "a"
	.incbin "baserom.gba", 0x0031efe0, 0x00001fd0
	.section .rom.003249e7, "a"
	.incbin "baserom.gba", 0x003249e7, 0x00000001
	.section .rom.0032b09b, "a"
	.incbin "baserom.gba", 0x0032b09b, 0x000086f9
	.section .rom.003357e5, "a"
	.incbin "baserom.gba", 0x003357e5, 0x00000003
	.section .rom.003370f5, "a"
	.incbin "baserom.gba", 0x003370f5, 0x00000003
	.section .rom.0033abf9, "a"
	.incbin "baserom.gba", 0x0033abf9, 0x0000068b
	.section .rom.0033f6c6, "a"
	.incbin "baserom.gba", 0x0033f6c6, 0x00000002
	.section .rom.0034fe36, "a"
	.incbin "baserom.gba", 0x0034fe36, 0x00000002
	.section .rom.00353426, "a"
	.incbin "baserom.gba", 0x00353426, 0x00000002
	.section .rom.0035eeca, "a"
	.incbin "baserom.gba", 0x0035eeca, 0x00000002
	.section .rom.0036f57a, "a"
	.incbin "baserom.gba", 0x0036f57a, 0x00000002
	.section .rom.0037701a, "a"
	.incbin "baserom.gba", 0x0037701a, 0x00000002
	.section .rom.0037a85a, "a"
	.incbin "baserom.gba", 0x0037a85a, 0x00000002
	.section .rom.00383c8e, "a"
	.incbin "baserom.gba", 0x00383c8e, 0x00000002
	.section .rom.0038f0b6, "a"
	.incbin "baserom.gba", 0x0038f0b6, 0x00000002
	.section .rom.00396f72, "a"
	.incbin "baserom.gba", 0x00396f72, 0x00000002
	.section .rom.0039ba02, "a"
	.incbin "baserom.gba", 0x0039ba02, 0x00000002
	.section .rom.0039fcb6, "a"
	.incbin "baserom.gba", 0x0039fcb6, 0x00000002
	.section .rom.003a3636, "a"
	.incbin "baserom.gba", 0x003a3636, 0x00000002
	.section .rom.003afe72, "a"
	.incbin "baserom.gba", 0x003afe72, 0x00000002
	.section .rom.003be7ba, "a"
	.incbin "baserom.gba", 0x003be7ba, 0x00000002
	.section .rom.003c40bd, "a"
	.incbin "baserom.gba", 0x003c40bd, 0x00000003
	.section .rom.003c5a3f, "a"
	.incbin "baserom.gba", 0x003c5a3f, 0x00000001
	.section .rom.003c885d, "a"
	.incbin "baserom.gba", 0x003c885d, 0x00000003
	.section .rom.003cbfa9, "a"
	.incbin "baserom.gba", 0x003cbfa9, 0x00000003
	.section .rom.003cd7bf, "a"
	.incbin "baserom.gba", 0x003cd7bf, 0x00000001
	.section .rom.003cddaa, "a"
	.incbin "baserom.gba", 0x003cddaa, 0x00000002
	.section .rom.003ce46e, "a"
	.incbin "baserom.gba", 0x003ce46e, 0x00000002
	.section .rom.003ce755, "a"
	.incbin "baserom.gba", 0x003ce755, 0x00000003
	.section .rom.003cfabb, "a"
	.incbin "baserom.gba", 0x003cfabb, 0x00000001
	.section .rom.003cfea5, "a"
	.incbin "baserom.gba", 0x003cfea5, 0x00000003
	.section .rom.003d0277, "a"
	.incbin "baserom.gba", 0x003d0277, 0x00000001
	.section .rom.003d0b17, "a"
	.incbin "baserom.gba", 0x003d0b17, 0x00000001
	.section .rom.003d0fba, "a"
	.incbin "baserom.gba", 0x003d0fba, 0x00000002
	.section .rom.003d2173, "a"
	.incbin "baserom.gba", 0x003d2173, 0x00000001
	.section .rom.003d3636, "a"
	.incbin "baserom.gba", 0x003d3636, 0x00000002
	.section .rom.003d45ff, "a"
	.incbin "baserom.gba", 0x003d45ff, 0x00000001
	.section .rom.003d485a, "a"
	.incbin "baserom.gba", 0x003d485a, 0x00000002
	.section .rom.003d62b5, "a"
	.incbin "baserom.gba", 0x003d62b5, 0x00000003
	.section .rom.003d6801, "a"
	.incbin "baserom.gba", 0x003d6801, 0x00000003
	.section .rom.003d858a, "a"
	.incbin "baserom.gba", 0x003d858a, 0x00000002
	.section .rom.003d8803, "a"
	.incbin "baserom.gba", 0x003d8803, 0x00000001
	.section .rom.003d8cca, "a"
	.incbin "baserom.gba", 0x003d8cca, 0x00000002
	.section .rom.003da89f, "a"
	.incbin "baserom.gba", 0x003da89f, 0x00000001
	.section .rom.003dc49d, "a"
	.incbin "baserom.gba", 0x003dc49d, 0x00000003
	.section .rom.003dc6be, "a"
	.incbin "baserom.gba", 0x003dc6be, 0x00000002
	.section .rom.003dcafb, "a"
	.incbin "baserom.gba", 0x003dcafb, 0x00000001
	.section .rom.003dcc0d, "a"
	.incbin "baserom.gba", 0x003dcc0d, 0x00000003
	.section .rom.003dd38b, "a"
	.incbin "baserom.gba", 0x003dd38b, 0x00000001
	.section .rom.003dd829, "a"
	.incbin "baserom.gba", 0x003dd829, 0x00000003
	.section .rom.003de53e, "a"
	.incbin "baserom.gba", 0x003de53e, 0x00000002
	.section .rom.003df2be, "a"
	.incbin "baserom.gba", 0x003df2be, 0x00000002
	.section .rom.003df64f, "a"
	.incbin "baserom.gba", 0x003df64f, 0x00000001
	.section .rom.003e1396, "a"
	.incbin "baserom.gba", 0x003e1396, 0x00000002
	.section .rom.003e216d, "a"
	.incbin "baserom.gba", 0x003e216d, 0x00000003
	.section .rom.003e299a, "a"
	.incbin "baserom.gba", 0x003e299a, 0x00000002
	.section .rom.003e3025, "a"
	.incbin "baserom.gba", 0x003e3025, 0x00000003
	.section .rom.003e31e3, "a"
	.incbin "baserom.gba", 0x003e31e3, 0x00000001
	.section .rom.003e3f3b, "a"
	.incbin "baserom.gba", 0x003e3f3b, 0x00000001
	.section .rom.003e4487, "a"
	.incbin "baserom.gba", 0x003e4487, 0x00000001
	.section .rom.003e4861, "a"
	.incbin "baserom.gba", 0x003e4861, 0x00000003
	.section .rom.003e4c0f, "a"
	.incbin "baserom.gba", 0x003e4c0f, 0x00000001
	.section .rom.003e6bb3, "a"
	.incbin "baserom.gba", 0x003e6bb3, 0x00000001
	.section .rom.003e794f, "a"
	.incbin "baserom.gba", 0x003e794f, 0x00000001
	.section .rom.003e7b6b, "a"
	.incbin "baserom.gba", 0x003e7b6b, 0x00000001
	.section .rom.003e7e67, "a"
	.incbin "baserom.gba", 0x003e7e67, 0x00000001
	.section .rom.003ea015, "a"
	.incbin "baserom.gba", 0x003ea015, 0x00000003
	.section .rom.003eade3, "a"
	.incbin "baserom.gba", 0x003eade3, 0x00000001
	.section .rom.003ebf45, "a"
	.incbin "baserom.gba", 0x003ebf45, 0x00000003
	.section .rom.003ec49f, "a"
	.incbin "baserom.gba", 0x003ec49f, 0x00000001
	.section .rom.003edd89, "a"
	.incbin "baserom.gba", 0x003edd89, 0x00000003
	.section .rom.003ee62a, "a"
	.incbin "baserom.gba", 0x003ee62a, 0x00000002
	.section .rom.003eea07, "a"
	.incbin "baserom.gba", 0x003eea07, 0x00000001
	.section .rom.003eec91, "a"
	.incbin "baserom.gba", 0x003eec91, 0x00000003
	.section .rom.003ef037, "a"
	.incbin "baserom.gba", 0x003ef037, 0x00000001
	.section .rom.003ef292, "a"
	.incbin "baserom.gba", 0x003ef292, 0x00000002
	.section .rom.003ef64a, "a"
	.incbin "baserom.gba", 0x003ef64a, 0x00000002
	.section .rom.003f0b33, "a"
	.incbin "baserom.gba", 0x003f0b33, 0x00000001
	.section .rom.003f20f6, "a"
	.incbin "baserom.gba", 0x003f20f6, 0x00000a6e
	.section .rom.003f393a, "a"
	.incbin "baserom.gba", 0x003f393a, 0x00000002
	.section .rom.003f3cbd, "a"
	.incbin "baserom.gba", 0x003f3cbd, 0x00000003
	.section .rom.003f496d, "a"
	.incbin "baserom.gba", 0x003f496d, 0x00000003
	.section .rom.003f54b5, "a"
	.incbin "baserom.gba", 0x003f54b5, 0x00000003
	.section .rom.003f564e, "a"
	.incbin "baserom.gba", 0x003f564e, 0x00000002
	.section .rom.003f5edb, "a"
	.incbin "baserom.gba", 0x003f5edb, 0x00000001
	.section .rom.003f69a2, "a"
	.incbin "baserom.gba", 0x003f69a2, 0x00000002
	.section .rom.003f6eba, "a"
	.incbin "baserom.gba", 0x003f6eba, 0x00000002
	.section .rom.003f74de, "a"
	.incbin "baserom.gba", 0x003f74de, 0x00000002
	.section .rom.003f7d45, "a"
	.incbin "baserom.gba", 0x003f7d45, 0x00000003
	.section .rom.003f8169, "a"
	.incbin "baserom.gba", 0x003f8169, 0x00000003
	.section .rom.003f8403, "a"
	.incbin "baserom.gba", 0x003f8403, 0x00000001
	.section .rom.003f9d9f, "a"
	.incbin "baserom.gba", 0x003f9d9f, 0x00000001
	.section .rom.003fbb16, "a"
	.incbin "baserom.gba", 0x003fbb16, 0x00000002
	.section .rom.003fda8b, "a"
	.incbin "baserom.gba", 0x003fda8b, 0x00000001
	.section .rom.003fdf5b, "a"
	.incbin "baserom.gba", 0x003fdf5b, 0x00000001
	.section .rom.003fe5ed, "a"
	.incbin "baserom.gba", 0x003fe5ed, 0x00000a37
	.section .rom.004003f5, "a"
	.incbin "baserom.gba", 0x004003f5, 0x00000003
	.section .rom.00400ec6, "a"
	.incbin "baserom.gba", 0x00400ec6, 0x00000002
	.section .rom.00401a03, "a"
	.incbin "baserom.gba", 0x00401a03, 0x00000001
	.section .rom.00402043, "a"
	.incbin "baserom.gba", 0x00402043, 0x00001589
	.section .rom.0040362d, "a"
	.incbin "baserom.gba", 0x0040362d, 0x00000003
	.section .rom.0040399b, "a"
	.incbin "baserom.gba", 0x0040399b, 0x00000001
	.section .rom.00403f39, "a"
	.incbin "baserom.gba", 0x00403f39, 0x00000003
	.section .rom.0040426d, "a"
	.incbin "baserom.gba", 0x0040426d, 0x00000003
	.section .rom.004045a9, "a"
	.incbin "baserom.gba", 0x004045a9, 0x00000003
	.section .rom.004055c2, "a"
	.incbin "baserom.gba", 0x004055c2, 0x00000002
	.section .rom.00408bce, "a"
	.incbin "baserom.gba", 0x00408bce, 0x00000002
	.section .rom.00409371, "a"
	.incbin "baserom.gba", 0x00409371, 0x00000003
	.section .rom.0040a703, "a"
	.incbin "baserom.gba", 0x0040a703, 0x00000001
	.section .rom.0040ab33, "a"
	.incbin "baserom.gba", 0x0040ab33, 0x00000001
	.section .rom.0040c929, "a"
	.incbin "baserom.gba", 0x0040c929, 0x00000003
	.section .rom.0040d23e, "a"
	.incbin "baserom.gba", 0x0040d23e, 0x00000002
	.section .rom.0040ed72, "a"
	.incbin "baserom.gba", 0x0040ed72, 0x00000002
	.section .rom.0040fdc1, "a"
	.incbin "baserom.gba", 0x0040fdc1, 0x00000003
	.section .rom.00410477, "a"
	.incbin "baserom.gba", 0x00410477, 0x00000001
	.section .rom.0041151d, "a"
	.incbin "baserom.gba", 0x0041151d, 0x00000003
	.section .rom.0042492f, "a"
	.incbin "baserom.gba", 0x0042492f, 0x00000001
	.section .rom.00424a81, "a"
	.incbin "baserom.gba", 0x00424a81, 0x00000003
	.section .rom.00424f12, "a"
	.incbin "baserom.gba", 0x00424f12, 0x00000002
	.section .rom.00425109, "a"
	.incbin "baserom.gba", 0x00425109, 0x00000003
	.section .rom.004267c6, "a"
	.incbin "baserom.gba", 0x004267c6, 0x00000002
	.section .rom.00427ce5, "a"
	.incbin "baserom.gba", 0x00427ce5, 0x00000003
	.section .rom.004288b1, "a"
	.incbin "baserom.gba", 0x004288b1, 0x00000003
	.section .rom.00429426, "a"
	.incbin "baserom.gba", 0x00429426, 0x00000002
	.section .rom.0042b2cf, "a"
	.incbin "baserom.gba", 0x0042b2cf, 0x00000001
	.section .rom.0042ca5d, "a"
	.incbin "baserom.gba", 0x0042ca5d, 0x00000003
	.section .rom.0042df0e, "a"
	.incbin "baserom.gba", 0x0042df0e, 0x00000002
	.section .rom.0043065b, "a"
	.incbin "baserom.gba", 0x0043065b, 0x00000001
	.section .rom.00431ef5, "a"
	.incbin "baserom.gba", 0x00431ef5, 0x00000003
	.section .rom.004334be, "a"
	.incbin "baserom.gba", 0x004334be, 0x00000002
	.section .rom.00434f4e, "a"
	.incbin "baserom.gba", 0x00434f4e, 0x00000002
	.section .rom.0043f536, "a"
	.incbin "baserom.gba", 0x0043f536, 0x00000002
	.section .rom.004416ea, "a"
	.incbin "baserom.gba", 0x004416ea, 0x00000002
	.section .rom.00445851, "a"
	.incbin "baserom.gba", 0x00445851, 0x00000003
	.section .rom.00448a76, "a"
	.incbin "baserom.gba", 0x00448a76, 0x00000002
	.section .rom.0044a17a, "a"
	.incbin "baserom.gba", 0x0044a17a, 0x00000002
	.section .rom.00450d62, "a"
	.incbin "baserom.gba", 0x00450d62, 0x00000002
	.section .rom.004596e6, "a"
	.incbin "baserom.gba", 0x004596e6, 0x00000002
	.section .rom.004611c2, "a"
	.incbin "baserom.gba", 0x004611c2, 0x00000002
	.section .rom.00464256, "a"
	.incbin "baserom.gba", 0x00464256, 0x00000002
	.section .rom.00467665, "a"
	.incbin "baserom.gba", 0x00467665, 0x00000003
	.section .rom.00469ed1, "a"
	.incbin "baserom.gba", 0x00469ed1, 0x00000003
	.section .rom.0046b9c2, "a"
	.incbin "baserom.gba", 0x0046b9c2, 0x00000002
	.section .rom.0046c7da, "a"
	.incbin "baserom.gba", 0x0046c7da, 0x00000002
	.section .rom.0046d4c5, "a"
	.incbin "baserom.gba", 0x0046d4c5, 0x00000003
	.section .rom.00476982, "a"
	.incbin "baserom.gba", 0x00476982, 0x00000002
	.section .rom.0047768d, "a"
	.incbin "baserom.gba", 0x0047768d, 0x00000003
	.section .rom.004797a1, "a"
	.incbin "baserom.gba", 0x004797a1, 0x00000003
	.section .rom.0047a413, "a"
	.incbin "baserom.gba", 0x0047a413, 0x00000001
	.section .rom.0047b02b, "a"
	.incbin "baserom.gba", 0x0047b02b, 0x00000001
	.section .rom.0047b79f, "a"
	.incbin "baserom.gba", 0x0047b79f, 0x00000001
	.section .rom.0047d02f, "a"
	.incbin "baserom.gba", 0x0047d02f, 0x00000001
	.section .rom.0047d9fd, "a"
	.incbin "baserom.gba", 0x0047d9fd, 0x00000003
	.section .rom.0047e61b, "a"
	.incbin "baserom.gba", 0x0047e61b, 0x00000001
	.section .rom.00480ccf, "a"
	.incbin "baserom.gba", 0x00480ccf, 0x00000001
	.section .rom.004833e2, "a"
	.incbin "baserom.gba", 0x004833e2, 0x00000002
	.section .rom.00488337, "a"
	.incbin "baserom.gba", 0x00488337, 0x00000001
	.section .rom.0048c661, "a"
	.incbin "baserom.gba", 0x0048c661, 0x00000003
	.section .rom.0048d24e, "a"
	.incbin "baserom.gba", 0x0048d24e, 0x00000002
	.section .rom.00491e03, "a"
	.incbin "baserom.gba", 0x00491e03, 0x00000001
	.section .rom.0049416d, "a"
	.incbin "baserom.gba", 0x0049416d, 0x00000003
	.section .rom.00495b0f, "a"
	.incbin "baserom.gba", 0x00495b0f, 0x00000001
	.section .rom.0049a3d1, "a"
	.incbin "baserom.gba", 0x0049a3d1, 0x00000003
	.section .rom.0049e499, "a"
	.incbin "baserom.gba", 0x0049e499, 0x00000003
	.section .rom.004a1b62, "a"
	.incbin "baserom.gba", 0x004a1b62, 0x00000002
	.section .rom.004a9b33, "a"
	.incbin "baserom.gba", 0x004a9b33, 0x00000001
	.section .rom.004b0e43, "a"
	.incbin "baserom.gba", 0x004b0e43, 0x00000001
	.section .rom.004b5dc3, "a"
	.incbin "baserom.gba", 0x004b5dc3, 0x00000001
	.section .rom.004ba773, "a"
	.incbin "baserom.gba", 0x004ba773, 0x0000051d
	.section .rom.004c009f, "a"
	.incbin "baserom.gba", 0x004c009f, 0x00000001
	.section .rom.004c022d, "a"
	.incbin "baserom.gba", 0x004c022d, 0x00000003
	.section .rom.004c528d, "a"
	.incbin "baserom.gba", 0x004c528d, 0x00000003
	.section .rom.004c97f9, "a"
	.incbin "baserom.gba", 0x004c97f9, 0x00000003
	.section .rom.004ce9bd, "a"
	.incbin "baserom.gba", 0x004ce9bd, 0x00000003
	.section .rom.004ceb53, "a"
	.incbin "baserom.gba", 0x004ceb53, 0x00000001
	.section .rom.004d17a7, "a"
	.incbin "baserom.gba", 0x004d17a7, 0x00000001
	.section .rom.004d845b, "a"
	.incbin "baserom.gba", 0x004d845b, 0x00000001
	.section .rom.004db596, "a"
	.incbin "baserom.gba", 0x004db596, 0x00000002
	.section .rom.004db717, "a"
	.incbin "baserom.gba", 0x004db717, 0x00000001
	.section .rom.004dded1, "a"
	.incbin "baserom.gba", 0x004dded1, 0x00000003
	.section .rom.004e051a, "a"
	.incbin "baserom.gba", 0x004e051a, 0x00000002
	.section .rom.004e2bfe, "a"
	.incbin "baserom.gba", 0x004e2bfe, 0x00000002
	.section .rom.004e3c73, "a"
	.incbin "baserom.gba", 0x004e3c73, 0x00000001
	.section .rom.004e5dc6, "a"
	.incbin "baserom.gba", 0x004e5dc6, 0x00000002
	.section .rom.004e5f59, "a"
	.incbin "baserom.gba", 0x004e5f59, 0x00000003
	.section .rom.004e879b, "a"
	.incbin "baserom.gba", 0x004e879b, 0x00000001
	.section .rom.004ea5b1, "a"
	.incbin "baserom.gba", 0x004ea5b1, 0x00000003
	.section .rom.004ecc92, "a"
	.incbin "baserom.gba", 0x004ecc92, 0x00000002
	.section .rom.004eddd9, "a"
	.incbin "baserom.gba", 0x004eddd9, 0x00000003
	.section .rom.004f0eed, "a"
	.incbin "baserom.gba", 0x004f0eed, 0x00000003
	.section .rom.004f5055, "a"
	.incbin "baserom.gba", 0x004f5055, 0x00000003
	.section .rom.004f6652, "a"
	.incbin "baserom.gba", 0x004f6652, 0x00000002
	.section .rom.004f791b, "a"
	.incbin "baserom.gba", 0x004f791b, 0x00000001
	.section .rom.004fad33, "a"
	.incbin "baserom.gba", 0x004fad33, 0x00000001
	.section .rom.004faec1, "a"
	.incbin "baserom.gba", 0x004faec1, 0x00000003
	.section .rom.004fcc72, "a"
	.incbin "baserom.gba", 0x004fcc72, 0x00000002
	.section .rom.005007e9, "a"
	.incbin "baserom.gba", 0x005007e9, 0x00000003
	.section .rom.00501cf7, "a"
	.incbin "baserom.gba", 0x00501cf7, 0x00000001
	.section .rom.005028cb, "a"
	.incbin "baserom.gba", 0x005028cb, 0x00000001
	.section .rom.005029c9, "a"
	.incbin "baserom.gba", 0x005029c9, 0x00000003
	.section .rom.00503dae, "a"
	.incbin "baserom.gba", 0x00503dae, 0x00000002
	.section .rom.00504f31, "a"
	.incbin "baserom.gba", 0x00504f31, 0x00000003
	.section .rom.005058ba, "a"
	.incbin "baserom.gba", 0x005058ba, 0x00000002
	.section .rom.00508537, "a"
	.incbin "baserom.gba", 0x00508537, 0x00000001
	.section .rom.0050861a, "a"
	.incbin "baserom.gba", 0x0050861a, 0x00000002
	.section .rom.00509805, "a"
	.incbin "baserom.gba", 0x00509805, 0x00000003
	.section .rom.0050b48d, "a"
	.incbin "baserom.gba", 0x0050b48d, 0x00000003
	.section .rom.0050b99b, "a"
	.incbin "baserom.gba", 0x0050b99b, 0x00000001
	.section .rom.0050badb, "a"
	.incbin "baserom.gba", 0x0050badb, 0x00000001
	.section .rom.0050ccf6, "a"
	.incbin "baserom.gba", 0x0050ccf6, 0x00000002
	.section .rom.0050ce51, "a"
	.incbin "baserom.gba", 0x0050ce51, 0x00000003
	.section .rom.00511277, "a"
	.incbin "baserom.gba", 0x00511277, 0x00000001
	.section .rom.00512fa6, "a"
	.incbin "baserom.gba", 0x00512fa6, 0x00000002
	.section .rom.00513d1f, "a"
	.incbin "baserom.gba", 0x00513d1f, 0x00000001
	.section .rom.00513e53, "a"
	.incbin "baserom.gba", 0x00513e53, 0x00000001
	.section .rom.005162d6, "a"
	.incbin "baserom.gba", 0x005162d6, 0x00000002
	.section .rom.00517053, "a"
	.incbin "baserom.gba", 0x00517053, 0x00000001
	.section .rom.00517b32, "a"
	.incbin "baserom.gba", 0x00517b32, 0x00000002
	.section .rom.005189aa, "a"
	.incbin "baserom.gba", 0x005189aa, 0x00000002
	.section .rom.0051aa7e, "a"
	.incbin "baserom.gba", 0x0051aa7e, 0x00000002
	.section .rom.0051b3c5, "a"
	.incbin "baserom.gba", 0x0051b3c5, 0x00000003
	.section .rom.0051c012, "a"
	.incbin "baserom.gba", 0x0051c012, 0x00000002
	.section .rom.0051c239, "a"
	.incbin "baserom.gba", 0x0051c239, 0x00000003
	.section .rom.0051db7b, "a"
	.incbin "baserom.gba", 0x0051db7b, 0x00000001
	.section .rom.0051dc8f, "a"
	.incbin "baserom.gba", 0x0051dc8f, 0x00000001
	.section .rom.0051f655, "a"
	.incbin "baserom.gba", 0x0051f655, 0x00000003
	.section .rom.0051f79f, "a"
	.incbin "baserom.gba", 0x0051f79f, 0x00000001
	.section .rom.0052186b, "a"
	.incbin "baserom.gba", 0x0052186b, 0x00000001
	.section .rom.00521993, "a"
	.incbin "baserom.gba", 0x00521993, 0x00000001
	.section .rom.0052321b, "a"
	.incbin "baserom.gba", 0x0052321b, 0x00000001
	.section .rom.005254bd, "a"
	.incbin "baserom.gba", 0x005254bd, 0x00000003
	.section .rom.005255ff, "a"
	.incbin "baserom.gba", 0x005255ff, 0x00000001
	.section .rom.00527607, "a"
	.incbin "baserom.gba", 0x00527607, 0x00000001
	.section .rom.00527712, "a"
	.incbin "baserom.gba", 0x00527712, 0x00000002
	.section .rom.005296f6, "a"
	.incbin "baserom.gba", 0x005296f6, 0x00000002
	.section .rom.0052ae1b, "a"
	.incbin "baserom.gba", 0x0052ae1b, 0x00000001
	.section .rom.0052becf, "a"
	.incbin "baserom.gba", 0x0052becf, 0x00000001
	.section .rom.0052dbb6, "a"
	.incbin "baserom.gba", 0x0052dbb6, 0x00000002
	.section .rom.0052dd11, "a"
	.incbin "baserom.gba", 0x0052dd11, 0x00000003
	.section .rom.0052fc5a, "a"
	.incbin "baserom.gba", 0x0052fc5a, 0x00000002
	.section .rom.0052fddf, "a"
	.incbin "baserom.gba", 0x0052fddf, 0x00000001
	.section .rom.00532931, "a"
	.incbin "baserom.gba", 0x00532931, 0x00000003
	.section .rom.00534ff6, "a"
	.incbin "baserom.gba", 0x00534ff6, 0x00000002
	.section .rom.0053774e, "a"
	.incbin "baserom.gba", 0x0053774e, 0x00000002
	.section .rom.00538fa1, "a"
	.incbin "baserom.gba", 0x00538fa1, 0x00000003
	.section .rom.0053b6a1, "a"
	.incbin "baserom.gba", 0x0053b6a1, 0x00000003
	.section .rom.0053d46d, "a"
	.incbin "baserom.gba", 0x0053d46d, 0x00000003
	.section .rom.0053f9b7, "a"
	.incbin "baserom.gba", 0x0053f9b7, 0x00000001
	.section .rom.00540c82, "a"
	.incbin "baserom.gba", 0x00540c82, 0x00000002
	.section .rom.00541933, "a"
	.incbin "baserom.gba", 0x00541933, 0x00000001
	.section .rom.0054239d, "a"
	.incbin "baserom.gba", 0x0054239d, 0x00000003
	.section .rom.00542493, "a"
	.incbin "baserom.gba", 0x00542493, 0x00000001
	.section .rom.00543ef2, "a"
	.incbin "baserom.gba", 0x00543ef2, 0x00000002
	.section .rom.00544c96, "a"
	.incbin "baserom.gba", 0x00544c96, 0x00000002
	.section .rom.005467cf, "a"
	.incbin "baserom.gba", 0x005467cf, 0x00000001
	.section .rom.005496bd, "a"
	.incbin "baserom.gba", 0x005496bd, 0x00000003
	.section .rom.00550ea3, "a"
	.incbin "baserom.gba", 0x00550ea3, 0x00000001
	.section .rom.00550f6f, "a"
	.incbin "baserom.gba", 0x00550f6f, 0x00000001
	.section .rom.00556731, "a"
	.incbin "baserom.gba", 0x00556731, 0x00000003
	.section .rom.00556873, "a"
	.incbin "baserom.gba", 0x00556873, 0x00000001
	.section .rom.00557ca1, "a"
	.incbin "baserom.gba", 0x00557ca1, 0x00000003
	.section .rom.0055a9de, "a"
	.incbin "baserom.gba", 0x0055a9de, 0x00000002
	.section .rom.0055cada, "a"
	.incbin "baserom.gba", 0x0055cada, 0x00000002
	.section .rom.005644e6, "a"
	.incbin "baserom.gba", 0x005644e6, 0x00000002
	.section .rom.00564682, "a"
	.incbin "baserom.gba", 0x00564682, 0x00000002
	.section .rom.00568f1d, "a"
	.incbin "baserom.gba", 0x00568f1d, 0x00000003
	.section .rom.0056b41e, "a"
	.incbin "baserom.gba", 0x0056b41e, 0x00000002
	.section .rom.00572e0f, "a"
	.incbin "baserom.gba", 0x00572e0f, 0x00000001
	.section .rom.00572faa, "a"
	.incbin "baserom.gba", 0x00572faa, 0x00000002
	.section .rom.00575b59, "a"
	.incbin "baserom.gba", 0x00575b59, 0x00000003
	.section .rom.00577c2d, "a"
	.incbin "baserom.gba", 0x00577c2d, 0x00000003
	.section .rom.00579c86, "a"
	.incbin "baserom.gba", 0x00579c86, 0x00001cee
	.section .rom.0057d11d, "a"
	.incbin "baserom.gba", 0x0057d11d, 0x00000003
	.section .rom.0057fdf9, "a"
	.incbin "baserom.gba", 0x0057fdf9, 0x00000003
	.section .rom.00581bcb, "a"
	.incbin "baserom.gba", 0x00581bcb, 0x00000001
	.section .rom.00581d0b, "a"
	.incbin "baserom.gba", 0x00581d0b, 0x00000001
	.section .rom.00584d65, "a"
	.incbin "baserom.gba", 0x00584d65, 0x00000003
	.section .rom.00587ce2, "a"
	.incbin "baserom.gba", 0x00587ce2, 0x00000002
	.section .rom.0058a5a9, "a"
	.incbin "baserom.gba", 0x0058a5a9, 0x00000003
	.section .rom.00591511, "a"
	.incbin "baserom.gba", 0x00591511, 0x00000003
	.section .rom.00592db7, "a"
	.incbin "baserom.gba", 0x00592db7, 0x00000001
	.section .rom.005936b6, "a"
	.incbin "baserom.gba", 0x005936b6, 0x00000002
	.section .rom.0059442f, "a"
	.incbin "baserom.gba", 0x0059442f, 0x00000001
	.section .rom.0059458f, "a"
	.incbin "baserom.gba", 0x0059458f, 0x00000001
	.section .rom.005961e7, "a"
	.incbin "baserom.gba", 0x005961e7, 0x00000001
	.section .rom.00597a7f, "a"
	.incbin "baserom.gba", 0x00597a7f, 0x00000001
	.section .rom.00597c03, "a"
	.incbin "baserom.gba", 0x00597c03, 0x00000001
	.section .rom.0059a73f, "a"
	.incbin "baserom.gba", 0x0059a73f, 0x00000001
	.section .rom.0059ceb9, "a"
	.incbin "baserom.gba", 0x0059ceb9, 0x00000003
	.section .rom.0059dea6, "a"
	.incbin "baserom.gba", 0x0059dea6, 0x00000002
	.section .rom.0059f567, "a"
	.incbin "baserom.gba", 0x0059f567, 0x00000001
	.section .rom.0059f711, "a"
	.incbin "baserom.gba", 0x0059f711, 0x00000003
	.section .rom.005a24a5, "a"
	.incbin "baserom.gba", 0x005a24a5, 0x00000003
	.section .rom.005a653a, "a"
	.incbin "baserom.gba", 0x005a653a, 0x00000002
	.section .rom.005a667b, "a"
	.incbin "baserom.gba", 0x005a667b, 0x00000001
	.section .rom.005a8543, "a"
	.incbin "baserom.gba", 0x005a8543, 0x00000001
	.section .rom.005a86e5, "a"
	.incbin "baserom.gba", 0x005a86e5, 0x00000003
	.section .rom.005aa787, "a"
	.incbin "baserom.gba", 0x005aa787, 0x00000001
	.section .rom.005ab714, "a"
	.incbin "baserom.gba", 0x005ab714, 0x000022d8
	.section .rom.005b04dd, "a"
	.incbin "baserom.gba", 0x005b04dd, 0x00000003
	.section .rom.005b0662, "a"
	.incbin "baserom.gba", 0x005b0662, 0x00000002
	.section .rom.005b7daf, "a"
	.incbin "baserom.gba", 0x005b7daf, 0x00000001
	.section .rom.005b82b9, "a"
	.incbin "baserom.gba", 0x005b82b9, 0x00000003
	.section .rom.005b9991, "a"
	.incbin "baserom.gba", 0x005b9991, 0x00000003
	.section .rom.005b9b35, "a"
	.incbin "baserom.gba", 0x005b9b35, 0x00000003
	.section .rom.005bb43a, "a"
	.incbin "baserom.gba", 0x005bb43a, 0x00000002
	.section .rom.005bcc67, "a"
	.incbin "baserom.gba", 0x005bcc67, 0x00000001
	.section .rom.005bcde5, "a"
	.incbin "baserom.gba", 0x005bcde5, 0x00000003
	.section .rom.005c16d9, "a"
	.incbin "baserom.gba", 0x005c16d9, 0x00000003
	.section .rom.005c271e, "a"
	.incbin "baserom.gba", 0x005c271e, 0x00000002
	.section .rom.005c3542, "a"
	.incbin "baserom.gba", 0x005c3542, 0x00000002
	.section .rom.005c4887, "a"
	.incbin "baserom.gba", 0x005c4887, 0x00000001
	.section .rom.005c49e1, "a"
	.incbin "baserom.gba", 0x005c49e1, 0x00000003
	.section .rom.005c6f9f, "a"
	.incbin "baserom.gba", 0x005c6f9f, 0x00000001
	.section .rom.005c7116, "a"
	.incbin "baserom.gba", 0x005c7116, 0x00000002
	.section .rom.005cba09, "a"
	.incbin "baserom.gba", 0x005cba09, 0x00000003
	.section .rom.005cc471, "a"
	.incbin "baserom.gba", 0x005cc471, 0x00000003
	.section .rom.005ceab3, "a"
	.incbin "baserom.gba", 0x005ceab3, 0x00000001
	.section .rom.005cec1a, "a"
	.incbin "baserom.gba", 0x005cec1a, 0x00000002
	.section .rom.005cfd61, "a"
	.incbin "baserom.gba", 0x005cfd61, 0x00000003
	.section .rom.005d1abf, "a"
	.incbin "baserom.gba", 0x005d1abf, 0x00000001
	.section .rom.005d1c26, "a"
	.incbin "baserom.gba", 0x005d1c26, 0x00000002
	.section .rom.005d44b5, "a"
	.incbin "baserom.gba", 0x005d44b5, 0x00000003
	.section .rom.005d45ea, "a"
	.incbin "baserom.gba", 0x005d45ea, 0x00000002
	.section .rom.005d7226, "a"
	.incbin "baserom.gba", 0x005d7226, 0x00000002
	.section .rom.005da9c5, "a"
	.incbin "baserom.gba", 0x005da9c5, 0x00000003
	.section .rom.005ddd13, "a"
	.incbin "baserom.gba", 0x005ddd13, 0x00000001
	.section .rom.005dde8f, "a"
	.incbin "baserom.gba", 0x005dde8f, 0x00000001
	.section .rom.005e0a3e, "a"
	.incbin "baserom.gba", 0x005e0a3e, 0x00000002
	.section .rom.005e2cb2, "a"
	.incbin "baserom.gba", 0x005e2cb2, 0x00000002
	.section .rom.005e5523, "a"
	.incbin "baserom.gba", 0x005e5523, 0x00000001
	.section .rom.005e8a17, "a"
	.incbin "baserom.gba", 0x005e8a17, 0x00000001
	.section .rom.005ed63e, "a"
	.incbin "baserom.gba", 0x005ed63e, 0x00000002
	.section .rom.005ee2c1, "a"
	.incbin "baserom.gba", 0x005ee2c1, 0x00000003
	.section .rom.005ee403, "a"
	.incbin "baserom.gba", 0x005ee403, 0x00000001
	.section .rom.005ef973, "a"
	.incbin "baserom.gba", 0x005ef973, 0x00000001
	.section .rom.005efade, "a"
	.incbin "baserom.gba", 0x005efade, 0x00000002
	.section .rom.005f1839, "a"
	.incbin "baserom.gba", 0x005f1839, 0x00000003
	.section .rom.005f197b, "a"
	.incbin "baserom.gba", 0x005f197b, 0x00000001
	.section .rom.005f417a, "a"
	.incbin "baserom.gba", 0x005f417a, 0x00000002
	.section .rom.005f42bb, "a"
	.incbin "baserom.gba", 0x005f42bb, 0x00000001
	.section .rom.005f5cd3, "a"
	.incbin "baserom.gba", 0x005f5cd3, 0x00000001
	.section .rom.005f724e, "a"
	.incbin "baserom.gba", 0x005f724e, 0x00000002
	.section .rom.005f878a, "a"
	.incbin "baserom.gba", 0x005f878a, 0x00000002
	.section .rom.005f891a, "a"
	.incbin "baserom.gba", 0x005f891a, 0x00000002
	.section .rom.005fabd3, "a"
	.incbin "baserom.gba", 0x005fabd3, 0x00000001
	.section .rom.005fee1a, "a"
	.incbin "baserom.gba", 0x005fee1a, 0x00000002
	.section .rom.00600c11, "a"
	.incbin "baserom.gba", 0x00600c11, 0x00000003
	.section .rom.00603dbf, "a"
	.incbin "baserom.gba", 0x00603dbf, 0x00000001
	.section .rom.006063f3, "a"
	.incbin "baserom.gba", 0x006063f3, 0x00000001
	.section .rom.00608f4b, "a"
	.incbin "baserom.gba", 0x00608f4b, 0x00000001
	.section .rom.00609113, "a"
	.incbin "baserom.gba", 0x00609113, 0x00002905
	.section .rom.0060f51d, "a"
	.incbin "baserom.gba", 0x0060f51d, 0x00000003
	.section .rom.00610a8e, "a"
	.incbin "baserom.gba", 0x00610a8e, 0x00000002
	.section .rom.006132b5, "a"
	.incbin "baserom.gba", 0x006132b5, 0x00000003
	.section .rom.00613425, "a"
	.incbin "baserom.gba", 0x00613425, 0x00000003
	.section .rom.00615ce7, "a"
	.incbin "baserom.gba", 0x00615ce7, 0x00000001
	.section .rom.006184af, "a"
	.incbin "baserom.gba", 0x006184af, 0x00000001
	.section .rom.006197a9, "a"
	.incbin "baserom.gba", 0x006197a9, 0x00000003
	.section .rom.0061b0ef, "a"
	.incbin "baserom.gba", 0x0061b0ef, 0x00000001
	.section .rom.00626582, "a"
	.incbin "baserom.gba", 0x00626582, 0x00000002
	.section .rom.00626742, "a"
	.incbin "baserom.gba", 0x00626742, 0x00000002
	.section .rom.00629aa9, "a"
	.incbin "baserom.gba", 0x00629aa9, 0x00000003
	.section .rom.0062bf9a, "a"
	.incbin "baserom.gba", 0x0062bf9a, 0x00000002
	.section .rom.0062ed7b, "a"
	.incbin "baserom.gba", 0x0062ed7b, 0x00000001
	.section .rom.0062f915, "a"
	.incbin "baserom.gba", 0x0062f915, 0x00000003
	.section .rom.00630776, "a"
	.incbin "baserom.gba", 0x00630776, 0x00000002
	.section .rom.006308ce, "a"
	.incbin "baserom.gba", 0x006308ce, 0x00000002
	.section .rom.0063871b, "a"
	.incbin "baserom.gba", 0x0063871b, 0x00000001
	.section .rom.00638887, "a"
	.incbin "baserom.gba", 0x00638887, 0x00000001
	.section .rom.0063b623, "a"
	.incbin "baserom.gba", 0x0063b623, 0x00000001
	.section .rom.0063d7b2, "a"
	.incbin "baserom.gba", 0x0063d7b2, 0x00000002
	.section .rom.0063deba, "a"
	.incbin "baserom.gba", 0x0063deba, 0x00000002
	.section .rom.0063eace, "a"
	.incbin "baserom.gba", 0x0063eace, 0x00000002
	.section .rom.0064fba1, "a"
	.incbin "baserom.gba", 0x0064fba1, 0x00000003
	.section .rom.0064fd36, "a"
	.incbin "baserom.gba", 0x0064fd36, 0x00000002
	.section .rom.00651246, "a"
	.incbin "baserom.gba", 0x00651246, 0x00000002
	.section .rom.0065302b, "a"
	.incbin "baserom.gba", 0x0065302b, 0x00000001
	.section .rom.006540be, "a"
	.incbin "baserom.gba", 0x006540be, 0x00000002
	.section .rom.0065632e, "a"
	.incbin "baserom.gba", 0x0065632e, 0x00000002
	.section .rom.006564c5, "a"
	.incbin "baserom.gba", 0x006564c5, 0x00000003
	.section .rom.0065846b, "a"
	.incbin "baserom.gba", 0x0065846b, 0x00000001
	.section .rom.0065d242, "a"
	.incbin "baserom.gba", 0x0065d242, 0x00000002
	.section .rom.00660ca3, "a"
	.incbin "baserom.gba", 0x00660ca3, 0x00000001
	.section .rom.006639d9, "a"
	.incbin "baserom.gba", 0x006639d9, 0x00000003
	.section .rom.00665d21, "a"
	.incbin "baserom.gba", 0x00665d21, 0x00000003
	.section .rom.006674ff, "a"
	.incbin "baserom.gba", 0x006674ff, 0x00000001
	.section .rom.006676b1, "a"
	.incbin "baserom.gba", 0x006676b1, 0x00000003
	.section .rom.00678052, "a"
	.incbin "baserom.gba", 0x00678052, 0x00000002
	.section .rom.006781b1, "a"
	.incbin "baserom.gba", 0x006781b1, 0x00000003
	.section .rom.0067b90b, "a"
	.incbin "baserom.gba", 0x0067b90b, 0x00000001
	.section .rom.0067ba6e, "a"
	.incbin "baserom.gba", 0x0067ba6e, 0x00000002
	.section .rom.00681751, "a"
	.incbin "baserom.gba", 0x00681751, 0x00000003
	.section .rom.00684137, "a"
	.incbin "baserom.gba", 0x00684137, 0x00000001
	.section .rom.00686342, "a"
	.incbin "baserom.gba", 0x00686342, 0x00000002
	.section .rom.00686483, "a"
	.incbin "baserom.gba", 0x00686483, 0x00000001
	.section .rom.00687e9a, "a"
	.incbin "baserom.gba", 0x00687e9a, 0x00000002
	.section .rom.0068ec01, "a"
	.incbin "baserom.gba", 0x0068ec01, 0x00000003
	.section .rom.0068ed7f, "a"
	.incbin "baserom.gba", 0x0068ed7f, 0x00000001
	.section .rom.00690d29, "a"
	.incbin "baserom.gba", 0x00690d29, 0x00000003
	.section .rom.006934a2, "a"
	.incbin "baserom.gba", 0x006934a2, 0x00000002
	.section .rom.00694d09, "a"
	.incbin "baserom.gba", 0x00694d09, 0x00000003
	.section .rom.0069872a, "a"
	.incbin "baserom.gba", 0x0069872a, 0x00000002
	.section .rom.006988f3, "a"
	.incbin "baserom.gba", 0x006988f3, 0x00000001
	.section .rom.006a38db, "a"
	.incbin "baserom.gba", 0x006a38db, 0x00000001
	.section .rom.006a5a27, "a"
	.incbin "baserom.gba", 0x006a5a27, 0x00000001
	.section .rom.006a5bb1, "a"
	.incbin "baserom.gba", 0x006a5bb1, 0x00000003
	.section .rom.006a7636, "a"
	.incbin "baserom.gba", 0x006a7636, 0x00000002
	.section .rom.006aa66d, "a"
	.incbin "baserom.gba", 0x006aa66d, 0x00000003
	.section .rom.006aa833, "a"
	.incbin "baserom.gba", 0x006aa833, 0x00000001
	.section .rom.006ab767, "a"
	.incbin "baserom.gba", 0x006ab767, 0x00000001
	.section .rom.006ad81a, "a"
	.incbin "baserom.gba", 0x006ad81a, 0x00000002
	.section .rom.006ad9c1, "a"
	.incbin "baserom.gba", 0x006ad9c1, 0x00000003
	.section .rom.006b29eb, "a"
	.incbin "baserom.gba", 0x006b29eb, 0x00000001
	.section .rom.006b577b, "a"
	.incbin "baserom.gba", 0x006b577b, 0x00000001
	.section .rom.006b593b, "a"
	.incbin "baserom.gba", 0x006b593b, 0x00000001
	.section .rom.006b7e2f, "a"
	.incbin "baserom.gba", 0x006b7e2f, 0x00000001
	.section .rom.006b8001, "a"
	.incbin "baserom.gba", 0x006b8001, 0x00000003
	.section .rom.006ba092, "a"
	.incbin "baserom.gba", 0x006ba092, 0x00000002
	.section .rom.006bc192, "a"
	.incbin "baserom.gba", 0x006bc192, 0x00000002
	.section .rom.006bed46, "a"
	.incbin "baserom.gba", 0x006bed46, 0x00000002
	.section .rom.006bf833, "a"
	.incbin "baserom.gba", 0x006bf833, 0x00000001
	.section .rom.006bfa06, "a"
	.incbin "baserom.gba", 0x006bfa06, 0x00000002
	.section .rom.006c2335, "a"
	.incbin "baserom.gba", 0x006c2335, 0x00000003
	.section .rom.006c3acd, "a"
	.incbin "baserom.gba", 0x006c3acd, 0x00000003
	.section .rom.006c4d41, "a"
	.incbin "baserom.gba", 0x006c4d41, 0x00000003
	.section .rom.006c71d1, "a"
	.incbin "baserom.gba", 0x006c71d1, 0x00000003
	.section .rom.006c951d, "a"
	.incbin "baserom.gba", 0x006c951d, 0x00000003
	.section .rom.006ca707, "a"
	.incbin "baserom.gba", 0x006ca707, 0x00000001
	.section .rom.006ca88e, "a"
	.incbin "baserom.gba", 0x006ca88e, 0x00000002
	.section .rom.006cedda, "a"
	.incbin "baserom.gba", 0x006cedda, 0x00000002
	.section .rom.006cef2f, "a"
	.incbin "baserom.gba", 0x006cef2f, 0x00000001
	.section .rom.006cf06f, "a"
	.incbin "baserom.gba", 0x006cf06f, 0x00000001
	.section .rom.006cfd43, "a"
	.incbin "baserom.gba", 0x006cfd43, 0x00000001
	.section .rom.006cfec2, "a"
	.incbin "baserom.gba", 0x006cfec2, 0x00000002
	.section .rom.006d177b, "a"
	.incbin "baserom.gba", 0x006d177b, 0x00000001
	.section .rom.006d18fd, "a"
	.incbin "baserom.gba", 0x006d18fd, 0x00000003
	.section .rom.006d338f, "a"
	.incbin "baserom.gba", 0x006d338f, 0x00000001
	.section .rom.006d3509, "a"
	.incbin "baserom.gba", 0x006d3509, 0x00000003
	.section .rom.006d5247, "a"
	.incbin "baserom.gba", 0x006d5247, 0x00000001
	.section .rom.006d5373, "a"
	.incbin "baserom.gba", 0x006d5373, 0x00000001
	.section .rom.006d54e9, "a"
	.incbin "baserom.gba", 0x006d54e9, 0x00000003
	.section .rom.006d738f, "a"
	.incbin "baserom.gba", 0x006d738f, 0x00000001
	.section .rom.006d7515, "a"
	.incbin "baserom.gba", 0x006d7515, 0x00000003
	.section .rom.006d8473, "a"
	.incbin "baserom.gba", 0x006d8473, 0x00000001
	.section .rom.006d9723, "a"
	.incbin "baserom.gba", 0x006d9723, 0x00000001
	.section .rom.006d98de, "a"
	.incbin "baserom.gba", 0x006d98de, 0x00000002
	.section .rom.006dc382, "a"
	.incbin "baserom.gba", 0x006dc382, 0x00000002
	.section .rom.006dc4f5, "a"
	.incbin "baserom.gba", 0x006dc4f5, 0x00000003
	.section .rom.006dd8dd, "a"
	.incbin "baserom.gba", 0x006dd8dd, 0x00000003
	.section .rom.006e03bd, "a"
	.incbin "baserom.gba", 0x006e03bd, 0x00000003
	.section .rom.006e0567, "a"
	.incbin "baserom.gba", 0x006e0567, 0x00000001
	.section .rom.006e2962, "a"
	.incbin "baserom.gba", 0x006e2962, 0x00000002
	.section .rom.006e2af1, "a"
	.incbin "baserom.gba", 0x006e2af1, 0x00000003
	.section .rom.006e5d71, "a"
	.incbin "baserom.gba", 0x006e5d71, 0x00000003
	.section .rom.006e5f52, "a"
	.incbin "baserom.gba", 0x006e5f52, 0x00000002
	.section .rom.006e84e7, "a"
	.incbin "baserom.gba", 0x006e84e7, 0x00000001
	.section .rom.006e8676, "a"
	.incbin "baserom.gba", 0x006e8676, 0x00000002
	.section .rom.006eb26e, "a"
	.incbin "baserom.gba", 0x006eb26e, 0x00000002
	.section .rom.006ebdbb, "a"
	.incbin "baserom.gba", 0x006ebdbb, 0x00000001
	.section .rom.006ebf22, "a"
	.incbin "baserom.gba", 0x006ebf22, 0x00000002
	.section .rom.006ee937, "a"
	.incbin "baserom.gba", 0x006ee937, 0x00000001
	.section .rom.006f0576, "a"
	.incbin "baserom.gba", 0x006f0576, 0x00000002
	.section .rom.006f0a33, "a"
	.incbin "baserom.gba", 0x006f0a33, 0x00000001
	.section .rom.006f1dab, "a"
	.incbin "baserom.gba", 0x006f1dab, 0x00000001
	.section .rom.006f32ef, "a"
	.incbin "baserom.gba", 0x006f32ef, 0x00000001
	.section .rom.006f3479, "a"
	.incbin "baserom.gba", 0x006f3479, 0x00000003
	.section .rom.006f4557, "a"
	.incbin "baserom.gba", 0x006f4557, 0x00000001
	.section .rom.006f46e5, "a"
	.incbin "baserom.gba", 0x006f46e5, 0x00000003
	.section .rom.006f6339, "a"
	.incbin "baserom.gba", 0x006f6339, 0x00000003
	.section .rom.006f8d1b, "a"
	.incbin "baserom.gba", 0x006f8d1b, 0x00000001
	.section .rom.006fc245, "a"
	.incbin "baserom.gba", 0x006fc245, 0x00000003
	.section .rom.006fdb4f, "a"
	.incbin "baserom.gba", 0x006fdb4f, 0x00000001
	.section .rom.006ffbf7, "a"
	.incbin "baserom.gba", 0x006ffbf7, 0x00000001
	.section .rom.0070264b, "a"
	.incbin "baserom.gba", 0x0070264b, 0x00000001
	.section .rom.00704297, "a"
	.incbin "baserom.gba", 0x00704297, 0x00000001
	.section .rom.007056dd, "a"
	.incbin "baserom.gba", 0x007056dd, 0x00000003
	.section .rom.0070755d, "a"
	.incbin "baserom.gba", 0x0070755d, 0x00000003
	.section .rom.0070912e, "a"
	.incbin "baserom.gba", 0x0070912e, 0x00000002
	.section .rom.0070b862, "a"
	.incbin "baserom.gba", 0x0070b862, 0x00000002
	.section .rom.0070d4a7, "a"
	.incbin "baserom.gba", 0x0070d4a7, 0x00000001
	.section .rom.0070e8ed, "a"
	.incbin "baserom.gba", 0x0070e8ed, 0x00000003
	.section .rom.0071046b, "a"
	.incbin "baserom.gba", 0x0071046b, 0x00000001
	.section .rom.0071169b, "a"
	.incbin "baserom.gba", 0x0071169b, 0x00000001
	.section .rom.007117f5, "a"
	.incbin "baserom.gba", 0x007117f5, 0x00000003
	.section .rom.00714cfb, "a"
	.incbin "baserom.gba", 0x00714cfb, 0x00000001
	.section .rom.00714e66, "a"
	.incbin "baserom.gba", 0x00714e66, 0x00000002
	.section .rom.007177b6, "a"
	.incbin "baserom.gba", 0x007177b6, 0x00001166
	.section .rom.0071a101, "a"
	.incbin "baserom.gba", 0x0071a101, 0x00000003
	.section .rom.0071d347, "a"
	.incbin "baserom.gba", 0x0071d347, 0x00000001
	.section .rom.0071fc3e, "a"
	.incbin "baserom.gba", 0x0071fc3e, 0x00000002
	.section .rom.00728f77, "a"
	.incbin "baserom.gba", 0x00728f77, 0x00000001
	.section .rom.0072a2a6, "a"
	.incbin "baserom.gba", 0x0072a2a6, 0x00000002
	.section .rom.0072b49f, "a"
	.incbin "baserom.gba", 0x0072b49f, 0x00000001
	.section .rom.0072b601, "a"
	.incbin "baserom.gba", 0x0072b601, 0x00000003
	.section .rom.0072dfcd, "a"
	.incbin "baserom.gba", 0x0072dfcd, 0x00000003
	.section .rom.007314f9, "a"
	.incbin "baserom.gba", 0x007314f9, 0x00000003
	.section .rom.0073593b, "a"
	.incbin "baserom.gba", 0x0073593b, 0x00000001
	.section .rom.00735aae, "a"
	.incbin "baserom.gba", 0x00735aae, 0x00000002
	.section .rom.00739d13, "a"
	.incbin "baserom.gba", 0x00739d13, 0x00000001
	.section .rom.0073d52e, "a"
	.incbin "baserom.gba", 0x0073d52e, 0x00000002
	.section .rom.0073ef56, "a"
	.incbin "baserom.gba", 0x0073ef56, 0x00000002
	.section .rom.00741603, "a"
	.incbin "baserom.gba", 0x00741603, 0x00000001
	.section .rom.00743667, "a"
	.incbin "baserom.gba", 0x00743667, 0x00000001
	.section .rom.00745523, "a"
	.incbin "baserom.gba", 0x00745523, 0x00000001
	.section .rom.0074876d, "a"
	.incbin "baserom.gba", 0x0074876d, 0x00000003
	.section .rom.0074a133, "a"
	.incbin "baserom.gba", 0x0074a133, 0x00000001
	.section .rom.0074a2a5, "a"
	.incbin "baserom.gba", 0x0074a2a5, 0x00000003
	.section .rom.0074d2b2, "a"
	.incbin "baserom.gba", 0x0074d2b2, 0x00000002
	.section .rom.0074d46a, "a"
	.incbin "baserom.gba", 0x0074d46a, 0x00000002
	.section .rom.0074ecdb, "a"
	.incbin "baserom.gba", 0x0074ecdb, 0x00000001
	.section .rom.0074ee5a, "a"
	.incbin "baserom.gba", 0x0074ee5a, 0x00000002
	.section .rom.00752663, "a"
	.incbin "baserom.gba", 0x00752663, 0x00000001
	.section .rom.007527d7, "a"
	.incbin "baserom.gba", 0x007527d7, 0x00000001
	.section .rom.0075359f, "a"
	.incbin "baserom.gba", 0x0075359f, 0x00000001
	.section .rom.00753745, "a"
	.incbin "baserom.gba", 0x00753745, 0x00000003
	.section .rom.007557a0, "a"
	.incbin "baserom.gba", 0x007557a0, 0x000000e4
	.section .rom.0075859b, "a"
	.incbin "baserom.gba", 0x0075859b, 0x00000001
	.section .rom.00758782, "a"
	.incbin "baserom.gba", 0x00758782, 0x00000002
	.section .rom.0075ba1e, "a"
	.incbin "baserom.gba", 0x0075ba1e, 0x00004602
	.section .rom.00760126, "a"
	.incbin "baserom.gba", 0x00760126, 0x00000002
	.section .rom.00761cf9, "a"
	.incbin "baserom.gba", 0x00761cf9, 0x00000003
	.section .rom.007637a9, "a"
	.incbin "baserom.gba", 0x007637a9, 0x00000003
	.section .rom.00763905, "a"
	.incbin "baserom.gba", 0x00763905, 0x00000003
	.section .rom.00769b4b, "a"
	.incbin "baserom.gba", 0x00769b4b, 0x00000001
	.section .rom.00769c4f, "a"
	.incbin "baserom.gba", 0x00769c4f, 0x00000001
	.section .rom.0076a66b, "a"
	.incbin "baserom.gba", 0x0076a66b, 0x00000001
	.section .rom.0076a751, "a"
	.incbin "baserom.gba", 0x0076a751, 0x00000003
	.section .rom.0076a9ed, "a"
	.incbin "baserom.gba", 0x0076a9ed, 0x00000003
	.section .rom.0076ea4a, "a"
	.incbin "baserom.gba", 0x0076ea4a, 0x00000002
	.section .rom.0076effe, "a"
	.incbin "baserom.gba", 0x0076effe, 0x00000002
	.section .rom.0076ff93, "a"
	.incbin "baserom.gba", 0x0076ff93, 0x00000001
	.section .rom.00771996, "a"
	.incbin "baserom.gba", 0x00771996, 0x00000002
	.section .rom.00772605, "a"
	.incbin "baserom.gba", 0x00772605, 0x00000003
	.section .rom.007728ad, "a"
	.incbin "baserom.gba", 0x007728ad, 0x00000003
	.section .rom.00773732, "a"
	.incbin "baserom.gba", 0x00773732, 0x00000002
	.section .rom.007748c3, "a"
	.incbin "baserom.gba", 0x007748c3, 0x00000001
	.section .rom.00774a8b, "a"
	.incbin "baserom.gba", 0x00774a8b, 0x00000001
	.section .rom.00774dd7, "a"
	.incbin "baserom.gba", 0x00774dd7, 0x0000051d
	.section .rom.0077563f, "a"
	.incbin "baserom.gba", 0x0077563f, 0x0000051d
	.section .rom.00775ea7, "a"
	.incbin "baserom.gba", 0x00775ea7, 0x0000051d
	.section .rom.0077670f, "a"
	.incbin "baserom.gba", 0x0077670f, 0x0000051d
	.section .rom.00776f77, "a"
	.incbin "baserom.gba", 0x00776f77, 0x0000051d
	.section .rom.007777df, "a"
	.incbin "baserom.gba", 0x007777df, 0x0000051d
	.section .rom.00778047, "a"
	.incbin "baserom.gba", 0x00778047, 0x0000051d
	.section .rom.007788af, "a"
	.incbin "baserom.gba", 0x007788af, 0x00087751
