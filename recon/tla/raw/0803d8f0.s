.syntax unified
	.thumb
	.global Func_0803d8f0
	.thumb_func
Func_0803d8f0:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r1, #193
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #68
	mov	r8, r2
	bl	Func_08014cc0
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	Ui_PrepareTransferFromTableEntry
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	movs	r1, #128
	adds	r2, r5, #0
	mov	r0, r8
	bl	VramBlock_LoadCached
	movs	r0, #68
	bl	Runtime_ReleaseHeapBlock
	movs	r0, #1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
