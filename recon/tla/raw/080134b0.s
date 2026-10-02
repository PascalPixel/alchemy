.syntax unified
	.thumb
	.global System_WaitForFrameInterrupt
	.thumb_func
System_WaitForFrameInterrupt:
	push {lr}
	ldr r3, .L_08013548
	ldrb r3, [r3, #2]
	cmp r3, #0
	beq .L_080134d8
	ldr r3, .L_0801354c
	movs r2, #255
	ldrh r1, [r3]
	lsls r2, r2, #8
	adds r2, #254
	ands r2, r1
	strh r2, [r3]
	adds r0, r3, #0
	movs r1, #1
.L_080134cc:
	ldrh r2, [r0]
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080134cc
	b .L_08013544
.L_080134d8:
	ldr r3, .L_08013550
	ldrh r2, [r3]
	adds r4, r2, #0
	strh r3, [r3]
	ldr r1, .L_0801354c
	movs r3, #255
	ldrh r2, [r1]
	lsls r3, r3, #8
	adds r3, #254
	ands r3, r2
	strh r3, [r1]
	ldr r3, .L_08013554
	ldr r1, .L_08013548
	ldr r3, [r3]
	ldrb r2, [r1]
	strb r3, [r1]
	ldrb r3, [r1, #3]
	movs r3, #1
	strb r3, [r1, #3]
	ldr r3, .L_08013558
	ldr r0, [r3]
	ldrb r2, [r0, #4]
	adds r3, r2, #0
	cmp r3, #1
	bhi .L_0801350e
	ldrb r3, [r0, #11]
	b .L_08013512
.L_0801350e:
	adds r3, r2, #0
	adds r3, #255
.L_08013512:
	ldrb r2, [r1, #1]
	strb r3, [r1, #1]
	ldr r3, .L_08013550
	strh r4, [r3]
	bl Func_081c0088
	bl SoundDriver_EnterFrameUpdate
	cmp r0, #0
	bne .L_0801352e
	ldr r3, .L_0801355c
	movs r0, #8
	mov lr, r3
	.2byte 0xf800
.L_0801352e:
	ldr r3, .L_08013548
	ldr r0, .L_0801354c
	ldrb r2, [r3, #3]
	movs r2, #0
	strb r2, [r3, #3]
	movs r1, #1
.L_0801353a:
	ldrh r2, [r0]
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0801353a
.L_08013544:
	pop {pc}
	.2byte 0x0000
.L_08013548:
	.4byte Data_03001138
.L_0801354c:
	.4byte Data_0300121c
.L_08013550:
	.4byte 0x04000208
.L_08013554:
	.4byte gFrameTick
.L_08013558:
	.4byte Data_03007ff0
.L_0801355c:
	.4byte IwramSoundRenderFrame
