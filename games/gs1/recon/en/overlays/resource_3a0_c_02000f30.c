#include "types.h"
/* AUDITED GENERATED CALL SCRIPT: actor-19 presentation and story progression. */
#define FieldScene_RunActorNineteenScript Func_02000f30
#define gFieldSceneWorkspace (*(u8 **)0x03001ebc)
void Func_020010ca(); void Func_020010de(); void Func_020010f2(); void Func_02002298();
void *Func_02002302(); void Func_02002304(); void Func_0200232a(); void Func_02002346();
void *Func_02002392(); s32 Func_0200239e(); void Func_020023aa(); void Func_020023c8();
void Func_020023f0(); void Func_020023fc(); void Func_0200241c(); void Func_0200242c();
void Func_02002458(); void Func_02002482(); void Func_020024d4();
void Func_02000f30(void) {
    void *actor;
    Func_02002302(); Func_0200232a(19,33592992); Func_02002298(33588057,3200);
    Func_02002346(19); Func_0200241c(124);
    Func_020010ca(11010048,524288,20447232,0,0,0,131073,0);
    Func_020010de(11010048,524288,20447232,13107,0,0,131073,0);
    Func_020010f2(11010048,524288,20447232,4294954189,0,0,131073,0);
    Func_02002304(33588057); actor=Func_02002392(19); *(u16 *)((*(void **)(actor+80))+30)=32768;
    Func_020023fc(21,11010048,20447232,32768); Func_02002392(20); Func_0200242c(14,19,0);
    Func_0200242c(14,2); Func_020023aa(10); Func_02002458(6141);
    if (Func_0200239e(515)!=0) (*(u16 *)(gFieldSceneWorkspace+472))++;
    Func_02002482(14,0); Func_020023c8(515); Func_020024d4(); Func_020023f0();
}
