#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 80
#define ESCUCHAC 23
#define TAMBUFFER 8096
/**
 * @brief Socket http 
 * @param Utilizando un cliente
 * @param Como navegador
 * @return Un web socket
 */
int main() {

int server;
int client;
int localerror;
struct sockaddr_in myAddr;
struct sockaddr_in cliente_addr;
socklen_t clienteLen;	
int status;
int escucha_da, socket_da, pid;
  size_t longitud;
  static struct sockaddr_in servidordir;
  static struct sockaddr_in clientedir;

  int i, archivo_da;
  long r, tam;
  static char buffer[TAMBUFFER+1];
  struct stat st;


//Iniciamos la apertura del Socket
server = socket(PF_INET,SOCK_STREAM,0);
if(server == -1) {
//localerror = errno;
fprintf(stderr, "Error: %s\n",strerror(localerror));
return 1;
}

//Nos adjudicamos el Puerto.
bzero(&myAddr,sizeof(myAddr));
myAddr.sin_family = AF_INET;
myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
myAddr.sin_port = htons(PORT);

status = bind(server, (struct sockaddr *)&myAddr, sizeof(myAddr));
if(status == 0) {
//localerror = errno;
fprintf(stderr,"Can't Bind Port: %s\n",strerror(localerror));
close(server);
return 1;
}

//Nos ponemos el modo de Escucha
status = listen(server,5);
if(status == -1) {
//localerror = errno;
fprintf(stderr,"Can't listen on socket(%s)\n",strerror(localerror));
close(server);
return 1;
}	

//Esperamos una Conexión
while(1) {
bzero(&cliente_addr,sizeof(cliente_addr));
client = accept(server,(struct sockaddr *)&cliente_addr,&clienteLen);
if(client == -1) {
//localerror = errno;
fprintf(stderr,"Error acepting conection %s\n",strerror(localerror));
close(server);
return 1;
}

//Inicia el protocolo...
r = read (socket_da, buffer, TAMBUFFER);
buffer[r-1] = '\0';
printf ("%s\n", buffer);
if (strncmp (buffer, "GET ", 4) == 0) {
    for (i = 4; i < TAMBUFFER; i++)
      if (buffer[i] == ' ') {
	buffer[i] = '\0';
	break;
      }
		
    if (!strncmp (&buffer[0], "GET /\0", 6))
      strcpy (buffer, "GET /index.html");
		
    if ((archivo_da = open (&buffer[5], O_RDONLY)) == -1) {
      archivo_da = open ("404.html", O_RDONLY);
      fstat (archivo_da, &st);
      tam = st.st_size;
      sprintf (buffer, "HTTP/1.0 404 Not Found\r\n\r\n", tam);
    } else { 
      fstat (archivo_da, &st);
      tam = st.st_size;		
      sprintf (buffer, "HTTP/1.0 200 OK\r\n\r\n", tam);
    }

    write (socket_da, buffer, strlen (buffer));
		
while ((r = read (archivo_da, buffer, TAMBUFFER)) > 0 ){
      write (socket_da, buffer, r);
	}
	}
  }
//close(server);

return 0;
}
