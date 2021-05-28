#include<bits/stdc++.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
using namespace std;
#define ll unsigned long long

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main()
{
	srand(time(NULL));
	// p = 1217 and q = 1213
	ll N = 1476221, f;
	cout << "N = " << N << endl;

	cout << "Enter number of secrets : ";
	cin >> f;
	vector<ll>secrets, V;
	for (ll i = 0; i < f; i++)
	{
		cout << "Enter Secret " << i + 1 << " : ";
		ll secret;
		cin >> secret;
		secrets.push_back(secret);
		V.push_back((secrets[i] * secrets[i]) % N);
	}

	ll r = rand() % (N - 1) + 1;
	cout << "r = " << r << endl;
	ll x = (r * r) % N;
	cout << "x = r^2(mod n) =" << x << endl;

	ll y;
	char c[255];

	// authentication
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[255];

	portno = atoi("8787");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("Error opening socket ");

	server = gethostbyname("127.0.0.1");

	if (server == NULL)
		error("Error, no such host\n");

	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("Connection failed\n");

	ll k = 0;
	while (1)
	{
		bzero(buffer, 255);
		if (k == 0)
		{
			cout << "Sending x = r^2(mod n) = " << x << " to server...." << endl;

			string temp = to_string(x);
			for (ll i = 0; i < temp.size(); i++)
			{
				buffer[i] = temp[i];
			}
			buffer[temp.size()] = '\0';
		}
		else if (k == 1)
		{
			y = r;
			for (ll i = 0; i < f; i++)
			{
				if (c[i] == '1')
				{
					y = (y * secrets[i]) % N;
				}
			}
			cout << "Sending y = (r * ((s1^c1) * (s2^c2) * (s3^c3))*...*(sk^ck)) (mod n) = " << y << " to server...." << endl;
			string temp = to_string(y);
			for (ll i = 0; i < temp.size(); i++)
			{
				buffer[i] = temp[i];
			}
			buffer[temp.size()] = '\0';
		}
		else
		{
			cout << "You : ";
			fgets(buffer, 255, stdin);
		}

		n = write(sockfd, buffer, strlen(buffer));
		if (buffer[0] == 'e')
			break;

		if (n < 0)
			error("Error on Writing\n");

		bzero(buffer, 255);
		n = read (sockfd, buffer, 255);
		cout << buffer << endl;
		if (n < 0)
			error("Error on reading\n");
		if (k == 0)
		{
			strcpy(c, buffer);
			cout << "Received Challenge vector from B : " << c << endl;
		}
		else if (k == 1)
		{
			if (buffer[0] == 'F')
			{
				cout << "You dont know the secret bye..." << endl;
				break;
			}
			else
				cout << "You are Authenticated. Chat now..." << endl;
		}
		else
		{
			cout << "B : " << buffer << endl;
			if (buffer[0] == 'e')
				break;
		}

		k++;
		if (k > 10000)
		{
			k = 5;
		}
	}
	close(sockfd);
	return 0;
}