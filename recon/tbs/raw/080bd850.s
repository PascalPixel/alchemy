@ Exact nested functions of BattleEvent_Playback, kept as the compiler's own
@ assembly while the parent remains a draft (recon/tbs/en/main/080bd898.c).
	.code	16
.text
	.align	2, 0
	.thumb_func
	.type	 BattleEvent_ClearRecordTiles.0,function
BattleEvent_ClearRecordTiles.0:
	push	{lr}
	mov	ip, r3
	mov	r3, r9
	push	{r3}
	mov	r3, ip
	sub	sp, sp, #4
	mov	r3, r9
	str	r3, [sp]
	ldrb	r3, [r0, #28]
	ldr	r2, .L4
	lsl	r3, r3, #2
	add	r3, r3, r2
	ldrh	r2, [r3, #2]
	ldr	r3, .L4+4
	add	r2, r2, r3
	mov	r3, r0
	add	r3, r3, #32
	add	r0, r0, #33
	ldrb	r1, [r3]
	ldrb	r3, [r0]
	mov	r0, r2
	mul	r1, r1, r3
	ldr	r3, .L4+8
	bl	_call_via_r3
	add	sp, sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r0}
	bx	r0
.L5:
	.align	2, 0
.L4:
	.word	gVramBlockCache
	.word	100728832
	.word	50332004
.Lfe1:
	.size	 BattleEvent_ClearRecordTiles.0,.Lfe1-BattleEvent_ClearRecordTiles.0
