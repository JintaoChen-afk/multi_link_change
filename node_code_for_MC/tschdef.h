#ifndef __TSCHDEF_H__
#define __TSCHDEF_H__

// coordinator node ID
#define COORDINATOR_NODE 3

// number of nodes
#define NUM_NODES 5//5 // 70 for testbed

// slotframe length (in slot)
// #define FRAME_LENGTH 25/3//100 
#define FRAME_LENGTH 10//100 

// size of predefined schedule
#define SCHEDULE_SIZE 10//10//5

// max number of schedule entries related to each node
#define MAX_SCHED_ENTRIES 10//10//5

typedef struct
{
	uint16_t slot;
	uint8_t channel;
	uint8_t flow; 										//(1-98=data unicast, 99=broadcast)
	uint8_t role;										//(1=sender is a source, 2=sender is a forwarder)
	uint8_t sender;						
	uint8_t receiver; 	

}schedule_struct;

typedef struct
{
	uint16_t seq;
	uint8_t source;
	//following added by yehan 
	uint8_t update_sched_flag;
	schedule_struct update_sched[SCHEDULE_SIZE];
}tsch_bcast_struct;

/*
typedef struct
{
	int temprature;
	int humidity;
	int light;
}sensor_struct;
*/



typedef struct
{
	uint16_t seq;
	uint8_t flow;
	uint8_t source; 
	//long int data;
	char data[77];
	uint8_t us;
	//sensor_struct sensor_data;
	//uint8_t ack;							
	uint8_t update_sched_flag;
	//char uschedule[31];
	//uint8_t sch_length;
	//schedule_struct update_sched[SCHEDULE_SIZE];
}tsch_data_struct;

#define SBCAST_LEN sizeof(tsch_bcast_struct)
#define SDATA_LEN sizeof(tsch_data_struct)

/*--------- Variables ----------*/

uint8_t nodeID;								   // contains node id
schedule_struct sched[SCHEDULE_SIZE];  // array containing predefined schedule
uint8_t schedIndex[MAX_SCHED_ENTRIES]; // entries in "sched" related to the node
uint8_t countEntries;				  // number of entries related to the node

/*--------- Functions ----------*/
void set_coordinator();
void init_schedule();
void create_serial_msg();
const linkaddr_t* get_node_address(uint8_t index);

void send_broadcast();
void send_unicast(uint8_t rec_addr, uint8_t flow);

uint8_t forwardToUnicast(uint8_t flowID, uint8_t sender);
int findSchedEntry(uint8_t flowID, uint8_t sender);




#endif
