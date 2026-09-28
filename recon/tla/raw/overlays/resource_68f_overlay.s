.syntax unified
	.thumb
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_0200007c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200007c
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r4, r3
	adds	r3, r2, #0
	lsls	r4, r4, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_0200007c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	ldr	r3, [pc, #420]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl 0x0200b058
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02000100
	cmp	r7, #0
	beq.n	.L_02000100
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02000108
.L_02000100:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02000108:
	mov	r3, sl
	bl 0x0200af98
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02000116
	b.n	.L_02000262
.L_02000116:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200af80
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200af90
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200aff0
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	adds	r0, r6, #0
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x02008038
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #256]
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000262
	cmp	r7, #0
	beq.n	.L_02000262
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000198
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200b0b0
.L_02000198:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
.L_020001a0:
	cmp	r3, #0
	beq.n	.L_020001b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02008038
.L_020001b8:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020001cc
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020001cc:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000212
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020001fa
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200aee8
.L_020001ee:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_0200020c
.L_020001fa:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200aee8
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_0200020c:
	bl 0x0200aee8
	str	r0, [r6, #52]
.L_02000212:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200022e
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200af80
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200af90
.L_0200022e:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000240
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02000240:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000252
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02000252:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000262
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02000262:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200b334
	.4byte 0x02008081
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200af70
	movs	r0, #8
	movs	r1, #52
	bl 0x0200b0f0
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r5, #60
.L_0200029e:
	cmp	r5, #0
	beq.n	.L_020002b0
	movs	r0, #1
	bl 0x0200aef0
	ldr	r3, [r6, #40]
	subs	r5, #1
	cmp	r3, #0
	bne.n	.L_0200029e
.L_020002b0:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_020002c4
	movs	r0, #0
	b.n	.L_020002ea
.L_020002c4:
	cmp	r0, #2
	bhi.n	.L_020002d8
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_020002da
.L_020002d8:
	ldr	r4, [pc, #16]
.L_020002da:
	lsls	r3, r2, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
.L_020002ea:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, lr}
	adds	r5, r1, #0
	adds	r1, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	cmp	r4, #0
	bne.n	.L_02000304
	movs	r0, #0
	b.n	.L_02000330
.L_02000304:
	cmp	r0, #2
	bhi.n	.L_02000318
	lsls	r3, r0, #3
	subs	r3, r3, r0
	movs	r0, #156
	lsls	r3, r3, #3
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r4, [r4, r3]
	b.n	.L_0200031a
.L_02000318:
	ldr	r4, [pc, #24]
.L_0200031a:
	lsls	r3, r2, #7
	adds	r3, r5, r3
	lsls	r3, r3, #2
	adds	r4, r4, r3
	ldrb	r0, [r4, #2]
	ldrb	r3, [r4, #3]
	lsls	r0, r0, #8
	orrs	r0, r3
	asrs	r3, r1, #8
	strb	r3, [r4, #2]
	strb	r1, [r4, #3]
.L_02000330:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0201
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	ldr	r3, [r5, #8]
	movs	r0, #0
	asrs	r3, r3, #20
	mov	r9, r3
	ldr	r3, [r5, #16]
	mov	r1, r9
	asrs	r3, r3, #20
	mov	sl, r3
	mov	r2, sl
	bl 0x020082b4
	mov	r1, r9
	mov	r2, sl
	mov	r8, r0
	movs	r0, #2
	bl 0x020082b4
	movs	r2, #34
	adds	r2, r2, r5
	adds	r6, r0, #0
	mov	fp, r2
	ldrb	r0, [r2, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x0200afd8
	adds	r3, r5, #0
	adds	r3, #100
	asrs	r7, r0, #19
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_020003b0
	ldr	r3, [pc, #112]
	mov	r2, r8
	ands	r6, r3
	movs	r3, #129
	negs	r3, r3
	ands	r2, r3
	ldr	r3, [r5, #20]
	mov	r8, r2
	asrs	r3, r3, #19
	cmp	r3, r7
	beq.n	.L_020003a6
	subs	r7, #4
.L_020003a6:
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x02008038
	b.n	.L_020003ca
.L_020003b0:
	movs	r3, #255
	ands	r6, r3
	lsls	r3, r3, #8
	mov	r2, r8
	orrs	r6, r3
	movs	r3, #128
	orrs	r2, r3
	adds	r0, r5, #0
	movs	r1, #2
	mov	r8, r2
	adds	r7, #4
	bl 0x02008038
.L_020003ca:
	mov	r1, r9
	mov	r2, sl
	mov	r3, r8
	movs	r0, #0
	bl 0x020082f0
	mov	r1, r9
	mov	r2, sl
	adds	r3, r6, #0
	movs	r0, #2
	bl 0x020082f0
	mov	r3, r9
.L_020003e4:
	mov	r2, sl
	lsls	r0, r3, #20
	mov	r3, fp
	lsls	r1, r2, #20
	ldrb	r2, [r3, #0]
	adds	r3, r7, #0
	bl 0x0200b030
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x00ff
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_02000478
	adds	r7, r0, #0
.L_02000418:
	ldrh	r0, [r7, #0]
	bl 0x0200b058
	movs	r3, #4
	ldrsh	r2, [r7, r3]
	movs	r1, #0
	mov	r8, r2
	adds	r6, r0, #0
	movs	r3, #2
	ldrsh	r5, [r7, r3]
	bl 0x0200aff0
	mov	r2, r8
	lsls	r0, r2, #16
	lsrs	r0, r0, #16
	bl 0x0200af68
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r5, r5, r0
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl 0x0200af80
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r1, r6, #0
	adds	r1, #89
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r6, #0
	adds	r3, #100
	mov	r2, r8
	strh	r2, [r3, #0]
	adds	r0, r6, #0
	adds	r7, #6
	bl 0x02008338
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02000418
.L_02000478:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.global Func_02000480
	.thumb_func
Func_02000480:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb3a0
	.2byte 0x0200
	.global Func_02000488
	.thumb_func
Func_02000488:
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_020004a0
	ldr	r0, [pc, #20]
	b.n	.L_020004aa
.L_020004a0:
	ldr	r3, [pc, #20]
	movs	r0, #0
	cmp	r2, r3
	bne.n	.L_020004aa
	ldr	r0, [pc, #16]
.L_020004aa:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000d3
	.4byte 0x0200b3d0
	.4byte 0x000000d4
	.2byte 0xb3f0
	.2byte 0x0200
	.global Func_020004c0
	.thumb_func
Func_020004c0:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb410
	.2byte 0x0200
	.global Func_020004c8
	.thumb_func
Func_020004c8:
	push	{lr}
	ldr	r3, [pc, #72]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	beq.n	.L_0200050e
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_020004e6
	ldr	r0, [pc, #60]
	b.n	.L_02000510
.L_020004e6:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_020004f0
	ldr	r0, [pc, #56]
	b.n	.L_02000510
.L_020004f0:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020004fa
	ldr	r0, [pc, #56]
	b.n	.L_02000510
.L_020004fa:
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000504
	ldr	r0, [pc, #52]
	b.n	.L_02000510
.L_02000504:
	ldr	r3, [pc, #52]
	cmp	r2, r3
	bne.n	.L_0200050e
	ldr	r0, [pc, #52]
	b.n	.L_02000510
.L_0200050e:
	ldr	r0, [pc, #52]
.L_02000510:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000cf
	.4byte 0x000000d0
	.4byte 0x0200b648
	.4byte 0x000000d1
	.4byte 0x0200b660
	.4byte 0x000000d2
	.4byte 0x0200b690
	.4byte 0x000000d3
	.4byte 0x0200b708
	.4byte 0x000000d4
	.4byte 0x0200b7c8
	.2byte 0xb618
	.2byte 0x0200
	push	{lr}
	sub	sp, #8
	bl 0x0200b048
	movs	r3, #24
	movs	r2, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #19
	movs	r1, #55
	movs	r2, #7
	movs	r3, #8
	bl 0x0200afe8
	movs	r3, #7
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #102
	movs	r2, #24
	movs	r3, #110
	movs	r0, #24
	bl 0x0200afc0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #66
	bl 0x0200af70
	bl 0x0200b050
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #8
	bl 0x0200b048
	movs	r3, #32
	movs	r2, #46
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #37
	movs	r1, #55
	movs	r2, #7
	movs	r3, #8
	bl 0x0200afe8
	movs	r3, #7
	movs	r2, #8
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #102
	movs	r2, #32
	movs	r3, #110
	movs	r0, #32
	bl 0x0200afc0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #67
	bl 0x0200af70
	bl 0x0200b050
	add	sp, #8
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	sub	sp, #8
	bl 0x0200b048
	movs	r0, #0
	bl 0x0200b148
	movs	r2, #170
	lsls	r2, r2, #1
	adds	r7, r5, r2
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	movs	r6, #37
	cmp	r3, #3
	bne.n	.L_020005fa
	movs	r6, #25
.L_020005fa:
	movs	r0, #158
	bl 0x0200b1a8
	movs	r3, #1
	str	r3, [sp, #0]
	mov	r8, r3
	adds	r2, r6, #0
	movs	r5, #2
	movs	r1, #38
	movs	r3, #49
	movs	r0, #30
.L_02000610:
	str	r5, [sp, #4]
	bl 0x0200afc0
	movs	r0, #10
	bl 0x0200b040
.L_0200061c:
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r1, #38
	adds	r2, r6, #0
	movs	r3, #49
	movs	r0, #32
	str	r5, [sp, #4]
	bl 0x0200afc0
	movs	r0, #10
	bl 0x0200b040
	ldr	r6, [pc, #152]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	ldr	r0, [r6, #0]
	bl 0x0200b060
	ldr	r0, [r6, #0]
	bl 0x0200b058
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	ldr	r0, [r6, #0]
	bl 0x0200b058
	ldr	r5, [r0, #8]
	mov	r2, r8
	ldr	r0, [r6, #0]
	asrs	r5, r5, #19
	orrs	r5, r2
	bl 0x0200b058
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	lsls	r5, r5, #3
	mov	r3, r8
	orrs	r2, r3
	adds	r1, r5, #0
	ldr	r0, [r6, #0]
	bl 0x0200b078
	movs	r0, #123
	bl 0x0200b1a8
	ldr	r0, [r6, #0]
	bl 0x0200b058
	ldr	r5, [r0, #8]
	ldr	r0, [r6, #0]
	bl 0x0200b058
	movs	r3, #18
	ldrsh	r2, [r0, r3]
	asrs	r5, r5, #19
	mov	r3, r8
	orrs	r2, r3
	lsls	r5, r5, #3
	subs	r2, #16
	adds	r1, r5, #0
	ldr	r0, [r6, #0]
	bl 0x0200b070
	movs	r0, #5
	bl 0x0200b040
	bl 0x0200b128
	bl 0x0200b130
	ldr	r0, [r6, #0]
	bl 0x0200b080
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	bl 0x0200b0e8
	bl 0x0200b050
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x6c426883
	.4byte 0x189b6c81
	.4byte 0x68c36083
	.4byte 0x185b6cc2
	.4byte 0x690360c3
	.4byte 0x6103189b
	.4byte 0x69836b02
	.4byte 0x6183189b
	.4byte 0x69c36b42
	.4byte 0x61c3189b
	.4byte 0x18c94b01
	.4byte 0x47706481
	.2byte 0xb334
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #60
	add	r7, sp, #20
	movs	r3, #1
	str	r1, [sp, #16]
	str	r3, [r7, #0]
	movs	r3, #24
	adds	r3, #255
	strh	r3, [r7, #24]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r7, #20]
	str	r3, [r7, #16]
	ldr	r3, [pc, #88]
	str	r2, [r7, #12]
	str	r2, [r7, #8]
	str	r3, [r7, #36]
	ldr	r3, [sp, #16]
	movs	r2, #0
	mov	fp, r0
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L_020007cc
.L_02000744:
	bl 0x0200af08
	ldr	r3, [pc, #60]
	ands	r0, r3
	lsls	r0, r0, #12
	strh	r0, [r7, #32]
	bl 0x0200af08
	mov	r8, r0
	movs	r2, #15
	mov	r3, r8
	ands	r3, r2
	mov	sl, r2
	mov	r8, r3
	subs	r2, #23
	add	r8, r2
	mov	r3, r8
	lsls	r3, r3, #14
	mov	r8, r3
	bl 0x0200af08
	mov	r2, sl
	adds	r5, r0, #0
	ands	r5, r2
	bl 0x0200af08
	mov	r2, fp
	ldr	r6, [r2, #0]
	movs	r3, #31
	ands	r3, r0
	lsls	r3, r3, #16
	subs	r6, r6, r3
	b.n	.L_02000790
	.2byte 0x0000
	.4byte 0x0000000f
	.2byte 0x86d5
	.2byte 0x0200
.L_02000790:
	movs	r3, #240
	lsls	r3, r3, #12
	adds	r6, r6, r3
	bl 0x0200af08
	mov	r3, fp
	mov	r2, sl
	ldr	r1, [r3, #4]
	ands	r0, r2
.L_020007a2:
	ldr	r2, [r3, #8]
	movs	r3, #0
	str	r3, [sp, #4]
	movs	r3, #175
	lsls	r0, r0, #16
	lsls	r3, r3, #17
	adds	r5, #8
	adds	r1, r1, r0
	str	r3, [sp, #8]
	lsls	r5, r5, #14
	mov	r3, r8
	adds	r0, r6, #0
	str	r5, [sp, #0]
	str	r7, [sp, #12]
	bl 0x020080b8
	ldr	r3, [sp, #16]
	movs	r2, #1
	add	r9, r2
	cmp	r9, r3
	bne.n	.L_02000744
.L_020007cc:
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{lr}
	sub	sp, #20
	cmp	r0, #2
	beq.n	.L_020007ee
	cmp	r0, #2
	ble.n	.L_02000838
	cmp	r0, #3
	beq.n	.L_0200080c
	b.n	.L_02000838
.L_020007ee:
	ldr	r3, [pc, #76]
	add	r0, sp, #8
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	movs	r2, #204
	movs	r3, #188
	lsls	r3, r3, #17
	lsls	r2, r2, #8
	str	r3, [r0, #8]
	adds	r2, #204
	movs	r1, #1
	bl 0x02008708
	b.n	.L_02000838
.L_0200080c:
	movs	r3, #4
	str	r0, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #31
	movs	r1, #85
	movs	r2, #20
	movs	r3, #85
	bl 0x0200afc0
	ldr	r3, [pc, #28]
	add	r0, sp, #8
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	movs	r3, #188
	lsls	r3, r3, #17
	movs	r2, #192
	str	r3, [r0, #8]
	lsls	r2, r2, #9
	movs	r1, #8
	bl 0x02008708
.L_02000838:
	add	sp, #20
	pop	{pc}
	.2byte 0x0000
	.2byte 0x015b
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	movs	r6, #7
	ldr	r7, [r3, #0]
	sub	sp, #56
	ands	r7, r6
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_0200089a
	bl 0x0200af08
	movs	r5, #15
	ands	r5, r0
	bl 0x0200af08
	movs	r3, #209
	lsls	r3, r3, #1
	ands	r0, r6
	adds	r3, #255
	add	r6, sp, #16
	strh	r3, [r6, #24]
	mov	r3, r8
	ldr	r4, [r3, #8]
	ldr	r1, [r3, #12]
	ldr	r2, [r3, #16]
	movs	r3, #128
.L_02000878:
	lsls	r3, r3, #8
	subs	r5, #8
	lsls	r5, r5, #16
	subs	r0, #8
	str	r3, [sp, #0]
	movs	r3, #128
	lsls	r0, r0, #16
	lsls	r3, r3, #13
	adds	r4, r4, r5
	adds	r1, r1, r0
	str	r3, [sp, #8]
	adds	r0, r4, #0
	movs	r3, #0
	str	r7, [sp, #4]
	str	r6, [sp, #12]
	bl 0x020080b8
.L_0200089a:
	add	sp, #56
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, lr}
	movs	r0, #8
	sub	sp, #8
	bl 0x0200b058
	adds	r5, r0, #0
	bl 0x0200b048
	movs	r0, #0
	bl 0x0200b148
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #68
	bl 0x0200af70
	movs	r3, #20
	movs	r2, #22
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #22
	movs	r0, #11
	movs	r2, #3
	movs	r3, #2
	bl 0x0200afe8
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r2, r5, #0
	str	r3, [r5, #72]
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	bl 0x02008298
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b088
	movs	r1, #172
	movs	r2, #186
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #68
	bl 0x0200b088
	movs	r0, #68
	bl 0x0200b058
	ldr	r3, [pc, #12]
	str	r3, [r0, #108]
	bl 0x0200b050
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x8841
	.2byte 0x0200
	push	{lr}
	bl 0x0200b048
	movs	r0, #0
	bl 0x0200b148
	ldr	r0, [pc, #40]
	movs	r1, #1
	bl 0x0200b038
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #73
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_02000952
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #173
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #1
.L_02000950:
	strh	r3, [r2, #0]
.L_02000952:
	bl 0x0200b050
	pop	{pc}
	.2byte 0x1a95
	.2byte 0x0000
	push	{lr}
	bl 0x0200989c
	pop	{pc}
	push	{r5, lr}
	movs	r0, #8
	bl 0x0200b058
	adds	r5, r0, #0
	bl 0x0200b048
	movs	r0, #0
	bl 0x0200b148
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl 0x0200b110
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200b108
	movs	r0, #2
	bl 0x0200b118
	movs	r0, #132
	bl 0x0200b1a8
	movs	r0, #160
	movs	r1, #224
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200b000
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r1, r1
	negs	r0, r0
	bl 0x0200b000
	movs	r0, #2
	bl 0x0200b040
	movs	r0, #128
	movs	r1, #0
	lsls	r0, r0, #9
	bl 0x0200b108
	movs	r0, #2
	bl 0x0200b118
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #90
	strb	r3, [r2, #0]
	movs	r1, #212
	movs	r2, #134
	str	r3, [r5, #108]
	lsls	r2, r2, #2
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200b068
	ldr	r1, [pc, #84]
	adds	r0, r5, #0
	bl 0x0200af90
	movs	r0, #8
	bl 0x0200b080
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200af70
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r5, #200
	ldr	r1, [r3, #0]
	lsls	r5, r5, #6
	movs	r0, #8
	bl 0x0200b158
	adds	r3, r5, #0
	movs	r1, #21
	movs	r2, #33
	movs	r0, #0
	bl 0x020082f0
	adds	r3, r5, #0
	movs	r1, #22
	movs	r2, #33
	movs	r0, #0
	bl 0x020082f0
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r1, #26
	movs	r2, #33
	movs	r0, #0
	bl 0x020082f0
	bl 0x0200b050
	pop	{r5, pc}
	.4byte 0x0200b370
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	adds	r6, r1, #0
	mov	sl, r0
	movs	r1, #26
	ldrsh	r0, [r3, r1]
	sub	sp, #56
	mov	r9, r3
	bl 0x0200b058
	adds	r7, r0, #0
	cmp	r6, #7
	bgt.n	.L_02000adc
	movs	r3, #24
	add	r5, sp, #16
	adds	r3, #255
	strh	r3, [r5, #24]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #12]
	str	r3, [r5, #8]
	movs	r3, #128
	movs	r2, #0
	lsls	r3, r3, #8
	mov	r8, r2
	str	r2, [r5, #0]
	str	r3, [r5, #20]
	str	r3, [r5, #16]
	bl 0x0200af08
	ldr	r3, [pc, #60]
	ands	r0, r3
	lsls	r0, r0, #12
	strh	r0, [r5, #32]
	bl 0x0200af08
	ldr	r1, [r7, #12]
	lsls	r2, r6, #1
	adds	r2, r2, r6
	lsls	r2, r2, #16
	movs	r4, #128
	subs	r1, r1, r2
	lsls	r4, r4, #13
	movs	r3, #15
	adds	r1, r1, r4
	ands	r3, r0
	mov	r4, r8
	ldr	r0, [r7, #8]
	ldr	r2, [r7, #16]
	subs	r3, #8
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	movs	r4, #180
	lsls	r3, r3, #13
	lsls	r4, r4, #15
	str	r4, [sp, #8]
	str	r5, [sp, #12]
	bl 0x020080b8
	b.n	.L_02000ad4
	.2byte 0x0000
	.2byte 0x000f
	.2byte 0x0000
.L_02000ad4:
	ldr	r3, [r7, #28]
	ldr	r1, [pc, #88]
	adds	r3, r3, r1
	str	r3, [r7, #28]
.L_02000adc:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L_02000b22
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #28]
	adds	r3, r7, #0
	adds	r3, #100
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	bl 0x0200af70
	mov	r2, r9
	movs	r1, #26
	ldrsh	r0, [r2, r1]
	bl 0x0200b058
	bl 0x02008338
	ldr	r2, [r7, #8]
	ldr	r3, [r7, #16]
	asrs	r2, r2, #20
	mov	r4, sl
	asrs	r3, r3, #20
	adds	r2, #64
	movs	r0, #67
	movs	r1, #23
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	bl 0x0200afc0
	movs	r3, #0
	str	r3, [r7, #16]
	str	r3, [r7, #12]
	str	r3, [r7, #8]
.L_02000b22:
	add	sp, #56
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb500
	bl 0x02008a48
	pop	{pc}
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	ldr	r5, [pc, #180]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200b058
	adds	r6, r0, #0
	bl 0x0200b048
	movs	r0, #0
	bl 0x0200b148
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	negs	r1, r1
	negs	r0, r0
	bl 0x0200b0e0
	ldr	r0, [r5, #0]
	bl 0x0200b058
	movs	r1, #0
	bl 0x0200aff0
	movs	r3, #0
	mov	r8, r3
	movs	r3, #128
	lsls	r3, r3, #7
.L_02000b82:
	strh	r3, [r6, #6]
	movs	r0, #10
.L_02000b86:
	bl 0x0200b040
	ldr	r0, [r5, #0]
	movs	r1, #22
	bl 0x0200b098
	movs	r0, #30
	bl 0x0200b040
	movs	r1, #129
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200b0c8
	ldr	r0, [r5, #0]
	movs	r1, #2
	bl 0x0200b0a8
	movs	r0, #20
	bl 0x0200b040
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r6, #6]
	movs	r1, #5
	ldr	r0, [r5, #0]
	bl 0x0200b098
	ldr	r0, [r5, #0]
	movs	r1, #24
	bl 0x0200b0a0
	movs	r0, #40
	bl 0x0200b040
	movs	r0, #152
	movs	r1, #228
	movs	r3, #12
	lsls	r1, r1, #18
	movs	r2, #0
	negs	r3, r3
	lsls	r0, r0, #17
	bl 0x0200b030
	ldr	r0, [r5, #0]
	bl 0x0200b058
	mov	r3, r8
	str	r3, [r0, #68]
	movs	r0, #11
	bl 0x0200b0e8
	bl 0x0200b050
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	ldr	r3, [pc, #56]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #48]
	cmp	r2, r3
	bne.n	.L_02000c4c
	ldr	r0, [pc, #48]
	bl 0x0200b170
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #188
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #20]
	ldr	r2, [r0, #8]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	lsls	r2, r2, #8
	ands	r3, r1
	orrs	r2, r3
	adds	r0, #100
	strh	r2, [r0, #0]
	b.n	.L_02000c58
	.4byte 0x000000ff
	.4byte 0x02000240
	.4byte 0x000000d3
	.2byte 0xb264
	.2byte 0x0200
.L_02000c4c:
	ldr	r3, [pc, #12]
	cmp	r2, r3
	bne.n	.L_02000c58
	ldr	r0, [pc, #12]
	bl 0x0200b170
.L_02000c58:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x000000d4
	.2byte 0xb26e
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #188
	lsls	r1, r1, #16
	adds	r3, r3, r2
	asrs	r1, r1, #16
	ldr	r5, [r3, #0]
	mov	r8, r1
	bl 0x0200b048
	movs	r0, #0
	bl 0x0200b148
	adds	r3, r5, #0
	adds	r3, #100
	ldrh	r1, [r3, #0]
	ldr	r2, [r5, #8]
	lsls	r3, r1, #16
	asrs	r2, r2, #20
	asrs	r3, r3, #24
	subs	r7, r2, r3
	ldr	r2, [r5, #16]
	movs	r3, #255
	ands	r3, r1
	asrs	r2, r2, #20
	subs	r6, r2, r3
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #48]
	b.n	.L_02000cd8
.L_02000cac:
	ldr	r3, [r5, #8]
	ldr	r0, [r5, #16]
	lsls	r1, r7, #20
	adds	r1, r1, r3
	lsls	r3, r6, #20
	ldr	r2, [r5, #12]
	adds	r3, r3, r0
	adds	r0, r5, #0
	bl 0x0200afb0
	mov	r3, r8
	lsls	r0, r3, #16
	lsrs	r0, r0, #16
	movs	r1, #1
	bl 0x0200b0d8
	adds	r0, r5, #0
	bl 0x0200afb8
	movs	r0, #1
	bl 0x0200aef0
.L_02000cd8:
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	adds	r1, r1, r7
	adds	r2, r2, r6
	movs	r0, #1
	bl 0x020082b4
	asrs	r0, r0, #8
	cmp	r0, #50
	bne.n	.L_02000d08
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	adds	r1, r1, r7
	adds	r2, r2, r6
	movs	r0, #2
	bl 0x020082b4
	asrs	r0, r0, #8
	cmp	r0, #255
	bne.n	.L_02000cac
.L_02000d08:
	ldr	r3, [r5, #8]
	movs	r2, #1
	asrs	r3, r3, #19
.L_02000d0e:
	orrs	r3, r2
	lsls	r3, r3, #19
	str	r3, [r5, #8]
	ldr	r3, [r5, #16]
	asrs	r3, r3, #19
	orrs	r3, r2
.L_02000d1a:
	lsls	r3, r3, #19
	str	r3, [r5, #16]
	bl 0x0200b178
	bl 0x0200b050
	pop	{r3}
.L_02000d28:
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #188
	adds	r3, r3, r1
	ldr	r6, [r3, #0]
	bl 0x0200b048
	movs	r0, #0
	bl 0x0200b148
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	movs	r0, #2
	bl 0x0200afd8
	ldr	r3, [r6, #12]
	cmp	r0, r3
	beq.n	.L_02000d88
	movs	r2, #34
	adds	r2, r2, r6
	movs	r3, #2
	adds	r7, r6, #0
	strb	r3, [r2, #0]
	adds	r7, #85
	movs	r3, #3
	strb	r3, [r7, #0]
	adds	r0, r6, #0
	mov	r8, r2
	bl 0x02008298
	movs	r0, #188
	bl 0x0200b1a8
	adds	r0, r6, #0
	bl 0x02008298
	movs	r5, #0
	mov	r3, r8
	strb	r5, [r7, #0]
	strb	r5, [r3, #0]
.L_02000d88:
	bl 0x0200b178
	ldr	r3, [pc, #76]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_02000dd0
	ldr	r3, [r6, #8]
	asrs	r3, r3, #19
	cmp	r3, #27
	bne.n	.L_02000db0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #71
	bl 0x0200af70
.L_02000db0:
	ldr	r3, [r6, #8]
	asrs	r3, r3, #19
	cmp	r3, #25
	bne.n	.L_02000dd0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #71
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_02000dd0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #70
	bl 0x0200af70
.L_02000dd0:
	bl 0x0200b050
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x00d3
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	bl 0x0200afc8
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200af78
	ldr	r5, [pc, #124]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #6
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02000e70
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #39
	movs	r1, #100
	movs	r2, #53
	movs	r3, #100
	bl 0x0200afc0
	movs	r3, #53
	movs	r2, #37
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #43
	movs	r1, #41
	movs	r2, #3
	movs	r3, #3
	bl 0x0200afe8
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
.L_02000e38:
	ldr	r0, [r5, #0]
	bl 0x0200b058
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #38
	bne.n	.L_02000e70
	ldr	r0, [r5, #0]
	bl 0x0200b058
	movs	r2, #6
	ldrsh	r3, [r0, r2]
	ldr	r0, [r5, #0]
	cmp	r3, #0
	bge.n	.L_02000e64
	movs	r1, #218
	movs	r2, #150
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200b088
	b.n	.L_02000e70
.L_02000e64:
	movs	r1, #218
	movs	r2, #158
	lsls	r1, r1, #18
	lsls	r2, r2, #18
	bl 0x0200b088
.L_02000e70:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	bl 0x0200afd0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #18
	bl 0x0200af70
	ldr	r5, [pc, #112]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	movs	r2, #128
	subs	r3, #6
	lsls	r3, r3, #16
	lsls	r2, r2, #9
	cmp	r3, r2
	bhi.n	.L_02000ef8
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #43
	movs	r1, #100
	movs	r2, #53
	movs	r3, #100
	bl 0x0200afc0
	movs	r3, #53
	movs	r2, #37
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #43
	movs	r1, #37
	movs	r2, #3
	movs	r3, #3
	bl 0x0200afe8
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200b058
	ldr	r3, [r0, #16]
	asrs	r3, r3, #20
	cmp	r3, #38
	ble.n	.L_02000eea
	movs	r3, #248
	lsls	r3, r3, #5
	movs	r0, #0
	movs	r1, #54
	movs	r2, #37
	bl 0x020082f0
	b.n	.L_02000ef8
.L_02000eea:
	movs	r3, #248
	lsls	r3, r3, #5
	movs	r0, #0
	movs	r1, #54
	movs	r2, #39
	bl 0x020082f0
.L_02000ef8:
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	bl 0x0200b148
	pop	{pc}
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #20
.L_02000f10:
	cmp	r0, #2
	beq.n	.L_02000f6a
	cmp	r0, #2
	bgt.n	.L_02000f1e
	cmp	r0, #1
	beq.n	.L_02000f24
	b.n	.L_02000fa6
.L_02000f1e:
	cmp	r0, #3
	beq.n	.L_02000f8a
	b.n	.L_02000fa6
.L_02000f24:
	movs	r3, #4
	str	r3, [sp, #4]
	movs	r5, #3
	movs	r0, #18
	movs	r1, #97
	movs	r2, #6
	movs	r3, #97
	str	r5, [sp, #0]
	bl 0x0200afc0
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r0, #82
.L_02000f3e:
	movs	r1, #34
	movs	r2, #70
	movs	r3, #34
	str	r5, [sp, #0]
	bl 0x0200afc0
	movs	r3, #7
	movs	r2, #35
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #7
	movs	r1, #36
	movs	r2, #1
	movs	r3, #1
	bl 0x0200afe8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #75
	bl 0x0200af70
	b.n	.L_02000fa6
.L_02000f6a:
	movs	r3, #240
	add	r0, sp, #8
	lsls	r3, r3, #15
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	movs	r2, #204
	movs	r3, #142
	lsls	r3, r3, #18
	lsls	r2, r2, #8
	str	r3, [r0, #8]
	adds	r2, #204
	movs	r1, #1
	bl 0x02008708
	b.n	.L_02000fa6
.L_02000f8a:
	movs	r3, #240
	add	r0, sp, #8
	lsls	r3, r3, #15
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	movs	r3, #142
	lsls	r3, r3, #18
	movs	r2, #192
.L_02000f9c:
	str	r3, [r0, #8]
	lsls	r2, r2, #9
	movs	r1, #8
	bl 0x02008708
.L_02000fa6:
	add	sp, #20
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #0
	bl 0x0200b148
.L_02000fb4:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r6, r1, #0
	sub	sp, #8
	adds	r5, r0, #0
	cmp	r6, #0
	bne.n	.L_02000fce
	bl 0x0200ae00
	ldr	r0, [pc, #116]
	bl 0x0200af00
.L_02000fce:
	cmp	r5, #1
	bne.n	.L_02001002
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #13
	movs	r1, #26
	movs	r2, #13
	movs	r3, #16
	bl 0x0200afc0
	movs	r3, #13
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #13
	movs	r1, #26
	movs	r2, #3
	movs	r3, #3
	bl 0x0200afe8
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #72
	bl 0x0200af70
.L_02001002:
	cmp	r5, #2
	bne.n	.L_02001018
	movs	r0, #232
	movs	r1, #128
.L_0200100a:
	movs	r2, #140
	lsls	r0, r0, #16
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	movs	r3, #2
	bl 0x0200ad44
.L_02001018:
	cmp	r5, #3
	bne.n	.L_0200102e
	movs	r0, #232
	movs	r1, #128
	movs	r2, #140
	lsls	r0, r0, #16
	lsls	r1, r1, #13
	lsls	r2, r2, #17
	movs	r3, #30
	bl 0x0200ad44
.L_0200102e:
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	cmp	r6, r3
	bne.n	.L_0200103c
	bl 0x0200aebc
.L_0200103c:
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x937d
	.2byte 0x0200
	.global Func_02001044
	.thumb_func
Func_02001044:
	push	{lr}
	ldr	r3, [pc, #80]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #72]
	cmp	r2, r3
	bne.n	.L_0200105c
	ldr	r0, [pc, #68]
	b.n	.L_02001096
.L_0200105c:
	ldr	r3, [pc, #68]
	cmp	r2, r3
	beq.n	.L_02001094
	ldr	r3, [pc, #68]
	cmp	r2, r3
	bne.n	.L_0200106c
	ldr	r0, [pc, #64]
	b.n	.L_02001096
.L_0200106c:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02001076
	ldr	r0, [pc, #64]
	b.n	.L_02001096
.L_02001076:
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_02001080
	ldr	r0, [pc, #60]
	b.n	.L_02001096
.L_02001080:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_0200108a
	ldr	r0, [pc, #60]
	b.n	.L_02001096
.L_0200108a:
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_02001094
	ldr	r0, [pc, #56]
	b.n	.L_02001096
.L_02001094:
	ldr	r0, [pc, #56]
.L_02001096:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000ce
	.4byte 0x0200b7f8
	.4byte 0x000000cf
	.4byte 0x000000d0
	.4byte 0x0200b93c
	.4byte 0x000000d1
	.4byte 0x0200b9e4
	.4byte 0x000000d2
	.4byte 0x0200ba5c
	.4byte 0x000000d3
	.4byte 0x0200bb28
	.4byte 0x000000d4
	.4byte 0x0200bc90
	.2byte 0xb81c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200b058
	adds	r6, r0, #0
	ldr	r3, [r6, #80]
	ldr	r2, [r6, #16]
	mov	r8, r3
	ldr	r3, [r5, #76]
	ldr	r7, [r5, #80]
	cmp	r2, r3
	bgt.n	.L_02001124
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #12
	movs	r3, #0
	ldrh	r2, [r1, #0]
	str	r3, [r5, #16]
	str	r3, [r5, #8]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #252
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #8]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_02001182
	.4byte 0x00000001
	.2byte 0x0240
	.2byte 0x0200
.L_02001124:
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #12
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #252
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #28]
	ldrh	r3, [r1, #0]
	movs	r0, #128
	orrs	r3, r2
	lsls	r0, r0, #2
	strh	r3, [r1, #0]
	adds	r0, #18
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_02001158
	movs	r3, #0
	str	r3, [r5, #16]
	str	r3, [r5, #8]
	b.n	.L_02001182
	.2byte 0x0002
	.2byte 0x0000
.L_02001158:
	ldr	r3, [r6, #8]
	ldr	r1, [r7, #40]
	str	r3, [r5, #8]
	ldr	r3, [r6, #12]
	str	r3, [r5, #12]
	ldr	r3, [r5, #76]
	ldr	r2, [r6, #16]
	subs	r2, r2, r3
	subs	r3, r3, r2
	str	r3, [r5, #16]
	ldrh	r3, [r6, #6]
	mvns	r3, r3
	strh	r3, [r5, #6]
	mov	r3, r8
	ldr	r2, [r3, #40]
	ldr	r3, [r2, #16]
	str	r3, [r1, #16]
	ldrh	r3, [r2, #2]
	strh	r3, [r1, #2]
	ldrb	r3, [r2, #20]
	strb	r3, [r1, #20]
.L_02001182:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200aee8
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_020011b8
	adds	r3, #15
.L_020011b8:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
.L_020011d6:
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
.L_020011e6:
	push	{r6, r7}
	ldr	r3, [pc, #380]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200b058
	adds	r7, r0, #0
	bl 0x0200b048
	movs	r0, #0
	bl 0x0200b148
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200b0e0
	bl 0x0200afa8
	movs	r0, #1
	bl 0x0200aef0
	movs	r3, #130
	lsls	r3, r3, #16
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r5, r7, #0
	str	r3, [r7, #72]
	adds	r5, #85
	movs	r3, #0
	str	r3, [r7, #68]
	strb	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r4, #214
	lsls	r4, r4, #1
	movs	r2, #128
	adds	r3, r3, r4
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	bl 0x0200b120
	bl 0x0200b130
	movs	r0, #204
	bl 0x0200b1a8
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200b040
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #260]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_0200127a:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200af20
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200af18
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200af08
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #192]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200af08
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #180]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r4
	ldr	r4, [r6, #4]
	str	r5, [r6, #8]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #160]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x020080b8
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_0200127a
	movs	r0, #188
	bl 0x0200b1a8
	ldr	r5, [pc, #116]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200b0c8
	ldr	r0, [r5, #0]
	movs	r1, #22
	bl 0x0200b098
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200b000
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200b000
	bl 0x0200b008
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200b0c8
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #88
	str	r2, [r3, #0]
	bl 0x0200b050
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02009189
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b058
	ldr	r3, [r0, #12]
	asrs	r3, r3, #19
	cmp	r3, #0
	beq.n	.L_020013a2
	movs	r0, #0
	movs	r1, #14
.L_02001398:
	movs	r2, #17
	movs	r3, #0
	bl 0x020082f0
	b.n	.L_020013b0
.L_020013a2:
	movs	r3, #200
	lsls	r3, r3, #6
	movs	r0, #0
	movs	r1, #14
	movs	r2, #17
	bl 0x020082f0
.L_020013b0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.global Func_020013b8
	.thumb_func
Func_020013b8:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #214
	lsls	r0, r0, #1
	movs	r2, #129
	adds	r3, r3, r0
	lsls	r2, r2, #2
	ldr	r1, [pc, #880]
	str	r2, [r3, #0]
	subs	r2, #36
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #872]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_020013e0
	b.n	.L_020017c4
.L_020013e0:
	ldr	r3, [pc, #864]
	cmp	r2, r3
	beq.n	.L_020013e8
	b.n	.L_0200150a
.L_020013e8:
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #1
	bge.n	.L_020013f8
	b.n	.L_020017c4
.L_020013f8:
	cmp	r3, #4
	ble.n	.L_02001402
	cmp	r3, #11
	beq.n	.L_0200142a
	b.n	.L_020017c4
.L_02001402:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #66
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_02001414
	bl 0x02008548
.L_02001414:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #67
	bl 0x0200af68
	cmp	r0, #0
	bne.n	.L_02001424
	b.n	.L_020017c4
.L_02001424:
	bl 0x0200858c
	b.n	.L_020017c4
.L_0200142a:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #69
	bl 0x0200af68
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020014c6
	movs	r0, #8
	bl 0x0200b058
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200b058
	movs	r1, #0
	bl 0x0200aff0
	adds	r3, r5, #0
	adds	r3, #89
	strb	r6, [r3, #0]
	subs	r3, #4
	strb	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #12
	movs	r1, #186
	adds	r0, r5, #0
	str	r3, [r5, #20]
	str	r3, [r5, #12]
	adds	r1, #255
	bl 0x0200b020
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #68
	bl 0x0200af68
	cmp	r0, #0
	bne.n	.L_0200147a
	b.n	.L_020017c4
.L_0200147a:
	movs	r3, #3
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #31
	movs	r1, #85
	movs	r2, #20
	movs	r3, #85
	bl 0x0200afc0
	movs	r3, #20
	movs	r2, #22
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #2
	movs	r0, #11
	movs	r1, #22
	movs	r2, #3
	bl 0x0200afe8
	movs	r1, #172
	movs	r2, #186
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	movs	r0, #68
	bl 0x0200b088
	movs	r0, #68
	bl 0x0200b058
	ldr	r3, [pc, #656]
	movs	r1, #0
	str	r3, [r0, #108]
	movs	r2, #0
	movs	r0, #8
	bl 0x0200b088
	b.n	.L_020017c4
.L_020014c6:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #68
	bl 0x0200af68
	cmp	r0, #0
	bne.n	.L_020014d6
	b.n	.L_020017c4
.L_020014d6:
	movs	r3, #3
	movs	r2, #4
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #31
	movs	r1, #85
	movs	r2, #20
	movs	r3, #85
	bl 0x0200afc0
	movs	r3, #20
	movs	r2, #22
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #11
	movs	r1, #22
	movs	r2, #3
	movs	r3, #2
	bl 0x0200afe8
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b088
	b.n	.L_020017c4
.L_0200150a:
	ldr	r3, [pc, #576]
	cmp	r2, r3
	bne.n	.L_020015ba
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #7
	ble.n	.L_02001520
	b.n	.L_020017c4
.L_02001520:
	cmp	r3, #6
	bge.n	.L_02001526
	b.n	.L_020017c4
.L_02001526:
	movs	r0, #100
	bl 0x0200af68
	cmp	r0, #0
	bne.n	.L_02001594
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200af68
	cmp	r0, #0
	bne.n	.L_02001568
	movs	r1, #172
	movs	r2, #134
	movs	r0, #8
	lsls	r1, r1, #17
	lsls	r2, r2, #18
	bl 0x0200b088
	movs	r3, #204
	lsls	r3, r3, #6
	movs	r1, #21
	movs	r2, #33
	movs	r0, #0
	bl 0x020082f0
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r0, #0
	movs	r1, #22
	movs	r2, #33
	bl 0x020082f0
	b.n	.L_02001584
.L_02001568:
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl 0x0200af68
	cmp	r0, #0
	bne.n	.L_02001584
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r0, #0
	movs	r1, #26
	movs	r2, #33
.L_02001580:
	bl 0x020082f0
.L_02001584:
	ldr	r3, [pc, #436]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	movs	r0, #8
	bl 0x0200b158
.L_02001594:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_020015a2
	b.n	.L_020017c4
.L_020015a2:
	ldr	r3, [pc, #408]
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #7
	beq.n	.L_020015b4
	b.n	.L_020017c4
.L_020015b4:
	bl 0x020091e0
	b.n	.L_020017c4
.L_020015ba:
	ldr	r3, [pc, #404]
	cmp	r2, r3
	bne.n	.L_020015c8
	ldr	r0, [pc, #400]
	bl 0x02008404
	b.n	.L_020017c4
.L_020015c8:
	ldr	r3, [pc, #396]
	cmp	r2, r3
	beq.n	.L_020015d0
	b.n	.L_02001770
.L_020015d0:
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r3, #1
	cmp	r3, #9
	bls.n	.L_020015e2
	b.n	.L_020017c4
.L_020015e2:
	ldr	r2, [pc, #376]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.2byte 0x0000
	.4byte 0x02009656
	.4byte 0x02009656
	.4byte 0x02009614
	.4byte 0x020096a4
	.4byte 0x020096a4
	.4byte 0x020096fe
	.4byte 0x020096fe
	.4byte 0x020097c4
	.4byte 0x020097c4
	.4byte 0x02009728
	.4byte 0x21ec208f
	.4byte 0x004023c8
	.4byte 0x22000449
	.4byte 0xf001041b
	.4byte 0x1c05fcb9
	.4byte 0xd1002d00
	.4byte 0x2101e0ca
	.4byte 0xfd02f7fe
	.4byte 0x32551c2a
	.4byte 0x70132300
	.4byte 0x21001c28
	.4byte 0xfcd6f001
	.4byte 0x210e1c28
	.4byte 0xfd32f001
	.4byte 0x21011c28
	.4byte 0xfcd2f001
	.4byte 0x4842e0b6
	.4byte 0xfed4f7fe
	.4byte 0x01002090
	.4byte 0xf0013047
	.4byte 0x2800fc81
	.4byte 0x21d8d014
	.4byte 0x200d2288
	.4byte 0x04120409
	.4byte 0xfd08f001
	.4byte 0x01002090
	.4byte 0xf0013046
	.4byte 0x2800fc73
	.4byte 0x21c8d006
	.4byte 0x200c2288
	.4byte 0x04120409
	.4byte 0xfcfaf001
	.4byte 0xf0014833
	.4byte 0x200cfd67
	.4byte 0xf0012102
	.4byte 0xe08ffd0b
	.4byte 0x01002090
	.4byte 0xf001304b
	.4byte 0x2800fc5d
	.4byte 0x2304d01c
	.4byte 0x25039301
	.4byte 0x21612012
	.4byte 0x23612206
	.4byte 0xf0019500
	.4byte 0x2302fc7d
	.4byte 0x20529301
	.4byte 0x22462122
	.4byte 0x95002322
	.4byte 0xfc74f001
	.4byte 0x22232307
	.4byte 0x92019300
	.4byte 0x21242007
	.4byte 0x23012201
	.4byte 0xfc7ef001
	.4byte 0x22002100
	.4byte 0x20042300
	.4byte 0xf0014d1c
	.4byte 0x2390fc4f
	.4byte 0x2100e007
	.4byte 0x23002200
	.4byte 0x4d182004
	.4byte 0xfc46f001
	.4byte 0x049b239c
	.4byte 0x4b1664c3
	.4byte 0x66c31c02
	.4byte 0x23003255
	.4byte 0x70136028
	.4byte 0xf7fe2103
	.4byte 0xe04dfc89
	.4byte 0x30ff200a
	.4byte 0xfc1cf001
	.4byte 0xd1472800
	.4byte 0xfd54f7ff
	.4byte 0x0000e044
	.4byte 0x02000240
	.4byte 0x000000ce
	.4byte 0x000000cf
	.4byte 0x02008841
	.4byte 0x000000d1
	.4byte 0x000000d2
	.4byte 0x0200b340
	.4byte 0x000000d3
	.4byte 0x020095ec
	.4byte 0x0200b35a
	.4byte 0x0200b264
	.4byte 0x0200bccc
	.2byte 0x90d5
	.2byte 0x0200
.L_02001770:
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_020017c4
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200af70
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #72
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_020017b4
	movs	r3, #3
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r0, #13
	movs	r1, #26
	movs	r2, #13
	movs	r3, #16
	bl 0x0200afc0
	movs	r3, #13
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #13
	movs	r1, #26
	movs	r2, #3
	movs	r3, #3
	bl 0x0200afe8
	b.n	.L_020017be
.L_020017b4:
	movs	r1, #144
	ldr	r0, [pc, #24]
	lsls	r1, r1, #3
	bl 0x0200aef8
.L_020017be:
	ldr	r0, [pc, #20]
	bl 0x0200b168
.L_020017c4:
	movs	r0, #0
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x000000d4
	.4byte 0x0200937d
	.2byte 0xb26e
	.2byte 0x0200
	.global Func_020017d8
	.thumb_func
Func_020017d8:
	movs	r0, #0
	bx	lr
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r2, r5, #0
	adds	r2, #100
	movs	r0, #0
	ldrsh	r1, [r2, r0]
	ldrh	r3, [r2, #0]
	cmp	r1, #0
	beq.n	.L_020017f4
	subs	r3, #1
	strh	r3, [r2, #0]
	b.n	.L_0200185a
.L_020017f4:
	adds	r3, r5, #0
	adds	r3, #90
	movs	r0, #131
	strb	r1, [r3, #0]
	lsls	r0, r0, #1
	bl 0x0200af68
	movs	r3, #1
	negs	r3, r3
	cmp	r0, #0
	bne.n	.L_0200181a
	ldr	r3, [pc, #80]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #76]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
.L_0200181a:
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_0200182c
	adds	r0, r5, #0
	movs	r1, #9
	bl 0x0200af80
	b.n	.L_0200185a
.L_0200182c:
	ldrh	r1, [r5, #6]
	movs	r2, #128
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	lsls	r2, r2, #5
	cmp	r3, r2
	ble.n	.L_0200183e
	adds	r3, r2, #0
.L_0200183e:
	ldr	r2, [pc, #36]
	cmp	r3, r2
	bge.n	.L_02001846
	adds	r3, r2, #0
.L_02001846:
	adds	r3, r1, r3
	adds	r0, r5, #0
	movs	r1, #2
	strh	r3, [r5, #6]
	bl 0x0200af80
	adds	r0, r5, #0
.L_02001854:
	movs	r1, #48
	bl 0x0200af88
.L_0200185a:
	pop	{r5, pc}
	.4byte 0x03001150
	.4byte 0x0200b272
	.2byte 0xf000
	.2byte 0xffff
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #192
.L_02001872:
	lsls	r2, r2, #4
	adds	r2, #162
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02001898
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200af70
	bl 0x0200b100
	bl 0x0200b140
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200af78
.L_02001898:
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #176]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #40
	bl 0x0200b138
	adds	r7, r0, #0
.L_020018bc:
	bl 0x02009868
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r1, [pc, #132]
	ldr	r3, [r7, #8]
	add	r2, sp, #28
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	mov	r0, sl
	str	r3, [sp, #12]
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [sp, #8]
	str	r3, [r0, #8]
	ldr	r2, [sp, #12]
	str	r3, [sp, #16]
	adds	r3, r7, #0
	adds	r3, #34
	str	r2, [sp, #20]
	str	r3, [sp, #4]
	adds	r1, r2, #0
	ldrb	r0, [r3, #0]
	ldr	r2, [sp, #16]
	bl 0x0200afe0
	str	r0, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r1, [r7, #8]
	ldr	r0, [sp, #16]
	ldr	r3, [pc, #68]
	ldr	r6, [r7, #16]
	subs	r1, r2, r1
	subs	r6, r0, r6
	mov	r8, r3
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x182d
	movs	r0, #128
	lsls	r0, r0, #11
	cmp	r5, r0
	bge.n	.L_02001974
	ldr	r3, [pc, #36]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #36]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	mov	r9, r2
	lsls	r3, r2, #16
	ldr	r2, [pc, #24]
	cmp	r3, r2
	bne.n	.L_020019a0
.L_0200195a:
	b.n	.L_02001b36
	.4byte 0x02000240
	.4byte 0xfff00000
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200b2b2
	.2byte 0x0000
	.2byte 0xffff
.L_02001974:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x0200af10
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #128
	ldr	r2, [pc, #16]
	mov	r9, r0
	lsls	r3, r3, #6
	add	r3, r9
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r9, r3
	b.n	.L_020019a0
	.2byte 0xc000
	.2byte 0xffff
.L_020019a0:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200af28
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
.L_020019b2:
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200afe0
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_02001a22
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
.L_020019c8:
	ldr	r2, [r2, #8]
	bl 0x0200afd8
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02001a22
	ldr	r0, [sp, #12]
	mov	r2, sl
	str	r0, [r2, #0]
	ldr	r3, [sp, #8]
	adds	r0, r7, #0
	str	r3, [r2, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r2, r7, #0
	str	r3, [r7, #52]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #8]
	ldr	r2, [r7, #12]
	bl 0x0200afb0
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200af80
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x0200af88
	adds	r0, r7, #0
	bl 0x0200afb8
	ldr	r3, [pc, #292]
	str	r3, [r7, #108]
	b.n	.L_02001acc
.L_02001a22:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_02001b18
.L_02001a36:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200afd8
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02001aec
	ldrh	r3, [r7, #32]
	movs	r2, #89
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r0, #0
	adds	r2, r2, r5
	mov	sl, r0
	mov	r8, r2
.L_02001a64:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001a8e
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001a8e
	cmp	r5, r7
	beq.n	.L_02001a8e
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200b028
	cmp	r0, #0
	bge.n	.L_02001aec
.L_02001a8e:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02001a64
	ldr	r0, [r6, #0]
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #10
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	str	r2, [sp, #16]
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #52]
	ldr	r3, [r6, #8]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	bl 0x0200afb0
	adds	r0, r7, #0
	bl 0x0200afb8
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_02001b12
.L_02001acc:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200af28
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200afe0
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_02001a36
.L_02001aec:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	adds	r0, r7, #0
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #16]
	bl 0x0200afb0
	adds	r0, r7, #0
	bl 0x0200afb8
	movs	r0, #2
	bl 0x0200aef0
	b.n	.L_020018bc
.L_02001b12:
	movs	r0, #10
	bl 0x0200aef0
.L_02001b18:
	movs	r3, #0
	str	r3, [r7, #108]
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200af80
.L_02001b36:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x97dd
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #80]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #40
	bl 0x0200b138
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	adds	r7, r0, #0
	strh	r3, [r2, #0]
.L_02001b6e:
	bl 0x02009868
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	str	r3, [r7, #64]
	movs	r3, #0
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r1, [pc, #32]
	ldr	r3, [r7, #8]
	add	r2, sp, #28
	mov	sl, r2
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	adds	r3, r3, r2
	mov	r0, sl
	str	r3, [sp, #12]
	str	r3, [r0, #0]
	b.n	.L_02001bb4
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x0200bcd0
	.2byte 0x0000
	.2byte 0xfff0
.L_02001bb4:
	.2byte 0x68fb
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [sp, #8]
	str	r3, [r0, #8]
	ldr	r2, [sp, #12]
	str	r3, [sp, #16]
	adds	r3, r7, #0
	adds	r3, #34
	str	r2, [sp, #20]
	str	r3, [sp, #4]
	adds	r1, r2, #0
	ldrb	r0, [r3, #0]
	ldr	r2, [sp, #16]
	bl 0x0200afe0
	str	r0, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r1, [r7, #8]
	ldr	r0, [sp, #16]
	ldr	r3, [pc, #60]
	ldr	r6, [r7, #16]
	subs	r1, r2, r1
	subs	r6, r0, r6
	mov	r8, r3
	adds	r0, r1, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x1c31
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x182d
	movs	r0, #128
	lsls	r0, r0, #11
	cmp	r5, r0
	bge.n	.L_02001c30
	ldr	r3, [pc, #28]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #28]
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrsh	r2, [r1, r3]
	mov	r9, r2
	lsls	r3, r2, #16
	ldr	r2, [pc, #16]
	cmp	r3, r2
	bne.n	.L_02001c5c
	b.n	.L_02001e26
	.4byte 0x0300021c
	.4byte 0x03001150
	.4byte 0x0200b2b2
	.2byte 0x0000
	.2byte 0xffff
.L_02001c30:
	.2byte 0x9b04
	ldr	r2, [sp, #20]
	ldr	r0, [r7, #16]
	ldr	r1, [r7, #8]
	subs	r0, r3, r0
	subs	r1, r2, r1
	bl 0x0200af10
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #128
	ldr	r2, [pc, #16]
	mov	r9, r0
	lsls	r3, r3, #6
	add	r3, r9
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r9, r3
	b.n	.L_02001c5c
	.2byte 0xc000
	.2byte 0xffff
.L_02001c5c:
	.2byte 0x2080
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200af28
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldrb	r0, [r3, #0]
	ldr	r2, [r2, #8]
	bl 0x0200afe0
	mov	fp, r0
	cmp	r0, #255
	beq.n	.L_02001cd6
	ldr	r3, [sp, #4]
	mov	r2, sl
	ldrb	r0, [r3, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	bl 0x0200afd8
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02001cd6
	ldr	r0, [sp, #12]
	mov	r2, sl
	str	r0, [r2, #0]
	ldr	r3, [sp, #8]
	adds	r0, r7, #0
	str	r3, [r2, #8]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r2, r7, #0
	str	r3, [r7, #52]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [r7, #12]
	ldr	r3, [sp, #8]
	bl 0x0200afb0
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200af80
	adds	r0, r7, #0
	movs	r1, #48
	bl 0x0200af88
	movs	r5, #0
	b.n	.L_02001cfe
.L_02001cd6:
	movs	r3, #0
	mov	r0, r9
	strh	r0, [r7, #6]
	str	r3, [r7, #36]
	str	r3, [r7, #44]
	ldr	r2, [sp, #12]
	str	r2, [r7, #8]
	ldr	r3, [sp, #8]
	str	r3, [r7, #16]
	b.n	.L_02001e26
.L_02001cea:
	ldr	r3, [pc, #360]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_02001cf6
	b.n	.L_02001e26
.L_02001cf6:
	movs	r0, #1
	bl 0x0200aef0
	adds	r5, #1
.L_02001cfe:
	cmp	r5, #179
	bgt.n	.L_02001d0c
	adds	r0, r7, #0
	bl 0x0200b018
	cmp	r0, #0
	beq.n	.L_02001cea
.L_02001d0c:
	ldr	r3, [pc, #328]
	str	r3, [r7, #108]
	b.n	.L_02001dda
.L_02001d12:
	ldr	r2, [sp, #4]
	ldr	r1, [r6, #0]
	ldrb	r0, [r2, #0]
	ldr	r2, [r6, #8]
	bl 0x0200afd8
	ldr	r3, [r7, #12]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	bgt.n	.L_02001dfa
	ldr	r3, [pc, #296]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02001e26
	ldrh	r3, [r7, #32]
	movs	r2, #89
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #20]
	movs	r0, #0
	adds	r2, r2, r5
	mov	sl, r0
	mov	r8, r2
.L_02001d4a:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02001d74
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001d74
	cmp	r5, r7
	beq.n	.L_02001d74
	ldrh	r3, [r5, #32]
	adds	r0, r5, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #28
	bl 0x0200b028
	cmp	r0, #0
	bge.n	.L_02001dfa
.L_02001d74:
	movs	r0, #1
	add	sl, r0
	movs	r2, #128
	mov	r3, sl
	add	r8, r2
	adds	r5, #128
	cmp	r3, #63
	ble.n	.L_02001d4a
	ldr	r0, [r6, #0]
	movs	r3, #128
	str	r0, [sp, #20]
	lsls	r3, r3, #10
	ldr	r2, [r6, #8]
	adds	r0, r7, #0
	str	r2, [sp, #16]
	str	r3, [r7, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r7, #52]
	movs	r5, #0
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl 0x0200afb0
	b.n	.L_02001db2
.L_02001daa:
	movs	r0, #1
	bl 0x0200aef0
	adds	r5, #1
.L_02001db2:
	cmp	r5, #179
	bgt.n	.L_02001dca
	adds	r0, r7, #0
	bl 0x0200b018
	cmp	r0, #0
	bne.n	.L_02001dca
	ldr	r3, [pc, #144]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_02001daa
.L_02001dca:
	ldr	r3, [pc, #136]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_02001e26
	ldr	r3, [sp, #24]
	cmp	fp, r3
	bne.n	.L_02001e20
.L_02001dda:
	movs	r0, #128
	add	r2, sp, #28
	lsls	r0, r0, #13
	mov	r1, r9
	bl 0x0200af28
	ldr	r2, [sp, #4]
	add	r6, sp, #28
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200afe0
	mov	fp, r0
	cmp	r0, #255
	bne.n	.L_02001d12
.L_02001dfa:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r7, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	ldr	r2, [r7, #12]
	adds	r0, r7, #0
	ldr	r1, [sp, #20]
	ldr	r3, [sp, #16]
	bl 0x0200afb0
	adds	r0, r7, #0
	bl 0x0200afb8
	movs	r0, #2
	bl 0x0200aef0
	b.n	.L_02001b6e
.L_02001e20:
	movs	r0, #10
	bl 0x0200aef0
.L_02001e26:
	movs	r3, #0
	str	r3, [r7, #108]
	adds	r1, r7, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #52]
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200af80
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200bcd0
	.4byte 0x020097dd
	.4byte 0x80184b01
	.4byte 0x00004770
	.2byte 0xbcd0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #92]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	sub	sp, #32
	bl 0x0200b138
	adds	r5, r0, #0
	ldrh	r3, [r5, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #60]
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	str	r3, [sp, #16]
.L_02001e9a:
	bl 0x02009868
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #56]
	str	r3, [r5, #64]
	movs	r3, #0
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	ldr	r2, [pc, #28]
	ldr	r3, [r5, #8]
	movs	r1, #128
	lsls	r1, r1, #12
	ands	r3, r2
	mov	r9, r1
	add	r6, sp, #20
	add	r3, r9
	str	r3, [r6, #0]
	mov	r8, r3
	b.n	.L_02001edc
	.2byte 0x0000
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
.L_02001edc:
	.2byte 0x68eb
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	ands	r3, r2
	adds	r7, r3, r1
	mov	r2, r8
	str	r7, [r6, #8]
	str	r2, [sp, #8]
	str	r7, [sp, #4]
	movs	r3, #34
	adds	r3, r3, r5
	ldrb	r0, [r3, #0]
	adds	r1, r2, #0
	adds	r2, r7, #0
	mov	fp, r3
	bl 0x0200afe0
	str	r0, [sp, #12]
	movs	r0, #128
	ldr	r1, [sp, #16]
	lsls	r0, r0, #13
	adds	r2, r6, #0
	bl 0x0200af28
	mov	r1, fp
	ldrb	r0, [r1, #0]
	ldr	r2, [r6, #8]
	ldr	r1, [r6, #0]
	bl 0x0200afe0
	mov	sl, r0
	cmp	r0, #255
	beq.n	.L_02001f70
	mov	r2, fp
	ldrb	r0, [r2, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl 0x0200afd8
	ldr	r3, [r5, #12]
	subs	r0, r0, r3
	cmp	r0, r9
	bgt.n	.L_02001f70
	ldr	r3, [sp, #8]
	ldr	r2, [pc, #48]
	str	r3, [r6, #0]
	ldr	r1, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r1, [r6, #8]
	str	r3, [r5, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #52]
	adds	r3, r5, #0
	adds	r3, #100
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200af80
	adds	r0, r5, #0
	movs	r1, #48
	bl 0x0200af88
	ldr	r3, [pc, #8]
	str	r3, [r5, #108]
	b.n	.L_0200201a
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x97dd
	.2byte 0x0200
.L_02001f70:
	add	r1, sp, #16
	ldrh	r1, [r1, #0]
	movs	r3, #0
	mov	r2, r8
	strh	r1, [r5, #6]
	str	r3, [r5, #36]
	str	r3, [r5, #44]
	str	r2, [r5, #8]
	str	r7, [r5, #16]
	b.n	.L_02002066
.L_02001f84:
	mov	r3, fp
	ldrb	r0, [r3, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl 0x0200afd8
	ldr	r3, [r5, #12]
	movs	r1, #128
	subs	r0, r0, r3
	lsls	r1, r1, #12
	cmp	r0, r1
	bgt.n	.L_0200203a
	ldrh	r3, [r5, #32]
	movs	r2, #0
	subs	r3, #2
	str	r3, [sp, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #20]
	movs	r3, #89
	adds	r3, r3, r6
	mov	r9, r2
	mov	r8, r3
.L_02001fb2:
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02001fdc
	mov	r1, r8
	ldrb	r2, [r1, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001fdc
	cmp	r6, r5
	beq.n	.L_02001fdc
	ldrh	r3, [r6, #32]
	adds	r0, r6, #0
	adds	r0, #8
	subs	r3, #2
	ldr	r1, [sp, #0]
	add	r2, sp, #20
	bl 0x0200b028
	cmp	r0, #0
	bge.n	.L_0200203a
.L_02001fdc:
	movs	r2, #1
	add	r9, r2
	movs	r3, #128
	mov	r1, r9
	add	r8, r3
	adds	r6, #128
	cmp	r1, #63
	ble.n	.L_02001fb2
	ldr	r2, [r7, #0]
	adds	r0, r5, #0
	str	r2, [sp, #8]
	ldr	r3, [r7, #8]
	str	r3, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	bl 0x0200afb0
	adds	r0, r5, #0
	bl 0x0200afb8
	ldr	r1, [sp, #12]
	cmp	sl, r1
	bne.n	.L_02002060
.L_0200201a:
	movs	r0, #128
	ldr	r1, [sp, #16]
	add	r2, sp, #20
	lsls	r0, r0, #13
	bl 0x0200af28
	mov	r2, fp
	add	r7, sp, #20
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl 0x0200afe0
	mov	sl, r0
	cmp	r0, #255
	bne.n	.L_02001f84
.L_0200203a:
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	ldr	r1, [sp, #8]
	ldr	r3, [sp, #4]
	bl 0x0200afb0
	adds	r0, r5, #0
	bl 0x0200afb8
	movs	r0, #2
	bl 0x0200aef0
	b.n	.L_02001e9a
.L_02002060:
	movs	r0, #10
	bl 0x0200aef0
.L_02002066:
	movs	r3, #0
	str	r3, [r5, #108]
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #52]
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200af80
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	sub	sp, #4
	cmp	r3, r2
	beq.n	.L_020020f8
	adds	r7, r0, #0
.L_020020aa:
	ldrh	r3, [r7, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200b058
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #89
	movs	r2, #2
	ldrsh	r6, [r7, r2]
	ldrb	r2, [r1, #0]
	movs	r3, #4
	ldrsh	r4, [r7, r3]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #0
	str	r4, [sp, #0]
	bl 0x0200aff0
	ldr	r4, [sp, #0]
	lsls	r6, r6, #16
	lsls	r4, r4, #16
	lsrs	r4, r4, #16
	lsrs	r6, r6, #16
	adds	r5, #34
	ldrb	r3, [r5, #0]
	adds	r2, r4, #0
	mov	r0, r8
	adds	r1, r6, #0
	adds	r7, #6
	bl 0x0200a184
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020020aa
.L_020020f8:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r5, r0, #0
	mov	r9, r1
	mov	sl, r2
	movs	r1, #255
	ldr	r2, [r3, #0]
	b.n	.L_0200216c
.L_0200211c:
	ldrh	r3, [r5, #0]
	movs	r1, #26
	ldrsh	r7, [r2, r1]
	cmp	r7, r3
	bne.n	.L_02002168
	adds	r0, r7, #0
	bl 0x0200b058
	adds	r5, #2
	ldrh	r2, [r5, #0]
	mov	r3, sl
	adds	r6, r0, #0
	mov	r8, r2
	ldrh	r5, [r5, #2]
	cmp	r3, #7
	bgt.n	.L_02002144
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #64]
	adds	r3, r3, r1
	str	r3, [r6, #28]
.L_02002144:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_02002176
	adds	r0, r5, #0
	bl 0x0200af70
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	adds	r0, r7, #0
	mov	r1, r8
	adds	r2, r5, #0
	bl 0x0200a184
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #28]
	b.n	.L_02002176
.L_02002168:
	adds	r5, #6
	movs	r1, #255
.L_0200216c:
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_0200211c
.L_02002176:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r5, r3, #0
	mov	r8, r2
	adds	r6, r1, #0
	bl 0x0200b058
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r5, [r2, r3]
	adds	r7, r0, #0
	bl 0x0200b160
	lsls	r0, r0, #2
	adds	r5, r5, r0
	mov	r0, r8
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_020021f8
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #0
	strb	r3, [r5, #2]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	cmp	r6, #1
	beq.n	.L_020021e4
	cmp	r6, #1
	bcc.n	.L_020021da
	cmp	r6, #2
	beq.n	.L_020021ee
	b.n	.L_02002226
.L_020021da:
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200af80
	b.n	.L_02002226
.L_020021e4:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200af80
	b.n	.L_02002226
.L_020021ee:
	adds	r0, r7, #0
	movs	r1, #6
	bl 0x0200af80
	b.n	.L_02002226
.L_020021f8:
	movs	r3, #255
	strb	r3, [r5, #2]
.L_020021fc:
	cmp	r6, #1
	beq.n	.L_02002214
	cmp	r6, #1
	bcc.n	.L_0200220a
	cmp	r6, #2
	beq.n	.L_0200221e
	b.n	.L_02002226
.L_0200220a:
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200af80
	b.n	.L_02002226
.L_02002214:
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x0200af80
	b.n	.L_02002226
.L_0200221e:
	adds	r0, r7, #0
	movs	r1, #5
	bl 0x0200af80
.L_02002226:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #20
	str	r3, [sp, #16]
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	mov	fp, r0
	cmp	r3, r2
	beq.n	.L_02002312
.L_02002252:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	adds	r0, r3, #0
	str	r3, [sp, #12]
	bl 0x0200b058
	mov	r2, fp
	ldrh	r2, [r2, #2]
	adds	r7, r0, #0
	str	r2, [sp, #8]
	movs	r3, #34
	adds	r3, r3, r7
	adds	r0, r2, #0
	ldrb	r2, [r3, #0]
	mov	r9, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #16]
	adds	r0, #1
	ldr	r5, [r2, r3]
	ldr	r2, [pc, #196]
	adds	r3, r5, r2
	ldr	r2, [pc, #196]
	asrs	r3, r3, #2
	adds	r6, r3, r2
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_020022a0
	ldr	r0, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b088
	b.n	.L_02002300
.L_020022a0:
	adds	r0, r7, #0
	bl 0x0200b160
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r5, r5, r3
	str	r5, [sp, #4]
	mov	r2, r9
	ldrb	r0, [r2, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	bl 0x0200b010
	mov	r3, r9
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #16]
	mov	sl, r0
	ldrb	r0, [r3, #0]
	bl 0x0200afd8
	adds	r5, r0, #0
	ldr	r0, [sp, #12]
	bl 0x0200b0c0
	ldr	r2, [sp, #4]
	movs	r3, #128
	asrs	r5, r5, #19
	strb	r3, [r2, #3]
	adds	r5, #4
	mov	r3, r9
	adds	r2, r5, #0
	ldrb	r0, [r3, #0]
	mov	r1, sl
	bl 0x0200b180
	add	r8, r6
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_02002300
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200af80
.L_02002300:
	movs	r3, #4
	add	fp, r3
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02002252
.L_02002312:
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b058
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x0200afd8
	ldr	r3, [r5, #12]
	cmp	r3, r0
	bge.n	.L_0200233a
	str	r0, [r5, #20]
	str	r0, [r5, #12]
.L_0200233a:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #12
	adds	r5, r0, #0
	bl 0x0200b0f8
	cmp	r0, #0
	beq.n	.L_0200236a
	b.n	.L_020024da
.L_0200236a:
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b058
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r6, #12]
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	str	r3, [r0, #8]
	bl 0x0200b188
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_02002396
	b.n	.L_020024da
.L_02002396:
	b.n	.L_020024cc
.L_02002398:
	ldrh	r7, [r5, #0]
	adds	r0, r7, #0
	bl 0x0200b058
	cmp	r0, r8
	beq.n	.L_020023a8
	adds	r5, #4
	b.n	.L_020024cc
.L_020023a8:
	ldrh	r5, [r5, #2]
	bl 0x0200b048
	adds	r0, r5, #0
	bl 0x0200af68
	cmp	r0, #0
	bne.n	.L_02002416
	movs	r0, #125
	bl 0x0200b1a8
	adds	r0, r7, #0
	bl 0x0200b058
	movs	r1, #7
	bl 0x0200b0b0
	movs	r0, #2
	bl 0x0200aef0
	movs	r1, #0
	mov	r0, r8
	bl 0x0200af80
	adds	r0, r7, #0
	bl 0x0200b058
	movs	r1, #0
	bl 0x0200b0b0
	movs	r0, #2
	bl 0x0200aef0
	adds	r0, r7, #0
	bl 0x0200b058
	movs	r1, #7
	bl 0x0200b0b0
	movs	r0, #4
	bl 0x0200aef0
	adds	r0, r7, #0
	bl 0x0200b058
	movs	r1, #0
	bl 0x0200b0b0
	movs	r0, #0
	bl 0x0200a9d0
	adds	r0, r5, #0
	bl 0x0200af70
	b.n	.L_020024c6
.L_02002416:
	adds	r5, #1
	mov	sl, r5
	mov	r0, sl
	bl 0x0200af68
	cmp	r0, #0
	bne.n	.L_020024c6
	adds	r6, #85
	strb	r0, [r6, #0]
	movs	r0, #185
	bl 0x0200b1a8
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200b000
	movs	r0, #0
	bl 0x0200a9d0
	movs	r5, #2
	movs	r0, #8
	mov	r7, r8
	bl 0x0200aef0
	negs	r5, r5
	mov	r0, r8
	movs	r1, #2
	adds	r7, #34
	bl 0x0200af80
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x0200aca8
	movs	r0, #1
	bl 0x0200a9d0
	movs	r0, #16
	bl 0x0200aef0
	ldrb	r1, [r7, #0]
	adds	r0, r5, #0
	bl 0x0200aca8
	movs	r0, #4
	bl 0x0200aef0
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200b000
	movs	r0, #8
	bl 0x0200aef0
	movs	r3, #3
	strb	r3, [r6, #0]
	movs	r0, #5
	bl 0x0200aef0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	mov	r0, r8
	bl 0x0200afa0
	movs	r0, #2
	bl 0x0200aef0
	movs	r0, #188
	bl 0x0200b1a8
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aef0
	mov	r0, sl
	bl 0x0200af70
.L_020024c6:
	bl 0x0200b050
	b.n	.L_020024da
.L_020024cc:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_020024da
	b.n	.L_02002398
.L_020024da:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl 0x0200b058
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r7, r0, #0
	cmp	r3, r2
	beq.n	.L_020025c2
.L_02002506:
	ldrh	r3, [r5, #0]
	cmp	r3, r6
	beq.n	.L_02002510
	adds	r5, #4
	b.n	.L_020025b6
.L_02002510:
	ldrh	r5, [r5, #2]
	bl 0x0200b048
	adds	r3, r5, #1
	mov	r8, r3
	mov	r0, r8
	bl 0x0200af68
	cmp	r0, #0
	bne.n	.L_020025b0
	movs	r0, #185
	bl 0x0200b1a8
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200b000
	movs	r0, #0
	bl 0x0200a9d0
	movs	r0, #8
	bl 0x0200aef0
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200af80
	adds	r3, r7, #0
	adds	r3, #34
	movs	r0, #4
	ldrb	r1, [r3, #0]
	adds	r2, r6, #0
	negs	r0, r0
	bl 0x0200ac1c
	movs	r0, #1
	bl 0x0200a9d0
	movs	r0, #16
	bl 0x0200aef0
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200b000
	movs	r0, #8
	bl 0x0200aef0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x0200afa0
	movs	r0, #2
	bl 0x0200aef0
	movs	r0, #188
	bl 0x0200b1a8
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aef0
	adds	r0, r5, #0
	bl 0x0200af70
	mov	r0, r8
	bl 0x0200af70
.L_020025b0:
	bl 0x0200b050
	b.n	.L_020025c2
.L_020025b6:
	movs	r2, #255
	ldrh	r3, [r5, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02002506
.L_020025c2:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	bl 0x0200b058
	movs	r3, #3
	adds	r0, #92
	strb	r3, [r0, #0]
	adds	r0, r5, #0
	bl 0x0200b0c0
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #16
	ldr	r5, [pc, #324]
	str	r3, [sp, #12]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	mov	fp, r0
	ldr	r1, [r5, #0]
	movs	r0, #8
	bl 0x0200b090
	ldr	r1, [r5, #0]
	movs	r0, #9
	bl 0x0200b090
	ldr	r1, [r5, #0]
	movs	r0, #10
	bl 0x0200b090
	movs	r0, #1
	bl 0x0200aef0
	movs	r0, #8
	bl 0x0200a5c8
	movs	r0, #9
	bl 0x0200a5c8
	movs	r0, #10
	bl 0x0200a5c8
	movs	r1, #0
	movs	r0, #9
	bl 0x0200b098
	movs	r0, #1
	bl 0x0200aef0
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b088
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b088
	movs	r2, #0
	movs	r0, #10
	movs	r1, #0
	bl 0x0200b088
	movs	r0, #1
	bl 0x0200aef0
	b.n	.L_0200271a
.L_02002666:
	mov	r3, fp
	ldrh	r3, [r3, #0]
	mov	r9, r3
	mov	r0, r9
	bl 0x0200b058
	mov	r2, fp
	ldrh	r2, [r2, #2]
	adds	r5, r0, #0
	str	r2, [sp, #8]
	adds	r7, r5, #0
	adds	r7, #34
	adds	r0, r2, #0
	ldrb	r2, [r7, #0]
	adds	r0, #1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r2, #156
	lsls	r2, r2, #1
	lsls	r3, r3, #3
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	ldr	r6, [r2, r3]
	ldr	r2, [pc, #168]
	adds	r3, r6, r2
	ldr	r2, [pc, #168]
	asrs	r3, r3, #2
	adds	r2, r2, r3
	mov	sl, r2
	adds	r2, r5, #0
	adds	r2, #92
	movs	r3, #3
	strb	r3, [r2, #0]
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_020026bc
	mov	r0, r9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b088
	b.n	.L_02002716
.L_020026bc:
	adds	r0, r5, #0
	bl 0x0200b160
	mov	r8, r0
	mov	r3, r8
	lsls	r3, r3, #2
	adds	r6, r6, r3
	str	r6, [sp, #4]
	add	r8, sl
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r7, #0]
	bl 0x0200b010
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	mov	sl, r0
	ldrb	r0, [r7, #0]
	bl 0x0200afd8
	adds	r5, r0, #0
	mov	r0, r9
	bl 0x0200b0c0
	ldr	r6, [sp, #4]
	asrs	r5, r5, #19
	movs	r3, #128
	adds	r5, #4
	adds	r2, r5, #0
	strb	r3, [r6, #3]
	ldrb	r0, [r7, #0]
	mov	r1, sl
	bl 0x0200b180
	mov	r2, r8
	strb	r0, [r2, #0]
	ldr	r0, [sp, #8]
	bl 0x0200af68
	cmp	r0, #0
	beq.n	.L_02002716
	mov	r0, r9
	movs	r1, #9
	bl 0x0200b0d0
.L_02002716:
	movs	r3, #4
	add	fp, r3
.L_0200271a:
	mov	r2, fp
	ldrh	r3, [r2, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_02002666
	movs	r0, #10
	bl 0x0200aef0
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xfdff0000
	.2byte 0x4000
	.2byte 0x0202
	push	{r5, lr}
	adds	r5, r1, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	bl 0x0200afa0
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x0200afa0
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #12
	adds	r6, r0, #0
	bl 0x0200b0f8
	cmp	r0, #0
	beq.n	.L_0200277c
	b.n	.L_0200293e
.L_0200277c:
	ldr	r3, [pc, #460]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b058
	adds	r5, r0, #0
	movs	r0, #8
	bl 0x0200b058
	ldr	r3, [r5, #8]
	adds	r7, r0, #0
.L_02002796:
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r1, #0
	ldr	r3, [r5, #12]
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
	bl 0x0200b188
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_020027b0
	b.n	.L_0200293e
.L_020027b0:
	b.n	.L_02002930
.L_020027b2:
	ldrh	r3, [r6, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200b058
	cmp	r0, sl
	beq.n	.L_020027c4
	adds	r6, #4
	b.n	.L_02002930
.L_020027c4:
	ldrh	r6, [r6, #2]
	bl 0x0200b048
	adds	r0, r6, #0
	bl 0x0200af68
	cmp	r0, #0
	bne.n	.L_0200285e
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200af80
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x0200a748
	movs	r0, #1
	bl 0x0200aef0
	movs	r0, #125
	bl 0x0200b1a8
	movs	r0, #8
	bl 0x0200b058
	movs	r1, #7
	bl 0x0200b0b0
	movs	r0, #2
	bl 0x0200aef0
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200af80
	movs	r1, #9
	mov	r0, r8
	bl 0x0200b0d0
	movs	r0, #8
	bl 0x0200b058
	movs	r1, #0
	bl 0x0200b0b0
	movs	r0, #2
	bl 0x0200aef0
	movs	r0, #8
	bl 0x0200b058
	movs	r1, #7
	bl 0x0200b0b0
	movs	r0, #4
	bl 0x0200aef0
	movs	r0, #8
	bl 0x0200b058
	movs	r1, #0
	bl 0x0200b0b0
	movs	r0, #0
	bl 0x0200a9d0
	mov	r0, sl
	adds	r1, r7, #0
	bl 0x0200a748
	movs	r0, #1
	bl 0x0200aef0
	adds	r0, r6, #0
	bl 0x0200af70
	b.n	.L_0200292a
.L_0200285e:
	adds	r6, #1
	mov	r9, r6
	mov	r0, r9
	bl 0x0200af68
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0200292a
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200af80
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x0200a748
	adds	r5, #85
	movs	r0, #1
	bl 0x0200aef0
	strb	r6, [r5, #0]
	movs	r0, #185
	bl 0x0200b1a8
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r1, #11
	lsls	r0, r0, #9
	bl 0x0200b000
	movs	r0, #0
	bl 0x0200a9d0
	mov	r8, r5
	movs	r0, #8
	movs	r6, #2
	mov	r5, sl
	bl 0x0200aef0
	negs	r6, r6
	adds	r0, r7, #0
	movs	r1, #2
	adds	r5, #34
	bl 0x0200af80
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x0200aca8
	movs	r0, #1
	bl 0x0200a9d0
	movs	r0, #16
	bl 0x0200aef0
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	bl 0x0200aca8
	movs	r0, #4
	bl 0x0200aef0
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200b000
	movs	r0, #8
	bl 0x0200aef0
	movs	r3, #3
	mov	r2, r8
	strb	r3, [r2, #0]
	movs	r0, #5
	bl 0x0200aef0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	adds	r0, r7, #0
	bl 0x0200afa0
	movs	r0, #2
	bl 0x0200aef0
	movs	r0, #188
	bl 0x0200b1a8
	bl 0x0200ab04
	movs	r0, #20
	bl 0x0200aef0
	mov	r0, r9
	bl 0x0200af70
.L_0200292a:
	bl 0x0200b050
	b.n	.L_0200293e
.L_02002930:
	movs	r2, #255
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_0200293e
	b.n	.L_020027b2
.L_0200293e:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200aee8
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002980
	adds	r3, #15
.L_02002980:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	ldr	r3, [pc, #36]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b058
	ldr	r3, [r0, #80]
	ldr	r4, [r6, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r4, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r4, #9]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #256]
	mov	r8, r0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl 0x0200b058
	movs	r2, #0
	adds	r7, r0, #0
	mov	r9, r2
	mov	sl, r2
.L_020029f2:
	bl 0x0200af08
	lsls	r3, r0, #3
	subs	r3, r3, r0
	ldr	r2, [r7, #12]
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	lsls	r3, r3, #16
	subs	r2, r2, r3
	mov	r3, sl
	lsls	r1, r3, #17
	ldr	r3, [r7, #8]
	ldr	r0, [pc, #212]
	adds	r1, r1, r3
	ldr	r3, [pc, #212]
	adds	r1, r1, r0
	movs	r0, #30
	adds	r2, r2, r3
	adds	r0, #255
	ldr	r3, [r7, #16]
	bl 0x0200af98
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02002ac6
	mov	r1, r9
	ldr	r0, [r6, #80]
	bl 0x0200b150
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r6, #0
	bl 0x0200aff0
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x0200af80
	adds	r0, r6, #0
	ldr	r1, [pc, #152]
	bl 0x0200af90
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	ldr	r1, [r6, #80]
	movs	r0, #13
	ldrb	r3, [r1, #9]
	negs	r0, r0
	adds	r2, r0, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	mov	r2, r8
	strb	r3, [r1, #9]
	cmp	r2, #0
	beq.n	.L_02002a90
	mov	r3, sl
	lsls	r5, r3, #13
	adds	r0, r5, #0
	bl 0x0200af20
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #108]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x6470
	adds	r0, r5, #0
	bl 0x0200af18
	b.n	.L_02002a94
.L_02002a90:
	mov	r0, r8
	str	r0, [r6, #68]
.L_02002a94:
	str	r0, [r6, #76]
	bl 0x0200af08
	movs	r2, #192
	lsls	r0, r0, #14
	lsls	r2, r2, #7
	lsrs	r0, r0, #16
	adds	r0, r0, r2
	negs	r0, r0
	str	r0, [r6, #72]
	bl 0x0200af08
	ldr	r3, [pc, #68]
	lsls	r0, r0, #9
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r3, r6, #0
	adds	r3, #100
	strh	r0, [r3, #0]
	ldr	r3, [pc, #60]
	str	r3, [r6, #48]
	ldr	r3, [pc, #60]
	str	r3, [r6, #52]
	ldr	r3, [pc, #60]
	str	r3, [r6, #108]
.L_02002ac6:
	movs	r0, #1
	add	sl, r0
	mov	r2, sl
	cmp	r2, #7
	bls.n	.L_020029f2
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff80000
	.4byte 0xfffe0000
	.4byte 0x0200b2d4
	.4byte 0x0300021c
	.4byte 0x00013333
	.4byte 0xffffff00
	.4byte 0xfffff800
	.4byte 0xfffffa00
	.2byte 0xa951
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #244]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200b058
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02002bf8
	movs	r3, #0
	mov	r9, r3
	mov	sl, r3
.L_02002b28:
	movs	r0, #30
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, #255
	bl 0x0200af98
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_02002bee
	mov	r1, r9
	ldr	r0, [r7, #80]
	bl 0x0200b150
	movs	r4, #0
	mov	r8, r4
	adds	r3, r7, #0
	mov	r2, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r3, #4
	strb	r2, [r3, #0]
	movs	r1, #0
	mov	r9, r0
	adds	r0, r7, #0
	bl 0x0200aff0
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200af80
	ldr	r1, [pc, #160]
	adds	r0, r7, #0
	bl 0x0200af90
	mov	r3, sl
	lsls	r5, r3, #12
	adds	r0, r5, #0
	bl 0x0200af20
	mov	r4, r8
	str	r4, [r7, #72]
	str	r0, [r7, #68]
	adds	r0, r5, #0
	bl 0x0200af18
	ldr	r3, [r7, #68]
	str	r0, [r7, #76]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r7, #68]
	bl 0x0200af08
	lsls	r3, r0, #1
	ldr	r2, [r7, #68]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #108]
	adds	r2, r2, r3
	str	r2, [r7, #68]
	bl 0x0200af08
	lsls	r3, r0, #1
	ldr	r2, [r7, #76]
	adds	r3, r3, r0
	ldr	r4, [pc, #96]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	adds	r2, r2, r4
	str	r2, [r7, #76]
	bl 0x0200af08
	ldr	r2, [pc, #84]
	lsls	r0, r0, #12
	lsrs	r0, r0, #16
	adds	r3, r7, #0
	adds	r0, r0, r2
	adds	r3, #100
	strh	r0, [r3, #0]
	mov	r3, r8
	str	r3, [r7, #48]
	str	r3, [r7, #52]
	ldr	r3, [pc, #68]
	ldr	r0, [r7, #80]
	str	r3, [r7, #108]
	ldr	r3, [r6, #80]
	movs	r1, #12
	ldrb	r3, [r3, #9]
	movs	r4, #13
	ands	r1, r3
	ldrb	r3, [r0, #9]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #9]
.L_02002bee:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_02002b28
.L_02002bf8:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200b304
	.4byte 0xffffa000
	.4byte 0xffffd000
	.4byte 0xfffff800
	.2byte 0xa951
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	adds	r0, r2, #0
	adds	r5, r1, #0
	bl 0x0200b058
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r3, [r2, r3]
	ldr	r2, [pc, #88]
	adds	r7, r0, #0
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	adds	r5, r7, #0
	asrs	r3, r3, #2
	adds	r5, #34
	adds	r6, r3, r1
	ldr	r2, [r7, #16]
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl 0x0200b010
	ldr	r2, [r7, #16]
	mov	r8, r0
	ldr	r1, [r7, #8]
	ldrb	r0, [r5, #0]
	bl 0x0200afd8
	ldr	r3, [r7, #8]
	asrs	r2, r0, #19
	add	r2, sl
	cmp	r3, #0
	bge.n	.L_02002c76
	ldr	r1, [pc, #48]
	adds	r3, r3, r1
.L_02002c76:
	ldr	r0, [r7, #16]
	asrs	r1, r3, #20
	cmp	r0, #0
	bge.n	.L_02002c82
	ldr	r3, [pc, #36]
	adds	r0, r0, r3
.L_02002c82:
	asrs	r3, r0, #20
	lsls	r3, r3, #7
	adds	r3, r1, r3
	ldrb	r0, [r5, #0]
	mov	r1, r8
	adds	r6, r6, r3
	bl 0x0200b180
	strb	r0, [r6, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfdff0000
	.4byte 0x02024000
	.2byte 0xffff
	.2byte 0x000f
	push	{lr}
	ldr	r3, [pc, #16]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r3, #0]
	bl 0x0200ac1c
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	movs	r3, #192
	adds	r5, r7, r2
	lsls	r3, r3, #4
	movs	r2, #63
	adds	r6, r7, r3
	mov	r8, r2
.L_02002cde:
	ldr	r3, [r5, #24]
	cmp	r3, #19
	bhi.n	.L_02002d2c
	movs	r2, #176
	lsls	r2, r2, #5
	adds	r2, #2
	adds	r1, r7, r2
	ldrh	r1, [r1, #0]
	movs	r2, #7
	asrs	r3, r3, #2
	ands	r3, r2
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	ands	r1, r3
	ldrh	r3, [r6, #8]
	adds	r0, r6, #0
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r6, #8]
	adds	r1, r5, #0
	bl 0x0200b198
	adds	r0, r5, #0
	movs	r1, #63
	ldr	r2, [pc, #20]
	bl 0x0200b1a0
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	b.n	.L_02002d2c
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x8000
	.2byte 0xffff
.L_02002d2c:
	.2byte 0x2301
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	adds	r6, #40
	adds	r5, #28
	cmp	r2, #0
	bge.n	.L_02002cde
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r2, [sp, #0]
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	adds	r2, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	fp, r3
	cmp	r2, #0
	ble.n	.L_02002df0
	adds	r7, r2, #0
.L_02002d6c:
	bl 0x0200af08
	movs	r1, #176
	lsls	r1, r1, #5
	add	r1, fp
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	mov	sl, r1
	lsls	r6, r3, #3
	subs	r6, r6, r3
	lsls	r6, r6, #2
	movs	r3, #160
	add	r6, fp
	lsls	r3, r3, #3
	adds	r5, r6, r3
	movs	r1, #0
	str	r1, [r5, #24]
	ldr	r2, [sp, #8]
	mov	r8, r1
	str	r2, [r5, #0]
	ldr	r3, [sp, #4]
	mov	r9, r0
	str	r3, [r5, #4]
	ldr	r1, [sp, #0]
	subs	r7, #1
	str	r1, [r5, #8]
	bl 0x0200af08
	movs	r2, #128
	lsls	r2, r2, #12
	lsls	r0, r0, #3
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r5, #0
	bl 0x0200af28
	mov	r3, r8
	str	r3, [r5, #12]
	movs	r3, #160
	lsls	r3, r3, #11
	mov	r1, r8
	str	r3, [r5, #16]
	str	r1, [r5, #20]
	bl 0x0200af08
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #12
	movs	r2, #128
	adds	r6, r6, r3
	lsls	r2, r2, #10
	lsls	r0, r0, #1
	adds	r0, r0, r2
	mov	r1, r9
	adds	r2, r6, #0
	bl 0x0200af28
	mov	r1, sl
	ldrh	r3, [r1, #0]
	movs	r2, #63
	adds	r3, #1
	ands	r3, r2
	mov	r2, sl
	strh	r3, [r2, #0]
	cmp	r7, #0
	bne.n	.L_02002d6c
.L_02002df0:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #8
	movs	r0, #220
	sub	sp, #4
	bl 0x0200af30
	adds	r6, r0, #0
	ldr	r0, [pc, #152]
	bl 0x0200af60
	adds	r1, r6, #0
	bl 0x0200af40
	bl 0x0200af58
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r2, r6, #0
	adds	r5, r0, #0
	bl 0x0200af50
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r1, #2
	mov	sl, r0
	adds	r3, r6, r1
	mov	r2, sl
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r5, [r3, #0]
	movs	r2, #160
	movs	r3, #192
	lsls	r2, r2, #3
	lsls	r3, r3, #4
	movs	r1, #63
	adds	r7, r6, r2
	adds	r5, r6, r3
	mov	r8, r1
.L_02002e58:
	mov	r2, sl
	movs	r3, #128
	str	r2, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #8
	lsls	r3, r3, #23
	bl 0x0200b190
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	movs	r1, #13
	strb	r3, [r5, #5]
	negs	r1, r1
	movs	r3, #15
	ands	r3, r2
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #240
	strh	r3, [r5, #30]
	subs	r3, #241
	add	r8, r3
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r5, #40
	adds	r7, #28
	cmp	r2, #0
	bge.n	.L_02002e58
	movs	r1, #176
	lsls	r1, r1, #5
	adds	r2, r6, r1
	movs	r3, #0
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl 0x0200aef8
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x000001f0
	.2byte 0xacc1
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #28]
	ldr	r5, [r3, #0]
	bl 0x0200af00
	movs	r3, #176
	lsls	r3, r3, #5
	adds	r3, #4
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl 0x0200af48
	movs	r0, #220
	bl 0x0200af38
	pop	{r5, pc}
	.4byte 0x0200acc1
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x000c000b
	.4byte 0x000e000d
	.4byte 0x0008ffff
	.4byte 0xffffffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0xa000e000
	.4byte 0x4000c000
	.4byte 0x60002000
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0xffffffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0xffffffff
	.4byte 0x4000c000
	.4byte 0xffffffff
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0xffffffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0x80000000
	.4byte 0x4000c000
	.4byte 0x80000000
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x0200b1b0
	.4byte 0x0200b1ec
	.4byte 0x0200b228
	.4byte 0x00070008
	.4byte 0x00090200
	.4byte 0x02010007
	.4byte 0x0007000a
	.4byte 0x000b0202
	.4byte 0x02030007
	.4byte 0x0008ffff
	.4byte 0x02000007
	.4byte 0x00070009
	.4byte 0x000a0201
	.4byte 0x02020007
	.4byte 0x0000ffff
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x80010000
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00500330
	.4byte 0x03400210
	.4byte 0x02200060
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000f00b0
	.4byte 0x00c000c0
	.4byte 0x00d00021
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000ce
	.4byte 0x1012e002
	.4byte 0xffffffff
	.4byte 0x102010cf
	.4byte 0xffffffff
	.4byte 0x000000cf
	.4byte 0x101020ce
	.4byte 0xffffffff
	.4byte 0x1020b0cf
	.4byte 0xffffffff
	.4byte 0x103070cf
	.4byte 0xffffffff
	.4byte 0x104080cf
	.4byte 0xffffffff
	.4byte 0x105050d3
	.4byte 0xffffffff
	.4byte 0x106100cf
	.4byte 0xffffffff
	.4byte 0x107030cf
	.4byte 0xffffffff
	.4byte 0x108040cf
	.4byte 0xffffffff
	.4byte 0x109120cf
	.4byte 0xffffffff
	.4byte 0x10a090d0
	.4byte 0xffffffff
	.4byte 0x10b020cf
	.4byte 0xffffffff
	.4byte 0x10c040d3
	.4byte 0xffffffff
	.4byte 0x10d0f0cf
	.4byte 0xffffffff
	.4byte 0x10e110cf
	.4byte 0xffffffff
	.4byte 0x10f0d0cf
	.4byte 0xffffffff
	.4byte 0x110060cf
	.4byte 0xffffffff
	.4byte 0x1110e0cf
	.4byte 0xffffffff
	.4byte 0x112090cf
	.4byte 0xffffffff
	.4byte 0x000000d0
	.4byte 0x101030d0
	.4byte 0xffffffff
	.4byte 0x102070d0
	.4byte 0xffffffff
	.4byte 0x103010d0
	.4byte 0xffffffff
	.4byte 0x1040a0d0
	.4byte 0xffffffff
	.4byte 0x105020d1
	.4byte 0xffffffff
	.4byte 0x1060b0d0
	.4byte 0xffffffff
	.4byte 0x107020d0
	.4byte 0xffffffff
	.4byte 0x1080c0d0
	.4byte 0xffffffff
	.4byte 0x1090a0cf
	.4byte 0xffffffff
	.4byte 0x10a040d0
	.4byte 0xffffffff
	.4byte 0x10b060d0
	.4byte 0xffffffff
	.4byte 0x10c080d0
	.4byte 0xffffffff
	.4byte 0x000000d1
	.4byte 0x101020d2
	.4byte 0xffffffff
	.4byte 0x102050d0
	.4byte 0xffffffff
	.4byte 0x103060d2
	.4byte 0xffffffff
	.4byte 0x104050d2
	.4byte 0xffffffff
	.4byte 0x105060d1
	.4byte 0xffffffff
	.4byte 0x106050d1
	.4byte 0xffffffff
	.4byte 0x000000d2
	.4byte 0x101030d2
	.4byte 0xffffffff
	.4byte 0x102010d1
	.4byte 0xffffffff
	.4byte 0x103010d2
	.4byte 0xffffffff
	.4byte 0x104080d2
	.4byte 0xffffffff
	.4byte 0x105040d1
	.4byte 0xffffffff
	.4byte 0x106030d1
	.4byte 0xffffffff
	.4byte 0x107010d3
	.4byte 0xffffffff
	.4byte 0x108040d2
	.4byte 0xffffffff
	.4byte 0x109020d3
	.4byte 0xffffffff
	.4byte 0x10a090d3
	.4byte 0xffffffff
	.4byte 0x10b070d1
	.4byte 0xffffffff
	.4byte 0x000000d3
	.4byte 0x101070d2
	.4byte 0xffffffff
	.4byte 0x102090d2
	.4byte 0xffffffff
	.4byte 0x103020d4
	.4byte 0xffffffff
	.4byte 0x1040c0cf
	.4byte 0xffffffff
	.4byte 0x105050cf
	.4byte 0xffffffff
	.4byte 0x106010d4
	.4byte 0xffffffff
	.4byte 0x107080d3
	.4byte 0xffffffff
	.4byte 0x108070d3
	.4byte 0xffffffff
	.4byte 0x1090a0d2
	.4byte 0xffffffff
	.4byte 0x10a0a0d3
	.4byte 0xffffffff
	.4byte 0x000000d4
	.4byte 0x101060d3
	.4byte 0xffffffff
	.4byte 0x102030d3
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x006400f5
	.4byte 0x00000007
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020085d1
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x020085d1
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000003
	.4byte 0xffff0034
	.4byte 0x02008921
	.4byte 0x50008a05
	.4byte 0x09420032
	.4byte 0x02008549
	.4byte 0x50008a05
	.4byte 0x09430033
	.4byte 0x0200858d
	.4byte 0x50009705
	.4byte 0x09440034
	.4byte 0x020087dd
	.4byte 0x00009705
	.4byte 0x09440034
	.4byte 0x020088a9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte 0x0200895d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte 0x0200895d
	.4byte 0x00000002
	.4byte 0x02000033
	.4byte 0x02008965
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008281
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008b3d
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte 0x0200895d
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x02008b35
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x02008b35
	.4byte 0x50008615
	.4byte 0x0202000a
	.4byte 0x02008b35
	.4byte 0x50008615
	.4byte 0x0203000b
	.4byte 0x02008b35
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff001e
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x02008f01
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte 0x02008b35
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte 0x02008b35
	.4byte 0x50008615
	.4byte 0x0202000a
	.4byte 0x02008b35
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte 0x02008bfd
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x02008d2d
	.4byte 0x80008c15
	.4byte 0xffff000b
	.4byte 0x02008fad
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte 0x02008bfd
	.4byte 0x50008c15
	.4byte 0xffff000b
	.4byte 0x02008c65
	.4byte 0x10008c15
	.4byte 0x0946000c
	.4byte 0x02008bfd
	.4byte 0x00008c15
	.4byte 0x0946000c
	.4byte 0x02008d2d
	.4byte 0x10008c15
	.4byte 0x0947000d
	.4byte 0x02008bfd
	.4byte 0x00008c15
	.4byte 0x0947000d
	.4byte 0x02008d2d
	.4byte 0x80008c15
	.4byte 0xffff000e
	.4byte 0x02008fad
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte 0x02008bfd
	.4byte 0x50008c15
	.4byte 0xffff000e
	.4byte 0x02008c65
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008e79
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008de5
	.4byte 0x80009705
	.4byte 0xffff0034
	.4byte 0x02008fad
	.4byte 0x50009705
	.4byte 0x094b0034
	.4byte 0x02008f0d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte 0x02008bfd
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008d2d
	.4byte 0x50009705
	.4byte 0x09480032
	.4byte 0x02008fb9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
