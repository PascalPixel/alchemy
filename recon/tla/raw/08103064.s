.syntax unified
	.thumb
	.global Func_08103064
	.thumb_func
Func_08103064:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r1, #0
	mov r11, r0
	adds r0, r5, #0
	sub sp, #4
	adds r6, r2, #0
	bl Owner_GetState
	mov r10, r0
	movs r0, #1
	negs r0, r0
	movs r7, #0
	cmp r6, r0
	bne .L_081030fc
	lsls r5, r5, #8
	movs r6, #132
	str r5, [sp, #0]
	movs r1, #0
	lsls r6, r6, #1
	mov r8, r1
	add r6, r10
	mov r9, r1
.L_0810309c:
	ldr r2, [r6]
	mov r3, r8
	lsls r1, r3, #5
	mov r0, r11
	lsls r3, r7, #1
	movs r5, #0
	mov lr, r9
	mov r12, r2
	adds r4, r3, r0
.L_081030ae:
	movs r2, #1
	lsls r2, r5
	mov r3, r12
	ands r3, r2
	cmp r3, #0
	beq .L_081030c8
	ldr r2, .L_08103164
	ldr r0, [sp, #0]
	adds r3, r1, #0
	orrs r3, r5
	orrs r3, r2
	orrs r3, r0
	b .L_081030de
.L_081030c8:
	mov r3, lr
	adds r3, #248
	mov r0, r10
	ldr r3, [r0, r3]
	ands r3, r2
	cmp r3, #0
	beq .L_081030e4
	ldr r2, [sp, #0]
	adds r3, r1, #0
	orrs r3, r5
	orrs r3, r2
.L_081030de:
	strh r3, [r4]
	adds r7, #1
	adds r4, #2
.L_081030e4:
	adds r5, #1
	cmp r5, #19
	ble .L_081030ae
	movs r0, #1
	add r8, r0
	movs r3, #4
	mov r1, r8
	adds r6, #4
	add r9, r3
	cmp r1, #3
	ble .L_0810309c
	b .L_08103152
.L_081030fc:
	movs r0, #132
	lsls r3, r6, #2
	lsls r0, r0, #1
	adds r2, r3, r0
	mov r1, r10
	ldr r2, [r1, r2]
	lsls r4, r6, #5
	mov r12, r2
	mov r6, r11
	lsls r2, r7, #1
	adds r0, r2, r6
	ldr r2, .L_08103164
	movs r1, #1
	adds r3, #248
	movs r5, #0
	mov lr, r1
	mov r9, r2
	mov r8, r3
.L_08103120:
	mov r1, lr
	lsls r1, r5
	mov r3, r12
	ands r3, r1
	cmp r3, #0
	beq .L_08103136
	adds r3, r4, #0
	orrs r3, r5
	mov r6, r9
	orrs r3, r6
	b .L_08103146
.L_08103136:
	mov r2, r10
	mov r6, r8
	ldr r3, [r2, r6]
	ands r3, r1
	cmp r3, #0
	beq .L_0810314c
	adds r3, r4, #0
	orrs r3, r5
.L_08103146:
	strh r3, [r0]
	adds r7, #1
	adds r0, #2
.L_0810314c:
	adds r5, #1
	cmp r5, #19
	ble .L_08103120
.L_08103152:
	adds r0, r7, #0
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08103164:
	.4byte 0xffff8000
