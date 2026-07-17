@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08094544
	.thumb_func
Func_08094544:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #444]
	ldr	r7, [r3, #0]
	ldr	r3, [pc, #444]
	movs	r4, #14
	ldrsh	r2, [r3, r4]
	sub	sp, #24
	movs	r1, #12
	ldrsh	r0, [r3, r1]
	str	r2, [sp, #20]
	movs	r2, #10
	ldrsh	r1, [r3, r2]
	mov	ip, r0
	movs	r5, #8
	ldrsh	r0, [r3, r5]
	str	r1, [sp, #16]
	movs	r2, #6
	ldrsh	r5, [r3, r2]
	movs	r4, #4
	ldrsh	r1, [r3, r4]
	movs	r4, #240
	str	r5, [sp, #12]
	lsls	r4, r4, #4
	adds	r3, r7, r4
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #4
	subs	r3, r3, r2
	movs	r5, #241
	lsls	r3, r3, #7
	lsls	r5, r5, #4
	adds	r4, r7, r3
	adds	r3, r7, r5
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #384]
	mov	lr, r3
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r3, [sp, #20]
	lsls	r3, r3, #16
	str	r3, [sp, #8]
	subs	r5, #8
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	adds	r3, r7, r5
	ldr	r3, [r3, #0]
	adds	r6, r3, #0
	muls	r6, r2
	mov	r2, lr
	cmp	r2, #0
	bne.n	.L0
	movs	r5, #0
	adds	r3, r4, #0
.L1:
	mov	r4, ip
	adds	r5, #1
	strh	r4, [r3, #0]
	strh	r0, [r3, #4]
	strh	r1, [r3, #8]
	adds	r3, #12
	cmp	r5, #160
	bne.n	.L1
	b.n	.L2
.L0:
	ldr	r5, [pc, #332]
	adds	r3, r7, r5
	ldr	r3, [r3, #0]
	mov	r2, ip
	mov	r8, r3
	lsls	r3, r2, #16
	lsls	r1, r1, #16
	lsls	r2, r0, #16
	lsrs	r3, r3, #16
	ldr	r0, [pc, #316]
	lsrs	r2, r2, #16
	lsrs	r1, r1, #16
	str	r3, [sp, #4]
	movs	r5, #0
	mov	sl, r0
	mov	fp, r2
	mov	r9, r1
.L4:
	movs	r2, #255
	asrs	r3, r6, #16
	ands	r3, r2
	ldr	r1, [pc, #300]
	lsls	r3, r3, #1
	ldrsh	r0, [r1, r3]
	mov	r1, r8
	movs	r0, r0
	mov	ip, pc
	bx	sl
	cmp	r0, #0
	bge.n	.L3
	adds	r0, #255
.L3:
	lsls	r3, r0, #8
	ldr	r0, [sp, #4]
	lsrs	r3, r3, #16
	adds	r2, r0, r3
	mov	r1, fp
	strh	r2, [r4, #0]
	adds	r4, #4
	adds	r2, r1, r3
	strh	r2, [r4, #0]
	add	r3, r9
	adds	r4, #4
	adds	r5, #1
	strh	r3, [r4, #0]
	add	r6, lr
	adds	r4, #4
	cmp	r5, #160
	bne.n	.L4
.L2:
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #4
	subs	r3, r3, r2
	ldr	r5, [pc, #232]
	lsls	r3, r3, #7
	adds	r3, r7, r3
	adds	r4, r3, #2
	adds	r3, r7, r5
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #204]
	mov	lr, r3
	ldr	r1, [sp, #8]
	adds	r3, r7, r0
	ldrh	r2, [r3, #0]
	subs	r5, #8
	lsrs	r3, r1, #16
	adds	r2, r2, r3
	adds	r3, r7, r5
	ldr	r3, [r3, #0]
	mov	r0, lr
	adds	r6, r3, #0
	muls	r6, r2
	cmp	r0, #0
	bne.n	.L5
	movs	r5, #0
	adds	r3, r4, #0
.L6:
	add	r1, sp, #20
	add	r2, sp, #16
	add	r4, sp, #12
	ldrh	r1, [r1, #0]
	ldrh	r2, [r2, #0]
	ldrh	r4, [r4, #0]
	adds	r5, #1
	strh	r1, [r3, #0]
	strh	r2, [r3, #4]
	strh	r4, [r3, #8]
	adds	r3, #12
	cmp	r5, #160
	bne.n	.L6
	b.n	.L7
.L5:
	ldr	r5, [pc, #164]
	ldr	r1, [sp, #12]
	adds	r3, r7, r5
	ldr	r3, [r3, #0]
	ldr	r0, [sp, #16]
	lsls	r2, r1, #16
	ldr	r1, [sp, #8]
	mov	r8, r3
	lsrs	r1, r1, #16
	lsls	r3, r0, #16
	ldr	r0, [pc, #132]
	lsrs	r3, r3, #16
	lsrs	r2, r2, #16
	str	r1, [sp, #0]
	movs	r5, #0
	mov	sl, r0
	mov	fp, r3
	mov	r9, r2
.L9:
	asrs	r3, r6, #16
	movs	r2, #255
	ands	r3, r2
	ldr	r2, [pc, #112]
	lsls	r3, r3, #1
	ldrsh	r0, [r2, r3]
	mov	r1, r8
	mov	ip, pc
	bx	sl
	cmp	r0, #0
	bge.n	.L8
	adds	r0, #255
.L8:
	lsls	r3, r0, #8
	ldr	r0, [sp, #0]
	lsrs	r3, r3, #16
	adds	r2, r0, r3
	mov	r1, fp
	strh	r2, [r4, #0]
	adds	r4, #4
	adds	r2, r1, r3
	strh	r2, [r4, #0]
	add	r3, r9
	adds	r4, #4
	adds	r5, #1
	strh	r3, [r4, #0]
	add	r6, lr
	adds	r4, #4
	cmp	r5, #160
	bne.n	.L9
.L7:
	ldr	r3, [pc, #48]
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	movs	r4, #240
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r4, r4, #4
	adds	r1, r7, r4
	ldrb	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ed8
	.4byte 0x03001ad0
	.4byte 0x00000f02
	.4byte 0x00000f18
	.4byte 0x03000118
	.4byte 0x0809ed84
	.4byte 0x00000f14
	.4byte 0x00000f1c
