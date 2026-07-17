@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08010230
	.thumb_func
Func_08010230:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #440]
	adds	r7, r0, #0
	ldr	r3, [r3, #0]
	adds	r0, r1, #0
	movs	r1, #130
	sub	sp, #32
	lsls	r1, r1, #1
	str	r3, [sp, #20]
	adds	r3, r3, r1
	mov	lr, r3
	ldr	r3, [pc, #424]
	adds	r0, r0, r3
	ldr	r3, [sp, #20]
	ldr	r2, [pc, #420]
	adds	r3, #236
	ldr	r3, [r3, #0]
	adds	r7, r7, r2
	cmp	r7, r3
	bge.n	.L0
	adds	r7, r3, #0
.L0:
	ldr	r3, [sp, #20]
	adds	r3, #244
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #404]
	adds	r3, r3, r5
	cmp	r7, r3
	ble.n	.L1
	adds	r7, r3, #0
.L1:
	ldr	r3, [sp, #20]
	adds	r3, #240
	ldr	r3, [r3, #0]
	cmp	r0, r3
	bge.n	.L2
	adds	r0, r3, #0
.L2:
	ldr	r3, [sp, #20]
	adds	r3, #248
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #380]
	adds	r3, r3, r1
	cmp	r0, r3
	ble.n	.L3
	adds	r0, r3, #0
.L3:
	ldr	r2, [sp, #20]
	adds	r2, #228
	str	r2, [sp, #16]
	str	r7, [r2, #0]
	ldr	r3, [sp, #20]
	movs	r5, #0
	adds	r3, #232
	str	r3, [sp, #12]
	str	r0, [r3, #0]
	str	r5, [sp, #28]
.L13:
	ldr	r1, [sp, #28]
	movs	r2, #128
	lsls	r2, r2, #1
	ldr	r5, [sp, #20]
	adds	r3, r1, r2
	ldrb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L4
	b.n	.L5
.L4:
	ldr	r1, [sp, #16]
	mov	r2, lr
	ldr	r0, [r1, #0]
	movs	r4, #22
	ldr	r1, [r2, #16]
	ldr	r3, [pc, #328]
	mov	ip, pc
	bx	r3
	ldr	r5, [sp, #12]
	mov	r2, lr
	adds	r7, r0, #0
	ldr	r1, [r2, #20]
	ldr	r0, [r5, #0]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r3, lr
	ldr	r2, [r3, #24]
	cmp	r2, #0
	beq.n	.L6
	ldr	r3, [r3, #32]
	mov	r5, lr
	adds	r3, r3, r2
	adds	r7, r7, r3
	str	r3, [r5, #32]
	ldrh	r3, [r5, #40]
	ldr	r1, [pc, #288]
	lsls	r3, r3, #19
	orrs	r3, r1
	ands	r7, r3
.L6:
	mov	r3, lr
	ldr	r2, [r3, #28]
	cmp	r2, #0
	beq.n	.L7
	ldr	r3, [r3, #36]
	mov	r5, lr
	adds	r3, r3, r2
	adds	r0, r0, r3
	str	r3, [r5, #36]
	ldrh	r3, [r5, #42]
	ldr	r1, [pc, #260]
	lsls	r3, r3, #19
	orrs	r3, r1
	ands	r0, r3
	movs	r4, #32
.L7:
	mov	r2, lr
	ldr	r3, [r2, #8]
	adds	r7, r7, r3
	ldr	r3, [r2, #12]
	adds	r0, r0, r3
	movs	r3, #48
	add	lr, r3
	adds	r1, r7, #0
	cmp	r7, #0
	bge.n	.L8
	ldr	r5, [pc, #232]
	adds	r1, r7, r5
.L8:
	asrs	r7, r1, #19
	adds	r2, r0, #0
	cmp	r0, #0
	bge.n	.L9
	ldr	r3, [pc, #220]
	adds	r2, r0, r3
.L9:
	ldr	r5, [sp, #28]
	lsls	r3, r5, #11
	ldr	r5, [pc, #216]
	asrs	r0, r2, #19
	adds	r5, r3, r5
	lsrs	r3, r2, #31
	adds	r3, r0, r3
	movs	r2, #127
	asrs	r3, r3, #1
	ands	r3, r2
	lsls	r6, r3, #7
	movs	r2, #0
	movs	r3, #30
	lsrs	r4, r4, #1
	ands	r3, r0
	mov	r8, r2
	mov	fp, r4
	str	r5, [sp, #24]
	lsls	r5, r3, #5
	cmp	r8, fp
	bcs.n	.L5
	lsrs	r3, r1, #31
	adds	r3, r7, r3
	asrs	r3, r3, #1
	str	r3, [sp, #8]
	movs	r1, #30
	movs	r3, #127
	mov	r9, r3
	mov	sl, r1
.L11:
	ldr	r4, [sp, #8]
	mov	r2, r9
	adds	r0, r7, #0
	mov	r3, sl
	movs	r1, #0
	ands	r4, r2
	ands	r0, r3
	mov	ip, r1
.L10:
	adds	r3, r6, r4
	ldr	r2, [pc, #148]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #0]
	ldr	r2, [pc, #144]
	lsls	r1, r1, #20
	lsrs	r1, r1, #17
	str	r1, [sp, #4]
	adds	r3, r1, r2
	ldr	r1, [sp, #24]
	adds	r2, r5, r0
	lsls	r2, r2, #1
	adds	r2, r2, r1
	str	r2, [sp, #0]
	ldr	r3, [r3, #0]
	str	r3, [r2, #0]
	ldr	r2, [sp, #4]
	ldr	r1, [pc, #120]
	adds	r3, r2, r1
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #0]
	str	r3, [r2, #64]
	movs	r2, #1
	mov	r3, r9
	adds	r4, #1
	add	ip, r2
	ands	r4, r3
	adds	r0, #2
	mov	r1, sl
	mov	r3, ip
	ands	r0, r1
	cmp	r3, #15
	bls.n	.L10
	movs	r3, #254
	lsls	r3, r3, #6
	adds	r6, #128
	ands	r6, r3
	movs	r3, #240
	adds	r5, #64
	lsls	r3, r3, #2
	add	r8, r2
	ands	r5, r3
	cmp	r8, fp
	bcc.n	.L11
.L5:
	ldr	r5, [sp, #28]
	adds	r5, #1
	str	r5, [sp, #28]
	cmp	r5, #2
	bhi.n	.L12
	b.n	.L13
.L12:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0xffa00000
	.4byte 0xff880000
	.4byte 0xff100000
	.4byte 0xff600000
	.4byte 0x03000118
	.4byte 0x0007ffff
	.4byte 0x06002800
	.4byte 0x02010000
	.4byte 0x02020000
	.4byte 0x02020004
