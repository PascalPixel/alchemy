.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_0200292c
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	ldr r0, .L_02008044
	bx lr
.L_02008044:
	.4byte Data_0200295c
	.section .text.x02008048,"ax",%progbits
	.global Func_02000048
	.thumb_func
Func_02000048:
	ldr r0, .L_0200804c
	bx lr
.L_0200804c:
	.4byte Data_0200298c
	.section .text.x02008050,"ax",%progbits
	.global Func_02000050
	.thumb_func
Func_02000050:
	ldr r0, .L_02008054
	bx lr
.L_02008054:
	.4byte Data_02002a14
	.section .text.x02008058,"ax",%progbits
	.global Func_02000058
	.thumb_func
Func_02000058:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008082
	ldr r0, .L_020080f8
	bl Func_020028ac
	adds r0, r5, #0
	movs r1, #0
	bl Func_020028c4
	movs r0, #10
	movs r1, #4
	bl Object_LinkObjectAndSetCallback
	b .L_020080f6
.L_02008082:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080ba
	bl Func_0200282c
	movs r0, #0
	bl Func_020028fc
	ldr r0, .L_020080fc
	bl Func_020028ac
	adds r0, r5, #0
	movs r1, #0
	bl Func_020028c4
	movs r1, #192
	adds r0, r5, #0
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_02002834
	b .L_020080f6
.L_020080ba:
	ldr r0, .L_02008100
	bl Func_020028ac
	movs r1, #0
	adds r0, r5, #0
	bl UiText_OpenMessageAtObject
	bl Func_0200291c
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020080dc
	bl Func_0200031c
	b .L_020080f6
.L_020080dc:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #0
	movs r1, #0
	bl Func_020028c4
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_020080f6:
	pop {r5, pc}
.L_020080f8:
	.4byte 0x00002780
.L_020080fc:
	.4byte 0x00002767
.L_02008100:
	.4byte 0x0000274d
	.section .text.x02008104,"ax",%progbits
	.global Func_02000104
	.thumb_func
Func_02000104:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #102
	adds r5, r1, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200811a
	ldr r0, .L_02008148
	b .L_0200812a
.L_0200811a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008138
	ldr r0, .L_0200814c
.L_0200812a:
	bl Func_020028ac
	adds r0, r5, #0
	movs r1, #0
	bl Func_020028c4
	b .L_02008146
.L_02008138:
	ldr r0, .L_02008150
	bl Func_020028ac
	adds r0, r5, #0
	movs r1, #0
	bl Func_020028c4
.L_02008146:
	pop {r5, pc}
.L_02008148:
	.4byte 0x00002781
.L_0200814c:
	.4byte 0x00002768
.L_02008150:
	.4byte 0x0000274f
	.section .text.x02008154,"ax",%progbits
	.global Func_02000154
	.thumb_func
Func_02000154:
	push {r5, r6, lr}
	ldr r5, .L_0200819c
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_020028ac
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_0200291c
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008184
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_020028ac
	b .L_02008190
.L_02008184:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_020028ac
.L_02008190:
	adds r0, r6, #0
	movs r1, #0
	bl Func_020028c4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200819c:
	.4byte 0x00002783
	.section .text.x020081a0,"ax",%progbits
	.global Func_020001a0
	.thumb_func
Func_020001a0:
	push {r5, r6, lr}
	ldr r5, .L_020081e8
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_020028ac
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_0200291c
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020081d0
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_020028ac
	b .L_020081dc
.L_020081d0:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_020028ac
.L_020081dc:
	adds r0, r6, #0
	movs r1, #0
	bl Func_020028c4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020081e8:
	.4byte 0x0000278a
	.section .text.x020081ec,"ax",%progbits
	.global Func_020001ec
	.thumb_func
Func_020001ec:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Object_GetById
	ldrh r7, [r0, #6]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #103
	bl GameFlag_SetBit
	bl Func_0200282c
	movs r0, #0
	bl Func_020028fc
	movs r1, #2
	adds r1, #255
	movs r2, #30
	adds r0, r5, #0
	bl Func_020028dc
	movs r2, #20
	movs r1, #4
	adds r0, r5, #0
	bl ObjectMotion_SetAngleToward
	ldr r6, .L_02008278
	adds r0, r6, #0
	bl Func_020028ac
	movs r1, #0
	adds r0, r5, #0
	bl UiText_OpenMessageAtObject
	bl Func_0200291c
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200824e
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r6, #1
	bl Func_020028ac
	b .L_0200825a
.L_0200824e:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r6, #2
	bl Func_020028ac
.L_0200825a:
	adds r0, r5, #0
	movs r1, #0
	bl Func_020028c4
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02002834
	pop {r5, r6, r7, pc}
.L_02008278:
	.4byte 0x0000278d
	.section .text.x0200827c,"ax",%progbits
	.global Func_0200027c
	.thumb_func
Func_0200027c:
	ldr r0, .L_02008280
	bx lr
.L_02008280:
	.4byte Data_02002b94
	.section .text.x02008284,"ax",%progbits
	.global Func_02000284
	.thumb_func
Func_02000284:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	movs r0, #11
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r1, #2
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	movs r0, #15
	bl Object_GetById
	adds r3, r0, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	str r5, [r0, #20]
	str r5, [r0, #12]
	movs r0, #17
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	strb r5, [r3]
	str r5, [r0, #20]
	str r5, [r0, #12]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082fa
	movs r0, #10
	movs r1, #100
	movs r2, #112
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r0, #6]
	movs r0, #1
	bl WaitFrames
.L_020082fa:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008312
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02002874
.L_02008312:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008318,"ax",%progbits
	.global Func_02000318
	.thumb_func
Func_02000318:
	movs r0, #0
	bx lr
	.section .text.x0200831c,"ax",%progbits
	.global Func_0200031c
	.thumb_func
Func_0200031c:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_SetBit
	movs r0, #7
	bl Party_RemoveActiveOwner
	bl Func_0200282c
	movs r0, #0
	bl Func_020028fc
	ldr r0, .L_02008734
	bl Func_020028ac
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #112
	movs r2, #128
	bl ObjectMotion_SetPositionAndReset
	movs r3, #176
	movs r0, #12
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02002904
	movs r2, #16
	movs r3, #128
	lsls r3, r3, #8
	movs r0, #7
	movs r1, #16
	negs r2, r2
	bl Func_02002904
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_020028dc
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #45
	movs r0, #12
	bl Func_020028dc
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r2, #0
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_020028dc
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #10
	bl Func_020028dc
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #45
	movs r0, #7
	bl Func_020028dc
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #35
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_020028bc
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #60
	movs r0, #10
	bl Func_020028dc
	movs r1, #48
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #24
	negs r1, r1
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_020028dc
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #7
	bl Func_020028dc
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #12
	bl Func_020028dc
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #12
	movs r1, #0
	bl Func_020028bc
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_020028dc
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r2, #0
	movs r1, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	movs r1, #0
	movs r2, #10
	b .L_02008738
	.2byte 0x0000
.L_02008734:
	.4byte 0x00002750
.L_02008738:
	bl Func_020028bc
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #7
	bl Func_020028e4
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #12
	negs r1, r1
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_020028dc
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #4
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r1, #0
	movs r0, #10
	bl Func_020028bc
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #9
	movs r0, #7
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #1
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	adds r1, #204
	adds r2, #102
	movs r0, #7
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #12
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #7
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #7
	bl Func_020028e4
	movs r0, #55
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #12
	movs r1, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #10
	movs r1, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	ldr r1, .L_02008a4c
	ldr r2, .L_02008a50
	bl ObjectMotion_SetSpeedParameters
	movs r1, #8
	negs r1, r1
	movs r2, #88
	movs r0, #7
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #4
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #12
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #0
	movs r2, #0
	movs r0, #7
	bl Func_02002874
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_020028dc
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #12
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020089be
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #12
	movs r1, #0
	bl Func_020028bc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020089e4
.L_020089be:
	movs r0, #40
	bl Battle_WaitMode0
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
	adds r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
.L_020089e4:
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #12
	ldr r1, .L_02008a54
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02008a58
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008a36
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl ObjectMotion_ResetAndSetPosition
.L_02008a36:
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02002874
	bl Func_02002834
	pop {pc}
.L_02008a4c:
	.4byte 0x00023333
.L_02008a50:
	.4byte 0x00011999
.L_02008a54:
	.4byte 0x00013333
.L_02008a58:
	.4byte gPartyState
	.section .text.x02008a5c,"ax",%progbits
	.global Func_02000a5c
	.thumb_func
Func_02000a5c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #11
	sub sp, #48
	bl Object_GetById
	mov r11, r0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008a86
	bl .L_0200927c
.L_02008a86:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_SetBit
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	bl Func_0200282c
	movs r0, #0
	bl Func_020028fc
	ldr r0, .L_02008d54
	bl Func_020028ac
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r0, #4
	movs r1, #136
	movs r2, #152
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #8
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020028dc
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #8
	movs r2, #16
	negs r2, r2
	negs r1, r1
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020028e4
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #12
	movs r2, #12
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_020028dc
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008c64
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	movs r2, #10
	adds r0, #10
	bl Func_020028bc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008c8a
.L_02008c64:
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #7
	strh r3, [r2]
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
.L_02008c8a:
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020028e4
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_020028dc
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008d58
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	movs r2, #10
	adds r0, #10
	bl Func_020028bc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008d7e
.L_02008d54:
	.4byte 0x00002769
.L_02008d58:
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #7
	strh r3, [r2]
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
.L_02008d7e:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #10
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #20
	bl Func_020028bc
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #10
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #89
	movs r2, #92
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	movs r2, #10
	adds r0, #10
	movs r1, #0
	bl Func_020028bc
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #10
	bl Func_020028dc
	movs r1, #3
	movs r0, #10
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #182
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002924
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl Func_020028e4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl Func_020028e4
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Object_SetModeById
	mov r1, r11
	adds r1, #85
	str r1, [sp, #0]
	movs r2, #0
	movs r3, #36
	strb r2, [r1]
	add r3, sp
	mov r1, r11
	mov r10, r3
	ldr r3, [r1, #8]
	mov r1, r10
	str r3, [r1]
	mov r1, r11
	ldr r3, [r1, #12]
	mov r1, r10
	str r3, [r1, #4]
	mov r1, r11
	ldr r3, [r1, #16]
	mov r1, r10
	str r3, [r1, #8]
	add r3, sp, #24
	mov r8, r3
	movs r3, #232
	mov r1, r8
	lsls r3, r3, #15
	str r3, [r1]
	str r2, [r1, #4]
	movs r3, #220
	movs r2, #176
	lsls r3, r3, #15
	lsls r2, r2, #8
	str r3, [r1, #8]
	str r2, [sp, #4]
	movs r7, #0
.L_02008ede:
	movs r1, #192
	lsls r0, r7, #15
	bl Engine_MathDivide
	adds r6, r0, #0
	bl Math_Sine
	ldr r3, [sp, #4]
	movs r1, #128
	lsls r1, r1, #2
	adds r3, r3, r1
	lsls r0, r0, #5
	str r0, [sp, #8]
	str r3, [sp, #4]
	add r2, sp, #12
	mov r9, r2
	mov r1, r8
	mov r2, r10
	ldr r5, [r2]
	ldr r3, [r1]
	movs r1, #192
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl Engine_MathDivide
	mov r3, r9
	adds r5, r5, r0
	str r5, [r3]
	adds r0, r6, #0
	bl Math_Sine
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #4]
	ldr r3, [r1, #4]
	adds r6, r0, #0
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	movs r1, #192
	bl Engine_MathDivide
	lsls r3, r6, #3
	subs r3, r3, r6
	adds r5, r5, r0
	lsls r3, r3, #3
	adds r5, r5, r3
	mov r3, r9
	str r5, [r3, #4]
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #8]
	ldr r3, [r1, #8]
	movs r1, #192
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl Engine_MathDivide
	mov r3, r9
	adds r5, r5, r0
	str r5, [r3, #8]
	ldr r1, [sp, #4]
	mov r2, r9
	ldr r0, [sp, #8]
	bl Vector_AddPolarOffsetFar
	mov r1, r9
	ldr r3, [r1]
	mov r2, r11
	str r3, [r2, #8]
	ldr r3, [r1, #4]
	str r3, [r2, #12]
	ldr r3, [r1, #8]
	str r3, [r2, #16]
	ldr r1, [sp, #4]
	movs r2, #128
	lsls r2, r2, #7
	adds r3, r1, r2
	mov r1, r11
	strh r3, [r1, #6]
	cmp r7, #128
	bne .L_02008f92
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_02008f92:
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #191
	ble .L_02008ede
	movs r3, #128
	lsls r3, r3, #8
	mov r2, r11
	strh r3, [r2, #6]
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #11
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	movs r1, #100
	movs r2, #112
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #10
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #0
	movs r1, #8
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #11
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #8
	strb r3, [r0]
	negs r1, r1
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #10
	bl Func_020028dc
	movs r1, #3
	movs r0, #11
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	ldr r5, .L_020090dc
	ldr r3, [sp, #0]
	mov r1, r11
	strb r5, [r3]
	mov r2, r10
	ldr r3, [r1, #8]
	movs r7, #0
	str r3, [r2]
	ldr r3, [r1, #12]
	str r3, [r2, #4]
	ldr r3, [r1, #16]
	mov r1, r8
	str r3, [r2, #8]
	movs r3, #132
	lsls r3, r3, #16
	str r3, [r1]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r1, #4]
	movs r2, #132
	movs r3, #200
	lsls r3, r3, #15
	lsls r2, r2, #7
	str r3, [r1, #8]
	str r2, [sp, #4]
	b .L_020090e0
	.2byte 0x0000
.L_020090dc:
	.4byte 0x00000000
.L_020090e0:
	movs r1, #192
	lsls r0, r7, #14
	bl Engine_MathDivide
	bl Math_Sine
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	str r3, [sp, #8]
	ldr r3, [sp, #4]
	movs r1, #192
	lsls r1, r1, #2
	adds r3, r3, r1
	str r3, [sp, #4]
	mov r2, r8
	mov r1, r10
	ldr r3, [r2]
	ldr r5, [r1]
	movs r1, #192
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl Engine_MathDivide
	mov r2, r9
	adds r5, r5, r0
	str r5, [r2]
	movs r1, #192
	lsls r0, r7, #15
	bl Engine_MathDivide
	bl Math_Sine
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #4]
	ldr r3, [r1, #4]
	adds r6, r0, #0
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	movs r1, #192
	bl Engine_MathDivide
	lsls r6, r6, #5
	adds r5, r5, r0
	adds r5, r5, r6
	mov r3, r9
	str r5, [r3, #4]
	mov r2, r10
	mov r1, r8
	ldr r5, [r2, #8]
	ldr r3, [r1, #8]
	movs r1, #192
	subs r3, r3, r5
	adds r0, r7, #0
	muls r0, r3
	bl Engine_MathDivide
	mov r3, r9
	adds r5, r5, r0
	str r5, [r3, #8]
	ldr r1, [sp, #4]
	mov r2, r9
	ldr r0, [sp, #8]
	bl Vector_AddPolarOffsetFar
	mov r1, r9
	ldr r3, [r1]
	mov r2, r11
	str r3, [r2, #8]
	ldr r3, [r1, #4]
	str r3, [r2, #12]
	ldr r3, [r1, #8]
	str r3, [r2, #16]
	ldr r1, [sp, #4]
	movs r2, #128
	lsls r2, r2, #7
	adds r3, r1, r2
	mov r1, r11
	strh r3, [r1, #6]
	cmp r7, #153
	bne .L_02009194
	movs r1, #224
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_02009194:
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #191
	ble .L_020090e0
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r3, #208
	mov r2, r11
	lsls r3, r3, #8
	strh r3, [r2, #6]
	ldr r3, [r2, #8]
	mov r1, r10
	str r3, [r1]
	movs r7, #0
	ldr r3, [r2, #12]
	str r3, [r1, #4]
	ldr r3, [r2, #16]
	mov r2, r8
	str r3, [r1, #8]
	movs r3, #180
	lsls r3, r3, #15
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r2, #4]
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r2, #8]
.L_020091d8:
	mov r1, r8
	ldr r3, [r1]
	mov r1, r10
	ldr r2, [r1]
	subs r3, r3, r2
	muls r3, r7
	cmp r3, #0
	bge .L_020091ea
	adds r3, #15
.L_020091ea:
	asrs r3, r3, #4
	adds r3, r2, r3
	mov r2, r11
	str r3, [r2, #8]
	mov r1, r8
	ldr r3, [r1, #4]
	mov r1, r10
	ldr r2, [r1, #4]
	subs r3, r3, r2
	muls r3, r7
	cmp r3, #0
	bge .L_02009204
	adds r3, #15
.L_02009204:
	asrs r3, r3, #4
	adds r3, r2, r3
	mov r2, r11
	str r3, [r2, #12]
	mov r1, r8
	ldr r3, [r1, #8]
	mov r1, r10
	ldr r2, [r1, #8]
	subs r3, r3, r2
	muls r3, r7
	cmp r3, #0
	bge .L_0200921e
	adds r3, #15
.L_0200921e:
	asrs r3, r3, #4
	adds r3, r2, r3
	mov r2, r11
	str r3, [r2, #16]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #15
	ble .L_020091d8
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl Func_02002874
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r0, #10
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	bl Func_02002834
.L_0200927c:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200928c,"ax",%progbits
	.global Func_0200128c
	.thumb_func
Func_0200128c:
	push {r5, r6, r7, lr}
	adds r4, r0, #0
	adds r6, r2, #0
	adds r5, r1, #0
	lsls r3, r3, #16
	movs r0, #244
	asrs r7, r3, #16
	lsls r0, r0, #1
	adds r3, r6, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl Func_02002804
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020092d0
	movs r1, #1
	ldr r5, [r6, #80]
	bl Func_020027f4
	ldr r1, .L_020092d8
	adds r0, r6, #0
	bl Func_020027fc
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [sp, #16]
	ldr r1, .L_020092d4
	adds r2, #9
	strh r3, [r2]
	strb r1, [r5, #26]
	strh r7, [r5, #18]
.L_020092d0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020092d4:
	.4byte 0x00000000
.L_020092d8:
	.4byte Data_02002dec
	.section .text.x020092dc,"ax",%progbits
	.global Func_020012dc
	.thumb_func
Func_020012dc:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	bl Object_GetById
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #12
	ldr r0, [r5, #8]
	mov r10, r3
	ldr r1, [r5, #12]
	movs r3, #224
	lsls r3, r3, #13
	mov r8, r3
	movs r3, #128
	ldr r2, [r5, #16]
	add r1, r8
	lsls r3, r3, #5
	movs r6, #15
	add r0, r10
	str r6, [sp, #0]
	mov r11, r3
	bl Func_0200128c
	movs r0, #151
	bl Func_02002924
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r3, .L_02009394
	ldr r1, [r5, #12]
	adds r0, r0, r3
	movs r3, #240
	ldr r2, [r5, #16]
	add r1, r8
	lsls r3, r3, #8
	str r6, [sp, #0]
	mov r9, r3
	bl Func_0200128c
	movs r0, #151
	bl Func_02002924
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	add r1, r8
	mov r3, r11
	add r0, r10
	str r6, [sp, #0]
	bl Func_0200128c
	movs r0, #151
	bl Func_02002924
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r3, .L_02009394
	ldr r2, [r5, #16]
	add r1, r8
	adds r0, r0, r3
	mov r3, r9
	str r6, [sp, #0]
	bl Func_0200128c
	movs r0, #151
	bl Func_02002924
	movs r0, #15
	bl Battle_WaitMode0
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
.L_02009394:
	.4byte 0xfff80000
	.section .text.x02009398,"ax",%progbits
	.global Func_02001398
	.thumb_func
Func_02001398:
	push {lr}
	ldr r1, [r0, #80]
	adds r0, #100
	ldrh r3, [r0]
	movs r2, #3
	ands r2, r3
	ldr r4, [r1, #40]
	cmp r2, #1
	beq .L_020093cc
	cmp r2, #1
	bgt .L_020093b4
	cmp r2, #0
	beq .L_020093be
	b .L_020093e8
.L_020093b4:
	cmp r2, #2
	beq .L_020093d0
	cmp r2, #3
	beq .L_020093de
	b .L_020093e8
.L_020093be:
	movs r3, #7
	strb r3, [r4, #5]
	movs r3, #1
	strb r3, [r1, #25]
	movs r3, #2
	strb r3, [r1, #26]
	b .L_020093e8
.L_020093cc:
	movs r3, #0
	b .L_020093d8
.L_020093d0:
	movs r2, #7
	movs r3, #0
	strb r2, [r4, #5]
	movs r2, #1
.L_020093d8:
	strb r2, [r1, #25]
	strb r3, [r1, #26]
	b .L_020093e8
.L_020093de:
	movs r2, #0
	movs r3, #1
	strb r2, [r4, #5]
	strb r3, [r1, #25]
	strb r2, [r1, #26]
.L_020093e8:
	ldrh r3, [r0]
	adds r3, #1
	strh r3, [r0]
	pop {pc}
	.section .text.x020093f0,"ax",%progbits
	.global Func_020013f0
	.thumb_func
Func_020013f0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_SetBit
	bl Func_0200282c
	movs r0, #0
	bl Func_020028fc
	ldr r0, .L_020095bc
	bl Func_020028ac
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #128
	movs r0, #18
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #18
	movs r1, #32
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #32
	movs r0, #18
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #220
	movs r2, #176
	movs r0, #4
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #220
	movs r2, #176
	movs r0, #4
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02002874
	movs r3, #160
	movs r0, #12
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #7
	bl Func_02002904
	movs r1, #16
	movs r3, #128
	movs r0, #5
	negs r1, r1
	movs r2, #0
	lsls r3, r3, #7
	bl Func_02002904
	movs r1, #32
	movs r3, #128
	lsls r3, r3, #7
	movs r0, #6
	negs r1, r1
	movs r2, #0
	bl Func_02002904
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #216
	movs r1, #1
	movs r2, #184
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_020028f4
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_020028dc
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #10
	adds r1, #255
	movs r2, #40
	movs r0, #18
	bl Func_020028dc
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r1, #0
	movs r0, #18
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020095c0
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020095e2
.L_020095bc:
	.4byte 0x0000279b
.L_020095c0:
	movs r0, #35
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #18
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
.L_020095e2:
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_020028dc
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #12
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #18
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #18
	bl Func_020028e4
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #12
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #2
	movs r0, #18
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #18
	bl Func_020028dc
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #5
	bl Func_020028dc
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl Func_020028bc
	movs r0, #18
	movs r1, #12
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r0, #12
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl Func_020028e4
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	movs r2, #128
	movs r0, #18
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #12
	movs r0, #18
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_020028dc
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #18
	bl Func_020028dc
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_020028dc
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #2
	adds r1, #255
	movs r2, #45
	movs r0, #18
	bl Func_020028dc
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #18
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #18
	movs r1, #12
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r0, #12
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #18
	bl Func_020028dc
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_020028bc
	movs r1, #2
	movs r0, #18
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #12
	movs r0, #18
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_020028dc
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #18
	bl Func_020012dc
	movs r0, #151
	bl Func_02002924
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #18
	bl Func_020028dc
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #12
	bl Func_020028dc
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	movs r2, #128
	movs r0, #18
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #18
	movs r1, #12
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #12
	movs r0, #18
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #18
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_020028dc
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #50
	adds r1, #255
	movs r0, #18
	bl Func_020028dc
	movs r0, #12
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #18
	bl Func_020028dc
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #5
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009bee
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009c2a
.L_02009bee:
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #18
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
.L_02009c2a:
	movs r1, #2
	movs r0, #12
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #12
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009cae
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_020028bc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009cd0
.L_02009cae:
	movs r0, #35
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #12
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
.L_02009cd0:
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_020028dc
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #6
	bl Func_020028dc
	movs r1, #254
	lsls r1, r1, #7
	movs r2, #0
	adds r1, #255
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #254
	lsls r1, r1, #7
	movs r0, #5
	adds r1, #255
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_020028dc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_020028dc
	movs r2, #0
	movs r1, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #6
	bl Func_020028e4
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #254
	lsls r1, r1, #7
	movs r0, #5
	adds r1, #255
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl Func_020028e4
	movs r0, #6
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #6
	bl Func_020028e4
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #18
	bl Func_020028dc
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_020028dc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_020028dc
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #12
	adds r1, #153
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #12
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #12
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_020028bc
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_020028bc
	movs r1, #2
	movs r0, #12
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #18
	bl Func_020028e4
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #5
	bl Func_020028dc
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	adds r1, #1
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_020028dc
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #18
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_020028bc
	movs r1, #2
	movs r0, #18
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #18
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #18
	movs r1, #12
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_020028dc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_020028dc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_020028dc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #45
	movs r0, #12
	bl Func_020028dc
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #12
	movs r0, #12
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_020028dc
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #12
	bl Func_020028dc
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_020028bc
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #4
	bl Object_SetModeById
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #12
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #18
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #2
	movs r2, #50
	adds r1, #255
	movs r0, #12
	bl Func_020028dc
	movs r1, #2
	movs r0, #18
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #208
	lsls r1, r1, #8
	adds r1, #10
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #12
	bl Func_020028dc
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #8
	adds r1, #255
	movs r2, #42
	movs r0, #18
	bl Func_020028dc
	movs r0, #18
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #18
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #2
	movs r2, #12
	negs r2, r2
	negs r1, r1
	movs r0, #18
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #18
	ldr r1, .L_0200a790
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #8
	movs r0, #18
	negs r1, r1
	movs r2, #40
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_02002814
	movs r1, #243
	movs r2, #243
	lsls r1, r1, #9
	lsls r2, r2, #8
	adds r1, #102
	adds r2, #51
	movs r0, #18
	bl ObjectMotion_SetSpeedParameters
	movs r0, #133
	bl Func_02002924
	movs r1, #2
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_Launch
	movs r0, #18
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #0
	strb r3, [r0]
	movs r1, #0
	mov r10, r2
	movs r0, #18
	subs r2, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #18
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl Func_02002814
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #153
	adds r2, #204
	movs r0, #18
	bl ObjectMotion_SetSpeedParameters
	movs r0, #18
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r2, #24
	negs r2, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #18
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #18
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r1, #6
	movs r2, #23
	movs r0, #18
	bl ObjectMotion_Launch
	movs r0, #18
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #100
	mov r2, r10
	strh r2, [r3]
	ldr r3, .L_0200a794
	movs r1, #0
	str r3, [r0, #108]
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r2, #10
	bl Func_020028bc
	movs r0, #18
	ldr r1, .L_0200a798
	ldr r2, .L_0200a79c
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #32
	movs r0, #18
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #18
	bl Object_GetById
	mov r8, r0
	movs r0, #134
	bl Func_02002924
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Func_02002814
	movs r0, #18
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #16
	ands r5, r3
	movs r1, #0
	negs r2, r2
	strb r5, [r0]
	movs r0, #18
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #18
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	adds r2, #100
	orrs r6, r3
	movs r3, #3
	strb r6, [r0]
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	mov r3, r10
	mov r2, r8
	str r3, [r2, #108]
	movs r0, #18
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl Func_02002814
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_020028dc
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #18
	movs r1, #0
	bl Func_020028bc
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #4
	lsls r1, r1, #1
	bl Func_020028e4
	movs r0, #5
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl Func_020028e4
	movs r0, #6
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #6
	lsls r1, r1, #1
	bl Func_020028e4
	movs r0, #12
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl Func_020028e4
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r0, #18
	movs r1, #0
	movs r2, #48
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl Func_02002874
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_020028dc
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl Func_020028bc
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r0, #12
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #12
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_020028bc
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #12
	ldr r1, .L_0200a7a0
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_0200a7a4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a710
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl ObjectMotion_ResetAndSetPosition
.L_0200a710:
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02002874
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200a7a0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a74e
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200a74e:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02002874
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200a7a0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a7a8
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
	b .L_0200a7a8
	.2byte 0x0000
.L_0200a790:
	.4byte 0x00019999
.L_0200a794:
	.4byte Func_02001398
.L_0200a798:
	.4byte 0x00023333
.L_0200a79c:
	.4byte 0x00011999
.L_0200a7a0:
	.4byte 0x00013333
.L_0200a7a4:
	.4byte gPartyState
.L_0200a7a8:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02002874
	bl Func_02002834
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.section .rodata.x0200a92c,"a",%progbits
	.global Data_0200292c
Data_0200292c:
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
	.global Data_0200295c
Data_0200295c:
	.4byte 0x002001a0
	.4byte 0x01b00250
	.4byte 0x02600030
	.4byte 0x000dffff
	.4byte 0xffe802b0
	.4byte 0x02c00260
	.4byte 0x0270fff8
	.4byte 0x000effff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200298c
Data_0200298c:
	.4byte 0x000000cb
	.4byte 0x101010ca
	.4byte 0xffffffff
	.4byte 0x102020ca
	.4byte 0xffffffff
	.4byte 0x103030ca
	.4byte 0xffffffff
	.4byte 0x104060cb
	.4byte 0xffffffff
	.4byte 0x105070cb
	.4byte 0xffffffff
	.4byte 0x106040cb
	.4byte 0xffffffff
	.4byte 0x107050cb
	.4byte 0xffffffff
	.4byte 0x1080a0cb
	.4byte 0xffffffff
	.4byte 0x1090b0cb
	.4byte 0xffffffff
	.4byte 0x10a080cb
	.4byte 0xffffffff
	.4byte 0x10b090cb
	.4byte 0xffffffff
	.4byte 0x10c0c0ca
	.4byte 0xffffffff
	.4byte 0x10d0e0cb
	.4byte 0xffffffff
	.4byte 0x10e0d0cb
	.4byte 0xffffffff
	.4byte 0x10f0f0ca
	.4byte 0xffffffff
	.4byte 0x110100ca
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02002a14
Data_02002a14:
	.4byte 0xffff009d
	.4byte 0x00000002
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00004000
	.4byte 0xffff009c
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001d000
	.4byte 0xffff00a0
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff00db
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0003b000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x024c0000
	.4byte 0x0003b000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00035000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00033000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00033000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x0003d000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x0001b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002b94
Data_02002b94:
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
	.4byte 0x00004401
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000002
	.4byte 0x0966001e
	.4byte Func_02000a5c
	.4byte 0x00000002
	.4byte 0x0969001f
	.4byte Func_020013f0
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002749
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000274a
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000274b
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000274c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000058
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte Func_02000104
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02000154
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002786
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002787
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002788
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002789
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_020001a0
	.4byte 0x00008d15
	.4byte 0x0967040d
	.4byte Func_020001ec
	.4byte 0x00008d15
	.4byte 0x0967040e
	.4byte Func_020001ec
	.4byte 0x00008d15
	.4byte 0x0967040f
	.4byte Func_020001ec
	.4byte 0x00008d15
	.4byte 0x09670410
	.4byte Func_020001ec
	.4byte 0x00008d15
	.4byte 0x09670411
	.4byte Func_020001ec
	.4byte 0x00008d15
	.4byte 0x09670413
	.4byte Func_020001ec
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002790
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002791
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002792
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002793
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002794
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002795
	.4byte 0x000000f3
	.4byte 0xffff00c8
	.4byte 0x00403064
	.4byte 0x000000f3
	.4byte 0xffff00c9
	.4byte 0x00403065
	.4byte 0x000000f3
	.4byte 0xffff00ca
	.4byte 0x00403066
	.4byte 0x000000f3
	.4byte 0xffff00cb
	.4byte 0x00403067
	.4byte 0x000000f3
	.4byte 0xffff00cc
	.4byte 0x00403068
	.4byte 0x000000f3
	.4byte 0xffff00cd
	.4byte 0x00403069
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x0040306a
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x0040306b
	.4byte 0x000001c3
	.4byte 0xffff00d0
	.4byte 0x0040306d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002dec
Data_02002dec:
	.4byte 0x00000026
