.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014d78, 0x08014d78
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08038070, 0x08038070
	.set sub_08038080, 0x08038080
	.set sub_08038260, 0x08038260
	.set sub_080ad010, 0x080ad010
	.set sub_080ad020, 0x080ad020
	.set sub_080ad050, 0x080ad050
	.set sub_080ad1c0, 0x080ad1c0
	.set sub_080c8510, 0x080c8510
	.set sub_080f8170, 0x080f8170
	.set sub_080f8a44, 0x080f8a44
	.set sub_080f8ab4, 0x080f8ab4
	.set sub_080f8c94, 0x080f8c94
	.set sub_080fa458, 0x080fa458
	.set sub_080fac58, 0x080fac58
	.set sub_080fad1c, 0x080fad1c
	.set sub_080fad88, 0x080fad88
	.set sub_080fadd0, 0x080fadd0
	.set sub_080fb554, 0x080fb554
	.set sub_080fb638, 0x080fb638
	.set sub_080fbe24, 0x080fbe24
	.set sub_080fc12c, 0x080fc12c
	.set sub_081c0010, 0x081c0010
	.global Overlay_080fae2c
Overlay_080fae2c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	mov	r8, r1
	adds	r6, r0, #0
	bl	sub_08016ca4
	movs	r2, #226
	lsls	r2, r2, #1
	adds	r5, r7, r2
	adds	r1, r5, #0
	movs	r2, #0
	bl	sub_080fad88
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	ldr	r0, [r7, #36]
	bl	sub_08038260
	mov	r0, r8
	bl	sub_080f8c94
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_080fadd0
	adds	r0, r6, #0
	bl	sub_080fad1c
	cmp	r0, #0
	bne.n	.L_080fae82
	ldr	r0, [pc, #16]
	ldr	r1, [r7, #36]
	movs	r2, #8
	movs	r3, #24
	bl	sub_08038080
.L_080fae82:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x1006
	.2byte 0x0000
.L_080fae8c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	mov	r9, r1
	movs	r1, #0
	sub	sp, #4
	mov	fp, r2
	ldr	r7, [r3, #0]
	mov	sl, r1
	adds	r5, r0, #0
	bl	sub_08016ca4
	mov	r2, r9
	str	r0, [sp, #0]
	lsls	r3, r2, #1
	adds	r3, #216
	ldrh	r3, [r0, r3]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	mov	r8, r3
	bl	sub_080ad010
	mov	r3, fp
	cmp	r3, #1
	bne.n	.L_080faed8
	movs	r1, #128
	lsls	r1, r1, #1
	mov	sl, r1
.L_080faed8:
	ldrb	r0, [r0, #2]
	cmp	r0, #11
	bls.n	.L_080faee0
	b.n	.L_080fb08a
.L_080faee0:
	ldr	r2, [pc, #436]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080fb004
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faff8
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faf70
	.4byte 0x080faf18
	.2byte 0xb004
	.2byte 0x080f
	cmp	r5, r6
	bne.n	.L_080faf20
	movs	r3, #9
	b.n	.L_080faffe
.L_080faf20:
	adds	r0, r6, #0
	bl	sub_08016ca4
	str	r0, [sp, #0]
	movs	r0, #166
	lsls	r0, r0, #1
	bl	sub_08014d78
	movs	r2, #166
	lsls	r2, r2, #1
	ldr	r3, [pc, #356]
	ldr	r1, [sp, #0]
	mov	fp, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c30
	bl	.L_080fb0a4
	adds	r2, r0, #0
	cmp	r2, #0
	beq.n	.L_080fafd6
	ldr	r3, [pc, #340]
	mov	r1, r8
	ands	r1, r3
	adds	r0, r6, #0
	mov	r8, r1
	bl	sub_080ad020
	movs	r3, #1
	adds	r2, r0, #0
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080fafc8
	movs	r3, #9
	mov	r1, sl
	orrs	r1, r3
	mov	sl, r1
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	b.n	.L_080fafce
	cmp	r5, r6
	bne.n	.L_080faf78
	movs	r3, #2
	b.n	.L_080faffe
.L_080faf78:
	adds	r0, r6, #0
	bl	sub_08016ca4
	str	r0, [sp, #0]
	movs	r0, #166
	lsls	r0, r0, #1
	bl	sub_08014d78
	movs	r2, #166
	lsls	r2, r2, #1
	ldr	r3, [pc, #268]
	ldr	r1, [sp, #0]
	mov	fp, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c30
	bl	.L_080fb0a4
	adds	r2, r0, #0
	cmp	r2, #0
	beq.n	.L_080fafd6
	ldr	r3, [pc, #252]
	mov	r1, r8
	ands	r1, r3
	adds	r0, r6, #0
	mov	r8, r1
	bl	sub_080ad020
	movs	r3, #1
	adds	r2, r0, #0
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080fafc8
	movs	r3, #2
	mov	r1, sl
	orrs	r1, r3
	mov	sl, r1
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	b.n	.L_080fafce
.L_080fafc8:
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	mov	r2, r9
.L_080fafce:
	mov	r3, sl
	bl	sub_080f8170
	b.n	.L_080fafe2
.L_080fafd6:
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	sub_080f8170
.L_080fafe2:
	movs	r2, #166
	ldr	r3, [pc, #180]
	ldr	r0, [sp, #0]
	mov	r1, fp
	lsls	r2, r2, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4658
	bl	sub_08013164
	b.n	.L_080fb08a
	cmp	r6, r5
	bne.n	.L_080fb012
	movs	r3, #4
.L_080faffe:
	mov	r2, sl
	orrs	r2, r3
	mov	sl, r2
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	sub_080f8170
	b.n	.L_080fb08a
.L_080fb012:
	adds	r0, r6, #0
	bl	sub_08016ca4
	str	r0, [sp, #0]
	movs	r0, #166
	lsls	r0, r0, #1
	bl	sub_08014d78
	movs	r2, #166
	lsls	r2, r2, #1
	ldr	r3, [pc, #116]
	ldr	r1, [sp, #0]
	mov	fp, r0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c30
	bl	.L_080fb0a4
	adds	r2, r0, #0
	cmp	r2, #0
	beq.n	.L_080fb06a
	adds	r0, r6, #0
	mov	r1, r8
	bl	sub_080ad020
	movs	r3, #1
	adds	r2, r0, #0
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L_080fb05c
	movs	r3, #4
	mov	r1, sl
	orrs	r1, r3
	mov	sl, r1
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	b.n	.L_080fb062
.L_080fb05c:
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	mov	r2, r9
.L_080fb062:
	mov	r3, sl
	bl	sub_080f8170
	b.n	.L_080fb076
.L_080fb06a:
	ldr	r0, [r7, #40]
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	sub_080f8170
.L_080fb076:
	movs	r2, #166
	ldr	r3, [pc, #32]
	ldr	r0, [sp, #0]
	mov	r1, fp
	lsls	r2, r2, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4658
	bl	sub_08013164
.L_080fb08a:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x080faee8
	.4byte 0x03000730
	.2byte 0xfdff
	.2byte 0xffff
.L_080fb0a4:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	bl	sub_08016ca4
	movs	r3, #216
	ldrh	r3, [r0, r3]
	movs	r1, #0
	movs	r6, #0
	adds	r0, #216
	b.n	.L_080fb0fa
.L_080fb0b8:
	ldrh	r2, [r0, #0]
	ldr	r3, [pc, #40]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080fb0f0
	lsrs	r3, r2, #11
	adds	r2, r3, #1
	cmp	r3, #0
	bne.n	.L_080fb0cc
	movs	r2, #1
.L_080fb0cc:
	cmp	r2, #0
	beq.n	.L_080fb0e8
	adds	r5, r2, #0
.L_080fb0d2:
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_080ad050
	subs	r5, #1
	adds	r1, r0, #0
	cmp	r5, #0
	bne.n	.L_080fb0d2
	b.n	.L_080fb0e8
	.2byte 0x0200
	.2byte 0x0000
.L_080fb0e8:
	movs	r0, #0
	cmp	r1, #2
	bne.n	.L_080fb102
	b.n	.L_080fb0fe
.L_080fb0f0:
	adds	r6, #1
	adds	r0, #2
	cmp	r6, #14
	bgt.n	.L_080fb100
	ldrh	r3, [r0, #0]
.L_080fb0fa:
	cmp	r3, #0
	bne.n	.L_080fb0b8
.L_080fb0fe:
	movs	r1, #1
.L_080fb100:
	adds	r0, r1, #0
.L_080fb102:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #0
	sub	sp, #16
	movs	r3, #192
	mov	r8, r1
	add	r1, sp, #8
	lsls	r3, r3, #18
	adds	r3, #220
	mov	fp, r1
	ldr	r6, [r3, #0]
	movs	r2, #0
	movs	r3, #1
	mov	r0, fp
	mov	sl, r2
	mov	r9, r3
	bl	.L_080fb410
	movs	r2, #135
	lsls	r2, r2, #2
	adds	r2, r6, r2
	str	r2, [sp, #4]
	movs	r7, #0
	ldrh	r3, [r2, #0]
	cmp	r3, #1
	beq.n	.L_080fb178
	bl	sub_080fa458
	ldr	r0, [r6, #56]
	bl	sub_08038260
	adds	r3, r6, #0
	adds	r3, #240
	ldr	r5, [r3, #0]
	bl	sub_080fbe24
	adds	r0, r5, #0
	bl	sub_08038260
	movs	r3, #3
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #3
	movs	r3, #16
	bl	sub_08038070
	bl	sub_080fc12c
	mov	r0, fp
	adds	r1, r5, #0
	bl	sub_080fb554
.L_080fb178:
	ldr	r1, [sp, #4]
	mov	r3, r8
	movs	r2, #177
	strh	r3, [r1, #0]
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r6, r2
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_080fb1f6
	mov	r1, fp
	movs	r3, #2
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_080fb1a2
	movs	r2, #0
	movs	r7, #2
	mov	sl, r2
.L_080fb1a2:
	mov	r1, fp
	movs	r3, #3
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_080fb1b2
	movs	r2, #1
	movs	r7, #0
	mov	sl, r2
.L_080fb1b2:
	mov	r1, fp
	movs	r3, #1
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_080fb1c2
	movs	r2, #0
	movs	r7, #1
	mov	sl, r2
.L_080fb1c2:
	mov	r1, fp
	movs	r3, #4
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_080fb1d2
	movs	r2, #1
	movs	r7, #1
	mov	sl, r2
.L_080fb1d2:
	mov	r1, fp
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_080fb218
	movs	r2, #0
	movs	r7, #0
	mov	sl, r2
	b.n	.L_080fb218
.L_080fb1e4:
	movs	r0, #113
	bl	sub_081c0010
	movs	r3, #1
	movs	r1, #177
	negs	r3, r3
	lsls	r1, r1, #1
	mov	r8, r3
	b.n	.L_080fb35e
.L_080fb1f6:
	movs	r1, #3
	adds	r0, r5, #0
	bl	sub_08002064
	lsls	r0, r0, #24
	asrs	r7, r0, #24
	movs	r1, #3
	adds	r0, r5, #0
	bl	sub_08002054
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	mov	sl, r0
	lsls	r3, r0, #1
	add	r3, sl
	adds	r3, r3, r7
	mov	r8, r3
.L_080fb218:
	lsls	r0, r7, #2
	adds	r0, r0, r7
	mov	r3, sl
	lsls	r1, r3, #3
	lsls	r0, r0, #3
	adds	r0, #86
	adds	r1, #30
	bl	sub_080f8ab4
	b.n	.L_080fb3d4
.L_080fb22c:
	mov	r1, r9
	cmp	r1, #0
	beq.n	.L_080fb2d2
	movs	r2, #0
	adds	r0, r7, #3
	movs	r1, #3
	mov	r9, r2
	bl	sub_08002064
	mov	r2, sl
	adds	r2, #2
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	subs	r2, r2, r3
	mov	sl, r2
	lsls	r3, r2, #1
	adds	r7, r0, #0
	add	r3, sl
	adds	r3, r3, r7
	mov	r8, r3
	bl	sub_080fac58
	mov	r3, r8
	cmp	r3, #2
	ble.n	.L_080fb296
	movs	r1, #152
	lsls	r1, r1, #2
	adds	r2, r6, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	subs	r1, #248
	adds	r2, r6, r1
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	.L_080fae8c
	mov	r2, r8
	cmp	r2, #3
	bne.n	.L_080fb2d2
	movs	r1, #144
	ldr	r0, [pc, #372]
	lsls	r1, r1, #3
	bl	sub_080145a8
	b.n	.L_080fb2d2
.L_080fb296:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_080fb2be
	movs	r1, #152
	lsls	r1, r1, #2
	adds	r3, r6, r1
	movs	r2, #128
	strb	r5, [r3, #0]
	lsls	r2, r2, #2
	adds	r2, #22
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	subs	r1, #248
	adds	r2, r6, r1
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	.L_080fae8c
	b.n	.L_080fb2d2
.L_080fb2be:
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	adds	r3, r6, r2
	ldrb	r1, [r3, #0]
	ldr	r0, [r6, #40]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080f8170
.L_080fb2d2:
	lsls	r0, r7, #2
	mov	r3, sl
	adds	r0, r0, r7
	lsls	r1, r3, #3
	lsls	r0, r0, #3
	adds	r0, #86
	adds	r1, #30
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [pc, #284]
	movs	r3, #1
	ldr	r2, [r5, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fb368
	mov	r1, fp
	mov	r2, r8
	ldrsb	r3, [r1, r2]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_080fb30c
	movs	r0, #114
	bl	sub_081c0010
	b.n	.L_080fb368
.L_080fb30c:
	mov	r2, r8
	cmp	r2, #5
	bhi.n	.L_080fb354
	lsls	r3, r2, #2
	ldr	r2, [pc, #244]
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.2byte 0xb334
	lsrs	r7, r1, #32
	.2byte 0xb33c
	lsrs	r7, r1, #32
	.2byte 0xb344
	lsrs	r7, r1, #32
	.2byte 0xb344
	lsrs	r7, r1, #32
	.2byte 0xb34c
	lsrs	r7, r1, #32
	.2byte 0xb344
	lsrs	r7, r1, #32
	movs	r0, #174
	bl	sub_081c0010
	b.n	.L_080fb35a
	movs	r0, #175
	bl	sub_081c0010
	b.n	.L_080fb35a
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_080fb35a
	movs	r0, #117
	bl	sub_081c0010
	b.n	.L_080fb35a
.L_080fb354:
	movs	r0, #112
	bl	sub_081c0010
.L_080fb35a:
	movs	r1, #177
	lsls	r1, r1, #1
.L_080fb35e:
	adds	r1, #255
	adds	r3, r6, r1
	mov	r2, r8
	strb	r2, [r3, #0]
	b.n	.L_080fb3e4
.L_080fb368:
	ldr	r2, [r5, #4]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fb374
	b.n	.L_080fb1e4
.L_080fb374:
	ldr	r2, [r5, #12]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fb38e
	subs	r3, #65
	movs	r1, #1
	movs	r0, #111
	add	sl, r3
	mov	r9, r1
	bl	sub_081c0010
	b.n	.L_080fb3d4
.L_080fb38e:
	ldr	r2, [r5, #12]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fb3a6
	movs	r2, #1
	movs	r0, #111
	add	sl, r2
	mov	r9, r2
	bl	sub_081c0010
	b.n	.L_080fb3d4
.L_080fb3a6:
	ldr	r2, [r5, #12]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fb3be
	movs	r3, #1
	movs	r0, #111
	adds	r7, #1
	mov	r9, r3
	bl	sub_081c0010
	b.n	.L_080fb3d4
.L_080fb3be:
	ldr	r3, [r5, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fb3d4
	movs	r1, #1
	movs	r0, #111
	subs	r7, #1
	mov	r9, r1
	bl	sub_081c0010
.L_080fb3d4:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_080fb3e4
	b.n	.L_080fb22c
.L_080fb3e4:
	movs	r3, #152
	lsls	r3, r3, #2
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	bl	sub_080fac58
	mov	r0, r8
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080fabe1
	.4byte 0x03001150
	.2byte 0xb31c
	.2byte 0x080f
.L_080fb410:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r1, #182
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad010
	ldrb	r3, [r0, #2]
	mov	r8, r0
	cmp	r3, #0
	bne.n	.L_080fb448
	movs	r2, #1
	movs	r3, #1
	negs	r2, r2
	strb	r3, [r5, #0]
	adds	r3, r2, #0
	b.n	.L_080fb452
.L_080fb448:
	movs	r1, #1
	negs	r1, r1
	adds	r3, r1, #0
	strb	r3, [r5, #0]
	movs	r3, #1
.L_080fb452:
	strb	r3, [r5, #1]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #22
	movs	r1, #182
	adds	r3, r7, r2
	lsls	r1, r1, #1
	ldrb	r0, [r3, #0]
	adds	r3, r7, r1
	ldrh	r1, [r3, #0]
	bl	sub_080fb638
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_080fb478
	movs	r3, #1
	strb	r3, [r5, #0]
	b.n	.L_080fb47a
.L_080fb478:
	strb	r0, [r5, #0]
.L_080fb47a:
	movs	r3, #182
	lsls	r3, r3, #1
	adds	r6, r7, r3
	ldrh	r2, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fb496
	movs	r1, #1
	negs	r1, r1
	adds	r3, r1, #0
	strb	r3, [r5, #0]
	ldrh	r2, [r6, #0]
.L_080fb496:
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #22
	adds	r3, r7, r1
	subs	r1, #23
	ldrb	r0, [r3, #0]
	ands	r1, r2
	bl	sub_080ad1c0
	cmp	r0, #0
	bne.n	.L_080fb4b4
	movs	r2, #1
	negs	r2, r2
	adds	r3, r2, #0
	strb	r3, [r5, #1]
.L_080fb4b4:
	movs	r1, #1
	strb	r1, [r5, #3]
	strb	r1, [r5, #5]
	strb	r1, [r5, #2]
	movs	r3, #128
	ldrh	r2, [r6, #0]
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fb4d4
	strb	r1, [r5, #4]
	movs	r1, #1
	negs	r1, r1
	adds	r3, r1, #0
	strb	r3, [r5, #1]
	b.n	.L_080fb4dc
.L_080fb4d4:
	movs	r2, #1
	negs	r2, r2
	adds	r3, r2, #0
	strb	r3, [r5, #4]
.L_080fb4dc:
	mov	r3, r8
	ldrb	r2, [r3, #3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fb506
	movs	r2, #1
	negs	r2, r2
	adds	r1, r2, #0
	strb	r1, [r5, #4]
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fb506
	strb	r1, [r5, #3]
	strb	r1, [r5, #5]
.L_080fb506:
	movs	r1, #182
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r0, #255
	ands	r0, r3
	bl	sub_080c8510
	cmp	r0, #0
	beq.n	.L_080fb522
	movs	r3, #1
	strb	r3, [r5, #0]
.L_080fb522:
	movs	r2, #139
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bhi.n	.L_080fb538
	movs	r1, #1
	negs	r1, r1
	adds	r3, r1, #0
	strb	r3, [r5, #3]
.L_080fb538:
	mov	r3, r8
	ldrb	r2, [r3, #3]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fb54c
	movs	r1, #1
	negs	r1, r1
	adds	r3, r1, #0
	strb	r3, [r5, #5]
.L_080fb54c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
