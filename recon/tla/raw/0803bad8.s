.syntax unified
	.thumb
	.global Func_0803bad8
	.thumb_func
Func_0803bad8:
	ldr r3, .L_0803bb08
	strh r3, [r7, #6]
	ldr r3, [sp, #16]
	mov r2, r9
	str r4, [r3]
	ldr r3, [sp, #12]
	str r2, [r3]
	adds r3, r4, #0
	adds r3, #19
	lsrs r4, r3, #3
	lsls r3, r4, #3
	adds r4, r3, #0
	subs r4, #16
	cmp r7, #0
	beq .L_0803bb44
	movs r6, #0
	movs r5, #0
.L_0803bafa:
	mov r2, r10
	ldrh r3, [r5, r2]
	cmp r3, #1
	bhi .L_0803bb0e
	ldr r3, .L_0803bb08
	strh r3, [r7]
	b .L_0803bb0c
.L_0803bb08:
	.4byte 0x00000000
.L_0803bb0c:
	b .L_0803bb3a
.L_0803bb0e:
	ldr r2, [sp, #8]
	ldrh r3, [r5, r2]
	subs r0, r4, r3
	subs r0, #4
	cmp r0, #0
	bge .L_0803bb1c
	movs r0, #0
.L_0803bb1c:
	mov r3, r10
	ldrh r1, [r5, r3]
	lsls r0, r0, #8
	subs r1, #1
	str r4, [sp, #0]
	bl Math_Div
	movs r2, #192
	lsls r2, r2, #4
	ldr r4, [sp, #0]
	cmp r0, r2
	bls .L_0803bb38
	movs r0, #128
	lsls r0, r0, #2
.L_0803bb38:
	strh r0, [r7]
.L_0803bb3a:
	adds r7, #2
	adds r6, #1
	adds r5, #2
	cmp r6, r11
	bls .L_0803bafa
.L_0803bb44:
	bl UiWork_ResetCounters
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
