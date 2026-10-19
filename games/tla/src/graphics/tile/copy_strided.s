.syntax unified
	.thumb
	.global Render_CopyStridedCode
Render_CopyStridedCode:
	.global Render_CopyStrided
	.thumb_func
Render_CopyStrided:
	bx	pc
AlchemyUncredited_080217a6:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080217a6:
	bics	r1, r1, #7
	bicsne	r2, r2, #7
	bxeq	lr
	push	{r5, r6, r7, r8, r9, sl, fp}
.L_080217b8:
	mov	ip, r1
.L_080217bc:
	ldm	r0, {r4, r5}
	add	r0, r0, r1
	ldm	r0, {r6, r7}
	add	r0, r0, r1
	ldm	r0, {r8, r9}
	add	r0, r0, r1
	ldm	r0, {sl, fp}
	add	r0, r0, r1
	stmia	r3!, {r4, r5, r6, r7, r8, r9, sl, fp}
	ldm	r0, {r4, r5}
	add	r0, r0, r1
	ldm	r0, {r6, r7}
	add	r0, r0, r1
	ldm	r0, {r8, r9}
	add	r0, r0, r1
	ldm	r0!, {sl, fp}
	add	r0, r0, r1
	stmia	r3!, {r4, r5, r6, r7, r8, r9, sl, fp}
	subs	ip, ip, #8
	subgt	r0, r0, r1, lsl #3
	bgt	.L_080217bc
	sub	r0, r0, r1
	subs	r2, r2, #8
	bgt	.L_080217b8
	pop	{r5, r6, r7, r8, r9, sl, fp}
	bx	lr
	.global Render_CopyStridedCodeSize
	.set Render_CopyStridedCodeSize, . - Render_CopyStridedCode
