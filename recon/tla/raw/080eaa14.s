.syntax unified
	.thumb
	.global Func_080eaa14
	.thumb_func
Func_080eaa14:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r0, [sp, #12]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	str r3, [sp, #8]
	adds r2, r3, r1
	movs r3, #0
	str r3, [r2]
	ldr r2, [sp, #12]
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_080eaa4a
	b .L_080eab58
.L_080eaa4a:
	ldr r3, [sp, #12]
	movs r7, #0
	ldrh r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #80]
	ldr r2, [sp, #12]
	ldrb r5, [r3, #24]
	movs r1, #2
	ldrsh r3, [r2, r1]
	lsls r3, r3, #16
	lsrs r0, r3, #16
	mov r11, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080eaa74
	cmp r5, #0
	beq .L_080eaa7a
	b .L_080eaa78
.L_080eaa74:
	cmp r5, #1
	beq .L_080eaa7a
.L_080eaa78:
	movs r7, #1
.L_080eaa7a:
	cmp r7, #0
	beq .L_080eab44
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	movs r3, #34
	adds r3, r3, r6
	ldrb r2, [r3]
	mov r10, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r1, [r1, r3]
	ldr r3, .L_080eab68
	mov r8, r1
	ldr r1, .L_080eab6c
	add r3, r8
	asrs r3, r3, #2
	adds r1, r1, r3
	adds r0, r6, #0
	mov r9, r1
	bl Func_080eab70
	str r0, [sp, #4]
	mov r2, r10
	ldrb r0, [r2]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Func_080202f0
	str r0, [sp, #0]
	mov r3, r10
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Func_080201c0
	movs r2, #192
	ldr r1, [sp, #8]
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r1, r2
	str r6, [r3]
	mov r3, r11
	lsrs r5, r3, #16
	asrs r7, r0, #19
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080eaaf0
	adds r0, r5, #0
	subs r7, #4
	bl GameFlag_ClearBit
	b .L_080eaaf8
.L_080eaaf0:
	adds r0, r5, #0
	adds r7, #4
	bl GameFlag_SetBit
.L_080eaaf8:
	ldr r3, [r6, #8]
	ldr r2, [r6, #16]
	orrs r3, r2
	cmp r3, #0
	beq .L_080eab44
	ldr r1, [sp, #4]
	mov r2, r10
	lsls r3, r1, #2
	add r9, r1
	ldrb r0, [r2]
	ldr r1, [sp, #0]
	adds r2, r7, #0
	add r8, r3
	bl Func_080eab98
	mov r3, r9
	mov r1, r11
	strb r0, [r3]
	lsrs r0, r1, #16
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080eab34
	movs r3, #0
	mov r2, r8
	strb r3, [r2, #2]
	ldrb r2, [r2, #3]
	movs r3, #128
	orrs r3, r2
	b .L_080eab40
.L_080eab34:
	movs r3, #255
	mov r2, r8
	strb r3, [r2, #2]
	ldrb r2, [r2, #3]
	movs r3, #127
	ands r3, r2
.L_080eab40:
	mov r1, r8
	strb r3, [r1, #3]
.L_080eab44:
	ldr r2, [sp, #12]
	movs r1, #255
	adds r2, #4
	str r2, [sp, #12]
	lsls r1, r1, #8
	ldrh r3, [r2]
	adds r1, #255
	cmp r3, r1
	beq .L_080eab58
	b .L_080eaa4a
.L_080eab58:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080eab68:
	.4byte 0xfdff0000
.L_080eab6c:
	.4byte Data_02024000
