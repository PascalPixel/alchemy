@ Compression format 2 decoder. Its loader (0x080158cc) allocates a
@ 0x4f0-byte heap block, has DMA3 copy the 1,264 bytes from 0x08012b70
@ (size and source in its literal pool at 0x08015914: this module, the
@ decoder's nibble words at 0x08012bb4 and DECODE_2_LOOP.S), calls the copy
@ and frees it. Behind a 4-byte Thumb entry (bx pc) it returns 4 with
@ movne r0, #4; bxne lr before it saves any register, then loads its
@ nibble order (add ip, pc, #36; ldm ip, {sl, fp}) and jumps into the loop.
@ GCC saves registers on entry, before any return. The two nibble words
@ are a data table (DECODE_2_NIBBLES.S) and carry no credit, nor does the
@ entry, which is TLA's own. After the entry these 64 bytes equal TBS
@ SYSTEM/RESOURCE/DECODE_2.S (0x08002808).
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank, compression format 2 decoder
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08012b70
	.set	.L_08012e54, 0x08012e54
	.set	.L_08012c7c, 0x08012c7c
	.global Resource_Decode2
	.thumb_func
Resource_Decode2:
AlchemyUncredited_08012b70:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08012b70:
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
	blmi	.L_08012e54
	b	.L_08012c7c
