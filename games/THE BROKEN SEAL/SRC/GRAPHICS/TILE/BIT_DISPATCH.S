@ Command dispatcher of the image decoder in DECODE.S: it takes the low three
@ bits of the bit word in r3 and jumps through the eight-entry table that
@ follows (ldr pc, [pc, ip, lsl #2]) without a range check; DECODE.S rewrites
@ that table for the RAM copy it runs from. GCC's switch jumps always compare
@ against the table size first. Every TBS and TLA edition has these bytes.
@ The nop after its jump (0x080b5214), which the jump always skips, carries
@ no credit.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank
.syntax unified
.text
	.arm
	.global Func_080b520c
Func_080b520c:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_080b5214:
	nop
AlchemyUncreditedEnd_080b5214:
