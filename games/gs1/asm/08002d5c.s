@ 転送済みサム分岐の相対位置補正処理。転送後のアーム領域で実行する。
@ 呼出側の転送元と転送長により境界を確定した再構築コード。
.syntax unified
	.arm
	.global Func_08002d5c
Func_08002d5c:
	push	{r5, r6}
	add	r1, r1, r1
	add	r2, r0, r1
	add	r0, r0, #2
	mov	r6, r0
	mov	r4, #31
	mov	r5, #30
.L_08002d78:
	cmp	r0, r2
	bcs	.L_08002dd0
	ldrh	ip, [r0], #2
	cmp	r4, ip, lsr #11
	bne	.L_08002d78
	ldrh	r3, [r0, #-4]
	cmp	r5, r3, lsr #11
	bne	.L_08002d78
	bic	ip, ip, #63488
	bic	r3, r3, #63488
	orr	ip, ip, r3, lsl #11
	lsl	ip, ip, #1
	sub	r3, r0, r6
	sub	ip, ip, r3
	lsr	r3, ip, #12
	bic	r3, r3, #63488
	orr	r3, r3, #61440
	lsr	ip, ip, #1
	orr	ip, ip, #63488
	strh	r3, [r0, #-4]
	strh	ip, [r0, #-2]
	b	.L_08002d78
.L_08002dd0:
	pop	{r5, r6}
	bx	lr
