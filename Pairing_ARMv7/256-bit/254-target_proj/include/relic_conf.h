/*
 * RELIC is an Efficient LIbrary for Cryptography
 * Copyright (C) 2007, 2008, 2009 RELIC Authors
 *
 * This file is part of RELIC. RELIC is legal property of its developers,
 * whose names are not listed here. Please refer to the COPYRIGHT file
 * for contact information.
 *
 * RELIC is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * RELIC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with RELIC. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 *
 * Project configuration.
 *
 * @version $Id: relic_conf.h.in 45 2009-07-04 23:45:48Z dfaranha $
 * @ingroup relic
 */

#ifndef RELIC_CONF_H
#define RELIC_CONF_H

/** Project version. */
#define VERSION "0.2.1"

/** Debugging support. */
/* #undef DEBUG */
/** Profiling support. */
/* #undef PROFL */
/** Error handling support. */
/* #undef CHECK */
/** Verbose error messages. */
/* #undef VERBS */
/** Trace support. */
/* #undef TRACE */
/** Multithreading. */
/* #undef MULTI */
/** Build documentation. */
#define DOCUM
/** Build only the selected algorithms. */
/* #undef STRIP */
/** Build with printing disabled. */
/* #undef QUIET */
/** Build shared library. */
/* #undef SHLIB */
/** Build static library. */
#define STLIB
/** Build with multithreading support. */
/* #undef MULTI */

/** Number of times each test is ran. */
#define TESTS    50
/** Number of times each benchmark is ran. */
#define BENCH    50

/** Number of available cores. */
#define CORES    1

/** Atmel AVR ATMega128 8-bit architecture. */
#define AVR		 1
/** MSP430 16-bit architecture. */
#define MSP      2
/** ARM 32-bit architecture. */
#define ARM      3
/** Intel x86-compatible 32-bit architecture. */
#define X86		 4
/** AMD64-compatible 64-bit architecture. */
#define X86_64	 5 
/** Architecture. */
#define ARCH	 ARM

/** Size of word in this architecture. */
#define WORD     32

/** Byte boundary to align digit vectors. */
#define ALIGN    1

/** Build multiple precision integer module. */
#define WITH_BN
/** Build prime field module. */
#define WITH_FP
/** Build binary field module. */
#define WITH_FB
/** Build prime elliptic curve module. */
#define WITH_EP
/** Build binary elliptic curve module. */
#define WITH_EB
/** Build elliptic curve cryptography module. */
#define WITH_EC
/** Build pairings over prime curves module. */
#define WITH_PP
/** Build pairings over binary curves module. */
#define WITH_PB
/** Build pairing-based cryptography module. */
#define WITH_PC
/** Build hash functions. */
#define WITH_MD
/** Build cryptographic protocols. */
#define WITH_CP

/** Easy C-only backend. */
#define EASY	 1
/** GMP backend. */
#define GMP		 2
/** Arithmetic backend. */
#define ARITH    GMP

/** Required precision in bits. */
#define BN_PRECI 1024
/** A multiple precision integer can store w words. */
#define SINGLE	 0
/** A multiple precision integer can store the result of an addition. */
#define CARRY	 1
/** A multiple precision integer can store the result of a multiplication. */
#define DOUBLE	 2
/** Effective size of a multiple precision integer. */
#define BN_MAGNI DOUBLE
/** Number of Karatsuba steps. */
#define BN_KARAT 0

/** Multiple precision arithmetic method */
#define BN_METHD "COMBA;COMBA;MONTY;SLIDE;BASIC;BASIC"

/** Schoolbook multiplication. */
#define BASIC    1
/** Comba multiplication. */
#define COMBA    2
/** Chosen multiple precision multiplication method. */
#define BN_MUL   COMBA

/** Schoolbook squaring. */
#define BASIC    1
/** Comba squaring. */
#define COMBA    2
/** Chosen multiple precision multiplication method. */
#define BN_SQR   COMBA

/** Division modular reduction. */
#define BASIC    1
/** Barrett modular reduction. */
#define BARRT    2
/** Montgomery modular reduction. */
#define MONTY    3
/** Pseudo-Mersenne modular reduction. */
#define PMERS    4
/** Chosen multiple precision modular reduction method. */
#define BN_MOD   MONTY

/** Binary modular exponentiation. */
#define BASIC    1
/** Constant-time Montgomery ladder modular exponentiation. */
#define CONST    2
/** Sliding window modular exponentiation. */
#define SLIDE    3
/** Chosen multiple precision modular exponentiation method. */
#define BN_MXP   SLIDE

/** Basic Euclidean GCD Algorithm. */ 
#define BASIC    1
/** Lehmer's fast GCD Algorithm. */
#define LEHME    2
/** Stein's binary GCD Algorithm. */
#define STEIN    3
/** Chosen multiple precision greatest common divisor method. */
#define BN_GCD   BASIC

/** Basic prime generation. */
#define BASIC    1
/** Safe prime generation. */
#define SAFEP    2
/** Strong prime generation. */
#define STRON    3
/** Chosen prime generation algorithm. */
#define BN_GEN   BASIC

/** Prime field size in bits. */
#define FP_PRIME 256
/** Number of Karatsuba steps. */
#define FP_KARAT 0
/** Prefer Pseudo-Mersenne primes over random primes. */
#define FP_PMERS

/** Prime field arithmetic method */
#define FP_METHD "COMBA;COMBA;MONTY"

/** Schoolbook multiplication. */
#define BASIC    1
/** Comba multiplication. */
#define COMBA    2
/** Integrated modular multiplication. */
#define INTEG    3
/** Chosen prime field multiplication method. */
#define FP_MUL   COMBA

/** Schoolbook squaring. */
#define BASIC    1
/** Comba squaring. */
#define COMBA    2
/** Integrated modular squaring. */
#define INTEG    3
/** Chosen prime field multiplication method. */
#define FP_SQR   COMBA

/** Division-based reduction. */
#define BASIC    1
/** Fast reduction modulo special form prime. */
#define QUICK    2
/** Montgomery modular reduction. */
#define MONTY    3
/** Chosen prime field reduction method. */
#define FP_RDC   MONTY

/** Irreducible polynomial size in bits. */
#define FB_POLYN 283
/** Number of Karatsuba steps. */
#define FB_KARAT 0
/** Prefer trinomials over pentanomials. */
#define FB_TRINO

/** Binary field arithmetic method */
#define FB_METHD "LODAH;TABLE;QUICK;BASIC;QUICK;QUICK;EXGCD"

/** Shift-and-add multiplication. */
#define BASIC    1
/** Lopez-Dahab multiplication. */
#define LODAH	 2
/** Integrated modular multiplication. */
#define INTEG	 3
/** Left-to-right Comb multiplication. */
#define LCOMB	 4
/** Right-to-left Comb multiplication. */
#define RCOMB	 5
/** Chosen binary field multiplication method. */
#define FB_MUL   LODAH

/** Basic squaring. */
#define BASIC    1
/** Table-based squaring. */
#define TABLE    2
/** Integrated modular squaring. */
#define INTEG	 3
/** Chosen binary field squaring method. */
#define FB_SQR   TABLE

/** Shift-and-add modular reduction. */
#define BASIC    1
/** Fast reduction modulo a trinomial or pentanomial. */
#define QUICK	 2
/** Chosen binary field modular reduction method. */
#define FB_RDC   QUICK

/** Square root by repeated squaring. */
#define BASIC    1
/** Fast square root extraction. */
#define QUICK	 2
/** Chosen binary field modular reduction method. */
#define FB_SRT   BASIC

/** Trace by repeated squaring. */
#define BASIC    1
/** Fast trace computation. */
#define QUICK	 2
/** Chosen trace computation method. */
#define FB_TRC   QUICK

/** Solve by half-trace computation. */
#define BASIC    1
/** Solve with precomputed half-traces. */
#define QUICK	 2
/** Chosen method to solve a quadratic equation. */
#define FB_SLV   QUICK

/** Binary inversion. */
#define BASIC    1
/** Extended Euclidean algorithm. */
#define EXGCD    2
/** Almost inverse algorithm. */
#define ALMOS    3
/** Chosen binary field inversion method. */
#define FB_INV   EXGCD 

/** Support for ordinary curves. */
#define EP_ORDIN
/** Support for supersingular curves. */
#define EP_SUPER
/** Support for NIST standardized curves. */
#define EP_STAND
/** Use mixed coordinates. */
#define EP_MIXED
/** Build precomputation table for generator. */
#define EP_PRECO
/** Width of precomputation table for fixed point methods. */
#define EP_DEPTH 4
/** Width of window processing for unknown point methods. */
#define EP_WIDTH 4

/** Prime elliptic curve arithmetic method. */
#define EP_METHD "PROJC;WTNAF;COMBS;INTER"

/** Affine coordinates. */
#define BASIC	 1
/** L�pez-Dahab Projective coordinates. */
#define PROJC	 2
/** Chosen binary elliptic curve coordinate method. */
#define EP_ADD	 PROJC

/** Affine coordinates. */
#define BASIC	 1
/** L�pez-Dahab Projective coordinates. */
#define PROJC	 2
/** Chosen binary elliptic curve coordinate method. */
#define EB_ADD	 PROJC

/** Binary point multiplication. */ 
#define BASIC	 1
/** Constant-time point multiplication. */
#define CONST	 2
/** Sliding window. */ 
#define SLIDE    3
/** 4-width (T)NAF. */ 
#define WTNAF	 4
/** Chosen prime elliptic curve point multiplication method. */
#define EP_MUL	 WTNAF

/** Binary point multiplication. */ 
#define BASIC	 1
/** Yao's windowing method. */
#define YAOWI	 2
/** NAF windowing method. */ 
#define NAFWI	 3
/** w-(T)NAF method. */
#define WTNAF    4
/** Single-table comb method. */
#define COMBS	 5
/** Double-table comb method. */
#define COMBD    6
/** Chosen prime elliptic curve point multiplication method. */
#define EP_FIX	 COMBS

/** Basic simultaneouns point multiplication. */
#define BASIC    1
/** Shamir's trick. */
#define TRICK    2
/** Interleaving of w-(T)NAFs. */
#define INTER    3
/** Joint sparse form. */
#define JOINT    4
/** Chosen prime elliptic curve simulteanous point multiplication method. */
#define EP_SIM   INTER 

/** Support for ordinary curves. */
#define EB_ORDIN
/** Support for supersingular curves. */
#define EB_SUPER
/** Support for Koblitz anomalous binary curves. */
#define EB_KBLTZ
/** Support for NIST standardized curves. */
#define EB_STAND
/** Use mixed coordinates. */
#define EB_MIXED
/** Build precomputation table for generator. */
#define EB_PRECO
/** Width of precomputation table for fixed point methods. */
#define EB_DEPTH 4
/** Width of window processing for unknown point methods. */
#define EB_WIDTH 4

/** Binary elliptic curve arithmetic method. */
#define EB_METHD "PROJC;WTNAF;COMBS;INTER"

/** Affine coordinates. */
#define BASIC	 1
/** L�pez-Dahab Projective coordinates. */
#define PROJC	 2
/** Chosen binary elliptic curve coordinate method. */
#define EB_ADD	 PROJC

/** Binary point multiplication. */ 
#define BASIC	 1
/** Constant-time point multiplication. */
#define CONST	 2
/** Sliding window. */ 
#define SLIDE    3
/** 4-width (T)NAF. */ 
#define WTNAF	 4
/** Chosen binary elliptic curve point multiplication method. */
#define EB_MUL	 WTNAF

/** Binary point multiplication. */ 
#define BASIC	 1
/** Yao's windowing method. */
#define YAOWI	 2
/** NAF windowing method. */ 
#define NAFWI	 3
/** w-(T)NAF method. */
#define WTNAF    4
/** Single-table comb method. */
#define COMBS	 5
/** Double-table comb method. */
#define COMBD    6
/** Chosen binary elliptic curve point multiplication method. */
#define EB_FIX	 COMBS

/** Basic simultaneouns point multiplication. */
#define BASIC    1
/** Shamir's trick. */
#define TRICK    2
/** Interleaving of w-(T)NAFs. */
#define INTER    3
/** Joint sparse form. */
#define JOINT    4
/** Chosen binary elliptic curve simulteanous point multiplication method. */
#define EB_SIM   INTER 

/** Prefer Koblitz anomalous (binary or prime) curves. */
#define EC_KBLTZ

/** Binary elliptic curve arithmetic method. */
#define EC_METHD "PRIME"

/** Prime curves. */
#define PRIME	 1
/** Binary curves. */
#define BINAR	 2
/** Chosen elliptic curve cryptography method. */
#define EC_CUR	 PRIME

/** Parallel implementation. */
/* #undef PB_PARAL */

/** Bilinear pairing method. */
#define PB_METHD "ETATS"

/** Pairing computation with square roots. */ 
#define ETATS	 1
/** Pairing computation without square roots. */
#define ETATN	 2
/** Chosen binary elliptic curve point multiplication method. */
#define PB_MAP	 ETATS

/** Binary elliptic curve arithmetic method. */
#define PC_METHD "PRIME"

/** Prime curves. */
#define PRIME	 1
/** Binary curves. */
#define BINAR	 2
/** Chosen elliptic curve cryptography method. */
#define PC_CUR	 PRIME

/** Bilinear pairing method. */
#define MD_METHD "SH256"

/** SHA-1 hash function. */
#define SHONE    1
/** SHA-224 hash function. */
#define SH224    2
/** SHA-256 hash function. */
#define SH256    3
/** SHA-384 hash function. */
#define SH384    4
/** SHA-512 hash function. */
#define SH512    5
/** Chosen hash function. */
#define MD_MAP   SH256

/** RSA without padding. */
#define EMPTY    1
/** RSA PKCS#1 v1.5 padding. */
#define PKCS1    2
/** RSA PKCS#1 v2.1 padding. */
#define PKCS2    3
/** Chosen RSA padding method. */
#define CP_RSAPD   PKCS1

/** Slow RSA decryption/signature. */
#define BASIC    1
/** Fast RSA decryption/signature with CRT. */
#define QUICK    2
/** Chosen RSA method. */
#define CP_RSA     QUICK

/** Standard ECDSA. */
#define BASIC    1
/** ECDSA with fast verification. */
#define QUICK    2
/** Chosen ECDSA method. */
#define CP_ECDSA     

/** Automatic memory allocation. */
#define AUTO     1
/** Static memory allocation. */
#define	STATIC   2
/** Dynamic memory allocation. */
#define DYNAMIC  3
/** Stack memory allocation. */
#define STACK    4
/** Chosen memory allocation policy. */
#define ALLOC    AUTO

/** FIPS 186-2 generator. */
#define FIPS     1
/** Chosen random generator. */
#define RAND     FIPS

/** Standard C library generator. */
#define LIBC     1
/** Device node generator. */
#define DEV      2
/** Chosen random generator seeder. */
#define SEED     LIBC

/** Undefined/No operating system. */
#define NONE     1
/** GNU/Linux operating system. */
#define LINUX    2
/** FreeBSD operating system. */
#define FREEBSD  3
/** Windows operating system. */
#define WINDOWS  4
/** Detected operation system. */
#define OPSYS    LINUX

/** No timer. */
#define NONE     1
/** Per-process high-resolution timer. */
#define HREAL    2
/** Per-process high-resolution timer. */
#define HPROC    3
/** Per-thread high-resolution timer. */
#define HTHRD    4
/** POSIX-compatible timer. */
#define POSIX    5
/** ANSI-compatible timer. */
#define ANSI     6
/** Cycle-counting timer. */
#define CYCLE    7
/** Chosen timer. */
#define TIMER    CYCLE

#ifndef ASM

/**
 * Prints the project options selected at build time.
 */
void conf_print(void);

#endif /* ASM */

#endif /* !RELIC_CONF_H */
