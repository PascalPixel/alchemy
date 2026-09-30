.syntax unified
	.thumb
	.global Func_0802d438
	.thumb_func
Func_0802d438:
	push {lr}
	cmp r2, #7
	bhi .L_0802d446
	ldrb r0, [r0]
	lsls r0, r0, #24
	asrs r0, r0, #24
	b .L_0802d458
.L_0802d446:
	cmp r1, #7
	bhi .L_0802d452
	ldrb r0, [r0, #1]
	lsls r0, r0, #24
	asrs r0, r0, #24
	b .L_0802d458
.L_0802d452:
	ldrb r0, [r0, #2]
	lsls r0, r0, #24
	asrs r0, r0, #24
.L_0802d458:
	lsls r0, r0, #19
	pop {pc}
