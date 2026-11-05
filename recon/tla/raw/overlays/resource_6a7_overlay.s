.syntax unified
	.thumb
	.section .text.x02008618,"ax",%progbits
	.balign 4
	.global Func_02000618
	.thumb_func
Func_02000618:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, .L_02008678
	subs r2, #172
	str r2, [r3]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r2, #241
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #90
	bne .L_0200866e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_SetBit
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r5, r2
	movs r2, #1
	strh r2, [r3]
	ldr r0, .L_0200867c
	movs r1, #4
	bl Func_02000af8
.L_0200866e:
	bl Func_02000b08
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_02008678:
	.4byte gMainRoutineWork
.L_0200867c:
	.4byte 0x0000010c
	.section .text.x020086b0,"ax",%progbits
	.balign 4
	.global Func_020006b0
	.thumb_func
Func_020006b0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r1
	movs r1, #240
	lsls r1, r1, #5
	sub sp, #28
	adds r1, #88
	movs r0, #220
	str r2, [sp, #20]
	bl HeapSlot_AllocFar
	movs r1, #0
	mov r9, r0
	ldr r0, .L_020088fc
	str r1, [sp, #8]
	bl Resource_GetTableEntry
	mov r1, r9
	bl Func_020009f0
	bl Resource_FindFreeEntry
	movs r1, #160
	mov r2, r9
	lsls r1, r1, #3
	str r0, [sp, #16]
	bl VramCache_Load
	movs r7, #192
	lsls r7, r7, #5
	movs r6, #128
	str r0, [sp, #12]
	movs r2, #0
	adds r7, #112
	lsls r6, r6, #5
	mov r8, r2
	add r7, r9
	add r6, r9
.L_02008706:
	ldr r2, [sp, #12]
	mov r1, r8
	movs r3, #3
	ands r3, r1
	lsls r3, r3, #3
	adds r3, r3, r2
	str r3, [sp, #0]
	movs r3, #128
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	lsls r3, r3, #23
	bl Func_02000b40
	ldrb r1, [r6, #9]
	movs r3, #0
	movs r2, #13
	mov r11, r3
	negs r2, r2
	movs r3, #250
	strh r3, [r6, #30]
	adds r3, r2, #0
	ands r1, r3
	ldrb r3, [r6, #5]
	movs r2, #32
	orrs r3, r2
	strb r3, [r6, #5]
	movs r3, #15
	ands r1, r3
	strb r1, [r6, #9]
	mov r0, r8
	movs r1, #6
	bl __divsi3
	mov r1, r10
	ldr r3, [r1]
	adds r5, r0, #0
	lsls r2, r5, #20
	adds r3, r3, r2
	movs r1, #6
	str r3, [r7]
	mov r0, r8
	bl __modsi3
	mov r2, r10
	ldr r3, [r2, #4]
	lsls r0, r0, #20
	subs r3, r3, r0
	str r3, [r7, #4]
	subs r5, #4
	ldr r3, [r2, #8]
	adds r6, #40
	str r3, [r7, #8]
	bl Random16Far
	lsls r3, r0, #1
	adds r3, r3, r0
	lsrs r3, r3, #1
	muls r3, r5
	str r3, [r7, #12]
	bl Random16Far
	movs r3, #128
	movs r2, #1
	lsls r3, r3, #10
	lsls r0, r0, #2
	add r8, r2
	adds r0, r0, r3
	mov r1, r11
	mov r3, r8
	str r0, [r7, #16]
	str r1, [r7, #20]
	str r1, [r7, #24]
	adds r7, #28
	cmp r3, #53
	ble .L_02008706
	movs r0, #192
	lsls r0, r0, #3
	bl Heap_AllocEwram
	movs r5, #128
	lsls r5, r5, #5
	mov r10, r0
	adds r0, r5, #0
	bl Heap_Alloc
	adds r5, r0, #0
	ldr r0, .L_02008900
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_020009f0
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #3
	adds r2, r5, #0
	mov r11, r0
	bl VramCache_Load
	str r0, [sp, #4]
	adds r0, r5, #0
	bl Heap_Release
	mov r5, r10
	movs r1, #95
	adds r5, #12
	mov r8, r1
.L_020087e0:
	bl Random16Far
	movs r2, #1
	lsls r0, r0, #5
	negs r2, r2
	lsrs r0, r0, #16
	add r8, r2
	negs r0, r0
	mov r3, r8
	str r0, [r5]
	adds r5, #16
	cmp r3, #0
	bge .L_020087e0
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_02008904
	ldr r1, .L_02008908
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r6, #0
.L_02008810:
	movs r3, #31
	ands r3, r6
	cmp r3, #0
	bne .L_0200881e
	movs r0, #145
	bl Func_02000b58
.L_0200881e:
	cmp r6, #60
	bne .L_0200883a
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02000b10
	movs r0, #90
	bl Func_02000b18
	movs r0, #141
	bl Func_02000b58
.L_0200883a:
	cmp r6, #105
	bne .L_0200885e
	ldr r1, [sp, #20]
	mov lr, r1
	.2byte 0xf800
	movs r2, #1
	str r2, [sp, #8]
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02000a50
	movs r0, #228
	bl Func_02000b58
.L_0200885e:
	cmp r6, #130
	bne .L_02008866
	movs r3, #1
	str r3, [sp, #8]
.L_02008866:
	ldr r1, [sp, #8]
	cmp r1, #0
	beq .L_020088ac
	movs r7, #192
	lsls r7, r7, #5
	movs r5, #128
	adds r7, #112
	lsls r5, r5, #5
	movs r2, #53
	add r7, r9
	add r5, r9
	mov r8, r2
.L_0200887e:
	ldr r3, [r7, #24]
	cmp r3, #59
	bhi .L_0200889c
	adds r0, r5, #0
	adds r1, r7, #0
	bl Func_02000b48
	adds r0, r7, #0
	movs r1, #63
	ldr r2, .L_0200890c
	bl Func_02000b50
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_0200889c:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r1, r8
	adds r5, #40
	adds r7, #28
	cmp r1, #0
	bge .L_0200887e
.L_020088ac:
	mov r5, r10
.L_020088ae:
	ldr r2, [r5, #12]
	cmp r2, #0
	bne .L_02008910
	movs r3, #128
	lsls r3, r3, #23
	str r3, [r5, #4]
	str r2, [r5, #8]
	bl Random16Far
	ldr r3, .L_020088f4
	lsls r0, r0, #7
	lsrs r0, r0, #16
	adds r0, #52
	ands r0, r3
	ldr r2, .L_020088f8
	ldrh r3, [r5, #6]
	ands r3, r2
	orrs r3, r0
	strh r3, [r5, #6]
	bl Random16Far
	ldrb r3, [r5, #9]
	movs r1, #13
	movs r2, #240
	lsls r0, r0, #6
	negs r1, r1
	orrs r3, r2
	lsrs r0, r0, #16
	adds r2, r1, #0
	adds r0, #16
	ands r3, r2
	strb r0, [r5, #4]
	strb r3, [r5, #9]
	b .L_02008910
	.2byte 0x0000
.L_020088f4:
	.4byte 0x000001ff
.L_020088f8:
	.4byte 0xfffffe00
.L_020088fc:
	.4byte 0x000001f2
.L_02008900:
	.4byte 0x000001e8
.L_02008904:
	.4byte Field_Palette685
.L_02008908:
	.4byte 0x050003e0
.L_0200890c:
	.4byte 0xffffd000
.L_02008910:
	ldr r3, [r5, #12]
	cmp r3, #0
	blt .L_02008934
	ldr r2, [sp, #4]
	asrs r3, r3, #2
	lsls r3, r3, #2
	adds r3, r2, r3
	ldr r2, .L_02008950
	ldr r1, .L_02008954
	ands r3, r2
	ldrh r2, [r5, #8]
	adds r0, r5, #0
	ands r2, r1
	orrs r2, r3
	strh r2, [r5, #8]
	movs r1, #100
	bl Func_02000a10
.L_02008934:
	ldr r3, [r5, #12]
	adds r3, #1
	str r3, [r5, #12]
	cmp r3, #31
	ble .L_02008958
	bl Random16Far
	lsls r3, r0, #3
	subs r3, r3, r0
	lsrs r3, r3, #16
	negs r3, r3
	str r3, [r5, #12]
	b .L_02008958
	.2byte 0x0000
.L_02008950:
	.4byte 0x000003ff
.L_02008954:
	.4byte 0xfffffc00
.L_02008958:
	movs r3, #190
	lsls r3, r3, #3
	adds r5, #16
	add r3, r10
	cmp r5, r3
	ble .L_020088ae
	movs r0, #1
	adds r6, #1
	bl wait
	cmp r6, #149
	bgt .L_02008972
	b .L_02008810
.L_02008972:
	mov r0, r11
	bl Resource_ResetEntry
	mov r0, r10
	bl Heap_Release
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl Func_02000a50
	ldr r0, [sp, #16]
	bl Resource_ResetEntry
	movs r0, #220
	bl HeapSlot_Release
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
