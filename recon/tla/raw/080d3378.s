.syntax unified
	.thumb
	.global Func_080d3378
	.thumb_func
Func_080d3378:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #30
	adds r5, r1, #0
	adds r0, #255
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	bl Func_080200c0
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080d3458
	ldr r2, [r6, #80]
	mov r8, r2
	bl Random16
	movs r3, #1
	ands r0, r3
	cmp r0, #1
	bne .L_080d33bc
	adds r0, r6, #0
	movs r1, #2
	bl Object_SetMode
	ldr r1, .L_080d33b8
	adds r0, r6, #0
	bl Object_SetCallback
	b .L_080d33cc
.L_080d33b8:
	.4byte Data_080f085c
.L_080d33bc:
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetMode
	ldr r1, .L_080d344c
	adds r0, r6, #0
	bl Object_SetCallback
.L_080d33cc:
	cmp r5, #0
	beq .L_080d33d8
	adds r0, r6, #0
	adds r1, r5, #0
	bl Object_SetPartAttribute
.L_080d33d8:
	adds r3, r6, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	bl Random16
	movs r1, #10
	bl Math_ModU
	ldr r3, .L_080d3450
	adds r0, #5
	muls r3, r0
	str r3, [r6, #52]
	bl Random16
	movs r1, #15
	bl Math_ModU
	movs r3, #200
	subs r0, #7
	lsls r3, r3, #5
	lsls r0, r0, #1
	adds r3, #153
	muls r3, r0
	str r3, [r6, #48]
	adds r3, r6, #0
	adds r3, #100
	strh r5, [r3]
	adds r0, r6, #0
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	adds r2, r6, #0
	strb r3, [r0]
	adds r2, #97
	movs r3, #1
	strb r3, [r2]
	ldr r3, .L_080d3454
	ldr r1, .L_080d3448
	str r3, [r6, #108]
	mov r3, r8
	strb r1, [r3, #26]
	ldr r3, [r7, #80]
	movs r2, #12
	ldrb r3, [r3, #9]
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	mov r2, r8
	strb r3, [r2, #9]
	b .L_080d3458
.L_080d3448:
	.4byte 0x00000000
.L_080d344c:
	.4byte Data_080f0874
.L_080d3450:
	.4byte 0xffffe667
.L_080d3454:
	.4byte Func_080d333c
.L_080d3458:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
