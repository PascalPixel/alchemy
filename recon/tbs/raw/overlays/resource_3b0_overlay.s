.syntax unified
	.thumb
	.section .text.x02008240,"ax",%progbits
	.balign 4
	.p2align 2
	.global FuneHobashira_ApplyEntryState
	.thumb_func
FuneHobashira_ApplyEntryState:
	push {r5, lr}
	movs r0, #162
	lsls r0, r0, #1
	bl 0x02009204
	ldr r3, [pc, #396]
	movs r0, #224
	ldr r3, [r3]
	lsls r0, r0, #1
	ldr r2, [pc, #392]
	adds r3, r3, r0
	str r2, [r3]
	ldr r0, [pc, #388]
	bl 0x020091fc
	cmp r0, #0
	bne .L_02000240_0
	ldr r0, [pc, #384]
	bl 0x020091fc
	cmp r0, #0
	beq .L_02000240_1
.L_02000240_0:
	ldr r0, [pc, #376]
	bl 0x020091fc
	cmp r0, #0
	bne .L_02000240_1
	movs r0, #138
	lsls r0, r0, #4
	bl 0x020091fc
	cmp r0, #0
	bne .L_02000240_1
	ldr r5, [pc, #360]
	bl 0x020091c4
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [r5]
	ldr r5, [pc, #352]
	bl 0x020091c4
	lsls r0, r0, #16
	lsrs r0, r0, #16
	movs r1, #200
	str r0, [r5]
	lsls r1, r1, #4
	ldr r0, [pc, #340]
	bl 0x020091bc
.L_02000240_1:
	ldr r0, [pc, #336]
	bl 0x020091fc
	cmp r0, #0
	beq .L_02000240_2
	ldr r0, [pc, #312]
	bl 0x020091fc
	cmp r0, #0
	bne .L_02000240_2
	movs r1, #164
	movs r2, #164
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009264
.L_02000240_2:
	ldr r1, [pc, #308]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	subs r3, #1
	cmp r3, #13
	bhi .L_02000240_3
	ldr r2, [pc, #292]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	strh r0, [r3, #24]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r5, #26]
	lsls r0, r0, #8
	strh r6, [r7, #26]
	lsls r0, r0, #8
	strh r0, [r3, #28]
	lsls r0, r0, #8
	strh r2, [r6, #28]
	lsls r0, r0, #8
	strh r4, [r1, #30]
	lsls r0, r0, #8
	ldr r0, [pc, #232]
	bl 0x020091fc
	cmp r0, #0
	bne .L_02000240_3
	movs r0, #0
	bl 0x02009234
	adds r5, r0, #0
	bl 0x02009214
	bl 0x020092e4
	movs r3, #224
	lsls r3, r3, #14
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	str r3, [r5, #12]
	negs r0, r0
	movs r3, #0
	bl 0x020092c4
	movs r0, #1
	bl 0x020091b4
	movs r0, #0
	movs r1, #0
	bl 0x020092bc
	bl 0x020091dc
	movs r0, #1
	bl 0x020091b4
	bl 0x0200921c
	b .L_02000240_3
	.2byte 0x481e
	.2byte 0xf000
	.2byte 0xff47
	.2byte 0x2800
	.2byte 0xd002
	.2byte 0xf000
	.2byte 0xf879
	.2byte 0xe02b
	.2byte 0xf000
	.2byte 0xf848
	.2byte 0xe028
	.2byte 0x20e2
	.2byte 0x4b21
	.2byte 0x0040
	.2byte 0x180a
	.2byte 0x8013
	.2byte 0x23e3
	.2byte 0x005b
	.2byte 0x18ca
	.2byte 0x231e
	.2byte 0x8013
	.2byte 0xf000
	.2byte 0xf8e7
	.2byte 0xe01b
	.2byte 0x20e2
	.2byte 0x4b1b
	.2byte 0x0040
	.2byte 0x180a
	.2byte 0x8013
	.2byte 0x23e3
	.2byte 0x005b
	.2byte 0x18ca
	.2byte 0x231e
	.2byte 0x8013
	.2byte 0xf000
	.2byte 0xfa00
	.2byte 0xe00e
	.2byte 0x20e2
	.2byte 0x4b14
	.2byte 0x0040
	.2byte 0x180a
	.2byte 0x8013
	.2byte 0x23e3
	.2byte 0x005b
	.2byte 0x18ca
	.2byte 0x231e
	.2byte 0x8013
	.2byte 0xf000
	.2byte 0xfb97
	.2byte 0xe001
	.2byte 0xf000
	.2byte 0xfd54
.L_02000240_3:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x00000209
	.4byte 0x00000927
	.4byte 0x00000928
	.4byte 0x0000093e
	.4byte 0x02009940
	.4byte 0x02009928
	.4byte 0x020090a1
	.4byte 0x00000925
	.4byte 0x02000240
	.4byte 0x020082e0
	.4byte 0x00000109
	.2byte 0x006f
	.2byte 0x0000
	.section .text.x02008e78,"ax",%progbits
	.balign 4
	.p2align 2
	.global FieldScene_RunSevenActorEnsemble
	.thumb_func
FieldScene_RunSevenActorEnsemble:
	push {r5, r6, lr}
	bl 0x02009214
	movs r1, #15
	movs r0, #0
	bl 0x02009284
	movs r0, #0
	bl 0x02009234
	movs r1, #0
	bl 0x020091f4
	ldr r0, [pc, #464]
	bl 0x02009224
	movs r0, #1
	bl 0x020091b4
	movs r0, #9
	bl 0x02008a84
	movs r0, #10
	bl 0x02008a84
	movs r0, #11
	bl 0x02008a84
	movs r0, #12
	bl 0x02008a84
	movs r0, #13
	bl 0x02008a84
	movs r0, #14
	bl 0x02008a84
	movs r0, #15
	bl 0x02008a84
	ldr r1, [pc, #412]
	movs r0, #8
	bl 0x02009244
	ldr r5, [pc, #408]
	ldr r3, [pc, #412]
	ldr r2, [r5]
	movs r6, #224
	lsls r6, r6, #1
	str r3, [r2, r6]
	bl 0x020092ec
	bl 0x020092fc
	movs r0, #200
	lsls r0, r0, #1
	bl 0x0200920c
	movs r0, #9
	bl 0x0200924c
	movs r0, #10
	bl 0x0200924c
	movs r0, #11
	bl 0x0200924c
	movs r0, #12
	bl 0x0200924c
	movs r0, #13
	bl 0x0200924c
	movs r0, #14
	bl 0x0200924c
	movs r0, #15
	bl 0x0200924c
	movs r1, #192
	movs r2, #192
	movs r0, #9
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #11
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #12
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #13
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #14
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #9
	movs r0, #15
	lsls r1, r1, #10
	bl 0x0200923c
	ldr r1, [pc, #248]
	movs r0, #9
	bl 0x02009244
	ldr r1, [pc, #244]
	movs r0, #10
	bl 0x02009244
	ldr r1, [pc, #240]
	movs r0, #11
	bl 0x02009244
	ldr r1, [pc, #236]
	movs r0, #12
	bl 0x02009244
	ldr r1, [pc, #232]
	movs r0, #13
	bl 0x02009244
	ldr r1, [pc, #228]
	movs r0, #14
	bl 0x02009244
	ldr r1, [pc, #224]
	movs r0, #15
	bl 0x02009244
	movs r0, #40
	bl 0x0200920c
	movs r0, #8
	movs r1, #3
	bl 0x02009274
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x020092b4
	movs r0, #120
	bl 0x0200920c
	movs r0, #8
	movs r1, #1
	bl 0x02009274
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #60
	bl 0x020092ac
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200923c
	movs r2, #172
	movs r0, #8
	movs r1, #164
	lsls r2, r2, #1
	bl 0x0200925c
	movs r0, #8
	movs r1, #4
	movs r2, #10
	bl 0x0200926c
	movs r1, #6
	movs r2, #20
	movs r0, #8
	bl 0x0200926c
	ldr r0, [pc, #124]
	bl 0x0200928c
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009294
	ldr r2, [r5]
	ldr r3, [pc, #112]
	str r3, [r2, r6]
	bl 0x020092f4
	bl 0x020092fc
	ldr r1, [pc, #104]
	movs r0, #226
	ldr r2, [pc, #104]
	lsls r0, r0, #1
	adds r3, r1, r0
	strh r2, [r3]
	movs r3, #227
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #2
	strh r3, [r2]
	bl 0x02009130
	cmp r0, #11
	bne .L_02000e78_0
	movs r0, #15
	bl 0x020092d4
	b .L_02000e78_1
.L_02000e78_0:
	movs r0, #14
	bl 0x020092d4
.L_02000e78_1:
	bl 0x0200921c
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200976c
	.4byte 0x0200939c
	.4byte 0x03001ebc
	.4byte 0x00000203
	.4byte 0x02009450
	.4byte 0x02009480
	.4byte 0x020094b0
	.4byte 0x020094e0
	.4byte 0x02009510
	.4byte 0x02009540
	.4byte 0x02009570
	.4byte 0x00001ee4
	.4byte 0x00000202
	.4byte 0x02000240
	.4byte 0x0000006f
	.section .rodata,"a",%progbits
	.global FuneHobashira_FollowCameraActions
FuneHobashira_FollowCameraActions:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000800
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000800
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffff800
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global FuneHobashira_LookoutActions
FuneHobashira_LookoutActions:
	.4byte 0x00000022
	.4byte 0x0200807d
	.global FuneHobashira_DriftActions
FuneHobashira_DriftActions:
	.4byte 0x00000022
	.4byte 0x020080c1
	.global FuneHobashira_WaveActions
FuneHobashira_WaveActions:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffb000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002800
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x00f20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00a60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00dc0000
	.4byte 0x00000000
	.4byte 0x00860000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00e40000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x011a0000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x009a0000
	.4byte 0x00000000
	.4byte 0x00870000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x09090000
	.4byte 0x0d70090d
	.4byte 0x0db315cb
	.4byte 0x15f5164c
	.4byte 0x1f0e1aad
	.4byte 0x1f6f227a
	.4byte 0x23f223b1
	.4byte 0x2bf727f4
	.global FuneHobashira_SceneTableA
FuneHobashira_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x000000a5
	.4byte 0xc00000eb
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x003800b8
	.4byte 0xc0000160
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x006000a4
	.4byte 0xc0000141
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x006000a4
	.4byte 0xc0000141
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x006000a4
	.4byte 0xc0000141
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x006000a4
	.4byte 0xc0000141
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x006000a4
	.4byte 0xc0000141
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHobashira_SceneTableB
FuneHobashira_SceneTableB:
	.4byte 0x001000b0
	.4byte 0x00c0015c
	.4byte 0x016c0020
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHobashira_SceneTableC
FuneHobashira_SceneTableC:
	.4byte 0x0000006e
	.4byte 0x0010406d
	.4byte 0x00a0a06f
	.4byte 0x00b0d06f
	.4byte 0x00c0d06d
	.4byte 0x00d0e06d
	.4byte 0x00e1106d
	.4byte 0x00f1306d
	.4byte 0x000001ff
	.global FuneHobashira_SceneTableD
FuneHobashira_SceneTableD:
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHobashira_StagingObjects
FuneHobashira_StagingObjects:
	.4byte 0xffff011e
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHobashira_EnsembleObjects
FuneHobashira_EnsembleObjects:
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00a90000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x00860000
	.4byte 0x00000000
	.4byte 0x00bd0000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x00d10000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x00810000
	.4byte 0x00000000
	.4byte 0x00ae0000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x008b0000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x009a0000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x00024000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x01a40000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHobashira_LandingObjects
FuneHobashira_LandingObjects:
	.4byte 0xffff00c5
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0016
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
	.global FuneHobashira_SceneTableE
FuneHobashira_SceneTableE:
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte 0x020081f1
	.4byte 0x00000000
	.4byte 0x09280008
	.4byte 0x00001e23
	.4byte 0x00000000
	.4byte 0x08a00008
	.4byte 0x00001e25
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001f63
	.4byte 0x00008d15
	.4byte 0x09280008
	.4byte 0x00001e24
	.4byte 0x00008d15
	.4byte 0x08a00008
	.4byte 0x00001e26
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001f64
	.4byte 0x00000003
	.4byte 0x0924000b
	.4byte 0x020081fd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHobashira_FlagValues
FuneHobashira_FlagValues:
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000008
	.4byte 0x00000004
	.4byte 0x00000009
	.4byte 0x0000000c
	.4byte 0x00000006
	.4byte 0x00000003
	.4byte 0x00000000
