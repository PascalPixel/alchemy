.syntax unified
	.thumb
	.global SceneTransform_ApplyPitch
	.global Func_08004bd4
	.thumb_func
SceneTransform_ApplyPitch:
Func_08004bd4:
	push	{r5, r6, lr}
	sub	sp, #48
	adds	r5, r0, #0
	bl	Trig_Sin
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Trig_Cos
	mov	ip, r0
	mov	r5, sp
	adds	r0, r5, #0
	movs	r1, #128
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	lsls	r1, r1, #9
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	mov	r3, ip
	str	r6, [r5, #20]
	negs	r6, r6
	str	r3, [r5, #16]
	str	r3, [r5, #32]
	str	r6, [r5, #28]
	ldr	r3, [pc, #12]
	adds	r0, r5, #0
	bl	_call_via_r3
	add	sp, #48
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x030002c0
