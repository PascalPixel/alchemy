@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08095290
	.thumb_func
Func_08095290:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	adds	r6, r0, #0
	mov	sl, r1
	movs	r0, #30
	ldr	r1, [pc, #140]
	sub	sp, #4
	bl	Func_080048f4
	ldr	r3, [pc, #136]
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	mov	r8, r3
	mov	r0, sp
	movs	r3, #0
	mov	r9, r3
	str	r3, [r0, #0]
	adds	r1, r5, #0
	ldr	r3, [pc, #120]
	ldr	r2, [pc, #124]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r1, r8
	adds	r2, r5, #0
	adds	r0, r6, #0
	movs	r3, #1
	bl	Func_08090a5c
	movs	r3, #168
	lsls	r3, r3, #4
	adds	r6, r5, r3
	mov	r1, r8
	adds	r2, r6, #0
	mov	r0, sl
	movs	r3, #1
	bl	Func_08090a5c
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r2, r5, r3
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r3, #12
	bl	Func_0809088c
	movs	r3, #224
	lsls	r3, r3, #4
	add	r8, r3
	mov	r2, r8
	adds	r0, r5, #0
	movs	r1, #0
	movs	r3, #1
	bl	Func_08090a5c
	movs	r3, #252
	lsls	r3, r3, #5
	adds	r2, r5, r3
	movs	r3, #120
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	movs	r1, #200
	adds	r5, r5, r3
	mov	r3, r9
	strh	r3, [r5, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #40]
	bl	Func_080041d8
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x00001f88
	.4byte 0x03001ed0
	.4byte 0x040000d4
	.4byte 0x850007e2
	.4byte 0x00001f82
	.4byte 0x080949a9
