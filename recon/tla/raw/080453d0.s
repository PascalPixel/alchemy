.syntax unified
	.thumb
	.global Func_080453d0
	.thumb_func
Func_080453d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #0
	mov r9, r0
	mov r10, r2
	mov r8, r2
	mov r11, r2
.L_080453e8:
	movs r3, #0
	mov lr, r3
	mov r3, r11
	add r3, r10
	lsls r7, r3, #6
.L_080453f2:
	mov r2, r9
	adds r6, r2, r7
	mov r2, lr
	lsls r3, r2, #5
	ldr r2, .L_0804545c
	adds r5, r3, r2
	movs r3, #0
	mov r12, r3
.L_08045402:
	ldrh r1, [r5]
	movs r0, #0
	adds r5, #2
	movs r4, #0
.L_0804540a:
	adds r3, r1, #0
	movs r2, #15
	ands r3, r2
	add r3, r8
	lsls r2, r3, #1
	ldr r3, .L_08045460
	lsrs r1, r1, #4
	ldrh r2, [r3, r2]
	lsls r3, r4, #2
	lsls r2, r3
	adds r4, #1
	orrs r0, r2
	cmp r4, #3
	ble .L_0804540a
	movs r2, #1
	add r12, r2
	mov r3, r12
	strh r0, [r6]
	adds r6, #2
	cmp r3, #15
	ble .L_08045402
	add lr, r2
	mov r2, lr
	adds r7, #32
	cmp r2, #9
	ble .L_080453f2
	movs r3, #16
	add r8, r3
	movs r3, #1
	movs r2, #4
	add r10, r3
	add r11, r2
	mov r2, r10
	cmp r2, #1
	ble .L_080453e8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0804545c:
	.4byte 0x06000600
.L_08045460:
	.4byte Data_0805f770
