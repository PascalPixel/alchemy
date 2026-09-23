@ 4bpp tile decompressor, copied and run as one 636-byte unit (size and
@ source {0x27c, 0x08038b14} in the literal pools at 0x0803dc04 and
@ 0x080455a8): this module, the nibble words at 0x08038b4c and
@ DECOMPRESS_4BPP_LOOP.S. Behind a 4-byte Thumb entry (bx pc) it saves r5
@ and r6 without lr, loads the starting order of its move-to-front nibble
@ table (add r5, pc, #40; ldm r5, {r2, r3}), primes the bit word of an
@ unaligned source with rrx and jumps into the loop. GCC saves lr with any
@ register and never emits rrx. The two nibble words are a data table
@ (DECOMPRESS_4BPP_NIBBLES.S) and carry no credit, nor does the entry, which
@ is TLA's own. After the entry these 52 bytes equal TBS
@ GRAPHICS/TILE/DECOMPRESS_4BPP.S (0x08015afc).
@ credit: library — TBS/TLA common ARM text and tile kernel bank, 4bpp decompressor
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08038b14
	.set	.L_08038b70, 0x08038b70
	.global Tile_Decompress4bpp
	.thumb_func
Tile_Decompress4bpp:
AlchemyUncredited_08038b14:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08038b14:
	push	{r5, r6}
	add	r5, pc, #40
	ldm	r5, {r2, r3}
	mov	r5, #15
	mov	ip, #1
	ands	r6, r0, #3
	beq	.L_08038b70
	rsbs	r6, ip, r6, lsl #3
	bic	r0, r0, #3
	ldr	ip, [r0], #4
	rrx	ip, ip
	lsr	ip, ip, r6
	b	.L_08038b70
