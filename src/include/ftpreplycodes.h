/* This header file have all the reple codes for ftp defined on the
 * 959 rfc for use it in a */

#define OK "200 Command okay."
#define BADCOM "500 Syntax error, command unrecognized."
#define BADARG "501 Syntax error in parameters or arguments."
#define NOTIMPSITE "202 Command not implemented, superfluous at this site."
#define NOTIMP "502 Command not implemented."
#define BADSEQ "503 Bad sequence of commands."
#define NOTIMPPAR "504 Command not implemented for that parameter."
#define RESTMARK "110 Restart marker reply."
#define SYSSTAT "211 System status, or system help reply."
#define DIRSTAT "212 Directory status."
#define FILESTAT "213 File status."
#define HELPMSG "214 Help message."
#define SYSTYPE "215 NAME system type."
#define READYN "120 Service ready in nnn minutes."
#define READYNU "220 Service ready for new user."
#define CLOSING "221 Service closing control connection."
#define SNOTAV "421 Service not available, closing control connection."
#define STARTTRANS "125 Data connection already open; transfer starting."
#define OPENDCN "225 Data connection open; no transfer in progress."
#define EOPENDC "425 Can't open data connection."
#define CLOSEDC "226 Closing data connection."
#define CONCLOSED "426 Connection closed; transfer aborted."
#define PASSIVEMODE "227 Entering Passive Mode (h1,h2,h3,h4,p1,p2)."
#define LOGINOK "230 User logged in, proceed."
#define ENOTLOGIN "530 Not logged in."
#define USEROK "331 User name okay, need password."
#define ELOGACC "332 Need account for login."
#define ESTORACC "532 Need account for storing files."
#define OKFILESTAT "150 File status okay; about to open data connection."
#define OKFILEACT "250 Requested file action okay, completed."
#define MKDIROK "257 \"PATHNAME\" created."
#define EFILEXTRAINFO "350 Requested file action pending further information."
#define EFILEBUSY "450 Requested file action not taken. File exists but is unavailable, maybe it's busy"
#define ENOFILE "550 Requested action not taken. File unavailable, not found or not access."
#define ELOCAL "451 Requested action aborted. Local error in processing."
#define EPAGETYPEUNK "551 Requested action aborted. Page type unknown."
#define ESTORAGE "452 Requested action not taken. Insufficient storage space in system."