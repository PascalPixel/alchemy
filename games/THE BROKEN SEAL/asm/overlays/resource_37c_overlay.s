.syntax unified
.include "games/THE BROKEN SEAL/SRC/COMMON/OVERLAY.INC"
	.thumb
	.set sub_02000118, 0x02000118
	.set sub_0200011a, 0x0200011a
	.set sub_0200012a, 0x0200012a
	.set sub_0200013a, 0x0200013a
	.set sub_02000146, 0x02000146
	.global Overlay_02000000
Overlay_02000000:
.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_DOU/ENTRY.INC"
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
.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_DOU/IMPORT.INC"
	.4byte 0xffff0000
	.4byte 0x00000057
	.4byte 0xc000008e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000057
	.4byte 0xc000008e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000d
	.4byte 0x0010200c
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
	.4byte 0x00000003
	.4byte 0x03500064
	.4byte 0x00300000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
