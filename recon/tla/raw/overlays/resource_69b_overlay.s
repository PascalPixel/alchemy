.syntax unified
	.thumb
	.section .text.x020080d0,"ax",%progbits
	.global Func_020000d0
	.thumb_func
Func_020000d0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200810c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r3, [r3]
	adds r5, r0, #0
	mov r8, r3
	mov r0, r8
	bl Actor_GetById
	adds r6, r0, #0
	bl Func_0200224c
	movs r0, #0
	bl Func_020022dc
	ldr r2, .L_02008110
	cmp r5, #32
	bne .L_02008114
	movs r3, #136
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r2, #4]
	movs r3, #148
	b .L_02008122
.L_0200810c:
	.4byte gAdventure
.L_02008110:
	.4byte gData_02002b88
.L_02008114:
	movs r3, #248
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r2, #4]
	movs r3, #144
.L_02008122:
	lsls r3, r3, #18
	str r3, [r2, #8]
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200821c
	bl Task_Add
	mov r0, r8
	movs r1, #1
	bl Camera_FollowActor
	movs r5, #35
.L_0200813a:
	ldr r3, [r6, #12]
	ldr r2, .L_02008220
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #12]
	subs r5, #1
	bl wait
	cmp r5, #0
	bge .L_0200813a
	movs r5, #128
	lsls r5, r5, #9
	movs r0, #140
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	str r5, [r6, #24]
	str r5, [r6, #28]
	lsls r0, r0, #1
	bl Actor_Spawn
	movs r1, #2
	adds r7, r0, #0
	bl Func_020021b4
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #24]
	str r5, [r7, #28]
	ldr r1, [r7, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r2, #128
	ldr r3, [r6, #12]
	lsls r2, r2, #14
	cmp r3, r2
	ble .L_020081be
.L_0200818e:
	ldr r2, .L_02008220
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #12]
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r6, #6]
	ldr r3, [r6, #8]
	ldr r2, .L_02008224
	str r3, [r7, #8]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r6, #16]
	str r3, [r7, #16]
	bl wait
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #14
	cmp r3, r2
	bgt .L_0200818e
.L_020081be:
	adds r0, r7, #0
	movs r1, #6
	bl Func_020021b4
	movs r5, #14
.L_020081c8:
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl wait
	cmp r5, #0
	bge .L_020081c8
	adds r0, r7, #0
	bl Func_020021d4
	mov r0, r8
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r3, #128
	ldr r2, .L_02008218
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #34
	strb r2, [r3]
	adds r2, r6, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	ldr r0, .L_0200821c
	bl Task_RemoveFar
	bl Func_02000054
	bl Func_02002254
	b .L_02008228
	.2byte 0x0000
.L_02008218:
	.4byte 0x00000000
.L_0200821c:
	.4byte Func_020000b4
.L_02008220:
	.4byte 0xffff0000
.L_02008224:
	.4byte 0xfffa0000
.L_02008228:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008230,"ax",%progbits
	.global Func_02000230
	.thumb_func
Func_02000230:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_020083dc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r3, [r3]
	adds r5, r0, #0
	mov r8, r3
	mov r0, r8
	bl Actor_GetById
	adds r7, r0, #0
	bl Func_0200224c
	movs r0, #0
	bl Func_020022dc
	ldr r2, .L_020083e0
	cmp r5, #30
	bne .L_0200826c
	movs r3, #136
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r2, #4]
	movs r3, #148
	b .L_0200827a
.L_0200826c:
	movs r3, #248
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r2, #4]
	movs r3, #144
.L_0200827a:
	lsls r3, r3, #18
	str r3, [r2, #8]
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020083e4
	bl Task_Add
	movs r5, #128
	mov r0, r8
	movs r1, #1
	bl Camera_FollowActor
	lsls r5, r5, #9
	movs r0, #140
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	ldr r1, [r7, #8]
	str r5, [r7, #24]
	str r5, [r7, #28]
	lsls r0, r0, #1
	bl Actor_Spawn
	movs r1, #2
	adds r6, r0, #0
	bl Func_020021b4
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #24]
	str r5, [r6, #28]
	ldr r1, [r6, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r3, [r7, #12]
	movs r2, #216
	lsls r2, r2, #15
	cmp r3, r2
	bge .L_02008300
.L_020082d0:
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #12]
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r7, #6]
	ldr r3, [r7, #8]
	ldr r2, .L_020083e8
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	bl wait
	ldr r3, [r7, #12]
	ldr r2, .L_020083ec
	cmp r3, r2
	ble .L_020082d0
.L_02008300:
	movs r3, #0
	str r3, [r7, #24]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	movs r2, #144
	lsls r2, r2, #16
	cmp r3, r2
	bge .L_0200832c
.L_02008310:
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl wait
	ldr r3, [r7, #12]
	ldr r2, .L_020083f0
	cmp r3, r2
	ble .L_02008310
.L_0200832c:
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #24]
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r6, #28]
	movs r5, #31
.L_0200833a:
	ldr r3, [r6, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	movs r2, #128
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #28]
	movs r0, #1
	subs r5, #1
	bl wait
	cmp r5, #0
	bge .L_0200833a
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #24]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	adds r0, r6, #0
	movs r1, #6
	bl Func_020021b4
	movs r3, #192
	lsls r3, r3, #7
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r5, #19
.L_02008378:
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r7, #6]
	ldr r3, [r7, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r0, #1
	subs r5, #1
	bl wait
	cmp r5, #0
	bge .L_02008378
	adds r0, r6, #0
	bl Func_020021d4
	mov r0, r8
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r3, #128
	lsls r3, r3, #7
	adds r2, r7, #0
	strh r3, [r7, #6]
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	adds r2, #51
	movs r3, #3
	strb r3, [r2]
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r7, #12]
	str r3, [r7, #20]
	ldr r0, .L_020083e4
	bl Task_RemoveFar
	bl Func_02000054
	bl Func_02002254
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020083dc:
	.4byte gAdventure
.L_020083e0:
	.4byte gData_02002b88
.L_020083e4:
	.4byte Func_020000b4
.L_020083e8:
	.4byte 0xfffa0000
.L_020083ec:
	.4byte 0x006bffff
.L_020083f0:
	.4byte 0x008fffff
	.section .text.x020087d0,"ax",%progbits
	.global Func_020007d0
	.thumb_func
Func_020007d0:
	push {r5, r6, r7, lr}
	movs r7, #0
	movs r6, #0
.L_020087d6:
	movs r0, #168
	movs r1, #0
	movs r2, #0
	movs r3, #0
	lsls r0, r0, #2
	bl Func_020021cc
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	subs r2, #50
	strb r3, [r2]
	bl Random16Far
	movs r3, #192
	lsls r3, r3, #11
	lsls r0, r0, #2
	adds r0, r0, r3
	str r0, [r5, #48]
	bl Random16Far
	movs r3, #128
	lsls r0, r0, #11
	lsls r3, r3, #4
	lsrs r0, r0, #16
	adds r0, r0, r3
	ldr r3, .L_02008850
	str r0, [r5, #52]
	str r3, [r5, #108]
	bl Random16Far
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	adds r3, #2
	strh r6, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #2
	bl Actor_SetPartAttribute
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r7, #1
	strb r3, [r1, #9]
	adds r6, #5
	cmp r7, #22
	bne .L_020087d6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008850:
	.4byte Func_02000720
	.section .text.x020089da,"ax",%progbits
	.2byte 0x0000
	.section .text.x020089e8,"ax",%progbits
	.global Func_020009e8
	.thumb_func
Func_020009e8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008a94
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_020089fa
	adds r0, #3
.L_020089fa:
	asrs r0, r0, #2
	movs r1, #5
	bl __modsi3
	ldr r3, .L_02008a98
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008a4e
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
	beq .L_02008a2e
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_02008a8a
.L_02008a2e:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_02008a8a
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02008a8a
.L_02008a4e:
	movs r5, #0
	movs r6, #4
.L_02008a52:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl __modsi3
	ldr r3, .L_02008a9c
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_02008a52
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_02008aa0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02008a94
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02008a8a:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a94:
	.4byte gData_02002b34
.L_02008a98:
	.4byte gData_02002b38
.L_02008a9c:
	.4byte gData_02002b98
.L_02008aa0:
	.4byte 0x05000184
	.section .text.x02008b7c,"ax",%progbits
	.global Func_02000b7c
	.thumb_func
Func_02000b7c:
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
	ldr r3, .L_02008c88
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_0200224c
	movs r0, #0
	bl Func_020022dc
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
	beq .L_02008c1c
.L_02008bd6:
	ldr r3, [r7, #8]
	ldr r2, .L_02008c8c
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_02008bf2
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_02008bf2:
	ldr r3, .L_02008c90
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
	bl wait
	mov r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_02008bd6
.L_02008c1c:
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
	ldr r0, .L_02008c84
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
	b .L_02008c94
.L_02008c84:
	.4byte 0x00000001
.L_02008c88:
	.4byte gAdventure
.L_02008c8c:
	.4byte 0x0003ffff
.L_02008c90:
	.4byte gFrameCount
.L_02008c94:
	bl Camera_MoveWithinMapBounds
	bl Func_020022b4
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_02008cdc
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
	bl Func_020021ec
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_02008cfa
	b .L_02008ce0
	.2byte 0x0000
.L_02008cdc:
	.4byte 0x00000000
.L_02008ce0:
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #59
	bgt .L_02008cfa
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_02008ce0
.L_02008cfa:
	movs r0, #127
	bl Func_0200232c
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_02008d1a
.L_02008d08:
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #59
	bgt .L_02008d1a
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02008d08
.L_02008d1a:
	adds r0, r7, #0
	bl Func_020021f4
	ldr r5, .L_02008d64
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
	bl Func_02002254
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008d64:
	.4byte gAdventure
	.section .text.x02008d94,"ax",%progbits
	.balign 4
	.global Func_02000d94
	.thumb_func
Func_02000d94:
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
	ldr r3, .L_02008dfc
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
	bne .L_02008df2
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008df2
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008df2
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02008e00
.L_02008df2:
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetAnimationSpeed
	b .L_02008f3e
.L_02008dfc:
	.4byte gAdventure
.L_02008e00:
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
	bne .L_02008e20
	movs r0, #231
	bl Func_0200232c
.L_02008e20:
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
	bl Func_020022fc
	cmp r0, #255
	beq .L_02008f22
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_020022e4
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_02008f22
	ldr r2, .L_02008f10
	cmp r5, r2
	blt .L_02008f22
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02008ee8
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_02008e80
	subs r5, r3, r2
.L_02008e80:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_02000d68
	cmp r0, #12
	bgt .L_02008ea0
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_02008ea0
	movs r2, #1
	mov r8, r2
.L_02008ea0:
	mov r3, r8
	cmp r3, #0
	beq .L_02008ee8
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008ee8
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
	ldr r3, .L_02008f14
	movs r2, #128
	ldr r0, .L_02008f0c
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
.L_02008ee8:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_02008f18
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
	b .L_02008f1c
.L_02008f0c:
	.4byte 0x00000000
.L_02008f10:
	.4byte 0xffe00000
.L_02008f14:
	.4byte gAdventure
.L_02008f18:
	.4byte IwramMulQ16
.L_02008f1c:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_02008f3e
.L_02008f22:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_02008f4c
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_020021c4
	movs r0, #228
	bl Func_0200232c
	ldr r3, .L_02008f50
	str r5, [r3]
.L_02008f3e:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008f4c:
	.4byte gData_02002b3c
.L_02008f50:
	.4byte Data_02002b94
	.section .text.x02008f54,"ax",%progbits
	.global Func_02000f54
	.thumb_func
Func_02000f54:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_0200232c
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_02009008
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
	bl Func_020021cc
	movs r1, #2
	adds r7, r0, #0
	bl Func_020021b4
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
	ldr r2, .L_02009004
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
	ldr r3, .L_0200900c
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
	ldr r3, .L_02009010
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_02009014
.L_02009004:
	.4byte 0x00000000
.L_02009008:
	.4byte IwramMulQ16
.L_0200900c:
	.4byte Func_02000d94
.L_02009010:
	.4byte 0xfffa0000
.L_02009014:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_0200165c
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020090cc,"ax",%progbits
	.global Func_020010cc
	.thumb_func
Func_020010cc:
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
	bl Func_0200227c
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_0200227c
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200227c
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_0200227c
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
	beq .L_020091a8
	ldr r3, [r6, #12]
	ldr r2, .L_02009240
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_02009244
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02009248
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
.L_020091a8:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020091f2
	ldr r3, [r7, #12]
	ldr r2, .L_02009240
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02009244
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_0200924c
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_02009250
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
.L_020091f2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009232
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009232
	ldr r3, [r6, #12]
	ldr r2, .L_02009254
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02009258
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
.L_02009232:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009240:
	.4byte 0x00066640
.L_02009244:
	.4byte 0x0001eb80
.L_02009248:
	.4byte 0xfffd70c0
.L_0200924c:
	.4byte 0x00028f40
.L_02009250:
	.4byte 0xfffff800
.L_02009254:
	.4byte 0x00199900
.L_02009258:
	.4byte 0x001b8480
	.section .text.x0200925c,"ax",%progbits
	.global Func_0200125c
	.thumb_func
Func_0200125c:
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
	ldr r3, .L_020093f0
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_020093f4
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_020093f8
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_020093fc
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02009400
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_020092ae
	b .L_020093e2
.L_020092ae:
	ldr r2, .L_02009404
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_020092bc
	b .L_020093d2
.L_020092bc:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_020092c4
	b .L_020093d2
.L_020092c4:
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
	ldr r3, .L_02009408
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_0200933a
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
	bhi .L_020093d2
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_020093d2
	cmp r4, #239
	bgt .L_020093d2
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
	ldr r3, .L_0200940c
	b .L_02009376
.L_0200933a:
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
	bhi .L_020093d2
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_020093d2
	cmp r4, #175
	bgt .L_020093d2
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
	ldr r3, .L_02009410
.L_02009376:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02009414
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_020093b4
	adds r0, r5, #0
	bl Func_020022f4
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
	b .L_020093c8
.L_020093b4:
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
.L_020093c8:
	adds r0, r6, #0
	mov r1, r11
	bl Func_0200218c
	adds r6, #12
.L_020093d2:
	ldr r3, .L_02009400
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_020093e2
	b .L_020092ae
.L_020093e2:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020093f0:
	.4byte 0xffff0000
.L_020093f4:
	.4byte gData_02002bd8
.L_020093f8:
	.4byte gResourceTableEntries
.L_020093fc:
	.4byte Data_02002c1c
.L_02009400:
	.4byte gData_02002bda
.L_02009404:
	.4byte gData_02002bdc
.L_02009408:
	.4byte gData_02002cdc
.L_0200940c:
	.4byte 0x40002000
.L_02009410:
	.4byte 0xc000a000
.L_02009414:
	.4byte gData_02002cde
	.section .text.x02009622,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200965a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009950,"ax",%progbits
	.global Func_02001950
	.thumb_func
Func_02001950:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_02009b80
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
	ldr r3, .L_02009b84
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
	bge .L_02009ad4
.L_02009a04:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_02009ac8
.L_02009a18:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_02009ab8
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_02009ab8
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_02009ab8
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
	bne .L_02009a6c
	cmp r5, r10
	bne .L_02009aaa
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_02009aaa
.L_02009a6c:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009aaa
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
	bl Func_02002204
.L_02009aaa:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_02009ab8:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_02009a18
.L_02009ac8:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_02009a04
.L_02009ad4:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009b2c
	ldr r3, .L_02009b88
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
	bge .L_02009b2c
.L_02009b06:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_02009b1c
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_02009b1c
	mov r0, r8
	strh r2, [r0, #12]
.L_02009b1c:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_02009b06
.L_02009b2c:
	movs r0, #128
	lsls r0, r0, #1
	bl Heap_Alloc
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_02009b3a:
	ldr r3, .L_02009b8c
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_02009b3a
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
	ldr r0, .L_02009b90
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
.L_02009b80:
	.4byte gData_02002ce0
.L_02009b84:
	.4byte IwramClearWords
.L_02009b88:
	.4byte gAdventure
.L_02009b8c:
	.4byte 0x11111111
.L_02009b90:
	.4byte Func_02001824
	.section .text.x02009c8c,"ax",%progbits
	.global Func_02001c8c
	.thumb_func
Func_02001c8c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_02009d3c
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
	bl Func_02001b94
	movs r0, #229
	bl Func_0200232c
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
	bl Func_02002204
	movs r0, #12
	bl FieldEvent_Wait
	movs r3, #128
	ldr r2, .L_02009d34
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
	ldr r2, .L_02009d38
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_02009d40
	.2byte 0x0000
.L_02009d34:
	.4byte 0x00000000
.L_02009d38:
	.4byte 0x00008000
.L_02009d3c:
	.4byte gAdventure
.L_02009d40:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Actor_SetModeById
	movs r0, #16
	bl FieldEvent_Wait
.L_02009d54:
	cmp r7, #5
	bne .L_02009d5e
	movs r0, #204
	bl Func_0200232c
.L_02009d5e:
	ldr r3, [r6, #24]
	ldr r1, .L_02009dbc
	ldr r2, .L_02009dc0
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_02009dc4
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl wait
	cmp r7, #39
	ble .L_02009d54
	ldr r3, .L_02009dc8
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
.L_02009dbc:
	.4byte 0xfffffc00
.L_02009dc0:
	.4byte 0xfffffd00
.L_02009dc4:
	.4byte 0xffff6667
.L_02009dc8:
	.4byte gAdventure
	.section .text.x02009e22,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009e24,"ax",%progbits
	.global Func_02001e24
	.thumb_func
Func_02001e24:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009fa8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Actor_GetById
	adds r7, r0, #0
	bl Func_0200224c
	movs r0, #0
	bl Func_020022dc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_020021e4
	movs r0, #1
	bl wait
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
	bl Func_0200232c
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl FieldEvent_Wait
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_02009fac
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_02009ebe:
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
	ldr r3, .L_02009fb0
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_02009fb4
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
	ldr r4, .L_02009fb8
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_0200165c
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_02009ebe
	movs r0, #188
	bl Func_0200232c
	ldr r5, .L_02009fa8
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_0200229c
	ldr r0, [r5]
	movs r1, #49
	bl Actor_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02002224
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02002224
	bl Func_0200222c
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_0200229c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl FieldEvent_Wait
	ldr r0, [r5]
	movs r1, #1
	bl Actor_SetModeById
	bl Func_02002254
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009fa8:
	.4byte gAdventure
.L_02009fac:
	.4byte Effect_AdvanceMotion
.L_02009fb0:
	.4byte 0xffffa000
.L_02009fb4:
	.4byte 0xffffd000
.L_02009fb8:
	.4byte 0x01090001
	.section .text.x02009fbc,"ax",%progbits
	.global Func_02001fbc
	.thumb_func
Func_02001fbc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a064
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200a068
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
	bge .L_0200a058
.L_02009ff0:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200a04c
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200a04c
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a020
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02001c1c
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200a058
.L_0200a020:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200a058
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02001c8c
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
	b .L_0200a05a
.L_0200a04c:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_02009ff0
.L_0200a058:
	movs r0, #0
.L_0200a05a:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a064:
	.4byte gAdventure
.L_0200a068:
	.4byte gData_02002ce0
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gData_02002b60
gData_02002b60:
	.space 0x00000028
	.global gData_02002b88
gData_02002b88:
	.space 0x0000000c
	.global Data_02002b94
Data_02002b94:
	.space 0x00000004
	.global gData_02002b98
gData_02002b98:
	.space 0x00000020
	.global gData_02002bb8
gData_02002bb8:
	.space 0x00000004
	.global gData_02002bbc
gData_02002bbc:
	.space 0x0000001c
	.global gData_02002bd8
gData_02002bd8:
	.space 0x00000002
	.global gData_02002bda
gData_02002bda:
	.space 0x00000002
	.global gData_02002bdc
gData_02002bdc:
	.space 0x00000040
	.global Data_02002c1c
Data_02002c1c:
	.space 0x000000c0
	.global gData_02002cdc
gData_02002cdc:
	.space 0x00000002
	.global gData_02002cde
gData_02002cde:
	.space 0x00000002
	.global gData_02002ce0
gData_02002ce0:
