
#include <stdio.h>
#include <stdlib.h> /* for malloc, free, srand, rand */
#include <string.h>

/*******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional or bidirectional
   data transfer protocols (from A to B. Bidirectional transfer of data
   is for extra credit and is not required).  Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

#define BIDIRECTIONAL 0    /* change to 1 if you're doing extra credit */
/* and write a routine called B_output */

/* a "msg" is the data unit passed from layer 5 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 5 via the students transport level protocol entities.         */

struct msg {
    char data[20];
}; //message, 배열 크기 20인 data 구조체

/* a packet is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined packet structure, which all   */
/* students must follow. */

struct pkt {
    int seqnum;
    int acknum;
    int checksum;
    char payload[20];
}; //정보를 가지고 있는 패킷.

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* Here I define some function prototypes to avoid warnings */
/* in my compiler. Also I declared these functions void and */
/* changed the implementation to match */

void init(); //초기화하는 함수.
void generate_next_arrival();
void insertevent(struct event* p);

struct Sender
{
    int base;
    int next_seq;
    int window_size;
    int data_next;
    int ACKstate = 0;
    int Ack_num = 0;
    struct pkt packet_data[64];
    int expect_seq;  //기대하는 sequence가 맞는지 비교
    struct pkt packet_to_send; //send에게 보낼 packet
};

struct Sender A;

struct Receiver
{
    int base;
    int next_seq;
    int window_size;
    int data_next;
    struct pkt packet_data[64];
    int expect_seq;  //기대하는 sequence가 맞는지 비교
    struct pkt packet_to_send; //send에게 보낼 packet

};

struct Receiver B;

int CheckSum(struct pkt* packet) // checsum : 
{
    int checksum = 0; // 0 초기화
    checksum += packet->seqnum; // seqnum 더하기
    checksum += packet->acknum; // acknum 더하기
    for (int i = 0; i < 20; i++) // 배열크기 더하기 
        checksum += packet->payload[i];
    return checksum;
}

void tolayer3(int AorB, struct pkt packet);
void tolayer5(int AorB, char* datasent);
void starttimer(int AorB, float increment);
void stoptimer(int AorB);

void Send_packet_A(void)
{
    while (A.next_seq < A.data_next && A.next_seq < A.base + A.window_size)
    {
        struct pkt* packet = &A.packet_data[A.next_seq];
        if (A.ACKstate == 0)
            printf("A : Send_packet without ACK (seq = %d)\n", packet->seqnum);
        else
            printf("A : Send_packet with ACK (ACK = %d, seq = %d)\n", A.packet_to_send.acknum, packet->seqnum);

        tolayer3(0, *packet);
        if (A.base == A.next_seq)
            starttimer(0, float(15));
        A.next_seq++;
    }
}

void Send_packet_B(void)
{
    while (B.next_seq < B.data_next && B.next_seq < B.base + B.window_size)
    {
        struct pkt* packet = &B.packet_data[B.next_seq];
        printf("B : Send_packet with ACK (ACK = %d, seq = %d)\n", B.packet_to_send.acknum, packet->seqnum);
        tolayer3(1, *packet);
        if (B.base == B.next_seq)
            //starttimer(0, float(15));
        B.next_seq++;
    }
}

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
    struct pkt* packet = &A.packet_data[A.data_next];
    packet->seqnum = A.data_next; //다음 seqnum
    memcpy(packet->payload, message.data, 20); //데이터 받아오기
    packet->checksum = CheckSum(packet);
    A.data_next++;
    Send_packet_A();

    /*
    if (A.next_seq < A.base + A.window_size && A.next_seq < A.data_next)
    {
     //   if (A.base == 1)
            printf("A : Send_packet without ACK (seq = %d), %s \n", packet->seqnum, packet->payload);
    //    else
       //     printf("A : Send_packet with ACK (ACK = %d, seq = %d), %s \n", packet->acknum, packet->seqnum, packet->payload);
        if (A.next_seq == A.base) //data를 전송했을 때 부터 time start
            starttimer(0, (float)10); //start_time
        packet = &A.packet_data[A.next_seq]; //??
        tolayer3(0, *packet);
        A.next_seq++;
    }
    */
}

void B_output(struct msg message) /* need be completed only for extra credit */
{
    struct pkt* packet = &B.packet_data[A.data_next];
    packet->seqnum = A.data_next; //다음 seqnum
    memcpy(packet->payload, message.data, 20); //데이터 받아오기
    packet->checksum = CheckSum(packet);
    A.data_next++;
    Send_packet_B();
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
    if (packet.checksum != CheckSum(&packet)) //손실있는 데이터 받았을 때
    {
        printf("A_input 손실된 ACK\n");
        return; //drop
    }

    if (packet.acknum < A.base) //중복된 ACK들어왔을 때
    {
        printf("A_input 중복된 ACK \n");
        return; //drop
    }

    if (packet.seqnum != A.expect_seq)// 다른 패킷 받았을때
    {
        printf("A : not expected packet (seq = %d)\n", packet.seqnum);
        printf("A : send_packet with ACK (ACK = %d, seq = %d)\n", B.expect_seq, B.packet_to_send.seqnum);
        tolayer3(0, A.packet_to_send); // send packet
        return;
    }
    printf("A_input : ACK = %d seq =%d\n", packet.acknum, packet.seqnum);
    A.base = packet.acknum + 1; // A.base++; 가능? 
    A.ACKstate = 1;
    if ((packet.acknum + 1) == A.next_seq)//손실 없이 데이터를 주고 받았을 때
    {
        stoptimer(0); //타이머 정지
        printf("A: 확인 응답 완료 %d\n", A.base - 1);
        //Send_packet_A();
    }

    A.packet_to_send.acknum = A.expect_seq;
    A.packet_to_send.checksum = CheckSum(&A.packet_to_send);
    //printf("A : Send_packet with ACK (ACK = %d, seq = %d)\n", A.packet_to_send.acknum, A.packet_to_send.seqnum);
    //tolayer3(0, A.packet_to_send);
    A.expect_seq++;

}

void B_input(struct pkt packet) // receive packet
{
    B.packet_to_send.seqnum = B.next_seq;
    B.next_seq++;

    if (packet.checksum != CheckSum(&packet)) //패킷 손상
    {
        printf("B : packet corrupted (seq = %d)\n", packet.seqnum);
        printf("B : Send_packet with ACK (ACK = %d, seq = %d)\n", B.expect_seq, B.packet_to_send.seqnum);
        tolayer3(1, B.packet_to_send); // send packet
        return;
    }
    if (packet.seqnum != B.expect_seq) // 다른 패킷 받았을때
    {
        printf("B : not expected packet (seq = %d)\n", packet.seqnum);
        printf("B : send_packet with ACK (ACK = %d, seq = %d)\n", B.expect_seq, B.packet_to_send.seqnum);
        tolayer3(1, B.packet_to_send);
        return;
    }

    // 알맞게 패킷을 받았을때
    B.packet_to_send.acknum = B.expect_seq;
    B.packet_to_send.checksum = CheckSum(&B.packet_to_send);
    printf("B : Send_packet with ACK (ACK = %d, seq = %d)\n", B.packet_to_send.acknum, B.packet_to_send.seqnum);
    tolayer5(1, packet.payload); //layer5에 data 저장.
    tolayer3(1, B.packet_to_send);
    B.expect_seq++;

}

/* called when A's, B's timer goes off */
void A_timerinterrupt()
{
    for (int i = A.base; i < A.next_seq; ++i)
    {
        struct pkt* packet = &A.packet_data[i];
        printf("A : timerinterrupt : Send_packet with ACK (ACK = %d, seq = %d)\n", A.packet_to_send.acknum, packet->seqnum);
        tolayer3(0, *packet);
    }
    starttimer(0, float(15));
}

void B_timerinterrupt()
{
    for (int i = B.base; i < B.next_seq; ++i)
    {
        struct pkt* packet = &B.packet_data[i];
        printf("B : timerinterrupt : Send_packet with ACK (ACK = %d, seq = %d)\n", packet[i].acknum, packet->seqnum);
        tolayer3(0, *packet);
    }
    //starttimer(0, float(15));
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void A_init() //A 초기화 
{
    A.base = 1;
    A.next_seq = 1;
    A.window_size = 8;
    A.data_next = 1;
    A.expect_seq = 1;
    A.packet_to_send.seqnum = -1;
    A.packet_to_send.acknum = 0;
    memset(A.packet_to_send.payload, 0, 20);
    A.packet_to_send.checksum = CheckSum(&A.packet_to_send);
    A.ACKstate = 0;
    A.Ack_num = 999;
}

void B_init()
{
    B.base = 1;
    B.next_seq = 1;
    B.window_size = 8;
    B.data_next = 1;
    B.expect_seq = 1;
    B.packet_to_send.seqnum = -1;
    B.packet_to_send.acknum = 0;
    memset(B.packet_to_send.payload, 0, 20);
    B.packet_to_send.checksum = CheckSum(&B.packet_to_send);
}

/*****************************************************************
***************** NETWORK EMULATION CODE STARTS BELOW ***********
The code below emulates the layer 3 and below network environment:
  - emulates the tranmission and delivery (possibly with bit-level corruption
    and packet loss) of packets across the layer 3/4 interface
  - handles the starting/stopping of a timer, and generates timer
    interrupts (resulting in calling students timer handler).
  - generates message to be sent (passed from later 5 to 4)

THERE IS NOT REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, you should have
to, and you defeinitely should not have to modify
******************************************************************/
//layer3 이하의 네트워크 환경 *변경 필요 없음* *접근 필요 없음*

struct event {
    float evtime;           /* event time */
    int evtype;             /* event type code */
    int eventity;           /* entity where event occurs */
    struct pkt* pktptr;     /* ptr to packet (if any) assoc w/ this event */
    struct event* prev;
    struct event* next;
};
struct event* evlist = NULL;   /* the event list */

/* possible events: */
#define  TIMER_INTERRUPT 0
#define  FROM_LAYER5     1
#define  FROM_LAYER3     2

#define  OFF             0
#define  ON              1
#define   A    0
#define   B    1



int TRACE = 1;             /* for my debugging */
int nsim = 0;              /* number of messages from 5 to 4 so far */
int nsimmax = 0;           /* number of msgs to generate, then stop */
float time = (float)0.000;
float lossprob;            /* probability that a packet is dropped  */
float corruptprob;         /* probability that one bit is packet is flipped */
float lambda;              /* arrival rate of messages from layer 5 */
int   ntolayer3;           /* number sent into layer 3 */
int   nlost;               /* number lost in media */
int ncorrupt;              /* number corrupted by media*/

void main()
{
    struct event* eventptr;
    struct msg  msg2give;
    struct pkt  pkt2give;

    int i, j;
    /* char c; // Unreferenced local variable removed */

    init();
    A_init();
    B_init();

    while (1) {
        eventptr = evlist;            /* get next event to simulate */
        if (eventptr == NULL)
            goto terminate;
        evlist = evlist->next;        /* remove this event from event list */
        if (evlist != NULL)
            evlist->prev = NULL;
        if (TRACE >= 2) {
            //          printf("\nEVENT time: %f,", eventptr->evtime);
            //          printf("  type: %d", eventptr->evtype);
            //          if (eventptr->evtype == 0)
          //                printf(", timerinterrupt  ");
           //           else if (eventptr->evtype == 1)
            //              printf(", fromlayer5 ");
            //          else
           //               printf(", fromlayer3 ");
           //           printf(" entity: %d\n", eventptr->eventity);
        }
        time = eventptr->evtime;        /* update time to next event time */
        if (nsim == nsimmax)
            break;                        /* all done with simulation */
        if (eventptr->evtype == FROM_LAYER5) {
            generate_next_arrival();   /* set up future arrival */
            /* fill in msg to give with string of same letter */
            j = nsim % 26;
            for (i = 0; i < 20; i++)
                msg2give.data[i] = 97 + j;
            if (TRACE > 2) {
                printf("          MAINLOOP: data given to student: ");
                for (i = 0; i < 20; i++)
                    printf("%c", msg2give.data[i]);
                printf("\n");
            }
            nsim++;
            if (eventptr->eventity == A)
                A_output(msg2give);
            else
                B_output(msg2give);
        }
        else if (eventptr->evtype == FROM_LAYER3) {
            pkt2give.seqnum = eventptr->pktptr->seqnum;
            pkt2give.acknum = eventptr->pktptr->acknum;
            pkt2give.checksum = eventptr->pktptr->checksum;
            for (i = 0; i < 20; i++)
                pkt2give.payload[i] = eventptr->pktptr->payload[i];
            if (eventptr->eventity == A)      /* deliver packet by calling */
                A_input(pkt2give);            /* appropriate entity */
            else
                B_input(pkt2give);
            free(eventptr->pktptr);          /* free the memory for packet */
        }
        else if (eventptr->evtype == TIMER_INTERRUPT) {
            if (eventptr->eventity == A)
                A_timerinterrupt();
            else
                B_timerinterrupt();
        }
        else {
            printf("INTERNAL PANIC: unknown event type \n");
        }
        free(eventptr);
    }

terminate:
    printf(" Simulator terminated at time %f\n after sending %d msgsfrom layer5\n", time, nsim);
}



void init()                         /* initialize the simulator */
{
    int i;
    float sum, avg;
    float jimsrand();


    printf("-----  Stop and Wait Network Simulator Version 1.1 -------- \n\n");
    printf("Enter the number of messages to simulate: ");
    scanf("%d", &nsimmax);
    printf("Enter  packet loss probability [enter 0.0 for no loss]:");
    scanf("%f", &lossprob);
    printf("Enter packet corruption probability [0.0 for no corruption]:");
    scanf("%f", &corruptprob);
    printf("Enter average time between messages from sender's layer5 [ >0.0]:");
    scanf("%f", &lambda);
    printf("Enter TRACE:");
    scanf("%d", &TRACE);

    srand(9999);              /* init random number generator */
    sum = (float)0.0;         /* test random number generator for students */
    for (i = 0; i < 1000; i++)
        sum = sum + jimsrand();    /* jimsrand() should be uniform in [0,1] */
    avg = sum / (float)1000.0;
    if (avg < 0.25 || avg > 0.75) {
        printf("It is likely that random number generation on your machine\n");
        printf("is different from what this emulator expects.  Please take\n");
        printf("a look at the routine jimsrand() in the emulator code. Sorry. \n");
        exit(0);
    }

    ntolayer3 = 0;
    nlost = 0;
    ncorrupt = 0;

    time = (float)0.0;                    /* initialize time to 0.0 */
    generate_next_arrival();     /* initialize event list */
}

/****************************************************************************/
/* jimsrand(): return a float in range [0,1].  The routine below is used to */
/* isolate all random number generation in one location.  We assume that the*/
/* system-supplied rand() function return an int in therange [0,mmm]        */
/****************************************************************************/
float jimsrand()
{
    double mmm = RAND_MAX;   /* largest int  - MACHINE DEPENDENT!!!!!!!!   */
    float x;                   /* individual students may need to change mmm */
    x = (float)(rand() / mmm);            /* x should be uniform in [0,1] */
    return(x);
}

/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/

void generate_next_arrival()
{
    double x, log(), ceil();
    struct event* evptr;
    /* char *malloc(); // malloc redefinition removed */
    /* float ttime; // Unreferenced local variable removed */
    /* int tempint; // Unreferenced local variable removed */

    if (TRACE > 2)
        printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");

    x = lambda * jimsrand() * 2;  /* x is uniform on [0,2*lambda] */
                              /* having mean of lambda        */
    evptr = (struct event*)malloc(sizeof(struct event));
    evptr->evtime = (float)(time + x);
    evptr->evtype = FROM_LAYER5;
    if (BIDIRECTIONAL && (jimsrand() > 0.5))
        evptr->eventity = B;
    else
        evptr->eventity = A;
    insertevent(evptr);
}


void insertevent(struct event* p)
{
    struct event* q, * qold;

    if (TRACE > 2) {
        printf("            INSERTEVENT: time is %lf\n", time);
        printf("            INSERTEVENT: future time will be %lf\n", p->evtime);
    }
    q = evlist;     /* q points to header of list in which p struct inserted */
    if (q == NULL) {   /* list is empty */
        evlist = p;
        p->next = NULL;
        p->prev = NULL;
    }
    else {
        for (qold = q; q != NULL && p->evtime > q->evtime; q = q->next)
            qold = q;
        if (q == NULL) {   /* end of list */
            qold->next = p;
            p->prev = qold;
            p->next = NULL;
        }
        else if (q == evlist) { /* front of list */
            p->next = evlist;
            p->prev = NULL;
            p->next->prev = p;
            evlist = p;
        }
        else {     /* middle of list */
            p->next = q;
            p->prev = q->prev;
            q->prev->next = p;
            q->prev = p;
        }
    }
}

void printevlist()
{
    struct event* q;
    /* int i; // Unreferenced local variable removed */
    printf("--------------\nEvent List Follows:\n");
    for (q = evlist; q != NULL; q = q->next) {
        printf("Event time: %f, type: %d entity: % d\n", q->evtime, q->evtype, q->eventity);
    }
    printf("--------------\n");
}



/********************** Student-callable ROUTINES ***********************/

/* called by students routine to cancel a previously-started timer */
void stoptimer(int AorB) /* A or B is trying to stop timer */
{
    struct event* q;/* ,*qold; // Unreferenced local variable removed */

    if (TRACE > 2)
        printf("          STOP TIMER: stopping timer at %f\n", time);
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB)) {
            /* remove this event */
            if (q->next == NULL && q->prev == NULL)
                evlist = NULL;         /* remove first and only event on list */
            else if (q->next == NULL) /* end of list - there is one in front */
                q->prev->next = NULL;
            else if (q == evlist) { /* front of list - there must be event after */
                q->next->prev = NULL;
                evlist = q->next;
            }
            else {     /* middle of list */
                q->next->prev = q->prev;
                q->prev->next = q->next;
            }
            free(q);
            return;
        }
    printf("Warning: unable to cancel your timer. It wasn't running.\n");
}


void starttimer(int AorB, float increment) /* A or B is trying to stop timer */
{
    struct event* q;
    struct event* evptr;
    /* char *malloc(); // malloc redefinition removed */

    if (TRACE > 2)
        printf("          START TIMER: starting timer at %f\n", time);
    /* be nice: check to see if timer is already started, if so, then  warn */
   /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB)) {
            printf("Warning: attempt to start a timer that is already started\n");
            return;
        }

    /* create future event for when timer goes off */
    evptr = (struct event*)malloc(sizeof(struct event));
    evptr->evtime = (float)(time + increment);
    evptr->evtype = TIMER_INTERRUPT;
    evptr->eventity = AorB;
    insertevent(evptr);
}


/************************** TOLAYER3 ***************/
void tolayer3(int AorB, struct pkt packet)
{
    struct pkt* mypktptr;
    struct event* evptr, * q;
    /* char *malloc(); // malloc redefinition removed */
    float lastime, x, jimsrand();
    int i;


    ntolayer3++;

    /* simulate losses: */
    if (jimsrand() < lossprob) {
        nlost++;
        if (TRACE > 0)
            printf("          TOLAYER3: packet being lost\n");
        return;
    }

    /* make a copy of the packet student just gave me since he/she may decide */
    /* to do something with the packet after we return back to him/her */
    mypktptr = (struct pkt*)malloc(sizeof(struct pkt));
    mypktptr->seqnum = packet.seqnum;
    mypktptr->acknum = packet.acknum;
    mypktptr->checksum = packet.checksum;
    for (i = 0; i < 20; i++)
        mypktptr->payload[i] = packet.payload[i];
    if (TRACE > 2) {
        printf("          TOLAYER3: seq: %d, ack %d, check: %d ", mypktptr->seqnum,
            mypktptr->acknum, mypktptr->checksum);
        for (i = 0; i < 20; i++)
            printf("%c", mypktptr->payload[i]);
        printf("\n");
    }

    /* create future event for arrival of packet at the other side */
    evptr = (struct event*)malloc(sizeof(struct event));
    evptr->evtype = FROM_LAYER3;   /* packet will pop out from layer3 */
    evptr->eventity = (AorB + 1) % 2; /* event occurs at other entity */
    evptr->pktptr = mypktptr;       /* save ptr to my copy of packet */
                                    /* finally, compute the arrival time of packet at the other end.
                                    medium can not reorder, so make sure packet arrives between 1 and 10
                                    time units after the latest arrival time of packets
                                    currently in the medium on their way to the destination */
    lastime = time;
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next) */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == FROM_LAYER3 && q->eventity == evptr->eventity))
            lastime = q->evtime;
    evptr->evtime = lastime + 1 + 9 * jimsrand();



    /* simulate corruption: */
    if (jimsrand() < corruptprob) {
        ncorrupt++;
        if ((x = jimsrand()) < .75)
            mypktptr->payload[0] = 'Z';   /* corrupt payload */
        else if (x < .875)
            mypktptr->seqnum = 999999;
        else
            mypktptr->acknum = 999999;
        if (TRACE > 0)
            printf("          TOLAYER3: packet being corrupted\n");
    }

    if (TRACE > 2)
        printf("          TOLAYER3: scheduling arrival on other side\n");
    insertevent(evptr);
}

void tolayer5(int AorB, char* datasent)
{
    int i;
    if (TRACE > 2) {
        printf("          TOLAYER5: data received: ");
        for (i = 0; i < 20; i++)
            printf("%c", datasent[i]);
        printf("\n");
    }
}
