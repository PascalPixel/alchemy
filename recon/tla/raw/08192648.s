.syntax unified
	.thumb
	.global Func_08192648
	.thumb_func
Func_08192648:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	str r1, [sp, #24]
	adds r6, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #104]
	ldr r1, [r3, #92]
	mov r10, r2
	ldr r2, [r3, #96]
	adds r3, #188
	str r4, [sp, #16]
	ldr r4, [r3]
	mov r11, r1
	mov r9, r2
	str r4, [sp, #20]
	cmp r6, #0
	bne .L_081926a6
	lsrs r5, r0, #1
	lsls r1, r5, #1
	adds r1, r1, r5
	lsls r1, r1, #3
	adds r1, r1, r5
	movs r2, #136
	lsls r1, r1, #5
	lsls r2, r2, #6
	adds r2, #18
	add r1, r11
	adds r1, r1, r2
	ldr r2, [sp, #24]
	movs r0, #20
	mov r3, r10
	str r0, [sp, #0]
	movs r0, #40
	str r0, [sp, #4]
	subs r2, #10
	subs r3, #20
	mov r0, r9
	mov lr, r4
	.2byte 0xf800
	b .L_081926ce
.L_081926a6:
	lsrs r5, r0, #1
	lsls r1, r5, #3
	adds r1, r1, r5
	movs r3, #166
	lsls r1, r1, #5
	lsls r3, r3, #7
	ldr r2, [sp, #24]
	adds r3, #18
	movs r0, #12
	add r1, r11
	adds r1, r1, r3
	str r0, [sp, #0]
	mov r3, r10
	movs r0, #24
	str r0, [sp, #4]
	subs r2, #6
	subs r3, #12
	mov r0, r9
	mov lr, r4
	.2byte 0xf800
.L_081926ce:
	lsls r2, r6, #1
	movs r3, #8
	subs r7, r3, r2
	movs r4, #1
	lsrs r6, r7, #1
	mov r1, r10
	mov r8, r4
	adds r4, r1, r6
	cmp r4, #112
	bgt .L_0819272a
	subs r2, r7, #1
	lsls r3, r7, #1
	str r2, [sp, #8]
	str r3, [sp, #12]
.L_081926ea:
	ldr r1, [sp, #8]
	ldr r2, .L_08192738
	lsls r3, r1, #1
	ldrh r1, [r2, r3]
	lsls r3, r5, #1
	adds r3, r3, r5
	lsls r3, r3, #7
	adds r3, r3, r5
	lsls r3, r3, #1
	adds r1, r1, r3
	ldr r3, [sp, #24]
	movs r2, #224
	lsls r2, r2, #3
	add r1, r11
	adds r1, r1, r2
	subs r2, r3, r6
	ldr r3, [sp, #12]
	str r7, [sp, #0]
	str r3, [sp, #4]
	mov r0, r9
	adds r3, r4, #0
	ldr r4, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	movs r1, #1
	add r8, r1
	mov r3, r8
	muls r3, r6
	mov r2, r10
	adds r4, r2, r3
	cmp r4, #112
	ble .L_081926ea
.L_0819272a:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08192738:
	.4byte Data_08197410
