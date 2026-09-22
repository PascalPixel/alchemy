.syntax unified
	.thumb
	.global Overlay_081c0738
Overlay_081c0738:
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
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r3, [r1, #24]
	movs	r2, #3
	cmp	r3, #0
	bne.n	.L_081c076a
	movs	r2, #12
.L_081c076a:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr
