@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08020198
	.thumb_func
Func_08020198:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	sub	sp, #20
	adds	r7, r1, #0
	cmp	r5, #0
	beq.n	.L0
	bl	Func_08016478
	movs	r3, #4
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #0
	movs	r2, #4
	movs	r3, #13
	bl	Func_0801e41c
	adds	r0, r7, #0
	adds	r0, #16
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_0801e8b0
	ldr	r0, [pc, #108]
	adds	r1, r5, #0
	movs	r2, #72
	movs	r3, #0
	bl	Func_0801e940
	movs	r6, #0
	ldrb	r0, [r7, #28]
	movs	r1, #2
	adds	r2, r5, #0
	movs	r3, #80
	str	r6, [sp, #0]
	bl	Func_0801e9d4
	ldr	r3, [pc, #84]
	ldrb	r0, [r7, #29]
	adds	r1, r5, #0
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #16
	bl	Func_0801e7c0
	movs	r2, #0
	movs	r3, #32
	ldr	r0, [pc, #68]
	adds	r1, r5, #0
	bl	Func_0801e7c0
	ldr	r0, [r7, #32]
	add	r1, sp, #4
	bl	Func_0801f680
	adds	r1, r5, #0
	movs	r2, #48
	movs	r3, #40
	bl	Func_0801e940
	movs	r6, #48
	ldr	r0, [r7, #36]
	movs	r1, #6
	adds	r2, r5, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl	Func_0801ea08
	ldr	r0, [pc, #28]
	adds	r1, r5, #0
	movs	r2, #48
	movs	r3, #48
	bl	Func_0801e7c0
.L0:
	add	sp, #20
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080371e0
	.4byte 0x00000741
	.4byte 0x00000009
	.4byte 0x00000c88
