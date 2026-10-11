.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r1, .L_0200807c
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008080
	cmp r2, r3
	bne .L_02008078
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_02008078
	ldr r0, .L_02008084
	b .L_0200807a
.L_02008078:
	ldr r0, .L_02008088
.L_0200807a:
	pop {pc}
.L_0200807c:
	.4byte gPartyState
.L_02008080:
	.4byte 0x000000b7
.L_02008084:
	.4byte Data_020003a8
.L_02008088:
	.4byte Data_02000228
	.section .text.x0200808c,"ax",%progbits
	.global Func_0200008c
	.thumb_func
Func_0200008c:
	push {lr}
	adds r1, r0, #0
	movs r0, #25
	bl Func_02000178
	pop {pc}
	.section .text.x02008098,"ax",%progbits
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {lr}
	adds r1, r0, #0
	movs r0, #10
	bl Func_02000180
	pop {pc}
	.section .text.x020080a4,"ax",%progbits
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	push {lr}
	ldr r1, .L_020080e8
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_020080ec
	cmp r2, r3
	bne .L_020080d0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_020080d0
	ldr r0, .L_020080f0
	b .L_020080e4
.L_020080d0:
	movs r0, #136
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080e2
	ldr r0, .L_020080f4
	b .L_020080e4
.L_020080e2:
	ldr r0, .L_020080f8
.L_020080e4:
	pop {pc}
	.2byte 0x0000
.L_020080e8:
	.4byte gPartyState
.L_020080ec:
	.4byte 0x000000b7
.L_020080f0:
	.4byte Data_02000780
.L_020080f4:
	.4byte Data_020005ac
.L_020080f8:
	.4byte Data_020003d8
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	.ifdef TLA_EDITION_JA
	movs r2, #129
	.else
	movs r2, #133
	.endif
	lsls r0, r0, #1
	lsls r2, r2, #1
	adds r3, r3, r0
	adds r2, #255
	ldr r1, .L_02008140
	str r2, [r3]
	.ifdef TLA_EDITION_JA
	subs r2, #33
	.else
	subs r2, #41
	.endif
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008144
	cmp r2, r3
	bne .L_0200813a
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bhi .L_0200813a
	bl Func_0200014c
.L_0200813a:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02008140:
	.4byte gPartyState
.L_02008144:
	.4byte 0x000000b7
	.section .text.x0200814c,"ax",%progbits
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #48
	adds r2, #93
	str r2, [r3]
	adds r0, #255
	bl GameFlag_ClearBit
	pop {pc}
