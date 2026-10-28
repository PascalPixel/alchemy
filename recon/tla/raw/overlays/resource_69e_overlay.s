.syntax unified
	.thumb
	.section .text.x020081a4,"ax",%progbits
	.global Func_020001a4
	.thumb_func
Func_020001a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020081c6
	ldr r3, .L_020081e8
	movs r1, #3
	ldr r0, [r3]
	bl __umodsi3
	cmp r0, #0
	bne .L_0200828c
.L_020081c6:
	movs r0, #23
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081ec
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #8
	b .L_020081f6
.L_020081e8:
	.4byte gFrameCount
.L_020081ec:
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #6
.L_020081f6:
	lsrs r2, r2, #16
	lsls r2, r2, #16
	adds r2, r2, r3
	ldr r3, .L_0200827c
	movs r0, #168
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	lsls r0, r0, #2
	bl Func_02002828
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200828c
	ldr r1, .L_02008280
	adds r0, r7, #0
	ldr r6, [r7, #80]
	bl Func_02002820
	movs r1, #1
	adds r0, r7, #0
	bl Actor_SetPartAttribute
	adds r3, r7, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	bl Random16Far
	ldr r3, .L_02008284
	adds r2, r7, #0
	adds r2, #100
	ands r3, r0
	strh r3, [r2]
	adds r3, r7, #0
	adds r3, #102
	strh r5, [r3]
	ldr r3, .L_02008288
	ldr r1, .L_02008278
	str r3, [r7, #108]
	mov r8, r1
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #16
	subs r0, r0, r3
	lsrs r0, r0, #20
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	mov r3, r8
	ldrb r2, [r6, #9]
	strb r3, [r6, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
	b .L_0200828c
	.2byte 0x0000
.L_02008278:
	.4byte 0x00000000
.L_0200827c:
	.4byte 0xffe40000
.L_02008280:
	.4byte gData_02003214
.L_02008284:
	.4byte 0x0ffff000
.L_02008288:
	.4byte Func_02000148
.L_0200828c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008cb6,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008cc4,"ax",%progbits
	.balign 4
	.global Func_02000cc4
	.thumb_func
Func_02000cc4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008d70
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_02008cd6
	adds r0, #3
.L_02008cd6:
	asrs r0, r0, #2
	movs r1, #5
	bl __modsi3
	ldr r3, .L_02008d74
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008d2a
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrh r1, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_02008d0a
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_02008d66
.L_02008d0a:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_02008d66
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02008d66
.L_02008d2a:
	movs r5, #0
	movs r6, #4
.L_02008d2e:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl __modsi3
	ldr r3, .L_02008d78
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_02008d2e
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_02008d7c
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02008d70
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02008d66:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d70:
	.4byte gData_020033d8
.L_02008d74:
	.4byte gData_020033dc
.L_02008d78:
	.4byte gData_02003404
.L_02008d7c:
	.4byte 0x05000184
	.section .text.x02008e58,"ax",%progbits
	.balign 4
	.global Func_02000e58
	.thumb_func
Func_02000e58:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_02008f64
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_020028b0
	movs r0, #0
	bl Func_020029a8
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	movs r5, #0
	strh r5, [r3]
	movs r3, #85
	adds r3, r3, r6
	mov r9, r3
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #99
	adds r3, r3, r7
	mov r8, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_02008ef8
.L_02008eb2:
	ldr r3, [r7, #8]
	ldr r2, .L_02008f68
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_02008ece
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_02008ece:
	ldr r3, .L_02008f6c
	adds r1, r6, #0
	ldr r2, [r3]
	ldrb r3, [r3]
	adds r1, #35
	lsls r3, r3, #12
	strh r3, [r6, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	movs r0, #1
	eors r3, r2
	strb r3, [r1]
	bl WaitFrames
	mov r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_02008eb2
.L_02008ef8:
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #68
	movs r2, #1
	add r3, r10
	strh r2, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	strh r2, [r3]
	ldr r3, [r7, #8]
	ldrh r1, [r7, #6]
	subs r2, #3
	asrs r3, r3, #19
	ands r3, r2
	asrs r1, r1, #13
	adds r3, r3, r1
	subs r3, #1
	lsls r3, r3, #19
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	ldr r0, .L_02008f60
	asrs r3, r3, #19
	ands r3, r2
	movs r2, #2
	ands r1, r2
	subs r3, r3, r1
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r6, #16]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #40]
	adds r3, r6, #0
	adds r3, #35
	strb r0, [r3]
	mov r2, r9
	movs r3, #3
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	b .L_02008f70
.L_02008f60:
	.4byte 0x00000001
.L_02008f64:
	.4byte gAdventure
.L_02008f68:
	.4byte 0x0003ffff
.L_02008f6c:
	.4byte gFrameCount
.L_02008f70:
	bl Camera_MoveWithinMapBounds
	bl Func_02002970
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_02008fb8
	lsls r3, r3, #9
	str r3, [r7, #52]
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r0, #0
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r6, #16]
	adds r2, r0, #0
	ldr r1, [r6, #8]
	adds r0, r7, #0
	bl Func_02002840
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_02008fd6
	b .L_02008fbc
	.2byte 0x0000
.L_02008fb8:
	.4byte 0x00000000
.L_02008fbc:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_02008fd6
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_02008fbc
.L_02008fd6:
	movs r0, #127
	bl Func_02002a40
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_02008ff6
.L_02008fe4:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_02008ff6
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02008fe4
.L_02008ff6:
	adds r0, r7, #0
	bl Func_02002848
	ldr r5, .L_02009040
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl Camera_FollowActor
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #70
	add r2, r10
	movs r3, #1
	strh r3, [r2]
	movs r3, #170
	lsls r3, r3, #1
	movs r6, #0
	add r3, r10
	strh r6, [r3]
	bl Func_020028b8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009040:
	.4byte gAdventure
	.section .text.x02009070,"ax",%progbits
	.balign 4
	.global Func_02001070
	.thumb_func
Func_02001070:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_020090d8
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Actor_GetById
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #0
	bne .L_020090ce
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020090ce
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020090ce
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020090dc
.L_020090ce:
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetAnimationSpeed
	b .L_0200921a
.L_020090d8:
	.4byte gAdventure
.L_020090dc:
	adds r0, r6, #0
	movs r1, #16
	bl Actor_SetAnimationSpeed
	adds r3, r6, #0
	adds r3, #100
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #31
	ands r3, r2
	cmp r3, #31
	bne .L_020090fc
	movs r0, #231
	bl Func_02002a40
.L_020090fc:
	ldr r3, [r7, #80]
	ldr r0, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
	movs r2, #2
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	bl Func_02002a20
	cmp r0, #255
	beq .L_020091fe
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_020029b0
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_020091fe
	ldr r2, .L_020091ec
	cmp r5, r2
	blt .L_020091fe
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_020091c4
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_0200915c
	subs r5, r3, r2
.L_0200915c:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_02001044
	cmp r0, #12
	bgt .L_0200917c
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_0200917c
	movs r2, #1
	mov r8, r2
.L_0200917c:
	mov r3, r8
	cmp r3, #0
	beq .L_020091c4
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020091c4
	ldrh r3, [r6, #6]
	str r6, [r7, #104]
	strh r3, [r7, #6]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #181
	lsls r2, r2, #1
	strb r3, [r1]
	add r2, r10
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_020091f0
	movs r2, #128
	ldr r0, .L_020091e8
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r0, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_020091c4:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_020091f4
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	ldr r1, [r6, #48]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	b .L_020091f8
.L_020091e8:
	.4byte 0x00000000
.L_020091ec:
	.4byte 0xffe00000
.L_020091f0:
	.4byte gAdventure
.L_020091f4:
	.4byte IwramMulQ16
.L_020091f8:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_0200921a
.L_020091fe:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_02009228
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02002820
	movs r0, #228
	bl Func_02002a40
	ldr r3, .L_0200922c
	str r5, [r3]
.L_0200921a:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009228:
	.4byte gData_020033e0
.L_0200922c:
	.4byte gData_02003400
	.section .text.x02009230,"ax",%progbits
	.global Func_02001230
	.thumb_func
Func_02001230:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_02002a40
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_020092e4
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	add r2, sp, #56
	adds r3, r3, r0
	str r3, [r2]
	mov r8, r2
	ldrh r0, [r5, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #16]
	mov r2, r8
	adds r3, r3, r0
	str r3, [r2, #8]
	movs r0, #140
	ldr r1, [r2]
	lsls r0, r0, #1
	ldr r2, [r5, #12]
	bl Func_02002828
	movs r1, #2
	adds r7, r0, #0
	bl Func_02002810
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r3, r7, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r2, .L_020092e0
	ldrh r3, [r5, #6]
	add r4, sp, #16
	strh r3, [r7, #6]
	adds r3, r7, #0
	adds r3, #100
	strh r6, [r3]
	subs r3, #2
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	ldr r3, .L_020092e8
	str r3, [r7, #108]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #1
	str r3, [r4]
	movs r3, #7
	str r3, [r4, #4]
	mov r3, r8
	ldr r0, [r3]
	ldr r2, [r3, #8]
	ldr r3, .L_020092ec
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_020092f0
.L_020092e0:
	.4byte 0x00000000
.L_020092e4:
	.4byte IwramMulQ16
.L_020092e8:
	.4byte Func_02001070
.L_020092ec:
	.4byte 0xfffa0000
.L_020092f0:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_02001c88
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009308,"ax",%progbits
	.global Func_02001308
	.thumb_func
Func_02001308:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_020093a4
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_02009396
	add r6, sp, #16
	movs r3, #3
	str r3, [r6]
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #14
	str r3, [r6, #4]
	bl Random16Far
	mov r2, r10
	lsls r3, r0, #3
	ldr r2, [r2, #8]
	adds r3, r3, r0
	lsrs r3, r3, #16
	subs r3, #4
	lsls r3, r3, #16
	mov r8, r2
	add r8, r3
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	movs r2, #32
	subs r2, r2, r3
	mov r3, r10
	ldr r5, [r3, #12]
	lsls r2, r2, #16
	adds r5, r5, r2
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r2, #160
	lsls r2, r2, #11
	lsls r0, r0, #16
	adds r0, r0, r2
	movs r1, #10
	bl __divsi3
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r0, [sp, #0]
	str r3, [sp, #8]
	mov r0, r8
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_02001c88
.L_02009396:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020093a4:
	.4byte gFrameCount
	.section .text.x020093a8,"ax",%progbits
	.global Func_020013a8
	.thumb_func
Func_020013a8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Actor_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Actor_GetById
	adds r7, r0, #0
	movs r0, #23
	bl Actor_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r1, #128
	mov r8, r2
	movs r2, #248
	movs r0, #24
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020028f8
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020028f8
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_020028f8
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_020028f8
	movs r0, #24
	bl Actor_GetById
	movs r3, #85
	movs r2, #0
	adds r3, r3, r5
	str r2, [r0, #24]
	strb r2, [r3]
	mov r11, r3
	ldr r3, [r5, #20]
	movs r0, #160
	str r3, [r5, #12]
	movs r3, #85
	adds r3, r3, r6
	strb r2, [r3]
	mov r9, r3
	ldr r3, [r6, #20]
	lsls r0, r0, #4
	str r3, [r6, #12]
	movs r3, #85
	adds r3, r3, r7
	strb r2, [r3]
	mov r10, r3
	ldr r3, [r7, #20]
	adds r0, #10
	str r3, [r7, #12]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009484
	ldr r3, [r6, #12]
	ldr r2, .L_0200951c
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_02009520
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02009524
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Actor_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #11
	bl Actor_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_02009484:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094ce
	ldr r3, [r7, #12]
	ldr r2, .L_0200951c
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02009520
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02009528
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_0200952c
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Actor_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Actor_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_020094ce:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200950e
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200950e
	ldr r3, [r6, #12]
	ldr r2, .L_02009530
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02009534
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	mov r2, r10
	strb r3, [r2]
	mov r2, r11
	strb r3, [r2]
.L_0200950e:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200951c:
	.4byte 0x00066640
.L_02009520:
	.4byte 0x0001eb80
.L_02009524:
	.4byte 0xfffd70c0
.L_02009528:
	.4byte 0x00028f40
.L_0200952c:
	.4byte 0xfffff800
.L_02009530:
	.4byte 0x00199900
.L_02009534:
	.4byte 0x001b8480
	.section .text.x02009538,"ax",%progbits
	.global Func_02001538
	.thumb_func
Func_02001538:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #16
	mov r10, r3
	movs r2, #232
	movs r3, #95
	movs r7, #128
	lsls r2, r2, #4
	str r3, [sp, #0]
	lsls r7, r7, #3
	add r2, r10
	add r7, r10
	mov r11, r2
.L_02009564:
	ldr r2, [r7, #20]
	ldr r0, [r7, #24]
	mov r9, r2
	cmp r0, #0
	blt .L_02009632
	cmp r0, r9
	bgt .L_02009632
	mov r1, r9
	lsls r0, r0, #15
	bl __divsi3
	bl Math_Sine
	movs r3, #232
	movs r2, #236
	lsls r3, r3, #5
	lsls r2, r2, #5
	adds r3, #140
	add r2, r10
	add r3, r10
	ldr r5, [r2]
	ldr r3, [r3]
	ldr r2, [r7, #24]
	subs r3, r3, r5
	mov r8, r0
	mov r1, r9
	adds r0, r2, #0
	muls r0, r3
	add r6, sp, #4
	bl __divsi3
	ldr r3, [r7, #12]
	adds r5, r5, r0
	mov r2, r8
	muls r2, r3
	adds r3, r2, #0
	adds r5, r5, r3
	str r5, [r6]
	ldr r0, [r7, #24]
	mov r1, r9
	lsls r0, r0, #15
	bl __divsi3
	bl Math_Sine
	movs r2, #232
	movs r3, #232
	lsls r2, r2, #5
	lsls r3, r3, #5
	adds r2, #132
	adds r3, #144
	add r2, r10
	add r3, r10
	ldr r5, [r2]
	ldr r3, [r3]
	ldr r2, [r7, #24]
	subs r3, r3, r5
	mov r8, r0
	mov r1, r9
	adds r0, r2, #0
	muls r0, r3
	bl __divsi3
	ldr r3, [r7, #16]
	adds r5, r5, r0
	mov r2, r8
	muls r2, r3
	adds r3, r2, #0
	adds r5, r5, r3
	movs r2, #232
	movs r3, #232
	str r5, [r6, #4]
	lsls r2, r2, #5
	lsls r3, r3, #5
	adds r2, #136
	adds r3, #148
	add r2, r10
	add r3, r10
	ldr r5, [r2]
	ldr r3, [r3]
	ldr r2, [r7, #24]
	subs r3, r3, r5
	adds r0, r2, #0
	muls r0, r3
	mov r1, r9
	bl __divsi3
	adds r5, r5, r0
	adds r0, r6, #0
	str r5, [r6, #8]
	bl Func_020029b0
	ldr r3, [r6]
	mov r2, r11
	str r3, [r2, #12]
	mov r0, r11
	ldr r3, [r6, #8]
	str r3, [r2, #16]
	bl Func_020029e8
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_02009632:
	ldr r2, [sp, #0]
	movs r3, #40
	subs r2, #1
	add r11, r3
	str r2, [sp, #0]
	adds r7, #28
	cmp r2, #0
	bge .L_02009564
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x02009718,"ax",%progbits
	.global Func_02001718
	.thumb_func
Func_02001718:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	movs r1, #237
	adds r5, r0, #0
	lsls r1, r1, #5
	movs r0, #220
	sub sp, #4
	bl HeapSlot_AllocFar
	ldr r3, .L_02009850
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r7, r0, #0
	ldr r0, [r3]
	bl Actor_GetById
	movs r3, #236
	lsls r3, r3, #5
	adds r2, r7, r3
	ldr r3, [r5]
	movs r1, #232
	str r3, [r2]
	lsls r1, r1, #5
	ldr r3, [r5, #4]
	adds r1, #132
	adds r2, r7, r1
	str r3, [r2]
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #136
	adds r2, r7, r3
	ldr r3, [r5, #8]
	adds r1, #8
	str r3, [r2]
	adds r2, r7, r1
	ldr r3, [r6]
	adds r1, #8
	str r3, [r2]
	movs r3, #232
	lsls r3, r3, #5
	adds r3, #144
	adds r2, r7, r3
	ldr r3, [r6, #4]
	mov r11, r0
	str r3, [r2]
	adds r2, r7, r1
	ldr r3, [r6, #8]
	ldr r0, .L_02009854
	str r3, [r2]
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_020027b8
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #3
	adds r2, r7, #0
	adds r5, r0, #0
	bl VramCache_Load
	movs r2, #232
	lsls r2, r2, #5
	adds r2, #154
	mov r9, r0
	adds r3, r7, r2
	mov r1, r9
	strh r1, [r3]
	adds r2, #2
	movs r1, #128
	adds r3, r7, r2
	lsls r1, r1, #3
	movs r2, #232
	strh r5, [r3]
	adds r1, r1, r7
	movs r3, #0
	lsls r2, r2, #4
	mov r10, r3
	mov r8, r1
	adds r6, r7, r2
.L_020097c8:
	movs r5, #15
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #1
	add r3, r9
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #4
	movs r2, #4
	movs r3, #0
	bl Func_020029e0
	ldrb r3, [r6, #5]
	movs r2, #32
	orrs r3, r2
	strb r3, [r6, #5]
	ldrb r3, [r6, #9]
	mov r0, r11
	ands r5, r3
	strb r5, [r6, #9]
	bl Func_020029f0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	lsls r0, r0, #2
	orrs r3, r0
	strb r3, [r6, #9]
	mov r0, r11
	bl Func_020029f8
	movs r3, #1
	mov r2, r8
	negs r3, r3
	subs r0, #1
	strh r0, [r6, #30]
	str r3, [r2, #24]
	movs r3, #1
	add r10, r3
	movs r1, #28
	mov r2, r10
	adds r6, #40
	add r8, r1
	cmp r2, #95
	ble .L_020097c8
	movs r1, #232
	lsls r1, r1, #5
	adds r1, #152
	adds r2, r7, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_02009858
	bl Task_Add
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009850:
	.4byte gAdventure
.L_02009854:
	.4byte 0x000001ef
.L_02009858:
	.4byte Func_02001538
	.section .text.x02009888,"ax",%progbits
	.balign 4
	.global Func_02001888
	.thumb_func
Func_02001888:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_02009a1c
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02009a20
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02009a24
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02009a28
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02009a2c
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_020098da
	b .L_02009a0e
.L_020098da:
	ldr r2, .L_02009a30
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_020098e8
	b .L_020099fe
.L_020098e8:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_020098f0
	b .L_020099fe
.L_020098f0:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_02009a34
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02009966
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_020099fe
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_020099fe
	cmp r4, #239
	bgt .L_020099fe
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009a38
	b .L_020099a2
.L_02009966:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_020099fe
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_020099fe
	cmp r4, #175
	bgt .L_020099fe
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009a3c
.L_020099a2:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02009a40
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_020099e0
	adds r0, r5, #0
	bl Func_020029f0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_020099f4
.L_020099e0:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_020099f4:
	adds r0, r6, #0
	mov r1, r11
	bl Func_020027d8
	adds r6, #12
.L_020099fe:
	ldr r3, .L_02009a2c
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02009a0e
	b .L_020098da
.L_02009a0e:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009a1c:
	.4byte 0xffff0000
.L_02009a20:
	.4byte gData_02003444
.L_02009a24:
	.4byte gResourceTableEntries
.L_02009a28:
	.4byte gData_02003488
.L_02009a2c:
	.4byte gData_02003446
.L_02009a30:
	.4byte gData_02003448
.L_02009a34:
	.4byte gData_02003548
.L_02009a38:
	.4byte 0x40002000
.L_02009a3c:
	.4byte 0xc000a000
.L_02009a40:
	.4byte gData_0200354a
	.section .text.x02009c4e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009c86,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009c88,"ax",%progbits
	.global Func_02001c88
	.thumb_func
Func_02001c88:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02009e40
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_02009cd0
	cmp r7, #0
	beq .L_02009cd0
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02009cd8
.L_02009cd0:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02009cd8:
	mov r3, r10
	bl Func_02002828
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02009ce6
	b .L_02009e32
.L_02009ce6:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02002810
	ldr r2, .L_02009e44
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02002820
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, .L_02009e48
	mov r1, r9
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	adds r0, r6, #0
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Scene_SetActorSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_02009e4c
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009e32
	cmp r7, #0
	beq .L_02009e32
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02009d68
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Actor_SetPartAttribute
.L_02009d68:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009d88
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Scene_SetActorSpritePriority
.L_02009d88:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_02009d9c
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02009d9c:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009de2
	ldr r3, .L_02009e44
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_02009dca
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02009ddc
.L_02009dca:
	ldr r2, .L_02009e4c
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02009e4c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02009ddc:
	bl __divsi3
	str r0, [r6, #52]
.L_02009de2:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009dfe
	adds r0, r6, #0
	movs r1, #1
	bl Func_02002810
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02002820
.L_02009dfe:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009e10
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_02009e10:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009e22
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_02009e22:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009e32
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02009e32:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009e40:
	.4byte gAdventure
.L_02009e44:
	.4byte gData_020033f4
.L_02009e48:
	.4byte BattleEffect_UpdateObjectMotionScaleAndLinkedAngle
.L_02009e4c:
	.4byte 0xffff0000
	.section .text.x02009f7c,"ax",%progbits
	.global Func_02001f7c
	.thumb_func
Func_02001f7c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200a1ac
	str r1, [sp, #40]
	mov r8, r0
	movs r1, #32
	add r1, r8
	mov r9, r1
	mov r12, r9
	adds r5, r2, #0
	mov r2, r12
	adds r6, r3, #0
	str r2, [sp, #8]
	ldr r3, .L_0200a1b0
	movs r1, #4
	ldr r7, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	add r0, sp, #44
	ldrh r0, [r0]
	mov r1, r8
	strh r0, [r1, #4]
	add r1, sp, #40
	ldrh r1, [r1]
	mov r3, r8
	strh r1, [r3]
	strh r5, [r3, #2]
	movs r3, #255
	lsls r3, r3, #8
	mov r5, r8
	mov r0, r8
	adds r3, #255
	mov r1, r8
	strh r6, [r5, #6]
	movs r2, #0
	strh r7, [r0, #8]
	strh r3, [r1, #12]
	mov r3, r8
	strh r2, [r3, #10]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	mov r12, r3
	lsls r2, r2, #1
	mov r1, r12
	add r2, r12
	adds r1, #236
	ldr r0, [r1]
	ldr r3, [r2, #8]
	ldr r5, [r2, #48]
	adds r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #32]
	adds r1, #4
	ldr r3, [r2, #12]
	ldr r2, [r1]
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #28]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #24]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r2, r2, #20
	lsls r2, r2, #7
	adds r2, r2, r0
	lsls r2, r2, #2
	asrs r3, r3, #20
	adds r5, r5, r2
	movs r0, #0
	str r3, [sp, #20]
	str r5, [sp, #36]
	str r0, [sp, #12]
	cmp r0, r3
	bge .L_0200a100
.L_0200a030:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200a0f4
.L_0200a044:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200a0e4
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200a0e4
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200a0e4
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	mov r0, r9
	adds r7, r2, r3
	strh r7, [r0]
	ldr r1, [sp, #12]
	ldr r2, [sp, #28]
	add r0, sp, #40
	ldrh r0, [r0]
	adds r6, r1, r2
	mov r3, r9
	mov r1, r9
	strh r6, [r3, #2]
	strh r0, [r1, #4]
	ldr r1, [sp, #40]
	movs r0, #10
	adds r1, #1
	adds r0, #255
	str r1, [sp, #40]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a098
	cmp r5, r10
	bne .L_0200a0d6
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200a0d6
.L_0200a098:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0d6
	mov r2, r8
	ldrh r4, [r2, #6]
	ldrh r5, [r2, #8]
	movs r3, #8
	ldrsh r1, [r2, r3]
	movs r3, #6
	ldrsh r0, [r2, r3]
	movs r2, #64
	adds r3, r2, #0
	ands r3, r4
	ands r2, r5
	lsls r3, r3, #16
	lsls r2, r2, #16
	asrs r3, r3, #16
	asrs r2, r2, #16
	orrs r7, r3
	orrs r6, r2
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002858
.L_0200a0d6:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200a0e4:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200a044
.L_0200a0f4:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200a030
.L_0200a100:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a158
	ldr r3, .L_0200a1b4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	mov r0, r8
	asrs r1, r3, #20
	ldr r3, [sp, #8]
	mov r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	cmp r2, r3
	bge .L_0200a158
.L_0200a132:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200a148
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200a148
	mov r0, r8
	strh r2, [r0, #12]
.L_0200a148:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200a132
.L_0200a158:
	movs r0, #128
	lsls r0, r0, #1
	bl Heap_Alloc
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200a166:
	ldr r3, .L_0200a1b8
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200a166
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramCache_Load
	adds r0, r5, #0
	bl Heap_Release
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a1bc
	bl Task_Add
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a1ac:
	.4byte Data_0200354c
.L_0200a1b0:
	.4byte IwramClearWords
.L_0200a1b4:
	.4byte gAdventure
.L_0200a1b8:
	.4byte 0x11111111
.L_0200a1bc:
	.4byte Func_02001e50
	.section .text.x0200a2b8,"ax",%progbits
	.global Func_020022b8
	.thumb_func
Func_020022b8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200a368
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r8, r0
	ldr r0, [r1]
	sub sp, #8
	mov r10, r1
	bl Actor_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #64
	asrs r7, r3, #20
	mov r3, r8
	ldrh r1, [r3, #6]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	orrs r7, r3
	ldrh r3, [r1, #8]
	ldr r5, [r6, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_020021c0
	movs r0, #229
	bl Func_02002a40
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #2
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002858
	movs r0, #12
	bl FieldEvent_WaitFrames
	movs r3, #128
	ldr r2, .L_0200a360
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200a364
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200a36c
	.2byte 0x0000
.L_0200a360:
	.4byte 0x00000000
.L_0200a364:
	.4byte 0x00008000
.L_0200a368:
	.4byte gAdventure
.L_0200a36c:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Actor_SetModeById
	movs r0, #16
	bl FieldEvent_WaitFrames
.L_0200a380:
	cmp r7, #5
	bne .L_0200a38a
	movs r0, #204
	bl Func_02002a40
.L_0200a38a:
	ldr r3, [r6, #24]
	ldr r1, .L_0200a3e8
	ldr r2, .L_0200a3ec
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200a3f0
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_0200a380
	ldr r3, .L_0200a3f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	mov r1, r8
	strh r3, [r1, #14]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a3e8:
	.4byte 0xfffffc00
.L_0200a3ec:
	.4byte 0xfffffd00
.L_0200a3f0:
	.4byte 0xffff6667
.L_0200a3f4:
	.4byte gAdventure
	.section .text.x0200a44e,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a450,"ax",%progbits
	.global Func_02002450
	.thumb_func
Func_02002450:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a5d4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Actor_GetById
	adds r7, r0, #0
	bl Func_020028b0
	movs r0, #0
	bl Func_020029a8
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_02002838
	movs r0, #1
	bl WaitFrames
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #214
	lsls r4, r4, #1
	movs r2, #128
	adds r3, r3, r4
	lsls r2, r2, #1
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_02002a40
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl FieldEvent_WaitFrames
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200a5d8
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200a4ea:
	mov r4, r10
	lsls r5, r4, #12
	adds r0, r5, #0
	bl Math_Cosine
	add r6, sp, #16
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl Math_Sine
	ldr r3, [r6]
	str r0, [r6, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200a5dc
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200a5e0
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_0200a5e4
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02001c88
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200a4ea
	movs r0, #188
	bl Func_02002a40
	ldr r5, .L_0200a5d4
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02002958
	ldr r0, [r5]
	movs r1, #49
	bl Actor_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02002870
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02002870
	bl Func_02002878
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02002958
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl FieldEvent_WaitFrames
	ldr r0, [r5]
	movs r1, #1
	bl Actor_SetModeById
	bl Func_020028b8
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a5d4:
	.4byte gAdventure
.L_0200a5d8:
	.4byte Effect_AdvanceMotion
.L_0200a5dc:
	.4byte 0xffffa000
.L_0200a5e0:
	.4byte 0xffffd000
.L_0200a5e4:
	.4byte 0x01090001
	.section .text.x0200a5e8,"ax",%progbits
	.global Func_020025e8
	.thumb_func
Func_020025e8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a690
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200a694
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_0200a684
.L_0200a61c:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200a678
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200a678
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a64c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02002248
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200a684
.L_0200a64c:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200a684
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_020022b8
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_0200a686
.L_0200a678:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200a61c
.L_0200a684:
	movs r0, #0
.L_0200a686:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a690:
	.4byte gAdventure
.L_0200a694:
	.4byte Data_0200354c
	.section .bss,"aw",%nobits
	.global gData_02003400
gData_02003400:
	.space 0x00000004
	.global gData_02003404
gData_02003404:
	.space 0x00000020
	.global gData_02003424
gData_02003424:
	.space 0x00000004
	.global gData_02003428
gData_02003428:
	.space 0x0000001c
	.global gData_02003444
gData_02003444:
	.space 0x00000002
	.global gData_02003446
gData_02003446:
	.space 0x00000002
	.global gData_02003448
gData_02003448:
	.space 0x00000040
	.global gData_02003488
gData_02003488:
	.space 0x000000c0
	.global gData_02003548
gData_02003548:
	.space 0x00000002
	.global gData_0200354a
gData_0200354a:
	.space 0x00000002
	.global Data_0200354c
Data_0200354c:
