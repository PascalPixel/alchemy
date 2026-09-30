.syntax unified
	.thumb
	.global Func_080daecc
	.thumb_func
Func_080daecc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r0, [sp, #8]
	str r1, [sp, #4]
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	adds r2, r3, #0
	adds r2, #164
	ldr r2, [r2]
	adds r7, r0, #0
	movs r0, #186
	lsls r0, r0, #1
	ldr r5, [r3, #32]
	cmp r2, #0
	bne .L_080daf0e
	ldr r3, [r7, #80]
	cmp r3, #0
	beq .L_080daf0a
	ldr r3, [r3, #40]
	cmp r3, #0
	beq .L_080daf0a
	movs r1, #0
	ldrsh r0, [r3, r1]
.L_080daf0a:
	bl Func_080da9a8
.L_080daf0e:
	ldr r0, [sp, #8]
	bl Func_080dae70
	cmp r0, #0
	beq .L_080daf1a
	b .L_080db096
.L_080daf1a:
	bl Func_080dae3c
	mov r10, r0
	cmp r0, #0
	bne .L_080daf26
	b .L_080db096
.L_080daf26:
	ldr r3, [r7, #8]
	ldr r2, [r7, #12]
	asrs r3, r3, #20
	mov r9, r3
	ldr r3, [r7, #16]
	mov r0, r9
	subs r3, r3, r2
	movs r2, #212
	asrs r3, r3, #20
	lsls r2, r2, #1
	mov r8, r3
	adds r3, r5, r2
	ldr r1, [r3]
	mov r3, r8
	lsls r2, r3, #9
	lsls r3, r0, #2
	movs r0, #128
	movs r4, #0
	movs r6, #4
	adds r2, r2, r3
	lsls r0, r0, #2
.L_080daf50:
	adds r2, r2, r0
	adds r3, r1, r2
	subs r6, #1
	strb r4, [r3, #2]
	cmp r6, #0
	bge .L_080daf50
	ldr r2, .L_080db0a4
	ldr r3, [r7, #8]
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #12
	adds r3, r3, r1
	ldr r5, [r7, #12]
	mov r9, r3
	ldr r3, [r7, #16]
	ands r5, r2
	movs r6, #128
	adds r1, r1, r5
	ands r3, r2
	lsls r6, r6, #13
	adds r3, r3, r6
	adds r0, r7, #0
	mov r11, r1
	movs r1, #0
	mov r8, r3
	bl Object_SetMode
	adds r2, r7, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	ldr r3, [r7, #16]
	ldr r1, .L_080db0a8
	adds r3, r3, r6
	str r3, [r7, #16]
	ldr r3, [r7, #12]
	adds r0, r7, #0
	adds r3, r3, r1
	str r3, [r7, #12]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	movs r2, #240
	mov r7, r10
	lsls r2, r2, #12
	movs r4, #6
	adds r7, #24
	movs r6, #0
	adds r5, r5, r2
	b .L_080daff2
.L_080dafb8:
	mov r3, r9
	mov r1, r8
	str r3, [r0, #4]
	str r5, [r0, #8]
	str r1, [r0, #12]
	cmp r6, #0
	bne .L_080dafca
	movs r3, #1
	b .L_080dafe6
.L_080dafca:
	cmp r6, #1
	bne .L_080dafd2
	movs r3, #2
	b .L_080dafe6
.L_080dafd2:
	cmp r6, r4
	bne .L_080dafda
	movs r3, #5
	b .L_080dafe6
.L_080dafda:
	lsrs r3, r6, #31
	adds r3, r6, r3
	asrs r3, r3, #1
	lsls r3, r3, #1
	subs r3, r6, r3
	adds r3, #3
.L_080dafe6:
	strb r3, [r0, #18]
	ldr r2, .L_080db0ac
	str r0, [r7]
	adds r5, r5, r2
	adds r7, r0, #0
	adds r6, #1
.L_080daff2:
	cmp r6, r4
	bgt .L_080db002
	str r4, [sp, #0]
	bl Func_080daea8
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_080dafb8
.L_080db002:
	ldr r3, [sp, #4]
	cmp r3, #0
	bne .L_080db024
	mov r1, r10
	movs r6, #0
	ldr r0, [r1, #24]
	cmp r6, r4
	bgt .L_080db076
	cmp r0, #0
	beq .L_080db076
.L_080db016:
	adds r6, #1
	ldr r0, [r0]
	cmp r6, r4
	bgt .L_080db076
	cmp r0, #0
	bne .L_080db016
	b .L_080db076
.L_080db024:
	movs r7, #128
	movs r5, #1
	lsls r7, r7, #9
.L_080db02a:
	mov r2, r10
	movs r6, #0
	ldr r0, [r2, #24]
	cmp r6, r4
	bgt .L_080db060
	cmp r0, #0
	beq .L_080db060
	lsls r3, r5, #15
	negs r2, r3
	mov r12, r7
.L_080db03e:
	mov r3, r9
	str r3, [r0, #4]
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	mov r1, r11
	subs r3, r1, r3
	str r3, [r0, #8]
	adds r6, #1
	mov r3, r8
	str r3, [r0, #12]
	add r2, r12
	ldr r0, [r0]
	cmp r6, r4
	bgt .L_080db060
	cmp r0, #0
	bne .L_080db03e
.L_080db060:
	movs r0, #1
	str r4, [sp, #0]
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #9
	adds r5, #1
	adds r7, r7, r0
	ldr r4, [sp, #0]
	cmp r5, #28
	ble .L_080db02a
.L_080db076:
	mov r1, r10
	movs r3, #1
	str r3, [r1, #20]
	add r2, sp, #8
	movs r3, #0
	str r3, [r1, #16]
	ldrh r2, [r2]
	movs r3, #128
	lsls r3, r3, #1
	str r3, [r1, #4]
	mov r3, r10
	strh r2, [r3]
	movs r3, #224
	lsls r3, r3, #12
	mov r0, r10
	str r3, [r0, #8]
.L_080db096:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080db0a4:
	.4byte 0xfff00000
.L_080db0a8:
	.4byte 0xffc00000
.L_080db0ac:
	.4byte 0xfff20000
