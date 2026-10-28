.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008668,"ax",%progbits
	.global Func_02000668
	.thumb_func
Func_02000668:
	push {r5, lr}
	ldr r2, [r0, #44]
	ldr r3, [r0, #36]
	adds r3, r3, r2
	asrs r3, r3, #1
	ldr r2, .L_020086ac
	cmp r3, #0
	bge .L_0200867a
	negs r3, r3
.L_0200867a:
	movs r4, #187
	lsls r4, r4, #8
	adds r4, #128
	cmp r3, r4
	bls .L_0200868a
	ldr r3, [r2]
	adds r3, #4
	b .L_02008692
.L_0200868a:
	cmp r3, #0
	beq .L_02008694
	ldr r3, [r2]
	adds r3, #2
.L_02008692:
	str r3, [r2]
.L_02008694:
	ldr r5, .L_020086ac
	ldr r3, [r5]
	adds r3, r3, r1
	str r3, [r5]
	cmp r3, #40
	ble .L_020086a8
	bl Func_020006b0
	movs r3, #0
	str r3, [r5]
.L_020086a8:
	pop {r5, pc}
	.2byte 0x0000
.L_020086ac:
	.4byte gData_02006294
	.section .text.x02008f60,"ax",%progbits
	.global Func_02000f60
	.thumb_func
Func_02000f60:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r7, .L_020092a4
	movs r2, #150
	ldr r3, [r7]
	lsls r2, r2, #1
	sub sp, #8
	cmp r3, r2
	bge .L_02008f80
	bl Func_02000d18
	cmp r0, #0
	bne .L_02008f7e
	b .L_02009294
.L_02008f7e:
	b .L_0200929c
.L_02008f80:
	movs r2, #150
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_02008fb2
	movs r5, #13
	movs r0, #15
	movs r1, #15
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005178
	movs r3, #63
	str r3, [sp, #0]
	movs r0, #65
	movs r1, #15
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005178
	bl Func_02000d64
	b .L_02009294
.L_02008fb2:
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #183
	mov r8, r2
	cmp r3, r8
	ble .L_02008fc0
	b .L_0200919c
.L_02008fc0:
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	ldr r3, [r7]
	cmp r0, #0
	beq .L_020090ae
	movs r2, #158
	lsls r2, r2, #1
	cmp r3, r2
	bgt .L_02009022
	ldr r3, [r6, #12]
	movs r2, #144
	lsls r2, r2, #11
	movs r5, #128
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r6, #12]
	lsls r1, r1, #10
	movs r0, #11
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #192
	lsls r1, r1, #9
	movs r0, #13
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #9
	movs r0, #14
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #15
	adds r2, r5, #0
	bl Func_02000cd8
	movs r0, #16
	b .L_020090dc
.L_02009022:
	movs r2, #150
	lsls r2, r2, #2
	cmp r3, r2
	bge .L_0200905a
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090ee
	ldr r3, [r7]
	movs r2, #200
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_020090ee
	movs r3, #13
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #22
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02005178
	ldr r0, .L_020092a8
	bl Task_RemoveFar
	b .L_020090ee
.L_0200905a:
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r3, r1
	ble .L_020090fc
	ldr r3, [r6, #12]
	ldr r2, .L_020092ac
	movs r5, #192
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #192
	str r3, [r6, #12]
	lsls r1, r1, #10
	movs r0, #11
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #144
	lsls r1, r1, #10
	movs r0, #13
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #9
	movs r0, #14
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #15
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #16
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #16
	b .L_0200915c
.L_020090ae:
	movs r2, #158
	lsls r2, r2, #1
	cmp r3, r2
	bgt .L_020090e6
	ldr r3, [r6, #12]
	ldr r2, .L_020092b0
	movs r5, #128
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r6, #12]
	lsls r1, r1, #9
	movs r0, #11
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #13
	adds r2, r5, #0
	bl Func_02000cd8
	movs r0, #14
.L_020090dc:
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cd8
	b .L_0200916a
.L_020090e6:
	movs r2, #150
	lsls r2, r2, #2
	cmp r3, r2
	bge .L_020090f2
.L_020090ee:
	ldr r5, .L_020092b4
	b .L_02009112
.L_020090f2:
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r3, r1
	bgt .L_0200912c
.L_020090fc:
	ldr r5, .L_020092b4
	ldrh r2, [r5]
	cmp r2, #0
	bne .L_02009112
	ldr r3, .L_020092b8
	movs r0, #131
	str r2, [r3]
	lsls r0, r0, #2
	str r1, [r7]
	bl GameFlag_ClearBit
.L_02009112:
	ldrh r0, [r5]
	bl Math_Sine
	ldr r3, [r6, #12]
	asrs r0, r0, #1
	adds r3, r3, r0
	str r3, [r6, #12]
	ldrh r3, [r5]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5]
	b .L_0200916a
.L_0200912c:
	ldr r3, [r6, #12]
	ldr r2, .L_020092bc
	movs r5, #192
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #192
	str r3, [r6, #12]
	lsls r1, r1, #9
	movs r0, #11
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #13
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #14
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #14
.L_0200915c:
	bl Object_GetById
	ldr r3, [r0, #28]
	cmp r3, r5
	bgt .L_0200916a
	mov r3, r8
	str r3, [r7]
.L_0200916a:
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009178
	b .L_02009294
.L_02009178:
	ldr r3, .L_020092c0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r0, #12]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #60]
	movs r3, #0
	str r3, [r0, #40]
	b .L_02009294
.L_0200919c:
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #184
	cmp r3, r2
	bne .L_02009262
	movs r5, #13
	movs r0, #8
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005178
	movs r3, #63
	str r3, [sp, #0]
	movs r0, #58
	movs r3, #1
	movs r1, #0
	movs r2, #1
	str r5, [sp, #4]
	bl Func_02005178
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl Func_02005230
	movs r1, #1
	movs r0, #9
	bl Object_SetModeById
	movs r0, #9
	bl Object_GetById
	movs r1, #216
	movs r3, #216
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009294
	ldr r5, .L_020092c0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r2, #0
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	bl Object_SetPositionAndResetMotion
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	b .L_02009294
.L_02009262:
	bl Func_02000d18
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200929c
	bl Random16Far
	ldr r3, [r7]
	lsls r0, r0, #7
	lsrs r0, r0, #16
	adds r3, r3, r0
	str r3, [r7]
	bl Random16Far
	movs r2, #156
	lsls r2, r2, #6
	adds r2, #16
	adds r3, r0, #0
	muls r3, r2
	lsrs r3, r3, #16
	adds r3, r3, r2
	ldr r2, [r7]
	cmp r2, r3
	bls .L_02009294
	str r5, [r7]
.L_02009294:
	ldr r2, .L_020092a4
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200929c:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020092a4:
	.4byte gData_020062a0
.L_020092a8:
	.4byte Func_02000f60
.L_020092ac:
	.4byte 0xfff94000
.L_020092b0:
	.4byte 0x00027999
.L_020092b4:
	.4byte gData_020062a8
.L_020092b8:
	.4byte gData_02006298
.L_020092bc:
	.4byte 0xfffc499a
.L_020092c0:
	.4byte gAdventure
	.section .text.x020092c4,"ax",%progbits
	.global Func_020012c4
	.thumb_func
Func_020012c4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r7, .L_02009614
	movs r2, #150
	ldr r3, [r7]
	lsls r2, r2, #1
	sub sp, #8
	cmp r3, r2
	bge .L_020092e4
	bl Func_02000d3c
	cmp r0, #0
	bne .L_020092e2
	b .L_02009604
.L_020092e2:
	b .L_0200960c
.L_020092e4:
	movs r2, #150
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_02009318
	movs r3, #23
	str r3, [sp, #0]
	movs r5, #15
	movs r0, #15
	movs r1, #15
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005178
	movs r3, #73
	str r3, [sp, #0]
	movs r0, #65
	movs r1, #15
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005178
	bl Func_02000e60
	b .L_02009604
.L_02009318:
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #183
	mov r8, r2
	cmp r3, r8
	ble .L_02009326
	b .L_02009508
.L_02009326:
	movs r0, #10
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	ldr r3, [r7]
	cmp r0, #0
	beq .L_02009416
	movs r2, #158
	lsls r2, r2, #1
	cmp r3, r2
	bgt .L_02009386
	ldr r3, [r6, #12]
	movs r2, #144
	lsls r2, r2, #11
	movs r5, #128
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r6, #12]
	lsls r1, r1, #10
	movs r0, #12
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #192
	lsls r1, r1, #9
	movs r0, #17
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #9
	movs r0, #18
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #19
	adds r2, r5, #0
	bl Func_02000cd8
	movs r0, #20
	b .L_02009444
.L_02009386:
	movs r2, #150
	lsls r2, r2, #2
	cmp r3, r2
	bge .L_020093c2
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009456
	ldr r3, [r7]
	movs r2, #200
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_02009456
	movs r3, #23
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #23
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02005178
	ldr r0, .L_02009618
	bl Task_RemoveFar
	b .L_02009456
.L_020093c2:
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r3, r1
	ble .L_02009464
	ldr r3, [r6, #12]
	ldr r2, .L_0200961c
	movs r5, #192
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #192
	str r3, [r6, #12]
	lsls r1, r1, #10
	movs r0, #12
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #144
	lsls r1, r1, #10
	movs r0, #17
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #9
	movs r0, #18
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #19
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #20
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #20
	b .L_020094c6
.L_02009416:
	movs r2, #158
	lsls r2, r2, #1
	cmp r3, r2
	bgt .L_0200944e
	ldr r3, [r6, #12]
	ldr r2, .L_02009620
	movs r5, #128
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r6, #12]
	lsls r1, r1, #9
	movs r0, #12
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #17
	adds r2, r5, #0
	bl Func_02000cd8
	movs r0, #18
.L_02009444:
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cd8
	b .L_020094d4
.L_0200944e:
	movs r2, #150
	lsls r2, r2, #2
	cmp r3, r2
	bge .L_0200945a
.L_02009456:
	ldr r5, .L_02009624
	b .L_0200947c
.L_0200945a:
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r3, r1
	bgt .L_02009496
.L_02009464:
	ldr r5, .L_02009624
	ldrh r2, [r5]
	cmp r2, #0
	bne .L_0200947c
	ldr r3, .L_02009628
	movs r0, #135
	lsls r0, r0, #1
	str r2, [r3]
	adds r0, #255
	str r1, [r7]
	bl GameFlag_ClearBit
.L_0200947c:
	ldrh r0, [r5]
	bl Math_Sine
	ldr r3, [r6, #12]
	asrs r0, r0, #1
	adds r3, r3, r0
	str r3, [r6, #12]
	ldrh r3, [r5]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5]
	b .L_020094d4
.L_02009496:
	ldr r3, [r6, #12]
	ldr r2, .L_0200962c
	movs r5, #192
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #192
	str r3, [r6, #12]
	lsls r1, r1, #9
	movs r0, #12
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #17
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #18
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #18
.L_020094c6:
	bl Object_GetById
	ldr r3, [r0, #28]
	cmp r3, r5
	bgt .L_020094d4
	mov r3, r8
	str r3, [r7]
.L_020094d4:
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020094e4
	b .L_02009604
.L_020094e4:
	ldr r3, .L_02009630
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r0, #12]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #60]
	movs r3, #0
	str r3, [r0, #40]
	b .L_02009604
.L_02009508:
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #184
	cmp r3, r2
	bne .L_020095d2
	movs r3, #23
	str r3, [sp, #0]
	movs r5, #15
	movs r0, #8
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005178
	movs r3, #73
	str r3, [sp, #0]
	movs r0, #58
	movs r3, #1
	movs r1, #0
	movs r2, #1
	str r5, [sp, #4]
	bl Func_02005178
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r2, #0
	movs r0, #20
	movs r1, #0
	bl Func_02005230
	movs r1, #1
	movs r0, #10
	bl Object_SetModeById
	movs r0, #10
	bl Object_GetById
	movs r1, #188
	movs r3, #248
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009604
	ldr r5, .L_02009630
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r2, #0
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	bl Object_SetPositionAndResetMotion
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	b .L_02009604
.L_020095d2:
	bl Func_02000d3c
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200960c
	bl Random16Far
	ldr r3, [r7]
	lsls r0, r0, #7
	lsrs r0, r0, #16
	adds r3, r3, r0
	str r3, [r7]
	bl Random16Far
	movs r2, #156
	lsls r2, r2, #6
	adds r2, #16
	adds r3, r0, #0
	muls r3, r2
	lsrs r3, r3, #16
	adds r3, r3, r2
	ldr r2, [r7]
	cmp r2, r3
	bls .L_02009604
	str r5, [r7]
.L_02009604:
	ldr r2, .L_02009614
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200960c:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009614:
	.4byte gData_020062a4
.L_02009618:
	.4byte Func_020012c4
.L_0200961c:
	.4byte 0xfff94000
.L_02009620:
	.4byte 0x00027999
.L_02009624:
	.4byte gData_020062aa
.L_02009628:
	.4byte gData_0200629c
.L_0200962c:
	.4byte 0xfffc499a
.L_02009630:
	.4byte gAdventure
	.section .text.x020099d4,"ax",%progbits
	.global Func_020019d4
	.thumb_func
Func_020019d4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009b38
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	movs r6, #0
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r5, #16]
	asrs r7, r3, #20
	mov r3, r8
	cmp r3, #13
	bne .L_02009a0c
	cmp r7, #13
	bne .L_02009a0c
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r6, #1
	b .L_02009a14
.L_02009a0c:
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_02009a14:
	mov r2, r8
	cmp r2, #23
	bne .L_02009a2c
	cmp r7, #15
	bne .L_02009a2c
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r6, #1
	b .L_02009a36
.L_02009a2c:
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_02009a36:
	cmp r6, #0
	beq .L_02009a44
	movs r0, #126
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02009a4c
.L_02009a44:
	movs r0, #126
	adds r0, #255
	bl GameFlag_ClearBit
.L_02009a4c:
	ldr r3, .L_02009b3c
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02009a8c
	ldr r3, [r5, #12]
	cmp r3, #0
	bne .L_02009a8c
	ldr r3, [r5, #8]
	movs r2, #142
	lsls r2, r2, #17
	cmp r3, r2
	ble .L_02009a8c
	movs r2, #154
	lsls r2, r2, #17
	cmp r3, r2
	bge .L_02009a8c
	ldr r3, [r5, #16]
	movs r2, #200
	lsls r2, r2, #16
	cmp r3, r2
	bge .L_02009a8c
	movs r0, #134
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009a8c
	ldr r1, .L_02009b40
	adds r0, r5, #0
	bl Func_02000778
.L_02009a8c:
	ldr r3, .L_02009b44
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02009aca
	ldr r3, [r5, #12]
	cmp r3, #0
	bne .L_02009aca
	ldr r3, [r5, #8]
	movs r2, #165
	lsls r2, r2, #17
	cmp r3, r2
	ble .L_02009aca
	ldr r2, .L_02009b48
	cmp r3, r2
	bgt .L_02009aca
	ldr r3, [r5, #16]
	movs r2, #216
	lsls r2, r2, #16
	cmp r3, r2
	bge .L_02009aca
	movs r0, #134
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009aca
	ldr r1, .L_02009b4c
	adds r0, r5, #0
	bl Func_02000778
.L_02009aca:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #14
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009b06
	movs r0, #131
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009b30
	movs r0, #135
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009b30
	movs r0, #1
	negs r0, r0
	bl Func_02005388
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #14
	bl GameFlag_ClearBit
	b .L_02009b30
.L_02009b06:
	movs r0, #131
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009b20
	movs r0, #135
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009b30
.L_02009b20:
	movs r0, #162
	bl Func_02005388
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #14
	bl GameFlag_SetBit
.L_02009b30:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009b38:
	.4byte gAdventure
.L_02009b3c:
	.4byte gData_02006298
.L_02009b40:
	.4byte 0x00026666
.L_02009b44:
	.4byte gData_0200629c
.L_02009b48:
	.4byte 0x0165ffff
.L_02009b4c:
	.4byte 0x00039999
	.section .text.x02009b50,"ax",%progbits
	.global Func_02001b50
	.thumb_func
Func_02001b50:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009c80
	movs r0, #133
	lsls r0, r0, #2
	adds r0, r0, r3
	mov r9, r0
	ldr r0, [r0]
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r6, #8]
	ldr r1, [r6, #16]
	asrs r3, r2, #20
	asrs r0, r1, #20
	mov r10, r3
	ldr r3, [r6, #12]
	mov r8, r0
	movs r0, #128
	lsls r0, r0, #13
	movs r7, #0
	cmp r3, r0
	bgt .L_02009c76
	ldr r0, .L_02009c84
	adds r3, r2, r0
	movs r2, #240
	lsls r2, r2, #16
	cmp r3, r2
	bhi .L_02009c76
	ldr r0, .L_02009c88
	movs r2, #200
	adds r3, r1, r0
	lsls r2, r2, #15
	cmp r3, r2
	bhi .L_02009c76
	mov r3, r10
	cmp r3, #27
	bne .L_02009bda
	mov r0, r8
	cmp r0, #12
	bne .L_02009bda
	movs r0, #13
	bl Object_GetById
	mov r2, r9
	adds r5, r0, #0
	ldr r0, [r2]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #13
	movs r1, #4
	bl Object_SetModeById
	adds r3, r6, #0
	adds r3, #85
	strb r7, [r3]
	movs r0, #130
	ldr r3, [r5, #12]
	lsls r0, r0, #2
	str r3, [r6, #12]
	bl GameFlag_SetBit
	movs r7, #1
.L_02009bda:
	mov r3, r10
	cmp r3, #37
	bne .L_02009c22
	mov r0, r8
	cmp r0, #14
	bne .L_02009c22
	movs r0, #14
	bl Object_GetById
	ldr r3, .L_02009c80
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #14
	movs r1, #4
	bl Object_SetModeById
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #133
	ldr r3, [r5, #12]
	lsls r0, r0, #1
	str r3, [r6, #12]
	adds r0, #255
	bl GameFlag_SetBit
	movs r7, #1
.L_02009c22:
	cmp r7, #0
	beq .L_02009c30
	movs r0, #126
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02009c76
.L_02009c30:
	movs r0, #13
	movs r1, #0
	bl Object_SetModeById
	movs r1, #0
	movs r0, #14
	bl Object_SetModeById
	ldr r3, .L_02009c80
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	adds r2, r6, #0
	adds r2, #85
	movs r3, #3
	movs r0, #130
	strb r3, [r2]
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #126
	adds r0, #255
	bl GameFlag_ClearBit
.L_02009c76:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009c80:
	.4byte gAdventure
.L_02009c84:
	.4byte 0xfe700000
.L_02009c88:
	.4byte 0xff4c0000
	.section .text.x0200a3e8,"ax",%progbits
	.global Func_020023e8
	.thumb_func
Func_020023e8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a5a4
	movs r2, #133
	lsls r2, r2, #2
	adds r2, r2, r3
	ldr r0, [r2]
	mov r8, r2
	bl Object_GetById
	movs r3, #6
	ldrsh r5, [r0, r3]
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	bl Func_020052f0
	movs r2, #0
	mov r10, r2
	mov r3, r10
	adds r0, #85
	strb r3, [r0]
	movs r1, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	ldr r0, .L_0200a5a8
	bl Func_02005280
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	mov r2, r8
	ldr r0, [r2]
	movs r1, #12
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	mov r3, r8
	ldr r0, [r3]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #12
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	lsls r5, r5, #16
	bl Func_02005298
	lsrs r5, r5, #16
	mov r2, r8
	ldr r0, [r2]
	adds r1, r5, #0
	movs r2, #10
	bl ObjectMotion_ArmCallback
	ldr r5, .L_0200a5ac
	ldr r0, [r5]
	cmp r0, #0
	beq .L_0200a4ba
	movs r1, #3
	bl Func_02005330
.L_0200a4ba:
	movs r0, #198
	movs r1, #0
	bl PartyInventory_GiveItem
	ldr r0, [r5]
	cmp r0, #0
	beq .L_0200a4cc
	bl Func_02005148
.L_0200a4cc:
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_020052d8
	movs r0, #212
	movs r2, #200
	movs r3, #1
	movs r1, #0
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	movs r0, #12
	bl Object_GetById
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r5, r0, #0
	adds r1, #204
	movs r0, #12
	adds r2, #102
	ldr r6, [r5, #80]
	bl ObjectMotion_SetSpeedParameters
	movs r1, #212
	movs r2, #188
	lsls r2, r2, #1
	movs r0, #12
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #12
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200a5b0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #222
	movs r2, #188
	lsls r2, r2, #1
	movs r0, #12
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #7
	movs r0, #12
	bl Object_SetModeById
	ldr r3, .L_0200a5b4
	ldr r2, .L_0200a5a0
	str r3, [r5, #24]
	adds r7, r5, #0
	movs r3, #224
	adds r7, #85
	lsls r3, r3, #8
	strb r2, [r7]
	strh r3, [r6, #18]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #12
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #246
	movs r2, #128
	b .L_0200a5b8
.L_0200a5a0:
	.4byte 0x00000000
.L_0200a5a4:
	.4byte gAdventure
.L_0200a5a8:
	.4byte MsgKandoreanTemple08
.L_0200a5ac:
	.4byte gData_020062ac
.L_0200a5b0:
	.4byte 0x00019999
.L_0200a5b4:
	.4byte 0xffff0000
.L_0200a5b8:
	movs r3, #188
	adds r0, r5, #0
	lsls r1, r1, #17
	lsls r2, r2, #12
	lsls r3, r3, #17
	bl Func_02005160
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #3
	strb r3, [r7]
	mov r3, r10
	str r3, [r5, #20]
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r10
	str r3, [r5, #24]
	movs r1, #1
	strh r2, [r6, #18]
	movs r0, #12
	bl Object_SetModeById
	movs r0, #12
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200a670
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #252
	movs r2, #188
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #12
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #248
	movs r2, #197
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #216
	movs r2, #197
	lsls r2, r2, #1
	movs r0, #12
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	mov r3, r8
	ldr r1, [r3]
	movs r0, #12
	bl Object_LinkObjectAndSetCallback
	movs r1, #2
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #74
	bl GameFlag_SetBit
	bl Func_020051c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a670:
	.4byte 0x00019999
	.section .text.x0200aa84,"ax",%progbits
	.balign 4
	.ifdef TLA_EDITION_JA
	.global Func_02002a84
	.thumb_func
Func_02002a84:
	push {r5, r6, lr}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Func_020052c0
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	movs r0, #192
	movs r5, #0
	adds r3, #85
	lsls r0, r0, #2
	strb r5, [r3]
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200aad8
	movs r0, #8
	bl Object_GetById
	movs r1, #156
	movs r2, #128
	movs r3, #236
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r3, #19
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #3
	bl Func_02005178
	b .L_ja_0200ab10
.L_ja_0200aad8:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200ab10
	movs r0, #8
	bl Object_GetById
	movs r1, #172
	movs r2, #128
	movs r3, #236
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r3, #19
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #3
	movs r3, #3
	bl Func_02005178
.L_ja_0200ab10:
	ldr r0, .L_ja_0200abe0
	bl Func_020053b8
	movs r0, #10
	bl Object_GetById
	movs r3, #179
	adds r6, r0, #0
	lsls r3, r3, #8
	adds r3, #51
	adds r2, r6, #0
	str r3, [r6, #24]
	str r3, [r6, #28]
	adds r2, #89
	movs r3, #0
	movs r0, #0
	strb r3, [r2]
	bl Func_02005338
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #45
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200ab76
	movs r3, #1
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #0
	movs r0, #41
	movs r1, #0
	movs r2, #20
	bl Func_02005170
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
.L_ja_0200ab76:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200abda
	ldr r2, .L_ja_0200abe4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_ja_0200aba6
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r0, #12]
	b .L_ja_0200abda
.L_ja_0200aba6:
	cmp r3, #3
	bne .L_ja_0200abda
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #45
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200abda
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	bl Func_02005028
.L_ja_0200abda:
	sub sp, #-8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_ja_0200abe0:
	.4byte gData_020054e0
.L_ja_0200abe4:
	.4byte gAdventure
	.else
	.global Func_02002a84
	.thumb_func
Func_02002a84:
	push {r5, r6, lr}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Func_020052c0
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	movs r0, #192
	movs r5, #0
	adds r3, #85
	lsls r0, r0, #2
	strb r5, [r3]
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aaf4
	movs r0, #8
	bl Object_GetById
	movs r1, #156
	movs r2, #128
	movs r3, #236
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r3, #19
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #3
	bl Func_02005178
	b .L_0200ab2c
.L_0200aaf4:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab2c
	movs r0, #8
	bl Object_GetById
	movs r1, #172
	movs r2, #128
	movs r3, #236
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r3, #19
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #3
	movs r3, #3
	bl Func_02005178
.L_0200ab2c:
	ldr r0, .L_0200ac14
	bl Func_020053b8
	movs r0, #10
	bl Object_GetById
	movs r3, #179
	adds r6, r0, #0
	lsls r3, r3, #8
	adds r3, #51
	adds r2, r6, #0
	str r3, [r6, #24]
	str r3, [r6, #28]
	adds r2, #89
	movs r3, #0
	movs r0, #0
	strb r3, [r2]
	bl Func_02005338
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #45
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab92
	movs r3, #1
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #0
	movs r0, #41
	movs r1, #0
	movs r2, #20
	bl Func_02005170
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
.L_0200ab92:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200abf6
	ldr r2, .L_0200ac18
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_0200abc2
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r0, #12]
	b .L_0200abf6
.L_0200abc2:
	cmp r3, #3
	bne .L_0200abf6
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #45
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200abf6
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	bl Func_02005028
.L_0200abf6:
	ldr r3, .L_0200ac18
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_0200ac0e
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200ac0e:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ac14:
	.4byte gData_020054e0
.L_0200ac18:
	.4byte gAdventure
	.endif
	.section .text.x0200aec2,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200aec4,"ax",%progbits
	.global Func_02002ec4
	.thumb_func
Func_02002ec4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200b058
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_02005158
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
	bl Func_020053d8
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl FieldEvent_WaitFrames
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200b05c
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200af5e:
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
	ldr r3, .L_0200b060
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200b064
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
	ldr r4, .L_0200b068
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_020000b8
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200af5e
	movs r0, #188
	bl Func_020053d8
	ldr r5, .L_0200b058
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_020052d0
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005198
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02005198
	bl Func_020051a0
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_020052d0
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
	bl Object_SetModeById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_020051c8
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b058:
	.4byte gAdventure
.L_0200b05c:
	.4byte Effect_AdvanceMotion
.L_0200b060:
	.4byte 0xffffa000
.L_0200b064:
	.4byte 0xffffd000
.L_0200b068:
	.4byte 0x01090001
	.section .text.x0200b06c,"ax",%progbits
	.global Func_0200306c
	.thumb_func
Func_0200306c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200b2b8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Object_GetById
	mov r8, r0
	bl Func_020052f0
	adds r7, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_020052f0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r0, #85
	strb r3, [r0]
	strb r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #162
	bl Func_020053d8
	bl Func_02000d64
	movs r2, #0
	mov r10, r2
.L_0200b0f6:
	ldr r3, [r6, #12]
	movs r2, #144
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r6, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	movs r5, #128
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	movs r2, #230
	lsls r2, r2, #9
	adds r2, #204
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r7, #12]
	movs r0, #11
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #16
	bl Func_02000cd8
	mov r1, r10
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	movs r2, #240
	asrs r1, r3, #16
	lsls r2, r2, #12
	mov r10, r1
	cmp r3, r2
	bls .L_0200b0f6
	bl Func_02005158
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
.L_0200b194:
	ldr r3, [r6, #12]
	ldr r2, .L_0200b2bc
	mov r1, r8
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r5, #192
	ldr r3, [r1, #12]
	lsls r5, r5, #5
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	ldr r2, .L_0200b2c0
	movs r1, #192
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #11
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #144
	movs r0, #13
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #16
	bl Func_02000cf8
	movs r0, #1
	bl WaitFrames
	movs r0, #16
	bl Object_GetById
	ldr r3, [r0, #28]
	cmp r3, r5
	bgt .L_0200b194
	ldr r2, .L_0200b2c4
	movs r3, #0
	movs r0, #131
	str r3, [r2]
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020053d8
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl Func_02005230
	movs r1, #1
	movs r0, #9
	bl Object_SetModeById
	movs r0, #9
	bl Object_GetById
	movs r1, #216
	movs r3, #216
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	mov r1, r8
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r2, #0
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200b2b8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_020051c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b2b8:
	.4byte gAdventure
.L_0200b2bc:
	.4byte 0xfff94000
.L_0200b2c0:
	.4byte 0xfffd4ccd
.L_0200b2c4:
	.4byte gData_02006298
	.section .text.x0200b2c8,"ax",%progbits
	.global Func_020032c8
	.thumb_func
Func_020032c8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200b514
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	mov r8, r0
	bl Func_020052f0
	adds r7, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_020052f0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r0, #85
	strb r3, [r0]
	strb r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_ClearBit
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #162
	bl Func_020053d8
	bl Func_02000e60
	movs r2, #0
	mov r10, r2
.L_0200b352:
	ldr r3, [r6, #12]
	movs r2, #144
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r6, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	movs r5, #128
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	movs r2, #230
	lsls r2, r2, #9
	adds r2, #204
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r7, #12]
	movs r0, #12
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #20
	bl Func_02000cd8
	mov r1, r10
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	movs r2, #240
	asrs r1, r3, #16
	lsls r2, r2, #12
	mov r10, r1
	cmp r3, r2
	bls .L_0200b352
	bl Func_02005158
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #10
	bl FieldEvent_WaitFrames
.L_0200b3f0:
	ldr r3, [r6, #12]
	ldr r2, .L_0200b518
	mov r1, r8
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r5, #192
	ldr r3, [r1, #12]
	lsls r5, r5, #5
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	ldr r2, .L_0200b51c
	movs r1, #192
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #12
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #144
	movs r0, #17
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #18
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #20
	bl Func_02000cf8
	movs r0, #1
	bl WaitFrames
	movs r0, #20
	bl Object_GetById
	ldr r3, [r0, #28]
	cmp r3, r5
	bgt .L_0200b3f0
	ldr r2, .L_0200b520
	movs r0, #135
	movs r3, #0
	lsls r0, r0, #1
	str r3, [r2]
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020053d8
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r2, #0
	movs r0, #20
	movs r1, #0
	bl Func_02005230
	movs r1, #1
	movs r0, #10
	bl Object_SetModeById
	movs r0, #10
	bl Object_GetById
	movs r1, #188
	movs r3, #248
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	mov r1, r8
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r2, #0
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200b514
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_020051c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b514:
	.4byte gAdventure
.L_0200b518:
	.4byte 0xfff94000
.L_0200b51c:
	.4byte 0xfffd4ccd
.L_0200b520:
	.4byte gData_0200629c
	.section .text.x0200ba14,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02003a14
	.thumb_func
Func_02003a14:
	push {lr}
	ldr r2, .L_ja_0200ba04
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	movs r3, #0
	str r3, [r2]
	subs r3, #13
	ldrb r2, [r1, #23]
	movs r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	bl Func_02005338
	pop {pc}
	.2byte 0x0000
.L_ja_0200ba04:
	.4byte gData_02006294
	.else
	.global Func_02003a14
	.thumb_func
Func_02003a14:
	push {lr}
	ldr r2, .L_0200ba54
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	movs r3, #0
	str r3, [r2]
	subs r3, #13
	ldrb r2, [r1, #23]
	movs r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	bl Func_02005338
	ldr r3, .L_0200ba58
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #1
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200ba52
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200ba52:
	pop {pc}
.L_0200ba54:
	.4byte gData_02006294
.L_0200ba58:
	.4byte gAdventure
	.endif
	.section .text.x0200be8c,"ax",%progbits
	.balign 4
	.global Func_02003e8c
	.thumb_func
Func_02003e8c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_020052f0
	movs r2, #0
	mov r10, r2
	mov r3, r10
	adds r0, #85
	strb r3, [r0]
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_0200c268
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #200
	movs r2, #199
	lsls r1, r1, #17
	lsls r2, r2, #18
	ldr r0, [r5]
	bl Func_02005230
	movs r0, #1
	bl WaitFrames
	bl Event_StartScreenTransition
	movs r1, #200
	movs r2, #188
	ldr r0, [r5]
	lsls r2, r2, #2
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200c26c
	adds r1, #153
	bl Func_020052d8
	movs r0, #218
	movs r1, #144
	movs r2, #166
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Camera_MoveWithinMapBounds
	bl Func_020052e8
	movs r2, #0
	movs r1, #4
	movs r0, #12
	bl ObjectMotion_Launch
	ldr r0, .L_0200c270
	bl Func_02005280
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #228
	movs r2, #172
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #17
	movs r1, #0
	bl Camera_MoveWithinMapBounds
	bl Func_020052e8
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #131
	bl Func_020053d8
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #13
	bl Func_02005270
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #13
	bl Func_02005270
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #13
	bl Object_GetById
	movs r3, #85
	adds r7, r0, #0
	adds r3, r3, r7
	mov r2, r10
	ldr r6, [r7, #80]
	strb r2, [r3]
	mov r8, r3
	movs r5, #0
.L_0200bf82:
	ldr r3, [r7, #12]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #204
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	adds r5, #1
	bl FieldEvent_WaitFrames
	cmp r5, #63
	bls .L_0200bf82
	movs r2, #32
	movs r1, #32
	adds r0, r6, #0
	bl Func_02003e34
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #129
	movs r0, #13
	lsls r1, r1, #1
	bl Func_020052d0
	movs r2, #64
	negs r2, r2
	adds r0, r6, #0
	movs r1, #32
	bl Func_02003e34
	adds r0, r6, #0
	movs r1, #32
	movs r2, #64
	bl Func_02003e34
	movs r2, #32
	adds r0, r6, #0
	movs r1, #64
	bl Func_02003e34
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #13
	bl Func_020052d0
	movs r0, #60
	bl FieldEvent_WaitFrames
	mov r2, r8
	movs r3, #3
	strb r3, [r2]
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #159
	bl Func_020053d8
	movs r3, #192
	lsls r3, r3, #10
	movs r5, #0
	str r3, [r7, #40]
	movs r0, #60
	strh r5, [r6, #18]
	mov r8, r3
	bl FieldEvent_WaitFrames
	movs r0, #13
	movs r1, #0
	bl Func_02005298
	movs r0, #218
	movs r1, #144
	movs r2, #166
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Camera_MoveWithinMapBounds
	bl Func_020052e8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #12
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #238
	movs r2, #166
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #237
	movs r2, #166
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #236
	movs r2, #162
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_02005230
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_0200c274
	adds r1, #102
	bl Func_020052d8
	movs r0, #182
	movs r1, #144
	movs r2, #166
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Camera_MoveWithinMapBounds
	bl Func_020052e8
	movs r1, #164
	movs r2, #162
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #12
	bl Func_02005230
	movs r0, #1
	bl WaitFrames
	movs r1, #163
	movs r2, #166
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #166
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200c26c
	adds r1, #153
	bl Func_020052d8
	movs r0, #171
	movs r2, #172
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #17
	movs r1, #0
	bl Camera_MoveWithinMapBounds
	bl Func_020052e8
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #131
	bl Func_020053d8
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #14
	bl Func_02005270
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #14
	bl Func_02005270
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r0, #14
	adds r1, #255
	bl Func_020052d0
	movs r0, #14
	bl Object_GetById
	movs r2, #85
	adds r7, r0, #0
	adds r2, r2, r7
	mov r3, r8
	strb r3, [r2]
	mov r10, r2
.L_0200c15e:
	ldr r3, [r7, #12]
	movs r2, #152
	lsls r2, r2, #6
	adds r2, #102
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	adds r5, #1
	bl FieldEvent_WaitFrames
	cmp r5, #63
	bls .L_0200c15e
	movs r3, #128
	lsls r3, r3, #5
	ldr r5, .L_0200c278
	mov r8, r3
	adds r0, r7, #0
	movs r1, #64
	mov r2, r8
	bl Func_02003e60
	adds r0, r7, #0
	adds r2, r5, #0
	movs r1, #128
	bl Func_02003e60
	adds r0, r7, #0
	movs r1, #128
	mov r2, r8
	bl Func_02003e60
	adds r2, r5, #0
	adds r0, r7, #0
	movs r1, #64
	bl Func_02003e60
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #14
	bl Func_020052d0
	mov r2, r10
	movs r3, #3
	strb r3, [r2]
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #159
	bl Func_020053d8
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r7, #40]
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #14
	movs r1, #0
	bl Func_02005298
	movs r0, #182
	movs r1, #144
	movs r2, #166
	movs r3, #1
	lsls r2, r2, #18
	lsls r0, r0, #17
	lsls r1, r1, #15
	bl Camera_MoveWithinMapBounds
	bl Func_020052e8
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r2, #10
	movs r0, #12
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #200
	movs r2, #172
	movs r3, #1
	lsls r2, r2, #18
	lsls r0, r0, #17
	movs r1, #0
	bl Camera_MoveWithinMapBounds
	bl Func_020052e8
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #131
	bl Func_020053d8
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #15
	bl Func_02005270
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #15
	bl Func_02005270
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #15
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	movs r5, #0
	adds r3, #85
	ldr r6, [r7, #80]
	strb r5, [r3]
	b .L_0200c27c
	.2byte 0x0000
.L_0200c268:
	.4byte gAdventure
.L_0200c26c:
	.4byte 0x0004cccc
.L_0200c270:
	.4byte MsgKandoreanTemple04
.L_0200c274:
	.4byte 0x00013333
.L_0200c278:
	.4byte 0xfffff000
.L_0200c27c:
	ldr r3, [r7, #12]
	movs r2, #204
	lsls r2, r2, #6
	adds r2, #51
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	adds r5, #1
	bl FieldEvent_WaitFrames
	cmp r5, #63
	bls .L_0200c27c
	movs r2, #64
	adds r0, r6, #0
	movs r1, #32
	movs r5, #0
	bl Func_02003e34
	movs r1, #1
	movs r0, #15
	strh r5, [r6, #18]
	bl Motion_SetVarCbAndRefresh
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r2, #64
	negs r2, r2
	adds r0, r6, #0
	movs r1, #32
	bl Func_02003e34
	movs r0, #15
	strh r5, [r6, #18]
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #60
	bl FieldEvent_WaitFrames
.L_0200c2cc:
	ldr r3, [r7, #12]
	ldr r2, .L_0200c4cc
	movs r0, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	adds r5, #1
	bl FieldEvent_WaitFrames
	cmp r5, #63
	bls .L_0200c2cc
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #15
	bl Func_020052c8
	movs r2, #20
	movs r0, #15
	movs r1, #0
	bl Func_02005290
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #4
	movs r2, #20
	adds r1, #255
	movs r0, #15
	bl Func_020052c8
	movs r0, #15
	movs r1, #1
	bl Object_SetModeById
	movs r0, #15
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #176
	movs r2, #20
	movs r0, #15
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #15
	movs r1, #0
	bl Func_02005298
	movs r0, #182
	movs r1, #144
	movs r2, #166
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Camera_MoveWithinMapBounds
	bl Func_020052e8
	movs r1, #129
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #12
	bl Func_020052c8
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #15
	movs r1, #0
	bl Func_02005298
	movs r1, #176
	movs r2, #40
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #15
	movs r1, #0
	bl Func_02005298
	movs r1, #6
	movs r2, #80
	adds r1, #255
	movs r0, #12
	bl Func_020052c8
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #163
	movs r2, #166
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #164
	movs r2, #162
	lsls r2, r2, #2
	lsls r1, r1, #1
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #12
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #220
	movs r2, #150
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005230
	movs r0, #200
	movs r2, #172
	movs r3, #1
	lsls r0, r0, #17
	movs r1, #0
	lsls r2, r2, #18
	bl Camera_MoveWithinMapBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #15
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #200
	movs r2, #161
	lsls r2, r2, #2
	lsls r1, r1, #1
	movs r0, #15
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #188
	bl Func_020053d8
	bl Func_02001f3c
	movs r1, #0
	movs r0, #12
	bl Func_02005298
	ldr r5, .L_0200c4d0
	adds r0, r5, #0
	bl Task_RemoveFar
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #200
	movs r2, #160
	movs r0, #15
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #200
	movs r2, #188
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #17
	movs r1, #0
	bl Camera_MoveWithinMapBounds
	bl Func_020052e8
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Task_Add
	bl Func_020051c8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #72
	bl GameFlag_SetBit
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c4cc:
	.4byte 0xffffcccd
.L_0200c4d0:
	.4byte Func_02001c8c
	.section .text.x0200c82c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_0200482c
	.thumb_func
Func_0200482c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_020052f0
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #1
	bl WaitFrames
	movs r1, #188
	movs r2, #196
	movs r0, #16
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02005238
	ldr r6, .L_ja_0200cabc
	movs r1, #133
	lsls r1, r1, #2
	adds r6, r6, r1
	movs r2, #204
	movs r1, #188
	ldr r0, [r6]
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02005238
	movs r7, #192
	movs r3, #176
	movs r1, #236
	movs r2, #227
	movs r0, #12
	lsls r1, r1, #17
	lsls r3, r3, #8
	lsls r2, r2, #17
	lsls r7, r7, #18
	bl Func_02005238
	ldr r2, [r7, #108]
	movs r3, #128
	movs r5, #214
	lsls r3, r3, #2
	adds r3, #2
	lsls r5, r5, #1
	str r3, [r2, r5]
	bl Event_StartScreenTransition
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_ja_0200cac0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #232
	movs r0, #12
	lsls r1, r1, #1
	adds r2, r5, #0
	bl ObjectMotion_SetPositionAndReset
	movs r1, #216
	movs r2, #204
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl Func_020052d0
	ldr r0, .L_ja_0200cac4
	bl Func_02005280
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #12
	bl Func_020052c8
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r2, #40
	movs r0, #12
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #128
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #10
	bl ObjectMotion_ArmCallback
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #16
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_ja_0200c906
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_ja_0200c924
.L_ja_0200c906:
	ldr r2, [r7, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #12
	adds r3, #1
	movs r1, #3
	strh r3, [r2]
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
.L_ja_0200c924:
	ldr r5, .L_ja_0200cabc
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #16
	bl Func_020052c8
	movs r0, #16
	movs r1, #0
	bl Func_02005298
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	bl Func_020052d0
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #0
	movs r2, #40
	bl Func_02005290
	movs r1, #160
	movs r2, #40
	movs r0, #12
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #16
	bl Func_020052c8
	movs r0, #16
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_020052d8
	movs r0, #212
	movs r1, #128
	movs r2, #192
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #16
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #12
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	ldr r1, .L_ja_0200cac8
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_ja_0200cacc
	movs r0, #16
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_ja_0200cad0
	movs r0, #12
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #40
	bl FieldEvent_WaitFrames
	bl Func_02000bcc
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r0, #160
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #16
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_ja_0200cad4
	bl Func_020047f0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_ja_0200caea
	.2byte 0x0000
.L_ja_0200cabc:
	.4byte gAdventure
.L_ja_0200cac0:
	.4byte 0x00013333
.L_ja_0200cac4:
	.4byte 0x00001892
.L_ja_0200cac8:
	.4byte gData_02005564
.L_ja_0200cacc:
	.4byte gData_020055a0
.L_ja_0200cad0:
	.4byte gData_020055e4
.L_ja_0200cad4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	bl Func_020047f0
.L_ja_0200caea:
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_ja_0200cbdc
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r2, #0
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #16
	bl Func_020052c8
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #16
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #160
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #16
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_ja_0200cbe0
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #16
	bl Func_020052c8
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #16
	movs r1, #0
	bl Func_02005298
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_ja_0200cc28
.L_ja_0200cbdc:
	.4byte gAdventure
.L_ja_0200cbe0:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #12
	adds r3, #2
	strh r3, [r2]
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #8
	adds r1, #255
	movs r0, #16
	movs r2, #20
	bl Func_020052c8
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #16
	movs r1, #0
	bl Func_02005298
.L_ja_0200cc28:
	ldr r6, .L_ja_0200cdc4
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #212
	movs r2, #200
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #17
	movs r1, #0
	bl Camera_MoveWithinMapBounds
	movs r0, #153
	movs r1, #152
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #153
	adds r1, #51
	bl Func_020052d8
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #204
	movs r0, #12
	adds r1, #153
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r6]
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r0, #16
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r1, #244
	movs r2, #208
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #236
	movs r2, #216
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #16
	movs r1, #0
	movs r2, #80
	bl Func_02005290
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #12
	bl Func_020052c8
	movs r1, #176
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #212
	movs r1, #128
	movs r2, #192
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_ja_0200cdc8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #244
	movs r2, #208
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #252
	movs r2, #188
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #80]
	movs r0, #12
	movs r1, #1
	mov r10, r3
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_ja_0200cdcc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #246
	movs r2, #188
	lsls r2, r2, #1
	movs r0, #12
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #12
	movs r1, #7
	bl Object_SetModeById
	ldr r3, .L_ja_0200cdd0
	ldr r2, .L_ja_0200cdc0
	str r3, [r5, #24]
	movs r1, #0
	movs r3, #224
	adds r7, r5, #0
	mov r8, r1
	lsls r3, r3, #8
	mov r1, r10
	adds r7, #85
	strh r3, [r1, #18]
	strb r2, [r7]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #12
	adds r1, #102
	adds r2, #51
	b .L_ja_0200cdd4
	.2byte 0x0000
.L_ja_0200cdc0:
	.4byte 0x00000000
.L_ja_0200cdc4:
	.4byte gAdventure
.L_ja_0200cdc8:
	.4byte 0x00013333
.L_ja_0200cdcc:
	.4byte 0x00019999
.L_ja_0200cdd0:
	.4byte 0xffff0000
.L_ja_0200cdd4:
	bl ObjectMotion_SetSpeedParameters
	movs r1, #222
	movs r2, #160
	movs r3, #188
	lsls r2, r2, #14
	lsls r1, r1, #17
	lsls r3, r3, #17
	adds r0, r5, #0
	bl Func_02005160
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #3
	strb r3, [r7]
	movs r0, #12
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	mov r3, r8
	mov r1, r10
	strh r3, [r1, #18]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #20]
	movs r0, #12
	movs r1, #1
	bl Object_SetModeById
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_ja_0200cf08
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #212
	movs r2, #188
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #204
	movs r1, #128
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #12
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	movs r2, #180
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #16
	bl ObjectMotion_EnableActionAndResetMotion
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #10
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	ldr r0, [r6]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r6]
	bl Object_GetById
	cmp r0, #0
	beq .L_ja_0200cee0
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #16
	bl ObjectMotion_ResetAndSetPosition
.L_ja_0200cee0:
	movs r0, #16
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl Func_02005230
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl GameFlag_SetBit
	bl Func_020051c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200cf08:
	.4byte 0x00019999
	.else
	.global Func_0200482c
	.thumb_func
Func_0200482c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_020052f0
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #1
	bl WaitFrames
	movs r1, #188
	movs r2, #196
	movs r0, #16
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02005238
	ldr r6, .L_0200cb50
	movs r1, #133
	lsls r1, r1, #2
	adds r6, r6, r1
	movs r2, #204
	movs r1, #188
	ldr r0, [r6]
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02005238
	movs r7, #192
	movs r3, #176
	movs r1, #236
	movs r2, #227
	movs r0, #12
	lsls r1, r1, #17
	lsls r3, r3, #8
	lsls r2, r2, #17
	lsls r7, r7, #18
	bl Func_02005238
	ldr r2, [r7, #108]
	movs r3, #128
	movs r5, #214
	lsls r3, r3, #2
	adds r3, #2
	lsls r5, r5, #1
	str r3, [r2, r5]
	bl Event_StartScreenTransition
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200cb54
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #232
	movs r0, #12
	lsls r1, r1, #1
	adds r2, r5, #0
	bl ObjectMotion_SetPositionAndReset
	movs r1, #216
	movs r2, #204
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl Func_020052d0
	ldr r0, .L_0200cb58
	bl Func_02005280
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #12
	bl Func_020052c8
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r2, #40
	movs r0, #12
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #128
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #10
	bl ObjectMotion_ArmCallback
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #16
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200c99a
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200c9b8
.L_0200c99a:
	ldr r2, [r7, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #12
	adds r3, #1
	movs r1, #3
	strh r3, [r2]
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
.L_0200c9b8:
	ldr r5, .L_0200cb50
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #16
	bl Func_020052c8
	movs r0, #16
	movs r1, #0
	bl Func_02005298
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	bl Func_020052d0
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #0
	movs r2, #40
	bl Func_02005290
	movs r1, #160
	movs r2, #40
	movs r0, #12
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #16
	bl Func_020052c8
	movs r0, #16
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_020052d8
	movs r0, #212
	movs r1, #128
	movs r2, #192
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #16
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #12
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	ldr r1, .L_0200cb5c
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200cb60
	movs r0, #16
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200cb64
	movs r0, #12
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #40
	bl FieldEvent_WaitFrames
	bl Func_02000bcc
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r0, #160
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #16
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200cb68
	bl Func_020047f0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200cb7e
	.2byte 0x0000
.L_0200cb50:
	.4byte gAdventure
.L_0200cb54:
	.4byte 0x00013333
.L_0200cb58:
	.4byte MsgKandoreanTemple07
.L_0200cb5c:
	.4byte gData_02005564
.L_0200cb60:
	.4byte gData_020055a0
.L_0200cb64:
	.4byte gData_020055e4
.L_0200cb68:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	bl Func_020047f0
.L_0200cb7e:
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_0200cc70
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r2, #0
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #16
	bl Func_020052c8
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #16
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #160
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #16
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200cc74
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #16
	bl Func_020052c8
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #16
	movs r1, #0
	bl Func_02005298
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_0200ccbc
.L_0200cc70:
	.4byte gAdventure
.L_0200cc74:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #12
	adds r3, #2
	strh r3, [r2]
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #8
	adds r1, #255
	movs r0, #16
	movs r2, #20
	bl Func_020052c8
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #16
	movs r1, #0
	bl Func_02005298
.L_0200ccbc:
	ldr r6, .L_0200ce58
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #212
	movs r2, #200
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #17
	movs r1, #0
	bl Camera_MoveWithinMapBounds
	movs r0, #153
	movs r1, #152
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #153
	adds r1, #51
	bl Func_020052d8
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #204
	movs r0, #12
	adds r1, #153
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r6]
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r0, #16
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r1, #244
	movs r2, #208
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #236
	movs r2, #216
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #16
	movs r1, #0
	movs r2, #80
	bl Func_02005290
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #12
	bl Func_020052c8
	movs r1, #176
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #212
	movs r1, #128
	movs r2, #192
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200ce5c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #244
	movs r2, #208
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #252
	movs r2, #188
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #80]
	movs r0, #12
	movs r1, #1
	mov r10, r3
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200ce60
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #246
	movs r2, #188
	lsls r2, r2, #1
	movs r0, #12
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #12
	movs r1, #7
	bl Object_SetModeById
	ldr r3, .L_0200ce64
	ldr r2, .L_0200ce54
	str r3, [r5, #24]
	movs r1, #0
	movs r3, #224
	adds r7, r5, #0
	mov r8, r1
	lsls r3, r3, #8
	mov r1, r10
	adds r7, #85
	strh r3, [r1, #18]
	strb r2, [r7]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #12
	adds r1, #102
	adds r2, #51
	b .L_0200ce68
	.2byte 0x0000
.L_0200ce54:
	.4byte 0x00000000
.L_0200ce58:
	.4byte gAdventure
.L_0200ce5c:
	.4byte 0x00013333
.L_0200ce60:
	.4byte 0x00019999
.L_0200ce64:
	.4byte 0xffff0000
.L_0200ce68:
	bl ObjectMotion_SetSpeedParameters
	movs r1, #222
	movs r2, #160
	movs r3, #188
	lsls r2, r2, #14
	lsls r1, r1, #17
	lsls r3, r3, #17
	adds r0, r5, #0
	bl Func_02005160
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #3
	strb r3, [r7]
	movs r0, #12
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	mov r3, r8
	mov r1, r10
	strh r3, [r1, #18]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #20]
	movs r0, #12
	movs r1, #1
	bl Object_SetModeById
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200cfb0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #212
	movs r2, #188
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #204
	movs r1, #128
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #12
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	movs r2, #180
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #16
	bl ObjectMotion_EnableActionAndResetMotion
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #10
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	ldr r0, [r6]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r6]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200cf74
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #16
	bl ObjectMotion_ResetAndSetPosition
.L_0200cf74:
	movs r0, #16
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl Func_02005230
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl GameFlag_SetBit
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Func_020051c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cfb0:
	.4byte 0x00019999
	.endif
	.section .bss,"aw",%nobits
	.global gData_02006294
gData_02006294:
	.space 0x00000004
	.global gData_02006298
gData_02006298:
	.space 0x00000004
	.global gData_0200629c
gData_0200629c:
	.space 0x00000004
	.global gData_020062a0
gData_020062a0:
	.space 0x00000004
	.global gData_020062a4
gData_020062a4:
	.space 0x00000004
	.global gData_020062a8
gData_020062a8:
	.space 0x00000002
	.global gData_020062aa
gData_020062aa:
	.space 0x00000002
	.global gData_020062ac
gData_020062ac:
