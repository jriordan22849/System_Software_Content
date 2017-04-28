#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //for threading , link with lpthread
#include <time.h>


#define TEXT_FILE "/Users/macbookpro/Documents/College/Fourth_Year/System_software/Assignment2/text.txt"
#define MARKETING "/Users/macbookpro/Documents/College/Fourth_Year/System_software/Assignment2/server/Marketing/"
#define OFFERS "/Users/macbookpro/Documents/College/Fourth_Year/System_software/Assignment2/server/Offers/"
#define PROMOTIONS "/Users/macbookpro/Documents/College/Fourth_Year/System_software/Assignment2/server/Promotions/"
#define SALES "/Users/macbookpro/Documents/College/Fourth_Year/System_software/Assignment2/server/Sales/"
#define ROOT "/Users/macbookpro/Documents/College/Fourth_Year/System_software/Assignment2/server/"
#define LOG "/Users/macbookpro/Documents/College/Fourth_Year/System_software/Assignment2/server/Log/log.txt"
 
//the thread function
void *connection_handler(void *);

pthread_mutex_t lock_file;
 
int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;
	 
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	 
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	 
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	 
	//Listen
	listen(socket_desc , 3);
	 
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	 
	 
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		 
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		 
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		 
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		puts("Handler assigned");
	}
	 
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	 
	return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];
	char username[100];
	char password[100];
	char directoryChosen[2000];
	char successOfFile[50];
	char fileName[50];
	int index = 0;
	char *success_message;
	char *failure_message;
	char *file_transfer_message;
	
	char file_line[200];
	char userInfo[200], passInfo[200];
	int user_found = 0;
	char file_contents[2000];
	char transferSuccess[200];
	FILE *fptr;
	
	//Receive a message from client
	while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{
		if(index == 0) {
			puts("Username");
			strcpy(username, client_message);
			puts(username);
		} else if(index == 1) {
			puts("Password");
			strcpy(password, client_message);
			puts(password);
			break;
		}
		index ++;
	}
	
	// Verify crentials
	
	puts(message);
	
	// Read in the password file
	// Check credentials
	FILE *auth_file = fopen(TEXT_FILE, "r");

	while(fgets(file_line, 80, auth_file)) {
		sscanf(file_line, "%s  %s", userInfo, passInfo);
		
		if((strcmp(username, userInfo) == 0) && (strcmp(password, passInfo) == 0)) {
			user_found = 1;
		}
		
	}
	
	if(user_found == 1) {
		message = "Credentials found\n";
		puts(message);
		
		success_message = "User Found";
		write(sock , success_message , strlen(success_message));	
	} else {
		message = "Credentials not found\n";
		puts(message);
		
		failure_message = "Not found";
		write(sock , failure_message , strlen(failure_message));
	}
	
	int indexx = 0;
	char file_name[200];
	char contents[2000];
	char file_path[2000];
	while( (read_size = recv(sock , directoryChosen , 2000 , 0)) > 0 )
	{
		// get directory
		if(indexx == 0) {
			puts("Directory Chosen");
			
			if( strcmp(directoryChosen, "1") == 0) {
				puts("Sales directory");
				strcpy(file_path, SALES);
			} 
			
			if (strcmp(directoryChosen, "2") == 0) {
				puts("Promotion directory");
				strcpy(file_path, PROMOTIONS);
			}  
			
			if(strcmp(directoryChosen, "3") == 0) {
				puts("Offer directory");
				strcpy(file_path, OFFERS);
			}  
			
			if( strcmp(directoryChosen, "4") == 0) {
				puts("Marketing directory");
				strcpy(file_path, MARKETING);
			} 
			if( strcmp(directoryChosen, "5") == 0) {
				puts("Root directory");
				strcpy(file_path, ROOT);
			} 

		}
		
		if(indexx == 1) {
			puts("File name: ");
			puts(directoryChosen);
			strcat(file_name, directoryChosen);
		}

		if(indexx == 2) {
			puts("Contents of file: ");
			puts(directoryChosen);
			strcat(contents, directoryChosen);
			memset(directoryChosen, 0, 2000);
		}
		
		if(indexx == 3) {
			strcat(successOfFile, directoryChosen);
			write(sock , "Finished" , strlen("Finished"));
		}
		
		indexx ++;	
	}
	
	puts(file_path);
	
	strcat(file_path, file_name);
	puts("\n");
	puts("File path");
	puts(file_path);
	
	puts("Content");
	puts(contents);
	
	puts("File transfer");
	puts(successOfFile);
	fclose(fptr);
	
	pthread_mutex_lock(&lock_file);

	 FILE *file_open = fopen(file_path, "w");
	 if(file_open == NULL) {
		printf("File %s Cannot be opened file on server.\n", file_name);
		file_transfer_message = "Fail";
		//write(sock , file_transfer_message , strlen(file_transfer_message));
	}
	else {
		int lstr = 0;
		lstr = strlen( contents );
		for(int i = 0 ; i < lstr; i++) {
			fputc(contents[i], file_open);  /* Write string to file backwards */
		}
		send(sock, "Done", strlen("Done"), 0);  
	}

	fclose(file_open);
	pthread_mutex_unlock(&lock_file);
	
	// Write to Log file
	char breakLine[] = "------------------------------------------------";
	FILE *fptr2;
	fptr2 = fopen(LOG,"a");
	if(fptr2 == NULL) {
		printf("Error");
		exit(1);
	}
	
	// Get current time
	time_t now;
	time(&now); 

	// Write to the file
	fprintf(fptr2,"\nUser: %s", username);
	fprintf(fptr2,"\nTime : %s",ctime(&now));
	fprintf(fptr2,"Transfer of file %s completed to directory %s:",file_name, file_path);
	fprintf(fptr2,"\n%s",breakLine);
	fclose(fptr2);

	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
		 
	//Free the socket pointer
	free(socket_desc);
	 
	return 0;
}
