#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <sys/socket.h>
#include <iostream>
using namespace std;

#define RCVBUFSIZE 1024


const char* receviceByURL(const char *servIP, const char*request,
		unsigned short servPort) {
	int sock; // Socket descriptor
	struct sockaddr_in servAddr; // server address
	char recvBuffer[RCVBUFSIZE]; // Buffer for response string
	unsigned int requestLen; // Length of string to send
	int bytesRcvd; // Bytes read in single recv()
	bool status = true;
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		status = false;
	}
	if (status) {
		// Convert dotted decimal into binary server address.
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = inet_addr(servIP);
		servAddr.sin_port = htons(servPort);

		// Connect to the server.
		if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr))
				< 0) {
			status = false;
		}
	}
	if (status) {
		requestLen = strlen(request);
		if (send(sock, request, requestLen, 0) != requestLen) {
			status = false;
		}
	}
	string msg;
	if (status) {
		std::cout << "My IP Address: " << endl;
		int index = 0;
		while (status) {
			//读取接受到数据
			if ((bytesRcvd = recv(sock, recvBuffer, RCVBUFSIZE - 1, 0)) <= 0) {
				status = false;
			}
			if (status) {
				if (bytesRcvd < (RCVBUFSIZE - 1)) {
					recvBuffer[bytesRcvd] = '\0';
					status = false;
				} else {
				}
			}
			msg.append(recvBuffer);
			index++;
		}
	}
	close(sock);
	return msg.c_str();
}

int main() {
	const char*hostStr = "api.taose69.com";
	const char*questPath = "/items/getAllParentsCategorys";
	struct hostent* host = gethostbyname(hostStr);
	char** current_addr = host->h_addr_list;
	if (*current_addr != NULL) {
		struct in_addr* addr = (struct in_addr*) (*current_addr);
		printf("address: %s\n", inet_ntoa(*addr));
		const char* ip = inet_ntoa(*addr);
		string request;
		request.append("GET ");
		request.append(questPath);
		request.append(" HTTP/1.1\r\nHost: ");
		request.append(hostStr);
		request.append("\n");
		request.append(
				"Content-Type:application/x-www-form-urlencoded ;charset=UTF-8\n");
		request.append(
				"User-Agent:AndroidBrunjoy/1.0 Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.2 Safari/537.17\n");
		request.append("\r\n\r\n ");
		const char * msg = receviceByURL(ip, request.c_str(), 80);
		cout << "============receviceData============================" << endl;
		cout << msg << endl;
		cout << "========================================" << endl;
	}

}
