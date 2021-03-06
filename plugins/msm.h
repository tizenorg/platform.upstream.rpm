/*
 * This file is part of MSM security plugin
 * Greatly based on the code of MSSF security plugin
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Tero Aho <ext-tero.aho@nokia.com>
 *
 * Copyright (C) 2011 - 2013 Intel Corporation.
 *
 * Contact: Elena Reshetova <elena.reshetova@intel.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifndef MSM_H
#define MSM_H

#define SMACK64 "security.SMACK64"
#define SMACK64EXEC "security.SMACK64EXEC"
#define SMACK64TRANSMUTE "security.SMACK64TRANSMUTE"

#define SMACK_RULES_PATH "/etc/smack/accesses.d/"
#define DEVICE_SECURITY_POLICY "/etc/device-sec-policy"

#define SMACK_ISOLATED_LABEL "Isolated"

#define SMACK_LABEL_LENGTH 255
#define SMACK_ACCESS_TYPE_LENGTH 6
#define SMACK_UNINSTALL 1 
#define RANK_LIMIT 10000

#define DBUS_SERVICE   1
#define DBUS_PATH      2
#define DBUS_INTERFACE 3
#define DBUS_METHOD    4
#define DBUS_SIGNAL    5

#include "plugin.h"
#include "debug.h"

#include <uthash.h>
#include <sys/capability.h>
#include <sys/smack.h>
#include <magic.h>
#include "rpmio/rpmio.h"

/** \ingroup msm
 * List definitions.
 * All lists are doubly-linked, the last element is stored to list pointer,
 * which means that lists must be looped using the prev pointer, or by
 * calling LISTHEAD first to go to start in order to use the next pointer.
 */
#define LISTADD(list, node)			\
    do {					\
	(node)->prev = (list);			\
	if (list) (node)->next = (list)->next;	\
	else (node)->next = NULL;		\
	if (list) (list)->next = (node);	\
	(list) = (node);			\
    } while (0);

#define NODEADD(node1, node2)					\
    do {							\
	(node2)->prev = (node1);				\
	(node2)->next = (node1)->next;				\
	if ((node1)->next) (node1)->next->prev = (node2);	\
	(node1)->next = (node2);				\
    } while (0);

#define LISTCAT(list, first, last)		\
    if ((first) && (last)) {			\
	(first)->prev = (list);			\
	(list) = (last);			\
    }

#define LISTDEL(list, node)					\
    do {							\
	if ((node)->prev) (node)->prev->next = (node)->next;	\
	if ((node)->next) (node)->next->prev = (node)->prev;	\
	if (!((node)->prev) && !((node)->next)) (list) = NULL;	\
    } while (0);

#define LISTHEAD(list, node)					\
    for ((node) = (list); (node)->prev; (node) = (node)->prev);	
    
#define LISTTAIL(list, node)					\
    for ((node) = (list); (node)->next; (node) = (node)->next);

/** \ingroup msm
 * Structure definitions.
 * These structures represent the parsed security manifest of a package.
 */

typedef struct file_x {
    const char *path; /* file path */
    ino_t ino; /* file inode */
    struct file_x *prev;
    struct file_x *next;
} file_x;

typedef struct filesystem_x {
    const char *path; /* filesystem object absolute path */
    const char *label; /* SMACK64 xattr */
    const char *exec_label; /* SMACK64EXEC xattr */
    const char *type; /* can be set as TRANSMUTABLE for directory */
    struct filesystem_x *prev;
    struct filesystem_x *next;
 } filesystem_x;

typedef struct ac_domain_x { /* structure for storing ac domain */
    const char *name; /* ac domain name */
    const char *match;
    const char *type; /* ac domain policy type: "shared" or "restricted" or "NULL" (private) */
    const char *plist; /* list of packages that allowed to request domain, if policy is "restricted" */
    const char *pkg_name; /* package that defined ac domain */
    struct ac_domain_x *prev;
    struct ac_domain_x *next;
    struct sw_source_x *sw_source; /* sw source of the package that defined the domain */
    const char *origin;
    UT_hash_handle hh;
    int allowed;
    struct ac_domain_x *older; /* previous version in upgrades */
    struct ac_domain_x *newer; /* the newer upgraded version */
} ac_domain_x;

typedef struct annotation_x {
    const char *name;
    const char *value;
} annotation_x;

typedef struct member_x {
    int type;
    const char *name;
    struct annotation_x *annotation;
    struct member_x *prev;
    struct member_x *next;
} member_x;

typedef struct interface_x {
    const char *name;
    struct annotation_x *annotation;
    struct member_x *members;
    struct interface_x *prev;
    struct interface_x *next;
} interface_x;

typedef struct node_x {
    const char *name;
    struct annotation_x *annotation;
    struct member_x *members;
    struct interface_x *interfaces;
    struct node_x *prev;
    struct node_x *next;
} node_x;

typedef struct dbus_x {
    const char *name;
    const char *own;
    const char *bus;
    struct annotation_x *annotation;
    struct node_x *nodes;
    struct dbus_x *prev;
    struct dbus_x *next;
} dbus_x;

typedef struct provide_x {
    const char *name; /* _system_ or NULL */
    struct ac_domain_x *ac_domains;
    struct filesystem_x *filesystems;
    struct dbus_x *dbuss;
    const char *origin;
    struct provide_x *prev;
    struct provide_x *next;
} provide_x;

typedef struct request_x {
    const char *ac_domain;
} request_x;
 
typedef struct keyinfo_x {
    const unsigned char *keydata;
    size_t keylen;
    struct keyinfo_x *prev;
    struct keyinfo_x *next;
} keyinfo_x;

typedef struct access_x {
    const char *data;
    const char *type;
    struct access_x *prev;
    struct access_x *next;
} access_x;

typedef struct origin_x {
    const char *type;
    struct keyinfo_x *keyinfos;
    struct access_x *accesses;
    struct origin_x *prev;
    struct origin_x *next;
} origin_x;

typedef struct constraint_x {
    const char *name;
    const char *value;
    struct constraint_x *prev;
    struct constraint_x *next;
} constraint_x;

typedef struct d_request_x {
    const char *label_name;
    const char *ac_type;
    struct d_request_x *prev;
    struct d_request_x *next;
} d_request_x;

typedef struct d_permit_x {
    const char *label_name;
    const char *to_label_name;
    const char *ac_type;
    struct d_permit_x *prev;
    struct d_permit_x *next;
} d_permit_x;

typedef struct d_provide_x {
    const char *label_name;    
    struct d_provide_x *prev;
    struct d_provide_x *next;
} d_provide_x;

typedef struct define_x {
    const char *name; /* ac domain name */
    const char *policy; 
    const char *plist; /* list of packages that are allowed to request the ac domain */
    struct d_request_x *d_requests;
    struct d_permit_x *d_permits;
    struct d_provide_x *d_provides;
    struct define_x *prev;
    struct define_x *next;
} define_x;

typedef struct package_x {
    const char *name; /* package name */
    struct sw_source_x *sw_source; /* package sw source */
    struct provide_x *provides;
    const char *modified; /* internal packages */
    struct package_x *prev;
    struct package_x *next;
    UT_hash_handle hh;
    struct package_x *older; /* previous version in upgrades */
    struct package_x *newer; /* the newer upgraded version */
} package_x;

typedef struct sw_source_x {
    const char *name;
    const char *rankkey;
    struct package_x *packages; /* config processing */
    struct ac_domain_x *allowmatches; /* list of allow wildcards */
    struct ac_domain_x *allows; /* hash of allowed ac domains */
    struct ac_domain_x *denymatches; /* list of deny wildcards */
    struct ac_domain_x *denys; /* hash of denied ac domains */
    struct origin_x *origins;
    struct sw_source_x *prev;
    struct sw_source_x *next;
    struct sw_source_x *parent;
    struct sw_source_x *older; /* previous version in upgrades */
    struct sw_source_x *newer; /* the newer upgraded version */
} sw_source_x;

typedef struct manifest_x { /*package manifest */
    struct sw_source_x *sw_source; /* package sw source */
    const char *name; /* package name */
    const char *package_type; /* package type: system or application */
    struct provide_x *provides; /* assign section */
    struct request_x *request; /* request section */
    struct sw_source_x *sw_sources; /*defined software sources(non-NULL only for configuration manifests)*/
    struct define_x *defines; /* define section(s) */
    struct file_x *files; /* installed files */
} manifest_x;

/** \ingroup msm
 * Frees the given pointer and sets it to NULL
 * @param ptr	address of pointer to be freed
 * @return	
 */
void msmFreePointer(void **ptr);

/** \ingroup msm
 * Process package security manifest.
 * @param buffer	xml data buffer
 * @param size		buffer length
 * @param current	sw source for package
 * @param packagename	name of the package
 * @return		pointer to structure on success
 */
manifest_x *msmProcessManifestXml(const char *buffer, int size, sw_source_x *current, const char *packagename);

/** \ingroup msm
 * Loads device security policy.
 * @param rootDir	--root rpm optional prefix
 * @param dsp		pointer to the loaded policy
 * @return		RPMRC_OK or RPMRC_FAIL
 */
rpmRC msmLoadDeviceSecurityPolicy(const char* rootDir, manifest_x **dsp);

/** \ingroup msm
 * Process device security policy file.
 * @param filename	file name
 * @return		pointer to structure on success
 */
manifest_x *msmProcessDevSecPolicyXml(const char *filename);

/** \ingroup msm
 * Creates a directory for the smack rules. 
 * @param rootDir	--root rpm optional prefix
 * @return		RPMRC_OK or RPMRC_FAIL
 */
rpmRC msmSetupSmackRulesDir(const char* rootDir);

/** \ingroup msm
 * Free all structures reserved during manifest processing.
 * @param mfx		pointer to structure
 */
manifest_x* msmFreeManifestXml(manifest_x * mfx);

/** \ingroup msm
 * Go through all sw sources in manifest, import keys to RPM keyring.
 * @param smack_accesses	smack_accesses handle for setting smack rules
 * @param mfx			package manifest
 * @param ts			rpm transaction set
 * @return			0 on success, else -1
 */
int msmSetupSWSources(struct smack_accesses *smack_accesses, manifest_x *mfx, rpmts ts);

/** \ingroup msm
 * Create package structure for package being installed.
 * @param name		package name
 * @param sw_source	package sw source
 * @param provides	provided ac domains
 * @param modified	for internal packages
 * @return		allocated and initialized package struct
 */
package_x *msmCreatePackage(const char *name, sw_source_x *sw_source, provide_x *provides, const char *modified);

/** \ingroup msm
 * Go through all provides in manifest, add provided ac domains to hash.
 * @param packages	pointer to packages list
 * @param sw_source	link to sw source in device security policy
 * @param rule_set	rule set for setting smack rules
 * @return		0 on success, else -1
 */
int msmSetupPackages(struct smack_accesses *smack_accesses, package_x *packages, sw_source_x *sw_source);

/** \ingroup msm
 * Setup define section of manifest
 * @param smack_accesses	smack_accesses handle for setting smack rules
 * @param mfx			package manifest
 * @return			0 on success, else -1
 */
int msmSetupDefines(struct smack_accesses *smack_accesses, manifest_x *mfx);

/** \ingroup msm
 * Setup smack rules according to the manifest
 * @param smack_accesses	smack_accesses handle for setting smack rules
 * @param package_name		package name
 * @param flag			    flag to indicate installation or uninstallation  
 * @param SmackEnabled      flag to indicate Smack presence in the kernel
 * @return			        0 on success, else -1
 */
int msmSetupSmackRules(struct smack_accesses *smack_accesses, const char* package_name, int flag, int SmackEnabled);

/** \ingroup msm
 * Check previous installation of package. 
 * @param name		package name
 * @return		package or NULL
 */
package_x *msmCheckPackage(const char *name);

/** \ingroup msm
 * Cancel the installation of package (rules and config data). 
 * @param name		package name
 */
void msmCancelPackage(const char *name);

/** \ingroup msm
 * Free package structure. 
 * @param package	package
 * @return		next package in list or NULL
 */
package_x *msmFreePackage(package_x *package);

/** \ingroup msm
 * Set extended attributes of the file based on manifest.
 * @param mfx		package manifest
 * @param filepath	path of the file
 * @param cookie	magic cookie
 * @return	0 on success, else -1
 */
int msmSetFileXAttributes(manifest_x *mfx, const char* filepath, magic_t cookie);

/** \ingroup msm
 * Set setup the request section of manifest.
 * @param mfx			package manifest
 * @return			0 on success, else -1
 */
int msmSetupRequests(manifest_x *mfx);

/** \ingroup msm
 * Package is removed, remove all related Smack rules.
 * @param mfx			package manifest
 * @param smack_accesses	smack_accesses handle for setting smack rules
 * @param SmackEnabled      flag to indicate Smack presence in the kernel
 */
void msmRemoveRules(struct smack_accesses *smack_accesses, manifest_x *mfx, int SmackEnabled);

/** \ingroup msm
 * Setup DBus policies for package
 * @param package	package
 * @param mfx		package manifest 
 */
int msmSetupDBusPolicies(package_x *package, manifest_x *mfx);

/** \ingroup msm
 * Package is removed, remove related data in device security policy.
 * @param mfx		package manifest
 */
void msmRemoveConfig(manifest_x *mfx);

/** \ingroup msm
 * String compare which allows wildcards (* and ?) in s2.
 * @param s1		string to compare
 * @param s2		string to compare
 * @return		0 if s1 matches s2
 */
int strwcmp(const char *s1, const char *s2);

/** \ingroup msm
 * Saves configuration into /etc/dev-sec-policy.
 * @param mfx		data to serialize
 * @param rooDir	ts->rootDir prefix
 * @return		RPMRC_OK or RPMRC_FAIL
 */
rpmRC msmSaveDeviceSecPolicyXml(manifest_x *root, const char *rootDir);

/** \ingroup msm
 * Depth first tree traversal for sw source tree.
 * @param sw_sources	sw source tree
 * @param func		function to call for each sw source until 0 is returned
 * @param param		parameter for the function
 * @param param2	second parameter for the function
 * @return		matching sw source or NULL
 */
sw_source_x *msmSWSourceTreeTraversal(sw_source_x *sw_sources, int (func)(sw_source_x *, void *, void *), void *param, void* param2);

/** \ingroup msm
 * Free internal hashes.
 */
void msmFreeInternalHashes(void);

/** \ingroup msm
 * Finds a sw source by key info.
 * @param sw_source	sw source
 * @param param		searched param
 * @return		0 if found
 */
int msmFindSWSourceByKey(sw_source_x *sw_source, void *param);

/** \ingroup msm
 * Finds a sw source by sw source name.
 * @param sw_source	sw source
 * @param param		searched param
 * @return		0 if found
 */
int msmFindSWSourceByName(sw_source_x *sw_source, void *param);

/** \ingroup msm
 * Finds a sw source by sw source signature.
 * @param sw_source	sw source
 * @param param		searched param
 * @param param2	searched param2
 * @return		0 if found
 */
int msmFindSWSourceBySignature(sw_source_x *sw_source, void *param, void* param2);

#endif
