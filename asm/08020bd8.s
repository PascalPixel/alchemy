@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08020bd8
	.thumb_func
Func_08020bd8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #96
	mov	r2, sp
	movs	r1, #0
	adds	r2, #81
	str	r1, [sp, #36]
	str	r1, [sp, #32]
	str	r1, [sp, #28]
	add	r6, sp, #80
	str	r2, [sp, #24]
	str	r0, [sp, #44]
	bl	Func_08077008
	ldr	r3, [pc, #340]
	str	r0, [sp, #20]
	ldr	r3, [r3, #0]
	str	r3, [sp, #16]
	movs	r3, #1
	str	r3, [sp, #12]
	mov	r9, r3
	bl	Func_0800479c
	movs	r5, #2
	movs	r1, #6
	movs	r2, #24
	movs	r3, #9
	movs	r0, #3
	str	r5, [sp, #0]
	bl	Func_080162d4
	movs	r1, #3
	mov	r8, r0
	movs	r2, #8
	movs	r3, #3
	movs	r0, #8
	str	r5, [sp, #0]
	bl	Func_080162d4
	str	r0, [sp, #40]
	ldr	r0, [sp, #44]
	bl	Func_08019d2c
	movs	r2, #3
	movs	r3, #1
	movs	r1, #0
	bl	Func_08019da8
	ldr	r1, [pc, #276]
	mov	r0, r8
	bl	Func_080209d0
	movs	r3, #7
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #18
	movs	r2, #0
	movs	r3, #18
	bl	Func_0801e41c
	ldr	r2, [pc, #256]
	ldr	r1, [sp, #16]
	adds	r3, r1, r2
	add	r1, sp, #12
	add	r2, sp, #36
	ldrb	r1, [r1, #0]
	ldrb	r2, [r2, #0]
	strb	r1, [r3, #0]
	strb	r2, [r6, #0]
	mov	r0, sp
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #20]
	adds	r0, #94
.L1:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [sp, #32]
	adds	r3, #1
	str	r3, [sp, #32]
	ldr	r3, [sp, #28]
	adds	r3, #1
	str	r3, [sp, #28]
.L0:
	cmp	r1, r0
	ble.n	.L1
	ldr	r1, [sp, #24]
	movs	r3, #0
	strb	r3, [r1, #14]
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #20]
	bl	Func_08020b64
	bl	Func_08004080
	adds	r5, r0, #0
	movs	r6, #18
	movs	r7, #5
	cmp	r5, #95
	bgt.n	.L2
	ldr	r2, [pc, #176]
	movs	r1, #128
	bl	Func_08003fa4
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	adds	r0, r5, #0
	mov	r2, r8
	str	r3, [sp, #0]
	bl	Func_0801eadc
	add	r2, sp, #64
	adds	r5, r0, #0
	str	r5, [r2, #0]
	mov	r3, r8
	mov	fp, r2
	ldrh	r1, [r3, #12]
	ldrh	r2, [r3, #14]
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	adds	r1, #140
	adds	r2, #52
	mov	r0, fp
	bl	Func_080b0038
	b.n	.L3
.L2:
	add	r1, sp, #64
	mov	fp, r1
.L3:
	bl	Func_08004080
	adds	r5, r0, #0
	cmp	r5, #95
	bgt.n	.L4
	ldr	r2, [pc, #112]
	movs	r1, #128
	bl	Func_08003fa4
	movs	r1, #128
	lsls	r1, r1, #23
	movs	r3, #0
	adds	r0, r5, #0
	mov	r2, r8
	str	r3, [sp, #0]
	bl	Func_0801eadc
	movs	r2, #48
	adds	r5, r0, #0
	add	r2, sp
	str	r5, [r2, #0]
	movs	r3, #255
	strb	r3, [r5, #15]
	mov	sl, r2
	movs	r3, #13
	ldrb	r2, [r5, #25]
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r5, #25]
	ldr	r0, [sp, #24]
	bl	Func_08020b14
	adds	r1, r0, #0
	adds	r1, #70
	mov	r0, sl
	movs	r2, #22
	bl	Func_080b0038
	b.n	.L5
.L31:
	mov	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r0, [sp, #40]
	bl	Func_08016478
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #20]
	bl	Func_08020b64
	movs	r0, #10
	bl	Func_080030f8
	b.n	.L6
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x08073864
	.4byte 0x00000ea3
	.4byte 0x080310a4
	.4byte 0x080317e4
.L4:
	add	r1, sp, #48
	mov	sl, r1
.L5:
	ldr	r4, [pc, #48]
	ldr	r3, [pc, #52]
	ldr	r0, [pc, #52]
	adds	r1, r4, #0
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #32]
	strh	r3, [r4, #8]
	ldr	r2, [sp, #32]
	ldr	r3, [sp, #24]
	adds	r2, r2, r3
	str	r2, [sp, #8]
.L26:
	movs	r5, #1
	cmp	r6, #18
	bne.n	.L7
	cmp	r7, #4
	bne.n	.L8
	movs	r5, #3
.L8:
	cmp	r7, #5
	bne.n	.L7
	movs	r5, #3
	b.n	.L7
	movs	r0, r0
	.4byte 0x00006318
	.4byte 0x050001c0
	.4byte 0x040000d4
	.4byte 0x050001e0
	.4byte 0x84000008
.L7:
	movs	r1, #1
	movs	r3, #14
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	adds	r1, r6, #0
	adds	r2, r7, #0
	adds	r3, r5, #0
	mov	r0, r8
	bl	Func_08020a60
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #1
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	mov	r0, r8
	adds	r3, r5, #0
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	Func_08020a60
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L9
	movs	r1, #0
	mov	r2, r8
	mov	r9, r1
	ldrh	r1, [r2, #12]
	ldrh	r2, [r2, #14]
	adds	r1, r1, r6
	adds	r2, r2, r7
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	subs	r1, #7
	adds	r2, #15
	mov	r0, fp
	movs	r3, #3
	bl	Func_080b0030
.L9:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L10
	movs	r1, #0
	ldr	r0, [sp, #24]
	str	r1, [sp, #12]
	bl	Func_08020b14
	adds	r1, r0, #0
	adds	r1, #70
	mov	r0, sl
	movs	r2, #22
	movs	r3, #3
	bl	Func_080b0030
.L10:
	mov	r0, fp
	bl	Func_080b0020
	mov	r0, sl
	bl	Func_080b0028
	ldr	r3, [pc, #68]
	ldr	r0, [r3, #0]
	mov	r3, sl
	ldr	r5, [r3, #0]
	movs	r2, #7
	ldr	r4, [pc, #64]
	lsrs	r0, r0, #1
	ands	r0, r2
	ldrsb	r3, [r4, r0]
	ldrh	r1, [r5, #6]
	adds	r1, r1, r3
	ldr	r3, [pc, #40]
	ldr	r2, [pc, #40]
	ands	r1, r3
	ldrh	r3, [r5, #22]
	ands	r3, r2
	orrs	r3, r1
	adds	r0, #5
	movs	r1, #7
	ands	r0, r1
	strh	r3, [r5, #22]
	ldrb	r2, [r5, #8]
	ldrb	r3, [r4, r0]
	adds	r2, r2, r3
	strb	r2, [r5, #20]
	ldr	r5, [pc, #28]
	ldr	r2, [r5, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L11
	b.n	.L12
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x03001800
	.4byte 0x080371f6
	.4byte 0x03001b04
.L12:
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	mov	r9, r2
	subs	r7, #1
	cmp	r6, #18
	beq.n	.L13
	movs	r3, #1
	negs	r3, r3
	cmp	r7, r3
	bne.n	.L11
	movs	r7, #5
	b.n	.L11
.L13:
	movs	r3, #3
	eors	r3, r7
	negs	r2, r3
	orrs	r2, r3
	lsrs	r7, r2, #31
	movs	r3, #5
	subs	r7, r3, r7
.L11:
	ldr	r2, [r5, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L14
	movs	r0, #111
	bl	Func_080f9010
	movs	r1, #1
	mov	r9, r1
	adds	r7, #1
	cmp	r6, #18
	beq.n	.L15
	cmp	r7, #6
	bne.n	.L14
	movs	r7, #0
	b.n	.L14
.L15:
	movs	r2, #6
	eors	r2, r7
	negs	r3, r2
	orrs	r3, r2
	lsrs	r7, r3, #31
	adds	r7, #4
.L14:
	ldr	r2, [r5, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L16
	movs	r0, #111
	bl	Func_080f9010
	movs	r3, #1
	movs	r2, #1
	subs	r6, #1
	negs	r3, r3
	mov	r9, r2
	cmp	r6, r3
	bne.n	.L17
	subs	r3, r7, #4
	movs	r6, #18
	cmp	r3, #1
	bls.n	.L16
	movs	r6, #16
	b.n	.L16
.L17:
	cmp	r6, #5
	beq.n	.L18
	cmp	r6, #11
	beq.n	.L18
	cmp	r6, #17
	bne.n	.L16
.L18:
	ldr	r5, [pc, #340]
	subs	r6, #1
.L16:
	ldr	r2, [r5, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L19
	movs	r0, #111
	bl	Func_080f9010
	adds	r6, #1
	movs	r1, #1
	mov	r9, r1
	cmp	r6, #19
	bne.n	.L20
	movs	r6, #0
	b.n	.L21
.L20:
	cmp	r6, #5
	beq.n	.L22
	cmp	r6, #11
	beq.n	.L22
	cmp	r6, #17
	bne.n	.L21
.L22:
	adds	r6, #1
.L21:
	cmp	r6, #18
	bne.n	.L19
	subs	r3, r7, #4
	cmp	r3, #1
	bls.n	.L19
	movs	r6, #0
.L19:
	ldr	r3, [pc, #284]
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L23
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	mov	r9, r2
	movs	r6, #18
	movs	r7, #5
.L23:
	ldr	r2, [pc, #252]
	ldr	r5, [r2, #0]
	movs	r3, #2
	ands	r5, r3
	cmp	r5, #0
	beq.n	.L24
	movs	r0, #113
	bl	Func_080f9010
.L34:
	ldr	r3, [sp, #28]
	cmp	r3, #0
	beq.n	.L25
	ldr	r1, [sp, #8]
	subs	r3, #1
	str	r3, [sp, #28]
	subs	r1, #1
	movs	r3, #0
	str	r1, [sp, #8]
	strb	r3, [r1, #0]
	ldr	r0, [sp, #40]
	bl	Func_08016478
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #24]
	bl	Func_08020b64
	movs	r2, #1
	str	r2, [sp, #12]
	b.n	.L26
.L25:
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #36]
	b.n	.L6
.L24:
	ldr	r3, [r2, #0]
	movs	r1, #1
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L27
	b.n	.L26
.L27:
	movs	r0, #112
	bl	Func_080f9010
	cmp	r6, #18
	bne.n	.L28
	cmp	r7, #5
	bne.n	.L29
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L30
	b.n	.L31
.L30:
	ldr	r2, [sp, #20]
	ldr	r1, [sp, #24]
	movs	r0, #0
.L32:
	ldrb	r3, [r1, #0]
	adds	r0, #1
	strb	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #1
	cmp	r0, #14
	ble.n	.L32
	b.n	.L6
.L29:
	cmp	r7, #4
	beq.n	.L33
	b.n	.L26
.L33:
	b.n	.L34
.L28:
	mov	r3, r8
	ldrh	r2, [r3, #12]
	ldrh	r3, [r3, #14]
	adds	r3, r3, r7
	adds	r2, r2, r6
	adds	r3, #1
	adds	r2, #1
	lsls	r3, r3, #5
	adds	r3, r3, r2
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #28]
	lsls	r3, r3, #1
	ldrb	r3, [r3, r1]
	cmp	r2, #5
	bne.n	.L35
	b.n	.L26
.L35:
	ldr	r1, [sp, #8]
	adds	r2, #1
	strb	r3, [r1, #0]
	adds	r1, #1
	str	r1, [sp, #8]
	strb	r5, [r1, #0]
	str	r2, [sp, #28]
	cmp	r2, #5
	bne.n	.L36
	movs	r2, #1
	movs	r6, #18
	movs	r7, #5
	mov	r9, r2
.L36:
	ldr	r0, [sp, #40]
	bl	Func_08016478
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #24]
	bl	Func_08020b64
	movs	r3, #1
	str	r3, [sp, #12]
	b.n	.L26
.L6:
	mov	r0, r8
	movs	r1, #2
	bl	Func_08016418
	movs	r1, #2
	ldr	r0, [sp, #40]
	bl	Func_08016418
	ldr	r0, [sp, #44]
	bl	Func_08019e48
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [sp, #36]
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001b04
	.4byte 0x03001c94
