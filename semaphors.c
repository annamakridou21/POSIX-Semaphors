#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

sem_t empty,full;
pthread_mutex_t mutex;
int flag=0;
struct waiting_room {
    int AM;                 /*ftiaxnw mia domh gia to dwmatio anamonhs aka diadromo*/
    struct waiting_room *next;
};

struct study_room {     /*ftiaxnw mia domh gia to anagnwsthrio aka study room*/
    int AM;
    struct study_room *next;
};

typedef struct waiting_room *WR;        /*kefales gia to waiting room kai to study room*/
typedef struct study_room *SR;
struct waiting_room* headWR=NULL;
struct study_room* headSR=NULL;

int waiting_room_insert(WR current,int x) {     /*algorithmos eisodou sto diadromo*/
    WR newnode;
    WR last;
    newnode=(struct waiting_room*)malloc(sizeof(struct waiting_room));
    if (!newnode) return 0;
    last=headWR;
    newnode->AM=x;
    newnode->next=NULL;                                        
    if(current==NULL) {    /*an h kefalh einai NULL tote to newnode einai to head ths listas*/
        newnode->next=headWR;                          
        headWR=newnode;
    }
    else{ 
        while(last->next!=NULL) {   /*alliws vazw to stoixeio sto telos ths listas. vriskw to telos ths listas me to last pointer*/
            last=last->next;
        }       
        last->next=newnode;   /*teleutaio stoixeio to newnode*/
    }
    return 0;
}

int waiting_room_delete(int x) {
    WR current=headWR;
    if(headWR==NULL)return 0;           /*diagrafh stoixeiou ouras:diagrafw to prwto node kai vazw to head ston epomeno*/
    else if(headWR->next==NULL)headWR=NULL; 
    else headWR=headWR->next;
    free(current);
    return 0;
}
void waiting_room_print() {
    WR current;
    current=headWR;
    printf("Waiting Room: ");
    if(headWR!=NULL)printf("|");
    while (current!= NULL) {        /*diasxizw olo to diadromo kai tupwnw enan enan tous foithtes se anamonh*/
        printf("| %d |",current->AM);
        current=current->next;
    }
    if(headWR!=NULL)printf("|");
    printf("\n\n");
}

int waiting_room_length() {
    int count=0;
    WR current=headWR; 
    while (current!=NULL) {         /*vriskw to megethos ths ouras anamonhs*/
        count++;
        current=current->next;
    }
    return count;
}

int waiting_room_lookup(int x) {
    WR current=headWR;
    while(current!=NULL) {
        if(current->AM==x) return 1;                   /*epistrefw 1 an vrw to stoixeio pou psaxnw*/
        current=current->next;
    }
    return 0;
}

int study_room_insert(SR current,int x) {
    SR newnode;
    SR last;
    newnode=(struct study_room*)malloc(sizeof(struct study_room));
    if (!newnode) return 0;
    last=headSR;
    newnode->AM=x;
    newnode->next=NULL;                                        
    if(current==NULL) {   /*an h lista einai adeia tote to newnode einai to head ths listas*/
        newnode->next=headSR;                          
        headSR=newnode;
    }
    else{ 
        while(last->next!=NULL) {   /*alliws vazw to stoixeio sto telos ths listas. vriskw to telos ths listas me to last pointer*/
            last=last->next;
        }       
        last->next=newnode;     /*teleutaio stoixeio to newnode*/
    }
    return 0;
}


int study_room_delete(int x) {
    SR current,prev;
    current=headSR;
    prev=current;
    while(current->AM!=x && current!=NULL) {    /*psaxnw to stoixeio pou thelw na diagrapsw*/
        prev=current;          /*krataw kai enan prev pointer epeidh thelw o prev pointer na deixnei sto next tou stoixeiou pou tha diagrapsw*/
        current=current->next;
    }
    if(current==headSR) {          /*an thelw na diagrapsw to prwto stoixeio ths listas tote apla proxwraw to head kai xanetai to prwto stoixeio*/
        headSR=headSR->next;
    }
    else {
        if(current->next!=NULL) {
            prev->next=current->next;    /*an den thelw na svhsw to teleutaio stoixeio tote o prev pointer deixnei sto next tou current.etsi to current xanetai*/
        }
        else prev->next=NULL;     /*an thelw na svhsw to teleutaio stoixeio ths listas tote o next pointer tou prev deixnei sto null. ara to teleutaio stoixeio xanetai*/
    }
    free(current);
    return 0;
}

void study_room_print() {
    SR current;
    current=headSR;
    printf("Study Room: ");
    if(headSR!=NULL)printf("|");
        while (current!= NULL) {        /*diasxizw olo to anagnwsthrio kai tupwnw enan enan tous foithtes pou diavazoun*/
        printf("| %d |",current->AM);
        current=current->next;
    }
    if(headSR!=NULL)printf("|");
    printf("\n\n");
}

int study_room_length() {
    int count=0;
    SR current=headSR; 
    while (current!=NULL) {         /*vriskw to megethos ths ouras anamonhs*/
        count++;
        current=current->next;
    }
    return count;
}

void *enter_study_room(void *AM) {
    int i,count,random,upper=15,lower=5,am;
    am = *((int *)AM);
    pthread_mutex_lock(&mutex); /*eisodos sth krisimh perioxh*/
    if(waiting_room_lookup(am)==0){     /*de ksanavazw ta blocked threads*/
        waiting_room_insert(headWR,am);     /*opoios thelei na mpei sto anagnwsthrio pernaei prwta apo ton diadromo*/
    }
    pthread_mutex_unlock(&mutex);   /*eksodos apth krisimh perioxh*/
    
    sem_wait(&empty);       /*meiwnetai o arithmos twn eleutherwn thesewn afou mpainei foithths.o sem einai arxika 8. an einai >0 mpainei, alliws blockarei to thread kai perimenei na ginei sem_post*/
    pthread_mutex_lock(&mutex); /*eisodos sth krisimh perioxh*/
    if(headWR!=NULL)am=headWR->AM;
    printf("Student %d entered the study room.\n",am);
    study_room_insert(headSR, am);  /*eisodos sth domh study room*/
    waiting_room_delete(am);     /*mphke sto room ara efuge apo ton diadromo*/
    if(study_room_length()<=7)flag=0;  /*to flag to xrhsimopoiw gia na kserw pote na mplokarw tous foithtes pou erxontai*/
    if(study_room_length()==8)flag=1;  /*PERIPTWSH 1an to anagnwsthrio einai gemato prepei na perimenoun oloi eksw mexri na adeiasei teleiws*/
    study_room_print(); /*tupwnw tous foithtes mesa*/
    waiting_room_print();   /*tupwnw tous foithtes eksw*/
    pthread_mutex_unlock(&mutex);   /*eksodos apth krisimh perioxh*/
    sem_post(&full);    /*auksanetai o arithmos twn kateilhmenwn thesewn. arxika o sem einai 0 giati den kathetai kaneis mesa*/

    random=(rand()%upper-lower+1)+lower;    /*tupos gia upologismo random arithmou in range (5,15)*/
    sleep(random);  /*diavazei 5-15 deuterolepta*/

    sem_wait(&full);        /*wra na fugei, ara meiwnoume ton arithmo ton kateilhmenwn thesewn. an mphke prin 1 px twra to full tha ginei 0*/
    pthread_mutex_lock(&mutex); /*eisodos sth krisimh perioxh*/
    printf("Student %d exited the study hall after studying for %d secs\n\n",am,random);
    study_room_delete(am);     /*feugei apo to anagnwsthrio*/
    if(study_room_length()==0)flag=2;  /*PERIPTWSH 2 an adeiase to dwmatio tote vazw mesa osous perimenan*/
    study_room_print(); /*print study room*/
    waiting_room_print();   /*tupwnw tous foithtes eksw*/
    pthread_mutex_unlock(&mutex); /*eksodos apo th krisimh perioxh*/
    if(flag!=2)sem_post(&empty); /*auksanw ton arithmo twn adeiwn thesewn afou efuge kapoios. auto se periptwsh pou to room den einai hdh adeio*/
    
    if(flag==1){
        sem_init(&empty,0,0);   /*PERIPTWSH 1 to anagnwsthrio einai gemato kai exoume foithtes pou perimenoun. o tropos gia na tous mplokarw einai na kanw to semaphor 0. etsi to thread perimenei mexri na ginei >0*/
        sem_init(&full,0,8);
    }
    if(flag==2){    /*PERIPTWSH 2 to anagnwsthrio einai adeio kai prepei na mpoun oi waiting foithtes*/
        count=waiting_room_length();    /*metrame posoi einai apeksw*/
        if(count<8){        /*an einai ligoteroi apo 8 tote xwrane oxi*/
            for(i=0;i<count;i++){   
                sem_post(&empty);   /*auksanw kai meiwnw to semaphor kata count gia na mpoun akrivws tosoi*/
                sem_wait(&full);
            }
        }
        else {  /*alliws vazoume mono 8 kai oi upoloipoi ksanamplokarontai apto semaphor*/
            for(i=0;i<8;i++){
                sem_post(&empty);
                sem_wait(&full);
            }
        }
    }
    free(AM);
    return NULL;
}

int main() {
    int i,N, *AM;
    pthread_t *threads;
    printf("Please enter the number of students N: ");
    do{
        scanf("%d", &N);
        if (N<20 || N>40) {     /*oi foithtes prepei nanai 20-40*/
            printf("You have exceeded the limit. Try again.\n");
        }
    } 
    while (N<20 || N >40);
    threads = (pthread_t *)malloc(N * sizeof(pthread_t));
    if (!threads)return 0;
    sem_init(&empty, 0, 8); /*oi adeies theseis einai 8*/
    sem_init(&full, 0, 0);  /*oi gemates theseis einai 0*/
    pthread_mutex_init(&mutex, NULL);  /*arxikopoihsh tou mutex*/  
    srand(time(0));         /*seed gia to random number*/
    for (i=0; i<N; i++) {
        AM=(int *)malloc(sizeof(int));  /*h parametros prepei na dothei san void* enw einai int*/
        if (!AM) return 0;      /*krataw enan int pointer sto i+1 pou einai to AM kathe thread*/
        *AM=i+1;                /*dhmiourgw N threads me parametro to AM*/
        pthread_create(&threads[i], NULL, enter_study_room, (void *)AM);
    }
    for (i=0; i<N; i++) {
        pthread_join(threads[i], NULL);    /*to main thread perimenei ola ta threads na ektelestoyn*/
    }
    sem_destroy(&empty);        /*afou teleiwsei h ektelesh katastrefw tous shmaforous pou xrhsimopoihsa*/
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    free(threads);
    return 0;
}
