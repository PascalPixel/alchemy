.syntax unified
	.thumb
	.set sub_08022768, 0x08022768
	.set sub_080b5090, 0x080b5090
	.global BattleLayout_HighlightPartyPanels
	.global Func_0802281c
	.thumb_func
BattleLayout_HighlightPartyPanels:
Func_0802281c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #148]
	adds	r5, r0, #0
	movs	r1, #0
	movs	r0, #1
	sub	sp, #4
	ldr	r7, [r3, #0]
	bl	sub_080b5090
	mov	r8, r0
	lsls	r3, r0, #1
	add	r3, r8
	lsls	r3, r3, #1
	movs	r0, #29
	subs	r0, r0, r3
	movs	r3, #15
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r3, #5
	movs	r2, #25
	bl	sub_08022768
	ldrh	r3, [r5, #0]
	movs	r6, #0
	cmp	r3, #255
	beq.n	.L_080228a8
	movs	r0, #0
.L_08022856:
	movs	r3, #88
	ldrsh	r2, [r7, r3]
	ldrh	r3, [r0, r5]
	movs	r1, #0
	b.n	.L_0802286e
.L_08022860:
	adds	r1, #1
	cmp	r1, #3
	bgt.n	.L_08022878
	lsls	r3, r1, #1
	adds	r3, #88
	ldrsh	r2, [r7, r3]
	ldrh	r3, [r0, r5]
.L_0802286e:
	cmp	r2, r3
	beq.n	.L_08022878
	cmp	r2, #255
	bne.n	.L_08022860
	movs	r1, #4
.L_08022878:
	cmp	r1, #4
	beq.n	.L_08022898
	mov	r3, r8
	subs	r2, r3, r1
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	movs	r0, #29
	subs	r0, r0, r3
	movs	r3, #14
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #7
	movs	r3, #5
	bl	sub_08022768
.L_08022898:
	adds	r6, #1
	cmp	r6, #3
	bgt.n	.L_080228a8
	lsls	r3, r6, #1
	adds	r0, r3, #0
	ldrh	r3, [r0, r5]
	cmp	r3, #255
	bne.n	.L_08022856
.L_080228a8:
	movs	r0, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
