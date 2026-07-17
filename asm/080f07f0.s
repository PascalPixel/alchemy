@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f07f0
	.thumb_func
Func_080f07f0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r6, #144
	lsls	r6, r6, #4
	sub	sp, #44
	mov	sl, r0
	adds	r0, r6, #0
	str	r1, [sp, #8]
	adds	r7, r2, #0
	bl	Func_08004970
	movs	r1, #0
	movs	r2, #192
	mov	r3, sl
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	mov	r9, r2
	cmp	r3, #0
	bne.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	movs	r5, #128
	lsls	r5, r5, #2
	adds	r0, r5, #0
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L2
	ldr	r3, [pc, #496]
	ldr	r0, [sp, #4]
	adds	r1, r6, #0
	movs	r2, #0
	bl	Func_080072f0
	adds	r0, r5, #0
	bl	Func_080770c8
	b.n	.L3
.L2:
	ldr	r4, [sp, #4]
	movs	r5, #128
	lsls	r5, r5, #4
	movs	r2, #128
	adds	r1, r4, r5
	ldr	r3, [pc, #472]
	lsls	r2, r2, #1
	adds	r0, r4, #0
	bl	Func_080072f0
	movs	r2, #128
	ldr	r1, [sp, #4]
	lsls	r2, r2, #1
	adds	r0, r1, r2
	ldr	r3, [pc, #448]
	adds	r1, r5, #0
	movs	r2, #0
	bl	Func_080072f0
.L3:
	mov	r4, sl
	ldrb	r0, [r4, #0]
	movs	r3, #0
	mov	r8, r3
	adds	r4, #1
	cmp	r0, #0
	beq.n	.L4
	ldr	r2, [pc, #432]
.L6:
	cmp	r0, #31
	bls.n	.L5
	adds	r3, r0, #0
	subs	r3, #32
	ldrb	r3, [r2, r3]
	add	r8, r3
.L5:
	ldrb	r0, [r4, #0]
	adds	r4, #1
	cmp	r0, #0
	bne.n	.L6
.L4:
	cmp	r7, #2
	bne.n	.L7
	mov	r4, r9
	mov	r1, r8
	subs	r4, r4, r1
	str	r4, [sp, #0]
	b.n	.L8
.L7:
	cmp	r7, #1
	bne.n	.L8
	mov	r2, r9
	mov	r4, r8
	subs	r3, r2, r4
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #0]
.L8:
	mov	r4, sl
	ldrb	r0, [r4, #0]
	movs	r1, #0
	adds	r4, #1
	mov	r8, r1
	mov	sl, r4
	cmp	r0, #0
	beq.n	.L9
.L15:
	cmp	r0, #31
	bls.n	.L10
	movs	r2, #32
	negs	r2, r2
	adds	r2, r2, r0
	ldr	r1, [pc, #356]
	lsls	r3, r2, #3
	adds	r4, r1, r3
	mov	lr, r2
	ldr	r1, [sp, #0]
	ldr	r2, [sp, #4]
	adds	r3, r2, r1
	mov	r2, r8
	adds	r1, r3, r2
	movs	r3, #0
	mov	ip, r3
	movs	r2, #1
	movs	r3, #15
	mov	fp, r2
	mov	r9, r3
.L13:
	ldr	r3, [pc, #332]
	ldrb	r7, [r4, #0]
	movs	r6, #128
	adds	r4, #1
	movs	r5, #7
	adds	r2, r1, r3
.L12:
	adds	r3, r7, #0
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L11
	mov	r3, fp
	strb	r3, [r2, #0]
	mov	r3, r9
	strb	r3, [r1, #0]
.L11:
	subs	r5, #1
	adds	r2, #1
	adds	r1, #1
	lsrs	r6, r6, #1
	cmp	r5, #0
	bge.n	.L12
	movs	r2, #1
	add	ip, r2
	mov	r3, ip
	adds	r1, #248
	cmp	r3, #7
	ble.n	.L13
	movs	r3, #1
	cmp	r0, #31
	bls.n	.L14
	ldr	r4, [pc, #264]
	mov	r1, lr
	ldrb	r3, [r4, r1]
.L14:
	add	r8, r3
.L10:
	mov	r2, sl
	ldrb	r0, [r2, #0]
	movs	r3, #1
	add	sl, r3
	cmp	r0, #0
	bne.n	.L15
.L9:
	movs	r4, #24
	movs	r2, #96
	mov	sl, r4
	ldr	r4, [sp, #4]
	mov	r8, r2
	movs	r6, #128
	movs	r3, #7
	movs	r2, #192
	adds	r1, r4, #0
	movs	r7, #96
	lsls	r6, r6, #1
	mov	ip, r3
	mov	lr, r2
.L18:
	cmp	r7, #0
	beq.n	.L16
	mov	r5, r8
	adds	r2, r4, #0
.L17:
	ldrb	r3, [r2, #1]
	ldrb	r0, [r2, #0]
	lsls	r3, r3, #4
	orrs	r0, r3
	subs	r5, #1
	strb	r0, [r1, #0]
	adds	r2, #2
	adds	r4, #2
	adds	r1, #1
	cmp	r5, #0
	bne.n	.L17
.L16:
	subs	r3, r1, r7
	mov	r2, lr
	adds	r1, r3, r6
	subs	r3, r4, r2
	adds	r4, r3, r6
	movs	r3, #1
	negs	r3, r3
	add	ip, r3
	mov	r2, ip
	cmp	r2, #0
	bge.n	.L18
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L19
	ldr	r4, [sp, #8]
	ldr	r0, [sp, #4]
	lsls	r1, r4, #5
	mov	ip, sl
.L20:
	ldr	r3, [pc, #164]
	ldr	r4, [pc, #168]
	adds	r2, r1, r3
	ldr	r3, [r0, #0]
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r4, #128
	lsls	r4, r4, #1
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #128
	str	r3, [r2, #0]
	ldr	r3, [pc, #148]
	lsls	r4, r4, #2
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r2, #0]
	ldr	r3, [pc, #140]
	lsls	r4, r4, #2
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #128
	str	r3, [r2, #0]
	ldr	r3, [pc, #128]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #160
	str	r3, [r2, #0]
	ldr	r3, [pc, #120]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #192
	str	r3, [r2, #0]
	ldr	r3, [pc, #108]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	movs	r4, #224
	str	r3, [r2, #0]
	ldr	r3, [pc, #100]
	lsls	r4, r4, #3
	adds	r2, r1, r3
	adds	r3, r0, r4
	ldr	r3, [r3, #0]
	str	r3, [r2, #0]
	movs	r2, #1
	negs	r2, r2
	add	ip, r2
	mov	r3, ip
	adds	r1, #32
	adds	r0, #4
	cmp	r3, #0
	bne.n	.L20
.L19:
	ldr	r0, [sp, #4]
	bl	Func_08002df0
	movs	r0, #0
.L1:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03000168
	.4byte 0x03001388
	.4byte 0x080f11bd
	.4byte 0x080f1770
	.4byte 0x00000101
	.4byte 0x06010000
	.4byte 0x06010004
	.4byte 0x06010008
	.4byte 0x0601000c
	.4byte 0x06010010
	.4byte 0x06010014
	.4byte 0x06010018
	.4byte 0x0601001c
