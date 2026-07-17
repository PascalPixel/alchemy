@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a7f44
	.thumb_func
Func_080a7f44:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #220]
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #220]
	mov	r8, r3
	add	r2, r8
	ldrb	r3, [r2, #0]
	sub	sp, #56
	cmp	r3, #1
	bls.n	.L0
	cmp	r1, #1
	bne.n	.L1
	ldrb	r3, [r2, #0]
	subs	r3, #1
	cmp	r0, r3
	bne.n	.L2
.L0:
	movs	r0, #0
	b.n	.L3
.L1:
	cmp	r0, #0
	beq.n	.L0
.L2:
	mov	r7, sp
	movs	r2, #0
	add	r3, sp, #52
	mov	ip, r7
.L4:
	str	r2, [r3, #0]
	subs	r3, #4
	cmp	r3, ip
	bge.n	.L4
	ldr	r3, [pc, #172]
	add	r3, r8
	ldrb	r3, [r3, #0]
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L5
	ldr	r5, [pc, #160]
	movs	r2, #130
	lsls	r2, r2, #2
	add	r5, r8
	adds	r4, r7, #0
	add	r2, r8
.L6:
	ldrh	r3, [r2, #0]
	stmia	r4!, {r3}
	ldrb	r3, [r5, #0]
	adds	r6, #1
	adds	r2, #2
	cmp	r6, r3
	blt.n	.L6
.L5:
	cmp	r1, #1
	bne.n	.L7
	lsls	r3, r0, #2
	adds	r1, r3, #4
	b.n	.L8
.L7:
	lsls	r3, r0, #2
	subs	r1, r3, #4
.L8:
	ldr	r6, [r7, r3]
	ldr	r2, [r7, r1]
	str	r2, [r7, r3]
	str	r6, [r7, r1]
	ldr	r3, [pc, #112]
	add	r3, r8
	ldrb	r3, [r3, #0]
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L9
	ldr	r2, [pc, #100]
	movs	r5, #130
	add	r2, r8
	lsls	r5, r5, #2
	mov	sl, r2
	add	r5, r8
.L10:
	ldrh	r0, [r5, #0]
	bl	Func_08077168
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r3
	blt.n	.L10
.L9:
	ldr	r3, [pc, #68]
	add	r3, r8
	ldrb	r3, [r3, #0]
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L11
	ldr	r3, [pc, #56]
	add	r3, r8
	mov	sl, r3
	adds	r5, r7, #0
.L12:
	ldmia	r5!, {r0}
	bl	Func_08077150
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r6, #1
	cmp	r6, r3
	blt.n	.L12
.L11:
	movs	r0, #130
	lsls	r0, r0, #2
	add	r0, r8
	bl	Func_08077158
	ldr	r3, [pc, #24]
	add	r3, r8
	strb	r0, [r3, #0]
	movs	r0, #1
.L3:
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00000219
