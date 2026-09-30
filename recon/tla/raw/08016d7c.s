.syntax unified
	.thumb
	.global Func_08016d7c
	.thumb_func
Func_08016d7c:
	push {lr}
	ldr r1, .L_08016d94
	lsls r3, r0, #20
	lsrs r0, r3, #23
	ldrb r2, [r1, r0]
	adds r3, r2, #0
	cmp r3, #254
	bhi .L_08016d90
	adds r3, r2, #1
	strb r3, [r1, r0]
.L_08016d90:
	ldrb r0, [r1, r0]
	pop {pc}
.L_08016d94:
	.4byte GameFlagBytes
