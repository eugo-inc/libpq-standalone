/*------------------------------------------------------------------------
 * PostgreSQL manual configuration settings
 *
 * This file contains various configuration symbols and limits.  In
 * all cases, changing them is only useful in very rare situations or
 * for developers.  If you edit any of these, be sure to do a *full*
 * rebuild (and an initdb if noted).
 *
 * Portions Copyright (c) 1996-2024, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/pg_config_manual.h
 *------------------------------------------------------------------------
 */

/*
 * Maximum length for identifiers (e.g. table names, column names,
 * function names).  Names actually are limited to one fewer byte than this,
 * because the length must include a trailing zero byte.
 *
 * Changing this requires an initdb.
 */
#define NAMEDATALEN 64

/*
 * Decide whether built-in 8-byte types, including float8, int8, and
 * timestamp, are passed by value.  This is on by default if sizeof(Datum) >=
 * 8 (that is, on 64-bit platforms).  If sizeof(Datum) < 8 (32-bit platforms),
 * this must be off.  We keep this here as an option so that it is easy to
 * test the pass-by-reference code paths on 64-bit platforms.
 *
 * Changing this requires an initdb.
 */
#if SIZEOF_VOID_P >= 8
#define USE_FLOAT8_BYVAL 1
#endif


/*
 * MAXPGPATH: standard size of a pathname buffer in PostgreSQL (hence,
 * maximum usable pathname length is one less).
 *
 * We'd use a standard system header symbol for this, if there weren't
 * so many to choose from: MAXPATHLEN, MAX_PATH, PATH_MAX are all
 * defined by different "standards", and often have different values
 * on the same platform!  So we just punt and use a reasonably
 * generous setting here.
 */
#define MAXPGPATH		1024

/*
 * You can try changing this if you have a machine with bytes of
 * another size, but no guarantee...
 */
#define BITS_PER_BYTE		8

/*
 * Preferred alignment for disk I/O buffers.  On some CPUs, copies between
 * user space and kernel space are significantly faster if the user buffer
 * is aligned on a larger-than-MAXALIGN boundary.  Ideally this should be
 * a platform-dependent value, but for now we just hard-wire it.
 */
#define ALIGNOF_BUFFER	32

/*
 * If EXEC_BACKEND is defined, the postmaster uses an alternative method for
 * starting subprocesses: Instead of simply using fork(), as is standard on
 * Unix platforms, it uses fork()+exec() or something equivalent on Windows,
 * as well as lots of extra code to bring the required global state to those
 * new processes.  This must be enabled on Windows (because there is no
 * fork()).  On other platforms, it's only useful for verifying those
 * otherwise Windows-specific code paths.
 */
#if defined(WIN32) && !defined(__CYGWIN__)
#define EXEC_BACKEND
#endif

/*
 * USE_SSL code should be compiled only when compiling with an SSL
 * implementation.
 */
#ifdef USE_OPENSSL
#define USE_SSL
#endif

/*
 * This is the default directory in which AF_UNIX socket files are
 * placed.  Caution: changing this risks breaking your existing client
 * applications, which are likely to continue to look in the old
 * directory.  But if you just hate the idea of sockets in /tmp,
 * here's where to twiddle it.  You can also override this at runtime
 * with the postmaster's -k switch.
 *
 * If set to an empty string, then AF_UNIX sockets are not used by default: A
 * server will not create an AF_UNIX socket unless the run-time configuration
 * is changed, a client will connect via TCP/IP by default and will only use
 * an AF_UNIX socket if one is explicitly specified.
 *
 * This is done by default on Windows because there is no good standard
 * location for AF_UNIX sockets and many installations on Windows don't
 * support them yet.
 */
#ifndef WIN32
#define DEFAULT_PGSOCKET_DIR  "/tmp"
#else
#define DEFAULT_PGSOCKET_DIR ""
#endif

/*
 * Assumed cache line size.  This doesn't affect correctness, but can be used
 * for low-level optimizations.  This is mostly used to pad various data
 * structures, to ensure that highly-contended fields are on different cache
 * lines.  Too small a value can hurt performance due to false sharing, while
 * the only downside of too large a value is a few bytes of wasted memory.
 * The default is 128, which should be large enough for all supported
 * platforms.
 */
#define PG_CACHE_LINE_SIZE		128
