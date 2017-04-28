#include<stdio.h> //printf
#include<string.h>    //strlen
#include<stdlib.h>
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

void checkFileTransfer(int sock);
 
int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char username[500];
	char server_reply[2000];
	char server_success_reply[2000];
	char password[500];
	char file_name[2000];
	char choice[50];
	char response[10];
	char file_contents[2000];
	char* str;
	int read_size;
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	 
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );
 
	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	 
	puts("Connected\n");
	 
	int displayLoggin = 0;
	//keep communicating with server
	while(1)
	{
		if(displayLoggin == 0) {
			printf("Enter Username : ");
			scanf("%s" , username);
			
			printf("Enter Password : ");
			scanf("%s" , password);
			 
			send(sock, username, strlen(username), 0);
			send(sock, password, strlen(password), 0);
			displayLoggin++;
			
		} else if(displayLoggin == 1) {
			
			//Receive a reply from the server
			if( recv(sock , server_reply , 2000 , 0) < 0)
			{
				puts("recv failed");
				break;
			}
			 
			puts("Server reply :");
			puts(server_reply);
					
			if(strcmp(server_reply, "User Found")  == 0) {
				
				// Display menu to user
				puts("**** Choose directory to transfer file **** ");
				puts("1. Sales Directory");
				puts("2. Promotions Directory");
				puts("3. Offer Directory");
				puts("4. Marketing Directory");
				puts("5. Root Directory");
				
				scanf("%s",&choice);
				
				printf("Option Choosing %s \n",choice);
				send(sock, choice, strlen(choice), 0);
				
				puts("Enter file name to transfer to the directory");
				scanf("%s" , file_name);
				
				// Open file to transfer contents over.
				FILE *file = fopen( file_name, "r" );  
				char c;
				
				if(!file) {
					printf("Error, no file found");
				}
				
				
				if (file) {
					// send the file name
					send(sock, file_name, strlen(file_name), 0);
					int num_of_chars = 0;
					while ((c = getc(file)) != EOF) {
						num_of_chars++;
					}
					// Reset to start of file.
					fseek(file, 0, SEEK_SET);

					char* file_contents = (char*)malloc(sizeof(char) * num_of_chars);

					// read in file contents
					int i = 0;
					puts("Contents to add to file: ");
					while ((c = getc(file)) != EOF) {
						file_contents[i] = c;
						i++;
					}
					printf("%s", file_contents);
					
					// send the dfile contents to the server.
					send(sock, file_contents, strlen(file_contents), 0);
					send(sock, "Done", strlen("Done"), 0);
					
					memset(server_reply, 0, 2000);
					if( recv(sock , server_reply , 2000 , 0) < 0)
					{
						puts("recv failed");
						break;
					}
					 
					puts("\nServer reply :");
					if(strcmp(server_reply, "Finished") == 0) {
						puts("File Transfer Success");
					} else {
						puts("File Transfer Failed");
					}
					
					
					fclose(file);
					
					//recv(sock, response, 10, 0);
//					while( (read_size = recv(sock , server_success_reply , 2000 , 0)) > 0 )
//					{
//						if(strcmp(server_success_reply, "Success")  == 0) {
//							puts("File transgfer Complete");	
//							break;
//						}
//						
//					}
					
					break;
					
					//checkFileTransfer(sock);
				} 		
			
		}
		
		 
			
		} else {
			puts("User not found, try again");
			break;
		}

	}
	 
	close(sock);
	return 0;
}

void checkFileTransfer(int sock) {
	char server_success_reply[2000];
	
	puts("Server reply");
	
	if( recv(sock , server_success_reply , 2000 , 0) < 0)
	{
		puts("recv failed");
	}
	puts(server_success_reply);
}