.syntax unified
	.thumb
	.global Func_08042dc4
	.thumb_func
Func_08042dc4:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	ldr r2, [r3]
	movs r1, #196
	lsls r1, r1, #6
	adds r1, #66
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #3
	bgt .L_08042de0
	lsls r0, r0, #1
.L_08042de0:
	movs r3, #197
	lsls r3, r3, #6
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, r3, r0
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #6
	bls .L_08042df6
	movs r0, #6
.L_08042df6:
	lsls r3, r0, #1
	ldr r2, .L_08042e0c
	adds r3, r3, r0
	ldr r1, .L_08042e10
	ldr r4, .L_08042e08
	lsls r3, r3, #2
	adds r0, r3, r2
	movs r2, #11
	b .L_08042e14
.L_08042e08:
	.4byte 0x0000f080
.L_08042e0c:
	.4byte Data_0805f5ec
.L_08042e10:
	.4byte 0x06002252
.L_08042e14:
	ldrb r3, [r0]
	subs r2, #1
	adds r3, r3, r4
	strh r3, [r1]
	adds r0, #1
	adds r1, #2
	cmp r2, #0
	bge .L_08042e14
	pop {pc}
	.2byte 0x0000
