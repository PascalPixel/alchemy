@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b3284
	.thumb_func
Func_080b3284:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r9, r1
	sub	sp, #4
	adds	r6, r0, #0
	bl	Func_080b010c
	ldr	r3, [pc, #236]
	ldr	r1, [pc, #240]
	ldr	r5, [r3, #0]
	movs	r2, #1
	adds	r3, r5, r1
	strb	r2, [r3, #0]
	cmp	r6, #5
	bne.n	.L0
	adds	r1, #3
	adds	r3, r5, r1
	strb	r2, [r3, #0]
.L0:
	mov	r0, r9
	bl	Func_0808a080
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	movs	r2, #233
	ldrh	r3, [r3, #0]
	lsls	r2, r2, #2
	adds	r7, r5, r2
	strh	r3, [r7, #0]
	movs	r2, #0
	movs	r3, #0
	movs	r1, #0
	ldrh	r0, [r7, #0]
	bl	Func_080150f8
	mov	r8, r0
	adds	r0, r6, #0
	bl	Func_080b3210
	movs	r1, #5
	adds	r6, r0, #0
	bl	Func_08015120
	ldr	r3, [pc, #176]
	mov	sl, r3
	mov	r0, sl
	bl	Func_080b04dc
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #12
	movs	r3, #4
	movs	r0, #0
	bl	Func_08015010
	str	r0, [r5, #12]
	bl	Func_080b10cc
	movs	r0, #0
	bl	Func_080b0634
	cmp	r0, #0
	beq.n	.L1
	mov	r0, sl
	adds	r0, #3
	b.n	.L2
.L1:
	ldr	r3, [pc, #132]
	ldr	r3, [r3, #16]
	cmp	r6, r3
	bls.n	.L3
	mov	r0, sl
	adds	r0, #2
.L2:
	bl	Func_080b04dc
	ldr	r0, [r5, #12]
	movs	r1, #2
	bl	Func_08015018
	b.n	.L4
.L3:
	movs	r1, #2
	ldr	r0, [r5, #12]
	bl	Func_08015018
	mov	r0, sl
	adds	r0, #1
	bl	Func_080b04dc
	movs	r1, #2
	mov	r0, r8
	bl	Func_08015018
	adds	r0, r6, #0
	bl	Func_080b3398
	mov	r0, r9
	bl	Func_0808a080
	ldr	r3, [r0, #80]
	ldr	r3, [r3, #40]
	ldrh	r3, [r3, #0]
	strh	r3, [r7, #0]
	movs	r1, #0
	ldrh	r0, [r7, #0]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080150f8
	mov	r8, r0
	mov	r0, sl
	adds	r0, #4
	bl	Func_080b04dc
.L4:
	mov	r0, r8
	movs	r1, #2
	bl	Func_08015018
	bl	Func_080b0204
	movs	r0, #0
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x000003a9
	.4byte 0x00000d1c
	.4byte 0x02000240
