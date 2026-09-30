.syntax unified
	.thumb
	.global Func_080da6e0
	.thumb_func
Func_080da6e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #44
	str r1, [sp, #40]
	str r2, [sp, #36]
	str r3, [sp, #32]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #160
	ldr r3, [r3]
	ldr r2, [sp, #76]
	str r3, [sp, #28]
	movs r3, #0
	mov r9, r3
	ldr r3, [sp, #40]
	ldr r1, [sp, #80]
	subs r2, r2, r3
	ldr r3, [sp, #36]
	mov r10, r0
	subs r1, r1, r3
	ldr r0, [sp, #84]
	asrs r4, r1, #8
	ldr r1, [sp, #32]
	asrs r2, r2, #8
	subs r0, r0, r1
	str r4, [sp, #0]
	adds r1, r2, #0
	ldr r7, .L_080da900
	asrs r6, r0, #8
	adds r0, r2, #0
	mov lr, r7
	.2byte 0xf800
	ldr r4, [sp, #0]
	adds r5, r0, #0
	adds r1, r4, #0
	adds r0, r4, #0
	mov lr, r7
	.2byte 0xf800
	adds r1, r6, #0
	mov r8, r0
	adds r0, r6, #0
	mov lr, r7
	.2byte 0xf800
	add r5, r8
	adds r5, r5, r0
	ldr r3, .L_080da904
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	movs r1, #6
	bl __divsi3
	mov r2, r10
	lsls r5, r2, #3
	subs r5, r5, r2
	adds r0, #1
	str r0, [sp, #24]
	ldr r1, [sp, #28]
	lsls r5, r5, #2
	adds r3, r5, #0
	movs r0, #128
	adds r3, #36
	lsls r0, r0, #2
	ldr r7, [r1, r3]
	bl Runtime_BumpAllocate
	ldr r2, [sp, #28]
	adds r5, #32
	mov r1, r9
	str r0, [sp, #8]
	str r1, [r2, r5]
	ldr r2, [sp, #24]
	adds r3, r0, #0
	cmp r2, #0
	blt .L_080da794
	adds r2, #1
	mov r8, r2
.L_080da784:
	movs r1, #1
	negs r1, r1
	add r8, r1
	mov r2, r8
	stmia r3!, {r7}
	ldr r7, [r7]
	cmp r2, #0
	bne .L_080da784
.L_080da794:
	ldr r3, [sp, #28]
	movs r1, #3
	ldr r0, [r3]
	bl Object_SetMode
	ldr r1, [sp, #28]
	ldr r0, [r1, #4]
	movs r1, #3
	bl Object_SetMode
	movs r0, #4
	bl WaitFrames
	ldr r3, [sp, #24]
	movs r2, #0
	str r2, [sp, #20]
	cmp r2, r3
	ble .L_080da7ba
	b .L_080da8ea
.L_080da7ba:
	ldr r1, [sp, #20]
	ldr r3, [sp, #24]
	movs r2, #0
	lsls r1, r1, #12
	movs r5, #128
	mov r8, r2
	mov r10, r1
	lsls r5, r5, #11
	cmp r8, r3
	ble .L_080da7d0
	b .L_080da8d6
.L_080da7d0:
	ldr r1, .L_080da900
	mov r9, r1
.L_080da7d4:
	ldr r1, [sp, #8]
	mov r2, r8
	lsls r3, r2, #2
	ldr r7, [r3, r1]
	movs r3, #0
	strb r3, [r7, #17]
	ldr r2, [sp, #20]
	cmp r8, r2
	bgt .L_080da82e
	mov r0, r10
	bl Trig_Cos
	adds r1, r5, #0
	mov lr, r9
	.2byte 0xf800
	ldr r3, [sp, #40]
	adds r0, r3, r0
	str r0, [r7, #4]
	ldr r1, [sp, #36]
	str r0, [sp, #16]
	str r1, [r7, #8]
	mov r0, r10
	str r1, [sp, #12]
	bl Trig_Sin
	adds r1, r5, #0
	mov lr, r9
	.2byte 0xf800
	ldr r2, [sp, #32]
	movs r3, #2
	adds r0, r2, r0
	str r0, [r7, #12]
	mov r11, r0
	movs r0, #192
	adds r1, r5, #0
	strb r3, [r7, #16]
	lsls r0, r0, #11
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r0, r0, #16
	add r10, r0
	lsls r0, r0, #2
	adds r5, r5, r0
	b .L_080da8ca
.L_080da82e:
	ldr r1, [sp, #16]
	ldr r3, [r7, #4]
	subs r3, r1, r3
	asrs r2, r3, #8
	ldr r1, [sp, #12]
	ldr r3, [r7, #8]
	adds r0, r2, #0
	subs r3, r1, r3
	asrs r4, r3, #8
	ldr r3, [r7, #12]
	mov r1, r11
	subs r3, r1, r3
	str r4, [sp, #0]
	adds r1, r2, #0
	asrs r6, r3, #8
	mov lr, r9
	.2byte 0xf800
	ldr r4, [sp, #0]
	adds r5, r0, #0
	adds r1, r4, #0
	adds r0, r4, #0
	mov lr, r9
	.2byte 0xf800
	adds r1, r6, #0
	str r0, [sp, #4]
	adds r0, r6, #0
	mov lr, r9
	.2byte 0xf800
	ldr r2, [sp, #4]
	ldr r3, .L_080da904
	adds r5, r5, r2
	adds r5, r5, r0
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	adds r5, r0, #0
	cmp r5, #6
	ble .L_080da8d6
	ldr r1, [sp, #16]
	ldr r3, [r7, #4]
	subs r3, r3, r1
	lsls r0, r3, #1
	adds r0, r0, r3
	adds r1, r5, #0
	lsls r0, r0, #1
	bl __divsi3
	ldr r2, [sp, #16]
	adds r0, r2, r0
	str r0, [r7, #4]
	str r0, [sp, #16]
	ldr r1, [sp, #12]
	ldr r3, [r7, #8]
	subs r3, r3, r1
	lsls r0, r3, #1
	adds r0, r0, r3
	adds r1, r5, #0
	lsls r0, r0, #1
	bl __divsi3
	ldr r2, [sp, #12]
	mov r1, r11
	adds r0, r2, r0
	str r0, [r7, #8]
	str r0, [sp, #12]
	ldr r3, [r7, #12]
	subs r3, r3, r1
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #1
	adds r1, r5, #0
	bl __divsi3
	movs r3, #2
	add r0, r11
	str r0, [r7, #12]
	strb r3, [r7, #16]
	mov r11, r0
.L_080da8ca:
	ldr r3, [sp, #24]
	movs r2, #1
	add r8, r2
	cmp r8, r3
	bgt .L_080da8d6
	b .L_080da7d4
.L_080da8d6:
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #20]
	ldr r2, [sp, #24]
	adds r1, #1
	str r1, [sp, #20]
	cmp r1, r2
	bgt .L_080da8ea
	b .L_080da7ba
.L_080da8ea:
	ldr r0, [sp, #8]
	bl Sys_Free
	movs r0, #0
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080da900:
	.4byte IwramMulQ16
.L_080da904:
	.4byte IwramFillWords + 0x74
