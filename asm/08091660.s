@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08091660
	.thumb_func
Func_08091660:
	push	{r5, lr}
	ldr	r5, [pc, #72]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #48]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r0, #52]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r0, #56]
	str	r3, [r0, #64]
	movs	r2, #249
	movs	r3, #0
	str	r3, [r0, #36]
	str	r3, [r0, #44]
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	movs	r1, #12
	bl	Func_08009080
	b.n	.L1
.L0:
	movs	r1, #1
	bl	Func_08009080
.L1:
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
