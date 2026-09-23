.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08004278, 0x08004278
	.set sub_080cdbc0, 0x080cdbc0
	.global Unnamed_080df864
	.global Func_080df864
	.thumb_func
Unnamed_080df864:
Func_080df864:
	ldr	r0, [pc, #76]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #156
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.2byte 0xec3d
	.2byte 0x080e
	asrs	r2, r1, #10
	movs	r0, r0
	.2byte 0xec2f
	.2byte 0x080e
	subs	r4, r1, #4
	lsls	r0, r0, #12
	.2byte 0xec36
	.2byte 0x080e
	ldrb	r0, [r5, #0]
	movs	r0, r0
	lsrs	r0, r6, #11
	lsls	r1, r0, #8
	udf	#72
	lsrs	r6, r1, #32
	subs	r0, #0
	lsls	r1, r0, #8
	.2byte 0xc000
	.2byte 0xffff
	.2byte 0x7824
	movs	r0, r0
	.4byte 0x080cd261
