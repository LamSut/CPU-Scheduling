#include<stdio.h>

typedef struct{
	int arrival, burst, waiting, execute, id; 
	char gantt[100];
} Process; //Process structure
//arrival: arrival time
//burst: bursting time
//waiting: the time that this Process started waiting after the last bursting
//execute: the time after the last bursting of this Process
//id: the intial sequence number of this Process
typedef struct{ 
	Process p[20];	
	int front, rear; 
} Queue; //using for Round Robin Scheduler

void makenullQueue(Queue *q){ 
	q->front = -1;	q->rear = -1;}

int isEmpty(Queue q){ return q.front==-1; }
int isFull(Queue q){ return q.rear-q.front+1==20; }

Process front(Queue q){
	if(isEmpty(q)) printf("Empty Queue");
	else return q.p[q.front];
}

void deQueue(Queue *q){
	if(!isEmpty(*q)){
		q->front = q->front+1;
		if(q->front > q->rear) makenullQueue(q);
	}
	else printf("Can't deQueue, it's empty");
}

void enQueue(Process p, Queue *q){
	if(!isFull(*q)){
		if(isEmpty(*q))	q->front=0;
		int i;
		if(q->rear==19){
			for(i=q->front; i<=q->rear; i++)
				q->p[i-q->front]=q->p[i];
			q->rear=19-q->front; q->front=0;
		}
		q->rear=q->rear+1;
		q->p[q->rear]=p;
	}
	else printf("Can't enQueue, it's full");
}
void makenullGantt(char gantt[]){
	int i;
	for(i=0; i<100; i++)
		gantt[i]='\0';
} //create a blank Gantt chart

void FCFS(Process *A, int n){
	printf("\n************* FCFS SCHEDULER *******************\n");
	int time=0, remain=n;
	float waitingT=0, turnaroundT=0; //total waiting time & turn around time
	A[n].arrival=666;
	A[n].burst=666;
	int i, j;
	while(remain>0){ //there is any Process that didn't executed;
		int min=n;
		for(i=0; i<n; i++){
			if(A[i].arrival <= time && A[i].execute==0){
				if(A[i].arrival < A[min].arrival)
					min = i; //find the Process that has min arrival time & didn't executed
				else if(A[i].burst<A[min].burst && A[i].arrival == A[min].arrival)
					min = i; //if there's a pair of Processes that have equal arrival time				
			}		//choose the less bursting time one (for the min total waiting time/response time)
		}	  
		if(min!=n){ //found a suitable Process
			for(j=0; j<A[min].arrival; j++) 
				A[min].gantt[j] = '-'; //not arrival yet
			for(j=A[min].arrival; j<time; j++)	{
				A[min].gantt[j] = 'W'; //waiting
				waitingT++; //add total waiting time
				turnaroundT++; // add total turn around time
			}
			for(j=time;j<time+A[min].burst;j++){
				A[min].gantt[j] = 'R'; //bursting
				turnaroundT++; // add total turn around time
			}
			time+=A[min].burst;
			A[min].execute=time; //time when this Process finish the current bursting
			remain--;	//done 1 Process
		}
		else time++; //skip an amount of time if can't found the suitable Process	
	}
	for(i=0; i<n; i++){
		for(j=A[i].execute; j<time;j++)
			A[i].gantt[j] = '-';	//fill after bursting
	}
	for(i=0; i<n; i++){ //print Gantt chart
		for(j=0; j<time;j++){
			printf("%c",A[i].gantt[j]); 
		}	
		printf("\n");	
	}
	float AVGW=waitingT/(float)n, AVGR=waitingT/(float)n, //in non-preemptive, waiting time = response time
		  AVGT=turnaroundT/(float)n; //the average waiting time, response time, turn around time
	printf("AVGW= %.2f, AVGR= %.2f, AVGT= %.2f\n",AVGW,AVGR,AVGT);
}

void SJFpreemptive(Process *A, int n){
	printf("\n************* SJF Preemptive SCHEDULER *******************\n");
	int time=0, remain=n;
	float waitingT=0, responseT=0, turnaroundT=0; //total waiting time & response time & turn around time
	A[n].burst=666;
	int i, j;
	while(remain>0){ //there is any Process that didn't executed;
		int min=n;
		for(i=0; i<n; i++){
			if(A[i].arrival <= time && A[i].burst>0){
				if(A[i].burst < A[min].burst)
					min = i; //if there's a pair of Processes that have equal bursting time	
				else if(A[i].burst == A[min].burst && A[i].execute==0)
					min = i; //if there's a pair of Processes that have equal bursting time
			}		//choose the one that has not executed yet (for the min total response time)
		}
		if(min!=n){ //found a suitable Process
			if(A[min].execute==0){ //Process hasn't been executed
				for(j=0; j<A[min].arrival; j++) 
					A[min].gantt[j] = '-'; //not arrival yet
				for(j=A[min].arrival; j<time; j++)	{
					A[min].gantt[j] = 'W'; //waiting
					waitingT++; //add total waiting time 
					responseT++; //add total response time
					turnaroundT++; //add total turn around time
				}
			}
			else{ //Process has been executed but not done yet
				for(j=A[min].waiting; j<time; j++){
					A[min].gantt[j] = 'W'; //waiting
					waitingT++; //add total waiting time 
					turnaroundT++; //add total turn around time
				}
			}
			A[min].gantt[time] = 'R'; //bursting
			turnaroundT++;  //add total turn around time
			A[min].burst--; //reduce the remaining bursting time of this Process
			
			A[min].execute = time+1; // time when this Process finish the current bursting
			A[min].waiting = time+1; // add time that this Process have to wait for the next bursting
			
			if(A[min].burst==0)
				remain--; //if this Process has no bursting time left, it's done
		}
		time++;	//skip an amount of time	
	}
	for(i=0; i<n; i++){
		for(j=A[i].execute; j<time;j++)
			A[i].gantt[j] = '-';	//fill after bursting
	}
	for(i=0; i<n; i++){ //print Gantt chart
		for(j=0; j<time;j++){
			printf("%c",A[i].gantt[j]);
		}	
		printf("\n");	
	}
	float AVGW=waitingT/(float)n, AVGR=responseT/(float)n, 
		  AVGT=turnaroundT/(float)n; //the average waiting time, response time, turn around time
	printf("AVGW= %.2f, AVGR= %.2f, AVGT= %.2f\n",AVGW,AVGR,AVGT);
}

void RoundRobin(Process *A, int n, int quantum){
	printf("\n************* ROUND ROBIN SCHEDULER *******************\n");
	Queue Q;
	makenullQueue(&Q);
	Process tmp; tmp.burst=0; //create a clone Process

	int time=0, remain=n;
	float waitingT=0, responseT=0, turnaroundT=0; //total waiting time & response time & turn around time
	int i, j;
	for(i=0; i<n; i++){
		A[i].id=i;
		A[i].execute=-1;
	} //set up each Process 
	  //(if their execute time = -1, it hasn't been added to the Queue)
	
	while(remain>0){ //there is any Process that didn't executed;
		
		A[n].arrival=666;
		A[n].burst=666;
		while(1){
			int min=n;
			for(i=0; i<n; i++){
				if(A[i].arrival <= time && A[i].execute==-1){ 
				//find Processes that already arrival and haven't been added to the Queue
					if(A[i].arrival < A[min].arrival)
						min = i; 	//choose if the Process has min arrival time
					else if(A[i].burst<A[min].burst && A[i].arrival == A[min].arrival)
						min = i; 	//if there's a pair of Processes that have equal arrival time	
					}			//choose the less bursting time one
				}						 
			if(min!=n){
				A[min].execute=0; //confirm the current Process has been added to the Queue
				enQueue(A[min],&Q); //add this Process to the Queue
				min=n;	//reset the "min" value for other searchings
			}
			else break;
		}
		
		if(tmp.burst>0)	//check if this clone has any remaining bursting time
			enQueue(tmp,&Q); //add a Process (the clone of the last bursting one) to the Queue
			
		if(!isEmpty(Q)){ //check if the Queue is not empty
						//We will work with the Process in the Front of Queue					
			if(front(Q).execute==0){ //check if Process hasn't been executed
				for(i=0; i<front(Q).arrival;i++)
					A[front(Q).id].gantt[i] = '-';	//not arrival yet	
				for(i=front(Q).arrival; i<time; i++){
					A[front(Q).id].gantt[i] = 'W';	//waiting
					waitingT++;		//add total waiting time 
					responseT++; 	//add total response time
					turnaroundT++;	//add total turn around time
				}	
			}
			else{
				for(i=front(Q).waiting; i<time; i++){ //start from the time that this Process started waiting
					A[front(Q).id].gantt[i] = 'W'; //waiting
					waitingT++; 	//add total waiting time 
					turnaroundT++;	//add total turn around time
				}
			}
			
			if(front(Q).burst<=quantum){	/*check if the remaining bursting time of 
							this Process is less than Quantum*/ 
				for(i=time; i<time+front(Q).burst;i++){
					A[front(Q).id].gantt[i] = 'R'; //bursting
					turnaroundT++; //add total turn around time
				}
				time+=front(Q).burst; //add an amount of time that equal to the last bursting time
				
				A[front(Q).id].burst=0; //there's no bursting time left of this Process
				A[front(Q).id].execute=time; //time after the last bursting
				
				deQueue(&Q); //remove the "Front" of the Queue
				remain--; //a Process is done
				tmp.burst=0; //the clone of this Process has no bursting time (can't add to Queue later)
			}
			else{
				for(i=time; i<time+quantum; i++){ //just bursting with an amount of time that equal to Quantum
					A[front(Q).id].gantt[i] = 'R'; //bursting
					turnaroundT++; //add total turn around time
				}
				time+=quantum; //add an amount of time that equal to the last bursting time (equal to Quantum)
				
				A[front(Q).id].burst-=quantum; //reduce this Process bursting time 
							       //an amount of time that equal to the Quantum
							       
				A[front(Q).id].waiting=time; //time that the current Process starts waiting
				A[front(Q).id].execute=time; //time after the last bursting
				tmp = A[front(Q).id]; //cloning this Process
				deQueue(&Q); //remove the "Front" of the Queue
			}
		}
		else time++; //skip time if the Queue is empty
	}
	
	for(i=0; i<n; i++){
		for(j=A[i].execute; j<time;j++)
			A[i].gantt[j] = '-';	//fill the Gantt chart
	}
	for(i=0; i<n; i++){
		for(j=0; j<time;j++){
			printf("%c",A[i].gantt[j]); //print the Gantt chart
		}	
		printf("\n");	
	}
	float AVGW=waitingT/(float)n, AVGR=responseT/(float)n, 
		  AVGT=turnaroundT/(float)n;
	printf("AVGW= %.2f, AVGR= %.2f, AVGT= %.2f\n",AVGW,AVGR,AVGT); 
	//print the average waiting time, response time, turn around time.
	printf("Quantum = %d\n",quantum); //show the Quantum
}

int main(){
	Process A[20];
	int n, quantum;
	int i;
	printf("Input the quantity of Processes:"); 
	scanf("%d",&n);
	printf("Input the quantum value:");
	scanf("%d",&quantum);
	
	for(i=0; i<n; i++){
		printf("Input the arrival time of Process %d: ",i+1);
		scanf("%d",&A[i].arrival);
		printf("Input the burst time of Process %d: ",i+1);
		scanf("%d",&A[i].burst);
		makenullGantt(A[i].gantt);
		A[i].execute=0;
	}
	
	Process A1[20];
		for(i=0; i<n; i++){
			A1[i].arrival=A[i].arrival;
			A1[i].burst=A[i].burst;
			makenullGantt(A1[i].gantt);
		    A1[i].waiting=0;
			A1[i].execute=0;
			A1[i].id=i;
	}
	FCFS(A1,n);
	
	Process A2[20];
	for(i=0; i<n; i++){
		A2[i].arrival=A[i].arrival;
		A2[i].burst=A[i].burst;
		makenullGantt(A2[i].gantt);
		A2[i].waiting=0;
		A2[i].execute=0;
		A2[i].id=i;
	}
	SJFpreemptive(A2,n);

	Process A3[20];
	for(i=0; i<n; i++){
		A3[i].arrival=A[i].arrival;
		A3[i].burst=A[i].burst;
		makenullGantt(A3[i].gantt);
		A3[i].waiting=0;
		A3[i].execute=0;
		A3[i].id=i;
	}
	RoundRobin(A,n,quantum);
	return 0;
}
