@ Why assembly: WorldMap_BuildScanlineTableArm is a Thumb entry whose bx pc switches to the ARM
@ code of WorldMap_BuildScanlineTableInArm that follows, a mode switch inside a function; its
@ halfword after bx pc is never run. WorldMap_BuildScanlineTableInArm, an ARM copy of the
@ perspective map's scanline table builder (WorldMap_BuildScanlineTable is the Thumb one),
@ calls IwramSqrt and IwramTransformVector by bl through two 8-byte far-call
@ veneers placed after its literal pool, and two of its smull take free
@ registers for their high halves (r2/r1 and r2/r0) and shift them back, which
@ GCC's register-pair multiply cannot emit.
.syntax unified
	.thumb
	.global WorldMap_BuildScanlineTableArm
	.thumb_func
WorldMap_BuildScanlineTableArm:
	bx	pc
AlchemyUncredited_080212da:
	.2byte	0x0200
AlchemyUncreditedEnd_080212da:
	.arm
	.global WorldMap_BuildScanlineTableInArm
	.type WorldMap_BuildScanlineTableInArm, %function
WorldMap_BuildScanlineTableInArm:
	push	{r5, r6, r7, r8, r9, sl, lr}
	mov	r5, #0
	sub	sp, sp, #76
	str	r5, [sp, #48]
	ldr	r3, [r1]
	str	r0, [sp, #8]
	mov	r7, r2
	str	r3, [sp, #44]
	add	r0, sp, #44
	ldr	r3, [r1, #8]
	add	r1, sp, #12
	str	r3, [sp, #52]
	bl	.L_08021438
	ldr	r3, [sp, #20]
	ldr	r1, [sp, #8]
	smull	r2, r3, r1, r3
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	ldr	r0, [sp, #16]
	mov	r8, r5
	ldr	r2, [pc, #240]
	rsb	r0, r3, r0
	str	r0, [sp, #4]
	mov	r9, #32768
	ldr	r1, [r2]
	mov	sl, r8
	rsb	r1, r1, #0
	str	r1, [sp]
.L_0802134c:
	ldr	r2, [pc, #212]
	ldr	r0, [sp]
	ldr	r1, [r2]
	ldr	r3, [pc, #204]
	rsb	r1, r8, r1
	lsl	r1, r1, #16
	mov	lr, pc
	bx	r3
	ldr	r1, [sp, #8]
	mov	r5, r0
	ldr	r2, [pc, #176]
	cmp	r5, r1
	subne	r0, r5, r1
	moveq	r0, #1
	ldr	r1, [sp, #4]
	mov	lr, pc
	bx	r2
	subs	r6, r0, #0
	bge	.L_08021440
	rsb	r1, r6, #0
	smull	r3, r1, r9, r1
	lsl	r1, r1, #16
	orr	r1, r1, r3, lsr #16
	ldr	r3, [pc, #116]
	ldr	r2, [pc, #120]
	ldr	r0, [r3]
	mov	lr, pc
	bx	r2
	str	r0, [r7]
	smull	r2, r1, r5, r6
	lsl	r5, r1, #16
	orr	r5, r5, r2, lsr #16
	add	r2, sp, #16
	ldm	r2, {r2, r3}
	rsb	r3, r6, r3
	asr	r3, r3, #4
	rsb	r5, r2, r5
	asr	r5, r5, #4
	smull	r2, r0, r3, r3
	lsl	r3, r0, #16
	orr	r3, r3, r2, lsr #16
	smull	r1, r0, r5, r5
	lsl	r0, r0, #16
	orr	r0, r0, r1, lsr #16
	add	r0, r3, r0
	bl	.L_08021430
	lsl	r0, r0, #12
	cmp	r5, #0
	rsblt	r0, r0, #0
	smull	r3, r0, r9, r0
	lsl	r0, r0, #16
	orr	r0, r0, r3, lsr #16
	str	r0, [r7, #4]
	b	.L_08021448
	.4byte gCameraSceneParameters
	.4byte Data_030011f0
	.4byte IwramRatioMulQ14
.L_08021430:
	ldr	pc, [pc, #-4]
	.4byte IwramSqrt
.L_08021438:
	ldr	pc, [pc, #-4]
	@ The Japanese edition's second veneer reaches IwramSqrt, as its first
	@ does, so the transform's call takes a square root there; the later
	@ editions reach IwramTransformVector.
	.ifdef TLA_EDITION_JA
	.4byte IwramSqrt
	.else
	.4byte IwramTransformVector
	.endif
.L_08021440:
	str	sl, [r7]
	str	sl, [r7, #4]
.L_08021448:
	str	sl, [r7, #8]
	add	r8, r8, #1
	str	sl, [r7, #12]
	add	r7, r7, #20
	cmp	r8, #159
	ble	.L_0802134c
	add	sp, sp, #76
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
