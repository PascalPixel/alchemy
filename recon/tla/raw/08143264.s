.syntax unified
	.thumb
	.global Func_08143264
	.thumb_func
Func_08143264:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #18
	ldr r6, [r0, #92]
	ldr r2, .L_08143344
	ldrh r3, [r2]
	adds r1, r3, #0
	strh r2, [r2]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	adds r4, r6, r3
	ldr r3, [r4]
	cmp r3, #1
	bne .L_08143330
	ldr r5, [r0, #96]
	movs r3, #0
	str r3, [r4]
	strh r1, [r2]
	movs r2, #239
	lsls r2, r2, #7
	adds r3, r6, r2
	ldr r3, [r3]
	cmp r3, #1
	beq .L_081432ba
	cmp r3, #1
	bgt .L_081432a0
	cmp r3, #0
	beq .L_081432aa
	b .L_08143320
.L_081432a0:
	cmp r3, #2
	beq .L_081432e0
	cmp r3, #3
	beq .L_0814330a
	b .L_08143320
.L_081432aa:
	movs r2, #240
	ldr r3, .L_08143348
	ldr r0, .L_0814334c
	adds r1, r5, #0
	lsls r2, r2, #7
	mov lr, r3
	.2byte 0xf800
	b .L_08143320
.L_081432ba:
	movs r2, #240
	ldr r3, .L_08143348
	adds r1, r5, #0
	lsls r2, r2, #7
	ldr r0, .L_0814334c
	mov lr, r3
	.2byte 0xf800
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r6, r2
	movs r1, #240
	ldr r2, [r3]
	adds r0, r5, #0
	ldr r3, .L_08143350
	lsls r1, r1, #6
	mov lr, r3
	.2byte 0xf800
	b .L_08143320
.L_081432e0:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r6, r2
	ldr r3, [r3]
	cmp r3, #50
	bne .L_081432fc
	movs r2, #240
	ldr r1, .L_0814334c
	lsls r2, r2, #6
	adds r0, r5, #0
	bl ColorBuffer_Halve
	b .L_08143320
.L_081432fc:
	movs r2, #240
	ldr r1, .L_0814334c
	lsls r2, r2, #6
	adds r0, r5, #0
	bl ColorBuffer_ScaleThreeQuarters
	b .L_08143320
.L_0814330a:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r6, r2
	ldr r1, [r3]
	movs r3, #240
	ldr r2, .L_0814334c
	lsls r3, r3, #6
	adds r0, r5, #0
	bl ColorBuffer_Darken
.L_08143320:
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #228
	adds r3, r6, r2
	ldr r2, [r3]
	movs r2, #1
	str r2, [r3]
	b .L_08143340
.L_08143330:
	strh r1, [r2]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #228
	adds r2, r6, r3
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_08143340:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08143344:
	.4byte 0x04000208
.L_08143348:
	.4byte IwramCopyWords
.L_0814334c:
	.4byte 0x06008000
.L_08143350:
	.4byte IwramFillWords
