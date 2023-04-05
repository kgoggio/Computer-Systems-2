/*-------------------------------------------------------------------------*
 *---									---*
 *---		hashApp.h						---*
 *---									---*
 *---	    This file declares constants common to both the client and	---*
 *---	server of an application where the client asks the user for a	---*
 *---	filename, which it sends to the server.  The server computes	---*
 *---	SHA-512 as a hexadecimal string and sends it back to the	---*
 *---	client.								---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0					Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/


/*---		Header file inclusion					---*/

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>	// read(), write()
#include	<sys/socket.h>	// For socket()
#include	<netinet/in.h>	// For sockaddr_in and htons()
#include	<netdb.h>	// For getaddrinfo()
#include	<errno.h>	// For errno var
#include	<sys/stat.h>	// For open(), read(),write(), stat()
#include	<fcntl.h>	// and close()
#include	<wait.h>	// For wait()


/*---		Definition of constants:				---*/

const int	BUFFER_LEN		= 1024;
#define		DEFAULT_HOSTNAME	"localhost"
#define		HASH_APP_PATH		"/usr/bin/openssl"