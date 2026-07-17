@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b9b30
	.thumb_func
Func_080b9b30:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r0
	movs	r3, #0
	ldrsh	r0, [r0, r3]
	movs	r2, #0
	sub	sp, #32
	mov	sl, r1
	mov	r9, r2
	cmp	r0, #255
	bne.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	bl	Func_08077008
	movs	r2, #56
	ldrsh	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L2
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L2:
	ldr	r2, [pc, #432]
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L3
	mov	r0, r8
	movs	r1, #1
	bl	Func_080bd424
.L3:
	ldr	r2, [pc, #416]
	ldr	r7, [r2, #0]
	movs	r3, #60
	str	r3, [r7, #4]
	adds	r3, r2, #0
	subs	r3, #140
	ldr	r6, [r3, #0]
	mov	r3, r9
	str	r3, [r7, #20]
	ldr	r3, [pc, #404]
	subs	r2, #128
	adds	r1, r6, r3
	movs	r3, #128
	ldr	r5, [r2, #0]
	lsls	r3, r3, #9
	str	r3, [r1, #0]
	bl	Func_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	Func_080051d8
	movs	r0, #255
	movs	r1, #192
	ldr	r3, [pc, #376]
	lsls	r1, r1, #8
	lsls	r0, r0, #17
	bl	Func_080072f0
	adds	r1, r0, #0
	movs	r0, #255
	ldr	r2, [pc, #364]
	lsls	r0, r0, #17
	bl	Func_08005258
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L4
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r7, #0]
	mov	r0, sl
	bl	Func_080030f8
.L4:
	mov	r2, r8
	ldrh	r3, [r2, #0]
	add	r0, sp, #28
	strh	r3, [r0, #0]
	movs	r3, #255
	strh	r3, [r0, #2]
	movs	r1, #1
	bl	Func_080c10e8
	ldr	r3, [pc, #324]
	mov	r0, r8
	adds	r1, r6, r3
	bl	Func_080be378
	cmp	r0, #0
	bne.n	.L5
	movs	r2, #213
	lsls	r2, r2, #3
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	subs	r3, #1
	cmp	r3, #8
	bhi.n	.L6
	ldr	r2, [pc, #300]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080b9c28
	.4byte 0x080b9c34
	.4byte 0x080b9c58
	.4byte 0x080b9c76
	.4byte 0x080b9c40
	.4byte 0x080b9c5e
	.4byte 0x080b9c84
	.4byte 0x080b9c6a
	.4byte 0x080b9c4c
	ldr	r3, [pc, #252]
	movs	r1, #0
	adds	r0, r6, r3
	bl	Func_080ba27c
	b.n	.L6
	ldr	r2, [pc, #240]
	movs	r1, #0
	adds	r0, r6, r2
	bl	Func_080ba2c0
	b.n	.L6
	ldr	r3, [pc, #228]
	movs	r1, #1
	adds	r0, r6, r3
	bl	Func_080b9ec0
	b.n	.L6
	ldr	r2, [pc, #216]
	movs	r1, #0
	adds	r0, r6, r2
	bl	Func_080b9ec0
	b.n	.L6
	ldr	r3, [pc, #204]
	movs	r1, #0
	b.n	.L7
	ldr	r2, [pc, #200]
	movs	r1, #1
	adds	r0, r6, r2
	bl	Func_080ba978
	b.n	.L6
	ldr	r3, [pc, #188]
	movs	r1, #2
.L7:
	adds	r0, r6, r3
	bl	Func_080ba978
	b.n	.L6
	ldr	r2, [pc, #176]
	movs	r1, #0
	adds	r0, r6, r2
	mov	r2, r8
	bl	Func_080ba6ac
	b.n	.L6
	ldr	r3, [pc, #160]
	adds	r0, r6, r3
	bl	Func_080b9dc4
	cmp	r0, #0
	beq.n	.L8
	movs	r2, #1
	mov	r9, r2
.L8:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L6
	b.n	.L9
.L5:
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L10
	bl	Func_080bb65c
	movs	r0, #3
	bl	Func_080030f8
.L10:
	movs	r0, #0
	movs	r1, #0
	bl	Func_080c10e8
.L6:
	bl	Func_080b7e7c
	ldr	r3, [pc, #108]
	adds	r0, r6, r3
	bl	Func_080bfba4
	bl	Func_080b6c90
	mov	r5, sp
	movs	r0, #3
	adds	r1, r5, #0
	bl	Func_080b6c08
	cmp	r0, #0
	ble.n	.L11
	adds	r6, r5, #0
	adds	r5, r0, #0
.L12:
	ldrh	r0, [r6, #0]
	subs	r5, #1
	adds	r6, #2
	bl	Func_080b8000
	cmp	r5, #0
	bne.n	.L12
.L11:
	movs	r3, #255
	mov	r2, r8
	strh	r3, [r2, #0]
.L9:
	ldr	r3, [pc, #64]
	movs	r2, #201
	ldr	r3, [r3, #0]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	movs	r0, #2
	ldrh	r1, [r3, #0]
	movs	r2, #0
	bl	Func_080c0774
	mov	r0, r9
.L1:
	add	sp, #32
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000129
	.4byte 0x03001f00
	.4byte 0x00000644
	.4byte 0x0300013c
	.4byte 0x7fff0000
	.4byte 0x00000654
	.4byte 0x080b9c04
	.4byte 0x03001e74
