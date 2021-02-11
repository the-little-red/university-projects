#include "main.h"

void server_loop (int socket) {

	printf("-------------------------------------\n");
	printf("server: rodando em modo servidor\n");
	printf("server: escutando a interface %s\n", TRANSFER_DEVICE_SERVER);
	printf("-------------------------------------\n");
	packet p;

	while (1) {

		if (!(p = packet_recv (socket)))
			continue;

		if (p->type == TYPE_LS)
			server_ls (socket, (char*) p->data);

		else if (p->type == TYPE_CD)
			server_cd (socket, (char*) p->data);

		else if (p->type == TYPE_PUT)
			file_recv (socket, (char*) p->data);

		else if (p->type == TYPE_GET)
			file_send (socket, (char*) p->data);

		free(p);

	}

	return;
}

void server_ls (int socket, char* data) {

	struct dirent *dp;

	char cwd[1024];
	getcwd (cwd, sizeof(cwd));
	char *out = malloc (PACKET_DATA);
    FILE *fp;
    long int size;

	DIR *dir = opendir (cwd);
	while ((dp = readdir (dir)) != NULL) {
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
			out = realloc (out, sizeof(char) * (strlen (out) + strlen (dp->d_name) + 10));
			out = strcat (out, dp->d_name);

            if (strcmp(data, "-l") == 0) {
                fp = fopen (dp->d_name, "r");
                fseek (fp, 0, SEEK_END);
                size = ftell (fp);
                fclose (fp);
                sprintf (out, "%s\t\t%ld", out, size);
            }

            sprintf (out, "%s\n", out);
		}
	}
	closedir(dir);
	out[strlen (out)] = '\0';

	packet p = packet_mount (_seq, TYPE_SHOW,
		(unsigned char*) out, strlen (out));
	packet_send (socket, p);

}

void server_cd (int socket, char* dir) {

	char cwd[1024];
	packet p = packet_mount (_seq, 0, NULL, 0);

	if ((chdir ((char*) dir)) == 0) {
		getcwd (cwd, sizeof(cwd));
		p->type = TYPE_SHOW;
		if (DEBUG)
			printf ("cd: diretorio: %s\n", dir);
	} else {
		p->type = TYPE_ERROR;
		if (DEBUG)
			printf ("cd: %s\n", strerror(errno));
	}

	packet_send (socket, p);
}
