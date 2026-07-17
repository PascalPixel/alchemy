@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080970f8
	.thumb_func
Func_080970f8:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #116]
	ldr	r6, [r3, #0]
	strh	r0, [r6, #24]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	adds	r5, r1, #0
	bl	Func_0808ba1c
	strh	r5, [r6, #26]
	lsls	r5, r5, #16
	adds	r7, r0, #0
	asrs	r5, r5, #16
	str	r7, [r6, #16]
	adds	r0, r5, #0
	bl	Func_0808ba1c
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r1, r3, r2
	movs	r3, #192
	lsls	r3, r3, #8
	ands	r1, r3
	str	r0, [r6, #20]
	str	r1, [r6, #0]
	cmp	r0, #0
	beq.n	.L0
	ldr	r3, [r0, #108]
	str	r3, [r6, #56]
	ldr	r3, [r0, #0]
	str	r3, [r6, #60]
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	ldrb	r2, [r3, #5]
	adds	r3, r6, #0
	adds	r3, #68
	strb	r2, [r3, #0]
	ldr	r3, [r0, #8]
	str	r3, [r6, #4]
	ldr	r3, [r0, #16]
	str	r3, [r6, #12]
	ldr	r3, [r0, #12]
	str	r3, [r6, #8]
	b.n	.L1
.L0:
	ldr	r3, [r7, #8]
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #12]
	ldr	r3, [r7, #12]
	movs	r0, #128
	str	r3, [r6, #8]
	lsls	r0, r0, #13
	adds	r2, r6, #4
	bl	Func_0800447c
.L1:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
