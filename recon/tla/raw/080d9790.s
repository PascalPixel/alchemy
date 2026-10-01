.syntax unified
	.thumb
	.global Func_080d9790
	.thumb_func
Func_080d9790:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	str r2, [sp, #28]
	str r3, [sp, #24]
	str r1, [sp, #32]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #156
	ldr r3, [r3]
	lsls r0, r0, #5
	mov r10, r3
	add r0, r10
	adds r5, r0, #0
	movs r2, #0
	adds r5, #12
	mov r8, r2
	movs r2, #10
	ldrsh r3, [r5, r2]
	ldr r7, [r5, #28]
	cmp r3, #0
	bne .L_080d97c8
	b .L_080d995e
.L_080d97c8:
	mov r3, r8
	strh r3, [r5, #10]
	movs r3, #4
	ldrsh r2, [r5, r3]
	movs r0, #128
	lsls r0, r0, #2
	str r2, [sp, #20]
	bl Runtime_BumpAllocate
	str r0, [sp, #4]
	adds r6, r0, #0
	movs r3, #8
	ldrsh r0, [r5, r3]
	mov r2, r8
	str r2, [r5, #24]
	cmp r0, #0
	beq .L_080d97ee
	bl GameFlag_ClearBit
.L_080d97ee:
	ldr r2, [sp, #20]
	cmp r2, #0
	blt .L_080d9808
	adds r2, #1
	mov r8, r2
.L_080d97f8:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	stmia r6!, {r7}
	ldr r7, [r7]
	cmp r2, #0
	bne .L_080d97f8
.L_080d9808:
	mov r3, r10
	ldr r0, [r3]
	movs r1, #3
	bl Object_SetMode
	mov r2, r10
	ldr r0, [r2, #4]
	movs r1, #3
	bl Object_SetMode
	movs r0, #4
	bl WaitFrames
	ldr r2, [sp, #20]
	movs r3, #0
	str r3, [sp, #16]
	cmp r3, r2
	ble .L_080d982e
	b .L_080d9956
.L_080d982e:
	ldr r3, [sp, #16]
	movs r2, #0
	lsls r3, r3, #12
	mov r10, r3
	ldr r3, [sp, #20]
	movs r5, #128
	mov r8, r2
	lsls r5, r5, #11
	cmp r8, r3
	bgt .L_080d9942
	ldr r2, .L_080d996c
	mov r11, r2
.L_080d9846:
	mov r2, r8
	lsls r3, r2, #2
	ldr r2, [sp, #4]
	ldr r7, [r3, r2]
	movs r3, #0
	strb r3, [r7, #19]
	ldr r3, [sp, #16]
	cmp r8, r3
	bgt .L_080d98a0
	mov r0, r10
	bl Trig_Cos
	adds r1, r5, #0
	mov lr, r11
	.2byte 0xf800
	ldr r2, [sp, #32]
	adds r0, r2, r0
	str r0, [r7, #4]
	ldr r3, [sp, #28]
	str r0, [sp, #12]
	str r3, [r7, #8]
	mov r0, r10
	str r3, [sp, #8]
	bl Trig_Sin
	adds r1, r5, #0
	mov lr, r11
	.2byte 0xf800
	ldr r2, [sp, #24]
	movs r3, #2
	adds r0, r2, r0
	str r0, [r7, #12]
	mov r9, r0
	movs r0, #192
	adds r1, r5, #0
	strb r3, [r7, #18]
	lsls r0, r0, #11
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r0, r0, #16
	add r10, r0
	lsls r0, r0, #2
	adds r5, r5, r0
	b .L_080d9938
.L_080d98a0:
	ldr r3, [sp, #12]
	ldr r1, [r7, #4]
	ldr r2, [sp, #8]
	subs r1, r3, r1
	ldr r5, [r7, #8]
	ldr r6, [r7, #12]
	asrs r1, r1, #8
	mov r3, r9
	adds r0, r1, #0
	subs r5, r2, r5
	subs r6, r3, r6
	mov lr, r11
	.2byte 0xf800
	asrs r5, r5, #8
	adds r3, r0, #0
	str r3, [sp, #0]
	adds r1, r5, #0
	adds r0, r5, #0
	mov lr, r11
	.2byte 0xf800
	asrs r6, r6, #8
	adds r5, r0, #0
	adds r1, r6, #0
	adds r0, r6, #0
	mov lr, r11
	.2byte 0xf800
	ldr r3, [sp, #0]
	ldr r2, .L_080d9970
	adds r3, r3, r5
	adds r3, r3, r0
	adds r0, r3, #0
	mov lr, r2
	.2byte 0xf800
	adds r5, r0, #0
	cmp r5, #6
	ble .L_080d9942
	ldr r2, [sp, #12]
	ldr r3, [r7, #4]
	adds r1, r5, #0
	subs r3, r3, r2
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #1
	bl __divsi3
	ldr r3, [sp, #12]
	adds r1, r5, #0
	adds r0, r3, r0
	str r0, [r7, #4]
	str r0, [sp, #12]
	ldr r2, [sp, #8]
	ldr r3, [r7, #8]
	subs r3, r3, r2
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #1
	bl __divsi3
	ldr r3, [sp, #8]
	mov r2, r9
	adds r0, r3, r0
	str r0, [r7, #8]
	str r0, [sp, #8]
	ldr r3, [r7, #12]
	adds r1, r5, #0
	subs r3, r3, r2
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #1
	bl __divsi3
	movs r3, #2
	add r0, r9
	str r0, [r7, #12]
	strb r3, [r7, #18]
	mov r9, r0
.L_080d9938:
	ldr r2, [sp, #20]
	movs r3, #1
	add r8, r3
	cmp r8, r2
	ble .L_080d9846
.L_080d9942:
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	adds r3, #1
	str r3, [sp, #16]
	cmp r3, r2
	bgt .L_080d9956
	b .L_080d982e
.L_080d9956:
	ldr r0, [sp, #4]
	bl Sys_Free
	movs r0, #0
.L_080d995e:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d996c:
	.4byte IwramMulQ16
.L_080d9970:
	.4byte IwramFillWords + 0x74
	.4byte 0x00004770
