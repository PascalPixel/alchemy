@ Transfer command executor run from the stack. Its loader (0x08013d84)
@ reserves (0x6b + 4) & ~3 = 108 bytes of stack, has DMA3 copy exactly these
@ 108 bytes there and calls the copy. Behind a 4-byte Thumb entry (bx pc)
@ the ARM body picks a byte, halfword or word access with three conditional
@ loads (ldrbcc, ldrheq, ldrgt) after one compare and saves r5 alone with
@ stmfd sp!, {r5}; GCC saves lr with any register. After that entry its
@ 104-byte body equals TBS SYSTEM/RESOURCE/EXECUTE_TRANSFER.S (0x08002cf4)
@ byte for byte.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank, transfer command executor
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08013060
	.global Resource_ExecuteTransfer
	.thumb_func
Resource_ExecuteTransfer:
AlchemyUncredited_08013060:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08013060:
	stmfd	sp!, {r5}
	add	r0, r0, #4
.L_0801306c:
	ldm	r0!, {r2, r3, r4}
	ands	ip, r4, #0x30000
	beq	.L_080130bc
	tst	r4, #0xc0000
	beq	.L_0801309c
	cmp	ip, #0x20000
	ldrbcc	r5, [r3]
	ldrheq	r5, [r3]
	ldrgt	r5, [r3]
	tst	r4, #0x40000
	orrne	r2, r5, r2
	biceq	r2, r5, r2
.L_0801309c:
	cmp	ip, #0x20000
	strbcc	r2, [r3]
	strheq	r2, [r3]
	strgt	r2, [r3]
.L_080130ac:
	subs	r1, r1, #1
	bgt	.L_0801306c
	ldmfd	sp!, {r5}
	bx	lr
.L_080130bc:
	mov	ip, #0x4000000
	add	ip, ip, #212
	stm	ip, {r2, r3, r4}
	b	.L_080130ac
