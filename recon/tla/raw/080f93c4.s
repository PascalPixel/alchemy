.syntax unified
	.thumb
	.global Func_080f93c4
	.thumb_func
Func_080f93c4:
	push {lr}
	ldr r3, .L_080f9428
	ldr r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #0
	bne .L_080f93f4
	movs r3, #4
	ands r3, r2
	cmp r3, #0
	beq .L_080f93e8
	ldr r1, .L_080f942c
	ldr r3, .L_080f9430
	ldr r0, .L_080f9434
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	b .L_080f93f4
.L_080f93e8:
	ldr r3, .L_080f9438
	ldr r0, .L_080f9434
	movs r1, #32
	ldr r2, .L_080f943c
	mov lr, r3
	.2byte 0xf800
.L_080f93f4:
	ldr r3, .L_080f9440
	movs r2, #8
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_080f9414
	movs r0, #113
	bl Audio_PlayCue
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_SetBit
	ldr r0, .L_080f9444
	bl Func_08014644
.L_080f9414:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r2, #227
	lsls r2, r2, #4
	adds r3, r3, r2
.L_080f9420:
	ldr r3, [r3]
	cmp r3, #0
	bne .L_080f9420
	pop {pc}
.L_080f9428:
	.4byte Data_0300122c
.L_080f942c:
	.4byte Data_08105948
.L_080f9430:
	.4byte IwramCopyWords
.L_080f9434:
	.4byte 0x06002540
.L_080f9438:
	.4byte IwramFillWords
.L_080f943c:
	.4byte 0x44444444
.L_080f9440:
	.4byte gInput
.L_080f9444:
	.4byte Func_080f93c4
