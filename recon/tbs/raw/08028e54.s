.syntax unified
	.thumb
	.global	Menu_RunConfirmSelectionAt
	.thumb_func
Menu_RunConfirmSelectionAt:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #72]
	adds	r7, r2, #0
	adds	r6, r1, #0
	mov	r8, r3
	adds	r5, r0, #0
	bl	AffineEffect_InitializeWork
	movs	r0, #5
	bl	Menu_AppendResourceEntry
	movs	r0, #6
	bl	Menu_AppendResourceEntry
	mov	r3, r8
	adds	r1, r6, #0
	movs	r2, #3
	adds	r0, r5, #0
	bl	Menu_LayoutResourceEntries
	adds	r0, r7, #0
	bl	Menu_RunResourceSelectionLoop
	adds	r7, r0, #0
	bl	Menu_EndResourceSelection
	movs	r3, #1
	negs	r3, r3
	cmp	r7, r3
	bne	.L0_0
	movs	r7, #1
.L0_0:
	adds	r0, r7, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte	0x0000
	.4byte	0x00000024
