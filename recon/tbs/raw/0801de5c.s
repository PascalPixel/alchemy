.syntax unified
	.thumb
	.set sub_08002df0, 0x08002df0
	.set sub_08002f40, 0x08002f40
	.set sub_08004938, 0x08004938
	.set sub_080072f0, 0x080072f0
	.global UiText_RenderStringTiles
	.global Func_0801de5c
	.thumb_func
UiText_RenderStringTiles:
Func_0801de5c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	mov	r8, r3
	ldr	r3, [pc, #724]
	movs	r5, #128
	ldr	r3, [r3, #0]
	lsls	r5, r5, #4
	adds	r6, r0, #0
	adds	r0, r5, #0
	mov	fp, r3
	bl	sub_08004938
	str	r0, [sp, #8]
	ldr	r0, [pc, #708]
	bl	sub_08002f40
	ldr	r3, [pc, #704]
	str	r0, [sp, #4]
	add	r3, fp
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #12
	str	r3, [sp, #0]
	movs	r1, #16
	ldr	r3, [pc, #696]
	add	r0, sp, #20
	bl	sub_080072f0
	movs	r2, #240
	ldr	r1, [sp, #0]
	lsls	r2, r2, #8
	cmp	r1, r2
	bne.n	.L_0801ded4
	add	r3, sp, #20
	mov	sl, r3
	ldr	r3, [pc, #676]
	add	r3, fp
	ldrh	r2, [r3, #0]
	ldr	r1, [pc, #676]
	movs	r3, #15
	ands	r3, r2
	ldrb	r3, [r1, r3]
	mov	r4, sl
	strb	r3, [r4, #1]
	movs	r3, #3
	strb	r3, [r4, #3]
	ldr	r0, [sp, #8]
	ldr	r3, [pc, #660]
	adds	r1, r5, #0
	ldr	r2, [pc, #660]
	bl	sub_080072f0
	b.n	.L_0801def6
.L_0801ded4:
	ldr	r3, [pc, #640]
	add	r3, fp
	ldrb	r2, [r3, #0]
	movs	r7, #20
	movs	r3, #15
	add	r7, sp
	ands	r3, r2
	strb	r3, [r7, #1]
	movs	r3, #1
	strb	r3, [r7, #3]
	ldr	r0, [sp, #8]
	ldr	r3, [pc, #628]
	adds	r1, r5, #0
	ldr	r2, [pc, #632]
	mov	sl, r7
	bl	sub_080072f0
.L_0801def6:
	cmp	r6, #0
	bne.n	.L_0801defc
	b.n	.L_0801e042
.L_0801defc:
	b.n	.L_0801e036
.L_0801defe:
	cmp	r1, #30
	bhi.n	.L_0801dfb6
	subs	r3, r1, #3
	cmp	r3, #26
	bls.n	.L_0801df0a
	b.n	.L_0801e036
.L_0801df0a:
	ldr	r2, [pc, #608]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0801dfaa
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801df98
	.4byte 0x0801df80
	.4byte 0x0801df98
	.4byte 0x0801df98
	.4byte 0x0801dfb2
	.4byte 0x0801dfb2
	.4byte 0x0801e036
	.4byte 0x0801dfb0
	.4byte 0x0801dfb0
	.4byte 0x0801e036
	.4byte 0x0801dfb2
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801e036
	.4byte 0x0801dfb0
	.2byte 0xdfb2
	.2byte 0x0801
	ldr	r3, [pc, #468]
	ldrh	r1, [r6, #0]
	add	r3, fp
	strh	r1, [r3, #0]
	ldr	r2, [pc, #464]
	movs	r3, #15
	ands	r3, r1
	ldrb	r3, [r2, r3]
	mov	r1, sl
	adds	r6, #2
	strb	r3, [r1, #1]
	b.n	.L_0801e036
	ldr	r3, [pc, #444]
	movs	r2, #15
	add	r3, fp
	strh	r2, [r3, #0]
	ldr	r3, [pc, #440]
	ldrb	r3, [r3, r2]
	mov	r2, sl
	strb	r3, [r2, #1]
	b.n	.L_0801e036
	ldr	r3, [pc, #452]
	ldrb	r3, [r3, #0]
	b.n	.L_0801e034
	adds	r6, #2
	adds	r6, #2
	b.n	.L_0801e036
.L_0801dfb6:
	movs	r3, #255
	ands	r1, r3
	ldr	r4, [sp, #4]
	ldr	r0, [sp, #8]
	lsls	r3, r1, #5
	movs	r7, #0
	adds	r5, r4, r3
	add	r0, r8
	mov	r9, r7
	mov	lr, sl
.L_0801dfca:
	ldmia	r5!, {r2}
	movs	r4, #3
.L_0801dfce:
	movs	r7, #15
	adds	r3, r2, #0
	ands	r3, r7
	mov	r7, lr
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_0801dfde
	strb	r3, [r0, #0]
.L_0801dfde:
	lsrs	r2, r2, #4
	movs	r7, #15
	adds	r3, r2, #0
	ands	r3, r7
	mov	r7, sl
	ldrb	r3, [r7, r3]
	adds	r0, #1
	cmp	r3, #0
	beq.n	.L_0801dff2
	strb	r3, [r0, #0]
.L_0801dff2:
	subs	r4, #1
	adds	r0, #1
	lsrs	r2, r2, #4
	cmp	r4, #0
	bge.n	.L_0801dfce
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r0, #248
	cmp	r3, #7
	ble.n	.L_0801dfca
	ldr	r4, [pc, #360]
	cmp	ip, r4
	beq.n	.L_0801e014
	ldr	r7, [pc, #360]
	cmp	ip, r7
	bne.n	.L_0801e01a
.L_0801e014:
	movs	r1, #8
	add	r8, r1
	b.n	.L_0801e036
.L_0801e01a:
	ldr	r2, [pc, #352]
	cmp	ip, r2
	bne.n	.L_0801e024
	movs	r3, #3
	b.n	.L_0801e034
.L_0801e024:
	cmp	r1, #31
	bls.n	.L_0801e032
	ldr	r2, [pc, #324]
	adds	r3, r1, #0
	subs	r3, #32
	ldrb	r3, [r2, r3]
	b.n	.L_0801e034
.L_0801e032:
	movs	r3, #1
.L_0801e034:
	add	r8, r3
.L_0801e036:
	ldrh	r1, [r6, #0]
	adds	r6, #2
	mov	ip, r1
	cmp	r1, #0
	beq.n	.L_0801e042
	b.n	.L_0801defe
.L_0801e042:
	mov	r3, r8
	adds	r3, #7
	lsrs	r6, r3, #3
	ldr	r5, [sp, #8]
	movs	r1, #128
	lsls	r4, r6, #2
	lsls	r7, r6, #3
	lsls	r1, r1, #1
	movs	r2, #7
	adds	r0, r5, #0
	mov	sl, r4
	mov	r8, r7
	mov	lr, r1
	mov	r9, r2
.L_0801e05e:
	cmp	r6, #0
	beq.n	.L_0801e096
	ldr	r3, [pc, #284]
	ldr	r7, [pc, #284]
	mov	ip, r3
	adds	r4, r6, #0
.L_0801e06a:
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	lsls	r3, r1, #4
	orrs	r1, r3
	lsrs	r3, r2, #4
	orrs	r2, r3
	mov	r3, ip
	ands	r1, r3
	ands	r2, r7
	lsls	r3, r1, #8
	orrs	r1, r3
	lsrs	r3, r2, #8
	orrs	r2, r3
	lsls	r3, r1, #4
	lsrs	r3, r3, #16
	lsls	r2, r2, #16
	orrs	r3, r2
	subs	r4, #1
	adds	r5, #8
	stmia	r0!, {r3}
	cmp	r4, #0
	bne.n	.L_0801e06a
.L_0801e096:
	mov	r4, sl
	movs	r2, #1
	subs	r3, r0, r4
	mov	r7, lr
	mov	r1, r8
	negs	r2, r2
	adds	r0, r3, r7
	add	r9, r2
	subs	r3, r5, r1
	adds	r5, r3, r7
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_0801e05e
	cmp	r6, #0
	bne.n	.L_0801e0b6
	b.n	.L_0801e22a
.L_0801e0b6:
	ldr	r4, [pc, #208]
	movs	r7, #234
	movs	r1, #218
	ldr	r2, [pc, #204]
	add	r4, fp
	lsls	r7, r7, #4
	lsls	r1, r1, #4
	ldr	r0, [sp, #8]
	mov	lr, r4
	add	r7, fp
	mov	ip, r1
	mov	r8, r2
	mov	r9, r6
.L_0801e0d0:
	mov	r3, lr
	ldrb	r2, [r3, #0]
	movs	r5, #127
	cmp	r2, #0
	beq.n	.L_0801e0dc
	movs	r5, #255
.L_0801e0dc:
	ldr	r4, [sp, #16]
	ldrh	r3, [r4, #0]
	mov	r1, r8
	ands	r1, r3
	adds	r3, r1, #0
	subs	r3, #128
	cmp	r3, #127
	bls.n	.L_0801e1a0
	cmp	r2, #0
	beq.n	.L_0801e100
	movs	r2, #128
	lsls	r2, r2, #2
	cmp	r1, r2
	bcc.n	.L_0801e100
	movs	r3, #160
	lsls	r3, r3, #2
	cmp	r1, r3
	bcc.n	.L_0801e1a0
.L_0801e100:
	ldrh	r1, [r7, #0]
	mov	r2, ip
	ands	r1, r5
	adds	r3, r1, r2
	mov	r2, fp
	ldrb	r3, [r2, r3]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_0801e12a
.L_0801e112:
	adds	r1, #1
	adds	r4, #1
	ands	r1, r5
	cmp	r4, r5
	bhi.n	.L_0801e12a
	movs	r2, #218
	lsls	r2, r2, #4
	adds	r3, r1, r2
	mov	r2, fp
	ldrb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_0801e112
.L_0801e12a:
	adds	r3, r1, #1
	ands	r3, r5
	strh	r3, [r7, #0]
	mov	r3, ip
	adds	r2, r1, r3
	mov	r4, fp
	movs	r3, #1
	strb	r3, [r4, r2]
	cmp	r1, #127
	bls.n	.L_0801e190
	movs	r2, #192
	lsls	r2, r2, #1
	adds	r1, r1, r2
	b.n	.L_0801e194
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000013
	.4byte 0x00000ea7
	.4byte 0x03000164
	.4byte 0x00000eae
	.4byte 0x080371b4
	.4byte 0x03000168
	.4byte 0x04040404
	.4byte 0x0e0e0e0e
	.4byte 0x0801df14
	.4byte 0x080370d4
	.4byte 0x0000f01d
	.4byte 0x0000f01f
	.4byte 0x0000f01e
	.4byte 0x0ff00ff0
	.4byte 0x00ff00ff
	.4byte 0x00000ea2
	.2byte 0x03ff
	.2byte 0x0000
.L_0801e190:
	movs	r3, #128
	orrs	r1, r3
.L_0801e194:
	ldr	r3, [sp, #0]
	ldr	r4, [sp, #16]
	orrs	r3, r1
	strh	r3, [r4, #0]
	ldr	r2, [sp, #12]
	strh	r3, [r2, #0]
.L_0801e1a0:
	movs	r3, #192
	lsls	r2, r1, #5
	lsls	r3, r3, #19
	adds	r1, r2, r3
	ldr	r4, [pc, #152]
	ldr	r3, [r0, #0]
	str	r3, [r1, #0]
	adds	r1, r2, r4
	movs	r4, #128
	lsls	r4, r4, #1
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #128
	str	r3, [r1, #0]
	ldr	r3, [pc, #136]
	lsls	r4, r4, #2
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r1, #0]
	ldr	r3, [pc, #128]
	lsls	r4, r4, #2
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #128
	str	r3, [r1, #0]
	ldr	r3, [pc, #116]
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #160
	str	r3, [r1, #0]
	ldr	r3, [pc, #108]
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r1, #0]
	ldr	r3, [pc, #96]
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #224
	str	r3, [r1, #0]
	ldr	r3, [pc, #88]
	lsls	r4, r4, #3
	adds	r1, r2, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	str	r3, [r1, #0]
	movs	r3, #1
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	negs	r3, r3
	add	r9, r3
	adds	r1, #2
	adds	r2, #2
	mov	r4, r9
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	adds	r0, #4
	cmp	r4, #0
	beq.n	.L_0801e22a
	b.n	.L_0801e0d0
.L_0801e22a:
	ldr	r0, [sp, #8]
	bl	sub_08002df0
	adds	r0, r6, #0
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x06000004
	.4byte 0x06000008
	.4byte 0x0600000c
	.4byte 0x06000010
	.4byte 0x06000014
	.4byte 0x06000018
	.4byte 0x0600001c
