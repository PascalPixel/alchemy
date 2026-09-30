.syntax unified
	.thumb
	.global Func_0803fc38
	.thumb_func
Func_0803fc38:
	push {lr}
	movs r1, #197
	lsls r1, r1, #3
	movs r0, #208
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #0
	adds r4, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r4, #0
	ldr r2, .L_0803fd08
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, .L_0803fd0c
	movs r0, #147
	lsls r0, r0, #1
	adds r0, #255
	adds r3, r1, r0
	movs r0, #160
	ldrb r2, [r3]
	lsls r0, r0, #3
	adds r0, #148
	adds r3, r4, r0
	strb r2, [r3]
	movs r3, #160
	lsls r3, r3, #3
	movs r0, #128
	adds r3, #153
	lsls r0, r0, #2
	adds r2, r4, r3
	adds r0, #38
	movs r3, #24
	strb r3, [r2]
	adds r3, r1, r0
	movs r0, #160
	ldrb r2, [r3]
	lsls r0, r0, #3
	adds r0, #149
	adds r3, r4, r0
	strb r2, [r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #154
	movs r0, #139
	adds r2, r4, r3
	lsls r0, r0, #2
	movs r3, #15
	strb r3, [r2]
	adds r3, r1, r0
	movs r0, #160
	ldrb r2, [r3]
	lsls r0, r0, #3
	adds r0, #150
	adds r3, r4, r0
	strb r2, [r3]
	movs r3, #160
	lsls r3, r3, #3
	movs r0, #128
	adds r3, #155
	lsls r0, r0, #2
	adds r2, r4, r3
	adds r0, #42
	movs r3, #3
	strb r3, [r2]
	adds r3, r1, r0
	movs r0, #147
	ldrb r2, [r3]
	lsls r0, r0, #3
	adds r0, #255
	adds r3, r4, r0
	strb r2, [r3]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #156
	adds r3, r4, r2
	movs r0, #2
	strb r0, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #74
	adds r1, r1, r3
	ldrb r2, [r1]
	movs r1, #179
	lsls r1, r1, #3
	adds r3, r4, r1
	strb r2, [r3]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #157
	adds r4, r4, r2
	movs r1, #144
	strb r0, [r4]
	lsls r1, r1, #3
	ldr r0, .L_0803fd10
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {pc}
	.2byte 0x0000
.L_0803fd08:
	.4byte 0x8500018a
.L_0803fd0c:
	.4byte gPartyState
.L_0803fd10:
	.4byte Func_0803fb60
