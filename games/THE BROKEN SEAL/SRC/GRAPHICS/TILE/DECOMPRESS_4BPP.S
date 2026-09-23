@ Decodes 4bpp pixel data whose nibbles are coded through a move-to-front
@ table kept in r2 and r3. This entry saves r5 and r6 without lr, loads
@ the table's starting order from DECOMPRESS_4BPP_NIBBLES.S
@ (add r5, pc, #40; ldm r5, {r2, r3}), primes the bit word of an unaligned
@ source with rrx and jumps into DECOMPRESS_4BPP_LOOP.S. GCC saves lr with
@ any register and never emits rrx. Two loaders copy the 632-byte block of
@ this entry, the table and the loop to RAM and run it there. Every TBS and
@ TLA edition has these bytes.
@ credit: library — TBS/TLA common ARM text and tile kernel bank
.syntax unified
	.set	.L_08015b54, 0x08015b54
.text
	.arm
	.global Func_08015afc
Func_08015afc:
	push	{r5, r6}
	add	r5, pc, #40
	ldm	r5, {r2, r3}
	mov	r5, #15
	mov	ip, #1
	ands	r6, r0, #3
	beq .L_08015b54
	rsbs	r6, ip, r6, lsl #3
	bic	r0, r0, #3
	ldr	ip, [r0], #4
	rrx	ip, ip
	lsr	ip, ip, r6
	b .L_08015b54
