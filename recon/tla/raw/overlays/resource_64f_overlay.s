.syntax unified
	.thumb
	.section .text.x02008494,"ax",%progbits
	.balign 4
	.global Func_02000494
	.thumb_func
Func_02000494:
	push {r5, r6, lr}
	bl Func_02000988
	movs r0, #0
	bl Func_02000a70
	ldr r0, .L_020085d8
	bl Func_02000a00
	ldr r6, .L_020085dc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r6, r3
	movs r2, #4
	movs r1, #0
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #30
	bl FieldEvent_WaitTicks
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #0
	ldr r0, [r5]
	movs r2, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl FieldEvent_WaitTicks
	ldr r0, [r5]
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #154
	movs r2, #228
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_020009d0
	movs r1, #150
	movs r2, #230
	movs r0, #13
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #154
	movs r2, #228
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_020009d0
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #158
	movs r2, #230
	movs r0, #15
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #154
	movs r2, #228
	movs r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_020009d0
	movs r1, #154
	movs r2, #230
	lsls r2, r2, #2
	movs r0, #14
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl Func_02000a08
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl Func_02000a08
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #15
	movs r1, #0
	bl Func_02000a08
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	ldr r0, .L_020085e0
	movs r1, #99
	bl Party_SetFields1eeAnd1f0
	ldr r0, .L_020085e4
	movs r1, #98
	bl Party_SetFields1f2And1f4
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r6, r6, r3
	movs r3, #2
	strb r3, [r6]
	movs r0, #9
	movs r1, #2
	bl Func_02000a40
	bl Func_02000990
	pop {r5, r6, pc}
.L_020085d8:
	.4byte MsgHaThatWas
.L_020085dc:
	.4byte gPartyState
.L_020085e0:
	.4byte 0x00000007
.L_020085e4:
	.4byte 0x00000004
