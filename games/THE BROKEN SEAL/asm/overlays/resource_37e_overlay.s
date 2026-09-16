.syntax unified
.include "games/THE BROKEN SEAL/SRC/COMMON/OVERLAY.INC"
	.thumb
	.set sub_02000118, 0x02000118
	.set sub_0200012a, 0x0200012a
	.set sub_02000132, 0x02000132
	.set sub_0200013e, 0x0200013e
	.global Overlay_02000000
Overlay_02000000:
.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_ROKA/ENTRY.INC"
AlchemyC_02000030:
	.space 0x8
AlchemyC_02000038:
	.space 0x4
AlchemyC_0200003c:
	.space 0x8
AlchemyC_02000044:
	.space 0x8
AlchemyC_0200004c:
	.space 0x8
AlchemyC_02000054:
	.space 0x4c
.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_ROKA/IMPORT.INC"
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000002d7
	.4byte 0xc000005c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000038
	.4byte 0x40000268
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00105010
	.4byte 0x00201010
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
