.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08009080, 0x08009080
	.set sub_08015130, 0x08015130
	.set sub_080b6c08, 0x080b6c08
	.set sub_080b6c90, 0x080b6c90
	.set sub_080b7b6c, 0x080b7b6c
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_080b7f70, 0x080b7f70
	.set sub_080b9d34, 0x080b9d34
	.set sub_080bb938, 0x080bb938
	.set sub_080bbabc, 0x080bbabc
	.set sub_080be02c, 0x080be02c
	.set sub_080c0cec, 0x080c0cec
	.set sub_080c0f98, 0x080c0f98
	.set sub_080c10e8, 0x080c10e8
	.set sub_080c1798, 0x080c1798
	.set sub_080c1a14, 0x080c1a14
	.set sub_080c9008, 0x080c9008
	.set sub_080c9010, 0x080c9010
	.set sub_080c9018, 0x080c9018
	.set sub_080f9010, 0x080f9010
	.global BattlePresentation_RunUnitTransition
	.global Func_080b9ec0
	.thumb_func
BattlePresentation_RunUnitTransition:
Func_080b9ec0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #124
	add	r7, sp, #12
	str	r1, [sp, #8]
	adds	r1, r7, #0
	mov	r9, r0
	ldr	r5, [pc, #56]
	bl	sub_080b9d34
	mov	r0, r9
	ldrb	r0, [r0, #0]
	str	r0, [sp, #4]
	mov	r1, r9
	ldrb	r1, [r1, #2]
	str	r1, [sp, #0]
	mov	r2, r9
	ldr	r3, [r2, #88]
	movs	r2, #128
	lsls	r2, r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b9f18
	adds	r3, r5, #0
	adds	r3, #140
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #6
	cmp	r0, #7
	bls.n	.L_080b9f0a
	movs	r3, #160
	lsls	r3, r3, #7
.L_080b9f0a:
	str	r3, [r2, #0]
	movs	r3, #60
	str	r3, [r2, #4]
	b.n	.L_080b9f32
	movs	r0, r0
	.2byte 0x1e74
	.2byte 0x0300
.L_080b9f18:
	adds	r3, r5, #0
	adds	r3, #140
	ldr	r1, [r3, #0]
	ldr	r3, [sp, #4]
	ldr	r2, [pc, #84]
	cmp	r3, #7
	bhi.n	.L_080b9f2a
	movs	r2, #128
	lsls	r2, r2, #6
.L_080b9f2a:
	ldr	r3, [r1, #0]
	cmp	r3, r2
	beq.n	.L_080b9f32
	str	r2, [r1, #0]
.L_080b9f32:
	movs	r1, #0
	movs	r0, #0
	bl	sub_080c10e8
	ldr	r3, [pc, #64]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	sub_08015130
	ldr	r0, [sp, #4]
	bl	sub_080b7dd0
	ldr	r0, [r0, #0]
	mov	fp, r0
	add	r0, sp, #96
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #24]
	mov	sl, r0
	strh	r3, [r2, #0]
	movs	r0, #3
	mov	r1, sl
	bl	sub_080b6c08
	movs	r6, #0
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L_080b9fc2
	movs	r5, #0
	b.n	.L_080b9f84
	.4byte 0x00003f40
	.4byte 0xffffe000
	.4byte 0x03001e74
	.2byte 0x0050
	.2byte 0x0400
.L_080b9f84:
	mov	r1, sl
	ldrh	r3, [r5, r1]
	cmp	r3, #254
	beq.n	.L_080b9fba
	ldr	r2, [sp, #4]
	adds	r0, r3, #0
	cmp	r0, r2
	beq.n	.L_080b9fb2
	ldr	r3, [sp, #0]
	movs	r2, #0
	cmp	r3, #7
	bhi.n	.L_080b9f9e
	movs	r2, #1
.L_080b9f9e:
	movs	r3, #0
	cmp	r0, #7
	bhi.n	.L_080b9fa6
	movs	r3, #1
.L_080b9fa6:
	cmp	r2, r3
	beq.n	.L_080b9fba
	movs	r1, #1
	bl	sub_080c0f98
	b.n	.L_080b9fba
.L_080b9fb2:
	mov	r0, fp
	movs	r1, #3
	bl	sub_08009080
.L_080b9fba:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r8
	bne.n	.L_080b9f84
.L_080b9fc2:
	movs	r0, #154
	bl	sub_080f9010
	mov	r2, r9
	ldr	r1, [r2, #80]
	movs	r3, #0
	ldr	r0, [r7, #8]
	movs	r2, #0
	bl	sub_080c1798
	ldr	r0, [sp, #8]
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080b9fe8
	ldr	r0, [sp, #4]
	movs	r1, #1
	bl	sub_080c0f98
.L_080b9fe8:
	ldr	r1, [pc, #40]
	ldr	r5, [pc, #44]
	movs	r6, #0
	mov	fp, r1
.L_080b9ff0:
	mov	r2, fp
	subs	r3, r2, r6
	ldr	r0, [pc, #36]
	orrs	r3, r5
	strh	r3, [r0, #0]
	adds	r6, #1
	movs	r0, #1
	bl	sub_080030f8
	cmp	r6, #16
	bne.n	.L_080b9ff0
	mov	r1, r9
	ldr	r3, [r1, #92]
	cmp	r3, #0
	beq.n	.L_080ba044
	cmp	r3, #1
	bne.n	.L_080ba032
	b.n	.L_080ba020
	.4byte 0x00000010
	.4byte 0x00001000
	.2byte 0x0052
	.2byte 0x0400
.L_080ba020:
	ldrb	r1, [r1, #0]
	movs	r0, #0
	bl	sub_080bbabc
	ldr	r1, [pc, #72]
	movs	r0, #4
	bl	sub_080bbabc
	b.n	.L_080ba03a
.L_080ba032:
	ldr	r1, [pc, #68]
	movs	r0, #4
	bl	sub_080bbabc
.L_080ba03a:
	bl	sub_080bb938
	bl	sub_080c1a14
	b.n	.L_080ba1ba
.L_080ba044:
	movs	r6, #0
	movs	r2, #0
	cmp	r6, r8
	bcs.n	.L_080ba09e
	ldr	r0, [sp, #8]
	movs	r3, #1
	ands	r0, r3
	mov	ip, r0
	mov	r5, sl
	mov	r1, sl
.L_080ba058:
	ldrh	r3, [r5, #0]
	ldr	r0, [sp, #4]
	adds	r5, #2
	cmp	r3, r0
	bne.n	.L_080ba07c
	mov	r3, ip
	cmp	r3, #0
	bne.n	.L_080ba098
	add	r0, sp, #4
	ldrh	r0, [r0, #0]
	adds	r2, #1
	strh	r0, [r1, #0]
	b.n	.L_080ba096
	movs	r0, r0
	.4byte 0x00000856
	.2byte 0x0855
	.2byte 0x0000
.L_080ba07c:
	ldr	r0, [sp, #0]
	movs	r4, #0
	cmp	r0, #7
	bls.n	.L_080ba086
	movs	r4, #1
.L_080ba086:
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L_080ba08e
	movs	r0, #1
.L_080ba08e:
	cmp	r4, r0
	beq.n	.L_080ba098
	strh	r3, [r1, #0]
	adds	r2, #1
.L_080ba096:
	adds	r1, #2
.L_080ba098:
	adds	r6, #1
	cmp	r6, r8
	bcc.n	.L_080ba058
.L_080ba09e:
	ldr	r3, [pc, #36]
	lsls	r2, r2, #1
	mov	r1, sl
	strh	r3, [r1, r2]
	mov	r0, sl
	movs	r1, #0
	bl	sub_080b7b6c
	mov	r2, r9
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	movs	r0, #0
	cmp	r0, r3
	bge.n	.L_080ba0d8
	mov	ip, r3
	mov	r1, sl
	adds	r2, #2
	mov	r0, ip
	b.n	.L_080ba0c8
	.2byte 0x00ff
	.2byte 0x0000
.L_080ba0c8:
	ldrb	r3, [r2, #0]
	subs	r0, #1
	strh	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #2
	cmp	r0, #0
	bne.n	.L_080ba0c8
	mov	r0, ip
.L_080ba0d8:
	ldr	r2, [pc, #20]
	lsls	r3, r0, #1
	mov	r0, sl
	strh	r2, [r0, r3]
	ldr	r3, [r7, #20]
	movs	r6, #0
	adds	r2, r7, #0
	cmp	r3, #0
	beq.n	.L_080ba138
	movs	r5, #0
	b.n	.L_080ba0f4
	movs	r0, r0
	.2byte 0x00ff
	.2byte 0x0000
.L_080ba0f4:
	lsls	r3, r6, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_080b7dd0
	movs	r1, #0
	ldr	r0, [r0, #0]
	bl	sub_080b7f70
	adds	r3, r0, #0
	adds	r3, #39
	ldrb	r3, [r3, #0]
	subs	r3, #1
	movs	r1, #0
	cmp	r3, #0
	beq.n	.L_080ba12c
	mov	ip, r3
	adds	r3, r5, r7
	adds	r2, r3, #0
	adds	r2, #52
	adds	r0, #40
.L_080ba11e:
	ldmia	r0!, {r3}
	ldrb	r3, [r3, #5]
	adds	r1, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, ip
	bne.n	.L_080ba11e
.L_080ba12c:
	ldr	r3, [r7, #20]
	adds	r6, #1
	adds	r5, #4
	adds	r2, r7, #0
	cmp	r6, r3
	bne.n	.L_080ba0f4
.L_080ba138:
	mov	r3, r9
	ldr	r2, [r3, #88]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080ba152
	ldr	r0, [sp, #0]
	cmp	r0, #7
	bls.n	.L_080ba15a
	movs	r3, #0
	str	r3, [r7, #4]
	b.n	.L_080ba162
.L_080ba152:
	mov	r1, r9
	ldrb	r3, [r1, #2]
	cmp	r3, #7
	bhi.n	.L_080ba160
.L_080ba15a:
	movs	r3, #1
	str	r3, [r7, #4]
	b.n	.L_080ba162
.L_080ba160:
	str	r2, [r7, #4]
.L_080ba162:
	mov	r2, r9
	ldr	r3, [r2, #88]
	movs	r2, #128
	lsls	r2, r2, #10
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ba178
	ldr	r3, [r7, #4]
	movs	r2, #1
	eors	r3, r2
	str	r3, [r7, #4]
.L_080ba178:
	movs	r1, #200
	ldr	r0, [pc, #48]
	lsls	r1, r1, #4
	bl	sub_080041d8
	mov	r3, r9
	ldr	r0, [r3, #88]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080ba198
	adds	r0, r7, #0
	bl	sub_080c9010
	b.n	.L_080ba1b6
.L_080ba198:
	movs	r3, #128
	lsls	r3, r3, #7
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080ba1b0
	adds	r0, r7, #0
	bl	sub_080c9008
	b.n	.L_080ba1b6
	movs	r0, r0
	.2byte 0xd899
	.2byte 0x080b
.L_080ba1b0:
	adds	r0, r7, #0
	bl	sub_080c9018
.L_080ba1b6:
	bl	sub_080be02c
.L_080ba1ba:
	bl	sub_080b6c90
	movs	r0, #3
	mov	r1, sl
	bl	sub_080b6c08
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #40]
	mov	r8, r0
	strh	r3, [r2, #0]
	movs	r6, #0
	cmp	r0, #0
	beq.n	.L_080ba216
	movs	r5, #0
.L_080ba1d6:
	mov	r0, sl
	ldrh	r3, [r5, r0]
	cmp	r3, #254
	beq.n	.L_080ba20e
	ldr	r1, [sp, #4]
	adds	r0, r3, #0
	cmp	r0, r1
	beq.n	.L_080ba20e
	ldr	r3, [sp, #0]
	movs	r2, #0
	cmp	r3, #7
	bhi.n	.L_080ba1fc
	movs	r2, #1
	b.n	.L_080ba1fc
	movs	r0, r0
	.4byte 0x00003f40
	.2byte 0x0050
	.2byte 0x0400
.L_080ba1fc:
	movs	r3, #0
	cmp	r0, #7
	bhi.n	.L_080ba204
	movs	r3, #1
.L_080ba204:
	cmp	r2, r3
	beq.n	.L_080ba20e
	movs	r1, #1
	bl	sub_080c0f98
.L_080ba20e:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r8
	bne.n	.L_080ba1d6
.L_080ba216:
	ldr	r7, [pc, #40]
	ldr	r5, [pc, #32]
	movs	r6, #0
.L_080ba21c:
	adds	r3, r6, #0
	orrs	r3, r5
	strh	r3, [r7, #0]
	movs	r0, #1
	adds	r6, #1
	bl	sub_080030f8
	cmp	r6, #16
	bne.n	.L_080ba21c
	mov	r0, r8
	movs	r6, #0
	cmp	r0, #0
	beq.n	.L_080ba254
	mov	r5, sl
	b.n	.L_080ba244
	movs	r0, r0
	.4byte 0x00001000
	.2byte 0x0052
	.2byte 0x0400
.L_080ba244:
	ldrh	r0, [r5, #0]
	movs	r1, #0
	adds	r6, #1
	adds	r5, #2
	bl	sub_080c0f98
	cmp	r6, r8
	bne.n	.L_080ba244
.L_080ba254:
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	movs	r0, #0
	bl	sub_080c0cec
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	add	sp, #124
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0000
