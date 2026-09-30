.syntax unified
	.thumb
	.global Func_080d2260
	.thumb_func
Func_080d2260:
.L_080d2260:
	push	{lr}
	bl	Func_080cdf5c
	bl	ObjectTable_Get
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r0, #56]
	str	r3, [r0, #64]
	movs	r3, #0
	str	r3, [r0, #36]
	str	r3, [r0, #44]
	movs	r2, #128
	ldr	r3, [pc, #28]
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L_080d229c
	movs	r1, #12
	bl	Object_SetMode
	b.n	.L_080d22a2
.L_080d229c:
	movs	r1, #1
	bl	Object_SetMode
.L_080d22a2:
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
	.global Func_080d22a8
	.thumb_func
Func_080d22a8:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl	0x08038208
	bl	.L_080d2260
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #162
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_080d22cc
	bl	Func_080cdec8
.L_080d22cc:
	movs	r1, #203
	movs	r2, #192
	lsls	r1, r1, #4
	lsls	r2, r2, #4
	movs	r5, #0
	adds	r3, r6, r1
	adds	r2, #178
	strh	r5, [r3, #0]
	adds	r1, #4
	adds	r3, r6, r2
	strh	r5, [r3, #0]
	adds	r3, r6, r1
	strh	r5, [r3, #0]
	movs	r3, #218
	lsls	r3, r3, #1
	movs	r1, #220
	adds	r2, r6, r3
	lsls	r1, r1, #1
	movs	r3, #16
	str	r3, [r2, #0]
	adds	r3, r6, r1
	str	r5, [r3, #0]
	movs	r3, #227
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r2, #0]
	adds	r1, #16
	movs	r3, #1
	adds	r2, r6, r1
	negs	r3, r3
	strh	r3, [r2, #0]
	movs	r3, #229
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #1
	negs	r3, r3
	movs	r1, #144
	strh	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #36]
	bl	Func_080145a8
	movs	r0, #153
	lsls	r0, r0, #1
	bl	GameFlag_ClearBitFar
	ldr	r3, [pc, #28]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r2, r6, r1
	adds	r1, #52
	adds	r3, r3, r1
	ldr	r3, [r3, #0]
	str	r3, [r2, #0]
	movs	r2, #242
	lsls	r2, r2, #1
	adds	r3, r6, r2
	str	r5, [r3, #0]
	pop	{r5, r6, pc}
	.4byte 0x080d21f5
	.2byte 0x0240
	.2byte 0x0200
	.global Func_080d2350
	.thumb_func
Func_080d2350:
	push	{lr}
	ldr	r0, [pc, #52]
	bl	Func_08014644
	ldr	r2, [pc, #48]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #118
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_080d237a
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	Func_080d4384
	b.n	.L_080d2382
.L_080d237a:
	movs	r0, #8
	movs	r1, #1
	bl	Func_080d4384
.L_080d2382:
	bl	Func_080ad2b8
	pop	{pc}
	.4byte 0x080d21f5
	.2byte 0x0240
	.2byte 0x0200
	bx	lr
	.2byte 0x0000
