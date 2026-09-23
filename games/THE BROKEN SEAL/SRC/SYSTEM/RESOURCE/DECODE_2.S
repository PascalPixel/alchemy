@ Resource decoder for stream type 2, run from a heap copy that the loader
@ at 0x08005394 makes of the 1,260-byte block formed by this entry,
@ DECODE_2_NIBBLES.S and DECODE_2_LOOP.S. It returns 4 with movne r0, #4;
@ bxne lr before it saves any register, then loads its nibble order from
@ DECODE_2_NIBBLES.S (add ip, pc, #36; ldm ip, {sl, fp}), primes its bit
@ buffer and jumps into the loop. GCC saves registers on entry, before any
@ return. Every TBS and TLA edition has these bytes.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank
.syntax unified
	.set	.L_08002ae8, 0x08002ae8
	.set	.L_08002910, 0x08002910
	.arm
	.global Func_08002808
Func_08002808:
	ldrb	r2, [r0], #1
	cmp	r2, #2
	movne	r0, #4
	bxne	lr
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	add	ip, pc, #36
	ldm	ip, {sl, fp}
	mov	ip, #15
	sub	r1, r1, #1
	add	r9, r1, #33
	ands	r2, r0, #1
	lsl	r4, r2, #3
	ldrbne	r2, [r0], #1
	subs	r4, r4, #16
	blmi	.L_08002ae8
	b	.L_08002910
