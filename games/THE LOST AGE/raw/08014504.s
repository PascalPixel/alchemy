.syntax unified
	.thumb
	.global Overlay_08014504
Overlay_08014504:
	push	{lr}
	lsrs	r2, r2, #2
	movs	r4, #0
	cmp	r4, r2
	bcs.n	.L_08014518
.L_0801450e:
	ldmia	r1!, {r3}
	adds	r4, #1
	stmia	r0!, {r3}
	cmp	r4, r2
	bcc.n	.L_0801450e
.L_08014518:
	pop	{pc}
