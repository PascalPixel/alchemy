.syntax unified
	.thumb
	.set sub_08016ce4, 0x08016ce4
	.set sub_08020090, 0x08020090
	.set sub_080200c0, 0x080200c0
	.set sub_08020138, 0x08020138
	.set sub_080201c0, 0x080201c0
	.set sub_08020218, 0x08020218
	.set sub_080ca9cc, 0x080ca9cc
	.set sub_080cad84, 0x080cad84
	.set sub_080cdfb0, 0x080cdfb0
	.set sub_080d3b28, 0x080d3b28
	.global Func_080ceb80
	.thumb_func
Func_080ceb80:
	push	{lr}
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	bl	sub_080cdfb0
	adds	r3, r0, #0
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	pop	{pc}
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl	sub_080201c0
	str	r0, [r5, #12]
	str	r0, [r5, #20]
	pop	{r5, pc}
.L_080ceba8:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #112]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldr	r5, [pc, #104]
	movs	r7, #0
	ldrh	r0, [r5, #0]
	b.n	.L_080cebea
.L_080cebbe:
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	ldrh	r0, [r5, #0]
	cmp	r2, #0
	beq.n	.L_080cebea
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080cebea
	mov	ip, r1
.L_080cebd6:
	adds	r5, #4
	movs	r3, #0
	ldrsh	r2, [r5, r3]
	ldrh	r0, [r5, #0]
	cmp	r2, #0
	beq.n	.L_080cebea
	mov	r3, ip
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cebd6
.L_080cebea:
	lsls	r3, r0, #16
	adds	r5, #2
	asrs	r2, r3, #16
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	adds	r5, #2
	cmp	r2, #0
	beq.n	.L_080cec18
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	ands	r3, r2
	cmp	r3, r6
	bne.n	.L_080cebbe
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_080cec16
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080cebbe
.L_080cec16:
	adds	r7, r5, #0
.L_080cec18:
	adds	r0, r7, #0
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x2204
	.2byte 0x080f
.L_080cec24:
	push	{lr}
	movs	r3, #0
	str	r3, [r1, #0]
	str	r3, [r1, #4]
	str	r3, [r1, #8]
	bl	sub_080ca9cc
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	bl	.L_080ceba8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	mov	r8, r0
	mov	fp, r3
	cmp	r0, #0
	bne.n	.L_080cec58
	b.n	.L_080cee2c
.L_080cec58:
	movs	r2, #0
	ldrsh	r7, [r0, r2]
	movs	r1, #0
	movs	r3, #2
	mov	r9, r1
	add	r8, r3
	cmp	r7, #0
	bne.n	.L_080cec6a
	b.n	.L_080cee2c
.L_080cec6a:
	movs	r3, #128
	lsls	r3, r3, #7
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080cec76
	b.n	.L_080cee2c
.L_080cec76:
	mov	r2, r8
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	movs	r1, #0
	ldrsh	r6, [r2, r1]
	mov	sl, r3
	movs	r3, #4
	ldrsh	r2, [r2, r3]
	movs	r1, #6
	str	r2, [sp, #0]
	add	r8, r1
	cmp	r7, #128
	beq.n	.L_080ceca4
	movs	r2, #1
	negs	r2, r2
	cmp	sl, r2
	beq.n	.L_080ceca4
	mov	r0, sl
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ceca4
	b.n	.L_080cee08
.L_080ceca4:
	add	r5, sp, #4
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	.L_080cec24
	cmp	r0, #0
	beq.n	.L_080cecb4
	b.n	.L_080cee08
.L_080cecb4:
	adds	r3, r7, #0
	subs	r3, #128
	movs	r1, #128
	movs	r0, #232
	lsls	r3, r3, #16
	lsls	r1, r1, #9
	movs	r6, #0
	adds	r0, #255
	cmp	r3, r1
	bls.n	.L_080cece6
	movs	r0, #10
	adds	r0, #255
	cmp	r7, #130
	beq.n	.L_080cece6
	movs	r0, #161
	lsls	r0, r0, #1
	cmp	r7, #132
	beq.n	.L_080cece6
	movs	r0, #234
	adds	r0, #255
	cmp	r7, #131
	beq.n	.L_080cece6
	movs	r0, #234
	movs	r6, #1
	adds	r0, #255
.L_080cece6:
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	bl	sub_080200c0
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_080cecf8
	b.n	.L_080cee08
.L_080cecf8:
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, fp
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080ced18
	adds	r1, r5, #0
	movs	r2, #0
	adds	r1, #85
	movs	r3, #2
	strb	r3, [r1, #0]
	str	r2, [r5, #20]
	str	r2, [r5, #12]
	b.n	.L_080ced70
.L_080ced18:
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	movs	r0, #0
	bl	sub_080201c0
	ldr	r3, [r5, #12]
	str	r0, [r5, #20]
	adds	r3, r3, r0
	str	r3, [r5, #12]
	cmp	r6, #0
	beq.n	.L_080ced70
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r2, [pc, #144]
	ldr	r3, [r5, #20]
	ldr	r1, [pc, #144]
	adds	r3, r3, r2
	str	r3, [r5, #20]
	str	r3, [r5, #12]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	ldr	r3, [r5, #8]
	asrs	r4, r3, #20
	ldr	r3, [r5, #16]
	asrs	r0, r3, #20
	cmp	r2, #0
	beq.n	.L_080ced5c
	movs	r1, #156
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldr	r1, [r3, #0]
.L_080ced5c:
	lsls	r3, r0, #7
	adds	r3, r4, r3
	lsls	r3, r3, #2
	adds	r1, r1, r3
	ldrb	r3, [r1, #2]
	subs	r3, #242
	cmp	r3, #5
	bhi.n	.L_080ced70
	movs	r3, #0
	strb	r3, [r1, #2]
.L_080ced70:
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08020090
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020218
	mov	r0, sl
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ced92
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08020090
.L_080ced92:
	adds	r0, r5, #0
	bl	sub_08020138
	ldr	r2, [r5, #8]
	cmp	r2, #0
	bge.n	.L_080ceda6
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_080ceda6:
	adds	r3, r5, #0
	adds	r3, #100
	asrs	r2, r2, #16
	strh	r2, [r3, #0]
	ldr	r1, [pc, #20]
	ldr	r3, [r5, #16]
	mov	sl, r1
	cmp	r3, #0
	bge.n	.L_080cedd0
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
	b.n	.L_080cedd0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xfff00000
	.2byte 0x0000
	.2byte 0x0201
.L_080cedd0:
	adds	r2, r5, #0
	asrs	r3, r3, #16
	adds	r2, #102
	strh	r3, [r2, #0]
	adds	r6, r5, #0
	movs	r3, #1
	subs	r2, #67
	strb	r3, [r2, #0]
	mov	r1, r9
	adds	r6, #89
	movs	r2, #138
	strb	r3, [r6, #0]
	lsls	r2, r2, #1
	lsls	r3, r1, #2
	adds	r3, r3, r2
	mov	r1, fp
	str	r5, [r1, r3]
	cmp	r7, #131
	bne.n	.L_080cee00
	mov	r0, r9
	adds	r0, #64
	ldr	r1, [sp, #0]
	bl	sub_080d3b28
.L_080cee00:
	cmp	r7, #133
	bne.n	.L_080cee08
	mov	r2, sl
	strb	r2, [r6, #0]
.L_080cee08:
	movs	r3, #1
	add	r9, r3
	mov	r1, r9
	cmp	r1, #15
	bgt.n	.L_080cee2c
	mov	r3, r8
	movs	r2, #0
	ldrsh	r7, [r3, r2]
	movs	r1, #2
	add	r8, r1
	cmp	r7, #0
	beq.n	.L_080cee2c
	ldr	r2, [pc, #24]
	adds	r3, r7, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080cee2c
	b.n	.L_080cec76
.L_080cee2c:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x4000
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080cee7a
	cmp	r5, #0
	bne.n	.L_080cee5a
	ldr	r3, [pc, #40]
	str	r5, [r0, #8]
	str	r5, [r0, #16]
	str	r3, [r0, #76]
	b.n	.L_080cee7a
.L_080cee5a:
	ldr	r0, [r0, #80]
	cmp	r0, #0
	beq.n	.L_080cee7a
	ldrb	r3, [r0, #23]
	ldrb	r2, [r0, #5]
	adds	r3, #4
	movs	r1, #63
	strb	r3, [r0, #23]
	adds	r3, r1, #0
	ands	r3, r2
	movs	r2, #64
	orrs	r3, r2
	strb	r3, [r0, #5]
	ldrb	r3, [r0, #7]
	ands	r1, r3
	strb	r1, [r0, #7]
.L_080cee7a:
	pop	{r5, pc}
	.2byte 0x5927
	.2byte 0x3141
	push	{lr}
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080ceea8
	ldr	r0, [r0, #80]
	cmp	r0, #0
	beq.n	.L_080ceea8
	ldrb	r1, [r0, #5]
	movs	r3, #8
	movs	r2, #63
	strb	r3, [r0, #23]
	adds	r3, r2, #0
	ands	r3, r1
	strb	r3, [r0, #5]
	ldrb	r3, [r0, #7]
	ands	r2, r3
	movs	r3, #64
	orrs	r2, r3
	strb	r2, [r0, #7]
.L_080ceea8:
	pop	{pc}
	.align 2, 0
