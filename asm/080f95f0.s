@ IWRAMの音響ミキサ入口03007001を呼ぶROM側制御。
@ 転送先との呼出し契約を保ったC一致は未達。build_asm.tsで確認する。
.syntax unified
	.thumb
	.global Func_080f95f0
	.thumb_func
Func_080f95f0:
	ldr	r0, [pc, #104]
	ldr	r0, [r0, #0]
	ldr	r2, [pc, #104]
	ldr	r3, [r0, #0]
	cmp	r2, r3
	beq.n	.L0
	bx	lr
.L0:
	adds	r3, #1
	str	r3, [r0, #0]
	push	{r4, r5, r6, r7, lr}
	mov	r1, r8
	mov	r2, r9
	mov	r3, sl
	mov	r4, fp
	push	{r0, r1, r2, r3, r4}
	sub	sp, #24
	ldrb	r1, [r0, #12]
	cmp	r1, #0
	beq.n	.L1
	ldr	r2, [pc, #80]
	ldrb	r2, [r2, #0]
	cmp	r2, #160
	bcs.n	.L2
	adds	r2, #228
.L2:
	adds	r1, r1, r2
.L1:
	str	r1, [sp, #20]
	ldr	r3, [r0, #32]
	cmp	r3, #0
	beq.n	.L3
	ldr	r0, [r0, #36]
	bl	Func_080f9a12
	ldr	r0, [sp, #24]
.L3:
	ldr	r3, [r0, #40]
	bl	Func_080f9a12
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #16]
	mov	r8, r3
	ldr	r5, [pc, #44]
	adds	r5, r5, r0
	ldrb	r4, [r0, #4]
	subs	r7, r4, #1
	bls.n	.L4
	ldrb	r1, [r0, #11]
	subs	r1, r1, r7
	mov	r2, r8
	muls	r2, r1
	adds	r5, r5, r2
.L4:
	str	r5, [sp, #8]
	ldr	r6, [pc, #24]
	ldr	r3, [pc, #12]
	bx	r3
	movs	r0, r0
	.4byte 0x03007ff0
	.4byte 0x68736d53
	.4byte 0x03007001
	.4byte 0x04000006
	.4byte 0x00000350
	.4byte 0x00000630
