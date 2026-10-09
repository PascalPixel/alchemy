@ C2: push saves callee-saved r5/r6 without lr; body also edits branch
@ instructions.
.syntax unified
	.arm
	.global Resource_PatchThumbBranch
Resource_PatchThumbBranch:
	push	{r5, r6}
	add	r1, r1, r1
	add	r2, r0, r1
	add	r0, r0, #2
	mov	r6, r0
	mov	r4, #31
	mov	r5, #30
.L_08002d78:
	cmp	r0, r2
	bcs	.L_08002dd0
	ldrh	ip, [r0], #2
	cmp	r4, ip, lsr #11
	bne	.L_08002d78
	ldrh	r3, [r0, #-4]
	cmp	r5, r3, lsr #11
	bne	.L_08002d78
	bic	ip, ip, #63488
	bic	r3, r3, #63488
	orr	ip, ip, r3, lsl #11
	lsl	ip, ip, #1
	sub	r3, r0, r6
	sub	ip, ip, r3
	lsr	r3, ip, #12
	bic	r3, r3, #63488
	orr	r3, r3, #61440
	lsr	ip, ip, #1
	orr	ip, ip, #63488
	strh	r3, [r0, #-4]
	strh	ip, [r0, #-2]
	b	.L_08002d78
.L_08002dd0:
	pop	{r5, r6}
	bx	lr
	.global Resource_PatchThumbBranchCodeSize
	.set Resource_PatchThumbBranchCodeSize, . - Resource_PatchThumbBranch
