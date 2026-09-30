.syntax unified
	.thumb
	.global Func_080dc044
	.thumb_func
Func_080dc044:
	push {r5, r6, lr}
	adds r4, r0, #0
	movs r0, #209
	adds r5, r1, #0
	lsls r0, r0, #1
	adds r3, r2, #0
	adds r0, #255
	adds r1, r4, #0
	adds r2, r5, #0
	bl Func_080dc10c
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080dc0b0
	ldr r1, .L_080dc0b4
	bl Object_SetCallback
	bl Random16
	movs r3, #128
	lsls r3, r3, #9
	adds r2, r6, #0
	adds r2, #85
	adds r0, r0, r3
	str r3, [r6, #52]
	movs r3, #2
	str r0, [r6, #48]
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #61
	str r3, [r6, #72]
	bl Random16
	adds r5, r0, #0
	bl Random16
	subs r5, r5, r0
	str r5, [r6, #40]
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #3
	adds r5, r5, r3
	bl Random16
	adds r1, r5, #0
	adds r2, r0, #0
	adds r0, r6, #0
	bl Func_080db974
.L_080dc0b0:
	adds r0, r6, #0
	pop {r5, r6, pc}
.L_080dc0b4:
	.4byte Data_080f0e78
