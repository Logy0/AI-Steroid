
#ifndef UNICODE
#define UNICODE
#endif
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <array>
#include <iostream>
#include "../headers/Defines.hpp"
#include "../headers/AsteroidManager.hpp"
#include "../headers/Asteroid.hpp"
#include "../headers/TextureManager.hpp"
#include "../headers/IAGridVisual.hpp"

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
SOCKET createSocket()
{
	// Declare some variables
	WSADATA wsaData;

	int iResult = 0;            // used to return function results

								// the listening socket to be created
	SOCKET ListenSocket = INVALID_SOCKET;

	// The socket address to be passed to bind
	sockaddr_in service;

	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"Error at WSAStartup()\n");
		return 1;
	}
	//----------------------
	// Create a SOCKET for listening for 
	// incoming connection requests
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		wprintf(L"socket function failed with error: %u\n", WSAGetLastError());
		WSACleanup();
		
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(55001);

	//----------------------
	// Bind the socket.
	iResult = bind(ListenSocket, (SOCKADDR *)&service, sizeof(service));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"bind failed with error %u\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		
	}
	else
		wprintf(L"bind returned success\n");

	if (listen(ListenSocket, 2) == SOCKET_ERROR)
		wprintf(L"listen function failed with error: %d\n", WSAGetLastError());

	SOCKET AcceptSocket = INVALID_SOCKET;
	AcceptSocket = accept(ListenSocket, NULL, NULL);
	if (AcceptSocket == INVALID_SOCKET) {
		wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
		closesocket(AcceptSocket);
		WSACleanup();
		return -1;
	}
	else
	{
		wprintf(L"Client connected.\n");
		return AcceptSocket;
	}
}
int sendtoSocket(SOCKET AcceptSocket,char *data,int n)
{
	
	//cout << (int)strlen((const char *)data) << endl;
	int iResult = send(AcceptSocket, (char*)data, n, 0);
	
	if (iResult == SOCKET_ERROR) {
		wprintf(L"send failed with error: %d\n", WSAGetLastError());
		closesocket(AcceptSocket);
		WSACleanup();
		return -1;
	}
	//printf("Bytes Sent: %d\n", iResult);
	return 0;
}
int closeSocket(SOCKET AcceptSocket)
{
	shutdown(AcceptSocket, 2);
	closesocket(AcceptSocket);
	return 0;
}
int readfromSocket(SOCKET AcceptSocket, int *x, int *y)
{
	char buf[2];
	int iResult = recv(AcceptSocket, buf, 2, MSG_WAITALL);

	if (iResult == SOCKET_ERROR) {
		wprintf(L"read failed with error: %d\n", WSAGetLastError());
		closesocket(AcceptSocket);
		WSACleanup();
		return -1;
	}
	*x = (int)buf[0];
	*y = (int)buf[1];
	return 0;
}
int reset(AsteroidManager manager)
{
	return 0;
}
int main()
{
	int v_x = int(GRID_X/2);
	int v_y = int(GRID_Y/2);
	SOCKET AcceptSocket = createSocket();

	sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Test genetaration");
	window.setFramerateLimit(1);

	sf::Clock clock;
	float sec = 0;
	int n_ast = 30;
	float score = 0;
	char cscore = 0;
	bool gameover = false;
	bool drawwin = false;

	IAGridVisual grid;
	TextureManager textureMgr;
	textureMgr.add("asteroid", PATH_TEXTURES + "asteroids.png");
	AsteroidManager manager(textureMgr);
	bool toggle = false;
	float px,py,vx,vy;
	int x, y;
	for (int k = 0;k < n_ast;k++)
	{
		px = float(rand() % WINDOW_X);
		py = float(rand() % WINDOW_Y);
		vx = float(rand() % 40-20);
		vy = float(rand() % 40-20);
		while (abs(px - v_x/GRID_X*WINDOW_X) <= 2 || abs(py - v_y/GRID_Y*WINDOW_Y) <= 2)
		{
			px = float(rand() % WINDOW_X);
			py = float(rand() % WINDOW_Y);
		}

		manager.generate({ px,py }, {vx,vy});
	}
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::A)
				{
					toggle = true;
				}
				if (event.key.code == sf::Keyboard::R)
				{
					reset(manager);
				}
				if (event.key.code == sf::Keyboard::D)
				{
					drawwin = !drawwin;
				}

			}
		}
		sec = clock.getElapsedTime().asSeconds();
		manager.update(1.53);
		clock.restart();
		window.clear();
		window.draw(manager);
		if (ENABLE_GRID_TEST_MOD)
		{
			grid.reset();
			for (const auto& it : manager.getAsteroids())
			{
				grid.update(it);
			}
			grid.updateVisual();

		}
		
		if (toggle)
		{
			for (int k = 0;k < 20;k++)
			{
				sendtoSocket(AcceptSocket, grid.grid_value[k], GRID_Y);
			}
			//sendtoSocket(AcceptSocket, &cscore,1);
			toggle = true;
			readfromSocket(AcceptSocket, &x, &y);
			cout << x << " " << y << endl;
			grid.grid_value[y][x] = (char)2;
		}

		if (drawwin)
		{
			window.setFramerateLimit(1);
			grid.updateVisual();
			window.draw(grid);

			window.display();
		}
		else
		{
			window.setFramerateLimit(0);
		}

		
		
		//cscore = char(1.5E-5 * 10000);
		/*
		if (grid.grid_value[v_y][v_x] != 1)
		{
			score += sec;
		}
		else
		{
			score = 0;
			sec = 0;
		}
		cout << "score : " << score << endl;
		*/
		
	}
}