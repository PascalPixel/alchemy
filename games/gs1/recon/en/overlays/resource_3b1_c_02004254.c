#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunConditionalActorPresentation:
 * 83 calls, no loops, the actor-record transfer, and all event-dependent
 * presentation branches across the complete callable owner. */

#define FieldScene_RunConditionalActorPresentation Func_02004254

void Func_02007a5c();
void Func_02008b0e();
void Func_02008b50();
void Func_02008b7a();
void Func_02008bf8();
void Func_02008c1a();
void Func_02008ca8();
void Func_02008ce2();
void Func_02008d1c();
void Func_02008d20();
void Func_02008da2();
void Func_02008dca();
void Func_02008dda();
void Func_02008de8();
void Func_02008dfa();
s32 Func_02009290();
void Func_0200a6aa();
void Func_0200a726();
s32 Func_0200a77a();
void Func_0200a788();
void Func_0200a790();
void Func_0200a7b8();
void *Func_0200a7d4();
void Func_0200a7e0();
void Func_0200a7ea();
void Func_0200a81a();
u8 *Func_0200a828();
void Func_0200a842();
void Func_0200a852();
s32 Func_0200a85c();
void Func_0200a862();
void Func_0200a872();
void Func_0200a876();
s32 Func_0200a888();
void Func_0200a892();
void Func_0200a896();
void Func_0200a8b0();
void Func_0200a8be();
s32 Func_0200a8d0();
void Func_0200a8d6();
void Func_0200a8f8();
void Func_0200a908();
void Func_0200a916();
void Func_0200a920();
void Func_0200a928();
void Func_0200a942();
void Func_0200a94c();
void Func_0200a94e();
void Func_0200a980();
void Func_0200a988();
void Func_0200a99c();
void Func_0200a9a0();
void Func_0200a9a8();
void Func_0200a9c4();
void Func_0200a9c6();
void Func_0200a9c8();
void Func_0200a9d4();
void Func_0200a9e0();
void Func_0200aa06();
void Func_0200aa0e();
void Func_0200aa10();
void Func_0200aa18();
void Func_0200aa1a();
void Func_0200aa1c();
void Func_0200aa20();
void Func_0200aa26();
void Func_0200aa2a();
void Func_0200aa3a();
void Func_0200aa4a();
void Func_0200aa4c();
void *Func_0200aa58();
void Func_0200aa66();
void Func_0200aa7c();
void Func_0200aa8c();
void Func_0200aa8e();
void Func_0200aac2();
void Func_0200aace();
void Func_0200aad0();
void Func_0200aad6();
void Func_0200aae4();
void Func_0200aae6();
void Func_0200aafc();
void Func_0200ab18();
void Func_0200ab30();
void Func_0200ab38();
void Func_0200ab50();
void Func_0200aba8();

void Func_02004254(s32 scene_actor)
{
    s32 actor;
    s32 alternate_pose;
    s32 common_pose_height;
    u8 *record;

    Func_0200a726();
    Func_02008b50(24, 0, 0);
    Func_02008b0e(0, 432, 134, 32768);
    Func_02007a5c(1);
    Func_0200a6aa(1);
    Func_0200a8be();
    Func_0200a790(0, 52428, 26214);
    Func_0200a7d4(0, 406, 134);
    Func_0200a7e0(0, 406, 152);
    Func_0200a7ea(0, 421, 152);
    Func_0200a842(27, 1);
    Func_0200a788(20);
    Func_0200a862(27, 0, 10);
    if (Func_0200a77a(768) != 0) {
        actor = Func_02009290(scene_actor, 0);
        Func_0200a872(27, 1);
        Func_0200a7b8(20);
        Func_0200a892(27, 0, 10);
        Func_0200a8b0(7868);
        Func_02008b7a(40987);
        Func_0200a876(0, 3);
        Func_0200a81a(0, 65536, 32768);
        Func_0200a85c(0, 432, 168);
        Func_0200a908(0, 49152, 0);
        record = Func_0200a828(0);
        if (record != 0) {
            Func_0200a896(actor, *(u32 *)(record + 8), *(u32 *)(record + 16));
        }
        Func_0200a852(actor, 65536, 32768);
        Func_0200a896(actor, 448, 168);
        Func_0200a942(actor, 45056, 20);
        Func_0200a94e(27, 12288, 20);
        Func_0200a8d6(27, 3);
        Func_02008bf8(27);
        Func_0200a980(actor, 258, 60);
        Func_0200a920(27, 1);
        Func_0200a8f8(27, 3);
        Func_02008c1a(27);
        Func_0200a916(actor, 3);
        alternate_pose = 0;
        if (Func_0200a85c(2347) != 0) {
            Func_0200a99c(0, 8192, 0);
            Func_0200a9a8(27, 12288, 0);
            common_pose_height = 204;
            goto apply_common_pose;
        } else {
            if (Func_0200a888(2346) != 0) {
                Func_0200a928(0, 422, 154);
                Func_0200a9d4(0, 24576, 0);
                Func_0200a9e0(27, 20480, 0);
                Func_0200a94c(actor, 410, 204);
                Func_02008ca8(actor, 53248);
                alternate_pose = 1;
            } else if (Func_0200a8d0(2345) != 0) {
                Func_0200aa10(0, 8192, 0);
                Func_0200aa1c(27, 12288, 0);
                common_pose_height = 172;
apply_common_pose:
                Func_0200a988(actor, 470, common_pose_height);
                Func_02008ce2(actor, 45056);
            } else {
                Func_0200a9a0(0, 422, 154);
                Func_0200aa4c(0, 24576, 0);
                Func_0200aa58(27, 20480, 0);
                Func_0200a9c4(actor, 410, 172);
                Func_02008d20(actor, 53248);
                alternate_pose = 1;
            }
        }
        Func_0200aa3a(27, 0, 20);
        Func_02008d1c(8219);
        Func_0200aa18(0, 3);
        Func_0200aa20(27, 3);
        Func_0200a9c6(27, 65536, 32768);
        if (alternate_pose != 0) {
            Func_0200aa0e(27, 428, 164);
            Func_0200aa1a(27, 408, 164);
        }
        Func_0200aa26(27, 408, 134);
        Func_0200aa2a(27, 440, 134);
        Func_0200a9c8(40);
        Func_02008dfa(9, 10, 0);
    } else {
        Func_0200aac2(7863);
        actor = 40987;
        Func_0200aae6(actor, 0, 40);
        Func_0200ab18(27, 257, 60);
        Func_02008da2(actor);
    Func_0200ab30(0, 258);
        Func_0200aa06(60);
        Func_0200ab38(27, 259, 40);
        Func_0200aad0(27, 2);
        Func_02008dca(actor);
        Func_0200ab50(27, 261, 40);
        Func_02008dda(actor);
        Func_0200aad6(27, 4);
        Func_02008de8(actor);
        Func_0200aae4(0, 3);
        Func_0200aa4a(20);
        Func_0200aba8(4);
    }
}
