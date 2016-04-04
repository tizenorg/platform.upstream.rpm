#ifndef H_RPMTE
#define H_RPMTE

/** \ingroup rpmts rpmte
 * \file lib/rpmte.h
 * Structures used for an "rpmte" transaction element.
 */

#include <rpm/rpmtypes.h>
#include <rpm/argv.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \ingroup rpmte
 * Transaction element type.
 */
typedef enum rpmElementType_e {
    TR_ADDED		= (1 << 0),	/*!< Package will be installed. */
    TR_REMOVED		= (1 << 1)	/*!< Package will be removed. */
} rpmElementType;

typedef rpmFlags rpmElementTypes;

/** \ingroup rpmte
 * Retrieve header from transaction element.
 * @param te		transaction element
 * @return		header (new reference)
 */
__attribute__ ((visibility ("default"))) Header rpmteHeader(rpmte te);

/** \ingroup rpmte
 * Save header into transaction element.
 * @param te		transaction element
 * @param h		header
 * @return		NULL always
 */
__attribute__ ((visibility ("default"))) Header rpmteSetHeader(rpmte te, Header h);

/** \ingroup rpmte
 * Retrieve type of transaction element.
 * @param te		transaction element
 * @return		type
 */
__attribute__ ((visibility ("default"))) rpmElementType rpmteType(rpmte te);

/** \ingroup rpmte
 * Retrieve name string of transaction element.
 * @param te		transaction element
 * @return		name string
 */
__attribute__ ((visibility ("default"))) const char * rpmteN(rpmte te);

/** \ingroup rpmte
 * Retrieve epoch string of transaction element.
 * @param te		transaction element
 * @return		epoch string
 */
__attribute__ ((visibility ("default"))) const char * rpmteE(rpmte te);

/** \ingroup rpmte
 * Retrieve version string of transaction element.
 * @param te		transaction element
 * @return		version string
 */
__attribute__ ((visibility ("default"))) const char * rpmteV(rpmte te);

/** \ingroup rpmte
 * Retrieve release string of transaction element.
 * @param te		transaction element
 * @return		release string
 */
__attribute__ ((visibility ("default"))) const char * rpmteR(rpmte te);

/** \ingroup rpmte
 * Retrieve arch string of transaction element.
 * @param te		transaction element
 * @return		arch string
 */
__attribute__ ((visibility ("default"))) const char * rpmteA(rpmte te);

/** \ingroup rpmte
 * Retrieve os string of transaction element.
 * @param te		transaction element
 * @return		os string
 */
__attribute__ ((visibility ("default"))) const char * rpmteO(rpmte te);

/** \ingroup rpmte
 * Retrieve isSource attribute of transaction element.
 * @param te		transaction element
 * @return		isSource attribute
 */
__attribute__ ((visibility ("default"))) int rpmteIsSource(rpmte te);

/** \ingroup rpmte
 * Retrieve color bits of transaction element.
 * @param te		transaction element
 * @return		color bits
 */
__attribute__ ((visibility ("default"))) rpm_color_t rpmteColor(rpmte te);

/** \ingroup rpmte
 * Set color bits of transaction element.
 * @param te		transaction element
 * @param color		new color bits
 * @return		previous color bits
 */
__attribute__ ((visibility ("default"))) rpm_color_t rpmteSetColor(rpmte te, rpm_color_t color);

/** \ingroup rpmte
 * Retrieve last instance installed to the database.
 * @param te		transaction element
 * @return		last install instance.
 */
__attribute__ ((visibility ("default"))) unsigned int rpmteDBInstance(rpmte te);

/** \ingroup rpmte
 * Set last instance installed to the database.
 * @param te		transaction element
 * @param instance	Database instance of last install element.
 * @return		last install instance.
 */
__attribute__ ((visibility ("default"))) void rpmteSetDBInstance(rpmte te, unsigned int instance);

/** \ingroup rpmte
 * Retrieve size in bytes of package file.
 * @todo Signature header is estimated at 256b.
 * @param te		transaction element
 * @return		size in bytes of package file.
 */
__attribute__ ((visibility ("default"))) rpm_loff_t rpmtePkgFileSize(rpmte te);

/** \ingroup rpmte
 * Retrieve parent transaction element.
 * @param te		transaction element
 * @return		parent transaction element
 */
__attribute__ ((visibility ("default"))) rpmte rpmteParent(rpmte te);

/** \ingroup rpmte
 * Set parent transaction element.
 * @param te		transaction element
 * @param pte		new parent transaction element
 * @return		previous parent transaction element
 */
__attribute__ ((visibility ("default"))) rpmte rpmteSetParent(rpmte te, rpmte pte);

/** \ingroup rpmte
 * Return problem set info of transaction element.
 * @param te		transaction element
 * @return		problem set (or NULL if none)
 */
__attribute__ ((visibility ("default"))) rpmps rpmteProblems(rpmte te);

/** \ingroup rpmte
 * Destroy problem set info of transaction element.
 * @param te		transaction element
 */
__attribute__ ((visibility ("default"))) void rpmteCleanProblems(rpmte te);

/** \ingroup rpmte
 * Destroy dependency set info of transaction element.
 * @param te		transaction element
 */
__attribute__ ((visibility ("default"))) void rpmteCleanDS(rpmte te);

/** \ingroup rpmte
 * Set dependent element of TR_REMOVED transaction element.
 * @param te		transaction element
 * @param depends       dependent transaction element
 */
__attribute__ ((visibility ("default"))) void rpmteSetDependsOn(rpmte te, rpmte depends);

/** \ingroup rpmte
 * Retrieve dependent element of TR_REMOVED transaction element.
 * @param te		transaction element
 * @return		dependent transaction element
 */
__attribute__ ((visibility ("default"))) rpmte rpmteDependsOn(rpmte te);

/** \ingroup rpmte
 * Retrieve rpmdb instance of TR_REMOVED transaction element.
 * @param te		transaction element
 * @return		rpmdb instance
 */
__attribute__ ((visibility ("default"))) int rpmteDBOffset(rpmte te);

/** \ingroup rpmte
 * Retrieve [epoch:]version-release string from transaction element.
 * @param te		transaction element
 * @return		[epoch:]version-release string
 */
__attribute__ ((visibility ("default"))) const char * rpmteEVR(rpmte te);

/** \ingroup rpmte
 * Retrieve name-[epoch:]version-release string from transaction element.
 * @param te		transaction element
 * @return		name-[epoch:]version-release string
 */
__attribute__ ((visibility ("default"))) const char * rpmteNEVR(rpmte te);

/** \ingroup rpmte
 * Retrieve name-[epoch:]version-release.arch string from transaction element.
 * @param te		transaction element
 * @return		name-[epoch:]version-release.arch string
 */
__attribute__ ((visibility ("default"))) const char * rpmteNEVRA(rpmte te);

/** \ingroup rpmte
 * Retrieve key from transaction element.
 * @param te		transaction element
 * @return		key
 */
__attribute__ ((visibility ("default"))) fnpyKey rpmteKey(rpmte te);

/** \ingroup rpmte
 * Return failure status of transaction element.
 * If the element itself failed, this is 1, larger count means one of
 * it's parents failed.
 * @param te		transaction element
 * @return		number of failures for this transaction element
 */
__attribute__ ((visibility ("default"))) int rpmteFailed(rpmte te);

/** \ingroup rpmte
 * Retrieve dependency tag set from transaction element.
 * @param te		transaction element
 * @param tag		dependency tag
 * @return		dependency tag set
 */
__attribute__ ((visibility ("default"))) rpmds rpmteDS(rpmte te, rpmTagVal tag);

/** \ingroup rpmte
 * Retrieve file info tag set from transaction element.
 * @param te		transaction element
 * @return		file info tag set
 */
__attribute__ ((visibility ("default"))) rpmfi rpmteFI(rpmte te);

/** \ingroup rpmte
 * Retrieve list of collections
 * @param te		transaction element
 * @return		list of collections
 */
__attribute__ ((visibility ("default"))) ARGV_const_t rpmteCollections(rpmte te);

/** \ingroup rpmte
 * Determine a transaction element is part of a collection
 * @param te		transaction element
 * @param collname	collection name
 * @return		1 if collname is part of a collection, 0 if not
 */
__attribute__ ((visibility ("default"))) int rpmteHasCollection(rpmte te, const char * collname);


#ifdef __cplusplus
}
#endif

#endif	/* H_RPMTE */
