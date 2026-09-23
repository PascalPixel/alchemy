@ Opaque tile expander, copied and run as one 128-byte unit (size and source
@ {0x80, 0x08038e30} in its loader's literal pool at 0x0803dc14). Behind a
@ 4-byte Thumb entry (bx pc) the ARM body loads its masks from the three
@ words after the code (EXPAND_OPAQUE_MASKS.S) with add sl, pc, #100 and
@ saves r5-sl without lr; GCC saves lr with any register. After that entry
@ its 112-byte body and the masks equal TBS GRAPHICS/TILE/EXPAND_OPAQUE.S
@ (0x08015e10) and EXPAND_OPAQUE_MASKS.S byte for byte. The entry, which is
@ TLA's own, and the mask table carry no credit.
@ credit: library — TBS/TLA common ARM text and tile kernel bank, opaque tile expander
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08038e30
	.global Tile_ExpandOpaque
	.thumb_func
Tile_ExpandOpaque:
AlchemyUncredited_08038e30:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08038e30:
	push	{r5, r6, r7, r8, r9, sl}
	add	sl, pc, #100
	ldm	sl, {r4, r6, r7}
.L_08038e40:
	add	sl, r0, r2, lsl #3
.L_08038e44:
	add	r9, r1, #32
.L_08038e48:
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
	bne	.L_08038e48
	sub	r0, r0, r2, lsl #6
	add	r0, r0, #8
	cmp	r0, sl
	bne	.L_08038e44
	add	r0, r0, r2, lsl #6
	sub	r0, r0, r2, lsl #3
	subs	r3, r3, #1
	bne	.L_08038e40
	pop	{r5, r6, r7, r8, r9, sl}
	bx	lr
