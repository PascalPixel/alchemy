.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02000348
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_02000378
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_02000390
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	bl Func_020002b8
	movs r0, #0
	bl Func_02000340
	ldr r0, .L_020080b8
	bl Func_02000300
	movs r1, #0
	movs r0, #8
	bl Func_02000308
	ldr r3, .L_020080bc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008096
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	movs r2, #2
	bl Func_02000310
	b .L_020080b2
.L_02008096:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #8
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #2
	bl Func_02000310
.L_020080b2:
	bl Func_020002c0
	pop {pc}
.L_020080b8:
	.4byte 0x0000190a
.L_020080bc:
	.4byte gPartyState
	.section .text.x020080c0,"ax",%progbits
	.global Func_020000c0
	.thumb_func
Func_020000c0:
	ldr r0, .L_020080c4
	bx lr
.L_020080c4:
	.4byte Data_020003d8
	.section .text.x020080c8,"ax",%progbits
	.global Func_020000c8
	.thumb_func
Func_020000c8:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #2
	sub sp, #8
	bl Func_02000298
	cmp r0, #0
	beq .L_020080da
	b .L_02008216
.L_020080da:
	bl Func_020002b8
	movs r0, #0
	bl Func_02000340
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02000298
	cmp r0, #0
	bne .L_02008122
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02000328
	movs r0, #164
	movs r1, #1
	movs r2, #130
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Func_02000330
	ldr r3, .L_0200821c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #158
	subs r2, #216
	bl ObjectMotion_ResetAndSetPositionInMode2
.L_02008122:
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #148
	ands r5, r3
	strb r5, [r0]
	movs r2, #252
	movs r0, #9
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #170
	movs r2, #252
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #128
	orrs r5, r3
	strb r5, [r0]
	lsls r1, r1, #7
	movs r0, #8
	movs r2, #0
	bl Func_02000320
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl Func_02000320
	ldr r5, .L_0200821c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r5]
	bl Func_02000320
	movs r0, #128
	lsls r0, r0, #2
	bl Func_020002a0
	bl Func_02000338
	movs r3, #8
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #11
	movs r1, #9
	movs r2, #4
	movs r3, #1
	bl Func_020002a8
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02000298
	cmp r0, #0
	bne .L_02008216
	ldr r0, .L_02008220
	bl Func_02000300
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_02000310
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02000318
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_020002a0
	bl Func_020002c0
.L_02008216:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_0200821c:
	.4byte gPartyState
.L_02008220:
	.4byte 0x00001908
	.section .text.x02008224,"ax",%progbits
	.global Func_02000224
	.thumb_func
Func_02000224:
	push {lr}
	bl Func_020002b8
	movs r0, #0
	bl Func_02000340
	ldr r0, .L_0200827c
	bl Func_02000300
	movs r0, #164
	movs r1, #1
	movs r2, #130
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	bl Func_02000330
	bl Func_02000338
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_02000318
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #9
	bl Func_02000318
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #22
	bl Func_020002a0
	bl Func_020002c0
	pop {pc}
.L_0200827c:
	.4byte 0x00001910
	.section .text.x02008280,"ax",%progbits
	.global Func_02000280
	.thumb_func
Func_02000280:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	movs r0, #0
	bx lr
	.section .text.x02008294,"ax",%progbits
	.global Func_02000294
	.thumb_func
Func_02000294:
	movs r0, #0
	bx lr
	.section .rodata.x02008348,"a",%progbits
	.global Data_02000348
Data_02000348:
	.4byte 0xffff0000
	.4byte 0x00000098
	.4byte 0x400000b0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000378
Data_02000378:
	.4byte 0x00000035
	.4byte 0x1010a002
	.4byte 0xffffffff
	.4byte 0x1020b002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02000390
Data_02000390:
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x00bc0000
	.4byte 0x00000000
	.4byte 0x01040000
	.4byte 0x00014000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x01040000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020003d8
Data_020003d8:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x0913000a
	.4byte Func_020000c8
	.4byte 0x00000002
	.4byte 0x0916000a
	.4byte Func_02000224
	.4byte 0x00000000
	.4byte 0x09130008
	.4byte Func_02000054
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001912
	.4byte 0x00000000
	.4byte 0x09130009
	.4byte 0x0000190d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001913
	.4byte 0x00008d15
	.4byte 0x09130008
	.4byte 0x0000190e
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001914
	.4byte 0x00008d15
	.4byte 0x09130009
	.4byte 0x0000190f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001915
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
