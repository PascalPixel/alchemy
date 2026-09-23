@ Builds the object-attribute list from the linked object heads at 0x03001800,
@ then writes the affine parameter words. The runtime copies these 224 bytes
@ to a heap block, calls them and frees them (loader 0x08003e10).
@ It is ARM-state code with an internal subroutine entered by conditional blne
@ and left by bxeq lr, and it links into the IWRAM routine at 0x03000168 by
@ hand (add lr, pc; bx r4). Neither form is compiler output. No TLA
@ edition carries these bytes; TLA has its own revision (TLA
@ GRAPHICS/BUILD_OAM_LIST.S).
@ credit: handwritten — ARM object-attribute list builder copied to the heap
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08001dc8
	.global Render_BuildOamList
Render_BuildOamList:
	push	{r5, r6, r7, r8, r9, lr}
	ldr	r2, [pc, #196]
	mov	ip, #64
	mov	r1, #128
	mov	r9, r0
.L_08001ddc:
	ldmdb	r2!, {r3, r4, r5, r6}
	movs	r6, r6
	blne	.L_08001e20
	movs	r6, r5
	blne	.L_08001e20
	movs	r6, r4
	blne	.L_08001e20
	movs	r6, r3
	blne	.L_08001e20
	subs	ip, ip, #1
	bne	.L_08001ddc
	mov	r2, #192
	orr	r2, r2, #0xc00000
	lsl	r1, r1, #3
	ldr	r4, [pc, #128]
	add	lr, pc, #52
	bx	r4
.L_08001e20:
	ldm	r6, {r6, r7, r8}
	stmia	r0!, {r7, r8}
	subs	r1, r1, #1
	beq	.L_08001e54
	cmp	r6, #0
	bxeq	lr
	ldm	r6, {r6, r7, r8}
	stmia	r0!, {r7, r8}
	subs	r1, r1, #1
	beq	.L_08001e54
	cmp	r6, #0
	bxeq	lr
	b	.L_08001e20
.L_08001e54:
	add	r0, r9, #6
	ldr	r2, [pc, #64]
	ldrb	r4, [r2]
	ldr	r1, [pc, #60]
	cmp	r4, #0
	beq	.L_08001e90
.L_08001e6c:
	ldm	r1!, {r2, r3}
	strh	r2, [r0], #32
	lsr	r2, r2, #16
	strh	r2, [r0, #-24]
	strh	r3, [r0, #-16]
	lsr	r3, r3, #16
	strh	r3, [r0, #-8]
	subs	r4, r4, #1
	bne	.L_08001e6c
.L_08001e90:
	pop	{r5, r6, r7, r8, r9, lr}
	bx	lr
	.4byte	0x03001800
	.4byte	0x03000168
	.4byte	0x03001d00
	.4byte	0x03001d40
