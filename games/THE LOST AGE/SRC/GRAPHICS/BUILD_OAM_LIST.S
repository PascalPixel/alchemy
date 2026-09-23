@ ARM list builder that runs from the stack, with its Thumb loader. The
@ loader reads {DMA3 register base, 0x080018b4, 0x84000050} from a PC-relative
@ block with add r1, pc and ldmia r1, {r1, r2, r4}, has DMA3 copy these 320
@ bytes into a 320-byte stack buffer and calls the copy; neither approved
@ compiler emits an add rd, pc address form. Behind its bx pc entry the ARM
@ body reads a PC-relative parameter block, walks lists with ldmdb r2!, enters
@ a subroutine with movs; blne and links into IWRAM at 0x03000260 by hand with
@ add lr, pc; bx r4, as the TBS object-attribute list builder
@ (GRAPHICS/RENDER/BUILD_OAM_LIST.S, 0x08001dc8) does. The body is TLA's own.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: handwritten — stack-executed ARM list builder with its stack-copy wrapper
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x080018b4
	.global Render_BuildOamListKernel
	.thumb_func
Render_BuildOamListKernel:
.L_080018b4:
AlchemyUncredited_080018b4:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080018b4:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	add	r1, pc, #0x118
	ldm	r1, {r2, sl, fp, ip}
	mov	r1, #128
	mov	r9, r0
	mov	r4, #0x100
.L_080018d0:
	ldmdb	r2!, {r3}
	movs	r3, r3
	blne	.L_0800192c
	subs	r4, r4, #32
	bgt	.L_080018d0
	rsb	r2, r1, #128
	ldrb	r4, [sl]
	cmp	r2, r4, lsl #2
	lslle	r2, r4, #2
	ldrh	r4, [fp, #2]
	cmp	r4, r2, lsl #1
	lslle	r4, r2, #1
	rsb	r3, r1, #128
	subs	r3, r4, r3, lsl #1
	orr	r4, r4, r2, lsl #17
	str	r4, [fp]
	ble	.L_08001998
	lsl	r1, r3, #2
	mov	r2, #192
	orr	r2, r2, #0xc00000
	ldr	r4, [pc, #200]
	add	lr, pc, #108
	bx	r4
.L_0800192c:
	add	r5, ip, r4, lsl #2
.L_08001930:
	lsls	r3, r3, #1
	sub	r5, r5, #4
	bcs	.L_08001954
	bxeq	lr
	lsls	r3, r3, #1
	sub	r5, r5, #4
	bcs	.L_08001954
	bne	.L_08001930
	bxeq	lr
.L_08001954:
	ldrcs	r6, [r5]
.L_08001958:
	ldm	r6, {r6, r7, r8}
	stmia	r0!, {r7, r8}
	subs	r1, r1, #1
	beq	.L_0800198c
	cmp	r6, #0
	beq	.L_08001930
	ldm	r6, {r6, r7, r8}
	stmia	r0!, {r7, r8}
	subs	r1, r1, #1
	beq	.L_0800198c
	cmp	r6, #0
	beq	.L_08001930
	b	.L_08001958
.L_0800198c:
	mov	r1, #0x100
	orr	r1, r1, #0x1000000
	str	r1, [fp]
.L_08001998:
	add	r0, r9, #6
	ldr	r2, [pc, #60]
	ldrb	r4, [r2]
	ldr	r1, [pc, #64]
	cmp	r4, #0
	beq	.L_080019d4
.L_080019b0:
	ldm	r1!, {r2, r3}
	strh	r2, [r0], #32
	lsr	r2, r2, #16
	strh	r2, [r0, #-24]
	strh	r3, [r0, #-16]
	lsr	r3, r3, #16
	strh	r3, [r0, #-8]
	subs	r4, r4, #1
	bne	.L_080019b0
.L_080019d4:
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
	.4byte	0x030011b0
	.4byte	0x030011fc
	.4byte	0x03001248
	.4byte	0x02003010
	.4byte	0x020054d0
	.4byte	0x03000260
	.thumb
	.global Render_BuildOamList
	.thumb_func
Render_BuildOamList:
	push	{lr}
	add	r1, pc, #16
	ldmia	r1, {r1, r2, r4}
	sub	sp, #0x140
	mov	r3, sp
	stmia	r1!, {r2, r3, r4}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #0x140
	pop	{pc}
	.4byte	0x040000d4
	.4byte	.L_080018b4
	.4byte	0x84000050
