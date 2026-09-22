.syntax unified
	.thumb
	.set sub_080f93a4, 0x080f93a4
	.global Overlay_080fbe48
Overlay_080fbe48:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #4
	adds	r3, #240
	ldr	r0, [r3, #0]
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #13
	movs	r2, #0
	movs	r3, #17
	bl	sub_080f93a4
	add	sp, #4
	pop	{pc}
