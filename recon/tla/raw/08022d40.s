.syntax unified
	.thumb
	.global Func_08022d40
	.thumb_func
Func_08022d40:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r2, #0
	mov r8, r2
	mov r10, r0
	bl Resource_GetMetadataRecordFar
	adds r7, r0, #0
	bl Resource_FindFreeEntry
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #16]
	ldrb r3, [r7]
	adds r6, r0, #0
	movs r0, #0
	cmp r3, #0
	bne .L_08022d6a
	b .L_08022e74
.L_08022d6a:
	ldrb r3, [r5, #20]
	movs r2, #0
	b .L_08022d7a
.L_08022d70:
	adds r2, #1
	adds r5, #56
	cmp r2, #63
	bgt .L_08022d80
	ldrb r3, [r5, #20]
.L_08022d7a:
	cmp r3, #0
	bne .L_08022d70
	mov r8, r5
.L_08022d80:
	mov r3, r8
	movs r0, #0
	cmp r3, #0
	beq .L_08022e74
	cmp r6, #96
	beq .L_08022e74
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	bl VramBlock_LoadCached
	mov r12, r0
	cmp r0, #0
	bne .L_08022da0
	movs r0, #0
	b .L_08022e74
.L_08022da0:
	mov r2, r8
	movs r3, #0
	strh r3, [r2, #18]
	movs r3, #1
	strb r3, [r2, #26]
	ldrb r3, [r7]
	strb r6, [r2, #16]
	ldrb r2, [r7, #1]
	lsls r3, r3, #8
	adds r0, r3, r2
	movs r3, #129
	lsls r3, r3, #5
	ldr r1, .L_08022e7c
	cmp r0, r3
	beq .L_08022e2e
	cmp r0, r3
	bhi .L_08022df2
	movs r3, #129
	movs r1, #128
	lsls r3, r3, #4
	lsls r1, r1, #8
	cmp r0, r3
	beq .L_08022e2e
	cmp r0, r3
	bhi .L_08022dd8
	subs r3, #8
	movs r1, #0
	b .L_08022e28
.L_08022dd8:
	movs r2, #128
	lsls r2, r2, #5
	movs r1, #128
	adds r2, #8
	lsls r1, r1, #7
	cmp r0, r2
	beq .L_08022e2e
	movs r3, #128
	lsls r3, r3, #5
	movs r1, #128
	adds r3, #16
	lsls r1, r1, #23
	b .L_08022e28
.L_08022df2:
	movs r3, #129
	lsls r3, r3, #6
	ldr r1, .L_08022e80
	cmp r0, r3
	beq .L_08022e2e
	cmp r0, r3
	bhi .L_08022e12
	movs r2, #128
	lsls r2, r2, #6
	adds r2, #16
	ldr r1, .L_08022e84
	cmp r0, r2
	beq .L_08022e2e
	movs r1, #128
	subs r3, #32
	b .L_08022e26
.L_08022e12:
	movs r2, #128
	lsls r2, r2, #7
	adds r2, #32
	ldr r1, .L_08022e88
	cmp r0, r2
	beq .L_08022e2e
	movs r3, #128
	lsls r3, r3, #7
	movs r1, #192
	adds r3, #64
.L_08022e26:
	lsls r1, r1, #24
.L_08022e28:
	cmp r0, r3
	beq .L_08022e2e
	movs r1, #0
.L_08022e2e:
	mov r4, r8
	movs r2, #0
	stmia r4!, {r2}
	movs r3, #128
	lsls r3, r3, #6
	orrs r1, r3
	stmia r4!, {r1}
	movs r1, #128
	lsls r1, r1, #4
	mov r3, r12
	orrs r3, r1
	str r3, [r4]
	mov r4, r8
	adds r4, #28
	movs r3, #192
	str r2, [r4]
	lsls r3, r3, #7
	mov r2, r8
	str r3, [r2, #32]
	ldr r3, .L_08022e8c
	movs r2, #187
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	mov r2, r8
	lsrs r3, r3, #5
	orrs r3, r1
	str r3, [r2, #36]
	adds r0, r5, #0
	mov r1, r10
	bl ResourceMetadata_Register
	movs r3, #1
	negs r3, r3
	mov r0, r8
.L_08022e74:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_08022e7c:
	.4byte 0x80008000
.L_08022e80:
	.4byte 0xc0008000
.L_08022e84:
	.4byte 0x80004000
.L_08022e88:
	.4byte 0xc0004000
.L_08022e8c:
	.4byte ResourceTableEntries
