.syntax unified
	.thumb
	.section .rodata.x020080c4,"a",%progbits
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x00000139
	.4byte 0x0000013a
	.4byte 0x0000013b
	.4byte 0x0000013c
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000054
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000068
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_0200007c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02000090
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02000200
	.4byte 0x03600400
	.4byte 0xffffffff
	.4byte 0xffffffff
