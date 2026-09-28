@ Exact nested functions of BattlePresentation_AppendLinkedActions, kept as the compiler's own
@ assembly while the parent remains a draft (recon/tbs/en/main/080b9724.c).
	.code	16
.text
	.align	2, 0
	.thumb_func
	.type	 BattleLink_SendActions.0,function
BattleLink_SendActions.0:
	push	{r5, r6, r7, lr}
	mov	r7, r9
	push	{r7}
	sub	sp, sp, #4
	mov	r3, sp
	mov	r2, r9
	str	r2, [r3]
	mov	r7, r2
	sub	r3, r7, #4
	ldr	r0, [r3]
	mov	r1, #20
	bl	SerialRuntime_BeginTransferA
	mov	r3, #1
	mov	r5, #150
	neg	r3, r3
	mov	r6, #0
	lsl	r5, r5, #1
	cmp	r0, r3
	bne	.L5
	b	.L3
.L7:
	mov	r0, #1
	sub	r5, r5, #1
	bl	WaitFrames
	cmp	r5, #0
	blt	.L25
	ldr	r3, .L26
	ldrh	r2, [r3]
	mov	r3, #3
	and	r3, r3, r2
	cmp	r3, #3
	beq	.L9
	add	r6, r6, #1
	cmp	r6, #24
	ble	.L5
	b	.L25
.L9:
	mov	r6, #0
.L5:
	bl	SerialRuntime_GetActiveTransfers
	cmp	r0, #0
	bne	.L7
	mov	r3, r7
	sub	r3, r3, #8
	ldr	r1, [r3]
	cmp	r1, #0
	beq	.L13
	sub	r3, r3, #4
	ldr	r0, [r3]
	bl	SerialRuntime_BeginTransferA
	mov	r2, #1
	neg	r2, r2
	cmp	r0, r2
	bne	.L15
	b	.L3
.L17:
	mov	r0, #1
	sub	r5, r5, #1
	bl	WaitFrames
	cmp	r5, #0
	blt	.L25
	ldr	r3, .L26
	ldrh	r2, [r3]
	mov	r3, #3
	and	r3, r3, r2
	cmp	r3, #3
	beq	.L19
	add	r6, r6, #1
	cmp	r6, #24
	ble	.L15
.L25:
	mov	r0, #1
	neg	r0, r0
	b	.L3
.L19:
	mov	r6, #0
.L15:
	bl	SerialRuntime_GetActiveTransfers
	cmp	r0, #0
	bne	.L17
.L13:
	mov	r0, #0
.L3:
	add	sp, sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
.L27:
	.align	2, 0
.L26:
	.word	gLinkStatus
.Lfe1:
	.size	 BattleLink_SendActions.0,.Lfe1-BattleLink_SendActions.0
	.align	2, 0
	.thumb_func
	.type	 BattleLink_ReceiveActions.1,function
BattleLink_ReceiveActions.1:
	push	{r5, r6, r7, lr}
	mov	r7, r9
	mov	r6, r8
	push	{r6, r7}
	mov	r1, r9
	sub	sp, sp, #4
	mov	r8, r1
	mov	r3, sp
	mov	r7, r8
	str	r1, [r3]
	sub	r7, r7, #4
	ldr	r0, [r7]
	bl	SerialRuntime_BeginTransferB
	mov	r2, #1
	mov	r5, #150
	neg	r2, r2
	mov	r6, #0
	lsl	r5, r5, #1
	cmp	r0, r2
	bne	.L30
	b	.L28
.L32:
	ldr	r3, .L56
	ldrh	r3, [r3]
	cmp	r3, #20
	bhi	.L55
	mov	r0, #1
	sub	r5, r5, #1
	bl	WaitFrames
	cmp	r5, #0
	blt	.L55
	ldr	r3, .L56+4
	ldrh	r2, [r3]
	mov	r3, #3
	and	r3, r3, r2
	cmp	r3, #3
	beq	.L35
	add	r6, r6, #1
	cmp	r6, #24
	ble	.L30
	b	.L55
.L35:
	mov	r6, #0
.L30:
	bl	SerialRuntime_GetActiveTransfers
	cmp	r0, #0
	bne	.L32
	ldr	r3, .L56
	ldrh	r3, [r3]
	cmp	r3, #20
	bne	.L55
	mov	r3, #16
	neg	r3, r3
	add	r3, r3, r8
	mov	r9, r3
	ldr	r3, [r7]
	ldr	r2, [r3]
	mov	r1, r9
	str	r2, [r1]
	ldr	r3, [r3]
	cmp	r3, #0
	beq	.L40
	mov	r3, r8
	mov	r2, r8
	sub	r3, r3, #20
	sub	r2, r2, #12
	ldr	r3, [r3]
	ldr	r0, [r2]
	lsl	r3, r3, #4
	add	r0, r0, r3
	bl	SerialRuntime_BeginTransferB
	mov	r2, #1
	neg	r2, r2
	cmp	r0, r2
	bne	.L42
	b	.L28
.L44:
	ldr	r3, .L56
	ldrh	r3, [r3]
	mov	r8, r3
	mov	r3, r9
	ldr	r0, [r3]
	lsl	r0, r0, #4
	add	r0, r0, #19
	mov	r1, #20
	bl	Math_DivU
	lsl	r3, r0, #2
	add	r3, r3, r0
	lsl	r3, r3, #2
	cmp	r8, r3
	bhi	.L54
	mov	r0, #1
	sub	r5, r5, #1
	bl	WaitFrames
	cmp	r5, #0
	blt	.L55
	ldr	r3, .L56+4
	ldrh	r2, [r3]
	mov	r3, #3
	and	r3, r3, r2
	cmp	r3, #3
	beq	.L47
	add	r6, r6, #1
	cmp	r6, #24
	ble	.L42
	b	.L55
.L47:
	mov	r6, #0
.L42:
	bl	SerialRuntime_GetActiveTransfers
	cmp	r0, #0
	bne	.L44
	mov	r1, r9
	ldr	r0, [r1]
	ldr	r3, .L56
	lsl	r0, r0, #4
	ldrh	r3, [r3]
	add	r0, r0, #19
	mov	r1, #20
	mov	r8, r3
	bl	Math_DivU
	lsl	r3, r0, #2
	add	r3, r3, r0
	lsl	r3, r3, #2
	cmp	r8, r3
	beq	.L40
.L54:
.L55:
	mov	r0, #1
	neg	r0, r0
	b	.L28
.L40:
	mov	r0, #0
.L28:
	add	sp, sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	r9, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
.L57:
	.align	2, 0
.L56:
	.word	gSerialReceivedSize
	.word	gLinkStatus
.Lfe2:
	.size	 BattleLink_ReceiveActions.1,.Lfe2-BattleLink_ReceiveActions.1
