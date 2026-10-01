	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ The fourteen interrupt-handler words of the resident runtime, which the
@ dispatcher in RUNTIME.S loads with ldrne ip, [pc, #...]. They are zero in
@ this image: after copying the runtime, the start-up routine at 0x0800300c
@ fills them by DMA3 from the ROM table at 0x08007320 (control 0x8400000e,
@ 14 words). Run-time variables carry no credit.
.syntax unified
	.section .data,"aw",%progbits
	.arm

@ 起動後に常駐側から割込み先十四本を転送する表。
IwramIrqVBlank:
	.global Data_030000e0
Data_030000e0:
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
	@ The European editions' ROM table carries a halt loop (svc 0x30000,
	@ then a branch back to it) in the DMA2 and DMA3 words, and start-up
	@ installs this copy as the Game Pak interrupt handler.
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
