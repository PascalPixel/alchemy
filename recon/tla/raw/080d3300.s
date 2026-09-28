.syntax unified
	.thumb
	.global ObjectMotion_SetVariantCallback
	.global Func_080d3300
	.thumb_func
ObjectMotion_SetVariantCallback:
Func_080d3300:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	ObjectTable_Get
	cmp	r0, #0
	beq.n	.L_080d3324
	cmp	r5, #0
	ble.n	.L_080d3324
	cmp	r5, #3
	ble.n	.L_080d3316
	movs	r5, #3
.L_080d3316:
	movs	r1, #3
	ldr	r3, [pc, #12]
	subs	r1, r1, r5
	lsls	r1, r1, #7
	adds	r1, r1, r3
	bl	Object_SetCallback
.L_080d3324:
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x080f06d8
