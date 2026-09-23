@ Masked tile expander, copied and run as one 160-byte unit (size and source
@ {0xa0, 0x08038d90} in its loader's literal pool at 0x0803dc0c). Behind a
@ 4-byte Thumb entry (bx pc) the ARM body loads its masks from the three
@ words after the code (EXPAND_MASKED_MASKS.S) with add sl, pc, #132 and
@ saves r5-sl without lr; GCC saves lr with any register. After that entry
@ its 144-byte body and the masks equal TBS GRAPHICS/TILE/EXPAND_MASKED.S
@ (0x08015d74) and EXPAND_MASKED_MASKS.S byte for byte. The entry, which is
@ TLA's own, and the mask table carry no credit.
@ credit: library — TBS/TLA common ARM text and tile kernel bank, masked tile expander
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08038d90
	.global Tile_ExpandMasked
	.thumb_func
Tile_ExpandMasked:
AlchemyUncredited_08038d90:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08038d90:
	push	{r5, r6, r7, r8, r9, sl}
	add	sl, pc, #132
	ldm	sl, {r4, r6, r7}
.L_08038da0:
	add	sl, r0, r2, lsl #3
.L_08038da4:
	add	r9, r1, #32
.L_08038da8:
	ldm	r0, {r5, ip}
	orr	r5, r5, r5, lsr #4
	and	r5, r5, r4
	orr	r5, r5, r5, lsr #8
	and	r5, r5, r6
	orr	ip, ip, ip, lsr #4
	and	ip, ip, r4
	orr	ip, ip, ip, lsr #8
	orr	ip, r5, ip, lsl #16
	orr	r5, ip, ip, lsr #1
	orr	r5, r5, r5, lsr #2
	and	r5, r5, r7
	rsbs	r5, r5, r5, lsl #4
	mvns	r8, r5
	ldrne	r8, [r1]
	bicne	r8, r8, r5
	orrne	ip, ip, r8
	str	ip, [r1], #4
	add	r0, r0, r2, lsl #3
	cmp	r1, r9
	bne	.L_08038da8
	sub	r0, r0, r2, lsl #6
	add	r0, r0, #8
	cmp	r0, sl
	bne	.L_08038da4
	add	r0, r0, r2, lsl #6
	sub	r0, r0, r2, lsl #3
	subs	r3, r3, #1
	bne	.L_08038da0
	pop	{r5, r6, r7, r8, r9, sl}
	bx	lr
