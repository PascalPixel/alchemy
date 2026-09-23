.syntax unified
	.thumb
	.set sub_08016f94, 0x08016f94
	.set sub_0801700c, 0x0801700c
	.set sub_0801787c, 0x0801787c
	.global Func_08017054
	.thumb_func
Func_08017054:
	adds	r2, r0, #0
	ldr	r1, [pc, #20]
	adds	r0, r2, #1
	str	r0, [r1, #0]
	ldr	r3, [pc, #16]
	movs	r0, #1
	eors	r3, r0
	ldr	r0, [pc, #16]
	ldr	r1, [pc, #8]
	subs	r0, r0, r1
	lsls	r0, r0, #15
	b.n	.L_08017084
	.4byte 0x0200741c
	.4byte 0x08017051
	.2byte 0x7055
	.2byte 0x0801
.L_08017078:
	ldrh	r0, [r3, #0]
	strh	r0, [r2, #0]
	adds	r3, #2
	adds	r2, #2
	subs	r0, r1, #1
	lsls	r0, r0, #16
.L_08017084:
	lsrs	r1, r0, #16
	cmp	r1, #0
	bne.n	.L_08017078
	bx	lr
	push	{r4, r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r4, r0, #0
	adds	r5, r1, #0
	lsls	r4, r4, #24
	lsrs	r4, r4, #24
	lsls	r2, r2, #24
	lsrs	r6, r2, #24
	movs	r0, #0
	mov	r8, r0
	adds	r0, r4, #0
	bl	sub_08016f94
	ldr	r7, [pc, #8]
	movs	r0, #192
	lsls	r0, r0, #8
	orrs	r4, r0
	lsls	r4, r4, #16
	b.n	.L_080170dc
	.2byte 0x741c
	.2byte 0x0200
.L_080170b8:
	ldr	r0, [pc, #20]
	ldr	r1, [r0, #0]
	ldr	r0, [pc, #20]
	ldrh	r1, [r1, #20]
	cmp	r1, r0
	bne.n	.L_080170ca
	ldr	r1, [pc, #16]
	movs	r0, #240
	strb	r0, [r1, #0]
.L_080170ca:
	lsrs	r4, r4, #16
	mov	r8, r4
	b.n	.L_08017104
	.4byte 0x02007408
	.4byte 0x00001cc2
	.2byte 0x5555
	.2byte 0x0e00
.L_080170dc:
	ldr	r1, [r7, #0]
	adds	r0, r5, #0
	bl	sub_0801787c
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	cmp	r0, r6
	beq.n	.L_08017104
	ldr	r0, [pc, #36]
	ldrb	r0, [r0, #0]
	cmp	r0, #0
	beq.n	.L_080170dc
	ldr	r1, [r7, #0]
	adds	r0, r5, #0
	bl	sub_0801787c
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	cmp	r0, r6
	bne.n	.L_080170b8
.L_08017104:
	bl	sub_0801700c
	mov	r0, r8
	pop	{r3}
	mov	r8, r3
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x7424
	.2byte 0x0200
	push	{r4, lr}
	adds	r4, r0, #0
	subs	r3, r2, #1
	cmp	r2, #0
	beq.n	.L_08017134
	movs	r2, #1
	negs	r2, r2
.L_08017126:
	ldrb	r0, [r4, #0]
	strb	r0, [r1, #0]
	adds	r4, #1
	adds	r1, #1
	subs	r3, #1
	cmp	r3, r2
	bne.n	.L_08017126
.L_08017134:
	pop	{r4}
	pop	{r0}
	bx	r0
	.align 2, 0
