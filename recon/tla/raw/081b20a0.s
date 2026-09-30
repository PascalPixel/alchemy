.syntax unified
	.thumb
	.global Func_081b20a0
	.thumb_func
Func_081b20a0:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #18
	ldr r5, [r0, #92]
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	adds r3, r5, r2
	ldr r3, [r3]
	cmp r3, #1
	bne .L_081b2132
	movs r2, #239
	lsls r2, r2, #7
	adds r3, r5, r2
	ldr r3, [r3]
	ldr r4, [r0, #96]
	cmp r3, #1
	beq .L_081b20ca
	cmp r3, #2
	beq .L_081b20f2
	b .L_081b211a
.L_081b20ca:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r0, r4, #0
	ldr r1, .L_081b2144
	ldr r2, .L_081b2148
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r5, r2
	movs r1, #128
	ldr r2, [r3]
	adds r0, r4, #0
	ldr r3, .L_081b214c
	lsls r1, r1, #8
	mov lr, r3
	.2byte 0xf800
	b .L_081b211a
.L_081b20f2:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	adds r3, r5, r2
	ldr r3, [r3]
	cmp r3, #50
	bne .L_081b210e
	movs r2, #128
	ldr r1, .L_081b2144
	lsls r2, r2, #8
	adds r0, r4, #0
	bl ColorBuffer_HalveNonzero
	b .L_081b211a
.L_081b210e:
	movs r2, #128
	ldr r1, .L_081b2144
	lsls r2, r2, #8
	adds r0, r4, #0
	bl ColorBuffer_ScaleNonzeroThreeQuarters
.L_081b211a:
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r5, r3
	movs r3, #0
	str r3, [r2]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #228
	adds r2, r5, r3
	movs r3, #1
	b .L_081b213e
.L_081b2132:
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #228
	adds r2, r5, r3
	ldr r3, [r2]
	adds r3, #1
.L_081b213e:
	str r3, [r2]
	pop {r5, pc}
	.2byte 0x0000
.L_081b2144:
	.4byte 0x06003500
.L_081b2148:
	.4byte 0x84002000
.L_081b214c:
	.4byte IwramFillWords
