.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008468,"ax",%progbits
	.balign 4
	.global Func_02000468
	.thumb_func
Func_02000468:
	push {r5, lr}
	adds r5, r0, #0
	movs r2, #99
	adds r2, r2, r5
	ldrb r3, [r2]
	mov r12, r2
	cmp r3, #0
	beq .L_020084b6
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_020084b6
	movs r3, #212
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r5, #16]
	ldr r3, [r5, #12]
	ldr r1, [r5, #8]
	subs r2, r2, r3
	asrs r2, r2, #20
	adds r3, r2, #1
	asrs r1, r1, #20
	ldr r4, [r0]
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	subs r2, #1
	movs r3, #0
	strb r3, [r4, #2]
	lsls r2, r2, #7
	ldr r4, [r0]
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r4, r4, r1
	mov r2, r12
	strb r3, [r4, #2]
	strb r3, [r2]
.L_020084b6:
	pop {r5, pc}
	.section .text.x0200895c,"ax",%progbits
	.global Func_0200095c
	.thumb_func
Func_0200095c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008ccc
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #128
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r1, #0
	str r1, [sp, #16]
	mov r8, r3
	mov r10, r0
	mov r9, r1
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	movs r0, #207
	bl Func_02003cd8
	mov r5, r10
	mov r2, r10
	movs r0, #140
	ldr r3, [r5, #16]
	ldr r1, [r2, #8]
	lsls r0, r0, #1
	ldr r2, [r2, #12]
	bl Import_ActorCreate
	movs r1, #2
	adds r7, r0, #0
	bl Func_02003b28
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, .L_02008cd0
	add r0, sp, #16
	str r3, [r7, #24]
	movs r3, #204
	lsls r3, r3, #6
	ldrb r0, [r0]
	adds r3, #51
	str r3, [r7, #28]
	adds r3, r7, #0
	adds r3, #85
	mov r1, r8
	strb r0, [r3]
	ldr r0, [r1, #20]
	ldr r4, [r7, #80]
	ldr r3, [r0, #80]
	ldrb r1, [r4, #9]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	ldr r5, .L_02008cd4
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	adds r3, r3, r5
	adds r0, r7, #0
	bl Func_02003b58
	movs r0, #166
	ldr r3, [r7, #28]
	lsls r0, r0, #9
	adds r0, #203
	cmp r3, r0
	bgt .L_02008a26
.L_02008a0a:
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #153
	adds r3, r3, r1
	str r3, [r7, #28]
	movs r0, #1
	bl FieldEvent_Wait
	movs r2, #166
	ldr r3, [r7, #28]
	lsls r2, r2, #9
	adds r2, #203
	cmp r3, r2
	ble .L_02008a0a
.L_02008a26:
	adds r0, r7, #0
	mov r5, r8
	bl Func_02003b60
	ldr r3, [r5, #20]
	ldr r0, .L_02008cd4
	ldr r3, [r3, #16]
	movs r1, #230
	adds r3, r3, r0
	str r3, [r7, #16]
	ldr r3, [r7, #28]
	lsls r1, r1, #9
	adds r1, #203
	cmp r3, r1
	bgt .L_02008b00
.L_02008a44:
	ldr r2, .L_02008cd8
	movs r3, #3
	ldr r6, [r2]
	mov r11, r2
	ands r6, r3
	cmp r6, #0
	bne .L_02008aba
	mov r5, r8
	ldr r3, [r5, #20]
	movs r0, #168
	ldr r1, [r3, #8]
	movs r2, #0
	ldr r3, [r3, #16]
	lsls r0, r0, #2
	bl Import_ActorCreate
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	ldr r3, [r5, #8]
	str r3, [r5, #68]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r5, #72]
	ldr r3, [r5, #16]
	str r3, [r5, #76]
	bl Random16Far
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5, #48]
	adds r0, r5, #0
	movs r1, #7
	bl Func_02003b28
	adds r0, r5, #0
	ldr r1, .L_02008cdc
	bl Func_02003b38
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #2
	bl Actor_SetPartAttribute
	ldr r3, .L_02008ce0
	str r3, [r5, #108]
.L_02008aba:
	mov r0, r11
	ldr r3, [r0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02008ae4
	ldr r3, [r7, #24]
	movs r2, #200
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	mov r1, r8
	adds r3, r3, r2
	ldr r0, [r1, #20]
	str r3, [r7, #28]
	movs r1, #15
	bl Actor_SetPartAttribute
	b .L_02008aee
.L_02008ae4:
	mov r2, r8
	ldr r0, [r2, #20]
	movs r1, #4
	bl Actor_SetPartAttribute
.L_02008aee:
	movs r0, #1
	bl FieldEvent_Wait
	movs r5, #230
	ldr r3, [r7, #28]
	lsls r5, r5, #9
	adds r5, #203
	cmp r3, r5
	ble .L_02008a44
.L_02008b00:
	mov r1, r8
	ldr r0, [r1, #20]
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r0, #136
	bl Func_02003cd8
	mov r3, r8
	ldr r2, [r3, #20]
	mov r0, r10
	ldr r3, [r2, #8]
	ldrh r5, [r0, #6]
	asrs r3, r3, #20
	str r3, [sp, #20]
	ldr r3, [r2, #16]
	movs r1, #128
	asrs r3, r3, #20
	str r3, [sp, #24]
	lsls r1, r1, #6
	movs r3, #192
	adds r5, r5, r1
	lsls r3, r3, #8
	ands r5, r3
	adds r0, r5, #0
	bl Math_Cosine
	asrs r0, r0, #16
	str r0, [sp, #28]
	adds r0, r5, #0
	bl Math_Sine
	asrs r0, r0, #16
	str r0, [sp, #32]
.L_02008b44:
	ldr r1, [sp, #20]
	ldr r2, [sp, #24]
	movs r0, #2
	bl Func_02000348
	ldr r3, [sp, #20]
	ldr r5, [sp, #28]
	mov r11, r0
	adds r1, r3, r5
	ldr r0, [sp, #24]
	mov r2, r11
	ldr r3, [sp, #32]
	asrs r2, r2, #8
	mov r11, r2
	movs r5, #2
	adds r2, r0, r3
	mov r0, r11
	str r1, [sp, #20]
	str r2, [sp, #24]
	add r9, r5
	cmp r0, #0
	beq .L_02008b44
	movs r0, #2
	bl Func_02000348
	adds r1, r0, #0
	asrs r1, r1, #8
	subs r1, #1
	str r1, [sp, #16]
	movs r2, #2
	ldr r0, [sp, #28]
	mov r3, r8
	negs r2, r2
	add r9, r2
	ldr r2, [r3, #20]
	mov r3, r9
	muls r3, r0
	movs r5, #10
	ldrsh r6, [r2, r5]
	movs r1, #18
	ldrsh r5, [r2, r1]
	ldr r2, [sp, #32]
	lsls r3, r3, #3
	ldr r1, [r7, #80]
	adds r6, r6, r3
	mov r3, r9
	muls r3, r2
	ldrb r2, [r1, #9]
	lsls r3, r3, #3
	adds r5, r5, r3
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #48]
	str r3, [r7, #52]
	adds r0, r5, #0
	adds r3, r6, #0
	ldr r2, [r7, #12]
	lsls r1, r3, #16
	lsls r3, r0, #16
	adds r0, r7, #0
	str r6, [sp, #20]
	str r5, [sp, #24]
	bl Func_02003b58
	movs r0, #160
	movs r1, #160
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Func_02003c20
	lsls r6, r6, #16
	lsls r5, r5, #16
	movs r1, #1
	adds r0, r6, #0
	negs r1, r1
	adds r2, r5, #0
	movs r3, #1
	bl Camera_MoveWithinMapBounds
	b .L_02008c6c
.L_02008bf0:
	ldr r1, .L_02008cd8
	movs r3, #1
	ldr r0, [r1]
	mov r9, r1
	mov r10, r0
	mov r2, r10
	ands r2, r3
	mov r10, r2
	cmp r2, #0
	bne .L_02008c66
	add r6, sp, #76
	str r3, [r6]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #162
	strh r3, [r6, #24]
	movs r3, #2
	str r3, [r6, #4]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	lsls r0, r0, #12
	mov r8, r3
	bl Math_Cosine
	add r5, sp, #116
	lsls r0, r0, #1
	str r0, [r5]
	bl Random16Far
	ldr r3, [r7, #12]
	movs r2, #31
	ands r2, r0
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r1, r9
	ldr r0, [r1]
	mov r2, r8
	ands r0, r2
	lsls r0, r0, #12
	bl Math_Sine
	str r0, [r5, #8]
	ldr r4, [r7, #8]
	ldr r1, [r5, #4]
	ldr r3, [r5]
	ldr r2, [r7, #16]
	str r0, [sp, #4]
	movs r0, #152
	lsls r0, r0, #13
	mov r5, r10
	str r0, [sp, #8]
	adds r0, r4, #0
	str r5, [sp, #0]
	str r6, [sp, #12]
	bl Func_0200015c
.L_02008c66:
	movs r0, #1
	bl FieldEvent_Wait
.L_02008c6c:
	adds r0, r7, #0
	bl Func_02003ba8
	cmp r0, #0
	beq .L_02008bf0
	mov r0, r11
	cmp r0, #255
	bne .L_02008c84
	movs r0, #136
	bl Func_02003cd8
	b .L_02008e78
.L_02008c84:
	ldr r5, [sp, #28]
	movs r1, #10
	ldrsh r2, [r7, r1]
	lsls r3, r5, #1
	ldr r1, [sp, #32]
	adds r3, r3, r5
	lsls r3, r3, #4
	adds r3, r2, r3
	str r3, [sp, #20]
	movs r0, #18
	ldrsh r2, [r7, r0]
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #4
	adds r3, r2, r3
	str r3, [sp, #24]
	ldr r2, [sp, #20]
	ldr r5, [sp, #24]
	movs r3, #128
	lsls r3, r3, #8
	lsls r1, r2, #16
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	lsls r3, r5, #16
	adds r0, r7, #0
	bl Func_02003b58
	movs r0, #235
	bl Func_02003cd8
	movs r0, #30
	negs r0, r0
	add r11, r0
	b .L_02008e36
	.2byte 0x0000
.L_02008ccc:
	.4byte gMainRoutineWork
.L_02008cd0:
	.4byte 0x0001b333
.L_02008cd4:
	.4byte 0xffff0000
.L_02008cd8:
	.4byte gFrameCount
.L_02008cdc:
	.4byte gData_02003d1c
.L_02008ce0:
	.4byte Func_02000840
.L_02008ce4:
	ldr r1, .L_02008ee0
	movs r2, #1
	ldr r3, [r1]
	mov r9, r1
	ands r3, r2
	cmp r3, #0
	bne .L_02008d5a
	add r3, sp, #36
	str r2, [r3]
	mov r8, r3
	bl Random16Far
	movs r3, #255
	lsls r3, r3, #8
	movs r5, #128
	lsls r5, r5, #9
	adds r3, #255
	mov r10, r5
	ands r3, r0
	add r3, r10
	mov r0, r8
	str r3, [r0, #12]
	str r3, [r0, #8]
	mov r1, r9
	ldr r0, [r1]
	movs r6, #128
	lsls r6, r6, #1
	adds r6, #255
	ands r0, r6
	lsls r0, r0, #12
	bl Math_Cosine
	mov r2, r10
	add r5, sp, #116
	lsls r0, r0, #1
	str r2, [r5, #4]
	str r0, [r5]
	mov r3, r9
	ldr r0, [r3]
	ands r0, r6
	lsls r0, r0, #12
	bl Math_Sine
	str r0, [r5, #8]
	ldr r6, [r7, #8]
	ldr r3, [r5]
	ldr r4, [r5, #4]
	ldr r1, [r7, #12]
	ldr r2, [r7, #16]
	str r0, [sp, #4]
	movs r0, #160
	lsls r0, r0, #12
	str r0, [sp, #8]
	mov r5, r8
	adds r0, r6, #0
	str r4, [sp, #0]
	str r5, [sp, #12]
	bl Func_0200015c
.L_02008d5a:
	mov r0, r9
	ldr r3, [r0]
	movs r4, #3
	ands r3, r4
	cmp r3, #0
	bne .L_02008e30
	ldr r6, .L_02008ee4
	mov r1, r11
	lsls r5, r1, #3
	adds r3, r5, #4
	ldrsh r2, [r6, r3]
	ldr r3, [r7, #8]
	ldrh r0, [r6, r5]
	asrs r3, r3, #20
	adds r2, r2, r3
	adds r3, r5, #6
	mov r10, r2
	str r2, [sp, #20]
	ldrsh r2, [r6, r3]
	ldr r3, [r7, #16]
	adds r1, r5, #2
	asrs r3, r3, #20
	adds r2, r2, r3
	mov r9, r2
	str r2, [sp, #24]
	mov r3, r11
	movs r2, #1
	ands r3, r2
	mov r8, r1
	cmp r3, #0
	beq .L_02008de0
	ldrsh r1, [r6, r1]
	lsls r0, r0, #16
	movs r2, #1
	mov r3, r9
	subs r3, #1
	str r2, [sp, #0]
	asrs r0, r0, #16
	mov r2, r10
	str r4, [sp, #4]
	bl Func_02003b68
	mov r2, r8
	ldrsh r1, [r6, r2]
	ldrsh r0, [r6, r5]
	mov r2, r10
	mov r3, r9
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #2
	bl Func_02003b80
	mov r2, r8
	ldrsh r0, [r6, r5]
	ldrsh r1, [r6, r2]
	ldr r2, [r7, #8]
	ldr r3, [r7, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #2
	bl Func_02003b80
	b .L_02008e2c
.L_02008de0:
	mov r2, r8
	ldrsh r1, [r6, r2]
	mov r2, r11
	asrs r3, r2, #1
	ldr r2, [sp, #24]
	lsls r0, r0, #16
	subs r3, r2, r3
	movs r2, #1
	str r2, [sp, #0]
	str r2, [sp, #4]
	asrs r0, r0, #16
	mov r2, r10
	bl Func_02003b68
	mov r2, r8
	ldrsh r1, [r6, r2]
	ldrsh r0, [r6, r5]
	mov r2, r10
	mov r3, r9
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	bl Func_02003b80
	mov r2, r8
	ldrsh r0, [r6, r5]
	ldrsh r1, [r6, r2]
	ldr r2, [r7, #8]
	ldr r3, [r7, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	bl Func_02003b80
.L_02008e2c:
	bl Func_02003b50
.L_02008e30:
	movs r0, #1
	bl FieldEvent_Wait
.L_02008e36:
	adds r0, r7, #0
	bl Func_02003ba8
	cmp r0, #0
	bne .L_02008e42
	b .L_02008ce4
.L_02008e42:
	mov r5, r11
	cmp r5, #2
	bne .L_02008e64
	ldr r3, .L_02008ee4
	movs r1, #16
	ldrsh r0, [r3, r1]
	movs r2, #18
	ldrsh r1, [r3, r2]
	movs r2, #1
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #24]
	ldr r2, [sp, #20]
	bl Func_02003b68
	bl Func_02003b50
.L_02008e64:
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02003cd8
	ldr r3, [sp, #16]
	movs r5, #133
	lsls r5, r5, #4
	adds r0, r3, r5
	bl GameFlag_SetBit
.L_02008e78:
	movs r3, #0
	str r3, [r7, #52]
	str r3, [r7, #48]
	str r3, [r7, #64]
	str r3, [r7, #60]
	str r3, [r7, #56]
	movs r5, #0
.L_02008e86:
	ldr r3, [r7, #24]
	ldr r0, .L_02008ee8
	ldr r1, .L_02008eec
	adds r3, r3, r0
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	movs r2, #224
	adds r3, r3, r1
	str r3, [r7, #28]
	ldr r3, [r7, #12]
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	adds r5, #1
	bl FieldEvent_Wait
	cmp r5, #8
	bne .L_02008e86
	adds r0, r7, #0
	bl Func_02003b48
	movs r0, #30
	bl FieldEvent_Wait
	ldr r3, .L_02008ef0
	movs r5, #133
	lsls r5, r5, #2
	adds r3, r3, r5
	ldr r0, [r3]
	movs r1, #1
	bl Func_02003c38
	bl Func_02003c30
	bl Func_02003bd0
	add sp, #128
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008ee0:
	.4byte gFrameCount
.L_02008ee4:
	.4byte gData_02004674
.L_02008ee8:
	.4byte 0xffffd99a
.L_02008eec:
	.4byte 0xffffc000
.L_02008ef0:
	.4byte gMainRoutineWork
	.section .text.x02009908,"ax",%progbits
	.global Func_02001908
	.thumb_func
Func_02001908:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r7, .L_020099fc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r5, r0, #0
	mov r8, r3
	movs r3, #179
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #173
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #175
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #180
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r3, r3, r7
	mov r10, r3
	ldrb r3, [r3]
	cmp r3, #5
	bne .L_02009980
.L_02009976:
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetAnimationSpeed
	b .L_02009b42
.L_02009980:
	adds r0, r6, #0
	movs r1, #16
	bl Actor_SetAnimationSpeed
	ldr r2, [r6, #16]
	ldr r3, [r6, #12]
	ldr r1, [r6, #8]
	subs r2, r2, r3
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	bl Func_02000348
	asrs r0, r0, #8
	cmp r0, #212
	bne .L_020099ce
.L_020099a0:
	movs r1, #142
	lsls r1, r1, #1
	adds r0, r6, #0
	adds r1, #255
	bl Func_02003cd0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #99
	strb r3, [r2]
	str r3, [r6, #108]
	ldrh r3, [r6, #6]
	movs r2, #192
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_020099c4
	ldr r1, .L_02009a00
	b .L_020099c6
.L_020099c4:
	ldr r1, .L_02009a04
.L_020099c6:
	adds r0, r6, #0
	bl Func_02003b38
	b .L_02009b42
.L_020099ce:
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02009a7a
	mov r2, r10
	ldrb r3, [r2]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r7, #0
	adds r0, #8
	adds r1, #8
	cmp r3, #2
	bne .L_02009a08
	bl Func_020018d8
	cmp r0, #16
	bgt .L_02009a1e
	ldr r2, [r5, #16]
	ldr r3, [r6, #16]
	b .L_02009a14
	.2byte 0x0000
.L_020099fc:
	.4byte gMainRoutineWork
.L_02009a00:
	.4byte gData_02003de0
.L_02009a04:
	.4byte gData_02003dcc
.L_02009a08:
	bl Func_020018ac
	cmp r0, #8
	bgt .L_02009a1e
	ldr r2, [r5, #12]
	ldr r3, [r6, #12]
.L_02009a14:
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009a1e
	movs r7, #1
.L_02009a1e:
	cmp r7, #0
	beq .L_02009a7a
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009a7a
	ldrh r3, [r6, #6]
	str r6, [r5, #104]
	strh r3, [r5, #6]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r2, #12
	ldr r3, [r6, #80]
	ldr r0, [r5, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	movs r2, #181
	lsls r2, r2, #1
	add r2, r8
	strb r3, [r0, #9]
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_02009a88
	movs r2, #128
	ldr r4, .L_02009a84
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r4, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_02009a7a:
	mov r3, r8
	adds r3, #52
	str r3, [sp, #0]
	movs r7, #8
	b .L_02009a8c
.L_02009a84:
	.4byte 0x00000000
.L_02009a88:
	.4byte gMainRoutineWork
.L_02009a8c:
	ldr r3, [sp, #0]
	ldmia r3!, {r5}
	adds r2, r3, #0
	str r2, [sp, #0]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02009ade
	ldr r1, [r6, #104]
	ldr r3, [r5, #8]
	ldr r2, [r1, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	cmp r2, r3
	bne .L_02009ac0
	ldr r2, [r1, #12]
	ldr r3, [r5, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009ac0
	ldr r2, [r1, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_02009ade
.L_02009ac0:
	adds r0, r5, #0
	adds r1, r6, #0
	adds r0, #8
	adds r1, #8
	bl Func_020018ac
	cmp r0, #8
	bgt .L_02009ade
	ldr r2, [r5, #12]
	ldr r3, [r6, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009ade
	b .L_020099a0
.L_02009ade:
	adds r7, #1
	cmp r7, #64
	bne .L_02009a8c
	ldrh r3, [r6, #6]
	movs r2, #192
	lsls r2, r2, #8
	adds r0, r3, #0
	cmp r3, r2
	bne .L_02009b1a
	bl Math_Cosine
	movs r1, #192
	lsls r1, r1, #9
	ldr r5, .L_02009b50
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	movs r1, #192
	lsls r1, r1, #9
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	subs r3, r3, r0
	str r3, [r6, #12]
	b .L_02009b42
.L_02009b1a:
	bl Math_Cosine
	movs r1, #192
	lsls r1, r1, #9
	ldr r5, .L_02009b50
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	movs r1, #192
	lsls r1, r1, #9
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	adds r3, r3, r0
	str r3, [r6, #16]
.L_02009b42:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009b50:
	.4byte IwramMulQ16
	.section .text.x0200a6b8,"ax",%progbits
	.global Func_020026b8
	.thumb_func
Func_020026b8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #208
	lsls r0, r0, #3
	sub sp, #8
	bl Heap_Alloc
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	adds r5, r0, #0
	movs r0, #188
	lsls r0, r0, #1
	adds r0, r0, r3
	mov r8, r0
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #254
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	ldr r0, .L_0200a770
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02003af0
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_0200a774
	adds r2, #208
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #208
	lsls r2, r2, #2
	adds r0, r5, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r1, .L_0200a778
	adds r2, #208
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	bl Heap_Release
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #3
	lsls r3, r3, #19
	adds r2, #14
	adds r3, #8
	strh r2, [r3]
	mov r0, sp
	ldr r3, .L_0200a76c
	adds r0, #6
	strh r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_0200a77c
	ldr r2, .L_0200a780
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #144
	lsls r0, r0, #2
	bl Heap_Alloc
	adds r5, r0, #0
	ldr r0, .L_0200a784
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02003af0
	movs r7, #0
	adds r4, r5, #0
	b .L_0200a788
	.2byte 0x0000
.L_0200a76c:
	.4byte 0x00000010
.L_0200a770:
	.4byte 0x000001be
.L_0200a774:
	.4byte 0x0600e800
.L_0200a778:
	.4byte 0x0600ec00
.L_0200a77c:
	.4byte 0x06002000
.L_0200a780:
	.4byte 0x81000280
.L_0200a784:
	.4byte 0x000001bf
.L_0200a788:
	ldrh r3, [r4]
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #64
	movs r2, #144
	adds r3, r3, r0
	adds r7, #1
	lsls r2, r2, #1
	strh r3, [r4]
	adds r4, #2
	cmp r7, r2
	bne .L_0200a788
	adds r4, r5, #0
	movs r7, #0
.L_0200a7a4:
	ldr r2, .L_0200a96c
	lsls r1, r7, #6
	adds r1, r1, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r7, #1
	adds r4, #32
	cmp r7, #18
	bne .L_0200a7a4
	adds r0, r5, #0
	bl Heap_Release
	movs r0, #246
	bl Func_02003cd8
	movs r0, #12
	bl Actor_GetById
	movs r3, #9
	adds r0, #98
	strb r3, [r0]
	movs r0, #12
	bl Actor_GetById
	movs r3, #1
	adds r0, #99
	strb r3, [r0]
	movs r0, #12
	bl Actor_GetById
	ldr r3, .L_0200a970
	movs r7, #0
	str r3, [r0, #108]
.L_0200a7f4:
	ldr r3, .L_0200a974
	ldr r6, [r3]
	movs r3, #3
	ands r6, r3
	cmp r6, #0
	bne .L_0200a898
	movs r0, #12
	bl Actor_GetById
	str r0, [sp, #0]
	movs r0, #12
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #12
	bl Actor_GetById
	ldr r3, [sp, #0]
	ldr r2, [r5, #12]
	ldr r1, [r3, #8]
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Import_ActorCreate
	adds r5, r0, #0
	movs r0, #12
	bl Actor_GetById
	ldr r3, [r0, #80]
	ldr r4, [r5, #80]
	ldrb r3, [r3, #9]
	movs r1, #12
	ands r1, r3
	movs r0, #13
	ldrb r3, [r4, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	orrs r3, r1
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	strb r3, [r4, #9]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	ldr r3, [r5, #8]
	str r3, [r5, #68]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #72]
	ldr r3, [r5, #16]
	str r3, [r5, #76]
	bl Random16Far
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5, #48]
	adds r0, r5, #0
	movs r1, #7
	bl Func_02003b28
	adds r0, r5, #0
	ldr r1, .L_0200a978
	bl Func_02003b38
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #2
	bl Actor_SetPartAttribute
	ldr r3, .L_0200a97c
	str r3, [r5, #108]
.L_0200a898:
	movs r0, #1
	adds r7, #1
	bl FieldEvent_Wait
	cmp r7, #45
	bne .L_0200a7f4
	ldr r3, .L_0200a980
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_02003c30
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02003c20
	movs r0, #152
	movs r1, #1
	movs r2, #144
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	bl Func_02003c30
	ldr r0, .L_0200a984
	bl Task_RemoveFar
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	ldrh r1, [r2]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r1
	strh r3, [r2]
	movs r0, #128
	ldrh r3, [r2]
	lsls r0, r0, #9
	strh r3, [r2]
	movs r1, #0
	bl Func_02003c58
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02003c50
	movs r0, #1
	bl Func_02003c60
	movs r0, #1
	bl wait
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02003c58
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003c50
	movs r0, #8
	bl Func_02003c60
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0200a988
	bl Task_Add
	movs r0, #163
	bl Func_02003cd8
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200a968
	movs r0, #192
	orrs r3, r2
	strh r3, [r1]
	movs r2, #128
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02003b98
	ldr r6, .L_0200a98c
	movs r7, #128
	lsls r7, r7, #7
	b .L_0200a990
.L_0200a968:
	.4byte 0x00000100
.L_0200a96c:
	.4byte 0x0600200f
.L_0200a970:
	.4byte Func_0200092c
.L_0200a974:
	.4byte gFrameCount
.L_0200a978:
	.4byte gData_02003d1c
.L_0200a97c:
	.4byte Func_020008ac
.L_0200a980:
	.4byte gMainRoutineWork
.L_0200a984:
	.4byte Func_020024b0
.L_0200a988:
	.4byte Func_0200257c
.L_0200a98c:
	.4byte 0xffff8000
.L_0200a990:
	mov r3, r8
	str r7, [r3, #24]
	str r6, [r3, #28]
	movs r0, #12
	bl Actor_GetById
	adds r5, r0, #0
	bl Random16Far
	movs r3, #7
	ands r0, r3
	adds r5, #98
	strb r0, [r5]
	movs r0, #1
	bl wait
	movs r0, #128
	ldr r2, .L_0200aa18
	lsls r0, r0, #3
	movs r3, #144
	adds r7, r7, r0
	lsls r3, r3, #11
	adds r6, r6, r2
	cmp r7, r3
	bne .L_0200a990
	ldr r0, .L_0200aa1c
	bl Task_RemoveFar
	movs r1, #200
	ldr r0, .L_0200aa20
	lsls r1, r1, #4
	bl Task_Add
	ldr r3, .L_0200aa0c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200aa10
	subs r2, #2
	strh r3, [r2]
	movs r7, #0
.L_0200a9e4:
	ldr r3, .L_0200aa0c
	asrs r2, r7, #5
	subs r3, r3, r2
	ldr r2, .L_0200aa14
	movs r6, #128
	lsls r6, r6, #19
	orrs r3, r2
	adds r6, #82
	strh r3, [r6]
	movs r0, #12
	bl Actor_GetById
	adds r5, r0, #0
	bl Random16Far
	movs r3, #7
	ands r0, r3
	adds r5, #98
	b .L_0200aa24
	.2byte 0x0000
.L_0200aa0c:
	.4byte 0x00000008
.L_0200aa10:
	.4byte 0x00003f42
.L_0200aa14:
	.4byte 0x00001000
.L_0200aa18:
	.4byte 0xfffffc00
.L_0200aa1c:
	.4byte Func_0200257c
.L_0200aa20:
	.4byte Func_02000ef4
.L_0200aa24:
	strb r0, [r5]
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #2
	adds r7, #1
	adds r0, #255
	cmp r7, r0
	bne .L_0200a9e4
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r0, #30
	bl wait
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003cd8
	ldr r3, .L_0200aa90
	movs r2, #128
	strh r3, [r6]
	ldr r3, .L_0200aa94
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02003b98
	movs r0, #12
	bl Actor_GetById
	movs r3, #6
	adds r0, #98
	strb r3, [r0]
	movs r7, #0
.L_0200aa82:
	ldr r3, .L_0200aa90
	asrs r1, r7, #3
	movs r0, #128
	lsls r2, r1, #8
	subs r3, r3, r1
	b .L_0200aa98
	.2byte 0x0000
.L_0200aa90:
	.4byte 0x00000010
.L_0200aa94:
	.4byte 0x00003f41
.L_0200aa98:
	lsls r0, r0, #19
	adds r0, #82
	orrs r2, r3
	strh r2, [r0]
	adds r7, #1
	movs r0, #1
	bl FieldEvent_Wait
	cmp r7, #129
	bne .L_0200aa82
	ldr r0, .L_0200ab5c
	bl Task_RemoveFar
	movs r0, #12
	bl Actor_GetById
	movs r5, #0
	adds r0, #99
	strb r5, [r0]
	movs r0, #30
	bl wait
	movs r0, #12
	bl Actor_GetById
	str r5, [r0, #108]
	movs r0, #12
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	bl Func_02003ba0
	movs r0, #60
	bl FieldEvent_Wait
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r5, [r3]
	bl Func_02003b00
	bl Func_02003af8
	ldr r5, .L_0200ab60
	movs r2, #147
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	adds r2, #1
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrb r1, [r3]
	bl Func_02003bb8
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #10
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200ab58
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	movs r3, #133
	lsls r3, r3, #2
	adds r7, r5, r3
	ldr r0, [r7]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #223
	ands r3, r2
	strb r3, [r0]
	ldr r0, [r7]
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	b .L_0200ab64
	.2byte 0x0000
.L_0200ab58:
	.4byte 0x00000001
.L_0200ab5c:
	.4byte Func_02000ef4
.L_0200ab60:
	.4byte gMainRoutineWork
.L_0200ab64:
	orrs r5, r3
	strb r5, [r0]
	ldr r0, [r7]
	bl Camera_FollowActor
	bl Func_02003c30
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #122
	bl GameFlag_SetBit
	bl Func_02003bd0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gData_020046a0
gData_020046a0:
