.syntax unified
	.thumb
	.global Func_0802d394
	.thumb_func
Func_0802d394:
	push {lr}
	cmp r1, #7
	bhi .L_0802d3a2
	ldrb r0, [r0]
	lsls r0, r0, #24
	asrs r0, r0, #24
	b .L_0802d3a8
.L_0802d3a2:
	ldrb r0, [r0, #1]
	lsls r0, r0, #24
	asrs r0, r0, #24
.L_0802d3a8:
	lsls r0, r0, #19
	pop {pc}
