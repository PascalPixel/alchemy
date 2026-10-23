@ C2: Decompress_ByteLzArm:
@ bcs/bcc consume lsls lr's carry. The decoder dispatch also enters
@ eight-byte code slots, unlike the compiler's four-byte switch entries.
@ Its return pops the r1 saved at entry into r0.
@ C2: Oam_BuildBufferKernel:
@ unshifted movs r6, r6/r5/r4/r3 copy and test; ldmdb r2! also loads
@ a four-register list outside the block-copy patterns.
@ C2: PaletteFade_ScaleThreeQuartersKernel to PaletteFade_HalveKernel:
@ both kernels save callee-saved r5/r6 without lr, and write modified
@ words back with stmia r0!, {r3-r6}, beyond their initial block copy.
@ C2: PaletteFade_DarkenKernel to PaletteFade_BackupKernelsEnd:
@ stmia r0!, {r4-r7} writes arithmetic-modified words, not a block copy.
@ C2: PaletteFade_BrightenKernel to PaletteFade_HalveNonzeroKernel:
@ all three kernels save callee-saved r5/r6 (and further registers in
@ BrightenKernel) without saving lr.
@ C2: PaletteFade_DarkenPartialKernel:
@ ldm r0!, {r4-r7} and stmia r2!, {r4-r7} write back through a
@ four-register list starting at r4. GCC 2.95.2's block-copy expander
@ fixes that list at r0-r3; its general ldm/stm peepholes never write back.
@ C2: PaletteFade_BrightenPartialKernel to PaletteFade_KernelsEnd:
@ push {r5-r9} saves callee-saved registers without lr.
@ C2: Decompress_HalfwordLz:
@ bcc and adc consume lsls r3's carry; the intervening ldrh keeps flags.
.syntax unified
	.arm
	.global Decompress_ByteLzArm
Decompress_ByteLzArm:
	push	{r1, r5, r6, r7, lr}
.Lbyte_flags:
	ldrb	lr, [r0], #1
	lsls	lr, lr, #24
	bne	.Lbyte_mixed
	ands	r2, r0, #3
	ldmeq	r0!, {r4, r5}
	beq	.Lbyte_literals_8
	ldm	r0, {r4, r5, r6}
	lsl	r2, r2, #3
	rsb	r7, r2, #32
	lsr	r4, r4, r2
	orr	r4, r4, r5, lsl r7
	lsr	r5, r5, r2
	orr	r5, r5, r6, lsl r7
	add	r0, r0, #8
.Lbyte_literals_8:
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
	b	.Lbyte_flags
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
	b	.Lbyte_flags
.Lbyte_mixed:
	orr	lr, lr, #0x800000
	lsls	lr, lr, #1
	bcs	.Lbyte_match
.Lbyte_literal:
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	lsls	lr, lr, #1
	bcc	.Lbyte_literal
	beq	.Lbyte_flags
.Lbyte_match:
	ldrb	ip, [r0], #1
	ldrb	r3, [r0], #1
	and	r2, ip, #240
	rsb	r3, r3, r1
	sub	r4, r3, r2, lsl #4
	ands	ip, ip, #15
	beq	.Lbyte_match_long
	rsb	ip, ip, #15
	add	pc, pc, ip, lsl #3
AlchemyUncredited_decode_byte_lz_1:
	nop
AlchemyUncreditedEnd_decode_byte_lz_1:
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
	bcc	.Lbyte_literal
	beq	.Lbyte_flags
	b	.Lbyte_match
.Lbyte_match_long:
	cmp	r4, r1
	beq	.Lbyte_done
	ldrb	r3, [r0], #1
	add	r3, r3, #16
	and	ip, r3, #15
	rsb	ip, ip, #15
	add	pc, pc, ip, lsl #3
AlchemyUncredited_decode_byte_lz_2:
	nop
.Lbyte_match_block:
AlchemyUncreditedEnd_decode_byte_lz_2:
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
	bge	.Lbyte_match_block
	lsls	lr, lr, #1
	bcc	.Lbyte_literal
	beq	.Lbyte_flags
	b	.Lbyte_match
.Lbyte_done:
	pop	{r0, r5, r6, r7, ip}
	sub	r0, r1, r0
	bx	ip
	.global Decompress_ByteLzCodeSize
	.set Decompress_ByteLzCodeSize, . - Decompress_ByteLzArm
.syntax unified
	.arm
	.global Oam_BuildBufferKernel
Oam_BuildBufferKernel:
	push	{r5, r6, r7, r8, r9, lr}
	ldr	r2, [pc, #196]
	mov	ip, #64
	mov	r1, #128
	mov	r9, r0
.Loam_bucket:
	ldmdb	r2!, {r3, r4, r5, r6}
	movs	r6, r6
	blne	.Loam_chain
	movs	r6, r5
	blne	.Loam_chain
	movs	r6, r4
	blne	.Loam_chain
	movs	r6, r3
	blne	.Loam_chain
	subs	ip, ip, #1
	bne	.Loam_bucket
	mov	r2, #192
	orr	r2, r2, #0xc00000
	lsl	r1, r1, #3
	ldr	r4, [pc, #128]
	add	lr, pc, #52
	bx	r4
.Loam_chain:
	ldm	r6, {r6, r7, r8}
	stmia	r0!, {r7, r8}
	subs	r1, r1, #1
	beq	.Loam_affine
	cmp	r6, #0
	bxeq	lr
	ldm	r6, {r6, r7, r8}
	stmia	r0!, {r7, r8}
	subs	r1, r1, #1
	beq	.Loam_affine
	cmp	r6, #0
	bxeq	lr
	b	.Loam_chain
.Loam_affine:
	add	r0, r9, #6
	ldr	r2, [pc, #64]
	ldrb	r4, [r2]
	ldr	r1, [pc, #60]
	cmp	r4, #0
	beq	.Loam_done
.Loam_affine_next:
	ldm	r1!, {r2, r3}
	strh	r2, [r0], #32
	lsr	r2, r2, #16
	strh	r2, [r0, #-24]
	strh	r3, [r0, #-16]
	lsr	r3, r3, #16
	strh	r3, [r0, #-8]
	subs	r4, r4, #1
	bne	.Loam_affine_next
.Loam_done:
	pop	{r5, r6, r7, r8, r9, lr}
	bx	lr
	.4byte	gFrameTick
	.4byte	IwramFillWords
	.4byte	gOamMatrixCount
	.4byte	gOamMatrices
	.global LoadedRuntime_Size
	.set LoadedRuntime_Size, . - Oam_BuildBufferKernel
.syntax unified
	.arm
	.global PaletteFade_ScaleThreeQuartersKernel
PaletteFade_ScaleThreeQuartersKernel:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.Lthree_quarters_next:
	ldm	r0, {r3, r4, r5, r6}
	stmia	r1!, {r3, r4, r5, r6}
	adds	r3, r3, r3, lsl #1
	andne	r3, ip, r3, lsr #2
	adds	r4, r4, r4, lsl #1
	andne	r4, ip, r4, lsr #2
	adds	r5, r5, r5, lsl #1
	andne	r5, ip, r5, lsr #2
	adds	r6, r6, r6, lsl #1
	andne	r6, ip, r6, lsr #2
	stmia	r0!, {r3, r4, r5, r6}
	cmp	r0, r2
	bne	.Lthree_quarters_next
	pop	{r5, r6}
	bx	lr
	.global PaletteFade_HalveKernel
PaletteFade_HalveKernel:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.Lhalve_next:
	ldm	r0, {r3, r4, r5, r6}
	stmia	r1!, {r3, r4, r5, r6}
	and	r3, ip, r3, lsr #1
	and	r4, ip, r4, lsr #1
	and	r5, ip, r5, lsr #1
	and	r6, ip, r6, lsr #1
	stmia	r0!, {r3, r4, r5, r6}
	cmp	r0, r2
	bne	.Lhalve_next
	pop	{r5, r6}
	bx	lr
.L_halve_end:
	.global PaletteFade_BackupAndScaleThreeQuartersCodeSize
	.set PaletteFade_BackupAndScaleThreeQuartersCodeSize, PaletteFade_HalveKernel - PaletteFade_ScaleThreeQuartersKernel
	.global PaletteFade_BackupAndHalveCodeSize
	.set PaletteFade_BackupAndHalveCodeSize, .L_halve_end - PaletteFade_HalveKernel
.syntax unified
	.arm
	.global PaletteFade_DarkenKernel
PaletteFade_DarkenKernel:
	rsb	ip, r1, #0x80000000
	add	ip, ip, #0x800000
	add	ip, ip, #0x8000
	add	ip, ip, #128
	mov	r1, #0x100
	add	r1, r1, #1
	orr	r1, r1, r1, lsl #16
	push	{r5, r6, r7, r8, lr}
	add	lr, r0, r3
.Ldarken_next:
	ldm	r0, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	add	r8, r4, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r4, r8, r3
	add	r8, r5, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r5, r8, r3
	add	r8, r6, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r6, r8, r3
	add	r8, r7, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r7, r8, r3
	stmia	r0!, {r4, r5, r6, r7}
	cmp	r0, lr
	bne	.Ldarken_next
	pop	{r5, r6, r7, r8, ip}
	bx	ip
PaletteFade_BackupKernelsEnd:
	.global PaletteFade_BackupAndDarkenCodeSize
	.set PaletteFade_BackupAndDarkenCodeSize, PaletteFade_BackupKernelsEnd - PaletteFade_DarkenKernel
.syntax unified
	.arm
	.global PaletteFade_BrightenKernel
PaletteFade_BrightenKernel:
	push	{r5, r6, r7, r8, r9}
	mov	r8, #64
	orr	r8, r8, r8, lsl #8
	orr	r8, r8, r8, lsl #16
	sub	ip, r8, r8, lsr #6
	add	r3, r0, r3
.Lbrighten_next:
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
	bne	.Lbrighten_next
	pop	{r5, r6, r7, r8, r9}
	bx	lr
	.global PaletteFade_ScaleNonzeroThreeQuartersKernel
PaletteFade_ScaleNonzeroThreeQuartersKernel:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.Lnonzero_three_quarters_next:
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
	bne	.Lnonzero_three_quarters_next
	pop	{r5, r6}
	bx	lr
	.global PaletteFade_HalveNonzeroKernel
PaletteFade_HalveNonzeroKernel:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.Lnonzero_halve_next:
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
	bne	.Lnonzero_halve_next
	pop	{r5, r6}
	bx	lr
.L_halve_nonzero_end:
	.global PaletteFade_BackupAndBrightenCodeSize
	.set PaletteFade_BackupAndBrightenCodeSize, PaletteFade_ScaleNonzeroThreeQuartersKernel - PaletteFade_BrightenKernel
	.global PaletteFade_BackupAndScaleNonzeroThreeQuartersCodeSize
	.set PaletteFade_BackupAndScaleNonzeroThreeQuartersCodeSize, PaletteFade_HalveNonzeroKernel - PaletteFade_ScaleNonzeroThreeQuartersKernel
	.global PaletteFade_BackupAndHalveNonzeroCodeSize
	.set PaletteFade_BackupAndHalveNonzeroCodeSize, .L_halve_nonzero_end - PaletteFade_HalveNonzeroKernel
.syntax unified
	.arm
	.global PaletteFade_DarkenPartialKernel
PaletteFade_DarkenPartialKernel:
	rsb	ip, r1, #0x80000000
	add	ip, ip, #0x800000
	add	ip, ip, #0x8000
	add	ip, ip, #128
	mov	r1, #0x100
	add	r1, r1, #1
	orr	r1, r1, r1, lsl #16
	push	{r5, r6, r7, r8, lr}
	add	lr, r0, r3
.Ldarken_partial_next:
	ldm	r0!, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	cmp	r4, #0
	bne	.Ldarken_word0
	cmp	r5, #0
	bne	.Ldarken_word1
	cmp	r6, #0
	bne	.Ldarken_word2
	cmp	r7, #0
	bne	.Ldarken_word3
	cmp	r0, lr
	bne	.Ldarken_partial_next
	b	.Ldarken_partial_done
.Ldarken_word0:
	add	r8, r4, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-16]
	cmp	r5, #0
	beq	.Ldarken_check2
.Ldarken_word1:
	add	r8, r5, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-12]
.Ldarken_check2:
	cmp	r6, #0
	beq	.Ldarken_check3
.Ldarken_word2:
	add	r8, r6, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-8]
.Ldarken_check3:
	cmp	r7, #0
	beq	.Ldarken_partial_loop
.Ldarken_word3:
	add	r8, r7, ip
	and	r3, r1, r8, lsr #7
	rsb	r3, r3, r3, lsl #7
	and	r3, r8, r3
	strne	r3, [r0, #-4]
.Ldarken_partial_loop:
	cmp	r0, lr
	bne	.Ldarken_partial_next
.Ldarken_partial_done:
	pop	{r5, r6, r7, r8, ip}
	bx	ip
.L_darken_partial_end:
	.global PaletteFade_BackupAndDarkenPartialCodeSize
	.set PaletteFade_BackupAndDarkenPartialCodeSize, .L_darken_partial_end - PaletteFade_DarkenPartialKernel
.syntax unified
	.arm
	.global PaletteFade_BrightenPartialKernel
PaletteFade_BrightenPartialKernel:
	push	{r5, r6, r7, r8, r9}
	mov	r8, #64
	orr	r8, r8, r8, lsl #8
	orr	r8, r8, r8, lsl #16
	sub	ip, r8, r8, lsr #6
	add	r3, r0, r3
.Lbrighten_partial_next:
	ldm	r0!, {r4, r5, r6, r7}
	stmia	r2!, {r4, r5, r6, r7}
	cmp	r4, ip
	bne	.Lbrighten_word0
	cmp	r5, ip
	bne	.Lbrighten_word1
	cmp	r6, ip
	bne	.Lbrighten_word2
	cmp	r7, ip
	bne	.Lbrighten_word3
	cmp	r0, r3
	bne	.Lbrighten_partial_next
	b	.Lbrighten_partial_done
.Lbrighten_word0:
	add	r4, r4, r1
	and	r9, r4, r8
	bic	r4, r4, r9
	sub	r9, r9, r9, lsr #6
	orr	r4, r4, r9
	str	r4, [r0, #-16]
	cmp	r5, ip
	beq	.Lbrighten_check2
.Lbrighten_word1:
	add	r5, r5, r1
	and	r9, r5, r8
	bic	r5, r5, r9
	sub	r9, r9, r9, lsr #6
	orr	r5, r5, r9
	str	r5, [r0, #-12]
.Lbrighten_check2:
	cmp	r6, ip
	beq	.Lbrighten_check3
.Lbrighten_word2:
	add	r6, r6, r1
	and	r9, r6, r8
	bic	r6, r6, r9
	sub	r9, r9, r9, lsr #6
	orr	r6, r6, r9
	str	r6, [r0, #-8]
.Lbrighten_check3:
	cmp	r7, ip
	beq	.Lbrighten_partial_loop
.Lbrighten_word3:
	add	r7, r7, r1
	and	r9, r7, r8
	bic	r7, r7, r9
	sub	r9, r9, r9, lsr #6
	orr	r7, r7, r9
	str	r7, [r0, #-4]
.Lbrighten_partial_loop:
	cmp	r0, r3
	bne	.Lbrighten_partial_next
.Lbrighten_partial_done:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
PaletteFade_KernelsEnd:
	.global PaletteFade_BackupAndBrightenPartialCodeSize
	.set PaletteFade_BackupAndBrightenPartialCodeSize, PaletteFade_KernelsEnd - PaletteFade_BrightenPartialKernel
.syntax unified
	.arm
	.global Decompress_HalfwordLz
Decompress_HalfwordLz:
	mov	r3, #0x80000000
	b	.Lhalfword_next_flag
.Lhalfword_literal:
	strh	r2, [r1], #2
.Lhalfword_next_flag:
	lsls	r3, r3, #1
.Lhalfword_read:
	ldrh	r2, [r0], #2
	bcc	.Lhalfword_literal
	bne	.Lhalfword_match
	adc	r3, r2, r2
	lsls	r3, r3, #16
	b	.Lhalfword_read
.Lhalfword_match:
	cmp	r2, #0
	bxeq	lr
	sub	ip, r1, r2, lsr #5
	sub	ip, ip, r2, lsr #5
	and	r2, r2, #31
	add	r2, r2, #2
.Lhalfword_copy:
	ldrh	r4, [ip], #2
	strh	r4, [r1], #2
	subs	r2, r2, #1
	bne	.Lhalfword_copy
	b	.Lhalfword_next_flag
	.global Decompress_HalfwordsCodeSize
	.set Decompress_HalfwordsCodeSize, . - Decompress_HalfwordLz
