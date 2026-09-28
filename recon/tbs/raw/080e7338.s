@ Exact nested functions of BattleEffect_RunParticleStreams, kept as the compiler's own
@ assembly while the parent remains a draft (recon/tbs/en/main/080e7404.c).
	.code	16
.text
	.align	2, 0
	.thumb_func
	.type	 ParticleStream_AddPrimary.0,function
ParticleStream_AddPrimary.0:
	push	{r5, r6, r7, lr}
	mov	r7, r9
	push	{r7}
	sub	sp, sp, #4
	mov	r4, r9
	mov	r3, sp
	str	r4, [r3]
	mov	r3, r4
	mov	r5, r1
	mov	r1, r3
	sub	r1, r1, #136
	ldr	r3, [r1]
	mov	r7, #232
	lsl	r7, r7, #7
	mov	r6, r2
	add	r2, r3, r7
	ldr	r3, [r2, #24]
	mov	r7, #1
	neg	r7, r7
	mov	r4, #0
	cmp	r3, r7
	bne	.L7
	str	r4, [r2, #24]
	b	.L11
.L7:
	add	r4, r4, #1
	cmp	r4, #16
	beq	.L6
	lsl	r3, r4, #3
	sub	r3, r3, r4
	ldr	r2, [r1]
	lsl	r3, r3, #2
	add	r2, r2, r3
	mov	r3, #232
	lsl	r3, r3, #7
	add	r2, r2, r3
	mov	r7, #1
	ldr	r3, [r2, #24]
	neg	r7, r7
	cmp	r3, r7
	bne	.L7
	mov	r3, #0
	str	r3, [r2, #24]
.L11:
	str	r0, [r2]
	str	r5, [r2, #4]
	str	r6, [r2, #12]
.L6:
	add	sp, sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
.Lfe1:
	.size	 ParticleStream_AddPrimary.0,.Lfe1-ParticleStream_AddPrimary.0
	.align	2, 0
	.thumb_func
	.type	 ParticleStream_AddSecondary.1,function
ParticleStream_AddSecondary.1:
	push	{r5, r6, lr}
	mov	r6, r9
	push	{r6}
	sub	sp, sp, #4
	mov	r2, r9
	mov	r3, sp
	str	r2, [r3]
	mov	r3, r2
	mov	r5, r1
	mov	r1, r3
	sub	r1, r1, #136
	ldr	r3, [r1]
	mov	r6, #225
	lsl	r6, r6, #7
	add	r2, r3, r6
	ldr	r3, [r2, #24]
	mov	r6, #1
	neg	r6, r6
	mov	r4, #0
	cmp	r3, r6
	bne	.L15
	str	r4, [r2, #24]
	b	.L19
.L15:
	add	r4, r4, #1
	cmp	r4, #32
	beq	.L14
	lsl	r3, r4, #3
	sub	r3, r3, r4
	ldr	r2, [r1]
	lsl	r3, r3, #2
	add	r2, r2, r3
	mov	r3, #225
	lsl	r3, r3, #7
	add	r2, r2, r3
	mov	r6, #1
	ldr	r3, [r2, #24]
	neg	r6, r6
	cmp	r3, r6
	bne	.L15
	mov	r3, #0
	str	r3, [r2, #24]
.L19:
	str	r0, [r2]
	str	r5, [r2, #4]
.L14:
	add	sp, sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
.Lfe2:
	.size	 ParticleStream_AddSecondary.1,.Lfe2-ParticleStream_AddSecondary.1
	.align	2, 0
