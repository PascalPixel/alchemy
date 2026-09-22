.syntax unified
	.thumb
	.set sub_08021918, 0x08021918
	.set sub_08022d1c, 0x08022d1c
	.global Overlay_08022a24
Overlay_08022a24:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	adds	r5, r1, #0
	cmp	r6, #0
	beq.n	.L_08022a82
	cmp	r5, #0
	beq.n	.L_08022a82
	adds	r0, r5, #0
	bl	sub_08022d1c
	ldr	r3, [r6, #40]
	movs	r0, #0
	cmp	r5, r3
	beq.n	.L_08022a52
	adds	r2, r6, #0
	adds	r2, #40
.L_08022a44:
	adds	r0, #1
	cmp	r0, #3
	bhi.n	.L_08022a52
	adds	r2, #4
	ldr	r3, [r2, #0]
	cmp	r5, r3
	bne.n	.L_08022a44
.L_08022a52:
	cmp	r0, #4
	beq.n	.L_08022a82
	lsls	r3, r0, #2
	movs	r2, #0
	adds	r3, #40
	str	r2, [r6, r3]
	adds	r2, r0, #1
	movs	r4, #0
	cmp	r2, #3
	bhi.n	.L_08022a7c
	lsls	r3, r2, #2
	adds	r3, r3, r6
	adds	r1, r3, #0
	adds	r1, #40
.L_08022a6e:
	ldmia	r1!, {r3}
	cmp	r3, #0
	beq.n	.L_08022a76
	adds	r4, #1
.L_08022a76:
	adds	r2, #1
	cmp	r2, #3
	bls.n	.L_08022a6e
.L_08022a7c:
	cmp	r4, #0
	bne.n	.L_08022a82
	strb	r0, [r6, #27]
.L_08022a82:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r1, #0
	cmp	r5, #0
	beq.n	.L_08022aca
	cmp	r7, #3
	bhi.n	.L_08022aca
	lsls	r3, r7, #2
	adds	r6, r3, #0
	adds	r6, #40
	ldr	r0, [r5, r6]
	cmp	r0, #0
	beq.n	.L_08022aca
	bl	sub_08022d1c
	movs	r3, #0
	adds	r2, r7, #1
	str	r3, [r5, r6]
	movs	r0, #0
	cmp	r2, #3
	bhi.n	.L_08022ac4
	lsls	r3, r2, #2
	adds	r3, r3, r5
	adds	r1, r3, #0
	adds	r1, #40
.L_08022ab6:
	ldmia	r1!, {r3}
	cmp	r3, #0
	beq.n	.L_08022abe
	adds	r0, #1
.L_08022abe:
	adds	r2, #1
	cmp	r2, #3
	bls.n	.L_08022ab6
.L_08022ac4:
	cmp	r0, #0
	bne.n	.L_08022aca
	strb	r7, [r5, #27]
.L_08022aca:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	ldr	r3, [r5, #12]
	adds	r6, r1, #0
	movs	r7, #128
	ands	r7, r6
	cmp	r3, #0
	beq.n	.L_08022b02
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	sub_08021918
	ldrb	r3, [r0, #5]
	cmp	r6, r3
	bge.n	.L_08022b02
	ldr	r2, [r5, #12]
	lsls	r3, r6, #2
	ldr	r2, [r3, r2]
	ldrb	r3, [r0, #4]
	str	r2, [r5, #16]
	strb	r3, [r5, #4]
	movs	r3, #16
	strb	r3, [r5, #21]
	cmp	r7, #0
	bne.n	.L_08022b02
	strb	r7, [r5, #20]
	strh	r7, [r5, #2]
.L_08022b02:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	movs	r3, #127
	adds	r6, r0, #0
	movs	r4, #128
	ands	r4, r7
	ands	r7, r3
	ldrb	r3, [r6, #24]
	sub	sp, #8
	cmp	r3, r7
	beq.n	.L_08022b78
	movs	r1, #0
	ldrb	r2, [r6, #27]
	b.n	.L_08022b72
.L_08022b20:
	lsls	r3, r1, #2
	adds	r3, #40
	ldr	r5, [r6, r3]
	cmp	r5, #0
	beq.n	.L_08022b70
	ldr	r3, [r5, #12]
	cmp	r3, #0
	beq.n	.L_08022b70
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_08021918
	ldrb	r3, [r0, #5]
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r7, r3
	bge.n	.L_08022b6e
	ldr	r2, [r5, #12]
	lsls	r3, r7, #2
	ldr	r2, [r3, r2]
	ldrb	r3, [r0, #4]
	str	r2, [r5, #16]
	strb	r3, [r5, #4]
	movs	r3, #16
	strb	r3, [r5, #21]
	cmp	r4, #0
	bne.n	.L_08022b5e
	strb	r4, [r5, #20]
	strh	r4, [r5, #2]
.L_08022b5e:
	cmp	r1, #0
	bne.n	.L_08022b6e
	ldrb	r3, [r0, #7]
	ldrb	r2, [r6, #27]
	strb	r3, [r6, #23]
	ldrb	r3, [r0, #6]
	strb	r3, [r6, #22]
	b.n	.L_08022b70
.L_08022b6e:
	ldrb	r2, [r6, #27]
.L_08022b70:
	adds	r1, #1
.L_08022b72:
	cmp	r1, r2
	blt.n	.L_08022b20
	strb	r7, [r6, #24]
.L_08022b78:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, r7, pc}
