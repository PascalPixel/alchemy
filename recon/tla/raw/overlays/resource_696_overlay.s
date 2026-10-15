
	.include "games/common/include/game/ed_asm.h"
	.set SHAMAN_GRAPHIC1_SCALE, 256
	.set SHAMAN_GRAPHIC4_START_SCALE, 512
	.set SHAMAN_GRAPHIC4_SMALL_SCALE, 256
	.set SHAMAN_GRAPHIC4_LARGE_SCALE, 384
	.ifdef TLA_EDITION_ES
	.set SHAMAN_GRAPHIC4_START_SCALE, 384
	.endif
	.ifdef TLA_EDITION_IT
	.set SHAMAN_GRAPHIC4_START_SCALE, 384
	.endif
	.ifdef TLA_EDITION_FR
	.set SHAMAN_GRAPHIC1_SCALE, 384
	.set SHAMAN_GRAPHIC4_SMALL_SCALE, 384
	.set SHAMAN_GRAPHIC4_LARGE_SCALE, 512
	.endif
	.set SHAMAN_GRAPHIC7_PALETTE, (2 * 32)
	.ifdef TLA_EDITION_DE
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_ES
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_FR
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_IT
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
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
	bl ObjectDispatch_SetSingleChildFlags
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
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_0200827c:
	.4byte 0xffff0000
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
	bl MemAlloc
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
	bl MemFree
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
	bl MemAlloc
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
	bl MemFree
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
	bl ObjectDispatch_SetSingleChildFlags
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
	.4byte gFrameCount
.L_0200883c:
	.4byte Data_02004ae0
.L_02008840:
	.4byte Effect_CircleAndBlink
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
	.section .text.x0200a90c,"ax",%progbits
	.balign 4
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
	bl UnitGet
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
	.ifdef TLA_EDITION_FR
	movs r5, #0
.L_shaman_french_graphic_loop:
	ldr r4, [sp, #0]
	lsls r3, r5, #5
	subs r3, #16
	muls r3, r4
	movs r1, #64
	ldr r0, .L_0200af0c
	cmp r3, #0
	bge .L_shaman_french_graphic_position
	adds r3, #255
.L_shaman_french_graphic_position:
	asrs r3, r3, #8
	adds r3, r6, r3
	movs r7, #48
	adds r2, r3, #0
	adds r7, #255
	adds r3, #152
	adds r2, #88
	cmp r3, r7
	bhi .L_shaman_french_graphic_next
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
.L_shaman_french_graphic_next:
	movs r2, #8
	adds r5, #1
	add r10, r2
	cmp r5, #1
	ble .L_shaman_french_graphic_loop
	b .L_0200ae8e
	.else
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
	.endif
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
	.ifndef TLA_EDITION_FR
	.2byte 0x0000
	.endif
.L_0200af08:
	.4byte Data_020064b0
.L_0200af0c:
	.4byte 0x80004000
.L_0200af10:
	.4byte Data_02006494
	.ifndef TLA_EDITION_FR
.L_0200af14:
	.4byte 0xc0004000
	.endif
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
