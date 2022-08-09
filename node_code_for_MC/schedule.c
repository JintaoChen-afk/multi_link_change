#include <stdio.h>
#include "net/mac/tsch/tsch.h"
#include "net/mac/tsch/tsch-queue.h"
#include "tschdef.h"

/*-------------------- variables --------------------*/

// TSCH schedule
schedule_struct sched[SCHEDULE_SIZE] = 
{
	//{slot, channel offset, flowID (1-98=data, 99=bcast), role, sender, receiver (0=broadcast)}
	// role: =1(sender is a source), =2(sender is a forwarder)


//	{3, 0, 1, 1, 1, 2},  // 1->2 (unicast)
	//{2, 0, 5, 1, 2, 3},  // 1->2,3 (broadcast)
	//{2, 0, 1, 1, 13, 6},
{2, 0, 1, 1, 1, 2},
{4, 0, 2, 1, 4, 2},
{6, 0, 3, 1, 3, 5},


//{6, 0, 2, 1, 3, 4},
{7,0,6,1,3,1},
{8,0,7,1,3,2},
{9,0,8,1,3,4}, 


/*
{12, 0, 1, 1, 1, 2},
{14, 0, 2, 1, 3, 4},
{22, 0, 1, 1, 1, 2},
{24, 0, 2, 1, 3, 4},
{32, 0, 1, 1, 1, 2},
{34, 0, 2, 1, 3, 4},
{42, 0, 1, 1, 1, 2},
{44, 0, 2, 1, 3, 4},*/

	//{7, 0, 3, 1, 1, 4},  // 1->3 (unicast)
	//{8, 0, 2, 1, 1, 5},  // 1->3 (unicast)

	// {2, 0, 1, 1, 2, 1},  // 2->1 (unicast)
	// {3, 0, 2, 1, 2, 3},  // 2->3 (unicast) 

	// {3, 0, 1, 1, 2, 1},  // 2->1 (unicast)
	// {4, 0, 2, 1, 2, 3},  // 2->3 (unicast)

	// {3, 0, 1, 1, 2, 1},  // 2->1 (unicast)
	// {4, 0, 2, 1, 3, 1},  // 3->1 (unicast)

    // {3, 0, 1, 1, 2, 1},  // 2->1 (unicast)
	// {5, 0, 2, 1, 2, 3},  // 2->3 (unicast)

	// {2, 0, 1, 1, 2, 1},  // 2->1 (unicast)
	// {4, 0, 2, 1, 2, 3},  // 2->3 (unicast) seq no of node 1 one perid latency

	// {2, 0, 1, 1, 1, 2},  // 1->2 (unicast)
	// {4, 0, 2, 1, 1, 3},  // 1->3 (unicast) 

	// {2, 0, 1, 1, 1, 2},  // 1->2 (unicast)
	// {3, 0, 2, 1, 1, 3},  // 1->3 (unicast) 

	// {3, 0, 1, 1, 2, 1},  // 2->1 (unicast)
	// {6, 0, 2, 1, 2, 3},  // 2->3 (unicast)
	// {9, 0, 99, 1, 2, 0}  // 2->1,3 (broadcast) seq no of node 1 one period latency


	// {3, 0, 1, 1, 1, 3},  // 1->3 (unicast)
	// {9, 0, 2, 1, 3, 1}  // 3->1 (unicast)

	 //// {3, 0, 1, 1, 1, 3},  // 1->3 (unicast)
	 //// {9, 0, 99, 1, 3, 0}  // 3->1,2 (broadcast) node 1 cannot receive the broadcast message

	 // {3, 0, 1, 1, 3, 1},  // 3->1 (unicast)
	 // {9, 0, 99, 1, 1, 0}  // 1->2,3 (broadcast) node 3 cannot receive the broadcast message

	//// {3, 0, 1, 1, 2, 1},  // 2->1 (unicast)
	//// {9, 0, 99, 1, 3, 0}  // 3->1,2 (broadcast) node 2 cannot receive the broadcast message

	// {3, 0, 1, 1, 2, 1},  // 2->1 (unicast)
	// {5, 0, 2, 1, 3, 1},  // 3->1 (unicast)
	// {7, 0, 3, 1, 3, 2},  // 3->2 (unicast)
	// {9, 0, 4, 1, 1, 2}  // 3->1,2 (broadcast) node 2 cannot receive the broadcast message

	// {3, 0, 1, 1, 2, 1},  // 2->1 (unicast)
	// {6, 0, 2, 1, 2, 3},  // 2->3 (unicast) seq no of node 1 is weird

	// {3, 0, 1, 1, 3, 2},  // 3->2 (unicast)
	// {6, 0, 2, 1, 3, 1},  // 3->1 (unicast) seq no of node 1 and 2 are weird

	// {6, 0, 2, 1, 3, 2},  // 3->2 (unicast)
	// {9, 0, 99, 1, 3, 0}  // 3->1,2 (broadcast)

    // {6, 0, 2, 1, 3, 1},  // 3->1 (unicast)
	// {9, 0, 99, 1, 3, 0}  // 3->1,2 (broadcast)

	// {3, 0, 1, 1, 2, 1},  // 2->1 (unicast)
	// {6, 0, 2, 1, 3, 1},  // 3->1 (unicast)



    // {3, 0, 1, 1, 1, 2},  // 1->2 (unicast)   
	// {6, 0, 2, 1, 1, 3},  // 1->3 (unicast)


    // {3, 0, 1, 1, 3, 2},  // 3->2 (unicast)
	// {6, 0, 2, 1, 3, 1},  // 3->1 (unicast)
	// {9, 0, 99, 1, 3, 0}  // 3->1,2 (broadcast) seq no of node 1 and 3 are weird


	//// {3, 0, 99, 1, 1, 0},  // 1->2,3 (broadcast) node 1 sometimes cannot receive packet, 
	//// {6, 0, 99, 1, 2, 0},  // 2->1,3 (broadcast) node 2 sometimes cannot receive packet, 
	//// {9, 0, 99, 1, 3, 0}  // 3->1,2 (broadcast) node 3 sometimes cannot receive packet, 
};

uint8_t schedIndex[MAX_SCHED_ENTRIES]; // entries in "sched" related to the node
uint8_t countEntries = 0;
struct tsch_slotframe *slotframe;


/*-------------------- variables --------------------*/
//int finSchedEntry(uint8_t flowID, uint8_t sender);
//uint8_t findReceiver(uint8_t index);


// cooja
const linkaddr_t nodes[NUM_NODES] = {{{1,0}}, {{2,0}}, {{3,0}}, {{4,0}}, {{5,0}}};
//const linkaddr_t nodes[NUM_NODES] = {{{4,0}}, {{3,0}}, {{2,0}}, {{1,0}}};

//const linkaddr_t nodes[NUM_NODES] = {{{1,0}}, {{2,0}}, {{3,0}}, {{4,0}},
//{{5,0}}, {{6,0}}, {{7,0}}, {{8,0}},
//{{9,0}}, {{10,0}}, {{11,0}}, {{12,0}},
//{{13,0}}, {{14,0}}, {{15,0}}, {{16,0}},
//{{17,0}}, {{18,0}}, {{19,0}}, {{20,0}}


//};
// testbed nodes
/*const linkaddr_t nodes[NUM_NODES] = {{{101,0}}, {{102,0}}, {{103,0}}, {{104,0}},
                                   {{105,0}}, {{106,0}}, {{107,0}}, {{108,0}},
                                   {{109,0}}, {{110,0}}, {{111,0}}, {{112,0}},
                                   {{113,0}}, {{114,0}}, {{115,0}}, {{116,0}},
                                   {{117,0}}, {{118,0}}, {{119,0}}, {{120,0}},
                                   {{121,0}}, {{122,0}}, {{123,0}}, {{124,0}},
                                   {{125,0}}, {{126,0}}, {{127,0}}, {{128,0}},
                                   {{129,0}}, {{130,0}}, {{131,0}}, {{132,0}},
                                   {{133,0}}, {{134,0}}, {{135,0}}, {{136,0}},
                                   {{137,0}}, {{138,0}}, {{139,0}}, {{140,0}},
                                   {{141,0}}, {{142,0}}, {{143,0}}, {{144,0}},
                                   {{145,0}}, {{146,0}}, {{147,0}}, {{148,0}},
                                   {{149,0}}, {{150,0}}, {{151,0}}, {{152,0}},
                                   {{153,0}}, {{154,0}}, {{155,0}}, {{156,0}},
                                   {{157,0}}, {{158,0}}, {{159,0}}, {{160,0}},
                                   {{161,0}}, {{162,0}}, {{163,0}}, {{164,0}},
                                   {{165,0}}, {{166,0}}, {{167,0}}, {{168,0}},
                                   {{169,0}}, {{170,0}}};*/

const linkaddr_t* get_node_address(uint8_t index)
{
    //return &nodes[index-101]; // testbed
    return &nodes[index-1];		// cooja
}

void init_schedule()
{
	uint8_t i;


	//printf("inti_schedule %d\n", nodeID);

	for(i=0;i<MAX_SCHED_ENTRIES;i++)
	{
		schedIndex[i] = 0;
	}

	slotframe = tsch_schedule_add_slotframe(0, FRAME_LENGTH);

	// add advertising slot
	tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED | LINK_OPTION_TIME_KEEPING |  LINK_OPTION_TX | LINK_OPTION_RX, LINK_TYPE_ADVERTISING_ONLY, &tsch_broadcast_address, 1, 0);
	// tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED | LINK_OPTION_TIME_KEEPING |  LINK_OPTION_TX | LINK_OPTION_RX, LINK_TYPE_ADVERTISING, &tsch_broadcast_address, 1, 0);

	// add data slot --> read schedule from "sched"
	for(i=0;i<SCHEDULE_SIZE;i++)
	{
		
		if(sched[i].flow==99) // boradcast //
		{
			if(nodeID==sched[i].sender) // sender //
			{
				// tsch_schedule_add_link(slotframe, LINK_OPTION_TX, LINK_TYPE_NORMAL, &tsch_broadcast_address, sched[i].slot, sched[i].channel);
				
				tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED |LINK_OPTION_TX| LINK_OPTION_RX, LINK_TYPE_NORMAL, &tsch_broadcast_address, sched[i].slot, sched[i].channel);
				//tsch_schedule_add_link(slotframe, LINK_OPTION_TX, LINK_TYPE_NORMAL, get_node_address(2), sched[i].slot, sched[i].channel);	
				schedIndex[countEntries] = i;
				countEntries++;
			}
			else // receiver //
			{
				// tsch_schedule_add_link(slotframe, LINK_OPTION_RX, LINK_TYPE_NORMAL, &tsch_broadcast_address, sched[i].slot, sched[i].channel);
				
				tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED |LINK_OPTION_RX| LINK_OPTION_TX, LINK_TYPE_NORMAL, &tsch_broadcast_address, sched[i].slot, sched[i].channel);
				// tsch_schedule_add_link(slotframe, LINK_OPTION_RX, LINK_TYPE_NORMAL, get_node_address(sched[i].sender), sched[i].slot, sched[i].channel);
				schedIndex[countEntries] = i;
				countEntries++;
			}
		} //  end boradcast
		else // unicast (data)
		{
			if((*get_node_address(sched[i].sender)).u8[0]==nodeID) // sender //
			{
				tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED |LINK_OPTION_TX, LINK_TYPE_NORMAL, get_node_address(sched[i].receiver), sched[i].slot, sched[i].channel);
				schedIndex[countEntries] = i;
				countEntries++;
			}
			else 
			{
				if((*get_node_address(sched[i].receiver)).u8[0]==nodeID) // receiver //
				{
					tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED |LINK_OPTION_RX, LINK_TYPE_NORMAL, get_node_address(sched[i].sender), sched[i].slot, sched[i].channel);
					schedIndex[countEntries] = i;
					countEntries++;
				}
			}
		} // end unicast (data)
	} // end for
}


void update_schedule(schedule_struct new_schedule[SCHEDULE_SIZE],const uint8_t new_frame_length)
{
	uint8_t i;
	countEntries = 0;
	tsch_schedule_remove_all_slotframes();
	/**
	if(slotframe == NULL){
		printf("slotframe NULL!");
	}
	else{
		printf("slotframe still alive!");
	}
	**/
	for(i=0;i<MAX_SCHED_ENTRIES;i++)
	{
		schedIndex[i] = 0;
	}
	// printf("App: new schedule %u %u %u %u %u %u\n", new_schedule[0].slot, new_schedule[0].channel, new_schedule[0].flow, new_schedule[0].role, new_schedule[0].sender, new_schedule[0].receiver); 	
	slotframe = tsch_schedule_add_slotframe(0, new_frame_length);
	tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED | LINK_OPTION_TIME_KEEPING |  LINK_OPTION_TX | LINK_OPTION_RX, LINK_TYPE_ADVERTISING, &tsch_broadcast_address, 1, 0);
	
	for(i=0;i<SCHEDULE_SIZE;i++)
	{
		
		if(new_schedule[i].flow==99) // boradcast //
		{
			if(nodeID==new_schedule[i].sender) // sender //
			{
				// tsch_schedule_add_link(slotframe, LINK_OPTION_TX, LINK_TYPE_NORMAL, &tsch_broadcast_address, sched[i].slot, sched[i].channel);
				
				tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED |LINK_OPTION_TX| LINK_OPTION_RX, LINK_TYPE_NORMAL, &tsch_broadcast_address, new_schedule[i].slot, new_schedule[i].channel);
				//tsch_schedule_add_link(slotframe, LINK_OPTION_TX, LINK_TYPE_NORMAL, get_node_address(2), sched[i].slot, sched[i].channel);	
				schedIndex[countEntries] = i;
				countEntries++;
			}
			else // receiver //
			{
				// tsch_schedule_add_link(slotframe, LINK_OPTION_RX, LINK_TYPE_NORMAL, &tsch_broadcast_address, sched[i].slot, sched[i].channel);
				
				tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED |LINK_OPTION_RX| LINK_OPTION_TX, LINK_TYPE_NORMAL, &tsch_broadcast_address, new_schedule[i].slot, new_schedule[i].channel);
				// tsch_schedule_add_link(slotframe, LINK_OPTION_RX, LINK_TYPE_NORMAL, get_node_address(sched[i].sender), sched[i].slot, sched[i].channel);
				schedIndex[countEntries] = i;
				countEntries++;
			}
		} //  end boradcast
		else // unicast (data)
		{
			if(new_schedule[i].sender==nodeID) // sender //
			{
				tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED |LINK_OPTION_TX, LINK_TYPE_NORMAL, get_node_address(new_schedule[i].receiver), new_schedule[i].slot, new_schedule[i].channel);
				schedIndex[countEntries] = i;
				countEntries++;
			}
			else 
			{
				// if(new_schedule[i].receiver==nodeID) // receiver //
				// {
					// tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED |LINK_OPTION_RX, LINK_TYPE_NORMAL, get_node_address(new_schedule[i].sender), new_schedule[i].slot, new_schedule[i].channel);
					tsch_schedule_add_link(slotframe, LINK_OPTION_SHARED |LINK_OPTION_RX| LINK_OPTION_TX, LINK_TYPE_NORMAL, &tsch_broadcast_address, new_schedule[i].slot, new_schedule[i].channel);
					schedIndex[countEntries] = i;
					countEntries++;
				// }
			}
		} // end unicast (data)
	} // end for
	for(i=0;i<SCHEDULE_SIZE;i++)
	{
		sched[i]=new_schedule[i];
	}
}

/*---------------------------------------- forwarding functions--------------------------------------------------------*/
uint8_t forwardToUnicast(uint8_t flowID, uint8_t sender) // 0-not forward, otherwise-receiver address
{
	uint8_t i;

	int currInd = findSchedEntry(flowID, sender);

	if(currInd>-1) 
	{
		for(i=currInd;i<countEntries;i++)
		{
			uint8_t index = schedIndex[i];;
			// entry that a node is a sender
			if(sched[index].flow==flowID && (*get_node_address(sched[index].sender)).u8[0]==nodeID)
			{
				return sched[index].receiver;
			}
		} // end each sched entry
	}
	return 0;
}
int findSchedEntry(uint8_t flowID, uint8_t sender)
{
	int i;
	for(i=0;i<countEntries;i++)
	{
		uint8_t index = schedIndex[i];
		
		// entry that a node is a receiver
		if(sched[index].flow==flowID && (*get_node_address(sched[index].sender)).u8[0]==sender && (*get_node_address(sched[index].receiver)).u8[0]==nodeID)
		{
			return i;
		}
	}
	return -1;
}
