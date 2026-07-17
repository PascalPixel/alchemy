@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080cc960
	.thumb_func
Func_080cc960:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #360]
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #360]
	sub	sp, #48
	str	r3, [sp, #8]
	adds	r2, r3, r1
	ldr	r3, [r2, #0]
	mov	fp, r3
	mov	r1, fp
	adds	r3, #1
	str	r3, [r2, #0]
	cmp	r1, #0
	bne.n	.L0
	movs	r2, #0
	ldr	r6, [pc, #340]
	ldr	r5, [pc, #340]
	mov	sl, r2
.L1:
	bl	Func_08004458
	movs	r3, #15
	ands	r0, r3
	adds	r3, r0, #0
	adds	r3, #48
	adds	r0, #40
	str	r3, [r5, #0]
	str	r0, [r5, #4]
	bl	Func_08004458
	ands	r0, r6
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r6
	str	r0, [r5, #16]
	bl	Func_08004458
	movs	r3, #1
	movs	r1, #128
	ands	r0, r6
	add	sl, r3
	lsls	r1, r1, #1
	str	r0, [r5, #20]
	adds	r5, #28
	cmp	sl, r1
	bne.n	.L1
.L0:
	movs	r2, #0
	add	r3, sp, #36
	str	r2, [r3, #4]
	str	r2, [r3, #8]
	mov	sl, r2
	ldr	r2, [pc, #272]
	mov	r9, r3
	mov	r8, r2
.L5:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L2
	adds	r3, #3
.L2:
	asrs	r3, r3, #2
	cmp	fp, r3
	ble.n	.L3
	mov	r1, r8
	ldr	r3, [r1, #0]
	cmp	r3, #0
	ble.n	.L3
	bl	Func_080049ac
	mov	r2, r8
	ldr	r0, [r2, #20]
	bl	Func_08004c6c
	mov	r3, r8
	ldr	r0, [r3, #12]
	bl	Func_08004bd4
	mov	r1, r8
	ldr	r0, [r1, #16]
	bl	Func_08004c1c
	mov	r2, r8
	ldr	r3, [r2, #0]
	add	r4, sp, #24
	mov	r1, r9
	str	r3, [r1, #0]
	mov	r0, r9
	adds	r1, r4, #0
	str	r4, [sp, #4]
	bl	Func_080e3944
	ldr	r4, [sp, #4]
	ldr	r3, [r4, #0]
	adds	r3, #64
	str	r3, [r4, #0]
	ldr	r3, [r4, #4]
	adds	r3, #80
	str	r3, [r4, #4]
	mov	r2, r8
	ldr	r3, [r2, #4]
	add	r7, sp, #12
	mov	r1, r9
	str	r3, [r1, #0]
	mov	r0, r9
	adds	r1, r7, #0
	bl	Func_080e3944
	ldr	r3, [r7, #0]
	adds	r3, #64
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r3, #80
	str	r3, [r7, #4]
	mov	r3, r8
	ldr	r2, [r3, #4]
	subs	r2, #4
	str	r2, [r3, #4]
	ldr	r3, [r3, #0]
	mov	r1, r8
	subs	r3, #4
	str	r3, [r1, #0]
	ldr	r4, [sp, #4]
	cmp	r2, #0
	bge.n	.L4
	movs	r3, #0
	str	r3, [r1, #4]
.L4:
	mov	r2, r8
	ldr	r5, [r2, #4]
	negs	r5, r5
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	ldr	r2, [r4, #0]
	ldr	r0, [r7, #0]
	asrs	r5, r5, #1
	adds	r6, r5, #0
	ldr	r3, [r4, #4]
	ldr	r1, [r7, #4]
	subs	r0, #1
	subs	r2, #1
	adds	r6, #48
	str	r4, [sp, #4]
	str	r6, [sp, #0]
	bl	Func_080cde90
	ldr	r4, [sp, #4]
	ldr	r1, [r7, #4]
	ldr	r3, [r4, #4]
	ldr	r2, [r4, #0]
	ldr	r0, [r7, #0]
	subs	r1, #1
	subs	r3, #1
	str	r6, [sp, #0]
	bl	Func_080cde90
	ldr	r4, [sp, #4]
	ldr	r0, [r7, #0]
	ldr	r1, [r7, #4]
	ldr	r2, [r4, #0]
	ldr	r3, [r4, #4]
	adds	r5, #56
	str	r5, [sp, #0]
	bl	Func_080cde90
.L3:
	movs	r1, #1
	add	sl, r1
	movs	r3, #28
	mov	r2, sl
	add	r8, r3
	cmp	r2, #64
	bne.n	.L5
	ldr	r3, [sp, #8]
	ldr	r1, [pc, #40]
	adds	r2, r3, r1
	movs	r3, #1
	str	r3, [r2, #0]
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x0000778c
	.4byte 0x0000ffff
	.4byte 0x02010000
	.4byte 0x00007824
