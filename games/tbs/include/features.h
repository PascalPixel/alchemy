#ifndef ALCHEMY_FEATURES_H
#define ALCHEMY_FEATURES_H

// How the code asks for what an edition has. The same in both games: edition.h sets LANGUAGE and
// each feature's 0 or 1, and includes this. A feature or fix is asked for by name, #if FEATURE for
// a block of code and WHEN(FEATURE, on, off) for a single value. What the language itself decides
// is picked with JAPANESE_OR, BY_LANGUAGE or another of the pickers below. Each expands to the one
// value the edition takes, so the compiler sees only that. A value given to a picker may not hold a
// comma.

// on where the edition has flag (1), off where it has not (0).
#define WHEN(flag, on, off)                 WHEN_PICK(flag, on, off)
#define WHEN_PICK(flag, on, off)            WHEN_PICK_##flag(on, off)
#define WHEN_PICK_0(on, off)                off
#define WHEN_PICK_1(on, off)                on

// The value for the edition's language, one given for each.
#define BY_LANGUAGE(ja, en, de, es, fr, it) BY_LANGUAGE_EXPAND(LANGUAGE, ja, en, de, es, fr, it)
#define BY_LANGUAGE_EXPAND(language, ja, en, de, es, fr, it)                                       \
    BY_LANGUAGE_PICK(language, ja, en, de, es, fr, it)
#define BY_LANGUAGE_PICK(language, ja, en, de, es, fr, it)                                         \
    BY_LANGUAGE_##language(ja, en, de, es, fr, it)
#define BY_LANGUAGE_0(ja, en, de, es, fr, it) ja
#define BY_LANGUAGE_1(ja, en, de, es, fr, it) en
#define BY_LANGUAGE_2(ja, en, de, es, fr, it) de
#define BY_LANGUAGE_3(ja, en, de, es, fr, it) es
#define BY_LANGUAGE_4(ja, en, de, es, fr, it) fr
#define BY_LANGUAGE_5(ja, en, de, es, fr, it) it

// One language's value, and the others' shared one.
#define JAPANESE_OR(ja, others)               BY_LANGUAGE(ja, others, others, others, others, others)
#define ENGLISH_OR(en, others)                BY_LANGUAGE(others, en, others, others, others, others)
#define GERMAN_OR(de, others)                 BY_LANGUAGE(others, others, de, others, others, others)
#define SPANISH_OR(es, others)                BY_LANGUAGE(others, others, others, es, others, others)
#define FRENCH_OR(fr, others)                 BY_LANGUAGE(others, others, others, others, fr, others)
#define ITALIAN_OR(it, others)                BY_LANGUAGE(others, others, others, others, others, it)

#endif
