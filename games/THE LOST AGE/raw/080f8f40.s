.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_080138a8, 0x080138a8
	.set sub_08038270, 0x08038270
	.set sub_080383f8, 0x080383f8
	.set sub_081c0010, 0x081c0010
	.global Overlay_080f8f40
Overlay_080f8f40:
	push	{lr}
	ldr	r2, [pc, #72]
	cmp	r0, #1
	beq.n	.L_080f8f5c
	cmp	r0, #1
	bgt.n	.L_080f8f52
	cmp	r0, #0
	beq.n	.L_080f8f58
	b.n	.L_080f8f62
.L_080f8f52:
	cmp	r0, #2
	beq.n	.L_080f8f60
	b.n	.L_080f8f62
.L_080f8f58:
	ldr	r2, [pc, #52]
	b.n	.L_080f8f62
.L_080f8f5c:
	ldr	r2, [pc, #52]
	b.n	.L_080f8f62
.L_080f8f60:
	ldr	r2, [pc, #52]
.L_080f8f62:
	ldrb	r3, [r2, #0]
	movs	r4, #255
	strb	r3, [r1, #0]
	lsls	r4, r4, #24
	lsls	r3, r3, #24
	movs	r0, #0
	cmp	r3, r4
	beq.n	.L_080f8f8a
.L_080f8f72:
	adds	r0, #1
	cmp	r0, #31
	bgt.n	.L_080f8f8a
	adds	r2, #1
	ldrb	r3, [r2, #0]
	adds	r1, #1
	movs	r4, #255
	strb	r3, [r1, #0]
	lsls	r4, r4, #24
	lsls	r3, r3, #24
	cmp	r3, r4
	bne.n	.L_080f8f72
.L_080f8f8a:
	pop	{pc}
	.4byte 0x081059ee
	.4byte 0x08105a20
	.4byte 0x08105a08
	.2byte 0x59fb
	.2byte 0x0810
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r1
	ldr	r1, [sp, #28]
	adds	r5, r0, #0
	adds	r7, r2, #0
	movs	r0, #1
	mov	r2, r8
	adds	r6, r3, #0
	mov	sl, r1
	negs	r0, r0
	cmp	r2, #0
	bne.n	.L_080f8fbe
	b.n	.L_080f90f6
.L_080f8fbe:
	ldr	r0, [pc, #320]
	bl	sub_080383f8
	adds	r1, r7, #0
	mov	r0, r8
	bl	sub_08002054
	adds	r1, r7, #0
	mov	r9, r0
	mov	r0, r8
	bl	sub_08002064
	cmp	r0, #0
	beq.n	.L_080f8fde
	movs	r3, #1
	add	r9, r3
.L_080f8fde:
	cmp	r5, #0
	beq.n	.L_080f8ffc
	ldr	r2, [pc, #288]
	movs	r3, #16
	ldr	r4, [r2, #12]
	ldr	r1, [r2, #12]
	ldr	r5, [r2, #12]
	ands	r4, r3
	ldr	r2, [r2, #12]
	movs	r3, #32
	ands	r1, r3
	movs	r3, #64
	ands	r5, r3
	movs	r3, #128
	b.n	.L_080f9014
.L_080f8ffc:
	ldr	r2, [pc, #260]
	movs	r3, #128
	ldr	r4, [r2, #12]
	ldr	r1, [r2, #12]
	ldr	r5, [r2, #12]
	ands	r4, r3
	ldr	r2, [r2, #12]
	movs	r3, #64
	ands	r1, r3
	movs	r3, #32
	ands	r5, r3
	movs	r3, #16
.L_080f9014:
	ands	r2, r3
	cmp	r5, #0
	beq.n	.L_080f904c
	movs	r0, #111
	bl	sub_081c0010
	mov	r1, sl
	ldr	r3, [r1, #0]
	subs	r3, #1
	str	r3, [r1, #0]
	cmp	r3, #0
	bge.n	.L_080f9032
	mov	r3, r9
	subs	r3, #1
	str	r3, [r1, #0]
.L_080f9032:
	mov	r2, sl
	ldr	r3, [r2, #0]
	mov	r2, r8
	adds	r0, r7, #0
	muls	r0, r3
	ldr	r3, [r6, #0]
	subs	r2, #1
	adds	r3, r3, r0
	cmp	r3, r2
	ble.n	.L_080f908e
	mov	r1, r8
	subs	r3, r1, r0
	b.n	.L_080f9082
.L_080f904c:
	cmp	r2, #0
	beq.n	.L_080f9096
	movs	r0, #111
	bl	sub_081c0010
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	mov	r2, r9
	subs	r2, #1
	cmp	r3, r2
	ble.n	.L_080f906a
	mov	r3, sl
	str	r5, [r3, #0]
.L_080f906a:
	mov	r1, sl
	ldr	r3, [r1, #0]
	mov	r2, r8
	adds	r0, r7, #0
	muls	r0, r3
	ldr	r3, [r6, #0]
	subs	r2, #1
	adds	r3, r3, r0
	cmp	r3, r2
	ble.n	.L_080f908e
	mov	r2, r8
	subs	r3, r2, r0
.L_080f9082:
	subs	r3, #1
	subs	r1, r7, #1
	str	r3, [r6, #0]
	cmp	r3, r1
	ble.n	.L_080f908e
	str	r1, [r6, #0]
.L_080f908e:
	bl	sub_080138a8
	movs	r0, #1
	b.n	.L_080f90f6
.L_080f9096:
	cmp	r1, #0
	beq.n	.L_080f90c4
	movs	r0, #111
	bl	sub_081c0010
	ldr	r3, [r6, #0]
	subs	r3, #1
	str	r3, [r6, #0]
	cmp	r3, #0
	bge.n	.L_080f90f4
	subs	r2, r7, #1
	str	r2, [r6, #0]
	mov	r1, sl
	ldr	r3, [r1, #0]
	mov	r1, r8
	muls	r3, r7
	subs	r3, r1, r3
	subs	r3, #1
	str	r3, [r6, #0]
	cmp	r3, r2
	ble.n	.L_080f90f4
	str	r2, [r6, #0]
	b.n	.L_080f90f4
.L_080f90c4:
	movs	r0, #1
	negs	r0, r0
	cmp	r4, #0
	beq.n	.L_080f90f6
	movs	r0, #111
	bl	sub_081c0010
	ldr	r2, [r6, #0]
	mov	r1, sl
	adds	r2, #1
	str	r2, [r6, #0]
	movs	r0, #0
	ldr	r3, [r1, #0]
	mov	r1, r8
	muls	r3, r7
	subs	r3, r1, r3
	cmp	r2, r3
	bne.n	.L_080f90ea
	str	r0, [r6, #0]
.L_080f90ea:
	ldr	r3, [r6, #0]
	subs	r2, r7, #1
	cmp	r3, r2
	ble.n	.L_080f90f4
	str	r0, [r6, #0]
.L_080f90f4:
	movs	r0, #0
.L_080f90f6:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x06002500
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, lr}
	movs	r3, #160
	lsls	r0, r0, #5
	lsls	r3, r3, #19
	adds	r5, r0, r3
	movs	r2, #128
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #64]
	adds	r1, r5, #0
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [pc, #52]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrh	r2, [r5, #8]
	movs	r0, #31
	lsls	r3, r2, #16
	lsrs	r4, r3, #26
	lsrs	r1, r3, #21
	ldr	r3, [pc, #28]
	adds	r4, #9
	ands	r1, r3
	ands	r0, r2
	cmp	r4, #31
	bls.n	.L_080f914a
	movs	r4, #31
.L_080f914a:
	adds	r1, #9
	cmp	r1, #31
	bls.n	.L_080f9152
	movs	r1, #31
.L_080f9152:
	adds	r0, #9
	cmp	r0, #31
	bls.n	.L_080f9164
	movs	r0, #31
	b.n	.L_080f9164
	.4byte 0x0000001f
	.2byte 0x01e0
	.2byte 0x0500
.L_080f9164:
	lsls	r3, r4, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	adds	r6, r2, #0
	sub	sp, #4
	adds	r1, r6, #0
	mov	r9, r0
	mov	fp, r3
	adds	r0, r5, #0
	movs	r3, #49
	ldr	r7, [sp, #36]
	mov	sl, r3
	bl	sub_08002054
	adds	r1, r6, #0
	mov	r8, r0
	adds	r0, r5, #0
	bl	sub_08002064
	cmp	r0, #0
	beq.n	.L_080f91a8
	movs	r3, #1
	add	r8, r3
.L_080f91a8:
	mov	r3, r8
	subs	r7, r7, r3
	cmp	r3, #1
	ble.n	.L_080f9214
	movs	r1, #241
	movs	r0, #0
	lsls	r1, r1, #8
	movs	r3, #1
	str	r0, [sp, #0]
	adds	r1, #40
	subs	r2, r7, #1
	negs	r3, r3
	mov	r0, r9
	movs	r5, #0
	bl	sub_08038270
	cmp	r5, r8
	bge.n	.L_080f91fe
.L_080f91cc:
	cmp	r5, fp
	bne.n	.L_080f91e2
	movs	r3, #2
	str	r3, [sp, #0]
	mov	r0, r9
	mov	r1, sl
	adds	r2, r7, #0
	subs	r3, #3
	bl	sub_08038270
	b.n	.L_080f91f2
.L_080f91e2:
	movs	r3, #3
	str	r3, [sp, #0]
	mov	r0, r9
	mov	r1, sl
	adds	r2, r7, #0
	subs	r3, #4
	bl	sub_08038270
.L_080f91f2:
	movs	r3, #1
	adds	r5, #1
	add	sl, r3
	adds	r7, #1
	cmp	r5, r8
	blt.n	.L_080f91cc
.L_080f91fe:
	movs	r1, #241
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r3, #1
	str	r2, [sp, #0]
	adds	r1, #41
	negs	r3, r3
	mov	r0, r9
	adds	r2, r7, #0
	bl	sub_08038270
.L_080f9214:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
