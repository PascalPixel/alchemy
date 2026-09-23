@ 16.16 fixed-point multiply of the resident runtime: smull, then the middle
@ 32 bits of the 64-bit product in r0. These 16 bytes equal TLA
@ SYSTEM/IWRAM_RUNTIME_MUL_Q16.S (0x080007d8) byte for byte.
@ credit: library — TBS/TLA common IWRAM runtime 16.16 multiply
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x0800089c
	.set AlchemyRunAddress, 0x0300012c

	.global IwramMulQ16
IwramMulQ16:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	bx	lr
