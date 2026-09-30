.syntax unified
	.thumb
	.global Func_081ac0b0
	.thumb_func
Func_081ac0b0:
	push {lr}
	lsls r0, r0, #16
	lsls r1, r1, #16
	asrs r1, r1, #16
	asrs r0, r0, #16
	muls r0, r1
	adds r3, r0, #0
	cmp r0, #0
	bge .L_081ac0c4
	adds r3, #255
.L_081ac0c4:
	lsls r0, r3, #8
	asrs r0, r0, #16
	pop {pc}
	.2byte 0x0000
