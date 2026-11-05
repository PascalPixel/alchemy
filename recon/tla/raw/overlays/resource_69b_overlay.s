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
	.4byte gMainRoutineWork
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
	.4byte gMainRoutineWork
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
	.section .text.x020089da,"ax",%progbits
	.2byte 0x0000
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
	.section .text.x02009e22,"ax",%progbits
	.2byte 0x0000
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
