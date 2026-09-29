.syntax unified
	.thumb
	.global UiTextResource_Release
	.thumb_func
UiTextResource_Release:
	push	{lr}
	bl	0x08014274
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #72]
	adds	r0, r5, #0
	bl	0x0803e8ec
	adds	r6, r0, #0
	ldrh	r3, [r6, #10]
	cmp	r3, #1
	beq.n	.L_0803f6e6
	cmp	r3, #6
	bne.n	.L_0803f74c
.L_0803f6e6:
	movs	r1, #193
	lsls	r1, r1, #3
	movs	r0, #68
	bl	Func_08014cc0
	movs	r3, #195
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldrh	r3, [r6, #8]
	adds	r7, r0, #0
	ldr	r0, [pc, #88]
	mov	r8, r3
	bl	Resource_GetTableEntry
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	adds	r2, r7, r3
	ldrh	r3, [r6, #8]
	adds	r1, r7, #0
	lsls	r3, r3, #1
	ldrh	r3, [r3, r0]
	adds	r0, r0, r3
	str	r0, [r2, #0]
	bl	0x0801591c
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	bne.n	.L_0803f726
	bl	Resource_FindFreeEntry
	strh	r0, [r5, #12]
.L_0803f726:
	movs	r1, #128
	ldrh	r0, [r5, #12]
	lsls	r1, r1, #3
	adds	r2, r7, #0
	bl	0x080142d4
	movs	r3, #1
	strh	r3, [r5, #10]
	mov	r3, r8
	strh	r3, [r5, #8]
	movs	r3, #40
	strh	r3, [r5, #34]
	strh	r3, [r5, #36]
	movs	r3, #240
	strh	r0, [r5, #14]
	strh	r3, [r5, #38]
	movs	r0, #68
	bl	Runtime_ReleaseSlot
.L_0803f74c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x01d7
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #72]
	movs	r2, #195
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	beq.n	.L_0803f776
	ldrh	r0, [r5, #12]
	bl	0x08014274
	movs	r3, #0
	strh	r3, [r5, #10]
.L_0803f776:
	pop	{r5, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	movs	r5, #0
.L_0803f782:
	bl	.L_0803f800
	adds	r0, r5, #0
	bl	Menu_SelectTopEntry
	adds	r5, r0, #0
	bl	.L_0803f810
	cmp	r5, #4
	bhi.n	.L_0803f7fa
	ldr	r2, [pc, #100]
	lsls	r3, r5, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0803f7b4
	.4byte 0x0803f7c8
	.4byte 0x0803f7d6
	.4byte 0x0803f7e0
	.2byte 0xf7ee
	.2byte 0x0803
	bl	0x080c82b0
	cmp	r0, #0
	bne.n	.L_0803f7be
	movs	r0, #255
.L_0803f7be:
	movs	r2, #177
	lsls	r2, r2, #1
	adds	r3, r6, r2
	strh	r0, [r3, #0]
	b.n	.L_0803f7fa
	bl	0x080f8008
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_0803f7fa
	b.n	.L_0803f782
	bl	0x080f8030
	cmp	r0, #0
	beq.n	.L_0803f7fa
	b.n	.L_0803f782
	bl	Func_080f8000
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_0803f7fa
	b.n	.L_0803f782
	bl	0x080f8010
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_0803f782
.L_0803f7fa:
	pop	{r5, r6, pc}
	.2byte 0xf7a0
	.2byte 0x0803
.L_0803f800:
	push	{lr}
	movs	r0, #1
	bl	Func_08042690
	movs	r0, #1
	bl	0x08013560
	pop	{pc}
.L_0803f810:
	push	{lr}
	bl	0x08042dac
	pop	{pc}
	push	{lr}
	bl	0x0803dc1c
	bl	0x0803dd24
	movs	r0, #1
	bl	0x08013560
	pop	{pc}
	.2byte 0x0000
