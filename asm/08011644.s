@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08011644
	.thumb_func
Func_08011644:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #176]
	ldr	r2, [pc, #180]
	ldr	r7, [r3, #0]
	mov	sl, r2
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r6, #160
	ldr	r5, [r3, #0]
	lsls	r6, r6, #19
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	ldr	r0, [r5, #0]
	mov	r8, r3
	bl	Func_08002f40
	mov	r1, sl
	bl	Func_080053e8
	mov	r3, r8
	mov	r2, sl
	strh	r3, [r2, #0]
	mov	r0, sl
	ldr	r3, [pc, #140]
	adds	r1, r6, #0
	ldr	r2, [pc, #140]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [r5, #4]
	bl	Func_08002f40
	ldr	r1, [pc, #132]
	bl	Func_08005340
	ldr	r0, [r5, #8]
	bl	Func_08002f40
	ldr	r1, [pc, #124]
	bl	Func_08005340
	ldr	r0, [r5, #12]
	bl	Func_08002f40
	ldr	r1, [pc, #116]
	bl	Func_08005340
	ldr	r0, [r5, #16]
	bl	Func_08002f40
	ldr	r1, [pc, #108]
	bl	Func_08005340
	ldr	r2, [pc, #104]
	ldr	r3, [pc, #108]
	str	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r3, #129
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #159
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [pc, #80]
	bl	Func_08002f40
	mov	r1, sl
	bl	Func_08005340
	ldr	r5, [pc, #24]
	bl	Func_080113e4
	adds	r3, r7, #0
	adds	r3, #252
	strb	r5, [r3, #0]
	ldr	r0, [pc, #60]
	bl	Func_0800439c
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001e70
	.4byte 0x02010000
	.4byte 0x040000d4
	.4byte 0x84000070
	.4byte 0x02038000
	.4byte 0x0203a000
	.4byte 0x0203c000
	.4byte 0x0203e000
	.4byte 0x03001cfc
	.4byte 0x0801161d
	.4byte 0x000000d5
	.4byte 0x0801179d
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
