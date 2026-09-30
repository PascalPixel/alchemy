.syntax unified
	.thumb
	.global Func_0801319c
	.thumb_func
Func_0801319c:
	push {r5, lr}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #176
	ldrh r1, [r2, #10]
	movs r3, #197
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r1
	strh r3, [r2, #10]
	movs r3, #254
	ldrh r1, [r2, #10]
	lsls r3, r3, #7
	adds r3, #255
	ands r3, r1
	strh r3, [r2, #10]
	sub sp, #4
	ldrh r3, [r2, #10]
	movs r2, #128
	ldr r3, .L_08013244
	lsls r2, r2, #7
	adds r2, #20
	strh r2, [r3]
	mov r0, sp
	movs r3, #128
	movs r5, #0
	lsls r3, r3, #19
	movs r1, #192
	str r5, [r0]
	adds r3, #212
	lsls r1, r1, #18
	ldr r2, .L_08013248
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Func_08014c6c
	bl Func_080133c4
	ldr r3, .L_0801324c
	str r5, [r3]
	ldr r3, .L_08013250
	strb r5, [r3]
	ldr r3, .L_08013254
	strb r5, [r3]
	ldr r3, .L_08013258
	strb r5, [r3]
	bl Func_080132d0
	bl Func_08014bac
	bl Func_08014b70
	ldr r3, .L_08013240
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #0
	ldr r2, .L_0801325c
	movs r1, #1
	bl Func_08013438
	ldr r2, .L_08013260
	movs r0, #13
	movs r1, #1
	bl Func_08013438
	movs r2, #192
	ldr r3, .L_08013264
	lsls r2, r2, #8
	adds r2, #15
	strh r2, [r3]
	bl Func_081c0008
	bl Func_08014368
	bl Scheduler_ResetTaskTable
	ldr r3, .L_08013268
	ldr r2, .L_0801326c
	str r5, [r3]
	movs r3, #1
	b .L_08013270
.L_08013240:
	.4byte 0x00000140
.L_08013244:
	.4byte 0x04000204
.L_08013248:
	.4byte 0x85001e00
.L_0801324c:
	.4byte gIoWriteQueue
.L_08013250:
	.4byte Data_03001110
.L_08013254:
	.4byte Data_03001238
.L_08013258:
	.4byte Data_0300123c
.L_0801325c:
	.4byte Func_0801399c
.L_08013260:
	.4byte IwramHalt
.L_08013264:
	.4byte 0x04000132
.L_08013268:
	.4byte Data_03007800
.L_0801326c:
	.4byte Data_0300120c
.L_08013270:
	strb r3, [r2]
	ldr r2, .L_08013298
	movs r3, #128
	lsls r3, r3, #1
	ldr r1, .L_08013294
	strh r3, [r2, #2]
	ldr r3, .L_0801329c
	strh r5, [r2]
	strb r1, [r3]
	movs r0, #10
	bl WaitFrames
	movs r0, #0
	bl Game_ResetForNewGameFar
	add sp, #4
	b .L_080132a0
	.2byte 0x0000
.L_08013294:
	.4byte 0x00000000
.L_08013298:
	.4byte gOamUsage
.L_0801329c:
	.4byte Data_03001180
.L_080132a0:
	pop {r5, pc}
	.2byte 0x0000
