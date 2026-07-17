@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a5388
	.thumb_func
Func_080a5388:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #0
	sub	sp, #12
	mov	r8, r3
	movs	r3, #1
	str	r3, [sp, #8]
	ldr	r3, [pc, #372]
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #372]
	mov	r9, r3
	add	r6, r9
	ldrb	r0, [r6, #0]
	bl	Func_08077008
	movs	r3, #187
	str	r0, [sp, #4]
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	mov	sl, r3
	ldrb	r3, [r6, #0]
	movs	r5, #166
	adds	r0, r3, #0
	movs	r2, #0
	lsls	r5, r5, #1
	bl	Func_080a3ef0
	adds	r0, r5, #0
	bl	Func_08004938
	ldr	r3, [pc, #332]
	ldr	r1, [sp, #4]
	adds	r2, r5, #0
	mov	fp, r0
	bl	Func_080072f0
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r7, [r3, #0]
	mov	r3, sl
	ldrb	r0, [r6, #0]
	ldrh	r1, [r3, #0]
	bl	Func_08077050
	adds	r0, #2
	cmp	r0, #1
	bhi.n	.L0
	b.n	.L1
.L7:
	movs	r0, #175
	bl	Func_080f9010
	b.n	.L2
.L0:
	ldr	r5, [pc, #292]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #24
	movs	r3, #24
	adds	r5, #1
	bl	Func_08015080
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #72
	movs	r3, #24
	bl	Func_08015080
	movs	r3, #24
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r1, #16
	movs	r2, #16
	movs	r3, #96
	bl	Func_08015068
	adds	r1, r7, #0
	ldr	r0, [pc, #248]
	movs	r2, #0
	movs	r3, #16
	bl	Func_08015080
	movs	r0, #110
	movs	r1, #32
	bl	Func_080a1ac0
	b.n	.L3
.L8:
	mov	r3, r8
	lsls	r0, r3, #1
	add	r0, r8
	lsls	r0, r0, #4
	adds	r0, #110
	movs	r1, #32
	bl	Func_080a1a40
	ldr	r5, [pc, #216]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L4
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	movs	r0, #111
	movs	r3, #1
	str	r3, [sp, #8]
	bl	Func_080f9010
.L4:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	movs	r3, #1
	movs	r0, #111
	add	r8, r3
	str	r3, [sp, #8]
	bl	Func_080f9010
.L5:
	movs	r0, #1
	bl	Func_080030f8
.L3:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L2
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L6
	mov	r0, r8
	movs	r3, #0
	adds	r0, #2
	movs	r1, #2
	str	r3, [sp, #8]
	bl	Func_080022fc
	mov	r8, r0
.L6:
	ldr	r1, [pc, #132]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L7
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	movs	r0, #113
	bl	Func_080f9010
.L1:
	movs	r3, #1
	mov	r8, r3
.L2:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L9
	movs	r3, #1
	mov	r8, r3
.L9:
	mov	r3, r8
	cmp	r3, #1
	bne.n	.L10
	movs	r2, #166
	ldr	r3, [pc, #60]
	ldr	r0, [sp, #4]
	mov	r1, fp
	lsls	r2, r2, #1
	bl	Func_080072f0
.L10:
	ldr	r5, [pc, #44]
	mov	r0, fp
	add	r5, r9
	bl	Func_08002df0
	ldrb	r0, [r5, #0]
	bl	Func_08077010
	ldrb	r0, [r5, #0]
	bl	Func_080772c0
	mov	r0, r8
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x0000021b
	.4byte 0x03001388
	.4byte 0x00000b2c
	.4byte 0x00000ad6
	.4byte 0x03001b04
	.4byte 0x03001c94
