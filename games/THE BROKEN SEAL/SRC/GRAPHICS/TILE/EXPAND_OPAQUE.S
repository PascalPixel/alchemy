@ Packs 8-bit pixels into 4bpp tile rows, overwriting the destination. It runs
@ from a 124-byte RAM copy of these 112 bytes and the three mask words after
@ them (EXPAND_OPAQUE_MASKS.S, a data table without credit), loads those
@ masks with add sl, pc, #100 and saves r5-sl without lr; GCC addresses
@ constants through its literal pool and saves lr with any register. Every
@ TBS and TLA edition has these bytes.
@ credit: library — TBS/TLA common ARM text and tile kernel bank
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
