#ifndef H_RPMLIB
#define	H_RPMLIB

/** \ingroup rpmcli rpmrc rpmdep rpmtrans rpmdb lead signature header payload dbi
 * \file lib/rpmlib.h
 *
 * In Memoriam: Steve Taylor <staylor@redhat.com> was here, now he's not.
 *
 */

#include <popt.h>

#include <rpm/rpmio.h>
#include <rpm/header.h>
#include <rpm/rpmtag.h>
#include <rpm/rpmds.h>	/* XXX move rpmlib provides to rpmds instead */
#include <rpm/rpmpgp.h>
#ifdef _RPM_4_4_COMPAT
#include <rpm/rpmlegacy.h> /* legacy compat definitions if enabled */
#endif

#ifdef __cplusplus
extern "C" {
#endif

__attribute__ ((visibility ("default"))) extern struct rpmMacroContext_s * rpmGlobalMacroContext;

__attribute__ ((visibility ("default"))) extern struct rpmMacroContext_s * rpmCLIMacroContext;

__attribute__ ((visibility ("default"))) extern const char * const RPMVERSION;

__attribute__ ((visibility ("default"))) extern const char * const rpmNAME;

__attribute__ ((visibility ("default"))) extern const char * const rpmEVR;

__attribute__ ((visibility ("default"))) extern const int rpmFLAGS;

/* ==================================================================== */
/** \name RPMRC */

/** \ingroup rpmrc
 * Build and install arch/os table identifiers.
 * @todo Eliminate from API.
 */
enum rpm_machtable_e {
    RPM_MACHTABLE_INSTARCH	= 0,	/*!< Install platform architecture. */
    RPM_MACHTABLE_INSTOS	= 1,	/*!< Install platform operating system. */
    RPM_MACHTABLE_BUILDARCH	= 2,	/*!< Build platform architecture. */
    RPM_MACHTABLE_BUILDOS	= 3	/*!< Build platform operating system. */
};
#define	RPM_MACHTABLE_COUNT	4	/*!< No. of arch/os tables. */

/** \ingroup rpmrc
 * Read macro configuration file(s) for a target.
 * @param file		colon separated files to read (NULL uses default)
 * @param target	target platform (NULL uses default)
 * @return		0 on success, -1 on error
 */
__attribute__ ((visibility ("default"))) int rpmReadConfigFiles(const char * file,
		const char * target);

/** \ingroup rpmrc
 * Return current arch name and/or number.
 * @todo Generalize to extract arch component from target_platform macro.
 * @retval name		address of arch name (or NULL)
 * @retval num		address of arch number (or NULL)
 */
__attribute__ ((visibility ("default"))) void rpmGetArchInfo( const char ** name,
		int * num);

/** \ingroup rpmrc
 * Return color for an arch
 * @param arch		name of an architecture
 * @return color        color of arch, -1 if the arch couldn't be determined
 */
__attribute__ ((visibility ("default"))) int rpmGetArchColor(const char *arch);

/** \ingroup rpmrc
 * Return current os name and/or number.
 * @todo Generalize to extract os component from target_platform macro.
 * @retval name		address of os name (or NULL)
 * @retval num		address of os number (or NULL)
 */
__attribute__ ((visibility ("default"))) void rpmGetOsInfo( const char ** name,
		int * num);

/** \ingroup rpmrc
 * Return arch/os score of a name.
 * An arch/os score measures the "nearness" of a name to the currently
 * running (or defined) platform arch/os. For example, the score of arch
 * "i586" on an i686 platform is (usually) 2. The arch/os score is used
 * to select one of several otherwise identical packages using the arch/os
 * tags from the header as hints of the intended platform for the package.
 * @todo Rewrite to use RE's against config.guess target platform output.
 *
 * @param type		any of the RPM_MACHTABLE_* constants
 * @param name		name
 * @return		arch score (0 is no match, lower is preferred)
 */
__attribute__ ((visibility ("default"))) int rpmMachineScore(int type, const char * name);

/** \ingroup rpmrc
 * Display current rpmrc (and macro) configuration.
 * @param fp		output file handle
 * @return		0 always
 */
__attribute__ ((visibility ("default"))) int rpmShowRC(FILE * fp);

/** \ingroup rpmrc
 * Destroy rpmrc arch/os compatibility tables.
 * @todo Eliminate from API.
 */
__attribute__ ((visibility ("default"))) void rpmFreeRpmrc(void);

/**
 * Compare headers to determine which header is "newer".
 * @param first		1st header
 * @param second	2nd header
 * @return		result of comparison
 */
__attribute__ ((visibility ("default"))) int rpmVersionCompare(Header first, Header second);

/**  \ingroup header
 * Check header consistency, performing headerGetEntry() the hard way.
 *  
 * Sanity checks on the header are performed while looking for a
 * header-only digest or signature to verify the blob. If found,
 * the digest or signature is verified.
 *
 * @param ts		transaction set
 * @param uh		unloaded header blob
 * @param uc		no. of bytes in blob (or 0 to disable)
 * @retval *msg		verification error message (or NULL)
 * @return		RPMRC_OK on success
 */
__attribute__ ((visibility ("default"))) rpmRC headerCheck(rpmts ts, const void * uh, size_t uc, char ** msg);

/**  \ingroup header
 * Return checked and loaded header.
 * @param ts		transaction set
 * @param fd		file handle
 * @retval hdrp		address of header (or NULL)
 * @retval *msg		verification error message (or NULL)
 * @return		RPMRC_OK on success
 */
__attribute__ ((visibility ("default"))) rpmRC rpmReadHeader(rpmts ts, FD_t fd, Header *hdrp, char ** msg);

/** \ingroup header
 * Return package header from file handle, verifying digests/signatures.
 * @param ts		transaction set
 * @param fd		file handle
 * @param fn		file name
 * @retval hdrp		address of header (or NULL)
 * @return		RPMRC_OK on success
 */
__attribute__ ((visibility ("default"))) rpmRC rpmReadPackageFile(rpmts ts, FD_t fd,
		const char * fn, Header * hdrp);

/** \ingroup rpmtrans
 * Install source package.
 * @param ts		transaction set
 * @param fd		file handle
 * @retval specFilePtr	address of spec file name (or NULL)
 * @retval cookie	address of cookie pointer (or NULL)
 * @return		rpmRC return code
 */
__attribute__ ((visibility ("default"))) rpmRC rpmInstallSourcePackage(rpmts ts, FD_t fd,
			char ** specFilePtr,
			char ** cookie);

/** \ingroup rpmtrans
 * Segmented string compare for version or release strings.
 *
 * @param a		1st string
 * @param b		2nd string
 * @return		+1 if a is "newer", 0 if equal, -1 if b is "newer"
 */
__attribute__ ((visibility ("default"))) int rpmvercmp(const char * a, const char * b);

#ifdef __cplusplus
}
#endif

#endif	/* H_RPMLIB */
