@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b56e0
	.thumb_func
Func_080b56e0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	mov	sl, r2
	bl	Func_08077098
.L18:
	movs	r5, #181
	lsls	r5, r5, #1
	bl	Func_0800479c
	bl	Func_08004760
	bl	Func_080040e8
	bl	Func_08004858
	bl	Func_0800403c
	adds	r0, r5, #0
	bl	Func_080770c8
	ldr	r3, [pc, #312]
	ldr	r3, [r3, #0]
	movs	r2, #128
	ands	r3, r2
	ldr	r6, [pc, #308]
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	movs	r3, #1
	negs	r3, r3
	adds	r0, r5, #0
	mov	r8, r3
	bl	Func_080770d0
	ldr	r2, [pc, #292]
	movs	r3, #85
	mov	r9, r2
	negs	r3, r3
	add	r3, r9
	ldr	r5, [pc, #288]
	movs	r7, #0
	mov	fp, r3
.L17:
	movs	r0, #32
	bl	Func_080770d0
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L2
.L14:
	ldr	r3, [r5, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	bl	Func_080b5534
.L3:
	ldr	r3, [r5, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L4
	bl	Func_080c2a08
.L4:
	ldr	r3, [r5, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L5
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L6
.L5:
	movs	r3, #1
	mov	sl, r3
	movs	r3, #5
	mov	r2, r9
	strb	r3, [r2, #0]
.L6:
	cmp	r7, r8
	beq.n	.L7
	bl	Func_08077098
	adds	r0, r7, #0
	bl	Func_080b5368
	mov	r8, r7
.L7:
	movs	r0, #1
	bl	Func_080030f8
.L2:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	adds	r6, #1
.L8:
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L9
	subs	r6, #1
.L9:
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L10
	subs	r6, #10
.L10:
	ldr	r3, [r5, #0]
	movs	r1, #128
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L11
	adds	r6, #10
.L11:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L12
	adds	r7, #1
.L12:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L13
	subs	r7, #1
.L13:
	ldr	r3, [r5, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L14
	ldr	r3, [pc, #92]
	ldr	r3, [r3, #0]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L15
	movs	r0, #182
	lsls	r0, r0, #1
	bl	Func_080770c8
.L15:
	movs	r0, #0
	bl	Func_08077010
	ldr	r3, [pc, #84]
	mov	r2, fp
	strh	r3, [r2, #0]
	cmp	r6, #28
	bne.n	.L16
	movs	r0, #183
	lsls	r0, r0, #1
	bl	Func_080770c8
.L16:
	movs	r0, #177
	lsls	r0, r0, #1
	bl	Func_080770c8
	adds	r0, r6, #0
	bl	Func_080b63c8
	bl	Func_0800479c
	bl	Func_08004760
	bl	Func_080040e8
	bl	Func_08004858
	bl	Func_0800403c
	b.n	.L17
.L1:
	movs	r0, #177
	lsls	r0, r0, #1
	bl	Func_080770c8
	ldr	r0, [pc, #12]
	bl	Func_080b63c8
	b.n	.L18
	movs	r0, r0
	.4byte 0x03001ae8
	.4byte 0x00000101
	.4byte 0x0200046b
	.4byte 0x03001b04
	.4byte 0x0000001d
