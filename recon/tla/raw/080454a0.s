.syntax unified
	.thumb
	.global Func_080454a0
	.thumb_func
Func_080454a0:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	mov	r8, r1
	movs	r1, #193
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #68
	bl	0x08014d00
	movs	r1, #26
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	0x0803d680
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	adds	r1, r5, #0
	mov	r0, r8
	bl	Func_080143f8
	adds	r5, r0, #0
	movs	r0, #68
	bl	Runtime_ReleaseSlot
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.global Resource_LoadIndexedEntryToBuffer
	.thumb_func
Resource_LoadIndexedEntryToBuffer:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r6, r1, #0
	movs	r1, #193
	mov	r8, r0
	lsls	r1, r1, #3
	movs	r0, #68
	sub	sp, #12
	bl	0x08014d00
	movs	r1, #1
	add	r2, sp, #8
	add	r3, sp, #4
	str	r1, [sp, #0]
	adds	r5, r0, #0
	movs	r1, #0
	mov	r0, r8
	str	r6, [sp, #8]
	bl	0x0803d98c
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	Func_080143f8
	adds	r5, r0, #0
	movs	r0, #68
	bl	Runtime_ReleaseSlot
	adds	r0, r5, #0
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
