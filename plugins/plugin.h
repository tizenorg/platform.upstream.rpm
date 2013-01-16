#include "system.h"

#include <rpm/rpmlib.h>
#include <rpm/rpmlog.h>
#include <rpm/rpmts.h>

#include "lib/rpmplugins.h"
#include "lib/rpmchroot.h"

/* general plugin hooks */
rpmRC PLUGINHOOK_INIT_FUNC(rpmts ts, const char * name, const char * opts);
rpmRC PLUGINHOOK_CLEANUP_FUNC(void);

/* collection plugin hooks */
rpmRC PLUGINHOOK_OPENTE_FUNC(rpmte te);
rpmRC PLUGINHOOK_COLL_POST_ANY_FUNC(void);
rpmRC PLUGINHOOK_COLL_POST_ADD_FUNC(void);
rpmRC PLUGINHOOK_COLL_PRE_REMOVE_FUNC(void);

/* per transaction plugin hooks */
rpmRC PLUGINHOOK_TSM_PRE_FUNC(rpmts ts);
rpmRC PLUGINHOOK_TSM_POST_FUNC(rpmts ts, int res);

/* per transaction element plugin hooks */
rpmRC PLUGINHOOK_PSM_PRE_FUNC(rpmte te);
rpmRC PLUGINHOOK_PSM_POST_FUNC(rpmte te, int res);
rpmRC PLUGINHOOK_VERIFY_FUNC(rpmKeyring keyring, rpmtd sigtd, pgpDigParams sig, DIGEST_CTX ctx, int res);

/*per scriptlet plugin hooks */
rpmRC PLUGINHOOK_SCRIPTLET_PRE_FUNC(const char *s_name, int type);
rpmRC PLUGINHOOK_SCRIPTLET_FORK_POST_FUNC(const char *path, int type);
rpmRC PLUGINHOOK_SCRIPTLET_POST_FUNC(const char *s_name, int type, int res);

/*per file plugin hooks */
rpmRC PLUGINHOOK_FSM_INIT_FUNC(const char* path, mode_t mode);
rpmRC PLUGINHOOK_FSM_COMMIT_FUNC(const char* path, mode_t mode, int type);
rpmRC PLUGINHOOK_FILE_CONFLICT_FUNC(rpmts ts, char* path, Header oldHeader, rpmfi oldFi, int rpmrc);
