@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08021e6c
	.thumb_func
Func_08021e6c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #60
	str	r0, [sp, #52]
	ldr	r3, [pc, #228]
	movs	r0, #128
	ldr	r3, [r3, #0]
	lsls	r0, r0, #3
	str	r3, [sp, #48]
	bl	Func_080040b4
	adds	r7, r0, #0
	movs	r0, #1
	str	r0, [sp, #44]
	movs	r0, #240
	movs	r1, #0
	lsls	r0, r0, #1
	str	r1, [sp, #40]
	bl	Func_08004970
	mov	r9, r0
	mov	r3, r9
	movs	r2, #255
	adds	r3, #255
	mov	ip, r9
.L0:
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r3, ip
	bge.n	.L0
	movs	r2, #128
	lsls	r2, r2, #1
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [sp, #52]
	cmp	r2, #0
	bne.n	.L1
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #20
	movs	r3, #3
	movs	r1, #17
	movs	r2, #10
	b.n	.L2
.L1:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #22
	movs	r3, #3
	movs	r1, #17
	movs	r2, #8
.L2:
	bl	Func_080162d4
	movs	r3, #214
	lsls	r3, r3, #1
	add	r3, r9
	str	r0, [r3, #0]
	movs	r3, #226
	lsls	r3, r3, #1
	movs	r0, #1
	add	r3, r9
	negs	r0, r0
	str	r0, [r3, #0]
	ldr	r3, [sp, #52]
	cmp	r3, #0
	bne.n	.L3
	movs	r2, #228
	lsls	r2, r2, #1
	mov	r4, r9
	movs	r3, #14
	str	r3, [r4, r2]
	movs	r1, #4
	adds	r3, r2, #4
	str	r1, [r4, r3]
	adds	r1, r2, #0
	movs	r3, #7
	adds	r1, #8
	str	r3, [r4, r1]
	ldr	r3, [pc, #84]
	adds	r2, #12
	str	r0, [r4, r2]
	ldr	r3, [r3, #0]
	movs	r2, #224
	lsls	r2, r2, #1
	ldr	r3, [r3, #60]
	add	r2, r9
	str	r3, [r2, #0]
	b.n	.L4
.L57:
	movs	r5, #2
	negs	r5, r5
	mov	sl, r5
	ldr	r2, [pc, #60]
	b.n	.L5
.L59:
	movs	r3, #224
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r6, #228
	lsls	r3, r3, #2
	lsls	r6, r6, #1
	adds	r3, r3, r6
	mov	r7, r9
	ldr	r3, [r7, r3]
	ldr	r2, [pc, #36]
	mov	sl, r3
	b.n	.L5
.L61:
	movs	r0, #113
	bl	Func_080f9010
	movs	r0, #1
	negs	r0, r0
	mov	sl, r0
	ldr	r2, [pc, #16]
	b.n	.L5
.L65:
	movs	r3, #228
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	mov	sl, r3
	b.n	.L5
	.4byte 0x03001e8c
	.4byte 0x03001f34
.L3:
	ldr	r3, [pc, #460]
	ldr	r3, [r3, #0]
	movs	r2, #224
	ldr	r3, [r3, #64]
	lsls	r2, r2, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r3, #228
	mov	r1, r9
	movs	r5, #0
	lsls	r3, r3, #1
	str	r5, [r1, r3]
	adds	r3, #4
	movs	r5, #1
	str	r5, [r1, r3]
	movs	r0, #0
	bl	Func_08077000
	ldr	r3, [r0, #0]
	movs	r5, #2
	cmp	r3, #0
	beq.n	.L6
	add	r5, sp, #56
	movs	r0, #0
	adds	r1, r5, #0
	bl	Func_080b5130
	mov	r2, sp
	adds	r2, #59
.L7:
	ldrb	r3, [r5, #0]
	ldr	r4, [sp, #40]
	adds	r5, #1
	adds	r4, r4, r3
	str	r4, [sp, #40]
	cmp	r5, r2
	ble.n	.L7
	movs	r2, #232
	mov	r5, r9
	lsls	r2, r2, #1
	movs	r3, #15
	str	r3, [r5, r2]
	movs	r5, #3
	cmp	r4, #0
	beq.n	.L6
	adds	r2, #4
	movs	r3, #16
	mov	r6, r9
	str	r3, [r6, r2]
	movs	r5, #4
.L6:
	movs	r1, #228
	lsls	r1, r1, #1
	lsls	r3, r5, #2
	mov	r0, r9
	adds	r3, r3, r1
	movs	r2, #2
	adds	r5, #1
	str	r2, [r0, r3]
	lsls	r3, r5, #2
	adds	r3, r3, r1
	movs	r2, #3
	adds	r5, #1
	str	r2, [r0, r3]
	lsls	r3, r5, #2
	adds	r3, r3, r1
	subs	r2, #4
	str	r2, [r0, r3]
.L4:
	movs	r5, #136
	lsls	r5, r5, #1
	add	r5, r9
	movs	r6, #228
	strh	r7, [r5, #0]
	lsls	r6, r6, #1
	add	r6, r9
	movs	r5, #150
	ldr	r2, [r6, #0]
	movs	r3, #1
	movs	r6, #230
	movs	r1, #0
	lsls	r5, r5, #1
	lsls	r6, r6, #1
	negs	r3, r3
	mov	r8, r1
	add	r5, r9
	add	r6, r9
	cmp	r2, r3
	beq.n	.L8
.L9:
	mov	r0, r9
	mov	r1, r8
	bl	Func_08021d88
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	cmp	r0, #5
	bgt.n	.L8
	strh	r7, [r5, #0]
	movs	r1, #1
	ldmia	r6!, {r2}
	negs	r1, r1
	adds	r5, #28
	cmp	r2, r1
	bne.n	.L9
.L8:
	movs	r0, #216
	lsls	r0, r0, #1
	mov	r2, r8
	add	r0, r9
	str	r2, [r0, #0]
	movs	r2, #218
	lsls	r2, r2, #1
	movs	r3, #160
	add	r2, r9
	lsls	r3, r3, #1
	strh	r3, [r2, #0]
	movs	r2, #219
	lsls	r2, r2, #1
	subs	r3, #16
	add	r2, r9
	strh	r3, [r2, #0]
	adds	r3, #136
	movs	r1, #0
	add	r3, r9
	strh	r1, [r3, #0]
	ldr	r3, [r0, #0]
	mov	r8, r1
	cmp	r8, r3
	bge.n	.L10
	movs	r1, #140
	adds	r4, r0, #0
	lsls	r1, r1, #1
	movs	r0, #138
	lsls	r0, r0, #1
	movs	r5, #136
	add	r1, r9
.L12:
	ldr	r3, [r4, #0]
	mov	r6, r8
	subs	r3, r6, r3
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #3
	adds	r3, r2, #0
	adds	r3, #155
	mov	r7, r9
	str	r3, [r0, r7]
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L11
	adds	r3, r2, #0
	adds	r3, #171
	str	r3, [r0, r7]
.L11:
	str	r5, [r1, #0]
	movs	r6, #1
	ldr	r3, [r4, #0]
	add	r8, r6
	adds	r1, #28
	adds	r0, #28
	cmp	r8, r3
	blt.n	.L12
.L10:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #148]
	bl	Func_080041d8
	ldr	r2, [pc, #144]
	movs	r0, #2
	movs	r1, #136
	bl	Func_0800307c
	movs	r0, #216
	movs	r2, #224
	lsls	r0, r0, #1
	lsls	r2, r2, #1
	add	r0, r9
	add	r2, r9
	adds	r7, r0, #0
	adds	r1, r2, #0
	str	r7, [sp, #16]
	str	r1, [sp, #24]
.L66:
	ldr	r3, [pc, #120]
	ldr	r3, [r3, #0]
	movs	r2, #31
	lsls	r3, r3, #1
	ldr	r1, [pc, #116]
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r1, [r1, r3]
	ldr	r4, [pc, #112]
	adds	r3, r1, r4
	str	r1, [sp, #36]
	cmp	r3, #0
	bge.n	.L13
	adds	r3, r1, #0
	subs	r3, #253
.L13:
	movs	r5, #152
	asrs	r3, r3, #2
	lsls	r5, r5, #1
	adds	r5, r3, r5
	str	r5, [sp, #36]
	movs	r3, #218
	add	r6, sp, #36
	ldrh	r6, [r6, #0]
	lsls	r3, r3, #1
	add	r3, r9
	strh	r6, [r3, #0]
	movs	r3, #219
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r2, [pc, #72]
	adds	r7, r6, #0
	ldr	r0, [pc, #40]
	strh	r7, [r3, #0]
	movs	r3, #32
	strh	r0, [r2, #4]
	strh	r3, [r2, #6]
	ldr	r1, [sp, #44]
	cmp	r1, #0
	bne.n	.L14
	b.n	.L15
.L14:
	ldr	r3, [sp, #48]
	ldr	r4, [pc, #52]
	movs	r2, #0
	str	r2, [sp, #44]
	adds	r2, r3, r4
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r5, [sp, #52]
	cmp	r5, #0
	bne.n	.L16
	b.n	.L17
.L16:
	b.n	.L18
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001f34
	.4byte 0x08021dfd
	.4byte 0x08021e15
	.4byte 0x03001e40
	.4byte 0x080366f8
	.4byte 0xffffff00
	.4byte 0x03001ad0
	.4byte 0x00000ea6
.L18:
	ldr	r7, [sp, #16]
	ldr	r3, [r7, #0]
	movs	r6, #0
	movs	r2, #6
	mov	lr, r6
	subs	r3, r2, r3
	cmp	lr, r3
	bge.n	.L19
	movs	r3, #216
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #28]
	subs	r2, r2, r3
	movs	r7, #3
	mov	ip, r2
	movs	r4, #0
.L22:
	mov	r1, lr
	adds	r3, r4, r1
	ldr	r2, [sp, #48]
	movs	r0, #0
	lsls	r3, r3, #1
	mov	r8, r0
	adds	r0, r3, r2
.L21:
	movs	r2, #0
	adds	r1, r0, #0
	b.n	.L20
	movs	r0, r0
	.4byte 0x0000f07f
.L20:
	adds	r3, r2, #0
	ands	r3, r7
	lsls	r3, r3, #1
	movs	r5, #137
	adds	r3, r3, r1
	lsls	r5, r5, #3
	adds	r3, r3, r5
	adds	r2, #1
	strh	r6, [r3, #0]
	cmp	r2, #2
	ble.n	.L20
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r0, #64
	cmp	r2, #2
	ble.n	.L21
	add	lr, r1
	adds	r4, #2
	cmp	lr, ip
	blt.n	.L22
.L19:
	ldr	r4, [sp, #16]
	movs	r3, #0
	mov	lr, r3
	ldr	r3, [r4, #0]
	cmp	lr, r3
	blt.n	.L23
	b.n	.L15
.L23:
	movs	r3, #216
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	mov	fp, r3
	lsls	r3, r3, #1
	add	r3, fp
	mov	sl, r3
	mov	r5, sl
	lsls	r5, r5, #1
	movs	r6, #0
	str	r5, [sp, #32]
	str	r6, [sp, #8]
.L26:
	ldr	r2, [sp, #8]
	ldr	r0, [sp, #48]
	add	r2, lr
	movs	r7, #0
	lsls	r3, r2, #1
	mov	r1, lr
	mov	r8, r7
	adds	r4, r3, r0
	adds	r6, r2, #0
	lsls	r5, r1, #4
.L25:
	str	r5, [sp, #4]
	movs	r0, #0
	mov	ip, r6
.L24:
	adds	r1, r0, #0
	movs	r2, #3
	ands	r1, r2
	mov	r3, ip
	adds	r2, r3, r1
	mov	r7, sl
	subs	r2, r2, r7
	ldr	r3, [pc, #72]
	ldr	r7, [sp, #4]
	lsls	r2, r2, #1
	adds	r2, r2, r3
	adds	r3, r7, r0
	movs	r7, #128
	lsls	r7, r7, #1
	adds	r3, r3, r7
	strh	r3, [r2, #0]
	ldr	r2, [sp, #32]
	lsls	r1, r1, #1
	adds	r1, r4, r1
	ldr	r3, [pc, #52]
	subs	r1, r1, r2
	ldr	r7, [pc, #40]
	adds	r1, r1, r3
	adds	r0, #1
	strh	r7, [r1, #0]
	cmp	r0, #2
	ble.n	.L24
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r4, #64
	adds	r6, #32
	adds	r5, #4
	cmp	r1, #2
	ble.n	.L25
	ldr	r2, [sp, #8]
	add	lr, r0
	adds	r2, #2
	str	r2, [sp, #8]
	cmp	lr, fp
	blt.n	.L26
	b.n	.L15
	.4byte 0x00000000
	.4byte 0x0600fd6c
	.4byte 0x0000046c
.L17:
	ldr	r4, [sp, #16]
	movs	r3, #0
	mov	lr, r3
	ldr	r3, [r4, #0]
	movs	r2, #6
	subs	r3, r2, r3
	cmp	lr, r3
	bge.n	.L27
	movs	r3, #216
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #48]
	subs	r2, r2, r3
	movs	r7, #3
	mov	ip, r2
	movs	r4, #0
.L31:
	mov	r0, lr
	adds	r3, r4, r0
	ldr	r1, [sp, #48]
	movs	r5, #0
	lsls	r3, r3, #1
	mov	r8, r5
	adds	r0, r3, r1
.L30:
	movs	r2, #0
	adds	r1, r0, #0
.L28:
	adds	r3, r2, #0
	ands	r3, r7
	lsls	r3, r3, #1
	ldr	r5, [pc, #20]
	adds	r3, r3, r1
	adds	r3, r3, r5
	adds	r2, #1
	strh	r6, [r3, #0]
	cmp	r2, #2
	ble.n	.L28
	b.n	.L29
	movs	r0, r0
	.4byte 0x0000f07f
	.4byte 0x00000444
.L29:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r0, #64
	cmp	r2, #2
	ble.n	.L30
	add	lr, r1
	adds	r4, #2
	cmp	lr, ip
	blt.n	.L31
.L27:
	ldr	r4, [sp, #16]
	movs	r3, #0
	mov	lr, r3
	ldr	r3, [r4, #0]
	cmp	lr, r3
	bge.n	.L15
	movs	r3, #216
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	mov	fp, r3
	lsls	r3, r3, #1
	add	r3, fp
	mov	sl, r3
	mov	r5, sl
	lsls	r5, r5, #1
	movs	r6, #0
	str	r5, [sp, #28]
	str	r6, [sp, #12]
.L34:
	ldr	r2, [sp, #12]
	ldr	r0, [sp, #48]
	add	r2, lr
	movs	r7, #0
	lsls	r3, r2, #1
	mov	r1, lr
	mov	r8, r7
	adds	r4, r3, r0
	adds	r6, r2, #0
	lsls	r5, r1, #4
.L33:
	str	r5, [sp, #4]
	movs	r0, #0
	mov	ip, r6
.L32:
	adds	r1, r0, #0
	movs	r2, #3
	ands	r1, r2
	mov	r3, ip
	adds	r2, r3, r1
	mov	r7, sl
	subs	r2, r2, r7
	ldr	r3, [pc, #76]
	ldr	r7, [sp, #4]
	lsls	r2, r2, #1
	adds	r2, r2, r3
	adds	r3, r7, r0
	movs	r7, #128
	lsls	r7, r7, #1
	adds	r3, r3, r7
	strh	r3, [r2, #0]
	ldr	r2, [sp, #28]
	lsls	r1, r1, #1
	adds	r1, r4, r1
	movs	r3, #141
	subs	r1, r1, r2
	lsls	r3, r3, #3
	ldr	r7, [pc, #40]
	adds	r1, r1, r3
	adds	r0, #1
	strh	r7, [r1, #0]
	cmp	r0, #2
	ble.n	.L32
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r4, #64
	adds	r6, #32
	adds	r5, #4
	cmp	r1, #2
	ble.n	.L33
	ldr	r2, [sp, #12]
	add	lr, r0
	adds	r2, #2
	str	r2, [sp, #12]
	cmp	lr, fp
	blt.n	.L34
	b.n	.L15
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0600fd68
.L15:
	movs	r7, #226
	movs	r6, #224
	lsls	r7, r7, #1
	lsls	r6, r6, #1
	add	r7, r9
	add	r6, r9
	ldr	r2, [r7, #0]
	ldr	r3, [r6, #0]
	cmp	r2, r3
	beq.n	.L35
	movs	r5, #214
	lsls	r5, r5, #1
	add	r5, r9
	ldr	r0, [r5, #0]
	bl	Func_08016478
	ldr	r2, [r6, #0]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r4, #142
	lsls	r4, r4, #1
	lsls	r3, r3, #2
	adds	r3, r3, r4
	mov	r1, r9
	ldr	r0, [r1, r3]
	ldr	r3, [pc, #88]
	ldr	r1, [r5, #0]
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #0
	bl	Func_0801e7c0
	ldr	r3, [r6, #0]
	str	r3, [r7, #0]
	ldr	r1, [r6, #0]
	movs	r2, #228
	lsls	r2, r2, #1
	lsls	r3, r1, #2
	adds	r3, r3, r2
	mov	r4, r9
	ldr	r2, [r4, r3]
	mov	r0, r9
	bl	Func_08021d88
.L35:
	movs	r0, #218
	lsls	r0, r0, #1
	add	r0, r9
	bl	Func_08003d28
	movs	r3, #222
	lsls	r3, r3, #1
	add	r3, r9
	str	r0, [r3, #0]
	ldr	r6, [sp, #16]
	movs	r5, #0
	ldr	r3, [r6, #0]
	mov	r8, r5
	cmp	r8, r3
	bge.n	.L36
	movs	r5, #130
	movs	r0, #63
	lsls	r5, r5, #1
	negs	r0, r0
	ldr	r6, [pc, #8]
	add	r5, r9
	adds	r7, r0, #0
	b.n	.L37
	movs	r0, r0
	.4byte 0xfffffe00
	.4byte 0x0000001f
.L37:
	ldr	r1, [sp, #24]
	ldr	r3, [r1, #0]
	cmp	r8, r3
	bne.n	.L38
	movs	r3, #222
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r2, [r3, #0]
	movs	r1, #31
	ldrb	r3, [r5, #7]
	ands	r2, r1
	lsls	r2, r2, #1
	ands	r3, r7
	orrs	r3, r2
	strb	r3, [r5, #7]
	ldrb	r3, [r5, #5]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r5, #5]
	ldr	r2, [sp, #36]
	lsls	r3, r2, #3
	subs	r2, r3, r2
	ldr	r1, [r5, #16]
	cmp	r2, #0
	bge.n	.L39
	ldr	r3, [pc, #56]
	adds	r2, r2, r3
.L39:
	asrs	r2, r2, #9
	ldr	r3, [pc, #44]
	adds	r2, r1, r2
	subs	r2, #14
	ands	r2, r3
	ldrh	r3, [r5, #6]
	ands	r3, r6
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r4, [sp, #36]
	lsls	r3, r4, #1
	adds	r3, r3, r4
	ldr	r2, [r5, #20]
	cmp	r3, #0
	bge.n	.L40
	adds	r3, #255
.L40:
	asrs	r3, r3, #8
	adds	r3, r2, r3
	subs	r3, #20
	strb	r3, [r5, #4]
	adds	r0, r5, #0
	movs	r1, #241
	bl	Func_08003dec
	b.n	.L41
	.4byte 0x000001ff
	.4byte 0x000001ff
.L38:
	ldr	r3, [pc, #52]
	ldr	r2, [r5, #16]
	ands	r2, r3
	ldrh	r3, [r5, #6]
	ands	r3, r6
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r3, [r5, #20]
	strb	r3, [r5, #4]
	ldrb	r3, [r5, #7]
	movs	r0, #4
	ands	r3, r7
	strb	r3, [r5, #7]
	negs	r0, r0
	ldrb	r3, [r5, #5]
	adds	r2, r0, #0
	ands	r3, r2
	strb	r3, [r5, #5]
.L41:
	ldr	r2, [sp, #16]
	movs	r1, #1
	ldr	r3, [r2, #0]
	add	r8, r1
	adds	r5, #28
	cmp	r8, r3
	blt.n	.L37
.L36:
	ldr	r3, [pc, #8]
	b.n	.L42
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x03001c94
.L42:
	ldr	r2, [pc, #644]
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #644]
	ldr	r2, [r2, #0]
	ldr	r7, [r3, #0]
	movs	r3, #216
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	mov	r8, r3
	cmp	r1, #0
	beq.n	.L43
	adds	r6, r2, #0
	adds	r6, #220
	ldr	r3, [r6, #0]
	movs	r7, #0
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L44
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L45
	ldr	r4, [sp, #24]
	ldr	r3, [r4, #0]
	movs	r5, #228
	lsls	r3, r3, #2
	lsls	r5, r5, #1
	adds	r3, r3, r5
	mov	r7, r9
	ldr	r3, [r7, r3]
	cmp	r3, #3
	bne.n	.L46
	movs	r7, #1
	movs	r5, #1
	b.n	.L47
.L46:
	movs	r7, #32
	movs	r5, #32
.L47:
	movs	r3, #30
	b.n	.L48
.L45:
	cmp	r3, #0
	bne.n	.L49
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #0]
	movs	r2, #228
	lsls	r3, r3, #2
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r4, r9
	ldr	r3, [r4, r3]
	cmp	r3, #16
	beq.n	.L50
	ldr	r0, [sp, #40]
	cmp	r0, #0
	bne.n	.L51
	cmp	r3, #15
	bne.n	.L51
.L50:
	cmp	r1, #1
	bne.n	.L52
	cmp	r3, #15
	bne.n	.L53
	ldr	r0, [pc, #532]
	b.n	.L54
.L53:
	cmp	r3, #16
	bne.n	.L55
	ldr	r0, [pc, #528]
.L54:
	movs	r1, #15
	movs	r2, #8
	bl	Func_08021e48
	str	r0, [sp, #20]
.L55:
	movs	r1, #155
	movs	r0, #102
	bl	Func_080b5128
	movs	r1, #1
	ldr	r0, [sp, #20]
	bl	Func_08016418
	mov	r1, r8
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	movs	r3, #45
	b.n	.L48
.L52:
	movs	r3, #200
	movs	r7, #1
	movs	r5, #1
	b.n	.L48
.L51:
	movs	r3, #40
	movs	r7, #16
	movs	r5, #16
	b.n	.L48
.L49:
	movs	r3, #60
	str	r3, [r6, #0]
	movs	r7, #1
	movs	r5, #1
	b.n	.L43
.L44:
	subs	r3, #1
.L48:
	str	r3, [r6, #0]
.L43:
	movs	r3, #192
	lsls	r3, r3, #2
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L56
	ldr	r2, [sp, #52]
	cmp	r2, #0
	beq.n	.L56
	b.n	.L57
.L56:
	movs	r3, #1
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L58
	b.n	.L59
.L58:
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L60
	movs	r3, #2
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L60
	b.n	.L61
.L60:
	movs	r3, #144
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L62
	movs	r0, #111
	bl	Func_080f9010
	ldr	r4, [sp, #24]
	ldr	r5, [sp, #16]
	ldr	r0, [r4, #0]
	ldr	r1, [r5, #0]
	adds	r0, #1
	bl	Func_080022fc
	ldr	r6, [sp, #24]
	str	r0, [r6, #0]
	b.n	.L63
.L62:
	movs	r3, #96
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L64
	movs	r0, #111
	bl	Func_080f9010
	ldr	r7, [sp, #24]
	ldr	r2, [sp, #16]
	ldr	r0, [r7, #0]
	ldr	r1, [r2, #0]
	adds	r0, r0, r1
	subs	r0, #1
	bl	Func_080022fc
	str	r0, [r7, #0]
	b.n	.L63
.L64:
	ldr	r2, [pc, #332]
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	bne.n	.L63
	b.n	.L65
.L63:
	movs	r0, #128
	lsls	r0, r0, #19
	ldr	r1, [pc, #332]
	bl	Func_0800387c
	ldr	r5, [pc, #332]
	ldr	r4, [sp, #48]
	movs	r6, #0
	adds	r3, r4, r5
	strb	r6, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L66
.L5:
	ldr	r7, [sp, #52]
	cmp	r7, #0
	beq.n	.L67
	ldr	r3, [r2, #0]
	movs	r2, #224
	lsls	r2, r2, #1
	add	r2, r9
	ldr	r2, [r2, #0]
	str	r2, [r3, #64]
	b.n	.L68
.L67:
	ldr	r3, [r2, #0]
	movs	r2, #224
	lsls	r2, r2, #1
	add	r2, r9
	ldr	r2, [r2, #0]
	str	r2, [r3, #60]
.L68:
	movs	r3, #216
	lsls	r3, r3, #1
	add	r3, r9
	movs	r0, #0
	ldr	r3, [r3, #0]
	mov	r8, r0
	cmp	r8, r3
	bge.n	.L69
	movs	r6, #216
	movs	r5, #136
	lsls	r6, r6, #1
	lsls	r5, r5, #1
	add	r6, r9
	add	r5, r9
.L70:
	ldrh	r0, [r5, #0]
	bl	Func_08003f3c
	movs	r1, #1
	ldr	r3, [r6, #0]
	add	r8, r1
	adds	r5, #28
	cmp	r8, r3
	blt.n	.L70
.L69:
	ldr	r3, [sp, #48]
	ldr	r4, [pc, #232]
	adds	r2, r3, r4
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r3, #214
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r6, [pc, #224]
	movs	r5, #3
	ldr	r0, [r3, #0]
	movs	r1, #1
	ldr	r7, [pc, #220]
	bl	Func_08016418
	mov	ip, r5
	movs	r4, #0
	movs	r5, #0
	mov	lr, r6
.L73:
	adds	r3, r5, r4
	ldr	r2, [sp, #48]
	movs	r0, #0
	lsls	r3, r3, #1
	mov	r8, r0
	adds	r1, r3, r2
.L72:
	movs	r2, #0
	adds	r0, r1, #0
.L71:
	adds	r3, r2, #0
	mov	r6, ip
	ands	r3, r6
	lsls	r3, r3, #1
	adds	r3, r3, r0
	adds	r3, r3, r7
	mov	r6, lr
	adds	r2, #1
	strh	r6, [r3, #0]
	cmp	r2, #2
	ble.n	.L71
	movs	r0, #1
	add	r8, r0
	mov	r2, r8
	adds	r1, #64
	cmp	r2, #2
	ble.n	.L72
	adds	r4, #1
	adds	r5, #2
	cmp	r4, #6
	ble.n	.L73
	ldr	r3, [sp, #48]
	ldr	r4, [pc, #148]
	adds	r2, r3, r4
	movs	r3, #1
	strb	r3, [r2, #0]
	bl	Func_080030f8
	ldr	r0, [pc, #140]
	bl	Func_08004278
	movs	r0, #2
	movs	r1, #0
	movs	r2, #0
	bl	Func_0800307c
	ldr	r1, [pc, #128]
	ldr	r0, [pc, #132]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L74
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #108]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L74:
	strh	r4, [r0, #0]
	mov	r0, r9
	bl	Func_08002df0
	ldr	r6, [pc, #56]
	ldr	r5, [sp, #48]
	movs	r3, #0
	adds	r2, r5, r6
	strb	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	mov	r0, sl
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f34
	.4byte 0x03001b04
	.4byte 0x00000c4a
	.4byte 0x00000c49
	.4byte 0x00001741
	.4byte 0x00000ea6
	.4byte 0x0000f07f
	.4byte 0x0000044a
	.4byte 0x00000ea3
	.4byte 0x08021dfd
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x00001541
