/* resource_378: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_378_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/ENTRY_STATE.o"(.text)
        "*/resource_378_overlay.o"(.text.x020086e8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/ALTAR_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/LEADER_PRIORITY.o"(.text)
        "*/resource_378_overlay.o"(.text.x0200888c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/COPY_ACTOR_POSE.o"(.text)
        "*/resource_378_overlay.o"(.text.x0200a660)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/FOLLOW_LEADER.o"(.text)
        "*/resource_378_overlay.o"(.text.x0200a6f0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/SPAWN_EFFECT.o"(.text)
        "*/resource_378_overlay.o"(.text.x0200a7d4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/ITEM_ICON.o"(.text)
        "*/resource_378_overlay.o"(.text.x0200b144)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/SPAWN_SPARK.o"(.text)
        "*/resource_378_overlay.o"(.text.x0200b270)
        "*/resource_378_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
