.syntax unified
	.thumb
	.global Func_0818284c
	.thumb_func
Func_0818284c:
	push {r5, r6, lr}
	movs r3, #192
	adds r6, r0, #0
	lsls r3, r3, #18
	movs r0, #0
	ldr r5, [r3, #92]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0818288c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_08182890
	adds r2, #2
	strh r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #180
	adds r2, r5, r3
	movs r3, #24
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #184
	adds r5, r5, r3
	movs r3, #0
	str r3, [r5]
	adds r0, r6, #0
	bl Func_08182898
	b .L_08182894
.L_0818288c:
	.4byte 0x00003f44
.L_08182890:
	.4byte 0x00001010
.L_08182894:
	pop {r5, r6, pc}
	.2byte 0x0000
