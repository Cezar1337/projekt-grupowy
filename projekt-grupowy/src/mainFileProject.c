#include <stdio.h>	//For standard things
#include <stdlib.h>	//malloc
#include <string.h>	//memset
#include <netinet/ip_icmp.h>	//Provides declarations for icmp header
#include <netinet/udp.h>	//Provides declarations for udp header
#include <netinet/tcp.h>	//Provides declarations for tcp header
#include <netinet/ip.h>	//Provides declarations for ip header
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>	//sifreq
#include <unistd.h>	//close
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#define BUF_SIZE 8400
#define SHM_KEY 0x1234
#define SIZE 1024

struct shmseg {
   int cnt;
   int complete;
   char buffer[BUF_SIZE];
};

struct packet {

	int id[1];
	int data[1024];

    struct addresses {
	char destinationIP[1024];
	char sourceIP[1024];
	}ipAddress;

}packet;

void ProcessPacket(unsigned char* , int);
void print_ip_header(unsigned char* , int);
void print_tcp_packet(unsigned char* , int);
void print_udp_packet(unsigned char * , int);
void print_icmp_packet(unsigned char* , int);
void PrintData (unsigned char* , int);

int sock_raw;
int sharedMemoryIDs[1024];
FILE *logfile;
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j;
struct sockaddr_in source,dest;

int itemsQ1[SIZE], frontQ1 = -1, rearQ1 = -1;
int itemsQ2[SIZE], frontQ2 = -1, rearQ2 = -1;
int itemsQ3[SIZE], frontQ3 = -1, rearQ3 = -1;

int main()
{

	//code that hopefully fixes the "cannot open display" error
	if (setenv ("DISPLAY", ":0", 0) == -1)
		error ("setenv");
	//-----------------initialize gtk--------------------
	//declare builder and window handlers
	GtkBuilder *builder;
	GtkWidget *window;

	gtk_init(NULL, NULL); //initialize gtk lib

	builder = gtk_builder_new_from_file("glade/switch_emulator.glade"); //load GUI layout from .glade file

	window = GTK_WIDGET(gtk_builder_get_object(builder, "switch_emulator")); //get window handler
	gtk_builder_connect_signals(builder, NULL);

	//get handlers to GUI elements here

	g_object_unref(builder);
	gtk_widget_show(window); //show the window
	gtk_main(); //start the gtk main loop

	//-----------------initialize gtk--------------------*/

	int saddr_size , data_size;
	struct sockaddr saddr;
	struct in_addr in;
    
	
	unsigned char *buffer = (unsigned char *)malloc(65536); 
	
	logfile=fopen("/home/mszostu/altput.txt","w");
	if(logfile==NULL) printf("Unable to create file.");
	printf("Starting...\n");
	//Create a raw socket that shall sniff
	sock_raw = socket(AF_INET , SOCK_RAW , IPPROTO_ICMP);
	if(sock_raw < 0)
	{
		printf("Socket Error\n");
		return 1;
	}
	while(1)
	{
		saddr_size = sizeof saddr;

		data_size = recvfrom(sock_raw , buffer , 1024 , 0 , &saddr , &saddr_size);
		if(data_size <0 )
		{
			printf("Recvfrom error , failed to get packets\n");
			return 1;
		}
		ProcessPacket(buffer , data_size);
	  //enQueue 3 elements
 

	} 

	close(sock_raw);
	printf("Finished");
	return 0;
}
// enqueue
void enQueue(int value) {
  if (rearQ1 == SIZE - 1)
    printf("\nQueue is Full!!");
  else {
    if (frontQ1 == -1)
      frontQ1 = 0;
    rearQ1++;
    itemsQ1[rearQ1] = value;
   // printf("\nInserted -> %d", value);
	
  }
}
// dequeue
void deQueue() {
  if (frontQ1 == -1) {}

  else {
    printf("\nDequeued : %d", itemsQ1[frontQ1]);
    frontQ1++;
    if (frontQ1 > rearQ1)
      frontQ1 = rearQ1 = -1;
  }
}

// enqueue2
void enQueue2(int value) {
  if (rearQ2 == SIZE - 1)
    printf("\nQueue is Full!!");
  else {
    if (frontQ2 == -1)
      frontQ2 = 0;
    rearQ2++;
    itemsQ2[rearQ2] = value;
   // printf("\nInserted -> %d", value);
	
  }
}
// dequeue2
void deQueue2() {
  if (frontQ2 == -1) {}

  else {
    printf("\nDequeued : %d", itemsQ2[frontQ2]);
    frontQ2++;
    if (frontQ2 > rearQ2)
      frontQ2 = rearQ2 = -1;
  }
}
// enqueue3
void enQueue3(int value) {
  if (rearQ3 == SIZE - 1)
    printf("\nQueue is Full!!");
  else {
    if (frontQ3 == -1)
      frontQ3 = 0;
    rearQ3++;
    itemsQ3[rearQ3] = value;
   // printf("\nInserted -> %d", value);
	
  }
}
// dequeue3
void deQueue3() {
  if (frontQ3 == -1) {}

  else {
    printf("\nDequeued : %d", itemsQ3[frontQ3]);
    frontQ3++;
    if (frontQ3 > rearQ3)
      frontQ3 = rearQ3 = -1;
  }
}
// Function to print the queue
void display() {
  if (rearQ1 == -1) {}
  else {
    int i;
    printf("\nQueued elements in queue 1 are:\n");
    for (i = frontQ1; i <= rearQ1; i++)
      printf("%d  ", itemsQ1[i]);
	  int numberOfElementsInQueue1 = i;
	 // printf("\nNumber of elements in Queue 1 equals: %d", numberOfElementsInQueue1);
  }
  printf("\n");
}
void display2() {
  if (rearQ2 == -1) {}
  else {
    int i;
    printf("\nQueued elements in queue 2 are:\n");
    for (i = frontQ2; i <= rearQ2; i++)
      printf("%d  ", itemsQ2[i]);
	  int numberOfElementsInQueue2 = i;
	//  printf("\nNumber of elements in Queue 2 equals: %d", numberOfElementsInQueue2);
  }
  printf("\n");
}
void display3() {
  if (rearQ3 == -1) {}
  else {
    int i;
    printf("\nQueued elements in queue 3 are:\n");
    for (i = frontQ3; i <= rearQ3; i++)
      printf("%d  ", itemsQ3[i]);
	  int numberOfElementsInQueue3 = i;
	 // printf("\nNumber of elements in Queue 3 equals: %d", numberOfElementsInQueue3); 
  }
  printf("\n");
}
void ProcessPacket(unsigned char* Buffer, int Size)
{
	// generacja wskaznika iph
	struct iphdr *iph = (struct iphdr*)Buffer;
	++total;
	switch (iph->protocol) //Check the Protocol and do accordingly...
	{
		case 1:  //ICMP Protocol
			++icmp;
			//PrintIcmpPacket(Buffer,Size);
			//checkIncomingPackets();
			print_icmp_packet(Buffer, Size);

		
		default: //Some Other Protocol like ARP etc.
			++others;
			break;
	}
}


void print_ip_header(unsigned char* Buffer, int Size)
{
	unsigned short iphdrlen;
	unsigned char tosbits;
	
	struct iphdr *iph = (struct iphdr *)Buffer;
	iphdrlen =iph->ihl*4;
	
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iph->saddr;
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iph->daddr;


	tosbits = iph->tos;
	fprintf(logfile,"\n");
	fprintf(logfile,"IP Header\n");
	fprintf(logfile,"   |-IP Version        : %d\n",(unsigned int)iph->version);
	fprintf(logfile,"   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
	fprintf(logfile,"   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
	fprintf(logfile,"   |-The ToS bits are [DSCP]  : %d\n",tosbits);
	fprintf(logfile,"   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
	fprintf(logfile,"   |-Identification    : %d\n",ntohs(iph->id));

	fprintf(logfile,"   |-TTL      : %d\n",(unsigned int)iph->ttl);
	fprintf(logfile,"   |-Protocol : %d\n",(unsigned int)iph->protocol);
	fprintf(logfile,"   |-Checksum : %d\n",ntohs(iph->check));
	fprintf(logfile,"   |-Source IP             : %s\n",inet_ntoa(source.sin_addr));
	fprintf(logfile,"   |-Destination IP        : %s\n",inet_ntoa(dest.sin_addr));


}

void print_icmp_packet(unsigned char* Buffer , int Size)
{

	unsigned short iphdrlen;
	
	struct iphdr *iph = (struct iphdr *)Buffer;
	iphdrlen = iph->ihl*4;
	
	struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen);
			
	fprintf(logfile,"\n\n***********************ICMP Packet*************************\n");	
	
	print_ip_header(Buffer , Size);
			
	fprintf(logfile,"\n");
		

	print_icmp_packet_address(Buffer, Size);
	fprintf(logfile,"\n###########################################################");
}

void PrintDataIcmp (unsigned char* data , int Size)
{
			int counter = 1;
	for(i=0 ; i < Size ; i++)

	{

		if( i!=0 && i%16==0)   //if one line of hex printing is complete...
		{
			fprintf(logfile,"         ");
			for(j=i-16 ; j<i ; j++)
			{
				if(data[j]>=32 && data[j]<=128) {
					fprintf(logfile,"%c",(unsigned char)data[j]); //if its a number or alphabet
				}

				
				else fprintf(logfile,"."); //otherwise print a dot
			}
			    

			fprintf(logfile,"\n");
		} 
		
		if(i%16==0) fprintf(logfile,"   ");
			fprintf(logfile," %02X",(unsigned int)data[i]);


		if(counter == Size) { // which byte is DSCP
		// dscp
	
		//fprintf(logfile,"[DSCP: 0x%02X]",(unsigned int)data[i+1]); // break przed samym polem DSCP
		int nval = (unsigned int)data[1];
		fprintf(logfile,"\nDSCP field: 0x0%d\n", nval);
		//break; // break przed samym polem DSCP
		
		//	
		}

		
		if( i==Size-1)  //print the last spaces
		{
			for(j=0;j<15-i%16;j++) fprintf(logfile,"   "); //extra spaces
			
			fprintf(logfile,"         ");
			
			for(j=i-i%16 ; j<=i ; j++)
			{
				if(data[j]>=32 && data[j]<=128) fprintf(logfile,"%c",(unsigned char)data[j]);
				
				else fprintf(logfile,".");
			}
			fprintf(logfile,"\n");
		}
	counter++;
	int counter = counter - 82;
	//fprintf(logfile,"\nDSCP field: 0x0%d", nval);
	}


}


void PrintData (unsigned char* data , int Size)
{
	
	for(i=0 ; i < Size ; i++)
	{
		if( i!=0 && i%16==0)   //if one line of hex printing is complete...
		{
			fprintf(logfile,"         ");
			for(j=i-16 ; j<i ; j++)
			{
				if(data[j]>=32 && data[j]<=128)
					fprintf(logfile,"%c",(unsigned char)data[j]); //if its a number or alphabet
				
				else fprintf(logfile,"."); //otherwise print a dot
			}
			fprintf(logfile,"\n");
		} 
		
		if(i%16==0) fprintf(logfile,"   ");
			fprintf(logfile," %02X",(unsigned int)data[i]);
				
		if( i==Size-1)  //print the last spaces
		{
			for(j=0;j<15-i%16;j++) fprintf(logfile,"   "); //extra spaces
			
			fprintf(logfile,"         ");
			
			for(j=i-i%16 ; j<=i ; j++)
			{
				if(data[j]>=32 && data[j]<=128) fprintf(logfile,"%c",(unsigned char)data[j]);
				else fprintf(logfile,".");
			}
			fprintf(logfile,"\n");
		}

	}
	unsigned short iphdrlen;
	unsigned char tosbits;


}

int cntQ1 = 0;
int cntQ2 = 0;
int cntQ3 = 0;

int packet_counter = -1;
int key = 0x1234;
void print_icmp_packet_address(unsigned char* Buffer , int Size) {
	struct packet arr_packet[1024]; // 1024 pakekiey, kazdy z nich ma membery 2: id oraz data (czyli dane)

	int i; 
    int addressesMatrixForSharedMemory[1][BUF_SIZE];
	int segment_id;
	unsigned char tosbits;	
	struct iphdr *iph = (struct iphdr *)Buffer;
	tosbits = iph->tos;
    int bytes[Size]; // size of packet + 1

	//for (m =1; m< 5; m++) {
  	for (i =0; i<Size; i++) {
	 //fprintf(logfile, "Buffer array element number: %d", i)	
	 fprintf(logfile, "\n");
	 fprintf (logfile,"Buffer arr element no: %d \n", i);	  
     fprintf(logfile,"Virtual memory address: %p\n", &Buffer[i]);
     char *b;
     b = &Buffer[i];
	 //fprintf(logfile, "\n");
	 fprintf(logfile, "Value stored at virtual memory address %p is %d [in DEC] ", b, *b);
	 fprintf(logfile, "\n");
     //fprintf(logfile, "This is BUFF: %s",b); // string literal, not a variable error fix
     printf("\n");

	 ////////////////////// writing /////////////////////

	char * shared_memory;
	int segment_size;
	const int shared_segment_size = BUF_SIZE;

	/* Allocate a shared memory segment.  */
	segment_id = shmget (key, shared_segment_size, IPC_CREAT );
	/* Attach the shared memory segment.  */
	printf("Shared memory segment ID is %d\n", segment_id);
	shared_memory = (char*) shmat (segment_id, 0, 0);
	/* Write a string to the shared memory segment.  */
	sprintf (shared_memory, b);
	// reading from shared memory segment (address)
	printf("Value stored at shared memory address %p is %d\n", shared_memory, *b);
    // zebranie wartosci to tablicy jednowymiarowej bytes
    bytes[i] = *b;
    //addressesMatrixForSharedMemory [i] = *b;
    printf("Shared memory address at which the value: %d [in DEC] is being stored is: %p\n", *b, shared_memory);
	// reading from shared memory segment (based on key)
	//printf("Value stored at shared memory unique key %d is %d\n", key ,*b);
    // reading 1 byte from shared memory having segment ID and unique key to it
	printf("Shared memory key %d is permanently combined with segment ID %d\n", key ,segment_id);
	// reading class of service from the packet
	printf("Class of service is: %d for the packet in segment %d\n", tosbits, segment_id);
    int byteNo = i+1;
    printf("Byte no: %d in segment: %d", i+1, segment_id);
	/* Detach the shared memory segment.  */
	//shmdt (shared_memory);
	printf("\nWrote buffer content to the segment\n");
    if (byteNo == Size){
        key++;
		packet_counter++;
    }
	printf("packet counter is %d\n", packet_counter +1);
	sharedMemoryIDs[packet_counter] = segment_id;
	}
	printf("\nRead bytes from member 'data' of the nth element of the array:...\n");
	for (int i =0; i<Size; i++) {
		 arr_packet[packet_counter+1].data[i] = bytes[i];
		 arr_packet[packet_counter+1].id[0] = segment_id;
		 printf("%X ", arr_packet[packet_counter+1].data[i]);
    }
	printf("\nRead from 'id' member of the nth element of the array : \n%d ", arr_packet[packet_counter+1].id[0]);


	printf("\nsharedMemoryIDs[%d]: %d\n",packet_counter, sharedMemoryIDs[packet_counter]);

	int buflen = 84;// packet size
	char* bufferToSend;
	bufferToSend = (char*)malloc(buflen);

	if (tosbits == 1) {
	enQueue(sharedMemoryIDs[packet_counter]);
    cntQ1++;
	display();
	} else if (tosbits == 2 ) {
	enQueue2(sharedMemoryIDs[packet_counter]);	
    cntQ2++;
	display2();
	} else {
	enQueue3(sharedMemoryIDs[packet_counter]);
    cntQ3++;	
	display3();
	}
    

	 int granularity = 5; // co ile pakietow dekolejkowanie
		//mechanizm ktora kolejka ma byc obsluzona pierwsza
		// najpierw pakiety z class = 1 wyciagane z kolejki
		// potem pakiety z klasa 2 wyciagane z kolejki
		if (packet_counter + 1 == granularity) { // jak juz wszystkie pakiety przeslane

		struct sockaddr_in sin;
		int len = sizeof(dest);
		int mysock = socket(AF_INET, SOCK_DGRAM, 0);
		sin.sin_port = htons(2000);
		sin.sin_addr.s_addr =   packet.ipAddress.destinationIP;
		sin.sin_family = AF_INET;
		int num = 84;

		char dataPreparedToBeSent[1024];

		if (frontQ1 != -1) { // jesli jest cos w kolejce z class = 1 to najpierw z tej sciagamy pakiety

		  for (int sizeOfItemsQueue1 =0 ; sizeOfItemsQueue1<cntQ1; sizeOfItemsQueue1++) {

			// write to a buffer the data from dequeued segmend ID
			for (int i = 1; i<= granularity; i++){
				if (arr_packet[i].id[0] == itemsQ1[frontQ1]) {
					printf("\nNumer pakietu: %d", i);
					for (int k = 0; k< Size+1; k++){
					printf("\nDATA TO BE WRITTEN: %d", arr_packet[i].data[k]);
					dataPreparedToBeSent[k] = arr_packet[i].data[k];
				//	strcpy(bufferToSend, "To ja Michal!!!");
					}
					memcpy (bufferToSend, dataPreparedToBeSent, Size);
	                strcpy (packet.ipAddress.destinationIP, inet_ntoa(dest.sin_addr));
			        strcpy (packet.ipAddress.sourceIP, inet_ntoa(source.sin_addr));
					//printf("\nSize of bufferToSend: %x", buflen);
				}
					int iResult = sendto(mysock, bufferToSend, buflen, 0, (struct sockaddr_in*) &sin, sizeof(sin));
			}
	        deQueue();
		  	display();
		  } // koniec sciagania z class 1
	


	if (frontQ2 != -1) { // jesli jest cos w kolejce class 2 to z niej
	  		for (int sizeOfItemsQueue2 =0 ; sizeOfItemsQueue2 < cntQ2; sizeOfItemsQueue2++) {  
		// write to a buffer the data from dequeued segmend ID
		for (int i = 1; i<= granularity; i++){
			if (arr_packet[i].id[0] == itemsQ2[frontQ2]) {
				printf("\nNumer pakietu: %d", i);
				for (int k = 0; k< Size; k++){
				//printf("\nDATA TO BE WRITTEN: %d", arr_packet[i].data[k]);
				memcpy(bufferToSend, (char*)&arr_packet[i].data[k], sizeof(int));
				}
                strcpy (packet.ipAddress.destinationIP, inet_ntoa(dest.sin_addr));
		        strcpy (packet.ipAddress.sourceIP, inet_ntoa(source.sin_addr));
			}
		}
				deQueue2();
	  			display2();
	 		 } // koniec sciagania z class 2

	  		if (frontQ3 != -1) { // jesli jest cos w class 3 to z niej
	 		 for (int sizeOfItemsQueue3 =0 ; sizeOfItemsQueue3<cntQ3; sizeOfItemsQueue3++) {  

				for (int i = 1; i<= granularity; i++){
			if (arr_packet[i].id[0] == itemsQ3[frontQ3]) {
				printf("\nNumer pakietu: %d", i);
				for (int k = 0; k< Size; k++){
				//printf("\nDATA TO BE WRITTEN: %d", arr_packet[i].data[k]);
				memcpy(bufferToSend, (char*)&arr_packet[i].data[k], sizeof(int));
				}
                strcpy (packet.ipAddress.destinationIP, inet_ntoa(dest.sin_addr));
		        strcpy (packet.ipAddress.sourceIP, inet_ntoa(source.sin_addr));
			}
		}  
				deQueue3();
	  			display3();
	  		} // koniec sciagania z class 3
	  		} else {}

	} else if (frontQ2 == -1) { // jesli nic nie ma class 2
	  	if (frontQ3 != -1) { // jesli jest cos w class 3 to z niej
	  for (int sizeOfItemsQueue3 =0 ; sizeOfItemsQueue3<cntQ3; sizeOfItemsQueue3++) {  
		  for (int i = 1; i<= granularity; i++){
			if (arr_packet[i].id[0] == itemsQ3[frontQ3]) {
				printf("\nNumer pakietu: %d", i);
				for (int k = 0; k< Size; k++){
				//printf("\nDATA TO BE WRITTEN: %d", arr_packet[i].data[k]);
				memcpy(bufferToSend, (char*)&arr_packet[i].data[k], sizeof(int));
				}
                strcpy (packet.ipAddress.destinationIP, inet_ntoa(dest.sin_addr));
		        strcpy (packet.ipAddress.sourceIP, inet_ntoa(source.sin_addr));
			}
		}
		deQueue3();
	  	display3();
	  } // koniec sciagania z class 3
	  } else {}
	}


	} else if (frontQ1 == -1) { // jesli nie ma nic w kolejce z class 1
	 	if (frontQ2 != -1) { // jesli jest cos w kolejce class 2 to z niej
	  		for (int sizeOfItemsQueue2 =0 ; sizeOfItemsQueue2<cntQ2; sizeOfItemsQueue2++) {  
				for (int i = 1; i<= granularity; i++){
				if (arr_packet[i].id[0] == itemsQ2[frontQ2]) {
					printf("\nNumer pakietu: %d", i);
				for (int k = 0; k< Size; k++){
				//printf("\nDATA TO BE WRITTEN: %d", arr_packet[i].data[k]);
				memcpy(bufferToSend, (char*)&arr_packet[i].data[k], sizeof(int));
				}
                    strcpy (packet.ipAddress.destinationIP, inet_ntoa(dest.sin_addr));
		            strcpy (packet.ipAddress.sourceIP, inet_ntoa(source.sin_addr));
				}
		}
				deQueue2();
	  			display2();
	 		 } // koniec sciagania z class 2
        if (frontQ3 != -1) { // jesli jest cos w class 3 to z niej
	 		 for (int sizeOfItemsQueue3 =0 ; sizeOfItemsQueue3<cntQ3; sizeOfItemsQueue3++) {  
				for (int i = 1; i<= granularity; i++){
				if (arr_packet[i].id[0] == itemsQ3[frontQ3]) {
				printf("\nNumer pakietu: %d", i);
				for (int k = 0; k< Size; k++){
				//printf("\nDATA TO BE WRITTEN: %d", arr_packet[i].data[k]);
				memcpy(bufferToSend, (char*)&arr_packet[i].data[k], sizeof(int));
				}				strcpy (packet.ipAddress.destinationIP, inet_ntoa(dest.sin_addr));
				strcpy (packet.ipAddress.sourceIP, inet_ntoa(source.sin_addr));
				}
			}
				deQueue3();
	  			display3();
	  		} // koniec sciagania z class 3
	  		} else {}

	 	} else if (frontQ2 == -1) { // jesli nic nie ma class 2
	  	if (frontQ3 != -1) { // jesli jest cos w class 3 to z niej
	  		for (int sizeOfItemsQueue3 =0 ; sizeOfItemsQueue3<cntQ3; sizeOfItemsQueue3++) {  
		  		for (int i = 1; i<= granularity; i++){
				if (arr_packet[i].id[0] == itemsQ3[frontQ3]) {
					printf("\nNumer pakietu: %d", i);
				for (int k = 0; k< Size; k++){
				//printf("\nDATA TO BE WRITTEN: %d", arr_packet[i].data[k]);
				memcpy(bufferToSend, (char*)&arr_packet[i].data[k], sizeof(int));
				}                    strcpy (packet.ipAddress.destinationIP, inet_ntoa(dest.sin_addr));
		            strcpy (packet.ipAddress.sourceIP, inet_ntoa(source.sin_addr));
				}
			}
		deQueue3();
	  	display3();
	  } // koniec sciagania z class 3
	  } else {}
	}

	}

    packet_counter = packet_counter - granularity;
	}



    struct shmid_ds shm_desc;
    /* destroy the shared memory segment. */
    if (shmctl(segment_id, IPC_RMID, &shm_desc) == -1) {
        perror("main: shmctl: ");
    }
}

int icmpPacketCounter = 1;
void incremento(int *n){
  (*n)++;
}

void on_switch_emulator_destroy() //window closing callback
{
	gtk_main_quit();
}


