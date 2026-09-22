.syntax unified
	.thumb
	.global Overlay_081c171c
Overlay_081c171c:
	mov	ip, lr
	movs	r1, #36
	ldr	r2, [pc, #40]
.L_081c1722:
	ldr	r3, [r2, #0]
	bl	.L_081c1736
	stmia	r0!, {r3}
	adds	r2, #4
	subs	r1, #1
	bgt.n	.L_081c1722
	bx	ip
	movs	r0, r0
	ldrb	r3, [r2, #0]
.L_081c1736:
	push	{r0}
	lsrs	r0, r2, #25
	bne.n	.L_081c1748
	ldr	r0, [pc, #12]
	cmp	r2, r0
	bcc.n	.L_081c1746
	lsrs	r0, r2, #14
	beq.n	.L_081c1748
.L_081c1746:
	movs	r3, #0
.L_081c1748:
	pop	{r0}
	bx	lr
	.4byte 0x081c343c
