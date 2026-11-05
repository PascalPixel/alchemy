.syntax unified
	.thumb
	.section .text.x020081d0,"ax",%progbits
	.global Func_020001d0
	.thumb_func
Func_020001d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #20
	str r3, [sp, #16]
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	mov r11, r0
	cmp r3, r2
	beq .L_020082b6
.L_020081f6:
	mov r3, r11
	ldrh r3, [r3]
	adds r0, r3, #0
	str r3, [sp, #12]
	bl Actor_GetById
	mov r2, r11
	ldrh r2, [r2, #2]
	adds r7, r0, #0
	str r2, [sp, #8]
	movs r3, #34
	adds r3, r3, r7
	adds r0, r2, #0
	ldrb r2, [r3]
	mov r9, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #16]
	adds r0, #1
	ldr r5, [r2, r3]
	ldr r2, .L_020082ec
	adds r3, r5, r2
	ldr r2, .L_020082f0
	asrs r3, r3, #2
	adds r6, r3, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008244
	ldr r0, [sp, #12]
	movs r1, #0
	movs r2, #0
	bl Func_02003c68
	b .L_020082a4
.L_02008244:
	adds r0, r7, #0
	bl Func_02003db0
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r5, r5, r3
	str r5, [sp, #4]
	mov r2, r9
	ldrb r0, [r2]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Func_02003c10
	mov r3, r9
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r10, r0
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	ldr r0, [sp, #12]
	bl Func_02003cc0
	ldr r2, [sp, #4]
	movs r3, #128
	asrs r5, r5, #19
	strb r3, [r2, #3]
	adds r5, #4
	mov r3, r9
	adds r2, r5, #0
	ldrb r0, [r3]
	mov r1, r10
	bl Func_02003dd0
	add r8, r6
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082a4
	adds r0, r7, #0
	movs r1, #0
	bl Func_02003ba0
.L_020082a4:
	movs r3, #4
	add r11, r3
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020081f6
.L_020082b6:
	ldr r3, .L_020082f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	cmp r3, r0
	bge .L_020082de
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_020082de:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020082ec:
	.4byte 0xfdff0000
.L_020082f0:
	.4byte gMapShapeGrid
.L_020082f4:
	.4byte gMainRoutineWork
	.section .text.x02008584,"ax",%progbits
	.balign 4
	.global Func_02000584
	.thumb_func
Func_02000584:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #16
	ldr r5, .L_020086e0
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_02003c78
	ldr r1, [r5]
	movs r0, #9
	bl Func_02003c78
	ldr r1, [r5]
	movs r0, #10
	bl Func_02003c78
	movs r0, #1
	bl wait
	movs r0, #8
	bl Func_0200056c
	movs r0, #9
	bl Func_0200056c
	movs r0, #10
	bl Func_0200056c
	movs r1, #0
	movs r0, #9
	bl Actor_SetModeById
	movs r0, #1
	bl wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003c68
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02003c68
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_02003c68
	movs r0, #1
	bl wait
	b .L_020086be
.L_0200860a:
	mov r3, r11
	ldrh r3, [r3]
	mov r9, r3
	mov r0, r9
	bl Actor_GetById
	mov r2, r11
	ldrh r2, [r2, #2]
	adds r5, r0, #0
	str r2, [sp, #8]
	adds r7, r5, #0
	adds r7, #34
	adds r0, r2, #0
	ldrb r2, [r7]
	adds r0, #1
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #12]
	ldr r6, [r2, r3]
	ldr r2, .L_020086e4
	adds r3, r6, r2
	ldr r2, .L_020086e8
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008660
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_02003c68
	b .L_020086ba
.L_02008660:
	adds r0, r5, #0
	bl Func_02003db0
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02003c10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_02003cc0
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02003dd0
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086ba
	mov r0, r9
	movs r1, #9
	bl ObjectVisual_CopyAttributes
.L_020086ba:
	movs r3, #4
	add r11, r3
.L_020086be:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200860a
	movs r0, #10
	bl wait
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020086e0:
	.4byte gMainRoutineWork
.L_020086e4:
	.4byte 0xfdff0000
.L_020086e8:
	.4byte gMapShapeGrid
	.section .text.x02008974,"ax",%progbits
	.global Func_02000974
	.thumb_func
Func_02000974:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02008a80
	mov r8, r0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Actor_GetById
	movs r2, #0
	adds r7, r0, #0
	mov r9, r2
	mov r10, r2
.L_02008996:
	bl Random16Far
	lsls r3, r0, #3
	subs r3, r3, r0
	ldr r2, [r7, #12]
	lsls r3, r3, #1
	lsrs r3, r3, #16
	lsls r3, r3, #16
	subs r2, r2, r3
	mov r3, r10
	lsls r1, r3, #17
	ldr r3, [r7, #8]
	ldr r0, .L_02008a84
	adds r1, r1, r3
	ldr r3, .L_02008a88
	adds r1, r1, r0
	movs r0, #30
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r7, #16]
	bl Func_02003bb0
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008a6a
	mov r1, r9
	ldr r0, [r6, #80]
	bl Func_02003d80
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	movs r1, #0
	mov r9, r0
	adds r0, r6, #0
	bl Actor_SetSingleChildFlags
	adds r0, r6, #0
	movs r1, #2
	bl Func_02003ba0
	adds r0, r6, #0
	ldr r1, .L_02008a8c
	bl Func_02003ba8
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #24]
	str r3, [r6, #28]
	ldr r1, [r6, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	mov r2, r8
	strb r3, [r1, #9]
	cmp r2, #0
	beq .L_02008a34
	mov r3, r10
	lsls r5, r3, #13
	adds r0, r5, #0
	bl Math_Cosine
	ldr r3, .L_02008a90
	ldr r1, .L_02008a94
	mov lr, r3
	.2byte 0xf800
	str r0, [r6, #68]
	adds r0, r5, #0
	bl Math_Sine
	b .L_02008a38
.L_02008a34:
	mov r0, r8
	str r0, [r6, #68]
.L_02008a38:
	str r0, [r6, #76]
	bl Random16Far
	movs r2, #192
	lsls r0, r0, #14
	lsls r2, r2, #7
	lsrs r0, r0, #16
	adds r0, r0, r2
	negs r0, r0
	str r0, [r6, #72]
	bl Random16Far
	ldr r3, .L_02008a98
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_02008a9c
	str r3, [r6, #48]
	ldr r3, .L_02008aa0
	str r3, [r6, #52]
	ldr r3, .L_02008aa4
	str r3, [r6, #108]
.L_02008a6a:
	movs r0, #1
	add r10, r0
	mov r2, r10
	cmp r2, #7
	bls .L_02008996
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a80:
	.4byte gMainRoutineWork
.L_02008a84:
	.4byte 0xfff80000
.L_02008a88:
	.4byte 0xfffe0000
.L_02008a8c:
	.4byte gData_02003e00
.L_02008a90:
	.4byte IwramMulQ16
.L_02008a94:
	.4byte 0x00013333
.L_02008a98:
	.4byte 0xffffff00
.L_02008a9c:
	.4byte 0xfffff800
.L_02008aa0:
	.4byte 0xfffffa00
.L_02008aa4:
	.4byte Effect_AdvanceMotionAtLeaderPriority
	.section .text.x02008c64,"ax",%progbits
	.global Func_02000c64
	.thumb_func
Func_02000c64:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	str r3, [sp, #4]
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	mov r11, r0
	cmp r3, r2
	beq .L_02008d34
.L_02008c8a:
	mov r3, r11
	ldrh r0, [r3]
	bl Actor_GetById
	mov r8, r0
	mov r7, r8
	adds r7, #34
	mov r2, r8
	ldr r1, [r2, #8]
	ldrb r0, [r7]
	ldr r2, [r2, #16]
	bl Func_02003c10
	str r0, [sp, #0]
	mov r3, r8
	ldr r1, [r3, #8]
	ldr r2, [r3, #16]
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	mov r10, r0
	mov r2, r10
	asrs r2, r2, #19
	mov r0, r8
	mov r10, r2
	bl Func_02003db0
	ldrb r2, [r7]
	mov r9, r0
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #4]
	mov r0, r8
	ldr r6, [r2, r3]
	ldr r3, .L_02008d6c
	ldr r2, .L_02008d70
	adds r5, r6, r3
	asrs r5, r5, #2
	adds r5, r5, r2
	mov r2, r8
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #4
	add r10, r3
	mov r2, r10
	ldrb r0, [r7]
	ldr r1, [sp, #0]
	bl Func_02003dd0
	mov r2, r9
	lsls r2, r2, #2
	add r5, r9
	mov r9, r2
	add r6, r9
	ldrb r3, [r6, #3]
	movs r2, #192
	orrs r3, r2
	strb r3, [r6, #3]
	movs r3, #128
	lsls r3, r3, #2
	adds r6, r6, r3
	ldrb r2, [r6, #3]
	movs r3, #64
	orrs r3, r2
	movs r2, #2
	add r11, r2
	mov r2, r11
	strb r3, [r6, #3]
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	strb r0, [r5]
	cmp r3, r2
	bne .L_02008c8a
.L_02008d34:
	ldr r3, .L_02008d74
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	cmp r3, r0
	bge .L_02008d5c
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_02008d5c:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d6c:
	.4byte 0xfdff0000
.L_02008d70:
	.4byte gMapShapeGrid
.L_02008d74:
	.4byte gMainRoutineWork
	.section .text.x02008d78,"ax",%progbits
	.global Func_02000d78
	.thumb_func
Func_02000d78:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r0, [sp, #4]
	bl Func_02003df0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r7, r0, #0
	str r3, [sp, #0]
	cmp r7, #0
	bne .L_02008dba
	ldr r3, .L_02008e90
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #1
	bl Func_02003d18
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_02008dba
	bl Actor_GetById
	adds r7, r0, #0
.L_02008dba:
	ldr r1, [sp, #0]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r1, r2
	movs r2, #0
	str r2, [r3]
	ldr r1, [sp, #4]
	movs r2, #255
	ldrh r3, [r1]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008e80
.L_02008dd6:
	ldr r3, [sp, #4]
	ldrh r0, [r3]
	bl Actor_GetById
	cmp r7, r0
	bne .L_02008e6e
	movs r1, #34
	adds r1, r1, r7
	ldrb r0, [r1]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	lsls r3, r0, #3
	mov r11, r1
	subs r3, r3, r0
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r2, [r2, r3]
	ldr r3, .L_02008e94
	mov r10, r2
	ldr r2, .L_02008e98
	ldr r1, [r7, #8]
	add r2, r10
	asrs r2, r2, #2
	mov r8, r2
	ldr r2, [r7, #16]
	add r8, r3
	bl Func_02003c10
	mov r1, r11
	ldr r2, [r7, #16]
	mov r9, r0
	ldrb r0, [r1]
	ldr r1, [r7, #8]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_02003db0
	asrs r5, r5, #19
	mov r2, r11
	subs r5, #4
	adds r6, r0, #0
	mov r1, r9
	ldrb r0, [r2]
	adds r2, r5, #0
	bl Func_02003dd0
	add r8, r6
	lsls r6, r6, #2
	add r10, r6
	mov r1, r10
	ldrb r2, [r1, #3]
	mov r3, r8
	strb r0, [r3]
	movs r3, #63
	ands r3, r2
	movs r2, #128
	strb r3, [r1, #3]
	lsls r2, r2, #2
	add r10, r2
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r3, #191
	ands r3, r2
	mov r1, r10
	strb r3, [r1, #3]
	ldr r2, [sp, #0]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r2, r1
	str r7, [r3]
.L_02008e6e:
	ldr r2, [sp, #4]
	movs r1, #255
	adds r2, #2
	str r2, [sp, #4]
	lsls r1, r1, #8
	ldrh r3, [r2]
	adds r1, #255
	cmp r3, r1
	bne .L_02008dd6
.L_02008e80:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e90:
	.4byte gMainRoutineWork
.L_02008e94:
	.4byte gMapShapeGrid
.L_02008e98:
	.4byte 0xfdff0000
	.section .text.x02008e9c,"ax",%progbits
	.global Func_02000e9c
	.thumb_func
Func_02000e9c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r3, r1
	ldr r6, [r3]
	cmp r6, #0
	beq .L_02008f32
	adds r7, r6, #0
	adds r7, #34
	ldrb r0, [r7]
	ldr r2, [r2, #32]
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r2, [r2, r3]
	ldr r3, .L_02008f3c
	mov r10, r2
	ldr r2, .L_02008f40
	ldr r1, [r6, #8]
	add r2, r10
	asrs r2, r2, #2
	mov r8, r2
	ldr r2, [r6, #16]
	add r8, r3
	bl Func_02003c10
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	mov r9, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	adds r0, r6, #0
	bl Func_02003db0
	asrs r5, r5, #19
	adds r5, #4
	adds r6, r0, #0
	mov r1, r9
	adds r2, r5, #0
	ldrb r0, [r7]
	bl Func_02003dd0
	add r8, r6
	lsls r6, r6, #2
	add r10, r6
	mov r3, r10
	ldrb r2, [r3, #3]
	mov r1, r8
	movs r3, #192
	orrs r3, r2
	strb r0, [r1]
	movs r2, #128
	mov r1, r10
	strb r3, [r1, #3]
	lsls r2, r2, #2
	add r10, r2
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r3, #64
	orrs r3, r2
	mov r1, r10
	strb r3, [r1, #3]
.L_02008f32:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008f3c:
	.4byte gMapShapeGrid
.L_02008f40:
	.4byte 0xfdff0000
	.section .text.x02008f8a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008fc2,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a696,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a698,"ax",%progbits
	.global Func_02002698
	.thumb_func
Func_02002698:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r0, #0
	movs r0, #125
	mov r9, r2
	adds r6, r1, #0
	bl Func_02003df8
	adds r0, r5, #0
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #2
	bl wait
	movs r1, #0
	adds r0, r5, #0
	bl Actor_SetModeById
	adds r0, r5, #0
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #4
	bl wait
	adds r0, r5, #0
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetPartAttribute
	adds r0, r6, #0
	bl Actor_GetById
	adds r7, r0, #0
	movs r0, #0
	mov r10, r0
	mov r8, r0
.L_0200a6f4:
	bl Random16Far
	lsls r3, r0, #3
	subs r3, r3, r0
	ldr r2, [r7, #12]
	lsls r3, r3, #1
	lsrs r3, r3, #16
	lsls r3, r3, #16
	subs r2, r2, r3
	mov r3, r8
	lsls r1, r3, #17
	ldr r3, [r7, #8]
	ldr r0, .L_0200a7c4
	adds r1, r1, r3
	ldr r3, .L_0200a7c8
	adds r1, r1, r0
	movs r0, #30
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r7, #16]
	bl Func_02003bb0
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200a7a4
	mov r1, r10
	ldr r0, [r6, #80]
	bl Func_02003d80
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r3, #4
	strb r5, [r3]
	movs r1, #0
	mov r10, r0
	adds r0, r6, #0
	bl Actor_SetSingleChildFlags
	adds r0, r6, #0
	movs r1, #2
	bl Func_02003ba0
	adds r0, r6, #0
	ldr r1, .L_0200a7cc
	bl Func_02003ba8
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	ldr r1, [r6, #80]
	str r3, [r6, #24]
	str r3, [r6, #28]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	str r5, [r6, #68]
	str r5, [r6, #76]
	bl Random16Far
	movs r3, #192
	lsls r0, r0, #14
	lsls r3, r3, #7
	lsrs r0, r0, #16
	adds r0, r0, r3
	negs r0, r0
	str r0, [r6, #72]
	bl Random16Far
	ldr r3, .L_0200a7d0
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_0200a7d4
	str r3, [r6, #48]
	ldr r3, .L_0200a7d8
	str r3, [r6, #52]
	ldr r3, .L_0200a7dc
	str r3, [r6, #108]
.L_0200a7a4:
	movs r0, #1
	add r8, r0
	mov r3, r8
	cmp r3, #7
	bls .L_0200a6f4
	mov r0, r9
	bl GameFlag_SetBit
	movs r0, #20
	bl FieldEvent_Wait
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200a7c4:
	.4byte 0xfff80000
.L_0200a7c8:
	.4byte 0xfffe0000
.L_0200a7cc:
	.4byte gData_02003f14
.L_0200a7d0:
	.4byte 0xffffff00
.L_0200a7d4:
	.4byte 0xfffff800
.L_0200a7d8:
	.4byte 0xfffffa00
.L_0200a7dc:
	.4byte Effect_AdvanceMotion
	.section .text.x0200b284,"ax",%progbits
	.global Func_02003284
	.thumb_func
Func_02003284:
	push {r5, r6, r7, lr}
	movs r0, #10
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b2c4
	ldr r5, .L_0200b51c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	ldr r0, [r5]
	bl Actor_SetSpritePriority
	ldr r0, [r5]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_0200b2c4:
	movs r0, #8
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #9
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #10
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #11
	movs r1, #1
	bl Actor_SetSpritePriority
	bl Func_02003d98
	movs r1, #8
	movs r2, #9
	movs r0, #0
	bl Func_02003da0
	movs r2, #11
	movs r1, #10
	movs r0, #1
	bl Func_02003da0
	movs r0, #12
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #13
	movs r1, #1
	bl Actor_SetSpritePriority
	bl Func_02003d90
	movs r1, #130
	movs r2, #12
	movs r3, #13
	lsls r1, r1, #2
	movs r0, #0
	bl Func_02003da8
	ldr r0, .L_0200b520
	bl Func_02003db8
	movs r0, #19
	bl Actor_GetById
	ldr r2, .L_0200b524
	ldr r3, [r0, #12]
	cmp r3, r2
	ble .L_0200b33c
	movs r0, #19
	movs r1, #1
	bl Actor_SetSpritePriority
	b .L_0200b344
.L_0200b33c:
	movs r0, #19
	movs r1, #2
	bl Actor_SetSpritePriority
.L_0200b344:
	movs r0, #20
	bl Func_02003cc0
	movs r0, #20
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #20
	bl Actor_SetSpritePriority
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #65
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b376
	bl Func_0200172c
	b .L_0200b38a
.L_0200b376:
	movs r3, #38
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #32
	movs r2, #1
	movs r3, #1
	bl Func_02003bf0
.L_0200b38a:
	movs r0, #14
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r6, #254
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #15
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #16
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #17
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	ldr r0, .L_0200b528
	bl Func_02000c64
	movs r0, #17
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r7, #0
	asrs r3, r3, #20
	cmp r3, #54
	bne .L_0200b404
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, [r3]
	bl Func_02003db0
	lsls r0, r0, #2
	adds r5, r5, r0
	ldrb r2, [r5, #3]
	movs r3, #127
	ands r3, r2
	strb r3, [r5, #3]
	movs r3, #11
	strb r3, [r5, #2]
.L_0200b404:
	movs r0, #14
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #15
	movs r1, #1
	bl Actor_SetSpritePriority
	bl Func_02001930
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b438
	movs r3, #128
	movs r1, #212
	movs r2, #208
	lsls r3, r3, #7
	movs r0, #25
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl Func_02003c70
.L_0200b438:
	movs r0, #18
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #238
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b4c0
	movs r0, #18
	bl Actor_GetById
	movs r3, #27
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r5, r0, #0
	movs r2, #1
	movs r3, #1
	movs r0, #27
	movs r1, #8
	bl Func_02003bf0
	movs r0, #18
	movs r1, #4
	bl Actor_SetModeById
	adds r1, r5, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	adds r5, #89
	ldrb r2, [r5]
	adds r3, r6, #0
	movs r0, #131
	ands r3, r2
	lsls r0, r0, #1
	strb r3, [r5]
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #161
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #69
	movs r1, #51
	movs r2, #92
	movs r3, #11
	bl Func_02003be0
	movs r3, #128
	movs r1, #154
	movs r2, #140
	lsls r3, r3, #7
	movs r0, #25
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003c70
.L_0200b4c0:
	movs r0, #135
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b4e8
	movs r0, #132
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #22
	bl GameFlag_SetBit
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Func_02003c68
.L_0200b4e8:
	ldr r0, .L_0200b52c
	bl Func_020001d0
	bl Func_020013b0
	bl Func_02001fc4
	movs r0, #27
	bl Actor_GetById
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	ldr r3, .L_0200b530
	str r7, [r0, #12]
	str r7, [r3]
	ldr r3, .L_0200b534
	movs r1, #144
	str r7, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0200b538
	bl Task_Add
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b51c:
	.4byte gMainRoutineWork
.L_0200b520:
	.4byte gData_02003fc4
.L_0200b524:
	.4byte 0xfff80000
.L_0200b528:
	.4byte gData_02003fc8
.L_0200b52c:
	.4byte gData_02003f80
.L_0200b530:
	.4byte gData_0200567c
.L_0200b534:
	.4byte gData_02005678
.L_0200b538:
	.4byte Func_02001ee8
	.section .bss,"aw",%nobits
	.global gData_02005678
gData_02005678:
	.space 0x00000004
	.global gData_0200567c
gData_0200567c:
