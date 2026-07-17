@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080196c4
	.thumb_func
Func_080196c4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #208]
	adds	r3, #200
	adds	r7, r2, #0
	ldr	r2, [r3, #0]
	mov	sl, r2
	mov	r8, r3
	mov	r3, sl
	sub	sp, #12
	mov	r9, r0
	adds	r6, r1, #0
	cmp	r3, #0
	bne.n	.L0
	ldr	r5, [pc, #188]
	movs	r0, #50
	adds	r1, r5, #0
	bl	Func_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #172]
	ldr	r0, [pc, #176]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, r8
	ldr	r3, [r2, #0]
.L0:
	mov	r5, sp
	mov	r1, r9
	adds	r0, r5, #0
	mov	r8, r3
	bl	Func_08019bac
	ldr	r3, [pc, #156]
	mov	r9, r3
	b.n	.L1
.L8:
	cmp	r0, #14
	beq.n	.L2
	cmp	r0, #14
	bhi.n	.L3
	cmp	r0, #12
	bhi.n	.L4
	cmp	r0, #8
	bcc.n	.L4
	b.n	.L5
.L3:
	cmp	r0, #15
	beq.n	.L5
	b.n	.L4
.L2:
	subs	r7, #3
	cmp	r7, #0
	ble.n	.L6
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	bl	Func_08007304
	adds	r6, #2
	add	r0, r9
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	bl	Func_08007304
	adds	r6, #2
	add	r0, r9
	b.n	.L7
.L5:
	subs	r7, #1
	cmp	r7, #0
	ble.n	.L6
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	bl	Func_08007304
	ldr	r2, [pc, #80]
	adds	r6, #2
	adds	r0, r0, r2
	b.n	.L7
.L4:
	subs	r7, #1
	cmp	r7, #0
	ble.n	.L6
.L7:
	strh	r0, [r6, #0]
	adds	r6, #2
.L1:
	adds	r0, r5, #0
	bl	Func_08007304
	cmp	r0, #0
	bne.n	.L8
.L6:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L9
	movs	r0, #50
	bl	Func_08002dd8
.L9:
	ldr	r3, [pc, #20]
	add	sp, #12
	strh	r3, [r6, #0]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001e50
	.4byte 0x00000140
	.4byte 0x040000d4
	.4byte 0x08015430
	.4byte 0x0000ffff
