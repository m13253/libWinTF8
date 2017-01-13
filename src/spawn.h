/*
  Copyright (c) 2014 StarBrilliant <m13253@hotmail.com>
  All rights reserved.

  Redistribution and use in source and binary forms are permitted
  provided that the above copyright notice and this paragraph are
  duplicated in all such forms and that any documentation,
  advertising materials, and other materials related to such
  distribution and use acknowledge that the software was developed by
  StarBrilliant.
  The name of StarBrilliant may not be used to endorse or promote
  products derived from this software without specific prior written
  permission.

  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
#pragma once
#ifndef WTF8_SPAWN_H_INCLUDED_
#define WTF8_SPAWN_H_INCLUDED_

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>
#else
#include <stddef.h>
#include <stdint.h>
#endif
#include "u8str.h"

#ifdef _WIN32
typedef uint32_t WTF8_pid_t;
#else
typedef int WTF8_pid_t;
#endif

#ifdef __cplusplus
/*!
 *  \addtogroup WTF8
 * libWinTF8 API for C++
 *  @{
 */

namespace WTF8 {
//! This exception will be thrown when libWTF8 have issues dealing with spawning sub-process
class process_spawn_error : public std::runtime_error {
public:
    process_spawn_error(const char *what) : std::runtime_error(what) {}
    process_spawn_error() : std::runtime_error("Unable to create a new process") {}
};

/*!\brief Start a process with the given executable filename and given arguments
 * \param file Path to the executable
 * \param argv Arrguments for the executable
 *
 * \remark argv[0] should be the executable name of the new process
 *
 * \result
 *   The Process ID of the new process
 *
 * \throws
 *   WTF8::process_spawn_error
 */
WTF8_pid_t spawnvp(const u8string &file, const std::vector<u8string> &argv);

/*!\brief Wait for a process to terminate and optionally fetch its exit code
 * \param pid PID of the process you want to wait for
 * \param force The exit status you expect the target process to return
 * \result
 *   true on success, false on failure
 */
bool waitpid(WTF8_pid_t pid, int *exit_code = nullptr);

/*!\brief Kill a process
 *
 * \param pid PID of the process you want to kill
 * \param force If you want to kill the process by force (like using SIGKILL instead of SIGTERM)
 *              Set to non-zero value to enable
 * \result
 *   true on success, false on failure
 */
bool kill(WTF8_pid_t pid, bool force = false);

}
#endif
/*! @} End of Doxygen Groups*/
/*!
 *  \addtogroup WTF8_C
 * libWinTF8 API extension for C
 *  @{
 */
#ifdef __cplusplus
extern "C" {
#endif

static const WTF8_pid_t WTF8_PROCESS_SPAWN_ERROR = ~(WTF8_pid_t) 0 /* -1 */;

/*!\brief Start a process with the given executable filename and given arguments
 *
 * \remark argv[0] should be the executable name of the new process
 * \param file Path to the executable
 * \param argv Arrguments for the executable
 *
 * \result
 *   The Process ID of the new process
 *
 * \throws
 *   Return WTF8_PROCESS_SPAWN_ERROR on failure
 */
WTF8_pid_t WTF8_spawnvp(const char *file, char *const *argv);

/*!\brief Wait for a process to terminate and optionally fetch its exit code
 * 
 * \param pid PID of the process you want to wait for
 * \param force The exit status you expect the target process to return
 * 
 * \result
 *   non-zero on success, zero on failure
 */
int WTF8_waitpid(WTF8_pid_t pid, int *exit_code);

/*!\brief Kill a process
 * 
 * \param pid PID of the process you want to kill
 * \param force If you want to kill the process by force (like using SIGKILL instead of SIGTERM)
 *              Set to non-zero value to enable
 *
 * \result
 *   non-zero on success, zero on failure
 */
int WTF8_kill(WTF8_pid_t pid, int force);

#ifdef __cplusplus
}
#endif

#endif
/*! @} End of Doxygen Groups*/
