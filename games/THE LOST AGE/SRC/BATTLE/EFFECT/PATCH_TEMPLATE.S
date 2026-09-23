@ ARM patch template that the battle-effect generator copies and patches
@ before it runs (descriptor {0x081972a0, 0x08197224, 0x081973f0} at
@ 0x0819674c). It holds placeholder instructions (and r2, r2, #0; a duplicated
@ mov r6, #0), branches over single instructions that are overwritten, and a
@ pop that a bne skips; it can only run once copied and patched. It is TLA's
@ revision of the Gousei template of TBS BATTLE/EFFECT/SENTOU_KOUKA_GOUSEI.S
@ (0x080edcc4): after its 4-byte Thumb entry, runs of 244, 28, 36 and 24
@ bytes match that template.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: handwritten — relocated ARM kernel, battle-effect patch template
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081972a0
	.global BattleFx_PatchTemplate
	.thumb_func
BattleFx_PatchTemplate:
AlchemyUncredited_081972a0:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_081972a0:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	add	r4, sp, #32
	ldm	r4, {r4, r5}
	add	r6, r1, r4
	sub	r1, r6, #1
	lsl	r6, r4, #1
	sub	r6, r5, #1
	mla	r1, r6, r4, r1
	rsb	r6, r4, #0
	lsl	r6, r6, #1
	mla	r1, r4, r5, r1
	sub	r1, r1, #1
	mov	r6, #0
	mov	r6, #0
	rsbs	ip, r3, #0
	addpl	r5, r5, r3
	mlapl	r1, r3, r4, r1
	mlapl	r1, ip, r4, r1
	movpl	r3, #0
	add	ip, r3, r5
	cmp	ip, #0
	rsbgt	r5, r3, #0
	cmp	r5, #0
	ble	.L_08197308
	and	r2, r2, #0
.L_08197308:
	rsbs	ip, r2, #0
	addpl	r4, r4, r2
	addpl	r6, r6, r2
	addpl	r1, r1, r2
	subpl	r6, r6, r2
	subpl	r1, r1, r2
	movpl	r2, #0
	add	ip, r2, r4
	subs	ip, ip, #0
	subgt	r6, r6, ip
	addgt	r6, r6, ip
	rsbgt	r4, r2, #0
	cmp	r4, #0
	ble	.L_08197344
	ands	r7, r2, #7
.L_08197344:
	beq	.L_08197358
	rsb	r7, r7, #8
	subs	r4, r4, r7
	addlt	r7, r4, r7
	movlt	r4, #0
.L_08197358:
	lsr	ip, r2, #3
	rsb	fp, ip, #16
	and	r2, r2, #7
	add	r2, r2, ip, lsl #6
	add	r0, r0, r2
	lsl	r3, r3, #3
	add	r2, r3, r5, lsl #3
	and	ip, r3, #56
	and	lr, r3, #0, 26
	orr	ip, ip, lr
	add	r9, r0, ip
	mov	lr, fp
	cmp	r7, #0
	beq	.L_08197394
	sub	ip, r1, r7
.L_08197394:
	add	ip, r1, r7
	cmp	r1, ip
	bne	.L_081973a4
	add	r9, r9, #56
.L_081973a4:
	subs	lr, lr, #1
	subeq	r9, r9, #0x400
	lsrs	ip, r4, #3
	beq	.L_081973b8
	add	r9, r9, #56
.L_081973b8:
	subs	lr, lr, #1
	subeq	r9, r9, #0x400
	subs	ip, ip, #1
	bne	.L_081973cc
	ands	ip, r4, #7
.L_081973cc:
	beq	.L_081973d4
	sub	ip, r1, ip
.L_081973d4:
	add	ip, r1, ip
	add	r1, r1, r6
	add	r3, r3, #8
	cmp	r3, r2
	bne	.L_081973ec
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
.L_081973ec:
	bx	lr
