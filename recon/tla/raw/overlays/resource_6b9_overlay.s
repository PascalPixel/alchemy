.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02000608
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_02000668
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_02000674
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020005c8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008064,"ax",%progbits
	.global Func_02000064
	.thumb_func
Func_02000064:
	push {lr}
	cmp r0, #1
	bne .L_02008074
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020005c8
.L_02008074:
	pop {pc}
	.2byte 0x0000
	.global Data_02000078
Data_02000078:
	.4byte 0x00004770
	.section .text.x0200807c,"ax",%progbits
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {lr}
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	pop {pc}
	.section .text.x02008088,"ax",%progbits
	.global Func_02000088
	.thumb_func
Func_02000088:
	push {r5, lr}
	sub sp, #8
	cmp r0, #1
	bne .L_020080b4
	movs r5, #34
	str r0, [sp, #4]
	movs r1, #1
	movs r0, #49
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl Func_020005b0
	movs r3, #3
	str r3, [sp, #4]
	movs r0, #49
	movs r1, #1
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl Func_020005a8
.L_020080b4:
	add sp, #8
	pop {r5, pc}
	.section .text.x020080b8,"ax",%progbits
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {lr}
	movs r0, #136
	lsls r0, r0, #2
	bl Func_02000588
	cmp r0, #0
	bne .L_020080d6
	movs r0, #136
	lsls r0, r0, #2
	bl Func_02000590
	movs r0, #14
	movs r1, #0
	bl Func_020005d8
.L_020080d6:
	pop {pc}
	.section .text.x020080d8,"ax",%progbits
	.global Func_020000d8
	.thumb_func
Func_020000d8:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	bl Func_0200029c
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	movs r3, #20
	bl Func_020001e0
	movs r0, #20
	bl WaitFrames
	bl Func_02000358
	pop {r5, pc}
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	ldr r0, .L_02008100
	bx lr
.L_02008100:
	.4byte Data_020006ec
	.section .text.x02008104,"ax",%progbits
	.global Func_02000104
	.thumb_func
Func_02000104:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r1, r2, r3
	subs r3, #172
	str r3, [r1]
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #55
	adds r3, r2, r1
	movs r1, #0
	strb r1, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #54
	adds r2, r2, r3
	ldr r3, .L_02008154
	strb r1, [r2]
	movs r1, #139
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #2
	strb r2, [r3]
	bl Func_020005e0
	movs r2, #11
	movs r1, #10
	movs r0, #0
	bl Func_020005e8
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02008154:
	.4byte gPartyState
	.section .text.x02008158,"ax",%progbits
	.global Func_02000158
	.thumb_func
Func_02000158:
	movs r0, #0
	bx lr
	.section .text.x0200815c,"ax",%progbits
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r2, #160
	lsls r2, r2, #3
	movs r3, #192
	adds r5, r7, r2
	lsls r3, r3, #4
	movs r2, #63
	adds r6, r7, r3
	mov r8, r2
.L_0200817a:
	ldr r3, [r5, #24]
	cmp r3, #19
	bhi .L_020081c8
	movs r2, #176
	lsls r2, r2, #5
	adds r2, #2
	adds r1, r7, r2
	ldrh r1, [r1]
	movs r2, #7
	asrs r3, r3, #2
	ands r3, r2
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_020081bc
	ldr r2, .L_020081c0
	ands r1, r3
	ldrh r3, [r6, #8]
	adds r0, r6, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	adds r1, r5, #0
	bl Func_020005f8
	adds r0, r5, #0
	movs r1, #63
	ldr r2, .L_020081c4
	bl Func_02000600
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	b .L_020081c8
.L_020081bc:
	.4byte 0x000003ff
.L_020081c0:
	.4byte 0xfffffc00
.L_020081c4:
	.4byte 0xffff8000
.L_020081c8:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	adds r6, #40
	adds r5, #28
	cmp r2, #0
	bge .L_0200817a
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020081e0,"ax",%progbits
	.global Func_020001e0
	.thumb_func
Func_020001e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r2, [sp, #0]
	str r0, [sp, #8]
	str r1, [sp, #4]
	adds r2, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r11, r3
	cmp r2, #0
	ble .L_0200828c
	adds r7, r2, #0
.L_02008208:
	bl Random16Far
	movs r1, #176
	lsls r1, r1, #5
	add r1, r11
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r10, r1
	lsls r6, r3, #3
	subs r6, r6, r3
	lsls r6, r6, #2
	movs r3, #160
	add r6, r11
	lsls r3, r3, #3
	adds r5, r6, r3
	movs r1, #0
	str r1, [r5, #24]
	ldr r2, [sp, #8]
	mov r8, r1
	str r2, [r5]
	ldr r3, [sp, #4]
	mov r9, r0
	str r3, [r5, #4]
	ldr r1, [sp, #0]
	subs r7, #1
	str r1, [r5, #8]
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #12
	lsls r0, r0, #3
	adds r0, r0, r2
	mov r1, r9
	adds r2, r5, #0
	bl Vector_AddPolarOffsetFar
	mov r3, r8
	str r3, [r5, #12]
	movs r3, #160
	lsls r3, r3, #11
	mov r1, r8
	str r3, [r5, #16]
	str r1, [r5, #20]
	bl Random16Far
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #12
	movs r2, #128
	adds r6, r6, r3
	lsls r2, r2, #10
	lsls r0, r0, #1
	adds r0, r0, r2
	mov r1, r9
	adds r2, r6, #0
	bl Vector_AddPolarOffsetFar
	mov r1, r10
	ldrh r3, [r1]
	movs r2, #63
	adds r3, #1
	ands r3, r2
	mov r2, r10
	strh r3, [r2]
	cmp r7, #0
	bne .L_02008208
.L_0200828c:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200829c,"ax",%progbits
	.global Func_0200029c
	.thumb_func
Func_0200029c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #8
	movs r0, #220
	sub sp, #4
	bl Runtime_AllocateHeapBlockFar
	adds r6, r0, #0
	ldr r0, .L_02008350
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02000560
	bl Resource_FindFreeEntry
	movs r1, #160
	lsls r1, r1, #3
	adds r2, r6, #0
	adds r5, r0, #0
	bl VramBlock_LoadCached
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #2
	mov r10, r0
	adds r3, r6, r1
	mov r2, r10
	adds r1, #2
	strh r2, [r3]
	adds r3, r6, r1
	strh r5, [r3]
	movs r2, #160
	movs r3, #192
	lsls r2, r2, #3
	lsls r3, r3, #4
	movs r1, #63
	adds r7, r6, r2
	adds r5, r6, r3
	mov r8, r1
.L_020082f4:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_020005f0
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
	bge .L_020082f4
	movs r1, #176
	lsls r1, r1, #5
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_02008354
	bl Func_02000530
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008350:
	.4byte 0x000001f0
.L_02008354:
	.4byte Func_0200015c
	.section .text.x02008358,"ax",%progbits
	.global Func_02000358
	.thumb_func
Func_02000358:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, .L_02008380
	ldr r5, [r3]
	bl Scheduler_RemoveCallbackFar
	movs r3, #176
	lsls r3, r3, #5
	adds r3, #4
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Func_02000568
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	pop {r5, pc}
.L_02008380:
	.4byte Func_0200015c
	.section .text.x02008384,"ax",%progbits
	.global Func_02000384
	.thumb_func
Func_02000384:
	push {lr}
	sub sp, #8
	adds r4, r3, #0
	cmp r0, #1
	bne .L_020083a2
	ldr r3, [sp, #16]
	adds r0, r1, #0
	str r3, [sp, #0]
	ldr r3, [sp, #20]
	adds r1, r2, #0
	str r3, [sp, #4]
	adds r2, r4, #0
	ldr r3, [sp, #12]
	bl Func_020005a0
.L_020083a2:
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x020083a8,"ax",%progbits
	.global Func_020003a8
	.thumb_func
Func_020003a8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r1, #0
	mov r10, r0
	adds r0, r6, #0
	adds r7, r2, #0
	mov r8, r3
	bl Object_GetById
	mov r2, r10
	adds r5, r0, #0
	cmp r2, #1
	bne .L_020083d8
	mov r3, r8
	lsls r2, r3, #16
	lsls r1, r7, #16
	adds r0, r6, #0
	bl Func_020005c8
	movs r3, #0
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_020083d8:
	mov r2, r10
	cmp r2, #2
	bne .L_0200840e
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #9
	cmp r3, r2
	bge .L_02008406
.L_020083e8:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #24]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #30
	adds r3, r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	str r3, [r5, #24]
	str r3, [r5, #28]
	cmp r3, r2
	ble .L_020083e8
.L_02008406:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_0200840e:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008418,"ax",%progbits
	.global Func_02000418
	.thumb_func
Func_02000418:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r1, #0
	mov r10, r0
	adds r0, r7, #0
	mov r8, r3
	adds r5, r2, #0
	bl Object_GetById
	mov r3, r10
	adds r6, r0, #0
	cmp r3, #2
	bne .L_0200845e
	mov r3, r8
	lsls r2, r3, #16
	adds r0, r7, #0
	lsls r1, r5, #16
	bl Func_020005c8
	movs r3, #128
	lsls r3, r3, #14
	adds r2, r6, #0
	str r3, [r6, #12]
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r3, #184
	lsls r3, r3, #5
	adds r3, #10
	str r3, [r6, #72]
	movs r0, #50
	bl WaitFrames
.L_0200845e:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008468,"ax",%progbits
	.global Func_02000468
	.thumb_func
Func_02000468:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	adds r7, r1, #0
	mov r8, r2
	mov r10, r3
	cmp r6, #1
	bne .L_0200849e
	movs r0, #177
	lsls r3, r2, #16
	lsls r1, r7, #16
	lsls r0, r0, #1
	movs r2, #0
	bl Func_02000598
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200849e
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_0200849e:
	cmp r6, #2
	bne .L_0200851a
	mov r2, r8
	lsls r3, r2, #16
	lsls r1, r7, #16
	movs r0, #252
	movs r2, #0
	bl Func_02000598
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200851a
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_020084cc:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #24]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #30
	adds r2, r2, r3
	ldrh r3, [r5, #6]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r5, #6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	str r2, [r5, #24]
	str r2, [r5, #28]
	cmp r2, r3
	ble .L_020084cc
	adds r3, #1
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r3, .L_02008524
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	strh r3, [r5, #6]
	mov r0, r10
	ldr r1, [sp, #24]
	bl Func_020005d8
.L_0200851a:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008524:
	.4byte gPartyState
	.section .rodata.x02008608,"a",%progbits
	.global Data_02000608
Data_02000608:
	.4byte 0xffff0000
	.4byte 0x00000198
	.4byte 0x40000058
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0001
	.4byte 0x00000198
	.4byte 0xc0000198
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0002
	.4byte 0x000001f8
	.4byte 0x400000e8
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000668
Data_02000668:
	.4byte 0x00000142
	.4byte 0x01402142
	.4byte 0x000001ff
	.global Data_02000674
Data_02000674:
	.4byte 0xffff0132
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020006ec
Data_020006ec:
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020000d8
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_020000b8
	.4byte 0x00002115
	.4byte 0xffff0008
	.4byte Func_02000054
	.4byte 0x50008615
	.4byte 0xffff0008
	.4byte Func_02000064
	.4byte 0x00008715
	.4byte 0xffff0009
	.4byte Data_02000078 + 0x1
	.4byte 0x00009815
	.4byte 0xffff0009
	.4byte Func_0200007c
	.4byte 0x00008515
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte Func_02000088
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
