# Battle

A battle in The Broken Seal goes in rounds. The player chooses a command for
everyone in the party, the enemies choose theirs, and then everybody acts in
turn, fastest first, until one side is gone or the party gets away. Two
pieces of code share every turn: the rules, which work out what happens, and
the show, which draws it. Watching how they hand over to each other explains
most of the battle code.

## What happens when Isaac casts Quake?

![Isaac casts Quake: the rock rises under the party, and the damage messages appear while it's still on screen, the aimed member taking more than the one beside him](https://alchemy.pascalpixel.com/abilities/quake.gif)

Everything the player can choose becomes an ability, and every ability is a
row number, the same row in every ability table. Quake is row 3. Its row of
`actions.tsv` says it strikes with Venus, reaches the aimed unit and one on
either side, costs 4 PP and has a power of 12. Every unit has a Power and a
Resist in each element, the numbers a party member's status screen lists, and
Quake's power is what the aimed unit takes when Isaac's Venus Power matches
its Venus Resist: 12 damage, plus a random 0 to 3. Its row of `actflags.tsv`
says which effect draws it, and its row of `actstat.tsv` how the battle shows
it. So any move can be followed from the menu to the damage it does by reading
one row across the tables in [Game rules](src/game/README.md).

In this recording Isaac's Quake lands on his own party, and the member he aimed
at takes more damage than the one beside him. That's the range at work:
Psynergy damage falls off with each step from the aimed unit, to 80 percent
one step away and 60 two steps away. Then the element comes in: each point
Isaac's Venus Power is ahead of the target's Venus Resist adds half a percent,
up to double, and each point behind takes half a percent off.
[Battles](src/battle/README.md#how-an-ability-resolves) has every formula and
table, worked through for Avalanche, Cure and Ragnarok.

## How can the damage appear before the effect is over?

Look at the recording again: "Ivan takes 17 damage!" comes up while the rocks
are still on screen. That takes some arranging, because an effect is an
ordinary function with a loop of its own. It draws a frame, waits for the next
one, and only returns when the animation is over, and the battle code that
called it is stuck waiting all that time.

So the battle shows its results from somewhere else: a task, a small routine
the game runs once every frame however deep the waiting goes. Before it starts
the effect, the battle schedules that task, `BattleEvent_Playback`, and the
task stays idle until the effect says the blow has landed. Most effects say so
on the frame of the impact, with `EFFECT_SHOW_DAMAGE`. From then on the task
works through the targets one at a time. For each one it runs the rules,
which change the target's HP and statuses and queue what should be shown,
the message's names and numbers, then the message, the flinch and the sound,
and it plays that queue over the following frames before moving on to the
next target.

The rules never draw and never wait, and the effect never does arithmetic.
The effect decides when the damage appears, and the rules alone decide how
much it is.

## How is an effect drawn?

![Isaac unleashes Flint: a burst of light gathers round him and strikes Ivan](https://alchemy.pascalpixel.com/abilities/flint.gif)

Most effects paint a picture of their own over the battle, a frame at a time:
a canvas 128 pixels square, shown on the background layer the hardware can
scale, stretched to twice its width so it spans the screen and blended over
the fighters. An effect copies small pictures onto it, moves particles of
light and rock around, and hands the finished frame to a callback that copies
it into video memory. The callback can fade what's left on the canvas as it
goes, so a spark's trail dies away without any code keeping track of it.

Every effect has the same shape: set up the canvas and the pictures, run a
fixed number of frames with each step in a fixed order, and release it all at
the end. The order matters more than it looks, because many steps draw random
numbers. Moving one step earlier would change every random value after it,
and the sparks would no longer fall where the original game puts them.

## Why are summons different?

![Isaac summons Judgment: the screen goes dark, Judgment rises out of the clouds and strikes, and the scene clears to a wide landscape](https://alchemy.pascalpixel.com/abilities/judgment.gif)

A summon isn't drawn on the canvas at all. It's a scene: the battle fades
away, and the summon's own backgrounds and sprites take over the screen until
it strikes. Its ability row carries a bit that sends it to
`BattleFx_RunSceneEffect` instead of the table of ordinary effects, and its
damage gets a bonus too, a few percent of the target's maximum HP for every
Djinni it uses. [Battle effects](src/effect/README.md) goes through the
effect families and how each one is drawn.
