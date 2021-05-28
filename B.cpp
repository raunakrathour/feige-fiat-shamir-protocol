#include<bits/stdc++.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
using namespace std;
#define ll unsigned long long

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main()
{
	// authentication pre-part
	srand (time(NULL));

	ll N = 1476221;  // Implicitly two prime number is multiplid to for n

	ll g, y, x, f;     // x will be send by A, we have to calculate y and g then verify y*y (mod n)==g
	cout << "N = " << N << endl;

	cout << "Enter number of secrets : ";
	cin >> f;
	vector<ll>secrets, V;
	for (ll i = 0; i < f; i++)                  // enter Secret vector
	{
		cout << "Enter Secret " << i + 1 << " ";
		ll secret;
		cin >> secret;
		secrets.push_back(secret);
		V.push_back((secrets[i] * secrets[i]) % N);    // calulate V which is public key of A
	}

	char c[255];
	for (ll i = 0; i < f; i++)                        // c vector randomly get 0 and 1 values
	{
		c[i] = '0' + rand() % 2;
	}

	// socket establishment
	int sockfd, newsockfd, portno, n, newsockfd1;
	char buffer[255];

	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);         // return file descriptor

	bzero((char*)&serv_addr, sizeof(serv_addr));      // clears buffers
	portno = atoi("8787");                            // convert character long longo string

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("Binding failed\n");

	listen(sockfd, 2);                               // max two connections at a time
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

	if (newsockfd < 0)
		error("Error on accept\n");

	ll k = 0;
	bool flag = false;

	while (1)
	{
		bzero(buffer, 255);
		n = read(newsockfd, buffer, 255);

		if (n < 0)
			error("Error on reading\n");

		if (k == 0)
		{
			x = stoi(buffer);
			g = x;
			for (ll i = 0; i < f; i++ )
			{
				if (c[i] == '1')
				{
					g = (g * V[i]) % N;
				}
			}
			bzero(buffer, 255);
			strcpy(buffer, c);
			cout << "Sending c = " << c << " to A" << endl;
		}
		else if (k == 1)
		{
			y = stoi(buffer);
			bzero(buffer, 255);
			cout << "G is " << g << " (y^2) mod N is " << (y * y) % N << endl;
			if ((y * y) % N == g )
			{
				cout << "Authentication Granted G == y^2" << endl;
				string temp = "Granted\n";
				bzero(buffer, 255);
				for (ll i = 0; i < temp.size(); i++)
				{
					buffer[i] = temp[i];
				}
				buffer[temp.size()] = '\0';
			}
			else
			{
				cout << "Access Denied A dont know the secret (G != y*y) !!!" << endl;
				flag = true;
				string temp = "Failed\n";
				bzero(buffer, 255);
				for (ll i = 0; i < temp.size(); i++)
				{
					buffer[i] = temp[i];
				}
				buffer[temp.size()] = '\0';
			}
		}
		else
		{
			cout << "A : " << buffer << endl;
			if (buffer[0] == 'e')
				break;
			cout << "You : ";

			bzero(buffer, 255);
			fgets(buffer, 255, stdin);
		}

		n = write(newsockfd, buffer, strlen(buffer));
		if (buffer[0] == 'e')
			break;

		if (flag)
			break;

		if (n < 0)
			error("Error on writing\n");

		k++;
	}

	close(newsockfd);
	close(sockfd);
	return 0;
}
