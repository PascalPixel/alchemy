.syntax unified
	.thumb
	.global Func_0802d414
	.thumb_func
Func_0802d414:
	push {lr}
	cmp r2, #7
	bhi .L_0802d42e
	cmp r1, #7
	bhi .L_0802d426
	ldrb r0, [r0]
	lsls r0, r0, #24
	asrs r0, r0, #24
	b .L_0802d434
.L_0802d426:
	ldrb r0, [r0, #1]
	lsls r0, r0, #24
	asrs r0, r0, #24
	b .L_0802d434
.L_0802d42e:
	ldrb r0, [r0, #2]
	lsls r0, r0, #24
	asrs r0, r0, #24
.L_0802d434:
	lsls r0, r0, #19
	pop {pc}
