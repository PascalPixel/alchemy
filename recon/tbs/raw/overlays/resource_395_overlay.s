.syntax unified
	.thumb
	.section .text.x02009070,"ax",%progbits
	.balign 4
	.global Func_02001070
	.thumb_func
Func_02001070:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #10
	bl 0x020099b0
	mov r8, r0
	movs r0, #14
	bl 0x020099b0
	mov r10, r0
	movs r0, #11
	bl 0x020099b0
	adds r7, r0, #0
	movs r0, #1
	bl 0x02009908
	movs r0, #14
	movs r1, #15
	bl 0x02009a18
	ldr r3, [pc, #56]
	movs r0, #224
	ldr r3, [r3]
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	ldr r2, [pc, #44]
	adds r0, #128
	ldr r1, [pc, #44]
	adds r3, r2, r0
	strh r1, [r3]
	ldr r3, [pc, #40]
	adds r2, r2, r3
	movs r3, #4
	strh r3, [r2]
	ldr r0, [pc, #36]
	ldr r6, [pc, #16]
	bl 0x02009980
	cmp r0, #0
	bne .L_02001070_0
	movs r0, #3
	bl 0x02009768
	b .L_02001070_0
	.4byte 0x00000000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000028
	.4byte 0x00000242
	.4byte 0x00000845
.L_02001070_0:
	movs r0, #8
	bl 0x020099b0
	movs r5, #6
	strh r5, [r0, #32]
	movs r0, #9
	bl 0x020099b0
	strh r5, [r0, #32]
	movs r0, #12
	bl 0x020099b0
	strh r5, [r0, #32]
	movs r0, #13
	bl 0x020099b0
	strh r5, [r0, #32]
	movs r0, #14
	bl 0x020099b0
	movs r1, #0
	bl 0x02009958
	movs r0, #10
	bl 0x020099b0
	movs r1, #0
	bl 0x02009958
	movs r0, #11
	bl 0x020099b0
	movs r1, #0
	bl 0x02009958
	movs r0, #8
	bl 0x020099b0
	movs r1, #0
	bl 0x02009958
	movs r0, #9
	bl 0x020099b0
	movs r1, #0
	bl 0x02009958
	movs r0, #8
	movs r1, #2
	bl 0x02009a48
	movs r0, #14
	movs r1, #2
	bl 0x02009a48
	movs r0, #9
	movs r1, #2
	bl 0x02009a48
	mov r3, r8
	adds r3, #85
	strb r6, [r3]
	adds r2, r7, #0
	movs r3, #224
	lsls r3, r3, #13
	mov r0, r8
	adds r2, #85
	str r3, [r0, #12]
	strb r6, [r2]
	mov r2, r10
	adds r2, #85
	str r3, [r7, #12]
	strb r6, [r2]
	mov r2, r10
	str r3, [r2, #12]
	movs r0, #9
	movs r1, #3
	bl 0x020099f0
	movs r1, #3
	movs r0, #8
	bl 0x020099f0
	movs r0, #8
	bl 0x020099b0
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #8
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl 0x020099b0
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #10
	bl 0x020099b0
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl 0x020099b0
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #14
	bl 0x020099b0
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.section .rodata,"a",%progbits
	.global SceneAction_ActorOneEntry
SceneAction_ActorOneEntry:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e60000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global SceneAction_ActorTwoEntry
SceneAction_ActorTwoEntry:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x015a0000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global SceneAction_ActorThreeEntry
SceneAction_ActorThreeEntry:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x014d0000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global SceneAction_GroupFinish
SceneAction_GroupFinish:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00d40000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gKorimaKiEntrances
gKorimaKiEntrances:
	.4byte 0xffff0000
	.4byte 0x000000d1
	.4byte 0x40000117
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x000001c0
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0xc00001a8
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x000001c0
	.4byte 0xffff0002
	.4byte 0x00100148
	.4byte 0x400000c8
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x000001c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKorimaKiRegions
gKorimaKiRegions:
	.4byte 0x00580140
	.4byte 0x015000b0
	.4byte 0x00c00068
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKorimaKiExits
gKorimaKiExits:
	.4byte 0x0000002c
	.4byte 0x0010202b
	.4byte 0x0020102d
	.4byte 0x000001ff
	.global gKorimaKiPlacements
gKorimaKiPlacements:
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x010e0000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x010e0000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00024000
	.4byte 0xffff0037
	.4byte 0x00000001
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKorimaKiEvents
gKorimaKiEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000b814
	.4byte 0x0845000c
	.4byte 0x020082c1
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008089
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008159
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x02008249
	.4byte 0x00008d15
	.4byte 0x0844040c
	.4byte 0x02008159
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x02008285
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKorimaKiEffectScript
gKorimaKiEffectScript:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001999
	.4byte 0x00000022
	.4byte 0x020091e9
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.space 8
	.global KorimaPalette_First
KorimaPalette_First:
	.space 1792
	.global KorimaPalette_Second
KorimaPalette_Second:
	.space 896
