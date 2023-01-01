#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>

#define TIME_PORT	27015

void main()
{
    WSAData wsaData;
    if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        cout << "Time Client: Error at WSAStartup()\n";
        return;
    }
    SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == connSocket)
    {
        cout << "Time Client: Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return;
    }
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(TIME_PORT);

    int bytesSent = 0, delay_sum = 0, bytesRecv=0, userchoise = 0, citychoise = 0, time_recieve = 0, prev_time_recieve = 0;
    char sendBuff[2];
    char recvBuff[255];
    DWORD sendTime = 0, time_recieveDWORD = 0;
    
    while (true)
    {
        /*
        MENU:
        1.GetTime
        2.GetTimeWithoutDate
        3.GetTimeSinceEpoch
        4.GetClientToServerDelayEstimation
        5.MeasureRTT
        6.GetTimeWithoutDateOrSeconds
        7.GetYear
        8.GetMonthAndDay
        9.GetSecondsSinceBeginingOfMonth
        10.GetWeekOfYear
        11.GetDaylightSavings
        12.GetTimeWithoutDateInCity
        13.MeasureTimeLap
        14.Exit";
        */
        cout << "Menu:\n";
        cout << "1.GetTime\n2.GetTimeWithoutDate\n3.GetTimeSinceEpoch\n4.GetClientToServerDelayEstimation\n5.MeasureRTT\n6.GetTimeWithoutDateOrSeconds\n7.GetYear\n8.GetMonthAndDay\n9.GetSecondsSinceBeginingOfMonth\n10.GetWeekOfYear\n11.GetDaylightSavings\n12.GetTimeWithoutDateInCity\n13.MeasureTimeLap\n14.Exit\n";
        cin >> userchoise;
        switch (userchoise)
        {
        case 1:
            sendBuff[0] = 'A'; sendBuff[1] = '\0';
            break;
        case 2:
            sendBuff[0] = 'B'; sendBuff[1] = '\0';
            break;
        case 3:
            sendBuff[0] = 'C'; sendBuff[1] = '\0';
            break;
        case 4:
            sendBuff[0] = 'D'; sendBuff[1] = '\0';
            for (int i = 0; i < 100; i++)
                bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
            for (int i = 0; i < 100; i++)
            {
                bytesRecv = recv(connSocket, recvBuff, 255, 0);
                recvBuff[bytesRecv] = '\0';            
                time_recieve = atoi(recvBuff);
                if (i > 0)
                    delay_sum += time_recieve - prev_time_recieve;
                
                prev_time_recieve = time_recieve;
            }
            cout << "Avg: " << delay_sum / 100.0 << "ms" << endl;
            break;
        case 5:
            sendBuff[0] = 'E'; sendBuff[1] = '\0';
            for (int i = 0; i < 100; i++)
            {
                sendTime = GetTickCount();
                bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
                bytesRecv = recv(connSocket, recvBuff, 255, 0);
                recvBuff[bytesRecv] = '\0'; 
                time_recieveDWORD = GetTickCount();
                delay_sum += time_recieveDWORD - sendTime;
            }
            cout << "RTT Avg: " << delay_sum / 100.0 << "ms" << endl;
            break;
        case 6:
            sendBuff[0] = 'F'; sendBuff[1] = '\0';
            break;
        case 7:
            sendBuff[0] = 'G'; sendBuff[1] = '\0';
            break;
        case 8:
            sendBuff[0] = 'H'; sendBuff[1] = '\0';
            break;
        case 9:
            sendBuff[0] = 'I'; sendBuff[1] = '\0';
            break;
        case 10:
            sendBuff[0] = 'J'; sendBuff[1] = '\0';
            break;
        case 11:
            sendBuff[0] = 'K'; sendBuff[1] = '\0';
            break;
        case 12://CITIES
            cout << "Choose a city from the list:" << endl;
            cout << "1.Doha (Qatar)" << endl;
            cout << "2.Prague (Czech republic)" << endl;
            cout << "3.New York (USA)" << endl;
            cout << "4.Berlin (Germany)" << endl;
            cin >> citychoise;
            if (citychoise == 1) {
                sendBuff[0] = 'N';//QATAR
                sendBuff[1] = '\0';
            }
            else if (citychoise == 2) {
                sendBuff[0] = 'O';//PRAGUE
                sendBuff[1] = '\0';
            }
            else if (citychoise == 3) {
                sendBuff[0] = 'P';//NY
                sendBuff[1] = '\0';
            }
            else if (citychoise == 4) {
                sendBuff[0] = 'Q';//BERLIN
                sendBuff[1] = '\0';
            }
            else {
                sendBuff[0] = 'R'; //UTC
                sendBuff[1] = '\0';
            }
            break;
        case 13:
            sendBuff[0] = 'M';
            sendBuff[1] = '\0';
            break;
        case 14:
            cout << "Exit" << endl;
            return;
        default:
            break;
        }
        sendBuff[1] = '\0';
        bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
        if (SOCKET_ERROR == bytesSent)
        {
            cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
            closesocket(connSocket);
            WSACleanup();
            return;
        }
        cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";
        // Gets the server's answer using simple recieve (no need to hold the server's address).
        bytesRecv = recv(connSocket, recvBuff, 255, 0);
        if (SOCKET_ERROR == bytesRecv)
        {
            cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
            closesocket(connSocket);
            WSACleanup();
            return;
        }
        recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
        cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
    }
    // Closing connections and Winsock.
    cout << "Time Client: Closing Connection.\n";
    closesocket(connSocket);
    system("pause");
}