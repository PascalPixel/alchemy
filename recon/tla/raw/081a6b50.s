.syntax unified
	.thumb
	.global Func_081a6b50
	.thumb_func
Func_081a6b50:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #28
	adds r3, #172
	add r5, sp, #24
	ldr r6, [r3]
	movs r3, #0
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #224
	adds r3, #212
	adds r0, r5, #0
	lsls r1, r1, #19
	ldr r2, .L_081a6c54
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #0
	ldrsh r3, [r6, r0]
	ldrh r2, [r6]
	cmp r3, #0
	beq .L_081a6b8c
	b .L_081a6d62
.L_081a6b8c:
	movs r1, #4
	ldrsh r3, [r6, r1]
	ldrh r2, [r6, #4]
	cmp r3, #0
	bne .L_081a6bbc
	movs r0, #2
	ldrsh r3, [r6, r0]
	ldr r1, .L_081a6c58
	lsls r3, r3, #1
	adds r3, #10
	ldrsh r3, [r1, r3]
	movs r0, #1
	negs r0, r0
	ldrh r2, [r6, #2]
	cmp r3, r0
	beq .L_081a6bb2
	adds r3, r2, #5
	strh r3, [r6, #2]
	adds r2, r3, #0
.L_081a6bb2:
	lsls r3, r2, #16
	asrs r3, r3, #15
	adds r3, #4
	ldrh r3, [r1, r3]
	b .L_081a6bbe
.L_081a6bbc:
	subs r3, r2, #1
.L_081a6bbe:
	strh r3, [r6, #4]
	movs r1, #8
	ldrsh r3, [r6, r1]
	ldrh r2, [r6, #8]
	cmp r3, #0
	bne .L_081a6bdc
	ldrh r3, [r6, #6]
	ldr r2, .L_081a6c5c
	adds r3, #3
	strh r3, [r6, #6]
	lsls r3, r3, #16
	asrs r3, r3, #15
	adds r3, #4
	ldrh r3, [r2, r3]
	b .L_081a6bde
.L_081a6bdc:
	subs r3, r2, #1
.L_081a6bde:
	strh r3, [r6, #8]
	movs r2, #6
	ldrsh r3, [r6, r2]
	ldr r7, .L_081a6c5c
	lsls r4, r3, #1
	ldrsh r0, [r7, r4]
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_081a6c78
	ldr r0, .L_081a6c60
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r1, .L_081a6c64
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, .L_081a6c68
	ldr r0, .L_081a6c6c
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Resource_DecodeByteLzInRam
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_081a6c70
	ldr r2, .L_081a6c74
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #1
	movs r2, #0
	strh r3, [r6]
	strh r3, [r6, #2]
	movs r3, #160
	strh r2, [r6, #4]
	strh r3, [r6, #6]
	strh r2, [r6, #8]
	ldr r3, .L_081a6c4c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_081a6c50
	adds r2, #2
	strh r3, [r2]
	b .L_081a6e82
	.2byte 0x0000
.L_081a6c4c:
	.4byte 0x00000250
.L_081a6c50:
	.4byte 0x00001001
.L_081a6c54:
	.4byte 0x85000100
.L_081a6c58:
	.4byte Data_081a8378
.L_081a6c5c:
	.4byte Data_081a84e6
.L_081a6c60:
	.4byte 0x0000001b
.L_081a6c64:
	.4byte 0x05000200
.L_081a6c68:
	.4byte Data_02026800
.L_081a6c6c:
	.4byte 0x0000001c
.L_081a6c70:
	.4byte 0x06010000
.L_081a6c74:
	.4byte 0x84000c00
.L_081a6c78:
	movs r2, #2
	ldrsh r3, [r6, r2]
	ldr r1, .L_081a6d24
	lsls r3, r3, #1
	ldrsh r6, [r1, r3]
	str r6, [sp, #12]
	adds r6, r3, #2
	ldrsh r6, [r1, r6]
	adds r2, r3, #6
	str r6, [sp, #4]
	adds r3, #8
	ldrsh r2, [r1, r2]
	ldrsh r1, [r1, r3]
	adds r2, r0, r2
	subs r2, #28
	str r2, [sp, #8]
	adds r3, r4, #2
	ldrsh r3, [r7, r3]
	ldr r2, [sp, #4]
	adds r3, r3, r1
	movs r1, #0
	mov r9, r1
	cmp r9, r2
	blt .L_081a6caa
	b .L_081a6e82
.L_081a6caa:
	movs r6, #16
	str r1, [sp, #0]
	add r6, sp
	mov r12, r6
	mov r10, r5
	mov r8, r3
.L_081a6cb6:
	ldr r1, [sp, #12]
	ldr r2, [sp, #0]
	ldr r6, [sp, #8]
	mov r0, r9
	movs r3, #7
	lsls r7, r0, #6
	adds r5, r1, r2
	mov r11, r8
	mov lr, r3
.L_081a6cc8:
	movs r3, #0
	mov r0, r10
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	mov r4, r12
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, r4, #0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_081a6d1c
	adds r2, r5, #0
	ldrh r1, [r4, #4]
	ands r2, r3
	ldr r3, .L_081a6d28
	adds r0, r4, #0
	ands r3, r1
	orrs r3, r2
	strh r3, [r4, #4]
	ldr r3, .L_081a6d20
	adds r2, r6, #0
	ldrh r1, [r4, #2]
	ands r2, r3
	ldr r3, .L_081a6d2c
	ands r3, r1
	orrs r3, r2
	add r1, sp, #16
	mov r2, r11
	strh r3, [r4, #2]
	strb r2, [r1]
	movs r2, #224
	lsls r2, r2, #19
	mov r12, r1
	movs r3, #128
	adds r1, r7, r2
	movs r2, #132
	lsls r3, r3, #19
	b .L_081a6d30
	.2byte 0x0000
.L_081a6d1c:
	.4byte 0x000003ff
.L_081a6d20:
	.4byte 0x000001ff
.L_081a6d24:
	.4byte Data_081a8378
.L_081a6d28:
	.4byte 0xfffffc00
.L_081a6d2c:
	.4byte 0xfffffe00
.L_081a6d30:
	lsls r2, r2, #24
	adds r3, #212
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #1
	negs r3, r3
	add lr, r3
	mov r0, lr
	adds r7, #8
	adds r6, #8
	adds r5, #1
	cmp r0, #0
	bge .L_081a6cc8
	ldr r2, [sp, #0]
	ldr r6, [sp, #4]
	movs r3, #1
	movs r1, #8
	adds r2, #8
	add r9, r3
	add r8, r1
	str r2, [sp, #0]
	cmp r9, r6
	blt .L_081a6cb6
	b .L_081a6e82
.L_081a6d62:
	movs r0, #128
	lsls r3, r2, #16
	lsls r0, r0, #9
	cmp r3, r0
	beq .L_081a6d6e
	b .L_081a6e82
.L_081a6d6e:
	movs r2, #6
	ldrsh r1, [r6, r2]
	ldr r0, .L_081a6e04
	mov r12, r1
	add r0, r12
	adds r3, r0, #0
	cmp r0, #0
	bge .L_081a6d82
	mov r3, r12
	subs r3, #253
.L_081a6d82:
	asrs r3, r3, #2
	adds r1, r3, #0
	adds r3, r0, #0
	adds r1, #30
	cmp r3, #0
	bge .L_081a6d92
	mov r3, r12
	subs r3, #225
.L_081a6d92:
	asrs r3, r3, #5
	adds r5, r3, #0
	adds r5, #50
	movs r0, #0
	adds r2, r5, #0
	bl Func_081a6ab4
	movs r0, #128
	movs r1, #83
	adds r2, r5, #0
	bl Func_081a6ab4
	movs r0, #6
	ldrsh r3, [r6, r0]
	ldr r1, .L_081a6e04
	adds r2, r3, r1
	cmp r2, #0
	bge .L_081a6dba
	adds r2, r3, #0
	subs r2, #253
.L_081a6dba:
	movs r7, #128
	movs r3, #140
	asrs r2, r2, #2
	lsls r7, r7, #1
	subs r1, r3, r2
	adds r0, r7, #0
	adds r2, r5, #0
	bl Func_081a6ab4
	ldrh r2, [r6, #6]
	movs r3, #224
	lsls r3, r3, #19
	adds r3, #6
	strh r2, [r3]
	ldr r2, .L_081a6e00
	adds r3, #8
	strh r2, [r3]
	adds r3, #8
	strh r2, [r3]
	ldrh r2, [r6, #6]
	strh r2, [r3, #8]
	movs r0, #6
	ldrsh r3, [r6, r0]
	ldrh r2, [r6, #6]
	cmp r3, #255
	bgt .L_081a6e08
	adds r3, r2, #2
	strh r3, [r6, #6]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, r7
	ble .L_081a6e08
	strh r7, [r6, #6]
	b .L_081a6e08
	.2byte 0x0000
.L_081a6e00:
	.4byte 0x00000000
.L_081a6e04:
	.4byte 0xffffff00
.L_081a6e08:
	movs r1, #2
	ldrsh r3, [r6, r1]
	ldrh r5, [r6, #2]
	cmp r3, #15
	bgt .L_081a6e82
	ldrh r0, [r6, #4]
	movs r1, #3
	adds r0, #1
	strh r0, [r6, #4]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl __modsi3
	lsls r0, r0, #16
	cmp r0, #0
	bne .L_081a6e38
	ldr r2, .L_081a6e50
	movs r1, #128
	adds r3, r5, #1
	lsls r1, r1, #19
	strh r3, [r6, #2]
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
.L_081a6e38:
	movs r2, #4
	ldrsh r3, [r6, r2]
	cmp r3, #8
	bne .L_081a6e46
	ldr r5, .L_081a6e54
	ldr r0, .L_081a6e58
	b .L_081a6e68
.L_081a6e46:
	cmp r3, #16
	bne .L_081a6e60
	ldr r5, .L_081a6e54
	ldr r0, .L_081a6e5c
	b .L_081a6e68
.L_081a6e50:
	.4byte 0x00001000
.L_081a6e54:
	.4byte Data_02026800
.L_081a6e58:
	.4byte 0x0000001d
.L_081a6e5c:
	.4byte 0x0000001e
.L_081a6e60:
	cmp r3, #24
	bne .L_081a6e82
	ldr r5, .L_081a6e90
	ldr r0, .L_081a6e94
.L_081a6e68:
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Resource_DecodeByteLzInRam
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_081a6e98
	ldr r2, .L_081a6e9c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_081a6e82:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081a6e90:
	.4byte Data_02026800
.L_081a6e94:
	.4byte 0x0000001f
.L_081a6e98:
	.4byte 0x06010000
.L_081a6e9c:
	.4byte 0x84000c00
