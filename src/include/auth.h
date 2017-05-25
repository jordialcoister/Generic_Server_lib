

/* Function to get authenticated by the linux system */
int sys_auth_user(const char *username, const char *password);

/* Function to get authenticated via PAM */
int pam_auth_user(const char *username, const char *password);

/* Function to get authenticated via public key */
int pkey_auth_user(const char *username, const char *password);

/* Function to get authenticated via ndis */
int ndis_auth_user(const char *username, const char *password);

/* Function to get authenticated via a kerberos server */
int kerberos_auth_user(const char *username, const char *password);




