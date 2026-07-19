.syntax unified
.text
	.arm
	.global Func_08015e10
Func_08015e10:
	push	{r5, r6, r7, r8, r9, sl}
	add	sl, pc, #100
	ldm	sl, {r4, r6, r7}
.L_08015e1c:
	add	sl, r0, r2, lsl #3
.L_08015e20:
	add	r9, r1, #32
.L_08015e24:
	ldm	r0, {r5, ip}
	orr	r5, r5, r5, lsr #4
	and	r5, r5, r4
	orr	r5, r5, r5, lsr #8
	and	r5, r5, r6
	orr	ip, ip, ip, lsr #4
	and	ip, ip, r4
	orr	ip, ip, ip, lsr #8
	orr	ip, r5, ip, lsl #16
	str	ip, [r1], #4
	add	r0, r0, r2, lsl #3
	cmp	r1, r9
	bne .L_08015e24
	sub	r0, r0, r2, lsl #6
	add	r0, r0, #8
	cmp	r0, sl
	bne .L_08015e20
	add	r0, r0, r2, lsl #6
	sub	r0, r0, r2, lsl #3
	subs	r3, r3, #1
	bne .L_08015e1c
	pop	{r5, r6, r7, r8, r9, sl}
	bx	lr
	.4byte 0x00ff00ff
	.4byte 0x0000ffff
	.4byte 0x11111111
