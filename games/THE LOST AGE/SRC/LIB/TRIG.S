@ Cosine and sine sharing one body: the cosine entry adds a quarter turn and
@ falls through into the sine entry, which rounds the angle to a 10-bit index
@ and reads a 1,024-entry Q14 sine table with add r0, pc and ldrsh, then
@ scales the entry to 16.16. One function falling into another has no C form,
@ and neither approved compiler emits an add rd, pc address form. The table
@ (2,048 bytes at 0x080020a8, after a halfword of alignment) is data in
@ SINE_TABLE.S and carries no credit. The routine is TLA's own: TBS LIB/TRIG.S
@ (0x0800230c) folds its angle into a quarter-wave table instead.
@ credit: handwritten — deliberate math primitive, cosine and sine
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08002090
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	movs	r1, #64
	lsls	r1, r1, #8
	adds	r0, r0, r1
	.global Math_Sine
	.thumb_func
Math_Sine:
	adds	r0, #32
	lsls	r1, r0, #16
	lsrs	r1, r1, #22
	lsls	r1, r1, #1
	add	r0, pc, #8
	ldrsh	r0, [r0, r1]
	lsls	r0, r0, #2
	bx	lr
