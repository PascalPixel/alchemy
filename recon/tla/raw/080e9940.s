.syntax unified
	.thumb
	.global Func_080e9940
	.thumb_func
Func_080e9940:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r5, #192
	lsls r5, r5, #18
	sub sp, #12
	ldr r7, [r5, #92]
	bl Func_080cdf5c
	bl ObjectTable_Get
	mov r10, r0
	ldr r5, [r5, #108]
	mov r3, r10
	mov r2, r10
	adds r3, #34
	ldr r1, [r2, #16]
	ldr r0, [r0, #8]
	ldrb r2, [r3]
	mov r8, r5
	bl Func_080dbcc0
	cmp r0, #0
	bne .L_080e9992
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_080e999e
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #68
	add r2, r8
	ldrh r3, [r2]
	subs r3, #1
	b .L_080e999c
.L_080e9992:
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #68
	add r2, r8
	movs r3, #112
.L_080e999c:
	strh r3, [r2]
.L_080e999e:
	movs r2, #128
	lsls r2, r2, #5
	adds r2, #52
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_080e99f0
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #68
	add r3, r8
	movs r2, #0
	ldrsh r6, [r3, r2]
	cmp r6, #112
	ble .L_080e99c0
	movs r6, #112
.L_080e99c0:
	cmp r6, #0
	bge .L_080e99c6
	movs r6, #0
.L_080e99c6:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #40
	lsls r0, r6, #15
	movs r1, #112
	adds r5, r7, r3
	bl Math_Div
	movs r2, #128
	lsls r2, r2, #8
	adds r0, r0, r2
	str r0, [r5]
	cmp r6, #0
	bne .L_080e99f0
	movs r2, #179
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #6
	add r2, r8
	adds r3, #153
	strh r3, [r2]
.L_080e99f0:
	ldr r6, .L_080e9ab0
	movs r5, #0
.L_080e99f4:
	movs r2, #128
	lsls r2, r2, #5
	adds r2, #50
	adds r3, r7, r2
	ldrh r0, [r3]
	lsrs r3, r5, #31
	lsls r0, r0, #16
	adds r3, r5, r3
	asrs r3, r3, #1
	asrs r0, r0, #18
	movs r1, #24
	adds r0, r0, r3
	bl Math_Mod
	adds r1, r5, #0
	cmp r5, #15
	bne .L_080e9a18
	movs r1, #14
.L_080e9a18:
	bl Func_080e989c
	strh r0, [r6]
	adds r6, #2
	adds r5, #1
	cmp r5, #15
	ble .L_080e99f4
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #50
	adds r2, r7, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r2, #128
	lsls r2, r2, #5
	adds r2, #40
	adds r3, r7, r2
	ldr r3, [r3]
	mov r9, r3
	lsls r3, r3, #1
	add r3, r9
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_080e9a4c
	adds r3, #7
.L_080e9a4c:
	asrs r3, r3, #3
	mov r8, r3
	movs r3, #128
	lsls r3, r3, #5
	adds r6, r7, r3
	ldr r3, .L_080e9ab4
	movs r2, #128
	ldr r3, [r3]
	lsls r2, r2, #5
	adds r2, #46
	adds r1, r7, r2
	lsrs r3, r3, #1
	movs r2, #1
	ands r3, r2
	ldrh r2, [r1]
	lsls r3, r3, #5
	adds r2, r2, r3
	ldr r3, .L_080e9aac
	ldrh r1, [r6, #8]
	ands r2, r3
	ldr r3, .L_080e9ab8
	mov r0, r10
	ands r3, r1
	orrs r3, r2
	strh r3, [r6, #8]
	bl Func_080db9c0
	ldrb r2, [r6, #9]
	movs r3, #3
	ands r0, r3
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r0, r0, #2
	orrs r3, r0
	strb r3, [r6, #9]
	mov r0, r10
	bl Func_080db9cc
	subs r0, #1
	strh r0, [r6, #30]
	mov r2, r10
	ldr r3, [r2, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	b .L_080e9abc
	.2byte 0x0000
.L_080e9aac:
	.4byte 0x000003ff
.L_080e9ab0:
	.4byte 0x050003c0
.L_080e9ab4:
	.4byte Data_0300122c
.L_080e9ab8:
	.4byte 0xfffffc00
.L_080e9abc:
	ldr r3, [r2, #12]
	str r3, [r5, #4]
	ldr r3, [r2, #16]
	str r3, [r5, #8]
	bl Camera_WorldToScreen
	ldr r3, [r5]
	mov r2, r8
	str r3, [r6, #12]
	adds r0, r6, #0
	ldr r3, [r5, #8]
	str r2, [r6, #24]
	str r3, [r6, #16]
	mov r3, r9
	str r3, [r6, #20]
	bl Func_080eb01c
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
