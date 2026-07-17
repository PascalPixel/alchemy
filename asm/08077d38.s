@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08077d38
	.thumb_func
Func_08077d38:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #8
	add	r5, sp, #4
	movs	r4, #0
	str	r4, [r5, #0]
	ldr	r3, [pc, #160]
	adds	r0, r5, #0
	ldr	r1, [pc, #160]
	ldr	r2, [pc, #164]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r4, [r5, #0]
	adds	r0, r5, #0
	ldr	r1, [pc, #156]
	ldr	r2, [pc, #160]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #128
	ldr	r2, [pc, #136]
	lsls	r3, r3, #24
.L0:
	ldr	r4, [r2, #8]
	ands	r4, r3
	cmp	r4, #0
	bne.n	.L0
	str	r4, [r5, #0]
	ldr	r3, [pc, #120]
	adds	r0, r5, #0
	ldr	r1, [pc, #136]
	ldr	r2, [pc, #140]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #120]
	movs	r1, #130
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #255
	strb	r2, [r3, #0]
	adds	r0, r5, #0
	str	r4, [r5, #0]
	ldr	r3, [pc, #92]
	ldr	r1, [pc, #120]
	ldr	r2, [pc, #120]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r4, [sp, #0]
	bl	Func_08078ee8
	ldr	r7, [pc, #80]
	movs	r3, #132
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	ldr	r1, [pc, #56]
	ldr	r2, [pc, #100]
	mov	sl, r1
	adds	r3, r7, r2
	movs	r1, #2
	strh	r1, [r3, #0]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r2, r7, r3
	ldr	r3, [pc, #44]
	mov	r8, r3
	movs	r3, #4
	strh	r3, [r2, #0]
	ldr	r3, [pc, #80]
	adds	r2, r7, r3
	movs	r3, #8
	strh	r3, [r2, #0]
	movs	r3, #134
	lsls	r3, r3, #2
	adds	r2, r7, r3
	subs	r3, #24
	strh	r3, [r2, #0]
	adds	r3, #26
	adds	r2, r7, r3
	movs	r3, #128
	lsls	r3, r3, #1
	strh	r3, [r2, #0]
	movs	r2, #135
	lsls	r2, r2, #2
	b.n	.L1
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x040000d4
	.4byte 0x02000240
	.4byte 0x850000b0
	.4byte 0x02001000
	.4byte 0x850003e1
	.4byte 0x02000040
	.4byte 0x85000080
	.4byte 0x02000500
	.4byte 0x85000298
	.4byte 0x00000212
	.4byte 0x00000216
.L1:
	adds	r3, r7, r2
	strh	r1, [r3, #0]
	movs	r1, #136
	ldr	r4, [sp, #0]
	lsls	r1, r1, #2
	adds	r3, r7, r1
	adds	r2, #6
	strh	r4, [r3, #0]
	subs	r1, #44
	adds	r3, r7, r2
	strh	r4, [r3, #0]
	adds	r3, r7, r1
	str	r4, [r3, #0]
	movs	r0, #0
	bl	Func_0807961c
	movs	r2, #131
	ldr	r4, [sp, #0]
	ldr	r5, [pc, #56]
	lsls	r2, r2, #2
	ldr	r1, [pc, #60]
	adds	r3, r7, r2
	str	r4, [r7, #16]
	subs	r2, #1
	strb	r5, [r3, #0]
	adds	r3, r7, r1
	strb	r5, [r3, #0]
	subs	r1, #5
	adds	r3, r7, r2
	strb	r5, [r3, #0]
	ldr	r6, [pc, #36]
	adds	r3, r7, r1
	mov	r2, sl
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	strb	r6, [r3, #0]
	str	r4, [r7, #0]
	bl	Func_08077cb8
	movs	r2, #174
	lsls	r2, r2, #2
	adds	r3, r7, r2
	str	r0, [r3, #0]
	ldr	r4, [sp, #0]
	ldr	r3, [pc, #16]
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x0000020a
	.4byte 0x03001c9c
.L2:
	str	r4, [r3, #0]
	ldr	r3, [pc, #88]
	mov	r1, sl
	strb	r1, [r3, #0]
	ldr	r1, [pc, #84]
	ldrb	r2, [r3, #0]
	adds	r3, r7, r1
	str	r4, [r7, #4]
	strb	r2, [r3, #0]
	ldr	r3, [pc, #80]
	ldr	r2, [pc, #80]
	strh	r4, [r3, #0]
	ldr	r3, [pc, #60]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #76]
	mov	r1, r8
	adds	r3, r7, r2
	adds	r2, #1
	strb	r1, [r3, #0]
	adds	r3, r7, r2
	strb	r1, [r3, #0]
	ldr	r1, [pc, #68]
	mov	r2, r8
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	adds	r1, #1
	ldr	r2, [pc, #60]
	adds	r3, r7, r1
	strb	r6, [r3, #0]
	adds	r1, #2
	adds	r3, r7, r2
	strb	r6, [r3, #0]
	adds	r2, #2
	adds	r3, r7, r1
	strb	r6, [r3, #0]
	adds	r1, #2
	adds	r3, r7, r2
	movs	r2, #16
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	adds	r1, #1
	strb	r2, [r3, #0]
	b.n	.L3
	movs	r0, r0
	.4byte 0xffffffff
	.4byte 0x03001d08
	.4byte 0x0000022a
	.4byte 0x03001d24
	.4byte 0x02002004
	.4byte 0x0000011d
	.4byte 0x0000011f
	.4byte 0x00000121
.L3:
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r2, #147
	lsls	r2, r2, #1
	adds	r3, r7, r2
	adds	r1, #2
	movs	r2, #32
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	ldr	r3, [pc, #32]
	adds	r1, #2
	adds	r2, r7, r3
	movs	r3, #64
	strb	r3, [r2, #0]
	adds	r2, r7, r1
	adds	r1, #1
	strb	r3, [r2, #0]
	adds	r2, r7, r1
	strb	r3, [r2, #0]
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000129
