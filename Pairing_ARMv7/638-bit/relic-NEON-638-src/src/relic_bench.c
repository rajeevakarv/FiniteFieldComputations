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
 * Implementation of useful benchmark routines.
 *
 * @version $Id: relic_bench.c 242 2010-01-29 01:48:37Z dfaranha $
 * @ingroup relic
 */

#include <stdio.h>
#include <string.h>

#include "relic_bench.h"
#include "relic_conf.h"
#include "relic_util.h"


/*============================================================================*/
/* Private definitions                                                        */
/*============================================================================*/

/**
 * Timer type.
 */
#if TIMER == HREAL || TIMER == HPROC || TIMER == HTHRD

#include <sys/time.h>
#include <time.h>
typedef struct timespec bench_t;

#elif TIMER == ANSI

#include <time.h>
typedef clock_t bench_t;

#elif TIMER == POSIX

#include <sys/time.h>
typedef struct timeval bench_t;

#elif TIMER == CYCLE

typedef unsigned long long bench_t;
#if ARCH == X86
static inline bench_t cycles(void) {
	unsigned long long int x;
	asm volatile (".byte 0x0f, 0x31\n\t":"=A" (x));
	return x;
}
#elif ARCH == X86_64
static inline bench_t cycles(void) {
	unsigned int hi, lo;
	asm volatile ("rdtsc\n\t":"=a" (lo), "=d"(hi));
	return ((bench_t) lo) | (((bench_t) hi) << 32);
}
#elif ARCH == MSP
#include "msp430util.h"
#define cycles()		msp430_get_cycles()

#elif ARCH == ARM
#include "relic_cpucycles.h"
#define SCALE 1
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "relic_cpucycles.h"
#define cycles()		cpucycles()
#endif

#else

typedef unsigned long long bench_t;

#endif

#if TIMER == CYCLE && ARCH == ARM
static int enabled = 0;

static int prev[3];
static unsigned long long prevcycles = 0;
static int now[3];
static long long cyclespersec = 0;

static void readticks(unsigned int *result)
{
  struct timeval t;
  unsigned int cc;
  if (!enabled) {
    asm volatile("mcr p15, 0, %0, c9, c12, 0" :: "r"(17));
    asm volatile("mcr p15, 0, %0, c9, c12, 1" :: "r"(0x8000000f));
    asm volatile("mcr p15, 0, %0, c9, c12, 3" :: "r"(0x8000000f));
    enabled = 1;
  }
  asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(cc));
  gettimeofday(&t,(struct timezone *) 0);
  result[0] = cc;
  result[1] = t.tv_usec;
  result[2] = t.tv_sec;
}

long long cpucycles_cortex(void)
{
  unsigned long long delta4;
  int deltan;
  int deltas;
  unsigned long long guesscycles;
  
  readticks(now);
  delta4 = (unsigned int) (now[0] - prev[0]); /* unsigned change in number of cycles mod 2^32 */
  deltan = now[1] - prev[1]; /* signed change in number of nanoseconds mod 10^9 */
  deltas = now[2] - prev[2]; /* signed change in number of seconds */
  if ((deltas == 0 && deltan < 200000) || (deltas == 1 && deltan < -800000))
    return (prevcycles + delta4) * SCALE;

  prev[0] = now[0];
  prev[1] = now[1];
  prev[2] = now[2];

  if ((deltas == 0 && deltan < 300000) || (deltas == 1 && deltan < -700000)) {
    // actual number of cycles cannot have increased by 2^32 in <0.3ms
    cyclespersec = 1000000 * (unsigned long long) delta4;
    cyclespersec /= deltan + 1000000 * (long long) deltas;
  } else {
    guesscycles = deltas * cyclespersec;
    guesscycles += (deltan * cyclespersec) / 1000000;
    while (delta4 + 2147483648ULL < guesscycles) delta4 += 4294967296ULL;
    /* XXX: could do longer-term extrapolation here */
  }

  prevcycles += delta4;
  return prevcycles * SCALE;
}

long long cpucycles_cortex_persecond(void)
{
  while (!cyclespersec) cpucycles_cortex();
  return cyclespersec * SCALE;
}
#endif


/**
 * Shared parameter for these timer.
 */
#if TIMER == HREAL
#define CLOCK			CLOCK_REALTIME
#elif TIMER == HPROC
#define CLOCK			CLOCK_PROCESS_CPUTIME_ID
#elif TIMER == HTHRD
#define CLOCK			CLOCK_THREAD_CPUTIME_ID
#else
#define CLOCK			NULL
#endif

/**
 * Stores the time measured before the execution of the benchmark.
 */
static bench_t before;

/**
 * Stores the time measured after the execution of the benchmark.
 */
static bench_t after;

/**
 * Stores the sum of timings for the current benchmark.
 */
static long long total;

/**
 * Benchmarking overhead to be measured and subtracted from benchmarks.
 */
static long long overhead;

/**
 * Dummy function for measuring benchmarking overhead.
 *
 * @param a				- the dummy parameter.
 */
static void empty(int *a) {
	(*a)++;
}

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

void bench_overhead(void) {
#if TIMER != NONE
	int a[BENCH + 1];
	int *tmpa;

	do {
		overhead = 0;
		for (int l = 0; l < BENCH; l++) {
			total = 0;
			/* Measure the cost of (n^2 + over). */
			bench_before();
			for (int i = 0; i < BENCH; i++) {
				tmpa = a;
				for (int j = 0; j < BENCH; j++) {
					empty(tmpa++);
				}
			}
			bench_after();
			/* Add the cost of (n^2 + over). */
			overhead += total;
		}
		/* Overhead stores the cost of n*(n^2 + over). */
		for (int l = 0; l < BENCH; l++) {
			total = 0;
			/* Measure the cost of (n^3 + over). */
			bench_before();
			for (int i = 0; i < BENCH; i++) {
				for (int k = 0; k < BENCH; k++) {
					tmpa = a;
					for (int j = 0; j < BENCH; j++) {
						empty(tmpa++);
					}
				}
			}
			bench_after();
			/* Subtract the cost of n^2. */
			overhead -= total / BENCH;
		}
		/* Now overhead stores n*over, so take the average to obtain the overhead
		 * to execute BENCH operations inside a benchmark. */
		overhead /= BENCH;
		/* Divide to obtain the overhead of one operation pair. */
		overhead /= BENCH;
		/* We assume that our overhead estimate is too high (due to cache
		 * effects, per example). The ratio 1/2 was found experimentally. */
		overhead = overhead / 2;
	} while (overhead < 0);

	total = overhead;
	bench_print();
#endif
}

void bench_reset() {
#if TIMER != NONE
	total = 0;
#else
	(void)before;
	(void)after;
	(void)overhead;
	(void)empty;
#endif
}

void bench_before() {
#if TIMER == HREAL || TIMER == HPROC || TIMER == HTHRD
	clock_gettime(CLOCK, &before);
#elif TIMER == ANSI
	before = clock();
#elif TIMER == POSIX
	gettimeofday(&before, NULL);
#elif TIMER == CYCLE
	before = cycles();
#endif
}

void bench_after() {
	long long result;
#if TIMER == HREAL || TIMER == HPROC || TIMER == HTHRD
	clock_gettime(CLOCK, &after);
	result = ((long)after.tv_sec - (long)before.tv_sec) * 1000000000;
	result += (after.tv_nsec - before.tv_nsec);
#elif TIMER == ANSI
	after = clock();
	result = (after - before) * 1000000000 / CLOCKS_PER_SEC;
#elif TIMER == POSIX
	gettimeofday(&after, NULL);
	result = ((long)after.tv_sec - (long)before.tv_sec) * 1000000000;
	result += (after.tv_usec - before.tv_usec) * 1000;
#elif TIMER == CYCLE
	after = cycles();
	result = (after - before);
#endif

#if TIMER != NONE
	total += result;
#else
	(void)result;
#endif
}

void bench_compute(int benches) {
#if TIMER != NONE
	total = total / benches - overhead;
#else
	(void)benches;
#endif
}

void bench_print() {
#if TIMER == CYCLE
	util_print("%lld cycles", total);
#else
	util_print("%lld nanosec", total);
#endif
	if (total < 0) {
		util_print(" (bad overhead estimation)\n");
	} else {
		util_print("\n");
	}
}

unsigned long long bench_get_total() {
	return total;
}

