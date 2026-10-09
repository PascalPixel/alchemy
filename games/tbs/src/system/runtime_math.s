@ C2: push saves r5-r9 without lr and computed add pc enters the unrolled store
@ body.
.syntax unified
	.arm

	.global IwramClearWords
IwramClearWords:
	mov	r2, #0
	.global IwramFillWords
IwramFillWords:
	push	{r5, r6, r7, r8, r9}
	mov	r3, r2
	mov	r4, r2
	mov	r5, r2
	mov	r6, r2
	mov	r7, r2
	mov	r8, r2
	mov	r9, r2
	ands	ip, r1, #0xe0
	rsb	ip, ip, #0xe0
	add	pc, pc, ip, lsr #3
.L_clear_256:
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	subs	r1, r1, #0x100
	bpl	.L_clear_256
	ands	r1, r1, #0x1c
	beq	.L_clear_done
.L_clear_word:
	stmia	r0!, {r2}
	subs	r1, r1, #4
	bgt	.L_clear_word
.L_clear_done:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
