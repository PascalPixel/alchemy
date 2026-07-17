@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a7380
	.thumb_func
Func_080a7380:
	push	{r5, r6, r7, lr}
	movs	r1, #167
	lsls	r1, r1, #4
	movs	r0, #55
	sub	sp, #4
	bl	Func_080048b0
	ldr	r3, [pc, #152]
	ldr	r2, [r3, #0]
	movs	r3, #1
	adds	r7, r0, #0
	movs	r1, #0
	strh	r3, [r2, #4]
	movs	r0, #0
	movs	r3, #20
	movs	r2, #30
	bl	Func_08015408
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #0
	bl	Func_080a1090
	movs	r2, #130
	lsls	r2, r2, #2
	adds	r0, r7, r2
	bl	Func_08077158
	ldr	r2, [pc, #112]
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	movs	r1, #3
	movs	r0, #0
	movs	r2, #0
	movs	r3, #7
	bl	Func_080a8034
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #5
	movs	r0, #13
	bl	Func_08015010
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r7, r2
	str	r0, [r3, #0]
	movs	r0, #169
	lsls	r0, r0, #1
	ldr	r1, [pc, #56]
	movs	r2, #7
	adds	r3, r7, r0
.L0:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L0
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #3
	strh	r3, [r2, #0]
	bl	Func_080a7440
	adds	r6, r0, #0
	ldr	r0, [r7, #36]
	bl	Func_08015278
	bl	Func_080a34c0
	ldr	r3, [pc, #20]
	ldr	r3, [r3, #0]
	movs	r5, #0
	strh	r5, [r3, #4]
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L1
	movs	r0, r0
	.4byte 0x0000001e
	.4byte 0x03001e68
	.4byte 0x00000219
.L1:
	movs	r0, #55
	bl	Func_08002dd8
	adds	r0, r6, #0
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
