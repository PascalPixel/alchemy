.syntax unified
	.thumb
	.global Func_08192828
	.thumb_func
Func_08192828:
	push {lr}
	asrs r2, r2, #1
	lsls r3, r3, #24
	sub sp, #4
	lsrs r4, r3, #24
	adds r0, r0, r2
	subs r1, r1, r2
	cmp r2, #48
	ble .L_0819283c
	movs r2, #48
.L_0819283c:
	adds r3, r1, r2
	subs r2, r0, r2
	str r4, [sp, #0]
	bl Func_08143eb4
	add sp, #4
	pop {pc}
	.2byte 0x0000
