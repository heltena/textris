/* compat.h - makes the 1997 Turbo C sources compile unchanged with clang.
 *
 * Included in the unity build after platform.c and before any game file.
 * (The basic types WORD, BYTE and PTR are defined at the top of platform.c,
 * which also defines TIPUS_H so that the original Tipus.h is skipped.)
 */
#ifndef TEXTRIS_WEB_COMPAT_H
#define TEXTRIS_WEB_COMPAT_H

/* Turbo C had a 16-bit int. The only plain "int" in the game is
 * "typedef int ENTER16;" in Gral.h, which must stay 16 bits wide. */
#define int short

/* Tecles.c and Video.c define tables as "const" that their headers declare
 * without it. Turbo C let that pass; clang calls it a redefinition. */
#define const

/* "BYTE main (void)" is not a valid C entry point for clang; here it is an
 * ordinary function that platform.c calls from port_run(). */
#define main textris_main

#endif
