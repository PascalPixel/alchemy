.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815c,"ax",%progbits
	.balign 4
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
	bl Object_GetById
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
	bl Engine_ObjectCreate
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
	bl Func_02001074
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_0200107c
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
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
	bl Object_SetSpritePriority
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
	bl Object_SetPartAttribute
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
	bl Object_SetSpritePriority
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
	bl Func_02001074
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_0200107c
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
	.4byte gPartyState
.L_02008330:
	.4byte Data_02001228
.L_02008334:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_02008338:
	.4byte 0xffff0000
	.section .text.x02008452,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008454,"ax",%progbits
	.balign 4
	.global Func_02000454
	.thumb_func
Func_02000454:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008538
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r8, r0
	ldr r0, [r3]
	bl Object_GetById
	mov r2, r8
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r7, r0, #0
	cmp r3, r2
	beq .L_020084f4
.L_0200847a:
	mov r3, r8
	ldrh r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	movs r1, #0
	str r3, [r5, #68]
	bl ObjectDispatch_SetSingleChildFlags
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	adds r6, r0, #0
	asrs r6, r6, #19
	adds r6, #2
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	adds r3, r6, #0
	bl Func_020010cc
	adds r3, r6, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #2
	bl Func_020010cc
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #0
	bl Func_020003b4
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r3, #128
	orrs r3, r0
	str r0, [r5, #76]
	asrs r2, r2, #20
	asrs r1, r1, #20
	movs r0, #0
	bl Func_020003f0
	movs r2, #2
	add r8, r2
	mov r2, r8
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200847a
.L_020084f4:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008532
	movs r0, #1
	bl WaitFrames
	adds r3, r7, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Map_GetTerrainHeight
	movs r3, #0
	str r3, [r7, #40]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #60]
	ldr r3, .L_02008538
	str r0, [r7, #20]
	str r0, [r7, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Object_AttachWorkTargetToObject
.L_02008532:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008538:
	.4byte gPartyState
	.section .text.x0200853c,"ax",%progbits
	.balign 4
	.global Func_0200053c
	.thumb_func
Func_0200053c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r3, r2
	ldr r6, [r3]
	adds r5, r0, #0
	movs r2, #14
	ldrsh r3, [r6, r2]
	lsls r5, r5, #3
	cmp r3, #31
	bgt .L_02008568
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085ae
.L_02008568:
	movs r1, #142
	movs r2, #134
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #134
	movs r0, #142
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	movs r1, #146
	movs r2, #138
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #138
	movs r0, #146
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	cmp r5, #8
	bne .L_02008638
	b .L_0200862c
.L_020085ae:
	movs r2, #18
	ldrsh r3, [r6, r2]
	movs r2, #134
	lsls r2, r2, #2
	cmp r3, r2
	beq .L_020085c8
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008644
.L_020085c8:
	movs r1, #134
	movs r2, #138
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #138
	movs r0, #134
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	movs r1, #138
	movs r2, #138
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #138
	movs r0, #138
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	movs r1, #142
	movs r2, #138
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #138
	movs r0, #142
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	cmp r5, #6
	bne .L_02008638
.L_0200862c:
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_020086a4
.L_02008638:
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_020086a4
.L_02008644:
	movs r1, #134
	movs r2, #134
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #134
	movs r0, #134
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	movs r1, #138
	movs r2, #134
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #134
	movs r0, #138
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	movs r1, #142
	movs r2, #134
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #134
	movs r0, #142
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
.L_020086a4:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020086a8,"ax",%progbits
	.balign 4
	.global Func_020006a8
	.thumb_func
Func_020006a8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	adds r3, #224
	ldr r2, [r3]
	movs r3, #192
	ldr r0, .L_020087e4
	lsls r3, r3, #4
	adds r3, #188
	movs r6, #0
	adds r1, r7, r3
	sub sp, #12
	mov r8, r0
	str r6, [r1]
	cmp r2, #0
	beq .L_020086f2
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r7, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_020086f2
	ldr r3, [r2, #20]
	str r3, [r1]
	adds r6, r3, #0
	movs r1, #28
	ldrsh r3, [r2, r1]
	cmp r3, #147
	beq .L_020087a2
	movs r0, #1
	bl Func_0200053c
	b .L_020087a2
.L_020086f2:
	ldr r3, .L_020087e8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldrh r1, [r0, #6]
	movs r3, #128
	lsls r3, r3, #6
	adds r1, r1, r3
	movs r3, #192
	lsls r3, r3, #8
	ands r1, r3
	ldr r3, [r0, #8]
	mov r5, sp
	str r3, [r5]
	adds r2, r5, #0
	ldr r3, [r0, #12]
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	movs r0, #128
	str r3, [r5, #8]
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	mov r0, r8
	movs r1, #255
	ldrh r3, [r0]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	beq .L_0200878e
.L_02008734:
	mov r2, r8
	ldrh r0, [r2]
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r5]
	ldr r3, [r6, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200877a
	ldr r3, [r5, #4]
	cmp r3, #0
	bge .L_02008758
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r3, r0
.L_02008758:
	asrs r2, r3, #16
	ldr r3, [r6, #12]
	cmp r3, #0
	bge .L_02008768
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
.L_02008768:
	asrs r3, r3, #16
	cmp r2, r3
	bne .L_0200877a
	ldr r2, [r5, #8]
	ldr r3, [r6, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_0200878e
.L_0200877a:
	movs r2, #2
	add r8, r2
	mov r0, r8
	movs r1, #255
	ldrh r3, [r0]
	lsls r1, r1, #8
	adds r1, #255
	movs r6, #0
	cmp r3, r1
	bne .L_02008734
.L_0200878e:
	cmp r6, #0
	beq .L_020087da
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r7, r2
	str r6, [r3]
	movs r0, #1
	bl Func_0200053c
.L_020087a2:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	asrs r5, r5, #19
	subs r5, #2
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	movs r2, #0
	adds r3, r5, #0
	bl Func_020010cc
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	movs r2, #2
	adds r3, r5, #0
	bl Func_020010cc
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	ldr r3, [r6, #76]
	movs r0, #0
	bl Func_020003f0
.L_020087da:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020087e4:
	.4byte Data_02001220
.L_020087e8:
	.4byte gPartyState
	.section .text.x020087ec,"ax",%progbits
	.balign 4
	.global Func_020007ec
	.thumb_func
Func_020007ec:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldr r4, [r5, #68]
	ldr r3, [r5, #8]
	ldr r2, [r5, #72]
	adds r3, r3, r4
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	ldr r6, [r5, #76]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	sub sp, #4
	adds r3, r3, r6
	str r3, [r5, #16]
	adds r0, r4, #0
	movs r1, #12
	str r4, [sp, #0]
	bl __divsi3
	ldr r4, [sp, #0]
	movs r1, #10
	subs r4, r4, r0
	str r4, [r5, #68]
	adds r0, r6, #0
	bl __divsi3
	ldr r3, [r5, #24]
	ldr r2, [r5, #48]
	subs r6, r6, r0
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r2, [r5, #52]
	ldr r3, [r5, #28]
	str r6, [r5, #76]
	adds r3, r3, r2
	str r3, [r5, #28]
	ldr r1, [r5, #80]
	adds r5, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r5]
	add sp, #4
	adds r3, r3, r2
	strh r3, [r1, #18]
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200887c,"ax",%progbits
	.balign 4
	.global Func_0200087c
	.thumb_func
Func_0200087c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #192
	adds r3, #224
	ldr r1, [r3]
	lsls r0, r0, #4
	adds r0, #188
	adds r3, r2, r0
	sub sp, #68
	ldr r7, [r3]
	cmp r1, #0
	beq .L_020088c4
	movs r4, #179
	lsls r4, r4, #1
	adds r3, r2, r4
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_020088c4
	movs r2, #28
	ldrsh r3, [r1, r2]
	cmp r3, #147
	beq .L_020088cc
	movs r0, #1
	negs r0, r0
	bl Func_0200053c
	b .L_020088cc
.L_020088c4:
	movs r0, #1
	negs r0, r0
	bl Func_0200053c
.L_020088cc:
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	cmp r0, r3
	bne .L_020088de
	b .L_02008a60
.L_020088de:
	movs r3, #34
	adds r3, r3, r7
	movs r5, #2
	movs r0, #85
	strb r5, [r3]
	movs r4, #0
	adds r0, r0, r7
	mov r11, r3
	movs r3, #3
	strb r3, [r0]
	mov r9, r0
	str r4, [r7, #68]
	movs r0, #1
	mov r8, r4
	bl WaitFrames
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #2
	bl Func_020010a4
	cmp r0, #0
	beq .L_0200897a
	adds r1, r7, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	movs r2, #128
	ldr r3, .L_02008958
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r6, #128
	ldr r3, .L_0200895c
	ldr r2, [r7, #16]
	lsls r6, r6, #19
	adds r4, r0, #2
	adds r6, #80
	strh r3, [r6]
	lsls r3, r4, #20
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	movs r1, #0
	ldr r0, [r7, #8]
	bl OverlayObject_PrepareObject
	adds r5, r0, #0
	movs r0, #204
	bl Func_02001164
	adds r0, r7, #0
	bl OverlayObject_WaitUntilIdle
	adds r0, r7, #0
	bl OverlayObject_WaitUntilIdle
	b .L_02008960
	.2byte 0x0000
.L_02008958:
	.4byte 0x00001000
.L_0200895c:
	.4byte 0x00003f10
.L_02008960:
	mov r2, r8
	str r2, [r7, #16]
	str r2, [r7, #12]
	str r2, [r7, #8]
	adds r0, r5, #0
	bl Func_0200108c
	movs r0, #1
	bl WaitFrames
	mov r3, r8
	strh r3, [r6]
	b .L_02008a1e
.L_0200897a:
	adds r0, r7, #0
	bl OverlayObject_WaitUntilIdle
	adds r0, r7, #0
	bl OverlayObject_WaitUntilIdle
	movs r0, #188
	bl Func_02001164
	add r4, sp, #16
	movs r3, #7
	str r3, [r4, #4]
	ldr r3, .L_02008ab8
	str r5, [r4]
	str r3, [r4, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r4, #8]
	str r3, [r4, #12]
	movs r0, #0
	mov r8, r4
	mov r10, r0
.L_020089a8:
	mov r2, r10
	lsls r5, r2, #12
	adds r0, r5, #0
	bl Math_Cosine
	movs r3, #0
	add r6, sp, #56
	lsls r0, r0, #1
	str r0, [r6]
	str r3, [r6, #4]
	adds r0, r5, #0
	bl Math_Sine
	ldr r3, [r6]
	lsls r0, r0, #1
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	str r0, [r6, #8]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_02008abc
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_02008ac0
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r0, [r7, #8]
	ldr r2, [r7, #16]
	ldr r1, [r7, #12]
	ldr r3, [r6]
	str r4, [sp, #0]
	ldr r4, .L_02008ac4
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_0200015c
	movs r0, #1
	add r10, r0
	mov r2, r10
	cmp r2, #16
	bls .L_020089a8
.L_02008a1e:
	movs r3, #0
	mov r0, r11
	mov r4, r9
	strb r3, [r4]
	strb r3, [r0]
	ldr r3, [r7, #8]
	add r0, sp, #56
	str r3, [r0]
	ldr r3, [r7, #12]
	ldr r2, .L_02008ac8
	movs r1, #1
	adds r3, r3, r2
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	bl Func_0200115c
	cmp r0, #0
	beq .L_02008a60
	ldr r3, .L_02008acc
	movs r2, #1
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #98
	strb r2, [r3]
	adds r3, r0, #0
	movs r0, #128
	adds r3, #98
	lsls r0, r0, #2
	strb r2, [r3]
	adds r0, #18
	bl GameFlag_SetBit
.L_02008a60:
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	asrs r5, r5, #19
	adds r5, #2
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	adds r3, r5, #0
	movs r2, #0
	bl Func_020010cc
	adds r3, r5, #0
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	movs r2, #2
	bl Func_020010cc
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #0
	bl Func_020003b4
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r3, #128
	str r0, [r7, #76]
	orrs r3, r0
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #0
	bl Func_020003f0
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008ab8:
	.4byte Func_020007ec
.L_02008abc:
	.4byte 0xffffa000
.L_02008ac0:
	.4byte 0xffffd000
.L_02008ac4:
	.4byte 0x010b0000
.L_02008ac8:
	.4byte 0xfff00000
.L_02008acc:
	.4byte Func_02000848
	.section .text.x02008ba8,"ax",%progbits
	.balign 4
	.global Func_02000ba8
	.thumb_func
Func_02000ba8:
	push {r5, lr}
	movs r0, #4
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	bl Func_0200114c
	movs r1, #128
	lsls r1, r1, #2
	movs r2, #8
	movs r3, #9
	movs r0, #0
	bl Func_02001154
	movs r0, #10
	bl Object_GetById
	adds r3, r0, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	movs r2, #10
	ldr r3, [r0, #8]
	movs r1, #10
	asrs r3, r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #16
	movs r2, #1
	movs r3, #1
	bl Func_020010ac
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008c38,"ax",%progbits
	.balign 4
	.global Func_02000c38
	.thumb_func
Func_02000c38:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #85
	ldrb r0, [r7]
	adds r6, r1, #0
	mov r8, r0
	adds r0, r5, #0
	bl Func_020010b4
	cmp r0, #0
	bne .L_02008cce
	bl Func_020010dc
	movs r0, #0
	bl Func_0200113c
	movs r1, #6
	adds r0, r5, #0
	bl Func_02001074
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Func_02001164
	adds r0, r5, #0
	movs r1, #7
	bl Func_02001074
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldrb r2, [r7]
	movs r3, #126
	ands r3, r2
	adds r0, r5, #0
	strb r3, [r7]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_02008cd8
	movs r2, #2
	ldrsh r1, [r6, r2]
	movs r0, #10
	ldrsh r2, [r6, r0]
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl ObjectMotion_SetPositionAndCommit
	adds r0, r5, #0
	movs r1, #6
	bl Func_02001074
	adds r0, r5, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	mov r2, r8
	strb r2, [r7]
	bl Func_020010e4
	movs r0, #1
	b .L_02008cd0
.L_02008cce:
	movs r0, #0
.L_02008cd0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008cd8:
	.4byte gPartyState
