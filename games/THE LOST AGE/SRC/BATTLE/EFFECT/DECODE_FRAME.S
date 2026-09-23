@ The battle-effect bank's own link of the frame decoder. Its loader (the
@ function at 0x080e43a4) allocates a 0x2c8-byte heap block and has DMA3 copy
@ the 488 bytes from 0x080c93cc to 0x080c95b4 into it (pool {0x2c8,
@ 0x080c95b4, 0x080c93cc} at 0x080e4458); the remaining 224 bytes of the block
@ are a table the decoder fills at run time. Behind a 4-byte Thumb entry
@ (bx pc) the ARM decoder enters its unrolled copy loop with
@ add pc, pc, r2, lsl #2, dispatches through halfword code offsets
@ (ldrsh r4, [pc, r4]; add pc, pc, r4) and keeps its mode word with
@ str r2, [pc, #124]; GCC emits neither jumps into unrolled code nor
@ PC-relative stores. After that entry these 472 bytes equal TBS
@ GRAPHICS/RENDER/DECODE.S (0x08009bb8) apart from one IWRAM address in the
@ literal pool (0x03001174 for 0x03001c90). The Thumb entry, which is TLA's
@ own, and the nop that add pc, pc, r2, lsl #2 always skips carry no credit.
@ The data after its final return (the mode word, two table addresses and
@ the 224-byte table, 0x080c95a8..0x080c9694) is DECODE_FRAME_DATA.S,
@ uncredited as TBS GRAPHICS/RENDER/DECODE_DATA.S.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank, text decoder (instance 2)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x080c93cc
	.global BattleFx_DecodeFrame
	.thumb_func
BattleFx_DecodeFrame:
AlchemyUncredited_080c93cc:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080c93cc:
	cmp	r2, #0
	bne	.L_080c94e8
	mov	r4, #255
	ands	r2, r0, #3
	ldm	r0!, {r3}
	add	r2, r2, r2, lsl #2
	add	pc, pc, r2, lsl #2
AlchemyUncredited_080c93ec:
	nop
.L_080c93f0:
AlchemyUncreditedEnd_080c93ec:
	ands	r2, r4, r3
	bxeq	lr
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #8
	bxeq	lr
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #16
	bxeq	lr
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #24
	bxeq	lr
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ldm	r0!, {r3}
	and	r2, r3, r3, lsr #16
	ands	r2, r2, r2, lsr #8
	beq	.L_080c93f0
.L_080c9450:
	ands	r2, r4, r3
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #8
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #16
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ands	r2, r4, r3, lsr #24
	subs	ip, r2, #223
	addhi	r1, r1, ip
	strbls	r2, [r1], #1
	ldm	r0!, {r3}
	and	r2, r3, r3, lsr #16
	ands	r2, r2, r2, lsr #8
	bne	.L_080c9450
	b	.L_080c93f0
	.4byte	0x03001174
	ldr	r2, [pc, #-12]
	ldrb	r2, [r2]
	b	.L_080c94d8
	mov	r2, #0
	b	.L_080c94d8
	mov	r2, #1
	b	.L_080c94d8
	mov	r2, #15
	b	.L_080c94d8
.L_080c94cc:
	subs	r3, r3, #223
	addhi	r1, r1, r3
	strbls	r2, [r1], #1
.L_080c94d8:
	ldrb	r3, [r0], #1
	cmp	r3, #0
	bne	.L_080c94cc
	bx	lr
.L_080c94e8:
	add	r4, r2, r2
	ldrsh	r4, [pc, r4]
	add	pc, pc, r4
	.2byte	0xfee0, 0x0020, 0x0020, 0x0020
	.2byte	0x0020, 0x0020, 0x0020, 0xffcc
	.2byte	0xffcc, 0x001c, 0x001c, 0x001c
	.2byte	0x001c, 0xffb0, 0xffc4, 0xffbc
	sub	r2, r2, #2
	ldr	r4, [pc, #136]
	cmp	r4, r2
	beq	.L_080c957c
	str	r2, [pc, #124]
	ldr	r3, [pc, #128]
	add	r2, r3, r2, lsl #8
	add	r3, pc, #124
	push	{r5, r6, r7, r8, r9}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	ldm	r2!, {r4, r5, r6, r7, r8, r9, ip}
	stmia	r3!, {r4, r5, r6, r7, r8, r9, ip}
	pop	{r5, r6, r7, r8, r9}
.L_080c957c:
	add	r2, pc, #48
	b	.L_080c9588
.L_080c9584:
	add	r1, r1, r4
.L_080c9588:
	ldrb	r3, [r0], #1
	subs	r4, r3, #223
	bhi	.L_080c9584
	cmp	r3, #0
	ldrbne	r3, [r2, r3]
	strbne	r3, [r1], #1
	bne	.L_080c9588
	bx	lr
