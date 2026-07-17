@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f26ec
	.thumb_func
Func_080f26ec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r0, [sp, #12]
	movs	r0, #0
	str	r0, [sp, #8]
	ldr	r5, [pc, #328]
	ldrb	r3, [r5, #0]
	movs	r1, #224
	movs	r0, #43
	str	r3, [sp, #4]
	bl	Func_080048b0
	mov	r8, r0
	bl	Func_08004760
	bl	Func_0800403c
	movs	r0, #1
	bl	Func_080030f8
	bl	Func_080040e8
	add	r1, sp, #8
	ldrb	r1, [r1, #0]
	ldr	r3, [pc, #292]
	adds	r2, r1, #0
	strb	r1, [r3, #0]
	strb	r2, [r5, #0]
	bl	Func_080f24a0
	bl	Func_080f377c
	movs	r1, #0
	movs	r0, #2
	bl	Func_080f3824
	ldr	r1, [pc, #272]
	ldr	r0, [pc, #276]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #252]
	adds	r3, #4
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L0:
	strh	r4, [r0, #0]
	movs	r0, #60
	bl	Func_080f3858
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #228]
	bl	Func_080041d8
	ldr	r0, [pc, #224]
	movs	r1, #18
	movs	r3, #1
	add	r1, sp
	mov	r9, r3
	mov	sl, r0
	mov	fp, r1
.L15:
	mov	r2, r8
	ldr	r1, [r2, #8]
	adds	r3, r1, #0
	subs	r3, #21
	cmp	r3, #217
	bhi.n	.L1
	ldr	r3, [pc, #204]
	ldr	r3, [r3, #0]
	movs	r2, #9
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	mov	r3, r9
	mov	r0, r8
	str	r3, [r0, #16]
	movs	r3, #239
	str	r3, [r0, #8]
	movs	r1, #239
.L1:
	movs	r2, #139
	adds	r3, r1, #1
	lsls	r2, r2, #1
	mov	r1, r8
	str	r3, [r1, #8]
	cmp	r3, r2
	ble.n	.L2
	b.n	.L3
.L2:
	ldr	r5, [r1, #12]
	movs	r1, #3
	adds	r0, r5, #0
	bl	Func_080022fc
	cmp	r0, #0
	bne.n	.L4
	mov	r0, sl
	ldrh	r3, [r0, #6]
	ldr	r1, [pc, #148]
	mov	r2, sl
	adds	r3, r3, r1
	strh	r3, [r2, #6]
	ldrh	r3, [r2, #6]
	movs	r2, #7
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L4
	mov	r0, r8
	ldr	r3, [r0, #0]
	lsls	r2, r3, #4
	subs	r2, r2, r3
	ldr	r0, [pc, #128]
	ldr	r1, [pc, #128]
	lsls	r2, r2, #6
	subs	r0, r0, r2
	subs	r1, r1, r2
	ldr	r3, [pc, #124]
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r1, r8
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	ldr	r5, [r1, #12]
.L4:
	mov	r3, r9
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L5
	mov	r2, sl
	ldrh	r3, [r2, #10]
	ldr	r0, [pc, #84]
	mov	r1, sl
	adds	r3, r3, r0
	strh	r3, [r1, #10]
	ldrh	r2, [r1, #10]
	movs	r3, #7
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L5
	mov	r0, r8
	ldr	r3, [r0, #4]
	lsls	r1, r3, #3
	cmp	r1, #24
	bgt.n	.L6
	lsls	r1, r3, #4
	subs	r1, r1, r3
	ldr	r2, [pc, #72]
	ldr	r0, [pc, #72]
	lsls	r1, r1, #7
	subs	r0, r0, r1
	ldr	r3, [pc, #56]
	subs	r1, r2, r1
	ldr	r2, [pc, #68]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L7
	movs	r0, r0
	.4byte 0x03001f58
	.4byte 0x03001d18
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x0000f740
	.4byte 0x080f2029
	.4byte 0x03001ad0
	.4byte 0x03001c94
	.4byte 0x0000ffff
	.4byte 0x02012580
	.4byte 0x06004b00
	.4byte 0x040000d4
	.4byte 0x800001e0
	.4byte 0x0600e4c0
	.4byte 0x020199c0
	.4byte 0x800003c0
.L6:
	mov	r3, fp
	movs	r0, #160
	strh	r2, [r3, #0]
	subs	r0, r0, r1
	movs	r1, #160
	bl	Func_080022fc
	lsls	r1, r0, #4
	subs	r1, r1, r0
	ldr	r2, [pc, #52]
	lsls	r1, r1, #4
	adds	r1, r1, r2
	ldr	r3, [pc, #48]
	mov	r0, fp
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #48]
	ldr	r3, [pc, #28]
	movs	r7, #0
.L9:
	movs	r6, #0
.L8:
	adds	r6, #1
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r6, #29
	bls.n	.L8
	strh	r3, [r1, #0]
	adds	r7, #1
	adds	r1, #2
	strh	r3, [r1, #0]
	adds	r1, #2
	cmp	r7, #4
	bls.n	.L9
	b.n	.L7
	.4byte 0x0000013b
	.4byte 0x06004ec0
	.4byte 0x040000d4
	.4byte 0x810003c0
	.4byte 0x0600f6c0
.L7:
	mov	r0, r8
	ldr	r3, [r0, #4]
	adds	r3, #1
	str	r3, [r0, #4]
.L5:
	mov	r1, r8
	ldr	r3, [r1, #8]
	cmp	r3, #239
	bne.n	.L10
	mov	r2, r9
	str	r2, [r1, #16]
	b.n	.L10
.L3:
	ldr	r0, [pc, #28]
	cmp	r3, r0
	bne.n	.L11
	movs	r3, #2
	mov	r1, r8
	str	r3, [r1, #16]
	b.n	.L10
.L11:
	ldr	r2, [pc, #16]
	cmp	r3, r2
	bne.n	.L12
	movs	r3, #0
	mov	r0, r8
	str	r3, [r0, #16]
	b.n	.L10
	movs	r0, r0
	.4byte 0x00000119
	.4byte 0x00000121
.L12:
	movs	r1, #140
	lsls	r1, r1, #1
	cmp	r3, r1
	bne.n	.L10
	movs	r0, #1
	bl	Func_08003bf8
	ldr	r0, [pc, #84]
	bl	Func_08004278
	ldr	r3, [pc, #80]
	mov	r2, r9
	strb	r2, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	mov	r0, sl
	strh	r3, [r0, #10]
	ldr	r0, [pc, #60]
	bl	Func_08002f40
	movs	r1, #160
	adds	r4, r0, #0
	ldr	r3, [pc, #52]
	lsls	r1, r1, #19
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #2
	ldr	r5, [pc, #48]
	movs	r3, #160
	adds	r4, r4, r2
	lsls	r3, r3, #19
	strh	r1, [r3, #0]
	adds	r0, r4, #0
	b.n	.L13
	.4byte 0x00000681
	.4byte 0x00001440
	.4byte 0x00000000
	.4byte 0x080f2029
	.4byte 0x03001d18
	.4byte 0x0400000c
	.4byte 0x00000016
	.4byte 0x040000d4
	.4byte 0x84000078
	.4byte 0x02010000
.L13:
	adds	r1, r5, #0
	bl	Func_080053e8
	ldr	r3, [pc, #200]
	adds	r0, r5, #0
	ldr	r1, [pc, #200]
	ldr	r2, [pc, #200]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #128
	ldr	r1, [pc, #196]
	lsls	r3, r3, #1
	movs	r7, #0
	b.n	.L14
.L10:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L15
.L17:
	movs	r6, #0
.L16:
	adds	r2, r3, #0
	movs	r0, #128
	lsls	r3, r2, #16
	lsls	r0, r0, #9
	adds	r3, r3, r0
	adds	r6, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r6, #29
	bls.n	.L16
	ldr	r2, [pc, #160]
	ldr	r0, [pc, #160]
	strh	r2, [r1, #0]
	adds	r1, #2
	strh	r0, [r1, #0]
	adds	r7, #1
	adds	r1, #2
.L14:
	cmp	r7, #19
	bls.n	.L17
	bl	Func_0800479c
	bl	Func_08004760
	ldr	r1, [sp, #12]
	cmp	r1, #0
	beq.n	.L18
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #14
	bl	Func_080048f4
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #120]
	bl	Func_080053e8
	mov	r5, r8
	adds	r5, #128
	movs	r7, #0
.L19:
	bl	Func_08004080
	lsls	r2, r7, #8
	lsrs	r2, r2, #1
	adds	r2, r6, r2
	movs	r1, #128
	bl	Func_08003fa4
	adds	r2, r5, #0
	movs	r3, #0
	stmia	r2!, {r3}
	ldr	r3, [pc, #88]
	stmia	r2!, {r3}
	adds	r7, #1
	adds	r5, #12
	str	r0, [r2, #0]
	cmp	r7, #4
	bls.n	.L19
	movs	r0, #14
	bl	Func_08002dd8
.L18:
	movs	r0, #30
	bl	Func_08003c3c
	bl	Func_08003ce0
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [sp, #12]
	movs	r2, #150
	lsls	r2, r2, #1
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L20
	movs	r0, #225
	lsls	r0, r0, #4
	mov	r9, r0
	b.n	.L20
.L25:
	movs	r1, #1
	str	r1, [sp, #8]
	b.n	.L21
	.4byte 0x00001540
	.4byte 0x040000d4
	.4byte 0x06004000
	.4byte 0x80004b00
	.4byte 0x06003000
	.4byte 0x000001ff
	.4byte 0x080f38bc
	.4byte 0x40004000
.L20:
	movs	r7, #0
	cmp	r7, r9
	bcs.n	.L21
	ldr	r2, [pc, #80]
	mov	fp, r2
.L26:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L22
	ldr	r0, [pc, #72]
	mov	r5, r8
	adds	r5, #128
	movs	r6, #0
	movs	r4, #80
	mov	sl, r0
.L23:
	ldr	r3, [pc, #52]
	adds	r2, r4, #0
	ands	r2, r3
	ldrh	r3, [r5, #6]
	mov	r1, sl
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	movs	r3, #124
	strb	r3, [r5, #4]
	adds	r0, r5, #0
	movs	r1, #0
	str	r4, [sp, #0]
	bl	Func_08003dec
	ldr	r4, [sp, #0]
	adds	r6, #1
	adds	r4, #32
	adds	r5, #12
	cmp	r6, #2
	bls.n	.L23
	movs	r1, #60
	adds	r0, r7, #0
	bl	Func_08002304
	ldr	r2, [pc, #16]
	b.n	.L24
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x04000052
	.4byte 0xfffffe00
	.4byte 0x080f39b1
.L24:
	ldr	r3, [pc, #52]
	ldrb	r1, [r2, r0]
	ldr	r2, [pc, #52]
	strh	r3, [r2, #0]
	movs	r3, #16
	subs	r3, r3, r1
	lsls	r3, r3, #8
	adds	r3, r3, r1
	mov	r0, fp
	strh	r3, [r0, #0]
.L22:
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	movs	r2, #9
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L25
	movs	r0, #1
	adds	r7, #1
	bl	Func_080030f8
	cmp	r7, r9
	bcc.n	.L26
.L21:
	add	r1, sp, #4
	ldrb	r1, [r1, #0]
	ldr	r3, [pc, #16]
	movs	r0, #43
	b.n	.L27
	movs	r0, r0
	.4byte 0x00002f50
	.4byte 0x04000050
	.4byte 0x03001c94
	.4byte 0x03001f58
.L27:
	strb	r1, [r3, #0]
	bl	Func_08002dd8
	ldr	r2, [pc, #20]
	ldr	r3, [pc, #20]
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [sp, #8]
	add	sp, #20
	b.n	.L28
	.4byte 0x00000000
	.4byte 0x04000050
.L28:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
