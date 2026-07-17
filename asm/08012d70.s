@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08012d70
	.thumb_func
Func_08012d70:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #108]
	ldr	r6, [r3, #0]
	movs	r3, #3
	ands	r3, r0
	lsls	r3, r3, #2
	adds	r3, #40
	sub	sp, #8
	movs	r7, #0
	mov	r8, r3
	movs	r4, #0
.L2:
	mov	r3, r8
	ldr	r5, [r6, r3]
	ldr	r3, [r5, #12]
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	Func_08185000
	ldrb	r3, [r0, #5]
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r1, r3
	bge.n	.L1
	ldrb	r3, [r0, #4]
	ldr	r2, [r5, #12]
	strb	r3, [r5, #4]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	str	r3, [r5, #16]
	lsls	r3, r7, #4
	strh	r3, [r5, #2]
	movs	r3, #16
	strb	r3, [r5, #21]
	movs	r3, #255
	strb	r4, [r5, #20]
	strb	r4, [r5, #23]
	strb	r3, [r5, #22]
.L1:
	ldrb	r3, [r0, #7]
	adds	r2, r6, #0
	adds	r2, #35
	strb	r3, [r2, #0]
	strh	r4, [r6, #30]
.L0:
	adds	r7, #1
	adds	r6, #56
	cmp	r7, #9
	ble.n	.L2
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e60
