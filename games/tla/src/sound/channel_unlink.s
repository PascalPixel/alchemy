@ Why SoundChannel_Unlink is assembly:
@ bcc on the flags of subs r1, #20 with no compare.
.syntax unified
	.thumb
	.global SoundChannel_Unlink
	.thumb_func
SoundChannel_Unlink:
	ldr	r1, [r0, #44]
	subs	r1, #20
	bcc.n	.L_081c0758
	adds	r0, #48
	ldmia	r0!, {r2, r3}
	cmp	r3, #0
	beq.n	.L_081c0748
	str	r2, [r3, #48]
.L_081c0748:
	cmp	r2, #0
	bne.n	.L_081c074e
	adds	r2, r1, #0
.L_081c074e:
	str	r3, [r2, #52]
	movs	r1, #0
	subs	r0, #56
	str	r1, [r0, #44]
	bx	lr
.L_081c0758:
	bx	lr
@ ply_note's own copy of clear_modM (clear_modulation.s), its compare placed first.
	.global clear_modM_note
	.thumb_func
clear_modM_note:
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r3, [r1, #24]
	movs	r2, #3
	cmp	r3, #0
	bne.n	.Lclear_mod_note_flags
	movs	r2, #12
.Lclear_mod_note_flags:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr
