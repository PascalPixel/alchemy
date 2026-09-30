.syntax unified
	.thumb
	.global Func_081c04e0
	.thumb_func
Func_081c04e0:
	bl Func_081c08a4 + 0x28
	ldr r0, .L_081c0530
	bl SoundDriver_CalculateStereoVolume + 0xe
	ldr r0, .L_081c0534
	bl Func_081c0a68
	ldr r0, .L_081c0538
	lsls r0, r0, #16
	lsrs r0, r0, #16
	cmp r0, #0
	beq .L_081c051a
	ldr r5, .L_081c053c
	adds r6, r0, #0
.L_081c04fe:
	ldr r4, [r5]
	ldr r1, [r5, #4]
	ldrb r2, [r5, #8]
	adds r0, r4, #0
	bl Func_081c0c0c
	ldrh r0, [r5, #10]
	strb r0, [r4, #11]
	ldr r0, .L_081c0540
	str r0, [r4, #24]
	adds r5, #12
	subs r6, #1
	cmp r6, #0
	bne .L_081c04fe
.L_081c051a:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0100
	.2byte 0x0400
	.2byte 0x0000
	.2byte 0x0000
.L_081c0530:
	.4byte 0x00000000
.L_081c0534:
	.4byte 0x00000000
.L_081c0538:
	.4byte 0x00000000
.L_081c053c:
	.4byte 0x00000000
.L_081c0540:
	.4byte 0x00000000
