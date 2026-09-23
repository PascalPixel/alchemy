@ Frame decoder. Each of its three loaders (the functions at 0x08021a84,
@ 0x0802386c and 0x08023e18) allocates a 0x2c8-byte heap block and has DMA3
@ copy the 488 bytes from 0x08020da8 to 0x08020f90 into it (pools {0x2c8,
@ 0x08020f90, 0x08020da8} at 0x08021e00, 0x08023bf4 and 0x08023ee4); the
@ remaining 224 bytes of the block are a table the decoder fills at run time.
@ Behind a 4-byte Thumb entry (bx pc) the ARM decoder enters its unrolled
@ copy loop with add pc, pc, r2, lsl #2, dispatches through halfword code
@ offsets (ldrsh r4, [pc, r4]; add pc, pc, r4) and keeps its mode word with
@ str r2, [pc, #124]; GCC emits neither jumps into unrolled code nor
@ PC-relative stores. After that entry these 472 bytes equal TBS
@ GRAPHICS/RENDER/DECODE.S (0x08009bb8) apart from one IWRAM address in the
@ literal pool (0x03001174 for 0x03001c90). The Thumb entry, which is TLA's
@ own, and the nop that add pc, pc, r2, lsl #2 always skips carry no credit.
@ The data after its final return (the mode word, two table addresses and
@ the 224-byte table, 0x08020f84..0x08021070) is DECODE_FRAME_DATA.S,
@ uncredited as TBS GRAPHICS/RENDER/DECODE_DATA.S.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank, text decoder (instance 1)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08020da8
	.global Render_DecodeFrame
	.thumb_func
Render_DecodeFrame:
AlchemyUncredited_08020da8:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08020da8:
	cmp	r2, #0
	bne	.L_08020ec4
	mov	r4, #255
	ands	r2, r0, #3
	ldm	r0!, {r3}
	add	r2, r2, r2, lsl #2
	add	pc, pc, r2, lsl #2
AlchemyUncredited_08020dc8:
	nop
.L_08020dcc:
AlchemyUncreditedEnd_08020dc8:
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
	beq	.L_08020dcc
.L_08020e2c:
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
	bne	.L_08020e2c
	b	.L_08020dcc
	.4byte	0x03001174
	ldr	r2, [pc, #-12]
	ldrb	r2, [r2]
	b	.L_08020eb4
	mov	r2, #0
	b	.L_08020eb4
	mov	r2, #1
	b	.L_08020eb4
	mov	r2, #15
	b	.L_08020eb4
.L_08020ea8:
	subs	r3, r3, #223
	addhi	r1, r1, r3
	strbls	r2, [r1], #1
.L_08020eb4:
	ldrb	r3, [r0], #1
	cmp	r3, #0
	bne	.L_08020ea8
	bx	lr
.L_08020ec4:
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
	beq	.L_08020f58
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
.L_08020f58:
	add	r2, pc, #48
	b	.L_08020f64
.L_08020f60:
	add	r1, r1, r4
.L_08020f64:
	ldrb	r3, [r0], #1
	subs	r4, r3, #223
	bhi	.L_08020f60
	cmp	r3, #0
	ldrbne	r3, [r2, r3]
	strbne	r3, [r1], #1
	bne	.L_08020f64
	bx	lr
