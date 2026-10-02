@ Shaman village: message ids are owned by the editable catalogs.
@ Trial: the unlocalized source matched EN 25,420 completely; JA 280 and DE 283
@ differing byte positions remained in full 25,420-byte loaded scenes.
@ Current JA/EN/DE scenes match completely; FR's extra graphic branch and
@ ES/IT's unresolved status/delay imports remain on their terminal scaffold.

	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ The palette selector table below supplies five 32-byte banks in JA/EN.
@ DE/ES/FR/IT give the eighth graphic its own sixth bank; tiles follow it.
	.set SHAMAN_PALETTE_ROWS, 5
	.set SHAMAN_GRAPHIC7_PALETTE, (2 * 32)
	.ifdef TLA_EDITION_DE
	.set SHAMAN_PALETTE_ROWS, 6
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_ES
	.set SHAMAN_PALETTE_ROWS, 6
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_FR
	.set SHAMAN_PALETTE_ROWS, 6
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_IT
	.set SHAMAN_PALETTE_ROWS, 6
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	ldr r1, [r0, #80]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #72]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #76]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r2, [r0, #48]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [r0, #52]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r0, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #18]
	bx lr
	.2byte 0x0000
	.section .text.x020080b8,"ax",%progbits
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02008270
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_02008100
	cmp r7, #0
	beq .L_02008100
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02008108
.L_02008100:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02008108:
	mov r3, r10
	bl Engine_ObjectCreate
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02008116
	b .L_02008262
.L_02008116:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Engine_ObjectSetMode
	ldr r2, .L_02008274
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Engine_ObjectSetScript
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02008278
	mov r1, r9
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	adds r0, r6, #0
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_0200827c
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008262
	cmp r7, #0
	beq .L_02008262
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02008198
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_02008198:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020081b8
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_020081b8:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_020081cc
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_020081cc:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008212
	ldr r3, .L_02008274
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020081fa
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200820c
.L_020081fa:
	ldr r2, .L_0200827c
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200827c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200820c:
	bl __divsi3
	str r0, [r6, #52]
.L_02008212:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200822e
	adds r0, r6, #0
	movs r1, #1
	bl Engine_ObjectSetMode
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Engine_ObjectSetScript
.L_0200822e:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008240
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_02008240:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008252
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_02008252:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008262
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02008262:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008270:
	.4byte gPartyState
.L_02008274:
	.4byte Data_02004c50
.L_02008278:
	.4byte Func_02000080
.L_0200827c:
	.4byte 0xffff0000
	.section .text.x02008280,"ax",%progbits
	.global Func_02000280
	.thumb_func
Func_02000280:
	push {lr}
	movs r0, #18
	movs r1, #3
	movs r2, #13
	bl Func_02004a34
	pop {pc}
	.2byte 0x0000
	.section .text.x02008290,"ax",%progbits
	.global Func_02000290
	.thumb_func
Func_02000290:
	push {lr}
	ldr r3, .L_020082b8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020082bc
	cmp r2, r3
	bne .L_020082a8
	ldr r0, .L_020082c0
	b .L_020082b4
.L_020082a8:
	ldr r3, .L_020082c4
	cmp r2, r3
	bne .L_020082b2
	ldr r0, .L_020082c8
	b .L_020082b4
.L_020082b2:
	ldr r0, .L_020082cc
.L_020082b4:
	pop {pc}
	.2byte 0x0000
.L_020082b8:
	.4byte gPartyState
.L_020082bc:
	.4byte 0x000000f5
.L_020082c0:
	.4byte Data_020052b4
.L_020082c4:
	.4byte 0x000000f6
.L_020082c8:
	.4byte Data_02005314
.L_020082cc:
	.4byte Data_02005284
	.section .text.x020082dc,"ax",%progbits
	.global Func_020002dc
	.thumb_func
Func_020002dc:
	push {lr}
	ldr r3, .L_02008340
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008344
	cmp r2, r3
	bne .L_020082f4
	ldr r0, .L_02008348
	b .L_0200833c
.L_020082f4:
	ldr r3, .L_0200834c
	cmp r2, r3
	bne .L_020082fe
	ldr r0, .L_02008350
	b .L_0200833c
.L_020082fe:
	ldr r3, .L_02008354
	cmp r2, r3
	bne .L_0200831c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #254
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008318
	ldr r2, .L_02008358
	movs r3, #1
	strb r3, [r2, #22]
.L_02008318:
	ldr r0, .L_02008358
	b .L_0200833c
.L_0200831c:
	ldr r3, .L_0200835c
	cmp r2, r3
	bne .L_02008326
	ldr r0, .L_02008360
	b .L_0200833c
.L_02008326:
	ldr r3, .L_02008364
	cmp r2, r3
	bne .L_02008330
	ldr r0, .L_02008368
	b .L_0200833c
.L_02008330:
	ldr r3, .L_0200836c
	cmp r2, r3
	bne .L_0200833a
	ldr r0, .L_02008370
	b .L_0200833c
.L_0200833a:
	ldr r0, .L_02008374
.L_0200833c:
	pop {pc}
	.2byte 0x0000
.L_02008340:
	.4byte gPartyState
.L_02008344:
	.4byte 0x000000f1
.L_02008348:
	.4byte Data_02005428
.L_0200834c:
	.4byte 0x000000f3
.L_02008350:
	.4byte Data_02005590
.L_02008354:
	.4byte 0x000000f2
.L_02008358:
	.4byte Data_020056b0
.L_0200835c:
	.4byte 0x000000f4
.L_02008360:
	.4byte Data_02005860
.L_02008364:
	.4byte 0x000000f5
.L_02008368:
	.4byte Data_02005998
.L_0200836c:
	.4byte 0x000000f6
.L_02008370:
	.4byte Data_02005a40
.L_02008374:
	.4byte Data_02005410
	.section .text.x02008378,"ax",%progbits
	.global Func_02000378
	.thumb_func
Func_02000378:
	push {r5, r6, r7, lr}
	ldr r6, .L_02008410
	movs r7, #0
.L_0200837e:
	ldr r3, .L_02008414
	movs r5, #160
	ldrb r0, [r3]
	subs r5, r5, r7
	adds r0, r7, r0
	lsls r0, r0, #8
	bl Math_Sine
	movs r1, #144
	subs r1, r1, r7
	ldr r3, .L_02008418
	lsls r1, r1, #10
	mov lr, r3
	.2byte 0xf800
	asrs r0, r0, #11
	subs r0, #4
	asrs r5, r5, #2
	strh r0, [r6]
	bl Random16Far
	lsls r1, r5, #1
	bl __umodsi3
	ldrh r3, [r6]
	subs r0, r0, r5
	adds r2, r3, r0
	lsls r3, r2, #16
	asrs r3, r3, #16
	strh r2, [r6]
	cmp r3, #56
	ble .L_020083c2
	adds r3, r2, #0
	subs r3, #56
	b .L_020083ce
.L_020083c2:
	movs r1, #64
	negs r1, r1
	cmp r3, r1
	bge .L_020083d0
	adds r3, r2, #0
	adds r3, #64
.L_020083ce:
	strh r3, [r6]
.L_020083d0:
	adds r7, #1
	adds r6, #2
	cmp r7, #160
	bne .L_0200837e
	ldr r6, .L_02008410
	movs r1, #128
	ldrh r3, [r6]
	lsls r1, r1, #19
	adds r1, #16
	strh r3, [r1]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r0, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r0
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r0, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r0
	strh r2, [r3, #10]
	adds r0, r6, #2
	ldrh r2, [r3, #10]
	ldr r2, .L_0200841c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008410:
	.4byte Data_02006350
.L_02008414:
	.4byte Data_0300122c
.L_02008418:
	.4byte IwramMulQ16
.L_0200841c:
	.4byte 0xa2600001
	.section .text.x02008420,"ax",%progbits
	.global Func_02000420
	.thumb_func
Func_02000420:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #188
	lsls r1, r1, #1
	adds r2, r3, r1
	ldr r3, [r2, #8]
	ldr r1, .L_02008454
	adds r3, r3, r1
	str r3, [r2, #8]
	ldr r3, [r2, #12]
	movs r1, #128
	lsls r1, r1, #10
	adds r3, r3, r1
	movs r1, #130
	lsls r1, r1, #19
	str r3, [r2, #12]
	cmp r3, r1
	ble .L_02008452
	movs r3, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #19
	str r3, [r2, #12]
.L_02008452:
	pop {pc}
.L_02008454:
	.4byte 0xfffe0000
	.section .text.x02008458,"ax",%progbits
	.global Func_02000458
	.thumb_func
Func_02000458:
	push {lr}
	ldr r3, .L_02008484
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	adds r2, r0, #0
	adds r2, #99
	ldrb r2, [r2]
	lsrs r3, r2
	movs r2, #1
	ands r3, r2
	adds r2, r0, #0
	adds r2, #98
	ldrb r2, [r2]
	adds r1, r2, #0
	muls r1, r3
	lsls r1, r1, #24
	lsrs r1, r1, #24
	bl Object_SetPartAttribute
	pop {pc}
	.2byte 0x0000
.L_02008484:
	.4byte Data_0300122c
	.section .text.x02008488,"ax",%progbits
	.global Func_02000488
	.thumb_func
Func_02000488:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	ldr r6, [r5, #12]
	ldr r0, [r5, #48]
	movs r3, #192
	lsls r3, r3, #12
	asrs r6, r6, #2
	adds r6, r6, r3
	movs r3, #255
	ands r0, r3
	lsls r0, r0, #11
	mov r8, r3
	bl Math_Cosine
	ldr r3, .L_02008500
	adds r1, r6, #0
	mov r10, r3
	mov lr, r10
	.2byte 0xf800
	ldr r3, [r5, #68]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r5, #48]
	str r3, [r5, #8]
	mov r3, r8
	ands r0, r3
	lsls r0, r0, #11
	bl Math_Sine
	adds r1, r6, #0
	mov lr, r10
	.2byte 0xf800
	ldr r3, [r5, #76]
	ldr r2, [r5, #72]
	adds r3, r3, r0
	str r3, [r5, #16]
	ldr r3, [r5, #12]
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r5, #48]
	movs r2, #2
	adds r3, #1
	str r3, [r5, #48]
	ldr r3, .L_02008504
	ldr r3, [r3]
	ands r3, r2
	lsrs r3, r3, #1
	lsls r1, r3, #3
	adds r1, r1, r3
	bl Object_SetPartAttribute
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008500:
	.4byte IwramMulQ16
.L_02008504:
	.4byte Data_0300122c
	.section .text.x02008508,"ax",%progbits
	.global Func_02000508
	.thumb_func
Func_02000508:
	push {r5, lr}
	adds r5, r0, #0
	ldr r3, [r5, #12]
	ldr r2, [r5, #72]
	ldr r1, [r5, #8]
	adds r3, r3, r2
	ldr r0, [r5, #68]
	str r3, [r5, #12]
	ldr r2, [r5, #76]
	ldr r3, [r5, #16]
	adds r1, r1, r0
	str r1, [r5, #8]
	adds r3, r3, r2
	asrs r1, r1, #16
	str r3, [r5, #16]
	cmp r1, #167
	bgt .L_0200853c
	movs r1, #192
	lsls r1, r1, #9
	adds r3, r0, r1
	str r3, [r5, #68]
	movs r2, #192
	ldr r3, [r5, #24]
	lsls r2, r2, #4
	adds r2, #204
	b .L_02008546
.L_0200853c:
	ldr r1, .L_02008574
	ldr r2, .L_02008578
	adds r3, r0, r1
	str r3, [r5, #68]
	ldr r3, [r5, #24]
.L_02008546:
	adds r3, r3, r2
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r3, [r5, #68]
	cmp r3, #0
	ble .L_0200855c
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	b .L_02008564
.L_0200855c:
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetSpritePriority
.L_02008564:
	ldr r2, [r5, #80]
	movs r1, #192
	ldrh r3, [r2, #18]
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r2, #18]
	pop {r5, pc}
	.2byte 0x0000
.L_02008574:
	.4byte 0xfffe8000
.L_02008578:
	.4byte 0xfffffae2
	.section .text.x0200857c,"ax",%progbits
	.global Func_0200057c
	.thumb_func
Func_0200057c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #208
	lsls r0, r0, #3
	sub sp, #72
	bl Runtime_BumpAllocate
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #254
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	adds r5, r0, #0
	strh r3, [r1]
	ldr r0, .L_0200864c
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0200479c
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_02008650
	adds r2, #208
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #208
	movs r2, #132
	lsls r1, r1, #2
	lsls r2, r2, #24
	adds r0, r5, r1
	adds r2, #208
	ldr r1, .L_02008654
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	bl Sys_Free
	ldr r3, .L_02008658
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r3, #128
	movs r1, #128
	lsls r3, r3, #3
	lsls r1, r1, #19
	adds r3, #13
	adds r1, #8
	strh r3, [r1]
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_02008644
	ldrh r3, [r1]
	mov r0, sp
	orrs r3, r2
	strh r3, [r1]
	ldr r3, .L_02008648
	adds r0, #70
	strh r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_0200865c
	ldr r2, .L_02008660
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #144
	lsls r0, r0, #2
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	ldr r0, .L_02008664
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_0200479c
	b .L_02008668
	.2byte 0x0000
.L_02008644:
	.4byte 0x00000001
.L_02008648:
	.4byte 0x00000010
.L_0200864c:
	.4byte 0x000001be
.L_02008650:
	.4byte 0x0600e800
.L_02008654:
	.4byte 0x0600ec00
.L_02008658:
	.4byte gPartyState
.L_0200865c:
	.4byte 0x06002000
.L_02008660:
	.4byte 0x81000280
.L_02008664:
	.4byte 0x000001bf
.L_02008668:
	movs r7, #0
	adds r4, r5, #0
.L_0200866c:
	ldrh r3, [r4]
	movs r0, #255
	lsls r0, r0, #8
	movs r2, #240
	adds r0, #64
	lsls r2, r2, #4
	adds r2, #255
	adds r3, r3, r0
	ands r3, r2
	ldr r2, .L_020086b4
	movs r1, #144
	orrs r3, r2
	adds r7, #1
	lsls r1, r1, #1
	strh r3, [r4]
	adds r4, #2
	cmp r7, r1
	bne .L_0200866c
	adds r4, r5, #0
	movs r7, #0
.L_02008694:
	ldr r2, .L_020086b8
	lsls r1, r7, #6
	adds r1, r1, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r7, #1
	adds r4, #32
	b .L_020086bc
	.2byte 0x0000
.L_020086b4:
	.4byte 0x00001000
.L_020086b8:
	.4byte 0x0600200f
.L_020086bc:
	cmp r7, #18
	bne .L_02008694
	adds r0, r5, #0
	bl Sys_Free
	movs r0, #246
	bl Engine_AudioPlayCue
	movs r0, #14
	bl Object_GetById
	movs r3, #9
	adds r0, #98
	strb r3, [r0]
	movs r0, #14
	bl Object_GetById
	movs r3, #1
	adds r0, #99
	strb r3, [r0]
	movs r0, #14
	bl Object_GetById
	ldr r3, .L_02008834
	movs r7, #0
	str r3, [r0, #108]
.L_020086f0:
	ldr r3, .L_02008838
	ldr r3, [r3]
	mov r8, r3
	mov r0, r8
	movs r3, #3
	ands r0, r3
	mov r8, r0
	cmp r0, #0
	bne .L_0200879a
	movs r0, #14
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #14
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r0, #168
	ldr r2, [r5, #12]
	ldr r1, [r6, #8]
	lsls r0, r0, #2
	bl Engine_ObjectCreate
	adds r5, r0, #0
	movs r0, #14
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r5, #80]
	ldrb r3, [r3, #9]
	movs r1, #12
	ands r1, r3
	movs r0, #13
	ldrb r3, [r4, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	orrs r3, r1
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	strb r3, [r4, #9]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	mov r1, r8
	adds r3, #85
	strb r1, [r3]
	ldr r3, [r5, #8]
	str r3, [r5, #68]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #72]
	ldr r3, [r5, #16]
	str r3, [r5, #76]
	bl Random16Far
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5, #48]
	adds r0, r5, #0
	movs r1, #7
	bl Engine_ObjectSetMode
	adds r0, r5, #0
	ldr r1, .L_0200883c
	bl Engine_ObjectSetScript
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	ldr r3, .L_02008840
	str r3, [r5, #108]
.L_0200879a:
	movs r0, #1
	adds r7, #1
	bl Battle_WaitMode0
	cmp r7, #45
	bne .L_020086f0
	ldr r3, .L_02008844
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_020049f4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_020049e4
	movs r0, #168
	movs r1, #1
	movs r2, #212
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	bl Motion_CamBounds
	bl Func_020049f4
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_02008848
	bl Scheduler_AddOrUpdateCallback
	movs r0, #207
	bl Engine_AudioPlayCue
	ldr r3, .L_02008824
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_02008828
	subs r2, #2
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200882c
	movs r0, #128
	orrs r3, r2
	strh r3, [r1]
	movs r2, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_02004864
	movs r7, #0
.L_0200881a:
	ldr r3, .L_02008830
	lsrs r2, r7, #2
	subs r3, r3, r2
	b .L_0200884c
	.2byte 0x0000
.L_02008824:
	.4byte 0x00000f00
.L_02008828:
	.4byte 0x00003f41
.L_0200882c:
	.4byte 0x00000100
.L_02008830:
	.4byte 0x0000000f
.L_02008834:
	.4byte Func_02000458
.L_02008838:
	.4byte Data_0300122c
.L_0200883c:
	.4byte Data_02004ae0
.L_02008840:
	.4byte Func_02000488
.L_02008844:
	.4byte gPartyState
.L_02008848:
	.4byte Func_02000378
.L_0200884c:
	movs r6, #128
	lsls r6, r6, #19
	lsls r3, r3, #8
	orrs r3, r2
	adds r6, #82
	strh r3, [r6]
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	movs r3, #7
	ands r0, r3
	adds r5, #98
	strb r0, [r5]
	adds r7, #1
	movs r0, #1
	bl Battle_WaitMode0
	cmp r7, #64
	bne .L_0200881a
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02004864
	ldr r3, .L_020088c8
	movs r2, #128
	strh r3, [r6]
	ldr r3, .L_020088cc
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r5, #16
	movs r0, #2
	movs r1, #83
	movs r2, #36
	movs r3, #64
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02004844
	movs r0, #64
	movs r1, #64
	movs r2, #2
	movs r3, #83
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02004844
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #10
	ldrh r2, [r1]
	b .L_020088d0
.L_020088c8:
	.4byte 0x00001000
.L_020088cc:
	.4byte 0x00003f42
.L_020088d0:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r2
	strh r3, [r1]
	movs r1, #144
	ldr r0, .L_02008924
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r7, #0
.L_020088e6:
	movs r3, #128
	ldr r6, .L_02008920
	lsls r3, r3, #19
	lsrs r2, r7, #2
	adds r3, #82
	mov r8, r3
	subs r3, r6, r2
	lsls r3, r3, #8
	orrs r3, r2
	mov r0, r8
	strh r3, [r0]
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	movs r3, #7
	ands r0, r3
	adds r5, #98
	strb r0, [r5]
	adds r7, #1
	movs r0, #1
	bl Battle_WaitMode0
	cmp r7, #68
	bne .L_020088e6
	b .L_02008928
	.2byte 0x0000
.L_02008920:
	.4byte 0x00000010
.L_02008924:
	.4byte Func_02000420
.L_02008928:
	movs r3, #13
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #90
	movs r2, #4
	movs r3, #19
	bl Func_02004844
	movs r2, #192
	movs r3, #128
	lsls r2, r2, #3
	lsls r3, r3, #19
	adds r2, #2
	adds r3, #12
	strh r2, [r3]
	adds r3, #200
	ldr r0, .L_02008998
	ldr r1, .L_0200899c
	ldr r2, .L_020089a0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	ldr r3, .L_02008994
	lsls r2, r2, #19
	mov r1, r8
	adds r2, #80
	strh r6, [r1]
	strh r3, [r2]
	movs r7, #0
.L_02008966:
	lsrs r2, r7, #2
	lsls r3, r2, #2
	subs r3, r7, r3
	lsls r0, r3, #3
	lsls r1, r2, #1
	subs r0, r0, r3
	adds r1, r1, r2
	movs r3, #13
	movs r2, #11
	lsls r0, r0, #1
	lsls r1, r1, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r0, #45
	movs r2, #4
	adds r1, #90
	movs r3, #19
	bl Func_02004844
	movs r2, #0
	mov r9, r2
	b .L_020089a4
	.2byte 0x0000
.L_02008994:
	.4byte 0x00000a44
.L_02008998:
	.4byte 0x06002800
.L_0200899c:
	.4byte 0x06003000
.L_020089a0:
	.4byte 0x84000200
.L_020089a4:
	movs r2, #3
	mov r3, r9
	ands r2, r3
	cmp r2, #0
	bne .L_02008a06
	add r0, sp, #16
	movs r3, #1
	str r3, [r0]
	ldr r3, .L_02008a44
	add r6, sp, #56
	str r3, [r0, #36]
	ldr r3, .L_02008a48
	str r2, [r6]
	str r3, [r6, #8]
	str r2, [r6, #4]
	mov r10, r0
	bl Random16Far
	movs r1, #127
	adds r5, r0, #0
	ands r5, r1
	movs r2, #208
	lsls r2, r2, #15
	lsls r5, r5, #16
	mov r8, r1
	adds r5, r5, r2
	bl Random16Far
	ldr r1, [r6, #4]
	mov r3, r8
	ands r0, r3
	ldr r3, [r6]
	str r1, [sp, #0]
	movs r2, #236
	ldr r1, [r6, #8]
	lsls r2, r2, #1
	str r1, [sp, #4]
	movs r1, #129
	lsls r1, r1, #17
	subs r2, r2, r0
	adds r1, #1
	mov r0, r10
	str r1, [sp, #8]
	str r0, [sp, #12]
	lsls r2, r2, #16
	adds r0, r5, #0
	movs r1, #0
	bl Func_020000b8
.L_02008a06:
	movs r1, #128
	ldr r6, .L_02008a40
	mov r2, r9
	lsls r1, r1, #19
	adds r1, #82
	lsrs r3, r2, #2
	mov r8, r1
	lsls r2, r3, #8
	subs r3, r6, r3
	orrs r2, r3
	mov r3, r8
	strh r2, [r3]
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	movs r3, #7
	ands r0, r3
	adds r5, #98
	strb r0, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add r9, r0
	mov r1, r9
	b .L_02008a4c
.L_02008a40:
	.4byte 0x00000010
.L_02008a44:
	.4byte Func_02000508
.L_02008a48:
	.4byte 0xfffe0000
.L_02008a4c:
	cmp r1, #64
	bne .L_020089a4
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_02008ae4
	ldr r1, .L_02008ae8
	ldr r2, .L_02008aec
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r8
	adds r7, #1
	strh r6, [r2]
	cmp r7, #8
	beq .L_02008a6c
	b .L_02008966
.L_02008a6c:
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #10
	subs r3, #200
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #251
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r2, #11
	movs r3, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #19
	movs r2, #38
	movs r0, #47
	movs r1, #19
	bl Func_02004844
	mov r3, r8
	strh r6, [r3]
	movs r2, #128
	ldr r3, .L_02008adc
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r7, #0
.L_02008aaa:
	ldr r3, .L_02008ae0
	lsrs r1, r7, #2
	movs r6, #128
	subs r3, r3, r1
	lsls r2, r1, #8
	lsls r6, r6, #19
	orrs r2, r3
	adds r6, #82
	strh r2, [r6]
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	movs r3, #7
	ands r0, r3
	adds r5, #98
	strb r0, [r5]
	adds r7, #1
	movs r0, #1
	bl Battle_WaitMode0
	b .L_02008af0
	.2byte 0x0000
.L_02008adc:
	.4byte 0x00003f42
.L_02008ae0:
	.4byte 0x00000010
.L_02008ae4:
	.4byte 0x06002800
.L_02008ae8:
	.4byte 0x06003000
.L_02008aec:
	.4byte 0x84000200
.L_02008af0:
	cmp r7, #68
	bne .L_02008aaa
	ldr r0, .L_02008b50
	bl Scheduler_RemoveCallbackFar
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #10
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_02008b44
	ldrh r3, [r1]
	movs r0, #36
	orrs r3, r2
	strh r3, [r1]
	movs r3, #16
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #64
	movs r2, #2
	movs r3, #83
	bl Func_02004844
	ldr r3, .L_02008b48
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_02008b4c
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	strh r3, [r6]
	negs r1, r1
	adds r2, #102
	negs r0, r0
	b .L_02008b54
.L_02008b44:
	.4byte 0x00000001
.L_02008b48:
	.4byte 0x00003f41
.L_02008b4c:
	.4byte 0x0000000f
.L_02008b50:
	.4byte Func_02000420
.L_02008b54:
	bl Func_02004864
	movs r0, #195
	lsls r0, r0, #1
	bl Engine_AudioPlayCue
	movs r0, #14
	bl Object_GetById
	movs r3, #6
	adds r0, #98
	strb r3, [r0]
	movs r7, #0
.L_02008b6e:
	ldr r3, .L_02008ba8
	lsrs r1, r7, #2
	movs r0, #128
	lsls r2, r1, #8
	subs r3, r3, r1
	lsls r0, r0, #19
	adds r0, #82
	orrs r2, r3
	strh r2, [r0]
	adds r7, #1
	movs r0, #1
	bl Battle_WaitMode0
	cmp r7, #65
	bne .L_02008b6e
	ldr r0, .L_02008bac
	bl Scheduler_RemoveCallbackFar
	movs r0, #14
	bl Object_GetById
	movs r5, #0
	adds r0, #99
	strb r5, [r0]
	movs r0, #30
	bl WaitFrames
	movs r0, #14
	b .L_02008bb0
.L_02008ba8:
	.4byte 0x00000010
.L_02008bac:
	.4byte Func_02000378
.L_02008bb0:
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #14
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	bl Func_0200486c
	movs r0, #60
	bl Battle_WaitMode0
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r5, [r3]
	bl Func_020047d4
	bl Func_020047bc
	ldr r6, .L_02008c6c
	movs r0, #147
	lsls r0, r0, #1
	movs r1, #128
	adds r0, #255
	lsls r1, r1, #2
	adds r3, r6, r0
	adds r1, #38
	ldrb r0, [r3]
	adds r3, r6, r1
	ldrb r1, [r3]
	bl Func_0200488c
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #223
	ands r3, r2
	strb r3, [r0]
	movs r5, #1
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #14
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #128
	orrs r5, r3
	strb r5, [r0]
	lsls r1, r1, #19
	ldr r2, .L_02008c68
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	bl Func_0200482c
	movs r0, #1
	bl WaitFrames
	ldr r0, [r6]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_020049f4
	movs r3, #6
	movs r2, #29
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #5
	movs r2, #9
	movs r3, #1
	movs r0, #1
	bl Func_0200484c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #76
	b .L_02008c70
.L_02008c68:
	.4byte 0x00000400
.L_02008c6c:
	.4byte gPartyState
.L_02008c70:
	bl GameFlag_SetBit
	add sp, #72
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.section .text.x02008c80,"ax",%progbits
	.global Func_02000c80
	.thumb_func
Func_02000c80:
	push {lr}
	ldr r3, .L_02008cd0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008cd4
	cmp r2, r3
	bne .L_02008c98
	ldr r0, .L_02008cd8
	b .L_02008ccc
.L_02008c98:
	ldr r3, .L_02008cdc
	cmp r2, r3
	bne .L_02008ca2
	ldr r0, .L_02008ce0
	b .L_02008ccc
.L_02008ca2:
	ldr r3, .L_02008ce4
	cmp r2, r3
	bne .L_02008cac
	ldr r0, .L_02008ce8
	b .L_02008ccc
.L_02008cac:
	ldr r3, .L_02008cec
	cmp r2, r3
	bne .L_02008cb6
	ldr r0, .L_02008cf0
	b .L_02008ccc
.L_02008cb6:
	ldr r3, .L_02008cf4
	cmp r2, r3
	bne .L_02008cc0
	ldr r0, .L_02008cf8
	b .L_02008ccc
.L_02008cc0:
	ldr r3, .L_02008cfc
	cmp r2, r3
	bne .L_02008cca
	ldr r0, .L_02008d00
	b .L_02008ccc
.L_02008cca:
	ldr r0, .L_02008d04
.L_02008ccc:
	pop {pc}
	.2byte 0x0000
.L_02008cd0:
	.4byte gPartyState
.L_02008cd4:
	.4byte 0x000000f1
.L_02008cd8:
	.4byte Data_02005aac
.L_02008cdc:
	.4byte 0x000000f3
.L_02008ce0:
	.4byte Data_02005bc0
.L_02008ce4:
	.4byte 0x000000f2
.L_02008ce8:
	.4byte Data_02005ca4
.L_02008cec:
	.4byte 0x000000f4
.L_02008cf0:
	.4byte Data_02005f98
.L_02008cf4:
	.4byte 0x000000f5
.L_02008cf8:
	.4byte Data_020061f0
.L_02008cfc:
	.4byte 0x000000f6
.L_02008d00:
	.4byte Data_020062d4
.L_02008d04:
	.4byte Data_02005aa0
	.section .text.x02008d08,"ax",%progbits
	.global Func_02000d08
	.thumb_func
Func_02000d08:
	push {lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_02008d24
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_020048cc
	pop {pc}
	.2byte 0x0000
.L_02008d24:
	.4byte MsgShamanEntranceLocked
	.section .text.x02008d28,"ax",%progbits
	.global Func_02000d28
	.thumb_func
Func_02000d28:
	push {lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_02008d44
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_020048cc
	pop {pc}
	.2byte 0x0000
.L_02008d44:
	.4byte MsgShamanEntranceClosed
	.section .text.x02008d48,"ax",%progbits
	.global Func_02000d48
	.thumb_func
Func_02000d48:
	push {lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_02008d64
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_020048cc
	pop {pc}
	.2byte 0x0000
.L_02008d64:
	.4byte MsgShamanTrialRoadPermissionSign
	.section .text.x02008d68,"ax",%progbits
	.global Func_02000d68
	.thumb_func
Func_02000d68:
	push {r5, lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_02008e30
	bl Func_0200498c
	movs r1, #0
	movs r0, #17
	bl UiText_OpenMessageAtObject
	ldr r3, .L_02008e34
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008e10
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	lsls r1, r1, #6
	ldr r0, .L_02008e38
	adds r1, #51
	bl Func_020049e4
	movs r0, #224
	movs r1, #1
	movs r2, #140
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02008e38
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #146
	ldr r0, [r5]
	movs r1, #246
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #146
	ldr r0, [r5]
	movs r1, #204
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #140
	lsls r2, r2, #1
	ldr r0, [r5]
	movs r1, #182
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #0
	bl Func_020049bc
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_02001380
	b .L_02008e2a
.L_02008e10:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #17
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_020049a4
.L_02008e2a:
	bl Func_020048cc
	pop {r5, pc}
.L_02008e30:
	.4byte MsgShamanGuardRepeatRulesBeforeTrial
.L_02008e34:
	.4byte gPartyState
.L_02008e38:
	.4byte 0x00019999
	.section .text.x02008e3c,"ax",%progbits
	.global Func_02000e3c
	.thumb_func
Func_02000e3c:
	push {r5, lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_02008eec
	bl Func_0200498c
	movs r1, #0
	movs r0, #16
	bl UiText_OpenMessageAtObject
	ldr r3, .L_02008ef0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008ecc
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	lsls r1, r1, #6
	ldr r0, .L_02008ef4
	adds r1, #51
	bl Func_020049e4
	movs r0, #224
	movs r1, #1
	movs r2, #140
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02008ef4
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #140
	lsls r2, r2, #1
	ldr r0, [r5]
	movs r1, #182
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #0
	bl Func_020049bc
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_02001380
	b .L_02008ee6
.L_02008ecc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #16
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_020049a4
.L_02008ee6:
	bl Func_020048cc
	pop {r5, pc}
.L_02008eec:
	.4byte MsgShamanOtherGuardRepeatRulesBeforeTrial
.L_02008ef0:
	.4byte gPartyState
.L_02008ef4:
	.4byte 0x00019999
	.section .text.x02008ef8,"ax",%progbits
	.global Func_02000ef8
	.thumb_func
Func_02000ef8:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_02008f28
	bl Func_0200498c
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r3, #173
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	bl Func_020048cc
	pop {r5, pc}
.L_02008f28:
	.4byte MsgShamanOutsidersLeaveTown
	.section .text.x02008f2c,"ax",%progbits
	.global Func_02000f2c
	.thumb_func
Func_02000f2c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_02008f5c
	bl Func_0200498c
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r3, #173
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	bl Func_020048cc
	pop {r5, pc}
.L_02008f5c:
	.4byte MsgShamanFearMoapaPower
	.section .text.x02008f60,"ax",%progbits
	.global Func_02000f60
	.thumb_func
Func_02000f60:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_02008f90
	bl Func_0200498c
	movs r0, #16
	movs r1, #0
	bl Func_020049a4
	movs r3, #173
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	bl Func_020048cc
	pop {r5, pc}
.L_02008f90:
	.4byte MsgShamanSpeakToMoapa
	.section .text.x02008f94,"ax",%progbits
	.global Func_02000f94
	.thumb_func
Func_02000f94:
	push {lr}
	ldr r3, .L_02008fbc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r2, #190
	ldrh r3, [r0, #6]
	lsls r2, r2, #7
	adds r2, #255
	adds r3, r3, r2
	ldr r2, .L_02008fc0
	lsls r3, r3, #16
	movs r0, #1
	cmp r3, r2
	bls .L_02008fba
	movs r0, #0
.L_02008fba:
	pop {pc}
.L_02008fbc:
	.4byte gPartyState
.L_02008fc0:
	.4byte 0x3ffe0000
	.section .text.x02008fc4,"ax",%progbits
	.global Func_02000fc4
	.thumb_func
Func_02000fc4:
	push {r5, r6, r7, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #254
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009004
	bl Func_02000f94
	cmp r0, #0
	beq .L_02008fe6
	movs r0, #11
	movs r1, #13
	bl Func_02004a84
	b .L_020090b4
.L_02008fe6:
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_020090b8
	bl Func_0200498c
	movs r1, #0
	movs r0, #13
	bl Func_020049ac
	bl Func_020048cc
	b .L_020090b4
.L_02009004:
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	bl Func_02000f94
	cmp r0, #0
	beq .L_020090a2
	movs r0, #11
	bl Func_02004a8c
	movs r1, #5
	adds r5, r0, #0
	movs r0, #13
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r7, .L_020090bc
	adds r0, r7, #0
	bl Func_0200498c
	adds r0, r5, #0
	movs r1, #5
	bl Func_0200487c
	adds r0, r5, #0
	movs r1, #5
	bl Func_0200487c
	movs r1, #0
	movs r0, #12
	bl UiText_OpenMessageAtObject
	ldr r6, .L_020090c0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009098
	ldr r3, [r6, #16]
	cmp r3, r5
	bcs .L_02009092
	movs r1, #6
	movs r0, #13
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #5
	movs r0, #13
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	adds r0, r7, #3
	bl Func_0200498c
	movs r0, #12
	movs r1, #0
	bl Func_020049a4
	b .L_02009098
.L_02009092:
	adds r0, r5, #0
	bl Func_02004a94
.L_02009098:
	movs r0, #13
	movs r1, #6
	bl Object_SetModeById
	b .L_020090b0
.L_020090a2:
	ldr r0, .L_020090c4
	bl Func_0200498c
	movs r0, #13
	movs r1, #0
	bl Func_020049a4
.L_020090b0:
	bl Func_020048cc
.L_020090b4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020090b8:
	.4byte MsgShamanVisitorsHaveTraveledFar
.L_020090bc:
	.4byte MsgShamanInnPaymentPrompt
.L_020090c0:
	.4byte gPartyState
.L_020090c4:
	.4byte MsgShamanSilentVillager
	.section .text.x020090c8,"ax",%progbits
	.global Func_020010c8
	.thumb_func
Func_020010c8:
	push {lr}
	bl Func_02000f94
	cmp r0, #0
	beq .L_020090da
	movs r0, #8
	bl Func_02004a7c
	b .L_0200910c
.L_020090da:
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #254
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090fa
	ldr r0, .L_02009110
	bl Func_0200498c
	b .L_02009100
.L_020090fa:
	ldr r0, .L_02009114
	bl Func_0200498c
.L_02009100:
	movs r0, #8
	movs r1, #0
	bl Func_020049a4
	bl Func_020048cc
.L_0200910c:
	pop {pc}
	.2byte 0x0000
.L_02009110:
	.4byte MsgShamanMoapaDefeatIncredible
.L_02009114:
	.4byte MsgShamanTribeForbidsTalking
	.section .text.x02009118,"ax",%progbits
	.global Func_02001118
	.thumb_func
Func_02001118:
	push {lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_02009138
	bl Func_0200498c
	movs r1, #0
	movs r0, #13
	bl Func_020049ac
	bl Func_020048cc
	pop {pc}
.L_02009138:
	.4byte MsgShamanCheckOtherMountainSide
	.section .text.x0200913c,"ax",%progbits
	.global Func_0200113c
	.thumb_func
Func_0200113c:
	push {lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #237
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200915e
	ldr r0, .L_020091b4
	bl Func_0200498c
	b .L_02009180
.L_0200915e:
	ldr r0, .L_020091b8
	bl Func_0200498c
	movs r1, #0
	movs r0, #11
	bl UiText_OpenMessageAtObject
	ldr r3, .L_020091bc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_0200918a
.L_02009180:
	movs r0, #11
	movs r1, #0
	bl Func_020049a4
	b .L_020091ae
.L_0200918a:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #11
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_020049a4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #237
	bl GameFlag_SetBit
.L_020091ae:
	bl Func_020048cc
	pop {pc}
.L_020091b4:
	.4byte MsgShamanMoapaNeverForgetsVictor
.L_020091b8:
	.4byte MsgShamanMoapaAsksVictorsNames
.L_020091bc:
	.4byte gPartyState
	.section .text.x020091c0,"ax",%progbits
	.global Func_020011c0
	.thumb_func
Func_020011c0:
	push {lr}
	bl Func_02000f94
	cmp r0, #0
	beq .L_020091d4
	movs r0, #29
	movs r1, #12
	bl Func_02004a74
	b .L_020091f0
.L_020091d4:
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_020091f4
	bl Func_0200498c
	movs r0, #12
	movs r1, #0
	bl Func_020049a4
	bl Func_020048cc
.L_020091f0:
	pop {pc}
	.2byte 0x0000
.L_020091f4:
	.4byte MsgShamanWelcomeContigoHero
	.section .text.x020091f8,"ax",%progbits
	.global Func_020011f8
	.thumb_func
Func_020011f8:
	push {lr}
	movs r0, #17
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r1, #4
	movs r2, #20
	movs r0, #17
	bl ObjectMotion_Launch
	movs r0, #229
	bl Engine_AudioPlayCue
	movs r0, #2
	bl WaitFrames
	ldr r1, .L_02009228
	movs r0, #16
	bl Object_SetActionCallbackAndRefreshById
	pop {pc}
	.2byte 0x0000
.L_02009228:
	.4byte Data_020050f8
	.section .text.x0200922c,"ax",%progbits
	.global Func_0200122c
	.thumb_func
Func_0200122c:
	push {lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200926c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200925c
	ldr r0, .L_0200936c
	bl Func_0200498c
	b .L_02009262
.L_0200925c:
	ldr r0, .L_02009370
	bl Func_0200498c
.L_02009262:
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	b .L_02009364
.L_0200926c:
	movs r0, #0
	bl Engine_AudioPlayCue
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009288
	ldr r0, .L_02009374
	bl Func_0200498c
	b .L_0200928e
.L_02009288:
	ldr r0, .L_02009378
	bl Func_0200498c
.L_0200928e:
	movs r1, #0
	movs r0, #17
	bl UiText_OpenMessageAtObject
	ldr r3, .L_0200937c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_020092b4
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	b .L_02009360
.L_020092b4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #0
	adds r3, #1
	strh r3, [r2]
	movs r0, #17
	bl Func_020049a4
	movs r0, #11
	bl Engine_AudioPlayCue
	movs r0, #17
	movs r1, #5
	bl Object_SetModeById
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	bl Func_020011f8
	movs r0, #17
	movs r1, #6
	bl Object_SetModeById
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	bl Func_020011f8
	movs r0, #17
	movs r1, #5
	bl Object_SetModeById
	movs r1, #0
	movs r0, #17
	bl Func_020049a4
	bl Func_020011f8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #1
	bl Object_SetModeById
	movs r0, #17
	movs r1, #3
	bl Object_SetModeById
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_020049d4
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r0, #17
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
.L_02009360:
	bl Func_02004a2c
.L_02009364:
	bl Func_020048cc
	pop {pc}
	.2byte 0x0000
.L_0200936c:
	.4byte MsgShamanFortuneAvoidNortheast
.L_02009370:
	.4byte MsgShamanFortuneAvoidSouth
.L_02009374:
	.4byte MsgShamanFortuneOfferAfterTrial
.L_02009378:
	.4byte MsgShamanFortuneOfferBeforeTrial
.L_0200937c:
	.4byte gPartyState
	.section .text.x02009380,"ax",%progbits
	.global Func_02001380
	.thumb_func
Func_02001380:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009396
	ldr r0, .L_02009628
	bl Func_0200498c
	b .L_02009518
.L_02009396:
	movs r0, #78
	bl Engine_AudioPlayCue
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r1, #204
	lsls r1, r1, #6
	ldr r0, .L_0200962c
	adds r1, #51
	bl Func_020049e4
	movs r0, #224
	movs r1, #1
	movs r2, #140
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	ldr r5, .L_02009630
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r2, #153
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02009634
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #140
	ldr r0, [r5]
	movs r1, #182
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	movs r2, #0
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #76
	bl Engine_AudioPlayCue
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	bl Func_020049bc
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	ldr r0, .L_02009638
	bl Func_0200498c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r1, #204
	lsls r1, r1, #7
	ldr r0, .L_0200963c
	adds r1, #102
	bl Func_020049e4
	movs r0, #224
	movs r1, #1
	movs r2, #182
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	bl Motion_CamBounds
	bl Func_020049f4
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #129
	movs r0, #15
	lsls r1, r1, #1
	bl Func_020049d4
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #224
	movs r1, #1
	movs r2, #140
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	bl Motion_CamBounds
	bl Func_020049f4
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #178
	movs r1, #1
	movs r2, #220
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_020049f4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #135
	movs r1, #1
	movs r2, #220
	lsls r2, r2, #16
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_020049f4
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #226
	movs r1, #1
	movs r2, #140
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	bl Func_020049f4
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
.L_02009518:
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #16
	movs r1, #3
	bl Object_SetModeById
	movs r0, #17
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #16
	ldr r1, .L_0200962c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #17
	ldr r1, .L_0200962c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #128
	movs r0, #16
	movs r1, #184
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #132
	movs r2, #128
	movs r0, #17
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r3, .L_02009630
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #192
	ldr r0, [r3]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #15
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_02009640
	adds r1, #204
	bl Func_020049e4
	movs r0, #224
	movs r1, #1
	movs r2, #182
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r0, #16
	movs r1, #184
	movs r2, #128
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #132
	movs r2, #128
	movs r0, #17
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #1
	movs r0, #16
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	movs r1, #0
	movs r2, #0
	movs r0, #17
	bl Func_02004934
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #158
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #28
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #10
	bl Func_02004a04
	pop {r5, pc}
.L_02009628:
	.4byte MsgShamanTrialRulesAreSimple
.L_0200962c:
	.4byte 0x00019999
.L_02009630:
	.4byte gPartyState
.L_02009634:
	.4byte 0x00013333
.L_02009638:
	.4byte MsgShamanListenCarefully
.L_0200963c:
	.4byte 0x00033333
.L_02009640:
	.4byte 0x00026666
	.section .text.x02009644,"ax",%progbits
	.global Func_02001644
	.thumb_func
Func_02001644:
	push {r5, lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r1, #204
	lsls r1, r1, #7
	ldr r0, .L_0200974c
	adds r1, #102
	bl Func_020049e4
	movs r0, #182
	movs r1, #1
	movs r2, #152
	lsls r2, r2, #17
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_020049f4
	ldr r0, .L_02009750
	bl Func_0200498c
	movs r1, #0
	movs r0, #15
	bl UiText_OpenMessageAtObject
	ldr r3, .L_02009754
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	movs r1, #224
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020096f4
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02009758
	adds r1, #153
	bl Func_020049e4
	movs r0, #168
	movs r1, #1
	movs r2, #184
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_0200975c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #184
	ldr r0, [r5]
	movs r1, #168
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_02009746
.L_020096f4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #15
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_020049a4
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_02009760
	adds r1, #102
	bl Func_020049e4
	movs r0, #168
	movs r1, #1
	movs r2, #140
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_0200975c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #140
	ldr r0, [r5]
	movs r1, #168
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
.L_02009746:
	bl Func_020048cc
	pop {r5, pc}
.L_0200974c:
	.4byte 0x00033333
.L_02009750:
	.4byte MsgShamanGiveUpTrialPrompt
.L_02009754:
	.4byte gPartyState
.L_02009758:
	.4byte 0x0004cccc
.L_0200975c:
	.4byte 0x00019999
.L_02009760:
	.4byte 0x00013333
	.section .text.x02009764,"ax",%progbits
	.global Func_02001764
	.thumb_func
Func_02001764:
	push {r5, r6, lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_02009874
	adds r1, #204
	bl Func_020049e4
	movs r0, #224
	movs r1, #1
	movs r2, #140
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	ldr r3, .L_02009878
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #140
	ldr r0, [r5]
	movs r1, #182
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #15
	bl Func_020049bc
	ldr r0, .L_0200987c
	bl Func_0200498c
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #15
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_02009810
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	bl Func_0200289c
	b .L_0200986c
.L_02009810:
	movs r6, #192
	lsls r6, r6, #18
	ldr r2, [r6, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	movs r1, #0
	strh r3, [r2]
	adds r0, #15
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200985a
	movs r0, #76
	bl Engine_AudioPlayCue
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_02001380
	b .L_0200986c
.L_0200985a:
	ldr r2, [r6, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	bl Func_0200290c
.L_0200986c:
	bl Func_020048cc
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009874:
	.4byte 0x00026666
.L_02009878:
	.4byte gPartyState
.L_0200987c:
	.4byte MsgShamanChallengeTrialRoadPrompt
	.section .text.x02009880,"ax",%progbits
	.global Func_02001880
	.thumb_func
Func_02001880:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009a90
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #15
	bl Object_GetById
	mov r8, r0
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r0, #16
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #17
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #15
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #160
	movs r2, #0
	movs r0, #15
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #6
	bl Func_020049bc
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #16
	ldr r1, .L_02009a94
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	movs r0, #15
	bl Object_GetById
	cmp r0, #0
	beq .L_02009906
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #16
	bl ObjectMotion_ResetAndSetPosition
.L_02009906:
	movs r0, #16
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl Func_02004934
	movs r1, #3
	movs r0, #15
	bl Object_SetModeById
	ldr r0, .L_02009a98
	bl Func_0200498c
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	ldr r0, [r7]
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #192
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #17
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #17
	movs r1, #2
	bl Object_SetModeById
	movs r0, #15
	bl Object_GetById
	cmp r0, #0
	beq .L_02009974
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #17
	bl ObjectMotion_ResetAndSetPosition
.L_02009974:
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r0, #17
	movs r1, #0
	bl Func_02004934
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	negs r1, r1
	bl Motion_CamBounds
	movs r1, #204
	lsls r1, r1, #6
	ldr r0, .L_02009a94
	adds r1, #51
	bl Func_020049e4
	movs r0, #224
	movs r1, #1
	movs r2, #200
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	adds r6, r5, #0
	mov r5, r8
	bl Motion_CamBounds
	adds r6, #99
	movs r3, #0
	adds r5, #99
	strb r3, [r6]
	strb r3, [r5]
	ldr r0, [r7]
	ldr r1, .L_02009a9c
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009aa0
	movs r0, #15
	bl ObjectMotion_EnableActionAndSetCallback
.L_020099e4:
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_020099e4
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_020099e4
	bl Func_02003040
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_020049e4
	movs r0, #224
	movs r1, #1
	movs r2, #168
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	ldr r5, .L_02009a90
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02009a94
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #15
	ldr r1, .L_02009a94
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #184
	movs r2, #128
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #132
	movs r2, #128
	movs r0, #15
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_020048b4
	movs r0, #11
	bl Func_02004a04
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009a90:
	.4byte gPartyState
.L_02009a94:
	.4byte 0x00019999
.L_02009a98:
	.4byte MsgShamanGuardsTakeOtherPath
.L_02009a9c:
	.4byte Data_02004c5c
.L_02009aa0:
	.4byte Data_02004cac
	.section .text.x02009aa4,"ax",%progbits
	.global Func_02001aa4
	.thumb_func
Func_02001aa4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009cb4
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #15
	bl Object_GetById
	mov r8, r0
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r0, #16
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #17
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #15
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #224
	movs r2, #0
	movs r0, #15
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #17
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #17
	ldr r1, .L_02009cb8
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #17
	movs r1, #2
	bl Object_SetModeById
	movs r0, #15
	bl Object_GetById
	cmp r0, #0
	beq .L_02009b2a
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #17
	bl ObjectMotion_ResetAndSetPosition
.L_02009b2a:
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r0, #17
	movs r1, #0
	bl Func_02004934
	movs r1, #3
	movs r0, #15
	bl Object_SetModeById
	ldr r0, .L_02009cbc
	bl Func_0200498c
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	ldr r0, [r7]
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	movs r0, #15
	bl Object_GetById
	cmp r0, #0
	beq .L_02009b98
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #16
	bl ObjectMotion_ResetAndSetPosition
.L_02009b98:
	movs r0, #16
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl Func_02004934
	movs r1, #224
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	negs r1, r1
	bl Motion_CamBounds
	movs r1, #204
	lsls r1, r1, #6
	ldr r0, .L_02009cb8
	adds r1, #51
	bl Func_020049e4
	movs r0, #224
	movs r1, #1
	movs r2, #200
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	adds r6, r5, #0
	mov r5, r8
	bl Motion_CamBounds
	adds r6, #99
	movs r3, #0
	adds r5, #99
	strb r3, [r6]
	ldr r1, .L_02009cc0
	movs r0, #15
	strb r3, [r5]
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r0, [r7]
	ldr r1, .L_02009cc4
	bl ObjectMotion_EnableActionAndSetCallback
.L_02009c08:
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_02009c08
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_02009c08
	bl Func_02003040
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_020049e4
	movs r0, #224
	movs r1, #1
	movs r2, #168
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	ldr r5, .L_02009cb4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02009cb8
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #15
	ldr r1, .L_02009cb8
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #15
	movs r1, #184
	movs r2, #128
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #132
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #1
	movs r0, #15
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_020048b4
	movs r0, #12
	bl Func_02004a04
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009cb4:
	.4byte gPartyState
.L_02009cb8:
	.4byte 0x00019999
.L_02009cbc:
	.4byte MsgShamanGuardsTakeOtherPath
.L_02009cc0:
	.4byte Data_02004c5c
.L_02009cc4:
	.4byte Data_02004cac
	.section .text.x02009cc8,"ax",%progbits
	.global Func_02001cc8
	.thumb_func
Func_02001cc8:
	push {r5, r6, r7, lr}
	movs r0, #14
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #32
	bne .L_02009d38
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #11
	bne .L_02009d38
	bl Func_020048c4
	adds r7, r5, #0
	movs r0, #0
	bl Func_02004a24
	adds r7, #85
	movs r3, #3
	strb r3, [r7]
.L_02009cf6:
	movs r0, #1
	bl WaitFrames
	ldr r6, [r5, #40]
	cmp r6, #0
	bne .L_02009cf6
	movs r0, #188
	bl Engine_AudioPlayCue
	movs r0, #10
	bl WaitFrames
	movs r3, #32
	movs r2, #11
	strb r6, [r7]
	movs r1, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #30
	bl Func_0200484c
	movs r0, #1
	bl WaitFrames
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #78
	bl GameFlag_SetBit
	bl Func_020048cc
.L_02009d38:
	add sp, #8
	pop {r5, r6, r7, pc}
	.section .text.x02009d3c,"ax",%progbits
	.global Func_02001d3c
	.thumb_func
Func_02001d3c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	sub sp, #8
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r0, #158
	bl Engine_AudioPlayCue
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #6
	movs r1, #40
	movs r2, #7
	movs r0, #43
	bl Func_02004844
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #240
	movs r2, #194
	movs r0, #15
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_02004934
	ldr r5, .L_02009f8c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	ldr r1, .L_02009f90
	ldr r2, .L_02009f94
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r6, #254
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0]
	movs r2, #0
	movs r1, #120
	mov r10, r2
	ldr r0, [r5]
	movs r2, #220
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r8, r2
	mov r2, r8
	orrs r3, r2
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	strb r3, [r0]
	adds r1, #102
	movs r0, #15
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #208
	movs r0, #15
	movs r1, #120
	bl ObjectMotion_SetPositionAndReset
	movs r0, #16
	movs r1, #15
	bl Func_02004944
	movs r1, #15
	movs r0, #17
	bl Func_02004944
	movs r0, #1
	bl WaitFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #16
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #17
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #16
	movs r1, #104
	movs r2, #212
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #212
	movs r0, #17
	movs r1, #136
	bl ObjectMotion_SetPositionAndReset
	movs r0, #16
	movs r1, #1
	bl Object_SetModeById
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #1
	movs r0, #15
	bl ObjectMotion_SetVariantCallback
	ldr r0, .L_02009f98
	bl Func_0200498c
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r1, #153
	adds r0, #204
	bl Func_020049e4
	bl Func_020049fc
	mov r3, r10
	adds r0, #85
	strb r3, [r0]
	movs r1, #200
	movs r0, #240
	movs r2, #224
	movs r3, #1
	lsls r0, r0, #15
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02009f9c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #128
	ldr r0, [r5]
	lsls r2, r2, #1
	movs r1, #120
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_020049bc
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #132
	ands r6, r3
	strb r6, [r0]
	movs r1, #120
	lsls r2, r2, #1
	ldr r0, [r5]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r2, r3
	movs r1, #4
	strb r2, [r0]
	mov r8, r2
	adds r1, #255
	movs r2, #20
	movs r0, #15
	bl Func_020049cc
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #17
	movs r1, #1
	bl ObjectMotion_SetVariantCallback
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #17
	bl Func_020049cc
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r0, #16
	movs r1, #1
	bl ObjectMotion_SetVariantCallback
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #16
	bl Func_020049cc
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #16
	movs r1, #0
	bl Func_020049a4
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #15
	bl Func_020049a4
	movs r0, #133
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_020048cc
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_02009f8c:
	.4byte gPartyState
.L_02009f90:
	.4byte 0x00026666
.L_02009f94:
	.4byte 0x00013333
.L_02009f98:
	.4byte MsgShamanMoapaGiveMeRoom
.L_02009f9c:
	.4byte 0x00019999
	.section .text.x02009fec,"ax",%progbits
	.global Func_02001fec
	.thumb_func
Func_02001fec:
	push {r5, r6, r7, lr}
	movs r0, #234
	movs r1, #240
	movs r2, #128
	movs r3, #132
	adds r0, #255
	lsls r1, r1, #15
	lsls r2, r2, #15
	lsls r3, r3, #17
	bl Engine_ObjectCreate
	adds r6, r0, #0
	movs r7, #0
	movs r0, #0
	cmp r6, #0
	beq .L_0200a064
	ldr r5, [r6, #80]
	movs r3, #33
	ldrb r2, [r5, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #9]
	adds r3, r6, #0
	adds r3, #85
	adds r2, r6, #0
	strb r7, [r3]
	adds r2, #92
	movs r3, #1
	movs r1, #193
	strb r3, [r2]
	lsls r1, r1, #3
	strb r7, [r5, #26]
	strb r7, [r5, #27]
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r7, r0, #0
	movs r0, #65
	bl Func_02004884
	movs r3, #128
	lsls r3, r3, #3
	adds r2, r7, r3
	movs r1, #128
	ldrb r0, [r5, #16]
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	adds r0, r6, #0
.L_0200a064:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a068,"ax",%progbits
	.global Func_02002068
	.thumb_func
Func_02002068:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r0, #78
	bl Engine_AudioPlayCue
	ldr r3, .L_0200a0d0
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Object_GetById
	movs r6, #192
	ldr r3, .L_0200a0cc
	lsls r6, r6, #8
	strh r6, [r0, #6]
	movs r0, #1
	mov r8, r3
	bl WaitFrames
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r1, #153
	adds r0, #204
	bl Func_020049e4
	bl Func_020049fc
	mov r2, r8
	adds r0, #85
	strb r2, [r0]
	movs r1, #160
	movs r0, #240
	movs r2, #246
	movs r3, #1
	lsls r0, r0, #15
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r2, #153
	b .L_0200a0d4
.L_0200a0cc:
	.4byte 0x00000000
.L_0200a0d0:
	.4byte gPartyState
.L_0200a0d4:
	lsls r2, r2, #8
	ldr r0, [r7]
	ldr r1, .L_0200a4d0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #132
	lsls r2, r2, #1
	ldr r0, [r7]
	movs r1, #120
	bl ObjectMotion_SetPositionAndReset
	movs r1, #28
	ldr r0, [r7]
	bl Object_SetModeById
	bl Func_02001fec
	movs r1, #128
	adds r5, r0, #0
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #15
	bl Func_020049cc
	movs r0, #15
	ldr r1, .L_0200a4d4
	ldr r2, .L_0200a4d0
	bl ObjectMotion_SetSpeedParameters
	movs r1, #120
	movs r2, #242
	movs r0, #15
	bl ObjectMotion_SetPositionAndReset
	movs r0, #29
	bl Engine_AudioPlayCue
	ldr r0, .L_0200a4d8
	bl Func_0200498c
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #20
	bl Battle_WaitMode0
	cmp r5, #0
	beq .L_0200a13e
	adds r0, r5, #0
	bl Func_0200481c
.L_0200a13e:
	ldr r0, [r7]
	movs r1, #1
	bl Object_SetModeById
	adds r1, r6, #0
	ldr r0, [r7]
	bl Func_020049bc
	ldr r1, [r7]
	movs r0, #18
	bl Func_02004944
	ldr r1, [r7]
	movs r0, #7
	bl Func_02004944
	ldr r1, [r7]
	movs r0, #5
	bl Func_02004944
	ldr r1, [r7]
	movs r0, #6
	bl Func_02004944
	movs r0, #1
	bl WaitFrames
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #18
	ldr r1, .L_0200a4dc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_0200a4dc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200a4dc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #6
	ldr r1, .L_0200a4dc
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a4e0
	movs r0, #18
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a4e4
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a4e8
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a4ec
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #17
	bl Func_020049cc
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #17
	ldr r1, .L_0200a4d0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #232
	movs r0, #17
	movs r1, #136
	bl ObjectMotion_SetPositionAndReset
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #16
	ldr r1, .L_0200a4d0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #232
	movs r0, #16
	movs r1, #104
	bl ObjectMotion_SetPositionAndReset
	movs r0, #16
	movs r1, #0
	bl Func_020049a4
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #15
	bl Func_020049cc
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #18
	bl Func_020049d4
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	bl Func_020049a4
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #15
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #120
	movs r2, #252
	movs r0, #15
	bl ObjectMotion_SetPositionAndReset
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #15
	bl Func_020049cc
	movs r0, #15
	movs r1, #136
	movs r2, #252
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #15
	bl Func_020049cc
	movs r0, #15
	movs r1, #104
	movs r2, #252
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #15
	bl Func_020049cc
	movs r0, #15
	movs r1, #152
	movs r2, #252
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #15
	bl Func_020049cc
	movs r0, #15
	movs r1, #88
	movs r2, #252
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #80
	movs r0, #15
	bl Func_020049cc
	movs r0, #15
	movs r1, #120
	movs r2, #252
	bl ObjectMotion_SetPositionAndReset
	movs r2, #242
	movs r0, #15
	movs r1, #120
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #15
	movs r1, #4
	bl Object_SetModeById
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r7]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #18
	bl Func_020049cc
	movs r2, #0
	movs r0, #18
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #18
	movs r1, #0
	bl Func_020049a4
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #15
	bl Func_020049cc
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	adds r1, r6, #0
	movs r0, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	movs r0, #5
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	ldr r0, [r7]
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	adds r1, r6, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #15
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #6
	bl Func_020049cc
	movs r0, #6
	movs r1, #0
	bl Func_020049a4
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #5
	bl Func_020049cc
	movs r0, #5
	movs r1, #0
	bl Func_020049a4
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_020049cc
	movs r0, #7
	movs r1, #0
	bl Func_020049a4
	movs r1, #128
	movs r2, #0
	movs r0, #15
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #129
	movs r0, #18
	lsls r1, r1, #1
	bl Func_020049d4
	movs r0, #18
	movs r1, #0
	bl Func_020049a4
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #15
	bl Func_020049cc
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #131
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #17
	bl Func_020049cc
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #15
	bl Func_020049cc
	movs r1, #224
	movs r2, #20
	movs r0, #15
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #17
	movs r1, #3
	bl Object_SetModeById
	movs r5, #160
	movs r0, #17
	movs r1, #0
	movs r6, #242
	bl Func_020049a4
	lsls r5, r5, #15
	lsls r6, r6, #16
	movs r0, #232
	movs r3, #224
	adds r1, r5, #0
	lsls r3, r3, #8
	adds r2, r6, #0
	lsls r0, r0, #15
	bl DriftScene_SpawnObject
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #248
	movs r3, #128
	b .L_0200a4f0
.L_0200a4d0:
	.4byte 0x00013333
.L_0200a4d4:
	.4byte 0x00026666
.L_0200a4d8:
	.4byte MsgShamanRecognizeShamanRod
.L_0200a4dc:
	.4byte 0x00019999
.L_0200a4e0:
	.4byte Data_02004cfc
.L_0200a4e4:
	.4byte Data_02004d40
.L_0200a4e8:
	.4byte Data_02004d84
.L_0200a4ec:
	.4byte Data_02004dc8
.L_0200a4f0:
	lsls r3, r3, #6
	adds r1, r5, #0
	adds r2, r6, #0
	lsls r0, r0, #15
	bl DriftScene_SpawnObject
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #131
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #16
	bl Func_020049cc
	movs r0, #16
	movs r1, #0
	bl Func_020049bc
	movs r0, #16
	movs r1, #0
	bl Func_020049a4
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #17
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #132
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #15
	bl Func_020049cc
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_020049e4
	movs r0, #180
	movs r1, #128
	adds r2, r6, #0
	lsls r1, r1, #14
	movs r3, #1
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r0, #16
	bl Object_GetById
	adds r5, r0, #0
	mov r3, r8
	adds r5, #99
	strb r3, [r5]
	ldr r0, [r7]
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #18
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #15
	ldr r1, .L_0200a780
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #16
	ldr r1, .L_0200a780
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #17
	ldr r1, .L_0200a780
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a784
	movs r0, #15
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	ldr r1, .L_0200a788
	movs r0, #17
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #16
	ldr r1, .L_0200a78c
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #78
	bl Engine_AudioPlayCue
.L_0200a622:
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_0200a622
	ldr r5, .L_0200a790
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #18
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #7
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	bl Func_02004a2c
	movs r1, #204
	lsls r1, r1, #7
	ldr r0, .L_0200a794
	adds r1, #102
	bl Func_020049e4
	movs r0, #240
	movs r1, #1
	movs r2, #130
	movs r3, #1
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_020049f4
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl Func_02004934
	movs r0, #6
	movs r1, #0
	bl Func_020049bc
	movs r0, #6
	movs r1, #0
	bl Func_020049a4
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_020049a4
	movs r0, #18
	movs r1, #0
	bl Func_020049a4
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #18
	movs r1, #0
	bl Func_020049a4
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	bl Func_020049bc
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r2, #128
	movs r0, #6
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #18
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_0200a798
	movs r0, #6
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #18
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #225
	bl GameFlag_SetBit
	bl Func_020048cc
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200a780:
	.4byte 0x00019999
.L_0200a784:
	.4byte Data_02004e0c
.L_0200a788:
	.4byte Data_02004e5c
.L_0200a78c:
	.4byte Data_02004eac
.L_0200a790:
	.4byte gPartyState
.L_0200a794:
	.4byte 0x00033333
.L_0200a798:
	.4byte Data_02004f08
	.section .text.x0200a79c,"ax",%progbits
	.global Func_0200279c
	.thumb_func
Func_0200279c:
	push {lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_0200a7bc
	bl Func_0200498c
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	bl Func_020048cc
	pop {pc}
.L_0200a7bc:
	.4byte MsgShamanShowMeSomething
	.section .text.x0200a7c0,"ax",%progbits
	.global Func_020027c0
	.thumb_func
Func_020027c0:
	push {lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_0200a7e0
	bl Func_0200498c
	movs r0, #16
	movs r1, #0
	bl Func_020049a4
	bl Func_020048cc
	pop {pc}
.L_0200a7e0:
	.4byte MsgShamanCannotBuyGift
	.section .text.x0200a7e4,"ax",%progbits
	.global Func_020027e4
	.thumb_func
Func_020027e4:
	push {lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_0200a804
	bl Func_0200498c
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	bl Func_020048cc
	pop {pc}
.L_0200a804:
	.4byte MsgShamanShowItToMoapa
	.section .text.x0200a808,"ax",%progbits
	.global Func_02002808
	.thumb_func
Func_02002808:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #254
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a86c
	movs r0, #0
	bl Engine_AudioPlayCue
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_0200a870
	bl Func_0200498c
	movs r0, #17
	movs r1, #5
	bl Object_SetModeById
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	ldr r3, .L_0200a874
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #192
	ldr r0, [r3]
	movs r2, #20
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	bl Func_020011f8
	movs r0, #17
	movs r1, #1
	bl Object_SetModeById
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_020048cc
.L_0200a86c:
	pop {pc}
	.2byte 0x0000
.L_0200a870:
	.4byte MsgShamanFortuneGreeting
.L_0200a874:
	.4byte gPartyState
	.section .text.x0200a878,"ax",%progbits
	.global Func_02002878
	.thumb_func
Func_02002878:
	push {lr}
	sub sp, #8
	movs r2, #17
	movs r3, #2
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r0, #0
	str r3, [sp, #0]
	bl Func_0200484c
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x0200a89c,"ax",%progbits
	.global Func_0200289c
	.thumb_func
Func_0200289c:
	push {r5, lr}
	ldr r5, .L_0200a904
	movs r3, #133
	lsls r3, r3, #2
	movs r2, #204
	adds r5, r5, r3
	lsls r2, r2, #8
	ldr r0, [r5]
	adds r2, #204
	ldr r1, .L_0200a908
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r2, #152
	ldr r0, [r5]
	movs r1, #168
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #184
	ldr r0, [r5]
	movs r1, #168
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	pop {r5, pc}
.L_0200a904:
	.4byte gPartyState
.L_0200a908:
	.4byte 0x00019999
	.section .text.x0200a90c,"ax",%progbits
	.global Func_0200290c
	.thumb_func
Func_0200290c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, .L_0200aa18
	sub sp, #20
	bl Func_0200498c
	movs r0, #65
	bl PartyInventory_FindOwner
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_0200a9aa
	mov r0, sp
	bl Party_ListActiveOwners
	movs r3, #0
	movs r2, #8
	mov r9, r0
	mov r8, r3
	mov r7, sp
	mov r10, r2
	cmp r8, r9
	bge .L_0200a992
.L_0200a942:
	movs r1, #0
	ldrsh r5, [r7, r1]
	adds r7, #2
	adds r0, r5, #0
	bl Owner_GetState
	adds r6, r0, #0
	movs r5, #0
	adds r6, #216
.L_0200a954:
	cmp r5, #14
	bgt .L_0200a984
	ldrh r0, [r6]
	cmp r0, #0
	beq .L_0200a984
	bl Func_02004894
	ldrb r3, [r0, #2]
	movs r2, #192
	adds r3, #255
	lsls r3, r3, #24
	lsls r2, r2, #18
	cmp r3, r2
	bhi .L_0200a97a
	mov r3, r10
	adds r3, #255
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r10, r3
.L_0200a97a:
	mov r3, r10
	adds r6, #2
	adds r5, #1
	cmp r3, #0
	bne .L_0200a954
.L_0200a984:
	mov r1, r10
	cmp r1, #0
	beq .L_0200a992
	movs r2, #1
	add r8, r2
	cmp r8, r9
	blt .L_0200a942
.L_0200a992:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r3, r1
	ldrh r3, [r2]
	mov r1, r10
	adds r3, #1
	strh r3, [r2]
	cmp r1, #0
	beq .L_0200a9c8
.L_0200a9aa:
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #15
	bl Func_020049cc
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	bl Func_0200289c
	b .L_0200aa0a
.L_0200a9c8:
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	ldr r5, .L_0200aa1c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r1, [r5]
	movs r0, #16
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r5]
	movs r0, #17
	bl Object_LinkObjectAndSetCallback
	ldr r1, [r5]
	movs r0, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #158
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200aa0a:
	add sp, #20
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200aa18:
	.4byte MsgShamanMissingShamanRod
.L_0200aa1c:
	.4byte gPartyState
	.section .text.x0200aa20,"ax",%progbits
	.global Func_02002a20
	.thumb_func
Func_02002a20:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200ab8c
	sub sp, #24
	str r0, [sp, #8]
	str r0, [sp, #12]
	ldr r3, .L_0200ab90
	ldr r2, .L_0200ab94
	movs r1, #0
	ldrsh r3, [r3, r1]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	mov r10, r3
.L_0200aa48:
	ldr r1, .L_0200ab98
	movs r2, #0
	ldrsh r5, [r1, r2]
	ldrh r3, [r1]
	cmp r5, #0
	bne .L_0200ab28
	ldr r4, .L_0200ab9c
	movs r2, #128
	ldr r0, [r4]
	lsls r2, r2, #6
	ldrh r3, [r0]
	adds r0, #2
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r0, [r4]
	cmp r3, r2
	beq .L_0200aaee
	cmp r3, r2
	bgt .L_0200aa80
	movs r7, #1
	negs r7, r7
	cmp r3, r7
	beq .L_0200ab16
	movs r1, #128
	lsls r1, r1, #5
	cmp r3, r1
	beq .L_0200aad4
	b .L_0200aa48
.L_0200aa80:
	movs r2, #128
	lsls r2, r2, #7
	cmp r3, r2
	beq .L_0200aaa2
	cmp r3, r2
	bgt .L_0200aa96
	movs r2, #192
	lsls r2, r2, #6
	cmp r3, r2
	beq .L_0200aaba
	b .L_0200aa48
.L_0200aa96:
	movs r5, #254
	lsls r5, r5, #7
	adds r5, #255
	cmp r3, r5
	beq .L_0200ab0c
	b .L_0200aa48
.L_0200aaa2:
	movs r7, #0
	ldrsh r3, [r0, r7]
	ldr r2, .L_0200aba0
	lsls r3, r3, #8
	str r3, [r2]
	adds r3, r0, #2
	ldrh r2, [r3]
	adds r3, #2
	ldr r1, .L_0200aba4
	str r3, [r4]
	ldr r3, .L_0200aba8
	b .L_0200ab06
.L_0200aaba:
	ldr r2, .L_0200aba4
	ldr r1, .L_0200abac
	ldrh r3, [r2]
	strh r3, [r1]
	ldr r1, .L_0200aba8
	ldrh r3, [r0]
	strh r3, [r2]
	adds r3, r0, #2
	ldrh r2, [r3]
	adds r3, #2
	str r3, [r4]
	ldr r3, .L_0200abb0
	b .L_0200ab06
.L_0200aad4:
	ldr r2, .L_0200abb4
	ldr r1, .L_0200abb8
	ldrh r3, [r2]
	strh r3, [r1]
	ldr r1, .L_0200abbc
	ldrh r3, [r0]
	strh r3, [r2]
	adds r3, r0, #2
	ldrh r2, [r3]
	adds r3, #2
	str r3, [r4]
	ldr r3, .L_0200abc0
	b .L_0200ab06
.L_0200aaee:
	ldr r2, .L_0200abc4
	ldr r1, .L_0200abc8
	ldrh r3, [r2]
	strh r3, [r1]
	ldr r1, .L_0200abcc
	ldrh r3, [r0]
	strh r3, [r2]
	adds r3, r0, #2
	ldrh r2, [r3]
	adds r3, #2
	str r3, [r4]
	ldr r3, .L_0200abd0
.L_0200ab06:
	strh r2, [r1]
	strh r5, [r3]
	b .L_0200aa48
.L_0200ab0c:
	ldrh r3, [r0]
	strh r3, [r1]
	adds r3, r0, #2
	str r3, [r4]
	b .L_0200aa48
.L_0200ab16:
	ldr r0, .L_0200abd4
	bl Scheduler_RemoveCallbackFar
	ldr r3, .L_0200ab90
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Resource_ResetEntry
	b .L_0200aef8
.L_0200ab28:
	subs r3, #1
	ldr r2, .L_0200abbc
	strh r3, [r1]
	mov r8, r2
	movs r3, #0
	ldrsh r7, [r2, r3]
	ldr r2, .L_0200abb4
	cmp r7, #0
	bne .L_0200ab42
	movs r5, #0
	ldrsh r4, [r2, r5]
	str r4, [sp, #0]
	b .L_0200ab72
.L_0200ab42:
	ldr r3, .L_0200abb8
	movs r0, #0
	ldrsh r6, [r3, r0]
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, .L_0200abc0
	subs r3, r3, r6
	ldrh r5, [r2]
	adds r1, r7, #0
	adds r5, #1
	strh r5, [r2]
	lsls r5, r5, #16
	asrs r5, r5, #16
	adds r0, r5, #0
	muls r0, r3
	bl __divsi3
	adds r6, r6, r0
	str r6, [sp, #0]
	cmp r5, r7
	blt .L_0200ab72
	ldr r3, .L_0200ab88
	mov r2, r8
	strh r3, [r2]
.L_0200ab72:
	ldr r3, .L_0200abcc
	ldr r2, .L_0200abc4
	movs r4, #0
	ldrsh r7, [r3, r4]
	mov r8, r3
	cmp r7, #0
	bne .L_0200abd8
	movs r7, #0
	ldrsh r5, [r2, r7]
	str r5, [sp, #4]
	b .L_0200ac08
.L_0200ab88:
	.4byte 0x00000000
.L_0200ab8c:
	.4byte Data_020064d0
.L_0200ab90:
	.4byte Data_02005158
.L_0200ab94:
	.4byte ResourceTableEntries
.L_0200ab98:
	.4byte Data_020064bc
.L_0200ab9c:
	.4byte Data_020064c0
.L_0200aba0:
	.4byte Data_020064a0
.L_0200aba4:
	.4byte Data_02006504
.L_0200aba8:
	.4byte Data_0200649c
.L_0200abac:
	.4byte Data_02006500
.L_0200abb0:
	.4byte Data_020064a8
.L_0200abb4:
	.4byte Data_020064a4
.L_0200abb8:
	.4byte Data_02006498
.L_0200abbc:
	.4byte Data_02006490
.L_0200abc0:
	.4byte Data_02006508
.L_0200abc4:
	.4byte Data_020064b4
.L_0200abc8:
	.4byte Data_020064b8
.L_0200abcc:
	.4byte Data_020064c4
.L_0200abd0:
	.4byte Data_020064ac
.L_0200abd4:
	.4byte Func_02002a20
.L_0200abd8:
	ldr r3, .L_0200ac2c
	movs r0, #0
	ldrsh r6, [r3, r0]
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, .L_0200ac30
	subs r3, r3, r6
	ldrh r5, [r2]
	adds r1, r7, #0
	adds r5, #1
	strh r5, [r2]
	lsls r5, r5, #16
	asrs r5, r5, #16
	adds r0, r5, #0
	muls r0, r3
	bl __divsi3
	adds r6, r6, r0
	str r6, [sp, #4]
	cmp r5, r7
	blt .L_0200ac08
	ldr r3, .L_0200ac28
	mov r2, r8
	strh r3, [r2]
.L_0200ac08:
	ldr r3, .L_0200ac34
	ldr r0, [sp, #0]
	movs r4, #0
	ldrsh r7, [r3, r4]
	add r5, sp, #16
	lsls r0, r0, #16
	mov r11, r3
	ldr r2, .L_0200ac38
	mov r8, r5
	mov r9, r0
	cmp r7, #0
	bne .L_0200ac3c
	movs r1, #0
	ldrsh r6, [r2, r1]
	b .L_0200ac6a
	.2byte 0x0000
.L_0200ac28:
	.4byte 0x00000000
.L_0200ac2c:
	.4byte Data_020064b8
.L_0200ac30:
	.4byte Data_020064ac
.L_0200ac34:
	.4byte Data_0200649c
.L_0200ac38:
	.4byte Data_02006504
.L_0200ac3c:
	ldr r3, .L_0200aca4
	adds r1, r7, #0
	movs r4, #0
	ldrsh r6, [r3, r4]
	movs r5, #0
	ldrsh r3, [r2, r5]
	ldr r2, .L_0200aca8
	subs r3, r3, r6
	ldrh r5, [r2]
	adds r5, #1
	strh r5, [r2]
	lsls r5, r5, #16
	asrs r5, r5, #16
	adds r0, r5, #0
	muls r0, r3
	bl __divsi3
	adds r6, r6, r0
	cmp r5, r7
	blt .L_0200ac6a
	ldr r3, .L_0200aca0
	mov r7, r11
	strh r3, [r7]
.L_0200ac6a:
	mov r0, r8
	ldr r3, [r0, #4]
	ldr r2, .L_0200acac
	ands r3, r2
	str r3, [r0, #4]
	mov r3, r9
	lsrs r1, r3, #16
	ldr r3, [sp, #16]
	ands r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	orrs r3, r1
	ands r3, r2
	lsls r1, r1, #16
	orrs r3, r1
	str r3, [sp, #16]
	bl AffineMatrix_BuildForEffect
	ldr r2, .L_0200acb0
	lsls r0, r0, #16
	ldr r3, [r2]
	asrs r0, r0, #16
	adds r3, r3, r6
	mov r8, r0
	str r3, [r2]
	b .L_0200acb4
.L_0200aca0:
	.4byte 0x00000000
.L_0200aca4:
	.4byte Data_02006500
.L_0200aca8:
	.4byte Data_020064a8
.L_0200acac:
	.4byte 0xffff0000
.L_0200acb0:
	.4byte Data_020064a0
.L_0200acb4:
	cmp r3, #0
	bge .L_0200acba
	adds r3, #255
.L_0200acba:
	asrs r6, r3, #8
	ldr r3, .L_0200af08
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #2
	bne .L_0200acc8
	b .L_0200ae34
.L_0200acc8:
	cmp r3, #2
	bgt .L_0200acd2
	cmp r3, #1
	beq .L_0200acdc
	b .L_0200ae8e
.L_0200acd2:
	cmp r3, #3
	beq .L_0200ad5a
	cmp r3, #4
	beq .L_0200adda
	b .L_0200ae8e
.L_0200acdc:
	movs r5, #0
.L_0200acde:
	ldr r7, [sp, #0]
	lsls r3, r5, #5
	subs r3, #48
	muls r3, r7
	movs r1, #56
	ldr r0, .L_0200af0c
	cmp r3, #0
	bge .L_0200acf0
	adds r3, #255
.L_0200acf0:
	asrs r3, r3, #8
	adds r3, r6, r3
	movs r4, #48
	adds r2, r3, #0
	adds r4, #255
	adds r3, #152
	adds r2, #88
	cmp r3, r4
	bhi .L_0200ad4e
	movs r7, #0
	mov r12, r7
	ldr r7, [sp, #12]
	mov r4, r12
	stmia r7!, {r4}
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r2, r3
	adds r3, r7, #0
	str r3, [sp, #12]
	lsls r3, r2, #16
	orrs r3, r1
	orrs r3, r0
	mov r0, r8
	lsls r2, r0, #25
	orrs r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	adds r2, r7, #0
	stmia r2!, {r3}
	movs r3, #244
	lsls r3, r3, #8
	mov r4, r10
	adds r1, r2, #0
	orrs r3, r4
	str r1, [sp, #12]
	stmia r2!, {r3}
	ldr r0, [sp, #8]
	adds r7, r2, #0
	adds r1, r0, #0
	adds r1, #12
	str r1, [sp, #8]
	movs r1, #236
	str r7, [sp, #12]
	bl Func_020047cc
.L_0200ad4e:
	movs r2, #8
	adds r5, #1
	add r10, r2
	cmp r5, #3
	ble .L_0200acde
	b .L_0200ae8e
.L_0200ad5a:
	movs r5, #0
.L_0200ad5c:
	ldr r4, [sp, #0]
	lsls r3, r5, #5
	subs r3, #16
	muls r3, r4
	movs r1, #48
	ldr r0, .L_0200af0c
	cmp r3, #0
	bge .L_0200ad6e
	adds r3, #255
.L_0200ad6e:
	asrs r3, r3, #8
	adds r3, r6, r3
	movs r7, #48
	adds r2, r3, #0
	adds r7, #255
	adds r3, #152
	adds r2, #88
	cmp r3, r7
	bhi .L_0200adce
	movs r3, #128
	ldr r7, [sp, #12]
	lsls r3, r3, #1
	adds r3, #255
	ands r2, r3
	movs r3, #0
	stmia r7!, {r3}
	lsls r3, r2, #16
	orrs r3, r1
	orrs r3, r0
	mov r0, r8
	lsls r2, r0, #25
	orrs r3, r2
	movs r2, #224
	lsls r2, r2, #3
	adds r4, r7, #0
	orrs r3, r2
	adds r2, r7, #0
	str r4, [sp, #12]
	stmia r2!, {r3}
	ldr r3, .L_0200af10
	adds r1, r2, #0
	movs r4, #0
	ldrsh r3, [r3, r4]
	movs r2, #244
	add r3, r10
	lsls r2, r2, #8
	adds r0, r1, #0
	orrs r3, r2
	stmia r0!, {r3}
	adds r7, r0, #0
	ldr r0, [sp, #8]
	str r7, [sp, #12]
	adds r1, r0, #0
	adds r1, #12
	str r1, [sp, #8]
	movs r1, #236
	bl Func_020047cc
.L_0200adce:
	movs r2, #8
	adds r5, #1
	add r10, r2
	cmp r5, #1
	ble .L_0200ad5c
	b .L_0200ae8e
.L_0200adda:
	adds r3, r6, #0
	movs r4, #152
	adds r2, r6, #0
	adds r3, #120
	lsls r4, r4, #1
	movs r1, #48
	ldr r0, .L_0200af14
	adds r2, #56
	cmp r3, r4
	bcs .L_0200ae8e
	ldr r5, [sp, #8]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	adds r4, r5, #0
	ands r2, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r2, #16
	orrs r3, r1
	mov r5, r8
	orrs r3, r0
	lsls r2, r5, #25
	orrs r3, r2
	movs r2, #224
	lsls r2, r2, #3
	adds r7, r4, #0
	orrs r3, r2
	str r7, [sp, #12]
	stmia r4!, {r3}
	ldr r3, .L_0200af10
	adds r7, r4, #0
	str r7, [sp, #12]
	movs r2, #244
	movs r0, #0
	ldrsh r3, [r3, r0]
	lsls r2, r2, #8
	add r3, r10
	orrs r3, r2
	str r3, [r4]
	ldr r0, [sp, #8]
	movs r1, #236
	bl Func_020047cc
	b .L_0200ae8e
.L_0200ae34:
	adds r3, r6, #0
	movs r4, #152
	movs r0, #128
	adds r2, r6, #0
	adds r3, #152
	lsls r4, r4, #1
	movs r1, #48
	lsls r0, r0, #24
	adds r2, #88
	cmp r3, r4
	bcs .L_0200ae8e
	ldr r5, [sp, #8]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	adds r4, r5, #0
	ands r2, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r2, #16
	orrs r3, r1
	mov r5, r8
	orrs r3, r0
	lsls r2, r5, #25
	orrs r3, r2
	movs r2, #224
	lsls r2, r2, #3
	adds r7, r4, #0
	orrs r3, r2
	str r7, [sp, #12]
	stmia r4!, {r3}
	ldr r3, .L_0200af10
	adds r7, r4, #0
	str r7, [sp, #12]
	movs r2, #244
	movs r0, #0
	ldrsh r3, [r3, r0]
	lsls r2, r2, #8
	add r3, r10
	orrs r3, r2
	str r3, [r4]
	ldr r0, [sp, #8]
	movs r1, #236
	bl Func_020047cc
.L_0200ae8e:
	ldr r0, .L_0200af18
	ldr r1, .L_0200af1c
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_0200aec0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	strh r2, [r0]
	movs r2, #252
	adds r3, #4
	lsls r2, r2, #6
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0200aec0:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r3, [r0]
	cmp r3, #31
	bgt .L_0200aef6
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r0]
	ldr r5, [sp, #4]
	movs r3, #16
	lsls r2, r2, #2
	subs r3, r3, r5
	adds r2, r2, r0
	lsls r3, r3, #8
	adds r2, #4
	orrs r3, r5
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_0200aef6:
	strh r4, [r1]
.L_0200aef8:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200af08:
	.4byte Data_020064b0
.L_0200af0c:
	.4byte 0x80004000
.L_0200af10:
	.4byte Data_02006494
.L_0200af14:
	.4byte 0xc0004000
.L_0200af18:
	.4byte gIoWriteQueue
.L_0200af1c:
	.4byte 0x04000208
	.section .text.x0200af20,"ax",%progbits
	.global Func_02002f20
	.thumb_func
Func_02002f20:
	push {r5, r6, lr}
	ldr r3, .L_0200af58
	ldr r2, .L_0200af5c
	adds r5, r0, #0
	adds r6, r1, #0
	strh r5, [r3]
	movs r1, #144
	lsls r3, r6, #4
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_0200af60
	bl Scheduler_AddOrUpdateCallback
	ldr r1, .L_0200af64
	cmp r5, #2
	bne .L_0200af44
	ldr r1, .L_0200af68
	b .L_0200af76
.L_0200af44:
	cmp r5, #4
	bne .L_0200af4c
	ldr r1, .L_0200af6c
	b .L_0200af76
.L_0200af4c:
	cmp r5, #3
	bne .L_0200af76
	cmp r6, #0
	beq .L_0200af74
	ldr r1, .L_0200af70
	b .L_0200af76
.L_0200af58:
	.4byte Data_020064b0
.L_0200af5c:
	.4byte Data_02006494
.L_0200af60:
	.4byte Func_02002a20
.L_0200af64:
	.4byte Data_0200515a
.L_0200af68:
	.4byte Data_02004b58
.L_0200af6c:
	.4byte Data_02005186
.L_0200af70:
	.4byte Data_02004b80
.L_0200af74:
	ldr r1, .L_0200af94
.L_0200af76:
	ldr r2, .L_0200af90
	ldr r3, .L_0200af98
	strh r2, [r3]
	ldr r3, .L_0200af9c
	str r1, [r3]
	ldr r3, .L_0200afa0
	strh r2, [r3]
	ldr r3, .L_0200afa4
	strh r2, [r3]
	ldr r2, .L_0200afa8
	movs r3, #0
	str r3, [r2]
	b .L_0200afac
.L_0200af90:
	.4byte 0x00000000
.L_0200af94:
	.4byte Data_02005204
.L_0200af98:
	.4byte Data_020064bc
.L_0200af9c:
	.4byte Data_020064c0
.L_0200afa0:
	.4byte Data_02006504
.L_0200afa4:
	.4byte Data_0200649c
.L_0200afa8:
	.4byte Data_020064a0
.L_0200afac:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200afb0,"ax",%progbits
	.global Func_02002fb0
	.thumb_func
Func_02002fb0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #(224 + SHAMAN_PALETTE_ROWS)
	lsls r0, r0, #5
	bl Runtime_BumpAllocateAlternatePool
	ldr r7, .L_0200b030
	adds r6, r0, #0
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_0200afd6
	bl Resource_FindFreeEntry
	strh r0, [r7]
.L_0200afd6:
	ldr r3, .L_0200b034
	ldrb r3, [r3, r5]
	mov r8, r3
	cmp r5, #8
	bne .L_0200afe2
	movs r5, #4
.L_0200afe2:
	ldr r0, .L_0200b038
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_0200479c
	mov r2, r8
	adds r0, r6, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r1, .L_0200b03c
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	lsls r2, r5, #10
	adds r2, r2, r6
	movs r1, #128
	adds r2, #(32 * SHAMAN_PALETTE_ROWS)
	movs r3, #0
	ldrsh r0, [r7, r3]
	lsls r1, r1, #3
	bl VramBlock_LoadCached
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #212
.L_0200b01c:
	ldr r3, [r2, #8]
	cmp r3, #0
	blt .L_0200b01c
	adds r0, r6, #0
	bl Sys_Free
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b030:
	.4byte Data_02005158
.L_0200b034:
	.4byte Data_02004bfe
.L_0200b038:
	.4byte 0x000001cc
.L_0200b03c:
	.4byte 0x050003e0
	.section .text.x0200b040,"ax",%progbits
	.global Func_02003040
	.thumb_func
Func_02003040:
	push {lr}
	ldr r2, .L_0200b0b8
	movs r3, #180
	strh r3, [r2, #30]
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #78
	bl Engine_AudioPlayCue
	movs r0, #5
	bl Func_02002fb0
	movs r1, #0
	movs r0, #2
	bl Func_02002f20
	movs r0, #236
	bl Engine_AudioPlayCue
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #2
	bl Func_02002f20
	movs r0, #236
	bl Engine_AudioPlayCue
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #6
	bl Func_02002fb0
	movs r1, #0
	movs r0, #2
	bl Func_02002f20
	movs r0, #236
	bl Engine_AudioPlayCue
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #7
	bl Func_02002fb0
	movs r1, #0
	movs r0, #4
	bl Func_02002f20
	movs r0, #237
	bl Engine_AudioPlayCue
	movs r0, #76
	bl Engine_AudioPlayCue
	pop {pc}
.L_0200b0b8:
	.4byte Data_0200515a
	.section .text.x0200b0bc,"ax",%progbits
	.global Func_020030bc
	.thumb_func
Func_020030bc:
	push {r5, r6, r7, lr}
	movs r7, #192
	lsls r7, r7, #18
	adds r3, r7, #0
	adds r3, #224
	movs r0, #16
	ldr r5, [r3]
	bl Object_GetById
	adds r2, r5, #0
	movs r3, #0
	adds r2, #34
	adds r5, #35
	strb r3, [r2]
	strb r3, [r5]
	adds r6, r0, #0
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r0, #0
	bl Engine_AudioPlayCue
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	negs r1, r1
	bl Motion_CamBounds
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_020049e4
	movs r0, #168
	movs r1, #1
	movs r2, #132
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	bl Motion_CamBounds
	ldr r5, .L_0200b51c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #134
	lsls r2, r2, #2
	ldr r0, [r5]
	movs r1, #168
	bl ObjectMotion_SetPositionAndReset
	ldr r1, [r5]
	movs r0, #18
	bl Func_02004944
	ldr r1, [r5]
	movs r0, #7
	bl Func_02004944
	ldr r1, [r5]
	movs r0, #5
	bl Func_02004944
	ldr r1, [r5]
	movs r0, #7
	bl Func_02004944
	ldr r1, [r5]
	movs r0, #6
	bl Func_02004944
	movs r0, #1
	bl WaitFrames
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #128
	movs r0, #18
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	ldr r0, [r5]
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	ldr r1, .L_0200b520
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200b524
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200b528
	movs r0, #18
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200b52c
	movs r0, #7
	bl Object_SetActionCallbackAndRefreshById
	movs r1, #0
	movs r0, #5
	bl Func_020049bc
	ldr r0, .L_0200b530
	bl Func_0200498c
	movs r0, #5
	movs r1, #0
	bl Func_020049a4
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #6
	movs r1, #0
	bl Func_020049a4
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #6
	movs r1, #0
	bl Func_020049bc
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #6
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #131
	movs r0, #6
	movs r1, #168
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #204
	lsls r1, r1, #6
	adds r1, #51
	ldr r0, .L_0200b534
	bl Func_020049e4
	bl Func_02003b24
	movs r0, #29
	bl Engine_AudioPlayCue
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200b538
	adds r1, #153
	bl Func_020049e4
	movs r0, #168
	movs r1, #1
	movs r2, #135
	movs r3, #1
	lsls r2, r2, #18
	negs r1, r1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_020049f4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #15
	bl Func_020049d4
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #129
	movs r0, #17
	lsls r1, r1, #1
	bl Func_020049d4
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r1, #129
	movs r0, #16
	lsls r1, r1, #1
	bl Func_020049d4
	movs r0, #128
	lsls r0, r0, #8
	movs r2, #20
	adds r0, #16
	movs r1, #0
	bl Func_0200499c
	movs r0, #15
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #15
	bl Func_020049cc
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r2, #0
	movs r0, #6
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_020049a4
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_020049a4
	movs r1, #224
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #15
	movs r1, #4
	bl Object_SetModeById
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #5
	bl Func_020049cc
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	bl Func_020049bc
	movs r0, #5
	movs r1, #0
	bl Func_020049a4
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #18
	bl Func_020049cc
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #18
	movs r1, #0
	bl Func_020049a4
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #17
	bl Func_020049cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #16
	bl Func_020049cc
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #17
	movs r1, #1
	bl ObjectMotion_SetVariantCallback
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r0, #16
	movs r1, #1
	bl ObjectMotion_SetVariantCallback
	movs r0, #16
	movs r1, #0
	bl Func_020049a4
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_020049cc
	movs r1, #192
	movs r2, #20
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #6
	movs r1, #0
	bl Func_020049a4
	movs r1, #129
	movs r0, #15
	lsls r1, r1, #1
	bl Func_020049d4
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_020049cc
	movs r0, #6
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #6
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #6
	movs r1, #0
	bl Func_020049a4
	movs r1, #160
	movs r0, #17
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #17
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r1, #224
	movs r0, #16
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #16
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #16
	movs r1, #0
	bl Func_020049a4
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #17
	bl Func_020049bc
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #3
	bl Object_SetModeById
	movs r0, #17
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	b .L_0200b53c
.L_0200b51c:
	.4byte gPartyState
.L_0200b520:
	.4byte Data_02004f80
.L_0200b524:
	.4byte Data_02004fc4
.L_0200b528:
	.4byte Data_02004f3c
.L_0200b52c:
	.4byte Data_02005008
.L_0200b530:
	.4byte MsgShamanCheerForCompanion
.L_0200b534:
	.4byte 0x00019999
.L_0200b538:
	.4byte 0x0004cccc
.L_0200b53c:
	adds r1, #204
	bl Func_020049e4
	movs r0, #168
	movs r1, #1
	movs r2, #140
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	bl Motion_CamBounds
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #7
	bl Func_020049bc
	movs r1, #128
	movs r2, #128
	movs r0, #17
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #140
	movs r0, #16
	movs r1, #120
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #140
	lsls r2, r2, #2
	movs r0, #17
	movs r1, #216
	bl ObjectMotion_SetPositionAndReset
	movs r0, #17
	movs r1, #1
	bl Object_SetModeById
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #16
	movs r1, #0
	bl Func_020049bc
	movs r2, #140
	movs r0, #16
	movs r1, #152
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #140
	lsls r2, r2, #2
	movs r0, #17
	movs r1, #184
	bl ObjectMotion_SetPositionAndReset
	movs r0, #16
	movs r1, #1
	bl Object_SetModeById
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	bl Func_020049bc
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #15
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #15
	movs r1, #2
	bl Object_SetModeById
	movs r0, #17
	movs r1, #2
	bl Object_SetModeById
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	movs r0, #15
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r2, #16
	movs r1, #0
	movs r0, #16
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #15
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #15
	movs r1, #1
	bl Object_SetModeById
	movs r0, #17
	movs r1, #1
	bl Object_SetModeById
	movs r0, #16
	movs r1, #1
	bl Object_SetModeById
	movs r1, #192
	movs r0, #18
	lsls r1, r1, #6
	bl Func_020049bc
	movs r0, #18
	movs r1, #0
	bl Func_020049a4
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #18
	bl Func_020049cc
	movs r0, #18
	movs r1, #0
	bl Func_020049a4
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #17
	bl Func_020049cc
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #16
	bl Func_020049cc
	movs r1, #132
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #5
	bl Func_020049cc
	movs r0, #5
	movs r1, #0
	bl Func_020049a4
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	bl Func_020049bc
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	bl Func_020049a4
	movs r1, #224
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #4
	adds r1, #255
	movs r2, #20
	movs r0, #15
	bl Func_020049cc
	movs r1, #4
	movs r2, #20
	adds r1, #255
	movs r0, #6
	bl Func_020049cc
	movs r0, #6
	movs r1, #0
	bl Func_020049a4
	movs r1, #192
	movs r2, #20
	movs r0, #15
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #4
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #131
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #17
	bl Func_020049cc
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r1, #224
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #6
	bl Func_020049bc
	movs r0, #16
	movs r1, #0
	bl Func_020049a4
	movs r1, #129
	movs r0, #15
	lsls r1, r1, #1
	bl Func_020049d4
	movs r1, #160
	movs r2, #20
	movs r0, #15
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	bl Func_020049bc
	movs r2, #10
	movs r0, #17
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #17
	movs r1, #0
	bl Func_020049a4
	movs r0, #16
	movs r1, #0
	bl Func_020049bc
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #6
	bl Func_020049bc
	movs r0, #16
	movs r1, #0
	bl Func_020049a4
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	bl Func_020049bc
	movs r0, #5
	movs r1, #0
	bl Func_020049a4
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #0
	bl Func_020049a4
	movs r1, #129
	movs r0, #15
	lsls r1, r1, #1
	bl Func_020049d4
	movs r1, #192
	movs r2, #40
	movs r0, #15
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #15
	bl Func_020049bc
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #15
	bl Func_020049bc
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #15
	bl Func_020049cc
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #15
	movs r1, #4
	bl Object_SetModeById
	movs r1, #0
	movs r0, #15
	bl UiText_OpenMessageAtObject
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	adds r6, #99
	ldr r5, [r5]
	cmp r0, #0
	bne .L_0200b8ec
	adds r0, r5, #0
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #15
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
	b .L_0200b920
.L_0200b8ec:
	adds r0, r5, #0
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #15
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #15
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_020049a4
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r0, #15
	movs r1, #0
	bl Func_020049a4
.L_0200b920:
	movs r2, #128
	lsls r2, r2, #2
	movs r0, #15
	movs r1, #212
	adds r2, #70
	bl ObjectMotion_SetPositionAndReset
	movs r0, #168
	movs r1, #1
	movs r2, #250
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	ldr r3, .L_0200bb14
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #18
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	ldr r5, .L_0200bb18
	movs r3, #0
	strb r3, [r6]
	adds r1, r5, #0
	movs r0, #15
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #17
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #16
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #78
	bl Engine_AudioPlayCue
.L_0200b99a:
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_0200b99a
	ldr r6, .L_0200bb14
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #18
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #7
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	bl Func_02004a2c
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200bb1c
	adds r1, #153
	bl Func_020049e4
	movs r0, #168
	movs r1, #1
	movs r2, #135
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r5, #128
	movs r0, #20
	bl Battle_WaitMode0
	lsls r5, r5, #7
	movs r1, #224
	movs r2, #140
	adds r3, r5, #0
	movs r0, #15
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_0200493c
	movs r1, #208
	movs r2, #132
	adds r3, r5, #0
	movs r0, #16
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_0200493c
	movs r1, #240
	movs r2, #132
	adds r3, r5, #0
	movs r0, #17
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_0200493c
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #6
	bl Func_020049bc
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #18
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #7
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	ldr r0, [r6]
	bl Object_GetById
	movs r5, #0
	str r5, [r0, #108]
	movs r0, #18
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #5
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #7
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #6
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #6
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_0200bb20
	movs r0, #7
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #18
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #76
	bl GameFlag_SetBit
	bl Func_020048cc
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bb14:
	.4byte gPartyState
.L_0200bb18:
	.4byte Data_0200504c
.L_0200bb1c:
	.4byte 0x0004cccc
.L_0200bb20:
	.4byte Data_020050bc
	.section .text.x0200bb24,"ax",%progbits
	.global Func_02003b24
	.thumb_func
Func_02003b24:
	push {lr}
	movs r1, #1
	movs r0, #6
	bl ObjectMotion_SetActionVariant
	movs r0, #131
	bl Engine_AudioPlayCue
	bl Func_02004a54
	movs r0, #6
	bl Object_GetById
	movs r1, #2
	bl Func_02004a6c
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #6
	bl Object_GetById
	movs r1, #0
	bl Func_02004a6c
	bl Func_02004a64
	bl Func_02004a5c
	bl Func_0200057c
	movs r0, #6
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	pop {pc}
	.section .text.x0200bb6c,"ax",%progbits
	.global Func_02003b6c
	.thumb_func
Func_02003b6c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r10, r3
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #8
	ldr r3, [r3, #108]
	ldr r6, [sp, #36]
	adds r5, r0, #0
	adds r7, r1, #0
	mov r8, r2
	mov r9, r3
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	cmp r6, #2
	bne .L_0200bba0
	movs r0, #188
	bl Engine_AudioPlayCue
	b .L_0200bba6
.L_0200bba0:
	movs r0, #158
	bl Engine_AudioPlayCue
.L_0200bba6:
	ldr r3, [sp, #40]
	adds r0, r5, #0
	str r3, [sp, #4]
	adds r1, r7, #0
	mov r2, r8
	mov r3, r10
	str r6, [sp, #0]
	bl Func_02004844
	movs r0, #20
	bl Battle_WaitMode0
	ldr r3, .L_0200bc50
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	cmp r6, #1
	bne .L_0200bc10
	ldr r0, [r5]
	bl Object_GetById
	adds r5, r0, #0
	ldr r0, [r5, #8]
	asrs r2, r0, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0200bbec
	adds r3, #15
.L_0200bbec:
	asrs r3, r3, #4
	lsls r3, r3, #4
	subs r3, r2, r3
	movs r1, #8
	subs r1, r1, r3
	lsls r1, r1, #16
	adds r1, r1, r0
	ldr r3, [r5, #16]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02004834
	adds r0, r5, #0
	bl Func_0200483c
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r5, #6]
.L_0200bc10:
	ldr r3, .L_0200bc50
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	ldr r1, .L_0200bc54
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #12
	bl WaitFrames
	movs r0, #123
	bl Engine_AudioPlayCue
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02004a04
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bc50:
	.4byte gPartyState
.L_0200bc54:
	.4byte Data_02004c08
	.section .text.x0200bc58,"ax",%progbits
	.global Func_02003c58
	.thumb_func
Func_02003c58:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #40
	movs r2, #8
	movs r3, #22
	bl Func_02003b6c
	add sp, #8
	pop {pc}
	.section .text.x0200bc74,"ax",%progbits
	.global Func_02003c74
	.thumb_func
Func_02003c74:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #42
	movs r2, #4
	movs r3, #26
	bl Func_02003b6c
	add sp, #8
	pop {pc}
	.section .text.x0200bc90,"ax",%progbits
	.global Func_02003c90
	.thumb_func
Func_02003c90:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #42
	movs r2, #27
	movs r3, #21
	bl Func_02003b6c
	add sp, #8
	pop {pc}
	.section .text.x0200bcac,"ax",%progbits
	.global Func_02003cac
	.thumb_func
Func_02003cac:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #40
	movs r2, #28
	movs r3, #17
	bl Func_02003b6c
	add sp, #8
	pop {pc}
	.section .text.x0200bcc8,"ax",%progbits
	.global Func_02003cc8
	.thumb_func
Func_02003cc8:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #42
	movs r2, #10
	movs r3, #8
	bl Func_02003b6c
	add sp, #8
	pop {pc}
	.section .text.x0200bce4,"ax",%progbits
	.global Func_02003ce4
	.thumb_func
Func_02003ce4:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bd16
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r0, #123
	bl Engine_AudioPlayCue
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #13
	bl Func_02004a04
	b .L_0200bd2a
.L_0200bd16:
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #40
	movs r2, #7
	movs r3, #6
	bl Func_02003b6c
.L_0200bd2a:
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200bd30,"ax",%progbits
	.global Func_02003d30
	.thumb_func
Func_02003d30:
	push {r5, r6, lr}
	movs r0, #28
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	movs r1, #0
	movs r2, #0
	movs r0, #17
	bl Func_02004934
	movs r0, #15
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r1, #1
	movs r0, #224
	movs r2, #182
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #0
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r0, #1
	bl WaitFrames
	movs r5, #128
	bl Func_0200482c
	movs r0, #1
	bl WaitFrames
	lsls r5, r5, #7
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #184
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #16
	lsls r2, r2, #16
	adds r3, r5, #0
	bl Func_0200493c
	movs r1, #132
	movs r2, #128
	adds r3, r5, #0
	movs r0, #17
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200493c
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #16
	ldr r1, .L_0200bf6c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #17
	ldr r1, .L_0200bf6c
	bl ObjectMotion_SetSpeedParameters
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_020049e4
	movs r0, #224
	movs r1, #1
	movs r2, #140
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r2, #128
	movs r0, #16
	movs r1, #184
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #132
	movs r2, #128
	movs r0, #17
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r3, .L_0200bf70
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	movs r1, #224
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #132
	movs r0, #16
	movs r1, #208
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #132
	lsls r2, r2, #1
	movs r0, #17
	movs r1, #240
	bl ObjectMotion_SetPositionAndReset
	movs r1, #1
	movs r0, #16
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	adds r1, r5, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #17
	adds r1, r5, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020049bc
	movs r2, #0
	movs r1, #0
	ldr r0, [r6]
	bl ObjectMotion_ArmCallback
	bl Func_020049f4
	ldr r0, .L_0200bf74
	bl Func_0200498c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #15
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_0200bef4
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #15
	bl UiText_OpenMessageAtObject
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200bee2
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #76
	bl Engine_AudioPlayCue
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_02001380
	b .L_0200bf6a
.L_0200bee2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	b .L_0200bf04
.L_0200bef4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
.L_0200bf04:
	strh r3, [r2]
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #15
	bl UiText_OpenMessageAtObject
	ldr r3, .L_0200bf70
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_0200bf38
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	bl Func_0200289c
	b .L_0200bf5a
.L_0200bf38:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #15
	movs r1, #0
	bl Func_020049a4
	bl Func_0200290c
.L_0200bf5a:
	bl Func_02004a2c
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_020048cc
.L_0200bf6a:
	pop {r5, r6, pc}
.L_0200bf6c:
	.4byte 0x00019999
.L_0200bf70:
	.4byte gPartyState
.L_0200bf74:
	.4byte MsgShamanMustFinishTrialRoad
	.section .text.x0200bf78,"ax",%progbits
	.global Func_02003f78
	.thumb_func
Func_02003f78:
	push {r5, lr}
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	ldr r0, .L_0200bff4
	bl Func_0200498c
	movs r0, #15
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #8
	strh r5, [r0, #6]
	movs r0, #16
	bl Object_GetById
	strh r5, [r0, #6]
	movs r0, #17
	bl Object_GetById
	movs r1, #1
	strh r5, [r0, #6]
	movs r2, #140
	movs r0, #224
	lsls r2, r2, #17
	movs r3, #0
	negs r1, r1
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r0, #1
	bl WaitFrames
	bl Func_0200482c
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #15
	bl Func_020049a4
	bl Func_0200289c
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_020048cc
	pop {r5, pc}
.L_0200bff4:
	.4byte MsgShamanMoapaAdvisesLeaving
	.section .text.x0200bff8,"ax",%progbits
	.global Func_02003ff8
	.thumb_func
Func_02003ff8:
	push {lr}
	ldr r3, .L_0200c044
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200c048
	cmp r2, r3
	bne .L_0200c012
	bl Func_02004060
	b .L_0200c040
.L_0200c012:
	ldr r3, .L_0200c04c
	cmp r2, r3
	bne .L_0200c01e
	bl Func_020041b8
	b .L_0200c040
.L_0200c01e:
	ldr r3, .L_0200c050
	cmp r2, r3
	bne .L_0200c02a
	bl Func_020043f8
	b .L_0200c040
.L_0200c02a:
	ldr r3, .L_0200c054
	cmp r2, r3
	bne .L_0200c036
	bl Func_02004534
	b .L_0200c040
.L_0200c036:
	ldr r3, .L_0200c058
	cmp r2, r3
	bne .L_0200c040
	bl Func_020046ec
.L_0200c040:
	movs r0, #0
	pop {pc}
.L_0200c044:
	.4byte gPartyState
.L_0200c048:
	.4byte 0x000000f1
.L_0200c04c:
	.4byte 0x000000f3
.L_0200c050:
	.4byte 0x000000f2
.L_0200c054:
	.4byte 0x000000f4
.L_0200c058:
	.4byte 0x000000f6
	.section .text.x0200c060,"ax",%progbits
	.global Func_02004060
	.thumb_func
Func_02004060:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #76
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c0a8
	movs r3, #9
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #19
	movs r2, #38
	movs r3, #19
	bl Func_02004844
	movs r3, #6
	movs r2, #29
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #5
	movs r0, #1
	movs r2, #9
	movs r3, #1
	bl Func_0200484c
	movs r0, #14
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200c0ee
.L_0200c0a8:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c0ee
	movs r5, #128
	lsls r5, r5, #7
	movs r1, #168
	movs r2, #142
	movs r0, #15
	lsls r1, r1, #16
	lsls r2, r2, #18
	adds r3, r5, #0
	bl Func_0200493c
	movs r1, #216
	movs r2, #134
	movs r0, #17
	lsls r1, r1, #16
	lsls r2, r2, #18
	adds r3, r5, #0
	bl Func_0200493c
	movs r1, #240
	movs r2, #134
	movs r0, #16
	lsls r1, r1, #15
	lsls r2, r2, #18
	adds r3, r5, #0
	bl Func_0200493c
	movs r0, #1
	bl WaitFrames
.L_0200c0ee:
	movs r0, #8
	movs r1, #2
	bl Object_SetModeById
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r0, #10
	movs r1, #2
	bl Object_SetModeById
	movs r0, #11
	movs r1, #2
	bl Object_SetModeById
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
	movs r0, #13
	movs r1, #2
	bl Object_SetModeById
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c148
	ldr r3, .L_0200c1b4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #11
	bne .L_0200c140
	bl Func_02003d30
	b .L_0200c148
.L_0200c140:
	cmp r3, #9
	bne .L_0200c148
	bl Func_02003f78
.L_0200c148:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #254
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c1ae
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	movs r3, #10
	str r3, [sp, #0]
	movs r5, #13
	movs r0, #10
	movs r1, #12
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl Func_0200484c
	movs r3, #15
	str r3, [sp, #0]
	movs r0, #15
	movs r1, #12
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl Func_0200484c
	movs r3, #9
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #9
	movs r1, #20
	movs r2, #3
	movs r3, #1
	bl Func_0200484c
.L_0200c1ae:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_0200c1b4:
	.4byte gPartyState
	.section .text.x0200c1b8,"ax",%progbits
	.global Func_020041b8
	.thumb_func
Func_020041b8:
	push {lr}
	movs r0, #8
	movs r1, #2
	sub sp, #8
	bl Object_SetModeById
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r0, #10
	movs r1, #2
	bl Object_SetModeById
	movs r0, #11
	movs r1, #2
	bl Object_SetModeById
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
	movs r0, #13
	movs r1, #2
	bl Object_SetModeById
	movs r0, #14
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #78
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c23c
	movs r0, #14
	bl Object_GetById
	movs r1, #130
	movs r2, #128
	movs r3, #184
	lsls r1, r1, #18
	lsls r2, r2, #14
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r3, #32
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_0200484c
	movs r0, #10
	bl WaitFrames
.L_0200c23c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #225
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c270
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #76
	movs r3, #2
	bl Func_02004844
	movs r3, #12
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #5
	movs r2, #3
	movs r3, #1
	bl Func_0200484c
.L_0200c270:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #77
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c294
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #40
	movs r2, #101
	movs r3, #11
	bl Func_02004844
	b .L_0200c29e
.L_0200c294:
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02004934
.L_0200c29e:
	movs r0, #16
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #89
	movs r3, #8
	strb r3, [r2]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r0, #24]
	str r3, [r0, #28]
	movs r0, #16
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200c2c8,"ax",%progbits
	.global Func_020042c8
	.thumb_func
Func_020042c8:
	push {lr}
	adds r1, r0, #0
	adds r1, #100
	movs r3, #0
	ldrsh r2, [r1, r3]
	ldr r3, [r0, #8]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #12]
	movs r2, #160
	ldr r3, [r0, #24]
	lsls r2, r2, #3
	adds r2, #30
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	ldrh r3, [r1]
	adds r3, #2
	strh r3, [r1]
	ldr r3, [r0, #104]
	subs r3, #1
	str r3, [r0, #104]
	cmp r3, #0
	bne .L_0200c30a
	bl Func_0200481c
.L_0200c30a:
	pop {pc}
	.section .text.x0200c30c,"ax",%progbits
	.global Func_0200430c
	.thumb_func
Func_0200430c:
	push {r5, r6, r7, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	movs r0, #30
	adds r3, r2, #0
	adds r0, #255
	adds r1, r4, #0
	adds r2, r5, #0
	bl Engine_ObjectCreate
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200c38c
	adds r2, r6, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
	adds r3, r6, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r3]
	adds r7, r6, #0
	adds r3, #15
	strh r1, [r3]
	adds r7, #35
	ldrb r2, [r7]
	movs r3, #254
	ands r3, r2
	strb r3, [r7]
	movs r1, #7
	bl Object_SetPartAttribute
	ldr r1, [r6, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	ldrb r3, [r7]
	movs r5, #2
	orrs r5, r3
	adds r0, r6, #0
	movs r1, #0
	strb r5, [r7]
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #24]
	str r3, [r6, #28]
	movs r3, #60
	str r3, [r6, #104]
	ldr r3, .L_0200c390
	adds r0, r6, #0
	movs r1, #5
	str r3, [r6, #108]
	bl Engine_ObjectSetMode
	adds r0, r6, #0
	movs r1, #1
	bl Animation_SetStateFlags
.L_0200c38c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c390:
	.4byte Func_020042c8
	.section .text.x0200c394,"ax",%progbits
	.global Func_02004394
	.thumb_func
Func_02004394:
	push {lr}
	ldr r3, .L_0200c3c4
	movs r2, #63
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200c3c2
	movs r0, #144
	movs r1, #132
	movs r2, #188
	lsls r0, r0, #15
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_0200430c
	movs r0, #208
	movs r1, #196
	movs r2, #152
	lsls r0, r0, #15
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_0200430c
.L_0200c3c2:
	pop {pc}
.L_0200c3c4:
	.4byte Data_0300122c
	.section .text.x0200c3c8,"ax",%progbits
	.global Func_020043c8
	.thumb_func
Func_020043c8:
	push {r5, lr}
	ldr r3, .L_0200c3f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r0, #21
	ldrb r5, [r3, #9]
	lsls r5, r5, #28
	lsrs r5, r5, #30
	adds r1, r5, #0
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	adds r1, r5, #0
	bl ObjectMotion_SetActionVariant
	pop {r5, pc}
	.2byte 0x0000
.L_0200c3f4:
	.4byte gPartyState
	.section .text.x0200c3f8,"ax",%progbits
	.global Func_020043f8
	.thumb_func
Func_020043f8:
	push {r5, r6, lr}
	movs r0, #10
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c480
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #254
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c434
	movs r1, #208
	movs r2, #200
	movs r0, #16
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_02004934
	movs r1, #136
	movs r2, #200
	movs r0, #17
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02004934
	b .L_0200c480
.L_0200c434:
	movs r0, #133
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c480
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #225
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c480
	movs r1, #240
	movs r2, #208
	movs r0, #15
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_02004934
	movs r1, #208
	movs r2, #212
	movs r0, #16
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_02004934
	movs r1, #136
	movs r2, #212
	movs r0, #17
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02004934
	movs r0, #1
	bl WaitFrames
.L_0200c480:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c4a2
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #43
	movs r1, #40
	movs r2, #7
	movs r3, #6
	bl Func_02004844
.L_0200c4a2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #77
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c4d4
	movs r5, #1
	movs r6, #2
	movs r0, #48
	movs r1, #40
	movs r2, #77
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004844
	movs r0, #48
	movs r1, #40
	movs r2, #81
	movs r3, #25
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004844
.L_0200c4d4:
	bl Func_02004a44
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #19
	movs r3, #20
	bl Func_02004a4c
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c4f8
	bl Func_02002878
.L_0200c4f8:
	ldr r3, .L_0200c520
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200c524
	subs r2, #2
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200c528
	bl Scheduler_AddOrUpdateCallback
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200c52c
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	b .L_0200c530
.L_0200c520:
	.4byte 0x00000c08
.L_0200c524:
	.4byte 0x00003f10
.L_0200c528:
	.4byte Func_020043c8
.L_0200c52c:
	.4byte Func_02004394
.L_0200c530:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200c534,"ax",%progbits
	.global Func_02004534
	.thumb_func
Func_02004534:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	movs r0, #12
	bl Object_GetById
	movs r3, #8
	adds r0, #89
	strb r3, [r0]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #254
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c582
	movs r3, #192
	movs r1, #176
	movs r2, #184
	movs r0, #8
	lsls r1, r1, #15
	lsls r2, r2, #15
	lsls r3, r3, #6
	bl Func_0200493c
	movs r1, #156
	movs r2, #144
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl Func_02004934
	b .L_0200c5a0
.L_0200c582:
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02004934
.L_0200c5a0:
	ldr r5, .L_0200c6b0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #30
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200c6ae
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c6ae
	movs r0, #1
	bl Func_020048ac
	bl Func_020048c4
	movs r0, #0
	bl Func_02004a24
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #41
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #20
	bl Battle_WaitMode0
	movs r5, #0
	b .L_0200c606
.L_0200c604:
	adds r5, #1
.L_0200c606:
	cmp r5, #59
	bhi .L_0200c618
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0200c6b4
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200c604
.L_0200c618:
	ldr r3, .L_0200c6b0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #42
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r5, #0
	b .L_0200c634
.L_0200c632:
	adds r5, #1
.L_0200c634:
	cmp r5, #29
	bhi .L_0200c646
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0200c6b4
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200c632
.L_0200c646:
	ldr r6, .L_0200c6b0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r6, r3
	movs r2, #0
	ldr r0, [r5]
	movs r1, #4
	bl ObjectMotion_Launch
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_0200c6b8
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #192
	lsls r1, r1, #2
	movs r2, #120
	adds r1, #45
	ldr r0, [r5]
	bl ObjectMotion_SetPositionAndCommit
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	bl Func_020048cc
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #30
	bne .L_0200c6ae
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #254
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #77
	bl GameFlag_SetBit
.L_0200c6ae:
	pop {r5, r6, pc}
.L_0200c6b0:
	.4byte gPartyState
.L_0200c6b4:
	.4byte gInput
.L_0200c6b8:
	.4byte 0x00019999
	.section .text.x0200c6bc,"ax",%progbits
	.global Func_020046bc
	.thumb_func
Func_020046bc:
	push {r5, lr}
	ldr r3, .L_0200c6e8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r0, #9
	ldrb r5, [r3, #9]
	lsls r5, r5, #28
	lsrs r5, r5, #30
	adds r1, r5, #0
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	adds r1, r5, #0
	bl ObjectMotion_SetActionVariant
	pop {r5, pc}
	.2byte 0x0000
.L_0200c6e8:
	.4byte gPartyState
	.section .text.x0200c6ec,"ax",%progbits
	.global Func_020046ec
	.thumb_func
Func_020046ec:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r0, #144
	adds r3, r3, r2
	lsls r0, r0, #4
	adds r2, #93
	str r2, [r3]
	adds r0, #254
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c718
	movs r1, #144
	ldr r0, .L_0200c730
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_0200c72c
.L_0200c718:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02004934
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02004934
.L_0200c72c:
	pop {pc}
	.2byte 0x0000
.L_0200c730:
	.4byte Func_020046bc
	.section .rodata.x0200caa4,"a",%progbits
.L_0200caa4:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02004ae0
Data_02004ae0:
.L_0200cae0:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200cb1c:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02004b58
Data_02004b58:
	.4byte 0x02001000
	.4byte 0x20000001
	.4byte 0x00010000
	.4byte 0x01001000
	.4byte 0x20000006
	.4byte 0x00060010
	.4byte 0x001e7fff
	.4byte 0x00002000
	.4byte 0x7fff001e
	.4byte 0xffff001e
	.global Data_02004b80
Data_02004b80:
	.4byte 0x00801000
	.4byte 0x20000001
	.4byte 0x00010000
	.4byte 0x01001000
	.4byte 0x20000006
	.4byte 0x00060010
	.4byte 0x003c7fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600f0
	.4byte 0x00067fff
	.4byte 0x01701000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600e0
	.4byte 0x00067fff
	.4byte 0x00002000
	.4byte 0x1000001e
	.4byte 0x00060160
	.4byte 0x00067fff
	.4byte 0x00d01000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060150
	.4byte 0x00067fff
	.4byte 0x00c01000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060140
	.4byte 0x00067fff
	.2byte 0xffff
	.global Data_02004bfe
Data_02004bfe:
	.2byte 0x2000
	.4byte 0x40602020
	.byte 64, SHAMAN_GRAPHIC7_PALETTE, 128, 0
	.global Data_02004c08
Data_02004c08:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02004c50
Data_02004c50:
	.4byte .L_0200caa4
	.4byte .L_0200cae0
	.4byte .L_0200cb1c
	.global Data_02004c5c
Data_02004c5c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02004cac
Data_02004cac:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02004cfc
Data_02004cfc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02004d40
Data_02004d40:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02004d84
Data_02004d84:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02004dc8
Data_02004dc8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02004e0c
Data_02004e0c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02004e5c
Data_02004e5c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02004eac
Data_02004eac:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02004f08
Data_02004f08:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.global Data_02004f3c
Data_02004f3c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02004f80
Data_02004f80:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02004fc4
Data_02004fc4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02005008
Data_02005008:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_0200504c
Data_0200504c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02340000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d60000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01b20000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020050bc
Data_020050bc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020050f8
Data_020050f8:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0xc0010000
	.4byte 0x00000011
	.global Data_02005158
Data_02005158:
	.2byte 0xffff
	.global Data_0200515a
Data_0200515a:
	.2byte 0x4000
	.4byte 0x0800ff44
	.4byte 0x01001000
	.4byte 0x20000001
	.4byte 0x00010010
	.4byte 0x000e7fff
	.4byte 0x00003000
	.4byte 0x7fff0014
	.4byte 0x3000003c
	.4byte 0x00140800
	.4byte 0x003c7fff
	.2byte 0xffff
	.global Data_02005186
Data_02005186:
	.2byte 0x1000
	.4byte 0x00010200
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.global Data_02005204
Data_02005204:
	.4byte 0x00801000
	.4byte 0x20000001
	.4byte 0x00010000
	.4byte 0x01001000
	.4byte 0x20000006
	.4byte 0x00060010
	.4byte 0x003c7fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060110
	.4byte 0x00067fff
	.4byte 0x01901000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060120
	.4byte 0x00067fff
	.4byte 0x00002000
	.4byte 0x1000001e
	.4byte 0x000601a0
	.4byte 0x00067fff
	.4byte 0x01301000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601b0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601c0
	.4byte 0x00067fff
	.4byte 0x0000ffff
	.global Data_02005284
Data_02005284:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020052b4
Data_020052b4:
	.4byte 0xffff000f
	.4byte 0x00000220
	.4byte 0xc00002a8
	.4byte 0x01a40000
	.4byte 0x02940230
	.4byte 0x000002d0
	.4byte 0xffff0010
	.4byte 0x000002f0
	.4byte 0xc00002a8
	.4byte 0x02760000
	.4byte 0x03660230
	.4byte 0x000002d0
	.4byte 0xffff0011
	.4byte 0x00000220
	.4byte 0xc0000388
	.4byte 0x01a40000
	.4byte 0x0294030c
	.4byte 0x000003ac
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005314
Data_02005314:
	.4byte 0xffff0006
	.4byte 0x000002d0
	.4byte 0xc0000220
	.4byte 0x02580000
	.4byte 0x0348017c
	.4byte 0x00000258
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x000000f1
	.4byte 0x0011f0f7
	.4byte 0x002200f7
	.4byte 0x003010f3
	.4byte 0x00a320f7
	.4byte 0x00b1f0f7
	.4byte 0x00c200f7
	.4byte 0x000000f3
	.4byte 0x001030f1
	.4byte 0x002080f2
	.4byte 0x0030b0f2
	.4byte 0x004110f5
	.4byte 0x0050e0f2
	.4byte 0x006090f2
	.4byte 0x000000f2
	.4byte 0x0013a002
	.4byte 0x0020f0f5
	.4byte 0x003100f5
	.4byte 0x004060f4
	.4byte 0x005080f4
	.4byte 0x0060b0f4
	.4byte 0x0070d0f4
	.4byte 0x008020f3
	.4byte 0x009060f3
	.4byte 0x00a060f6
	.4byte 0x00b030f3
	.4byte 0x00c030f4
	.4byte 0x00d010f4
	.4byte 0x00e050f3
	.4byte 0x000000f4
	.4byte 0x0010d0f2
	.4byte 0x002040f4
	.4byte 0x0030c0f2
	.4byte 0x004020f4
	.4byte 0x005090f4
	.4byte 0x006040f2
	.4byte 0x0070a0f4
	.4byte 0x008050f2
	.4byte 0x009050f4
	.4byte 0x00a070f4
	.4byte 0x00b060f2
	.4byte 0x00c0e0f4
	.4byte 0x00d070f2
	.4byte 0x00e0c0f4
	.4byte 0x000000f5
	.4byte 0x00f020f2
	.4byte 0x010030f2
	.4byte 0x011040f3
	.4byte 0x000000f6
	.4byte 0x0060a0f2
	.4byte 0x000001ff
	.global Data_02005410
Data_02005410:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005428
Data_02005428:
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff017e
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00034000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00034000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00034000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005590
Data_02005590:
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff0110
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00020000
	.4byte 0xffff0070
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00010000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00004000
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0x007900f6
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020056b0
Data_020056b0:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01ac0000
	.4byte 0x00033000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00010000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x016c0000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00018000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00fe0000
	.4byte 0x00000000
	.4byte 0x01b20000
	.4byte 0x00015000
	.4byte 0xffff0071
	.4byte 0x00000003
	.4byte 0x01b20000
	.4byte 0x00000000
	.4byte 0x01be0000
	.4byte 0x0001b000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00015000
	.4byte 0xffff008a
	.4byte 0x00000003
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x00e60000
	.4byte 0x00013000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005860
Data_02005860:
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x005a0000
	.4byte 0x0001b000
	.4byte 0xffff0088
	.4byte 0x00000003
	.4byte 0x006e0000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00014000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x007e0000
	.4byte 0x00013000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00034000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x017d0000
	.4byte 0x00005000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x017d0000
	.4byte 0x00005000
	.4byte 0xffff0087
	.4byte 0x00000003
	.4byte 0x008c0000
	.4byte 0x00000000
	.4byte 0x01840000
	.4byte 0x00013000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02b60000
	.4byte 0x00008000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x02920000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00015000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01cc0000
	.4byte 0x00010000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x00720000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x0001b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005998
Data_02005998:
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02a60000
	.4byte 0x0000b000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01fc0000
	.4byte 0x00000000
	.4byte 0x02900000
	.4byte 0x00003000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x021e0000
	.4byte 0x00000000
	.4byte 0x02740000
	.4byte 0x00003000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x02260000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x0000b000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x02f00000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00015000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02d40000
	.4byte 0x00000000
	.4byte 0x02740000
	.4byte 0x00013000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005a40
Data_02005a40:
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x01d20000
	.4byte 0x00013000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02ea0000
	.4byte 0x00000000
	.4byte 0x02040000
	.4byte 0x0001b000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02bc0000
	.4byte 0x00000000
	.4byte 0x020c0000
	.4byte 0x0001d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005aa0
Data_02005aa0:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005aac
Data_02005aac:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x10004e15
	.4byte 0x094c020e
	.4byte Func_020030bc
	.4byte 0x00004e15
	.4byte 0x094c040e
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x09e0000a
	.4byte Func_02001380
	.4byte 0x00000002
	.4byte 0x0201000a
	.4byte Func_02001764
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_02001644
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte Func_02001880
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte Func_02001aa4
	.4byte 0x00000000
	.4byte 0x094c000f
	.4byte MsgShamanYegelosSandChallenge
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgShamanChooseEitherRoad
	.4byte 0x00008d15
	.4byte 0x094c000f
	.4byte MsgShamanRodWasStolen
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgShamanMoapaCannotLose
	.4byte 0x00000000
	.4byte 0x094c0011
	.4byte MsgShamanTrialRoadAncientBattle
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_02000d68
	.4byte 0x00008d15
	.4byte 0x094c0011
	.4byte MsgShamanNeedContigoOrShamanPower
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgShamanMoapaGreatestWarrior
	.4byte 0x00000000
	.4byte 0x094c0010
	.4byte MsgShamanAncientHeroesVanishedSand
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_02000e3c
	.4byte 0x00008d15
	.4byte 0x094c0010
	.4byte MsgShamanSandChallengeWillFail
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgShamanJadeNeedsYegelosHeir
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005bc0
Data_02005bc0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000ce01
	.4byte 0x194d0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000280
	.4byte 0x00008c15
	.4byte 0x094e000e
	.4byte Func_02001cc8
	.4byte 0x0000c403
	.4byte 0x094d0016
	.4byte Func_02000d28
	.4byte 0x00000003
	.4byte 0xffff000a
	.4byte Func_02000d48
	.4byte 0x00000002
	.4byte 0x0203000b
	.4byte Func_02002808
	.4byte 0x0000c400
	.4byte 0xffff0011
	.4byte Func_0200122c
	.4byte 0x00008d15
	.4byte 0x0a210011
	.4byte MsgShamanCloudyFortuneBeforeTrial
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgShamanCloudyFortuneAfterTrial
	.4byte 0x00000000
	.4byte 0x09fe000f
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgShamanNewLegendaryBattle
	.4byte 0x00008d15
	.4byte 0x09fe000f
	.4byte MsgShamanAncestorsDistrustOutsiders
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgShamanContigoHeroVictorySurprise
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005ca4
Data_02005ca4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000ce01
	.4byte 0x194d0002
	.4byte 0x00000002
	.4byte 0x0000ce01
	.4byte 0x194d0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_02003c58
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_02003c74
	.4byte 0x0000c602
	.4byte 0x194d0006
	.4byte Func_02003c90
	.4byte 0x0000c602
	.4byte 0x194d0007
	.4byte Func_02003cac
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000ce01
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte Func_02003cc8
	.4byte 0x0000c602
	.4byte 0x094f000d
	.4byte Func_02001d3c
	.4byte 0x0000c602
	.4byte 0xffff000d
	.4byte Func_02003ce4
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00008515
	.4byte 0x02040013
	.4byte 0x00000000
	.4byte 0x00000c15
	.4byte 0x02050015
	.4byte Func_02002878
	.4byte 0x0000c403
	.4byte 0x094d0014
	.4byte Func_02000d08
	.4byte 0x0000c403
	.4byte 0x094d0016
	.4byte Func_02000d28
	.4byte 0x0000c403
	.4byte 0x094f0015
	.4byte Func_02001d3c
	.4byte 0x00000000
	.4byte 0x09fe0008
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgShamanMoapaImpressedByChallenge
	.4byte 0x00008d15
	.4byte 0x09fe0008
	.4byte MsgShamanWaitForOutsidersToLeave
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgShamanMoapaPraisesOpponents
	.4byte 0x00000000
	.4byte 0x09fe0009
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgShamanRespectForeignWarriors
	.4byte 0x00008d15
	.4byte 0x09fe0009
	.4byte MsgShamanChiefForbidsTalkingToOutsiders
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgShamanDoubtMoapaDefeat
	.4byte 0x00000000
	.4byte 0x09fe000a
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgShamanVictoryProvesSkill
	.4byte 0x00008d15
	.4byte 0x09fe000a
	.4byte MsgShamanWhoInvitedOutsiders
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgShamanHopeForStrongerShamanChild
	.4byte 0x00000000
	.4byte 0x09fe000b
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgShamanForeignersCanBeHeroes
	.4byte 0x00008d15
	.4byte 0x09fe000b
	.4byte MsgShamanRefuseToTalkToOutsiders
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgShamanWorldShouldWelcomeStrangers
	.4byte 0x00000000
	.4byte 0x09fe000c
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgShamanHoabnaPromiseFulfilled
	.4byte 0x00008d15
	.4byte 0x09fe000c
	.4byte MsgShamanTrustContigoFriends
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgShamanRemainFriendsWithContigo
	.4byte 0x00000000
	.4byte 0x09fe000d
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02001118
	.4byte 0x00008d15
	.4byte 0x09fe000d
	.4byte MsgShamanDistrustFriendlyOutsiders
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgShamanWishToBreakSandWalls
	.4byte 0x00000000
	.4byte 0x09fe000e
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgShamanChosenFewHavePower
	.4byte 0x00008d15
	.4byte 0x09fe000e
	.4byte MsgShamanShutDoorsAgainstOutsiders
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgShamanOutskirtsHaveDifferentPowers
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_02000ef8
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgShamanOutsidersAreNotContigo
	.4byte 0x00000000
	.4byte 0x09fe0011
	.4byte Func_02000f2c
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgShamanGuardPromisesRematchVictory
	.4byte 0x00008d15
	.4byte 0x09fe0011
	.4byte MsgShamanContigoTempleTreasure
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgShamanClimbingTrialRoadIsHard
	.4byte 0x00000000
	.4byte 0x09fe0010
	.4byte Func_02000f60
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgShamanVillagersBlameGuardDefeat
	.4byte 0x00008d15
	.4byte 0x09fe0010
	.4byte MsgShamanMoapaSparedOutsiders
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgShamanGuardBlamesMountainClimb
	.4byte 0x00004114
	.4byte 0x09e1000f
	.4byte Func_02002068
	.4byte 0x0001ff14
	.4byte 0xffff000f
	.4byte Func_0200279c
	.4byte 0x0001ff14
	.4byte 0xffff0010
	.4byte Func_020027c0
	.4byte 0x0001ff14
	.4byte 0xffff0011
	.4byte Func_020027e4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005f98
Data_02005f98:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x09fe0008
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgShamanMoapaWantsToKnowNames
	.4byte 0x00008d15
	.4byte 0x09fe0008
	.4byte MsgShamanMoapaLeftDoorOpen
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgShamanShameNotKnowingVictors
	.4byte 0x00000000
	.4byte 0x09fe0009
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgShamanFormalBattleNeedsNames
	.4byte 0x00008d15
	.4byte 0x09fe0009
	.4byte MsgShamanFatherNeverLoses
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgShamanFatherLostToRudeOutsiders
	.4byte 0x00000000
	.4byte 0x09fe000a
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgShamanNamesDoNotMatter
	.4byte 0x00008d15
	.4byte 0x09fe000a
	.4byte MsgShamanFirstForeignVisitors
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgShamanNamesBeforeBattleOldFashioned
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_0200113c
	.4byte 0x00008d15
	.4byte 0x09ed000b
	.4byte MsgShamanVictorsDeserveRespect
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgShamanVictorsNamesOnTrialColumns
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02000fc4
	.4byte 0x00008d15
	.4byte 0x09fe000d
	.4byte MsgShamanPayingStrangersMayStay
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgShamanMoapaMiserableAfterDefeat
	.4byte 0x00000000
	.4byte 0x09fe000e
	.4byte MsgShamanSilentVillager
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgShamanForgiveUnkindVillagers
	.4byte 0x00008d15
	.4byte 0x09fe000e
	.4byte MsgShamanMoapaAllowsStrangersToStay
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgShamanVillagersWorryTooMuch
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgShamanFinallyOpenLockedDoor
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgShamanQuietWhileLockedInside
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgShamanMissedTrialRoadChallenge
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgShamanHidingLawPreventedWatching
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgShamanEnjoyGettingOutside
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgShamanChildrenIgnoreTradition
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgShamanPlanPicnicMeal
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgShamanEagerForDinner
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgShamanSimpleMeatPotatoesBeans
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgShamanSaltAndPepperOnly
	.4byte 0x00000173
	.4byte 0xffff00c8
	.2byte MsgShamanRoastedRiverFish
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00c9
	.2byte MsgShamanTraditionalBoneBroth
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00ca
	.2byte MsgShamanStrangeBoiledVegetables
	.2byte 0x0040
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020061f0
Data_020061f0:
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgShamanAncientHesperiaAttekaWar
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgShamanNewContigoHeroStronger
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgShamanHoabnaYegelosDecideWar
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgShamanDoubtShamanSuperiority
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgShamanTownsChooseChampions
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgShamanGrandparentsUpsetByDefeat
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgShamanYegelosBraveJourney
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgShamanGrandparentsTiredOfWarStories
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020011c0
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgShamanHusbandWorriesAboutWeapons
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgShamanWarehouseOwnerFeelsResponsible
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgShamanWeaponsDidNotCauseLoss
	.4byte 0x00000173
	.4byte 0xffff00c8
	.2byte MsgShamanRoastedRiverFish
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00c9
	.2byte MsgShamanTraditionalBoneBroth
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00ca
	.2byte MsgShamanStrangeBoiledVegetables
	.2byte 0x0040
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020062d4
Data_020062d4:
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_020010c8
	.4byte 0x00008d15
	.4byte 0x09fe0008
	.4byte MsgShamanVisitorsMustFindMoapa
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgShamanFearRenewedContigoWar
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgShamanThreatenContigoRematch
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgShamanMoapaMustHaveBeenHungry
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgShamanPrayForShamanWarrior
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgShamanPrayForContigoConqueror
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global DriftScene_SpawnScript
DriftScene_SpawnScript:
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x00000026
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global Data_02006350
Data_02006350:
	.space 0x00000140
	.global Data_02006490
Data_02006490:
	.space 0x00000004
	.global Data_02006494
Data_02006494:
	.space 0x00000004
	.global Data_02006498
Data_02006498:
	.space 0x00000004
	.global Data_0200649c
Data_0200649c:
	.space 0x00000004
	.global Data_020064a0
Data_020064a0:
	.space 0x00000004
	.global Data_020064a4
Data_020064a4:
	.space 0x00000004
	.global Data_020064a8
Data_020064a8:
	.space 0x00000004
	.global Data_020064ac
Data_020064ac:
	.space 0x00000004
	.global Data_020064b0
Data_020064b0:
	.space 0x00000004
	.global Data_020064b4
Data_020064b4:
	.space 0x00000004
	.global Data_020064b8
Data_020064b8:
	.space 0x00000004
	.global Data_020064bc
Data_020064bc:
	.space 0x00000004
	.global Data_020064c0
Data_020064c0:
	.space 0x00000004
	.global Data_020064c4
Data_020064c4:
	.space 0x0000000c
	.global Data_020064d0
Data_020064d0:
	.space 0x00000030
	.global Data_02006500
Data_02006500:
	.space 0x00000004
	.global Data_02006504
Data_02006504:
	.space 0x00000004
	.global Data_02006508
Data_02006508:
