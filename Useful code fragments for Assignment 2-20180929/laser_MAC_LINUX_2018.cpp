#include<stdio.h> //printf
#include<string>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<unistd.h>

struct Points {
	double X;
	double Y;
};


class Laser
{
private:
	int portNumber;
	int sock;
	std::string ip;
	char message[1000] , server_reply[2000];


public:
	Points Ranges[361];

	~Laser() {}
	
	Laser(std::string ip,int portNumber) {

		struct sockaddr_in server;

		this->ip = ip;
		this->portNumber = portNumber;

		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1)
		{
		    printf("Could not create socket");
		}
		puts("Socket created");
		 
		server.sin_addr.s_addr = inet_addr(ip.c_str());
		server.sin_family = AF_INET;
		server.sin_port = htons(portNumber);

		//Connect to remote server
		if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
		{
		    perror("connect failed. Error");
		    // return 1;
		}

		//authenticate with the server
		// i.e. send student number and check response
	}

	void parseScan(){

		// one way to do this is to copy data into a stringstream object
		// then read from it into a string object until you get to the data you want
		// then read in the resolution (its stored a hex value)

		double StartAngle = 0.0; // process Data to get StartAngle (skip 23 spaces)
						   // then fill in an int32 byte by byte and divide the result by 10000.


		double Resolution = 0;//process Data to get resolution, i.e. skip 24 spaces (0x20) in Data,
					   //then fill in an Int16 byte by byte, divide result by 10000.


		int NumData = 0; // procrss Data to get NumData skip 25 spaces and
					 // then fill in an uint_16  byte by byte.


		// process Data to update ranges skip 26 spaces, then run a for loop
		for (int i = 0; i < NumData; i++)
		{
			Ranges[i].X = 0;// Process data to fill these values
			Ranges[i].Y = 0;// Process data to fill these values
		}

	}

	void getSingleScan(){
		int bytes;
		const char STX[1] = {0x02};
    	const char ETX[1] = {0x03};

		send(sock , STX, 1 , 0); //STX
        
        char scanOnce[100] = "sRN LMDscandata";
        //send laser data request to server
        bytes = send(sock , scanOnce , strlen(scanOnce) , 0);
        if(bytes < 0)
        {
            puts("Send failed");
            // return 1;
        }
        send(sock , ETX, 1 , 0); //ETX

        bytes = recv(sock, server_reply, sizeof(server_reply) , 0);

        //Receive a reply from the server
        if( bytes < 0)
        {
            puts("recv failed");
        }
         
        // debug stuff to see what you receive
        puts("Server reply :");
        puts(server_reply);

        parseScan();
	}
};


int main()
{
	Laser MyLaser = Laser("192.168.1.200", 23000);

	MyLaser.getSingleScan();

	// first data point
	printf("X = %f \tY = %f\n", MyLaser.Ranges[0].X, MyLaser.Ranges[0].Y);

	return 0;
}