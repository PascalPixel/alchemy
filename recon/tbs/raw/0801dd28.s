.syntax unified
	.thumb
	.global Func_0801dd28
	.thumb_func
Func_0801dd28:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #132
	str r1, [sp, #0]
	adds r6, r3, #0
	ldr r3, .L_0801de3c
	ldr r3, [r3]
	mov r11, r0
	ldr r0, .L_0801de40
	mov r9, r2
	mov r8, r3
	bl Resource_GetTableEntry
	mov r10, r0
	mov r0, r11
	ldrb r5, [r0]
	add r1, sp, #4
	movs r3, #192
	mov r12, r1
	lsls r7, r5, #5
	lsls r3, r3, #19
	mov lr, r12
	adds r2, r7, r3
	movs r1, #0
.L_0801dd62:
	ldrb r4, [r2]
	movs r0, #15
	adds r3, r4, #0
	ands r3, r0
	mov r0, r12
	strb r3, [r0]
	lsrs r3, r4, #4
	strb r3, [r0, #1]
	adds r1, #1
	movs r3, #2
	adds r2, #1
	add r12, r3
	cmp r1, #31
	bls .L_0801dd62
	mov r4, r9
	lsls r3, r4, #5
	mov r0, r10
	adds r2, r0, r3
	movs r3, #15
	mov r12, lr
	movs r1, #0
	mov r10, r3
.L_0801dd8e:
	ldrb r0, [r2]
	mov r4, r10
	adds r3, r0, #0
	ands r3, r4
	ldrb r3, [r6, r3]
	adds r2, #1
	cmp r3, #0
	beq .L_0801dda2
	mov r4, r12
	strb r3, [r4]
.L_0801dda2:
	movs r3, #1
	add r12, r3
	lsrs r3, r0, #4
	ldrb r3, [r6, r3]
	cmp r3, #0
	beq .L_0801ddb2
	mov r4, r12
	strb r3, [r4]
.L_0801ddb2:
	movs r0, #1
	adds r1, #1
	add r12, r0
	cmp r1, #31
	bls .L_0801dd8e
	mov r12, lr
	movs r1, #0
	mov r0, r12
.L_0801ddc2:
	ldrb r3, [r0, #1]
	ldrb r2, [r0]
	lsls r3, r3, #4
	orrs r2, r3
	movs r4, #1
	mov r3, r12
	adds r1, #1
	adds r0, #2
	strb r2, [r3]
	add r12, r4
	cmp r1, #31
	bls .L_0801ddc2
	lsls r3, r5, #24
	cmp r3, #0
	blt .L_0801de24
	movs r1, #234
	lsls r1, r1, #4
	movs r0, #218
	movs r4, #0
	add r1, r8
	movs r6, #127
	lsls r0, r0, #4
.L_0801ddee:
	ldrh r3, [r1]
	adds r2, r3, #1
	ands r2, r6
	lsls r3, r3, #24
	lsrs r5, r3, #24
	strh r2, [r1]
	mov r7, r8
	adds r2, r5, r0
	ldrb r3, [r7, r2]
	cmp r3, #0
	beq .L_0801de0a
	adds r4, #1
	cmp r4, #127
	bls .L_0801ddee
.L_0801de0a:
	movs r3, #1
	mov r0, r8
	strb r3, [r0, r2]
	movs r3, #128
	orrs r5, r3
	ldr r2, .L_0801de38
	adds r3, r5, #0
	orrs r3, r2
	mov r1, r11
	strh r3, [r1]
	ldr r2, [sp, #0]
	strh r3, [r2]
	lsls r7, r5, #5
.L_0801de24:
	movs r4, #192
	lsls r4, r4, #19
	ldr r3, .L_0801de44
	add r0, sp, #4
	adds r1, r7, r4
	ldr r2, .L_0801de48
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	add sp, #132
	b .L_0801de4c
.L_0801de38:
	.4byte 0x0000f000
.L_0801de3c:
	.4byte Data_03001e8c
.L_0801de40:
	.4byte 0x00000013
.L_0801de44:
	.4byte 0x040000d4
.L_0801de48:
	.4byte 0x84000008
.L_0801de4c:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
