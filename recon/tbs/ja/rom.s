@ tbs-ja's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.incbin "baserom.gba", 0x00009e7c, 0x0000bc54
	.global Tile_Decompress4bpp
Tile_Decompress4bpp:
	.incbin "baserom.gba", 0x00015ad0, 0x00000278
	.global Tile_Decompress4bppEnd
Tile_Decompress4bppEnd:
	.global Tile_ExpandMasked
Tile_ExpandMasked:
	.incbin "baserom.gba", 0x00015d48, 0x0000009c
	.global Tile_ExpandMaskedEnd
Tile_ExpandMaskedEnd:
	.global Tile_ExpandOpaque
Tile_ExpandOpaque:
	.incbin "baserom.gba", 0x00015de4, 0x0000007c
	.global Tile_ExpandOpaqueEnd
Tile_ExpandOpaqueEnd:
	.incbin "baserom.gba", 0x00015e60, 0x0001ba50
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x000318b0, 0x00000bc0
	.section .rom.000345e0, "a"
	.incbin "baserom.gba", 0x000345e0, 0x00054a94
	.global Object_GetById
	.thumb_func
Object_GetById:
	.incbin "baserom.gba", 0x00089074, 0x0005dfb0
	.global SentouKouka_Tenkai
SentouKouka_Tenkai:
	.incbin "baserom.gba", 0x000e7024, 0x00000230
	.global SentouKouka_TenkaiEnd
SentouKouka_TenkaiEnd:
	.incbin "baserom.gba", 0x000e7254, 0x00009420
	.section .rom.000f0674, "a"
	.incbin "baserom.gba", 0x000f0674, 0x0000039e
	.global Mixer_CallViaR3
	.thumb_func
Mixer_CallViaR3:
	.incbin "baserom.gba", 0x000f0a12, 0x00000006
	.section .rom.000f0a18, "a"
	.incbin "baserom.gba", 0x000f0a18, 0x00002060
	.section .rom.000f3504, "a"
	.incbin "baserom.gba", 0x000f3504, 0x00000090
	.section .rom.000f3624, "a"
	.incbin "baserom.gba", 0x000f3624, 0x00000060
	.section .rom.0017b698, "a"
	.incbin "baserom.gba", 0x0017b698, 0x0000098c
	.section .rom.00315f74, "a"
	.incbin "baserom.gba", 0x00315f74, 0x0000203c
	.section .rom.0031b9e7, "a"
	.incbin "baserom.gba", 0x0031b9e7, 0x00000001
	.section .rom.00322693, "a"
	.incbin "baserom.gba", 0x00322693, 0x000086f9
	.section .rom.00332335, "a"
	.incbin "baserom.gba", 0x00332335, 0x000002b7
	.section .rom.00336a2e, "a"
	.incbin "baserom.gba", 0x00336a2e, 0x00000002
	.section .rom.0034719e, "a"
	.incbin "baserom.gba", 0x0034719e, 0x00000002
	.section .rom.0034a78e, "a"
	.incbin "baserom.gba", 0x0034a78e, 0x00000002
	.section .rom.00356232, "a"
	.incbin "baserom.gba", 0x00356232, 0x00000002
	.section .rom.003668e2, "a"
	.incbin "baserom.gba", 0x003668e2, 0x00000002
	.section .rom.0036e382, "a"
	.incbin "baserom.gba", 0x0036e382, 0x00000002
	.section .rom.00371bc2, "a"
	.incbin "baserom.gba", 0x00371bc2, 0x00000002
	.section .rom.0037aff6, "a"
	.incbin "baserom.gba", 0x0037aff6, 0x00000002
	.section .rom.0038641e, "a"
	.incbin "baserom.gba", 0x0038641e, 0x00000002
	.section .rom.0038e2da, "a"
	.incbin "baserom.gba", 0x0038e2da, 0x00000002
	.section .rom.00392d6a, "a"
	.incbin "baserom.gba", 0x00392d6a, 0x00000002
	.section .rom.0039701e, "a"
	.incbin "baserom.gba", 0x0039701e, 0x00000002
	.section .rom.0039a99e, "a"
	.incbin "baserom.gba", 0x0039a99e, 0x00000002
	.section .rom.003a71da, "a"
	.incbin "baserom.gba", 0x003a71da, 0x00000002
	.section .rom.003b5b22, "a"
	.incbin "baserom.gba", 0x003b5b22, 0x00000002
	.section .rom.003bb425, "a"
	.incbin "baserom.gba", 0x003bb425, 0x00000003
	.section .rom.003bcdc7, "a"
	.incbin "baserom.gba", 0x003bcdc7, 0x00000001
	.section .rom.003bfbe5, "a"
	.incbin "baserom.gba", 0x003bfbe5, 0x00000003
	.section .rom.003c3339, "a"
	.incbin "baserom.gba", 0x003c3339, 0x00000003
	.section .rom.003c4b4f, "a"
	.incbin "baserom.gba", 0x003c4b4f, 0x00000001
	.section .rom.003c513a, "a"
	.incbin "baserom.gba", 0x003c513a, 0x00000002
	.section .rom.003c57fe, "a"
	.incbin "baserom.gba", 0x003c57fe, 0x00000002
	.section .rom.003c5ae5, "a"
	.incbin "baserom.gba", 0x003c5ae5, 0x00000003
	.section .rom.003c6e4b, "a"
	.incbin "baserom.gba", 0x003c6e4b, 0x00000001
	.section .rom.003c7235, "a"
	.incbin "baserom.gba", 0x003c7235, 0x00000003
	.section .rom.003c7607, "a"
	.incbin "baserom.gba", 0x003c7607, 0x00000001
	.section .rom.003c7ea7, "a"
	.incbin "baserom.gba", 0x003c7ea7, 0x00000001
	.section .rom.003c834a, "a"
	.incbin "baserom.gba", 0x003c834a, 0x00000002
	.section .rom.003c9503, "a"
	.incbin "baserom.gba", 0x003c9503, 0x00000001
	.section .rom.003ca9c6, "a"
	.incbin "baserom.gba", 0x003ca9c6, 0x00000002
	.section .rom.003cb98f, "a"
	.incbin "baserom.gba", 0x003cb98f, 0x00000001
	.section .rom.003cbbea, "a"
	.incbin "baserom.gba", 0x003cbbea, 0x00000002
	.section .rom.003cd645, "a"
	.incbin "baserom.gba", 0x003cd645, 0x00000003
	.section .rom.003cdb91, "a"
	.incbin "baserom.gba", 0x003cdb91, 0x00000003
	.section .rom.003cf91a, "a"
	.incbin "baserom.gba", 0x003cf91a, 0x00000002
	.section .rom.003cfb93, "a"
	.incbin "baserom.gba", 0x003cfb93, 0x00000001
	.section .rom.003d005a, "a"
	.incbin "baserom.gba", 0x003d005a, 0x00000002
	.section .rom.003d1c2f, "a"
	.incbin "baserom.gba", 0x003d1c2f, 0x00000001
	.section .rom.003d382d, "a"
	.incbin "baserom.gba", 0x003d382d, 0x00000003
	.section .rom.003d3a4e, "a"
	.incbin "baserom.gba", 0x003d3a4e, 0x00000002
	.section .rom.003d3e8b, "a"
	.incbin "baserom.gba", 0x003d3e8b, 0x00000001
	.section .rom.003d3f9d, "a"
	.incbin "baserom.gba", 0x003d3f9d, 0x00000003
	.section .rom.003d471b, "a"
	.incbin "baserom.gba", 0x003d471b, 0x00000001
	.section .rom.003d4bb9, "a"
	.incbin "baserom.gba", 0x003d4bb9, 0x00000003
	.section .rom.003d58ce, "a"
	.incbin "baserom.gba", 0x003d58ce, 0x00000002
	.section .rom.003d664e, "a"
	.incbin "baserom.gba", 0x003d664e, 0x00000002
	.section .rom.003d69df, "a"
	.incbin "baserom.gba", 0x003d69df, 0x00000001
	.section .rom.003d8726, "a"
	.incbin "baserom.gba", 0x003d8726, 0x00000002
	.section .rom.003d94fd, "a"
	.incbin "baserom.gba", 0x003d94fd, 0x00000003
	.section .rom.003d9d2a, "a"
	.incbin "baserom.gba", 0x003d9d2a, 0x00000002
	.section .rom.003da3b5, "a"
	.incbin "baserom.gba", 0x003da3b5, 0x00000003
	.section .rom.003da573, "a"
	.incbin "baserom.gba", 0x003da573, 0x00000001
	.section .rom.003db2cb, "a"
	.incbin "baserom.gba", 0x003db2cb, 0x00000001
	.section .rom.003db817, "a"
	.incbin "baserom.gba", 0x003db817, 0x00000001
	.section .rom.003dbbf1, "a"
	.incbin "baserom.gba", 0x003dbbf1, 0x00000003
	.section .rom.003dbf9f, "a"
	.incbin "baserom.gba", 0x003dbf9f, 0x00000001
	.section .rom.003ddf43, "a"
	.incbin "baserom.gba", 0x003ddf43, 0x00000001
	.section .rom.003decdf, "a"
	.incbin "baserom.gba", 0x003decdf, 0x00000001
	.section .rom.003deefb, "a"
	.incbin "baserom.gba", 0x003deefb, 0x00000001
	.section .rom.003df1f7, "a"
	.incbin "baserom.gba", 0x003df1f7, 0x00000001
	.section .rom.003e13a5, "a"
	.incbin "baserom.gba", 0x003e13a5, 0x00000003
	.section .rom.003e2173, "a"
	.incbin "baserom.gba", 0x003e2173, 0x00000001
	.section .rom.003e32d5, "a"
	.incbin "baserom.gba", 0x003e32d5, 0x00000003
	.section .rom.003e382f, "a"
	.incbin "baserom.gba", 0x003e382f, 0x00000001
	.section .rom.003e5119, "a"
	.incbin "baserom.gba", 0x003e5119, 0x00000003
	.section .rom.003e59ba, "a"
	.incbin "baserom.gba", 0x003e59ba, 0x00000002
	.section .rom.003e5d97, "a"
	.incbin "baserom.gba", 0x003e5d97, 0x00000001
	.section .rom.003e6021, "a"
	.incbin "baserom.gba", 0x003e6021, 0x00000003
	.section .rom.003e63c7, "a"
	.incbin "baserom.gba", 0x003e63c7, 0x00000001
	.section .rom.003e6622, "a"
	.incbin "baserom.gba", 0x003e6622, 0x00000002
	.section .rom.003e69da, "a"
	.incbin "baserom.gba", 0x003e69da, 0x00000002
	.section .rom.003e7ec3, "a"
	.incbin "baserom.gba", 0x003e7ec3, 0x00000001
	.section .rom.003e9486, "a"
	.incbin "baserom.gba", 0x003e9486, 0x00000a6e
	.section .rom.003eacca, "a"
	.incbin "baserom.gba", 0x003eacca, 0x00000002
	.section .rom.003eb04d, "a"
	.incbin "baserom.gba", 0x003eb04d, 0x00000003
	.section .rom.003ebcfd, "a"
	.incbin "baserom.gba", 0x003ebcfd, 0x00000003
	.section .rom.003ec845, "a"
	.incbin "baserom.gba", 0x003ec845, 0x00000003
	.section .rom.003ec9de, "a"
	.incbin "baserom.gba", 0x003ec9de, 0x00000002
	.section .rom.003ed26b, "a"
	.incbin "baserom.gba", 0x003ed26b, 0x00000001
	.section .rom.003edd32, "a"
	.incbin "baserom.gba", 0x003edd32, 0x00000002
	.section .rom.003ee24a, "a"
	.incbin "baserom.gba", 0x003ee24a, 0x00000002
	.section .rom.003ee86e, "a"
	.incbin "baserom.gba", 0x003ee86e, 0x00000002
	.section .rom.003ef0d5, "a"
	.incbin "baserom.gba", 0x003ef0d5, 0x00000003
	.section .rom.003ef4f9, "a"
	.incbin "baserom.gba", 0x003ef4f9, 0x00000003
	.section .rom.003ef793, "a"
	.incbin "baserom.gba", 0x003ef793, 0x00000001
	.section .rom.003f112f, "a"
	.incbin "baserom.gba", 0x003f112f, 0x00000001
	.section .rom.003f2ea6, "a"
	.incbin "baserom.gba", 0x003f2ea6, 0x00000002
	.section .rom.003f4e1b, "a"
	.incbin "baserom.gba", 0x003f4e1b, 0x00000001
	.section .rom.003f52eb, "a"
	.incbin "baserom.gba", 0x003f52eb, 0x00000001
	.section .rom.003f597d, "a"
	.incbin "baserom.gba", 0x003f597d, 0x00000a37
	.section .rom.003f7785, "a"
	.incbin "baserom.gba", 0x003f7785, 0x00000003
	.section .rom.003f8256, "a"
	.incbin "baserom.gba", 0x003f8256, 0x00000002
	.section .rom.003f8d93, "a"
	.incbin "baserom.gba", 0x003f8d93, 0x00000001
	.section .rom.003f93d3, "a"
	.incbin "baserom.gba", 0x003f93d3, 0x00001589
	.section .rom.003fa9bd, "a"
	.incbin "baserom.gba", 0x003fa9bd, 0x00000003
	.section .rom.003fad2b, "a"
	.incbin "baserom.gba", 0x003fad2b, 0x00000001
	.section .rom.003fb2c9, "a"
	.incbin "baserom.gba", 0x003fb2c9, 0x00000003
	.section .rom.003fb5fd, "a"
	.incbin "baserom.gba", 0x003fb5fd, 0x00000003
	.section .rom.003fb939, "a"
	.incbin "baserom.gba", 0x003fb939, 0x00000003
	.section .rom.003fc952, "a"
	.incbin "baserom.gba", 0x003fc952, 0x00000002
	.section .rom.003fff5e, "a"
	.incbin "baserom.gba", 0x003fff5e, 0x00000002
	.section .rom.00400701, "a"
	.incbin "baserom.gba", 0x00400701, 0x00000003
	.section .rom.00401a93, "a"
	.incbin "baserom.gba", 0x00401a93, 0x00000001
	.section .rom.00401ec3, "a"
	.incbin "baserom.gba", 0x00401ec3, 0x00000001
	.section .rom.00403cb9, "a"
	.incbin "baserom.gba", 0x00403cb9, 0x00000003
	.section .rom.004045ce, "a"
	.incbin "baserom.gba", 0x004045ce, 0x00000002
	.section .rom.00406102, "a"
	.incbin "baserom.gba", 0x00406102, 0x00000002
	.section .rom.00407151, "a"
	.incbin "baserom.gba", 0x00407151, 0x00000003
	.section .rom.00407807, "a"
	.incbin "baserom.gba", 0x00407807, 0x00000001
	.section .rom.004088ad, "a"
	.incbin "baserom.gba", 0x004088ad, 0x00000003
	.section .rom.0041bcbf, "a"
	.incbin "baserom.gba", 0x0041bcbf, 0x00000001
	.section .rom.0041be11, "a"
	.incbin "baserom.gba", 0x0041be11, 0x00000003
	.section .rom.0041c2a2, "a"
	.incbin "baserom.gba", 0x0041c2a2, 0x00000002
	.section .rom.0041c499, "a"
	.incbin "baserom.gba", 0x0041c499, 0x00000003
	.section .rom.0041db56, "a"
	.incbin "baserom.gba", 0x0041db56, 0x00000002
	.section .rom.0041f075, "a"
	.incbin "baserom.gba", 0x0041f075, 0x00000003
	.section .rom.0041fc41, "a"
	.incbin "baserom.gba", 0x0041fc41, 0x00000003
	.section .rom.004207b6, "a"
	.incbin "baserom.gba", 0x004207b6, 0x00000002
	.section .rom.0042265f, "a"
	.incbin "baserom.gba", 0x0042265f, 0x00000001
	.section .rom.00423ded, "a"
	.incbin "baserom.gba", 0x00423ded, 0x00000003
	.section .rom.0042529e, "a"
	.incbin "baserom.gba", 0x0042529e, 0x00000002
	.section .rom.004279eb, "a"
	.incbin "baserom.gba", 0x004279eb, 0x00000001
	.section .rom.00429285, "a"
	.incbin "baserom.gba", 0x00429285, 0x00000003
	.section .rom.0042a84e, "a"
	.incbin "baserom.gba", 0x0042a84e, 0x00000002
	.section .rom.0042c2de, "a"
	.incbin "baserom.gba", 0x0042c2de, 0x00000002
	.section .rom.0042d1e2, "a"
	.incbin "baserom.gba", 0x0042d1e2, 0x00000002
	.section .rom.00436e66, "a"
	.incbin "baserom.gba", 0x00436e66, 0x00000002
	.section .rom.0043901a, "a"
	.incbin "baserom.gba", 0x0043901a, 0x00000002
	.section .rom.0043d181, "a"
	.incbin "baserom.gba", 0x0043d181, 0x00000003
	.section .rom.004403a6, "a"
	.incbin "baserom.gba", 0x004403a6, 0x00000002
	.section .rom.00441aaa, "a"
	.incbin "baserom.gba", 0x00441aaa, 0x00000002
	.section .rom.00448692, "a"
	.incbin "baserom.gba", 0x00448692, 0x00000002
	.section .rom.00451016, "a"
	.incbin "baserom.gba", 0x00451016, 0x00000002
	.section .rom.00458af2, "a"
	.incbin "baserom.gba", 0x00458af2, 0x00000002
	.section .rom.0045bb86, "a"
	.incbin "baserom.gba", 0x0045bb86, 0x00000002
	.section .rom.0045ef95, "a"
	.incbin "baserom.gba", 0x0045ef95, 0x00000003
	.section .rom.00461801, "a"
	.incbin "baserom.gba", 0x00461801, 0x00000003
	.section .rom.004632f2, "a"
	.incbin "baserom.gba", 0x004632f2, 0x00000002
	.section .rom.0046410a, "a"
	.incbin "baserom.gba", 0x0046410a, 0x00000002
	.section .rom.00464df5, "a"
	.incbin "baserom.gba", 0x00464df5, 0x00000003
	.section .rom.0046e2b2, "a"
	.incbin "baserom.gba", 0x0046e2b2, 0x00000002
	.section .rom.0046efbd, "a"
	.incbin "baserom.gba", 0x0046efbd, 0x00000003
	.section .rom.004710d1, "a"
	.incbin "baserom.gba", 0x004710d1, 0x00000003
	.section .rom.00471d43, "a"
	.incbin "baserom.gba", 0x00471d43, 0x00000001
	.section .rom.0047295b, "a"
	.incbin "baserom.gba", 0x0047295b, 0x00000001
	.section .rom.004730cf, "a"
	.incbin "baserom.gba", 0x004730cf, 0x00000001
	.section .rom.0047495f, "a"
	.incbin "baserom.gba", 0x0047495f, 0x00000001
	.section .rom.0047532d, "a"
	.incbin "baserom.gba", 0x0047532d, 0x00000003
	.section .rom.00475f4b, "a"
	.incbin "baserom.gba", 0x00475f4b, 0x00000001
	.section .rom.004785ff, "a"
	.incbin "baserom.gba", 0x004785ff, 0x00000001
	.section .rom.0047ad12, "a"
	.incbin "baserom.gba", 0x0047ad12, 0x00000002
	.section .rom.0047fc67, "a"
	.incbin "baserom.gba", 0x0047fc67, 0x00000001
	.section .rom.00483f91, "a"
	.incbin "baserom.gba", 0x00483f91, 0x00000003
	.section .rom.00484b7e, "a"
	.incbin "baserom.gba", 0x00484b7e, 0x00000002
	.section .rom.00489733, "a"
	.incbin "baserom.gba", 0x00489733, 0x00000001
	.section .rom.0048ba9d, "a"
	.incbin "baserom.gba", 0x0048ba9d, 0x00000003
	.section .rom.0048d43f, "a"
	.incbin "baserom.gba", 0x0048d43f, 0x00000001
	.section .rom.00491d01, "a"
	.incbin "baserom.gba", 0x00491d01, 0x00000003
	.section .rom.00495dc9, "a"
	.incbin "baserom.gba", 0x00495dc9, 0x00000003
	.section .rom.00499492, "a"
	.incbin "baserom.gba", 0x00499492, 0x00000002
	.section .rom.004a1463, "a"
	.incbin "baserom.gba", 0x004a1463, 0x00000001
	.section .rom.004a8773, "a"
	.incbin "baserom.gba", 0x004a8773, 0x00000001
	.section .rom.004ad6f3, "a"
	.incbin "baserom.gba", 0x004ad6f3, 0x00000001
	.section .rom.004b20a3, "a"
	.incbin "baserom.gba", 0x004b20a3, 0x0000051d
	.section .rom.004b79cf, "a"
	.incbin "baserom.gba", 0x004b79cf, 0x00000001
	.section .rom.004b7b5d, "a"
	.incbin "baserom.gba", 0x004b7b5d, 0x00000003
	.section .rom.004bcbbd, "a"
	.incbin "baserom.gba", 0x004bcbbd, 0x00000003
	.section .rom.004c1129, "a"
	.incbin "baserom.gba", 0x004c1129, 0x00000003
	.section .rom.004c62f1, "a"
	.incbin "baserom.gba", 0x004c62f1, 0x00000003
	.section .rom.004c6487, "a"
	.incbin "baserom.gba", 0x004c6487, 0x00000001
	.section .rom.004c90db, "a"
	.incbin "baserom.gba", 0x004c90db, 0x00000001
	.section .rom.004cfd8f, "a"
	.incbin "baserom.gba", 0x004cfd8f, 0x00000001
	.section .rom.004d2eca, "a"
	.incbin "baserom.gba", 0x004d2eca, 0x00000002
	.section .rom.004d304b, "a"
	.incbin "baserom.gba", 0x004d304b, 0x00000001
	.section .rom.004d5805, "a"
	.incbin "baserom.gba", 0x004d5805, 0x00000003
	.section .rom.004d7e4e, "a"
	.incbin "baserom.gba", 0x004d7e4e, 0x00000002
	.section .rom.004da532, "a"
	.incbin "baserom.gba", 0x004da532, 0x00000002
	.section .rom.004db5a7, "a"
	.incbin "baserom.gba", 0x004db5a7, 0x00000001
	.section .rom.004dd6fa, "a"
	.incbin "baserom.gba", 0x004dd6fa, 0x00000002
	.section .rom.004dd88d, "a"
	.incbin "baserom.gba", 0x004dd88d, 0x00000003
	.section .rom.004e00cf, "a"
	.incbin "baserom.gba", 0x004e00cf, 0x00000001
	.section .rom.004e1ee5, "a"
	.incbin "baserom.gba", 0x004e1ee5, 0x00000003
	.section .rom.004e45c6, "a"
	.incbin "baserom.gba", 0x004e45c6, 0x00000002
	.section .rom.004e570d, "a"
	.incbin "baserom.gba", 0x004e570d, 0x00000003
	.section .rom.004e8821, "a"
	.incbin "baserom.gba", 0x004e8821, 0x00000003
	.section .rom.004ec989, "a"
	.incbin "baserom.gba", 0x004ec989, 0x00000003
	.section .rom.004edf86, "a"
	.incbin "baserom.gba", 0x004edf86, 0x00000002
	.section .rom.004ef24f, "a"
	.incbin "baserom.gba", 0x004ef24f, 0x00000001
	.section .rom.004f2667, "a"
	.incbin "baserom.gba", 0x004f2667, 0x00000001
	.section .rom.004f27f5, "a"
	.incbin "baserom.gba", 0x004f27f5, 0x00000003
	.section .rom.004f45a6, "a"
	.incbin "baserom.gba", 0x004f45a6, 0x00000002
	.section .rom.004f811d, "a"
	.incbin "baserom.gba", 0x004f811d, 0x00000003
	.section .rom.004f962b, "a"
	.incbin "baserom.gba", 0x004f962b, 0x00000001
	.section .rom.004fa1ff, "a"
	.incbin "baserom.gba", 0x004fa1ff, 0x00000001
	.section .rom.004fa2fd, "a"
	.incbin "baserom.gba", 0x004fa2fd, 0x00000003
	.section .rom.004fb6e2, "a"
	.incbin "baserom.gba", 0x004fb6e2, 0x00000002
	.section .rom.004fc865, "a"
	.incbin "baserom.gba", 0x004fc865, 0x00000003
	.section .rom.004fd1ee, "a"
	.incbin "baserom.gba", 0x004fd1ee, 0x00000002
	.section .rom.004ffe6b, "a"
	.incbin "baserom.gba", 0x004ffe6b, 0x00000001
	.section .rom.004fff4e, "a"
	.incbin "baserom.gba", 0x004fff4e, 0x00000002
	.section .rom.00501139, "a"
	.incbin "baserom.gba", 0x00501139, 0x00000003
	.section .rom.00502dc1, "a"
	.incbin "baserom.gba", 0x00502dc1, 0x00000003
	.section .rom.005032cf, "a"
	.incbin "baserom.gba", 0x005032cf, 0x00000001
	.section .rom.0050340f, "a"
	.incbin "baserom.gba", 0x0050340f, 0x00000001
	.section .rom.0050462a, "a"
	.incbin "baserom.gba", 0x0050462a, 0x00000002
	.section .rom.00504785, "a"
	.incbin "baserom.gba", 0x00504785, 0x00000003
	.section .rom.00508bab, "a"
	.incbin "baserom.gba", 0x00508bab, 0x00000001
	.section .rom.0050a8da, "a"
	.incbin "baserom.gba", 0x0050a8da, 0x00000002
	.section .rom.0050b653, "a"
	.incbin "baserom.gba", 0x0050b653, 0x00000001
	.section .rom.0050b787, "a"
	.incbin "baserom.gba", 0x0050b787, 0x00000001
	.section .rom.0050dc07, "a"
	.incbin "baserom.gba", 0x0050dc07, 0x00000001
	.section .rom.0050e983, "a"
	.incbin "baserom.gba", 0x0050e983, 0x00000001
	.section .rom.0050f462, "a"
	.incbin "baserom.gba", 0x0050f462, 0x00000002
	.section .rom.005102da, "a"
	.incbin "baserom.gba", 0x005102da, 0x00000002
	.section .rom.005123ae, "a"
	.incbin "baserom.gba", 0x005123ae, 0x00000002
	.section .rom.00512cf5, "a"
	.incbin "baserom.gba", 0x00512cf5, 0x00000003
	.section .rom.00513942, "a"
	.incbin "baserom.gba", 0x00513942, 0x00000002
	.section .rom.00513b69, "a"
	.incbin "baserom.gba", 0x00513b69, 0x00000003
	.section .rom.005154ab, "a"
	.incbin "baserom.gba", 0x005154ab, 0x00000001
	.section .rom.005155bf, "a"
	.incbin "baserom.gba", 0x005155bf, 0x00000001
	.section .rom.00516f85, "a"
	.incbin "baserom.gba", 0x00516f85, 0x00000003
	.section .rom.005170cf, "a"
	.incbin "baserom.gba", 0x005170cf, 0x00000001
	.section .rom.0051919b, "a"
	.incbin "baserom.gba", 0x0051919b, 0x00000001
	.section .rom.005192c3, "a"
	.incbin "baserom.gba", 0x005192c3, 0x00000001
	.section .rom.0051ab4b, "a"
	.incbin "baserom.gba", 0x0051ab4b, 0x00000001
	.section .rom.0051cded, "a"
	.incbin "baserom.gba", 0x0051cded, 0x00000003
	.section .rom.0051cf2f, "a"
	.incbin "baserom.gba", 0x0051cf2f, 0x00000001
	.section .rom.0051ef37, "a"
	.incbin "baserom.gba", 0x0051ef37, 0x00000001
	.section .rom.0051f042, "a"
	.incbin "baserom.gba", 0x0051f042, 0x00000002
	.section .rom.00521026, "a"
	.incbin "baserom.gba", 0x00521026, 0x00000002
	.section .rom.0052274b, "a"
	.incbin "baserom.gba", 0x0052274b, 0x00000001
	.section .rom.005237ff, "a"
	.incbin "baserom.gba", 0x005237ff, 0x00000001
	.section .rom.005254e6, "a"
	.incbin "baserom.gba", 0x005254e6, 0x00000002
	.section .rom.00525641, "a"
	.incbin "baserom.gba", 0x00525641, 0x00000003
	.section .rom.0052758a, "a"
	.incbin "baserom.gba", 0x0052758a, 0x00000002
	.section .rom.0052770f, "a"
	.incbin "baserom.gba", 0x0052770f, 0x00000001
	.section .rom.0052a261, "a"
	.incbin "baserom.gba", 0x0052a261, 0x00000003
	.section .rom.0052c926, "a"
	.incbin "baserom.gba", 0x0052c926, 0x00000002
	.section .rom.0052f07e, "a"
	.incbin "baserom.gba", 0x0052f07e, 0x00000002
	.section .rom.005308d1, "a"
	.incbin "baserom.gba", 0x005308d1, 0x00000003
	.section .rom.00532fd1, "a"
	.incbin "baserom.gba", 0x00532fd1, 0x00000003
	.section .rom.00534d9d, "a"
	.incbin "baserom.gba", 0x00534d9d, 0x00000003
	.section .rom.005372e7, "a"
	.incbin "baserom.gba", 0x005372e7, 0x00000001
	.section .rom.005385b2, "a"
	.incbin "baserom.gba", 0x005385b2, 0x00000002
	.section .rom.00539263, "a"
	.incbin "baserom.gba", 0x00539263, 0x00000001
	.section .rom.00539ccd, "a"
	.incbin "baserom.gba", 0x00539ccd, 0x00000003
	.section .rom.00539dc3, "a"
	.incbin "baserom.gba", 0x00539dc3, 0x00000001
	.section .rom.0053b822, "a"
	.incbin "baserom.gba", 0x0053b822, 0x00000002
	.section .rom.0053c5c6, "a"
	.incbin "baserom.gba", 0x0053c5c6, 0x00000002
	.section .rom.0053e0ff, "a"
	.incbin "baserom.gba", 0x0053e0ff, 0x00000001
	.section .rom.00540fed, "a"
	.incbin "baserom.gba", 0x00540fed, 0x00000003
	.section .rom.005487d3, "a"
	.incbin "baserom.gba", 0x005487d3, 0x00000001
	.section .rom.0054889f, "a"
	.incbin "baserom.gba", 0x0054889f, 0x00000001
	.section .rom.0054e14a, "a"
	.incbin "baserom.gba", 0x0054e14a, 0x00000002
	.section .rom.0054e28b, "a"
	.incbin "baserom.gba", 0x0054e28b, 0x00000001
	.section .rom.0054f6b9, "a"
	.incbin "baserom.gba", 0x0054f6b9, 0x00000003
	.section .rom.005523f6, "a"
	.incbin "baserom.gba", 0x005523f6, 0x00000002
	.section .rom.005544f2, "a"
	.incbin "baserom.gba", 0x005544f2, 0x00000002
	.section .rom.0055befa, "a"
	.incbin "baserom.gba", 0x0055befa, 0x00000002
	.section .rom.0055c096, "a"
	.incbin "baserom.gba", 0x0055c096, 0x00000002
	.section .rom.00560931, "a"
	.incbin "baserom.gba", 0x00560931, 0x00000003
	.section .rom.00562e32, "a"
	.incbin "baserom.gba", 0x00562e32, 0x00000002
	.section .rom.0056a823, "a"
	.incbin "baserom.gba", 0x0056a823, 0x00000001
	.section .rom.0056a9be, "a"
	.incbin "baserom.gba", 0x0056a9be, 0x00000002
	.section .rom.0056d56d, "a"
	.incbin "baserom.gba", 0x0056d56d, 0x00000003
	.section .rom.0056f641, "a"
	.incbin "baserom.gba", 0x0056f641, 0x00000003
	.section .rom.0057169a, "a"
	.incbin "baserom.gba", 0x0057169a, 0x00001cee
	.section .rom.00574b31, "a"
	.incbin "baserom.gba", 0x00574b31, 0x00000003
	.section .rom.0057780d, "a"
	.incbin "baserom.gba", 0x0057780d, 0x00000003
	.section .rom.005795df, "a"
	.incbin "baserom.gba", 0x005795df, 0x00000001
	.section .rom.0057971f, "a"
	.incbin "baserom.gba", 0x0057971f, 0x00000001
	.section .rom.0057c779, "a"
	.incbin "baserom.gba", 0x0057c779, 0x00000003
	.section .rom.0057f6f6, "a"
	.incbin "baserom.gba", 0x0057f6f6, 0x00000002
	.section .rom.00581fbd, "a"
	.incbin "baserom.gba", 0x00581fbd, 0x00000003
	.section .rom.00588f25, "a"
	.incbin "baserom.gba", 0x00588f25, 0x00000003
	.section .rom.0058a7cb, "a"
	.incbin "baserom.gba", 0x0058a7cb, 0x00000001
	.section .rom.0058b0ca, "a"
	.incbin "baserom.gba", 0x0058b0ca, 0x00000002
	.section .rom.0058be43, "a"
	.incbin "baserom.gba", 0x0058be43, 0x00000001
	.section .rom.0058bfa3, "a"
	.incbin "baserom.gba", 0x0058bfa3, 0x00000001
	.section .rom.0058dbfb, "a"
	.incbin "baserom.gba", 0x0058dbfb, 0x00000001
	.section .rom.0058f497, "a"
	.incbin "baserom.gba", 0x0058f497, 0x00000001
	.section .rom.0058f61b, "a"
	.incbin "baserom.gba", 0x0058f61b, 0x00000001
	.section .rom.00592157, "a"
	.incbin "baserom.gba", 0x00592157, 0x00000001
	.section .rom.005948d1, "a"
	.incbin "baserom.gba", 0x005948d1, 0x00000003
	.section .rom.005958be, "a"
	.incbin "baserom.gba", 0x005958be, 0x00000002
	.section .rom.00596f8b, "a"
	.incbin "baserom.gba", 0x00596f8b, 0x00000001
	.section .rom.00597135, "a"
	.incbin "baserom.gba", 0x00597135, 0x00000003
	.section .rom.00599ec9, "a"
	.incbin "baserom.gba", 0x00599ec9, 0x00000003
	.section .rom.0059df5e, "a"
	.incbin "baserom.gba", 0x0059df5e, 0x00000002
	.section .rom.0059e09f, "a"
	.incbin "baserom.gba", 0x0059e09f, 0x00000001
	.section .rom.0059ff67, "a"
	.incbin "baserom.gba", 0x0059ff67, 0x00000001
	.section .rom.005a0109, "a"
	.incbin "baserom.gba", 0x005a0109, 0x00000003
	.section .rom.005a21ab, "a"
	.incbin "baserom.gba", 0x005a21ab, 0x00000001
	.section .rom.005a3138, "a"
	.incbin "baserom.gba", 0x005a3138, 0x000022d8
	.section .rom.005a7f01, "a"
	.incbin "baserom.gba", 0x005a7f01, 0x00000003
	.section .rom.005a8086, "a"
	.incbin "baserom.gba", 0x005a8086, 0x00000002
	.section .rom.005af7d3, "a"
	.incbin "baserom.gba", 0x005af7d3, 0x00000001
	.section .rom.005afcdd, "a"
	.incbin "baserom.gba", 0x005afcdd, 0x00000003
	.section .rom.005b13b5, "a"
	.incbin "baserom.gba", 0x005b13b5, 0x00000003
	.section .rom.005b1559, "a"
	.incbin "baserom.gba", 0x005b1559, 0x00000003
	.section .rom.005b2e5e, "a"
	.incbin "baserom.gba", 0x005b2e5e, 0x00000002
	.section .rom.005b468b, "a"
	.incbin "baserom.gba", 0x005b468b, 0x00000001
	.section .rom.005b4809, "a"
	.incbin "baserom.gba", 0x005b4809, 0x00000003
	.section .rom.005b90fd, "a"
	.incbin "baserom.gba", 0x005b90fd, 0x00000003
	.section .rom.005ba142, "a"
	.incbin "baserom.gba", 0x005ba142, 0x00000002
	.section .rom.005baf66, "a"
	.incbin "baserom.gba", 0x005baf66, 0x00000002
	.section .rom.005bc2ab, "a"
	.incbin "baserom.gba", 0x005bc2ab, 0x00000001
	.section .rom.005bc405, "a"
	.incbin "baserom.gba", 0x005bc405, 0x00000003
	.section .rom.005be9c3, "a"
	.incbin "baserom.gba", 0x005be9c3, 0x00000001
	.section .rom.005beb3a, "a"
	.incbin "baserom.gba", 0x005beb3a, 0x00000002
	.section .rom.005c342d, "a"
	.incbin "baserom.gba", 0x005c342d, 0x00000003
	.section .rom.005c3e95, "a"
	.incbin "baserom.gba", 0x005c3e95, 0x00000003
	.section .rom.005c64d7, "a"
	.incbin "baserom.gba", 0x005c64d7, 0x00000001
	.section .rom.005c663e, "a"
	.incbin "baserom.gba", 0x005c663e, 0x00000002
	.section .rom.005c7785, "a"
	.incbin "baserom.gba", 0x005c7785, 0x00000003
	.section .rom.005c94e3, "a"
	.incbin "baserom.gba", 0x005c94e3, 0x00000001
	.section .rom.005c964a, "a"
	.incbin "baserom.gba", 0x005c964a, 0x00000002
	.section .rom.005cbed9, "a"
	.incbin "baserom.gba", 0x005cbed9, 0x00000003
	.section .rom.005cc00e, "a"
	.incbin "baserom.gba", 0x005cc00e, 0x00000002
	.section .rom.005cec4a, "a"
	.incbin "baserom.gba", 0x005cec4a, 0x00000002
	.section .rom.005d23e9, "a"
	.incbin "baserom.gba", 0x005d23e9, 0x00000003
	.section .rom.005d5737, "a"
	.incbin "baserom.gba", 0x005d5737, 0x00000001
	.section .rom.005d58b3, "a"
	.incbin "baserom.gba", 0x005d58b3, 0x00000001
	.section .rom.005d8462, "a"
	.incbin "baserom.gba", 0x005d8462, 0x00000002
	.section .rom.005da6d6, "a"
	.incbin "baserom.gba", 0x005da6d6, 0x00000002
	.section .rom.005dcf47, "a"
	.incbin "baserom.gba", 0x005dcf47, 0x00000001
	.section .rom.005e043b, "a"
	.incbin "baserom.gba", 0x005e043b, 0x00000001
	.section .rom.005e5062, "a"
	.incbin "baserom.gba", 0x005e5062, 0x00000002
	.section .rom.005e5ce5, "a"
	.incbin "baserom.gba", 0x005e5ce5, 0x00000003
	.section .rom.005e5e27, "a"
	.incbin "baserom.gba", 0x005e5e27, 0x00000001
	.section .rom.005e7397, "a"
	.incbin "baserom.gba", 0x005e7397, 0x00000001
	.section .rom.005e7502, "a"
	.incbin "baserom.gba", 0x005e7502, 0x00000002
	.section .rom.005e925d, "a"
	.incbin "baserom.gba", 0x005e925d, 0x00000003
	.section .rom.005e939f, "a"
	.incbin "baserom.gba", 0x005e939f, 0x00000001
	.section .rom.005ebb9e, "a"
	.incbin "baserom.gba", 0x005ebb9e, 0x00000002
	.section .rom.005ebcdf, "a"
	.incbin "baserom.gba", 0x005ebcdf, 0x00000001
	.section .rom.005ed6f7, "a"
	.incbin "baserom.gba", 0x005ed6f7, 0x00000001
	.section .rom.005eec72, "a"
	.incbin "baserom.gba", 0x005eec72, 0x00000002
	.section .rom.005f01ae, "a"
	.incbin "baserom.gba", 0x005f01ae, 0x00000002
	.section .rom.005f033e, "a"
	.incbin "baserom.gba", 0x005f033e, 0x00000002
	.section .rom.005f25f7, "a"
	.incbin "baserom.gba", 0x005f25f7, 0x00000001
	.section .rom.005f683e, "a"
	.incbin "baserom.gba", 0x005f683e, 0x00000002
	.section .rom.005f861d, "a"
	.incbin "baserom.gba", 0x005f861d, 0x00000003
	.section .rom.005fb7cb, "a"
	.incbin "baserom.gba", 0x005fb7cb, 0x00000001
	.section .rom.005fddff, "a"
	.incbin "baserom.gba", 0x005fddff, 0x00000001
	.section .rom.00600957, "a"
	.incbin "baserom.gba", 0x00600957, 0x00000001
	.section .rom.00600b1f, "a"
	.incbin "baserom.gba", 0x00600b1f, 0x00002905
	.section .rom.00606f29, "a"
	.incbin "baserom.gba", 0x00606f29, 0x00000003
	.section .rom.0060849a, "a"
	.incbin "baserom.gba", 0x0060849a, 0x00000002
	.section .rom.0060acc1, "a"
	.incbin "baserom.gba", 0x0060acc1, 0x00000003
	.section .rom.0060ae31, "a"
	.incbin "baserom.gba", 0x0060ae31, 0x00000003
	.section .rom.0060d6f3, "a"
	.incbin "baserom.gba", 0x0060d6f3, 0x00000001
	.section .rom.0060febb, "a"
	.incbin "baserom.gba", 0x0060febb, 0x00000001
	.section .rom.006111b5, "a"
	.incbin "baserom.gba", 0x006111b5, 0x00000003
	.section .rom.00612afb, "a"
	.incbin "baserom.gba", 0x00612afb, 0x00000001
	.section .rom.0061df8e, "a"
	.incbin "baserom.gba", 0x0061df8e, 0x00000002
	.section .rom.0061e14e, "a"
	.incbin "baserom.gba", 0x0061e14e, 0x00000002
	.section .rom.006214b5, "a"
	.incbin "baserom.gba", 0x006214b5, 0x00000003
	.section .rom.006239a6, "a"
	.incbin "baserom.gba", 0x006239a6, 0x00000002
	.section .rom.00626787, "a"
	.incbin "baserom.gba", 0x00626787, 0x00000001
	.section .rom.00627321, "a"
	.incbin "baserom.gba", 0x00627321, 0x00000003
	.section .rom.00628182, "a"
	.incbin "baserom.gba", 0x00628182, 0x00000002
	.section .rom.006282da, "a"
	.incbin "baserom.gba", 0x006282da, 0x00000002
	.section .rom.00630127, "a"
	.incbin "baserom.gba", 0x00630127, 0x00000001
	.section .rom.00630293, "a"
	.incbin "baserom.gba", 0x00630293, 0x00000001
	.section .rom.0063302f, "a"
	.incbin "baserom.gba", 0x0063302f, 0x00000001
	.section .rom.006351be, "a"
	.incbin "baserom.gba", 0x006351be, 0x00000002
	.section .rom.006358c6, "a"
	.incbin "baserom.gba", 0x006358c6, 0x00000002
	.section .rom.006364da, "a"
	.incbin "baserom.gba", 0x006364da, 0x00000002
	.section .rom.006475ad, "a"
	.incbin "baserom.gba", 0x006475ad, 0x00000003
	.section .rom.00647742, "a"
	.incbin "baserom.gba", 0x00647742, 0x00000002
	.section .rom.00648c52, "a"
	.incbin "baserom.gba", 0x00648c52, 0x00000002
	.section .rom.0064aa37, "a"
	.incbin "baserom.gba", 0x0064aa37, 0x00000001
	.section .rom.0064baca, "a"
	.incbin "baserom.gba", 0x0064baca, 0x00000002
	.section .rom.0064dd3a, "a"
	.incbin "baserom.gba", 0x0064dd3a, 0x00000002
	.section .rom.0064ded1, "a"
	.incbin "baserom.gba", 0x0064ded1, 0x00000003
	.section .rom.0064fe77, "a"
	.incbin "baserom.gba", 0x0064fe77, 0x00000001
	.section .rom.00654c4e, "a"
	.incbin "baserom.gba", 0x00654c4e, 0x00000002
	.section .rom.006586af, "a"
	.incbin "baserom.gba", 0x006586af, 0x00000001
	.section .rom.0065b3e5, "a"
	.incbin "baserom.gba", 0x0065b3e5, 0x00000003
	.section .rom.0065d72d, "a"
	.incbin "baserom.gba", 0x0065d72d, 0x00000003
	.section .rom.0065ef0b, "a"
	.incbin "baserom.gba", 0x0065ef0b, 0x00000001
	.section .rom.0065f0bd, "a"
	.incbin "baserom.gba", 0x0065f0bd, 0x00000003
	.section .rom.0066fa5e, "a"
	.incbin "baserom.gba", 0x0066fa5e, 0x00000002
	.section .rom.0066fbbd, "a"
	.incbin "baserom.gba", 0x0066fbbd, 0x00000003
	.section .rom.00673317, "a"
	.incbin "baserom.gba", 0x00673317, 0x00000001
	.section .rom.0067347a, "a"
	.incbin "baserom.gba", 0x0067347a, 0x00000002
	.section .rom.0067915d, "a"
	.incbin "baserom.gba", 0x0067915d, 0x00000003
	.section .rom.0067bb43, "a"
	.incbin "baserom.gba", 0x0067bb43, 0x00000001
	.section .rom.0067dd4e, "a"
	.incbin "baserom.gba", 0x0067dd4e, 0x00000002
	.section .rom.0067de8f, "a"
	.incbin "baserom.gba", 0x0067de8f, 0x00000001
	.section .rom.0067f8a6, "a"
	.incbin "baserom.gba", 0x0067f8a6, 0x00000002
	.section .rom.0068660d, "a"
	.incbin "baserom.gba", 0x0068660d, 0x00000003
	.section .rom.0068678b, "a"
	.incbin "baserom.gba", 0x0068678b, 0x00000001
	.section .rom.00688735, "a"
	.incbin "baserom.gba", 0x00688735, 0x00000003
	.section .rom.0068aeae, "a"
	.incbin "baserom.gba", 0x0068aeae, 0x00000002
	.section .rom.0068c715, "a"
	.incbin "baserom.gba", 0x0068c715, 0x00000003
	.section .rom.00690136, "a"
	.incbin "baserom.gba", 0x00690136, 0x00000002
	.section .rom.006902ff, "a"
	.incbin "baserom.gba", 0x006902ff, 0x00000001
	.section .rom.0069b2e7, "a"
	.incbin "baserom.gba", 0x0069b2e7, 0x00000001
	.section .rom.0069d433, "a"
	.incbin "baserom.gba", 0x0069d433, 0x00000001
	.section .rom.0069d5bd, "a"
	.incbin "baserom.gba", 0x0069d5bd, 0x00000003
	.section .rom.0069f042, "a"
	.incbin "baserom.gba", 0x0069f042, 0x00000002
	.section .rom.006a2079, "a"
	.incbin "baserom.gba", 0x006a2079, 0x00000003
	.section .rom.006a223f, "a"
	.incbin "baserom.gba", 0x006a223f, 0x00000001
	.section .rom.006a3173, "a"
	.incbin "baserom.gba", 0x006a3173, 0x00000001
	.section .rom.006a5226, "a"
	.incbin "baserom.gba", 0x006a5226, 0x00000002
	.section .rom.006a53cd, "a"
	.incbin "baserom.gba", 0x006a53cd, 0x00000003
	.section .rom.006aa3f7, "a"
	.incbin "baserom.gba", 0x006aa3f7, 0x00000001
	.section .rom.006ad187, "a"
	.incbin "baserom.gba", 0x006ad187, 0x00000001
	.section .rom.006ad347, "a"
	.incbin "baserom.gba", 0x006ad347, 0x00000001
	.section .rom.006af83b, "a"
	.incbin "baserom.gba", 0x006af83b, 0x00000001
	.section .rom.006afa0d, "a"
	.incbin "baserom.gba", 0x006afa0d, 0x00000003
	.section .rom.006b1a9e, "a"
	.incbin "baserom.gba", 0x006b1a9e, 0x00000002
	.section .rom.006b3b9e, "a"
	.incbin "baserom.gba", 0x006b3b9e, 0x00000002
	.section .rom.006b6752, "a"
	.incbin "baserom.gba", 0x006b6752, 0x00000002
	.section .rom.006b723f, "a"
	.incbin "baserom.gba", 0x006b723f, 0x00000001
	.section .rom.006b7412, "a"
	.incbin "baserom.gba", 0x006b7412, 0x00000002
	.section .rom.006b9d41, "a"
	.incbin "baserom.gba", 0x006b9d41, 0x00000003
	.section .rom.006bb4d9, "a"
	.incbin "baserom.gba", 0x006bb4d9, 0x00000003
	.section .rom.006bc74d, "a"
	.incbin "baserom.gba", 0x006bc74d, 0x00000003
	.section .rom.006bebdd, "a"
	.incbin "baserom.gba", 0x006bebdd, 0x00000003
	.section .rom.006c0f29, "a"
	.incbin "baserom.gba", 0x006c0f29, 0x00000003
	.section .rom.006c2113, "a"
	.incbin "baserom.gba", 0x006c2113, 0x00000001
	.section .rom.006c229a, "a"
	.incbin "baserom.gba", 0x006c229a, 0x00000002
	.section .rom.006c67e6, "a"
	.incbin "baserom.gba", 0x006c67e6, 0x00000002
	.section .rom.006c693b, "a"
	.incbin "baserom.gba", 0x006c693b, 0x00000001
	.section .rom.006c6a7b, "a"
	.incbin "baserom.gba", 0x006c6a7b, 0x00000001
	.section .rom.006c774f, "a"
	.incbin "baserom.gba", 0x006c774f, 0x00000001
	.section .rom.006c78ce, "a"
	.incbin "baserom.gba", 0x006c78ce, 0x00000002
	.section .rom.006c9187, "a"
	.incbin "baserom.gba", 0x006c9187, 0x00000001
	.section .rom.006c9309, "a"
	.incbin "baserom.gba", 0x006c9309, 0x00000003
	.section .rom.006cad9b, "a"
	.incbin "baserom.gba", 0x006cad9b, 0x00000001
	.section .rom.006caf15, "a"
	.incbin "baserom.gba", 0x006caf15, 0x00000003
	.section .rom.006ccc53, "a"
	.incbin "baserom.gba", 0x006ccc53, 0x00000001
	.section .rom.006ccd7f, "a"
	.incbin "baserom.gba", 0x006ccd7f, 0x00000001
	.section .rom.006ccef5, "a"
	.incbin "baserom.gba", 0x006ccef5, 0x00000003
	.section .rom.006ced9b, "a"
	.incbin "baserom.gba", 0x006ced9b, 0x00000001
	.section .rom.006cef21, "a"
	.incbin "baserom.gba", 0x006cef21, 0x00000003
	.section .rom.006cfe7f, "a"
	.incbin "baserom.gba", 0x006cfe7f, 0x00000001
	.section .rom.006d1123, "a"
	.incbin "baserom.gba", 0x006d1123, 0x00000001
	.section .rom.006d12de, "a"
	.incbin "baserom.gba", 0x006d12de, 0x00000002
	.section .rom.006d3d82, "a"
	.incbin "baserom.gba", 0x006d3d82, 0x00000002
	.section .rom.006d3ef5, "a"
	.incbin "baserom.gba", 0x006d3ef5, 0x00000003
	.section .rom.006d52dd, "a"
	.incbin "baserom.gba", 0x006d52dd, 0x00000003
	.section .rom.006d8ced, "a"
	.incbin "baserom.gba", 0x006d8ced, 0x00000003
	.section .rom.006d8e97, "a"
	.incbin "baserom.gba", 0x006d8e97, 0x00000001
	.section .rom.006db292, "a"
	.incbin "baserom.gba", 0x006db292, 0x00000002
	.section .rom.006db421, "a"
	.incbin "baserom.gba", 0x006db421, 0x00000003
	.section .rom.006de6a1, "a"
	.incbin "baserom.gba", 0x006de6a1, 0x00000003
	.section .rom.006de882, "a"
	.incbin "baserom.gba", 0x006de882, 0x00000002
	.section .rom.006e0e17, "a"
	.incbin "baserom.gba", 0x006e0e17, 0x00000001
	.section .rom.006e0fa6, "a"
	.incbin "baserom.gba", 0x006e0fa6, 0x00000002
	.section .rom.006e3b9e, "a"
	.incbin "baserom.gba", 0x006e3b9e, 0x00000002
	.section .rom.006e46eb, "a"
	.incbin "baserom.gba", 0x006e46eb, 0x00000001
	.section .rom.006e4852, "a"
	.incbin "baserom.gba", 0x006e4852, 0x00000002
	.section .rom.006e7267, "a"
	.incbin "baserom.gba", 0x006e7267, 0x00000001
	.section .rom.006e8ea6, "a"
	.incbin "baserom.gba", 0x006e8ea6, 0x00000002
	.section .rom.006e9363, "a"
	.incbin "baserom.gba", 0x006e9363, 0x00000001
	.section .rom.006ea6db, "a"
	.incbin "baserom.gba", 0x006ea6db, 0x00000001
	.section .rom.006ebc1f, "a"
	.incbin "baserom.gba", 0x006ebc1f, 0x00000001
	.section .rom.006ebda9, "a"
	.incbin "baserom.gba", 0x006ebda9, 0x00000003
	.section .rom.006ece87, "a"
	.incbin "baserom.gba", 0x006ece87, 0x00000001
	.section .rom.006ed015, "a"
	.incbin "baserom.gba", 0x006ed015, 0x00000003
	.section .rom.006eec69, "a"
	.incbin "baserom.gba", 0x006eec69, 0x00000003
	.section .rom.006f164b, "a"
	.incbin "baserom.gba", 0x006f164b, 0x00000001
	.section .rom.006f4b75, "a"
	.incbin "baserom.gba", 0x006f4b75, 0x00000003
	.section .rom.006f647f, "a"
	.incbin "baserom.gba", 0x006f647f, 0x00000001
	.section .rom.006f8527, "a"
	.incbin "baserom.gba", 0x006f8527, 0x00000001
	.section .rom.006faf7b, "a"
	.incbin "baserom.gba", 0x006faf7b, 0x00000001
	.section .rom.006fcbc7, "a"
	.incbin "baserom.gba", 0x006fcbc7, 0x00000001
	.section .rom.006fe00d, "a"
	.incbin "baserom.gba", 0x006fe00d, 0x00000003
	.section .rom.006ffe8d, "a"
	.incbin "baserom.gba", 0x006ffe8d, 0x00000003
	.section .rom.00701a5e, "a"
	.incbin "baserom.gba", 0x00701a5e, 0x00000002
	.section .rom.00704192, "a"
	.incbin "baserom.gba", 0x00704192, 0x00000002
	.section .rom.00705dd7, "a"
	.incbin "baserom.gba", 0x00705dd7, 0x00000001
	.section .rom.0070721d, "a"
	.incbin "baserom.gba", 0x0070721d, 0x00000003
	.section .rom.00708d9b, "a"
	.incbin "baserom.gba", 0x00708d9b, 0x00000001
	.section .rom.00709fcb, "a"
	.incbin "baserom.gba", 0x00709fcb, 0x00000001
	.section .rom.0070a125, "a"
	.incbin "baserom.gba", 0x0070a125, 0x00000003
	.section .rom.0070d623, "a"
	.incbin "baserom.gba", 0x0070d623, 0x00000001
	.section .rom.0070d78e, "a"
	.incbin "baserom.gba", 0x0070d78e, 0x00000002
	.section .rom.0071012a, "a"
	.incbin "baserom.gba", 0x0071012a, 0x00001166
	.section .rom.00712a75, "a"
	.incbin "baserom.gba", 0x00712a75, 0x00000003
	.section .rom.00715cbb, "a"
	.incbin "baserom.gba", 0x00715cbb, 0x00000001
	.section .rom.007185b2, "a"
	.incbin "baserom.gba", 0x007185b2, 0x00000002
	.section .rom.007218df, "a"
	.incbin "baserom.gba", 0x007218df, 0x00000001
	.section .rom.00722c0e, "a"
	.incbin "baserom.gba", 0x00722c0e, 0x00000002
	.section .rom.00723e07, "a"
	.incbin "baserom.gba", 0x00723e07, 0x00000001
	.section .rom.00723f69, "a"
	.incbin "baserom.gba", 0x00723f69, 0x00000003
	.section .rom.00726935, "a"
	.incbin "baserom.gba", 0x00726935, 0x00000003
	.section .rom.00729e61, "a"
	.incbin "baserom.gba", 0x00729e61, 0x00000003
	.section .rom.0072e29f, "a"
	.incbin "baserom.gba", 0x0072e29f, 0x00000001
	.section .rom.0072e412, "a"
	.incbin "baserom.gba", 0x0072e412, 0x00000002
	.section .rom.00732677, "a"
	.incbin "baserom.gba", 0x00732677, 0x00000001
	.section .rom.00735e92, "a"
	.incbin "baserom.gba", 0x00735e92, 0x00000002
	.section .rom.007378ba, "a"
	.incbin "baserom.gba", 0x007378ba, 0x00000002
	.section .rom.00739f67, "a"
	.incbin "baserom.gba", 0x00739f67, 0x00000001
	.section .rom.0073bfcb, "a"
	.incbin "baserom.gba", 0x0073bfcb, 0x00000001
	.section .rom.0073de87, "a"
	.incbin "baserom.gba", 0x0073de87, 0x00000001
	.section .rom.007410d1, "a"
	.incbin "baserom.gba", 0x007410d1, 0x00000003
	.section .rom.00742a93, "a"
	.incbin "baserom.gba", 0x00742a93, 0x00000001
	.section .rom.00742c05, "a"
	.incbin "baserom.gba", 0x00742c05, 0x00000003
	.section .rom.00745c12, "a"
	.incbin "baserom.gba", 0x00745c12, 0x00000002
	.section .rom.00745dca, "a"
	.incbin "baserom.gba", 0x00745dca, 0x00000002
	.section .rom.0074763b, "a"
	.incbin "baserom.gba", 0x0074763b, 0x00000001
	.section .rom.007477ba, "a"
	.incbin "baserom.gba", 0x007477ba, 0x00000002
	.section .rom.0074afc3, "a"
	.incbin "baserom.gba", 0x0074afc3, 0x00000001
	.section .rom.0074b137, "a"
	.incbin "baserom.gba", 0x0074b137, 0x00000001
	.section .rom.0074beff, "a"
	.incbin "baserom.gba", 0x0074beff, 0x00000001
	.section .rom.0074c0a5, "a"
	.incbin "baserom.gba", 0x0074c0a5, 0x00000003
	.section .rom.00750ee3, "a"
	.incbin "baserom.gba", 0x00750ee3, 0x00000001
	.section .rom.007510ca, "a"
	.incbin "baserom.gba", 0x007510ca, 0x00000002
	.section .rom.00754366, "a"
	.incbin "baserom.gba", 0x00754366, 0x00004606
	.section .rom.00758a72, "a"
	.incbin "baserom.gba", 0x00758a72, 0x00000002
	.section .rom.0075a649, "a"
	.incbin "baserom.gba", 0x0075a649, 0x00000003
	.section .rom.0075c0f9, "a"
	.incbin "baserom.gba", 0x0075c0f9, 0x00000003
	.section .rom.0075c255, "a"
	.incbin "baserom.gba", 0x0075c255, 0x00000003
	.section .rom.0076249b, "a"
	.incbin "baserom.gba", 0x0076249b, 0x00000001
	.section .rom.0076259f, "a"
	.incbin "baserom.gba", 0x0076259f, 0x00000001
	.section .rom.00762fbb, "a"
	.incbin "baserom.gba", 0x00762fbb, 0x00000001
	.section .rom.007630a1, "a"
	.incbin "baserom.gba", 0x007630a1, 0x00000003
	.section .rom.0076333d, "a"
	.incbin "baserom.gba", 0x0076333d, 0x00000003
	.section .rom.0076739a, "a"
	.incbin "baserom.gba", 0x0076739a, 0x00000002
	.section .rom.0076794e, "a"
	.incbin "baserom.gba", 0x0076794e, 0x00000002
	.section .rom.007688e3, "a"
	.incbin "baserom.gba", 0x007688e3, 0x00000001
	.section .rom.0076a2e6, "a"
	.incbin "baserom.gba", 0x0076a2e6, 0x00000002
	.section .rom.0076af55, "a"
	.incbin "baserom.gba", 0x0076af55, 0x00000003
	.section .rom.0076b1fd, "a"
	.incbin "baserom.gba", 0x0076b1fd, 0x00000003
	.section .rom.0076c082, "a"
	.incbin "baserom.gba", 0x0076c082, 0x00000002
	.section .rom.0076d213, "a"
	.incbin "baserom.gba", 0x0076d213, 0x00000001
	.section .rom.0076d3db, "a"
	.incbin "baserom.gba", 0x0076d3db, 0x00000001
	.section .rom.0076d727, "a"
	.incbin "baserom.gba", 0x0076d727, 0x0000051d
	.section .rom.0076df8f, "a"
	.incbin "baserom.gba", 0x0076df8f, 0x0000051d
	.section .rom.0076e7f7, "a"
	.incbin "baserom.gba", 0x0076e7f7, 0x0000051d
	.section .rom.0076f05f, "a"
	.incbin "baserom.gba", 0x0076f05f, 0x0000051d
	.section .rom.0076f8c7, "a"
	.incbin "baserom.gba", 0x0076f8c7, 0x0000051d
	.section .rom.0077012f, "a"
	.incbin "baserom.gba", 0x0077012f, 0x0000051d
	.section .rom.00770997, "a"
	.incbin "baserom.gba", 0x00770997, 0x0000051d
	.section .rom.007711ff, "a"
	.incbin "baserom.gba", 0x007711ff, 0x0008ee01
