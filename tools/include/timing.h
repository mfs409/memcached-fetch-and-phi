// -*-c++-*-
//
//  Copyright (C) 2011, 2014
//  University of Rochester Department of Computer Science
//    and
//  Lehigh University Department of Computer Science and Engineering
//
// License: Modified BSD
//          Please see the file LICENSE for licensing information

#pragma once

#include <thread>
#include <unistd.h>

/**
 *  sleep_ms simply wraps the POSIX usleep call.  Note that usleep expects a
 *  number of microseconds, not milliseconds
 */
inline void sleep_ms(uint32_t ms) { usleep(ms*1000); }

/**
 *  Yield the CPU
 */
inline void yield_cpu() { std::this_thread::yield(); }

/**
 *  The Linux clock_gettime is reasonably fast, has good resolution, and is not
 *  affected by TurboBoost.  Using MONOTONIC_RAW also means that the timer is
 *  not subject to NTP adjustments, which is preferably since an adjustment in
 *  mid-experiment could produce some funky results.
 */
inline uint64_t getElapsedTime()
{
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  uint64_t tt = (((long long)t.tv_sec) * 1000000000L) + ((long long)t.tv_nsec);
  return tt;
}
