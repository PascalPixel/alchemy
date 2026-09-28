.syntax unified
	.thumb
	.global SaveState_ProcessSelectedSlot
	.global Func_0801faa8
	.thumb_func
SaveState_ProcessSelectedSlot:
Func_0801faa8:
	push	{r5, r6, r7, lr}
	movs	r0, #128
	lsls	r0, r0, #5
	bl	Runtime_BumpAllocateAlternatePool
	ldr	r6, [pc, #124]
	adds	r5, r0, #0
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	movs	r3, #1
	negs	r3, r3
	movs	r7, #0
	cmp	r0, r3
	beq.n	.L_0801fb28
	bl	SaveState_InitializeWorkspace
	cmp	r0, #0
	beq.n	.L_0801fad8
	ldr	r0, [pc, #100]
	movs	r1, #1
	movs	r7, #9
	bl	UiText_ShowPositionedMessageAndWait
	b.n	.L_0801fb1a
.L_0801fad8:
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	adds	r1, r5, #0
	bl	SaveState_ReadRecordPayload
	cmp	r0, #0
	beq.n	.L_0801faf2
	ldr	r0, [pc, #80]
	movs	r1, #1
	bl	UiText_ShowPositionedMessageAndWait
	movs	r7, #2
	negs	r7, r7
.L_0801faf2:
	ldr	r1, [pc, #72]
	ldr	r3, [pc, #72]
	adds	r0, r5, r1
	subs	r0, r0, r3
	movs	r2, #16
	ldr	r3, [pc, #68]
	bl	_call_via_r3
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	adds	r1, r5, #0
	bl	SaveState_WriteRecord
	cmp	r0, #0
	beq.n	.L_0801fb1c
	ldr	r0, [pc, #36]
	movs	r1, #1
	bl	UiText_ShowPositionedMessageAndWait
	movs	r7, #3
.L_0801fb1a:
	negs	r7, r7
.L_0801fb1c:
	bl	SaveState_ReleaseWorkspace
	adds	r0, r5, #0
	bl	Party_Do
	adds	r0, r7, #0
.L_0801fb28:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02002004
	.4byte 0x0000000a
	.4byte 0x0000000b
	.4byte 0x020004e4
	.4byte 0x02000000
	.4byte 0x03001388
