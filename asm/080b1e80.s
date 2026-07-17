@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b1e80
	.thumb_func
Func_080b1e80:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #176]
	sub	sp, #4
	ldr	r7, [r3, #0]
	mov	sl, r0
	mov	r9, r1
	bl	Func_08077008
	mov	r1, r9
	lsls	r5, r1, #1
	adds	r6, r0, #0
	adds	r5, #216
	ldrh	r0, [r6, r5]
	bl	Func_08077018
	movs	r2, #1
	str	r2, [sp, #0]
	mov	r8, r0
	ldrh	r0, [r6, r5]
	bl	Func_080b19cc
	mov	r1, r9
	mov	fp, r0
	mov	r0, sl
	bl	Func_08077020
	mov	r3, r8
	ldrb	r2, [r3, #3]
	movs	r3, #16
	ands	r3, r2
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L0
	cmp	r0, #1
	ble.n	.L0
	ldr	r0, [pc, #112]
	bl	Func_080b04dc
	movs	r1, #226
	lsls	r1, r1, #2
	adds	r3, r7, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	mov	r8, r2
	ldr	r2, [pc, #96]
	adds	r3, r7, r2
	subs	r2, #10
	adds	r5, r7, r2
	ldr	r2, [r5, #0]
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	movs	r3, #4
	strb	r3, [r2, #5]
	movs	r3, #234
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #12
	strb	r3, [r2, #0]
	movs	r0, #0
	movs	r1, #128
	movs	r2, #48
	bl	Func_080b0a6c
	mov	r1, sl
	mov	r2, fp
	movs	r0, #0
	bl	Func_080b1614
	str	r0, [sp, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [r5, #0]
	bl	Func_080a1038
	movs	r0, #0
	mov	r1, r8
	adds	r2, r6, #0
	bl	Func_080b0a6c
.L0:
	ldr	r0, [sp, #0]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00000cad
	.4byte 0x0000038a
