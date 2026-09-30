.syntax unified
	.thumb
	.global Func_080d3c88
	.thumb_func
Func_080d3c88:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #60]
	sub sp, #64
	str r1, [sp, #32]
	movs r2, #0
	ldr r3, [r3, #108]
	adds r6, r0, #0
	str r3, [sp, #28]
	mov r10, r2
	mov r9, r2
	bl ObjectTable_ReadActiveValue
	movs r2, #240
	movs r1, #4
	lsls r2, r2, #8
	movs r3, #0
	str r1, [sp, #12]
	mov r11, r2
	ldr r1, [sp, #28]
	str r0, [sp, #24]
	str r3, [sp, #20]
	str r3, [sp, #16]
	movs r2, #226
	mov r3, r11
	lsls r2, r2, #1
	ands r3, r6
	mov r11, r3
	adds r3, r1, r2
	movs r2, #0
	ldrsh r1, [r3, r2]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	ands r6, r3
	movs r4, #0
	adds r0, r6, #0
	mov r8, r1
	str r4, [sp, #4]
	bl ObjectTable_Get
	ldr r1, [sp, #28]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	str r6, [r3]
	subs r2, #40
	adds r3, r1, r2
	ldr r3, [r3]
	movs r5, #0
	movs r7, #0
	ldr r4, [sp, #4]
	cmp r3, #0
	beq .L_080d3d04
	b .L_080d3f7a
.L_080d3d04:
	cmp r0, #0
	beq .L_080d3d36
	subs r2, #46
	adds r3, r1, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080d3d2e
	add r5, sp, #52
	adds r1, r5, #0
	adds r0, #8
	bl Func_08015778
	ldr r3, [r5]
	movs r7, #1
	asrs r4, r3, #3
	ldr r3, [r5, #4]
	asrs r5, r3, #3
	subs r5, #2
	b .L_080d3d82
.L_080d3d2e:
	add r5, sp, #52
	adds r1, r5, #0
	adds r0, r6, #0
	b .L_080d3d6e
.L_080d3d36:
	cmp r6, #7
	bgt .L_080d3d82
	str r6, [sp, #24]
	bl Func_080cdf5c
	bl ObjectTable_Get
	ldr r1, [sp, #28]
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080d3d66
	add r5, sp, #52
	adds r1, r5, #0
	adds r0, #8
	bl Func_08015778
	ldr r3, [r5]
	movs r7, #1
	b .L_080d3d7c
.L_080d3d66:
	bl Func_080cdf5c
	add r5, sp, #52
	adds r1, r5, #0
.L_080d3d6e:
	bl Func_080d5bec
	mvns r0, r0
	negs r3, r0
	orrs r3, r0
	lsrs r7, r3, #31
	ldr r3, [r5]
.L_080d3d7c:
	asrs r4, r3, #3
	ldr r3, [r5, #4]
	asrs r5, r3, #3
.L_080d3d82:
	cmp r7, #0
	bne .L_080d3d8e
	movs r3, #15
	str r3, [sp, #48]
	movs r3, #10
	b .L_080d3dde
.L_080d3d8e:
	movs r3, #0
	add r0, sp, #36
	str r3, [sp, #48]
	str r3, [sp, #44]
	add r1, sp, #48
	add r2, sp, #44
	add r3, sp, #40
	str r0, [sp, #0]
	mov r0, r8
	str r4, [sp, #4]
	bl Func_08038108 + 0x8
	ldr r3, [sp, #40]
	ldr r4, [sp, #4]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	subs r3, r4, r3
	str r3, [sp, #48]
	movs r3, #128
	lsls r3, r3, #7
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_080d3dc8
	ldr r3, [sp, #36]
	subs r3, r5, r3
	subs r3, #1
	b .L_080d3dde
.L_080d3dc8:
	mov r2, r11
	lsrs r3, r2, #15
	cmp r3, #0
	bne .L_080d3ddc
	cmp r5, #8
	bgt .L_080d3ddc
	ldr r3, [sp, #36]
	subs r3, r5, r3
	subs r3, #1
	b .L_080d3dde
.L_080d3ddc:
	adds r3, r5, #4
.L_080d3dde:
	str r3, [sp, #44]
	ldr r1, [sp, #32]
	ldrb r3, [r1, #4]
	cmp r3, #0
	beq .L_080d3dec
	movs r2, #5
	str r2, [sp, #12]
.L_080d3dec:
	movs r3, #128
	lsls r3, r3, #5
	mov r1, r11
	ands r3, r1
	adds r6, r4, #0
	cmp r3, #0
	beq .L_080d3e08
	ldr r2, [sp, #12]
	subs r3, r6, r2
	subs r6, r3, #2
	cmp r6, #0
	bge .L_080d3e44
	movs r6, #0
	b .L_080d3e44
.L_080d3e08:
	movs r3, #128
	lsls r3, r3, #6
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_080d3e24
	ldr r2, [sp, #12]
	adds r6, #2
	adds r3, r6, r2
	cmp r3, #29
	ble .L_080d3e44
	movs r3, #29
	subs r6, r3, r2
	b .L_080d3e44
.L_080d3e24:
	cmp r6, #15
	bgt .L_080d3e36
	ldr r1, [sp, #12]
	subs r3, r6, r1
	subs r6, r3, #2
	cmp r6, #0
	bge .L_080d3e44
	adds r6, r4, #2
	b .L_080d3e44
.L_080d3e36:
	ldr r2, [sp, #12]
	adds r6, #2
	adds r3, r6, r2
	cmp r3, #29
	ble .L_080d3e44
	subs r3, r4, r2
	subs r6, r3, #2
.L_080d3e44:
	ldr r0, [sp, #24]
	bl Func_080381e0
	movs r3, #1
	negs r3, r3
	adds r7, r0, #0
	mov r10, r3
	cmp r7, r10
	beq .L_080d3ec4
	mov r3, sp
	movs r1, #48
	movs r2, #44
	add r1, sp
	add r2, sp
	adds r3, #40
	add r7, sp, #36
	mov r0, r8
	mov r9, r1
	mov r11, r2
	str r3, [sp, #8]
	str r7, [sp, #0]
	bl Func_08038108 + 0x8
	ldr r2, [sp, #44]
	mov r8, r10
	subs r1, r2, #5
	str r1, [sp, #16]
	cmp r2, r5
	bgt .L_080d3e84
	ldr r3, [sp, #36]
	adds r3, r2, r3
	str r3, [sp, #16]
.L_080d3e84:
	ldr r3, [sp, #16]
	cmp r3, #0
	bge .L_080d3e92
	ldr r3, [sp, #36]
	adds r3, r2, r3
	str r3, [sp, #16]
	b .L_080d3e9e
.L_080d3e92:
	ldr r3, [sp, #16]
	adds r3, #5
	cmp r3, #19
	ble .L_080d3e9e
	subs r1, r2, #5
	str r1, [sp, #16]
.L_080d3e9e:
	ldr r3, [sp, #16]
	cmp r2, r3
	bge .L_080d3ee6
	movs r0, #1
	mov r1, r9
	ldr r3, [sp, #8]
	negs r0, r0
	mov r2, r11
	ldr r5, [sp, #36]
	str r7, [sp, #0]
	bl Func_08038108
	ldr r3, [sp, #36]
	movs r1, #1
	subs r5, r5, r3
	negs r1, r1
	adds r5, #1
	mov r8, r1
	b .L_080d3ee4
.L_080d3ec4:
	ldr r3, [sp, #44]
	cmp r3, r5
	bge .L_080d3ee6
	add r0, sp, #36
	add r3, sp, #40
	str r0, [sp, #0]
	add r1, sp, #48
	mov r0, r8
	add r2, sp, #44
	ldr r5, [sp, #36]
	bl Func_08038108
	ldr r3, [sp, #36]
	mov r8, r7
	subs r5, r5, r3
	adds r5, #1
.L_080d3ee4:
	str r5, [sp, #20]
.L_080d3ee6:
	cmp r6, #0
	bge .L_080d3eee
	movs r6, #0
	b .L_080d3efa
.L_080d3eee:
	ldr r2, [sp, #12]
	adds r3, r6, r2
	cmp r3, #29
	ble .L_080d3efa
	movs r3, #29
	subs r6, r3, r2
.L_080d3efa:
	ldr r1, [sp, #32]
	ldrb r3, [r1, #4]
	cmp r3, #0
	beq .L_080d3f28
	movs r0, #8
	bl WaitFrames
	ldr r2, [sp, #20]
	cmp r2, #0
	beq .L_080d3f1e
	ldr r3, [sp, #20]
	ldr r2, [sp, #44]
	ldr r1, [sp, #48]
	adds r2, r2, r3
	subs r2, #1
	mov r0, r8
	movs r3, #18
	b .L_080d3f52
.L_080d3f1e:
	ldr r1, [sp, #48]
	ldr r2, [sp, #44]
	mov r0, r8
	movs r3, #2
	b .L_080d3f52
.L_080d3f28:
	ldr r0, [sp, #24]
	bl Func_080d1eac
	ldr r1, [sp, #20]
	cmp r1, #0
	beq .L_080d3f46
	ldr r3, [sp, #20]
	ldr r2, [sp, #44]
	ldr r1, [sp, #48]
	adds r2, r2, r3
	lsls r3, r0, #16
	movs r0, #17
	orrs r3, r0
	subs r2, #1
	b .L_080d3f50
.L_080d3f46:
	lsls r3, r0, #16
	movs r0, #1
	orrs r3, r0
	ldr r1, [sp, #48]
	ldr r2, [sp, #44]
.L_080d3f50:
	mov r0, r8
.L_080d3f52:
	bl UiText_OpenMessageWindowFar
	mov r10, r0
	ldr r1, [sp, #32]
	ldrb r3, [r1, #4]
	ldr r0, [sp, #24]
	movs r1, #0
	adds r2, r6, #0
	ldr r3, [sp, #16]
	bl Func_080380f8
	mov r9, r0
	b .L_080d3f72
.L_080d3f6c:
	movs r0, #1
	bl WaitFrames
.L_080d3f72:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_080d3f6c
.L_080d3f7a:
	ldr r2, [sp, #28]
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r2, r1
	mov r2, r10
	str r2, [r3]
	ldr r1, [sp, #28]
	movs r2, #244
	lsls r2, r2, #1
	adds r3, r1, r2
	mov r1, r9
	str r1, [r3]
	ldr r3, [sp, #28]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r3, r1
	ldrh r3, [r2]
	mov r0, r10
	adds r3, #1
	strh r3, [r2]
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
