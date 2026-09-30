.syntax unified
	.thumb
	.global Func_080dc7e8
	.thumb_func
Func_080dc7e8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r3, [r3]
	ldr r0, [r2, #108]
	mov r8, r3
	mov r1, r8
	adds r3, r2, #0
	ldr r1, [r1, #16]
	adds r3, #128
	ldr r3, [r3]
	ldr r2, [r2, #32]
	sub sp, #4
	str r1, [sp, #0]
	mov r6, r8
	mov r5, r8
	mov r11, r0
	mov r10, r3
	mov r9, r2
	adds r6, #149
	adds r5, #80
	movs r7, #23
.L_080dc824:
	ldrb r3, [r6]
	adds r6, #72
	lsls r3, r3, #24
	cmp r3, #0
	beq .L_080dc834
	adds r0, r5, #0
	bl Func_080ebf68
.L_080dc834:
	subs r7, #1
	adds r5, #72
	cmp r7, #0
	bge .L_080dc824
	mov r0, r8
	movs r2, #30
	ldrsh r3, [r0, r2]
	cmp r3, #17
	blt .L_080dc852
	cmp r3, #27
	ble .L_080dc84e
	cmp r3, #30
	bne .L_080dc852
.L_080dc84e:
	bl Func_080dc954
.L_080dc852:
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #182
	add r3, r11
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080dc942
	bl Func_080e137c
	bl Func_080e1650
	movs r3, #197
	lsls r3, r3, #1
	add r3, r11
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080dc8b2
	mov r1, r10
	cmp r1, #0
	beq .L_080dc8b2
	movs r3, #168
	lsls r3, r3, #6
	adds r3, #1
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r7, #0
	cmp r3, #0
	beq .L_080dc8b2
	movs r5, #168
	lsls r5, r5, #6
	adds r5, #1
	add r5, r10
.L_080dc89e:
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #89
	bgt .L_080dc8b2
	movs r3, #0
	ldrsb r3, [r5, r3]
	cmp r3, #0
	bne .L_080dc89e
.L_080dc8b2:
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #183
	add r2, r11
	movs r3, #0
	strb r3, [r2]
	bl Func_080dbb40
	ldr r0, .L_080dc950
	bl Func_08014644
	mov r2, r8
	ldr r3, [r2, #68]
	mov r0, r9
	str r3, [r0, #4]
	ldr r3, [r2, #72]
	str r3, [r0, #8]
	ldr r3, [r2, #76]
	str r3, [r0, #12]
	ldr r1, [sp, #0]
	cmp r1, #0
	beq .L_080dc90a
	movs r3, #227
	lsls r3, r3, #3
	add r3, r8
	ldrb r3, [r3]
	adds r2, r1, #0
	adds r2, #35
	strb r3, [r2]
	movs r3, #224
	lsls r3, r3, #3
	adds r3, #25
	add r3, r8
	ldr r0, [r1, #80]
	ldrb r2, [r3]
	movs r1, #3
	ands r2, r1
	ldrb r1, [r0, #9]
	movs r3, #13
	negs r3, r3
	lsls r2, r2, #2
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
.L_080dc90a:
	mov r2, r8
	movs r0, #30
	ldrsh r3, [r2, r0]
	ldrh r1, [r2, #30]
	cmp r3, #8
	beq .L_080dc922
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #165
	add r2, r11
	movs r3, #1
	strb r3, [r2]
.L_080dc922:
	lsls r3, r1, #16
	movs r1, #224
	lsls r1, r1, #13
	cmp r3, r1
	beq .L_080dc938
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #173
	add r2, r11
	movs r3, #1
	strb r3, [r2]
.L_080dc938:
	bl Func_080d2c98
	movs r0, #224
	bl Runtime_ReleaseHeapBlock
.L_080dc942:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080dc950:
	.4byte Func_080dc3f0
