.syntax unified
	.thumb
	.global Resource_ExecuteTransferCode
Resource_ExecuteTransferCode:
	.global Resource_ExecuteTransfer
	.thumb_func
Resource_ExecuteTransfer:
	bx	pc
AlchemyUncredited_08013062:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08013062:
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
	.global Resource_ExecuteTransferSize
	.set Resource_ExecuteTransferSize, . - Resource_ExecuteTransferCode
