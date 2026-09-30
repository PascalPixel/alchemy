.syntax unified
	.thumb
	.global Func_080dcadc
	.thumb_func
Func_080dcadc:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, .L_080dcb30
	ldr r6, [r3, #60]
	bl Func_08014644
	ldr r2, .L_080dcb34
	ldr r3, .L_080dcb20
	ldr r5, .L_080dcb24
	strh r3, [r2]
	ldr r3, .L_080dcb38
	adds r2, #20
	strh r5, [r3]
	ldr r3, .L_080dcb28
	movs r1, #147
	strh r3, [r2]
	ldr r3, .L_080dcb2c
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_080dcb3c
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3]
	bl Func_08038348
	strb r5, [r6, #4]
	b .L_080dcb40
.L_080dcb20:
	.4byte 0x00007fff
.L_080dcb24:
	.4byte 0x00000000
.L_080dcb28:
	.4byte 0x0000294a
.L_080dcb2c:
	.4byte 0x00005294
.L_080dcb30:
	.4byte Func_080dcdc8
.L_080dcb34:
	.4byte 0x050001e2
.L_080dcb38:
	.4byte 0x050001e6
.L_080dcb3c:
	.4byte gPartyState
.L_080dcb40:
	pop {r5, r6, pc}
	.2byte 0x0000
