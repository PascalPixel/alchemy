.syntax unified
	.thumb
	.global Func_080e2a68
	.thumb_func
Func_080e2a68:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	adds r6, r1, #0
	mov r9, r2
	mov r10, r3
	cmp r0, #31
	ble .L_080e2a82
	movs r1, #31
	mov r8, r1
.L_080e2a82:
	movs r7, #0
.L_080e2a84:
	adds r0, r6, #0
	bl Trig_Cos
	mov r5, r8
	muls r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	asrs r3, r3, #16
	asrs r5, r5, #16
	adds r3, #32
	adds r5, #32
	lsls r3, r3, #6
	adds r3, r3, r5
	lsrs r2, r7, #31
	mov r1, r9
	lsls r3, r3, #1
	ldrh r3, [r3, r1]
	adds r2, r7, r2
	asrs r2, r2, #1
	adds r2, #225
	mov r1, r10
	strb r2, [r1, r3]
	movs r3, #184
	lsls r3, r3, #1
	adds r7, #1
	adds r6, r6, r3
	cmp r7, #29
	ble .L_080e2a84
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
