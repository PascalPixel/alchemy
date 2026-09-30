.syntax unified
	.thumb
	.global Func_080d3070
	.thumb_func
Func_080d3070:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	mov	sl, r3
	bl	ObjectTable_Get
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080d30e6
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #102
	adds	r1, #204
	adds	r0, r6, #0
	bl	ObjectMotion_SetSpeedParameters
	ldr	r3, [pc, #80]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r1, [r3, #0]
	adds	r0, r6, #0
	bl	Func_080d3214
	ldr	r3, [r5, #16]
	ldr	r0, [pc, #68]
	ldr	r1, [r5, #8]
	adds	r3, r3, r0
	ldr	r2, [r5, #12]
	adds	r0, r5, #0
	bl	Func_080200e8
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #91
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl	Object_SetMode
	mov	r2, r8
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	ObjectMotion_OffsetPositionAndResetMotion
	ldr	r1, [pc, #32]
	adds	r0, r5, #0
	bl	Object_SetCallback
	adds	r3, r5, #0
	adds	r3, #100
	mov	r2, sl
	strh	r2, [r3, #0]
.L_080d30e6:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0x080f32e0
