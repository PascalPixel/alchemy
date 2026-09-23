@ Output stage of TLA's resident mixer, behind a 4-byte Thumb entry
@ (bx pc). It keeps its state in the words at 0x03000d1c
@ (IWRAM_RUNTIME_MIX_RENDER_STATE.S; str r1, [pc, #-64]), selects one of
@ the two bgt templates after its return and stores it over the loop branch
@ at 0x03000e64 (ldrne/ldreq r1, [pc, #296]; str r1, [pc, #252]), and
@ normalises overflowing samples in a subroutine that branches on overflow
@ (adds r7, r7, r7; bvc) and returns with bxvc lr. Rewriting its own loop
@ branch is beyond both approved compilers. The loop branch as stored here
@ never runs, since the renderer writes a template over it on entry, and
@ carries no credit, as do the Thumb entry, which is TLA's own, and the
@ word after the final branch into which the renderer stores its mask
@ (str lr, [pc]). The word after that, 0x03006fc0, is the literal it loads
@ (ldr r0, [pc, #380]).
@ credit: handwritten — TLA IWRAM runtime frame renderer
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x080012f8
	.set AlchemyRunAddress, 0x03000d40
AlchemyUncredited_080012f8:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080012f8:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	r1, [pc, #-52]
	tst	r1, #12
	lsl	r1, r1, #4
	str	r1, [pc, #-64]
	ldrne	r1, [pc, #296]
	ldreq	r1, [pc, #296]
	str	r1, [pc, #252]
	ldr	r0, [pc, #380]
	ldr	r1, [pc, #-80]
	ldr	r9, [pc, #-68]
	ldr	r5, [pc, #-84]
	mov	r8, #0xff000000
	orr	r8, r8, r8, lsr #16
	and	lr, r8, r8, lsl #7
	mov	r3, #0xc60
	ldr	sl, [pc, #-96]
	str	r5, [pc, #-100]
	rsb	r2, r9, r1
	movs	r9, r9
	moveq	r2, #0
	moveq	r9, r1
	add	sl, sl, r2, lsl #1
	str	r2, [pc, #-116]
	ldr	r1, [pc, #-116]
	ldr	r6, [pc, #-116]
	ldm	r0, {r2, r4}
.L_03000db0:
	add	r2, r2, lr, lsr #9
	add	r4, r4, lr, lsr #9
	eor	ip, r2, r2, lsl #1
	ands	ip, ip, lr
	eoreq	ip, r4, r4, lsl #1
	andseq	ip, ip, lr
	bne	.L_03000ec0
.L_03000dcc:
	and	ip, r2, lr
	add	ip, r2, ip, lsr #9
	sub	r2, ip, lr, lsr #9
	and	ip, r8, ip, ror #15
	and	r7, r8, r2, ror #15
	orr	ip, r7, ip, lsr #8
	and	fp, r4, lr
	add	fp, r4, fp, lsr #9
	sub	r4, fp, lr, lsr #9
	and	fp, r8, fp, ror #15
	and	r7, r8, r4, ror #15
	orr	r7, r7, fp, lsr #8
	ldrsb	r2, [r5]
	mul	r2, r1, r2
	ldrsb	r4, [r5, #2]
	mul	r4, r1, r4
	ldrsb	fp, [r5, r3]!
	mla	r2, r6, fp, r2
	ldrsb	fp, [r5, #2]!
	mla	r4, r6, fp, r4
	strh	r7, [r5], #-2
	strh	ip, [r5], -r3
	bic	r7, r7, #255
	bic	r7, r7, #0xff00
	orr	r7, r7, ip, lsr #16
	str	r7, [r5], #4
	and	r7, r7, r8
	add	r7, r7, r7, lsr #7
	add	r4, r4, r7, asr #19
	rors	r7, r7, #16
	add	r2, r2, r7, asr #19
	ldrsb	fp, [sl], #2
	add	r2, r2, fp, lsl #21
	ldrsb	fp, [sl], #2
	add	r4, r4, fp, lsl #21
	stmia	r0!, {r2, r4}
	subs	r9, r9, #2
.L_03000e60:
	ldmgt	r0, {r2, r4}
AlchemyUncredited_0800141c:
	bgt	.L_03000db0
AlchemyUncreditedEnd_0800141c:
	ldr	r2, [pc, #-316]
	cmp	r2, #0
	strgt	r9, [pc, #-324]
	ldrgt	sl, [pc, #-336]
	movgt	r9, r2
	bgt	.L_03000e60
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
	.inst	0xcaffffd3	@ bgt template, stored over 0x03000e64
	.inst	0xcaffffd1	@ bgt template, stored over 0x03000e64
.L_03000e90:
	ror	r7, ip, #16
	adds	r7, r7, r7
	bvc	.L_03000ea8
	orr	ip, ip, #0x4000
	bic	ip, ip, #0x3fc0
	submi	ip, ip, #64
.L_03000ea8:
	adds	r7, ip, ip
	bxvc	lr
	orr	ip, ip, #0x40000000
	bic	ip, ip, #0x3fc00000
	submi	ip, ip, #0x400000
	bx	lr
.L_03000ec0:
	mov	ip, r2
	bl	.L_03000e90
	mov	r2, ip
	mov	ip, r4
	bl	.L_03000e90
	mov	r4, ip
	and	lr, r8, r8, lsl #7
	str	lr, [pc]
	b	.L_03000dcc
AlchemyUncredited_0800149c:
	.4byte	0
AlchemyUncreditedEnd_0800149c:
	.4byte	0x03006fc0
