.syntax unified
	.thumb
	.global LiteralPool_0809b7e4
	.global Func_0809b7e4
	.thumb_func
LiteralPool_0809b7e4:
Func_0809b7e4:
	subs	r0, r6, #4
	lsls	r0, r0, #12
	lsls	r2, r3, #28
	movs	r0, r0
	stmia	r5!, {r4}
	lsrs	r1, r1, #32
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	lsls	r5, r0, #5
	movs	r0, r0
	push	{r0, r2, r3, r4, r6, r7, lr}
	lsrs	r1, r1, #32
	push	{r0, r3, r7, lr}
	lsrs	r1, r1, #32
	lsrs	r2, r4, #4
	.2byte 0x0000
