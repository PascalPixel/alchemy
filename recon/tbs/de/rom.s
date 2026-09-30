@ tbs-de's scaffold: the base-ROM ranges its MAIN.LD places between the
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
	.incbin "baserom.gba", 0x0000967c, 0x0000b15c
	.global Tile_Decompress4bpp
Tile_Decompress4bpp:
	.incbin "baserom.gba", 0x000147d8, 0x00000278
	.global Tile_Decompress4bppEnd
Tile_Decompress4bppEnd:
	.global Tile_ExpandMasked
Tile_ExpandMasked:
	.incbin "baserom.gba", 0x00014a50, 0x0000009c
	.global Tile_ExpandMaskedEnd
Tile_ExpandMaskedEnd:
	.global Tile_ExpandOpaque
Tile_ExpandOpaque:
	.incbin "baserom.gba", 0x00014aec, 0x0000007c
	.global Tile_ExpandOpaqueEnd
Tile_ExpandOpaqueEnd:
	.incbin "baserom.gba", 0x00014b68, 0x0001b9c4
	.global RenderResource_PairSourceTable
RenderResource_PairSourceTable:
	.incbin "baserom.gba", 0x0003052c, 0x00000aa8
	.section .rom.00032bd4, "a"
	.incbin "baserom.gba", 0x00032bd4, 0x00062b1c
	.global Object_GetById
	.thumb_func
Object_GetById:
	.incbin "baserom.gba", 0x000956f0, 0x0005bf34
	.global SentouKouka_Tenkai
SentouKouka_Tenkai:
	.incbin "baserom.gba", 0x000f1624, 0x00000230
	.global SentouKouka_TenkaiEnd
SentouKouka_TenkaiEnd:
	.incbin "baserom.gba", 0x000f1854, 0x00008c20
	.section .rom.000fa474, "a"
	.incbin "baserom.gba", 0x000fa474, 0x0000039e
	.global Mixer_CallViaR3
	.thumb_func
Mixer_CallViaR3:
	.incbin "baserom.gba", 0x000fa812, 0x00000006
	.section .rom.000fa818, "a"
	.incbin "baserom.gba", 0x000fa818, 0x00002060
	.section .rom.000fd304, "a"
	.incbin "baserom.gba", 0x000fd304, 0x00000090
	.section .rom.000fd424, "a"
	.incbin "baserom.gba", 0x000fd424, 0x00000060
	.section .rom.00185498, "a"
	.incbin "baserom.gba", 0x00185498, 0x0000098c
	.section .rom.0031fdd8, "a"
	.incbin "baserom.gba", 0x0031fdd8, 0x00000fd8
	.section .rom.003247e7, "a"
	.incbin "baserom.gba", 0x003247e7, 0x00000001
	.section .rom.0032ae9b, "a"
	.incbin "baserom.gba", 0x0032ae9b, 0x000086f9
	.section .rom.003355e5, "a"
	.incbin "baserom.gba", 0x003355e5, 0x00000003
	.section .rom.00336ef5, "a"
	.incbin "baserom.gba", 0x00336ef5, 0x00000003
	.section .rom.0033a9f9, "a"
	.incbin "baserom.gba", 0x0033a9f9, 0x0000087b
	.section .rom.0033f6b6, "a"
	.incbin "baserom.gba", 0x0033f6b6, 0x00000002
	.section .rom.0034fe26, "a"
	.incbin "baserom.gba", 0x0034fe26, 0x00000002
	.section .rom.00353416, "a"
	.incbin "baserom.gba", 0x00353416, 0x00000002
	.section .rom.0035eeba, "a"
	.incbin "baserom.gba", 0x0035eeba, 0x00000002
	.section .rom.0036f56a, "a"
	.incbin "baserom.gba", 0x0036f56a, 0x00000002
	.section .rom.0037700a, "a"
	.incbin "baserom.gba", 0x0037700a, 0x00000002
	.section .rom.0037a84a, "a"
	.incbin "baserom.gba", 0x0037a84a, 0x00000002
	.section .rom.00383c7e, "a"
	.incbin "baserom.gba", 0x00383c7e, 0x00000002
	.section .rom.0038f0a6, "a"
	.incbin "baserom.gba", 0x0038f0a6, 0x00000002
	.section .rom.00396f62, "a"
	.incbin "baserom.gba", 0x00396f62, 0x00000002
	.section .rom.0039b9f2, "a"
	.incbin "baserom.gba", 0x0039b9f2, 0x00000002
	.section .rom.0039fca6, "a"
	.incbin "baserom.gba", 0x0039fca6, 0x00000002
	.section .rom.003a3626, "a"
	.incbin "baserom.gba", 0x003a3626, 0x00000002
	.section .rom.003afe62, "a"
	.incbin "baserom.gba", 0x003afe62, 0x00000002
	.section .rom.003be7aa, "a"
	.incbin "baserom.gba", 0x003be7aa, 0x00000002
	.section .rom.003c40ad, "a"
	.incbin "baserom.gba", 0x003c40ad, 0x00000003
	.section .rom.003c5a2f, "a"
	.incbin "baserom.gba", 0x003c5a2f, 0x00000001
	.section .rom.003c884d, "a"
	.incbin "baserom.gba", 0x003c884d, 0x00000003
	.section .rom.003cc349, "a"
	.incbin "baserom.gba", 0x003cc349, 0x00000003
	.section .rom.003cdb5f, "a"
	.incbin "baserom.gba", 0x003cdb5f, 0x00000001
	.section .rom.003ce14a, "a"
	.incbin "baserom.gba", 0x003ce14a, 0x00000002
	.section .rom.003ce80e, "a"
	.incbin "baserom.gba", 0x003ce80e, 0x00000002
	.section .rom.003ceaf5, "a"
	.incbin "baserom.gba", 0x003ceaf5, 0x00000003
	.section .rom.003cfe5b, "a"
	.incbin "baserom.gba", 0x003cfe5b, 0x00000001
	.section .rom.003d0245, "a"
	.incbin "baserom.gba", 0x003d0245, 0x00000003
	.section .rom.003d0617, "a"
	.incbin "baserom.gba", 0x003d0617, 0x00000001
	.section .rom.003d0eb7, "a"
	.incbin "baserom.gba", 0x003d0eb7, 0x00000001
	.section .rom.003d135a, "a"
	.incbin "baserom.gba", 0x003d135a, 0x00000002
	.section .rom.003d2513, "a"
	.incbin "baserom.gba", 0x003d2513, 0x00000001
	.section .rom.003d39d6, "a"
	.incbin "baserom.gba", 0x003d39d6, 0x00000002
	.section .rom.003d499f, "a"
	.incbin "baserom.gba", 0x003d499f, 0x00000001
	.section .rom.003d4bfa, "a"
	.incbin "baserom.gba", 0x003d4bfa, 0x00000002
	.section .rom.003d6655, "a"
	.incbin "baserom.gba", 0x003d6655, 0x00000003
	.section .rom.003d6ba1, "a"
	.incbin "baserom.gba", 0x003d6ba1, 0x00000003
	.section .rom.003d892a, "a"
	.incbin "baserom.gba", 0x003d892a, 0x00000002
	.section .rom.003d8ba3, "a"
	.incbin "baserom.gba", 0x003d8ba3, 0x00000001
	.section .rom.003d906a, "a"
	.incbin "baserom.gba", 0x003d906a, 0x00000002
	.section .rom.003dac3f, "a"
	.incbin "baserom.gba", 0x003dac3f, 0x00000001
	.section .rom.003dc83d, "a"
	.incbin "baserom.gba", 0x003dc83d, 0x00000003
	.section .rom.003dca5e, "a"
	.incbin "baserom.gba", 0x003dca5e, 0x00000002
	.section .rom.003dce9b, "a"
	.incbin "baserom.gba", 0x003dce9b, 0x00000001
	.section .rom.003dcfad, "a"
	.incbin "baserom.gba", 0x003dcfad, 0x00000003
	.section .rom.003dd72b, "a"
	.incbin "baserom.gba", 0x003dd72b, 0x00000001
	.section .rom.003ddbc9, "a"
	.incbin "baserom.gba", 0x003ddbc9, 0x00000003
	.section .rom.003de8de, "a"
	.incbin "baserom.gba", 0x003de8de, 0x00000002
	.section .rom.003df65e, "a"
	.incbin "baserom.gba", 0x003df65e, 0x00000002
	.section .rom.003df9ef, "a"
	.incbin "baserom.gba", 0x003df9ef, 0x00000001
	.section .rom.003e1736, "a"
	.incbin "baserom.gba", 0x003e1736, 0x00000002
	.section .rom.003e250d, "a"
	.incbin "baserom.gba", 0x003e250d, 0x00000003
	.section .rom.003e2d3a, "a"
	.incbin "baserom.gba", 0x003e2d3a, 0x00000002
	.section .rom.003e33c5, "a"
	.incbin "baserom.gba", 0x003e33c5, 0x00000003
	.section .rom.003e3583, "a"
	.incbin "baserom.gba", 0x003e3583, 0x00000001
	.section .rom.003e42db, "a"
	.incbin "baserom.gba", 0x003e42db, 0x00000001
	.section .rom.003e4827, "a"
	.incbin "baserom.gba", 0x003e4827, 0x00000001
	.section .rom.003e4c01, "a"
	.incbin "baserom.gba", 0x003e4c01, 0x00000003
	.section .rom.003e4faf, "a"
	.incbin "baserom.gba", 0x003e4faf, 0x00000001
	.section .rom.003e6f53, "a"
	.incbin "baserom.gba", 0x003e6f53, 0x00000001
	.section .rom.003e7cef, "a"
	.incbin "baserom.gba", 0x003e7cef, 0x00000001
	.section .rom.003e7f0b, "a"
	.incbin "baserom.gba", 0x003e7f0b, 0x00000001
	.section .rom.003e8207, "a"
	.incbin "baserom.gba", 0x003e8207, 0x00000001
	.section .rom.003ea3b5, "a"
	.incbin "baserom.gba", 0x003ea3b5, 0x00000003
	.section .rom.003eb183, "a"
	.incbin "baserom.gba", 0x003eb183, 0x00000001
	.section .rom.003ec2e5, "a"
	.incbin "baserom.gba", 0x003ec2e5, 0x00000003
	.section .rom.003ec83f, "a"
	.incbin "baserom.gba", 0x003ec83f, 0x00000001
	.section .rom.003ee129, "a"
	.incbin "baserom.gba", 0x003ee129, 0x00000003
	.section .rom.003ee9ca, "a"
	.incbin "baserom.gba", 0x003ee9ca, 0x00000002
	.section .rom.003eeda7, "a"
	.incbin "baserom.gba", 0x003eeda7, 0x00000001
	.section .rom.003ef031, "a"
	.incbin "baserom.gba", 0x003ef031, 0x00000003
	.section .rom.003ef3d7, "a"
	.incbin "baserom.gba", 0x003ef3d7, 0x00000001
	.section .rom.003ef632, "a"
	.incbin "baserom.gba", 0x003ef632, 0x00000002
	.section .rom.003ef9ea, "a"
	.incbin "baserom.gba", 0x003ef9ea, 0x00000002
	.section .rom.003f0ed3, "a"
	.incbin "baserom.gba", 0x003f0ed3, 0x00000001
	.section .rom.003f2496, "a"
	.incbin "baserom.gba", 0x003f2496, 0x00000a6e
	.section .rom.003f3cda, "a"
	.incbin "baserom.gba", 0x003f3cda, 0x00000002
	.section .rom.003f405d, "a"
	.incbin "baserom.gba", 0x003f405d, 0x00000003
	.section .rom.003f4d0d, "a"
	.incbin "baserom.gba", 0x003f4d0d, 0x00000003
	.section .rom.003f5855, "a"
	.incbin "baserom.gba", 0x003f5855, 0x00000003
	.section .rom.003f59ee, "a"
	.incbin "baserom.gba", 0x003f59ee, 0x00000002
	.section .rom.003f627b, "a"
	.incbin "baserom.gba", 0x003f627b, 0x00000001
	.section .rom.003f6d42, "a"
	.incbin "baserom.gba", 0x003f6d42, 0x00000002
	.section .rom.003f725a, "a"
	.incbin "baserom.gba", 0x003f725a, 0x00000002
	.section .rom.003f787e, "a"
	.incbin "baserom.gba", 0x003f787e, 0x00000002
	.section .rom.003f80e5, "a"
	.incbin "baserom.gba", 0x003f80e5, 0x00000003
	.section .rom.003f8509, "a"
	.incbin "baserom.gba", 0x003f8509, 0x00000003
	.section .rom.003f87a3, "a"
	.incbin "baserom.gba", 0x003f87a3, 0x00000001
	.section .rom.003fa13f, "a"
	.incbin "baserom.gba", 0x003fa13f, 0x00000001
	.section .rom.003fbeb6, "a"
	.incbin "baserom.gba", 0x003fbeb6, 0x00000002
	.section .rom.003fde2b, "a"
	.incbin "baserom.gba", 0x003fde2b, 0x00000001
	.section .rom.003fe2fb, "a"
	.incbin "baserom.gba", 0x003fe2fb, 0x00000001
	.section .rom.003fe98d, "a"
	.incbin "baserom.gba", 0x003fe98d, 0x00000a37
	.section .rom.00400795, "a"
	.incbin "baserom.gba", 0x00400795, 0x00000003
	.section .rom.00401266, "a"
	.incbin "baserom.gba", 0x00401266, 0x00000002
	.section .rom.00401da3, "a"
	.incbin "baserom.gba", 0x00401da3, 0x00000001
	.section .rom.004023e3, "a"
	.incbin "baserom.gba", 0x004023e3, 0x00001589
	.section .rom.004039cd, "a"
	.incbin "baserom.gba", 0x004039cd, 0x00000003
	.section .rom.00403d3b, "a"
	.incbin "baserom.gba", 0x00403d3b, 0x00000001
	.section .rom.004042d9, "a"
	.incbin "baserom.gba", 0x004042d9, 0x00000003
	.section .rom.0040460d, "a"
	.incbin "baserom.gba", 0x0040460d, 0x00000003
	.section .rom.00404949, "a"
	.incbin "baserom.gba", 0x00404949, 0x00000003
	.section .rom.00405962, "a"
	.incbin "baserom.gba", 0x00405962, 0x00000002
	.section .rom.00408f6e, "a"
	.incbin "baserom.gba", 0x00408f6e, 0x00000002
	.section .rom.00409711, "a"
	.incbin "baserom.gba", 0x00409711, 0x00000003
	.section .rom.0040aaa3, "a"
	.incbin "baserom.gba", 0x0040aaa3, 0x00000001
	.section .rom.0040aed3, "a"
	.incbin "baserom.gba", 0x0040aed3, 0x00000001
	.section .rom.0040ccc9, "a"
	.incbin "baserom.gba", 0x0040ccc9, 0x00000003
	.section .rom.0040d5de, "a"
	.incbin "baserom.gba", 0x0040d5de, 0x00000002
	.section .rom.0040f112, "a"
	.incbin "baserom.gba", 0x0040f112, 0x00000002
	.section .rom.00410161, "a"
	.incbin "baserom.gba", 0x00410161, 0x00000003
	.section .rom.00410817, "a"
	.incbin "baserom.gba", 0x00410817, 0x00000001
	.section .rom.004118bd, "a"
	.incbin "baserom.gba", 0x004118bd, 0x00000003
	.section .rom.00424ccf, "a"
	.incbin "baserom.gba", 0x00424ccf, 0x00000001
	.section .rom.00424e21, "a"
	.incbin "baserom.gba", 0x00424e21, 0x00000003
	.section .rom.004252b2, "a"
	.incbin "baserom.gba", 0x004252b2, 0x00000002
	.section .rom.004254a9, "a"
	.incbin "baserom.gba", 0x004254a9, 0x00000003
	.section .rom.00426b66, "a"
	.incbin "baserom.gba", 0x00426b66, 0x00000002
	.section .rom.00428085, "a"
	.incbin "baserom.gba", 0x00428085, 0x00000003
	.section .rom.00428c51, "a"
	.incbin "baserom.gba", 0x00428c51, 0x00000003
	.section .rom.004297c6, "a"
	.incbin "baserom.gba", 0x004297c6, 0x00000002
	.section .rom.0042b66f, "a"
	.incbin "baserom.gba", 0x0042b66f, 0x00000001
	.section .rom.0042cdfd, "a"
	.incbin "baserom.gba", 0x0042cdfd, 0x00000003
	.section .rom.0042e2ae, "a"
	.incbin "baserom.gba", 0x0042e2ae, 0x00000002
	.section .rom.004309fb, "a"
	.incbin "baserom.gba", 0x004309fb, 0x00000001
	.section .rom.00432295, "a"
	.incbin "baserom.gba", 0x00432295, 0x00000003
	.section .rom.0043385e, "a"
	.incbin "baserom.gba", 0x0043385e, 0x00000002
	.section .rom.004352ee, "a"
	.incbin "baserom.gba", 0x004352ee, 0x00000002
	.section .rom.004360ea, "a"
	.incbin "baserom.gba", 0x004360ea, 0x00000002
	.section .rom.0043fa5a, "a"
	.incbin "baserom.gba", 0x0043fa5a, 0x00000002
	.section .rom.00441c0e, "a"
	.incbin "baserom.gba", 0x00441c0e, 0x00000002
	.section .rom.00445d75, "a"
	.incbin "baserom.gba", 0x00445d75, 0x00000003
	.section .rom.00448f9a, "a"
	.incbin "baserom.gba", 0x00448f9a, 0x00000002
	.section .rom.0044a69e, "a"
	.incbin "baserom.gba", 0x0044a69e, 0x00000002
	.section .rom.00451286, "a"
	.incbin "baserom.gba", 0x00451286, 0x00000002
	.section .rom.00459c0a, "a"
	.incbin "baserom.gba", 0x00459c0a, 0x00000002
	.section .rom.004616e6, "a"
	.incbin "baserom.gba", 0x004616e6, 0x00000002
	.section .rom.0046477a, "a"
	.incbin "baserom.gba", 0x0046477a, 0x00000002
	.section .rom.00467b89, "a"
	.incbin "baserom.gba", 0x00467b89, 0x00000003
	.section .rom.0046a3f5, "a"
	.incbin "baserom.gba", 0x0046a3f5, 0x00000003
	.section .rom.0046bee6, "a"
	.incbin "baserom.gba", 0x0046bee6, 0x00000002
	.section .rom.0046ccfe, "a"
	.incbin "baserom.gba", 0x0046ccfe, 0x00000002
	.section .rom.0046d9e9, "a"
	.incbin "baserom.gba", 0x0046d9e9, 0x00000003
	.section .rom.00476ea6, "a"
	.incbin "baserom.gba", 0x00476ea6, 0x00000002
	.section .rom.00477bb1, "a"
	.incbin "baserom.gba", 0x00477bb1, 0x00000003
	.section .rom.00479cc5, "a"
	.incbin "baserom.gba", 0x00479cc5, 0x00000003
	.section .rom.0047a937, "a"
	.incbin "baserom.gba", 0x0047a937, 0x00000001
	.section .rom.0047b54f, "a"
	.incbin "baserom.gba", 0x0047b54f, 0x00000001
	.section .rom.0047bcc3, "a"
	.incbin "baserom.gba", 0x0047bcc3, 0x00000001
	.section .rom.0047d553, "a"
	.incbin "baserom.gba", 0x0047d553, 0x00000001
	.section .rom.0047df21, "a"
	.incbin "baserom.gba", 0x0047df21, 0x00000003
	.section .rom.0047eb3f, "a"
	.incbin "baserom.gba", 0x0047eb3f, 0x00000001
	.section .rom.004811f3, "a"
	.incbin "baserom.gba", 0x004811f3, 0x00000001
	.section .rom.00483906, "a"
	.incbin "baserom.gba", 0x00483906, 0x00000002
	.section .rom.0048885b, "a"
	.incbin "baserom.gba", 0x0048885b, 0x00000001
	.section .rom.0048cb85, "a"
	.incbin "baserom.gba", 0x0048cb85, 0x00000003
	.section .rom.0048d772, "a"
	.incbin "baserom.gba", 0x0048d772, 0x00000002
	.section .rom.00492327, "a"
	.incbin "baserom.gba", 0x00492327, 0x00000001
	.section .rom.00494691, "a"
	.incbin "baserom.gba", 0x00494691, 0x00000003
	.section .rom.00496033, "a"
	.incbin "baserom.gba", 0x00496033, 0x00000001
	.section .rom.0049a8f5, "a"
	.incbin "baserom.gba", 0x0049a8f5, 0x00000003
	.section .rom.0049e9bd, "a"
	.incbin "baserom.gba", 0x0049e9bd, 0x00000003
	.section .rom.004a2086, "a"
	.incbin "baserom.gba", 0x004a2086, 0x00000002
	.section .rom.004aa057, "a"
	.incbin "baserom.gba", 0x004aa057, 0x00000001
	.section .rom.004b1367, "a"
	.incbin "baserom.gba", 0x004b1367, 0x00000001
	.section .rom.004b62e7, "a"
	.incbin "baserom.gba", 0x004b62e7, 0x00000001
	.section .rom.004bac97, "a"
	.incbin "baserom.gba", 0x004bac97, 0x0000051d
	.section .rom.004c05c3, "a"
	.incbin "baserom.gba", 0x004c05c3, 0x00000001
	.section .rom.004c0751, "a"
	.incbin "baserom.gba", 0x004c0751, 0x00000003
	.section .rom.004c57b1, "a"
	.incbin "baserom.gba", 0x004c57b1, 0x00000003
	.section .rom.004c9d1d, "a"
	.incbin "baserom.gba", 0x004c9d1d, 0x00000003
	.section .rom.004ceee5, "a"
	.incbin "baserom.gba", 0x004ceee5, 0x00000003
	.section .rom.004cf07b, "a"
	.incbin "baserom.gba", 0x004cf07b, 0x00000001
	.section .rom.004d1ccf, "a"
	.incbin "baserom.gba", 0x004d1ccf, 0x00000001
	.section .rom.004d8983, "a"
	.incbin "baserom.gba", 0x004d8983, 0x00000001
	.section .rom.004dbabe, "a"
	.incbin "baserom.gba", 0x004dbabe, 0x00000002
	.section .rom.004dbc3f, "a"
	.incbin "baserom.gba", 0x004dbc3f, 0x00000001
	.section .rom.004de3f9, "a"
	.incbin "baserom.gba", 0x004de3f9, 0x00000003
	.section .rom.004e0a42, "a"
	.incbin "baserom.gba", 0x004e0a42, 0x00000002
	.section .rom.004e3126, "a"
	.incbin "baserom.gba", 0x004e3126, 0x00000002
	.section .rom.004e419b, "a"
	.incbin "baserom.gba", 0x004e419b, 0x00000001
	.section .rom.004e62ee, "a"
	.incbin "baserom.gba", 0x004e62ee, 0x00000002
	.section .rom.004e6481, "a"
	.incbin "baserom.gba", 0x004e6481, 0x00000003
	.section .rom.004e8cc3, "a"
	.incbin "baserom.gba", 0x004e8cc3, 0x00000001
	.section .rom.004eaad9, "a"
	.incbin "baserom.gba", 0x004eaad9, 0x00000003
	.section .rom.004ed1ba, "a"
	.incbin "baserom.gba", 0x004ed1ba, 0x00000002
	.section .rom.004ee301, "a"
	.incbin "baserom.gba", 0x004ee301, 0x00000003
	.section .rom.004f1415, "a"
	.incbin "baserom.gba", 0x004f1415, 0x00000003
	.section .rom.004f557d, "a"
	.incbin "baserom.gba", 0x004f557d, 0x00000003
	.section .rom.004f6b7a, "a"
	.incbin "baserom.gba", 0x004f6b7a, 0x00000002
	.section .rom.004f7e43, "a"
	.incbin "baserom.gba", 0x004f7e43, 0x00000001
	.section .rom.004fb25b, "a"
	.incbin "baserom.gba", 0x004fb25b, 0x00000001
	.section .rom.004fb3e9, "a"
	.incbin "baserom.gba", 0x004fb3e9, 0x00000003
	.section .rom.004fd19a, "a"
	.incbin "baserom.gba", 0x004fd19a, 0x00000002
	.section .rom.00500d11, "a"
	.incbin "baserom.gba", 0x00500d11, 0x00000003
	.section .rom.0050221f, "a"
	.incbin "baserom.gba", 0x0050221f, 0x00000001
	.section .rom.00502df3, "a"
	.incbin "baserom.gba", 0x00502df3, 0x00000001
	.section .rom.00502ef1, "a"
	.incbin "baserom.gba", 0x00502ef1, 0x00000003
	.section .rom.005042d6, "a"
	.incbin "baserom.gba", 0x005042d6, 0x00000002
	.section .rom.00505459, "a"
	.incbin "baserom.gba", 0x00505459, 0x00000003
	.section .rom.00505de2, "a"
	.incbin "baserom.gba", 0x00505de2, 0x00000002
	.section .rom.00508a5f, "a"
	.incbin "baserom.gba", 0x00508a5f, 0x00000001
	.section .rom.00508b42, "a"
	.incbin "baserom.gba", 0x00508b42, 0x00000002
	.section .rom.00509d2d, "a"
	.incbin "baserom.gba", 0x00509d2d, 0x00000003
	.section .rom.0050b9b5, "a"
	.incbin "baserom.gba", 0x0050b9b5, 0x00000003
	.section .rom.0050bec3, "a"
	.incbin "baserom.gba", 0x0050bec3, 0x00000001
	.section .rom.0050c003, "a"
	.incbin "baserom.gba", 0x0050c003, 0x00000001
	.section .rom.0050d21e, "a"
	.incbin "baserom.gba", 0x0050d21e, 0x00000002
	.section .rom.0050d379, "a"
	.incbin "baserom.gba", 0x0050d379, 0x00000003
	.section .rom.0051179f, "a"
	.incbin "baserom.gba", 0x0051179f, 0x00000001
	.section .rom.005134ce, "a"
	.incbin "baserom.gba", 0x005134ce, 0x00000002
	.section .rom.00514247, "a"
	.incbin "baserom.gba", 0x00514247, 0x00000001
	.section .rom.0051437b, "a"
	.incbin "baserom.gba", 0x0051437b, 0x00000001
	.section .rom.005167fb, "a"
	.incbin "baserom.gba", 0x005167fb, 0x00000001
	.section .rom.00517577, "a"
	.incbin "baserom.gba", 0x00517577, 0x00000001
	.section .rom.00518056, "a"
	.incbin "baserom.gba", 0x00518056, 0x00000002
	.section .rom.00518ece, "a"
	.incbin "baserom.gba", 0x00518ece, 0x00000002
	.section .rom.0051afa2, "a"
	.incbin "baserom.gba", 0x0051afa2, 0x00000002
	.section .rom.0051b8e9, "a"
	.incbin "baserom.gba", 0x0051b8e9, 0x00000003
	.section .rom.0051c536, "a"
	.incbin "baserom.gba", 0x0051c536, 0x00000002
	.section .rom.0051c75d, "a"
	.incbin "baserom.gba", 0x0051c75d, 0x00000003
	.section .rom.0051e09f, "a"
	.incbin "baserom.gba", 0x0051e09f, 0x00000001
	.section .rom.0051e1b3, "a"
	.incbin "baserom.gba", 0x0051e1b3, 0x00000001
	.section .rom.0051fb79, "a"
	.incbin "baserom.gba", 0x0051fb79, 0x00000003
	.section .rom.0051fcc3, "a"
	.incbin "baserom.gba", 0x0051fcc3, 0x00000001
	.section .rom.00521d8f, "a"
	.incbin "baserom.gba", 0x00521d8f, 0x00000001
	.section .rom.00521eb7, "a"
	.incbin "baserom.gba", 0x00521eb7, 0x00000001
	.section .rom.0052373f, "a"
	.incbin "baserom.gba", 0x0052373f, 0x00000001
	.section .rom.005259e1, "a"
	.incbin "baserom.gba", 0x005259e1, 0x00000003
	.section .rom.00525b23, "a"
	.incbin "baserom.gba", 0x00525b23, 0x00000001
	.section .rom.00527b2b, "a"
	.incbin "baserom.gba", 0x00527b2b, 0x00000001
	.section .rom.00527c36, "a"
	.incbin "baserom.gba", 0x00527c36, 0x00000002
	.section .rom.00529c1a, "a"
	.incbin "baserom.gba", 0x00529c1a, 0x00000002
	.section .rom.0052b33f, "a"
	.incbin "baserom.gba", 0x0052b33f, 0x00000001
	.section .rom.0052c3f3, "a"
	.incbin "baserom.gba", 0x0052c3f3, 0x00000001
	.section .rom.0052e0da, "a"
	.incbin "baserom.gba", 0x0052e0da, 0x00000002
	.section .rom.0052e235, "a"
	.incbin "baserom.gba", 0x0052e235, 0x00000003
	.section .rom.0053017e, "a"
	.incbin "baserom.gba", 0x0053017e, 0x00000002
	.section .rom.00530303, "a"
	.incbin "baserom.gba", 0x00530303, 0x00000001
	.section .rom.00532e55, "a"
	.incbin "baserom.gba", 0x00532e55, 0x00000003
	.section .rom.0053551a, "a"
	.incbin "baserom.gba", 0x0053551a, 0x00000002
	.section .rom.00537c72, "a"
	.incbin "baserom.gba", 0x00537c72, 0x00000002
	.section .rom.005394c5, "a"
	.incbin "baserom.gba", 0x005394c5, 0x00000003
	.section .rom.0053bbc5, "a"
	.incbin "baserom.gba", 0x0053bbc5, 0x00000003
	.section .rom.0053d991, "a"
	.incbin "baserom.gba", 0x0053d991, 0x00000003
	.section .rom.0053fedb, "a"
	.incbin "baserom.gba", 0x0053fedb, 0x00000001
	.section .rom.005411a6, "a"
	.incbin "baserom.gba", 0x005411a6, 0x00000002
	.section .rom.00541e57, "a"
	.incbin "baserom.gba", 0x00541e57, 0x00000001
	.section .rom.005428c1, "a"
	.incbin "baserom.gba", 0x005428c1, 0x00000003
	.section .rom.005429b7, "a"
	.incbin "baserom.gba", 0x005429b7, 0x00000001
	.section .rom.00544416, "a"
	.incbin "baserom.gba", 0x00544416, 0x00000002
	.section .rom.005451ba, "a"
	.incbin "baserom.gba", 0x005451ba, 0x00000002
	.section .rom.00546cf3, "a"
	.incbin "baserom.gba", 0x00546cf3, 0x00000001
	.section .rom.00549be1, "a"
	.incbin "baserom.gba", 0x00549be1, 0x00000003
	.section .rom.005513c7, "a"
	.incbin "baserom.gba", 0x005513c7, 0x00000001
	.section .rom.00551493, "a"
	.incbin "baserom.gba", 0x00551493, 0x00000001
	.section .rom.00556d3e, "a"
	.incbin "baserom.gba", 0x00556d3e, 0x00000002
	.section .rom.00556e7f, "a"
	.incbin "baserom.gba", 0x00556e7f, 0x00000001
	.section .rom.005582ad, "a"
	.incbin "baserom.gba", 0x005582ad, 0x00000003
	.section .rom.0055afea, "a"
	.incbin "baserom.gba", 0x0055afea, 0x00000002
	.section .rom.0055d0e6, "a"
	.incbin "baserom.gba", 0x0055d0e6, 0x00000002
	.section .rom.00564aee, "a"
	.incbin "baserom.gba", 0x00564aee, 0x00000002
	.section .rom.00564c8a, "a"
	.incbin "baserom.gba", 0x00564c8a, 0x00000002
	.section .rom.00569525, "a"
	.incbin "baserom.gba", 0x00569525, 0x00000003
	.section .rom.0056ba26, "a"
	.incbin "baserom.gba", 0x0056ba26, 0x00000002
	.section .rom.00573417, "a"
	.incbin "baserom.gba", 0x00573417, 0x00000001
	.section .rom.005735b2, "a"
	.incbin "baserom.gba", 0x005735b2, 0x00000002
	.section .rom.00576161, "a"
	.incbin "baserom.gba", 0x00576161, 0x00000003
	.section .rom.00578235, "a"
	.incbin "baserom.gba", 0x00578235, 0x00000003
	.section .rom.0057a28e, "a"
	.incbin "baserom.gba", 0x0057a28e, 0x00001cee
	.section .rom.0057d725, "a"
	.incbin "baserom.gba", 0x0057d725, 0x00000003
	.section .rom.00580401, "a"
	.incbin "baserom.gba", 0x00580401, 0x00000003
	.section .rom.005821d3, "a"
	.incbin "baserom.gba", 0x005821d3, 0x00000001
	.section .rom.00582313, "a"
	.incbin "baserom.gba", 0x00582313, 0x00000001
	.section .rom.0058536d, "a"
	.incbin "baserom.gba", 0x0058536d, 0x00000003
	.section .rom.005882ea, "a"
	.incbin "baserom.gba", 0x005882ea, 0x00000002
	.section .rom.0058abb1, "a"
	.incbin "baserom.gba", 0x0058abb1, 0x00000003
	.section .rom.00591b19, "a"
	.incbin "baserom.gba", 0x00591b19, 0x00000003
	.section .rom.005933bf, "a"
	.incbin "baserom.gba", 0x005933bf, 0x00000001
	.section .rom.00593cbe, "a"
	.incbin "baserom.gba", 0x00593cbe, 0x00000002
	.section .rom.00594a37, "a"
	.incbin "baserom.gba", 0x00594a37, 0x00000001
	.section .rom.00594b97, "a"
	.incbin "baserom.gba", 0x00594b97, 0x00000001
	.section .rom.005967ef, "a"
	.incbin "baserom.gba", 0x005967ef, 0x00000001
	.section .rom.0059808b, "a"
	.incbin "baserom.gba", 0x0059808b, 0x00000001
	.section .rom.0059820f, "a"
	.incbin "baserom.gba", 0x0059820f, 0x00000001
	.section .rom.0059ad4b, "a"
	.incbin "baserom.gba", 0x0059ad4b, 0x00000001
	.section .rom.0059d4c5, "a"
	.incbin "baserom.gba", 0x0059d4c5, 0x00000003
	.section .rom.0059e4b2, "a"
	.incbin "baserom.gba", 0x0059e4b2, 0x00000002
	.section .rom.0059fb7f, "a"
	.incbin "baserom.gba", 0x0059fb7f, 0x00000001
	.section .rom.0059fd29, "a"
	.incbin "baserom.gba", 0x0059fd29, 0x00000003
	.section .rom.005a2abd, "a"
	.incbin "baserom.gba", 0x005a2abd, 0x00000003
	.section .rom.005a6b52, "a"
	.incbin "baserom.gba", 0x005a6b52, 0x00000002
	.section .rom.005a6c93, "a"
	.incbin "baserom.gba", 0x005a6c93, 0x00000001
	.section .rom.005a8b5b, "a"
	.incbin "baserom.gba", 0x005a8b5b, 0x00000001
	.section .rom.005a8cfd, "a"
	.incbin "baserom.gba", 0x005a8cfd, 0x00000003
	.section .rom.005aad9f, "a"
	.incbin "baserom.gba", 0x005aad9f, 0x00000001
	.section .rom.005abd2c, "a"
	.incbin "baserom.gba", 0x005abd2c, 0x000022d8
	.section .rom.005b0af5, "a"
	.incbin "baserom.gba", 0x005b0af5, 0x00000003
	.section .rom.005b0c7a, "a"
	.incbin "baserom.gba", 0x005b0c7a, 0x00000002
	.section .rom.005b83c7, "a"
	.incbin "baserom.gba", 0x005b83c7, 0x00000001
	.section .rom.005b88d1, "a"
	.incbin "baserom.gba", 0x005b88d1, 0x00000003
	.section .rom.005b9fa9, "a"
	.incbin "baserom.gba", 0x005b9fa9, 0x00000003
	.section .rom.005ba14d, "a"
	.incbin "baserom.gba", 0x005ba14d, 0x00000003
	.section .rom.005bba52, "a"
	.incbin "baserom.gba", 0x005bba52, 0x00000002
	.section .rom.005bd27f, "a"
	.incbin "baserom.gba", 0x005bd27f, 0x00000001
	.section .rom.005bd3fd, "a"
	.incbin "baserom.gba", 0x005bd3fd, 0x00000003
	.section .rom.005c1cf1, "a"
	.incbin "baserom.gba", 0x005c1cf1, 0x00000003
	.section .rom.005c2d36, "a"
	.incbin "baserom.gba", 0x005c2d36, 0x00000002
	.section .rom.005c3b5a, "a"
	.incbin "baserom.gba", 0x005c3b5a, 0x00000002
	.section .rom.005c4e9f, "a"
	.incbin "baserom.gba", 0x005c4e9f, 0x00000001
	.section .rom.005c4ff9, "a"
	.incbin "baserom.gba", 0x005c4ff9, 0x00000003
	.section .rom.005c75b7, "a"
	.incbin "baserom.gba", 0x005c75b7, 0x00000001
	.section .rom.005c772e, "a"
	.incbin "baserom.gba", 0x005c772e, 0x00000002
	.section .rom.005cc021, "a"
	.incbin "baserom.gba", 0x005cc021, 0x00000003
	.section .rom.005cca89, "a"
	.incbin "baserom.gba", 0x005cca89, 0x00000003
	.section .rom.005cf0cb, "a"
	.incbin "baserom.gba", 0x005cf0cb, 0x00000001
	.section .rom.005cf232, "a"
	.incbin "baserom.gba", 0x005cf232, 0x00000002
	.section .rom.005d0379, "a"
	.incbin "baserom.gba", 0x005d0379, 0x00000003
	.section .rom.005d20d7, "a"
	.incbin "baserom.gba", 0x005d20d7, 0x00000001
	.section .rom.005d223e, "a"
	.incbin "baserom.gba", 0x005d223e, 0x00000002
	.section .rom.005d4acd, "a"
	.incbin "baserom.gba", 0x005d4acd, 0x00000003
	.section .rom.005d4c02, "a"
	.incbin "baserom.gba", 0x005d4c02, 0x00000002
	.section .rom.005d783e, "a"
	.incbin "baserom.gba", 0x005d783e, 0x00000002
	.section .rom.005dafdd, "a"
	.incbin "baserom.gba", 0x005dafdd, 0x00000003
	.section .rom.005de32b, "a"
	.incbin "baserom.gba", 0x005de32b, 0x00000001
	.section .rom.005de4a7, "a"
	.incbin "baserom.gba", 0x005de4a7, 0x00000001
	.section .rom.005e1056, "a"
	.incbin "baserom.gba", 0x005e1056, 0x00000002
	.section .rom.005e32ca, "a"
	.incbin "baserom.gba", 0x005e32ca, 0x00000002
	.section .rom.005e5b3b, "a"
	.incbin "baserom.gba", 0x005e5b3b, 0x00000001
	.section .rom.005e902f, "a"
	.incbin "baserom.gba", 0x005e902f, 0x00000001
	.section .rom.005edc56, "a"
	.incbin "baserom.gba", 0x005edc56, 0x00000002
	.section .rom.005ee8d9, "a"
	.incbin "baserom.gba", 0x005ee8d9, 0x00000003
	.section .rom.005eea1b, "a"
	.incbin "baserom.gba", 0x005eea1b, 0x00000001
	.section .rom.005eff8b, "a"
	.incbin "baserom.gba", 0x005eff8b, 0x00000001
	.section .rom.005f00f6, "a"
	.incbin "baserom.gba", 0x005f00f6, 0x00000002
	.section .rom.005f1e51, "a"
	.incbin "baserom.gba", 0x005f1e51, 0x00000003
	.section .rom.005f1f93, "a"
	.incbin "baserom.gba", 0x005f1f93, 0x00000001
	.section .rom.005f4792, "a"
	.incbin "baserom.gba", 0x005f4792, 0x00000002
	.section .rom.005f48d3, "a"
	.incbin "baserom.gba", 0x005f48d3, 0x00000001
	.section .rom.005f62eb, "a"
	.incbin "baserom.gba", 0x005f62eb, 0x00000001
	.section .rom.005f7866, "a"
	.incbin "baserom.gba", 0x005f7866, 0x00000002
	.section .rom.005f8da2, "a"
	.incbin "baserom.gba", 0x005f8da2, 0x00000002
	.section .rom.005f8f32, "a"
	.incbin "baserom.gba", 0x005f8f32, 0x00000002
	.section .rom.005fb1eb, "a"
	.incbin "baserom.gba", 0x005fb1eb, 0x00000001
	.section .rom.005ff432, "a"
	.incbin "baserom.gba", 0x005ff432, 0x00000002
	.section .rom.00601215, "a"
	.incbin "baserom.gba", 0x00601215, 0x00000003
	.section .rom.006043c3, "a"
	.incbin "baserom.gba", 0x006043c3, 0x00000001
	.section .rom.006069f7, "a"
	.incbin "baserom.gba", 0x006069f7, 0x00000001
	.section .rom.0060954f, "a"
	.incbin "baserom.gba", 0x0060954f, 0x00000001
	.section .rom.00609717, "a"
	.incbin "baserom.gba", 0x00609717, 0x00002905
	.section .rom.0060fb21, "a"
	.incbin "baserom.gba", 0x0060fb21, 0x00000003
	.section .rom.00611092, "a"
	.incbin "baserom.gba", 0x00611092, 0x00000002
	.section .rom.006138b9, "a"
	.incbin "baserom.gba", 0x006138b9, 0x00000003
	.section .rom.00613a29, "a"
	.incbin "baserom.gba", 0x00613a29, 0x00000003
	.section .rom.006162eb, "a"
	.incbin "baserom.gba", 0x006162eb, 0x00000001
	.section .rom.00618ab3, "a"
	.incbin "baserom.gba", 0x00618ab3, 0x00000001
	.section .rom.00619dad, "a"
	.incbin "baserom.gba", 0x00619dad, 0x00000003
	.section .rom.0061b6f3, "a"
	.incbin "baserom.gba", 0x0061b6f3, 0x00000001
	.section .rom.00626b86, "a"
	.incbin "baserom.gba", 0x00626b86, 0x00000002
	.section .rom.00626d46, "a"
	.incbin "baserom.gba", 0x00626d46, 0x00000002
	.section .rom.0062a0ad, "a"
	.incbin "baserom.gba", 0x0062a0ad, 0x00000003
	.section .rom.0062c59e, "a"
	.incbin "baserom.gba", 0x0062c59e, 0x00000002
	.section .rom.0062f37f, "a"
	.incbin "baserom.gba", 0x0062f37f, 0x00000001
	.section .rom.0062ff19, "a"
	.incbin "baserom.gba", 0x0062ff19, 0x00000003
	.section .rom.00630d7a, "a"
	.incbin "baserom.gba", 0x00630d7a, 0x00000002
	.section .rom.00630ed2, "a"
	.incbin "baserom.gba", 0x00630ed2, 0x00000002
	.section .rom.00638d1f, "a"
	.incbin "baserom.gba", 0x00638d1f, 0x00000001
	.section .rom.00638e8b, "a"
	.incbin "baserom.gba", 0x00638e8b, 0x00000001
	.section .rom.0063bc27, "a"
	.incbin "baserom.gba", 0x0063bc27, 0x00000001
	.section .rom.0063ddb6, "a"
	.incbin "baserom.gba", 0x0063ddb6, 0x00000002
	.section .rom.0063e4be, "a"
	.incbin "baserom.gba", 0x0063e4be, 0x00000002
	.section .rom.0063f0d2, "a"
	.incbin "baserom.gba", 0x0063f0d2, 0x00000002
	.section .rom.006501a5, "a"
	.incbin "baserom.gba", 0x006501a5, 0x00000003
	.section .rom.0065033a, "a"
	.incbin "baserom.gba", 0x0065033a, 0x00000002
	.section .rom.0065184a, "a"
	.incbin "baserom.gba", 0x0065184a, 0x00000002
	.section .rom.0065362f, "a"
	.incbin "baserom.gba", 0x0065362f, 0x00000001
	.section .rom.006546c2, "a"
	.incbin "baserom.gba", 0x006546c2, 0x00000002
	.section .rom.00656932, "a"
	.incbin "baserom.gba", 0x00656932, 0x00000002
	.section .rom.00656ac9, "a"
	.incbin "baserom.gba", 0x00656ac9, 0x00000003
	.section .rom.00658a6f, "a"
	.incbin "baserom.gba", 0x00658a6f, 0x00000001
	.section .rom.0065d846, "a"
	.incbin "baserom.gba", 0x0065d846, 0x00000002
	.section .rom.006612a7, "a"
	.incbin "baserom.gba", 0x006612a7, 0x00000001
	.section .rom.00663fdd, "a"
	.incbin "baserom.gba", 0x00663fdd, 0x00000003
	.section .rom.00666325, "a"
	.incbin "baserom.gba", 0x00666325, 0x00000003
	.section .rom.00667b03, "a"
	.incbin "baserom.gba", 0x00667b03, 0x00000001
	.section .rom.00667cb5, "a"
	.incbin "baserom.gba", 0x00667cb5, 0x00000003
	.section .rom.00678656, "a"
	.incbin "baserom.gba", 0x00678656, 0x00000002
	.section .rom.006787b5, "a"
	.incbin "baserom.gba", 0x006787b5, 0x00000003
	.section .rom.0067bf0f, "a"
	.incbin "baserom.gba", 0x0067bf0f, 0x00000001
	.section .rom.0067c072, "a"
	.incbin "baserom.gba", 0x0067c072, 0x00000002
	.section .rom.00681d55, "a"
	.incbin "baserom.gba", 0x00681d55, 0x00000003
	.section .rom.0068473b, "a"
	.incbin "baserom.gba", 0x0068473b, 0x00000001
	.section .rom.00686946, "a"
	.incbin "baserom.gba", 0x00686946, 0x00000002
	.section .rom.00686a87, "a"
	.incbin "baserom.gba", 0x00686a87, 0x00000001
	.section .rom.0068849e, "a"
	.incbin "baserom.gba", 0x0068849e, 0x00000002
	.section .rom.0068f205, "a"
	.incbin "baserom.gba", 0x0068f205, 0x00000003
	.section .rom.0068f383, "a"
	.incbin "baserom.gba", 0x0068f383, 0x00000001
	.section .rom.0069132d, "a"
	.incbin "baserom.gba", 0x0069132d, 0x00000003
	.section .rom.00693aa6, "a"
	.incbin "baserom.gba", 0x00693aa6, 0x00000002
	.section .rom.0069530d, "a"
	.incbin "baserom.gba", 0x0069530d, 0x00000003
	.section .rom.00698d2e, "a"
	.incbin "baserom.gba", 0x00698d2e, 0x00000002
	.section .rom.00698ef7, "a"
	.incbin "baserom.gba", 0x00698ef7, 0x00000001
	.section .rom.006a3edf, "a"
	.incbin "baserom.gba", 0x006a3edf, 0x00000001
	.section .rom.006a602b, "a"
	.incbin "baserom.gba", 0x006a602b, 0x00000001
	.section .rom.006a61b5, "a"
	.incbin "baserom.gba", 0x006a61b5, 0x00000003
	.section .rom.006a7c3a, "a"
	.incbin "baserom.gba", 0x006a7c3a, 0x00000002
	.section .rom.006aac71, "a"
	.incbin "baserom.gba", 0x006aac71, 0x00000003
	.section .rom.006aae37, "a"
	.incbin "baserom.gba", 0x006aae37, 0x00000001
	.section .rom.006abd6b, "a"
	.incbin "baserom.gba", 0x006abd6b, 0x00000001
	.section .rom.006ade1e, "a"
	.incbin "baserom.gba", 0x006ade1e, 0x00000002
	.section .rom.006adfc5, "a"
	.incbin "baserom.gba", 0x006adfc5, 0x00000003
	.section .rom.006b2fef, "a"
	.incbin "baserom.gba", 0x006b2fef, 0x00000001
	.section .rom.006b5d7f, "a"
	.incbin "baserom.gba", 0x006b5d7f, 0x00000001
	.section .rom.006b5f3f, "a"
	.incbin "baserom.gba", 0x006b5f3f, 0x00000001
	.section .rom.006b8433, "a"
	.incbin "baserom.gba", 0x006b8433, 0x00000001
	.section .rom.006b8605, "a"
	.incbin "baserom.gba", 0x006b8605, 0x00000003
	.section .rom.006ba696, "a"
	.incbin "baserom.gba", 0x006ba696, 0x00000002
	.section .rom.006bc796, "a"
	.incbin "baserom.gba", 0x006bc796, 0x00000002
	.section .rom.006bf34a, "a"
	.incbin "baserom.gba", 0x006bf34a, 0x00000002
	.section .rom.006bfe37, "a"
	.incbin "baserom.gba", 0x006bfe37, 0x00000001
	.section .rom.006c000a, "a"
	.incbin "baserom.gba", 0x006c000a, 0x00000002
	.section .rom.006c2939, "a"
	.incbin "baserom.gba", 0x006c2939, 0x00000003
	.section .rom.006c40d1, "a"
	.incbin "baserom.gba", 0x006c40d1, 0x00000003
	.section .rom.006c5345, "a"
	.incbin "baserom.gba", 0x006c5345, 0x00000003
	.section .rom.006c77d5, "a"
	.incbin "baserom.gba", 0x006c77d5, 0x00000003
	.section .rom.006c9b21, "a"
	.incbin "baserom.gba", 0x006c9b21, 0x00000003
	.section .rom.006cad0b, "a"
	.incbin "baserom.gba", 0x006cad0b, 0x00000001
	.section .rom.006cae92, "a"
	.incbin "baserom.gba", 0x006cae92, 0x00000002
	.section .rom.006cf3de, "a"
	.incbin "baserom.gba", 0x006cf3de, 0x00000002
	.section .rom.006cf533, "a"
	.incbin "baserom.gba", 0x006cf533, 0x00000001
	.section .rom.006cf673, "a"
	.incbin "baserom.gba", 0x006cf673, 0x00000001
	.section .rom.006d0347, "a"
	.incbin "baserom.gba", 0x006d0347, 0x00000001
	.section .rom.006d04c6, "a"
	.incbin "baserom.gba", 0x006d04c6, 0x00000002
	.section .rom.006d1d7f, "a"
	.incbin "baserom.gba", 0x006d1d7f, 0x00000001
	.section .rom.006d1f01, "a"
	.incbin "baserom.gba", 0x006d1f01, 0x00000003
	.section .rom.006d3993, "a"
	.incbin "baserom.gba", 0x006d3993, 0x00000001
	.section .rom.006d3b0d, "a"
	.incbin "baserom.gba", 0x006d3b0d, 0x00000003
	.section .rom.006d584b, "a"
	.incbin "baserom.gba", 0x006d584b, 0x00000001
	.section .rom.006d5977, "a"
	.incbin "baserom.gba", 0x006d5977, 0x00000001
	.section .rom.006d5aed, "a"
	.incbin "baserom.gba", 0x006d5aed, 0x00000003
	.section .rom.006d7993, "a"
	.incbin "baserom.gba", 0x006d7993, 0x00000001
	.section .rom.006d7b19, "a"
	.incbin "baserom.gba", 0x006d7b19, 0x00000003
	.section .rom.006d8a77, "a"
	.incbin "baserom.gba", 0x006d8a77, 0x00000001
	.section .rom.006d9d1b, "a"
	.incbin "baserom.gba", 0x006d9d1b, 0x00000001
	.section .rom.006d9ed6, "a"
	.incbin "baserom.gba", 0x006d9ed6, 0x00000002
	.section .rom.006dc97a, "a"
	.incbin "baserom.gba", 0x006dc97a, 0x00000002
	.section .rom.006dcaed, "a"
	.incbin "baserom.gba", 0x006dcaed, 0x00000003
	.section .rom.006dded5, "a"
	.incbin "baserom.gba", 0x006dded5, 0x00000003
	.section .rom.006e09b5, "a"
	.incbin "baserom.gba", 0x006e09b5, 0x00000003
	.section .rom.006e0b5f, "a"
	.incbin "baserom.gba", 0x006e0b5f, 0x00000001
	.section .rom.006e2f5a, "a"
	.incbin "baserom.gba", 0x006e2f5a, 0x00000002
	.section .rom.006e30e9, "a"
	.incbin "baserom.gba", 0x006e30e9, 0x00000003
	.section .rom.006e6369, "a"
	.incbin "baserom.gba", 0x006e6369, 0x00000003
	.section .rom.006e654a, "a"
	.incbin "baserom.gba", 0x006e654a, 0x00000002
	.section .rom.006e8adf, "a"
	.incbin "baserom.gba", 0x006e8adf, 0x00000001
	.section .rom.006e8c6e, "a"
	.incbin "baserom.gba", 0x006e8c6e, 0x00000002
	.section .rom.006eb866, "a"
	.incbin "baserom.gba", 0x006eb866, 0x00000002
	.section .rom.006ec3b3, "a"
	.incbin "baserom.gba", 0x006ec3b3, 0x00000001
	.section .rom.006ec51a, "a"
	.incbin "baserom.gba", 0x006ec51a, 0x00000002
	.section .rom.006eef2f, "a"
	.incbin "baserom.gba", 0x006eef2f, 0x00000001
	.section .rom.006f0b6e, "a"
	.incbin "baserom.gba", 0x006f0b6e, 0x00000002
	.section .rom.006f102b, "a"
	.incbin "baserom.gba", 0x006f102b, 0x00000001
	.section .rom.006f23a3, "a"
	.incbin "baserom.gba", 0x006f23a3, 0x00000001
	.section .rom.006f38e7, "a"
	.incbin "baserom.gba", 0x006f38e7, 0x00000001
	.section .rom.006f3a71, "a"
	.incbin "baserom.gba", 0x006f3a71, 0x00000003
	.section .rom.006f4b4f, "a"
	.incbin "baserom.gba", 0x006f4b4f, 0x00000001
	.section .rom.006f4cdd, "a"
	.incbin "baserom.gba", 0x006f4cdd, 0x00000003
	.section .rom.006f6931, "a"
	.incbin "baserom.gba", 0x006f6931, 0x00000003
	.section .rom.006f9313, "a"
	.incbin "baserom.gba", 0x006f9313, 0x00000001
	.section .rom.006fc83d, "a"
	.incbin "baserom.gba", 0x006fc83d, 0x00000003
	.section .rom.006fe147, "a"
	.incbin "baserom.gba", 0x006fe147, 0x00000001
	.section .rom.007001ef, "a"
	.incbin "baserom.gba", 0x007001ef, 0x00000001
	.section .rom.00702c43, "a"
	.incbin "baserom.gba", 0x00702c43, 0x00000001
	.section .rom.0070488f, "a"
	.incbin "baserom.gba", 0x0070488f, 0x00000001
	.section .rom.00705cd5, "a"
	.incbin "baserom.gba", 0x00705cd5, 0x00000003
	.section .rom.00707b55, "a"
	.incbin "baserom.gba", 0x00707b55, 0x00000003
	.section .rom.00709726, "a"
	.incbin "baserom.gba", 0x00709726, 0x00000002
	.section .rom.0070be5a, "a"
	.incbin "baserom.gba", 0x0070be5a, 0x00000002
	.section .rom.0070da9f, "a"
	.incbin "baserom.gba", 0x0070da9f, 0x00000001
	.section .rom.0070eee5, "a"
	.incbin "baserom.gba", 0x0070eee5, 0x00000003
	.section .rom.00710a63, "a"
	.incbin "baserom.gba", 0x00710a63, 0x00000001
	.section .rom.00711c93, "a"
	.incbin "baserom.gba", 0x00711c93, 0x00000001
	.section .rom.00711ded, "a"
	.incbin "baserom.gba", 0x00711ded, 0x00000003
	.section .rom.007152f3, "a"
	.incbin "baserom.gba", 0x007152f3, 0x00000001
	.section .rom.0071545e, "a"
	.incbin "baserom.gba", 0x0071545e, 0x00000002
	.section .rom.00717dae, "a"
	.incbin "baserom.gba", 0x00717dae, 0x00001166
	.section .rom.0071a6f9, "a"
	.incbin "baserom.gba", 0x0071a6f9, 0x00000003
	.section .rom.0071d93f, "a"
	.incbin "baserom.gba", 0x0071d93f, 0x00000001
	.section .rom.00720236, "a"
	.incbin "baserom.gba", 0x00720236, 0x00000002
	.section .rom.0072956f, "a"
	.incbin "baserom.gba", 0x0072956f, 0x00000001
	.section .rom.0072a89e, "a"
	.incbin "baserom.gba", 0x0072a89e, 0x00000002
	.section .rom.0072ba97, "a"
	.incbin "baserom.gba", 0x0072ba97, 0x00000001
	.section .rom.0072bbf9, "a"
	.incbin "baserom.gba", 0x0072bbf9, 0x00000003
	.section .rom.0072e5c5, "a"
	.incbin "baserom.gba", 0x0072e5c5, 0x00000003
	.section .rom.00731af1, "a"
	.incbin "baserom.gba", 0x00731af1, 0x00000003
	.section .rom.00735f2f, "a"
	.incbin "baserom.gba", 0x00735f2f, 0x00000001
	.section .rom.007360a2, "a"
	.incbin "baserom.gba", 0x007360a2, 0x00000002
	.section .rom.0073a307, "a"
	.incbin "baserom.gba", 0x0073a307, 0x00000001
	.section .rom.0073db22, "a"
	.incbin "baserom.gba", 0x0073db22, 0x00000002
	.section .rom.0073f54a, "a"
	.incbin "baserom.gba", 0x0073f54a, 0x00000002
	.section .rom.00741bf7, "a"
	.incbin "baserom.gba", 0x00741bf7, 0x00000001
	.section .rom.00743c5b, "a"
	.incbin "baserom.gba", 0x00743c5b, 0x00000001
	.section .rom.00745b17, "a"
	.incbin "baserom.gba", 0x00745b17, 0x00000001
	.section .rom.00748d61, "a"
	.incbin "baserom.gba", 0x00748d61, 0x00000003
	.section .rom.0074a723, "a"
	.incbin "baserom.gba", 0x0074a723, 0x00000001
	.section .rom.0074a895, "a"
	.incbin "baserom.gba", 0x0074a895, 0x00000003
	.section .rom.0074d8a2, "a"
	.incbin "baserom.gba", 0x0074d8a2, 0x00000002
	.section .rom.0074da5a, "a"
	.incbin "baserom.gba", 0x0074da5a, 0x00000002
	.section .rom.0074f2cb, "a"
	.incbin "baserom.gba", 0x0074f2cb, 0x00000001
	.section .rom.0074f44a, "a"
	.incbin "baserom.gba", 0x0074f44a, 0x00000002
	.section .rom.00752c53, "a"
	.incbin "baserom.gba", 0x00752c53, 0x00000001
	.section .rom.00752dc7, "a"
	.incbin "baserom.gba", 0x00752dc7, 0x00000001
	.section .rom.00753b8f, "a"
	.incbin "baserom.gba", 0x00753b8f, 0x00000001
	.section .rom.00753d35, "a"
	.incbin "baserom.gba", 0x00753d35, 0x00000003
	.section .rom.00755d90, "a"
	.incbin "baserom.gba", 0x00755d90, 0x000000e4
	.section .rom.00758b8b, "a"
	.incbin "baserom.gba", 0x00758b8b, 0x00000001
	.section .rom.00758d72, "a"
	.incbin "baserom.gba", 0x00758d72, 0x00000002
	.section .rom.0075c00e, "a"
	.incbin "baserom.gba", 0x0075c00e, 0x00004606
	.section .rom.0076071a, "a"
	.incbin "baserom.gba", 0x0076071a, 0x00000002
	.section .rom.007622ed, "a"
	.incbin "baserom.gba", 0x007622ed, 0x00000003
	.section .rom.00763d9d, "a"
	.incbin "baserom.gba", 0x00763d9d, 0x00000003
	.section .rom.00763ef9, "a"
	.incbin "baserom.gba", 0x00763ef9, 0x00000003
	.section .rom.0076a13f, "a"
	.incbin "baserom.gba", 0x0076a13f, 0x00000001
	.section .rom.0076a243, "a"
	.incbin "baserom.gba", 0x0076a243, 0x00000001
	.section .rom.0076ac5f, "a"
	.incbin "baserom.gba", 0x0076ac5f, 0x00000001
	.section .rom.0076ad45, "a"
	.incbin "baserom.gba", 0x0076ad45, 0x00000003
	.section .rom.0076afe1, "a"
	.incbin "baserom.gba", 0x0076afe1, 0x00000003
	.section .rom.0076f03e, "a"
	.incbin "baserom.gba", 0x0076f03e, 0x00000002
	.section .rom.0076f5f2, "a"
	.incbin "baserom.gba", 0x0076f5f2, 0x00000002
	.section .rom.00770587, "a"
	.incbin "baserom.gba", 0x00770587, 0x00000001
	.section .rom.00771f8a, "a"
	.incbin "baserom.gba", 0x00771f8a, 0x00000002
	.section .rom.00772bf9, "a"
	.incbin "baserom.gba", 0x00772bf9, 0x00000003
	.section .rom.00772ea1, "a"
	.incbin "baserom.gba", 0x00772ea1, 0x00000003
	.section .rom.00773d26, "a"
	.incbin "baserom.gba", 0x00773d26, 0x00000002
	.section .rom.00774eb7, "a"
	.incbin "baserom.gba", 0x00774eb7, 0x00000001
	.section .rom.0077507f, "a"
	.incbin "baserom.gba", 0x0077507f, 0x00000001
	.section .rom.007753cb, "a"
	.incbin "baserom.gba", 0x007753cb, 0x0000051d
	.section .rom.00775c33, "a"
	.incbin "baserom.gba", 0x00775c33, 0x0000051d
	.section .rom.0077649b, "a"
	.incbin "baserom.gba", 0x0077649b, 0x0000051d
	.section .rom.00776d03, "a"
	.incbin "baserom.gba", 0x00776d03, 0x0000051d
	.section .rom.0077756b, "a"
	.incbin "baserom.gba", 0x0077756b, 0x0000051d
	.section .rom.00777dd3, "a"
	.incbin "baserom.gba", 0x00777dd3, 0x0000051d
	.section .rom.0077863b, "a"
	.incbin "baserom.gba", 0x0077863b, 0x0000051d
	.section .rom.00778ea3, "a"
	.incbin "baserom.gba", 0x00778ea3, 0x0008715d
