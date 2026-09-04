@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x0800f9f4 (152 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_0800f9f4
	.thumb_func
Func_0800f9f4:
	push	{r5, r6, r7, lr}
	subs	r3, r0, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r4, r3, #1
	movs	r3, #1
	ands	r3, r0
	ldr	r6, [pc, #124]	@ (0x800fa80)
	ldr	r5, [pc, #124]	@ (0x800fa84)
	cmp	r3, #0
	beq.n	.L_0800fa7a
	ldr	r3, [pc, #124]	@ (0x800fa88)
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L_0800fa38
	cmp	r3, #1
	bgt.n	.L_0800fa1c
	cmp	r3, #0
	beq.n	.L_0800fa22
	b.n	.L_0800fa7a
.L_0800fa1c:
	cmp	r3, #2
	beq.n	.L_0800fa60
	b.n	.L_0800fa7a
.L_0800fa22:
	movs	r1, #0
	cmp	r1, r4
	bge.n	.L_0800fa7a
.L_0800fa28:
	ldrh	r3, [r6, #0]
	adds	r1, #1
	strh	r3, [r5, #0]
	adds	r6, #2
	adds	r5, #2
	cmp	r1, r4
	blt.n	.L_0800fa28
	b.n	.L_0800fa7a
.L_0800fa38:
	ldr	r6, [pc, #68]	@ (0x800fa80)
	movs	r1, #0
	movs	r7, #0
	adds	r0, r4, r6
	cmp	r1, r4
	bge.n	.L_0800fa7a
.L_0800fa44:
	ldrb	r3, [r6, #0]
	ldrb	r2, [r0, #0]
	lsls	r3, r3, #8
	orrs	r3, r2
	eors	r3, r7
	adds	r1, #1
	strh	r3, [r5, #0]
	adds	r0, #1
	adds	r6, #1
	adds	r5, #2
	adds	r7, r3, #0
	cmp	r1, r4
	blt.n	.L_0800fa44
	b.n	.L_0800fa7a
.L_0800fa60:
	movs	r2, #0
	cmp	r4, #0
	ble.n	.L_0800fa7a
	adds	r1, r4, #0
.L_0800fa68:
	ldrh	r3, [r6, #0]
	subs	r1, #1
	eors	r3, r2
	strh	r3, [r5, #0]
	adds	r6, #2
	adds	r5, #2
	adds	r2, r3, #0
	cmp	r1, #0
	bne.n	.L_0800fa68
.L_0800fa7a:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02010002
	.4byte 0x02020000
	.4byte 0x02010001
