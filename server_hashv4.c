/*-------------------------------------------------------------------------*
 *---									---*
 *---		server_hash.c						---*
 *---									---*
 *---	    This file defines a C program that waits for a client to	---*
 *---	connect, gets a filelength and filename from the client, runs	---*
 *---	a child process to compute the SHA-512 hash of that file,	---*
 *---	and sends it back to the client.				---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a					Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//	Compile with:
//	$ gcc server_hash.c -o server_hash -lpthread -g

/*---		Header file inclusion					---*/

#include	"hashApp.h"
#include	<pthread.h>

const int	LO_LEGAL_PORT	= 1025;
const int	HI_LEGAL_PORT	= 65535;
const int	ERROR_FD	= -1;
const int	NUM_CLIENTS_TO_SERVE
				= 4;

//  PURPOSE:  To attempt to create and return a file-descriptor for listening
//	to the OS telling this server when a client process has connect()-ed
//	to 'port'.  Returns that file-descriptor, or 'ERROR_FD' on failure.
int		getServerFileDescriptor
				(int		port,
				 const char*	progName
				)
{
  //  I.  Application validity check:
  if  (progName == NULL)
  {
    fprintf(stderr,"BUG: NULL ptr to getServerFileDescriptor().\n");
    exit(EXIT_FAILURE);
  }

  //  II.  Attempt to get socket file descriptor and bind it to 'port':
  //  II.A.  Create a socket
  int socketDescriptor = socket(AF_INET, // AF_INET domain
			        SOCK_STREAM, // Reliable TCP
			        0);

  if  (socketDescriptor < 0)
  {
    perror(progName);
    return(ERROR_FD);
  }

  //  II.B.  Attempt to bind 'socketDescriptor' to 'port':
  //  II.B.1.  We'll fill in this datastruct
  struct sockaddr_in socketInfo;

  //  II.B.2.  Fill socketInfo with 0's
  memset(&socketInfo,'\0',sizeof(socketInfo));

  //  II.B.3.  Use TCP/IP:
  socketInfo.sin_family = AF_INET;

  //  II.B.4.  Tell port in network endian with htons()
  socketInfo.sin_port = htons(port);

  //  II.B.5.  Allow machine to connect to this service
  socketInfo.sin_addr.s_addr = INADDR_ANY;

  //  II.B.6.  Try to bind socket with port and other specifications
  int status = bind(socketDescriptor, // from socket()
		    (struct sockaddr*)&socketInfo,
		    sizeof(socketInfo)
		   );

  if  (status < 0)
  {
    perror(progName);
    return(ERROR_FD);
  }

  //  II.B.6.  Set OS queue length:
  listen(socketDescriptor,5);

  //  III.  Finished:
  return(socketDescriptor);
}


//  PURPOSE:  To ask the user which port to attempt to monopolize, and to return
//	entered port number.
int		getPort		()
{
  //  I.  Application validity check:

  //  II.  Get port number
  int	port;

  do
  {
    char	buffer[BUFFER_LEN];

    printf("Please enter port number to monopolize [%d-%d]: ",
	   LO_LEGAL_PORT,HI_LEGAL_PORT
	  );
    fgets(buffer,BUFFER_LEN,stdin);
    port = strtol(buffer,NULL,10);
  }
  while  ( (port < LO_LEGAL_PORT)  ||  (port > HI_LEGAL_PORT) );

  //  III.  Finished:
  return(port);
}



//  PURPOSE:  To make a process that will compute the SHA-512 hash of the
//  	file named 'filePathCPtr', and send that value back to the client
//  	via file-descriptor 'toClient'.  No return value.
void	computeAndSendHash	(int		toClient,
				 const char*	filePathCPtr
				)
{
  //  I.  Application validity check:

  //  II.  Compute and send hash:
  //  C. HERE
  int recieved = open(filePathCPtr,O_RDONLY,0); //works

  if(recieved <0)
      write(toClient, "Cannot open file\n",17);
  
  int cpid = fork();
  
  //read(recieved, (void*)filePathCPtr, sizeof(filePathCPtr));
  read(recieved,stdin,sizeof(stdin));//stdin isn't what we can use
  
  //write(toClient, stdout, sizeof(stdout));
  write(toClient, filePathCPtr[0], sizeof(stdout));


  execl(HASH_APP_PATH,HASH_APP_PATH, "sha512", NULL); //right

  //  III.  Finished:
  wait(NULL);
}


//  PURPOSE:  To do the work of handling the client.  Communication with the
//	client take place using file-descriptor obtained from '(int*)vPtr'.
//	Returns 'NULL'.
void*		handleClient	(void*	vPtr
				)
{
  //  I.  Application validity check:
  int*	argArray	= (int*)vPtr;	// CHANGE THAT NULL!
  int	fd		= argArray[0];	// CHANGE THAT 0!
  int	threadNum	= argArray[1];	// CHANGE THAT 0!

  //  SOMETHING HERE
  free(vPtr); //or is it argarray?

  if  (fd < 0)
  {
    fprintf(stderr,"BUG: Illegal file descriptor to handleClient()\n");
    return(NULL);
  }

  //  II.  Handle the client:
  printf("Thread %d beginning.\n",threadNum);

  //  II.A.  Each iteration handles one more file:
  while  (1)
  {
    //  II.A.1.  Get length and then filepath from client:
    int		length;
    char	filepath[BUFFER_LEN];

    //  B.2. HERE
    //is this where the problems are?
    int convInt;
    read(fd, &convInt, sizeof(convInt));
    length = ntohl(convInt);
    read(fd,filepath,length); 

    if  (filepath[0] == '\0')
      break;

    printf("  %s\n",filepath);

    //  II.A.2.  Handle request:
    computeAndSendHash(fd,filepath);
  }

  //  III.  Finished:
  //  B.3. HERE
  close(fd);
  printf("Thread %d ending.\n",threadNum);
  return(NULL);
}


//  PURPOSE:  To serve the clients using file-descriptor 'listenFd' to tell
//	when a client has connected.  Each client is handled by its own child
//	thread.  No return value.
void	       	doServer  	(int  	 	listenFd
				)
{
  //  I.  Application validity check:
  if  (listenFd < 0)
  {
    fprintf(stderr,"Illegal file-descriptor to doServer()\n");
    exit(EXIT_FAILURE);
  }

  //  II.  Do the work of the server:
  int			i	= 0;
  pthread_t		thread;
  pthread_attr_t	attr;

  //  YOUR CODE HERE FOR A.1.
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

  while  (1)
  {
    //  YOUR CODE HERE FOR A.2.
    int sockets = accept(listenFd, NULL, NULL);
    int *intArray = (int*) calloc(2,2*sizeof(int));
    intArray[0] = sockets;
    intArray[1] = i;
    pthread_create(&thread, &attr, handleClient, (void*)intArray);
    i++;
  }

  //  YOUR CODE HERE FOR A.3.
  pthread_attr_destroy(&attr);

  //  III.  Finished:
}


//  PURPOSE:  To oversee the main work of the server.  Ignores 'argc' but
//	uses 'argv[0]' as the name of the program.  Returns 'EXIT_SUCCESS' to
//	OS on success or 'EXIT_FAILURE' otherwise.
int		main		(int		argc,
				 char*		argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Do server:
  int	port		= getPort();
  int	socketFd	= getServerFileDescriptor(port,argv[0]);

  doServer(socketFd);
  close(socketFd);

  //  III.  Finished:
  return(EXIT_SUCCESS);
}