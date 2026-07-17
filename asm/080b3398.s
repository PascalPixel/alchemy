@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b3398
	.thumb_func
Func_080b3398:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #16
	mov	r8, sp
	adds	r5, r0, #0
	mov	r0, r8
	bl	Func_08077158
	negs	r5, r5
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	Func_08077230
	cmp	r7, #0
	ble.n	.L0
	mov	sl, r8
	movs	r6, #0
	adds	r5, r7, #0
.L2:
	mov	r2, sl
	ldrsh	r0, [r6, r2]
	bl	Func_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	beq.n	.L1
	ldrh	r3, [r0, #52]
	strh	r3, [r0, #56]
	ldrh	r3, [r0, #54]
	strh	r3, [r0, #58]
	mov	r3, r8
	ldrsh	r0, [r6, r3]
	bl	Func_08077128
.L1:
	subs	r5, #1
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L2
.L0:
	ldr	r6, [pc, #84]
	movs	r3, #224
	ldr	r1, [r6, #0]
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r5, #228
	adds	r3, #73
	str	r3, [r2, #0]
	lsls	r5, r5, #1
	movs	r3, #60
	str	r3, [r1, r5]
	movs	r0, #20
	bl	Func_080030f8
	bl	Func_0808a368
	bl	Func_0808a370
	movs	r0, #86
	bl	Func_080f9010
	bl	Func_080b04c4
	movs	r0, #10
	bl	Func_080030f8
	bl	Func_0808a360
	bl	Func_0808a370
	movs	r0, #30
	bl	Func_080030f8
	ldr	r2, [r6, #0]
	movs	r3, #16
	str	r3, [r2, r5]
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
