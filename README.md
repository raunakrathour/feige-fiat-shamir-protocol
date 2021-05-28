# feige-fiat-shamir-protocol


This is the practical implementation of feige-fiat shamir authentication protocol using C++ socket programming.


Here we have used client(A) server(B) artitecture using socket programming.

A wants to chat with B. So A proof to B that he knows the secret using feige-fiat shamir protocol.

B verifies the identity. If not proved request will be declined otherwise chat start.


How to run the code:-

1. Run g++ A.cpp -o A. Output file A will form which is the client.

2. Run g++ B.cpp -o B. Output file B will form which is the server.

3. Now Run B first by ./B. Enter the secret vector.(Note secret will not shared, here it is only to calculate public key(V) of A)

4. Now Run A by ./A. Enter the secret vector A knows(Note A will not share the secret).

5. If A identity is verified chat starts otherwise declines.
