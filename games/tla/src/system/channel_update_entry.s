@ Why assembly: DMA timer branches on carry from lsls: each bcc consumes the
@ carry of lsls r1, r1, #7, a DMA channel's repeat bit; bgt branches on the
@ flags of subs r1, #1 with no compare.
@ Why assembly: it reads its three words with one pc-relative block load
@ (add r1, pc, #20; ldmia r1!, {r0, r2, r3}).
@ The sound driver's VBlank entry: checks the stack is in IWRAM and the
@ sound area is unlocked, saves the high registers, runs the two VBlank
@ callbacks and enters the IWRAM mixer with its buffer offset for this
@ VBlank. It calls its callbacks through lr with a lone BL suffix.
	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global SoundDriverVSyncRefresh
	.thumb_func
SoundDriverVSyncRefresh:
	ldr	r0, [pc, #100]
	ldr	r0, [r0, #0]
	ldr	r2, [pc, #92]
	ldr	r3, [r0, #0]
	subs	r3, r3, r2
	cmp	r3, #1
	bhi.n	.L_08000626
	ldrb	r1, [r0, #4]
	subs	r1, #1
	strb	r1, [r0, #4]
	bgt.n	.L_08000626
	ldrb	r1, [r0, #11]
	strb	r1, [r0, #4]
	ldr	r2, [pc, #36]
	ldr	r1, [r2, #8]
	ldr	r0, [pc, #36]
	lsls	r1, r1, #7
	bcc.n	.L_0800060e
	str	r0, [r2, #8]
.L_0800060e:
	ldr	r1, [r2, #20]
	lsls	r1, r1, #7
	bcc.n	.L_08000616
	str	r0, [r2, #20]
.L_08000616:
	movs	r1, #4
	lsls	r1, r1, #8
	strh	r1, [r2, #10]
	strh	r1, [r2, #22]
	movs	r1, #182
	lsls	r1, r1, #8
	strh	r1, [r2, #10]
	strh	r1, [r2, #22]
.L_08000626:
	bx	lr
	.4byte	0x040000bc
	.4byte	0x84400004

	.section .text,"ax",%progbits
	.balign 2

.syntax unified
	.thumb
	.global SoundDriverEnterVBlankUpdate
	.thumb_func
SoundDriverEnterVBlankUpdate:
	add	r1, pc, #20
	ldmia	r1!, {r0, r2, r3}
	mov	r1, sp
	cmp	r1, r0
	bcc.n	.L_08000642
	ldr	r0, [r3, #0]
	ldr	r3, [r0, #0]
	cmp	r2, r3
	beq.n	.L_08000654
.L_08000642:
	movs	r0, #1
	bx	lr
@ Never run: a zero halfword after bx lr that word-aligns the loaded words.
AlchemyUncredited08000646:
	.2byte	0x0000
AlchemyUncreditedEnd08000646:
	.4byte	0x03007900
	.4byte	0x68736d53
	.4byte	0x03007ff0
.L_08000654:
	push	{r5, r6, r7, lr}
	mov	r1, r8
	mov	r2, r9
	mov	r3, sl
	mov	r4, fp
	push	{r0, r1, r2, r3, r4}
	sub	sp, #24
	ldr	r3, [r0, #32]
	cmp	r3, #0
	beq.n	.L_08000670
	ldr	r0, [r0, #36]
	mov	lr, r3
	.inst.n	0xf800
	ldr	r0, [sp, #24]
.L_08000670:
	ldr	r3, [r0, #40]
	mov	lr, r3
	.inst.n	0xf800
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #16]
	str	r3, [sp, #0]
	ldr	r5, [pc, #40]
	ldr	r4, [pc, #52]
	ldrb	r4, [r4, #1]
	subs	r7, r4, #1
	bls.n	.L_08000690
	ldrb	r1, [r0, #11]
	subs	r1, r1, r7
	lsls	r2, r3, #1
	muls	r2, r1
	adds	r5, r5, r2
.L_08000690:
	str	r5, [sp, #8]
	ldr	r4, [sp, #24]
	ldr	r0, [r4, #20]
	mov	r9, r0
	ldr	r0, [r4, #24]
	mov	ip, r0
	ldrb	r0, [r4, #6]
	adds	r4, #80
	ldr	r3, [pc, #0]
	bx	r3
	.4byte	IwramSoundMixEntry + 1
	.4byte	MixerPcmBuffer
@ Unread here: 0x350 and REG_VCOUNT's address, as SoundMain's pool loads them.
AlchemyUncredited080006ac:
	.4byte	0x00000350
	.4byte	0x04000006
AlchemyUncreditedEnd080006ac:
	.4byte	SoundVBlankStateWork
