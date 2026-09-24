.syntax unified
	.thumb
	.global Func_08004a94
	.thumb_func
Func_08004a94:
	ldr	r3, [pc, #20]
	adds	r0, r3, #0
	movs	r1, #128
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	lsls	r1, r1, #9
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	bx	lr
	movs	r0, r0
	.2byte 0x0350
	.2byte 0x0300
