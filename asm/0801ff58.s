@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801ff58
	.thumb_func
Func_0801ff58:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #0
	ldr	r3, [pc, #112]
	mov	r8, r2
	movs	r2, #250
	lsls	r2, r2, #17
	ldr	r3, [r3, #0]
	mov	sl, r2
	movs	r2, #154
	lsls	r2, r2, #1
	sub	sp, #28
	adds	r7, r3, r2
	subs	r2, #32
	add	r4, sp, #4
	add	r6, sp, #12
	adds	r5, r3, r2
.L1:
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L0
	ldr	r3, [r5, #64]
	str	r3, [sp, #4]
	ldr	r3, [r5, #64]
	str	r3, [r4, #4]
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	mov	r3, sl
	str	r3, [r6, #4]
	movs	r2, #16
	ldrsh	r3, [r7, r2]
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r6, #8]
	movs	r3, #0
	str	r3, [r6, #12]
	movs	r3, #128
	adds	r2, r4, #0
	adds	r1, r6, #0
	lsls	r3, r3, #7
	str	r4, [sp, #0]
	bl	Func_08009008
	ldr	r4, [sp, #0]
.L0:
	movs	r3, #1
	add	r8, r3
	mov	r2, r8
	adds	r7, #2
	adds	r5, #4
	cmp	r2, #3
	ble.n	.L1
	add	sp, #28
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
