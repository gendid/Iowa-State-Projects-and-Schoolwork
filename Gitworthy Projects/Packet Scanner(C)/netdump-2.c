//Modified by Will Springer, SID Gendid, Spring 2020 4/24/2020
#define RETSIGTYPE void
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <pcap.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef setsignal_h
#define setsignal_h

RETSIGTYPE (*setsignal(int, RETSIGTYPE (*)(int)))(int);
#endif
//Prototypes
void printARP(const u_char *p, int length);
void printIP(const u_char *p, int length);
void printTCP(const u_char *p, int length, int ipop);
void printICMP(const u_char *p, int length, int ipop);
void printSMTP(const u_char *p, int length, int ipop);
void printPOP(const u_char *p, int length, int ipop);
void printIMAP(const u_char *p, int length, int ipop);
void printHTTP(const u_char *p, int length, int ipop);

//vars for counting types
int arpcount=0, ipcount=0, tcpcount=0, icmpcount=0, dnscount=0, smtpcount=0, popcount=0, imapcount=0, httpcount=0;


char cpre580f98[] = "netdump";

void raw_print(u_char *user, const struct pcap_pkthdr *h, const u_char *p);

int packettype;

char *program_name;

/* Externs */
extern void bpf_dump(const struct bpf_program *, int);

extern char *copy_argv(char **);

/* Forwards */
 void program_ending(int);

/* Length of saved portion of packet. */
int snaplen = 1500;;

static pcap_t *pd;

extern int optind;
extern int opterr;
extern char *optarg;
int pflag = 0, aflag = 0;

int
main(int argc, char **argv)
{
	int cnt, op, i, done = 0;
	bpf_u_int32 localnet, netmask;
	char *cp, *cmdbuf, *device;
	struct bpf_program fcode;
	 void (*oldhandler)(int);
	u_char *pcap_userdata;
	char ebuf[PCAP_ERRBUF_SIZE];

	cnt = -1;
	device = NULL;
	
	if ((cp = strrchr(argv[0], '/')) != NULL)
		program_name = cp + 1;
	else
		program_name = argv[0];

	opterr = 0;
	while ((i = getopt(argc, argv, "pa")) != -1)
	{
		switch (i)
		{
		case 'p':
			pflag = 1;
		break;
		case 'a':
			aflag = 1;
		break;
		case '?':
		default:
			done = 1;
		break;
		}
		if (done) break;
	}
	if (argc > (optind)) cmdbuf = copy_argv(&argv[optind]);
		else cmdbuf = "";

	if (device == NULL) {
		device = pcap_lookupdev(ebuf);
		if (device == NULL)
			error("%s", ebuf);
	}
	pd = pcap_open_live(device, snaplen,  1, 1000, ebuf);
	if (pd == NULL)
		error("%s", ebuf);
	i = pcap_snapshot(pd);
	if (snaplen < i) {
		warning("snaplen raised from %d to %d", snaplen, i);
		snaplen = i;
	}
	if (pcap_lookupnet(device, &localnet, &netmask, ebuf) < 0) {
		localnet = 0;
		netmask = 0;
		warning("%s", ebuf);
	}
	/*
	 * Let user own process after socket has been opened.
	 */
	setuid(getuid());

	if (pcap_compile(pd, &fcode, cmdbuf, 1, netmask) < 0)
		error("%s", pcap_geterr(pd));
	
	(void)setsignal(SIGTERM, program_ending);
	(void)setsignal(SIGINT, program_ending);
	/* Cooperate with nohup(1) */
	if ((oldhandler = setsignal(SIGHUP, program_ending)) != SIG_DFL)
		(void)setsignal(SIGHUP, oldhandler);

	if (pcap_setfilter(pd, &fcode) < 0)
		error("%s", pcap_geterr(pd));
	pcap_userdata = 0;
	(void)fprintf(stderr, "%s: listening on %s\n", program_name, device);
	if (pcap_loop(pd, cnt, raw_print, pcap_userdata) < 0) {
		(void)fprintf(stderr, "%s: pcap_loop: %s\n",
		    program_name, pcap_geterr(pd));
		exit(1);
	}
	pcap_close(pd);
	exit(0);
}

/* routine is executed on exit */
void program_ending(int signo)
{
	struct pcap_stat stat;
	
	//Print the counts
	printf("IP Count = %d\n", ipcount);
	printf("ARP Count = %d\n", arpcount);
	printf("TCP count = %d\n", tcpcount);
	printf("ICMP count = %d\n", icmpcount);
	printf("DNS count = %d\n", dnscount);
	printf("SMTP count = %d\n", smtpcount);
	printf("POP count = %d\n", popcount);
	printf("IMAP count = %d\n", imapcount);
	printf("HTTP count = %d\n", httpcount);
	
	if (pd != NULL && pcap_file(pd) == NULL) {
		(void)fflush(stdout);
		putc('\n', stderr);
		if (pcap_stats(pd, &stat) < 0)
			(void)fprintf(stderr, "pcap_stats: %s\n",
			    pcap_geterr(pd));
		else {
			(void)fprintf(stderr, "%d packets received by filter\n",
			    stat.ps_recv);
			(void)fprintf(stderr, "%d packets dropped by kernel\n",
			    stat.ps_drop);
		}
	}

	exit(0);
}

/* Like default_print() but data need not be aligned */
void
default_print_unaligned(register const u_char *cp, register u_int length)
{
	register u_int i, s;
	register int nshorts;

	nshorts = (u_int) length / sizeof(u_short);
	i = 0;
	while (--nshorts >= 0) {
		if ((i++ % 8) == 0)
			(void)printf("\n\t\t\t");
		s = *cp++;
		(void)printf(" %02x%02x", s, *cp++);
	}
	if (length & 1) {
		if ((i % 8) == 0)
			(void)printf("\n\t\t\t");
		(void)printf(" %02x", *cp);
	}
}

/*
 * By default, print the packet out in hex.
 */
void
default_print(register const u_char *bp, register u_int length)
{
	register const u_short *sp;
	register u_int i;
	register int nshorts;

	if ((long)bp & 1) {
		default_print_unaligned(bp, length);
		return;
	}
	sp = (u_short *)bp;
	nshorts = (u_int) length / sizeof(u_short);
	i = 0;
	while (--nshorts >= 0) {
		if ((i++ % 8) == 0)
			(void)printf("\n\t");
		(void)printf(" %04x", ntohs(*sp++));
	}
	if (length & 1) {
		if ((i % 8) == 0)
			(void)printf("\n\t");
		(void)printf(" %02x", *(u_char *)sp);
	}
}

/*
insert your code in this routine

*/

void raw_print(u_char *user, const struct pcap_pkthdr *h, const u_char *p)
{
        u_int length = h->len;
        u_int caplen = h->caplen;
	uint16_t etype;
	etype= p[12] * 256 + p[13]; 

        default_print(p, caplen);
	//Start Decoding Etherenet Header
	printf("\n\n----Decoding Etherenet Header----\n");
	printf("Destination Address = %02X:%02X:%02X:%02X:%02X:%02X\n", p[0], p[1], p[2], p[3], p[4], p[5]);
        printf("Source Address = %02X:%02X:%02X:%02X:%02X:%02X\n", p[6], p[7], p[8], p[9], p[10], p[11]);
	printf("Ethernet Type = 0x%04X\n", etype);
	if(etype == 0x0800)
	  {
	    printf("Payload = IPV4\n");
	  }
	if(etype == 0x0806)
	  {
	    printf("Payload = ARP\n");
	    arpcount++;
	  }
	printf("----End Ethernet Decoding----\n");
	if(etype==0x806)
	  {
	    printARP(p,length);
	  }
	else
	  {
	    printIP(p,length);
	  }

	//	printf("\n\n caplen = %d, length = %d\n", caplen, length);
        putchar('\n');
}

/* Prints the ARP header information */
void printARP(const u_char *p, int length)
{
  int i;
  uint16_t htype, ptype, op;
  htype=p[14] * 256 + p[15];
  ptype=p[16] * 256 + p[17];
  op=p[20] * 256 + p[21];
  
  printf("\t----Begin ARP Decoding----\n");
  if(htype==0x0001)
    {
      printf("\tHardware Type = 0x%04X -> Ethernet\n", htype);
    }
  else
    {
      printf("\tHardware Type= 0x%04X\n", htype);
    }
  if(ptype==0x0800)
    {
      printf("\tProtocol = 0x%04X -> IP\n", ptype);
    }
  else
    {
      printf("\tProtocol = 0x%04X\n", ptype);
    }
  if(p[18]==0x06)
    {
      printf("\tHardware Address Field Length = %d bytes -> Ethernet\n", p[18]);
    }
  else
    {
      printf("\tHardware Address Field Length = %d bytes\n", p[18]);
    }
  if(p[19]==0x04)
    {
      printf("\tProtocol Length = %d bytes -> IPv4\n", p[19]);
    }
  else
    {
      printf("\tProtocol Length = %d bytes\n", p[19]);
    }
  if(op==0x0001)
    {
      printf("\tOperation = Request\n");
    }
  else
    {
      printf("\tOperation = Reply\n");
    }
  printf("\tSender Hardware Address = %02X:%02x:%02X:%02X:%02X:%02X\n", p[20], p[21], p[22], p[23], p[24], p[25]);
  printf("\tSender IP Address = %d.%d.%d.%d\n", p[26], p[27], p[28], p[29]);
  printf("\tTarget Hardware Address = %02X:%02x:%02X:%02X:%02X:%02X\n", p[30], p[31], p[32], p[33], p[34], p[35]);
  printf("\tTarget IP Address = %d.%d.%d.%d\n", p[36], p[37], p[38], p[39]);
  printf("\tPayload : ");
  for (i=40; i<length;i++)
    {
      printf("%02X ", p[i]);
    }
  printf("\n");
  
  printf("\t----End of ARP Header----\n");

  
}

/* Prints the IP header information */
void printIP(const u_char *p, int length)
{
  int i, o=0;
  uint16_t plength, id, offset, checksum, dnscheckforward, dnscheckback;
  plength=p[16] * 256 + p[17];
  id=p[18] * 256 + p[19];
  offset=p[20] * 256 + p[21];
  checksum=p[24] * 256 + p[25];
  dnscheckforward=p[34] * 256 + p[35];
  dnscheckback=p[36] * 256 + p[37];
  
  
  printf("\t----Begin IP Header Decode----\n");
  printf("\tVersion Number = %d\n", p[14] >> 4);
  printf("\tHeader Length = %d\n", p[14] & 0b00001111);
  printf("\tType of Service = 0x%04X\n", p[15]);
  printf("\tPayload Length = %d bytes\n", plength);
  printf("\tID = 0x%04X\n", id);
  printf("\tFlags = %d%d%d\n", (p[20] & 0b10000000) >> 7, (p[20] & 0b01000000) >> 6, (p[20] & 0b00100000) >> 5);
  if (((p[20] & 0b01000000) >> 6) == 1)
    {
      printf("\t\tDo Not Fragment\n");
    }
  if(((p[20] & 0b00100000) >> 5) == 1)
    {
      printf("\t\tMore Fragments Coming\n");
    }
  printf("\tOffset = %d bytes\n", offset & 0b0001111111111111);
  printf("\tTime to Live = %d hops\n", p[22]);
  if(p[23] == 0x01)
    {
      printf("\tProtocol = 1 -> ICMP\n");
    }
  else if(p[23] == 0x06)
    {
      printf("\tProtocol = 6 -> TCP\n");
    }
 else if(p[23] == 0x11)
    {
      printf("\tProtocol = 17 -> UDP\n");
      //Checking for DNS by looking for port 53 on a UDP packet. 53 is the DNS port.
      if(((int) dnscheckforward == 53)||((int) dnscheckback == 53))
	{
	  printf("\tDNS DETECTED!!!!!\n");
	  ipcount--;
	  dnscount++;
	}
    }
 else
   {
     printf("\tProtocol = %d\n", p[23]);
   }
  printf("\tChecksum = 0x%04X\n", checksum);
  printf("\tSource IP = %d.%d.%d.%d\n", p[26], p[27], p[28], p[29]);
  printf("\tDestination IP = %d.%d.%d.%d\n", p[30], p[31], p[32], p[33]);
  if((int)(p[14] & 0b00001111) == 5)
    {
      printf("\tNo Options\n");
    }
  else
    {
      printf("\tOptions = 0x%02X%02X\n", p[34], p[35]);
      o+=2;
    }
  if ((p[23] != 0x01)&&(p[23] != 0x06))
    {
      printf("\tPayload = 0x");
      for(i=34+o; i<length; i++)
	{
	  printf("%02X ", p[i]);
	}
      printf("\n");
    }
  printf("\t----End IP Decode----\n");
  if(p[23] == 0x01)
    {
      printICMP(p, length, o);
    }
  if(p[23] == 0x06)
    {
      printTCP(p, length, o);
    }

  ipcount++;

}

/* Decodes the TCP Header */
void printTCP(const u_char *p, int length, int ipop)
{
  int i, o=0;
  uint16_t sport, dport, wsize, checksum, upoint;
  sport=p[34+ipop] * 256 + p[35+ipop];
  dport=p[36+ipop] * 256 + p[37+ipop];
  wsize=p[48+ipop] * 256 + p[49+ipop];
  checksum=p[50+ipop] * 256 + p[51+ipop];
  upoint=p[52+ipop] * 256 + p[53+ipop];
  
  printf("\t\t----Begin TCP Decode----\n");
  printf("\t\tSource Port = %d\n", sport);
  printf("\t\tDestination Port = %d\n", dport);
  printf("\t\tSequence Number = 0x");
  for(i=38+ipop; i<42+ipop; i++)
    {
      printf("%02X", p[i]);
    }
  printf("\n");
  printf("\t\tAcknowledgement Number = 0x");
  for(i=42+ipop; i<46+ipop; i++)
    {
      printf("%02X", p[i]);
    }
  printf("\n");
  printf("\t\tHeader Length = %d bytes\n", (p[46+ipop] >> 4) * 4);
  printf("\t\tFlags = %d %d %d %d %d %d\n", (p[47+ipop] & 0b00100000) >> 5,(p[47+ipop] & 0b00010000) >> 4, (p[47+ipop] & 0b00001000) >> 3, (p[47+ipop] & 0b00000100) >> 2,(p[47+ipop] & 0b00000010) >> 1,(p[47+ipop] & 0b00000001));
  if(((p[47+ipop] & 0b00100000) >> 5) == 1)
    {
      printf("\t\t\tContains Urgent Message\n");
    }
  if(((p[47+ipop] & 0b00010000) >> 4) == 1)
    {
      printf("\t\t\tAcknowledgement is Valid\n");
    }
  if(((p[47+ipop] & 0b00001000) >> 3) == 1)
    {
      printf("\t\t\tData should be pushed to application\n");
    }
  if(((p[47+ipop] & 0b00000100) >> 2) == 1)
    {
      printf("\t\t\tReset Packet\n");
    }
  if(((p[47+ipop] & 0b00000010) >> 1) == 1)
    {
      printf("\t\t\tSynchronize Packet\n");
    }
  if((p[47+ipop] & 0b00000001) == 1)
    {
      printf("\t\t\tFinish Packet\n");
    }
  printf("\t\tWindow Size = %d\n", wsize);
  printf("\t\tChecksum = 0x%04X\n", checksum);
  printf("\t\tUrgent Pointer = 0x%04X\n", upoint);
  if(((p[46+ipop] >> 4)*4)==20)
    {
      printf("\t\tNo Options\n");
    }
  else
    {
      o=(int) (((p[46+ipop] >> 4)*4));
      o= (o-20)/2;
      printf("\t\tOptions = 0x");
      for(i=54+ipop; i<54+ipop+o; i++)
	{
	  printf("%02X", p[i]);
	}
      printf("\n");
    }
  //Check for SMTP
  if((sport==25)||(dport==25))
    {
      printf("\t\t----TCP Header Complete----\n");
      printSMTP(p, length, ipop+o);

    }
  //Check for POP
  else if((sport==110)||(dport==110))
    {
      printf("\t\t----TCP Header Complete----\n");
      printPOP(p, length, ipop+o);
    }
  //Check for IMAP
  else if((sport==143)||(dport==143))
    {
      printf("\t\t----TCP Header Complete----\n");
      printIMAP(p, length, ipop+o);
    }
  //Check for HTTP
  else if((sport==80)||(dport==80))
    {
      printf("\t\t----TCP Header Complete----\n");
      printHTTP(p, length, ipop+o);
    }
  //It's just TCP
  else
    {
      printf("\t\tPayload = 0x");
      for(i=54+ipop+o; i<length; i++)
	{
	  printf("%02X", p[i]);
	}
      printf("\n");
      printf("\t\t----TCP Header Complete----\n");
      if((sport==53)||(dport==53)) //Detect DNS via the port
	{
	  printf("\t\tDNS DETECTED!!!!\n");
	  tcpcount--;
	  ipcount--;
	  dnscount++;
	}
      tcpcount++;
    }
}

/* Prints out ICMP Decoded */
void printICMP(const u_char *p, int length, int ipop)
{

  uint16_t id, checksum;
  int type= (int) p[34+ipop], code=(int) p[35+ipop], i=0;
  id=p[38+ipop] *256 + p[39+ipop];
  checksum=p[36+ipop] *256 + p[37+ipop];
  
  printf("\t\t----Begin ICMP Decode----\n");

  switch(type)
    {
    case 0: //Echo Reply
      printf("\t\tType = 0 -> Reply\n");
      printf("\t\tCode = %d\n", p[35+ipop]);
      printf("\t\tChecksum = 0x%04X\n", checksum);
      printf("\t\tID = %04X\n", id);
      printf("\t\tSequence Number = 0x%02X%02X\n", p[40+ipop], p[41+ipop]);
      printf("\t\tData = 0x");
      for(i=42+ipop; i<length; i++)
	{
	  printf("%02X", p[i]);
	}
      printf("\n");
      break;
    case 8: //Echo Request
      printf("\t\tType = 8 -> Request\n");
      printf("\t\tCode = %d\n", p[35+ipop]);
      printf("\t\tChecksum = 0x%04X\n", checksum);
      printf("\t\tID = %04X\n", id);
      printf("\t\tSequence Number = 0x%02X%02X\n", p[40+ipop], p[41+ipop]);
      printf("\t\tData = 0x");
      for(i=42+ipop; i<length; i++)
	{
	  printf("%02X", p[i]);
	}
      printf("\n");
      break;
    case 13: //Timestamp Request
      printf("\t\tType = 13 ->Timestamp Request\n");
      printf("\t\tCode = %d\n", p[35+ipop]);
      printf("\t\tChecksum = 0x%04X\n", checksum);
      printf("\t\tID = %04X\n", id);
      printf("\t\tSequence Number = 0x%02X%02X\n", p[40+ipop], p[41+ipop]);
      printf("\t\tOriginal Timestamp = 0x%02X%02X%02X%02X\n", p[42+ipop], p[43+ipop], p[44+ipop], p[45+ipop]);
      printf("\t\tRecieve Timestamp = 0x%02X%02X%02X%02X\n", p[46+ipop], p[47+ipop], p[48+ipop], p[49+ipop]);
      printf("\t\tTransmit Timestamp = 0x%02X%02X%02X%02X\n", p[50+ipop], p[51+ipop], p[52+ipop], p[53+ipop]);
      printf("\t\tData = 0x");
      for(i=54+ipop; i<length; i++)
	{
	  printf("%02X", p[i]);
	}
      printf("\n");
      break;
    case 14: //Timestamp Reply
      printf("\t\tType = 14 ->Timestamp Reply\n");
      printf("\t\tCode = %d\n", p[35+ipop]);
      printf("\t\tChecksum = 0x%04X\n", checksum);
      printf("\t\tID = %04X\n", id);
      printf("\t\tSequence Number = 0x%02X%02X\n", p[40+ipop], p[41+ipop]);
      printf("\t\tOriginal Timestamp = 0x%02X%02X%02X%02X\n", p[42+ipop], p[43+ipop], p[44+ipop], p[45+ipop]);
      printf("\t\tRecieve Timestamp = 0x%02X%02X%02X%02X\n", p[46+ipop], p[47+ipop], p[48+ipop], p[49+ipop]);
      printf("\t\tTransmit Timestamp = 0x%02X%02X%02X%02X\n", p[50+ipop], p[51+ipop], p[52+ipop], p[53+ipop]);
      printf("\t\tData = 0x");
      for(i=54+ipop; i<length; i++)
	{
	  printf("%02X", p[i]);
	}
      printf("\n");
      break;
    case 3: //Destination Unreachable
      printf("\t\tType = 0 -> Destination Unreachable\n");
      switch(code)
	{
	case 0:
	  printf("\t\tCode = 0 -> Network Unreachable\n");
	  break;
	case 1:
	  printf("\t\tCode = 1 -> Host Unreachable\n");
	  break;
	case 2:
	  printf("\t\tCode = 2 -> Protocol Unreachable on Target Host\n");
	  break;
	case 3:
	  printf("\t\tCode = 3 -> Port Unreachable on Target Host\n");
	  break;
	case 4:
	  printf("\t\tCode = 4 -> Fragmentation Needed and \"Don't frament\" Bit is Set\n");
	  break;
	case 5:
	  printf("\t\tCode = 5 -> Source Route Failed\n");
	  break;
	default:
	  printf("\t\tCode = %d\n", code);
	  break;
	}
       printf("\t\tChecksum = 0x%04X\n", checksum);
       printf("\t\tParameter = 0\n");
       printf("\t\tData = 0x");
       for(i=42+ipop; i<length; i++)
	{
	  printf("%02X", p[i]);
	}
      printf("\n");
      break;
    case 11: //Time Exceeded
      printf("\t\tType = 11 -> Time Exceeded\n");
      if(code)
	{
	  printf("\t\tCode = 1 -> Data Fragmenteded, not all fragments recieved\n");
	}
      else
	{
	  printf("\t\tCode = 0 -> Time to live expired, packet deleted\n");
	}
       printf("\t\tChecksum = 0x%04X\n", checksum);
       printf("\t\tParameter = 0\n");
       printf("\t\tData = 0x");
       for(i=42+ipop; i<length; i++)
	{
	  printf("%02X", p[i]);
	}
      printf("\n");
      break;
    case 5: //Redirect
      printf("\t\tType = 5 -> Redirect\n");
      switch(code)
	{
	case 0:
	  printf("\t\tCode = 0 -> Network Based Redirect\n");
	  break;
	case 1:
	  printf("\t\tCode = 1 -> Host Based Redirect\n");
	  break;
	case 2:
	  printf("\t\tCode = 2 -> Network Based Redirect of the Type of Service Specified\n");
	  break;
	case 3:
	  printf("\t\tCode = 3 -> Host Based Redirect of the Type of Service Specified\n");
	  break;
	default:
	  printf("\t\tCode = %d\n", code);
	  break;
	}
      printf("\t\tChecksum = 0x%04X\n", checksum);
      printf("\t\tParameter (IP) = %d.%d.%d.%d\n", p[42+ipop], p[43+ipop], p[44+ipop],  p[45+ipop]);
      printf("\t\tData = 0x");
      for(i=46+ipop; i<length; i++)
	{
	  printf("%02X", p[i]);
	}
      printf("\n");
      break;
    default:
      printf("\t\tType = %d\n", type);
      printf("\t\tCode = %d\n", code);
      printf("\t\tChecksum = 0x%04X\n", checksum);
      printf("\t\tParamaters and Data Unknown for this Type\n");
      break;
    }
  printf("\t\t----End ICMP Decode----\n");
  icmpcount++;


}

void printSMTP(const u_char *p, int length, int ipop)
{
  int i;
  printf("\t\t\t----Begin SMTP Decode----\n");
  printf("\t\t\tPayload= ");
  for(i=54+ipop; i<length;i++)
    {
      if((p[i]>=32)&&(p[i]<128))
	{
	  printf("%c", p[i]);
	}
    }
  printf("\n");
  printf("\t\t\t----End SMTP Decode----\n");
  smtpcount++;
}

void printPOP(const u_char *p, int length, int ipop)
{
  int i;
  printf("\t\t\t----Begin POP Decode----\n");
  printf("\t\t\tPayload= ");
  for(i=54+ipop; i<length;i++)
    {
      if((p[i]>=32)&&(p[i]<128))
	{
	  printf("%c", p[i]);
	}
    }
  printf("\n");
  printf("\t\t\t----End POP Decode----\n");
  popcount++;
}

void printIMAP(const u_char *p, int length, int ipop)
{
  int i;
  printf("\t\t\t----Begin IMAP Decode----\n");
  printf("\t\t\tPayload= ");
  for(i=54+ipop; i<length;i++)
    {
      if((p[i]>=32)&&(p[i]<128))
	{
	  printf("%c", p[i]);
	}
    }
  printf("\n");
  printf("\t\t\t----End IMAP Decode----\n");
  imapcount++;
}

void printHTTP(const u_char *p, int length, int ipop)
{
  int i;
  printf("\t\t\t----Begin HTTP Decode----\n");
  printf("\t\t\tPayload= ");
  for(i=54+ipop; i<length;i++)
    {
      if((p[i]>=32)&&(p[i]<128))
	{
	  printf("%c", p[i]);
	}
    }
  printf("\n");
  printf("\t\t\t----End HTTP Decode----\n");
  httpcount++;
}
