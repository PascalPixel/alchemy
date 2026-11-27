#ifndef ALCHEMY_SQUASH_AND_STRETCH_H
#define ALCHEMY_SQUASH_AND_STRETCH_H

/* The squash and stretch that people standing about play now and then
   (scSquashAndStretch). */

/* A random pause before an actor's next squash and stretch: 60 to 149
   ticks. */
#define SQUASH_AND_STRETCH_PAUSE() \
	(syMathRemainderUnsigned(syRandomNext(), 90) + 60)

#endif
