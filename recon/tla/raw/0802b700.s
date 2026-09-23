.syntax unified
	.thumb
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013eb4, 0x08013eb4
	.set sub_080145a8, 0x080145a8
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_08015768, 0x08015768
	.set sub_0801587c, 0x0801587c
	.set sub_0801591c, 0x0801591c
	.set sub_08016cfc, 0x08016cfc
	.set sub_0802a5e4, 0x0802a5e4
	.set sub_0802b1a0, 0x0802b1a0
	.set sub_0802b38c, 0x0802b38c
	.set sub_0802b6e8, 0x0802b6e8
	.set sub_0802b8fe, 0x0802b8fe
	.set sub_0802bc4e, 0x0802bc4e
	.set sub_0802dd70, 0x0802dd70
	.global Func_0802b700
	.thumb_func
Func_0802b700:
	push	{r5, lr}
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08016cfc
	adds	r0, r5, #0
	movs	r1, #0
	bl	.L_0802b738
	pop	{r5, pc}
	push	{r5, lr}
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08016cfc
	adds	r0, r5, #0
	movs	r1, #1
	bl	.L_0802b738
	pop	{r5, pc}
.L_0802b738:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #12
	ldr	r3, [pc, #148]
	add	r7, sp, #8
	mov	r8, r0
	adds	r0, r7, #0
	str	r3, [sp, #8]
	mov	sl, r1
	bl	sub_0802b6e8
	ldr	r3, [pc, #136]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	str	r0, [sp, #8]
.L_0802b75c:
	adds	r0, r7, #0
	bl	sub_0802b6e8
	movs	r3, #255
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	lsls	r3, r3, #8
	adds	r2, r0, #1
	adds	r3, #255
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0802b7ce
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r3, r0
	cmp	r3, r8
	bne.n	.L_0802b7c6
	ldr	r3, [sp, #8]
	ldrb	r0, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	ldrb	r1, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	ldrb	r6, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	ldrb	r5, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	ldrb	r2, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	ldrb	r4, [r3, #0]
	adds	r3, #1
	str	r3, [sp, #8]
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_0802b7b8
	adds	r3, r4, #0
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_0802b38c
	b.n	.L_0802b75c
.L_0802b7b8:
	str	r2, [sp, #0]
	adds	r3, r5, #0
	adds	r2, r6, #0
	str	r4, [sp, #4]
	bl	sub_0802b1a0
	b.n	.L_0802b75c
.L_0802b7c6:
	ldr	r3, [sp, #8]
	adds	r3, #6
	str	r3, [sp, #8]
	b.n	.L_0802b75c
.L_0802b7ce:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0202e002
	.2byte 0xe000
	.2byte 0x0202
	push	{r5, lr}
	lsls	r3, r0, #1
	ldr	r2, [pc, #56]
	adds	r3, r3, r0
	lsls	r3, r3, #2
	ldrh	r0, [r3, r2]
	ldr	r3, [pc, #52]
	adds	r5, r1, #0
	adds	r0, r0, r3
	bl	sub_08013300
	ldrb	r3, [r0, #0]
	str	r3, [r5, #0]
	ldrb	r3, [r0, #2]
	cmp	r3, #0
	bne.n	.L_0802b806
	movs	r3, #128
	lsls	r3, r3, #1
	b.n	.L_0802b808
.L_0802b806:
	ldrb	r3, [r0, #2]
.L_0802b808:
	str	r3, [r5, #4]
	ldrb	r3, [r0, #1]
	str	r3, [r5, #8]
	ldrb	r3, [r0, #3]
	cmp	r3, #0
	bne.n	.L_0802b81a
	movs	r3, #128
	lsls	r3, r3, #1
	b.n	.L_0802b81c
.L_0802b81a:
	ldrb	r3, [r0, #3]
.L_0802b81c:
	str	r3, [r5, #12]
	pop	{r5, pc}
	.4byte 0x0802f380
	.2byte 0x026c
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #56]
	movs	r1, #253
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [pc, #44]
	lsls	r3, r3, #2
	ldrh	r0, [r3, r2]
	ldr	r3, [pc, #40]
	adds	r0, r0, r3
	bl	sub_08013300
	adds	r5, r0, #0
	ldr	r3, [r5, #44]
	ldr	r1, [pc, #32]
	adds	r0, r5, r3
	bl	sub_0801587c
	bl	sub_0802a5e4
	ldr	r3, [r5, #48]
	ldr	r1, [pc, #24]
	adds	r0, r5, r3
	bl	sub_0801587c
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x0802f380
	.4byte 0x0000026c
	.4byte 0x02010000
	.2byte 0x4000
	.2byte 0x0202
.L_0802b878:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	mov	r8, r2
	ldr	r2, [r3, #32]
	mov	sl, r1
	movs	r1, #138
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldr	r3, [r3, #0]
	mov	r1, sl
	mov	fp, r3
	movs	r3, #1
	ands	r1, r3
	mov	sl, r1
	lsls	r0, r0, #1
	mov	r1, r8
	ands	r1, r3
	mov	r9, r0
	mov	r8, r1
	mov	r3, r9
	add	r3, r8
	lsls	r3, r3, #1
	add	r3, sl
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldr	r1, [sp, #32]
	movs	r3, #144
	lsls	r3, r3, #4
	adds	r3, #72
	adds	r2, r2, r3
	cmp	r1, #0
	bne.n	.L_0802b8d0
	ldrh	r3, [r2, #0]
	movs	r0, #0
	cmp	r5, r3
	beq.n	.L_0802b978
.L_0802b8d0:
	movs	r6, #128
	lsls	r6, r6, #3
	strh	r5, [r2, #0]
	movs	r0, #56
	adds	r1, r6, #0
	bl	sub_08014cc0
	lsls	r3, r5, #2
	mov	r2, fp
	adds	r7, r0, #0
	ldr	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L_0802b8f4
	add	r0, fp
	adds	r1, r7, #0
	bl	sub_0801591c
	b.n	sub_0802b8fe
.L_0802b8f4:
	ldr	r3, [pc, #140]
	adds	r0, r7, #0
	adds	r1, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x464b
	add	r3, r8
	lsls	r3, r3, #5
	ldr	r1, [pc, #128]
	add	r3, sl
	lsls	r3, r3, #6
	adds	r4, r7, #0
	adds	r5, r3, r1
	movs	r6, #0
.L_0802b910:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	adds	r1, r5, #0
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #1
	adds	r4, #64
	adds	r5, #128
	cmp	r6, #15
	bls.n	.L_0802b910
	ldr	r2, [sp, #32]
	cmp	r2, #0
	beq.n	.L_0802b970
	mov	r3, r9
	add	r3, r8
	lsls	r3, r3, #6
	ldr	r2, [pc, #80]
	add	r3, sl
	ldr	r5, [pc, #80]
	lsls	r3, r3, #5
	adds	r1, r3, r2
	adds	r4, r7, #0
	movs	r6, #0
.L_0802b948:
	movs	r0, #0
.L_0802b94a:
	ldrh	r3, [r4, #0]
	adds	r0, #1
	lsls	r3, r3, #2
	ldrh	r2, [r3, r5]
	adds	r4, #4
	strh	r2, [r1, #0]
	ldr	r2, [pc, #60]
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	adds	r2, r1, #0
	adds	r2, #64
	strh	r3, [r2, #0]
	adds	r1, #2
	cmp	r0, #15
	bls.n	.L_0802b94a
	adds	r6, #1
	adds	r1, #96
	cmp	r6, #15
	bls.n	.L_0802b948
.L_0802b970:
	movs	r0, #56
	bl	sub_0801314c
	movs	r0, #1
.L_0802b978:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000258
	.4byte 0x02020000
	.4byte 0x06004000
	.4byte 0x02010000
	.2byte 0x0002
	.2byte 0x0201
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #193
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #0
	sub	sp, #36
	bl	sub_08013eb4
	movs	r1, #144
	lsls	r1, r1, #4
	adds	r1, #120
	movs	r0, #32
	bl	sub_08014cc0
	movs	r3, #0
	adds	r7, r0, #0
	add	r0, sp, #32
	mov	r9, r3
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r7, #0
	ldr	r2, [pc, #732]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r3, r7, #0
	mov	r1, r9
	adds	r3, #228
	str	r1, [r3, #0]
	adds	r3, #4
	str	r1, [r3, #0]
	adds	r2, r7, #0
	movs	r3, #128
	adds	r2, #236
	lsls	r3, r3, #14
	str	r3, [r2, #0]
	movs	r3, #128
	adds	r2, #4
	lsls	r3, r3, #15
	str	r3, [r2, #0]
	ldr	r2, [pc, #700]
	adds	r3, r7, #0
	adds	r3, #244
	str	r2, [r3, #0]
	adds	r3, #4
	str	r2, [r3, #0]
	str	r1, [r7, #16]
	ldr	r0, [pc, #692]
	bl	sub_08013300
	movs	r2, #138
	lsls	r2, r2, #1
	adds	r3, r7, r2
	str	r0, [r3, #0]
	movs	r2, #252
	movs	r3, #128
	lsls	r2, r2, #6
	lsls	r3, r3, #19
	adds	r2, #158
	adds	r3, #80
	strh	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r2, #16
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	mov	r1, r9
	strh	r1, [r3, #0]
	ldr	r0, [pc, #652]
	bl	sub_08013300
	ldr	r1, [pc, #648]
	bl	sub_0801587c
	movs	r3, #248
	lsls	r3, r3, #5
	strh	r3, [r7, #20]
	movs	r3, #128
	strb	r3, [r7, #22]
	ldr	r0, [pc, #636]
	bl	sub_08013300
	ldr	r1, [pc, #636]
	bl	sub_0801587c
	movs	r2, #168
	movs	r3, #128
	lsls	r2, r2, #8
	lsls	r3, r3, #19
	adds	r2, #10
	adds	r3, #14
	strh	r2, [r3, #0]
	movs	r2, #170
	lsls	r2, r2, #8
	adds	r2, #14
	subs	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #133
	lsls	r2, r2, #8
	adds	r2, #1
	subs	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, #22
	strh	r2, [r3, #0]
	adds	r3, #2
	mov	r1, r9
	strh	r1, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	mov	r1, r9
	str	r1, [r3, #0]
	adds	r3, #4
	str	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	adds	r3, #2
	mov	r1, r9
	strh	r1, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	mov	r2, r9
	str	r2, [r3, #0]
	adds	r3, #4
	str	r2, [r3, #0]
	movs	r0, #48
	movs	r1, #76
	bl	sub_08014d00
	movs	r1, #208
	lsls	r1, r1, #6
	mov	sl, r0
	adds	r1, #132
	movs	r0, #28
	bl	sub_08014cc0
	movs	r3, #200
	movs	r1, #144
	lsls	r3, r3, #4
	lsls	r1, r1, #4
	movs	r2, #150
	adds	r3, r0, r3
	adds	r1, #92
	movs	r5, #255
	lsls	r2, r2, #4
	str	r3, [sp, #12]
	lsls	r5, r5, #17
	adds	r3, r7, r1
	adds	r2, r7, r2
	str	r0, [sp, #16]
	adds	r1, #12
	str	r5, [r3, #0]
	str	r2, [sp, #8]
	str	r5, [r2, #0]
	movs	r2, #128
	adds	r3, r7, r1
	lsls	r2, r2, #9
	adds	r1, #4
	str	r2, [r3, #0]
	adds	r3, r7, r1
	mov	r2, r9
	strh	r2, [r3, #0]
	ldr	r2, [pc, #464]
	mov	r3, r9
	mov	r1, sl
	str	r3, [r1, #24]
	str	r3, [r1, #28]
	movs	r3, #120
	str	r3, [r2, #12]
	movs	r3, #96
	asrs	r1, r5, #1
	str	r3, [r2, #16]
	adds	r0, r5, #0
	lsls	r2, r5, #1
	mov	r6, sl
	bl	sub_08015768
	adds	r6, #12
	mov	r2, r9
	str	r2, [r6, #0]
	str	r2, [r6, #4]
	str	r2, [r6, #8]
	bl	sub_08014de4
	adds	r0, r6, #0
	bl	sub_08015128
	movs	r3, #143
	lsls	r3, r3, #1
	adds	r3, r7, r3
	str	r3, [sp, #4]
	ldrh	r0, [r3, #0]
	bl	sub_08015068
	movs	r1, #142
	lsls	r1, r1, #1
	adds	r1, r1, r7
	ldrh	r0, [r1, #0]
	mov	r8, r1
	bl	sub_08015024
	add	r2, sp, #20
	mov	r3, r9
	mov	fp, r2
	str	r3, [r2, #0]
	str	r3, [r2, #4]
	mov	r1, sl
	str	r5, [r2, #8]
	mov	r0, fp
	ldr	r2, [pc, #376]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0xf7e9
	.2byte 0xf93a
	.2byte 0x4650
	adds	r1, r6, #0
	bl	sub_080156e8
	ldr	r5, [pc, #360]
	movs	r0, #104
	adds	r1, r5, #0
	bl	sub_08014cc0
	movs	r2, #132
	movs	r3, #128
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r1, r0, #0
	adds	r3, #212
	ldr	r0, [pc, #340]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, r8
	ldrh	r0, [r3, #0]
	bl	sub_08002090
	mov	r1, r8
	adds	r5, r0, #0
	ldrh	r0, [r1, #0]
	bl	sub_08002096
	ldr	r3, [pc, #320]
	adds	r1, r0, #0
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r2, [sp, #16]
	bl	sub_0802dd70
	ldr	r3, [pc, #304]
	movs	r5, #0
	str	r5, [r3, #0]
	mov	r1, r8
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #300]
	movs	r1, #192
	str	r3, [r2, #0]
	ldr	r3, [pc, #296]
	lsls	r1, r1, #18
	ldr	r2, [r3, #0]
	movs	r3, #1
	ands	r2, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	lsls	r3, r3, #10
	adds	r3, r2, r3
	str	r3, [sp, #0]
	ldr	r2, [sp, #16]
	ldr	r4, [r1, #104]
	mov	r0, sl
	adds	r1, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6035
	str	r5, [r6, #4]
	str	r5, [r6, #8]
	bl	sub_08014de4
	movs	r3, #224
	mov	r1, r8
	lsls	r3, r3, #8
	strh	r3, [r1, #0]
	ldr	r2, [sp, #4]
	strh	r5, [r2, #0]
	bl	sub_08014de4
	adds	r0, r6, #0
	bl	sub_08015128
	ldr	r3, [sp, #4]
	ldrh	r0, [r3, #0]
	bl	sub_08015068
	mov	r1, r8
	ldrh	r0, [r1, #0]
	bl	sub_08015024
	movs	r2, #144
	lsls	r2, r2, #4
	adds	r2, #118
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	sub_0802bc4e
	mov	r3, fp
	str	r5, [r3, #0]
	str	r5, [r3, #4]
	ldr	r1, [sp, #8]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #9
	adds	r3, r3, r2
	mov	r1, fp
	str	r3, [r1, #8]
	mov	r0, fp
	mov	r1, sl
	ldr	r2, [pc, #148]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x2380
	lsls	r3, r3, #19
	adds	r3, #76
	mov	r1, r9
	strh	r1, [r3, #0]
	movs	r3, #66
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #152]
	ldr	r0, [pc, #156]
	strh	r1, [r3, #4]
	strh	r1, [r3, #8]
	strh	r1, [r3, #12]
	movs	r1, #130
	mov	r2, r9
	lsls	r1, r1, #1
	strh	r2, [r3, #6]
	strh	r2, [r3, #10]
	strh	r2, [r3, #14]
	adds	r3, r7, r1
	strh	r2, [r3, #0]
	movs	r3, #131
	lsls	r3, r3, #1
	movs	r1, #128
	adds	r2, r7, r3
	lsls	r1, r1, #3
	movs	r3, #159
	strh	r3, [r2, #0]
	adds	r1, #133
	bl	sub_080145a8
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #112]
	bl	sub_080145a8
	movs	r2, #192
	movs	r1, #164
	lsls	r2, r2, #2
	lsls	r1, r1, #1
	movs	r3, #0
	adds	r2, #255
	adds	r0, r7, r1
.L_0802bca6:
	strh	r3, [r0, #0]
	adds	r3, #1
	adds	r0, #2
	cmp	r3, r2
	ble.n	.L_0802bca6
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x8500025e
	.4byte 0x3fe00000
	.4byte 0x00000197
	.4byte 0x00000198
	.4byte 0x02010000
	.4byte 0x00000199
	.4byte 0x0202e000
	.4byte 0x030011e0
	.4byte 0x03000354
	.4byte 0x00000298
	.4byte 0x0802146c
	.4byte 0x03000230
	.4byte 0x03001244
	.4byte 0x03001144
	.4byte 0x0300122c
	.4byte 0x03001120
	.4byte 0x0802c241
	.2byte 0xc089
	.2byte 0x0802
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r0, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r6, #0
	mov	r8, r3
	ldr	r3, [r3, #0]
	sub	sp, #4
	adds	r4, r2, #0
	mov	ip, r6
	cmp	r3, #0
	beq.n	.L_0802bd2e
	ldmia	r3!, {r6}
	ldr	r3, [r3, #4]
	mov	ip, r3
.L_0802bd2e:
	asrs	r1, r1, #4
	lsls	r3, r1, #5
	mov	r2, ip
	asrs	r5, r5, #4
	asrs	r0, r0, #3
	asrs	r4, r4, #3
	adds	r7, r3, r5
	asrs	r2, r2, #24
	asrs	r3, r0, #31
	mov	ip, r2
	asrs	r5, r4, #31
	lsrs	r2, r3, #31
	mov	lr, r3
	adds	r2, r0, r2
	lsrs	r3, r5, #31
	movs	r1, #31
	asrs	r2, r2, #1
	adds	r3, r4, r3
	ands	r2, r1
	asrs	r3, r3, #1
	ands	r3, r1
	lsls	r2, r2, #5
	adds	r2, r2, r3
	movs	r3, #164
	lsls	r3, r3, #1
	lsls	r2, r2, #1
	adds	r2, r2, r3
	asrs	r6, r6, #24
	mov	r3, r8
	strh	r7, [r3, r2]
	subs	r3, r6, r4
	cmp	r3, #0
	blt.n	.L_0802bd76
	cmp	r3, #1
	ble.n	.L_0802bd7c
	b.n	.L_0802bdc6
.L_0802bd76:
	subs	r3, r4, r6
	cmp	r3, #1
	bgt.n	.L_0802bdc6
.L_0802bd7c:
	mov	r2, ip
	subs	r3, r2, r0
	cmp	r3, #0
	blt.n	.L_0802bd8a
	cmp	r3, #1
	ble.n	.L_0802bd92
	b.n	.L_0802bdc6
.L_0802bd8a:
	mov	r2, ip
	subs	r3, r0, r2
	cmp	r3, #1
	bgt.n	.L_0802bdc6
.L_0802bd92:
	mov	r3, lr
	lsrs	r6, r5, #31
	lsrs	r5, r3, #31
	adds	r6, r4, r6
	adds	r5, r0, r5
	movs	r2, #1
	asrs	r6, r6, #1
	asrs	r5, r5, #1
	mov	r8, r2
	str	r2, [sp, #0]
	adds	r1, r6, #0
	adds	r2, r5, #0
	adds	r3, r7, #0
	movs	r0, #0
	bl	.L_0802b878
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r3, r7, r2
	mov	r2, r8
	str	r2, [sp, #0]
	movs	r0, #1
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	.L_0802b878
.L_0802bdc6:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.align 2, 0
