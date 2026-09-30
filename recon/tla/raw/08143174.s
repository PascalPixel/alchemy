.syntax unified
	.thumb
	.global Func_08143174
	.thumb_func
Func_08143174:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #92]
	ldr r1, .L_08143250
	ldrh r3, [r1]
	adds r0, r3, #0
	strh r1, [r1]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r5, r3
	ldr r3, [r2]
	cmp r3, #1
	bne .L_0814323c
	movs r3, #0
	str r3, [r2]
	strh r0, [r1]
	movs r2, #239
	lsls r2, r2, #7
	adds r3, r5, r2
	ldr r3, [r3]
	cmp r3, #1
	beq .L_081431c8
	cmp r3, #1
	bgt .L_081431ae
	cmp r3, #0
	beq .L_081431b8
	b .L_0814322c
.L_081431ae:
	cmp r3, #2
	beq .L_081431ee
	cmp r3, #3
	beq .L_08143216
	b .L_0814322c
.L_081431b8:
	movs r2, #240
	ldr r3, .L_08143254
	ldr r0, .L_08143258
	ldr r1, .L_0814325c
	lsls r2, r2, #7
	mov lr, r3
	.2byte 0xf800
	b .L_0814322c
.L_081431c8:
	movs r2, #240
	ldr r3, .L_08143254
	ldr r1, .L_0814325c
	lsls r2, r2, #7
	ldr r0, .L_08143258
	mov lr, r3
	.2byte 0xf800
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r5, r2
	movs r1, #240
	ldr r2, [r3]
	ldr r0, .L_0814325c
	ldr r3, .L_08143260
	lsls r1, r1, #7
	mov lr, r3
	.2byte 0xf800
	b .L_0814322c
.L_081431ee:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r5, r2
	ldr r3, [r3]
	cmp r3, #50
	bne .L_08143208
	ldr r1, .L_08143258
	adds r2, #124
	ldr r0, .L_0814325c
	bl ColorBuffer_Halve
	b .L_0814322c
.L_08143208:
	movs r2, #240
	ldr r1, .L_08143258
	lsls r2, r2, #7
	ldr r0, .L_0814325c
	bl ColorBuffer_ScaleThreeQuarters
	b .L_0814322c
.L_08143216:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r5, r2
	ldr r1, [r3]
	movs r3, #240
	ldr r2, .L_08143258
	lsls r3, r3, #7
	ldr r0, .L_0814325c
	bl ColorBuffer_Darken
.L_0814322c:
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #228
	adds r3, r5, r2
	ldr r2, [r3]
	movs r2, #1
	str r2, [r3]
	b .L_0814324c
.L_0814323c:
	strh r0, [r1]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #228
	adds r2, r5, r3
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0814324c:
	pop {r5, pc}
	.2byte 0x0000
.L_08143250:
	.4byte 0x04000208
.L_08143254:
	.4byte IwramCopyWords
.L_08143258:
	.4byte 0x06008000
.L_0814325c:
	.4byte gMapCellBuffer
.L_08143260:
	.4byte IwramFillWords
