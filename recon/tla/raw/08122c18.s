.syntax unified
	.thumb
	.set sub_08002074, 0x08002074
	.global Func_08122c18
	.thumb_func
Func_08122c18:
	push	{lr}
	movs	r1, #0
	movs	r0, #5
	bl	sub_08002074
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r2, #132
	adds	r3, #212
	movs	r0, #0
	movs	r1, #0
	lsls	r2, r2, #24
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	movs	r0, #0
	lsls	r2, r2, #24
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	movs	r0, #0
	lsls	r2, r2, #24
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{pc}
	.2byte 0x0000
