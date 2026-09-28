.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/ENTRY.INC"
	.section .text.x020080a0,"ax",%progbits
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_DOU/IMPORT.INC"
	.section .rodata,"a",%progbits
	.global SoruDou_SceneTable0
SoruDou_SceneTable0:
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
	.global SoruDou_SceneTable1
SoruDou_SceneTable1:
	.4byte 0x0000000d
	.4byte 0x0010200c
	.4byte 0x000001ff
	.global SoruDou_SceneTable2
SoruDou_SceneTable2:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global SoruDou_SceneTable3
SoruDou_SceneTable3:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03500064
	.4byte 0x00300000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
