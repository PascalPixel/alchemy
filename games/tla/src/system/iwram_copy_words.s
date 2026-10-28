.syntax unified
	.arm
	.thumb
	.global IwramCopyWords
IwramCopyWords:
	bx	pc
AlchemyUncredited_08000cea:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08000cea:
	push	{r5, r6, r7, r8, r9}
	ands	ip, r2, #224
	rsb	ip, ip, #240
	add	pc, pc, ip, lsr #2
.L_03000744:
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	subs	r2, r2, #0x100
	bpl	.L_03000744
	ands	r2, r2, #28
	beq	.L_030007a4
.L_03000794:
	ldm	r1!, {r3}
	stmia	r0!, {r3}
	subs	r2, r2, #4
	bgt	.L_03000794
.L_030007a4:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
