@ C2: add ip, pc, #36 followed by ldm ip, {sl, fp} loads a data block
@ relative to pc, including r11/fp as ordinary decoder state.
.syntax unified
	.arm
	.global Resource_Decode2
Resource_Decode2:
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
	blmi	Resource_RefillType2Bits
	b	Resource_DecodeType2Loop
