@ Symbol lookup, copied and run as one 100-byte unit (size and source {0x64,
@ 0x0803857c} in its loader's literal pool at 0x0803d1bc). Behind a 4-byte
@ Thumb entry (bx pc) the ARM body puts a sentinel bit on top of the bit word
@ with rrx from the carry of rsbs r4, r3, r4, lsl #3; GCC never emits rrx.
@ After that entry its 96-byte body equals TBS GRAPHICS/TEXT/LOOKUP_SYMBOL.S
@ (0x08015570) apart from one relocated literal word.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: library — TBS/TLA common ARM text and tile kernel bank, symbol lookup
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x0803857c
	.global Text_LookupSymbol
	.thumb_func
Text_LookupSymbol:
AlchemyUncredited_0803857c:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_0803857c:
	lsr	r3, r1, #8
	ldr	ip, [pc, #80]
	add	ip, ip, r3, lsl #3
	ldm	ip, {r2, r4}
	ands	r1, r1, #255
	beq	.L_080385b0
.L_08038598:
	ldrb	r3, [r4], #1
	add	r2, r2, r3
	cmp	r3, #255
	beq	.L_08038598
	subs	r1, r1, #1
	bne	.L_08038598
.L_080385b0:
	mov	r3, #1
	ands	r4, r2, #3
	beq	.L_080385d0
	rsbs	r4, r3, r4, lsl #3
	bic	r2, r2, #3
	ldr	r3, [r2], #4
	rrx	r3, r3
	lsr	r3, r3, r4
.L_080385d0:
	mov	r1, #0
	stm	r0, {r1, r2, r3}
	bx	lr
	.4byte	0x080a9f54
