	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
.syntax unified
	.section .data,"aw",%progbits
	.arm

IwramIrqVBlank:
	.global gIrqHandlers
gIrqHandlers:
	.4byte 0
IwramIrqHBlank:
	.4byte 0
IwramIrqVCount:
	.4byte 0
IwramIrqTimer0:
	.4byte 0
IwramIrqTimer1:
	.4byte 0
IwramIrqTimer2:
	.4byte 0
IwramIrqTimer3:
	.4byte 0
IwramIrqSerial:
	.4byte 0
IwramIrqDma0:
	.4byte 0
IwramIrqDma1:
	.4byte 0
	.ifndef TBS_EDITION_EN
	.if EDITION_INTERNATIONAL
	.global IwramHalt
IwramHalt:
	.else
IwramIrqDma2:
	.endif
	.else
IwramIrqDma2:
	.endif
	.4byte 0
IwramIrqDma3:
	.4byte 0
IwramIrqKeypad:
	.4byte 0
IwramIrqGamePak:
	.4byte 0
