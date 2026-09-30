@ tbs-fr's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.incbin "baserom.gba", 0x00002cf4, 0x00005b8c
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	.incbin "baserom.gba", 0x00008880, 0x00000018
	.global ObjectDispatch_InitializeFar
	.thumb_func
ObjectDispatch_InitializeFar:
	.incbin "baserom.gba", 0x00008898, 0x00000030
	.global Object_CreateFar
	.thumb_func
Object_CreateFar:
	.incbin "baserom.gba", 0x000088c8, 0x00000af0
	.global Render_DecodeFrame
Render_DecodeFrame:
	.incbin "baserom.gba", 0x000093b8, 0x000002c4
	.global Render_DecodeFrameEnd
Render_DecodeFrameEnd:
	.incbin "baserom.gba", 0x0000967c, 0x0000b45c
	.global Tile_Decompress4bpp
Tile_Decompress4bpp:
	.incbin "baserom.gba", 0x00014ad8, 0x00000278
	.global Tile_Decompress4bppEnd
Tile_Decompress4bppEnd:
	.global Tile_ExpandMasked
Tile_ExpandMasked:
	.incbin "baserom.gba", 0x00014d50, 0x0000009c
	.global Tile_ExpandMaskedEnd
Tile_ExpandMaskedEnd:
	.global Tile_ExpandOpaque
Tile_ExpandOpaque:
	.incbin "baserom.gba", 0x00014dec, 0x0000007c
	.global Tile_ExpandOpaqueEnd
Tile_ExpandOpaqueEnd:
	.incbin "baserom.gba", 0x00014e68, 0x0001b9b4
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x0003081c, 0x00000ab0
	.section .rom.00032ecc, "a"
	.incbin "baserom.gba", 0x00032ecc, 0x00064214
	.global Object_GetById
	.thumb_func
Object_GetById:
	.incbin "baserom.gba", 0x000970e0, 0x0005bf44
	.global SentouKouka_Tenkai
SentouKouka_Tenkai:
	.incbin "baserom.gba", 0x000f3024, 0x00000230
	.global SentouKouka_TenkaiEnd
SentouKouka_TenkaiEnd:
	.incbin "baserom.gba", 0x000f3254, 0x00008c20
	.section .rom.000fbe74, "a"
	.incbin "baserom.gba", 0x000fbe74, 0x0000039e
	.global Mixer_CallViaR3
	.thumb_func
Mixer_CallViaR3:
	.incbin "baserom.gba", 0x000fc212, 0x00000006
	.section .rom.000fc218, "a"
	.incbin "baserom.gba", 0x000fc218, 0x00002060
	.section .rom.000fed04, "a"
	.incbin "baserom.gba", 0x000fed04, 0x00000090
	.section .rom.000fee24, "a"
	.incbin "baserom.gba", 0x000fee24, 0x00000060
	.section .rom.00186e98, "a"
	.incbin "baserom.gba", 0x00186e98, 0x0000098c
	.section .rom.003217d8, "a"
	.incbin "baserom.gba", 0x003217d8, 0x00000fd8
	.section .rom.003261e7, "a"
	.incbin "baserom.gba", 0x003261e7, 0x00000001
	.section .rom.0032c89b, "a"
	.incbin "baserom.gba", 0x0032c89b, 0x000086f9
	.section .rom.00336fe5, "a"
	.incbin "baserom.gba", 0x00336fe5, 0x00000003
	.section .rom.00338909, "a"
	.incbin "baserom.gba", 0x00338909, 0x00000003
	.section .rom.0033c40d, "a"
	.incbin "baserom.gba", 0x0033c40d, 0x000006ab
	.section .rom.00340efa, "a"
	.incbin "baserom.gba", 0x00340efa, 0x00000002
	.section .rom.0035166a, "a"
	.incbin "baserom.gba", 0x0035166a, 0x00000002
	.section .rom.00354c5a, "a"
	.incbin "baserom.gba", 0x00354c5a, 0x00000002
	.section .rom.003606fe, "a"
	.incbin "baserom.gba", 0x003606fe, 0x00000002
	.section .rom.00370dae, "a"
	.incbin "baserom.gba", 0x00370dae, 0x00000002
	.section .rom.0037884e, "a"
	.incbin "baserom.gba", 0x0037884e, 0x00000002
	.section .rom.0037c08e, "a"
	.incbin "baserom.gba", 0x0037c08e, 0x00000002
	.section .rom.003854c2, "a"
	.incbin "baserom.gba", 0x003854c2, 0x00000002
	.section .rom.003908ea, "a"
	.incbin "baserom.gba", 0x003908ea, 0x00000002
	.section .rom.003987a6, "a"
	.incbin "baserom.gba", 0x003987a6, 0x00000002
	.section .rom.0039d236, "a"
	.incbin "baserom.gba", 0x0039d236, 0x00000002
	.section .rom.003a14ea, "a"
	.incbin "baserom.gba", 0x003a14ea, 0x00000002
	.section .rom.003a4e6a, "a"
	.incbin "baserom.gba", 0x003a4e6a, 0x00000002
	.section .rom.003b16a6, "a"
	.incbin "baserom.gba", 0x003b16a6, 0x00000002
	.section .rom.003bffee, "a"
	.incbin "baserom.gba", 0x003bffee, 0x00000002
	.section .rom.003c58f1, "a"
	.incbin "baserom.gba", 0x003c58f1, 0x00000003
	.section .rom.003c7273, "a"
	.incbin "baserom.gba", 0x003c7273, 0x00000001
	.section .rom.003ca091, "a"
	.incbin "baserom.gba", 0x003ca091, 0x00000003
	.section .rom.003cd23b, "a"
	.incbin "baserom.gba", 0x003cd23b, 0x00000001
	.section .rom.003cda81, "a"
	.incbin "baserom.gba", 0x003cda81, 0x00000003
	.section .rom.003cf297, "a"
	.incbin "baserom.gba", 0x003cf297, 0x00000001
	.section .rom.003cf882, "a"
	.incbin "baserom.gba", 0x003cf882, 0x00000002
	.section .rom.003cff46, "a"
	.incbin "baserom.gba", 0x003cff46, 0x00000002
	.section .rom.003d022d, "a"
	.incbin "baserom.gba", 0x003d022d, 0x00000003
	.section .rom.003d1593, "a"
	.incbin "baserom.gba", 0x003d1593, 0x00000001
	.section .rom.003d197d, "a"
	.incbin "baserom.gba", 0x003d197d, 0x00000003
	.section .rom.003d1d4f, "a"
	.incbin "baserom.gba", 0x003d1d4f, 0x00000001
	.section .rom.003d25ef, "a"
	.incbin "baserom.gba", 0x003d25ef, 0x00000001
	.section .rom.003d2a92, "a"
	.incbin "baserom.gba", 0x003d2a92, 0x00000002
	.section .rom.003d3c4b, "a"
	.incbin "baserom.gba", 0x003d3c4b, 0x00000001
	.section .rom.003d510e, "a"
	.incbin "baserom.gba", 0x003d510e, 0x00000002
	.section .rom.003d60d7, "a"
	.incbin "baserom.gba", 0x003d60d7, 0x00000001
	.section .rom.003d6332, "a"
	.incbin "baserom.gba", 0x003d6332, 0x00000002
	.section .rom.003d7d8d, "a"
	.incbin "baserom.gba", 0x003d7d8d, 0x00000003
	.section .rom.003d82d9, "a"
	.incbin "baserom.gba", 0x003d82d9, 0x00000003
	.section .rom.003da062, "a"
	.incbin "baserom.gba", 0x003da062, 0x00000002
	.section .rom.003da2db, "a"
	.incbin "baserom.gba", 0x003da2db, 0x00000001
	.section .rom.003da7a2, "a"
	.incbin "baserom.gba", 0x003da7a2, 0x00000002
	.section .rom.003dc377, "a"
	.incbin "baserom.gba", 0x003dc377, 0x00000001
	.section .rom.003ddf75, "a"
	.incbin "baserom.gba", 0x003ddf75, 0x00000003
	.section .rom.003de196, "a"
	.incbin "baserom.gba", 0x003de196, 0x00000002
	.section .rom.003de5d3, "a"
	.incbin "baserom.gba", 0x003de5d3, 0x00000001
	.section .rom.003de6e5, "a"
	.incbin "baserom.gba", 0x003de6e5, 0x00000003
	.section .rom.003dee63, "a"
	.incbin "baserom.gba", 0x003dee63, 0x00000001
	.section .rom.003df301, "a"
	.incbin "baserom.gba", 0x003df301, 0x00000003
	.section .rom.003e0016, "a"
	.incbin "baserom.gba", 0x003e0016, 0x00000002
	.section .rom.003e0d96, "a"
	.incbin "baserom.gba", 0x003e0d96, 0x00000002
	.section .rom.003e1127, "a"
	.incbin "baserom.gba", 0x003e1127, 0x00000001
	.section .rom.003e2e6e, "a"
	.incbin "baserom.gba", 0x003e2e6e, 0x00000002
	.section .rom.003e3c45, "a"
	.incbin "baserom.gba", 0x003e3c45, 0x00000003
	.section .rom.003e4472, "a"
	.incbin "baserom.gba", 0x003e4472, 0x00000002
	.section .rom.003e4afd, "a"
	.incbin "baserom.gba", 0x003e4afd, 0x00000003
	.section .rom.003e4cbb, "a"
	.incbin "baserom.gba", 0x003e4cbb, 0x00000001
	.section .rom.003e5a13, "a"
	.incbin "baserom.gba", 0x003e5a13, 0x00000001
	.section .rom.003e5f5f, "a"
	.incbin "baserom.gba", 0x003e5f5f, 0x00000001
	.section .rom.003e6339, "a"
	.incbin "baserom.gba", 0x003e6339, 0x00000003
	.section .rom.003e66e7, "a"
	.incbin "baserom.gba", 0x003e66e7, 0x00000001
	.section .rom.003e868b, "a"
	.incbin "baserom.gba", 0x003e868b, 0x00000001
	.section .rom.003e9427, "a"
	.incbin "baserom.gba", 0x003e9427, 0x00000001
	.section .rom.003e9643, "a"
	.incbin "baserom.gba", 0x003e9643, 0x00000001
	.section .rom.003e993f, "a"
	.incbin "baserom.gba", 0x003e993f, 0x00000001
	.section .rom.003ebaed, "a"
	.incbin "baserom.gba", 0x003ebaed, 0x00000003
	.section .rom.003ec8bb, "a"
	.incbin "baserom.gba", 0x003ec8bb, 0x00000001
	.section .rom.003eda1d, "a"
	.incbin "baserom.gba", 0x003eda1d, 0x00000003
	.section .rom.003edf77, "a"
	.incbin "baserom.gba", 0x003edf77, 0x00000001
	.section .rom.003ef861, "a"
	.incbin "baserom.gba", 0x003ef861, 0x00000003
	.section .rom.003f0102, "a"
	.incbin "baserom.gba", 0x003f0102, 0x00000002
	.section .rom.003f04df, "a"
	.incbin "baserom.gba", 0x003f04df, 0x00000001
	.section .rom.003f0769, "a"
	.incbin "baserom.gba", 0x003f0769, 0x00000003
	.section .rom.003f0b0f, "a"
	.incbin "baserom.gba", 0x003f0b0f, 0x00000001
	.section .rom.003f0d6a, "a"
	.incbin "baserom.gba", 0x003f0d6a, 0x00000002
	.section .rom.003f1122, "a"
	.incbin "baserom.gba", 0x003f1122, 0x00000002
	.section .rom.003f260b, "a"
	.incbin "baserom.gba", 0x003f260b, 0x00000001
	.section .rom.003f3bce, "a"
	.incbin "baserom.gba", 0x003f3bce, 0x00000a6e
	.section .rom.003f5412, "a"
	.incbin "baserom.gba", 0x003f5412, 0x00000002
	.section .rom.003f5795, "a"
	.incbin "baserom.gba", 0x003f5795, 0x00000003
	.section .rom.003f6445, "a"
	.incbin "baserom.gba", 0x003f6445, 0x00000003
	.section .rom.003f6f8d, "a"
	.incbin "baserom.gba", 0x003f6f8d, 0x00000003
	.section .rom.003f7126, "a"
	.incbin "baserom.gba", 0x003f7126, 0x00000002
	.section .rom.003f79b3, "a"
	.incbin "baserom.gba", 0x003f79b3, 0x00000001
	.section .rom.003f847a, "a"
	.incbin "baserom.gba", 0x003f847a, 0x00000002
	.section .rom.003f8992, "a"
	.incbin "baserom.gba", 0x003f8992, 0x00000002
	.section .rom.003f8fb6, "a"
	.incbin "baserom.gba", 0x003f8fb6, 0x00000002
	.section .rom.003f981d, "a"
	.incbin "baserom.gba", 0x003f981d, 0x00000003
	.section .rom.003f9c41, "a"
	.incbin "baserom.gba", 0x003f9c41, 0x00000003
	.section .rom.003f9edb, "a"
	.incbin "baserom.gba", 0x003f9edb, 0x00000001
	.section .rom.003fb877, "a"
	.incbin "baserom.gba", 0x003fb877, 0x00000001
	.section .rom.003fd5ee, "a"
	.incbin "baserom.gba", 0x003fd5ee, 0x00000002
	.section .rom.003ff563, "a"
	.incbin "baserom.gba", 0x003ff563, 0x00000001
	.section .rom.003ffa33, "a"
	.incbin "baserom.gba", 0x003ffa33, 0x00000001
	.section .rom.004000c5, "a"
	.incbin "baserom.gba", 0x004000c5, 0x00000a37
	.section .rom.00401ecd, "a"
	.incbin "baserom.gba", 0x00401ecd, 0x00000003
	.section .rom.0040299e, "a"
	.incbin "baserom.gba", 0x0040299e, 0x00000002
	.section .rom.004034db, "a"
	.incbin "baserom.gba", 0x004034db, 0x00000001
	.section .rom.00403b1b, "a"
	.incbin "baserom.gba", 0x00403b1b, 0x00001589
	.section .rom.00405105, "a"
	.incbin "baserom.gba", 0x00405105, 0x00000003
	.section .rom.00405473, "a"
	.incbin "baserom.gba", 0x00405473, 0x00000001
	.section .rom.00405a11, "a"
	.incbin "baserom.gba", 0x00405a11, 0x00000003
	.section .rom.00405d45, "a"
	.incbin "baserom.gba", 0x00405d45, 0x00000003
	.section .rom.00406081, "a"
	.incbin "baserom.gba", 0x00406081, 0x00000003
	.section .rom.0040709a, "a"
	.incbin "baserom.gba", 0x0040709a, 0x00000002
	.section .rom.0040a6a6, "a"
	.incbin "baserom.gba", 0x0040a6a6, 0x00000002
	.section .rom.0040ae49, "a"
	.incbin "baserom.gba", 0x0040ae49, 0x00000003
	.section .rom.0040c1db, "a"
	.incbin "baserom.gba", 0x0040c1db, 0x00000001
	.section .rom.0040c60b, "a"
	.incbin "baserom.gba", 0x0040c60b, 0x00000001
	.section .rom.0040e401, "a"
	.incbin "baserom.gba", 0x0040e401, 0x00000003
	.section .rom.0040ed16, "a"
	.incbin "baserom.gba", 0x0040ed16, 0x00000002
	.section .rom.0041084a, "a"
	.incbin "baserom.gba", 0x0041084a, 0x00000002
	.section .rom.00411899, "a"
	.incbin "baserom.gba", 0x00411899, 0x00000003
	.section .rom.00411f4f, "a"
	.incbin "baserom.gba", 0x00411f4f, 0x00000001
	.section .rom.00412ff5, "a"
	.incbin "baserom.gba", 0x00412ff5, 0x00000003
	.section .rom.00426407, "a"
	.incbin "baserom.gba", 0x00426407, 0x00000001
	.section .rom.00426559, "a"
	.incbin "baserom.gba", 0x00426559, 0x00000003
	.section .rom.004269ea, "a"
	.incbin "baserom.gba", 0x004269ea, 0x00000002
	.section .rom.00426be1, "a"
	.incbin "baserom.gba", 0x00426be1, 0x00000003
	.section .rom.0042829e, "a"
	.incbin "baserom.gba", 0x0042829e, 0x00000002
	.section .rom.004297bd, "a"
	.incbin "baserom.gba", 0x004297bd, 0x00000003
	.section .rom.0042a389, "a"
	.incbin "baserom.gba", 0x0042a389, 0x00000003
	.section .rom.0042aefe, "a"
	.incbin "baserom.gba", 0x0042aefe, 0x00000002
	.section .rom.0042cda7, "a"
	.incbin "baserom.gba", 0x0042cda7, 0x00000001
	.section .rom.0042e535, "a"
	.incbin "baserom.gba", 0x0042e535, 0x00000003
	.section .rom.0042f9e6, "a"
	.incbin "baserom.gba", 0x0042f9e6, 0x00000002
	.section .rom.00432133, "a"
	.incbin "baserom.gba", 0x00432133, 0x00000001
	.section .rom.004339cd, "a"
	.incbin "baserom.gba", 0x004339cd, 0x00000003
	.section .rom.00434f96, "a"
	.incbin "baserom.gba", 0x00434f96, 0x00000002
	.section .rom.00436a26, "a"
	.incbin "baserom.gba", 0x00436a26, 0x00000002
	.section .rom.004377db, "a"
	.incbin "baserom.gba", 0x004377db, 0x00000001
	.section .rom.0044114a, "a"
	.incbin "baserom.gba", 0x0044114a, 0x00000002
	.section .rom.004432fe, "a"
	.incbin "baserom.gba", 0x004432fe, 0x00000002
	.section .rom.00447465, "a"
	.incbin "baserom.gba", 0x00447465, 0x00000003
	.section .rom.0044a68a, "a"
	.incbin "baserom.gba", 0x0044a68a, 0x00000002
	.section .rom.0044bd8e, "a"
	.incbin "baserom.gba", 0x0044bd8e, 0x00000002
	.section .rom.00452976, "a"
	.incbin "baserom.gba", 0x00452976, 0x00000002
	.section .rom.0045b2fa, "a"
	.incbin "baserom.gba", 0x0045b2fa, 0x00000002
	.section .rom.00462dd6, "a"
	.incbin "baserom.gba", 0x00462dd6, 0x00000002
	.section .rom.00465e6a, "a"
	.incbin "baserom.gba", 0x00465e6a, 0x00000002
	.section .rom.00469279, "a"
	.incbin "baserom.gba", 0x00469279, 0x00000003
	.section .rom.0046bae5, "a"
	.incbin "baserom.gba", 0x0046bae5, 0x00000003
	.section .rom.0046d5d6, "a"
	.incbin "baserom.gba", 0x0046d5d6, 0x00000002
	.section .rom.0046e3ee, "a"
	.incbin "baserom.gba", 0x0046e3ee, 0x00000002
	.section .rom.0046f0d9, "a"
	.incbin "baserom.gba", 0x0046f0d9, 0x00000003
	.section .rom.00478596, "a"
	.incbin "baserom.gba", 0x00478596, 0x00000002
	.section .rom.004792a1, "a"
	.incbin "baserom.gba", 0x004792a1, 0x00000003
	.section .rom.0047b3b5, "a"
	.incbin "baserom.gba", 0x0047b3b5, 0x00000003
	.section .rom.0047c027, "a"
	.incbin "baserom.gba", 0x0047c027, 0x00000001
	.section .rom.0047cc3f, "a"
	.incbin "baserom.gba", 0x0047cc3f, 0x00000001
	.section .rom.0047d3b3, "a"
	.incbin "baserom.gba", 0x0047d3b3, 0x00000001
	.section .rom.0047ec43, "a"
	.incbin "baserom.gba", 0x0047ec43, 0x00000001
	.section .rom.0047f611, "a"
	.incbin "baserom.gba", 0x0047f611, 0x00000003
	.section .rom.0048022f, "a"
	.incbin "baserom.gba", 0x0048022f, 0x00000001
	.section .rom.004828e3, "a"
	.incbin "baserom.gba", 0x004828e3, 0x00000001
	.section .rom.00484ff6, "a"
	.incbin "baserom.gba", 0x00484ff6, 0x00000002
	.section .rom.00489f4b, "a"
	.incbin "baserom.gba", 0x00489f4b, 0x00000001
	.section .rom.0048e275, "a"
	.incbin "baserom.gba", 0x0048e275, 0x00000003
	.section .rom.0048ee62, "a"
	.incbin "baserom.gba", 0x0048ee62, 0x00000002
	.section .rom.00493a17, "a"
	.incbin "baserom.gba", 0x00493a17, 0x00000001
	.section .rom.00495d81, "a"
	.incbin "baserom.gba", 0x00495d81, 0x00000003
	.section .rom.00497723, "a"
	.incbin "baserom.gba", 0x00497723, 0x00000001
	.section .rom.0049bfe5, "a"
	.incbin "baserom.gba", 0x0049bfe5, 0x00000003
	.section .rom.004a00ad, "a"
	.incbin "baserom.gba", 0x004a00ad, 0x00000003
	.section .rom.004a3776, "a"
	.incbin "baserom.gba", 0x004a3776, 0x00000002
	.section .rom.004ab747, "a"
	.incbin "baserom.gba", 0x004ab747, 0x00000001
	.section .rom.004b2a57, "a"
	.incbin "baserom.gba", 0x004b2a57, 0x00000001
	.section .rom.004b79d7, "a"
	.incbin "baserom.gba", 0x004b79d7, 0x00000001
	.section .rom.004bc387, "a"
	.incbin "baserom.gba", 0x004bc387, 0x0000051d
	.section .rom.004c1cb3, "a"
	.incbin "baserom.gba", 0x004c1cb3, 0x00000001
	.section .rom.004c1e41, "a"
	.incbin "baserom.gba", 0x004c1e41, 0x00000003
	.section .rom.004c6ea1, "a"
	.incbin "baserom.gba", 0x004c6ea1, 0x00000003
	.section .rom.004cb40d, "a"
	.incbin "baserom.gba", 0x004cb40d, 0x00000003
	.section .rom.004d05d1, "a"
	.incbin "baserom.gba", 0x004d05d1, 0x00000003
	.section .rom.004d0767, "a"
	.incbin "baserom.gba", 0x004d0767, 0x00000001
	.section .rom.004d33bb, "a"
	.incbin "baserom.gba", 0x004d33bb, 0x00000001
	.section .rom.004da06f, "a"
	.incbin "baserom.gba", 0x004da06f, 0x00000001
	.section .rom.004dd1aa, "a"
	.incbin "baserom.gba", 0x004dd1aa, 0x00000002
	.section .rom.004dd32b, "a"
	.incbin "baserom.gba", 0x004dd32b, 0x00000001
	.section .rom.004dfae5, "a"
	.incbin "baserom.gba", 0x004dfae5, 0x00000003
	.section .rom.004e212e, "a"
	.incbin "baserom.gba", 0x004e212e, 0x00000002
	.section .rom.004e4812, "a"
	.incbin "baserom.gba", 0x004e4812, 0x00000002
	.section .rom.004e5887, "a"
	.incbin "baserom.gba", 0x004e5887, 0x00000001
	.section .rom.004e79da, "a"
	.incbin "baserom.gba", 0x004e79da, 0x00000002
	.section .rom.004e7b6d, "a"
	.incbin "baserom.gba", 0x004e7b6d, 0x00000003
	.section .rom.004ea3af, "a"
	.incbin "baserom.gba", 0x004ea3af, 0x00000001
	.section .rom.004ec1c5, "a"
	.incbin "baserom.gba", 0x004ec1c5, 0x00000003
	.section .rom.004ee8a6, "a"
	.incbin "baserom.gba", 0x004ee8a6, 0x00000002
	.section .rom.004ef9ed, "a"
	.incbin "baserom.gba", 0x004ef9ed, 0x00000003
	.section .rom.004f2b01, "a"
	.incbin "baserom.gba", 0x004f2b01, 0x00000003
	.section .rom.004f6c69, "a"
	.incbin "baserom.gba", 0x004f6c69, 0x00000003
	.section .rom.004f8266, "a"
	.incbin "baserom.gba", 0x004f8266, 0x00000002
	.section .rom.004f952f, "a"
	.incbin "baserom.gba", 0x004f952f, 0x00000001
	.section .rom.004fc947, "a"
	.incbin "baserom.gba", 0x004fc947, 0x00000001
	.section .rom.004fcad5, "a"
	.incbin "baserom.gba", 0x004fcad5, 0x00000003
	.section .rom.004fe886, "a"
	.incbin "baserom.gba", 0x004fe886, 0x00000002
	.section .rom.005023fd, "a"
	.incbin "baserom.gba", 0x005023fd, 0x00000003
	.section .rom.0050390b, "a"
	.incbin "baserom.gba", 0x0050390b, 0x00000001
	.section .rom.005044df, "a"
	.incbin "baserom.gba", 0x005044df, 0x00000001
	.section .rom.005045dd, "a"
	.incbin "baserom.gba", 0x005045dd, 0x00000003
	.section .rom.005059c2, "a"
	.incbin "baserom.gba", 0x005059c2, 0x00000002
	.section .rom.00506b45, "a"
	.incbin "baserom.gba", 0x00506b45, 0x00000003
	.section .rom.005074ce, "a"
	.incbin "baserom.gba", 0x005074ce, 0x00000002
	.section .rom.0050a14b, "a"
	.incbin "baserom.gba", 0x0050a14b, 0x00000001
	.section .rom.0050a22e, "a"
	.incbin "baserom.gba", 0x0050a22e, 0x00000002
	.section .rom.0050b419, "a"
	.incbin "baserom.gba", 0x0050b419, 0x00000003
	.section .rom.0050d0a1, "a"
	.incbin "baserom.gba", 0x0050d0a1, 0x00000003
	.section .rom.0050d5af, "a"
	.incbin "baserom.gba", 0x0050d5af, 0x00000001
	.section .rom.0050d6ef, "a"
	.incbin "baserom.gba", 0x0050d6ef, 0x00000001
	.section .rom.0050e90a, "a"
	.incbin "baserom.gba", 0x0050e90a, 0x00000002
	.section .rom.0050ea65, "a"
	.incbin "baserom.gba", 0x0050ea65, 0x00000003
	.section .rom.00512e8b, "a"
	.incbin "baserom.gba", 0x00512e8b, 0x00000001
	.section .rom.00514bba, "a"
	.incbin "baserom.gba", 0x00514bba, 0x00000002
	.section .rom.00515933, "a"
	.incbin "baserom.gba", 0x00515933, 0x00000001
	.section .rom.00515a67, "a"
	.incbin "baserom.gba", 0x00515a67, 0x00000001
	.section .rom.00517eea, "a"
	.incbin "baserom.gba", 0x00517eea, 0x00000002
	.section .rom.00518c67, "a"
	.incbin "baserom.gba", 0x00518c67, 0x00000001
	.section .rom.00519746, "a"
	.incbin "baserom.gba", 0x00519746, 0x00000002
	.section .rom.0051a5be, "a"
	.incbin "baserom.gba", 0x0051a5be, 0x00000002
	.section .rom.0051c692, "a"
	.incbin "baserom.gba", 0x0051c692, 0x00000002
	.section .rom.0051cfd9, "a"
	.incbin "baserom.gba", 0x0051cfd9, 0x00000003
	.section .rom.0051dc26, "a"
	.incbin "baserom.gba", 0x0051dc26, 0x00000002
	.section .rom.0051de4d, "a"
	.incbin "baserom.gba", 0x0051de4d, 0x00000003
	.section .rom.0051f78f, "a"
	.incbin "baserom.gba", 0x0051f78f, 0x00000001
	.section .rom.0051f8a3, "a"
	.incbin "baserom.gba", 0x0051f8a3, 0x00000001
	.section .rom.00521269, "a"
	.incbin "baserom.gba", 0x00521269, 0x00000003
	.section .rom.005213b3, "a"
	.incbin "baserom.gba", 0x005213b3, 0x00000001
	.section .rom.0052347f, "a"
	.incbin "baserom.gba", 0x0052347f, 0x00000001
	.section .rom.005235a7, "a"
	.incbin "baserom.gba", 0x005235a7, 0x00000001
	.section .rom.00524e2f, "a"
	.incbin "baserom.gba", 0x00524e2f, 0x00000001
	.section .rom.005270d1, "a"
	.incbin "baserom.gba", 0x005270d1, 0x00000003
	.section .rom.00527213, "a"
	.incbin "baserom.gba", 0x00527213, 0x00000001
	.section .rom.0052921b, "a"
	.incbin "baserom.gba", 0x0052921b, 0x00000001
	.section .rom.00529326, "a"
	.incbin "baserom.gba", 0x00529326, 0x00000002
	.section .rom.0052b30a, "a"
	.incbin "baserom.gba", 0x0052b30a, 0x00000002
	.section .rom.0052ca2f, "a"
	.incbin "baserom.gba", 0x0052ca2f, 0x00000001
	.section .rom.0052dae3, "a"
	.incbin "baserom.gba", 0x0052dae3, 0x00000001
	.section .rom.0052f7ca, "a"
	.incbin "baserom.gba", 0x0052f7ca, 0x00000002
	.section .rom.0052f925, "a"
	.incbin "baserom.gba", 0x0052f925, 0x00000003
	.section .rom.0053186e, "a"
	.incbin "baserom.gba", 0x0053186e, 0x00000002
	.section .rom.005319f3, "a"
	.incbin "baserom.gba", 0x005319f3, 0x00000001
	.section .rom.00534545, "a"
	.incbin "baserom.gba", 0x00534545, 0x00000003
	.section .rom.00536c0a, "a"
	.incbin "baserom.gba", 0x00536c0a, 0x00000002
	.section .rom.00539362, "a"
	.incbin "baserom.gba", 0x00539362, 0x00000002
	.section .rom.0053abb5, "a"
	.incbin "baserom.gba", 0x0053abb5, 0x00000003
	.section .rom.0053d2b5, "a"
	.incbin "baserom.gba", 0x0053d2b5, 0x00000003
	.section .rom.0053f081, "a"
	.incbin "baserom.gba", 0x0053f081, 0x00000003
	.section .rom.005415cb, "a"
	.incbin "baserom.gba", 0x005415cb, 0x00000001
	.section .rom.00542896, "a"
	.incbin "baserom.gba", 0x00542896, 0x00000002
	.section .rom.00543547, "a"
	.incbin "baserom.gba", 0x00543547, 0x00000001
	.section .rom.00543fb1, "a"
	.incbin "baserom.gba", 0x00543fb1, 0x00000003
	.section .rom.005440a7, "a"
	.incbin "baserom.gba", 0x005440a7, 0x00000001
	.section .rom.00545b06, "a"
	.incbin "baserom.gba", 0x00545b06, 0x00000002
	.section .rom.005468aa, "a"
	.incbin "baserom.gba", 0x005468aa, 0x00000002
	.section .rom.005483e3, "a"
	.incbin "baserom.gba", 0x005483e3, 0x00000001
	.section .rom.0054b2d1, "a"
	.incbin "baserom.gba", 0x0054b2d1, 0x00000003
	.section .rom.00552ab7, "a"
	.incbin "baserom.gba", 0x00552ab7, 0x00000001
	.section .rom.00552b83, "a"
	.incbin "baserom.gba", 0x00552b83, 0x00000001
	.section .rom.00558345, "a"
	.incbin "baserom.gba", 0x00558345, 0x00000003
	.section .rom.00558487, "a"
	.incbin "baserom.gba", 0x00558487, 0x00000001
	.section .rom.005598b5, "a"
	.incbin "baserom.gba", 0x005598b5, 0x00000003
	.section .rom.0055c5f2, "a"
	.incbin "baserom.gba", 0x0055c5f2, 0x00000002
	.section .rom.0055e6ee, "a"
	.incbin "baserom.gba", 0x0055e6ee, 0x00000002
	.section .rom.005660fa, "a"
	.incbin "baserom.gba", 0x005660fa, 0x00000002
	.section .rom.00566296, "a"
	.incbin "baserom.gba", 0x00566296, 0x00000002
	.section .rom.0056ab31, "a"
	.incbin "baserom.gba", 0x0056ab31, 0x00000003
	.section .rom.0056d032, "a"
	.incbin "baserom.gba", 0x0056d032, 0x00000002
	.section .rom.00574a23, "a"
	.incbin "baserom.gba", 0x00574a23, 0x00000001
	.section .rom.00574bbe, "a"
	.incbin "baserom.gba", 0x00574bbe, 0x00000002
	.section .rom.0057776d, "a"
	.incbin "baserom.gba", 0x0057776d, 0x00000003
	.section .rom.00579841, "a"
	.incbin "baserom.gba", 0x00579841, 0x00000003
	.section .rom.0057b89a, "a"
	.incbin "baserom.gba", 0x0057b89a, 0x00001cee
	.section .rom.0057ed31, "a"
	.incbin "baserom.gba", 0x0057ed31, 0x00000003
	.section .rom.00581a0d, "a"
	.incbin "baserom.gba", 0x00581a0d, 0x00000003
	.section .rom.005837df, "a"
	.incbin "baserom.gba", 0x005837df, 0x00000001
	.section .rom.0058391f, "a"
	.incbin "baserom.gba", 0x0058391f, 0x00000001
	.section .rom.00586979, "a"
	.incbin "baserom.gba", 0x00586979, 0x00000003
	.section .rom.005898f6, "a"
	.incbin "baserom.gba", 0x005898f6, 0x00000002
	.section .rom.0058c1bd, "a"
	.incbin "baserom.gba", 0x0058c1bd, 0x00000003
	.section .rom.00593125, "a"
	.incbin "baserom.gba", 0x00593125, 0x00000003
	.section .rom.005949cb, "a"
	.incbin "baserom.gba", 0x005949cb, 0x00000001
	.section .rom.005952ca, "a"
	.incbin "baserom.gba", 0x005952ca, 0x00000002
	.section .rom.00596043, "a"
	.incbin "baserom.gba", 0x00596043, 0x00000001
	.section .rom.005961a3, "a"
	.incbin "baserom.gba", 0x005961a3, 0x00000001
	.section .rom.00597dfb, "a"
	.incbin "baserom.gba", 0x00597dfb, 0x00000001
	.section .rom.00599693, "a"
	.incbin "baserom.gba", 0x00599693, 0x00000001
	.section .rom.00599817, "a"
	.incbin "baserom.gba", 0x00599817, 0x00000001
	.section .rom.0059c353, "a"
	.incbin "baserom.gba", 0x0059c353, 0x00000001
	.section .rom.0059eacd, "a"
	.incbin "baserom.gba", 0x0059eacd, 0x00000003
	.section .rom.0059faba, "a"
	.incbin "baserom.gba", 0x0059faba, 0x00000002
	.section .rom.005a117b, "a"
	.incbin "baserom.gba", 0x005a117b, 0x00000001
	.section .rom.005a1325, "a"
	.incbin "baserom.gba", 0x005a1325, 0x00000003
	.section .rom.005a40b9, "a"
	.incbin "baserom.gba", 0x005a40b9, 0x00000003
	.section .rom.005a814e, "a"
	.incbin "baserom.gba", 0x005a814e, 0x00000002
	.section .rom.005a828f, "a"
	.incbin "baserom.gba", 0x005a828f, 0x00000001
	.section .rom.005aa157, "a"
	.incbin "baserom.gba", 0x005aa157, 0x00000001
	.section .rom.005aa2f9, "a"
	.incbin "baserom.gba", 0x005aa2f9, 0x00000003
	.section .rom.005ac39b, "a"
	.incbin "baserom.gba", 0x005ac39b, 0x00000001
	.section .rom.005ad328, "a"
	.incbin "baserom.gba", 0x005ad328, 0x000022d8
	.section .rom.005b20f1, "a"
	.incbin "baserom.gba", 0x005b20f1, 0x00000003
	.section .rom.005b2276, "a"
	.incbin "baserom.gba", 0x005b2276, 0x00000002
	.section .rom.005b99c3, "a"
	.incbin "baserom.gba", 0x005b99c3, 0x00000001
	.section .rom.005b9ecd, "a"
	.incbin "baserom.gba", 0x005b9ecd, 0x00000003
	.section .rom.005bb5a5, "a"
	.incbin "baserom.gba", 0x005bb5a5, 0x00000003
	.section .rom.005bb749, "a"
	.incbin "baserom.gba", 0x005bb749, 0x00000003
	.section .rom.005bd04e, "a"
	.incbin "baserom.gba", 0x005bd04e, 0x00000002
	.section .rom.005be87b, "a"
	.incbin "baserom.gba", 0x005be87b, 0x00000001
	.section .rom.005be9f9, "a"
	.incbin "baserom.gba", 0x005be9f9, 0x00000003
	.section .rom.005c32ed, "a"
	.incbin "baserom.gba", 0x005c32ed, 0x00000003
	.section .rom.005c4332, "a"
	.incbin "baserom.gba", 0x005c4332, 0x00000002
	.section .rom.005c5156, "a"
	.incbin "baserom.gba", 0x005c5156, 0x00000002
	.section .rom.005c649b, "a"
	.incbin "baserom.gba", 0x005c649b, 0x00000001
	.section .rom.005c65f5, "a"
	.incbin "baserom.gba", 0x005c65f5, 0x00000003
	.section .rom.005c8bb3, "a"
	.incbin "baserom.gba", 0x005c8bb3, 0x00000001
	.section .rom.005c8d2a, "a"
	.incbin "baserom.gba", 0x005c8d2a, 0x00000002
	.section .rom.005cd61d, "a"
	.incbin "baserom.gba", 0x005cd61d, 0x00000003
	.section .rom.005ce085, "a"
	.incbin "baserom.gba", 0x005ce085, 0x00000003
	.section .rom.005d06c7, "a"
	.incbin "baserom.gba", 0x005d06c7, 0x00000001
	.section .rom.005d082e, "a"
	.incbin "baserom.gba", 0x005d082e, 0x00000002
	.section .rom.005d1975, "a"
	.incbin "baserom.gba", 0x005d1975, 0x00000003
	.section .rom.005d36d3, "a"
	.incbin "baserom.gba", 0x005d36d3, 0x00000001
	.section .rom.005d383a, "a"
	.incbin "baserom.gba", 0x005d383a, 0x00000002
	.section .rom.005d60c9, "a"
	.incbin "baserom.gba", 0x005d60c9, 0x00000003
	.section .rom.005d61fe, "a"
	.incbin "baserom.gba", 0x005d61fe, 0x00000002
	.section .rom.005d8e3a, "a"
	.incbin "baserom.gba", 0x005d8e3a, 0x00000002
	.section .rom.005dc5d9, "a"
	.incbin "baserom.gba", 0x005dc5d9, 0x00000003
	.section .rom.005df927, "a"
	.incbin "baserom.gba", 0x005df927, 0x00000001
	.section .rom.005dfaa3, "a"
	.incbin "baserom.gba", 0x005dfaa3, 0x00000001
	.section .rom.005e2652, "a"
	.incbin "baserom.gba", 0x005e2652, 0x00000002
	.section .rom.005e48c6, "a"
	.incbin "baserom.gba", 0x005e48c6, 0x00000002
	.section .rom.005e7137, "a"
	.incbin "baserom.gba", 0x005e7137, 0x00000001
	.section .rom.005ea62b, "a"
	.incbin "baserom.gba", 0x005ea62b, 0x00000001
	.section .rom.005ef252, "a"
	.incbin "baserom.gba", 0x005ef252, 0x00000002
	.section .rom.005efed5, "a"
	.incbin "baserom.gba", 0x005efed5, 0x00000003
	.section .rom.005f0017, "a"
	.incbin "baserom.gba", 0x005f0017, 0x00000001
	.section .rom.005f1587, "a"
	.incbin "baserom.gba", 0x005f1587, 0x00000001
	.section .rom.005f16f2, "a"
	.incbin "baserom.gba", 0x005f16f2, 0x00000002
	.section .rom.005f344d, "a"
	.incbin "baserom.gba", 0x005f344d, 0x00000003
	.section .rom.005f358f, "a"
	.incbin "baserom.gba", 0x005f358f, 0x00000001
	.section .rom.005f5d8e, "a"
	.incbin "baserom.gba", 0x005f5d8e, 0x00000002
	.section .rom.005f5ecf, "a"
	.incbin "baserom.gba", 0x005f5ecf, 0x00000001
	.section .rom.005f78e7, "a"
	.incbin "baserom.gba", 0x005f78e7, 0x00000001
	.section .rom.005f8e62, "a"
	.incbin "baserom.gba", 0x005f8e62, 0x00000002
	.section .rom.005fa39e, "a"
	.incbin "baserom.gba", 0x005fa39e, 0x00000002
	.section .rom.005fa52e, "a"
	.incbin "baserom.gba", 0x005fa52e, 0x00000002
	.section .rom.005fc7e7, "a"
	.incbin "baserom.gba", 0x005fc7e7, 0x00000001
	.section .rom.00600a2e, "a"
	.incbin "baserom.gba", 0x00600a2e, 0x00000002
	.section .rom.00602825, "a"
	.incbin "baserom.gba", 0x00602825, 0x00000003
	.section .rom.006059d3, "a"
	.incbin "baserom.gba", 0x006059d3, 0x00000001
	.section .rom.00608007, "a"
	.incbin "baserom.gba", 0x00608007, 0x00000001
	.section .rom.0060ab5f, "a"
	.incbin "baserom.gba", 0x0060ab5f, 0x00000001
	.section .rom.0060ad27, "a"
	.incbin "baserom.gba", 0x0060ad27, 0x00002905
	.section .rom.00611131, "a"
	.incbin "baserom.gba", 0x00611131, 0x00000003
	.section .rom.006126a2, "a"
	.incbin "baserom.gba", 0x006126a2, 0x00000002
	.section .rom.00614ec9, "a"
	.incbin "baserom.gba", 0x00614ec9, 0x00000003
	.section .rom.00615039, "a"
	.incbin "baserom.gba", 0x00615039, 0x00000003
	.section .rom.006178fb, "a"
	.incbin "baserom.gba", 0x006178fb, 0x00000001
	.section .rom.0061a0c3, "a"
	.incbin "baserom.gba", 0x0061a0c3, 0x00000001
	.section .rom.0061b3bd, "a"
	.incbin "baserom.gba", 0x0061b3bd, 0x00000003
	.section .rom.0061cd03, "a"
	.incbin "baserom.gba", 0x0061cd03, 0x00000001
	.section .rom.00628196, "a"
	.incbin "baserom.gba", 0x00628196, 0x00000002
	.section .rom.00628356, "a"
	.incbin "baserom.gba", 0x00628356, 0x00000002
	.section .rom.0062b6bd, "a"
	.incbin "baserom.gba", 0x0062b6bd, 0x00000003
	.section .rom.0062dbae, "a"
	.incbin "baserom.gba", 0x0062dbae, 0x00000002
	.section .rom.0063098f, "a"
	.incbin "baserom.gba", 0x0063098f, 0x00000001
	.section .rom.00631529, "a"
	.incbin "baserom.gba", 0x00631529, 0x00000003
	.section .rom.0063238a, "a"
	.incbin "baserom.gba", 0x0063238a, 0x00000002
	.section .rom.006324e2, "a"
	.incbin "baserom.gba", 0x006324e2, 0x00000002
	.section .rom.0063a32f, "a"
	.incbin "baserom.gba", 0x0063a32f, 0x00000001
	.section .rom.0063a49b, "a"
	.incbin "baserom.gba", 0x0063a49b, 0x00000001
	.section .rom.0063d237, "a"
	.incbin "baserom.gba", 0x0063d237, 0x00000001
	.section .rom.0063f3c6, "a"
	.incbin "baserom.gba", 0x0063f3c6, 0x00000002
	.section .rom.0063face, "a"
	.incbin "baserom.gba", 0x0063face, 0x00000002
	.section .rom.006406e2, "a"
	.incbin "baserom.gba", 0x006406e2, 0x00000002
	.section .rom.006517b5, "a"
	.incbin "baserom.gba", 0x006517b5, 0x00000003
	.section .rom.0065194a, "a"
	.incbin "baserom.gba", 0x0065194a, 0x00000002
	.section .rom.00652e5a, "a"
	.incbin "baserom.gba", 0x00652e5a, 0x00000002
	.section .rom.00654c3f, "a"
	.incbin "baserom.gba", 0x00654c3f, 0x00000001
	.section .rom.00655cd2, "a"
	.incbin "baserom.gba", 0x00655cd2, 0x00000002
	.section .rom.00657f42, "a"
	.incbin "baserom.gba", 0x00657f42, 0x00000002
	.section .rom.006580d9, "a"
	.incbin "baserom.gba", 0x006580d9, 0x00000003
	.section .rom.0065a07f, "a"
	.incbin "baserom.gba", 0x0065a07f, 0x00000001
	.section .rom.0065ee56, "a"
	.incbin "baserom.gba", 0x0065ee56, 0x00000002
	.section .rom.006628b7, "a"
	.incbin "baserom.gba", 0x006628b7, 0x00000001
	.section .rom.006655ed, "a"
	.incbin "baserom.gba", 0x006655ed, 0x00000003
	.section .rom.00667935, "a"
	.incbin "baserom.gba", 0x00667935, 0x00000003
	.section .rom.00669113, "a"
	.incbin "baserom.gba", 0x00669113, 0x00000001
	.section .rom.006692c5, "a"
	.incbin "baserom.gba", 0x006692c5, 0x00000003
	.section .rom.00679c66, "a"
	.incbin "baserom.gba", 0x00679c66, 0x00000002
	.section .rom.00679dc5, "a"
	.incbin "baserom.gba", 0x00679dc5, 0x00000003
	.section .rom.0067d51f, "a"
	.incbin "baserom.gba", 0x0067d51f, 0x00000001
	.section .rom.0067d682, "a"
	.incbin "baserom.gba", 0x0067d682, 0x00000002
	.section .rom.00683365, "a"
	.incbin "baserom.gba", 0x00683365, 0x00000003
	.section .rom.00685d4b, "a"
	.incbin "baserom.gba", 0x00685d4b, 0x00000001
	.section .rom.00687f56, "a"
	.incbin "baserom.gba", 0x00687f56, 0x00000002
	.section .rom.00688097, "a"
	.incbin "baserom.gba", 0x00688097, 0x00000001
	.section .rom.00689aae, "a"
	.incbin "baserom.gba", 0x00689aae, 0x00000002
	.section .rom.00690815, "a"
	.incbin "baserom.gba", 0x00690815, 0x00000003
	.section .rom.00690993, "a"
	.incbin "baserom.gba", 0x00690993, 0x00000001
	.section .rom.0069293d, "a"
	.incbin "baserom.gba", 0x0069293d, 0x00000003
	.section .rom.006950b6, "a"
	.incbin "baserom.gba", 0x006950b6, 0x00000002
	.section .rom.0069691d, "a"
	.incbin "baserom.gba", 0x0069691d, 0x00000003
	.section .rom.0069a33e, "a"
	.incbin "baserom.gba", 0x0069a33e, 0x00000002
	.section .rom.0069a507, "a"
	.incbin "baserom.gba", 0x0069a507, 0x00000001
	.section .rom.006a54ef, "a"
	.incbin "baserom.gba", 0x006a54ef, 0x00000001
	.section .rom.006a763b, "a"
	.incbin "baserom.gba", 0x006a763b, 0x00000001
	.section .rom.006a77c5, "a"
	.incbin "baserom.gba", 0x006a77c5, 0x00000003
	.section .rom.006a924a, "a"
	.incbin "baserom.gba", 0x006a924a, 0x00000002
	.section .rom.006ac281, "a"
	.incbin "baserom.gba", 0x006ac281, 0x00000003
	.section .rom.006ac447, "a"
	.incbin "baserom.gba", 0x006ac447, 0x00000001
	.section .rom.006ad37b, "a"
	.incbin "baserom.gba", 0x006ad37b, 0x00000001
	.section .rom.006af42e, "a"
	.incbin "baserom.gba", 0x006af42e, 0x00000002
	.section .rom.006af5d5, "a"
	.incbin "baserom.gba", 0x006af5d5, 0x00000003
	.section .rom.006b45ff, "a"
	.incbin "baserom.gba", 0x006b45ff, 0x00000001
	.section .rom.006b738f, "a"
	.incbin "baserom.gba", 0x006b738f, 0x00000001
	.section .rom.006b754f, "a"
	.incbin "baserom.gba", 0x006b754f, 0x00000001
	.section .rom.006b9a43, "a"
	.incbin "baserom.gba", 0x006b9a43, 0x00000001
	.section .rom.006b9c15, "a"
	.incbin "baserom.gba", 0x006b9c15, 0x00000003
	.section .rom.006bbca6, "a"
	.incbin "baserom.gba", 0x006bbca6, 0x00000002
	.section .rom.006bdda6, "a"
	.incbin "baserom.gba", 0x006bdda6, 0x00000002
	.section .rom.006c095a, "a"
	.incbin "baserom.gba", 0x006c095a, 0x00000002
	.section .rom.006c1447, "a"
	.incbin "baserom.gba", 0x006c1447, 0x00000001
	.section .rom.006c161a, "a"
	.incbin "baserom.gba", 0x006c161a, 0x00000002
	.section .rom.006c3f49, "a"
	.incbin "baserom.gba", 0x006c3f49, 0x00000003
	.section .rom.006c56e1, "a"
	.incbin "baserom.gba", 0x006c56e1, 0x00000003
	.section .rom.006c6955, "a"
	.incbin "baserom.gba", 0x006c6955, 0x00000003
	.section .rom.006c8de5, "a"
	.incbin "baserom.gba", 0x006c8de5, 0x00000003
	.section .rom.006cb131, "a"
	.incbin "baserom.gba", 0x006cb131, 0x00000003
	.section .rom.006cc31b, "a"
	.incbin "baserom.gba", 0x006cc31b, 0x00000001
	.section .rom.006cc4a2, "a"
	.incbin "baserom.gba", 0x006cc4a2, 0x00000002
	.section .rom.006d09ee, "a"
	.incbin "baserom.gba", 0x006d09ee, 0x00000002
	.section .rom.006d0b43, "a"
	.incbin "baserom.gba", 0x006d0b43, 0x00000001
	.section .rom.006d0c83, "a"
	.incbin "baserom.gba", 0x006d0c83, 0x00000001
	.section .rom.006d1957, "a"
	.incbin "baserom.gba", 0x006d1957, 0x00000001
	.section .rom.006d1ad6, "a"
	.incbin "baserom.gba", 0x006d1ad6, 0x00000002
	.section .rom.006d338f, "a"
	.incbin "baserom.gba", 0x006d338f, 0x00000001
	.section .rom.006d3511, "a"
	.incbin "baserom.gba", 0x006d3511, 0x00000003
	.section .rom.006d4fa3, "a"
	.incbin "baserom.gba", 0x006d4fa3, 0x00000001
	.section .rom.006d511d, "a"
	.incbin "baserom.gba", 0x006d511d, 0x00000003
	.section .rom.006d6e5b, "a"
	.incbin "baserom.gba", 0x006d6e5b, 0x00000001
	.section .rom.006d6f87, "a"
	.incbin "baserom.gba", 0x006d6f87, 0x00000001
	.section .rom.006d70fd, "a"
	.incbin "baserom.gba", 0x006d70fd, 0x00000003
	.section .rom.006d8fa3, "a"
	.incbin "baserom.gba", 0x006d8fa3, 0x00000001
	.section .rom.006d9129, "a"
	.incbin "baserom.gba", 0x006d9129, 0x00000003
	.section .rom.006da087, "a"
	.incbin "baserom.gba", 0x006da087, 0x00000001
	.section .rom.006db337, "a"
	.incbin "baserom.gba", 0x006db337, 0x00000001
	.section .rom.006db4f2, "a"
	.incbin "baserom.gba", 0x006db4f2, 0x00000002
	.section .rom.006ddf96, "a"
	.incbin "baserom.gba", 0x006ddf96, 0x00000002
	.section .rom.006de109, "a"
	.incbin "baserom.gba", 0x006de109, 0x00000003
	.section .rom.006df4f1, "a"
	.incbin "baserom.gba", 0x006df4f1, 0x00000003
	.section .rom.006e1fd1, "a"
	.incbin "baserom.gba", 0x006e1fd1, 0x00000003
	.section .rom.006e217b, "a"
	.incbin "baserom.gba", 0x006e217b, 0x00000001
	.section .rom.006e4576, "a"
	.incbin "baserom.gba", 0x006e4576, 0x00000002
	.section .rom.006e4705, "a"
	.incbin "baserom.gba", 0x006e4705, 0x00000003
	.section .rom.006e7985, "a"
	.incbin "baserom.gba", 0x006e7985, 0x00000003
	.section .rom.006e7b66, "a"
	.incbin "baserom.gba", 0x006e7b66, 0x00000002
	.section .rom.006ea0fb, "a"
	.incbin "baserom.gba", 0x006ea0fb, 0x00000001
	.section .rom.006ea28a, "a"
	.incbin "baserom.gba", 0x006ea28a, 0x00000002
	.section .rom.006ece82, "a"
	.incbin "baserom.gba", 0x006ece82, 0x00000002
	.section .rom.006ed9cf, "a"
	.incbin "baserom.gba", 0x006ed9cf, 0x00000001
	.section .rom.006edb36, "a"
	.incbin "baserom.gba", 0x006edb36, 0x00000002
	.section .rom.006f054b, "a"
	.incbin "baserom.gba", 0x006f054b, 0x00000001
	.section .rom.006f218a, "a"
	.incbin "baserom.gba", 0x006f218a, 0x00000002
	.section .rom.006f2647, "a"
	.incbin "baserom.gba", 0x006f2647, 0x00000001
	.section .rom.006f39bf, "a"
	.incbin "baserom.gba", 0x006f39bf, 0x00000001
	.section .rom.006f4f03, "a"
	.incbin "baserom.gba", 0x006f4f03, 0x00000001
	.section .rom.006f508d, "a"
	.incbin "baserom.gba", 0x006f508d, 0x00000003
	.section .rom.006f616b, "a"
	.incbin "baserom.gba", 0x006f616b, 0x00000001
	.section .rom.006f62f9, "a"
	.incbin "baserom.gba", 0x006f62f9, 0x00000003
	.section .rom.006f7f4d, "a"
	.incbin "baserom.gba", 0x006f7f4d, 0x00000003
	.section .rom.006fa92f, "a"
	.incbin "baserom.gba", 0x006fa92f, 0x00000001
	.section .rom.006fde59, "a"
	.incbin "baserom.gba", 0x006fde59, 0x00000003
	.section .rom.006ff763, "a"
	.incbin "baserom.gba", 0x006ff763, 0x00000001
	.section .rom.0070180b, "a"
	.incbin "baserom.gba", 0x0070180b, 0x00000001
	.section .rom.0070425f, "a"
	.incbin "baserom.gba", 0x0070425f, 0x00000001
	.section .rom.00705eab, "a"
	.incbin "baserom.gba", 0x00705eab, 0x00000001
	.section .rom.007072f1, "a"
	.incbin "baserom.gba", 0x007072f1, 0x00000003
	.section .rom.00709171, "a"
	.incbin "baserom.gba", 0x00709171, 0x00000003
	.section .rom.0070ad42, "a"
	.incbin "baserom.gba", 0x0070ad42, 0x00000002
	.section .rom.0070d476, "a"
	.incbin "baserom.gba", 0x0070d476, 0x00000002
	.section .rom.0070f0bb, "a"
	.incbin "baserom.gba", 0x0070f0bb, 0x00000001
	.section .rom.00710501, "a"
	.incbin "baserom.gba", 0x00710501, 0x00000003
	.section .rom.0071207f, "a"
	.incbin "baserom.gba", 0x0071207f, 0x00000001
	.section .rom.007132af, "a"
	.incbin "baserom.gba", 0x007132af, 0x00000001
	.section .rom.00713409, "a"
	.incbin "baserom.gba", 0x00713409, 0x00000003
	.section .rom.0071690f, "a"
	.incbin "baserom.gba", 0x0071690f, 0x00000001
	.section .rom.00716a7a, "a"
	.incbin "baserom.gba", 0x00716a7a, 0x00000002
	.section .rom.007193ca, "a"
	.incbin "baserom.gba", 0x007193ca, 0x00001166
	.section .rom.0071bd15, "a"
	.incbin "baserom.gba", 0x0071bd15, 0x00000003
	.section .rom.0071ef5b, "a"
	.incbin "baserom.gba", 0x0071ef5b, 0x00000001
	.section .rom.00721852, "a"
	.incbin "baserom.gba", 0x00721852, 0x00000002
	.section .rom.0072ab8b, "a"
	.incbin "baserom.gba", 0x0072ab8b, 0x00000001
	.section .rom.0072beba, "a"
	.incbin "baserom.gba", 0x0072beba, 0x00000002
	.section .rom.0072d0b3, "a"
	.incbin "baserom.gba", 0x0072d0b3, 0x00000001
	.section .rom.0072d215, "a"
	.incbin "baserom.gba", 0x0072d215, 0x00000003
	.section .rom.0072fbe1, "a"
	.incbin "baserom.gba", 0x0072fbe1, 0x00000003
	.section .rom.0073310d, "a"
	.incbin "baserom.gba", 0x0073310d, 0x00000003
	.section .rom.0073754f, "a"
	.incbin "baserom.gba", 0x0073754f, 0x00000001
	.section .rom.007376c2, "a"
	.incbin "baserom.gba", 0x007376c2, 0x00000002
	.section .rom.0073b927, "a"
	.incbin "baserom.gba", 0x0073b927, 0x00000001
	.section .rom.0073f142, "a"
	.incbin "baserom.gba", 0x0073f142, 0x00000002
	.section .rom.00740b6a, "a"
	.incbin "baserom.gba", 0x00740b6a, 0x00000002
	.section .rom.00743217, "a"
	.incbin "baserom.gba", 0x00743217, 0x00000001
	.section .rom.0074527b, "a"
	.incbin "baserom.gba", 0x0074527b, 0x00000001
	.section .rom.00747137, "a"
	.incbin "baserom.gba", 0x00747137, 0x00000001
	.section .rom.0074a381, "a"
	.incbin "baserom.gba", 0x0074a381, 0x00000003
	.section .rom.0074bd47, "a"
	.incbin "baserom.gba", 0x0074bd47, 0x00000001
	.section .rom.0074beb9, "a"
	.incbin "baserom.gba", 0x0074beb9, 0x00000003
	.section .rom.0074eec6, "a"
	.incbin "baserom.gba", 0x0074eec6, 0x00000002
	.section .rom.0074f07e, "a"
	.incbin "baserom.gba", 0x0074f07e, 0x00000002
	.section .rom.007508ef, "a"
	.incbin "baserom.gba", 0x007508ef, 0x00000001
	.section .rom.00750a6e, "a"
	.incbin "baserom.gba", 0x00750a6e, 0x00000002
	.section .rom.00754277, "a"
	.incbin "baserom.gba", 0x00754277, 0x00000001
	.section .rom.007543eb, "a"
	.incbin "baserom.gba", 0x007543eb, 0x00000001
	.section .rom.007551b3, "a"
	.incbin "baserom.gba", 0x007551b3, 0x00000001
	.section .rom.00755359, "a"
	.incbin "baserom.gba", 0x00755359, 0x00000003
	.section .rom.007573b4, "a"
	.incbin "baserom.gba", 0x007573b4, 0x000000e4
	.section .rom.0075a1af, "a"
	.incbin "baserom.gba", 0x0075a1af, 0x00000001
	.section .rom.0075a396, "a"
	.incbin "baserom.gba", 0x0075a396, 0x00000002
	.section .rom.0075d632, "a"
	.incbin "baserom.gba", 0x0075d632, 0x00004602
	.section .rom.00761d3a, "a"
	.incbin "baserom.gba", 0x00761d3a, 0x00000002
	.section .rom.0076390d, "a"
	.incbin "baserom.gba", 0x0076390d, 0x00000003
	.section .rom.007653bd, "a"
	.incbin "baserom.gba", 0x007653bd, 0x00000003
	.section .rom.00765519, "a"
	.incbin "baserom.gba", 0x00765519, 0x00000003
	.section .rom.0076b75f, "a"
	.incbin "baserom.gba", 0x0076b75f, 0x00000001
	.section .rom.0076b863, "a"
	.incbin "baserom.gba", 0x0076b863, 0x00000001
	.section .rom.0076c27f, "a"
	.incbin "baserom.gba", 0x0076c27f, 0x00000001
	.section .rom.0076c365, "a"
	.incbin "baserom.gba", 0x0076c365, 0x00000003
	.section .rom.0076c601, "a"
	.incbin "baserom.gba", 0x0076c601, 0x00000003
	.section .rom.0077065e, "a"
	.incbin "baserom.gba", 0x0077065e, 0x00000002
	.section .rom.00770c12, "a"
	.incbin "baserom.gba", 0x00770c12, 0x00000002
	.section .rom.00771ba7, "a"
	.incbin "baserom.gba", 0x00771ba7, 0x00000001
	.section .rom.007735aa, "a"
	.incbin "baserom.gba", 0x007735aa, 0x00000002
	.section .rom.00774219, "a"
	.incbin "baserom.gba", 0x00774219, 0x00000003
	.section .rom.007744c1, "a"
	.incbin "baserom.gba", 0x007744c1, 0x00000003
	.section .rom.00775346, "a"
	.incbin "baserom.gba", 0x00775346, 0x00000002
	.section .rom.007764d7, "a"
	.incbin "baserom.gba", 0x007764d7, 0x00000001
	.section .rom.0077669f, "a"
	.incbin "baserom.gba", 0x0077669f, 0x00000001
	.section .rom.007769eb, "a"
	.incbin "baserom.gba", 0x007769eb, 0x0000051d
	.section .rom.00777253, "a"
	.incbin "baserom.gba", 0x00777253, 0x0000051d
	.section .rom.00777abb, "a"
	.incbin "baserom.gba", 0x00777abb, 0x0000051d
	.section .rom.00778323, "a"
	.incbin "baserom.gba", 0x00778323, 0x0000051d
	.section .rom.00778b8b, "a"
	.incbin "baserom.gba", 0x00778b8b, 0x0000051d
	.section .rom.007793f3, "a"
	.incbin "baserom.gba", 0x007793f3, 0x0000051d
	.section .rom.00779c5b, "a"
	.incbin "baserom.gba", 0x00779c5b, 0x0000051d
	.section .rom.0077a4c3, "a"
	.incbin "baserom.gba", 0x0077a4c3, 0x00085b3d
