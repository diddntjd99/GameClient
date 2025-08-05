//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // 링커에 ws2_32 라이브러리 연결

using namespace std;

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    SOCKADDR_IN serverAddr{};
    char buffer[1024] = { 0 };

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup 실패" << endl;
        return 1;
    }

    // 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "소켓 생성 실패" << endl;
        WSACleanup();
        return 1;
    }

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    serverAddr.sin_port = htons(65432);

    // 서버에 연결 요청
    if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "서버 연결 실패" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    while (true) {
        string msg;
        cout << "전송할 메세지: ";
        cin >> msg;

        if (msg == "exit") {
            break;
        }

        // 메시지 전송
        send(clientSocket, msg.c_str(), msg.size(), 0);

        // 응답 수신
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            cout << "서버 응답: " << buffer << endl;
        }
    }

    // 소켓 종료
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
