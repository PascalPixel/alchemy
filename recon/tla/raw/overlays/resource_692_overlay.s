.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815c,"ax",%progbits
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r3, [sp, #0]
	ldr r3, .L_0200832c
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r10, r1
	ldr r7, [sp, #48]
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r10
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_020081a4
	cmp r7, #0
	beq .L_020081a4
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_020081ac
.L_020081a4:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_020081ac:
	mov r3, r8
	bl Import_ActorCreate
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020081ba
	b .L_0200831e
.L_020081ba:
	ldr r3, [r6, #80]
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	mov r8, r3
	bl Func_02004c98
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02004ca0
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, .L_02008334
	mov r1, r9
	str r3, [r6, #108]
	ldr r3, [sp, #0]
	adds r0, r6, #0
	str r3, [r6, #68]
	ldr r3, [sp, #36]
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
	ldr r3, .L_02008338
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200831e
	cmp r7, #0
	beq .L_0200831e
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200823c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Actor_SetPartAttribute
.L_0200823c:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02008274
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #3
	ldrb r2, [r7]
	adds r0, r6, #0
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	lsls r2, r2, #2
	mov r1, r8
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r1, [r7]
	bl Scene_SetActorSpritePriority
.L_02008274:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_02008288
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008288:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ce
	ldr r3, .L_02008330
	mov r1, r11
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020082b6
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020082c8
.L_020082b6:
	ldr r2, .L_02008338
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02008338
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020082c8:
	bl __divsi3
	str r0, [r6, #52]
.L_020082ce:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ea
	adds r0, r6, #0
	movs r1, #1
	bl Func_02004c98
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02004ca0
.L_020082ea:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_020082fc
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #18]
.L_020082fc:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200830e
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200830e:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200831e
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200831e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200832c:
	.4byte gSequenceParentWork
.L_02008330:
	.4byte gData_02005698
.L_02008334:
	.4byte BattleEffect_UpdateObjectMotionScaleAndLinkedAngle
.L_02008338:
	.4byte 0xffff0000
	.section .text.x020083b4,"ax",%progbits
	.global Func_020003b4
	.thumb_func
Func_020003b4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200852c
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #12
	bl Actor_GetById
	ldrh r3, [r0, #6]
	ldr r1, .L_02008530
	lsrs r3, r3, #12
	lsls r5, r3, #2
	ldr r2, .L_02008534
	mov r9, r1
	ldr r1, [r1, r5]
	mov r10, r2
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r0, #8]
	mov r7, sp
	adds r3, r3, r2
	str r3, [r7]
	lsls r1, r1, #16
	ldr r3, [r0, #12]
	mov r8, r0
	str r3, [r7, #4]
	ldr r3, [r0, #16]
	adds r0, r7, #0
	adds r3, r3, r1
	str r3, [r7, #8]
	mov r1, r8
	bl Actor_FindInCell
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200840a
	b .L_02008520
.L_0200840a:
	mov r0, r9
	ldr r1, [r0, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
	lsls r1, r1, #16
	adds r3, r3, r2
	str r3, [r7]
	adds r0, r7, #0
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r6, #0
	bl Actor_FindInCell
	cmp r0, #0
	beq .L_02008440
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_02008520
.L_02008440:
	ldr r3, [r6, #8]
	movs r0, #128
	str r3, [r7]
	lsls r0, r0, #13
	ldr r3, [r6, #12]
	adds r1, r6, #0
	adds r3, r3, r0
	str r3, [r7, #4]
	adds r0, r7, #0
	ldr r3, [r6, #16]
	str r3, [r7, #8]
	bl Actor_FindInCell
	cmp r0, #0
	beq .L_0200846c
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_02008520
.L_0200846c:
	adds r2, r6, #0
	adds r2, #34
	movs r3, #2
	strb r3, [r2]
	mov r2, r9
	ldr r1, [r2, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
	lsls r1, r1, #16
	adds r3, r3, r2
	str r3, [r7]
	adds r0, r6, #0
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02004cf8
	cmp r0, #0
	bgt .L_02008520
	adds r3, r6, #0
	adds r3, #98
	ldrb r3, [r3]
	mov r10, r3
	cmp r3, #0
	bne .L_02008520
	movs r1, #8
	mov r0, r8
	movs r5, #204
	bl Func_02004c98
	lsls r5, r5, #6
	movs r0, #15
	bl wait
	adds r5, #51
	movs r0, #185
	bl Func_02004f30
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02004cc8
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Func_02004cc8
	adds r0, r6, #0
	bl Func_02004cd0
	bl Func_02004eb8
	ldr r3, [r7]
	mov r1, r10
	str r3, [r6, #8]
	ldr r3, [r7, #8]
	str r1, [r6, #36]
	str r3, [r6, #16]
	str r1, [r6, #44]
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #24
	str r3, [r2, #56]
	str r3, [r2, #64]
	movs r0, #10
	ldrsh r3, [r2, r0]
	str r1, [r2, #36]
	lsls r3, r3, #16
	str r1, [r2, #44]
	str r3, [r2, #8]
	movs r1, #18
	ldrsh r3, [r2, r1]
	mov r0, r8
	lsls r3, r3, #16
	str r3, [r2, #16]
	movs r1, #1
	bl Func_02004c98
.L_02008520:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200852c:
	.4byte gSequenceParentWork
.L_02008530:
	.4byte gData_02004fec
.L_02008534:
	.4byte 0xffff0000
	.section .text.x020085dc,"ax",%progbits
	.balign 4
	.global Func_020005dc
	.thumb_func
Func_020005dc:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	mov r10, r3
	movs r3, #192
	sub sp, #4
	adds r4, r1, #0
	lsls r3, r3, #18
	mov r8, r0
	str r4, [sp, #0]
	adds r6, r2, #0
	ldr r5, [r3, #32]
	bl Func_02004d20
	mov r2, r8
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r5, [r5, r3]
	ldr r3, .L_02008634
	ldr r4, [sp, #0]
	ldr r2, .L_02008638
	adds r5, r5, r3
	lsls r6, r6, #7
	asrs r5, r5, #2
	adds r1, r0, #0
	adds r4, r4, r6
	adds r5, r5, r2
	mov r0, r8
	mov r2, r10
	adds r5, r5, r4
	bl Func_02004ef0
	strb r0, [r5]
	add sp, #4
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_02008634:
	.4byte 0xfdff0000
.L_02008638:
	.4byte gMapShapeGrid
	.section .text.x020086dc,"ax",%progbits
	.balign 4
	.global Func_020006dc
	.thumb_func
Func_020006dc:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	b .L_0200874c
.L_020086e2:
	ldrh r0, [r6]
	bl Actor_GetById
	adds r5, r0, #0
	adds r7, r5, #0
	movs r3, #0
	adds r7, #99
	strb r3, [r7]
	movs r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	bl Func_02000658
	movs r3, #64
	ands r0, r3
	cmp r0, #0
	beq .L_0200874a
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_02008758
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
	movs r3, #255
	strb r3, [r4, #2]
	lsls r2, r2, #7
	ldr r4, [r0]
	adds r1, r1, r2
	lsls r1, r1, #2
	movs r3, #1
	negs r3, r3
	adds r4, r4, r1
	strb r3, [r4, #2]
	movs r3, #1
	strb r3, [r7]
.L_0200874a:
	adds r6, #2
.L_0200874c:
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020086e2
.L_02008758:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200875c,"ax",%progbits
	.global Func_0200075c
	.thumb_func
Func_0200075c:
	push {r5, lr}
	adds r5, r0, #0
	movs r2, #99
	adds r2, r2, r5
	ldrb r3, [r2]
	mov r12, r2
	cmp r3, #0
	beq .L_020087aa
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_020087aa
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
.L_020087aa:
	pop {r5, pc}
	.section .text.x02008880,"ax",%progbits
	.global Func_02000880
	.thumb_func
Func_02000880:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	b .L_020088d0
.L_0200888a:
	ldrh r0, [r7]
	bl Actor_GetById
	movs r3, #34
	adds r6, r0, #0
	adds r3, r3, r6
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	mov r8, r3
	bl Map_GetTerrainHeight
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	adds r5, r0, #0
	movs r1, #0
	adds r0, r6, #0
	bl Actor_SetSingleChildFlags
	mov r2, r8
	ldrb r0, [r2]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	asrs r5, r5, #19
	adds r5, #4
	asrs r2, r2, #20
	adds r3, r5, #0
	asrs r1, r1, #20
	adds r7, #6
	bl Func_020005dc
.L_020088d0:
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200888a
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008a30,"ax",%progbits
	.global Func_02000a30
	.thumb_func
Func_02000a30:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r3, r1
	ldr r7, [r3]
	sub sp, #8
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	bl Func_02004ee8
	ldr r3, .L_02008b0c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008b10
	cmp r2, r3
	bne .L_02008b3a
	ldr r0, .L_02008b14
	bl Func_020006dc
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #2
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	cmp r0, r3
	bne .L_02008a84
	b .L_02008bf4
.L_02008a84:
	movs r2, #34
	adds r2, r2, r7
	movs r3, #2
	strb r3, [r2]
	movs r3, #85
	adds r3, r3, r7
	mov r8, r3
	mov r1, r8
	movs r3, #3
	movs r0, #1
	strb r3, [r1]
	mov r10, r2
	bl wait
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b22
	movs r1, #128
	lsls r1, r1, #5
	str r1, [r7, #72]
	adds r0, r7, #0
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #2
	orrs r3, r2
	strb r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r1, [r3]
	movs r6, #128
	ldr r3, .L_02008b08
	lsls r6, r6, #19
	adds r6, #80
	ldr r2, [r7, #16]
	strh r3, [r6]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	ldr r1, [r7, #20]
	ldr r0, [r7, #8]
	bl OverlayObject_PrepareObject
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0200063c
	movs r0, #213
	bl Func_02004f30
	adds r0, r5, #0
	bl Func_02004cb0
	adds r0, r7, #0
	movs r1, #4
	bl Func_02004c98
	movs r0, #1
	b .L_02008b18
	.2byte 0x0000
.L_02008b08:
	.4byte 0x00003f10
.L_02008b0c:
	.4byte gSequenceParentWork
.L_02008b10:
	.4byte 0x000000e7
.L_02008b14:
	.4byte gData_02005528
.L_02008b18:
	bl wait
	movs r3, #0
	strh r3, [r6]
	b .L_02008b2e
.L_02008b22:
	adds r0, r7, #0
	bl Func_0200063c
	movs r0, #188
	bl Func_02004f30
.L_02008b2e:
	movs r3, #0
	mov r1, r8
	mov r2, r10
	strb r3, [r1]
	strb r3, [r2]
	b .L_02008bf4
.L_02008b3a:
	ldr r3, .L_02008bd4
	cmp r2, r3
	bne .L_02008bf4
	ldr r0, .L_02008bd8
	bl Func_020006dc
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #2
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	cmp r0, r3
	beq .L_02008bf4
	movs r3, #34
	adds r3, r3, r7
	mov r9, r3
	mov r2, r9
	movs r3, #2
	strb r3, [r2]
	movs r3, #85
	adds r3, r3, r7
	mov r10, r3
	movs r1, #0
	movs r3, #3
	mov r8, r1
	mov r1, r10
	strb r3, [r1]
	movs r0, #1
	bl wait
	movs r2, #128
	lsls r2, r2, #5
	str r2, [r7, #72]
	adds r1, r7, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r5, #2
	orrs r5, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strb r5, [r1]
	movs r6, #128
	strh r2, [r3]
	ldr r3, .L_02008bd0
	lsls r6, r6, #19
	adds r6, #80
	ldr r2, [r7, #16]
	strh r3, [r6]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	ldr r1, [r7, #20]
	ldr r0, [r7, #8]
	bl OverlayObject_PrepareObject
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0200063c
	movs r0, #213
	bl Func_02004f30
	adds r0, r5, #0
	bl Func_02004cb0
	movs r1, #4
	adds r0, r7, #0
	bl Func_02004c98
	movs r0, #1
	b .L_02008bdc
.L_02008bd0:
	.4byte 0x00003f10
.L_02008bd4:
	.4byte 0x000000e8
.L_02008bd8:
	.4byte gData_0200552e
.L_02008bdc:
	bl wait
	ldr r5, .L_02008c14
	mov r1, r8
	strh r1, [r6]
	adds r0, r7, #0
	bl Func_0200063c
	mov r2, r10
	mov r3, r9
	strb r5, [r2]
	strb r5, [r3]
.L_02008bf4:
	ldr r3, [r7, #8]
	movs r1, #240
	asrs r6, r3, #20
	ldr r3, [r7, #16]
	lsls r1, r1, #1
	asrs r5, r3, #20
	ldr r3, .L_02008c18
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008c1c
	cmp r2, r3
	bne .L_02008c2a
	cmp r5, #16
	bne .L_02008cc2
	b .L_02008c20
.L_02008c14:
	.4byte 0x00000000
.L_02008c18:
	.4byte gSequenceParentWork
.L_02008c1c:
	.4byte 0x000000e3
.L_02008c20:
	movs r0, #159
	lsls r0, r0, #4
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008c2a:
	ldr r3, .L_02008cd4
	cmp r2, r3
	bne .L_02008c40
	cmp r5, #41
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #241
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008c40:
	ldr r3, .L_02008cd8
	cmp r2, r3
	bne .L_02008c76
	cmp r6, #21
	bne .L_02008cc2
	cmp r5, #43
	bne .L_02008c58
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #242
	bl GameFlag_SetBit
.L_02008c58:
	cmp r5, #45
	bne .L_02008c66
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #243
	bl GameFlag_SetBit
.L_02008c66:
	cmp r5, #47
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #244
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008c76:
	ldr r3, .L_02008cdc
	cmp r2, r3
	bne .L_02008cae
	cmp r6, #47
	bne .L_02008c9e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #245
	bl GameFlag_SetBit
	movs r3, #46
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #74
	movs r2, #1
	movs r3, #1
	bl Func_02004ce8
.L_02008c9e:
	cmp r6, #57
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #246
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008cae:
	ldr r3, .L_02008ce0
	cmp r2, r3
	bne .L_02008cc2
	cmp r6, #43
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #247
	bl GameFlag_SetBit
.L_02008cc2:
	bl Func_02004d48
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008cd4:
	.4byte 0x000000e4
.L_02008cd8:
	.4byte 0x000000e5
.L_02008cdc:
	.4byte 0x000000e7
.L_02008ce0:
	.4byte 0x000000e8
	.section .text.x02008ed0,"ax",%progbits
	.balign 4
	.global Func_02000ed0
	.thumb_func
Func_02000ed0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	mov r8, r0
	movs r0, #148
	adds r0, #255
	sub sp, #68
	bl Import_ActorCreate
	mov r1, r8
	adds r6, r0, #0
	ldrh r0, [r1, #6]
	bl Math_Cosine
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	lsls r0, r0, #1
	strb r3, [r2]
	movs r1, #1
	mov r10, r0
	adds r0, r6, #0
	bl Func_02004c98
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	ldr r3, [r6, #8]
	add r2, sp, #56
	str r3, [r2]
	ldr r3, [r6, #12]
	str r3, [r2, #4]
	ldr r3, [r6, #16]
	str r3, [r2, #8]
.L_02008f2e:
	add r7, sp, #56
	ldr r1, [r7]
	ldr r2, [r7, #8]
	ldr r3, [r7, #4]
	add r1, r10
	subs r2, r2, r3
	str r1, [r7]
	asrs r2, r2, #20
	asrs r1, r1, #20
	movs r0, #2
	bl Func_02000658
	asrs r0, r0, #8
	mov r11, r0
	cmp r0, #212
	beq .L_02008fc8
	cmp r0, #222
	beq .L_02008fc8
	movs r2, #8
	mov r9, r2
.L_02008f56:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	mov r0, r9
	lsls r3, r0, #2
	adds r3, #20
	ldr r5, [r2, r3]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02008fbc
	mov r1, r8
	ldr r2, [r1, #8]
	ldr r3, [r5, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008f90
	ldr r2, [r1, #12]
	ldr r3, [r5, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008f90
	ldr r2, [r1, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_02008fbc
.L_02008f90:
	adds r0, r5, #0
	adds r0, #8
	adds r1, r7, #0
	bl Func_020005b0
	cmp r0, #8
	bgt .L_02008fbc
	ldr r2, [r5, #12]
	ldr r3, [r7, #4]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008fbc
	ldr r3, [r5, #80]
	movs r0, #136
	ldr r3, [r3, #40]
	lsls r0, r0, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r0
	bne .L_020090b0
	b .L_02008fc8
.L_02008fbc:
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #64
	bne .L_02008f56
	b .L_02008f2e
.L_02008fc8:
	ldr r3, .L_02009134
	movs r5, #128
	lsls r5, r5, #10
	add r7, sp, #56
	str r5, [r6, #52]
	str r5, [r6, #48]
	ldr r2, [r7, #4]
	str r3, [r6, #108]
	ldr r1, [r7]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02004cc8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	bl Func_02004e10
	ldr r1, [r7, #4]
	ldr r2, [r7, #8]
	movs r3, #1
	ldr r0, [r7]
	bl Camera_MoveWithinMapBounds
	adds r0, r6, #0
	bl Func_02004cd0
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	bl Func_02004cb0
	movs r3, #0
	mov r9, r3
.L_0200900c:
	ldr r3, .L_02009138
	movs r2, #1
	ldr r3, [r3]
	mov r10, r3
	mov r0, r10
	ands r0, r2
	mov r10, r0
	cmp r0, #0
	bne .L_0200906a
	add r1, sp, #16
	mov r8, r1
	movs r3, #148
	mov r0, r8
	adds r3, #255
	strh r3, [r0, #24]
	ldr r3, .L_0200913c
	str r2, [r0]
	str r3, [r0, #36]
	ldr r3, .L_02009140
	movs r6, #31
	str r3, [r0, #28]
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ands r0, r6
	ldr r4, [r7]
	mov r3, r10
	ands r5, r6
	subs r0, #16
	ldr r1, [r7, #4]
	ldr r2, [r7, #8]
	lsls r0, r0, #13
	subs r5, #16
	str r3, [sp, #4]
	movs r3, #153
	lsls r3, r3, #17
	lsls r5, r5, #13
	str r0, [sp, #0]
	mov r0, r8
	str r3, [sp, #8]
	str r0, [sp, #12]
	adds r3, r5, #0
	adds r0, r4, #0
	bl Func_0200015c
.L_0200906a:
	movs r0, #1
	bl wait
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #16
	bne .L_0200900c
	movs r0, #24
	bl wait
	mov r3, r11
	cmp r3, #222
	bne .L_02009090
	bl Func_02001350
	bl Func_02004d48
	b .L_02009126
.L_02009090:
	ldr r3, .L_02009144
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_02004e20
	movs r0, #10
	bl wait
	bl Func_02004d48
	b .L_02009126
.L_020090b0:
	ldr r3, .L_02009134
	movs r5, #128
	lsls r5, r5, #10
	str r5, [r6, #52]
	str r5, [r6, #48]
	ldr r2, [r7, #4]
	str r3, [r6, #108]
	ldr r1, [r7]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02004cc8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	bl Func_02004e10
	ldr r2, [r7, #8]
	ldr r1, [r7, #4]
	movs r3, #1
	ldr r0, [r7]
	bl Camera_MoveWithinMapBounds
	adds r0, r6, #0
	bl Func_02004cd0
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	bl Func_02004cb0
	movs r0, #188
	lsls r0, r0, #2
	bl GameFlag_SetBit
	mov r1, r9
	lsls r0, r1, #16
	lsrs r0, r0, #16
	bl Func_02000ce4
	movs r0, #188
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	ldr r3, .L_02009144
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_02004e20
	movs r0, #10
	bl wait
	bl Func_02004d48
.L_02009126:
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009134:
	.4byte Func_02000d1c
.L_02009138:
	.4byte gFrameCount
.L_0200913c:
	.4byte Func_02000ea4
.L_02009140:
	.4byte gData_0200502c
.L_02009144:
	.4byte gSequenceParentWork
	.section .text.x020091fc,"ax",%progbits
	.balign 4
	.global Func_020011fc
	.thumb_func
Func_020011fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009344
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #12
	bl Actor_GetById
	adds r5, r0, #0
	ldrh r1, [r5, #6]
	movs r3, #128
	lsls r3, r3, #6
	adds r1, r1, r3
	movs r3, #192
	lsls r3, r3, #8
	ldr r2, .L_02009348
	ands r1, r3
	ldr r3, [r5, #8]
	movs r0, #128
	lsls r0, r0, #12
	ands r3, r2
	mov r6, sp
	adds r3, r3, r0
	str r3, [r6]
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r2
	adds r3, r3, r0
	movs r0, #128
	lsls r0, r0, #14
	adds r2, r6, #0
	str r3, [r6, #8]
	bl Vector_AddPolarOffsetFar
	adds r0, r6, #0
	adds r1, r5, #0
	bl Actor_FindInCell
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02009336
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r1, #6
	adds r0, r5, #0
	bl Func_02004c98
	movs r0, #6
	bl wait
	movs r0, #152
	bl Func_02004f30
	adds r0, r5, #0
	movs r1, #7
	bl Func_02004c98
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	ldr r3, .L_0200934c
	movs r2, #85
	str r3, [r5, #40]
	adds r2, r2, r5
	mov r10, r2
	ldrb r2, [r2]
	movs r3, #126
	ands r3, r2
	mov r2, r10
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, [r5, #8]
	ldr r0, [r6]
	movs r1, #12
	subs r0, r0, r3
	bl __divsi3
	ldr r3, [r5, #16]
	mov r11, r0
	ldr r0, [r6, #8]
	movs r1, #12
	subs r0, r0, r3
	bl __divsi3
	movs r3, #0
	mov r9, r0
	mov r8, r3
.L_020092ce:
	ldr r2, [r7, #8]
	ldr r3, [r6]
	movs r0, #1
	subs r2, r2, r3
	ldr r3, [r5, #8]
	add r2, r11
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6, #8]
	subs r2, r2, r3
	ldr r3, [r5, #16]
	add r2, r9
	adds r3, r3, r2
	str r3, [r5, #16]
	ldr r3, [r7, #8]
	str r3, [r6]
	ldr r3, [r7, #16]
	str r3, [r6, #8]
	bl wait
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #12
	bne .L_020092ce
	ldr r3, [r7, #8]
	movs r2, #128
	str r3, [r5, #8]
	ldr r3, [r7, #12]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #20]
	str r3, [r5, #12]
	ldr r3, [r7, #16]
	adds r0, r5, #0
	str r3, [r5, #16]
	movs r1, #6
	bl Func_02004c98
	adds r0, r5, #0
	movs r1, #1
	bl Actor_SetSingleChildFlags
	mov r3, r10
	ldrb r2, [r3]
	movs r3, #3
	orrs r3, r2
	mov r2, r10
	strb r3, [r2]
	bl Func_02004d48
.L_02009336:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009344:
	.4byte gSequenceParentWork
.L_02009348:
	.4byte 0xfff00000
.L_0200934c:
	.4byte 0x0004cccc
	.section .text.x02009350,"ax",%progbits
	.global Func_02001350
	.thumb_func
Func_02001350:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #0
	sub sp, #76
	str r1, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #160
	lsls r2, r2, #1
	adds r2, r3, r2
	add r3, sp, #36
	mov r8, r3
	movs r3, #148
	adds r3, #255
	mov r11, r1
	mov r9, r1
	mov r10, r1
	mov r1, r8
	str r2, [sp, #16]
	strh r3, [r1, #24]
	movs r3, #1
	str r3, [r1]
	ldr r3, .L_020096b4
	movs r2, #240
	str r3, [r1, #28]
	movs r3, #230
	lsls r3, r3, #9
	adds r3, #204
	str r3, [r1, #8]
	str r3, [r1, #12]
	ldr r3, .L_020096b8
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r3, .L_020096bc
	add r2, sp, #24
	cmp r1, r3
	bne .L_020093c6
	movs r3, #164
	lsls r3, r3, #17
	str r3, [sp, #20]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2, #4]
	movs r3, #200
	lsls r3, r3, #16
	movs r0, #192
	str r3, [r2, #8]
	movs r1, #20
	movs r2, #69
	lsls r0, r0, #2
	b .L_0200941c
.L_020093c6:
	ldr r3, .L_020096c0
	cmp r1, r3
	bne .L_020093fa
	movs r1, #232
	movs r3, #128
	lsls r3, r3, #14
	lsls r1, r1, #16
	str r1, [sp, #20]
	movs r0, #192
	str r3, [r2, #4]
	movs r3, #248
	lsls r3, r3, #16
	lsls r0, r0, #2
	str r3, [r2, #8]
	adds r0, #1
	movs r2, #14
	movs r3, #72
	mov r11, r2
	mov r9, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200942c
	movs r1, #212
	mov r10, r1
	b .L_0200942c
.L_020093fa:
	ldr r3, .L_020096c4
	cmp r1, r3
	bne .L_0200942c
	movs r3, #196
	lsls r3, r3, #17
	str r3, [sp, #20]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2, #4]
	movs r0, #192
	movs r3, #134
	lsls r3, r3, #18
	lsls r0, r0, #2
	str r3, [r2, #8]
	movs r1, #24
	movs r2, #90
	adds r0, #2
.L_0200941c:
	mov r11, r1
	mov r9, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200942c
	movs r3, #212
	mov r10, r3
.L_0200942c:
	movs r7, #0
.L_0200942e:
	bl Random16Far
	movs r5, #31
	ldr r1, [sp, #20]
	ands r0, r5
	lsls r0, r0, #16
	add r6, sp, #24
	adds r0, r1, r0
	str r0, [r6]
	movs r0, #145
	bl Func_02004f30
	bl Random16Far
	movs r3, #0
	ldr r4, [r6]
	ldr r1, [r6, #4]
	ldr r2, [r6, #8]
	str r3, [sp, #4]
	movs r3, #232
	lsls r3, r3, #14
	adds r3, #1
	ands r0, r5
	str r3, [sp, #8]
	lsls r0, r0, #12
	mov r3, r8
	str r0, [sp, #0]
	str r3, [sp, #12]
	adds r0, r4, #0
	movs r3, #0
	bl Func_0200015c
	movs r0, #160
	movs r1, #160
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
	adds r7, #1
	movs r0, #8
	bl FieldEvent_Wait
	cmp r7, #16
	bne .L_0200942e
	movs r0, #148
	bl Func_02004f30
	mov r1, r8
	movs r3, #1
	str r3, [r1]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r1, #8]
	str r3, [r1, #12]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r1, #16]
	str r3, [r1, #20]
	ldr r2, [sp, #20]
	ldr r3, .L_020096c8
	movs r7, #0
	adds r2, r2, r3
	str r2, [sp, #20]
	adds r5, r2, #0
.L_020094c4:
	str r5, [r6]
	bl Random16Far
	movs r3, #0
	movs r4, #31
	ldr r1, [r6, #4]
	ldr r2, [r6, #8]
	ands r4, r0
	ldr r0, [r6]
	str r3, [sp, #0]
	movs r3, #224
	lsls r3, r3, #12
	adds r3, #1
	str r3, [sp, #8]
	mov r3, r8
	lsls r4, r4, #10
	str r3, [sp, #12]
	movs r3, #0
	str r4, [sp, #4]
	bl Func_0200015c
	movs r1, #192
	lsls r1, r1, #11
	adds r7, #1
	adds r5, r5, r1
	cmp r7, #8
	bne .L_020094c4
	mov r3, r9
	movs r5, #3
	movs r7, #6
	movs r0, #68
	movs r1, #70
	mov r2, r11
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl Func_02004cd8
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
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004d10
	movs r0, #30
	bl FieldEvent_Wait
	mov r2, r11
	mov r3, r9
	movs r0, #71
	movs r1, #70
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl Func_02004cd8
	movs r3, #148
	mov r2, r8
	adds r3, #255
	strh r3, [r2, #24]
	movs r3, #1
	str r3, [r2]
	movs r3, #10
	str r3, [r2, #4]
	ldr r3, .L_020096cc
	str r3, [r2, #28]
	mov r3, r10
	cmp r3, #0
	beq .L_02009580
	movs r1, #70
	mov r3, r9
	movs r0, #68
	mov r2, r11
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl Func_02004cd8
	movs r0, #30
	bl FieldEvent_Wait
	ldr r3, [r6, #8]
	ldr r1, .L_020096d0
	adds r3, r3, r1
	str r3, [r6, #8]
	b .L_02009742
.L_02009580:
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_02004d10
	movs r0, #107
	bl Func_02004f30
	movs r5, #0
.L_02009598:
	movs r7, #0
.L_0200959a:
	bl Random16Far
	movs r1, #48
	bl __umodsi3
	ldr r2, [sp, #20]
	lsls r0, r0, #16
	adds r0, r2, r0
	ldr r2, [r6, #8]
	lsls r3, r5, #18
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #12
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #9
	str r0, [r6]
	ldr r1, [r6, #4]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #204
	lsls r3, r3, #14
	str r3, [sp, #8]
	mov r3, r8
	str r3, [sp, #12]
	adds r7, #1
	movs r3, #0
	bl Func_0200015c
	cmp r7, #6
	bne .L_0200959a
	movs r0, #3
	bl wait
	lsrs r3, r5, #2
	adds r3, #6
	str r3, [sp, #4]
	movs r7, #3
	movs r0, #74
	movs r1, #70
	mov r2, r11
	mov r3, r9
	adds r5, #1
	str r7, [sp, #0]
	bl Func_02004cd8
	cmp r5, #12
	bne .L_02009598
	movs r3, #8
	str r3, [sp, #4]
	movs r1, #70
	movs r0, #74
	mov r2, r11
	mov r3, r9
	str r7, [sp, #0]
	bl Func_02004cd8
	movs r1, #0
	mov r10, r1
.L_02009612:
	movs r3, #3
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	bne .L_0200965c
	movs r7, #0
.L_0200961e:
	bl Random16Far
	movs r1, #48
	add r5, sp, #24
	bl __umodsi3
	ldr r3, [sp, #20]
	ldr r2, [r5, #8]
	lsls r0, r0, #16
	adds r0, r3, r0
	movs r3, #192
	lsls r3, r3, #14
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #9
	str r0, [r5]
	ldr r1, [r5, #4]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #204
	lsls r3, r3, #14
	str r3, [sp, #8]
	mov r3, r8
	str r3, [sp, #12]
	adds r7, #1
	movs r3, #0
	bl Func_0200015c
	cmp r7, #4
	bne .L_0200961e
.L_0200965c:
	mov r1, r10
	cmp r1, #0
	bne .L_02009678
	ldr r2, [sp, #16]
	ldr r1, .L_020096d4
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r2, #12]
	bl Func_02004cc0
	movs r0, #1
	bl FieldEvent_Wait
	b .L_0200972c
.L_02009678:
	mov r2, r10
	cmp r2, #63
	bhi .L_02009696
	ldr r3, .L_020096a8
	lsrs r2, r2, #2
	subs r3, r3, r2
	movs r1, #128
	lsls r1, r1, #19
	lsls r3, r3, #8
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	movs r2, #128
	ldr r3, .L_020096ac
	b .L_020096a0
.L_02009696:
	mov r3, r10
	cmp r3, #64
	bne .L_020096d8
	movs r2, #128
	ldr r3, .L_020096b0
.L_020096a0:
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	b .L_0200972c
.L_020096a8:
	.4byte 0x00000010
.L_020096ac:
	.4byte 0x00003f44
.L_020096b0:
	.4byte 0x00000000
.L_020096b4:
	.4byte gData_02005050
.L_020096b8:
	.4byte gSequenceParentWork
.L_020096bc:
	.4byte 0x000000e3
.L_020096c0:
	.4byte 0x000000e4
.L_020096c4:
	.4byte 0x000000e5
.L_020096c8:
	.4byte 0xfff80000
.L_020096cc:
	.4byte gData_0200502c
.L_020096d0:
	.4byte 0xffd80000
.L_020096d4:
	.4byte 0xffe00000
.L_020096d8:
	mov r1, r10
	cmp r1, #127
	bhi .L_02009714
	ldr r2, [sp, #16]
	movs r1, #128
	ldr r3, [r2, #12]
	lsls r1, r1, #8
	adds r3, r3, r1
	str r3, [r2, #12]
	add r2, sp, #24
	ldr r3, [r2, #8]
	ldr r1, .L_02009814
	movs r0, #71
	adds r3, r3, r1
	str r3, [r2, #8]
	mov r3, r10
	subs r3, #47
	mov r2, r9
	lsrs r3, r3, #5
	subs r3, r2, r3
	movs r1, #1
	movs r2, #3
	str r2, [sp, #0]
	str r1, [sp, #4]
	adds r3, #8
	movs r1, #76
	mov r2, r11
	bl Func_02004cd8
	b .L_0200972c
.L_02009714:
	mov r3, r10
	cmp r3, #128
	bne .L_0200972c
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
.L_0200972c:
	movs r1, #1
	add r10, r1
	movs r0, #1
	bl wait
	mov r2, r10
	cmp r2, #160
	beq .L_0200973e
	b .L_02009612
.L_0200973e:
	bl Func_02004d18
.L_02009742:
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02004f30
	movs r0, #136
	bl Func_02004f30
	movs r3, #230
	lsls r3, r3, #9
	add r6, sp, #36
	adds r3, #204
	str r3, [r6, #12]
	str r3, [r6, #8]
	movs r7, #0
.L_0200975e:
	bl Random16Far
	movs r1, #48
	add r5, sp, #24
	bl __umodsi3
	ldr r3, [sp, #20]
	ldr r2, [r5, #8]
	lsls r0, r0, #16
	adds r0, r3, r0
	movs r3, #192
	lsls r3, r3, #14
	adds r2, r2, r3
	movs r3, #236
	lsls r3, r3, #14
	str r0, [r5]
	ldr r1, [r5, #4]
	adds r7, #1
	movs r5, #0
	str r3, [sp, #8]
	movs r3, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	str r6, [sp, #12]
	bl Func_0200015c
	cmp r7, #6
	bne .L_0200975e
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
	str r3, [sp, #0]
	movs r1, #64
	mov r2, r11
	mov r3, r9
	movs r0, #68
	str r7, [sp, #4]
	bl Func_02004cd8
	bl Func_02004d18
	movs r0, #30
	bl FieldEvent_Wait
	ldr r1, [sp, #16]
	ldr r3, .L_02009818
	str r5, [r1, #12]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200981c
	cmp r2, r3
	bne .L_020097f0
	bl Func_020034bc
	b .L_02009806
.L_020097f0:
	ldr r3, .L_02009820
	cmp r2, r3
	bne .L_020097fc
	bl Func_020034f0
	b .L_02009806
.L_020097fc:
	ldr r3, .L_02009824
	cmp r2, r3
	bne .L_02009806
	bl Func_02003548
.L_02009806:
	add sp, #76
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009814:
	.4byte 0xffff8000
.L_02009818:
	.4byte gSequenceParentWork
.L_0200981c:
	.4byte 0x000000e3
.L_02009820:
	.4byte 0x000000e4
.L_02009824:
	.4byte 0x000000e5
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
	.4byte gSequenceParentWork
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
	.4byte gSequenceParentWork
.L_02009c98:
	.4byte 0x000000e3
.L_02009c9c:
	.4byte 0xfff00000
	.section .text.x02009d64,"ax",%progbits
	.global Func_02001d64
	.thumb_func
Func_02001d64:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #68
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r0, #107
	bl Func_02004f30
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02004d10
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #194
	movs r1, #1
	movs r2, #136
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	bl Func_02004e20
	movs r3, #148
	add r6, sp, #16
	adds r3, #255
	strh r3, [r6, #24]
	movs r3, #2
	str r3, [r6]
	movs r3, #10
	str r3, [r6, #4]
	ldr r3, .L_0200a11c
	movs r2, #0
	str r3, [r6, #28]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #12]
	str r3, [r6, #8]
	add r3, sp, #56
	str r2, [r3, #4]
	mov r8, r3
	movs r7, #0
.L_02009dce:
	bl Random16Far
	movs r5, #31
	ands r0, r5
	movs r2, #190
	lsls r2, r2, #18
	lsls r0, r0, #16
	adds r0, r0, r2
	mov r3, r8
	str r0, [r3]
	bl Random16Far
	adds r2, r0, #0
	ands r2, r5
	movs r3, #128
	lsls r3, r3, #17
	lsls r2, r2, #16
	adds r2, r2, r3
	mov r3, r8
	str r2, [r3, #8]
	ldr r0, [r3]
	ldr r1, [r3, #4]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [sp, #0]
	movs r3, #236
	lsls r3, r3, #14
	str r3, [sp, #8]
	movs r5, #0
	movs r3, #0
	str r5, [sp, #4]
	str r6, [sp, #12]
	bl Func_0200015c
	movs r3, #7
	ands r3, r7
	cmp r3, #0
	bne .L_02009e24
	movs r0, #156
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004f30
.L_02009e24:
	cmp r7, #32
	bne .L_02009e68
	movs r1, #194
	movs r2, #248
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #186
	lsls r1, r1, #2
	movs r2, #216
	movs r0, #13
	bl Actor_ResetAndSetPosition
	movs r0, #13
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #13
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #13
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #13
	b .L_0200a04c
.L_02009e68:
	cmp r7, #40
	bne .L_02009eae
	movs r1, #194
	movs r2, #248
	movs r0, #20
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #194
	movs r2, #164
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #20
	bl Actor_ResetAndSetPosition
	movs r0, #20
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #20
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #20
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #20
	b .L_0200a04c
.L_02009eae:
	cmp r7, #48
	bne .L_02009ef2
	movs r1, #194
	movs r2, #248
	movs r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #214
	lsls r1, r1, #2
	movs r2, #232
	movs r0, #14
	bl Actor_ResetAndSetPosition
	movs r0, #14
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #14
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #14
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #14
	b .L_0200a04c
.L_02009ef2:
	cmp r7, #56
	bne .L_02009f38
	movs r1, #194
	movs r2, #248
	movs r0, #19
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #174
	movs r2, #156
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #19
	bl Actor_ResetAndSetPosition
	movs r0, #19
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #19
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #19
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #19
	b .L_0200a04c
.L_02009f38:
	cmp r7, #64
	bne .L_02009f7e
	movs r1, #194
	movs r2, #248
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #182
	movs r2, #140
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #15
	bl Actor_ResetAndSetPosition
	movs r0, #15
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #15
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #15
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #15
	b .L_0200a04c
.L_02009f7e:
	cmp r7, #72
	bne .L_02009fc4
	movs r1, #194
	movs r2, #248
	movs r0, #18
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #206
	movs r2, #140
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #18
	bl Actor_ResetAndSetPosition
	movs r0, #18
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #18
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #18
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #18
	b .L_0200a04c
.L_02009fc4:
	cmp r7, #80
	bne .L_0200a00a
	movs r1, #194
	movs r2, #248
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #206
	movs r2, #156
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #16
	bl Actor_ResetAndSetPosition
	movs r0, #16
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #16
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #16
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #16
	b .L_0200a04c
.L_0200a00a:
	cmp r7, #88
	bne .L_0200a056
	movs r1, #194
	movs r2, #248
	movs r0, #17
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #194
	lsls r1, r1, #2
	movs r2, #232
	movs r0, #17
	bl Actor_ResetAndSetPosition
	movs r0, #17
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #17
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #17
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #17
.L_0200a04c:
	bl Actor_GetById
	ldr r3, .L_0200a120
	str r3, [r0, #108]
	b .L_0200a0c4
.L_0200a056:
	cmp r7, #112
	bne .L_0200a0c4
	movs r1, #194
	movs r2, #248
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #174
	movs r0, #12
	lsls r1, r1, #2
	movs r2, #232
	bl Actor_ResetAndSetPosition
	movs r0, #174
	movs r1, #1
	movs r2, #232
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	movs r0, #12
	bl Actor_GetById
	movs r3, #208
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #12
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #12
	bl Actor_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #12
	bl Actor_GetById
	ldr r3, .L_0200a120
	movs r2, #230
	str r3, [r0, #108]
	movs r1, #1
	movs r0, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
.L_0200a0c4:
	movs r0, #3
	adds r7, #1
	bl wait
	cmp r7, #128
	beq .L_0200a0d2
	b .L_02009dce
.L_0200a0d2:
	movs r0, #12
	bl Actor_WaitForMoveById
	bl Func_02004d18
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02004f30
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #248
	bl GameFlag_SetBit
	ldr r3, .L_0200a124
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_02004e20
	movs r0, #10
	bl wait
	bl Func_02004d48
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a11c:
	.4byte gData_02005050
.L_0200a120:
	.4byte Func_02001d04
.L_0200a124:
	.4byte gSequenceParentWork
	.section .text.x0200a1c0,"ax",%progbits
	.balign 4
	.global Func_020021c0
	.thumb_func
Func_020021c0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	movs r2, #211
	lsls r2, r2, #4
	adds r3, r7, r2
	movs r0, #0
	ldrsb r0, [r3, r0]
	bl Actor_GetById
	movs r1, #0
	adds r5, r0, #0
	bl Actor_SetPartAttribute
	movs r0, #136
	bl Func_02004f30
	movs r6, #0
.L_0200a1e6:
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #5
	adds r6, #1
	bl wait
	cmp r6, #16
	bne .L_0200a1e6
	movs r2, #211
	lsls r2, r2, #4
	adds r3, r7, r2
	movs r0, #0
	ldrsb r0, [r3, r0]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #238
	adds r0, r0, r3
	bl GameFlag_SetBit
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a224
	b .L_0200a314
.L_0200a224:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a314
	movs r0, #11
	bl Actor_GetById
	adds r5, r0, #0
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02004d10
	movs r0, #141
	bl Func_02004f30
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #154
	movs r1, #1
	movs r2, #148
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02004e20
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #151
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004f30
	ldr r3, .L_0200a2c0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200a2c4
	subs r2, #2
	strh r3, [r2]
	ldr r2, [r5, #16]
	movs r3, #192
	lsls r3, r3, #14
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	ldr r0, [r5, #8]
	ldr r1, [r5, #20]
	bl OverlayObject_PrepareObject
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r7, r0, #0
	movs r6, #0
.L_0200a2b4:
	ldr r3, [r5, #12]
	ldr r2, .L_0200a2c8
	movs r0, #5
	adds r3, r3, r2
	str r3, [r5, #12]
	b .L_0200a2cc
.L_0200a2c0:
	.4byte 0x00001000
.L_0200a2c4:
	.4byte 0x00003f10
.L_0200a2c8:
	.4byte 0xffff0000
.L_0200a2cc:
	adds r6, #1
	bl FieldEvent_Wait
	cmp r6, #32
	bne .L_0200a2b4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #42
	bl Func_02004f30
	movs r1, #4
	adds r0, r5, #0
	bl Func_02004c98
	adds r0, r7, #0
	bl Func_02004cb0
	movs r0, #1
	bl FieldEvent_Wait
	ldr r3, .L_0200a310
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r1, #38
	movs r2, #18
	movs r3, #0
	movs r0, #0
	bl Func_02000694
	bl Func_02004d48
	b .L_0200a314
.L_0200a310:
	.4byte 0x00000000
.L_0200a314:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a4ac,"ax",%progbits
	.global Func_020024ac
	.thumb_func
Func_020024ac:
	push {r5, r6, r7, lr}
	sub sp, #8
	bl Func_02004f00
	bl Actor_GetById
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	lsls r3, r6, #16
	adds r1, r3, r5
	ldr r3, [r0, #36]
	cmp r3, #0
	bne .L_0200a53c
	ldr r3, [r0, #44]
	cmp r3, #0
	bne .L_0200a53c
	ldr r2, .L_0200a540
	movs r0, #1
	ldr r3, [r2]
	negs r0, r0
	cmp r3, r0
	beq .L_0200a53c
	cmp r1, r3
	beq .L_0200a53c
	str r0, [r2]
	ldr r1, .L_0200a544
	movs r5, #255
	lsls r5, r5, #8
	adds r5, #255
	ldr r0, [r1]
	asrs r6, r3, #16
	ldr r1, .L_0200a548
	ands r5, r3
	ldr r3, .L_0200a54c
	ldr r1, [r1]
	ldr r2, [r3]
	ldr r3, .L_0200a550
	ldr r3, [r3]
	str r0, [sp, #0]
	str r1, [sp, #4]
	movs r0, #73
	movs r1, #72
	bl Func_02004cf0
	ldr r3, .L_0200a554
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a558
	cmp r2, r3
	bne .L_0200a53c
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #2
	adds r1, r6, #0
	adds r2, r5, #0
	ldr r7, [r3, #108]
	bl Func_02000658
	asrs r0, r0, #8
	cmp r0, #0
	beq .L_0200a53c
	movs r1, #181
	adds r3, r0, #0
	lsls r1, r1, #1
	adds r3, #200
	adds r2, r7, r1
	strh r3, [r2]
.L_0200a53c:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_0200a540:
	.4byte gData_02006710
.L_0200a544:
	.4byte Data_020067f4
.L_0200a548:
	.4byte Data_020067f8
.L_0200a54c:
	.4byte Data_020067ec
.L_0200a550:
	.4byte Data_020067f0
.L_0200a554:
	.4byte gSequenceParentWork
.L_0200a558:
	.4byte 0x000000e9
	.section .text.x0200a6a4,"ax",%progbits
	.global Func_020026a4
	.thumb_func
Func_020026a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r1
	adds r6, r0, #0
	mov r2, r8
	movs r0, #144
	lsls r3, r2, #16
	lsls r1, r6, #16
	movs r2, #0
	lsls r0, r0, #1
	sub sp, #8
	bl Import_ActorCreate
	ldr r3, .L_0200a6f8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200a6fc
	subs r2, #2
	strh r3, [r2]
	adds r7, r0, #0
	movs r1, #0
	bl Scene_SetActorSpritePriority
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r1, [r7, #80]
	movs r3, #13
	ldrb r2, [r1, #5]
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #5]
	bl Func_02004d40
	movs r0, #0
	b .L_0200a700
.L_0200a6f8:
	.4byte 0x00000010
.L_0200a6fc:
	.4byte 0x00003f44
.L_0200a700:
	bl Func_02004e90
	ldr r3, .L_0200a73c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #74
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200a740
	movs r0, #106
	orrs r3, r2
	strh r3, [r1]
	bl Func_02004f30
	movs r5, #0
.L_0200a722:
	ldr r1, .L_0200a744
	movs r3, #128
	lsls r2, r5, #9
	lsls r3, r3, #19
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
	movs r0, #1
	adds r5, #1
	bl wait
	b .L_0200a748
	.2byte 0x0000
.L_0200a73c:
	.4byte 0x00003f1f
.L_0200a740:
	.4byte 0x00008000
.L_0200a744:
	.4byte 0x00000010
.L_0200a748:
	cmp r5, #8
	bne .L_0200a722
	movs r5, #0
.L_0200a74e:
	ldr r2, .L_0200a76c
	ldr r1, .L_0200a770
	movs r3, #128
	subs r2, r2, r5
	lsls r3, r3, #19
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #16
	bne .L_0200a74e
	b .L_0200a774
.L_0200a76c:
	.4byte 0x00000010
.L_0200a770:
	.4byte 0x00001000
.L_0200a774:
	mov r3, r8
	asrs r6, r6, #4
	asrs r5, r3, #4
	subs r5, #1
	adds r2, r6, #0
	movs r3, #1
	movs r1, #2
	str r3, [sp, #0]
	str r1, [sp, #4]
	adds r3, r5, #0
	adds r2, #64
	movs r0, #65
	movs r1, #1
	bl Func_02004cd8
	movs r3, #255
	adds r1, r6, #0
	adds r2, r5, #0
	lsls r3, r3, #8
	movs r0, #0
	bl Func_02000694
	adds r0, r7, #0
	bl Func_02004cb0
	bl Func_02004d48
	ldr r3, .L_0200a7c8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	add sp, #8
	b .L_0200a7cc
.L_0200a7c8:
	.4byte 0x00000000
.L_0200a7cc:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a7d4,"ax",%progbits
	.global Func_020027d4
	.thumb_func
Func_020027d4:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_0200a864
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	bl Func_02000658
	movs r3, #181
	lsls r3, r3, #1
	adds r5, r5, r3
	movs r2, #0
	ldrsh r3, [r5, r2]
	movs r6, #54
	subs r3, #201
	lsls r3, r3, #2
	asrs r0, r0, #8
	subs r6, r6, r3
	cmp r0, #1
	beq .L_0200a848
	cmp r0, #7
	beq .L_0200a848
	bl Func_0200257c
	movs r0, #132
	lsls r1, r6, #3
	lsls r0, r0, #1
	adds r1, #10
	bl Func_020026a4
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a838
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200a838:
	movs r3, #0
	ldrsh r0, [r5, r3]
	movs r2, #156
	lsls r2, r2, #1
	adds r0, r0, r2
	bl GameFlag_SetBit
	b .L_0200a860
.L_0200a848:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a860
	movs r0, #132
	lsls r1, r6, #3
	lsls r0, r0, #1
	adds r1, #10
	bl Func_020026a4
.L_0200a860:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a864:
	.4byte gSequenceParentWork
	.section .text.x0200a8ac,"ax",%progbits
	.balign 4
	.global Func_020028ac
	.thumb_func
Func_020028ac:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #211
	lsls r2, r2, #4
	adds r3, r3, r2
	movs r0, #0
	ldrsb r0, [r3, r0]
	bl Actor_GetById
	movs r1, #0
	adds r5, r0, #0
	bl Actor_SetPartAttribute
	movs r0, #136
	bl Func_02004f30
	movs r6, #0
.L_0200a8d2:
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #5
	adds r6, #1
	bl wait
	cmp r6, #16
	bne .L_0200a8d2
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_SetBit
	movs r0, #9
	bl Actor_GetById
	adds r5, r0, #0
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02004d10
	movs r0, #141
	bl Func_02004f30
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #240
	movs r1, #1
	movs r2, #172
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #15
	bl Camera_MoveWithinMapBounds
	bl Func_02004e20
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #151
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004f30
	ldr r3, .L_0200a984
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200a988
	subs r2, #2
	strh r3, [r2]
	ldr r2, [r5, #16]
	movs r3, #192
	lsls r3, r3, #14
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	ldr r0, [r5, #8]
	ldr r1, [r5, #20]
	bl OverlayObject_PrepareObject
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r7, r0, #0
	movs r6, #0
.L_0200a978:
	ldr r3, [r5, #12]
	ldr r2, .L_0200a98c
	movs r0, #5
	adds r3, r3, r2
	str r3, [r5, #12]
	b .L_0200a990
.L_0200a984:
	.4byte 0x00001000
.L_0200a988:
	.4byte 0x00003f10
.L_0200a98c:
	.4byte 0xffff0000
.L_0200a990:
	adds r6, #1
	bl FieldEvent_Wait
	cmp r6, #32
	bne .L_0200a978
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #42
	bl Func_02004f30
	movs r1, #4
	adds r0, r5, #0
	bl Func_02004c98
	adds r0, r7, #0
	bl Func_02004cb0
	movs r0, #1
	bl FieldEvent_Wait
	ldr r3, .L_0200a9d4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r1, #7
	movs r2, #21
	movs r3, #128
	movs r0, #0
	bl Func_02000694
	bl Func_02004d48
	b .L_0200a9d8
.L_0200a9d4:
	.4byte 0x00000000
.L_0200a9d8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200aa6c,"ax",%progbits
	.balign 4
	.global Func_02002a6c
	.thumb_func
Func_02002a6c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	movs r2, #102
	adds r2, r2, r6
	adds r5, r6, #0
	mov r10, r2
	ldrh r2, [r2]
	adds r5, #100
	ldrh r3, [r5]
	lsls r2, r2, #16
	ldr r7, [r6, #104]
	asrs r2, r2, #17
	adds r3, r3, r2
	strh r3, [r5]
	ldr r3, [r7, #8]
	movs r2, #128
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	movs r2, #8
	str r3, [r6, #16]
	adds r2, r2, r6
	movs r3, #0
	ldrsh r1, [r5, r3]
	ldr r0, [r6, #76]
	mov r8, r2
	bl Vector_AddPolarOffsetFar
	adds r2, r6, #0
	adds r2, #98
	ldrb r3, [r2]
	movs r0, #0
	adds r3, #255
	strb r3, [r2]
	lsls r3, r3, #24
	cmp r3, #0
	beq .L_0200ab1c
	ldr r3, [r6, #76]
	movs r2, #128
	lsls r2, r2, #10
	movs r0, #1
	cmp r3, r2
	beq .L_0200ab1c
	adds r0, r6, #0
	bl Func_02002a14
	ldr r2, .L_0200ab28
	ldr r3, [r6, #76]
	mov r9, r2
	add r3, r9
	str r3, [r6, #76]
	mov r3, r10
	ldrh r2, [r3]
	ldrh r3, [r5]
	lsls r2, r2, #16
	asrs r2, r2, #17
	adds r3, r3, r2
	strh r3, [r5]
	ldr r3, [r7, #8]
	mov r2, r8
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	ldr r0, [r6, #76]
	str r3, [r6, #16]
	mov r2, r8
	movs r3, #0
	ldrsh r1, [r5, r3]
	bl Vector_AddPolarOffsetFar
	adds r0, r6, #0
	bl Func_02002a14
	ldr r3, [r6, #76]
	movs r0, #1
	add r3, r9
	str r3, [r6, #76]
.L_0200ab1c:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ab28:
	.4byte 0xfffc0000
	.section .text.x0200ab2c,"ax",%progbits
	.global Func_02002b2c
	.thumb_func
Func_02002b2c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #102
	adds r0, r0, r7
	mov r8, r0
	adds r5, r7, #0
	adds r5, #100
	mov r1, r8
	ldrh r3, [r1]
	ldrh r0, [r5]
	adds r0, r0, r3
	strh r0, [r5]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Math_Cosine
	ldr r1, [r7, #76]
	ldr r6, .L_0200abc8
	mov lr, r6
	.2byte 0xf800
	str r0, [r7, #8]
	movs r2, #0
	ldrsh r0, [r5, r2]
	bl Math_Sine
	ldr r1, [r7, #76]
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r7, #8]
	ldr r2, [r7, #68]
	asrs r0, r0, #1
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [r7, #72]
	subs r5, #1
	adds r0, r0, r3
	str r0, [r7, #16]
	ldrb r3, [r5]
	cmp r3, #141
	beq .L_0200aba4
	ldr r3, .L_0200abcc
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200aba4
	adds r3, r7, #0
	adds r3, #98
	ldrb r0, [r3]
	lsls r0, r0, #10
	bl Math_Sine
	ldrb r3, [r5]
	muls r3, r0
	str r3, [r7, #76]
	ldrb r3, [r5]
	adds r3, #10
	strb r3, [r5]
.L_0200aba4:
	mov r3, r8
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r0, #0
	cmp r3, #0
	beq .L_0200abc2
	ldrb r3, [r5]
	cmp r3, #141
	bne .L_0200abc0
	adds r3, r2, #0
	subs r3, #128
	mov r1, r8
	strh r3, [r1]
.L_0200abc0:
	movs r0, #1
.L_0200abc2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200abc8:
	.4byte IwramMulQ16
.L_0200abcc:
	.4byte gFrameCount
	.section .text.x0200acd0,"ax",%progbits
	.global Func_02002cd0
	.thumb_func
Func_02002cd0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	mov r11, r1
	mov r9, r0
	bl Actor_GetById
	adds r7, r0, #0
	mov r0, r11
	bl Actor_GetById
	mov r10, r0
	movs r0, #78
	bl Func_02004f30
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl Func_02004e10
	movs r2, #10
	ldrsh r0, [r7, r2]
	movs r3, #14
	ldrsh r1, [r7, r3]
	movs r3, #18
	ldrsh r2, [r7, r3]
	lsls r1, r1, #16
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	movs r0, #141
	bl Func_02004f30
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_02004d10
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02004e48
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02004e40
	movs r0, #60
	bl Func_02004e50
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r7, #52]
	str r3, [r7, #48]
	ldr r1, .L_0200ada4
	mov r0, r9
	bl Actor_SetMovementTypeById
	movs r0, #194
	bl Func_02004f30
	movs r0, #45
	bl FieldEvent_Wait
	movs r1, #128
	mov r0, r9
	lsls r1, r1, #1
	bl Func_02004dc0
	ldr r3, .L_0200ad9c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200ada0
	subs r2, #2
	strh r3, [r2]
	movs r0, #0
	mov r8, r0
	b .L_0200ada8
	.2byte 0x0000
.L_0200ad9c:
	.4byte 0x00001008
.L_0200ada0:
	.4byte 0x00003f10
.L_0200ada4:
	.4byte gData_02005590
.L_0200ada8:
	movs r0, #168
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	lsls r0, r0, #2
	bl Import_ActorCreate
	adds r5, r0, #0
	movs r0, #246
	bl Func_02004f30
	bl Random16Far
	movs r3, #31
	ldr r2, [r5, #8]
	ands r3, r0
	subs r3, #16
	lsls r3, r3, #16
	adds r2, r2, r3
	str r2, [r5, #8]
	bl Random16Far
	movs r3, #15
	ldr r2, [r5, #12]
	ands r3, r0
	subs r3, #8
	lsls r3, r3, #16
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #48]
	movs r3, #204
	lsls r3, r3, #6
	str r2, [r5, #12]
	adds r3, #51
	adds r2, r5, #0
	adds r2, #85
	str r3, [r5, #52]
	movs r3, #0
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #10
	bl Actor_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Scene_SetActorSpritePriority
	adds r0, r5, #0
	movs r1, #7
	bl Func_02004c98
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	ldr r1, .L_0200ae68
	adds r0, r5, #0
	bl Func_02004ca0
	movs r6, #128
	ldr r3, .L_0200ae5c
	ldr r5, .L_0200ae60
	lsls r6, r6, #19
	adds r6, #82
	strh r3, [r6]
	movs r0, #2
	bl wait
	movs r0, #2
	strh r5, [r6]
	bl wait
	ldr r3, .L_0200ae64
	movs r0, #2
	strh r3, [r6]
	bl wait
	strh r5, [r6]
	movs r0, #2
	bl wait
	movs r2, #1
	add r8, r2
	mov r3, r8
	b .L_0200ae6c
.L_0200ae5c:
	.4byte 0x00001004
.L_0200ae60:
	.4byte 0x0000100a
.L_0200ae64:
	.4byte 0x00001010
.L_0200ae68:
	.4byte gData_02005544
.L_0200ae6c:
	cmp r3, #16
	bne .L_0200ada8
	ldr r3, .L_0200aeac
	movs r0, #30
	strh r3, [r6]
	bl wait
	movs r0, #0
	mov r8, r0
.L_0200ae7e:
	mov r0, r10
	ldr r3, [r0, #16]
	mov r2, r10
	movs r0, #168
	ldr r1, [r2, #8]
	lsls r0, r0, #2
	ldr r2, [r2, #12]
	bl Import_ActorCreate
	adds r5, r0, #0
	movs r0, #195
	bl Func_02004f30
	bl Random16Far
	ldr r3, .L_0200aeb0
	movs r2, #128
	ands r0, r3
	lsls r2, r2, #8
	adds r6, r5, #0
	adds r0, r0, r2
	adds r6, #100
	b .L_0200aeb4
.L_0200aeac:
	.4byte 0x00001008
.L_0200aeb0:
	.4byte 0x00007fff
.L_0200aeb4:
	strh r0, [r6]
	bl Random16Far
	mov r3, r8
	movs r2, #1
	ands r2, r3
	movs r3, #3
	ands r3, r0
	lsls r2, r2, #1
	adds r3, #9
	subs r2, #1
	lsls r2, r3
	ldr r7, .L_0200af08
	adds r3, r5, #0
	adds r3, #102
	strh r2, [r3]
	subs r3, #17
	strb r7, [r3]
	movs r3, #244
	lsls r3, r3, #15
	str r3, [r5, #76]
	mov r2, r8
	movs r3, #16
	subs r3, r3, r2
	lsls r3, r3, #3
	adds r2, r5, #0
	adds r3, #15
	adds r2, #98
	mov r0, r10
	str r0, [r5, #104]
	movs r1, #10
	strb r3, [r2]
	adds r0, r5, #0
	bl Actor_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Scene_SetActorSpritePriority
	adds r0, r5, #0
	movs r1, #7
	b .L_0200af0c
.L_0200af08:
	.4byte 0x00000000
.L_0200af0c:
	bl Func_02004c98
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	adds r2, r5, #0
	movs r3, #0
	ldrsh r1, [r6, r3]
	ldr r0, [r5, #76]
	adds r2, #8
	bl Vector_AddPolarOffsetFar
	adds r0, r5, #0
	ldr r1, .L_0200b004
	bl Func_02004ca0
	mov r0, r8
	cmp r0, #3
	bne .L_0200af60
	movs r1, #128
	mov r0, r11
	lsls r1, r1, #1
	bl Func_02004dc0
	mov r3, r10
	adds r3, #85
	strb r7, [r3]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	mov r2, r10
	str r3, [r2, #52]
	str r3, [r2, #48]
	mov r0, r11
	ldr r1, .L_0200b008
	bl Actor_SetMovementTypeById
.L_0200af60:
	movs r0, #8
	bl wait
	movs r3, #1
	add r8, r3
	mov r0, r8
	cmp r0, #16
	beq .L_0200af72
	b .L_0200ae7e
.L_0200af72:
	movs r0, #220
	bl Func_02004f30
	movs r0, #16
	bl wait
	movs r2, #2
	mov r8, r2
.L_0200af82:
	mov r3, r10
	ldr r1, [r3, #8]
	ldr r3, [r3, #12]
	mov r0, r8
	lsls r2, r0, #16
	adds r2, r2, r3
	ldr r3, .L_0200b00c
	mov r0, r10
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Import_ActorCreate
	adds r5, r0, #0
	bl Random16Far
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	adds r2, r5, #0
	movs r3, #128
	adds r2, #102
	lsls r3, r3, #4
	strh r3, [r2]
	adds r3, r5, #0
	mov r2, r8
	adds r3, #98
	strb r2, [r3]
	movs r2, #1
	adds r3, #1
	strb r2, [r3]
	ldr r1, .L_0200b000
	ldr r3, [r5, #8]
	movs r6, #0
	str r3, [r5, #68]
	ldr r3, [r5, #16]
	str r6, [r5, #76]
	str r3, [r5, #72]
	mov r3, r10
	str r3, [r5, #104]
	adds r3, r5, #0
	adds r3, #85
	strb r1, [r3]
	subs r3, #50
	strb r2, [r3]
	adds r0, r5, #0
	movs r1, #10
	bl Actor_SetPartAttribute
	adds r0, r5, #0
	movs r1, #7
	bl Func_02004c98
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	b .L_0200b010
.L_0200b000:
	.4byte 0x00000000
.L_0200b004:
	.4byte gData_02005560
.L_0200b008:
	.4byte gData_020055b4
.L_0200b00c:
	.4byte 0xfff80000
.L_0200b010:
	adds r0, r5, #0
	ldr r1, .L_0200b0bc
	bl Func_02004ca0
	movs r0, #2
	add r8, r0
	mov r2, r8
	cmp r2, #32
	bne .L_0200af82
	movs r0, #220
	bl Func_02004f30
	movs r0, #50
	bl wait
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02004e40
	movs r0, #8
	bl Func_02004e50
	movs r0, #16
	bl wait
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_02004d10
	mov r0, r9
	movs r1, #0
	bl Func_02004dc0
	mov r0, r11
	movs r1, #0
	bl Func_02004dc0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	bl Func_02004f30
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004e40
	movs r0, #80
	bl Func_02004e50
	mov r0, r9
	ldr r1, .L_0200b0c0
	bl Actor_SetMovementTypeById
	ldr r1, .L_0200b0c4
	mov r0, r11
	bl Actor_SetMovementTypeById
	ldr r3, .L_0200b0c8
	mov r0, r10
	str r3, [r0, #108]
	movs r0, #120
	bl wait
	mov r2, r10
	movs r0, #195
	str r6, [r2, #108]
	lsls r0, r0, #1
	bl Func_02004f30
	bl Func_02004ea0
	movs r0, #1
	bl wait
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b0bc:
	.4byte gData_02005570
.L_0200b0c0:
	.4byte gData_020055d8
.L_0200b0c4:
	.4byte gData_02005608
.L_0200b0c8:
	.4byte Func_02002c48
	.section .text.x0200b678,"ax",%progbits
	.global Func_02003678
	.thumb_func
Func_02003678:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	ldr r5, .L_0200b700
	str r2, [r3]
	subs r2, #36
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b704
	sub sp, #8
	cmp r2, r3
	bne .L_0200b70c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	movs r5, #192
	orrs r3, r2
	lsls r5, r5, #2
	strb r3, [r0]
	ldr r1, .L_0200b708
	movs r0, #8
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b6d4
	bl .L_0200bec6
.L_0200b6d4:
	ldr r3, .L_0200b6f8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b6fc
	subs r2, #2
	strh r3, [r2]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #71
	movs r1, #64
	movs r2, #47
	movs r3, #98
	bl Func_02004cd8
	b .L_0200bec6
.L_0200b6f8:
	.4byte 0x00001000
.L_0200b6fc:
	.4byte 0x00003f44
.L_0200b700:
	.4byte gSequenceParentWork
.L_0200b704:
	.4byte 0x000000e2
.L_0200b708:
	.4byte gData_02005074
.L_0200b70c:
	ldr r3, .L_0200b8dc
	cmp r2, r3
	bne .L_0200b7ae
	movs r0, #0
	bl Func_02004e60
	movs r0, #15
	bl Actor_GetById
	ldr r3, .L_0200b8e0
	movs r5, #0
	str r3, [r0, #108]
.L_0200b724:
	adds r0, r5, #0
	adds r0, #9
	bl Actor_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r0, #20]
	str r3, [r0, #12]
	cmp r5, #6
	bne .L_0200b724
	movs r0, #159
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b756
	movs r0, #15
	bl Actor_GetById
	movs r3, #132
	lsls r3, r3, #17
	str r3, [r0, #16]
.L_0200b756:
	ldr r0, .L_0200b8e4
	bl Func_02004ed8
	ldr r0, .L_0200b8e8
	bl Func_02000880
	ldr r3, .L_0200b8ec
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #16
	bl Actor_GetById
	adds r2, r0, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r0, #20]
	str r3, [r0, #12]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b7a8
	b .L_0200bb24
.L_0200b7a8:
	bl Func_020034bc
	b .L_0200bec6
.L_0200b7ae:
	ldr r3, .L_0200b8f0
	cmp r2, r3
	bne .L_0200b84c
	movs r0, #0
	bl Func_02004e60
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #241
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b7d4
	movs r0, #11
	bl Actor_GetById
	movs r3, #166
	lsls r3, r3, #18
	str r3, [r0, #16]
.L_0200b7d4:
	ldr r0, .L_0200b8f4
	bl Func_02004ed8
	bl Func_02004ec0
	movs r1, #13
	movs r2, #14
	movs r0, #0
	bl Func_02004ec8
	ldr r0, .L_0200b8e8
	bl Func_02000880
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Actor_GetById
	adds r2, r0, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r0, #20]
	str r3, [r0, #12]
	adds r0, #35
	ldrb r2, [r0]
	movs r5, #192
	movs r3, #128
	lsls r5, r5, #2
	orrs r3, r2
	adds r5, #1
	strb r3, [r0]
	ldr r1, .L_0200b8f8
	movs r0, #10
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #12
	ldr r1, .L_0200b8fc
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b84a
	b .L_0200bb24
.L_0200b84a:
	b .L_0200b8aa
.L_0200b84c:
	ldr r3, .L_0200b900
	cmp r2, r3
	beq .L_0200b854
	b .L_0200ba84
.L_0200b854:
	movs r0, #0
	bl Func_02004e60
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	ldr r0, .L_0200b904
	bl Func_02000880
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r6, #0
	cmp r3, #7
	bgt .L_0200b910
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #1
	ldr r1, .L_0200b908
	movs r0, #10
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #12
	ldr r1, .L_0200b90c
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b8b0
.L_0200b8aa:
	bl Func_020034f0
	b .L_0200bec6
.L_0200b8b0:
	ldr r3, .L_0200b8d4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b8d8
	subs r2, #2
	strh r3, [r2]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #71
	movs r1, #64
	movs r2, #35
	movs r3, #82
	bl Func_02004cd8
	b .L_0200bec6
.L_0200b8d4:
	.4byte 0x00001000
.L_0200b8d8:
	.4byte 0x00003f44
.L_0200b8dc:
	.4byte 0x000000e3
.L_0200b8e0:
	.4byte Func_02003630
.L_0200b8e4:
	.4byte gData_020054fc
.L_0200b8e8:
	.4byte gData_02005500
.L_0200b8ec:
	.4byte gSequenceParentWork
.L_0200b8f0:
	.4byte 0x000000e4
.L_0200b8f4:
	.4byte gData_02005508
.L_0200b8f8:
	.4byte gData_020050d4
.L_0200b8fc:
	.4byte gData_02005134
.L_0200b900:
	.4byte 0x000000e5
.L_0200b904:
	.4byte gData_0200550c
.L_0200b908:
	.4byte gData_0200519c
.L_0200b90c:
	.4byte gData_020051fc
.L_0200b910:
	movs r0, #23
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #24
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #25
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #3
	orrs r3, r5
	strb r3, [r0]
	movs r0, #23
	bl Actor_SetSpritePriority
	movs r0, #24
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r0, #25
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #242
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b96e
	movs r0, #23
	bl Actor_GetById
	movs r3, #172
	lsls r3, r3, #17
	str r3, [r0, #8]
.L_0200b96e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #243
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b988
	movs r0, #24
	bl Actor_GetById
	movs r3, #172
	lsls r3, r3, #17
	str r3, [r0, #8]
.L_0200b988:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #244
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b9a2
	movs r0, #25
	bl Actor_GetById
	movs r3, #172
	lsls r3, r3, #17
	str r3, [r0, #8]
.L_0200b9a2:
	ldr r0, .L_0200bb40
	bl Func_02004ed8
	movs r0, #26
	bl Actor_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #26
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #26
	bl Actor_GetById
	str r6, [r0, #20]
	str r6, [r5, #12]
	movs r0, #15
	bl Actor_GetById
	movs r5, #3
	adds r0, #92
	strb r5, [r0]
	movs r0, #16
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #17
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #18
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #19
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #20
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #21
	bl Actor_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #22
	bl Actor_GetById
	adds r0, #92
	ldr r6, .L_0200bb44
	strb r5, [r0]
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #2
	adds r1, r6, #0
	movs r0, #15
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #16
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200bb48
	movs r0, #17
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #18
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200bb4c
	movs r0, #19
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #20
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200bb50
	movs r0, #21
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #22
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bb24
	bl Func_02003548
	b .L_0200bec6
.L_0200ba84:
	ldr r3, .L_0200bb54
	cmp r2, r3
	bne .L_0200bb58
	movs r0, #0
	bl Func_02004e60
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #10
	bl Func_02004f08
	movs r1, #1
	movs r0, #11
	bl Func_02004f08
	movs r0, #8
	bl Actor_GetById
	movs r1, #2
	mov r8, r1
	adds r3, r0, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r6, #160
	lsls r6, r6, #13
	str r6, [r0, #20]
	str r6, [r0, #12]
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #128
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl Actor_GetById
	adds r3, r0, #0
	adds r3, #85
	mov r1, r8
	strb r1, [r3]
	str r6, [r0, #20]
	str r6, [r0, #12]
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bb24
	movs r1, #22
	movs r2, #26
	movs r3, #0
	movs r0, #0
	bl Func_02000694
	movs r1, #25
	movs r0, #0
	movs r2, #26
	movs r3, #0
	bl Func_02000694
	movs r0, #64
	movs r1, #3
	bl Actor_SetSpritePriority
	b .L_0200bec6
.L_0200bb24:
	ldr r3, .L_0200bb38
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200bb3c
	subs r2, #2
	strh r3, [r2]
	b .L_0200bec6
	.2byte 0x0000
.L_0200bb38:
	.4byte 0x00001000
.L_0200bb3c:
	.4byte 0x00003f44
.L_0200bb40:
	.4byte gData_02005520
.L_0200bb44:
	.4byte gData_0200525c
.L_0200bb48:
	.4byte gData_020052bc
.L_0200bb4c:
	.4byte gData_0200531c
.L_0200bb50:
	.4byte gData_0200537c
.L_0200bb54:
	.4byte 0x000000e6
.L_0200bb58:
	ldr r3, .L_0200bc7c
	cmp r2, r3
	beq .L_0200bb60
	b .L_0200bc90
.L_0200bb60:
	movs r0, #0
	bl Func_02004e60
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #245
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bbaa
	movs r3, #46
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #46
	movs r1, #74
	movs r2, #1
	bl Func_02004ce8
	movs r0, #9
	bl Actor_GetById
	movs r3, #190
	lsls r3, r3, #18
	str r3, [r0, #8]
.L_0200bbaa:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #246
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bbc6
	movs r1, #230
	movs r2, #154
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004da0
.L_0200bbc6:
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #2
	ldr r0, .L_0200bc80
	bl Func_02004ed8
	ldr r1, .L_0200bc84
	movs r0, #8
	adds r2, r5, #0
	bl Func_02000800
	ldr r1, .L_0200bc88
	movs r0, #10
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #11
	ldr r1, .L_0200bc8c
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bc4e
	movs r0, #9
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #246
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bc18
	movs r0, #12
	movs r1, #4
	bl Actor_SetModeById
.L_0200bc18:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #245
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bc34
	movs r3, #255
	lsls r3, r3, #8
	movs r0, #2
	movs r1, #46
	movs r2, #8
	bl Func_02000694
.L_0200bc34:
	movs r1, #10
	movs r2, #45
	movs r3, #0
	movs r0, #0
	bl Func_02000694
	movs r0, #0
	movs r1, #53
	movs r2, #40
	movs r3, #0
	bl Func_02000694
	b .L_0200bec6
.L_0200bc4e:
	ldr r3, .L_0200bc74
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200bc78
	subs r2, #2
	strh r3, [r2]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #71
	movs r1, #64
	movs r2, #50
	movs r3, #102
	bl Func_02004cd8
	b .L_0200bec6
	.2byte 0x0000
.L_0200bc74:
	.4byte 0x00001000
.L_0200bc78:
	.4byte 0x00003f44
.L_0200bc7c:
	.4byte 0x000000e7
.L_0200bc80:
	.4byte gData_02005528
.L_0200bc84:
	.4byte gData_020053dc
.L_0200bc88:
	.4byte gData_0200543c
.L_0200bc8c:
	.4byte gData_0200549c
.L_0200bc90:
	ldr r3, .L_0200bed0
	cmp r2, r3
	beq .L_0200bc98
	b .L_0200bdd4
.L_0200bc98:
	movs r6, #0
.L_0200bc9a:
	adds r0, r6, #0
	adds r0, #8
	bl Actor_GetById
	movs r1, #4
	adds r5, r0, #0
	ldr r7, .L_0200bed4
	bl Func_02004c98
	movs r1, #0
	adds r3, r5, #0
	mov r8, r1
	adds r3, #85
	mov r2, r8
	adds r6, #1
	strb r2, [r3]
	str r7, [r5, #20]
	str r7, [r5, #12]
	cmp r6, #3
	bne .L_0200bc9a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #247
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bce6
	movs r0, #23
	movs r1, #4
	bl Actor_SetModeById
	movs r1, #174
	movs r2, #162
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004da0
.L_0200bce6:
	ldr r0, .L_0200bed8
	bl Func_02004ed8
	movs r0, #0
	bl Func_02004e60
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bd0a
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200bd0a:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bd20
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200bd20:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bd62
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bd62
	movs r0, #11
	bl Actor_GetById
	mov r3, r8
	adds r0, #85
	strb r3, [r0]
	movs r0, #11
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #11
	bl Actor_GetById
	movs r1, #4
	str r7, [r0, #20]
	str r7, [r5, #12]
	movs r0, #11
	bl Actor_SetModeById
	b .L_0200bd70
.L_0200bd62:
	movs r3, #255
	lsls r3, r3, #8
	movs r0, #0
	movs r1, #38
	movs r2, #18
	bl Func_02000694
.L_0200bd70:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #249
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bd86
	movs r0, #12
	movs r1, #0
	bl Actor_SetModeById
.L_0200bd86:
	movs r6, #0
.L_0200bd88:
	adds r0, r6, #0
	adds r0, #13
	movs r1, #2
	adds r6, #1
	bl Actor_SetModeById
	cmp r6, #4
	bne .L_0200bd88
	movs r6, #0
.L_0200bd9a:
	adds r0, r6, #0
	adds r0, #12
	bl Actor_GetById
	adds r6, #1
	movs r1, #0
	bl Actor_SetSingleChildFlags
	cmp r6, #11
	bne .L_0200bd9a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #248
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bdbe
	b .L_0200bec6
.L_0200bdbe:
	movs r6, #0
.L_0200bdc0:
	adds r0, r6, #0
	adds r0, #12
	movs r1, #0
	movs r2, #0
	adds r6, #1
	bl Func_02004da0
	cmp r6, #9
	bne .L_0200bdc0
	b .L_0200bec6
.L_0200bdd4:
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200bedc
	cmp r2, r3
	bne .L_0200bec6
	bl Func_0200255c
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r6, #0
.L_0200bdf2:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #2
	adds r0, r6, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200be2a
	lsls r3, r6, #1
	movs r5, #24
	subs r5, r5, r3
	movs r2, #2
	movs r3, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #65
	movs r1, #1
	movs r2, #80
	adds r3, r5, #0
	bl Func_02004cd8
	movs r3, #255
	movs r0, #0
	movs r1, #16
	adds r2, r5, #0
	lsls r3, r3, #8
	bl Func_02000694
.L_0200be2a:
	adds r6, #1
	cmp r6, #6
	bne .L_0200bdf2
	movs r0, #10
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #11
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200be80
	movs r0, #9
	bl Actor_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #9
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #9
	bl Actor_GetById
	ldr r3, .L_0200bed4
	movs r1, #4
	str r3, [r0, #20]
	str r3, [r5, #12]
	movs r0, #9
	bl Actor_SetModeById
	b .L_0200be98
.L_0200be80:
	movs r3, #255
	movs r1, #7
	movs r2, #21
	lsls r3, r3, #8
	movs r0, #0
	bl Func_02000694
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200be98:
	movs r0, #8
	bl Actor_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #8
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r0, #20]
	str r3, [r5, #12]
	movs r0, #8
	bl Actor_GetById
	movs r1, #9
	bl Actor_SetActionCallback
.L_0200bec6:
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200bed0:
	.4byte 0x000000e8
.L_0200bed4:
	.4byte 0xffe00000
.L_0200bed8:
	.4byte gData_0200552e
.L_0200bedc:
	.4byte 0x000000e9
	.section .text.x0200bf44,"ax",%progbits
	.balign 4
	.global Func_02003f44
	.thumb_func
Func_02003f44:
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
	beq .L_0200c02a
.L_0200bf6a:
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
	ldr r2, .L_0200c060
	adds r3, r5, r2
	ldr r2, .L_0200c064
	asrs r3, r3, #2
	adds r6, r3, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bfb8
	ldr r0, [sp, #12]
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	b .L_0200c018
.L_0200bfb8:
	adds r0, r7, #0
	bl Func_02004ed0
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r5, r5, r3
	str r5, [sp, #4]
	mov r2, r9
	ldrb r0, [r2]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Func_02004d20
	mov r3, r9
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r10, r0
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	ldr r0, [sp, #12]
	bl Func_02004df8
	ldr r2, [sp, #4]
	movs r3, #128
	asrs r5, r5, #19
	strb r3, [r2, #3]
	adds r5, #4
	mov r3, r9
	adds r2, r5, #0
	ldrb r0, [r3]
	mov r1, r10
	bl Func_02004ef0
	add r8, r6
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c018
	adds r0, r7, #0
	movs r1, #0
	bl Func_02004c98
.L_0200c018:
	movs r3, #4
	add r11, r3
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200bf6a
.L_0200c02a:
	ldr r3, .L_0200c068
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
	bge .L_0200c052
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_0200c052:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c060:
	.4byte 0xfdff0000
.L_0200c064:
	.4byte gMapShapeGrid
.L_0200c068:
	.4byte gSequenceParentWork
	.section .text.x0200c06c,"ax",%progbits
	.global Func_0200406c
	.thumb_func
Func_0200406c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #12
	adds r5, r0, #0
	bl Func_02004e38
	cmp r0, #0
	beq .L_0200c082
	b .L_0200c1f2
.L_0200c082:
	ldr r3, .L_0200c1fc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	mov r0, sp
	str r3, [r0]
	movs r1, #0
	ldr r3, [r6, #12]
	str r3, [r0, #4]
	ldr r3, [r6, #16]
	str r3, [r0, #8]
	bl Func_02004ef8
	mov r8, r0
	cmp r0, #0
	bne .L_0200c0ae
	b .L_0200c1f2
.L_0200c0ae:
	b .L_0200c1e4
.L_0200c0b0:
	ldrh r7, [r5]
	adds r0, r7, #0
	bl Actor_GetById
	cmp r0, r8
	beq .L_0200c0c0
	adds r5, #4
	b .L_0200c1e4
.L_0200c0c0:
	ldrh r5, [r5, #2]
	bl Func_02004d40
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c12e
	movs r0, #125
	bl Func_02004f30
	adds r0, r7, #0
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #2
	bl wait
	movs r1, #0
	mov r0, r8
	bl Func_02004c98
	adds r0, r7, #0
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r0, #2
	bl wait
	adds r0, r7, #0
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #4
	bl wait
	adds r0, r7, #0
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r0, #0
	bl Func_020046e8
	adds r0, r5, #0
	bl GameFlag_SetBit
	b .L_0200c1de
.L_0200c12e:
	adds r5, #1
	mov r10, r5
	mov r0, r10
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c1de
	adds r6, #85
	strb r0, [r6]
	movs r0, #185
	bl Func_02004f30
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02004d10
	movs r0, #0
	bl Func_020046e8
	movs r5, #2
	movs r0, #8
	mov r7, r8
	bl wait
	negs r5, r5
	mov r0, r8
	movs r1, #2
	adds r7, #34
	bl Func_02004c98
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_020049c0
	movs r0, #1
	bl Func_020046e8
	movs r0, #16
	bl wait
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_020049c0
	movs r0, #4
	bl wait
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004d10
	movs r0, #8
	bl wait
	movs r3, #3
	strb r3, [r6]
	movs r0, #5
	bl wait
	movs r1, #0
	movs r2, #0
	movs r3, #0
	mov r0, r8
	bl Actor_SetPositionAndResetMotion
	movs r0, #2
	bl wait
	movs r0, #188
	bl Func_02004f30
	bl Func_0200481c
	movs r0, #20
	bl wait
	mov r0, r10
	bl GameFlag_SetBit
.L_0200c1de:
	bl Func_02004d48
	b .L_0200c1f2
.L_0200c1e4:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200c1f2
	b .L_0200c0b0
.L_0200c1f2:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c1fc:
	.4byte gSequenceParentWork
	.section .text.x0200c200,"ax",%progbits
	.global Func_02004200
	.thumb_func
Func_02004200:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	adds r5, r0, #0
	adds r0, r6, #0
	bl Actor_GetById
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	adds r7, r0, #0
	cmp r3, r2
	beq .L_0200c2da
.L_0200c21e:
	ldrh r3, [r5]
	cmp r3, r6
	beq .L_0200c228
	adds r5, #4
	b .L_0200c2ce
.L_0200c228:
	ldrh r5, [r5, #2]
	bl Func_02004d40
	adds r3, r5, #1
	mov r8, r3
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c2c8
	movs r0, #185
	bl Func_02004f30
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02004d10
	movs r0, #0
	bl Func_020046e8
	movs r0, #8
	bl wait
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004c98
	adds r3, r7, #0
	adds r3, #34
	movs r0, #4
	ldrb r1, [r3]
	adds r2, r6, #0
	negs r0, r0
	bl Func_02004934
	movs r0, #1
	bl Func_020046e8
	movs r0, #16
	bl wait
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004d10
	movs r0, #8
	bl wait
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, r7, #0
	bl Actor_SetPositionAndResetMotion
	movs r0, #2
	bl wait
	movs r0, #188
	bl Func_02004f30
	bl Func_0200481c
	movs r0, #20
	bl wait
	adds r0, r5, #0
	bl GameFlag_SetBit
	mov r0, r8
	bl GameFlag_SetBit
.L_0200c2c8:
	bl Func_02004d48
	b .L_0200c2da
.L_0200c2ce:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c21e
.L_0200c2da:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200c2f8,"ax",%progbits
	.balign 4
	.global Func_020042f8
	.thumb_func
Func_020042f8:
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
	ldr r5, .L_0200c454
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_02004da8
	ldr r1, [r5]
	movs r0, #9
	bl Func_02004da8
	ldr r1, [r5]
	movs r0, #10
	bl Func_02004da8
	movs r0, #1
	bl wait
	movs r0, #8
	bl Func_020042e0
	movs r0, #9
	bl Func_020042e0
	movs r0, #10
	bl Func_020042e0
	movs r1, #0
	movs r0, #9
	bl Actor_SetModeById
	movs r0, #1
	bl wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_02004da0
	movs r0, #1
	bl wait
	b .L_0200c432
.L_0200c37e:
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
	ldr r2, .L_0200c458
	adds r3, r6, r2
	ldr r2, .L_0200c45c
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c3d4
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	b .L_0200c42e
.L_0200c3d4:
	adds r0, r5, #0
	bl Func_02004ed0
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02004d20
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_02004df8
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02004ef0
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c42e
	mov r0, r9
	movs r1, #9
	bl ObjectVisual_CopyAttributes
.L_0200c42e:
	movs r3, #4
	add r11, r3
.L_0200c432:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c37e
	movs r0, #10
	bl wait
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c454:
	.4byte gSequenceParentWork
.L_0200c458:
	.4byte 0xfdff0000
.L_0200c45c:
	.4byte gMapShapeGrid
	.section .text.x0200c47c,"ax",%progbits
	.balign 4
	.global Func_0200447c
	.thumb_func
Func_0200447c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #12
	adds r6, r0, #0
	bl Func_02004e38
	cmp r0, #0
	beq .L_0200c494
	b .L_0200c656
.L_0200c494:
	ldr r3, .L_0200c664
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Actor_GetById
	ldr r3, [r5, #8]
	adds r7, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r1, #0
	ldr r3, [r5, #12]
	str r3, [r0, #4]
	ldr r3, [r5, #16]
	str r3, [r0, #8]
	bl Func_02004ef8
	mov r10, r0
	cmp r0, #0
	bne .L_0200c4c8
	b .L_0200c656
.L_0200c4c8:
	b .L_0200c648
.L_0200c4ca:
	ldrh r3, [r6]
	mov r8, r3
	mov r0, r8
	bl Actor_GetById
	cmp r0, r10
	beq .L_0200c4dc
	adds r6, #4
	b .L_0200c648
.L_0200c4dc:
	ldrh r6, [r6, #2]
	bl Func_02004d40
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c576
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004c98
	mov r1, r10
	adds r0, r7, #0
	bl Func_02004460
	movs r0, #1
	bl wait
	movs r0, #125
	bl Func_02004f30
	movs r0, #8
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #2
	bl wait
	adds r0, r7, #0
	movs r1, #0
	bl Func_02004c98
	movs r1, #9
	mov r0, r8
	bl ObjectVisual_CopyAttributes
	movs r0, #8
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r0, #2
	bl wait
	movs r0, #8
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #4
	bl wait
	movs r0, #8
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r0, #0
	bl Func_020046e8
	mov r0, r10
	adds r1, r7, #0
	bl Func_02004460
	movs r0, #1
	bl wait
	adds r0, r6, #0
	bl GameFlag_SetBit
	b .L_0200c642
.L_0200c576:
	adds r6, #1
	mov r9, r6
	mov r0, r9
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200c642
	adds r0, r7, #0
	movs r1, #0
	bl Func_02004c98
	mov r1, r10
	adds r0, r7, #0
	bl Func_02004460
	adds r5, #85
	movs r0, #1
	bl wait
	strb r6, [r5]
	movs r0, #185
	bl Func_02004f30
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02004d10
	movs r0, #0
	bl Func_020046e8
	mov r8, r5
	movs r0, #8
	movs r6, #2
	mov r5, r10
	bl wait
	negs r6, r6
	adds r0, r7, #0
	movs r1, #2
	adds r5, #34
	bl Func_02004c98
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_020049c0
	movs r0, #1
	bl Func_020046e8
	movs r0, #16
	bl wait
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_020049c0
	movs r0, #4
	bl wait
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004d10
	movs r0, #8
	bl wait
	movs r3, #3
	mov r2, r8
	strb r3, [r2]
	movs r0, #5
	bl wait
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, r7, #0
	bl Actor_SetPositionAndResetMotion
	movs r0, #2
	bl wait
	movs r0, #188
	bl Func_02004f30
	bl Func_0200481c
	movs r0, #20
	bl wait
	mov r0, r9
	bl GameFlag_SetBit
.L_0200c642:
	bl Func_02004d48
	b .L_0200c656
.L_0200c648:
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200c656
	b .L_0200c4ca
.L_0200c656:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c664:
	.4byte gSequenceParentWork
	.section .text.x0200c6e8,"ax",%progbits
	.global Func_020046e8
	.thumb_func
Func_020046e8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200c7f4
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
.L_0200c70a:
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
	ldr r0, .L_0200c7f8
	adds r1, r1, r3
	ldr r3, .L_0200c7fc
	adds r1, r1, r0
	movs r0, #30
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r7, #16]
	bl Import_ActorCreate
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200c7de
	mov r1, r9
	ldr r0, [r6, #80]
	bl Func_02004e98
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
	bl Func_02004c98
	adds r0, r6, #0
	ldr r1, .L_0200c800
	bl Func_02004ca0
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
	beq .L_0200c7a8
	mov r3, r10
	lsls r5, r3, #13
	adds r0, r5, #0
	bl Math_Cosine
	ldr r3, .L_0200c804
	ldr r1, .L_0200c808
	mov lr, r3
	.2byte 0xf800
	str r0, [r6, #68]
	adds r0, r5, #0
	bl Math_Sine
	b .L_0200c7ac
.L_0200c7a8:
	mov r0, r8
	str r0, [r6, #68]
.L_0200c7ac:
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
	ldr r3, .L_0200c80c
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_0200c810
	str r3, [r6, #48]
	ldr r3, .L_0200c814
	str r3, [r6, #52]
	ldr r3, .L_0200c818
	str r3, [r6, #108]
.L_0200c7de:
	movs r0, #1
	add r10, r0
	mov r2, r10
	cmp r2, #7
	bls .L_0200c70a
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c7f4:
	.4byte gSequenceParentWork
.L_0200c7f8:
	.4byte 0xfff80000
.L_0200c7fc:
	.4byte 0xfffe0000
.L_0200c800:
	.4byte gData_02005638
.L_0200c804:
	.4byte IwramMulQ16
.L_0200c808:
	.4byte 0x00013333
.L_0200c80c:
	.4byte 0xffffff00
.L_0200c810:
	.4byte 0xfffff800
.L_0200c814:
	.4byte 0xfffffa00
.L_0200c818:
	.4byte Effect_AdvanceMotionAtLeaderPriority
	.section .text.x0200c81c,"ax",%progbits
	.global Func_0200481c
	.thumb_func
Func_0200481c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200c91c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200c910
	movs r3, #0
	mov r9, r3
	mov r10, r3
.L_0200c840:
	movs r0, #30
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, #255
	bl Import_ActorCreate
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200c906
	mov r1, r9
	ldr r0, [r7, #80]
	bl Func_02004e98
	movs r4, #0
	mov r8, r4
	adds r3, r7, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	adds r3, #4
	strb r2, [r3]
	movs r1, #0
	mov r9, r0
	adds r0, r7, #0
	bl Actor_SetSingleChildFlags
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004c98
	ldr r1, .L_0200c920
	adds r0, r7, #0
	bl Func_02004ca0
	mov r3, r10
	lsls r5, r3, #12
	adds r0, r5, #0
	bl Math_Cosine
	mov r4, r8
	str r4, [r7, #72]
	str r0, [r7, #68]
	adds r0, r5, #0
	bl Math_Sine
	ldr r3, [r7, #68]
	str r0, [r7, #76]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r7, #68]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r7, #68]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200c924
	adds r2, r2, r3
	str r2, [r7, #68]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r7, #76]
	adds r3, r3, r0
	ldr r4, .L_0200c928
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r2, r2, r3
	adds r2, r2, r4
	str r2, [r7, #76]
	bl Random16Far
	ldr r2, .L_0200c92c
	lsls r0, r0, #12
	lsrs r0, r0, #16
	adds r3, r7, #0
	adds r0, r0, r2
	adds r3, #100
	strh r0, [r3]
	mov r3, r8
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r3, .L_0200c930
	ldr r0, [r7, #80]
	str r3, [r7, #108]
	ldr r3, [r6, #80]
	movs r1, #12
	ldrb r3, [r3, #9]
	movs r4, #13
	ands r1, r3
	ldrb r3, [r0, #9]
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #9]
.L_0200c906:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200c840
.L_0200c910:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c91c:
	.4byte gSequenceParentWork
.L_0200c920:
	.4byte gData_02005668
.L_0200c924:
	.4byte 0xffffa000
.L_0200c928:
	.4byte 0xffffd000
.L_0200c92c:
	.4byte 0xfffff800
.L_0200c930:
	.4byte Effect_AdvanceMotionAtLeaderPriority
	.section .text.x0200c934,"ax",%progbits
	.global Func_02004934
	.thumb_func
Func_02004934:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r10, r0
	adds r0, r2, #0
	adds r5, r1, #0
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	lsls r3, r5, #3
	subs r3, r3, r5
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r3, [r2, r3]
	ldr r2, .L_0200c9b4
	adds r7, r0, #0
	ldr r1, .L_0200c9b8
	adds r3, r3, r2
	adds r5, r7, #0
	asrs r3, r3, #2
	adds r5, #34
	adds r6, r3, r1
	ldr r2, [r7, #16]
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Func_02004d20
	ldr r2, [r7, #16]
	mov r8, r0
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #8]
	asrs r2, r0, #19
	add r2, r10
	cmp r3, #0
	bge .L_0200c98e
	ldr r1, .L_0200c9bc
	adds r3, r3, r1
.L_0200c98e:
	ldr r0, [r7, #16]
	asrs r1, r3, #20
	cmp r0, #0
	bge .L_0200c99a
	ldr r3, .L_0200c9bc
	adds r0, r0, r3
.L_0200c99a:
	asrs r3, r0, #20
	lsls r3, r3, #7
	adds r3, r1, r3
	ldrb r0, [r5]
	mov r1, r8
	adds r6, r6, r3
	bl Func_02004ef0
	strb r0, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c9b4:
	.4byte 0xfdff0000
.L_0200c9b8:
	.4byte gMapShapeGrid
.L_0200c9bc:
	.4byte 0x000fffff
	.section .text.x0200c9d8,"ax",%progbits
	.balign 4
	.global Func_020049d8
	.thumb_func
Func_020049d8:
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
.L_0200c9f6:
	ldr r3, [r5, #24]
	cmp r3, #19
	bhi .L_0200ca44
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
	ldr r3, .L_0200ca38
	ldr r2, .L_0200ca3c
	ands r1, r3
	ldrh r3, [r6, #8]
	adds r0, r6, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	adds r1, r5, #0
	bl Func_02004f18
	adds r0, r5, #0
	movs r1, #63
	ldr r2, .L_0200ca40
	bl Func_02004f20
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	b .L_0200ca44
.L_0200ca38:
	.4byte 0x000003ff
.L_0200ca3c:
	.4byte 0xfffffc00
.L_0200ca40:
	.4byte 0xffff8000
.L_0200ca44:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	adds r6, #40
	adds r5, #28
	cmp r2, #0
	bge .L_0200c9f6
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200cb18,"ax",%progbits
	.balign 4
	.global Func_02004b18
	.thumb_func
Func_02004b18:
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
	ldr r0, .L_0200cbcc
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02004c58
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
.L_0200cb70:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02004f10
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
	bge .L_0200cb70
	movs r1, #176
	lsls r1, r1, #5
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200cbd0
	bl Task_Add
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200cbcc:
	.4byte 0x000001f0
.L_0200cbd0:
	.4byte Func_020049d8
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
