.syntax unified
	.arm

	.global IwramCopyWords
IwramCopyWords:
	push	{r5, r6, r7, r8, r9}
	ands	ip, r2, #224, 0
	rsb	ip, ip, #240, 0
	add	pc, pc, ip, lsr #2
.L_audio_1398:
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	ldm	r1!, {r3, r4, r5, r6, r7, r8, r9, ip}
	stmia	r0!, {r3, r4, r5, r6, r7, r8, r9, ip}
	subs	r2, r2, #256
	bpl	.L_audio_1398
	ands	r2, r2, #28, 0
	beq	.L_audio_13f8
.L_audio_13e8:
	ldm	r1!, {r3}
	stmia	r0!, {r3}
	subs	r2, r2, #4, 0
	bgt	.L_audio_13e8
.L_audio_13f8:
	pop	{r5, r6, r7, r8, r9}
	bx	lr
