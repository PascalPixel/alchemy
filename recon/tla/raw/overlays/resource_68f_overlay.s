.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008708,"ax",%progbits
	.balign 4
	.global Func_02000708
	.thumb_func
Func_02000708:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #60
	add r7, sp, #20
	movs r3, #1
	str r1, [sp, #16]
	str r3, [r7]
	movs r3, #24
	adds r3, #255
	strh r3, [r7, #24]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r7, #20]
	str r3, [r7, #16]
	ldr r3, .L_0200878c
	str r2, [r7, #12]
	str r2, [r7, #8]
	str r3, [r7, #36]
	ldr r3, [sp, #16]
	movs r2, #0
	mov r11, r0
	mov r9, r2
	cmp r3, #0
	beq .L_020087cc
.L_02008744:
	bl Random16Far
	ldr r3, .L_02008788
	ands r0, r3
	lsls r0, r0, #12
	strh r0, [r7, #32]
	bl Random16Far
	mov r8, r0
	movs r2, #15
	mov r3, r8
	ands r3, r2
	mov r10, r2
	mov r8, r3
	subs r2, #23
	add r8, r2
	mov r3, r8
	lsls r3, r3, #14
	mov r8, r3
	bl Random16Far
	mov r2, r10
	adds r5, r0, #0
	ands r5, r2
	bl Random16Far
	mov r2, r11
	ldr r6, [r2]
	movs r3, #31
	ands r3, r0
	lsls r3, r3, #16
	subs r6, r6, r3
	b .L_02008790
	.2byte 0x0000
.L_02008788:
	.4byte 0x0000000f
.L_0200878c:
	.4byte Func_020006d4
.L_02008790:
	movs r3, #240
	lsls r3, r3, #12
	adds r6, r6, r3
	bl Random16Far
	mov r3, r11
	mov r2, r10
	ldr r1, [r3, #4]
	ands r0, r2
	ldr r2, [r3, #8]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #175
	lsls r0, r0, #16
	lsls r3, r3, #17
	adds r5, #8
	adds r1, r1, r0
	str r3, [sp, #8]
	lsls r5, r5, #14
	mov r3, r8
	adds r0, r6, #0
	str r5, [sp, #0]
	str r7, [sp, #12]
	bl Func_020000b8
	ldr r3, [sp, #16]
	movs r2, #1
	add r9, r2
	cmp r9, r3
	bne .L_02008744
.L_020087cc:
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020091de,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009b48,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02001b48
	.thumb_func
Func_02001b48:
	push {r5, r6, r7, lr}
	mov r7, fp
	mov r6, sl
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_ja_02009ba8
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3, #0]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_ja_02009bac
	ldr r3, .L_ja_02009ba4
	adds r7, r0, #0
	strh r3, [r2, #0]
.L_ja_02009b6e:
	bl Func_02001868
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_ja_02009bb0
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov sl, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, sl
	str r3, [sp, #12]
	str r3, [r0, #0]
	b .L_ja_02009bb4
.L_ja_02009ba4:
	.4byte 0x00000000
.L_ja_02009ba8:
	.4byte gMainRoutineWork
.L_ja_02009bac:
	.4byte gData_02003cd0
.L_ja_02009bb0:
	.4byte 0xfff00000
.L_ja_02009bb4:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3, #0]
	ldr r2, [sp, #16]
	bl Func_02002fe0
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_ja_02009c20
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_ja_02009c30
	ldr r3, .L_ja_02009c24
	movs r2, #15
	ldr r3, [r3, #0]
	ldr r1, .L_ja_02009c28
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_ja_02009c2c
	cmp r3, r2
	bne .L_ja_02009c5c
	b .L_ja_02009e24
.L_ja_02009c20:
	.4byte IwramMulQ16
.L_ja_02009c24:
	.4byte gInput
.L_ja_02009c28:
	.4byte gData_020032b2
.L_ja_02009c2c:
	.4byte 0xffff0000
.L_ja_02009c30:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_ja_02009c58
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_ja_02009c5c
.L_ja_02009c58:
	.4byte 0xffffc000
.L_ja_02009c5c:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, sl
	ldr r1, [r2, #0]
	ldrb r0, [r3, #0]
	ldr r2, [r2, #8]
	bl Func_02002fe0
	mov fp, r0
	cmp r0, #255
	beq .L_ja_02009cde
	ldr r3, [sp, #4]
	mov r2, sl
	ldrb r0, [r3, #0]
	ldr r1, [r2, #0]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_02009cde
	ldr r0, [sp, #12]
	mov r2, sl
	str r0, [r2, #0]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2, #0]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_02002fb0
	adds r0, r7, #0
	movs r1, #2
	bl Func_02002f80
	adds r0, r7, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	adds r0, r7, #0
	bl Func_02002fb8
	ldr r3, .L_ja_02009e34
	str r3, [r7, #108]
	b .L_ja_02009dba
.L_ja_02009cde:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_ja_02009e06
.L_ja_02009cf2:
	ldr r2, [sp, #4]
	ldr r1, [r6, #0]
	ldrb r0, [r2, #0]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_02009dda
	ldr r3, .L_ja_02009e38
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_02009dda
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov sl, r0
	mov r8, r2
.L_ja_02009d2a:
	ldr r3, [r5, #0]
	cmp r3, #0
	beq .L_ja_02009d54
	mov r3, r8
	ldrb r2, [r3, #0]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_ja_02009d54
	cmp r5, r7
	beq .L_ja_02009d54
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02003028
	cmp r0, #0
	bge .L_ja_02009dda
.L_ja_02009d54:
	movs r0, #1
	add sl, r0
	movs r2, #128
	mov r3, sl
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_ja_02009d2a
	ldr r0, [r6, #0]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6, #0]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_02002fb0
	b .L_ja_02009d92
.L_ja_02009d8a:
	movs r0, #1
	bl wait
	adds r5, #1
.L_ja_02009d92:
	cmp r5, #179
	bgt .L_ja_02009daa
	adds r0, r7, #0
	bl Func_02003018
	cmp r0, #0
	bne .L_ja_02009daa
	ldr r3, .L_ja_02009e38
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_ja_02009d8a
.L_ja_02009daa:
	ldr r3, .L_ja_02009e38
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_02009e06
	ldr r3, [sp, #24]
	cmp fp, r3
	bne .L_ja_02009e00
.L_ja_02009dba:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2, #0]
	ldr r1, [r6, #0]
	ldr r2, [r6, #8]
	bl Func_02002fe0
	mov fp, r0
	cmp r0, #255
	bne .L_ja_02009cf2
.L_ja_02009dda:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02002fb0
	adds r0, r7, #0
	bl Func_02002fb8
	movs r0, #2
	bl wait
	b .L_ja_02009b6e
.L_ja_02009e00:
	movs r0, #10
	bl wait
.L_ja_02009e06:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1, #0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1, #0]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02002f80
.L_ja_02009e24:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov sl, r6
	mov fp, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_02009e34:
	.4byte Func_020017dc
.L_ja_02009e38:
	.4byte gData_02003cd0
	.endif
	.section .text.x0200a22c,"ax",%progbits
	.global Func_0200222c
	.thumb_func
Func_0200222c:
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
	beq .L_0200a312
.L_0200a252:
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
	ldr r2, .L_0200a348
	adds r3, r5, r2
	ldr r2, .L_0200a34c
	asrs r3, r3, #2
	adds r6, r3, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2a0
	ldr r0, [sp, #12]
	movs r1, #0
	movs r2, #0
	bl Func_02003088
	b .L_0200a300
.L_0200a2a0:
	adds r0, r7, #0
	bl Func_02003160
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r5, r5, r3
	str r5, [sp, #4]
	mov r2, r9
	ldrb r0, [r2]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Func_02003010
	mov r3, r9
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r10, r0
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	ldr r0, [sp, #12]
	bl Func_020030c0
	ldr r2, [sp, #4]
	movs r3, #128
	asrs r5, r5, #19
	strb r3, [r2, #3]
	adds r5, #4
	mov r3, r9
	adds r2, r5, #0
	ldrb r0, [r3]
	mov r1, r10
	bl Func_02003180
	add r8, r6
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a300
	adds r0, r7, #0
	movs r1, #0
	bl Func_02002f80
.L_0200a300:
	movs r3, #4
	add r11, r3
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200a252
.L_0200a312:
	ldr r3, .L_0200a350
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
	bge .L_0200a33a
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_0200a33a:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a348:
	.4byte 0xfdff0000
.L_0200a34c:
	.4byte gMapShapeGrid
.L_0200a350:
	.4byte gMainRoutineWork
	.section .text.x0200a5e0,"ax",%progbits
	.balign 4
	.global Func_020025e0
	.thumb_func
Func_020025e0:
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
	ldr r5, .L_0200a73c
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_02003090
	ldr r1, [r5]
	movs r0, #9
	bl Func_02003090
	ldr r1, [r5]
	movs r0, #10
	bl Func_02003090
	movs r0, #1
	bl wait
	movs r0, #8
	bl Func_020025c8
	movs r0, #9
	bl Func_020025c8
	movs r0, #10
	bl Func_020025c8
	movs r1, #0
	movs r0, #9
	bl Actor_SetModeById
	movs r0, #1
	bl wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003088
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02003088
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_02003088
	movs r0, #1
	bl wait
	b .L_0200a71a
.L_0200a666:
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
	ldr r2, .L_0200a740
	adds r3, r6, r2
	ldr r2, .L_0200a744
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a6bc
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_02003088
	b .L_0200a716
.L_0200a6bc:
	adds r0, r5, #0
	bl Func_02003160
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02003010
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_020030c0
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02003180
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a716
	mov r0, r9
	movs r1, #9
	bl ObjectVisual_CopyAttributes
.L_0200a716:
	movs r3, #4
	add r11, r3
.L_0200a71a:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200a666
	movs r0, #10
	bl wait
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a73c:
	.4byte gMainRoutineWork
.L_0200a740:
	.4byte 0xfdff0000
.L_0200a744:
	.4byte gMapShapeGrid
	.section .text.x0200ae00,"ax",%progbits
	.balign 4
	.global Func_02002e00
	.thumb_func
Func_02002e00:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #8
	movs r0, #220
	sub sp, #4
	bl HeapSlot_AllocFar
	adds r6, r0, #0
	ldr r0, .L_0200aeb4
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02002f40
	bl Resource_FindFreeEntry
	movs r1, #160
	lsls r1, r1, #3
	adds r2, r6, #0
	adds r5, r0, #0
	bl VramCache_Load
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
.L_0200ae58:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02003190
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
	bge .L_0200ae58
	movs r1, #176
	lsls r1, r1, #5
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200aeb8
	bl Task_Add
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200aeb4:
	.4byte 0x000001f0
.L_0200aeb8:
	.4byte Func_02002cc0
	.section .bss,"aw",%nobits
	.global Data_02003ccc
Data_02003ccc:
	.space 0x00000004
	.global gData_02003cd0
gData_02003cd0:
