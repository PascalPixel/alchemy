@ C2: fp is a product high half rather than a frame pointer; stmia writes
@ arithmetic-transformed vectors, not compiler block-copy data.
.syntax unified
	.arm

	.global IwramTransformVector
IwramTransformVector:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldm	r0, {r2, r3, r4}
	add	r0, pc, #0xf0
	ldmia	r0!, {r5, r6, r7}
	smull	r8, r9, r2, r5
	smull	sl, fp, r2, r6
	smull	ip, lr, r2, r7
	ldmia	r0!, {r5, r6, r7}
	smlal	r8, r9, r3, r5
	smlal	sl, fp, r3, r6
	smlal	ip, lr, r3, r7
	ldmia	r0!, {r5, r6, r7}
	smlal	r8, r9, r4, r5
	smlal	sl, fp, r4, r6
	smlal	ip, lr, r4, r7
	lsr	r8, r8, #16
	orr	r8, r8, r9, lsl #16
	lsr	sl, sl, #16
	orr	sl, sl, fp, lsl #16
	lsr	ip, ip, #16
	orr	ip, ip, lr, lsl #16
	ldmia	r0!, {r5, r6, r7}
	add	r5, r5, r8
	add	r6, r6, sl
	add	r7, r7, ip
	stmia	r1!, {r5, r6, r7}
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr

	.global IwramTransformMatrix
IwramTransformMatrix:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	sub	sp, sp, #0x24
	mov	r4, sp
.L_matrix_row:
	add	r1, pc, #0x7c
	ldmia	r0!, {r2, r3}
	ldmia	r1!, {r5, r6, r7}
	smull	r8, r9, r5, r2
	smull	sl, fp, r6, r2
	smull	ip, lr, r7, r2
	ldmia	r1!, {r5, r6, r7}
	smlal	r8, r9, r5, r3
	smlal	sl, fp, r6, r3
	smlal	ip, lr, r7, r3
	ldmia	r1!, {r5, r6, r7}
	ldr	r2, [r0], #4
	smlal	r8, r9, r5, r2
	smlal	sl, fp, r6, r2
	smlal	ip, lr, r7, r2
	lsr	r8, r8, #16
	orr	r8, r8, r9, lsl #16
	lsr	sl, sl, #16
	orr	sl, sl, fp, lsl #16
	lsr	ip, ip, #16
	orr	ip, ip, lr, lsl #16
	sub	r2, r4, sp
	cmp	r2, #0x24
	stmiane	r4!, {r8, sl, ip}
	bne	.L_matrix_row
	ldmia	r1!, {r5, r6, r7}
	add	ip, ip, r7
	add	fp, sl, r6
	add	sl, r8, r5
	pop	{r0, r2, r3, r4, r5, r6, r7, r8, r9}
	stmdb	r1, {r0, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
