@ Writes a zero-terminated byte stream in which values above 0xdf skip
@ destination bytes. With r2 = 0 it copies the bytes, entering its unrolled
@ loop at the source's word alignment with add pc, pc, r2, lsl #2; other r2
@ values pick, through a table of halfword code offsets
@ (ldrsh r4, [pc, r4]; add pc, pc, r4), a constant fill value or a 256-byte
@ translation table that it first copies into the buffer of DECODE_DATA.S
@ after its code, remembering the choice there with str r2, [pc, #124].
@ Writing into those words only works in a RAM copy; GCC emits neither jumps
@ into unrolled code nor PC-relative stores. Three routines allocate heap
@ slot 52 with 0x2c4 bytes and copy from 0x08009bb8 into it with DMA3: the
@ copies whose source is loaded at 0x0800c670 and 0x0800c8ae move all 708
@ bytes (these 472 and DECODE_DATA.S), the one loaded at 0x0800aa48 only the
@ 484 bytes up to 0x08009d9c, leaving the buffer to be filled at run time.
@ Every TBS and TLA edition has these 472 bytes, differing only in the
@ relocated IWRAM literal (TLA GRAPHICS/DECODE_FRAME.S and
@ BATTLE/EFFECT/DECODE_FRAME.S).
@ The nop after its jump (0x08009bd4), which the jump always skips, carries
@ no credit.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank
.syntax unified
	.arm
	.global Func_08009bb8
Func_08009bb8:
	cmp	r2, #0
	bne	.L_08009cd0
	mov	r4, #255
	ands	r2, r0, #3
	ldm	r0!, {r3}
	add	r2, r2, r2, lsl #2
	add	pc, pc, r2, lsl #2
AlchemyUncredited_08009bd4:
	nop
.L_08009bd8:
AlchemyUncreditedEnd_08009bd4:
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
	beq	.L_08009bd8
.L_08009c38:
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
	bne	.L_08009c38
	b	.L_08009bd8
	.4byte 0x03001c90
	ldr	r2, [pc, #-12]
	ldrb	r2, [r2]
	b	.L_08009cc0
	mov	r2, #0
	b	.L_08009cc0
	mov	r2, #1
	b	.L_08009cc0
	mov	r2, #15
	b	.L_08009cc0
.L_08009cb4:
	subs	r3, r3, #223
	addhi	r1, r1, r3
	strbls	r2, [r1], #1
.L_08009cc0:
	ldrb	r3, [r0], #1
	cmp	r3, #0
	bne	.L_08009cb4
	bx	lr
.L_08009cd0:
	add	r4, r2, r2
	ldrsh	r4, [pc, r4]
	add	pc, pc, r4
	.2byte 0xfee0, 0x0020, 0x0020, 0x0020
	.2byte 0x0020, 0x0020, 0x0020, 0xffcc
	.2byte 0xffcc, 0x001c, 0x001c, 0x001c
	.2byte 0x001c, 0xffb0, 0xffc4, 0xffbc
	sub	r2, r2, #2
	ldr	r4, [pc, #136]
	cmp	r4, r2
	beq	.L_08009d64
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
.L_08009d64:
	add	r2, pc, #48
	b	.L_08009d70
.L_08009d6c:
	add	r1, r1, r4
.L_08009d70:
	ldrb	r3, [r0], #1
	subs	r4, r3, #223
	bhi	.L_08009d6c
	cmp	r3, #0
	ldrbne	r3, [r2, r3]
	strbne	r3, [r1], #1
	bne	.L_08009d70
	bx	lr
