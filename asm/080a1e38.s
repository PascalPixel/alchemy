@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a1e38
	.thumb_func
Func_080a1e38:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #112
	str	r0, [sp, #12]
	add	r5, sp, #16
	movs	r0, #0
	str	r0, [sp, #8]
	mov	fp, r0
	adds	r0, r1, #0
	adds	r1, r5, #0
	bl	Func_080a1f74
	add	r1, sp, #48
	mov	r9, r1
	movs	r2, #0
	movs	r6, #14
.L0:
	ldr	r0, [sp, #12]
	ldrh	r3, [r2, r0]
	subs	r6, #1
	strh	r3, [r2, r1]
	adds	r2, #2
	cmp	r6, #0
	bge.n	.L0
	movs	r1, #0
	mov	r8, r1
	mov	r2, r9
	movs	r6, #14
.L2:
	ldrh	r3, [r2, #0]
	adds	r2, #2
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #1
	add	r8, r3
.L1:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L2
	mov	r0, r8
	cmp	r0, #14
	bgt.n	.L3
	add	r3, sp, #80
	lsls	r2, r0, #1
	adds	r2, r2, r3
	ldr	r1, [pc, #16]
	movs	r3, #15
	subs	r6, r3, r0
.L4:
	subs	r6, #1
	strh	r1, [r2, #0]
	adds	r2, #2
	cmp	r6, #0
	bne.n	.L4
	b.n	.L3
	.4byte 0x00000000
.L3:
	ldrb	r3, [r5, #0]
	cmp	r3, #255
	beq.n	.L5
	mov	r1, sp
	adds	r1, #80
	str	r1, [sp, #4]
	mov	sl, r9
	adds	r7, r5, #0
.L13:
	movs	r6, #0
	movs	r4, #0
	cmp	r6, r8
	bge.n	.L6
	mov	r5, r9
.L10:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L7
	adds	r0, r3, #0
	str	r4, [sp, #0]
	bl	Func_08077018
	ldrb	r1, [r7, #0]
	movs	r2, #127
	ldrb	r3, [r0, #2]
	ands	r2, r1
	ldr	r4, [sp, #0]
	cmp	r2, r3
	bne.n	.L7
	movs	r3, #128
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L8
	ldrh	r2, [r5, #0]
	ldr	r3, [pc, #8]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	b.n	.L9
	movs	r0, r0
	.4byte 0x00000200
.L8:
	ldrh	r2, [r5, #0]
.L9:
	ldr	r3, [pc, #44]
	ands	r3, r2
	cmp	r4, r3
	bge.n	.L7
	str	r6, [sp, #8]
	adds	r4, r3, #0
.L7:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r8
	blt.n	.L10
.L6:
	cmp	r4, #0
	beq.n	.L11
	ldr	r0, [sp, #8]
	mov	r3, fp
	lsls	r2, r0, #1
	mov	r0, sl
	lsls	r1, r3, #1
	ldrh	r3, [r0, r2]
	ldr	r0, [sp, #4]
	strh	r3, [r0, r1]
	ldr	r3, [pc, #8]
	mov	r1, sl
	b.n	.L12
	.4byte 0x000001ff
	.4byte 0x00000000
.L12:
	strh	r3, [r1, r2]
	movs	r3, #1
	add	fp, r3
	b.n	.L13
.L11:
	adds	r7, #1
	ldrb	r3, [r7, #0]
	cmp	r3, #255
	bne.n	.L13
.L5:
	mov	r0, r8
	cmp	r0, #0
	ble.n	.L14
	add	r1, sp, #80
	movs	r2, #0
	mov	r6, r8
.L15:
	ldrh	r3, [r2, r1]
	ldr	r0, [sp, #12]
	subs	r6, #1
	strh	r3, [r2, r0]
	adds	r2, #2
	cmp	r6, #0
	bne.n	.L15
.L14:
	movs	r0, #1
	add	sp, #112
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
