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
