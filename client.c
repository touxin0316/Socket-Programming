// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

//can.cs.nthu.edu.tw/index.php
#define PORT 80

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    printf("please enter URL :");
    char link [300];
    scanf("%s", link);
    //printf("%s\n", link); 
    char tmp;
    char hostname[300] = {0};
    char obj[300] = {0};
    char request[10000];
    char response[1000000];
    int j = 0;
    for(int i = 0 ; i < strlen(link) ; i++)
    {
        if(link[i] == '/')
        {
            j = i;
            break;
        }
        else
        {
            hostname[i] = link[i];
        }    
    }
    int t = 0;
    for(int i = j+1;i < strlen(link);i++)
    {
        obj[t] = link[i];
        t ++;
    }
    //printf("%s\n",hostname);
    //printf("%s\n",obj);

    strcat(request,"GET /");
    strcat(request, obj);
    strcat(request," HTTP/1.1\r\nHost:");
    strcat(request, hostname);
    strcat(request,"\r\n\r\n");
    //printf("%s",request);
   
    struct hostent *h;
    if((h = gethostbyname(hostname)) == NULL)
	{
		printf("ERROR: gethostbyname error for hostname: %s\n", link);
		return 0;
	}
    memcpy(&serv_addr.sin_addr, h->h_addr_list[0], h->h_length);


    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    char *ptr,**pptr;
    char str[32];
    //pptr=h->h_addr_list;
    //for(;*pptr!=NULL;pptr++)
    inet_ntop(h->h_addrtype, *h->h_addr_list, str, sizeof(str));
    //printf("%s\n",str);
    // Convert IPv4 and IPv6 addresses from text to binary form
    
    if(inet_pton(AF_INET,  str, &serv_addr.sin_addr)<=0)
    {
       printf("\nInvalid address/ Address not supported \n");
       return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    int k = 0;
    int flag = 0;
    int count = 0;
    while(1)
    {
        
        send(sock , request , strlen(request) , k);
        valread = read(sock , buffer, 1024);
        //strcat(buffer_tmp, buffer);
        for(int i = 0;i<1024;i++)
        {
            if( flag == 0  && buffer[i] == '<' && buffer[i+1] == '/' && buffer[i+2] == 'h' && buffer[i+3] == 't'&& buffer[i+4] == 'm' && buffer[i+5] == 'l' && buffer[i+6] == '>')// && buffer[i+7] == '1')//)
            {
                //printf("hi");
                flag = 1;
                break;
            }
            else if(flag == 0  && buffer[i] == '<' && buffer[i+1] == 'a' && buffer[i+3] == 'h')
            {
                count += 1;
                int first = i+9;
                while(buffer[first] != '"')
                {
                    printf("%c",buffer[first]);
                    first ++;
                }
                printf("\n");
                    
            }
            
            //printf("%c",buffer[i]);
        }
        if(flag == 1)
            break;
    
        k++;
    }
    printf("The number of hyperlinks : %d\n", count);
    
    return 0;
}