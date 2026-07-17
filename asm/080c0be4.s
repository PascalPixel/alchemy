@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c0be4
	.thumb_func
Func_080c0be4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #232]
	ldr	r3, [r3, #0]
	mov	fp, r2
	mov	r8, r3
	movs	r2, #12
	add	r2, r8
	adds	r5, r0, #0
	mov	r9, r1
	lsls	r0, r4, #16
	movs	r1, #100
	sub	sp, #40
	mov	sl, r2
	bl	Func_080022ec
	mov	r3, sl
	mov	r2, r9
	str	r2, [r3, #4]
	mov	r2, fp
	str	r5, [r3, #0]
	str	r2, [r3, #8]
	movs	r6, #255
	ldr	r2, [pc, #192]
	add	r3, sp, #4
	movs	r5, #0
	lsls	r6, r6, #17
	movs	r1, #192
	str	r5, [r3, #0]
	str	r5, [r3, #4]
	str	r5, [r3, #8]
	adds	r7, r0, #0
	mov	fp, r2
	adds	r0, r6, #0
	lsls	r1, r1, #8
	mov	r9, r3
	bl	Func_08007310
	lsls	r2, r6, #1
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	Func_08005258
	bl	Func_080049ac
	mov	r0, sl
	bl	Func_08004cb4
	mov	r2, r8
	movs	r3, #54
	ldrsh	r0, [r2, r3]
	bl	Func_08004c1c
	mov	r2, r8
	movs	r3, #52
	ldrsh	r0, [r2, r3]
	bl	Func_08004bd4
	add	r0, sp, #28
	str	r5, [r0, #0]
	str	r5, [r0, #4]
	mov	r2, r8
	ldr	r3, [r2, #32]
	mov	r1, r8
	str	r3, [r0, #8]
	ldr	r3, [pc, #112]
	bl	Func_080072f0
	ldr	r3, [pc, #112]
	movs	r5, #120
	str	r5, [r3, #12]
	str	r5, [r3, #16]
	bl	Func_080049ac
	mov	r0, r8
	mov	r1, sl
	bl	Func_080051d8
	add	r6, sp, #16
	adds	r1, r6, #0
	mov	r0, r9
	bl	Func_08005268
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #0]
	subs	r2, r5, r2
	subs	r5, r5, r3
	lsls	r5, r5, #8
	movs	r1, #240
	adds	r3, r5, #0
	lsls	r1, r1, #15
	lsls	r5, r7, #8
	lsls	r2, r2, #8
	adds	r0, r1, #0
	subs	r5, r5, r7
	str	r7, [sp, #0]
	lsls	r6, r5, #1
	bl	Func_080c0a24
	movs	r1, #192
	adds	r0, r6, #0
	lsls	r1, r1, #8
	bl	Func_08007310
	lsls	r5, r5, #2
	adds	r1, r0, #0
	adds	r2, r5, #0
	adds	r0, r6, #0
	bl	Func_08005258
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e80
	.4byte 0x0300013c
	.4byte 0x03000250
	.4byte 0x03001ce0
