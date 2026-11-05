.syntax unified
	.thumb
	.section .text.x020080c6,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a3a4,"ax",%progbits
	.global Func_020023a4
	.thumb_func
Func_020023a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #134
	adds r5, r0, #0
	lsls r1, r1, #4
	movs r0, #220
	sub sp, #4
	bl HeapSlot_AllocFar
	adds r7, r0, #0
	adds r0, r5, #0
	bl Actor_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r1, #132
	movs r3, #0
	adds r2, #91
	lsls r1, r1, #4
	strb r3, [r2]
	adds r3, r7, r1
	str r6, [r3]
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #72
	adds r2, r7, r3
	ldr r3, [r6, #8]
	adds r1, #12
	str r3, [r2]
	adds r2, r7, r1
	ldr r3, [r6, #12]
	movs r1, #128
	str r3, [r2]
	movs r3, #133
	lsls r3, r3, #4
	adds r2, r7, r3
	ldr r3, [r6, #16]
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r2]
	adds r1, r7, #0
	ldr r0, .L_0200a520
	bl Func_02002824
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #4
	adds r2, r7, #0
	mov r11, r0
	bl VramCache_Load
	movs r2, #128
	lsls r2, r2, #4
	mov r10, r0
	adds r2, #90
	adds r3, r7, r2
	mov r1, r10
	strh r1, [r3]
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #88
	mov r2, r10
	adds r3, r7, r1
	adds r2, #32
	strh r2, [r3]
	movs r2, #208
	lsls r2, r2, #3
	movs r3, #128
	adds r2, r2, r7
	lsls r3, r3, #3
	movs r1, #15
	mov r8, r2
	adds r5, r7, r3
	mov r9, r1
.L_0200a444:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	adds r0, r5, #0
	bl Func_02002a0c
	adds r0, r6, #0
	bl Func_02002a2c
	subs r0, #2
	strh r0, [r5, #30]
	adds r0, r6, #0
	bl Func_02002a24
	ldrb r2, [r5, #9]
	movs r1, #13
	movs r3, #3
	negs r1, r1
	ands r0, r3
	adds r3, r1, #0
	ands r2, r3
	ldrb r3, [r5, #5]
	movs r1, #32
	orrs r3, r1
	lsls r0, r0, #2
	strb r3, [r5, #5]
	orrs r2, r0
	movs r3, #15
	ands r2, r3
	subs r3, #16
	strb r2, [r5, #9]
	add r9, r3
	mov r2, r8
	str r3, [r2, #24]
	mov r1, r9
	movs r3, #28
	adds r5, #40
	add r8, r3
	cmp r1, #0
	bge .L_0200a444
	movs r0, #145
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	lsls r0, r0, #1
	bl Func_0200286c
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #68
	adds r3, r7, r2
	str r0, [r3]
	mov r8, r0
	movs r1, #1
	bl Func_0200285c
	mov r0, r8
	movs r1, #0
	bl Actor_SetSingleChildFlags
	mov r2, r8
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r2, [r6, #80]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r2, #18]
	ldr r2, .L_0200a524
	ldr r3, [r6, #8]
	ldr r1, .L_0200a51c
	adds r3, r3, r2
	str r3, [r6, #8]
	adds r3, r6, #0
	adds r3, #85
	strb r1, [r3]
	movs r3, #128
	movs r1, #128
	lsls r3, r3, #4
	lsls r1, r1, #4
	movs r2, #128
	adds r1, #86
	adds r3, #84
	lsls r2, r2, #4
	movs r5, #0
	adds r6, r7, r3
	adds r2, #92
	adds r3, r7, r1
	strh r5, [r6]
	strh r5, [r3]
	adds r3, r7, r2
	strh r5, [r3]
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #94
	adds r2, r7, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #221
	b .L_0200a528
	.2byte 0x0000
.L_0200a51c:
	.4byte 0x00000000
.L_0200a520:
	.4byte gField_Tiles64D
.L_0200a524:
	.4byte 0xfff40000
.L_0200a528:
	bl Import_SoundPlay
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a588
	bl Task_Add
	movs r2, #186
	movs r1, #0
	ldrsh r3, [r6, r1]
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	beq .L_0200a560
.L_0200a544:
	movs r0, #1
	bl wait
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #84
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r3, r1
	bne .L_0200a544
.L_0200a560:
	ldr r0, .L_0200a588
	bl Task_RemoveFar
	mov r0, r8
	bl Func_02002874
	mov r0, r11
	bl Resource_ResetEntry
	movs r0, #220
	bl HeapSlot_Release
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a588:
	.4byte Func_02001fd0
	.section .text.x0200a58c,"ax",%progbits
	.global Func_0200258c
	.thumb_func
Func_0200258c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r2, #128
	lsls r2, r2, #4
	movs r3, #148
	adds r5, r7, r2
	lsls r3, r3, #5
	movs r2, #95
	adds r6, r7, r3
	mov r8, r2
.L_0200a5aa:
	ldr r3, [r5, #24]
	cmp r3, #15
	bhi .L_0200a5f4
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #130
	adds r1, r7, r2
	ldrh r1, [r1]
	movs r2, #7
	asrs r3, r3, #1
	ands r3, r2
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_0200a5ec
	ldr r2, .L_0200a5f0
	ands r1, r3
	ldrh r3, [r6, #8]
	adds r0, r6, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	adds r1, r5, #0
	bl Func_02002a14
	adds r0, r5, #0
	movs r1, #64
	movs r2, #0
	bl Func_02002a1c
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	b .L_0200a5f4
.L_0200a5ec:
	.4byte 0x000003ff
.L_0200a5f0:
	.4byte 0xfffffc00
.L_0200a5f4:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	adds r6, #40
	adds r5, #28
	cmp r2, #0
	bge .L_0200a5aa
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a6f4,"ax",%progbits
	.balign 4
	.global Func_020026f4
	.thumb_func
Func_020026f4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #136
	movs r0, #220
	sub sp, #4
	bl HeapSlot_AllocFar
	adds r6, r0, #0
	ldr r0, .L_0200a7a8
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02002824
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #4
	adds r2, r6, #0
	adds r5, r0, #0
	bl VramCache_Load
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #130
	mov r10, r0
	adds r3, r6, r1
	mov r2, r10
	adds r1, #2
	strh r2, [r3]
	adds r3, r6, r1
	strh r5, [r3]
	movs r2, #128
	movs r3, #148
	lsls r2, r2, #4
	lsls r3, r3, #5
	movs r1, #95
	adds r7, r6, r2
	adds r5, r6, r3
	mov r8, r1
.L_0200a74c:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02002a0c
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	movs r1, #13
	strb r3, [r5, #5]
	negs r1, r1
	movs r3, #15
	ands r3, r2
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	movs r3, #240
	strh r3, [r5, #30]
	subs r3, #241
	add r8, r3
	mov r2, r8
	str r3, [r7, #24]
	adds r5, #40
	adds r7, #28
	cmp r2, #0
	bge .L_0200a74c
	movs r1, #134
	lsls r1, r1, #6
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200a7ac
	bl Task_Add
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a7a8:
	.4byte 0x000001e1
.L_0200a7ac:
	.4byte Func_0200258c
