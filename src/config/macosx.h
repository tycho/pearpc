/* Define to 1 if you have the <asm/types.h> header file. */
/* #undef HAVE_ASM_TYPES_H */

/* Have TUN/TAP headers? */
/* #undef HAVE_BEOS_TUN */

/* Define to 1 if you have the `exp2' function. */
#define HAVE_EXP2 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <linux/if_tun.h> header file. */
/* #undef HAVE_LINUX_IF_TUN_H */

/* Define to 1 if you have the <linux/netlink.h> header file. */
/* #undef HAVE_LINUX_NETLINK_H */

/* Have TUN/TAP headers? */
/* #undef HAVE_LINUX_TUN */

/* Define to 1 if you have the `log2' function. */
#define HAVE_LOG2 1

/* Define to 1 if you have the <mach/clock.h> header file. */
#define HAVE_MACH_CLOCK_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the <pcap.h> header file. */
#define HAVE_PCAP_H 1

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define to 1 if you have the `setenv' function. */
#define HAVE_SETENV 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Which host endianess/byte-order? */
#define HOST_ENDIANESS HOST_ENDIANESS_LE

/* big-endian byte-order helper constant */
#define HOST_ENDIANESS_BE 1234

/* little-endian byte-order helper constant */
#define HOST_ENDIANESS_LE 4321

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Have pthread.h? */
#define PTHREAD_HDR <pthread.h>

/* The size of `char', as computed by sizeof. */
#define SIZEOF_CHAR 1

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long int', as computed by sizeof. */
#define SIZEOF_LONG_INT 4

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

#ifdef TARGET_CPU_X86
/* Location of system/arch/$ARCH_DIR/sysendian.h */
#define SYSTEM_ARCH_SPECIFIC_ENDIAN_DIR "system/arch/x86/sysendian.h"

/* Location of system/arch/$ARCH_DIR/sysfeatures.h */
#define SYSTEM_ARCH_SPECIFIC_FEATURES_DIR "system/arch/x86/sysfeatures.h"
#elif defined(TARGET_CPU_X64)
/* Location of system/arch/$ARCH_DIR/sysendian.h */
#define SYSTEM_ARCH_SPECIFIC_ENDIAN_DIR "system/arch/generic/sysendian.h"

/* Location of system/arch/$ARCH_DIR/sysfeatures.h */
#define SYSTEM_ARCH_SPECIFIC_FEATURES_DIR "system/arch/generic/sysfeatures.h"
#else
#error "No headers set for this architecture."
#endif

/* Location of system/osapi/$OSAPI_DIR/types.h */
#define SYSTEM_OSAPI_SPECIFIC_TYPES_HDR "system/osapi/posix/types.h"

/* Which signal to use for clock timer */
#define SYSTIMER_SIGNAL SIGALRM

/* Prefer POSIX realtime clock API */
/* #undef USE_POSIX_REALTIME_CLOCK */

/* Use interval timer clock API */
#define USE_POSIX_SETITIMER 1

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined TARGET_BIG_ENDIAN
#  define WORDS_BIGENDIAN 1
#endif

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#define YYTEXT_POINTER 1
