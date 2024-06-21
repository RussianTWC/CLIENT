#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

void recv_file(SOCKET* sock);

int main() {
	WORD dllVer = MAKEWORD(2, 1);
	WSADATA wsad;

	WSAStartup(dllVer, &wsad);

	SOCKADDR_IN addr_info;
	memset(&addr_info, 0, sizeof(SOCKADDR_IN));

	addr_info.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr_info.sin_port = htons(4321);
	addr_info.sin_family = AF_INET;

	SOCKET s_client = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(s_client, (SOCKADDR_IN*)&addr_info, sizeof(addr_info)) == 0) {
		printf("Connect\n");

		char* path = malloc(33 * sizeof(char));
		if (path == NULL)
			return -1;
		scanf_s("%s", path, 33);

		send(s_client, path, 33, 0);
		recv_file(&s_client);
	}
	else {
		printf("Not Connect\n");
	}

	system("pause");
	return 0;
}

void recv_file(SOCKET* sock) {
	char file_size_str[1024];
	char file_name[32];

	if (recv(*sock, file_size_str, 1024, 0) < 0) {
		printf("Error. file not sended");
		return;
	}
	int file_size = atoi(file_size_str);
	char* bytes = (char*)malloc((file_size + 1)*sizeof(char));
	if (bytes == NULL)
		return;

	if (recv(*sock, file_name, 32, 0) < 0) {
		printf("Error. file not sended");
		return;
	}

	FILE* file;
	fopen_s(&file, file_name, "wb");

	printf("size: %d\nname: %s\n", file_size, file_name);

	if (file == NULL) {
		printf("Error. file not open");
		return;
	}
	else {
		recv(*sock, bytes, file_size, 0);
		//printf("data: %s\n", bytes);

		fwrite(bytes, sizeof(*bytes), file_size, file);
		printf("file saved\n");
	}
	free(bytes);
	fclose(file);
	return;
}