.syntax unified
	.thumb
	.global Func_0802dd70
	.thumb_func
Func_0802dd70:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	str r0, [sp, #12]
	adds r7, r2, #0
	ldr r3, [r1]
	add r0, sp, #28
	movs r2, #0
	str r2, [r0, #4]
	str r3, [r0]
	ldr r3, [r1, #8]
	add r1, sp, #16
	str r3, [r0, #8]
	ldr r3, .L_0802de78
	mov lr, r3
	.2byte 0xf800
	mov r2, sp
	adds r2, #16
	str r2, [sp, #0]
	ldr r3, .L_0802de7c
	ldr r0, [r2, #8]
	ldr r1, [sp, #12]
	mov lr, r3
	.2byte 0xf800
	ldr r2, [sp, #0]
	ldr r3, [r2, #4]
	movs r2, #0
	subs r3, r3, r0
	str r3, [sp, #8]
	ldr r3, .L_0802de80
	mov r11, r2
	mov r9, r3
	ldr r3, [r3]
	negs r3, r3
	str r3, [sp, #4]
	b .L_0802ddc6
.L_0802ddc2:
	ldr r3, .L_0802de80
	mov r9, r3
.L_0802ddc6:
	mov r2, r9
	ldr r1, [r2, #16]
	ldr r2, .L_0802de84
	mov r3, r11
	subs r1, r1, r3
	lsls r1, r1, #16
	mov r8, r2
	ldr r0, [sp, #4]
	mov lr, r8
	.2byte 0xf800
	ldr r3, [sp, #12]
	adds r6, r0, #0
	subs r0, r6, r3
	cmp r0, #0
	bne .L_0802dde6
	movs r0, #1
.L_0802dde6:
	ldr r1, [sp, #8]
	mov lr, r8
	.2byte 0xf800
	adds r5, r0, #0
	cmp r5, #0
	bge .L_0802de52
	ldr r2, .L_0802de7c
	movs r1, #128
	mov r10, r2
	negs r0, r5
	lsls r1, r1, #8
	mov lr, r10
	.2byte 0xf800
	mov r3, r9
	adds r1, r0, #0
	ldr r0, [r3]
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	str r0, [r7]
	adds r0, r5, #0
	mov lr, r10
	.2byte 0xf800
	ldr r2, [sp, #0]
	adds r6, r0, #0
	ldr r1, [r2, #8]
	ldr r3, [r2, #4]
	subs r1, r1, r5
	asrs r1, r1, #4
	adds r0, r1, #0
	subs r6, r6, r3
	mov lr, r10
	.2byte 0xf800
	asrs r6, r6, #4
	adds r5, r0, #0
	adds r1, r6, #0
	adds r0, r6, #0
	mov lr, r10
	.2byte 0xf800
	adds r5, r5, r0
	ldr r3, .L_0802de88
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	lsls r0, r0, #12
	cmp r6, #0
	bge .L_0802de46
	negs r0, r0
.L_0802de46:
	movs r1, #128
	lsls r1, r1, #8
	mov lr, r10
	.2byte 0xf800
	str r0, [r7, #4]
	b .L_0802de58
.L_0802de52:
	movs r3, #0
	str r3, [r7]
	str r3, [r7, #4]
.L_0802de58:
	movs r3, #0
	str r3, [r7, #8]
	str r3, [r7, #12]
	movs r3, #1
	add r11, r3
	mov r2, r11
	adds r7, #20
	cmp r2, #159
	ble .L_0802ddc2
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0802de78:
	.4byte IwramTransformVector
.L_0802de7c:
	.4byte IwramMulQ16
.L_0802de80:
	.4byte gCameraSceneParameters
.L_0802de84:
	.4byte IwramRatioMulQ14
.L_0802de88:
	.4byte IwramFillWords + 0x74
