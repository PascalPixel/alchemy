.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002096, 0x08002096
	.set sub_080148e8, 0x080148e8
	.set sub_080cad84, 0x080cad84
	.set sub_080d9e74, 0x080d9e74
	.global Overlay_080d9f1c
Overlay_080d9f1c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r1, [sp, #32]
	str	r2, [sp, #28]
	str	r3, [sp, #24]
	str	r0, [sp, #36]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #160
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #72]
	str	r3, [sp, #20]
	movs	r3, #0
	str	r3, [sp, #16]
	ldr	r3, [sp, #32]
	ldr	r1, [sp, #76]
	subs	r2, r2, r3
	str	r2, [sp, #12]
	asrs	r2, r2, #8
	mov	r9, r2
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #24]
	ldr	r0, [sp, #80]
	subs	r1, r1, r2
	str	r1, [sp, #8]
	subs	r0, r0, r3
	asrs	r1, r1, #8
	mov	r8, r1
	adds	r1, r0, #0
	asrs	r1, r1, #8
	str	r0, [sp, #4]
	mov	sl, r1
	ldr	r6, [pc, #236]
	mov	r1, r9
	mov	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4641
	adds	r5, r0, #0
	mov	r0, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	mov	r8, r0
	mov	r0, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4445
	adds	r5, r5, r0
	ldr	r3, [pc, #208]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2106
	bl	sub_08002054
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #36]
	lsls	r2, r2, #3
	ldr	r1, [sp, #20]
	subs	r5, r2, r3
	lsls	r5, r5, #2
	adds	r3, r1, r5
	adds	r3, #36
	mov	r1, r9
	adds	r6, r0, #1
	mov	r0, sl
	str	r2, [sp, #0]
	mov	r8, r3
	bl	sub_080148e8
	ldr	r2, [sp, #20]
	movs	r3, #0
	lsls	r0, r0, #16
	adds	r5, #16
	lsrs	r0, r0, #16
	movs	r7, #0
	mov	fp, r3
	mov	r9, r3
	mov	sl, r3
	str	r0, [r2, r5]
	b.n	.L_080da01c
.L_080d9fcc:
	ldr	r3, [sp, #36]
	adds	r1, r6, #0
	adds	r3, #1
	strb	r3, [r5, #17]
	mov	r0, sl
	bl	sub_08002054
	ldr	r1, [sp, #32]
	adds	r0, r1, r0
	str	r0, [r5, #4]
	adds	r1, r6, #0
	mov	r0, r9
	bl	sub_08002054
	ldr	r2, [sp, #28]
	adds	r1, r6, #0
	adds	r0, r2, r0
	str	r0, [r5, #8]
	mov	r0, fp
	bl	sub_08002054
	ldr	r3, [sp, #24]
	adds	r0, r3, r0
	str	r0, [r5, #12]
	cmp	r7, r6
	bne.n	.L_080da004
	movs	r3, #2
	b.n	.L_080da006
.L_080da004:
	movs	r3, #1
.L_080da006:
	strb	r3, [r5, #16]
	mov	r1, r8
	str	r5, [r1, #0]
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #8]
	ldr	r1, [sp, #12]
	mov	r8, r5
	add	fp, r2
	add	r9, r3
	add	sl, r1
	adds	r7, #1
.L_080da01c:
	cmp	r7, r6
	bgt.n	.L_080da030
	bl	sub_080d9e74
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_080d9fcc
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #16]
.L_080da030:
	mov	r1, r8
	movs	r3, #0
	str	r3, [r1, #0]
	ldr	r2, [sp, #0]
	ldr	r1, [sp, #36]
	subs	r3, r2, r1
	ldr	r1, [sp, #20]
	lsls	r3, r3, #2
	adds	r3, #32
	movs	r2, #1
	str	r2, [r1, r3]
	ldr	r0, [sp, #16]
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300021c
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	adds	r0, r1, #0
	adds	r5, r2, #0
	sub	sp, #16
	adds	r7, r3, #0
	bl	sub_080cad84
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080cad84
	adds	r5, r0, #0
	cmp	r6, #0
	beq.n	.L_080da0a8
	cmp	r5, #0
	beq.n	.L_080da0a8
	ldr	r0, [r5, #8]
	ldr	r2, [r6, #12]
	ldr	r1, [r6, #8]
	ldr	r3, [r6, #16]
	str	r0, [sp, #0]
	movs	r4, #128
	ldr	r0, [r5, #12]
	lsls	r4, r4, #12
	adds	r0, r0, r4
	str	r0, [sp, #4]
	adds	r2, r2, r4
	ldr	r0, [r5, #16]
	str	r7, [sp, #12]
	str	r0, [sp, #8]
	mov	r0, r8
	bl	.L_080da0b0
.L_080da0a8:
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_080da0b0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #24
	str	r2, [sp, #16]
	ldr	r2, [sp, #56]
	str	r3, [sp, #12]
	str	r1, [sp, #20]
	movs	r3, #192
	lsls	r3, r3, #18
	subs	r2, r2, r1
	adds	r3, #160
	ldr	r3, [r3, #0]
	asrs	r1, r2, #8
	str	r2, [sp, #8]
	mov	r9, r0
	ldr	r2, [sp, #16]
	ldr	r0, [sp, #60]
	ldr	r4, [sp, #64]
	subs	r0, r0, r2
	ldr	r2, [sp, #12]
	mov	fp, r3
	adds	r3, r0, #0
	subs	r4, r4, r2
	asrs	r3, r3, #8
	mov	r8, r3
	adds	r3, r4, #0
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	asrs	r3, r3, #8
	ldr	r6, [pc, #184]
	adds	r0, r1, #0
	mov	sl, r3
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4641
	adds	r5, r0, #0
	mov	r0, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4651
	mov	r8, r0
	mov	r0, sl
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4445
	adds	r5, r5, r0
	ldr	r3, [pc, #156]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2106
	bl	sub_08002054
	mov	r2, r9
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #36
	mov	r2, fp
	ldr	r6, [r2, r3]
	movs	r3, #0
	adds	r7, r0, #1
	mov	r8, r3
	cmp	r8, r7
	bgt.n	.L_080da1a0
	mov	fp, r3
	mov	r9, r3
	mov	sl, r3
.L_080da142:
	mov	r2, r8
	lsls	r0, r2, #15
	adds	r1, r7, #0
	bl	sub_08002054
	bl	sub_08002096
	ldr	r3, [pc, #92]
	ldr	r1, [sp, #68]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c39
	adds	r5, r0, #0
	mov	r0, sl
	bl	sub_08002054
	ldr	r3, [sp, #20]
	adds	r1, r7, #0
	adds	r0, r3, r0
	str	r0, [r6, #4]
	mov	r0, r9
	bl	sub_08002054
	ldr	r2, [sp, #16]
	adds	r1, r7, #0
	adds	r0, r2, r0
	adds	r0, r0, r5
	str	r0, [r6, #8]
	mov	r0, fp
	bl	sub_08002054
	ldr	r3, [sp, #12]
	movs	r2, #1
	adds	r0, r3, r0
	str	r0, [r6, #12]
	strb	r2, [r6, #16]
	ldr	r3, [sp, #0]
	ldr	r2, [sp, #4]
	add	fp, r3
	ldr	r3, [sp, #8]
	add	r9, r2
	movs	r2, #1
	add	r8, r2
	ldr	r6, [r6, #0]
	add	sl, r3
	cmp	r8, r7
	ble.n	.L_080da142
.L_080da1a0:
	movs	r0, #0
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300021c
	.4byte 0x030002d4
