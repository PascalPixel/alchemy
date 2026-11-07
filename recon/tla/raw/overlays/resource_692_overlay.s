.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009828,"ax",%progbits
	.global Func_02001828
	.thumb_func
Func_02001828:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, .L_02009a9c
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r7, r0
	ldr r0, [r5]
	sub sp, #92
	bl Actor_GetById
	ldrh r3, [r0, #6]
	movs r1, #128
	lsls r1, r1, #6
	adds r1, r3, r1
	movs r3, #192
	lsls r3, r3, #8
	movs r2, #0
	ands r1, r3
	str r1, [sp, #36]
	str r2, [sp, #32]
	str r2, [sp, #28]
	str r2, [sp, #24]
	mov r10, r0
	mov r11, r2
	mov r9, r2
	mov r8, r2
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	mov r3, r10
	ldr r1, [r3, #8]
	ldr r0, .L_02009aa0
	movs r3, #128
	lsls r3, r3, #12
	ands r1, r0
	add r6, sp, #80
	adds r1, r1, r3
	str r1, [r6]
	mov r4, r10
	ldr r2, [r4, #16]
	asrs r1, r1, #16
	ands r2, r0
	adds r2, r2, r3
	str r2, [r6, #8]
	asrs r2, r2, #16
	ldr r0, [r5]
	bl Actor_SetPositionAndReset
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #146
	lsls r0, r0, #2
	bl Func_02004f30
	mov r0, r10
	adds r0, #35
	str r0, [sp, #20]
	movs r3, #191
	ldrb r2, [r0]
	ands r3, r2
	strb r3, [r0]
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02004d10
	movs r0, #107
	bl Func_02004f30
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_02009aa4
	cmp r2, r3
	bne .L_02009948
	movs r0, #20
	movs r3, #26
	movs r4, #160
	str r3, [sp, #24]
	mov r9, r0
	movs r2, #84
	mov r0, r11
	lsls r4, r4, #17
	movs r3, #212
	str r2, [sp, #28]
	str r4, [sp, #32]
	lsls r3, r3, #17
	str r0, [r6, #4]
	movs r0, #192
	movs r1, #89
	str r3, [r6, #8]
	lsls r0, r0, #2
	mov r8, r1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009914
	movs r1, #212
	mov r11, r1
.L_02009914:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r2, #0
	str r2, [sp, #36]
	movs r1, #15
	movs r2, #13
	movs r3, #64
	movs r0, #0
	bl Func_02000694
	movs r0, #15
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r6, #0
.L_02009936:
	adds r0, r6, #0
	adds r0, #9
	movs r1, #1
	adds r6, #1
	bl Actor_SetModeById
	cmp r6, #7
	bne .L_02009936
	b .L_02009a46
.L_02009948:
	ldr r1, .L_02009a9c
	movs r4, #240
	lsls r4, r4, #1
	adds r3, r1, r4
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009aa8
	cmp r2, r3
	bne .L_020099b8
	movs r3, #92
	movs r0, #224
	str r3, [sp, #28]
	lsls r0, r0, #17
	movs r3, #0
	movs r4, #21
	str r0, [sp, #32]
	str r4, [sp, #24]
	movs r0, #192
	str r3, [r6, #4]
	movs r3, #172
	lsls r3, r3, #17
	lsls r0, r0, #2
	movs r1, #28
	movs r2, #83
	str r3, [r6, #8]
	adds r0, #1
	mov r9, r1
	mov r8, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200998c
	movs r1, #212
	mov r11, r1
.L_0200998c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_ClearBit
	movs r1, #24
	movs r2, #15
	movs r3, #64
	movs r0, #0
	bl Func_02000694
	movs r1, #9
	movs r0, #0
	movs r2, #16
	movs r3, #64
	bl Func_02000694
	movs r0, #11
	movs r1, #4
	bl Actor_SetModeById
	b .L_02009a46
.L_020099b8:
	ldr r3, .L_02009aac
	cmp r2, r3
	bne .L_02009a46
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #8
	bgt .L_02009a0a
	movs r3, #13
	movs r4, #198
	str r3, [sp, #24]
	movs r0, #49
	movs r3, #0
	movs r2, #113
	lsls r4, r4, #18
	str r2, [sp, #28]
	str r4, [sp, #32]
	mov r9, r0
	str r3, [r6, #4]
	movs r0, #192
	movs r3, #216
	lsls r3, r3, #16
	lsls r0, r0, #2
	movs r1, #75
	str r3, [r6, #8]
	adds r0, #2
	mov r8, r1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020099fe
	movs r0, #212
	mov r11, r0
.L_020099fe:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
	b .L_02009a46
.L_02009a0a:
	movs r3, #115
	movs r0, #206
	str r3, [sp, #28]
	lsls r0, r0, #18
	movs r3, #0
	movs r4, #42
	str r0, [sp, #32]
	str r4, [sp, #24]
	movs r0, #192
	str r3, [r6, #4]
	movs r3, #170
	lsls r3, r3, #18
	lsls r0, r0, #2
	movs r1, #51
	movs r2, #104
	str r3, [r6, #8]
	adds r0, #2
	mov r9, r1
	mov r8, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009a3c
	movs r1, #212
	mov r11, r1
.L_02009a3c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
.L_02009a46:
	movs r0, #181
	bl Func_02004f30
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004d10
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
	mov r2, r8
	subs r2, #1
	str r2, [sp, #16]
	movs r3, #3
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #16]
	mov r0, r9
	mov r1, r8
	mov r2, r9
	bl Func_02004cd8
	mov r3, r11
	cmp r3, #0
	bne .L_02009ab0
	bl Func_02004d18
	movs r0, #20
	bl FieldEvent_Wait
	b .L_02009b90
	.2byte 0x0000
.L_02009a9c:
	.4byte gMainRoutineWork
.L_02009aa0:
	.4byte 0xfff00000
.L_02009aa4:
	.4byte 0x000000e3
.L_02009aa8:
	.4byte 0x000000e4
.L_02009aac:
	.4byte 0x000000e6
.L_02009ab0:
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_02004d10
	movs r3, #148
	add r7, sp, #40
	adds r3, #255
	strh r3, [r7, #24]
	movs r3, #2
	str r3, [r7]
	movs r3, #10
	str r3, [r7, #4]
	ldr r3, .L_02009b78
	movs r4, #0
	str r3, [r7, #28]
	mov r8, r4
.L_02009ad8:
	movs r6, #0
.L_02009ada:
	bl Random16Far
	movs r1, #48
	add r5, sp, #80
	bl __umodsi3
	mov r2, r8
	lsls r3, r2, #18
	ldr r2, [r5, #8]
	ldr r1, [sp, #32]
	adds r2, r2, r3
	movs r3, #184
	lsls r3, r3, #13
	adds r2, r2, r3
	lsls r0, r0, #16
	movs r3, #128
	adds r0, r1, r0
	lsls r3, r3, #9
	str r0, [r5]
	ldr r1, [r5, #4]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #204
	lsls r3, r3, #14
	str r3, [sp, #8]
	adds r6, #1
	movs r3, #0
	str r7, [sp, #12]
	bl Func_0200015c
	cmp r6, #6
	bne .L_02009ada
	movs r0, #3
	bl wait
	mov r4, r8
	lsrs r3, r4, #1
	movs r2, #3
	adds r3, #1
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #75
	movs r1, #64
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	bl Func_02004cd8
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #10
	bne .L_02009ad8
	movs r6, #0
.L_02009b46:
	ldr r3, .L_02009b70
	lsrs r1, r6, #2
	movs r0, #128
	subs r3, r3, r1
	lsls r2, r1, #8
	lsls r0, r0, #19
	orrs r2, r3
	adds r0, #82
	strh r2, [r0]
	ldr r3, .L_02009b74
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r0, #3
	adds r6, #1
	bl wait
	cmp r6, #64
	bne .L_02009b46
	b .L_02009b7c
.L_02009b70:
	.4byte 0x00000010
.L_02009b74:
	.4byte 0x00003f44
.L_02009b78:
	.4byte gData_0200502c
.L_02009b7c:
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #78
	movs r1, #64
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	bl Func_02004cd8
.L_02009b90:
	ldr r5, .L_02009c94
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_02009c98
	cmp r2, r3
	bne .L_02009bb2
	movs r0, #15
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_02009bb2:
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02004f30
	movs r0, #181
	bl Func_02004f30
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004d10
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
	movs r3, #3
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #16]
	movs r1, #63
	mov r2, r9
	movs r0, #72
	bl Func_02004cd8
	bl Func_02004d18
	movs r0, #146
	lsls r0, r0, #2
	bl Func_02004f30
	ldr r0, [sp, #20]
	movs r3, #64
	ldrb r2, [r0]
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #133
	lsls r1, r1, #2
	adds r6, r5, r1
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	mov r2, r10
	ldr r3, [r2, #8]
	ldr r1, .L_02009c9c
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	add r5, sp, #80
	adds r3, r3, r2
	str r3, [r5]
	mov r4, r10
	ldr r3, [r4, #12]
	str r3, [r5, #4]
	ldr r3, [r4, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r0, [sp, #36]
	movs r1, #128
	lsls r1, r1, #8
	eors r1, r0
	movs r0, #128
	adds r2, r5, #0
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	add r1, sp, #36
	ldrh r1, [r1]
	mov r2, r10
	strh r1, [r2, #6]
	movs r2, #2
	ldrsh r1, [r5, r2]
	ldr r0, [r6]
	movs r3, #10
	ldrsh r2, [r5, r3]
	bl Actor_SetPositionAndReset
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	bl Func_02004d48
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c94:
	.4byte gMainRoutineWork
.L_02009c98:
	.4byte 0x000000e3
.L_02009c9c:
	.4byte 0xfff00000
	.section .bss,"aw",%nobits
	.global Data_020067ec
Data_020067ec:
	.space 0x00000004
	.global Data_020067f0
Data_020067f0:
	.space 0x00000004
	.global Data_020067f4
Data_020067f4:
	.space 0x00000004
	.global Data_020067f8
Data_020067f8:
