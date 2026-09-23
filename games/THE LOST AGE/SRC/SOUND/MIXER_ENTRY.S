@ Guarded ROM-to-IWRAM entry of the sound mixer: after its driver ID check it
@ saves r0 and r8-r11 through r1-r4 (push {r0-r4}) in a private frame and
@ tail-branches into the IWRAM mixer that restores it; GCC 2.96 saves high
@ registers only in its prologue, through low registers it has already
@ pushed, highest first. These 132 bytes equal TBS SOUND/DRIVER/MIXER_ENTRY.S
@ (0x080f95f0) apart from the IWRAM mixer address in its literal pool
@ (0x03006001 for 0x03007001).
@ The zero halfword of alignment at 0x081c12f6 never runs and carries no
@ credit.
@ credit: library — TBS/TLA common sound driver guarded ROM-to-IWRAM frame entry (mixer entry)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c128c
	.global SoundDriver_EnterMixer
	.thumb_func
SoundDriver_EnterMixer:
	ldr	r0, [pc, #104]
	ldr	r0, [r0, #0]
	ldr	r2, [pc, #104]
	ldr	r3, [r0, #0]
	cmp	r2, r3
	beq.n	.L_081c129a
	bx	lr
.L_081c129a:
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
	beq.n	.L_081c12be
	ldr	r2, [pc, #80]
	ldrb	r2, [r2, #0]
	cmp	r2, #160
	bcs.n	.L_081c12bc
	adds	r2, #228
.L_081c12bc:
	adds	r1, r1, r2
.L_081c12be:
	str	r1, [sp, #20]
	ldr	r3, [r0, #32]
	cmp	r3, #0
	beq.n	.L_081c12ce
	ldr	r0, [r0, #36]
	bl	Func_081c16ae
	ldr	r0, [sp, #24]
.L_081c12ce:
	ldr	r3, [r0, #40]
	bl	Func_081c16ae
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #16]
	mov	r8, r3
	ldr	r5, [pc, #44]
	adds	r5, r5, r0
	ldrb	r4, [r0, #4]
	subs	r7, r4, #1
	bls.n	.L_081c12ee
	ldrb	r1, [r0, #11]
	subs	r1, r1, r7
	mov	r2, r8
	muls	r2, r1
	adds	r5, r5, r2
.L_081c12ee:
	str	r5, [sp, #8]
	ldr	r6, [pc, #24]
	ldr	r3, [pc, #12]
	bx	r3
AlchemyUncredited_081c12f6:
	.2byte	0x0000
AlchemyUncreditedEnd_081c12f6:
	.4byte	0x03007ff0
	.4byte	0x68736d53
	.4byte	0x03006001
	.4byte	0x04000006
	.4byte	0x00000350
	.4byte	0x00000630
