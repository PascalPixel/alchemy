.syntax unified
	.thumb
	.global Func_08192894
	.thumb_func
Func_08192894:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #44
	mov r0, r9
	add r3, sp, #40
	str r0, [r3]
	adds r3, r0, #0
	str r3, [sp, #12]
	subs r3, #4
	mov r9, r3
	mov r2, sp
	movs r3, #100
	movs r1, #0
	adds r2, #16
	movs r0, #160
	str r3, [sp, #0]
	str r1, [sp, #8]
	str r2, [sp, #4]
	lsls r0, r0, #1
	movs r1, #176
	movs r2, #0
	add r6, sp, #24
	mov r11, r0
	mov r10, r1
	mov r8, r2
.L_081928d0:
	mov r3, r9
	ldr r2, [r3]
	mov r0, r8
	ldrsh r3, [r2, r0]
	cmp r3, #0
	bne .L_081928de
	b .L_081929d0
.L_081928de:
	ldr r0, [sp, #4]
	lsls r3, r3, #1
	str r3, [r0, #4]
	str r3, [sp, #16]
	movs r3, #255
	movs r1, #0
	lsls r3, r3, #16
	str r1, [r6, #12]
	str r3, [r6, #4]
	ldr r3, [sp, #8]
	mov r0, r10
	lsls r5, r3, #2
	ldr r3, [r2, r0]
	mov r1, r11
	str r3, [r6]
	ldr r3, [r2, r1]
	adds r1, r6, #0
	str r3, [r6, #8]
	ldr r7, [sp, #12]
	ldr r2, [sp, #4]
	subs r7, #8
	ldr r3, [r7]
	ldr r0, [r5, r3]
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
	mov r2, r9
	ldr r1, [r2]
	movs r3, #232
	lsls r3, r3, #1
	mov r0, r10
	adds r2, r5, r3
	ldr r2, [r1, r2]
	ldr r3, [r1, r0]
	adds r3, r3, r2
	str r3, [r1, r0]
	movs r2, #152
	lsls r2, r2, #2
	mov r0, r11
	adds r3, r5, r2
	ldr r3, [r1, r3]
	ldr r2, [r1, r0]
	adds r2, r2, r3
	str r2, [r1, r0]
	mov r0, r10
	ldr r3, [r1, r0]
	movs r0, #128
	lsls r0, r0, #13
	adds r3, r3, r0
	movs r0, #128
	lsls r0, r0, #17
	cmp r3, r0
	bhi .L_08192954
	cmp r2, #0
	blt .L_08192954
	movs r3, #160
	lsls r3, r3, #16
	cmp r2, r3
	ble .L_08192960
.L_08192954:
	mov r0, r9
	ldr r2, [r0]
	ldr r3, .L_0819297c
	mov r1, r8
	strh r3, [r2, r1]
	b .L_081929d0
.L_08192960:
	ldr r3, [sp, #12]
	movs r2, #3
	subs r3, #12
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_08192980
	adds r2, r1, #2
	ldr r1, [sp, #0]
	ldrh r3, [r2, r1]
	adds r0, r1, #0
	adds r3, #1
	strh r3, [r2, r0]
	b .L_08192984
.L_0819297c:
	.4byte 0x00000000
.L_08192980:
	mov r1, r8
	adds r1, #100
.L_08192984:
	mov r2, r9
	ldr r4, [r2]
	adds r2, r4, #2
	ldrh r3, [r2, r1]
	cmp r3, #14
	bls .L_08192994
	ldr r3, .L_081929c0
	strh r3, [r2, r1]
.L_08192994:
	ldr r3, [r7]
	ldr r2, .L_081929c4
	ldr r0, [r5, r3]
	adds r3, r4, #2
	ldrh r3, [r3, r1]
	lsls r3, r3, #1
	adds r3, #72
	ldrh r1, [r4, r3]
	ldr r3, .L_081929c8
	ands r1, r3
	ldrh r3, [r0, #8]
	ands r3, r2
	orrs r3, r1
	strh r3, [r0, #8]
	mov r0, r8
	ldrh r3, [r4, r0]
	ldr r1, .L_081929cc
	mov r2, r8
	adds r3, r3, r1
	strh r3, [r4, r2]
	b .L_081929d0
	.2byte 0x0000
.L_081929c0:
	.4byte 0x00000000
.L_081929c4:
	.4byte 0xfffffc00
.L_081929c8:
	.4byte 0x000003ff
.L_081929cc:
	.4byte 0xffffff00
.L_081929d0:
	ldr r3, [sp, #0]
	ldr r2, [sp, #8]
	movs r0, #4
	adds r3, #2
	movs r1, #2
	adds r2, #1
	str r3, [sp, #0]
	add r11, r0
	add r10, r0
	add r8, r1
	str r2, [sp, #8]
	cmp r2, #36
	beq .L_081929ec
	b .L_081928d0
.L_081929ec:
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
