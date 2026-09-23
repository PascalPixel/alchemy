.syntax unified
	.thumb
	.set sub_08020010, 0x08020010
	.global Func_08192894
	.thumb_func
Func_08192894:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	mov	r0, r9
	add	r3, sp, #40
	str	r0, [r3, #0]
	adds	r3, r0, #0
	str	r3, [sp, #12]
	subs	r3, #4
	mov	r9, r3
	mov	r2, sp
	movs	r3, #100
	movs	r1, #0
	adds	r2, #16
	movs	r0, #160
	str	r3, [sp, #0]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	lsls	r0, r0, #1
	movs	r1, #176
	movs	r2, #0
	add	r6, sp, #24
	mov	fp, r0
	mov	sl, r1
	mov	r8, r2
.L_081928d0:
	mov	r3, r9
	ldr	r2, [r3, #0]
	mov	r0, r8
	ldrsh	r3, [r2, r0]
	cmp	r3, #0
	bne.n	.L_081928de
	b.n	.L_081929d0
.L_081928de:
	ldr	r0, [sp, #4]
	lsls	r3, r3, #1
	str	r3, [r0, #4]
	str	r3, [sp, #16]
	movs	r3, #255
	movs	r1, #0
	lsls	r3, r3, #16
	str	r1, [r6, #12]
	str	r3, [r6, #4]
	ldr	r3, [sp, #8]
	mov	r0, sl
	lsls	r5, r3, #2
	ldr	r3, [r2, r0]
	mov	r1, fp
	str	r3, [r6, #0]
	ldr	r3, [r2, r1]
	adds	r1, r6, #0
	str	r3, [r6, #8]
	ldr	r7, [sp, #12]
	ldr	r2, [sp, #4]
	subs	r7, #8
	ldr	r3, [r7, #0]
	ldr	r0, [r5, r3]
	movs	r3, #0
	bl	sub_08020010
	mov	r2, r9
	ldr	r1, [r2, #0]
	movs	r3, #232
	lsls	r3, r3, #1
	mov	r0, sl
	adds	r2, r5, r3
	ldr	r2, [r1, r2]
	ldr	r3, [r1, r0]
	adds	r3, r3, r2
	str	r3, [r1, r0]
	movs	r2, #152
	lsls	r2, r2, #2
	mov	r0, fp
	adds	r3, r5, r2
	ldr	r3, [r1, r3]
	ldr	r2, [r1, r0]
	adds	r2, r2, r3
	str	r2, [r1, r0]
	mov	r0, sl
	ldr	r3, [r1, r0]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r3, r3, r0
	movs	r0, #128
	lsls	r0, r0, #17
	cmp	r3, r0
	bhi.n	.L_08192954
	cmp	r2, #0
	blt.n	.L_08192954
	movs	r3, #160
	lsls	r3, r3, #16
	cmp	r2, r3
	ble.n	.L_08192960
.L_08192954:
	mov	r0, r9
	ldr	r2, [r0, #0]
	ldr	r3, [pc, #32]
	mov	r1, r8
	strh	r3, [r2, r1]
	b.n	.L_081929d0
.L_08192960:
	ldr	r3, [sp, #12]
	movs	r2, #3
	subs	r3, #12
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08192980
	adds	r2, r1, #2
	ldr	r1, [sp, #0]
	ldrh	r3, [r2, r1]
	adds	r0, r1, #0
	adds	r3, #1
	strh	r3, [r2, r0]
	b.n	.L_08192984
	.2byte 0x0000
	.2byte 0x0000
.L_08192980:
	mov	r1, r8
	adds	r1, #100
.L_08192984:
	mov	r2, r9
	ldr	r4, [r2, #0]
	adds	r2, r4, #2
	ldrh	r3, [r2, r1]
	cmp	r3, #14
	bls.n	.L_08192994
	ldr	r3, [pc, #44]
	strh	r3, [r2, r1]
.L_08192994:
	ldr	r3, [r7, #0]
	ldr	r2, [pc, #44]
	ldr	r0, [r5, r3]
	adds	r3, r4, #2
	ldrh	r3, [r3, r1]
	lsls	r3, r3, #1
	adds	r3, #72
	ldrh	r1, [r4, r3]
	ldr	r3, [pc, #32]
	ands	r1, r3
	ldrh	r3, [r0, #8]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r0, #8]
	mov	r0, r8
	ldrh	r3, [r4, r0]
	ldr	r1, [pc, #20]
	mov	r2, r8
	adds	r3, r3, r1
	strh	r3, [r4, r2]
	b.n	.L_081929d0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0xfffffc00
	.4byte 0x000003ff
	.2byte 0xff00
	.2byte 0xffff
.L_081929d0:
	ldr	r3, [sp, #0]
	ldr	r2, [sp, #8]
	movs	r0, #4
	adds	r3, #2
	movs	r1, #2
	adds	r2, #1
	str	r3, [sp, #0]
	add	fp, r0
	add	sl, r0
	add	r8, r1
	str	r2, [sp, #8]
	cmp	r2, #36
	beq.n	.L_081929ec
	b.n	.L_081928d0
.L_081929ec:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
