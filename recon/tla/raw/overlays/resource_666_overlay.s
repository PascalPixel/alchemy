.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008060
	ldr r0, .L_02008078
	b .L_02008074
.L_02008060:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008072
	ldr r0, .L_0200807c
	b .L_02008074
.L_02008072:
	ldr r0, .L_02008080
.L_02008074:
	pop {pc}
	.2byte 0x0000
.L_02008078:
	.4byte Data_0200116c
.L_0200807c:
	.4byte Data_02000f14
.L_02008080:
	.4byte Data_02000cbc
	.section .text.x020081ac,"ax",%progbits
	.global Func_020001ac
	.thumb_func
Func_020001ac:
	push {r5, lr}
	ldr r3, .L_020081e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_020081e0
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020081e8
	movs r0, #9
	adds r1, r5, #0
	bl Func_02000bc4
	b .L_020081f6
	.2byte 0x0000
.L_020081e0:
	.4byte 0xffffc000
.L_020081e4:
	.4byte gPartyState
.L_020081e8:
	ldr r0, .L_020081f8
	bl Func_02000b24
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000b3c
.L_020081f6:
	pop {r5, pc}
.L_020081f8:
	.4byte MsgScene666No14
	.section .text.x020081fc,"ax",%progbits
	.global Func_020001fc
	.thumb_func
Func_020001fc:
	push {r5, lr}
	ldr r3, .L_02008234
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02008230
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008238
	movs r0, #2
	adds r1, r5, #0
	bl Func_02000bd4
	b .L_02008246
	.2byte 0x0000
.L_02008230:
	.4byte 0xffffc000
.L_02008234:
	.4byte gPartyState
.L_02008238:
	ldr r0, .L_02008248
	bl Func_02000b24
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000b3c
.L_02008246:
	pop {r5, pc}
.L_02008248:
	.4byte MsgScene666No16
	.section .text.x0200824c,"ax",%progbits
	.global Func_0200024c
	.thumb_func
Func_0200024c:
	push {r5, lr}
	ldr r3, .L_02008280
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_0200827c
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008284
	adds r0, r5, #0
	bl Func_02000bcc
	b .L_02008292
.L_0200827c:
	.4byte 0xffffc000
.L_02008280:
	.4byte gPartyState
.L_02008284:
	ldr r0, .L_02008294
	bl Func_02000b24
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000b3c
.L_02008292:
	pop {r5, pc}
.L_02008294:
	.4byte MsgScene666No49
	.section .text.x020084f4,"ax",%progbits
	.global Func_020004f4
	.thumb_func
Func_020004f4:
	push {r5, r6, lr}
	ldr r5, .L_0200853c
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_02000b24
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02000ba4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008524
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02000b24
	b .L_02008530
.L_02008524:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000b24
.L_02008530:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02000b3c
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200853c:
	.4byte MsgScene666No74
	.section .text.x02008540,"ax",%progbits
	.global Func_02000540
	.thumb_func
Func_02000540:
	push {lr}
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008554
	ldr r0, .L_0200856c
	b .L_02008568
.L_02008554:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008566
	ldr r0, .L_02008570
	b .L_02008568
.L_02008566:
	ldr r0, .L_02008574
.L_02008568:
	pop {pc}
	.2byte 0x0000
.L_0200856c:
	.4byte Data_020019dc
.L_02008570:
	.4byte Data_02001730
.L_02008574:
	.4byte Data_020013dc
	.section .text.x020085b8,"ax",%progbits
	.global Func_020005b8
	.thumb_func
Func_020005b8:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	movs r0, #16
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	strb r3, [r0]
	movs r0, #16
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	movs r0, #31
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #15
	bl ObjectMotion_SetActionVariant
	movs r0, #0
	bl Func_02000bac
	movs r3, #128
	ldr r0, .L_02008720
	movs r1, #8
	movs r2, #15
	lsls r3, r3, #23
	bl Func_02000bb4
	ldr r5, .L_02008724
	movs r3, #128
	movs r1, #8
	movs r2, #15
	adds r0, r5, #0
	lsls r3, r3, #23
	bl Func_02000bb4
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #24]
	movs r0, #14
	bl Object_GetById
	ldr r3, .L_02008728
	str r3, [r0, #108]
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_0200872c
	str r3, [r0, #108]
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086d4
	ldr r3, .L_02008730
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #18
	cmp r3, r2
	blt .L_02008678
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008680
.L_02008678:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_02008680:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086c6
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #35
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086c6
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #3
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086c6
	movs r1, #188
	movs r2, #158
	movs r0, #31
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000aec
	movs r1, #196
	movs r2, #172
	movs r0, #32
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000aec
	b .L_0200871c
.L_020086c6:
	movs r0, #31
	movs r1, #0
	movs r2, #0
	bl Func_02000aec
	movs r0, #32
	b .L_020086f2
.L_020086d4:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086fc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #113
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200871c
	movs r0, #31
.L_020086f2:
	movs r1, #0
	movs r2, #0
	bl Func_02000aec
	b .L_0200871c
.L_020086fc:
	movs r0, #24
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #19
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #28
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_0200871c:
	movs r0, #0
	pop {r5, pc}
.L_02008720:
	.4byte Data_02001c90
.L_02008724:
	.4byte Data_02001c60
.L_02008728:
	.4byte Func_02000578
.L_0200872c:
	.4byte Func_02000598
.L_02008730:
	.4byte gPartyState
	.section .bss,"aw",%nobits
	.space 0x00000008
	.global Data_02001c60
Data_02001c60:
	.space 0x00000030
	.global Data_02001c90
Data_02001c90:
