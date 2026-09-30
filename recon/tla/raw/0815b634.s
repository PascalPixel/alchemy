.syntax unified
	.thumb
	.global Func_0815b634
	.thumb_func
Func_0815b634:
	push {lr}
	sub sp, #256
	movs r1, #160
	lsls r1, r1, #19
	movs r2, #128
	ldr r3, .L_0815b688
	add r0, sp, #128
	mov lr, r3
	.2byte 0xf800
	mov r1, sp
	mov r2, sp
	movs r0, #1
	add r4, sp, #128
	adds r1, #2
	adds r2, #254
.L_0815b652:
	ldrh r3, [r2]
	adds r0, #1
	strh r3, [r1]
	subs r2, #2
	adds r1, #2
	cmp r0, #64
	bne .L_0815b652
	ldrh r3, [r4]
	mov r2, sp
	movs r0, #160
	strh r3, [r2]
	mov r1, sp
	ldr r3, .L_0815b688
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_0815b684
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	add sp, #256
	pop {pc}
	.2byte 0x0000
.L_0815b684:
	.4byte 0x00000810
.L_0815b688:
	.4byte IwramCopyWords
