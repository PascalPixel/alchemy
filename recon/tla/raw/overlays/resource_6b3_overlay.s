.syntax unified
	.thumb
	.section .text.x02008098,"ax",%progbits
	.balign 4
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #128
	lsls r0, r0, #4
	sub sp, #4
	bl MemAlloc
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	mov r8, r0
	adds r3, #212
	ldr r0, .L_02008130
	ldr r1, .L_02008134
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_02008138
	ldr r6, .L_0200813c
	bl Resource_GetTableEntry
	mov r1, r8
	bl Func_020007ac
	ldr r5, .L_02008140
	bl Resource_FindFreeEntry
	movs r1, #128
	mov r2, r8
	str r0, [r5]
	lsls r1, r1, #4
	ldr r5, .L_02008144
	bl VramCache_Load
	movs r3, #192
	str r0, [r5]
	movs r1, #0
	str r0, [sp, #0]
	movs r2, #0
	adds r0, r6, #0
	lsls r3, r3, #24
	bl Func_02000834
	movs r3, #240
	strh r3, [r6, #30]
	ldrb r3, [r6, #9]
	movs r2, #13
	ldrb r1, [r6, #5]
	negs r2, r2
	ands r2, r3
	movs r3, #33
	negs r3, r3
	ands r3, r1
	strb r3, [r6, #5]
	movs r3, #15
	ands r2, r3
	movs r3, #224
	orrs r2, r3
	strb r2, [r6, #9]
	mov r0, r8
	bl MemFree
	ldr r5, .L_0200812c
	ldr r3, .L_02008148
	movs r1, #144
	strb r5, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0200814c
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	b .L_02008150
.L_0200812c:
	.4byte 0x00000000
.L_02008130:
	.4byte Data_02000874
.L_02008134:
	.4byte 0x050003c0
.L_02008138:
	.4byte 0x000001f8
.L_0200813c:
	.4byte gData_02000a00
.L_02008140:
	.4byte Data_020008f0
.L_02008144:
	.4byte Data_02000a30
.L_02008148:
	.4byte gData_02000a34
.L_0200814c:
	.4byte Func_0200005c
.L_02008150:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008244,"ax",%progbits
	.global Func_02000244
	.thumb_func
Func_02000244:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #28
	movs r4, #0
	movs r3, #0
	str r4, [sp, #24]
	str r4, [sp, #20]
	mov r9, r3
	ldr r3, .L_02008314
	movs r5, #128
	lsls r5, r5, #8
	add r4, sp, #12
	mov r8, r0
	adds r6, r1, #0
	mov r10, r2
	ands r5, r0
	add r2, sp, #20
	ldr r0, [r3]
	add r1, sp, #24
	add r3, sp, #16
	str r4, [sp, #0]
	bl UiText_GetResourceDimensions
	ldr r2, [sp, #16]
	movs r7, #0
	cmp r2, #24
	bgt .L_020082a4
	cmp r5, #0
	bne .L_02008292
	movs r3, #25
	subs r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r3, #5
	b .L_0200829c
.L_02008292:
	movs r3, #30
	subs r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
.L_0200829c:
	str r3, [sp, #24]
	ldr r3, [sp, #24]
	subs r4, r3, #5
	b .L_020082b2
.L_020082a4:
	movs r3, #30
	subs r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #24]
	adds r4, r3, #0
.L_020082b2:
	ldr r3, [sp, #12]
	subs r3, #1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r2, r3, #1
	cmp r6, #0
	bne .L_020082c8
	movs r3, #3
	str r3, [sp, #20]
	adds r7, r2, #1
	b .L_020082f2
.L_020082c8:
	cmp r6, #1
	bne .L_020082d8
	movs r3, #14
	subs r3, r3, r2
	str r3, [sp, #20]
	adds r3, r3, r2
	subs r7, r3, #2
	b .L_020082f2
.L_020082d8:
	cmp r6, #2
	bne .L_020082f2
	cmp r2, #3
	bne .L_020082ec
	movs r3, #13
	str r3, [sp, #20]
	movs r3, #2
	movs r7, #15
	mov r9, r3
	b .L_020082f2
.L_020082ec:
	movs r3, #16
	str r3, [sp, #20]
	movs r7, #15
.L_020082f2:
	mov r3, r10
	str r3, [sp, #4]
	mov r3, r9
	ldr r1, [sp, #24]
	ldr r2, [sp, #20]
	str r3, [sp, #8]
	mov r0, r8
	adds r3, r4, #0
	str r7, [sp, #0]
	bl Func_02000158
	add sp, #28
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008314:
	.4byte gData_02000a28
	.section .text.x02008318,"ax",%progbits
	.ifdef TLA_EDITION_DE
	.global Func_02000318
	.thumb_func
Func_02000318:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_de_020086a4
	movs r2, #139
	lsls r2, r2, #2
	adds r6, r3, r2
	subs r2, #24
	adds r3, r3, r2
	ldr r0, [r3]
	ldrb r7, [r6]
	bl Object_GetById
	movs r3, #0
	mov r8, r3
	mov r2, r8
	movs r5, #1
	adds r0, #85
	strb r2, [r0]
	strb r5, [r6]
	bl Func_02000844
	bl UiWork_InitializeWithResourceCounters
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #140
	strb r5, [r3, #6]
	adds r3, r3, r2
	strb r5, [r3]
	ldr r2, .L_de_020086a8
	ldr r3, .L_de_020086ac
	str r3, [r2]
	bl Func_02000098
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	movs r2, #40
	bl Func_02000244
	movs r0, #1
	movs r1, #1
	movs r2, #40
	bl Func_02000244
	movs r0, #6
	movs r1, #0
	movs r2, #40
	bl Func_02000244
	movs r0, #2
	movs r1, #1
	movs r2, #40
	bl Func_02000244
	movs r0, #2
	movs r1, #1
	movs r2, #40
	bl Func_02000244
	movs r0, #3
	movs r1, #0
	movs r2, #30
	bl Func_02000244
	movs r0, #7
	movs r1, #1
	movs r2, #30
	bl Func_02000244
	movs r0, #56
	movs r1, #0
	movs r2, #40
	bl Func_02000244
	movs r0, #60
	movs r1, #1
	movs r2, #30
	bl Func_02000244
	movs r0, #61
	movs r1, #1
	movs r2, #40
	bl Func_02000244
	movs r0, #0
	movs r1, #0
	movs r2, #40
	bl Func_02000244
	movs r0, #62
	movs r1, #1
	movs r2, #30
	bl Func_02000244
	movs r0, #0
	movs r1, #0
	movs r2, #30
	bl Func_02000244
	movs r0, #0
	movs r1, #0
	movs r2, #30
	bl Func_02000244
	movs r0, #5
	movs r1, #1
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #0
	movs r2, #30
	bl Func_02000244
	movs r1, #1
	movs r2, #30
	movs r0, #1
	bl Func_02000244
	movs r0, #67
	bl Func_0200086c
	ldr r5, .L_de_020086b0
	movs r3, #16
	strh r3, [r5, #14]
	strh r3, [r5, #10]
	ldr r1, .L_de_020086b4
	movs r2, #0
	ldr r0, .L_de_020086b8
	bl Func_0200084c
	movs r0, #100
	bl FieldEvent_WaitFrames
	movs r0, #60
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #61
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #0
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #62
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #1
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #5
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r1, #2
	movs r2, #40
	movs r0, #5
	bl Func_02000244
	movs r0, #120
	bl Func_02000854
	movs r0, #1
	bl WaitFrames
	bl Func_0200085c
	movs r0, #30
	bl FieldEvent_WaitFrames
	ldr r1, .L_de_020086b4
	movs r2, #120
	ldr r0, .L_de_020086bc
	bl Func_0200084c
	movs r0, #120
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #7
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #56
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #4
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #0
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #62
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #1
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #1
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #6
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #5
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #5
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #3
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #1
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r1, #2
	movs r2, #30
	movs r0, #1
	bl Func_02000244
	movs r0, #78
	bl Func_0200086c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #64
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #0
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #65
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #0
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #69
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #1
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	ldr r1, .L_de_020086b4
	movs r2, #0
	ldr r0, .L_de_020086c0
	bl Func_0200084c
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #73
	bl Func_0200086c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #64
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #64
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #65
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #67
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #62
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #70
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #68
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #56
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #1
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r1, #2
	movs r2, #40
	movs r0, #56
	bl Func_02000244
	movs r0, #200
	bl Func_02000854
	movs r0, #1
	bl WaitFrames
	bl Func_0200085c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #62
	movs r1, #2
	movs r2, #20
	bl Func_02000244
	movs r0, #0
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #66
	movs r1, #2
	movs r2, #20
	bl Func_02000244
	movs r0, #62
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #66
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	ldr r1, .L_de_020086b4
	movs r2, #0
	movs r0, #0
	bl Func_0200084c
	movs r0, #60
	bl FieldEvent_WaitFrames
	mov r3, r8
	mov r2, r8
	strh r3, [r5, #14]
	strh r2, [r5, #10]
	movs r0, #0
	bl Func_02000864
	ldr r0, .L_de_020086c4
	ldr r1, .L_de_020086b4
	movs r2, #0
	bl Func_0200084c
	movs r0, #60
	bl FieldEvent_WaitFrames
	b .L_de_020086ce
	.2byte 0x0000
.L_de_020086a4:
	.4byte gAdventure
.L_de_020086a8:
	.4byte gData_02000a28
.L_de_020086ac:
	.4byte 0x0000325c
.L_de_020086b0:
	.4byte gBgScroll
.L_de_020086b4:
	.4byte gMapCellBuffer
.L_de_020086b8:
	.4byte 0x00000079
.L_de_020086bc:
	.4byte 0x0000007a
.L_de_020086c0:
	.4byte 0x0000007b
.L_de_020086c4:
	.4byte 0x0000007d
.L_de_020086c8:
	movs r0, #1
	bl WaitFrames
.L_de_020086ce:
	ldr r3, .L_de_020087a4
	ldr r3, [r3, #4]
	cmp r3, #0
	beq .L_de_020086c8
	ldr r1, .L_de_020087a8
	movs r2, #0
	ldr r0, .L_de_020087ac
	bl Func_0200084c
	movs r0, #120
	bl FieldEvent_WaitFrames
	movs r0, #120
	bl Func_02000854
	movs r0, #1
	bl WaitFrames
	bl Func_0200085c
	movs r0, #180
	bl FieldEvent_WaitFrames
	ldr r1, .L_de_020087a8
	movs r2, #0
	ldr r0, .L_de_020087b0
	bl Func_0200084c
	movs r0, #150
	lsls r0, r0, #1
	bl FieldEvent_WaitFrames
	ldr r1, .L_de_020087a8
	movs r2, #0
	ldr r0, .L_de_020087b4
	bl Func_0200084c
	movs r0, #180
	bl FieldEvent_WaitFrames
	movs r0, #78
	bl Func_0200086c
	ldr r3, .L_de_020087b8
	movs r2, #139
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #150
	strb r7, [r3]
	lsls r0, r0, #1
	bl FieldEvent_WaitFrames
	ldr r3, .L_de_020087a4
	movs r5, #0
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_de_0200875a
.L_de_02008740:
	movs r0, #1
	bl WaitFrames
	movs r3, #168
	lsls r3, r3, #6
	adds r5, #1
	adds r3, #47
	cmp r5, r3
	bgt .L_de_0200875a
	ldr r3, .L_de_020087a4
	ldr r3, [r3, #4]
	cmp r3, #0
	beq .L_de_02008740
.L_de_0200875a:
	ldr r1, .L_de_020087a8
	movs r2, #0
	movs r0, #0
	bl Func_0200084c
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r2, #0
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r2, [r3]
	movs r2, #130
	lsls r2, r2, #5
	subs r3, #80
	strh r2, [r3]
	bl Func_020007cc
	bl Func_020007c4
	ldr r2, .L_de_020087bc
	movs r3, #1
	movs r0, #190
	strb r3, [r2]
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	ldr r0, .L_de_020087c0
	movs r1, #2
	bl Func_02000824
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_de_020087a4:
	.4byte gInput
.L_de_020087a8:
	.4byte gMapCellBuffer
.L_de_020087ac:
	.4byte 0x0000007e
.L_de_020087b0:
	.4byte 0x0000007f
.L_de_020087b4:
	.4byte 0x00000080
.L_de_020087b8:
	.4byte gAdventure
.L_de_020087bc:
	.4byte gRenderOamEnabled
.L_de_020087c0:
	.4byte 0x00000001
	.else
	.global Func_02000318
	.thumb_func
Func_02000318:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008668
	movs r2, #139
	lsls r2, r2, #2
	adds r6, r3, r2
	subs r2, #24
	adds r3, r3, r2
	ldr r0, [r3]
	ldrb r7, [r6]
	bl Object_GetById
	movs r3, #0
	mov r8, r3
	mov r2, r8
	movs r5, #1
	adds r0, #85
	strb r2, [r0]
	strb r5, [r6]
	bl Func_02000844
	bl UiWork_InitializeWithResourceCounters
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	.ifdef TLA_EDITION_JA
	movs r2, #136
	.else
	movs r2, #152
	.endif
	lsls r2, r2, #5
	adds r2, #140
	strb r5, [r3, #6]
	adds r3, r3, r2
	strb r5, [r3]
	ldr r2, .L_0200866c
	ldr r3, .L_02008670
	str r3, [r2]
	bl Func_02000098
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	movs r2, #40
	bl Func_02000244
	movs r0, #1
	movs r1, #1
	movs r2, #40
	bl Func_02000244
	movs r0, #6
	movs r1, #0
	movs r2, #40
	bl Func_02000244
	movs r0, #2
	movs r1, #1
	movs r2, #40
	bl Func_02000244
	movs r0, #3
	movs r1, #0
	movs r2, #30
	bl Func_02000244
	movs r0, #7
	movs r1, #1
	movs r2, #30
	bl Func_02000244
	movs r0, #56
	movs r1, #0
	movs r2, #40
	bl Func_02000244
	movs r0, #60
	movs r1, #1
	movs r2, #30
	bl Func_02000244
	movs r0, #61
	movs r1, #1
	movs r2, #40
	bl Func_02000244
	movs r0, #0
	movs r1, #0
	movs r2, #40
	bl Func_02000244
	movs r0, #62
	movs r1, #1
	movs r2, #30
	bl Func_02000244
	movs r0, #0
	movs r1, #0
	movs r2, #30
	bl Func_02000244
	movs r0, #0
	movs r1, #0
	movs r2, #30
	bl Func_02000244
	movs r0, #5
	movs r1, #1
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #0
	movs r2, #30
	bl Func_02000244
	movs r1, #1
	movs r2, #30
	movs r0, #1
	bl Func_02000244
	movs r0, #67
	bl Func_0200086c
	ldr r5, .L_02008674
	movs r3, #16
	strh r3, [r5, #14]
	strh r3, [r5, #10]
	ldr r1, .L_02008678
	movs r2, #0
	ldr r0, .L_0200867c
	bl Func_0200084c
	movs r0, #100
	bl FieldEvent_WaitFrames
	movs r0, #60
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #61
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #0
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #62
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #1
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r1, #2
	movs r2, #40
	movs r0, #5
	bl Func_02000244
	movs r0, #120
	bl Func_02000854
	movs r0, #1
	bl WaitFrames
	bl Func_0200085c
	movs r0, #30
	bl FieldEvent_WaitFrames
	ldr r1, .L_02008678
	movs r2, #120
	ldr r0, .L_02008680
	bl Func_0200084c
	movs r0, #120
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #7
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #56
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #4
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #0
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #62
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #1
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #1
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #6
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #5
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #3
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r1, #2
	movs r2, #30
	movs r0, #1
	bl Func_02000244
	movs r0, #78
	bl Func_0200086c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #64
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #0
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #65
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #0
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #69
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #1
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	ldr r1, .L_02008678
	movs r2, #0
	ldr r0, .L_02008684
	bl Func_0200084c
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #73
	bl Func_0200086c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #64
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #65
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #67
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #62
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #70
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #68
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #56
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #1
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r1, #2
	movs r2, #40
	movs r0, #56
	bl Func_02000244
	movs r0, #200
	bl Func_02000854
	movs r0, #1
	bl WaitFrames
	bl Func_0200085c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #62
	movs r1, #2
	movs r2, #20
	bl Func_02000244
	movs r0, #0
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	movs r0, #66
	movs r1, #2
	movs r2, #20
	bl Func_02000244
	movs r0, #62
	movs r1, #2
	movs r2, #30
	bl Func_02000244
	movs r0, #66
	movs r1, #2
	movs r2, #40
	bl Func_02000244
	ldr r1, .L_02008678
	movs r2, #0
	movs r0, #0
	bl Func_0200084c
	movs r0, #60
	bl FieldEvent_WaitFrames
	mov r3, r8
	mov r2, r8
	strh r3, [r5, #14]
	strh r2, [r5, #10]
	movs r0, #0
	bl Func_02000864
	ldr r0, .L_02008688
	ldr r1, .L_02008678
	movs r2, #0
	bl Func_0200084c
	movs r0, #60
	bl FieldEvent_WaitFrames
	b .L_02008692
	.2byte 0x0000
.L_02008668:
	.4byte gAdventure
.L_0200866c:
	.4byte gData_02000a28
.L_02008670:
	.4byte MsgFinallyWeVe
.L_02008674:
	.4byte gBgScroll
.L_02008678:
	.4byte gMapCellBuffer
.L_0200867c:
	.4byte 0x00000079
.L_02008680:
	.4byte 0x0000007a
.L_02008684:
	.4byte 0x0000007b
.L_02008688:
	.4byte 0x0000007d
.L_0200868c:
	movs r0, #1
	bl WaitFrames
.L_02008692:
	ldr r3, .L_02008768
	ldr r3, [r3, #4]
	cmp r3, #0
	beq .L_0200868c
	ldr r1, .L_0200876c
	movs r2, #0
	ldr r0, .L_02008770
	bl Func_0200084c
	movs r0, #120
	bl FieldEvent_WaitFrames
	movs r0, #120
	bl Func_02000854
	movs r0, #1
	bl WaitFrames
	bl Func_0200085c
	movs r0, #180
	bl FieldEvent_WaitFrames
	ldr r1, .L_0200876c
	movs r2, #0
	ldr r0, .L_02008774
	bl Func_0200084c
	movs r0, #150
	lsls r0, r0, #1
	bl FieldEvent_WaitFrames
	ldr r1, .L_0200876c
	movs r2, #0
	ldr r0, .L_02008778
	bl Func_0200084c
	movs r0, #180
	bl FieldEvent_WaitFrames
	movs r0, #78
	bl Func_0200086c
	ldr r3, .L_0200877c
	movs r2, #139
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #150
	strb r7, [r3]
	lsls r0, r0, #1
	bl FieldEvent_WaitFrames
	ldr r3, .L_02008768
	movs r5, #0
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_0200871e
.L_02008704:
	movs r0, #1
	bl WaitFrames
	movs r3, #168
	lsls r3, r3, #6
	adds r5, #1
	adds r3, #47
	cmp r5, r3
	bgt .L_0200871e
	ldr r3, .L_02008768
	ldr r3, [r3, #4]
	cmp r3, #0
	beq .L_02008704
.L_0200871e:
	ldr r1, .L_0200876c
	movs r2, #0
	movs r0, #0
	bl Func_0200084c
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r2, #0
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r2, [r3]
	movs r2, #130
	lsls r2, r2, #5
	subs r3, #80
	strh r2, [r3]
	bl Func_020007cc
	bl Func_020007c4
	ldr r2, .L_02008780
	movs r3, #1
	movs r0, #190
	strb r3, [r2]
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	ldr r0, .L_02008784
	movs r1, #2
	bl Func_02000824
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008768:
	.4byte gInput
.L_0200876c:
	.4byte gMapCellBuffer
.L_02008770:
	.4byte 0x0000007e
.L_02008774:
	.4byte 0x0000007f
.L_02008778:
	.4byte 0x00000080
.L_0200877c:
	.4byte gAdventure
.L_02008780:
	.4byte gRenderOamEnabled
.L_02008784:
	.4byte 0x00000001
	.endif
	.section .bss,"aw",%nobits
	.balign 16
	.global Data_020008f0
Data_020008f0:
	.space 0x00000110
	.global gData_02000a00
gData_02000a00:
	.space 0x00000028
	.global gData_02000a28
gData_02000a28:
	.space 0x00000004
	.global gData_02000a2c
gData_02000a2c:
	.space 0x00000004
	.global Data_02000a30
Data_02000a30:
	.space 0x00000004
	.global gData_02000a34
gData_02000a34:
	.space 0x00000004
	.global gData_02000a38
gData_02000a38:
