.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004080, 0x08004080
	.set sub_080041d8, 0x080041d8
	.set sub_080048f4, 0x080048f4
	.set sub_080053e8, 0x080053e8
	.set sub_080091a8, 0x080091a8
	.global Unnamed_0809509c
	.global Func_0809509c
	.thumb_func
Unnamed_0809509c:
Func_0809509c:
	push	{r5, r6, r7, lr}
	movs	r1, #130
	lsls	r1, r1, #3
	movs	r0, #29
	sub	sp, #4
	bl	sub_080048f4
	adds	r5, r0, #0
	adds	r7, r5, #0
	movs	r1, #0
	mov	r0, sp
	str	r1, [r0, #0]
	adds	r7, #8
	ldr	r3, [pc, #136]
	adds	r1, r5, #0
	ldr	r2, [pc, #136]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #14
	bl	sub_080048f4
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #120]
	bl	sub_080053e8
	bl	sub_08004080
	movs	r1, #128
	adds	r2, r6, #0
	str	r0, [r5, #0]
	lsls	r1, r1, #2
	bl	sub_08003fa4
	str	r0, [r5, #4]
	movs	r0, #14
	bl	sub_08002dd8
	movs	r5, #0
	movs	r6, #0
.L_080950f0:
	ldr	r3, [pc, #88]
	ldr	r3, [r3, #0]
	adds	r1, r7, #0
	stmia	r1!, {r6}
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #84]
	stmia	r1!, {r3}
	movs	r3, #212
	lsls	r3, r3, #8
	str	r3, [r1, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [r2, #8]
	asrs	r1, r1, #16
	str	r6, [r7, #12]
	str	r6, [r7, #20]
	asrs	r2, r2, #16
	movs	r0, #0
	bl	sub_080091a8
	ldr	r2, [pc, #36]
	adds	r3, r5, #0
	ands	r3, r2
	lsls	r0, r0, #16
	adds	r3, #1
	adds	r5, #1
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	adds	r7, #32
	cmp	r5, #31
	bls.n	.L_080950f0
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #32]
	bl	sub_080041d8
	add	sp, #4
	b.n	.L_08095158
	movs	r0, r0
	.4byte 0x0000000f
	.4byte 0x040000d4
	.4byte 0x85000104
	.4byte 0x080a00b8
	.4byte 0x03001e70
	.4byte 0x40000400
	.2byte 0x4e7d
	.2byte 0x0809
.L_08095158:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
