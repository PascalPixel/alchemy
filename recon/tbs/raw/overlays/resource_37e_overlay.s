.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/ENTRY.INC"
	.section .text.x020080a0,"ax",%progbits
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_MEIRO/IMPORT.INC"
	.section .rodata,"a",%progbits
	.global SoruRoka_SceneTable0
SoruRoka_SceneTable0:
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
	.global SoruRoka_SceneTable1
SoruRoka_SceneTable1:
	.4byte 0x0000000f
	.4byte 0x00105010
	.4byte 0x00201010
	.4byte 0x000001ff
	.global SoruRoka_SceneTable2
SoruRoka_SceneTable2:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global SoruRoka_SceneTable3
SoruRoka_SceneTable3:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
