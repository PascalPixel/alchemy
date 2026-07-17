@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0807822c
	.thumb_func
Func_0807822c:
	push	{r5, lr}
	bl	Func_08077394
	adds	r5, r0, #0
	movs	r2, #56
	ldrsh	r0, [r5, r2]
	movs	r3, #52
	ldrsh	r1, [r5, r3]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L0
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L0
	adds	r3, r0, #0
.L0:
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L1
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #1
	strh	r3, [r5, #20]
.L1:
	movs	r3, #58
	ldrsh	r0, [r5, r3]
	movs	r2, #54
	ldrsh	r1, [r5, r2]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L2
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L2
	adds	r3, r0, #0
.L2:
	strh	r3, [r5, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L3
	movs	r2, #58
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #1
	strh	r3, [r5, #22]
.L3:
	pop	{r5}
	pop	{r0}
	bx	r0
