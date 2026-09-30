.syntax unified
	.thumb
	.global Func_0818caa8
	.thumb_func
Func_0818caa8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r0, [sp, #12]
	str r2, [sp, #8]
	adds r6, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	mov r8, r3
	ldr r3, [r3, #96]
	adds r7, r1, #0
	movs r0, #188
	movs r1, #19
	mov r10, r3
	bl Func_081963ec
	lsrs r5, r6, #31
	adds r5, r6, r5
	asrs r5, r5, #1
	movs r3, #188
	add r8, r3
	subs r3, r7, r5
	mov r9, r3
	ldr r3, [sp, #8]
	str r5, [sp, #0]
	str r6, [sp, #4]
	subs r3, r3, r6
	mov r11, r3
	mov r3, r8
	ldr r1, [sp, #12]
	ldr r4, [r3]
	mov r2, r9
	mov r3, r11
	mov r0, r10
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #23
	movs r0, #188
	bl Func_081963ec
	str r5, [sp, #0]
	str r6, [sp, #4]
	mov r3, r8
	ldr r1, [sp, #12]
	ldr r4, [r3]
	adds r2, r7, #0
	mov r3, r11
	mov r0, r10
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #27
	movs r0, #188
	bl Func_081963ec
	str r5, [sp, #0]
	str r6, [sp, #4]
	mov r3, r8
	ldr r1, [sp, #12]
	ldr r4, [r3]
	mov r2, r9
	ldr r3, [sp, #8]
	mov r0, r10
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r1, #31
	movs r0, #188
	bl Func_081963ec
	str r5, [sp, #0]
	str r6, [sp, #4]
	mov r3, r8
	ldr r4, [r3]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	adds r2, r7, #0
	mov r0, r10
	mov lr, r4
	.2byte 0xf800
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
