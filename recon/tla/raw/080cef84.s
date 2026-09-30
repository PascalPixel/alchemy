.syntax unified
	.thumb
	.global Func_080cef84
	.thumb_func
Func_080cef84:
	push	{r5, lr}
	bl	ObjectTable_Get
	adds	r5, r0, #0
	movs	r0, #18
	bl	0x08013560
	adds	r0, r5, #0
	movs	r1, #7
	bl	Object_SetMode
	movs	r0, #146
	bl	Audio_PlayCue
	cmp	r5, #0
	beq.n	.L_080cefb2
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #40]
	adds	r0, r5, #0
	movs	r1, #1
	bl	ObjectDispatch_SetSingleChildField26Far
.L_080cefb2:
	pop	{r5, pc}
	push	{lr}
	bl	ObjectTable_Get
	movs	r1, #4
	bl	Object_SetMode
	movs	r0, #124
	bl	Audio_PlayCue
	movs	r0, #12
	bl	0x08013560
	pop	{pc}
	.2byte 0x0000
