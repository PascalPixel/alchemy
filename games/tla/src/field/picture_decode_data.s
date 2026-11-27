.syntax unified
	.section .data,"aw",%progbits
	.arm
	@ The table number last loaded, then the colour tables' address less
	@ 224 bytes (unused) and less 256 bytes: table n is the 256 bytes at
	@ that base + n * 256.
	.4byte	0x00000000
	.4byte	SpriteColorTables - 0xe0
	.4byte	SpriteColorTables - 0x100
	.global fdRenderDecodePictureCopyEnd
fdRenderDecodePictureCopyEnd:
	.4byte	0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0
	.global fdRenderDecodePictureEnd
fdRenderDecodePictureEnd:
