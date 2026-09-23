.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0801314c, 0x0801314c
	.set sub_08013560, 0x08013560
	.set sub_08191abc, 0x08191abc
	.set sub_081963ec, 0x081963ec
	.global Func_08191958
	.thumb_func
Func_08191958:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #140
	str	r2, [sp, #8]
	movs	r2, #128
	lsls	r0, r0, #24
	lsls	r2, r2, #17
	ands	r2, r0
	lsrs	r2, r2, #24
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #6
	subs	r3, #64
	lsls	r1, r1, #24
	mov	r8, r3
	movs	r3, #192
	lsrs	r1, r1, #24
	lsls	r3, r3, #18
	mov	r9, r1
	ldr	r1, [r3, #96]
	ldr	r3, [r3, #92]
	mov	sl, r1
	movs	r0, #104
	movs	r1, #3
	str	r3, [sp, #4]
	bl	sub_081963ec
	ldr	r3, [sp, #4]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r3, r1
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [sp, #4]
	adds	r1, #4
	adds	r2, r3, r1
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	movs	r7, #192
	movs	r2, #7
	movs	r3, #63
	movs	r5, #0
	mov	ip, r2
	negs	r7, r7
	mov	lr, r3
.L_081919c6:
	asrs	r2, r5, #3
	adds	r3, r5, #0
	mov	r1, ip
	ands	r3, r1
	lsls	r2, r2, #6
	movs	r6, #0
	adds	r4, r7, #0
	adds	r0, r3, r2
	b.n	.L_081919dc
	.2byte 0x0048
	.2byte 0x0000
.L_081919dc:
	lsls	r1, r5, #1
	cmp	r5, #31
	ble.n	.L_081919ec
	mov	r2, lr
	subs	r1, r2, r4
	cmp	r5, #95
	bgt.n	.L_081919ec
	movs	r1, #63
.L_081919ec:
	adds	r3, r6, #0
	mov	r2, ip
	ands	r3, r2
	lsls	r3, r3, #3
	asrs	r2, r6, #3
	lsls	r2, r2, #10
	adds	r3, r0, r3
	adds	r3, r3, r2
	adds	r6, #1
	mov	r2, sl
	strb	r1, [r2, r3]
	cmp	r6, #128
	bne.n	.L_081919dc
	adds	r5, #1
	adds	r7, #2
	cmp	r5, #128
	bne.n	.L_081919c6
	add	r6, sp, #12
	movs	r5, #0
	adds	r0, r6, #0
.L_08191a14:
	asrs	r3, r5, #1
	lsls	r2, r3, #10
	lsls	r1, r3, #5
	orrs	r2, r1
	orrs	r2, r3
	adds	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, #63
	bne.n	.L_08191a14
	ldr	r1, [pc, #172]
	ldr	r0, [pc, #176]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08191a58
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r1, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	adds	r2, #2
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #24
	adds	r2, #63
	str	r2, [r3, #0]
.L_08191a58:
	strh	r4, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #40
	mov	sl, r3
	mov	r1, r8
	lsls	r3, r1, #8
	mov	r2, sl
	str	r3, [r2, #0]
	mov	r3, r9
	lsls	r7, r3, #24
	asrs	r1, r7, #24
	cmp	r1, #0
	bge.n	.L_08191a76
	negs	r1, r1
.L_08191a76:
	movs	r0, #192
	bl	sub_08002054
	adds	r6, r0, #1
	asrs	r0, r0, #1
	mov	r9, r0
	movs	r5, #0
	cmp	r6, #0
	beq.n	.L_08191ac2
	ldr	r1, [sp, #4]
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	adds	r1, r1, r2
	mov	fp, sl
	asrs	r7, r7, #24
	mov	sl, r1
.L_08191a98:
	add	r8, r7
	mov	r1, r8
	lsls	r3, r1, #8
	mov	r2, fp
	str	r3, [r2, #0]
	mov	r1, sl
	movs	r3, #1
	str	r3, [r1, #0]
	movs	r0, #1
	bl	sub_08013560
	cmp	r5, r9
	bne.n	sub_08191abc
	ldr	r2, [sp, #8]
	cmp	r2, #0
	beq.n	sub_08191abc
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x3501
	cmp	r5, r6
	bne.n	.L_08191a98
.L_08191ac2:
	movs	r0, #104
	bl	sub_0801314c
	add	sp, #140
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldrb	r4, [r5, #27]
	movs	r0, #0
	adds	r6, r1, #0
	cmp	r0, r4
	bge.n	.L_08191b08
	adds	r1, r5, #0
	adds	r1, #40
.L_08191af2:
	ldmia	r1!, {r2}
	cmp	r2, #0
	beq.n	.L_08191b02
	ldr	r3, [r2, #16]
	cmp	r3, #0
	beq.n	.L_08191b02
	strb	r6, [r2, #5]
	ldrb	r4, [r5, #27]
.L_08191b02:
	adds	r0, #1
	cmp	r0, r4
	blt.n	.L_08191af2
.L_08191b08:
	movs	r3, #1
	strb	r3, [r5, #25]
	pop	{r5, r6, pc}
	.2byte 0x0000
