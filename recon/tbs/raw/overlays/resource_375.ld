/* resource_375: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/SCENE_TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/PLAN_VISIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/LEAVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/OVERHEAR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/ARRIVAL_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/ROCKSLIDE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/PROGRESS_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/IMPORT.o"(.text)
        "*/resource_375_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
