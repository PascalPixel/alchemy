.syntax unified
	.thumb
	.section .text.x02008b24,"ax",%progbits
	.p2align 2
	.global Func_02000b24
	.thumb_func
Func_02000b24:
	push	{r5, r6, lr}
	ldr	r3, [pc, #184]
	adds	r6, r1, #0
	ldr	r3, [r3, #0]
	movs	r1, #193
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #99
	bne.n	.L_02000b3e
	movs	r3, #0
	strh	r3, [r2, #0]
.L_02000b3e:
	ldr	r0, [pc, #164]
	bl 0x0200925c
	ldr	r3, [pc, #160]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #152]
	cmp	r2, r3
	bne.n	.L_02000b60
	ldr	r2, [pc, #152]
	adds	r0, r6, r2
	bl 0x02009254
	b.n	.L_02000b6e
.L_02000b60:
	ldr	r3, [pc, #144]
	cmp	r2, r3
	bne.n	.L_02000b6e
	ldr	r3, [pc, #144]
	adds	r0, r6, r3
	bl 0x02009254
.L_02000b6e:
	movs	r0, #132
	lsls	r0, r0, #2
	movs	r1, #0
	bl 0x0200926c
	movs	r0, #98
	movs	r1, #5
	bl 0x0200933c
	ldr	r1, [pc, #100]
	ldr	r3, [pc, #120]
	adds	r2, r1, r3
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r5, r1, #0
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02000bc6
	cmp	r6, #11
	bne.n	.L_02000baa
	movs	r0, #98
	movs	r1, #7
	bl 0x0200933c
	b.n	.L_02000bc6
.L_02000baa:
	cmp	r6, #12
	bne.n	.L_02000bc6
	movs	r1, #6
	movs	r0, #98
	bl 0x0200933c
	movs	r0, #12
	bl 0x020092a4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x020092cc
.L_02000bc6:
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200928c
	movs	r3, #3
	adds	r0, #85
	strb	r3, [r0, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0000020f
	.4byte 0x02000240
	.4byte 0x000000a4
	.4byte 0x000002f9
	.4byte 0x000000a5
	.4byte 0x00000309
	.2byte 0x022b
	.2byte 0x0000
	.section .text.x02008d24,"ax",%progbits
	.p2align 2
	.global FieldScene_RunOpeningAuxiliarySequence
	.thumb_func
FieldScene_RunOpeningAuxiliarySequence:
	push {lr}
	ldr r3, [pc, #140]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #132]
	sub sp, #8
	cmp r2, r3
	bne 0x02008dae
	movs r0, #14
	bl 0x0200928c
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r0, #14
	bl 0x0200928c
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	movs r1, #0
	movs r0, #14
	movs r2, #0
	bl 0x020092cc
	movs r3, #15
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #16
	movs r1, #44
	movs r2, #1
	bl 0x02009234
	movs r0, #100
	movs r1, #0
	movs r2, #0
	bl 0x0200935c
	movs r3, #127
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #12
	movs r1, #71
	movs r2, #1
	movs r3, #1
	bl 0x02009234
	movs r3, #12
	movs r2, #71
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #71
.L_02000d96:
	movs r2, #1
	movs r3, #1
	movs r0, #11
	bl 0x02009234
	ldr r0, [pc, #24]
.L_02000da2:
	bl 0x02009204
	ldr r3, [pc, #24]
	ldrh r2, [r3]
	ldr r3, [pc, #24]
	strh r2, [r3]
.L_02000dae:
	sub sp, #-8
	pop {r1}
	bx r1
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x00a5
	.2byte 0x0000
	.2byte 0x8ce5
	.2byte 0x0200
	.2byte 0x9a00
	.2byte 0x0200
	.2byte 0x019e
	.2byte 0x0500
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
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
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
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
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
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
	.4byte 0x0000001b
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200937c
	.4byte 0x020093b4
	.4byte 0x020093ec
	.4byte 0x00000022
	.4byte 0x02008325
	.4byte 0x00000022
	.4byte 0x0200833d
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008325
	.4byte 0x00000022
	.4byte 0x0200833d
	.4byte 0x00000010
	.global gSuharaSabakuEntrancesOther
gSuharaSabakuEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x00000100
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaSabakuEntrances1
gSuharaSabakuEntrances1:
	.4byte 0xffff0001
	.4byte 0x00000108
	.4byte 0x40000038
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000018
	.4byte 0x000003a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaSabakuEntrances2
gSuharaSabakuEntrances2:
	.4byte 0xffff0001
	.4byte 0x000001e8
	.4byte 0x800001c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001e8
	.4byte 0x800004b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000001b8
	.4byte 0x40000270
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000038
	.4byte 0x40000270
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaSabakuEntrances3
gSuharaSabakuEntrances3:
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0xc00000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001b8
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000278
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000288
	.4byte 0xc0000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaSabakuExits
gSuharaSabakuExits:
	.4byte 0x000000a4
	.4byte 0x0011c002
	.4byte 0x002010a5
	.4byte 0x000000a5
	.4byte 0x001020a4
	.4byte 0x00237002
	.4byte 0x003040a6
	.4byte 0x004010a6
	.4byte 0x000000a6
	.4byte 0x001040a5
	.4byte 0x002030a6
	.4byte 0x003020a6
	.4byte 0x004030a5
	.4byte 0x000001ff
	.global gSuharaSabakuPlacementsOther
gSuharaSabakuPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaSabakuPlacements1
gSuharaSabakuPlacements1:
	.4byte 0x03010121
	.4byte 0x02009430
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0x03020121
	.4byte 0x02009430
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0x03030121
	.4byte 0x02009430
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x01024000
	.4byte 0x03040121
	.4byte 0x02009430
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x01024000
	.4byte 0x03050121
	.4byte 0x02009430
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x01024000
	.4byte 0xffff0039
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
	.global gSuharaSabakuPlacements2
gSuharaSabakuPlacements2:
	.4byte 0x03110121
	.4byte 0x02009430
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x03120121
	.4byte 0x02009430
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0x03130121
	.4byte 0x02009430
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0x03140121
	.4byte 0x02009444
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0x03150121
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x005d005c
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaSabakuPlacements3
gSuharaSabakuPlacements3:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaSabakuActor12Action
gSuharaSabakuActor12Action:
	.4byte 0x00000022
	.4byte 0x020089cd
	.4byte 0x00000010
	.global gSuharaSabakuEventsOther
gSuharaSabakuEventsOther:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c401
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0x09b50009
	.4byte 0x02008839
	.4byte 0x00000002
	.4byte 0x03010029
	.4byte 0x02008559
	.4byte 0x00000002
	.4byte 0x0302002a
	.4byte 0x02008565
	.4byte 0x00000002
	.4byte 0x0303002b
	.4byte 0x02008571
	.4byte 0x00000002
	.4byte 0x0304002c
	.4byte 0x0200857d
	.4byte 0x00000002
	.4byte 0x0305002d
	.4byte 0x02008589
	.4byte 0x00000002
	.4byte 0x03110033
	.4byte 0x02008559
	.4byte 0x00000002
	.4byte 0x03120034
	.4byte 0x02008565
	.4byte 0x00000002
	.4byte 0x03130035
	.4byte 0x02008571
	.4byte 0x00000002
	.4byte 0x0206003d
	.4byte 0x020087fd
	.4byte 0x00000002
	.4byte 0x0207003e
	.4byte 0x02008809
	.4byte 0x00000002
	.4byte 0x0208003f
	.4byte 0x02008815
	.4byte 0x00000002
	.4byte 0x02090040
	.4byte 0x02008821
	.4byte 0x00000002
	.4byte 0x020a0041
	.4byte 0x0200882d
	.4byte 0x00000002
	.4byte 0x02060047
	.4byte 0x020087fd
	.4byte 0x00000002
	.4byte 0x02070048
	.4byte 0x02008809
	.4byte 0x00000002
	.4byte 0x02080049
	.4byte 0x02008815
	.4byte 0x00000002
	.4byte 0x03140036
	.4byte 0x0200857d
	.4byte 0x00000002
	.4byte 0x020e0051
	.4byte 0x02008add
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008315
	.4byte 0x00000003
	.4byte 0x03510064
	.4byte 0x00300000
	.4byte 0x00000013
	.4byte 0x0fb10067
	.4byte 0x0010008d
	.4byte 0x00000013
	.4byte 0x0fb20068
	.4byte 0x001000e5
	.4byte 0x00000013
	.4byte 0x0fb30065
	.4byte 0x001000c0
	.4byte 0x00000013
	.4byte 0x0ef60066
	.4byte 0x00500006
	.4byte 0x10002115
	.4byte 0x120f0008
	.4byte 0x02008b15
	.4byte 0x10002115
	.4byte 0x120f0009
	.4byte 0x02008b15
	.4byte 0x10002115
	.4byte 0x120f000a
	.4byte 0x02008b15
	.4byte 0x10002115
	.4byte 0x120f000b
	.4byte 0x02008b15
	.4byte 0x10002115
	.4byte 0x120f000c
	.4byte 0x02008b15
	.4byte 0x00002115
	.4byte 0x120f0008
	.4byte 0x02008b25
	.4byte 0x00002115
	.4byte 0x120f0009
	.4byte 0x02008b25
	.4byte 0x00002115
	.4byte 0x120f000a
	.4byte 0x02008b25
	.4byte 0x00002115
	.4byte 0x120f000b
	.4byte 0x02008b25
	.4byte 0x00002115
	.4byte 0x120f000c
	.4byte 0x02008b25
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008dc9
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008d25
	.4byte 0x00000006
	.4byte 0xffff0063
	.4byte 0x02008c01
	.4byte 0x00000006
	.4byte 0xffff0037
	.4byte 0x02008589
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSuharaSabakuEvents3
gSuharaSabakuEvents3:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.global gSuharaSabakuShownLevel
gSuharaSabakuShownLevel:
	.space 2
