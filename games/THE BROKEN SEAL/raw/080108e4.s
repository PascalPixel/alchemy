.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_08003bb4, 0x08003bb4
	.set sub_080041d8, 0x080041d8
	.set sub_080048b0, 0x080048b0
	.set sub_080048f4, 0x080048f4
	.set sub_080049ac, 0x080049ac
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_08005258, 0x08005258
	.set sub_08005340, 0x08005340
	.set sub_080053e8, 0x080053e8
	.set sub_080072ec, 0x080072ec
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080118d8, 0x080118d8
	.set sub_080123f4, 0x080123f4
	.global Overlay_080108e4
Overlay_080108e4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r3, #0
	ldr	r3, [pc, #216]
	mov	sl, r1
	mov	r8, r2
	movs	r1, #136
	ldr	r2, [r3, #0]
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldr	r6, [r3, #0]
	mov	r1, sl
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
	ldr	r1, [sp, #28]
	movs	r3, #206
	lsls	r3, r3, #2
	adds	r2, r2, r3
	cmp	r1, #0
	bne.n	.L_08010932
	ldrh	r3, [r2, #0]
	movs	r0, #0
	cmp	r5, r3
	beq.n	.L_080109be
.L_08010932:
	movs	r1, #128
	strh	r5, [r2, #0]
	lsls	r1, r1, #3
	movs	r0, #14
	bl	sub_080048b0
	lsls	r3, r5, #2
	adds	r7, r0, #0
	ldr	r0, [r3, r6]
	adds	r1, r7, #0
	adds	r0, r6, r0
	bl	sub_080053e8
	mov	r3, r9
	add	r3, r8
	lsls	r3, r3, #5
	add	r3, sl
	ldr	r2, [pc, #120]
	lsls	r3, r3, #6
	adds	r4, r7, #0
	adds	r5, r3, r2
	movs	r6, #0
.L_0801095e:
	ldr	r3, [pc, #116]
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r6, #1
	adds	r4, #64
	adds	r5, #128
	cmp	r6, #15
	bls.n	.L_0801095e
	ldr	r3, [sp, #28]
	cmp	r3, #0
	beq.n	.L_080109b6
	mov	r3, r9
	add	r3, r8
	lsls	r3, r3, #6
	add	r3, sl
	ldr	r2, [pc, #88]
	lsls	r3, r3, #5
	ldr	r5, [pc, #88]
	adds	r1, r3, r2
	adds	r4, r7, #0
	movs	r6, #0
.L_0801098e:
	movs	r0, #0
.L_08010990:
	ldrh	r3, [r4, #0]
	lsls	r3, r3, #2
	ldrh	r2, [r3, r5]
	strh	r2, [r1, #0]
	ldr	r2, [pc, #72]
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	adds	r2, r1, #0
	adds	r2, #64
	adds	r0, #1
	strh	r3, [r2, #0]
	adds	r1, #2
	adds	r4, #4
	cmp	r0, #15
	bls.n	.L_08010990
	adds	r6, #1
	adds	r1, #96
	cmp	r6, #15
	bls.n	.L_0801098e
.L_080109b6:
	movs	r0, #14
	bl	sub_08002dd8
	movs	r0, #1
.L_080109be:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e70
	.4byte 0x02020000
	.4byte 0x040000d4
	.4byte 0x84000010
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
	ldr	r3, [pc, #708]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #0
	sub	sp, #32
	bl	sub_08003bb4
	movs	r1, #215
	lsls	r1, r1, #2
	movs	r0, #8
	bl	sub_080048b0
	movs	r6, #0
	adds	r7, r0, #0
	add	r0, sp, #28
	str	r6, [r0, #0]
	ldr	r3, [pc, #676]
	adds	r1, r7, #0
	ldr	r2, [pc, #676]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r3, r7, #0
	adds	r3, #228
	str	r6, [r3, #0]
	adds	r3, #4
	str	r6, [r3, #0]
	adds	r2, r7, #0
	movs	r3, #128
	adds	r2, #236
	lsls	r3, r3, #14
	str	r3, [r2, #0]
	movs	r3, #128
	adds	r2, #4
	lsls	r3, r3, #15
	str	r3, [r2, #0]
	adds	r3, r7, #0
	movs	r2, #255
	lsls	r2, r2, #21
	adds	r3, #244
	str	r2, [r3, #0]
	adds	r3, #4
	str	r2, [r3, #0]
	str	r6, [r7, #16]
	ldr	r0, [pc, #632]
	bl	sub_08002f40
	movs	r1, #136
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r5, [pc, #624]
	str	r0, [r3, #0]
	ldr	r0, [pc, #624]
	bl	sub_08002f40
	adds	r1, r5, #0
	bl	sub_08005340
	adds	r0, r5, #0
	bl	sub_080118d8
	ldr	r2, [pc, #608]
	ldr	r3, [pc, #612]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #612]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r6, [r3, #0]
	ldr	r0, [pc, #604]
	bl	sub_08002f40
	ldr	r1, [pc, #604]
	bl	sub_08005340
	ldr	r0, [pc, #600]
	bl	sub_08002f40
	ldr	r1, [pc, #600]
	bl	sub_08005340
	movs	r3, #248
	lsls	r3, r3, #5
	strh	r3, [r7, #20]
	movs	r3, #128
	strb	r3, [r7, #22]
	ldr	r2, [pc, #588]
	ldr	r3, [pc, #588]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #588]
	subs	r3, #2
	strh	r2, [r3, #0]
	ldr	r2, [pc, #588]
	subs	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #22
	movs	r2, #128
	lsls	r2, r2, #1
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r6, [r3, #0]
	adds	r3, #2
	strh	r6, [r3, #0]
	adds	r3, #2
	movs	r1, #128
	lsls	r1, r1, #1
	strh	r1, [r3, #0]
	adds	r3, #2
	str	r6, [r3, #0]
	adds	r3, #4
	str	r6, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r6, [r3, #0]
	adds	r3, #2
	strh	r6, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #2
	str	r6, [r3, #0]
	adds	r3, #4
	str	r6, [r3, #0]
	movs	r0, #12
	movs	r1, #76
	bl	sub_080048f4
	ldr	r1, [pc, #520]
	mov	r9, r0
	movs	r0, #7
	bl	sub_080048b0
	movs	r2, #12
	add	r2, r9
	movs	r3, #200
	mov	r8, r2
	lsls	r3, r3, #4
	movs	r1, #210
	movs	r2, #211
	adds	r3, r0, r3
	lsls	r1, r1, #2
	movs	r5, #255
	lsls	r2, r2, #2
	str	r3, [sp, #8]
	lsls	r5, r5, #17
	adds	r3, r7, r1
	adds	r2, r7, r2
	str	r0, [sp, #12]
	adds	r1, #12
	str	r5, [r3, #0]
	str	r2, [sp, #4]
	str	r5, [r2, #0]
	movs	r2, #128
	adds	r3, r7, r1
	lsls	r2, r2, #9
	str	r2, [r3, #0]
	adds	r1, #4
	mov	r2, r9
	adds	r3, r7, r1
	str	r6, [r2, #24]
	str	r6, [r2, #28]
	ldr	r2, [pc, #456]
	strh	r6, [r3, #0]
	movs	r3, #120
	str	r3, [r2, #12]
	movs	r3, #96
	asrs	r1, r5, #1
	adds	r0, r5, #0
	str	r3, [r2, #16]
	lsls	r2, r5, #1
	bl	sub_08005258
	mov	r3, r8
	str	r6, [r3, #0]
	str	r6, [r3, #4]
	str	r6, [r3, #8]
	bl	sub_080049ac
	mov	r0, r8
	bl	sub_08004cb4
	movs	r1, #141
	lsls	r1, r1, #1
	adds	r1, r7, r1
	str	r1, [sp, #0]
	ldrh	r0, [r1, #0]
	bl	sub_08004c1c
	movs	r2, #140
	lsls	r2, r2, #1
	adds	r2, r2, r7
	ldrh	r0, [r2, #0]
	mov	fp, r2
	bl	sub_08004bd4
	add	r3, sp, #16
	mov	sl, r3
	str	r6, [r3, #0]
	str	r6, [r3, #4]
	str	r5, [r3, #8]
	mov	r1, r9
	ldr	r2, [pc, #380]
	mov	r0, sl
	bl	sub_080072ec
	bl	sub_080049ac
	mov	r0, r9
	mov	r1, r8
	bl	sub_080051d8
	ldr	r5, [pc, #364]
	movs	r0, #46
	adds	r1, r5, #0
	bl	sub_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #272]
	ldr	r0, [pc, #348]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, fp
	ldrh	r0, [r3, #0]
	bl	sub_0800231c
	mov	r1, fp
	adds	r5, r0, #0
	ldrh	r0, [r1, #0]
	bl	sub_08002322
	ldr	r3, [pc, #328]
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080072f0
	mov	r1, r8
	ldr	r2, [sp, #12]
	bl	sub_080123f4
	ldr	r3, [pc, #312]
	str	r6, [r3, #0]
	mov	r1, fp
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #308]
	str	r3, [r2, #0]
	ldr	r3, [pc, #308]
	ldr	r2, [r3, #0]
	movs	r3, #1
	ands	r2, r3
	ldr	r1, [pc, #304]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [sp, #8]
	adds	r1, #184
	lsls	r3, r3, #10
	ldr	r4, [r1, #0]
	adds	r3, r2, r3
	mov	r1, r8
	ldr	r2, [sp, #12]
	mov	r0, r9
	bl	sub_080072f4
	mov	r3, r8
	str	r6, [r3, #0]
	str	r6, [r3, #4]
	str	r6, [r3, #8]
	bl	sub_080049ac
	movs	r3, #224
	mov	r1, fp
	lsls	r3, r3, #8
	strh	r3, [r1, #0]
	ldr	r2, [sp, #0]
	strh	r6, [r2, #0]
	bl	sub_080049ac
	mov	r0, r8
	bl	sub_08004cb4
	ldr	r3, [sp, #0]
	ldrh	r0, [r3, #0]
	bl	sub_08004c1c
	mov	r1, fp
	ldrh	r0, [r1, #0]
	bl	sub_08004bd4
	mov	r2, sl
	str	r6, [r2, #0]
	str	r6, [r2, #4]
	ldr	r1, [sp, #4]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #9
	adds	r3, r3, r2
	mov	r1, sl
	str	r3, [r1, #8]
	mov	r0, sl
	mov	r1, r9
	ldr	r2, [pc, #176]
	bl	sub_080072ec
	ldr	r3, [pc, #204]
	strh	r6, [r3, #0]
	movs	r3, #66
	movs	r1, #128
	lsls	r1, r1, #19
	strh	r3, [r1, #0]
	ldr	r3, [pc, #196]
	movs	r2, #128
	lsls	r2, r2, #1
	strh	r6, [r3, #4]
	strh	r6, [r3, #6]
	strh	r6, [r3, #8]
	strh	r6, [r3, #10]
	strh	r6, [r3, #12]
	strh	r6, [r3, #14]
	adds	r3, r7, r2
	strh	r6, [r3, #0]
	movs	r3, #129
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #159
	strh	r3, [r2, #0]
	ldr	r1, [pc, #168]
	ldr	r0, [pc, #168]
	bl	sub_080041d8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #164]
	bl	sub_080041d8
	ldr	r1, [pc, #160]
	movs	r3, #255
	adds	r0, r7, r1
.L_08010ca6:
	strh	r3, [r0, #0]
	subs	r3, #1
	subs	r0, #2
	cmp	r3, #0
	bge.n	.L_08010ca6
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000c1ff
	.4byte 0x040000d4
	.4byte 0x850000d7
	.4byte 0x000000d4
	.4byte 0x0202d000
	.4byte 0x000000d6
	.4byte 0x00003f9e
	.4byte 0x04000050
	.4byte 0x00001010
	.4byte 0x000000d5
	.4byte 0x02010000
	.4byte 0x000000d7
	.4byte 0x0202c000
	.4byte 0x0000a80a
	.4byte 0x0400000e
	.4byte 0x0000aa0e
	.4byte 0x00000501
	.4byte 0x00003484
	.4byte 0x03001ce0
	.4byte 0x03000250
	.4byte 0x00000284
	.4byte 0x0800a0f8
	.4byte 0x0300013c
	.4byte 0x03001f60
	.4byte 0x03001af4
	.4byte 0x03001e40
	.4byte 0x03001e50
	.4byte 0x0400004c
	.4byte 0x03001ad0
	.4byte 0x00000c85
	.4byte 0x080111b5
	.4byte 0x08010ff1
	.4byte 0x00000336
