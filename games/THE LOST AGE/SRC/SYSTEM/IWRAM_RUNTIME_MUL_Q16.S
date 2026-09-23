@ 16.16 fixed-point multiply of TLA's resident runtime, behind a 4-byte
@ Thumb entry (bx pc): smull, then the middle 32 bits of the 64-bit
@ product in r0. After the entry these 16 bytes equal TBS
@ SYSTEM/RUNTIME_MUL_Q16.S (0x0800089c) byte for byte. The entry is TLA's
@ own and carries no credit.
@ credit: library — TBS/TLA common IWRAM runtime 16.16 multiply
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x080007d4
	.set AlchemyRunAddress, 0x0300021c
AlchemyUncredited_080007d4:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080007d4:
	smull	r2, r0, r1, r0
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	bx	lr
