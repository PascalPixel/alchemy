.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014d00, 0x08014d00
	.set sub_08020120, 0x08020120
	.set sub_08020138, 0x08020138
	.set sub_08020148, 0x08020148
	.set sub_080cad84, 0x080cad84
	.global Func_080d440c
	.thumb_func
Func_080d440c:
.L_080d440c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r1
	movs	r1, #213
	sub	sp, #16
	lsls	r1, r1, #4
	adds	r6, r0, #0
	movs	r0, #108
	str	r3, [sp, #12]
	adds	r7, r2, #0
	bl	sub_08014d00
	movs	r1, #230
	str	r0, [sp, #8]
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r5, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	movs	r2, #240
	adds	r3, r1, #0
	adds	r3, #236
	ldr	r3, [r3, #0]
	lsls	r2, r2, #15
	adds	r2, r3, r2
	str	r2, [sp, #4]
	adds	r3, r1, #0
	adds	r3, #240
	ldr	r2, [r5, #12]
	ldr	r3, [r3, #0]
	movs	r0, #192
	adds	r3, r3, r2
	lsls	r0, r0, #15
	adds	r0, r3, r0
	str	r0, [sp, #0]
	adds	r3, r1, #0
	adds	r3, #244
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #156]
	adds	r0, r0, r3
	adds	r3, r1, #0
	adds	r3, #248
	ldr	r3, [r3, #0]
	mov	fp, r0
	adds	r3, r3, r2
	ldr	r2, [pc, #148]
	adds	r0, r5, #0
	adds	r2, r2, r3
	movs	r3, #8
	adds	r3, r3, r5
	str	r3, [r1, #0]
	mov	r8, r3
	mov	r9, r2
	bl	sub_08020138
	movs	r3, #1
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L_080d4492
	mov	r0, r8
	ldr	r6, [r0, #0]
.L_080d4492:
	cmp	sl, r3
	bne.n	.L_080d449a
	ldr	r1, [r5, #12]
	mov	sl, r1
.L_080d449a:
	cmp	r7, r3
	bne.n	.L_080d44a0
	ldr	r7, [r5, #16]
.L_080d44a0:
	ldr	r2, [sp, #4]
	cmp	r6, r2
	bge.n	.L_080d44a8
	adds	r6, r2, #0
.L_080d44a8:
	ldr	r3, [sp, #0]
	cmp	r7, r3
	bge.n	.L_080d44b0
	adds	r7, r3, #0
.L_080d44b0:
	cmp	r6, fp
	ble.n	.L_080d44b6
	mov	r6, fp
.L_080d44b6:
	cmp	r7, r9
	ble.n	.L_080d44bc
	mov	r7, r9
.L_080d44bc:
	ldr	r0, [sp, #12]
	cmp	r0, #0
	bne.n	.L_080d44ea
	mov	r1, r8
	mov	r2, sl
	str	r6, [r1, #0]
	movs	r0, #1
	str	r2, [r5, #12]
	str	r7, [r5, #16]
	bl	sub_08013560
	ldr	r0, [sp, #8]
	movs	r1, #197
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	beq.n	.L_080d44f6
	bl	sub_08020120
	b.n	.L_080d44f6
.L_080d44ea:
	adds	r0, r5, #0
	adds	r1, r6, #0
	mov	r2, sl
	adds	r3, r7, #0
	bl	sub_08020148
.L_080d44f6:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xff880000
	.2byte 0x0000
	.2byte 0xffc0
	.2byte 0xb520
	adds	r5, r1, #0
	bl	sub_080cad84
	adds	r3, r0, #0
	cmp	r3, #0
	beq.n	.L_080d4528
	movs	r1, #1
	ldr	r0, [r3, #8]
	ldr	r2, [r3, #16]
	negs	r1, r1
	adds	r3, r5, #0
	bl	.L_080d440c
.L_080d4528:
	pop	{r5, pc}
	.2byte 0x0000
