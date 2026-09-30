.syntax unified
	.thumb
	.global Func_08143000
	.thumb_func
Func_08143000:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #92]
	ldr r5, [r3, #96]
	ldr r1, .L_08143100
	ldrh r3, [r1]
	adds r0, r3, #0
	strh r1, [r1]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r6, r3
	ldr r3, [r2]
	cmp r3, #1
	bne .L_081430ee
	movs r3, #0
	str r3, [r2]
	strh r0, [r1]
	movs r2, #239
	lsls r2, r2, #7
	adds r3, r6, r2
	ldr r3, [r3]
	cmp r3, #4
	bhi .L_081430de
	ldr r2, .L_08143104
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0814303c:
	.4byte .L_08143050
	.4byte .L_08143060
	.4byte .L_08143086
	.4byte .L_081430b0
	.4byte .L_081430c8
.L_08143050:
	movs r2, #128
	ldr r3, .L_08143108
	ldr r0, .L_0814310c
	adds r1, r5, #0
	lsls r2, r2, #7
	mov lr, r3
	.2byte 0xf800
	b .L_081430de
.L_08143060:
	movs r2, #128
	ldr r3, .L_08143108
	adds r1, r5, #0
	lsls r2, r2, #7
	ldr r0, .L_0814310c
	mov lr, r3
	.2byte 0xf800
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r6, r2
	movs r1, #128
	ldr r2, [r3]
	adds r0, r5, #0
	ldr r3, .L_08143110
	lsls r1, r1, #7
	mov lr, r3
	.2byte 0xf800
	b .L_081430de
.L_08143086:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r6, r2
	ldr r3, [r3]
	cmp r3, #50
	bne .L_081430a2
	movs r2, #128
	ldr r1, .L_0814310c
	lsls r2, r2, #7
	adds r0, r5, #0
	bl ColorBuffer_Halve
	b .L_081430de
.L_081430a2:
	movs r2, #128
	ldr r1, .L_0814310c
	lsls r2, r2, #7
	adds r0, r5, #0
	bl ColorBuffer_ScaleThreeQuarters
	b .L_081430de
.L_081430b0:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r6, r2
	ldr r1, [r3]
	movs r3, #128
	ldr r2, .L_0814310c
	lsls r3, r3, #7
	adds r0, r5, #0
	bl ColorBuffer_Darken
	b .L_081430de
.L_081430c8:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r6, r2
	ldr r1, [r3]
	movs r3, #128
	ldr r2, .L_0814310c
	lsls r3, r3, #7
	adds r0, r5, #0
	bl ColorBuffer_Brighten
.L_081430de:
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #228
	adds r3, r6, r2
	ldr r2, [r3]
	movs r2, #1
	str r2, [r3]
	b .L_081430fe
.L_081430ee:
	strh r0, [r1]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #228
	adds r2, r6, r3
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_081430fe:
	pop {r5, r6, pc}
.L_08143100:
	.4byte 0x04000208
.L_08143104:
	.4byte .L_0814303c
.L_08143108:
	.4byte IwramCopyWords
.L_0814310c:
	.4byte 0x06004000
.L_08143110:
	.4byte IwramFillWords
