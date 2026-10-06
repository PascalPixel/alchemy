.syntax unified
	.thumb
	.global Resource_PatchThumbBranchCode
Resource_PatchThumbBranchCode:
	.global Resource_PatchThumbBranch
	.thumb_func
Resource_PatchThumbBranch:
	bx	pc
AlchemyUncredited_080130ce:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080130ce:
	push	{r5, r6}
	add	r1, r1, r1
	add	r2, r0, r1
	add	r0, r0, #2
	mov	r6, r0
	mov	r4, #31
	mov	r5, #30
.L_080130ec:
	cmp	r0, r2
	bcs	.L_08013144
	ldrh	ip, [r0], #2
	cmp	r4, ip, lsr #11
	bne	.L_080130ec
	ldrh	r3, [r0, #-4]
	cmp	r5, r3, lsr #11
	bne	.L_080130ec
	bic	ip, ip, #0xf800
	bic	r3, r3, #0xf800
	orr	ip, ip, r3, lsl #11
	lsl	ip, ip, #1
	sub	r3, r0, r6
	sub	ip, ip, r3
	lsr	r3, ip, #12
	bic	r3, r3, #0xf800
	orr	r3, r3, #0xf000
	lsr	ip, ip, #1
	orr	ip, ip, #0xf800
	strh	r3, [r0, #-4]
	strh	ip, [r0, #-2]
	b	.L_080130ec
.L_08013144:
	pop	{r5, r6}
	bx	lr
	.global Resource_PatchThumbBranchCodeSize
	.set Resource_PatchThumbBranchCodeSize, . - Resource_PatchThumbBranchCode
