.syntax unified
	.thumb
	.set sub_080c8978, 0x080c8978
	.set sub_080c89a6, 0x080c89a6
	.set sub_080dc390, 0x080dc390
	.set sub_080eb01c, 0x080eb01c
	.global Func_080eb298
	.thumb_func
Func_080eb298:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	sub	sp, #12
	cmp	r6, #0
	beq.n	.L_080eb2c4
	ldr	r3, [r1, #0]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r1, #4]
	str	r3, [r5, #4]
	ldr	r3, [r1, #8]
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r3, [r5, #0]
	adds	r0, r6, #0
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	bl	sub_080eb01c
.L_080eb2c4:
	add	sp, #12
	pop	{r5, r6, pc}
	push	{lr}
	bl	sub_080c89a6
	pop	{pc}
	push	{lr}
	bl	sub_080c8978
	pop	{pc}
.L_080eb2d8:
	movs	r4, #192
	lsls	r4, r4, #18
	adds	r3, r4, #0
	adds	r3, #180
	ldr	r3, [r3, #0]
	lsls	r1, r1, #6
	adds	r1, r1, r0
	lsls	r1, r1, #1
	ldr	r4, [r4, #96]
	ldrh	r3, [r1, r3]
	strb	r2, [r4, r3]
	bx	lr
	push	{r5, lr}
	adds	r5, r2, #0
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #180
	ldr	r4, [r3, #0]
	ldr	r2, [r2, #96]
	cmp	r0, #63
	ble.n	.L_080eb30c
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r2, r2, r3
	subs	r0, #64
.L_080eb30c:
	lsls	r3, r1, #6
	adds	r3, r3, r0
	lsls	r3, r3, #1
	ldrh	r3, [r3, r4]
	strb	r5, [r2, r3]
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	adds	r5, r3, #0
	ldr	r3, [sp, #40]
	subs	r4, r2, r0
	mov	fp, r3
	movs	r3, #0
	cmp	r4, #0
	bge.n	.L_080eb340
	movs	r2, #1
	negs	r2, r2
	mov	r9, r2
	negs	r4, r4
	b.n	.L_080eb344
.L_080eb340:
	movs	r2, #1
	mov	r9, r2
.L_080eb344:
	subs	r7, r5, r1
	cmp	r7, #0
	bge.n	.L_080eb354
	movs	r2, #1
	negs	r2, r2
	mov	sl, r2
	negs	r7, r7
	b.n	.L_080eb358
.L_080eb354:
	movs	r2, #1
	mov	sl, r2
.L_080eb358:
	adds	r6, r0, #0
	adds	r5, r1, #0
	cmp	r4, r7
	ble.n	.L_080eb390
	movs	r2, #0
	mov	r8, r2
	cmp	r8, r4
	bge.n	.L_080eb3c0
.L_080eb368:
	adds	r0, r6, #0
	adds	r1, r5, #0
	mov	r2, fp
	str	r3, [sp, #4]
	str	r4, [sp, #0]
	bl	.L_080eb2d8
	ldr	r3, [sp, #4]
	ldr	r4, [sp, #0]
	adds	r3, r3, r7
	cmp	r3, r4
	blt.n	.L_080eb384
	subs	r3, r3, r4
	add	r5, sl
.L_080eb384:
	movs	r2, #1
	add	r8, r2
	add	r6, r9
	cmp	r8, r4
	blt.n	.L_080eb368
	b.n	.L_080eb3c0
.L_080eb390:
	cmp	r7, #0
	ble.n	.L_080eb3c0
	mov	r8, r7
.L_080eb396:
	adds	r0, r6, #0
	adds	r1, r5, #0
	mov	r2, fp
	str	r3, [sp, #4]
	str	r4, [sp, #0]
	bl	.L_080eb2d8
	ldr	r3, [sp, #4]
	ldr	r4, [sp, #0]
	adds	r3, r3, r4
	cmp	r3, r7
	blt.n	.L_080eb3b2
	subs	r3, r3, r7
	add	r6, r9
.L_080eb3b2:
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r2, r8
	add	r5, sl
	cmp	r2, #0
	bne.n	.L_080eb396
.L_080eb3c0:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
