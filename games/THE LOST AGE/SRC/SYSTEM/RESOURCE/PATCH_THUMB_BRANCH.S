@ Relocated Thumb branch patcher, which rewrites BL pairs in place. Its loader
@ (0x0801336c) allocates a 0x80-byte heap block, has DMA3 copy exactly this
@ module there (size and source in its literal pool at 0x080133b8), calls the
@ copy and frees it. Behind a 4-byte Thumb entry (bx pc) the ARM body saves
@ r5 and r6 without lr and subtracts registers with sub (sub r3, r0, r6);
@ GCC saves lr with any register and writes register subtraction as rsb.
@ After that entry its 124-byte body equals TBS
@ SYSTEM/RESOURCE/PATCH_THUMB_BRANCH.S (0x08002d5c) byte for byte.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank, relocated Thumb branch patcher
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x080130cc
	.global Resource_PatchThumbBranch
	.thumb_func
Resource_PatchThumbBranch:
AlchemyUncredited_080130cc:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080130cc:
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
