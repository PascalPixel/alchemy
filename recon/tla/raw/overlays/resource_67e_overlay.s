.syntax unified
	.thumb
	.section .text.x02008048,"ax",%progbits
	.global Func_02000048
	.thumb_func
Func_02000048:
	push {r5, r6, lr}
	movs r0, #12
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	bl Func_02000df0
	movs r0, #132
	bl Func_02000e08
	cmp r6, #20
	bne .L_0200814c
	cmp r5, #9
	bne .L_0200814c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080a0
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #36
	movs r1, #12
	movs r2, #22
	movs r3, #8
	bl Func_02000ce0
	movs r3, #22
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #12
	movs r2, #3
	movs r3, #3
	bl Func_02000ce8
.L_020080a0:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #36
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008142
	bl Func_02000d00
	movs r0, #0
	bl Func_02000dd0
	movs r0, #22
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02000da0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #22
	bl Func_02000d88
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #22
	bl Func_02000d88
	movs r0, #60
	bl Battle_WaitMode0
	ldr r0, .L_02008194
	bl Func_02000d60
	movs r1, #0
	movs r0, #22
	bl Func_02000d70
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #131
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #22
	bl Func_02000d88
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	bl Func_02000d70
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_02000d08
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #36
	bl GameFlag_SetBit
.L_02008142:
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008190
.L_0200814c:
	cmp r6, #19
	bne .L_02008190
	cmp r5, #9
	bne .L_02008190
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008190
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #36
	movs r1, #8
	movs r2, #22
	movs r3, #8
	bl Func_02000ce0
	movs r3, #22
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #8
	movs r2, #3
	movs r3, #3
	bl Func_02000ce8
.L_02008190:
	add sp, #8
	pop {r5, r6, pc}
.L_02008194:
	.4byte MsgTheMisalignedStone
