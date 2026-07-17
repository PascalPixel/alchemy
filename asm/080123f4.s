@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080123f4
	.thumb_func
Func_080123f4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r0, [sp, #12]
	ldr	r3, [r1, #0]
	add	r0, sp, #28
	adds	r7, r2, #0
	movs	r2, #0
	str	r2, [r0, #4]
	str	r3, [r0, #0]
	ldr	r3, [r1, #8]
	add	r1, sp, #16
	str	r3, [r0, #8]
	ldr	r3, [pc, #232]
	bl	Func_080072f0
	mov	r2, sp
	adds	r2, #16
	str	r2, [sp, #0]
	ldr	r3, [pc, #224]
	ldr	r0, [r2, #8]
	ldr	r1, [sp, #12]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	ldr	r2, [sp, #0]
	ldr	r3, [r2, #4]
	subs	r0, r3, r0
	str	r0, [sp, #8]
	ldr	r3, [pc, #208]
	mov	r9, r3
	ldr	r3, [r3, #0]
	movs	r2, #0
	negs	r3, r3
	str	r3, [sp, #4]
	mov	fp, r2
	b.n	.L0
.L5:
	ldr	r3, [pc, #192]
	mov	r9, r3
.L0:
	mov	r2, r9
	ldr	r1, [r2, #16]
	mov	r3, fp
	ldr	r2, [pc, #188]
	subs	r1, r1, r3
	lsls	r1, r1, #16
	mov	sl, r2
	ldr	r0, [sp, #4]
	bl	Func_0800730c
	ldr	r3, [sp, #12]
	mov	r8, r0
	subs	r0, r0, r3
	cmp	r0, #0
	bne.n	.L1
	movs	r0, #1
.L1:
	ldr	r1, [sp, #8]
	bl	Func_0800730c
	adds	r5, r0, #0
	cmp	r5, #0
	bge.n	.L2
	movs	r1, #128
	ldr	r6, [pc, #140]
	negs	r0, r5
	lsls	r1, r1, #8
	mov	ip, pc
	bx	r6
	mov	r2, r9
	adds	r1, r0, #0
	ldr	r0, [r2, #0]
	bl	Func_0800730c
	mov	r1, r8
	str	r0, [r7, #0]
	adds	r0, r5, #0
	mov	ip, pc
	bx	r6
	ldr	r3, [sp, #0]
	ldr	r1, [r3, #8]
	ldr	r3, [r3, #4]
	subs	r1, r1, r5
	asrs	r1, r1, #4
	subs	r5, r0, r3
	asrs	r5, r5, #4
	adds	r0, r1, #0
	mov	ip, pc
	bx	r6
	adds	r3, r0, #0
	adds	r1, r5, #0
	adds	r0, r5, #0
	movs	r0, r0
	mov	ip, pc
	bx	r6
	adds	r3, r3, r0
	ldr	r2, [pc, #88]
	adds	r0, r3, #0
	bl	Func_080072ec
	lsls	r0, r0, #12
	cmp	r5, #0
	bge.n	.L3
	negs	r0, r0
.L3:
	movs	r1, #128
	lsls	r1, r1, #8
	movs	r0, r0
	mov	ip, pc
	bx	r6
	str	r0, [r7, #4]
	b.n	.L4
.L2:
	movs	r3, #0
	str	r3, [r7, #0]
	str	r3, [r7, #4]
.L4:
	movs	r2, #1
	movs	r3, #0
	add	fp, r2
	str	r3, [r7, #8]
	str	r3, [r7, #12]
	mov	r3, fp
	adds	r7, #20
	cmp	r3, #159
	ble.n	.L5
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03000250
	.4byte 0x03000118
	.4byte 0x03001ce0
	.4byte 0x0300013c
	.4byte 0x030001d8
