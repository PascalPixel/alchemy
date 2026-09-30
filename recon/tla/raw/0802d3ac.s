.syntax unified
	.thumb
	.global Func_0802d3ac
	.thumb_func
Func_0802d3ac:
	push {lr}
	cmp r2, #7
	bhi .L_0802d3ba
	ldrb r0, [r0]
	lsls r0, r0, #24
	asrs r0, r0, #24
	b .L_0802d3c0
.L_0802d3ba:
	ldrb r0, [r0, #1]
	lsls r0, r0, #24
	asrs r0, r0, #24
.L_0802d3c0:
	lsls r0, r0, #19
	pop {pc}
