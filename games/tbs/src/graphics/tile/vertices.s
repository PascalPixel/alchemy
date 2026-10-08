@ C2: unpaired multiply halves. Five of its six smull keep both halves
@ outside a register pair: smull r0, r3, r1, r3 (apart, RdHi over Rs),
@ smull r1, r2, sl, r2 (RdHi over Rs), smull r3, r2, r1, r2 (reversed,
@ RdHi over Rs), smull r1, r3, r5, r2 (apart) and smull r3, r1, r4, r9
@ (reversed). agbcc_arm and agscc keep both halves only through mulsidi3
@ (smull %Q0, %R0: RdHi = RdLo + 1, "=&r" apart from the inputs);
@ smulsi3_highpart frees RdLo but discards it. Through the game ARM route
@ (K1) the s64 form pairs every smull (r5, r6; r7, r8; r1, r2; r9, sl;
@ r3, r4): differ 273 of the listing's 157 instructions (0 with the
@ inline asm), 274 with every device removed.
	.syntax unified
	.arm

	.set	POS_X, 0x00
	.set	POS_Z, 0x08

	.set	VTX_X, 0x00
	.set	VTX_Z, 0x04
	.set	VTX_DX, 0x08
	.set	VTX_DZ, 0x0c
	.set	VTX_U, 0x10
	.set	VTX_V, 0x12
	.set	VTX_SIZE, 0x14

	.set	OUT_A, 0x00
	.set	OUT_B, 0x04
	.set	OUT_X, 0x08
	.set	OUT_Y, 0x0c
	.set	OUT_SIZE, 0x10

	.set	FAR_ARCTAN2, 8 * 8 + 4
	.set	FAR_SIN, 11 * 8 + 4
	.set	FAR_COS, 12 * 8 + 4

	.set	PROJ_CENTER_X, 0x0c

	.set	LINES, 160

	.global	Transform_UpdateVertices
	.type	Transform_UpdateVertices, %function
Transform_UpdateVertices:
	push	{r5, r6, r7, r8, r9, sl, lr}
	mov	r5, r1
	ldr	r9, .L_FarCall_RuntimeTable
	sub	sp, sp, #8
	ldr	r1, [r5, #POS_X]
	ldr	ip, [r5, #POS_Z]
	mov	r7, r2
	ldr	r2, [r0, #POS_Z]
	mov	r6, r3
	ldr	r3, [r0, #POS_X]
	rsb	r4, r2, ip
	rsb	r1, r3, r1
	asr	r0, r1, #4
	asr	r1, r4, #4
	ldr	ip, [r9, #FAR_ARCTAN2]
	mov	lr, pc
	bx	ip
	lsl	r0, r0, #16
	lsr	r8, r0, #16				@ 角度
	mov	r1, #0x8000
	ldr	r3, [r5, #POS_X]
	smull	r0, r3, r1, r3
	lsl	r3, r3, #16
	orr	r2, r3, r0, lsr #16
	str	r2, [sp, #4]
	ldr	r2, [r5, #POS_Z]
	smull	r3, r2, r1, r2
	lsl	r2, r2, #16
	orr	r1, r2, r3, lsr #16
	str	r1, [sp]
	ldr	r2, .L_gScanlineAngle
	ldr	r3, [r2]
	cmp	r8, r3
	beq	.L_same

@ 角度が変わった
	mov	r0, r8
	str	r8, [r2]
	ldr	ip, [r9, #FAR_SIN]
	mov	lr, pc
	bx	ip
	mov	sl, r0					@ sin
	mov	r0, r8
	ldr	ip, [r9, #FAR_COS]
	mov	lr, pc
	bx	ip
	mov	r5, r0					@ cos
	rsb	r9, sl, #0
	mov	r8, #LINES - 1
.L_turn:
	ldm	r7, {r2, r4}
	smull	r1, r3, r5, r2
	lsl	r3, r3, #16
	orr	r3, r3, r1, lsr #16
	cmp	r3, #0
	addlt	r3, r3, #255
	asr	r3, r3, #8
	rsb	r3, r3, #0
	lsl	r1, r3, #16
	strh	r3, [r7, #VTX_U]
	asr	lr, r1, #16
	strh	r3, [r6, #OUT_A]
	smull	r1, r2, sl, r2
	lsl	r2, r2, #16
	orr	r2, r2, r1, lsr #16
	cmp	r2, #0
	addlt	r2, r2, #255
	asr	r2, r2, #8
	strh	r2, [r7, #VTX_V]
	strh	r2, [r6, #OUT_B]
	ldr	r3, .L_gProjection_CenterX
	ldr	r0, [r3]
	lsl	r3, lr, #8
	mul	lr, r0, r3
	lsl	r2, r2, #16
	asr	r2, r2, #8
	rsb	r0, r0, #0
	mul	ip, r0, r2
	smull	r3, r1, r4, r9
	lsl	r1, r1, #16
	orr	r1, r1, r3, lsr #16
	add	r1, lr, r1
	rsb	r1, r1, #0
	str	r1, [r7, #VTX_DX]
	smull	r2, r3, r4, r5
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	add	ip, ip, r3
	str	ip, [r7, #VTX_DZ]
	ldr	r2, [sp, #4]
	add	r1, r2, r1
	asr	r1, r1, #8
	str	r1, [r6, #OUT_X]
	ldr	r1, [sp]
	subs	r8, r8, #1
	ldr	r3, [r7, #VTX_DZ]
	add	ip, r6, #OUT_SIZE
	add	r3, r1, r3
	asr	r3, r3, #8
	str	r3, [r6, #OUT_Y]
	add	r7, r7, #VTX_SIZE
	ldm	r6, {r0, r1, r2, r3}
	add	r6, r6, #OUT_SIZE * 2
	stm	ip, {r0, r1, r2, r3}
	bpl	.L_turn
	b	.L_done

@ 角度が同じ
.L_same:
	mov	r8, #LINES - 1
	ldr	lr, .L_gFrameCount
.L_frame:
	ldr	r3, [lr]
	tst	r3, #1
	beq	.L_even
	ldr	r1, [r7, #VTX_DX]			@ 奇数フレーム
	ldr	r2, [sp, #4]
	ldrsh	r3, [r7, #VTX_V]
	add	r1, r2, r1
	cmp	r3, #0
	addlt	r3, r3, #3
	asr	r3, r3, #2
	ldrsh	r2, [r7, #VTX_U]
	add	r3, r3, r1, asr #8
	add	r2, r2, r2, lsr #31
	add	r3, r3, r2, asr #1
	str	r3, [r6, #OUT_X]
	ldrsh	r3, [r7, #VTX_U]
	ldr	r2, [sp]
	cmp	r3, #0
	addlt	r3, r3, #3
	ldr	r1, [r7, #VTX_DZ]
	asr	r3, r3, #2
	add	r1, r2, r1
	ldrsh	r2, [r7, #VTX_V]
	add	r3, r3, r1, asr #8
	add	r2, r2, r2, lsr #31
	add	r3, r3, r2, asr #1
	str	r3, [r6, #OUT_Y]
	b	.L_copy

	.align	2
.L_FarCall_RuntimeTable:
	.4byte	FarCall_RuntimeTable
.L_gScanlineAngle:
	.4byte	gScanlineAngle
.L_gProjection_CenterX:
	.4byte	gProjection + PROJ_CENTER_X
.L_gFrameCount:
	.4byte	gFrameCount

.L_even:
	ldr	r3, [r7, #VTX_DX]			@ 偶数フレーム
	ldr	r1, [sp, #4]
	add	r3, r1, r3
	asr	r3, r3, #8
	str	r3, [r6, #OUT_X]
	ldr	r3, [sp]
	ldr	r2, [r7, #VTX_DZ]
	add	r2, r3, r2
	asr	r2, r2, #8
	str	r2, [r6, #OUT_Y]
.L_copy:
	ldrh	r1, [r7, #VTX_U]
	strh	r1, [r6, #OUT_A]
	add	ip, r6, #OUT_SIZE
	ldrh	r2, [r7, #VTX_V]
	subs	r8, r8, #1
	strh	r2, [r6, #OUT_B]
	add	r7, r7, #VTX_SIZE
	ldm	r6, {r0, r1, r2, r3}
	add	r6, r6, #OUT_SIZE * 2
	stm	ip, {r0, r1, r2, r3}
	bpl	.L_frame
.L_done:
	add	sp, sp, #8
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
	.size	Transform_UpdateVertices, . - Transform_UpdateVertices
