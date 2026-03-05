/*
 * platform.hpp - Platform detection for Buck2 builds of libzmq.
 *
 * CMake normally generates this file with detected values.
 * For Buck2 builds, we use preprocessor detection instead.
 */

#ifndef __ZMQ_PLATFORM_HPP_INCLUDED__
#define __ZMQ_PLATFORM_HPP_INCLUDED__

/* Common settings */
#define ZMQ_POLL_BASED_ON_POLL 1
#define HAVE_STRNLEN 1
#define ZMQ_HAVE_UIO 1
#define ZMQ_HAVE_IPC 1
#define ZMQ_HAVE_STRUCT_SOCKADDR_UN 1
#define HAVE_POSIX_MEMALIGN 1

/* macOS */
#if defined(__APPLE__)
#define ZMQ_IOTHREAD_POLLER_USE_KQUEUE 1
#define ZMQ_HAVE_STRLCPY 1
#endif

/* Linux */
#if defined(__linux__)
#define ZMQ_HAVE_LINUX 1
#define ZMQ_IOTHREAD_POLLER_USE_EPOLL 1
#endif

/* Windows */
#if defined(_WIN32)
#define ZMQ_HAVE_WINDOWS 1
#define ZMQ_IOTHREAD_POLLER_USE_SELECT 1
#define _WIN32_WINNT 0x0600
#endif

#endif
