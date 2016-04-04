#ifndef H_RPMPS
#define H_RPMPS

/** \ingroup rpmps
 * \file lib/rpmps.h
 * Structures and prototypes used for an "rpmps" problem set.
 */

#include <stdio.h>
#include <rpm/rpmtypes.h>
#include <rpm/rpmprob.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \ingroup rpmps
 * Problem set iterator
 */
typedef struct rpmpsi_s * rpmpsi;

/** \ingroup rpmps
 * Reference a problem set instance.
 * @param ps		transaction set
 * @return		new transaction set reference
 */
__attribute__ ((visibility ("default"))) rpmps rpmpsLink (rpmps ps);

/** \ingroup rpmps
 * Return number of problems in set.
 * @param ps		problem set
 * @return		number of problems
 */
__attribute__ ((visibility ("default"))) int rpmpsNumProblems(rpmps ps);

/** \ingroup rpmps
 * Initialize problem set iterator.
 * @param ps		problem set
 * @return		problem set iterator
 */
__attribute__ ((visibility ("default"))) rpmpsi rpmpsInitIterator(rpmps ps);

/** \ingroup rpmps
 * Destroy problem set iterator.
 * @param psi		problem set iterator
 * @return		problem set iterator (NULL)
 */
__attribute__ ((visibility ("default"))) rpmpsi rpmpsFreeIterator(rpmpsi psi);

/** \ingroup rpmps
 * Return next problem from iterator
 * @param psi		problem set iterator
 * @return		next problem (weak ref), NULL on termination
 */
__attribute__ ((visibility ("default"))) rpmProblem rpmpsiNext(rpmpsi psi);

/** \ingroup rpmps
 * Return next problem set iterator index
 * @param psi		problem set iterator
 * @return		iterator index, -1 on termination
 */
__attribute__ ((visibility ("default"))) int rpmpsNextIterator(rpmpsi psi);

/** \ingroup rpmps
 * Return current problem from problem set
 * @param psi		problem set iterator
 * @return		current rpmProblem 
 */
__attribute__ ((visibility ("default"))) rpmProblem rpmpsGetProblem(rpmpsi psi);

/** \ingroup rpmps
 * Create a problem set.
 * @return		new problem set
 */
__attribute__ ((visibility ("default"))) rpmps rpmpsCreate(void);

/** \ingroup rpmps
 * Destroy a problem set.
 * @param ps		problem set
 * @return		NULL always
 */
__attribute__ ((visibility ("default"))) rpmps rpmpsFree(rpmps ps);

/** \ingroup rpmps
 * Print problems to file handle.
 * @param fp		file handle (NULL uses stderr)
 * @param ps		problem set
 */
__attribute__ ((visibility ("default"))) void rpmpsPrint(FILE *fp, rpmps ps);

/** \ingroup rpmps
 * Append a problem to current set of problems.
 * @param ps		problem set
 * @param prob		rpmProblem 
 */
__attribute__ ((visibility ("default"))) void rpmpsAppendProblem(rpmps ps, rpmProblem prob);

/** \ingroup rpmps
 * Merge problem set into another.
 * @param dest		destination problem set
 * @param src		source problem set
 * @return		number of problems merged
 */
__attribute__ ((visibility ("default"))) int rpmpsMerge(rpmps dest, rpmps src);

#ifdef __cplusplus
}
#endif

#endif	/* H_RPMPS */
