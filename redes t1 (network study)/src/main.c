#include "main.h"

int main (int argc, char **argv) {

	int socket;

    if (argc < 2) {
        socket_help ();

    } else if (strcmp (argv[1], "--client") == 0) {
		ext_cdtousr ();
		socket = socket_create (TRANSFER_DEVICE_CLIENT);
        client_loop (socket);
	}

    else if (strcmp (argv[1], "--server") == 0) {
		ext_cdtousr ();
		socket = socket_create (TRANSFER_DEVICE_SERVER);
        server_loop (socket);
	}

	else
		socket_help ();

    return 0;
}
