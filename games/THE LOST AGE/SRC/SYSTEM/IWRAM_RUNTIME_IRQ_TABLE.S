@ The fourteen interrupt-handler words of TLA's resident runtime, one per
@ interrupt bit, which the dispatcher in IWRAM_RUNTIME.S loads with
@ ldrne ip, [pc, #...] (the Game Pak word with ldr ip, [pc, #140]). They
@ are zero in this image: after copying the runtime, start-up fills them by
@ DMA3 from the ROM table at 0x080178b4 (control 0x8400000e, 14 words).
@ Run-time variables carry no credit.
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x0800079c
	.set AlchemyRunAddress, 0x030001e4
	.4byte	0x00000000, 0x00000000, 0x00000000, 0, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	.4byte	0, 0, 0, 0, 0x00000000, 0x00000000
