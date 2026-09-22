.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08038080, 0x08038080
	.set sub_080380d0, 0x080380d0
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_080f811c, 0x080f811c
	.set sub_080f815c, 0x080f815c
	.set sub_080f8170, 0x080f8170
	.set sub_080f8888, 0x080f8888
	.set sub_080f8a44, 0x080f8a44
	.set sub_080f8ab4, 0x080f8ab4
	.set sub_080f8e08, 0x080f8e08
	.set sub_080fa3d4, 0x080fa3d4
	.set sub_080fad1c, 0x080fad1c
	.set sub_080fad88, 0x080fad88
	.set sub_080fae2c, 0x080fae2c
	.set sub_081004b8, 0x081004b8
	.set sub_08100700, 0x08100700
	.set sub_08104c00, 0x08104c00
	.set sub_08104d5c, 0x08104d5c
	.set sub_08104ef8, 0x08104ef8
	.set sub_0810508c, 0x0810508c
	.set sub_081c0010, 0x081c0010
	.global Overlay_080fa458
Overlay_080fa458:
.L_080fa458:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r0, #13
	adds	r3, #76
	movs	r1, #31
.L_080fa468:
	ldmia	r3!, {r2}
	cmp	r2, #0
	beq.n	.L_080fa470
	strb	r0, [r2, #5]
.L_080fa470:
	subs	r1, #1
	cmp	r1, #0
	bge.n	.L_080fa468
	pop	{pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	bl	sub_0810508c
	bl	.L_080fa458
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #184
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r3, #13
	strb	r3, [r2, #5]
	adds	r0, #16
	movs	r1, #1
	bl	sub_080f815c
	adds	r0, r5, #0
	adds	r0, #36
	movs	r1, #1
	bl	sub_080f815c
	adds	r0, r5, #0
	adds	r0, #240
	movs	r1, #1
	bl	sub_080f815c
	adds	r0, r5, #0
	adds	r0, #40
	movs	r1, #1
	bl	sub_080f815c
	adds	r0, r5, #0
	adds	r0, #44
	movs	r1, #1
	bl	sub_080f815c
	adds	r0, r5, #0
	adds	r0, #48
	movs	r1, #1
	bl	sub_080f815c
	adds	r0, r5, #0
	adds	r0, #52
	movs	r1, #1
	bl	sub_080f815c
	adds	r0, r5, #0
	adds	r0, #56
	movs	r1, #1
	bl	sub_080f815c
	adds	r0, r5, #0
	adds	r0, #60
	movs	r1, #1
	bl	sub_080f815c
	adds	r0, r5, #0
	adds	r0, #64
	movs	r1, #1
	bl	sub_080f815c
	adds	r0, r5, #0
	adds	r0, #68
	movs	r1, #1
	bl	sub_080f815c
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	mov	sl, r0
	mov	r6, sl
	movs	r1, #0
	adds	r6, #28
	ldr	r0, [r7, #48]
	mov	r8, r1
	ldrsb	r5, [r7, r6]
	bl	sub_08038268
	movs	r1, #139
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	adds	r2, r7, #2
	strb	r3, [r2, r6]
	movs	r2, #1
	negs	r2, r2
	cmp	r5, r2
	bne.n	.L_080fa54c
	mov	r3, r8
	strb	r3, [r7, r6]
	movs	r6, #0
	b.n	.L_080fa55a
.L_080fa54c:
	lsls	r6, r5, #1
	adds	r0, r6, r5
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	sub_080f8ab4
.L_080fa55a:
	movs	r5, #129
	lsls	r5, r5, #2
	adds	r3, r6, r5
	ldrh	r0, [r7, r3]
	bl	sub_08016ca4
	movs	r1, #226
	lsls	r1, r1, #1
	adds	r6, r7, r1
	adds	r1, r6, #0
	movs	r2, #0
	bl	sub_080fad88
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r7, r2
	adds	r5, r7, r5
	strb	r0, [r3, #0]
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	.L_080fa5a4
	mov	r1, sl
	lsls	r3, r1, #2
	adds	r3, #20
	mov	r8, r0
	ldr	r0, [r7, r3]
	bl	sub_080f8888
	movs	r0, #1
	bl	sub_08013560
	mov	r0, r8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
.L_080fa5a4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	sub	sp, #24
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	adds	r6, r0, #0
	str	r3, [sp, #20]
	lsls	r3, r3, #1
	movs	r1, #30
	ldrsb	r1, [r7, r1]
	movs	r2, #2
	str	r1, [sp, #16]
	ldrh	r0, [r3, r6]
	movs	r1, #0
	mov	sl, r2
	str	r1, [sp, #12]
	mov	r9, r1
	str	r1, [sp, #8]
	bl	sub_08016ca4
	adds	r5, r7, #0
	movs	r3, #10
	mov	r2, sl
	adds	r5, #36
	mov	fp, r0
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	adds	r0, r5, #0
	movs	r1, #13
	movs	r2, #3
	movs	r3, #17
	bl	sub_080f811c
	cmp	r0, #0
	beq.n	.L_080fa604
	ldr	r1, [r5, #0]
	adds	r0, r7, #0
	bl	sub_080fa3d4
.L_080fa604:
	movs	r3, #44
	adds	r3, r3, r7
	mov	r8, r3
	mov	r1, sl
	movs	r3, #4
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	mov	r0, r8
	movs	r1, #13
	movs	r2, #13
	movs	r3, #17
	bl	sub_080f811c
	cmp	r0, #0
	beq.n	.L_080fa640
	ldr	r1, [sp, #12]
	mov	r3, r8
	ldr	r2, [r3, #0]
	movs	r0, #2
	str	r1, [sp, #0]
	movs	r3, #0
	movs	r1, #0
	bl	sub_080380d0
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r3, r7, r2
	str	r0, [r3, #0]
	movs	r3, #13
	strb	r3, [r0, #5]
.L_080fa640:
	ldr	r5, [pc, #512]
	mov	r3, r8
	ldr	r1, [r3, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038080
	adds	r5, #1
	mov	r2, r8
	ldr	r1, [r2, #0]
	movs	r3, #8
	movs	r2, #0
	adds	r0, r5, #0
	bl	sub_08038080
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	b.n	.L_080fa7f4
.L_080fa668:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_080fa702
	ldr	r3, [sp, #20]
	movs	r1, #0
	lsls	r3, r3, #1
	ldrh	r0, [r3, r6]
	mov	r9, r1
	ldr	r5, [r7, #40]
	bl	sub_08016ca4
	ldr	r2, [sp, #8]
	mov	fp, r0
	cmp	r2, #0
	beq.n	.L_080fa6c0
	ldr	r3, [sp, #20]
	lsls	r3, r3, #1
	ldrh	r0, [r3, r6]
	bl	sub_08016ca4
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r1, r7, r3
	movs	r2, #0
	bl	sub_080fad88
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r7, r1
	strb	r0, [r3, #0]
	ldr	r3, [sp, #20]
	lsls	r3, r3, #1
	ldrh	r0, [r3, r6]
	bl	.L_080fa84c
	ldr	r3, [sp, #20]
	adds	r0, r5, #0
	lsls	r3, r3, #1
	ldrh	r1, [r3, r6]
	movs	r2, #0
	movs	r3, #8
	bl	sub_080f8170
	b.n	.L_080fa6dc
.L_080fa6c0:
	ldr	r3, [sp, #20]
	movs	r1, #0
	lsls	r3, r3, #1
	ldrh	r0, [r3, r6]
	bl	sub_080fae2c
	ldr	r3, [sp, #20]
	adds	r0, r5, #0
	lsls	r3, r3, #1
	ldrh	r1, [r3, r6]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080f8170
.L_080fa6dc:
	mov	r2, sl
	cmp	r2, #2
	bne.n	.L_080fa6f8
	ldr	r0, [sp, #20]
	cmp	r0, #0
	bge.n	.L_080fa6ea
	adds	r0, #3
.L_080fa6ea:
	asrs	r0, r0, #2
	lsls	r0, r0, #2
	bl	sub_08104ef8
	movs	r0, #1
	bl	sub_08013560
.L_080fa6f8:
	ldr	r0, [r7, #16]
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #16]
	bl	sub_08104d5c
.L_080fa702:
	ldr	r2, [sp, #20]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080fa70c
	adds	r3, r2, #3
.L_080fa70c:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	sub_080f8a44
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [pc, #288]
	movs	r3, #1
	ldr	r2, [r5, #4]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fa7a4
	ldr	r2, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fa780
	mov	r2, r9
	adds	r2, #4
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080fa74c
	mov	r3, r9
	adds	r3, #7
.L_080fa74c:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r9, r3
	mov	r0, fp
	mov	r1, r9
	adds	r0, #216
	bl	sub_080f8e08
	mov	r3, r9
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r9, r3
	ldr	r3, [sp, #20]
	movs	r1, #0
	lsls	r3, r3, #1
	ldrh	r0, [r3, r6]
	bl	sub_080fae2c
	movs	r0, #112
	bl	sub_081c0010
	b.n	.L_080fa7a4
.L_080fa780:
	ldr	r3, [sp, #20]
	lsls	r3, r3, #1
	ldrh	r0, [r3, r6]
	bl	sub_080fad1c
	cmp	r0, #0
	beq.n	.L_080fa79e
	movs	r0, #112
	bl	sub_081c0010
	ldr	r2, [sp, #20]
	lsls	r3, r2, #1
	ldrh	r3, [r3, r6]
	str	r3, [sp, #12]
	b.n	.L_080fa804
.L_080fa79e:
	movs	r0, #114
	bl	sub_081c0010
.L_080fa7a4:
	ldr	r2, [r5, #4]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080fa7bc
	movs	r0, #113
	bl	sub_081c0010
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #12]
	b.n	.L_080fa802
.L_080fa7bc:
	ldr	r3, [r5, #4]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fa7d0
	movs	r1, #1
	str	r1, [sp, #8]
	mov	sl, r1
	b.n	.L_080fa7f4
.L_080fa7d0:
	ldr	r3, [r5, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080fa7e8
	ldr	r2, [sp, #8]
	cmp	r2, #1
	bne.n	.L_080fa7e8
	movs	r3, #0
	movs	r1, #1
	str	r3, [sp, #8]
	mov	sl, r1
	b.n	.L_080fa7f4
.L_080fa7e8:
	add	r0, sp, #20
	ldr	r1, [sp, #16]
	movs	r2, #4
	bl	sub_08104c00
	mov	sl, r0
.L_080fa7f4:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080fa802
	b.n	.L_080fa668
.L_080fa802:
	ldr	r2, [sp, #20]
.L_080fa804:
	strb	r2, [r7, #28]
	ldr	r3, [sp, #20]
	movs	r1, #128
	lsls	r3, r3, #1
	ldrh	r3, [r3, r6]
	lsls	r1, r1, #2
	str	r3, [r7, #8]
	ldr	r3, [sp, #20]
	adds	r1, #22
	lsls	r3, r3, #1
	ldrh	r2, [r3, r6]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r2, #13
	subs	r1, #154
	strb	r2, [r3, #5]
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	strb	r2, [r3, #5]
	ldr	r0, [sp, #12]
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000010b8
	.2byte 0x1150
	.2byte 0x0300
.L_080fa84c:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	adds	r6, r0, #0
	bl	sub_08100700
	ldr	r0, [r5, #36]
	bl	sub_08038260
	ldr	r0, [r5, #36]
	adds	r1, r6, #0
	movs	r2, #0
	bl	sub_081004b8
	pop	{r5, r6, pc}
