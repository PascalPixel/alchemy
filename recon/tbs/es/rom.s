@ tbs-es's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.incbin "baserom.gba", 0x00014e68, 0x0001bbbc
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x00030a24, 0x00000a90
	.section .rom.000330b4, "a"
	.incbin "baserom.gba", 0x000330b4, 0x00003690
	.section .rom.00073368, "a"
	.incbin "baserom.gba", 0x00073368, 0x00023d78
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
	.section .rom.003388f5, "a"
	.incbin "baserom.gba", 0x003388f5, 0x00000003
	.section .rom.0033c3f9, "a"
	.incbin "baserom.gba", 0x0033c3f9, 0x0000071f
	.section .rom.00340f5a, "a"
	.incbin "baserom.gba", 0x00340f5a, 0x00000002
	.section .rom.003516ca, "a"
	.incbin "baserom.gba", 0x003516ca, 0x00000002
	.section .rom.00354cba, "a"
	.incbin "baserom.gba", 0x00354cba, 0x00000002
	.section .rom.0036075e, "a"
	.incbin "baserom.gba", 0x0036075e, 0x00000002
	.section .rom.00370e0e, "a"
	.incbin "baserom.gba", 0x00370e0e, 0x00000002
	.section .rom.003788ae, "a"
	.incbin "baserom.gba", 0x003788ae, 0x00000002
	.section .rom.0037c0ee, "a"
	.incbin "baserom.gba", 0x0037c0ee, 0x00000002
	.section .rom.00385522, "a"
	.incbin "baserom.gba", 0x00385522, 0x00000002
	.section .rom.0039094a, "a"
	.incbin "baserom.gba", 0x0039094a, 0x00000002
	.section .rom.00398806, "a"
	.incbin "baserom.gba", 0x00398806, 0x00000002
	.section .rom.0039d296, "a"
	.incbin "baserom.gba", 0x0039d296, 0x00000002
	.section .rom.003a154a, "a"
	.incbin "baserom.gba", 0x003a154a, 0x00000002
	.section .rom.003a4eca, "a"
	.incbin "baserom.gba", 0x003a4eca, 0x00000002
	.section .rom.003b1706, "a"
	.incbin "baserom.gba", 0x003b1706, 0x00000002
	.section .rom.003c004e, "a"
	.incbin "baserom.gba", 0x003c004e, 0x00000002
	.section .rom.003c5951, "a"
	.incbin "baserom.gba", 0x003c5951, 0x00000003
	.section .rom.003c72d3, "a"
	.incbin "baserom.gba", 0x003c72d3, 0x00000001
	.section .rom.003ca0f1, "a"
	.incbin "baserom.gba", 0x003ca0f1, 0x00000003
	.section .rom.003cd06e, "a"
	.incbin "baserom.gba", 0x003cd06e, 0x00000002
	.section .rom.003cd8b5, "a"
	.incbin "baserom.gba", 0x003cd8b5, 0x00000003
	.section .rom.003cf0cb, "a"
	.incbin "baserom.gba", 0x003cf0cb, 0x00000001
	.section .rom.003cf6b6, "a"
	.incbin "baserom.gba", 0x003cf6b6, 0x00000002
	.section .rom.003cfd7a, "a"
	.incbin "baserom.gba", 0x003cfd7a, 0x00000002
	.section .rom.003d0061, "a"
	.incbin "baserom.gba", 0x003d0061, 0x00000003
	.section .rom.003d13c7, "a"
	.incbin "baserom.gba", 0x003d13c7, 0x00000001
	.section .rom.003d17b1, "a"
	.incbin "baserom.gba", 0x003d17b1, 0x00000003
	.section .rom.003d1b83, "a"
	.incbin "baserom.gba", 0x003d1b83, 0x00000001
	.section .rom.003d2423, "a"
	.incbin "baserom.gba", 0x003d2423, 0x00000001
	.section .rom.003d28c6, "a"
	.incbin "baserom.gba", 0x003d28c6, 0x00000002
	.section .rom.003d3a7f, "a"
	.incbin "baserom.gba", 0x003d3a7f, 0x00000001
	.section .rom.003d4f42, "a"
	.incbin "baserom.gba", 0x003d4f42, 0x00000002
	.section .rom.003d5f0b, "a"
	.incbin "baserom.gba", 0x003d5f0b, 0x00000001
	.section .rom.003d6166, "a"
	.incbin "baserom.gba", 0x003d6166, 0x00000002
	.section .rom.003d7bc1, "a"
	.incbin "baserom.gba", 0x003d7bc1, 0x00000003
	.section .rom.003d810d, "a"
	.incbin "baserom.gba", 0x003d810d, 0x00000003
	.section .rom.003d9e96, "a"
	.incbin "baserom.gba", 0x003d9e96, 0x00000002
	.section .rom.003da10f, "a"
	.incbin "baserom.gba", 0x003da10f, 0x00000001
	.section .rom.003da5d6, "a"
	.incbin "baserom.gba", 0x003da5d6, 0x00000002
	.section .rom.003dc1ab, "a"
	.incbin "baserom.gba", 0x003dc1ab, 0x00000001
	.section .rom.003ddda9, "a"
	.incbin "baserom.gba", 0x003ddda9, 0x00000003
	.section .rom.003ddfca, "a"
	.incbin "baserom.gba", 0x003ddfca, 0x00000002
	.section .rom.003de407, "a"
	.incbin "baserom.gba", 0x003de407, 0x00000001
	.section .rom.003de519, "a"
	.incbin "baserom.gba", 0x003de519, 0x00000003
	.section .rom.003dec97, "a"
	.incbin "baserom.gba", 0x003dec97, 0x00000001
	.section .rom.003df135, "a"
	.incbin "baserom.gba", 0x003df135, 0x00000003
	.section .rom.003dfe4a, "a"
	.incbin "baserom.gba", 0x003dfe4a, 0x00000002
	.section .rom.003e0bca, "a"
	.incbin "baserom.gba", 0x003e0bca, 0x00000002
	.section .rom.003e0f5b, "a"
	.incbin "baserom.gba", 0x003e0f5b, 0x00000001
	.section .rom.003e2ca2, "a"
	.incbin "baserom.gba", 0x003e2ca2, 0x00000002
	.section .rom.003e3a79, "a"
	.incbin "baserom.gba", 0x003e3a79, 0x00000003
	.section .rom.003e42a6, "a"
	.incbin "baserom.gba", 0x003e42a6, 0x00000002
	.section .rom.003e4931, "a"
	.incbin "baserom.gba", 0x003e4931, 0x00000003
	.section .rom.003e4aef, "a"
	.incbin "baserom.gba", 0x003e4aef, 0x00000001
	.section .rom.003e5847, "a"
	.incbin "baserom.gba", 0x003e5847, 0x00000001
	.section .rom.003e5d93, "a"
	.incbin "baserom.gba", 0x003e5d93, 0x00000001
	.section .rom.003e616d, "a"
	.incbin "baserom.gba", 0x003e616d, 0x00000003
	.section .rom.003e651b, "a"
	.incbin "baserom.gba", 0x003e651b, 0x00000001
	.section .rom.003e84bf, "a"
	.incbin "baserom.gba", 0x003e84bf, 0x00000001
	.section .rom.003e925b, "a"
	.incbin "baserom.gba", 0x003e925b, 0x00000001
	.section .rom.003e9477, "a"
	.incbin "baserom.gba", 0x003e9477, 0x00000001
	.section .rom.003e9773, "a"
	.incbin "baserom.gba", 0x003e9773, 0x00000001
	.section .rom.003eb921, "a"
	.incbin "baserom.gba", 0x003eb921, 0x00000003
	.section .rom.003ec6ef, "a"
	.incbin "baserom.gba", 0x003ec6ef, 0x00000001
	.section .rom.003ed851, "a"
	.incbin "baserom.gba", 0x003ed851, 0x00000003
	.section .rom.003eddab, "a"
	.incbin "baserom.gba", 0x003eddab, 0x00000001
	.section .rom.003ef695, "a"
	.incbin "baserom.gba", 0x003ef695, 0x00000003
	.section .rom.003eff36, "a"
	.incbin "baserom.gba", 0x003eff36, 0x00000002
	.section .rom.003f0313, "a"
	.incbin "baserom.gba", 0x003f0313, 0x00000001
	.section .rom.003f059d, "a"
	.incbin "baserom.gba", 0x003f059d, 0x00000003
	.section .rom.003f0943, "a"
	.incbin "baserom.gba", 0x003f0943, 0x00000001
	.section .rom.003f0b9e, "a"
	.incbin "baserom.gba", 0x003f0b9e, 0x00000002
	.section .rom.003f0f56, "a"
	.incbin "baserom.gba", 0x003f0f56, 0x00000002
	.section .rom.003f243f, "a"
	.incbin "baserom.gba", 0x003f243f, 0x00000001
	.section .rom.003f3a02, "a"
	.incbin "baserom.gba", 0x003f3a02, 0x00000a6e
	.section .rom.003f5246, "a"
	.incbin "baserom.gba", 0x003f5246, 0x00000002
	.section .rom.003f55c9, "a"
	.incbin "baserom.gba", 0x003f55c9, 0x00000003
	.section .rom.003f6279, "a"
	.incbin "baserom.gba", 0x003f6279, 0x00000003
	.section .rom.003f6dc1, "a"
	.incbin "baserom.gba", 0x003f6dc1, 0x00000003
	.section .rom.003f6f5a, "a"
	.incbin "baserom.gba", 0x003f6f5a, 0x00000002
	.section .rom.003f77e7, "a"
	.incbin "baserom.gba", 0x003f77e7, 0x00000001
	.section .rom.003f82ae, "a"
	.incbin "baserom.gba", 0x003f82ae, 0x00000002
	.section .rom.003f87c6, "a"
	.incbin "baserom.gba", 0x003f87c6, 0x00000002
	.section .rom.003f8dea, "a"
	.incbin "baserom.gba", 0x003f8dea, 0x00000002
	.section .rom.003f9651, "a"
	.incbin "baserom.gba", 0x003f9651, 0x00000003
	.section .rom.003f9a75, "a"
	.incbin "baserom.gba", 0x003f9a75, 0x00000003
	.section .rom.003f9d0f, "a"
	.incbin "baserom.gba", 0x003f9d0f, 0x00000001
	.section .rom.003fb6ab, "a"
	.incbin "baserom.gba", 0x003fb6ab, 0x00000001
	.section .rom.003fd422, "a"
	.incbin "baserom.gba", 0x003fd422, 0x00000002
	.section .rom.003ff397, "a"
	.incbin "baserom.gba", 0x003ff397, 0x00000001
	.section .rom.003ff867, "a"
	.incbin "baserom.gba", 0x003ff867, 0x00000001
	.section .rom.003ffef9, "a"
	.incbin "baserom.gba", 0x003ffef9, 0x00000a37
	.section .rom.00401d01, "a"
	.incbin "baserom.gba", 0x00401d01, 0x00000003
	.section .rom.004027d2, "a"
	.incbin "baserom.gba", 0x004027d2, 0x00000002
	.section .rom.0040330f, "a"
	.incbin "baserom.gba", 0x0040330f, 0x00000001
	.section .rom.0040394f, "a"
	.incbin "baserom.gba", 0x0040394f, 0x00001589
	.section .rom.00404f39, "a"
	.incbin "baserom.gba", 0x00404f39, 0x00000003
	.section .rom.004052a7, "a"
	.incbin "baserom.gba", 0x004052a7, 0x00000001
	.section .rom.00405845, "a"
	.incbin "baserom.gba", 0x00405845, 0x00000003
	.section .rom.00405b79, "a"
	.incbin "baserom.gba", 0x00405b79, 0x00000003
	.section .rom.00405eb5, "a"
	.incbin "baserom.gba", 0x00405eb5, 0x00000003
	.section .rom.00406ece, "a"
	.incbin "baserom.gba", 0x00406ece, 0x00000002
	.section .rom.0040a4da, "a"
	.incbin "baserom.gba", 0x0040a4da, 0x00000002
	.section .rom.0040ac7d, "a"
	.incbin "baserom.gba", 0x0040ac7d, 0x00000003
	.section .rom.0040c00f, "a"
	.incbin "baserom.gba", 0x0040c00f, 0x00000001
	.section .rom.0040c43f, "a"
	.incbin "baserom.gba", 0x0040c43f, 0x00000001
	.section .rom.0040e235, "a"
	.incbin "baserom.gba", 0x0040e235, 0x00000003
	.section .rom.0040eb4a, "a"
	.incbin "baserom.gba", 0x0040eb4a, 0x00000002
	.section .rom.0041067e, "a"
	.incbin "baserom.gba", 0x0041067e, 0x00000002
	.section .rom.004116cd, "a"
	.incbin "baserom.gba", 0x004116cd, 0x00000003
	.section .rom.00411d83, "a"
	.incbin "baserom.gba", 0x00411d83, 0x00000001
	.section .rom.00412e29, "a"
	.incbin "baserom.gba", 0x00412e29, 0x00000003
	.section .rom.0042623b, "a"
	.incbin "baserom.gba", 0x0042623b, 0x00000001
	.section .rom.0042638d, "a"
	.incbin "baserom.gba", 0x0042638d, 0x00000003
	.section .rom.0042681e, "a"
	.incbin "baserom.gba", 0x0042681e, 0x00000002
	.section .rom.00426a15, "a"
	.incbin "baserom.gba", 0x00426a15, 0x00000003
	.section .rom.004280d2, "a"
	.incbin "baserom.gba", 0x004280d2, 0x00000002
	.section .rom.004295f1, "a"
	.incbin "baserom.gba", 0x004295f1, 0x00000003
	.section .rom.0042a1bd, "a"
	.incbin "baserom.gba", 0x0042a1bd, 0x00000003
	.section .rom.0042ad32, "a"
	.incbin "baserom.gba", 0x0042ad32, 0x00000002
	.section .rom.0042cbdb, "a"
	.incbin "baserom.gba", 0x0042cbdb, 0x00000001
	.section .rom.0042e369, "a"
	.incbin "baserom.gba", 0x0042e369, 0x00000003
	.section .rom.0042f81a, "a"
	.incbin "baserom.gba", 0x0042f81a, 0x00000002
	.section .rom.00431f67, "a"
	.incbin "baserom.gba", 0x00431f67, 0x00000001
	.section .rom.00433801, "a"
	.incbin "baserom.gba", 0x00433801, 0x00000003
	.section .rom.00434dca, "a"
	.incbin "baserom.gba", 0x00434dca, 0x00000002
	.section .rom.0043685a, "a"
	.incbin "baserom.gba", 0x0043685a, 0x00000002
	.section .rom.00441032, "a"
	.incbin "baserom.gba", 0x00441032, 0x00000002
	.section .rom.004431e6, "a"
	.incbin "baserom.gba", 0x004431e6, 0x00000002
	.section .rom.0044734d, "a"
	.incbin "baserom.gba", 0x0044734d, 0x00000003
	.section .rom.0044a572, "a"
	.incbin "baserom.gba", 0x0044a572, 0x00000002
	.section .rom.0044bc76, "a"
	.incbin "baserom.gba", 0x0044bc76, 0x00000002
	.section .rom.0045285e, "a"
	.incbin "baserom.gba", 0x0045285e, 0x00000002
	.section .rom.0045b1e2, "a"
	.incbin "baserom.gba", 0x0045b1e2, 0x00000002
	.section .rom.00462cbe, "a"
	.incbin "baserom.gba", 0x00462cbe, 0x00000002
	.section .rom.00465d52, "a"
	.incbin "baserom.gba", 0x00465d52, 0x00000002
	.section .rom.00469161, "a"
	.incbin "baserom.gba", 0x00469161, 0x00000003
	.section .rom.0046b9cd, "a"
	.incbin "baserom.gba", 0x0046b9cd, 0x00000003
	.section .rom.0046d4be, "a"
	.incbin "baserom.gba", 0x0046d4be, 0x00000002
	.section .rom.0046e2d6, "a"
	.incbin "baserom.gba", 0x0046e2d6, 0x00000002
	.section .rom.0046efc1, "a"
	.incbin "baserom.gba", 0x0046efc1, 0x00000003
	.section .rom.0047847e, "a"
	.incbin "baserom.gba", 0x0047847e, 0x00000002
	.section .rom.00479189, "a"
	.incbin "baserom.gba", 0x00479189, 0x00000003
	.section .rom.0047b29d, "a"
	.incbin "baserom.gba", 0x0047b29d, 0x00000003
	.section .rom.0047bf0f, "a"
	.incbin "baserom.gba", 0x0047bf0f, 0x00000001
	.section .rom.0047cb27, "a"
	.incbin "baserom.gba", 0x0047cb27, 0x00000001
	.section .rom.0047d29b, "a"
	.incbin "baserom.gba", 0x0047d29b, 0x00000001
	.section .rom.0047eb2b, "a"
	.incbin "baserom.gba", 0x0047eb2b, 0x00000001
	.section .rom.0047f4f9, "a"
	.incbin "baserom.gba", 0x0047f4f9, 0x00000003
	.section .rom.00480117, "a"
	.incbin "baserom.gba", 0x00480117, 0x00000001
	.section .rom.004827cb, "a"
	.incbin "baserom.gba", 0x004827cb, 0x00000001
	.section .rom.00484ede, "a"
	.incbin "baserom.gba", 0x00484ede, 0x00000002
	.section .rom.00489e33, "a"
	.incbin "baserom.gba", 0x00489e33, 0x00000001
	.section .rom.0048e15d, "a"
	.incbin "baserom.gba", 0x0048e15d, 0x00000003
	.section .rom.0048ed4a, "a"
	.incbin "baserom.gba", 0x0048ed4a, 0x00000002
	.section .rom.004938ff, "a"
	.incbin "baserom.gba", 0x004938ff, 0x00000001
	.section .rom.00495c69, "a"
	.incbin "baserom.gba", 0x00495c69, 0x00000003
	.section .rom.0049760b, "a"
	.incbin "baserom.gba", 0x0049760b, 0x00000001
	.section .rom.0049becd, "a"
	.incbin "baserom.gba", 0x0049becd, 0x00000003
	.section .rom.0049ff95, "a"
	.incbin "baserom.gba", 0x0049ff95, 0x00000003
	.section .rom.004a365e, "a"
	.incbin "baserom.gba", 0x004a365e, 0x00000002
	.section .rom.004ab62f, "a"
	.incbin "baserom.gba", 0x004ab62f, 0x00000001
	.section .rom.004b293f, "a"
	.incbin "baserom.gba", 0x004b293f, 0x00000001
	.section .rom.004b78bf, "a"
	.incbin "baserom.gba", 0x004b78bf, 0x00000001
	.section .rom.004bc26f, "a"
	.incbin "baserom.gba", 0x004bc26f, 0x0000051d
	.section .rom.004c1b9b, "a"
	.incbin "baserom.gba", 0x004c1b9b, 0x00000001
	.section .rom.004c1d29, "a"
	.incbin "baserom.gba", 0x004c1d29, 0x00000003
	.section .rom.004c6d89, "a"
	.incbin "baserom.gba", 0x004c6d89, 0x00000003
	.section .rom.004cb2f5, "a"
	.incbin "baserom.gba", 0x004cb2f5, 0x00000003
	.section .rom.004d04b9, "a"
	.incbin "baserom.gba", 0x004d04b9, 0x00000003
	.section .rom.004d064f, "a"
	.incbin "baserom.gba", 0x004d064f, 0x00000001
	.section .rom.004d32a3, "a"
	.incbin "baserom.gba", 0x004d32a3, 0x00000001
	.section .rom.004d9f57, "a"
	.incbin "baserom.gba", 0x004d9f57, 0x00000001
	.section .rom.004dd092, "a"
	.incbin "baserom.gba", 0x004dd092, 0x00000002
	.section .rom.004dd213, "a"
	.incbin "baserom.gba", 0x004dd213, 0x00000001
	.section .rom.004df9cd, "a"
	.incbin "baserom.gba", 0x004df9cd, 0x00000003
	.section .rom.004e2016, "a"
	.incbin "baserom.gba", 0x004e2016, 0x00000002
	.section .rom.004e46fa, "a"
	.incbin "baserom.gba", 0x004e46fa, 0x00000002
	.section .rom.004e576f, "a"
	.incbin "baserom.gba", 0x004e576f, 0x00000001
	.section .rom.004e78c2, "a"
	.incbin "baserom.gba", 0x004e78c2, 0x00000002
	.section .rom.004e7a55, "a"
	.incbin "baserom.gba", 0x004e7a55, 0x00000003
	.section .rom.004ea297, "a"
	.incbin "baserom.gba", 0x004ea297, 0x00000001
	.section .rom.004ec0ad, "a"
	.incbin "baserom.gba", 0x004ec0ad, 0x00000003
	.section .rom.004ee78e, "a"
	.incbin "baserom.gba", 0x004ee78e, 0x00000002
	.section .rom.004ef8d5, "a"
	.incbin "baserom.gba", 0x004ef8d5, 0x00000003
	.section .rom.004f29e9, "a"
	.incbin "baserom.gba", 0x004f29e9, 0x00000003
	.section .rom.004f6b51, "a"
	.incbin "baserom.gba", 0x004f6b51, 0x00000003
	.section .rom.004f814e, "a"
	.incbin "baserom.gba", 0x004f814e, 0x00000002
	.section .rom.004f9417, "a"
	.incbin "baserom.gba", 0x004f9417, 0x00000001
	.section .rom.004fc82f, "a"
	.incbin "baserom.gba", 0x004fc82f, 0x00000001
	.section .rom.004fc9bd, "a"
	.incbin "baserom.gba", 0x004fc9bd, 0x00000003
	.section .rom.004fe76e, "a"
	.incbin "baserom.gba", 0x004fe76e, 0x00000002
	.section .rom.005022e5, "a"
	.incbin "baserom.gba", 0x005022e5, 0x00000003
	.section .rom.005037f3, "a"
	.incbin "baserom.gba", 0x005037f3, 0x00000001
	.section .rom.005043c7, "a"
	.incbin "baserom.gba", 0x005043c7, 0x00000001
	.section .rom.005044c5, "a"
	.incbin "baserom.gba", 0x005044c5, 0x00000003
	.section .rom.005058aa, "a"
	.incbin "baserom.gba", 0x005058aa, 0x00000002
	.section .rom.00506a2d, "a"
	.incbin "baserom.gba", 0x00506a2d, 0x00000003
	.section .rom.005073b6, "a"
	.incbin "baserom.gba", 0x005073b6, 0x00000002
	.section .rom.0050a033, "a"
	.incbin "baserom.gba", 0x0050a033, 0x00000001
	.section .rom.0050a116, "a"
	.incbin "baserom.gba", 0x0050a116, 0x00000002
	.section .rom.0050b301, "a"
	.incbin "baserom.gba", 0x0050b301, 0x00000003
	.section .rom.0050cf89, "a"
	.incbin "baserom.gba", 0x0050cf89, 0x00000003
	.section .rom.0050d497, "a"
	.incbin "baserom.gba", 0x0050d497, 0x00000001
	.section .rom.0050d5d7, "a"
	.incbin "baserom.gba", 0x0050d5d7, 0x00000001
	.section .rom.0050e7f2, "a"
	.incbin "baserom.gba", 0x0050e7f2, 0x00000002
	.section .rom.0050e94d, "a"
	.incbin "baserom.gba", 0x0050e94d, 0x00000003
	.section .rom.00512d73, "a"
	.incbin "baserom.gba", 0x00512d73, 0x00000001
	.section .rom.00514aa2, "a"
	.incbin "baserom.gba", 0x00514aa2, 0x00000002
	.section .rom.0051581b, "a"
	.incbin "baserom.gba", 0x0051581b, 0x00000001
	.section .rom.0051594f, "a"
	.incbin "baserom.gba", 0x0051594f, 0x00000001
	.section .rom.00517dd2, "a"
	.incbin "baserom.gba", 0x00517dd2, 0x00000002
	.section .rom.00518b4f, "a"
	.incbin "baserom.gba", 0x00518b4f, 0x00000001
	.section .rom.0051962e, "a"
	.incbin "baserom.gba", 0x0051962e, 0x00000002
	.section .rom.0051a4a6, "a"
	.incbin "baserom.gba", 0x0051a4a6, 0x00000002
	.section .rom.0051c57a, "a"
	.incbin "baserom.gba", 0x0051c57a, 0x00000002
	.section .rom.0051cec1, "a"
	.incbin "baserom.gba", 0x0051cec1, 0x00000003
	.section .rom.0051db0e, "a"
	.incbin "baserom.gba", 0x0051db0e, 0x00000002
	.section .rom.0051dd35, "a"
	.incbin "baserom.gba", 0x0051dd35, 0x00000003
	.section .rom.0051f677, "a"
	.incbin "baserom.gba", 0x0051f677, 0x00000001
	.section .rom.0051f78b, "a"
	.incbin "baserom.gba", 0x0051f78b, 0x00000001
	.section .rom.00521151, "a"
	.incbin "baserom.gba", 0x00521151, 0x00000003
	.section .rom.0052129b, "a"
	.incbin "baserom.gba", 0x0052129b, 0x00000001
	.section .rom.00523367, "a"
	.incbin "baserom.gba", 0x00523367, 0x00000001
	.section .rom.0052348f, "a"
	.incbin "baserom.gba", 0x0052348f, 0x00000001
	.section .rom.00524d17, "a"
	.incbin "baserom.gba", 0x00524d17, 0x00000001
	.section .rom.00526fb9, "a"
	.incbin "baserom.gba", 0x00526fb9, 0x00000003
	.section .rom.005270fb, "a"
	.incbin "baserom.gba", 0x005270fb, 0x00000001
	.section .rom.00529103, "a"
	.incbin "baserom.gba", 0x00529103, 0x00000001
	.section .rom.0052920e, "a"
	.incbin "baserom.gba", 0x0052920e, 0x00000002
	.section .rom.0052b1f2, "a"
	.incbin "baserom.gba", 0x0052b1f2, 0x00000002
	.section .rom.0052c917, "a"
	.incbin "baserom.gba", 0x0052c917, 0x00000001
	.section .rom.0052d9cb, "a"
	.incbin "baserom.gba", 0x0052d9cb, 0x00000001
	.section .rom.0052f6b2, "a"
	.incbin "baserom.gba", 0x0052f6b2, 0x00000002
	.section .rom.0052f80d, "a"
	.incbin "baserom.gba", 0x0052f80d, 0x00000003
	.section .rom.00531756, "a"
	.incbin "baserom.gba", 0x00531756, 0x00000002
	.section .rom.005318db, "a"
	.incbin "baserom.gba", 0x005318db, 0x00000001
	.section .rom.0053442d, "a"
	.incbin "baserom.gba", 0x0053442d, 0x00000003
	.section .rom.00536af2, "a"
	.incbin "baserom.gba", 0x00536af2, 0x00000002
	.section .rom.0053924a, "a"
	.incbin "baserom.gba", 0x0053924a, 0x00000002
	.section .rom.0053aa9d, "a"
	.incbin "baserom.gba", 0x0053aa9d, 0x00000003
	.section .rom.0053d19d, "a"
	.incbin "baserom.gba", 0x0053d19d, 0x00000003
	.section .rom.0053ef69, "a"
	.incbin "baserom.gba", 0x0053ef69, 0x00000003
	.section .rom.005414b3, "a"
	.incbin "baserom.gba", 0x005414b3, 0x00000001
	.section .rom.0054277e, "a"
	.incbin "baserom.gba", 0x0054277e, 0x00000002
	.section .rom.0054342f, "a"
	.incbin "baserom.gba", 0x0054342f, 0x00000001
	.section .rom.00543e99, "a"
	.incbin "baserom.gba", 0x00543e99, 0x00000003
	.section .rom.00543f8f, "a"
	.incbin "baserom.gba", 0x00543f8f, 0x00000001
	.section .rom.005459ee, "a"
	.incbin "baserom.gba", 0x005459ee, 0x00000002
	.section .rom.00546792, "a"
	.incbin "baserom.gba", 0x00546792, 0x00000002
	.section .rom.005482cb, "a"
	.incbin "baserom.gba", 0x005482cb, 0x00000001
	.section .rom.0054b1b9, "a"
	.incbin "baserom.gba", 0x0054b1b9, 0x00000003
	.section .rom.0055299f, "a"
	.incbin "baserom.gba", 0x0055299f, 0x00000001
	.section .rom.00552a6b, "a"
	.incbin "baserom.gba", 0x00552a6b, 0x00000001
	.section .rom.0055822d, "a"
	.incbin "baserom.gba", 0x0055822d, 0x00000003
	.section .rom.0055836f, "a"
	.incbin "baserom.gba", 0x0055836f, 0x00000001
	.section .rom.0055979d, "a"
	.incbin "baserom.gba", 0x0055979d, 0x00000003
	.section .rom.0055c4da, "a"
	.incbin "baserom.gba", 0x0055c4da, 0x00000002
	.section .rom.0055e5d6, "a"
	.incbin "baserom.gba", 0x0055e5d6, 0x00000002
	.section .rom.00565fe2, "a"
	.incbin "baserom.gba", 0x00565fe2, 0x00000002
	.section .rom.0056617e, "a"
	.incbin "baserom.gba", 0x0056617e, 0x00000002
	.section .rom.0056aa19, "a"
	.incbin "baserom.gba", 0x0056aa19, 0x00000003
	.section .rom.0056cf1a, "a"
	.incbin "baserom.gba", 0x0056cf1a, 0x00000002
	.section .rom.0057490b, "a"
	.incbin "baserom.gba", 0x0057490b, 0x00000001
	.section .rom.00574aa6, "a"
	.incbin "baserom.gba", 0x00574aa6, 0x00000002
	.section .rom.00577655, "a"
	.incbin "baserom.gba", 0x00577655, 0x00000003
	.section .rom.00579729, "a"
	.incbin "baserom.gba", 0x00579729, 0x00000003
	.section .rom.0057b782, "a"
	.incbin "baserom.gba", 0x0057b782, 0x00001cee
	.section .rom.0057ec19, "a"
	.incbin "baserom.gba", 0x0057ec19, 0x00000003
	.section .rom.005818f5, "a"
	.incbin "baserom.gba", 0x005818f5, 0x00000003
	.section .rom.005836c7, "a"
	.incbin "baserom.gba", 0x005836c7, 0x00000001
	.section .rom.00583807, "a"
	.incbin "baserom.gba", 0x00583807, 0x00000001
	.section .rom.00586861, "a"
	.incbin "baserom.gba", 0x00586861, 0x00000003
	.section .rom.005897de, "a"
	.incbin "baserom.gba", 0x005897de, 0x00000002
	.section .rom.0058c0a5, "a"
	.incbin "baserom.gba", 0x0058c0a5, 0x00000003
	.section .rom.0059300d, "a"
	.incbin "baserom.gba", 0x0059300d, 0x00000003
	.section .rom.005948b3, "a"
	.incbin "baserom.gba", 0x005948b3, 0x00000001
	.section .rom.005951b2, "a"
	.incbin "baserom.gba", 0x005951b2, 0x00000002
	.section .rom.00595f2b, "a"
	.incbin "baserom.gba", 0x00595f2b, 0x00000001
	.section .rom.0059608b, "a"
	.incbin "baserom.gba", 0x0059608b, 0x00000001
	.section .rom.00597ce3, "a"
	.incbin "baserom.gba", 0x00597ce3, 0x00000001
	.section .rom.0059957b, "a"
	.incbin "baserom.gba", 0x0059957b, 0x00000001
	.section .rom.005996ff, "a"
	.incbin "baserom.gba", 0x005996ff, 0x00000001
	.section .rom.0059c23b, "a"
	.incbin "baserom.gba", 0x0059c23b, 0x00000001
	.section .rom.0059e9b5, "a"
	.incbin "baserom.gba", 0x0059e9b5, 0x00000003
	.section .rom.0059f9a2, "a"
	.incbin "baserom.gba", 0x0059f9a2, 0x00000002
	.section .rom.005a1063, "a"
	.incbin "baserom.gba", 0x005a1063, 0x00000001
	.section .rom.005a120d, "a"
	.incbin "baserom.gba", 0x005a120d, 0x00000003
	.section .rom.005a3fa1, "a"
	.incbin "baserom.gba", 0x005a3fa1, 0x00000003
	.section .rom.005a8036, "a"
	.incbin "baserom.gba", 0x005a8036, 0x00000002
	.section .rom.005a8177, "a"
	.incbin "baserom.gba", 0x005a8177, 0x00000001
	.section .rom.005aa03f, "a"
	.incbin "baserom.gba", 0x005aa03f, 0x00000001
	.section .rom.005aa1e1, "a"
	.incbin "baserom.gba", 0x005aa1e1, 0x00000003
	.section .rom.005ac283, "a"
	.incbin "baserom.gba", 0x005ac283, 0x00000001
	.section .rom.005ad210, "a"
	.incbin "baserom.gba", 0x005ad210, 0x000022d8
	.section .rom.005b1fd9, "a"
	.incbin "baserom.gba", 0x005b1fd9, 0x00000003
	.section .rom.005b215e, "a"
	.incbin "baserom.gba", 0x005b215e, 0x00000002
	.section .rom.005b98ab, "a"
	.incbin "baserom.gba", 0x005b98ab, 0x00000001
	.section .rom.005b9db5, "a"
	.incbin "baserom.gba", 0x005b9db5, 0x00000003
	.section .rom.005bb48d, "a"
	.incbin "baserom.gba", 0x005bb48d, 0x00000003
	.section .rom.005bb631, "a"
	.incbin "baserom.gba", 0x005bb631, 0x00000003
	.section .rom.005bcf36, "a"
	.incbin "baserom.gba", 0x005bcf36, 0x00000002
	.section .rom.005be763, "a"
	.incbin "baserom.gba", 0x005be763, 0x00000001
	.section .rom.005be8e1, "a"
	.incbin "baserom.gba", 0x005be8e1, 0x00000003
	.section .rom.005c31d5, "a"
	.incbin "baserom.gba", 0x005c31d5, 0x00000003
	.section .rom.005c421a, "a"
	.incbin "baserom.gba", 0x005c421a, 0x00000002
	.section .rom.005c503e, "a"
	.incbin "baserom.gba", 0x005c503e, 0x00000002
	.section .rom.005c6383, "a"
	.incbin "baserom.gba", 0x005c6383, 0x00000001
	.section .rom.005c64dd, "a"
	.incbin "baserom.gba", 0x005c64dd, 0x00000003
	.section .rom.005c8a9b, "a"
	.incbin "baserom.gba", 0x005c8a9b, 0x00000001
	.section .rom.005c8c12, "a"
	.incbin "baserom.gba", 0x005c8c12, 0x00000002
	.section .rom.005cd505, "a"
	.incbin "baserom.gba", 0x005cd505, 0x00000003
	.section .rom.005cdf6d, "a"
	.incbin "baserom.gba", 0x005cdf6d, 0x00000003
	.section .rom.005d05af, "a"
	.incbin "baserom.gba", 0x005d05af, 0x00000001
	.section .rom.005d0716, "a"
	.incbin "baserom.gba", 0x005d0716, 0x00000002
	.section .rom.005d185d, "a"
	.incbin "baserom.gba", 0x005d185d, 0x00000003
	.section .rom.005d35bb, "a"
	.incbin "baserom.gba", 0x005d35bb, 0x00000001
	.section .rom.005d3722, "a"
	.incbin "baserom.gba", 0x005d3722, 0x00000002
	.section .rom.005d5fb1, "a"
	.incbin "baserom.gba", 0x005d5fb1, 0x00000003
	.section .rom.005d60e6, "a"
	.incbin "baserom.gba", 0x005d60e6, 0x00000002
	.section .rom.005d8d22, "a"
	.incbin "baserom.gba", 0x005d8d22, 0x00000002
	.section .rom.005dc4c1, "a"
	.incbin "baserom.gba", 0x005dc4c1, 0x00000003
	.section .rom.005df80f, "a"
	.incbin "baserom.gba", 0x005df80f, 0x00000001
	.section .rom.005df98b, "a"
	.incbin "baserom.gba", 0x005df98b, 0x00000001
	.section .rom.005e253a, "a"
	.incbin "baserom.gba", 0x005e253a, 0x00000002
	.section .rom.005e47ae, "a"
	.incbin "baserom.gba", 0x005e47ae, 0x00000002
	.section .rom.005e701f, "a"
	.incbin "baserom.gba", 0x005e701f, 0x00000001
	.section .rom.005ea513, "a"
	.incbin "baserom.gba", 0x005ea513, 0x00000001
	.section .rom.005ef13a, "a"
	.incbin "baserom.gba", 0x005ef13a, 0x00000002
	.section .rom.005efdbd, "a"
	.incbin "baserom.gba", 0x005efdbd, 0x00000003
	.section .rom.005efeff, "a"
	.incbin "baserom.gba", 0x005efeff, 0x00000001
	.section .rom.005f146f, "a"
	.incbin "baserom.gba", 0x005f146f, 0x00000001
	.section .rom.005f15da, "a"
	.incbin "baserom.gba", 0x005f15da, 0x00000002
	.section .rom.005f3335, "a"
	.incbin "baserom.gba", 0x005f3335, 0x00000003
	.section .rom.005f3477, "a"
	.incbin "baserom.gba", 0x005f3477, 0x00000001
	.section .rom.005f5c76, "a"
	.incbin "baserom.gba", 0x005f5c76, 0x00000002
	.section .rom.005f5db7, "a"
	.incbin "baserom.gba", 0x005f5db7, 0x00000001
	.section .rom.005f77cf, "a"
	.incbin "baserom.gba", 0x005f77cf, 0x00000001
	.section .rom.005f8d4a, "a"
	.incbin "baserom.gba", 0x005f8d4a, 0x00000002
	.section .rom.005fa286, "a"
	.incbin "baserom.gba", 0x005fa286, 0x00000002
	.section .rom.005fa416, "a"
	.incbin "baserom.gba", 0x005fa416, 0x00000002
	.section .rom.005fc6cf, "a"
	.incbin "baserom.gba", 0x005fc6cf, 0x00000001
	.section .rom.00600916, "a"
	.incbin "baserom.gba", 0x00600916, 0x00000002
	.section .rom.0060270d, "a"
	.incbin "baserom.gba", 0x0060270d, 0x00000003
	.section .rom.006058bb, "a"
	.incbin "baserom.gba", 0x006058bb, 0x00000001
	.section .rom.00607eef, "a"
	.incbin "baserom.gba", 0x00607eef, 0x00000001
	.section .rom.0060aa47, "a"
	.incbin "baserom.gba", 0x0060aa47, 0x00000001
	.section .rom.0060ac0f, "a"
	.incbin "baserom.gba", 0x0060ac0f, 0x00002905
	.section .rom.00611019, "a"
	.incbin "baserom.gba", 0x00611019, 0x00000003
	.section .rom.0061258a, "a"
	.incbin "baserom.gba", 0x0061258a, 0x00000002
	.section .rom.00614db1, "a"
	.incbin "baserom.gba", 0x00614db1, 0x00000003
	.section .rom.00614f21, "a"
	.incbin "baserom.gba", 0x00614f21, 0x00000003
	.section .rom.006177e3, "a"
	.incbin "baserom.gba", 0x006177e3, 0x00000001
	.section .rom.00619fab, "a"
	.incbin "baserom.gba", 0x00619fab, 0x00000001
	.section .rom.0061b2a5, "a"
	.incbin "baserom.gba", 0x0061b2a5, 0x00000003
	.section .rom.0061cbeb, "a"
	.incbin "baserom.gba", 0x0061cbeb, 0x00000001
	.section .rom.0062807e, "a"
	.incbin "baserom.gba", 0x0062807e, 0x00000002
	.section .rom.0062823e, "a"
	.incbin "baserom.gba", 0x0062823e, 0x00000002
	.section .rom.0062b5a5, "a"
	.incbin "baserom.gba", 0x0062b5a5, 0x00000003
	.section .rom.0062da96, "a"
	.incbin "baserom.gba", 0x0062da96, 0x00000002
	.section .rom.00630877, "a"
	.incbin "baserom.gba", 0x00630877, 0x00000001
	.section .rom.00631411, "a"
	.incbin "baserom.gba", 0x00631411, 0x00000003
	.section .rom.00632272, "a"
	.incbin "baserom.gba", 0x00632272, 0x00000002
	.section .rom.006323ca, "a"
	.incbin "baserom.gba", 0x006323ca, 0x00000002
	.section .rom.0063a217, "a"
	.incbin "baserom.gba", 0x0063a217, 0x00000001
	.section .rom.0063a383, "a"
	.incbin "baserom.gba", 0x0063a383, 0x00000001
	.section .rom.0063d11f, "a"
	.incbin "baserom.gba", 0x0063d11f, 0x00000001
	.section .rom.0063f2ae, "a"
	.incbin "baserom.gba", 0x0063f2ae, 0x00000002
	.section .rom.0063f9b6, "a"
	.incbin "baserom.gba", 0x0063f9b6, 0x00000002
	.section .rom.006405ca, "a"
	.incbin "baserom.gba", 0x006405ca, 0x00000002
	.section .rom.0065169d, "a"
	.incbin "baserom.gba", 0x0065169d, 0x00000003
	.section .rom.00651832, "a"
	.incbin "baserom.gba", 0x00651832, 0x00000002
	.section .rom.00652d42, "a"
	.incbin "baserom.gba", 0x00652d42, 0x00000002
	.section .rom.00654b27, "a"
	.incbin "baserom.gba", 0x00654b27, 0x00000001
	.section .rom.00655bba, "a"
	.incbin "baserom.gba", 0x00655bba, 0x00000002
	.section .rom.00657e2a, "a"
	.incbin "baserom.gba", 0x00657e2a, 0x00000002
	.section .rom.00657fc1, "a"
	.incbin "baserom.gba", 0x00657fc1, 0x00000003
	.section .rom.00659f67, "a"
	.incbin "baserom.gba", 0x00659f67, 0x00000001
	.section .rom.0065ed3e, "a"
	.incbin "baserom.gba", 0x0065ed3e, 0x00000002
	.section .rom.0066279f, "a"
	.incbin "baserom.gba", 0x0066279f, 0x00000001
	.section .rom.006654d5, "a"
	.incbin "baserom.gba", 0x006654d5, 0x00000003
	.section .rom.0066781d, "a"
	.incbin "baserom.gba", 0x0066781d, 0x00000003
	.section .rom.00668ffb, "a"
	.incbin "baserom.gba", 0x00668ffb, 0x00000001
	.section .rom.006691ad, "a"
	.incbin "baserom.gba", 0x006691ad, 0x00000003
	.section .rom.00679b4e, "a"
	.incbin "baserom.gba", 0x00679b4e, 0x00000002
	.section .rom.00679cad, "a"
	.incbin "baserom.gba", 0x00679cad, 0x00000003
	.section .rom.0067d407, "a"
	.incbin "baserom.gba", 0x0067d407, 0x00000001
	.section .rom.0067d56a, "a"
	.incbin "baserom.gba", 0x0067d56a, 0x00000002
	.section .rom.0068324d, "a"
	.incbin "baserom.gba", 0x0068324d, 0x00000003
	.section .rom.00685c33, "a"
	.incbin "baserom.gba", 0x00685c33, 0x00000001
	.section .rom.00687e3e, "a"
	.incbin "baserom.gba", 0x00687e3e, 0x00000002
	.section .rom.00687f7f, "a"
	.incbin "baserom.gba", 0x00687f7f, 0x00000001
	.section .rom.00689996, "a"
	.incbin "baserom.gba", 0x00689996, 0x00000002
	.section .rom.006906fd, "a"
	.incbin "baserom.gba", 0x006906fd, 0x00000003
	.section .rom.0069087b, "a"
	.incbin "baserom.gba", 0x0069087b, 0x00000001
	.section .rom.00692825, "a"
	.incbin "baserom.gba", 0x00692825, 0x00000003
	.section .rom.00694f9e, "a"
	.incbin "baserom.gba", 0x00694f9e, 0x00000002
	.section .rom.00696805, "a"
	.incbin "baserom.gba", 0x00696805, 0x00000003
	.section .rom.0069a226, "a"
	.incbin "baserom.gba", 0x0069a226, 0x00000002
	.section .rom.0069a3ef, "a"
	.incbin "baserom.gba", 0x0069a3ef, 0x00000001
	.section .rom.006a53d7, "a"
	.incbin "baserom.gba", 0x006a53d7, 0x00000001
	.section .rom.006a7523, "a"
	.incbin "baserom.gba", 0x006a7523, 0x00000001
	.section .rom.006a76ad, "a"
	.incbin "baserom.gba", 0x006a76ad, 0x00000003
	.section .rom.006a9132, "a"
	.incbin "baserom.gba", 0x006a9132, 0x00000002
	.section .rom.006ac169, "a"
	.incbin "baserom.gba", 0x006ac169, 0x00000003
	.section .rom.006ac32f, "a"
	.incbin "baserom.gba", 0x006ac32f, 0x00000001
	.section .rom.006ad263, "a"
	.incbin "baserom.gba", 0x006ad263, 0x00000001
	.section .rom.006af316, "a"
	.incbin "baserom.gba", 0x006af316, 0x00000002
	.section .rom.006af4bd, "a"
	.incbin "baserom.gba", 0x006af4bd, 0x00000003
	.section .rom.006b44e7, "a"
	.incbin "baserom.gba", 0x006b44e7, 0x00000001
	.section .rom.006b7277, "a"
	.incbin "baserom.gba", 0x006b7277, 0x00000001
	.section .rom.006b7437, "a"
	.incbin "baserom.gba", 0x006b7437, 0x00000001
	.section .rom.006b992b, "a"
	.incbin "baserom.gba", 0x006b992b, 0x00000001
	.section .rom.006b9afd, "a"
	.incbin "baserom.gba", 0x006b9afd, 0x00000003
	.section .rom.006bbb8e, "a"
	.incbin "baserom.gba", 0x006bbb8e, 0x00000002
	.section .rom.006bdc8e, "a"
	.incbin "baserom.gba", 0x006bdc8e, 0x00000002
	.section .rom.006c0842, "a"
	.incbin "baserom.gba", 0x006c0842, 0x00000002
	.section .rom.006c132f, "a"
	.incbin "baserom.gba", 0x006c132f, 0x00000001
	.section .rom.006c1502, "a"
	.incbin "baserom.gba", 0x006c1502, 0x00000002
	.section .rom.006c3e31, "a"
	.incbin "baserom.gba", 0x006c3e31, 0x00000003
	.section .rom.006c55c9, "a"
	.incbin "baserom.gba", 0x006c55c9, 0x00000003
	.section .rom.006c683d, "a"
	.incbin "baserom.gba", 0x006c683d, 0x00000003
	.section .rom.006c8ccd, "a"
	.incbin "baserom.gba", 0x006c8ccd, 0x00000003
	.section .rom.006cb019, "a"
	.incbin "baserom.gba", 0x006cb019, 0x00000003
	.section .rom.006cc203, "a"
	.incbin "baserom.gba", 0x006cc203, 0x00000001
	.section .rom.006cc38a, "a"
	.incbin "baserom.gba", 0x006cc38a, 0x00000002
	.section .rom.006d08d6, "a"
	.incbin "baserom.gba", 0x006d08d6, 0x00000002
	.section .rom.006d0a2b, "a"
	.incbin "baserom.gba", 0x006d0a2b, 0x00000001
	.section .rom.006d0b6b, "a"
	.incbin "baserom.gba", 0x006d0b6b, 0x00000001
	.section .rom.006d183f, "a"
	.incbin "baserom.gba", 0x006d183f, 0x00000001
	.section .rom.006d19be, "a"
	.incbin "baserom.gba", 0x006d19be, 0x00000002
	.section .rom.006d3277, "a"
	.incbin "baserom.gba", 0x006d3277, 0x00000001
	.section .rom.006d33f9, "a"
	.incbin "baserom.gba", 0x006d33f9, 0x00000003
	.section .rom.006d4e8b, "a"
	.incbin "baserom.gba", 0x006d4e8b, 0x00000001
	.section .rom.006d5005, "a"
	.incbin "baserom.gba", 0x006d5005, 0x00000003
	.section .rom.006d6d43, "a"
	.incbin "baserom.gba", 0x006d6d43, 0x00000001
	.section .rom.006d6e6f, "a"
	.incbin "baserom.gba", 0x006d6e6f, 0x00000001
	.section .rom.006d6fe5, "a"
	.incbin "baserom.gba", 0x006d6fe5, 0x00000003
	.section .rom.006d8e8b, "a"
	.incbin "baserom.gba", 0x006d8e8b, 0x00000001
	.section .rom.006d9011, "a"
	.incbin "baserom.gba", 0x006d9011, 0x00000003
	.section .rom.006d9f6f, "a"
	.incbin "baserom.gba", 0x006d9f6f, 0x00000001
	.section .rom.006db21f, "a"
	.incbin "baserom.gba", 0x006db21f, 0x00000001
	.section .rom.006db3da, "a"
	.incbin "baserom.gba", 0x006db3da, 0x00000002
	.section .rom.006dde7e, "a"
	.incbin "baserom.gba", 0x006dde7e, 0x00000002
	.section .rom.006ddff1, "a"
	.incbin "baserom.gba", 0x006ddff1, 0x00000003
	.section .rom.006df3d9, "a"
	.incbin "baserom.gba", 0x006df3d9, 0x00000003
	.section .rom.006e1eb9, "a"
	.incbin "baserom.gba", 0x006e1eb9, 0x00000003
	.section .rom.006e2063, "a"
	.incbin "baserom.gba", 0x006e2063, 0x00000001
	.section .rom.006e445e, "a"
	.incbin "baserom.gba", 0x006e445e, 0x00000002
	.section .rom.006e45ed, "a"
	.incbin "baserom.gba", 0x006e45ed, 0x00000003
	.section .rom.006e786d, "a"
	.incbin "baserom.gba", 0x006e786d, 0x00000003
	.section .rom.006e7a4e, "a"
	.incbin "baserom.gba", 0x006e7a4e, 0x00000002
	.section .rom.006e9fe3, "a"
	.incbin "baserom.gba", 0x006e9fe3, 0x00000001
	.section .rom.006ea172, "a"
	.incbin "baserom.gba", 0x006ea172, 0x00000002
	.section .rom.006ecd6a, "a"
	.incbin "baserom.gba", 0x006ecd6a, 0x00000002
	.section .rom.006ed8b7, "a"
	.incbin "baserom.gba", 0x006ed8b7, 0x00000001
	.section .rom.006eda1e, "a"
	.incbin "baserom.gba", 0x006eda1e, 0x00000002
	.section .rom.006f0433, "a"
	.incbin "baserom.gba", 0x006f0433, 0x00000001
	.section .rom.006f2072, "a"
	.incbin "baserom.gba", 0x006f2072, 0x00000002
	.section .rom.006f252f, "a"
	.incbin "baserom.gba", 0x006f252f, 0x00000001
	.section .rom.006f38a7, "a"
	.incbin "baserom.gba", 0x006f38a7, 0x00000001
	.section .rom.006f4deb, "a"
	.incbin "baserom.gba", 0x006f4deb, 0x00000001
	.section .rom.006f4f75, "a"
	.incbin "baserom.gba", 0x006f4f75, 0x00000003
	.section .rom.006f6053, "a"
	.incbin "baserom.gba", 0x006f6053, 0x00000001
	.section .rom.006f61e1, "a"
	.incbin "baserom.gba", 0x006f61e1, 0x00000003
	.section .rom.006f7e35, "a"
	.incbin "baserom.gba", 0x006f7e35, 0x00000003
	.section .rom.006fa817, "a"
	.incbin "baserom.gba", 0x006fa817, 0x00000001
	.section .rom.006fdd41, "a"
	.incbin "baserom.gba", 0x006fdd41, 0x00000003
	.section .rom.006ff64b, "a"
	.incbin "baserom.gba", 0x006ff64b, 0x00000001
	.section .rom.007016f3, "a"
	.incbin "baserom.gba", 0x007016f3, 0x00000001
	.section .rom.00704147, "a"
	.incbin "baserom.gba", 0x00704147, 0x00000001
	.section .rom.00705d93, "a"
	.incbin "baserom.gba", 0x00705d93, 0x00000001
	.section .rom.007071d9, "a"
	.incbin "baserom.gba", 0x007071d9, 0x00000003
	.section .rom.00709059, "a"
	.incbin "baserom.gba", 0x00709059, 0x00000003
	.section .rom.0070ac2a, "a"
	.incbin "baserom.gba", 0x0070ac2a, 0x00000002
	.section .rom.0070d35e, "a"
	.incbin "baserom.gba", 0x0070d35e, 0x00000002
	.section .rom.0070efa3, "a"
	.incbin "baserom.gba", 0x0070efa3, 0x00000001
	.section .rom.007103e9, "a"
	.incbin "baserom.gba", 0x007103e9, 0x00000003
	.section .rom.00711f67, "a"
	.incbin "baserom.gba", 0x00711f67, 0x00000001
	.section .rom.00713197, "a"
	.incbin "baserom.gba", 0x00713197, 0x00000001
	.section .rom.007132f1, "a"
	.incbin "baserom.gba", 0x007132f1, 0x00000003
	.section .rom.007167f7, "a"
	.incbin "baserom.gba", 0x007167f7, 0x00000001
	.section .rom.00716962, "a"
	.incbin "baserom.gba", 0x00716962, 0x00000002
	.section .rom.007192b2, "a"
	.incbin "baserom.gba", 0x007192b2, 0x00001166
	.section .rom.0071bbfd, "a"
	.incbin "baserom.gba", 0x0071bbfd, 0x00000003
	.section .rom.0071ee43, "a"
	.incbin "baserom.gba", 0x0071ee43, 0x00000001
	.section .rom.0072173a, "a"
	.incbin "baserom.gba", 0x0072173a, 0x00000002
	.section .rom.0072aa73, "a"
	.incbin "baserom.gba", 0x0072aa73, 0x00000001
	.section .rom.0072bda2, "a"
	.incbin "baserom.gba", 0x0072bda2, 0x00000002
	.section .rom.0072cf9b, "a"
	.incbin "baserom.gba", 0x0072cf9b, 0x00000001
	.section .rom.0072d0fd, "a"
	.incbin "baserom.gba", 0x0072d0fd, 0x00000003
	.section .rom.0072fac9, "a"
	.incbin "baserom.gba", 0x0072fac9, 0x00000003
	.section .rom.00732ff5, "a"
	.incbin "baserom.gba", 0x00732ff5, 0x00000003
	.section .rom.00737437, "a"
	.incbin "baserom.gba", 0x00737437, 0x00000001
	.section .rom.007375aa, "a"
	.incbin "baserom.gba", 0x007375aa, 0x00000002
	.section .rom.0073b80f, "a"
	.incbin "baserom.gba", 0x0073b80f, 0x00000001
	.section .rom.0073f02a, "a"
	.incbin "baserom.gba", 0x0073f02a, 0x00000002
	.section .rom.00740a52, "a"
	.incbin "baserom.gba", 0x00740a52, 0x00000002
	.section .rom.007430ff, "a"
	.incbin "baserom.gba", 0x007430ff, 0x00000001
	.section .rom.00745163, "a"
	.incbin "baserom.gba", 0x00745163, 0x00000001
	.section .rom.0074701f, "a"
	.incbin "baserom.gba", 0x0074701f, 0x00000001
	.section .rom.0074a269, "a"
	.incbin "baserom.gba", 0x0074a269, 0x00000003
	.section .rom.0074bc2f, "a"
	.incbin "baserom.gba", 0x0074bc2f, 0x00000001
	.section .rom.0074bda1, "a"
	.incbin "baserom.gba", 0x0074bda1, 0x00000003
	.section .rom.0074edae, "a"
	.incbin "baserom.gba", 0x0074edae, 0x00000002
	.section .rom.0074ef66, "a"
	.incbin "baserom.gba", 0x0074ef66, 0x00000002
	.section .rom.007507d7, "a"
	.incbin "baserom.gba", 0x007507d7, 0x00000001
	.section .rom.00750956, "a"
	.incbin "baserom.gba", 0x00750956, 0x00000002
	.section .rom.0075415f, "a"
	.incbin "baserom.gba", 0x0075415f, 0x00000001
	.section .rom.007542d3, "a"
	.incbin "baserom.gba", 0x007542d3, 0x00000001
	.section .rom.0075509b, "a"
	.incbin "baserom.gba", 0x0075509b, 0x00000001
	.section .rom.00755241, "a"
	.incbin "baserom.gba", 0x00755241, 0x00000003
	.section .rom.0075729c, "a"
	.incbin "baserom.gba", 0x0075729c, 0x000000e4
	.section .rom.0075a097, "a"
	.incbin "baserom.gba", 0x0075a097, 0x00000001
	.section .rom.0075a27e, "a"
	.incbin "baserom.gba", 0x0075a27e, 0x00000002
	.section .rom.0075d51a, "a"
	.incbin "baserom.gba", 0x0075d51a, 0x00004602
	.section .rom.00761c22, "a"
	.incbin "baserom.gba", 0x00761c22, 0x00000002
	.section .rom.007637f5, "a"
	.incbin "baserom.gba", 0x007637f5, 0x00000003
	.section .rom.007652a5, "a"
	.incbin "baserom.gba", 0x007652a5, 0x00000003
	.section .rom.00765401, "a"
	.incbin "baserom.gba", 0x00765401, 0x00000003
	.section .rom.0076b647, "a"
	.incbin "baserom.gba", 0x0076b647, 0x00000001
	.section .rom.0076b74b, "a"
	.incbin "baserom.gba", 0x0076b74b, 0x00000001
	.section .rom.0076c167, "a"
	.incbin "baserom.gba", 0x0076c167, 0x00000001
	.section .rom.0076c24d, "a"
	.incbin "baserom.gba", 0x0076c24d, 0x00000003
	.section .rom.0076c4e9, "a"
	.incbin "baserom.gba", 0x0076c4e9, 0x00000003
	.section .rom.00770546, "a"
	.incbin "baserom.gba", 0x00770546, 0x00000002
	.section .rom.00770afa, "a"
	.incbin "baserom.gba", 0x00770afa, 0x00000002
	.section .rom.00771a8f, "a"
	.incbin "baserom.gba", 0x00771a8f, 0x00000001
	.section .rom.00773492, "a"
	.incbin "baserom.gba", 0x00773492, 0x00000002
	.section .rom.00774101, "a"
	.incbin "baserom.gba", 0x00774101, 0x00000003
	.section .rom.007743a9, "a"
	.incbin "baserom.gba", 0x007743a9, 0x00000003
	.section .rom.0077522e, "a"
	.incbin "baserom.gba", 0x0077522e, 0x00000002
	.section .rom.007763bf, "a"
	.incbin "baserom.gba", 0x007763bf, 0x00000001
	.section .rom.00776587, "a"
	.incbin "baserom.gba", 0x00776587, 0x00000001
	.section .rom.007768d3, "a"
	.incbin "baserom.gba", 0x007768d3, 0x0000051d
	.section .rom.0077713b, "a"
	.incbin "baserom.gba", 0x0077713b, 0x0000051d
	.section .rom.007779a3, "a"
	.incbin "baserom.gba", 0x007779a3, 0x0000051d
	.section .rom.0077820b, "a"
	.incbin "baserom.gba", 0x0077820b, 0x0000051d
	.section .rom.00778a73, "a"
	.incbin "baserom.gba", 0x00778a73, 0x0000051d
	.section .rom.007792db, "a"
	.incbin "baserom.gba", 0x007792db, 0x0000051d
	.section .rom.00779b43, "a"
	.incbin "baserom.gba", 0x00779b43, 0x0000051d
	.section .rom.0077a3ab, "a"
	.incbin "baserom.gba", 0x0077a3ab, 0x00085c55
