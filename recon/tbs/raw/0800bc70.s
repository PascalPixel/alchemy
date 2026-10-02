@ Uncredited resource creator, reconstructed from the current owned
@ TBS English ROM and current physical source operands. The sole ordinary
@ C attempt omits the unsupported dead-result construction and remains a draft.
	.syntax unified
	.thumb
	.text
	.balign 4
	.global ResourceObject_Create
	.type ResourceObject_Create, %function
	.thumb_func
ResourceObject_Create:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #0
	mov r8, r1
	mov r10, r0
	bl Resource_GetMetadataRecordFar
	adds r7, r0, #0
	bl Resource_FindFreeEntry
	ldr r3, .LResourceCreate0
	ldr r5, [r3]
	ldrb r3, [r7]
	adds r6, r0, #0
	movs r0, #0
	cmp r3, #0
	bne .LResourceCreate1
	b .LResourceCreate2
.LResourceCreate1:
	adds r3, r5, #0
	adds r3, #32
	ldrb r3, [r3]
	movs r2, #0
	b .LResourceCreate3
.LResourceCreate5:
	adds r2, #1
	adds r5, #56
	cmp r2, #63
	bgt .LResourceCreate4
	adds r3, r5, #0
	adds r3, #32
	ldrb r3, [r3]
.LResourceCreate3:
	cmp r3, #0
	bne .LResourceCreate5
	mov r8, r5
.LResourceCreate4:
	mov r2, r8
	movs r0, #0
	cmp r2, #0
	beq .LResourceCreate2
	cmp r6, #96
	beq .LResourceCreate2
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	bl VramBlock_LoadCached
	mov r12, r0
	cmp r0, #0
	bne .LResourceCreate6
	movs r0, #0
	b .LResourceCreate2
.LResourceCreate6:
	movs r3, #0
	mov r2, r8
	mov r1, r8
	strb r6, [r1, #28]
	strh r3, [r2, #30]
	adds r2, #38
	movs r3, #1
	strb r3, [r2]
	ldrb r3, [r7]
	ldrb r2, [r7, #1]
	lsls r3, r3, #8
	adds r0, r3, r2
	movs r3, #129
	lsls r3, r3, #5
	ldr r4, .LResourceCreate7
	cmp r0, r3
	beq .LResourceCreate8
	cmp r0, r3
	bhi .LResourceCreate9
	movs r3, #129
	movs r4, #128
	lsls r3, r3, #4
	lsls r4, r4, #8
	cmp r0, r3
	beq .LResourceCreate8
	cmp r0, r3
	bhi .LResourceCreate10
	subs r3, #8
	movs r4, #0
	b .LResourceCreate11
.LResourceCreate10:
	ldr r1, .LResourceCreate12
	movs r4, #128
	lsls r4, r4, #7
	cmp r0, r1
	beq .LResourceCreate8
	ldr r2, .LResourceCreate13
	movs r4, #128
	lsls r4, r4, #23
	cmp r0, r2
	beq .LResourceCreate8
	b .LResourceCreate14
.LResourceCreate9:
	movs r3, #129
	lsls r3, r3, #6
	ldr r4, .LResourceCreate15
	cmp r0, r3
	beq .LResourceCreate8
	cmp r0, r3
	bhi .LResourceCreate16
	subs r3, #48
	ldr r4, .LResourceCreate17
	cmp r0, r3
	beq .LResourceCreate8
	ldr r1, .LResourceCreate18
	movs r4, #128
	lsls r4, r4, #24
	cmp r0, r1
	beq .LResourceCreate8
	b .LResourceCreate14
.LResourceCreate16:
	ldr r2, .LResourceCreate19
	ldr r4, .LResourceCreate20
	cmp r0, r2
	beq .LResourceCreate8
	ldr r3, .LResourceCreate21
	movs r4, #192
	lsls r4, r4, #24
.LResourceCreate11:
	cmp r0, r3
	beq .LResourceCreate8
.LResourceCreate14:
	movs r4, #0
.LResourceCreate8:
	mov r2, r8
	movs r1, #0
	movs r3, #128
	stmia r2!, {r1}
	lsls r3, r3, #6
	orrs r4, r3
	movs r0, #128
	stmia r2!, {r4}
	lsls r0, r0, #4
	mov r3, r12
	orrs r3, r0
	stmia r2!, {r3}
	movs r3, #192
	stmia r2!, {r1}
	lsls r3, r3, #7
	stmia r2!, {r3}
	movs r1, #187
	ldr r3, .LResourceCreate22
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r3, [r3]
	lsrs r3, r3, #5
	orrs r3, r0
	str r3, [r2]
	adds r0, r5, #0
	mov r1, r10
	bl ResourceMetadata_Register
	movs r2, #1
	negs r2, r2
	mov r0, r8
.LResourceCreate2:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.LResourceCreate0:
	.4byte gSpriteObjects
.LResourceCreate7:
	.4byte 0x80008000
.LResourceCreate12:
	.4byte 0x00001008
.LResourceCreate13:
	.4byte 0x00001010
.LResourceCreate15:
	.4byte 0xc0008000
.LResourceCreate17:
	.4byte 0x80004000
.LResourceCreate18:
	.4byte 0x00002020
.LResourceCreate19:
	.4byte 0x00004020
.LResourceCreate20:
	.4byte 0xc0004000
.LResourceCreate21:
	.4byte 0x00004040
.LResourceCreate22:
	.4byte gVramBlockCache
	.size ResourceObject_Create, .-ResourceObject_Create
