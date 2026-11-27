@ Why assembly: bcs/bcc consume lsls lr's carry. The decoder dispatch also
@ enters eight-byte code slots, unlike the compiler's four-byte switch
@ entries. Its return pops the r1 saved at entry into r0. Its Thumb entry's
@ bx pc switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
@ Why syRenderBuildOAMListKernel is assembly:
@ its Thumb entry's bx pc switches to the ARM code that follows, a mode
@ switch inside a function; the halfword after bx pc is never run.
@ bcs consumes the carry of lsls r3, r3, #1; movs r3, r3 copies and tests
@ without a shift; add r1, pc, #0x118 / ldm r1, {r2, sl, fp, ip} reads its
@ four addresses with one pc-relative block load; and fp (r11) holds
@ OamUsageWork's address, not a frame pointer.
@ Why syRenderBuildOAMList is assembly:
@ it reads its three DMA words, the channel's address, the kernel and the
@ control, with one pc-relative block load (add r1, pc, #16; ldmia r1,
@ {r1, r2, r4}).
@ Why ColorBuffer_ScaleThreeQuartersKernelCode to ColorBuffer_HalveKernelCode are assembly:
@ each Thumb entry's bx pc switches to the ARM code that follows, a mode
@ switch inside a function; the halfword after bx pc is never run.
@ push {r5-sl} saves callee-saved registers without lr.
@ Why ColorBuffer_DarkenKernelCode is assembly:
@ its Thumb entry's bx pc switches to the ARM code that follows, a mode
@ switch inside a function; the halfword after bx pc is never run.
@ fp (r11) holds the eighth colour word of each ldm and stmia, not a frame
@ pointer.
@ Why assembly: its Thumb entry's bx pc switches to the ARM code that follows,
@ a mode switch inside a function; the halfword after bx pc is never run.
@ push {r5-r9} saves callee-saved registers without lr.
@ Why syColBufferScaleThreeQuarters to syColBufferDarken are assembly:
@ each reads its three DMA words, the channel's address, the kernel and the
@ control, with one pc-relative block load (add r4, pc, #16; ldmia r4,
@ {r4, r5, r7}).
@ The loaders of the backup kernels above and of the brighten kernel: each
@ copies its ARM kernel by DMA into a buffer on
@ the stack and calls it there, through lr with a lone BL suffix.
@ Why ColorBuffer_ScaleNonzeroThreeQuartersKernelCode to ColorBuffer_HalveNonzeroKernelCode are assembly:
@ each Thumb entry's bx pc switches to the ARM code that follows, a mode
@ switch inside a function; the halfword after bx pc is never run.
@ push {r5, r6} saves callee-saved registers without lr.
@ Why ColorBuffer_DarkenPartialKernelCode is assembly:
@ its Thumb entry's bx pc switches to the ARM code that follows, a mode
@ switch inside a function; the halfword after bx pc is never run.
@ ldm r0!, {r4-r7} and stmia r2!, {r4-r7} write back through a
@ four-register list starting at r4. GCC 2.95.2's block-copy expander
@ fixes that list at r0-r3; its general ldm/stm peepholes never write back.
@ Why ColorBuffer_BrightenPartialKernelCode is assembly:
@ its Thumb entry's bx pc switches to the ARM code that follows, a mode
@ switch inside a function; the halfword after bx pc is never run.
@ push {r5-r9} saves callee-saved registers without lr.
@ Why syColBufferScaleNonzeroThreeQuarters to ColorBuffer_DarkenPartial are assembly:
@ each reads its three DMA words, the channel's address, the kernel and the
@ control, with one pc-relative block load (add r4, pc, #16; ldmia r4,
@ {r4, r5, r7}).
@ Why assembly: bcc and adc consume lsls r3's carry; the intervening ldrh
@ keeps flags. Its Thumb entry's bx pc switches to the ARM code that follows, a mode switch inside a function;
@ the halfword after bx pc is never run.
	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global Resource_DecodeByteLzCode
Resource_DecodeByteLzCode:
	.global Resource_DecodeByteLz
	.thumb_func
Resource_DecodeByteLz:
	bx	pc
AlchemyUncredited_0800165a:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_0800165a:
	push	{r1, r5, r6, r7, lr}
.L_08001660:
	ldrb	lr, [r0], #1
	lsls	lr, lr, #24
	bne	.L_08001718
	ands	r2, r0, #3
	ldmeq	r0!, {r4, r5}
	beq	.L_08001698
	ldm	r0, {r4, r5, r6}
	lsl	r2, r2, #3
	rsb	r7, r2, #32
	lsr	r4, r4, r2
	orr	r4, r4, r5, lsl r7
	lsr	r5, r5, r2
	orr	r5, r5, r6, lsl r7
	add	r0, r0, #8
.L_08001698:
	strb	r4, [r1], #1
	lsr	r4, r4, #8
	strb	r4, [r1], #1
	lsr	r4, r4, #8
	strb	r4, [r1], #1
	lsr	r4, r4, #8
	strb	r4, [r1], #1
	strb	r5, [r1], #1
	lsr	r5, r5, #8
	strb	r5, [r1], #1
	lsr	r5, r5, #8
	strb	r5, [r1], #1
	lsr	r5, r5, #8
	strb	r5, [r1], #1
	b	.L_08001660
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	ldrb	r2, [r0], #1
	strb	r2, [r1], #1
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	ldrb	r2, [r0], #1
	strb	r2, [r1], #1
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	ldrb	r2, [r0], #1
	strb	r2, [r1], #1
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	ldrb	r2, [r0], #1
	strb	r2, [r1], #1
	b	.L_08001660
.L_08001718:
	orr	lr, lr, #0x800000
	lsls	lr, lr, #1
	bcs	.L_08001738
.L_08001724:
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	lsls	lr, lr, #1
	bcc	.L_08001724
	beq	.L_08001660
.L_08001738:
	ldrb	ip, [r0], #1
	ldrb	r3, [r0], #1
	and	r2, ip, #240
	rsb	r3, r3, r1
	sub	r4, r3, r2, lsl #4
	ands	ip, ip, #15
	beq	.L_080017f0
	rsb	ip, ip, #15
	add	pc, pc, ip, lsl #3
@ The slot pc + 8 skips; never run.
AlchemyUncredited_0800175c:
	nop
AlchemyUncreditedEnd_0800175c:
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	lsls	lr, lr, #1
	bcc	.L_08001724
	beq	.L_08001660
	b	.L_08001738
.L_080017f0:
	cmp	r4, r1
	beq	.L_080018a8
	ldrb	r3, [r0], #1
	add	r3, r3, #16
	and	ip, r3, #15
	rsb	ip, ip, #15
	add	pc, pc, ip, lsl #3
@ The slot pc + 8 skips; never run.
AlchemyUncredited_0800180c:
	nop
.L_08001810:
AlchemyUncreditedEnd_0800180c:
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	subs	r3, r3, #16
	bge	.L_08001810
	lsls	lr, lr, #1
	bcc	.L_08001724
	beq	.L_08001660
	b	.L_08001738
.L_080018a8:
	pop	{r0, r5, r6, r7, ip}
	sub	r0, r1, r0
	bx	ip
	.global Resource_DecodeByteLzCodeSize
	.set Resource_DecodeByteLzCodeSize, . - Resource_DecodeByteLzCode

	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global syRenderBuildOAMListKernel
	.thumb_func
syRenderBuildOAMListKernel:
	.global syRenderBuildOAMListKernelCode
syRenderBuildOAMListKernelCode:
	bx	pc
AlchemyUncredited_080018b6:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080018b6:
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
	.4byte	OamDepthMasks + 32
	.4byte	OamMatrixCount
	.4byte	OamUsageWork
	.4byte	OamDepthLists
	.4byte	OamMatrices
	.4byte	syIWRAMWordsFill

@ The kernel's loader: copies the ARM kernel by DMA into a buffer on the stack
@ and calls it there, through lr with a lone BL suffix, as the palette
@ kernels' loaders do.
	.thumb
	.global syRenderBuildOAMList
	.thumb_func
syRenderBuildOAMList:
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
	.4byte	syRenderBuildOAMListKernelCode
	.4byte	0x84000050

	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global ColorBuffer_ScaleThreeQuartersKernelCode
ColorBuffer_ScaleThreeQuartersKernelCode:
.L_08001a14:
	bx	pc
AlchemyUncredited_08001a16:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001a16:
	push	{r5, r6, r7, r8, r9, sl}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001a2c:
	ldm	r0, {r3, r4, r5, r6, r7, r8, r9, sl}
	stmia	r1!, {r3, r4, r5, r6, r7, r8, r9, sl}
	adds	r3, r3, r3, lsl #1
	andne	r3, ip, r3, lsr #2
	adds	r4, r4, r4, lsl #1
	andne	r4, ip, r4, lsr #2
	adds	r5, r5, r5, lsl #1
	andne	r5, ip, r5, lsr #2
	adds	r6, r6, r6, lsl #1
	andne	r6, ip, r6, lsr #2
	adds	r7, r7, r7, lsl #1
	andne	r7, ip, r7, lsr #2
	adds	r8, r8, r8, lsl #1
	andne	r8, ip, r8, lsr #2
	adds	r9, r9, r9, lsl #1
	andne	r9, ip, r9, lsr #2
	adds	sl, sl, sl, lsl #1
	andne	sl, ip, sl, lsr #2
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, sl}
	ldm	r0, {r3, r4, r5, r6, r7, r8, r9, sl}
	stmia	r1!, {r3, r4, r5, r6, r7, r8, r9, sl}
	adds	r3, r3, r3, lsl #1
	andne	r3, ip, r3, lsr #2
	adds	r4, r4, r4, lsl #1
	andne	r4, ip, r4, lsr #2
	adds	r5, r5, r5, lsl #1
	andne	r5, ip, r5, lsr #2
	adds	r6, r6, r6, lsl #1
	andne	r6, ip, r6, lsr #2
	adds	r7, r7, r7, lsl #1
	andne	r7, ip, r7, lsr #2
	adds	r8, r8, r8, lsl #1
	andne	r8, ip, r8, lsr #2
	adds	r9, r9, r9, lsl #1
	andne	r9, ip, r9, lsr #2
	adds	sl, sl, sl, lsl #1
	andne	sl, ip, sl, lsr #2
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, sl}
	cmp	r0, r2
	bne	.L_08001a2c
	pop	{r5, r6, r7, r8, r9, sl}
	bx	lr
	.thumb
	.global ColorBuffer_HalveKernelCode
ColorBuffer_HalveKernelCode:
.L_08001ad4:
	bx	pc
AlchemyUncredited_08001ad6:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001ad6:
	push	{r5, r6, r7, r8, r9, sl}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001aec:
	ldm	r0, {r3, r4, r5, r6, r7, r8, r9, sl}
	stmia	r1!, {r3, r4, r5, r6, r7, r8, r9, sl}
	and	r3, ip, r3, lsr #1
	and	r4, ip, r4, lsr #1
	and	r5, ip, r5, lsr #1
	and	r6, ip, r6, lsr #1
	and	r7, ip, r7, lsr #1
	and	r8, ip, r8, lsr #1
	and	r9, ip, r9, lsr #1
	and	sl, ip, sl, lsr #1
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, sl}
	ldm	r0, {r3, r4, r5, r6, r7, r8, r9, sl}
	stmia	r1!, {r3, r4, r5, r6, r7, r8, r9, sl}
	and	r3, ip, r3, lsr #1
	and	r4, ip, r4, lsr #1
	and	r5, ip, r5, lsr #1
	and	r6, ip, r6, lsr #1
	and	r7, ip, r7, lsr #1
	and	r8, ip, r8, lsr #1
	and	r9, ip, r9, lsr #1
	and	sl, ip, sl, lsr #1
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, sl}
	cmp	r0, r2
	bne	.L_08001aec
	pop	{r5, r6, r7, r8, r9, sl}
	bx	lr
	.thumb
	.global ColorBuffer_DarkenKernelCode
ColorBuffer_DarkenKernelCode:
.L_08001b54:
	bx	pc
AlchemyUncredited_08001b56:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001b56:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	ip, [pc, #160]
	rsb	r1, r1, ip, lsl #7
	add	r3, r0, r3
.L_08001b68:
	ldm	r0, {r4, r5, r6, r7, r8, r9, sl, fp}
	stmia	r2!, {r4, r5, r6, r7, r8, r9, sl, fp}
	add	r4, r4, r1
	and	lr, ip, r4, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r4, r4, lr
	add	r5, r5, r1
	and	lr, ip, r5, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r5, r5, lr
	add	r6, r6, r1
	and	lr, ip, r6, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r6, r6, lr
	add	r7, r7, r1
	and	lr, ip, r7, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r7, r7, lr
	add	r8, r8, r1
	and	lr, ip, r8, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r8, r8, lr
	add	r9, r9, r1
	and	lr, ip, r9, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	r9, r9, lr
	add	sl, sl, r1
	and	lr, ip, sl, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	sl, sl, lr
	add	fp, fp, r1
	and	lr, ip, fp, lsr #7
	rsb	lr, lr, lr, lsl #7
	and	fp, fp, lr
	stmia	r0!, {r4, r5, r6, r7, r8, r9, sl, fp}
	cmp	r0, r3
	bne	.L_08001b68
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
	.4byte	0x01010101

	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global ColorBuffer_BrightenKernelCode
ColorBuffer_BrightenKernelCode:
.L_08001c08:
	bx	pc
AlchemyUncredited_08001c0a:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001c0a:
	push	{r5, r6, r7, r8, r9}
	mov	r8, #64
	orr	r8, r8, r8, lsl #8
	orr	r8, r8, r8, lsl #16
	sub	ip, r8, r8, lsr #6
	add	r3, r0, r3
.L_08001c24:
	ldm	r0, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	add	r4, r4, r1
	and	r9, r4, r8
	bic	r4, r4, r9
	sub	r9, r9, r9, lsr #6
	orr	r4, r4, r9
	add	r5, r5, r1
	and	r9, r5, r8
	bic	r5, r5, r9
	sub	r9, r9, r9, lsr #6
	orr	r5, r5, r9
	add	r6, r6, r1
	and	r9, r6, r8
	bic	r6, r6, r9
	sub	r9, r9, r9, lsr #6
	orr	r6, r6, r9
	add	r7, r7, r1
	and	r9, r7, r8
	bic	r7, r7, r9
	sub	r9, r9, r9, lsr #6
	orr	r7, r7, r9
	stmia	r0!, {r4, r5, r6, r7}
	cmp	r0, r3
	bne	.L_08001c24
	pop	{r5, r6, r7, r8, r9}
	bx	lr

	.section .text,"ax",%progbits
	.balign 2

.syntax unified
	.thumb
	.global syColBufferScaleThreeQuarters
	.thumb_func
syColBufferScaleThreeQuarters:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #192
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #192
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_ScaleThreeQuartersKernelCode
	.4byte	0x84000030
	.global syColBufferHalve
	.thumb_func
syColBufferHalve:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #128
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #128
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_HalveKernelCode
	.4byte	0x84000020
	.global syColBufferBrighten
	.thumb_func
syColBufferBrighten:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #136
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #136
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_BrightenKernelCode
	.4byte	0x84000022
	.global syColBufferDarken
	.thumb_func
syColBufferDarken:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #180
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	mov	lr, sp
	.inst.n	0xf800
	add	sp, #180
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_DarkenKernelCode
	.4byte	0x8400002d

	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global ColorBuffer_ScaleNonzeroThreeQuartersKernelCode
ColorBuffer_ScaleNonzeroThreeQuartersKernelCode:
	.global ColorBuffer_ScaleNonzeroThreeQuartersKernel
	.thumb_func
ColorBuffer_ScaleNonzeroThreeQuartersKernel:
.L_08001d10:
	bx	pc
AlchemyUncredited_08001d12:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001d12:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001d28:
	ldm	r0!, {r3, r4, r5, r6}
	stmia	r1!, {r3, r4, r5, r6}
	adds	r3, r3, r3, lsl #1
	andne	r3, ip, r3, lsr #2
	strne	r3, [r0, #-16]
	adds	r4, r4, r4, lsl #1
	andne	r4, ip, r4, lsr #2
	strne	r4, [r0, #-12]
	adds	r5, r5, r5, lsl #1
	andne	r5, ip, r5, lsr #2
	strne	r5, [r0, #-8]
	adds	r6, r6, r6, lsl #1
	andne	r6, ip, r6, lsr #2
	strne	r6, [r0, #-4]
	cmp	r0, r2
	bne	.L_08001d28
	pop	{r5, r6}
	bx	lr
	.thumb
	.global ColorBuffer_HalveNonzeroKernelCode
ColorBuffer_HalveNonzeroKernelCode:
	.global ColorBuffer_HalveNonzeroKernel
	.thumb_func
ColorBuffer_HalveNonzeroKernel:
.L_08001d70:
	bx	pc
AlchemyUncredited_08001d72:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001d72:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001d88:
	ldm	r0!, {r3, r4, r5, r6}
	stmia	r1!, {r3, r4, r5, r6}
	cmp	r3, #0
	andne	r3, ip, r3, lsr #1
	strne	r3, [r0, #-16]
	cmp	r4, #0
	andne	r4, ip, r4, lsr #1
	strne	r4, [r0, #-12]
	cmp	r5, #0
	andne	r5, ip, r5, lsr #1
	strne	r5, [r0, #-8]
	cmp	r6, #0
	andne	r6, ip, r6, lsr #1
	strne	r6, [r0, #-4]
	cmp	r0, r2
	bne	.L_08001d88
	pop	{r5, r6}
	bx	lr
	.thumb
	.global ColorBuffer_DarkenPartialKernelCode
ColorBuffer_DarkenPartialKernelCode:
	.global ColorBuffer_DarkenPartialKernel
	.thumb_func
ColorBuffer_DarkenPartialKernel:
.L_08001dd0:
	bx	pc
AlchemyUncredited_08001dd2:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001dd2:
	rsb	ip, r1, #0x80000000
	add	ip, ip, #0x800000
	add	ip, ip, #0x8000
	add	ip, ip, #128
	mov	r1, #0x100
	add	r1, r1, #1
	orr	r1, r1, r1, lsl #16
	push	{r5, r6, r7, r8, lr}
	add	lr, r0, r3
.L_08001df8:
	ldm	r0!, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	cmp	r4, #0
	bne	.L_08001e2c
	cmp	r5, #0
	bne	.L_08001e48
	cmp	r6, #0
	bne	.L_08001e64
	cmp	r7, #0
	bne	.L_08001e80
	cmp	r0, lr
	bne	.L_08001df8
	b	.L_08001e9c
.L_08001e2c:
	add	r8, r4, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-16]
	cmp	r5, #0
	beq	.L_08001e5c
.L_08001e48:
	add	r8, r5, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-12]
.L_08001e5c:
	cmp	r6, #0
	beq	.L_08001e78
.L_08001e64:
	add	r8, r6, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-8]
.L_08001e78:
	cmp	r7, #0
	beq	.L_08001e94
.L_08001e80:
	add	r8, r7, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-4]
.L_08001e94:
	cmp	r0, lr
	bne	.L_08001df8
.L_08001e9c:
	pop	{r5, r6, r7, r8, ip}
	bx	ip
	.thumb
	.global ColorBuffer_BrightenPartialKernelCode
ColorBuffer_BrightenPartialKernelCode:
	.global ColorBuffer_BrightenPartialKernel
	.thumb_func
ColorBuffer_BrightenPartialKernel:
.L_08001ea4:
	bx	pc
AlchemyUncredited_08001ea6:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001ea6:
	push	{r5, r6, r7, r8, r9}
	mov	r8, #64
	orr	r8, r8, r8, lsl #8
	orr	r8, r8, r8, lsl #16
	sub	ip, r8, r8, lsr #6
	add	r3, r0, r3
.L_08001ec0:
	ldm	r0!, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	cmp	r4, ip
	bne	.L_08001ef4
	cmp	r5, ip
	bne	.L_08001f14
	cmp	r6, ip
	bne	.L_08001f34
	cmp	r7, ip
	bne	.L_08001f54
	cmp	r0, r3
	bne	.L_08001ec0
	b	.L_08001f74
.L_08001ef4:
	add	r4, r4, r1
	and	r9, r4, r8
	bic	r4, r4, r9
	sub	r9, r9, r9, lsr #6
	orr	r4, r4, r9
	str	r4, [r0, #-16]
	cmp	r5, ip
	beq	.L_08001f2c
.L_08001f14:
	add	r5, r5, r1
	and	r9, r5, r8
	bic	r5, r5, r9
	sub	r9, r9, r9, lsr #6
	orr	r5, r5, r9
	str	r5, [r0, #-12]
.L_08001f2c:
	cmp	r6, ip
	beq	.L_08001f4c
.L_08001f34:
	add	r6, r6, r1
	and	r9, r6, r8
	bic	r6, r6, r9
	sub	r9, r9, r9, lsr #6
	orr	r6, r6, r9
	str	r6, [r0, #-8]
.L_08001f4c:
	cmp	r7, ip
	beq	.L_08001f6c
.L_08001f54:
	add	r7, r7, r1
	and	r9, r7, r8
	bic	r7, r7, r9
	sub	r9, r9, r9, lsr #6
	orr	r7, r7, r9
	str	r7, [r0, #-4]
.L_08001f6c:
	cmp	r0, r3
	bne	.L_08001ec0
.L_08001f74:
	pop	{r5, r6, r7, r8, r9}
	bx	lr

	.section .text,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global syColBufferScaleNonzeroThreeQuarters
	.thumb_func
syColBufferScaleNonzeroThreeQuarters:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #96
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	bl	_call_via_sp
	add	sp, #96
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_ScaleNonzeroThreeQuartersKernelCode
	.4byte	0x84000018
	.global syColBufferHalveNonzero
	.thumb_func
syColBufferHalveNonzero:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #96
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	bl	_call_via_sp
	add	sp, #96
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_HalveNonzeroKernelCode
	.4byte	0x84000018
	.global ColorBuffer_BrightenPartial
	.thumb_func
ColorBuffer_BrightenPartial:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #216
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	bl	_call_via_sp
	add	sp, #216
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_BrightenPartialKernelCode
	.4byte	0x84000036
	.global ColorBuffer_DarkenPartial
	.thumb_func
ColorBuffer_DarkenPartial:
	push	{r5, r6, r7, lr}
	add	r4, pc, #16
	ldmia	r4, {r4, r5, r7}
	sub	sp, #212
	mov	r6, sp
	stmia	r4!, {r5, r6, r7}
	bl	_call_via_sp
	add	sp, #212
	pop	{r5, r6, r7, pc}
	.4byte	0x040000d4
	.4byte	ColorBuffer_DarkenPartialKernelCode
	.4byte	0x84000035

	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
	.global syDecompressHalfwordLZ
syDecompressHalfwordLZ:
	.global Resource_DecodeHalfwordLz
	.thumb_func
Resource_DecodeHalfwordLz:
	bx	pc
AlchemyUncredited_08001ffe:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001ffe:
	mov	r3, #0x80000000
	b	.L_0800200c
.L_08002008:
	strh	r2, [r1], #2
.L_0800200c:
	lsls	r3, r3, #1
.L_08002010:
	ldrh	r2, [r0], #2
	bcc	.L_08002008
	bne	.L_08002028
	adc	r3, r2, r2
	lsls	r3, r3, #16
	b	.L_08002010
.L_08002028:
	cmp	r2, #0
	bxeq	lr
	sub	ip, r1, r2, lsr #5
	sub	ip, ip, r2, lsr #5
	and	r2, r2, #31
	add	r2, r2, #2
.L_08002040:
	ldrh	r4, [ip], #2
	strh	r4, [r1], #2
	subs	r2, r2, #1
	bne	.L_08002040
	b	.L_0800200c
	.global syDecompressHalfwordsCodeSize
	.set syDecompressHalfwordsCodeSize, . - syDecompressHalfwordLZ
