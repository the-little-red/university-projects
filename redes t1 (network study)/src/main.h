#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netpacket/packet.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

#define DEBUG 0

#define TYPE_ACK   'Y'
#define TYPE_NACK  'N'
#define TYPE_ERROR 'E'
#define TYPE_SHOW  'X'
#define TYPE_DATA  'D'
#define TYPE_END   'Z'
#define TYPE_NULL  '0'

#define TYPE_CD    'C'
#define TYPE_LS    'L'
#define TYPE_PUT   'P'
#define TYPE_GET   'G'

#define TRANSFER_PORT_CLIENT   7107
#define TRANSFER_PORT_SERVER   7107
#define TRANSFER_DEVICE_CLIENT "eth0"
#define TRANSFER_DEVICE_SERVER "eth0"

extern int _seq;

typedef struct _packet {
	unsigned char init; // sequencia de bits inicio
	unsigned char size; // tamanho do pacote
	unsigned char seq; // numero de sequencia
	unsigned char type; // tipo de dado
	unsigned char *data; // o dado em si
	unsigned char par; // verificador de erro crc (nosso caso bit de paridade)
} *packet;

void socket_help   ();
int  socket_create (const char *device);

packet         packet_mount  (int seq, int type,
                              unsigned char *data, int data_size);
packet         packet_umount (packet p);
unsigned char *packet_raw    (packet p);
packet         packet_unraw  (unsigned char *r);
int            packet_parity (packet p);
int 	 				 packet_crc    (unsigned char *message); //tentativa de implementar crc
int            packet_send   (int socket, packet p);
packet         packet_recv   (int socket);
int            packet_seq    ();

void client_loop  (int socket);
void client_help  ();
void client_cd    (int socket, char *folder);
void client_ls    (int socket, char *opt);
void client_get   (int socket, char *file);
void client_ll   (void);
void client_cc   (char* dir);

void server_loop  (int socket);
void server_ls    (int socket, char *data);
void server_cd    (int socket, char *dir);

char *ext_getexename (char *buf, size_t size);
void  ext_cdtousr    ();
char  ext_progress   (char c);

void file_send (int socket, char *file);
void file_recv (int socket, char *file);

void debug_packet (packet p);
void debug_raw    (unsigned char *r);

#define PACKET_INIT    126
#define PACKET_MINSIZE 2
#define PACKET_MAXSIZE 256
#define PACKET_DATA    252
#define PACKET_TRIES   16
#define PACKET_SEQ     8
#define PACKET_TIME    10000
