#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include <openssl/rand.h>
#include <openssl/err.h>



#define type(x) _Generic((x),                                                     \
        _Bool: "_Bool",                  unsigned char: "unsigned char",          \
         char: "char",                     signed char: "signed char",            \
    short int: "short int",         unsigned short int: "unsigned short int",     \
          int: "int",                     unsigned int: "unsigned int",           \
     long int: "long int",           unsigned long int: "unsigned long int",      \
long long int: "long long int", unsigned long long int: "unsigned long long int", \
        float: "float",                         double: "double",                 \
  long double: "long double",                   char *: "char *",                 \
       void *: "void *",                         int *: "int *",                  \
      default: "unknown")

#define AUTH_ID_LEN 4

struct topic
{
    char  group[10];
    char  pubTopic[10];
    char  subTopic[10];
};

struct sessionKeyReq
{
    unsigned char Auth_nonce[8] ;
    unsigned char Entity_nonce[8] ;
    int *NumKeys ;
    char Sender [20] ;
    char Purpose [10] ;
};

struct topic Topic;  // Topic declaration;
struct sessionKeyReq SessionKeyReq; // SessionkeyReq declaration;
unsigned char message[15];
unsigned char auth_id[AUTH_ID_LEN];

void sender()
{
    strcpy(SessionKeyReq.Sender,"net1.client");
}

void purpose()
{
    strcpy(SessionKeyReq.Purpose , "group");

}

void nonce_generator()  // nonce generator;
{
    unsigned char buffer[8];
    int length = 8;
    RAND_bytes(buffer,length);
    printf("Random Entity Number = ");
    for(int i=0;i<8; i++)
        {
        printf("%x ", buffer[i]);
        SessionKeyReq.Entity_nonce[i] = buffer[i];
        }
    printf("\n");
}    


void serializeSessionkeyReq() 
{
    *SessionKeyReq.NumKeys = 1;
    if(SessionKeyReq.Auth_nonce == NULL || SessionKeyReq.Entity_nonce == NULL || 
    SessionKeyReq.NumKeys == NULL || SessionKeyReq.Purpose == NULL ||  SessionKeyReq.Sender == NULL)
    {
        printf("Error: SessionKeyReq nonce or replyNonce or purpose or numKeys is missing.");
    }
    else
    {

    }
    
}


void TcpCommunication(int argc, char* argv[]) // TCP Connection(client)
{
    int my_sock;
    struct sockaddr_in serv_addr;
    int str_len;
    if(argc != 3)
    {
        printf("%s <IP> <PORT>\n", argv[0]);
        exit(1);
    }
    my_sock = socket(PF_INET,SOCK_STREAM,0); //1번
    if(my_sock == -1)
        printf("socket error \n");
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    if(connect(my_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1) //2번
        printf("connect error\n");
    memset(message,0x00,sizeof(message));
    str_len = read(my_sock,message,sizeof(message)-1); //3번
    if(str_len==-1)
        printf("read error\n");
    if(message[0] == 0)
        printf("Received AUTH_HELLO Message!!! \n");
        printf("Receiving message from Auth : ");
    for(int i=0; i<str_len ; i++)
    {
        printf("%x ",message[i]);
    }
    close(my_sock); //4번
  
}
void AuthID()
{
    for(int i =0 ; i<AUTH_ID_LEN; i++)
    {       
        auth_id[i] = message[i+2]; 
    }
}
void AuthNonce()
{
    for(int j = 0; j<sizeof(message)-(AUTH_ID_LEN+2)-1;j++)
    {
        SessionKeyReq.Auth_nonce[j] = message[AUTH_ID_LEN+2+j];
    }
}
int main(int argc, char* argv[])
{
        TcpCommunication(argc, argv);
        AuthID();
        AuthNonce();
        printf("\n");
        nonce_generator();
        purpose();

        if(message[0] == 0)   //Auth_Hello 받았을 때!
        {

        }
            
    
}

