.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_080048b0, 0x080048b0
	.set sub_08004970, 0x08004970
	.set sub_080072f0, 0x080072f0
	.set sub_080072f8, 0x080072f8
	.set sub_08015010, 0x08015010
	.set sub_08015278, 0x08015278
	.set sub_080152a8, 0x080152a8
	.set sub_080153e0, 0x080153e0
	.set sub_08015408, 0x08015408
	.set sub_08015410, 0x08015410
	.set sub_08015418, 0x08015418
	.set sub_08077158, 0x08077158
	.set sub_08077290, 0x08077290
	.set sub_080a1050, 0x080a1050
	.set sub_080a1070, 0x080a1070
	.set sub_080a1090, 0x080a1090
	.set sub_080a2144, 0x080a2144
	.set sub_080a2474, 0x080a2474
	.set sub_080a2490, 0x080a2490
	.set sub_080a34c0, 0x080a34c0
	.set sub_080a76d0, 0x080a76d0
	.set sub_080a8034, 0x080a8034
	.set sub_080ad274, 0x080ad274
	.set sub_080ad318, 0x080ad318
	.global Unnamed_080a7478
	.global Func_080a7478
	.thumb_func
Unnamed_080a7478:
Func_080a7478:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #167
	lsls	r1, r1, #4
	movs	r0, #55
	sub	sp, #4
	bl	sub_080048b0
	adds	r7, r0, #0
	movs	r0, #64
	bl	sub_08004970
	movs	r6, #128
	lsls	r6, r6, #6
	mov	r9, r0
	adds	r0, r6, #0
	bl	sub_08004970
	ldr	r3, [pc, #256]
	ldr	r2, [r3, #0]
	movs	r3, #1
	movs	r1, #0
	strh	r3, [r2, #4]
	mov	fp, r0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	sub_08015408
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080a1070
	movs	r0, #0
	bl	sub_080a1090
	movs	r0, #136
	lsls	r0, r0, #2
	movs	r2, #130
	adds	r3, r7, r0
	lsls	r2, r2, #2
	movs	r5, #0
	strh	r5, [r3, #0]
	adds	r0, r7, r2
	bl	sub_08077158
	ldr	r2, [pc, #200]
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	movs	r1, #3
	movs	r3, #7
	movs	r0, #0
	movs	r2, #0
	bl	sub_080a8034
	movs	r1, #160
	lsls	r1, r1, #19
	movs	r2, #64
	ldr	r5, [pc, #180]
	mov	r0, r9
	bl	sub_080072f8
	movs	r0, #14
	bl	sub_080a2144
	movs	r1, #160
	ldr	r3, [pc, #168]
	ldr	r0, [pc, #172]
	lsls	r1, r1, #19
	ldr	r2, [pc, #172]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #28
	ldr	r0, [pc, #168]
	ldr	r2, [pc, #168]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #4
	ldr	r0, [pc, #148]
	ldr	r2, [pc, #152]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, #28
	ldr	r0, [pc, #156]
	ldr	r2, [pc, #148]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r2, r6, #0
	ldr	r1, [pc, #148]
	mov	r0, fp
	bl	sub_080072f8
	ldr	r3, [pc, #144]
	adds	r1, r6, #0
	ldr	r2, [pc, #144]
	ldr	r0, [pc, #136]
	bl	sub_080072f0
	movs	r0, #1
	bl	sub_080153e0
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #5
	movs	r0, #13
	bl	sub_08015010
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r7, r2
	str	r0, [r3, #0]
	movs	r0, #169
	lsls	r0, r0, #1
	ldr	r1, [pc, #52]
	movs	r2, #7
	adds	r3, r7, r0
.L_080a7570:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_080a7570
	movs	r0, #1
	negs	r0, r0
	bl	sub_08077290
	cmp	r0, #0
	beq.n	.L_080a7594
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl	sub_080ad274
.L_080a7594:
	movs	r2, #141
	lsls	r2, r2, #2
	ldr	r0, [pc, #8]
	adds	r3, r7, r2
	b.n	.L_080a75d8
	movs	r0, r0
	.4byte 0x0000001e
	.4byte 0x00000080
	.4byte 0x03001e68
	.4byte 0x00000219
	.4byte 0x03001388
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x80000010
	.4byte 0x050001c8
	.4byte 0x80000001
	.4byte 0x050001e8
	.4byte 0x06004000
	.4byte 0x03000168
	.2byte 0x3333
	.2byte 0x3333
.L_080a75d8:
	movs	r1, #130
	movs	r2, #3
.L_080a75dc:
	subs	r2, #1
	strh	r1, [r3, #0]
	strh	r0, [r3, #8]
	adds	r1, #32
	adds	r3, #2
	cmp	r2, #0
	bge.n	.L_080a75dc
	ldr	r0, [pc, #208]
	bl	sub_08015418
	bl	sub_080a2474
	movs	r2, #0
	movs	r0, #136
	mov	r8, r2
	lsls	r0, r0, #2
	adds	r3, r7, r0
	mov	r0, r8
	strh	r0, [r3, #0]
	bl	sub_080a76d0
	mov	sl, r0
	bl	sub_080a2490
	ldr	r0, [r7, #36]
	bl	sub_08015278
	bl	sub_080ad318
	bl	sub_080a1050
	movs	r3, #20
	movs	r1, #0
	movs	r2, #30
	movs	r0, #0
	bl	sub_08015408
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080152a8
	movs	r0, #0
	bl	sub_080153e0
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #160
	ldr	r5, [pc, #128]
	mov	r1, r9
	movs	r2, #64
	lsls	r0, r0, #19
	bl	sub_080072f8
	movs	r2, #128
	mov	r1, fp
	lsls	r2, r2, #6
	ldr	r0, [pc, #112]
	bl	sub_080072f8
	mov	r0, fp
	bl	sub_08002df0
	mov	r0, r9
	bl	sub_08002df0
	ldr	r5, [pc, #100]
	ldr	r6, [pc, #100]
	ldr	r2, [r5, #0]
	movs	r3, #1
	strb	r3, [r2, r6]
	bl	sub_080a34c0
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	sub_08015408
	movs	r0, #55
	bl	sub_08002dd8
	adds	r3, r5, #0
	subs	r3, #36
	ldr	r3, [r3, #0]
	mov	r2, r8
	strh	r2, [r3, #4]
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	bl	sub_08015410
	ldr	r3, [r5, #0]
	movs	r0, #0
	adds	r3, r3, r6
	strb	r0, [r3, #0]
	add	sp, #4
	mov	r0, sl
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x06002500
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x03001e8c
	.4byte 0x00000ea6
