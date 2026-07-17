@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a195c
	.thumb_func
Func_080a195c:
	push	{r5, r6, lr}
	sub	sp, #28
	ldr	r3, [pc, #52]
	mov	r0, sp
	ldr	r5, [r3, #0]
	bl	Func_08077158
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #138
	lsls	r3, r3, #1
	adds	r6, r5, r3
	adds	r5, r0, #0
.L2:
	ldmia	r6!, {r0}
	cmp	r0, #0
	beq.n	.L1
	bl	Func_08009038
.L1:
	subs	r5, #1
	cmp	r5, #0
	bne.n	.L2
.L0:
	ldr	r0, [pc, #16]
	bl	Func_08004278
	add	sp, #28
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x080a19a1
