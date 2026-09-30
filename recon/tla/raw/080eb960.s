.syntax unified
	.thumb
	.global Func_080eb960
	.thumb_func
Func_080eb960:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #168
	lsls r1, r1, #5
	adds r5, r0, #0
	movs r0, #92
	sub sp, #48
	bl Runtime_AllocateHeapBlock
	str r0, [sp, #32]
	adds r0, r5, #0
	bl ObjectTable_Get
	mov r8, r0
	bl Func_080db9cc
	str r0, [sp, #12]
	mov r1, r8
	ldrh r1, [r1, #6]
	ldr r0, .L_080ebb04
	str r1, [sp, #8]
	bl Resource_GetTableEntry
	ldr r1, [sp, #32]
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r5, #128
	lsls r5, r5, #3
	adds r1, r5, #0
	ldr r2, [sp, #32]
	str r0, [sp, #24]
	bl VramBlock_LoadCached
	ldr r3, [sp, #32]
	movs r1, #176
	str r0, [sp, #20]
	movs r2, #0
	lsls r1, r1, #4
	adds r7, r3, r5
	mov r10, r2
	movs r6, #15
	adds r5, r3, r1
.L_080eb9c2:
	ldr r2, [sp, #20]
	mov r3, r10
	ands r3, r6
	lsls r3, r3, #1
	adds r3, r2, r3
	str r3, [sp, #0]
	movs r1, #4
	adds r0, r5, #0
	movs r2, #4
	movs r3, #0
	bl Func_080eaf98
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r5, #9]
	mov r0, r8
	bl Func_080db9cc
	strh r0, [r5, #30]
	mov r0, r8
	bl Func_080db9c0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r5, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	lsls r0, r0, #2
	orrs r3, r0
	strb r3, [r5, #9]
	movs r2, #1
	movs r3, #1
	negs r3, r3
	add r10, r2
	str r3, [r7, #24]
	mov r3, r10
	adds r5, #40
	adds r7, #28
	cmp r3, #63
	ble .L_080eb9c2
	mov r1, r8
	movs r3, #0
	str r3, [r1, #24]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r1, #28]
	mov r0, r8
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	movs r3, #0
	movs r2, #1
	str r2, [sp, #16]
	str r3, [sp, #28]
	mov r9, r3
	mov r11, r3
.L_080eba40:
	ldr r1, [sp, #16]
	cmp r1, #0
	beq .L_080ebaba
	mov r10, r1
.L_080eba48:
	bl Random16
	adds r6, r0, #0
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r2, #192
	mov r1, r9
	lsls r2, r2, #12
	lsls r5, r5, #1
	adds r5, r5, r2
	lsls r3, r1, #3
	ldr r2, [sp, #32]
	subs r3, r3, r1
	lsls r3, r3, #2
	movs r1, #128
	adds r3, r2, r3
	lsls r1, r1, #3
	adds r7, r3, r1
	movs r3, #0
	str r3, [r7, #24]
	mov r2, r8
	ldr r3, [r2, #8]
	movs r1, #160
	str r3, [r7]
	lsls r1, r1, #13
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r7, #4]
	ldr r3, [r2, #16]
	str r3, [r7, #8]
	bl Random16
	adds r2, r7, #0
	adds r1, r6, #0
	bl Func_0801489c
	movs r2, #1
	add r9, r2
	mov r3, r9
	str r6, [r7, #12]
	str r5, [r7, #20]
	cmp r3, #0
	bge .L_080ebaa4
	adds r3, #63
.L_080ebaa4:
	movs r2, #1
	asrs r3, r3, #6
	negs r2, r2
	lsls r3, r3, #6
	mov r1, r9
	add r10, r2
	subs r1, r1, r3
	mov r3, r10
	mov r9, r1
	cmp r3, #0
	bne .L_080eba48
.L_080ebaba:
	ldr r1, [sp, #32]
	movs r2, #128
	movs r3, #176
	lsls r2, r2, #3
	lsls r3, r3, #4
	adds r7, r1, r2
	adds r4, r1, r3
	movs r1, #63
	mov r10, r1
.L_080ebacc:
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_080ebb7e
	adds r6, r4, #0
	cmp r3, #49
	bhi .L_080ebb70
	ldr r3, [r7]
	add r5, sp, #36
	ldr r1, [r7, #12]
	ldr r0, [r7, #20]
	str r3, [r5]
	ldr r3, [r7, #4]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r7, #8]
	str r4, [sp, #4]
	str r3, [r5, #8]
	bl Func_0801489c
	mov r1, r8
	ldr r2, [r5, #8]
	ldr r3, [r1, #16]
	ldr r4, [sp, #4]
	cmp r2, r3
	ble .L_080ebb08
	ldr r3, [sp, #12]
	adds r3, #2
	b .L_080ebb0c
.L_080ebb04:
	.4byte 0x000001ea
.L_080ebb08:
	ldr r3, [sp, #12]
	subs r3, #2
.L_080ebb0c:
	strh r3, [r4, #30]
	adds r0, r5, #0
	str r4, [sp, #4]
	bl Func_080dc390
	ldr r3, [r5]
	ldr r1, .L_080ebb54
	str r3, [r6, #12]
	adds r0, r6, #0
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	ldr r2, [r7, #24]
	movs r3, #3
	ands r2, r3
	ldr r3, [sp, #20]
	lsls r2, r2, #1
	adds r2, r3, r2
	ldr r3, .L_080ebb58
	ands r2, r3
	ldrh r3, [r6, #8]
	ands r3, r1
	orrs r3, r2
	strh r3, [r6, #8]
	bl Func_080eb01c
	ldr r3, [r7, #12]
	movs r1, #128
	lsls r1, r1, #4
	adds r3, r3, r1
	str r3, [r7, #12]
	ldr r2, .L_080ebb5c
	ldr r3, [r7, #20]
	ldr r1, .L_080ebb60
	adds r3, r3, r2
	b .L_080ebb64
	.2byte 0x0000
.L_080ebb54:
	.4byte 0xfffffc00
.L_080ebb58:
	.4byte 0x000003ff
.L_080ebb5c:
	.4byte 0xffffcccd
.L_080ebb60:
	.4byte 0xffffc000
.L_080ebb64:
	str r3, [r7, #20]
	ldr r3, [r7, #4]
	ldr r4, [sp, #4]
	adds r3, r3, r1
	str r3, [r7, #4]
	ldr r3, [r7, #24]
.L_080ebb70:
	adds r3, #1
	str r3, [r7, #24]
	cmp r3, #50
	bne .L_080ebb7e
	movs r3, #1
	negs r3, r3
	str r3, [r7, #24]
.L_080ebb7e:
	movs r2, #1
	negs r2, r2
	add r10, r2
	mov r3, r10
	adds r4, #40
	adds r7, #28
	cmp r3, #0
	bge .L_080ebacc
	ldr r1, [sp, #28]
	cmp r1, #0
	beq .L_080ebb9a
	cmp r1, #1
	beq .L_080ebbac
	b .L_080ebbfa
.L_080ebb9a:
	mov r2, r11
	cmp r2, #10
	bne .L_080ebbfa
	movs r1, #1
	movs r3, #1
	negs r1, r1
	str r3, [sp, #28]
	mov r11, r1
	b .L_080ebbfa
.L_080ebbac:
	mov r2, r11
	cmp r2, #63
	bgt .L_080ebbcc
	mov r1, r8
	ldrh r3, [r1, #6]
	ldr r2, .L_080ebc2c
	adds r3, r3, r2
	mov r2, r11
	strh r3, [r1, #6]
	lsls r3, r2, #10
	movs r2, #128
	lsls r2, r2, #9
	str r3, [r1, #24]
	cmp r3, r2
	ble .L_080ebbcc
	str r2, [r1, #24]
.L_080ebbcc:
	mov r3, r11
	cmp r3, #64
	bne .L_080ebbe2
	add r1, sp, #8
	ldrh r1, [r1]
	mov r2, r8
	strh r1, [r2, #6]
	mov r0, r8
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26Far
.L_080ebbe2:
	mov r2, r11
	cmp r2, #10
	bne .L_080ebbec
	movs r3, #0
	str r3, [sp, #16]
.L_080ebbec:
	mov r1, r11
	cmp r1, #74
	bne .L_080ebbfa
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	str r2, [sp, #28]
.L_080ebbfa:
	movs r0, #1
	bl WaitFrames
	movs r2, #186
	ldr r1, [sp, #28]
	lsls r2, r2, #2
	movs r3, #1
	adds r2, #255
	add r11, r3
	cmp r1, r2
	beq .L_080ebc12
	b .L_080eba40
.L_080ebc12:
	ldr r0, [sp, #24]
	bl Func_08014274
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ebc2c:
	.4byte 0xfffff000
