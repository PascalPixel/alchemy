.syntax unified
	.thumb
	.global Func_0803bde4
	.thumb_func
Func_0803bde4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #144
	str r2, [sp, #12]
	mov r9, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #60]
	mov r8, r0
	movs r0, #154
	lsls r0, r0, #5
	adds r3, r6, r0
	ldrh r3, [r3]
	adds r7, r1, #0
	movs r1, #240
	str r3, [sp, #8]
	lsls r1, r1, #4
	adds r1, #56
	adds r3, r6, r1
	ldrh r3, [r3]
	ldr r2, [sp, #176]
	str r3, [sp, #4]
	cmp r2, #1
	beq .L_0803be96
	mov r3, r8
	ldrh r2, [r3, #22]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_0803be96
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #148
	ldr r3, [r3]
	ldr r3, [r3]
	cmp r3, r8
	bne .L_0803be48
	ldr r0, .L_0803bf78
	bl Resource_GetTableEntry
	movs r0, #3
	mov r11, r0
	cmp r7, #32
	bne .L_0803be48
	b .L_0803c058
.L_0803be48:
	ldr r0, .L_0803bf78
	bl Resource_GetTableEntry
	movs r1, #4
	mov r10, r0
	mov r11, r1
	cmp r7, #32
	bne .L_0803be5a
	b .L_0803c058
.L_0803be5a:
	ldr r5, .L_0803bf7c
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_0803bf80
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r10
	str r2, [sp, #0]
	adds r1, r7, #0
	ldr r2, [sp, #12]
	mov r3, r9
	mov r0, r8
	mov lr, r6
	.2byte 0xf800
	adds r5, r0, #0
	adds r0, r6, #0
	bl Sys_Free
	adds r0, r5, #0
	b .L_0803c05a
.L_0803be96:
	movs r3, #5
	mov r11, r3
	cmp r7, #32
	bne .L_0803bea0
	b .L_0803c058
.L_0803bea0:
	bl RenderOutput_AcquireFree
	adds r5, r0, #0
	movs r0, #0
	cmp r5, #0
	bne .L_0803beae
	b .L_0803c05a
.L_0803beae:
	ldr r0, .L_0803bf84
	ldr r2, .L_0803bf88
	subs r3, r5, r6
	adds r3, r3, r0
	adds r1, r3, #0
	muls r1, r2
	movs r3, #1
	movs r2, #0
	strb r3, [r5, #5]
	strb r2, [r5, #4]
	ldr r3, [sp, #176]
	mov r10, r1
	cmp r3, #1
	bne .L_0803bed4
	movs r0, #1
	movs r3, #2
	mov r11, r0
	strb r3, [r5, #5]
	b .L_0803bf20
.L_0803bed4:
	movs r1, #240
	lsls r1, r1, #4
	adds r1, #60
	adds r3, r6, r1
	ldrh r3, [r3]
	cmp r3, #3
	beq .L_0803bef6
	cmp r3, #3
	bgt .L_0803beec
	cmp r3, #2
	beq .L_0803bf0a
	b .L_0803bf10
.L_0803beec:
	cmp r3, #4
	beq .L_0803befc
	cmp r3, #5
	beq .L_0803bf06
	b .L_0803bf10
.L_0803bef6:
	movs r3, #5
	strb r3, [r5, #5]
	b .L_0803bf10
.L_0803befc:
	movs r3, #6
	strb r3, [r5, #5]
	movs r3, #8
	strh r3, [r5, #12]
	b .L_0803bf10
.L_0803bf06:
	movs r3, #7
	b .L_0803bf0c
.L_0803bf0a:
	movs r3, #4
.L_0803bf0c:
	strb r3, [r5, #5]
	strh r2, [r5, #12]
.L_0803bf10:
	add r1, sp, #16
	adds r0, r7, #0
	bl Func_0803a8fc
	cmp r0, #0
	bne .L_0803bf1e
	movs r0, #1
.L_0803bf1e:
	mov r11, r0
.L_0803bf20:
	ldrb r3, [r5, #5]
	cmp r3, #2
	bne .L_0803bf90
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #70
	adds r6, r6, r2
	ldrh r3, [r6]
	adds r7, r5, #0
	adds r7, #16
	cmp r3, #99
	bne .L_0803bf3e
	bl Resource_FindFreeEntry
	strh r0, [r6]
.L_0803bf3e:
	mov r3, r8
	ldrh r2, [r3, #12]
	movs r0, #255
	ldrh r3, [r3, #8]
	lsls r0, r0, #8
	adds r0, #254
	adds r3, r3, r0
	adds r2, r2, r3
	ldr r3, .L_0803bf74
	lsls r2, r2, #3
	adds r2, #4
	ands r2, r3
	ldrh r1, [r7, #6]
	ldr r3, .L_0803bf8c
	ands r3, r1
	mov r1, r8
	orrs r3, r2
	ldrb r2, [r1, #10]
	strh r3, [r7, #6]
	ldrb r3, [r1, #14]
	adds r2, #254
	adds r3, r3, r2
	lsls r3, r3, #3
	subs r3, #1
	strb r3, [r7, #4]
	b .L_0803c032
	.2byte 0x0000
.L_0803bf74:
	.4byte 0x000001ff
.L_0803bf78:
	.4byte 0x00000013
.L_0803bf7c:
	.4byte 0x0000031c
.L_0803bf80:
	.4byte Data_080385e0
.L_0803bf84:
	.4byte 0xfffff8d0
.L_0803bf88:
	.4byte 0xb6db6db7
.L_0803bf8c:
	.4byte 0xfffffe00
.L_0803bf90:
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #72
	adds r3, r6, r2
	ldrh r1, [r3]
	ldr r2, .L_0803bfe8
	add r1, r10
	lsls r1, r1, #5
	adds r1, r1, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	add r0, sp, #16
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldrb r2, [r6, #5]
	movs r3, #4
	adds r1, r5, #0
	ands r3, r2
	adds r1, #20
	cmp r3, #0
	beq .L_0803bfec
	ldr r0, [sp, #4]
	lsrs r3, r0, #1
	mov r0, r8
	ldrh r2, [r0, #14]
	add r3, r9
	lsls r2, r2, #3
	adds r3, r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #254
	adds r3, r3, r2
	ldr r2, .L_0803bfe4
	orrs r3, r2
	strh r3, [r1]
	adds r1, #2
	b .L_0803c008
	.2byte 0x0000
.L_0803bfe4:
	.4byte 0x00000400
.L_0803bfe8:
	.4byte 0x06010000
.L_0803bfec:
	ldr r0, [sp, #4]
	lsrs r3, r0, #1
	mov r0, r8
	ldrh r2, [r0, #14]
	add r3, r9
	lsls r2, r2, #3
	adds r3, r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #254
	adds r3, r3, r2
	strh r3, [r1]
	adds r1, r5, #0
	adds r1, #22
.L_0803c008:
	ldr r3, [sp, #8]
	ldr r0, [sp, #12]
	lsrs r2, r3, #1
	adds r2, r0, r2
	mov r0, r8
	ldrh r3, [r0, #12]
	adds r7, r5, #0
	lsls r3, r3, #3
	adds r2, r2, r3
	ldr r3, .L_0803c054
	adds r2, #2
	orrs r2, r3
	strh r2, [r1]
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #72
	adds r3, r6, r2
	ldrh r3, [r3]
	adds r7, #16
	add r3, r10
	strh r3, [r1, #2]
.L_0803c032:
	movs r3, #253
	strb r3, [r5, #15]
	ldrh r3, [r7, #6]
	movs r2, #0
	lsls r3, r3, #23
	lsrs r3, r3, #23
	strh r3, [r5, #6]
	str r2, [r5]
	ldrb r3, [r7, #4]
	mov r0, r8
	strh r3, [r5, #8]
	mov r3, r10
	strb r3, [r5, #14]
	adds r1, r5, #0
	bl RenderOutput_AppendToList
	b .L_0803c058
.L_0803c054:
	.4byte 0x00004000
.L_0803c058:
	mov r0, r11
.L_0803c05a:
	add sp, #144
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
