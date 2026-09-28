.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/ENTRY.INC"
	.section .text.x02008260,"ax",%progbits
	.balign 4
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/IMPORT.INC"
