@ Three colour-buffer kernels that save a backup copy while they fade a buffer:
@ scale every colour byte by 3/4, halve it, or darken it by a bias. Each loader
@ (0x080054e4, 0x08005534, 0x08005490) copies exactly its kernel onto the stack
@ and calls it there. They are ARM-state code: the first two save r5 and r6
@ without lr, and the darkening kernel pops its return address into ip and
@ returns with bx ip. GCC saves lr with any register and returns through lr.
@ No TLA edition carries these bytes; TLA has its own revisions (TLA
@ GRAPHICS/COLOR_BACKUP_KERNELS.S).
@ credit: handwritten — ARM colour-buffer fade kernels with backup, copied to the stack
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08001ea8
	.global ColorBuffer_ScaleThreeQuartersKernel
ColorBuffer_ScaleThreeQuartersKernel:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001ebc:
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
	bne	.L_08001ebc
	pop	{r5, r6}
	bx	lr
	.global ColorBuffer_HalveKernel
ColorBuffer_HalveKernel:
	push	{r5, r6}
	add	r2, r2, r0
	mov	ip, #0x3f00
	add	ip, ip, #63
	orr	ip, ip, ip, lsl #16
.L_08001f0c:
	ldm	r0, {r3, r4, r5, r6}
	stmia	r1!, {r3, r4, r5, r6}
	and	r3, ip, r3, lsr #1
	and	r4, ip, r4, lsr #1
	and	r5, ip, r5, lsr #1
	and	r6, ip, r6, lsr #1
	stmia	r0!, {r3, r4, r5, r6}
	cmp	r0, r2
	bne	.L_08001f0c
	pop	{r5, r6}
	bx	lr
	.global ColorBuffer_DarkenKernel
ColorBuffer_DarkenKernel:
	rsb	ip, r1, #0x80000000
	add	ip, ip, #0x800000
	add	ip, ip, #0x8000
	add	ip, ip, #128
	mov	r1, #0x100
	add	r1, r1, #1
	orr	r1, r1, r1, lsl #16
	push	{r5, r6, r7, r8, lr}
	add	lr, r0, r3
.L_08001f5c:
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
	bne	.L_08001f5c
	pop	{r5, r6, r7, r8, ip}
	bx	ip
