#ifndef CRACKLIB_H
#define CRACKLIB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Pass this function a password (pw) and a path to the
 * dictionaries (/usr/lib/cracklib_dict should be specified)
 * and it will either return a NULL string, meaning that the
 * password is good, or a pointer to a string that explains the
 * problem with the password.
 * You must link with -lcrack
 */

extern const char *FascistCheck(const char *pw, const char *dictpath);

/* This function returns the compiled in value for DEFAULT_CRACKLIB_DICT.
 */
extern const char *GetDefaultCracklibDict(void);

#ifdef __cplusplus
};
#endif

#endif
