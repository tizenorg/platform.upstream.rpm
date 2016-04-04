#ifndef _H_SPEC_
#define _H_SPEC_

/** \ingroup rpmbuild
 * \file build/rpmspec.h
 *  The rpmSpec and Package data structures used during build.
 */

#include <rpm/rpmstring.h>	/* StringBuf */
#include <rpm/rpmcli.h>	/* for QVA_t */

#ifdef __cplusplus
extern "C" {
#endif

/** \ingroup rpmbuild
 */
typedef struct Package_s * rpmSpecPkg;
typedef struct Source * rpmSpecSrc;
typedef struct rpmSpecIter_s * rpmSpecPkgIter;
typedef struct rpmSpecIter_s * rpmSpecSrcIter;

enum rpmSourceFlags_e {
    RPMBUILD_ISSOURCE	= (1 << 0),
    RPMBUILD_ISPATCH	= (1 << 1),
    RPMBUILD_ISICON	= (1 << 2),
    RPMBUILD_ISNO	= (1 << 3),
};

typedef rpmFlags rpmSourceFlags;

#define RPMBUILD_DEFAULT_LANG "C"

enum rpmSpecFlags_e {
    RPMSPEC_NONE	= 0,
    RPMSPEC_ANYARCH	= (1 << 0),
    RPMSPEC_FORCE	= (1 << 1),
    RPMSPEC_NOLANG	= (1 << 2),
};

typedef rpmFlags rpmSpecFlags;

/** \ingroup rpmbuild
 * Destroy Spec structure.
 * @param spec		spec file control structure
 * @return		NULL always
 */
__attribute__ ((visibility ("default"))) rpmSpec rpmSpecFree(rpmSpec spec);

/* Iterator for spec packages */
__attribute__ ((visibility ("default"))) rpmSpecPkgIter rpmSpecPkgIterInit(rpmSpec spec);
__attribute__ ((visibility ("default"))) rpmSpecPkg rpmSpecPkgIterNext(rpmSpecPkgIter iter);
__attribute__ ((visibility ("default"))) rpmSpecPkgIter rpmSpecPkgIterFree(rpmSpecPkgIter iter);

/* Getters for spec package attributes */
__attribute__ ((visibility ("default"))) Header rpmSpecPkgHeader(rpmSpecPkg pkg);

/* Iterator for spec sources */
__attribute__ ((visibility ("default"))) rpmSpecSrcIter rpmSpecSrcIterInit(rpmSpec spec);
__attribute__ ((visibility ("default"))) rpmSpecSrc rpmSpecSrcIterNext(rpmSpecSrcIter iter);
__attribute__ ((visibility ("default"))) rpmSpecSrcIter rpmSpecSrcIterFree(rpmSpecSrcIter iter);

/* Getters for spec source attributes */
__attribute__ ((visibility ("default"))) rpmSourceFlags rpmSpecSrcFlags(rpmSpecSrc src);
__attribute__ ((visibility ("default"))) int rpmSpecSrcNum(rpmSpecSrc src);
__attribute__ ((visibility ("default"))) const char * rpmSpecSrcFilename(rpmSpecSrc src, int full);

/*
 * Retrieve parsed spec script section (RPMBUILD_PREP, RPMBUILD_BUILD etc).
 * As a special case, RPMBUILD_NONE as section returns the entire spec in
 * preprocessed (macros expanded etc) format.
 */
__attribute__ ((visibility ("default"))) const char * rpmSpecGetSection(rpmSpec spec, int section);

/** \ingroup rpmbuild
 * Function to query spec file(s).
 * @param ts		transaction set
 * @param qva		parsed query/verify options
 * @param arg		query argument
 * @return		0 on success, else no. of failures
 */
__attribute__ ((visibility ("default"))) int rpmspecQuery(rpmts ts, QVA_t qva, const char * arg);

#ifdef __cplusplus
}
#endif

#endif /* _H_SPEC_ */
