.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/ENTRY.INC"
AlchemyC_02000030:
	.space 0x8
AlchemyC_02000038:
	.space 0x4
AlchemyC_0200003c:
	.space 0x8
AlchemyC_02000044:
	.space 0x28
AlchemyC_0200006c:
	.space 0x28
AlchemyC_02000094:
	.space 0x7c
AlchemyC_02000110:
	.space 0x30
AlchemyC_02000140:
	.space 0x30
AlchemyC_02000170:
	.space 0x20
AlchemyC_02000190:
	.space 0x20
AlchemyC_020001b0:
	.space 0x20
AlchemyC_020001d0:
	.space 0x12
	.balign 4, 0
AlchemyC_020001e4:
	.space 0x7c
.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/IMPORT.INC"
AlchemyData_020002d0:
	.space 0x200
