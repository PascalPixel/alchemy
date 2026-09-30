.syntax unified
	.thumb
	.global Func_0802d644
	.thumb_func
Func_0802d644:
	push {lr}
	adds r3, r0, #0
	adds r4, r1, #0
	adds r0, r2, #0
	adds r1, r3, #0
	adds r2, r4, #0
	bl Func_0802d45c
	asrs r0, r0, #19
	pop {pc}
