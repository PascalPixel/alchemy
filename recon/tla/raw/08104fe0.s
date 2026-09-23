.syntax unified
	.thumb
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08020018, 0x08020018
	.set sub_08020048, 0x08020048
	.set sub_08104ef8, 0x08104ef8
	.global Func_08104fe0
	.thumb_func
Func_08104fe0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	mov	r8, r0
	movs	r0, #138
	lsls	r0, r0, #1
	mov	r9, r1
	mov	sl, r2
	movs	r1, #7
	movs	r2, #0
	adds	r3, r5, r0
.L_08105002:
	subs	r1, #1
	str	r2, [r3, #0]
	subs	r3, #4
	cmp	r1, #0
	bge.n	.L_08105002
	movs	r0, #0
	bl	sub_08104ef8
	movs	r2, #140
	movs	r3, #148
	lsls	r2, r2, #1
	adds	r6, r5, #0
	lsls	r3, r3, #1
	movs	r1, #0
	adds	r4, r5, r2
	movs	r7, #0
	adds	r6, #248
	adds	r0, r5, r3
.L_08105026:
	ldmia	r6!, {r3}
	lsls	r2, r1, #2
	mov	ip, r2
	cmp	r3, #0
	beq.n	.L_0810505c
	mov	r3, r8
	movs	r2, #12
	ldrsh	r3, [r3, r2]
	mov	r2, r8
	add	r3, r9
	adds	r3, r7, r3
	lsls	r3, r3, #3
	strh	r3, [r4, #0]
	movs	r3, #14
	ldrsh	r2, [r2, r3]
	mov	lr, r2
	mov	r3, lr
	add	r3, sl
	lsls	r3, r3, #3
	adds	r3, #16
	movs	r2, #156
	strh	r3, [r0, #0]
	lsls	r2, r2, #1
	movs	r3, #128
	add	r2, ip
	lsls	r3, r3, #9
	str	r3, [r5, r2]
.L_0810505c:
	adds	r1, #1
	adds	r4, #2
	adds	r7, #3
	adds	r0, #2
	cmp	r1, #3
	ble.n	.L_08105026
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #9
	adds	r2, r5, r3
	movs	r1, #144
	movs	r3, #1
	strb	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x4da9
	.2byte 0x0810
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r6, #7
	adds	r5, r3, #0
	adds	r5, #248
.L_0810509c:
	ldmia	r5!, {r0}
	cmp	r0, #0
	beq.n	.L_081050a6
	bl	sub_08020048
.L_081050a6:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_0810509c
	ldr	r0, [pc, #4]
	bl	sub_08014644
	pop	{r5, r6, pc}
	.2byte 0x4da9
	.2byte 0x0810
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r2, #10
	adds	r3, r3, r2
	strb	r0, [r3, #0]
	bx	lr
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
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #10
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	sub	sp, #32
	cmp	r3, #0
	beq.n	.L_08105198
	movs	r1, #140
	lsls	r1, r1, #2
	movs	r3, #8
	adds	r6, r7, r1
	subs	r1, #16
	movs	r2, #0
	add	r3, sp
	adds	r1, r7, r1
	mov	fp, r2
	mov	sl, r3
	movs	r2, #144
	movs	r3, #148
	str	r1, [sp, #4]
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	add	r5, sp, #16
	mov	r8, r2
	mov	r9, r3
.L_0810511a:
	ldr	r3, [sp, #4]
	ldmia	r3!, {r0}
	adds	r2, r3, #0
	str	r2, [sp, #4]
	cmp	r0, #0
	beq.n	.L_08105184
	mov	r1, r9
	ldrsh	r3, [r1, r7]
	movs	r2, #229
	lsls	r3, r3, #15
	lsls	r2, r2, #15
	subs	r2, r2, r3
	mov	r3, r8
	ldr	r1, [r3, r7]
	cmp	r1, #0
	bge.n	.L_08105144
	negs	r3, r1
	mov	r4, sl
	str	r3, [sp, #8]
	str	r3, [r4, #4]
	b.n	.L_0810515e
.L_08105144:
	movs	r3, #128
	lsls	r3, r3, #9
	subs	r3, r3, r1
	cmp	r3, #0
	bge.n	.L_08105150
	adds	r3, #3
.L_08105150:
	asrs	r3, r3, #2
	adds	r3, r1, r3
	mov	r4, sl
	mov	r1, r8
	str	r3, [sp, #8]
	str	r3, [r4, #4]
	str	r3, [r7, r1]
.L_0810515e:
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	str	r2, [r5, #4]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r1, #8
	ldrsh	r3, [r6, r1]
	adds	r1, r5, #0
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r5, #8]
	movs	r3, #0
	str	r3, [r5, #12]
	movs	r2, #250
	ldrh	r3, [r6, #32]
	str	r2, [sp, #0]
	adds	r2, r4, #0
	bl	sub_08020018
.L_08105184:
	movs	r1, #1
	movs	r2, #4
	add	fp, r1
	add	r8, r2
	movs	r3, #2
	mov	r2, fp
	adds	r6, #2
	add	r9, r3
	cmp	r2, #3
	ble.n	.L_0810511a
.L_08105198:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
