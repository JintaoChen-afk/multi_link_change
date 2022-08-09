#include <stdio.h>
#include "contiki-conf.h"
#include "contiki.h"

#include "dev/light-sensor.h"
#include "dev/sht11/sht11-sensor.h"

#include "net/netstack.h"
#include "net/rime/rime.h"
#include "sys/timer.h"
#include "net/mac/tsch/tsch.h"
#include "dev/serial-line.h"
#include "tschdef.h"

int assoc_flag=0; // switch to 1 if association is successful
uint8_t serial_flag = 0; // 0=printf, 1=send to serial

uint16_t bcast_seq=0;
uint16_t data_seq=0;

uint8_t sched_state = 0;
uint8_t update_sched_flag = 0;
uint8_t spare_node = 4;
uint8_t spare_num = 0;
const uint8_t longT = 6;

const uint8_t frame_length1 = 10;
const uint8_t frame_length0 = 10;
static schedule_struct update_sched1[SCHEDULE_SIZE]={
{2, 0, 1, 1, 1, 2},
{3, 0, 9, 1, 1, 2},
{4, 0, 2, 1, 4, 2},
{6, 0, 3, 1, 3, 5},
{7,0,6,1,3,1},
{8,0,7,1,3,2},
{9,0,8,1,3,4}, 
};

static schedule_struct update_sched0[SCHEDULE_SIZE]={
{2, 0, 1, 1, 1, 2},
//{4, 0, 3, 1, 1, 3},
{4, 0, 2, 1, 3, 4},
//{3, 0, 99, 1, 1, 0}
{6, 0, 3, 1, 1, 2},
//{4, 0, 3, 1, 1, 3},
{8, 0, 4, 1, 3, 4},
};


void set_coordinator()
{
  if((*get_node_address(COORDINATOR_NODE)).u8[0]==nodeID)
  {
    printf("set as coordinator....");
    tsch_set_coordinator(1);
  }
  else
  {
    tsch_set_coordinator(0);
  }
}


#define SERIAL_BUF_SIZE 8
#define SERIAL_BUF_SIZE5 4
#define NUM_length 11
#define block_length 13

static char rx_buf[SERIAL_BUF_SIZE*8+13];
int i = 0;
int j = 0;
int lock = 1;
tsch_data_struct data_msg;
//data_msg.us = 0;
static int uart_rx_callback(char c) 
{
     rx_buf[i] = c;
     i = i + 1;
     //printf('%c\n',c);
     if(nodeID==1) printf("%c ",c);
     
     if(i > SERIAL_BUF_SIZE*8 + 11){
       rx_buf[i] = '\0';
       lock = 1;
       data_msg.us = 1;
       for(j=0;j< i;++j) data_msg.data[j] = rx_buf[j];
       data_msg.data[i] = '\0';
       //if(nodeID == 1) printf("%s\n",data_msg.data);
       lock = 0;
       i =0 ;
       //printf("%s",rx_buf);
       if(data_msg.data[0] =='$' && data_msg.data[1] == '1'){
         update_sched_flag = 1;
       }
    }
     
     return 1;
}









PROCESS(broadcast_test_process, "TSCH Node");
PROCESS(serial_input_process, "Serial input");

AUTOSTART_PROCESSES(&broadcast_test_process,&serial_input_process);

/*---------------------------------------------------------------------------*/
static struct unicast_conn uc;

 
// call when a node receive a unicast packet //
static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) 
{
 tsch_data_struct udata;
  packetbuf_copyto(&udata);
  //printf("rece\n");
  update_sched_flag = udata.update_sched_flag;
  if(serial_flag==0)
  {
    //if(udata.us == 1)
   //printf("%s, %d\n",udata.data,udata.seq);
    //else printf("1\n");
     if(nodeID!=2 && nodeID != 5) printf("App: unicast message received from %u, slot=%lu, flow=%u, seq=%u \n", from->u8[0], tsch_current_asn.ls4b, udata.flow, udata.seq);
     if(nodeID==2 && udata.us == 1 && (from->u8[0]==1 || from->u8[0]==4)) printf("%s %d\n",udata.data,from->u8[0]);
     if(nodeID==5 && udata.us == 1) printf("%s\n",udata.data);
     //printf("App: unicast message received from %u, slot=%lu, flow=%u, seq=%u \n", from->u8[0], tsch_current_asn.ls4b, udata.flow, udata.seq);
   }

   //if(udata.update_sched_flag == 1) update_sched_flag = 1;

  //if (nodeID==2)
  //{
  //  printf("%08ld %08ld %08ld\n", data.data, (long int)data.seq, tsch_current_asn.ls4b);
  //}
	//printf("unicast message received from %d.%d\n",
	// from->u8[0], from->u8[1]);

  // node needs to forward or not:  rec_addr= 0 --> node is a destination / otherwise, return address of the next hop
  uint8_t rec_addr = forwardToUnicast(udata.flow, from->u8[0]); 
//printf("%d\n",rec_addr);
  if(rec_addr>0)
  {
    if(serial_flag==0)
    {
      //printf("App: forward data message src=%u seq=%d flow=%u rec=%u\n", data.source, data.seq, data.flow, rec_addr);
    }
    packetbuf_copyfrom((uint8_t *)&udata, SDATA_LEN);
    unicast_send(&uc, get_node_address(rec_addr));
  }




}
// callback after sent //
static void sent_uc(struct unicast_conn *ptr, int status, int num_tx) 
{
  
}

static const struct unicast_callbacks unicast_callbacks = { recv_uc, sent_uc };

// construct packet, put packet in the buffer, and call unicast send //
void send_unicast(uint8_t rec_addr, uint8_t flow)
{
//get_sensor_data();
data_seq++;
  data_msg.source = nodeID;
  data_msg.seq = data_seq;
  data_msg.flow = flow;
  //data_msg.data = 0;
  //data_msg.sensor_data = sensor_data1;

  //data_msg.update_sched_flag = update_sched_flag;
  //uint8_t sched_size;
  //for(sched_size =0; sched_size<SCHEDULE_SIZE;sched_size++)
  //{
  //  data_msg.update_sched[sched_size] = update_sched1[sched_size];
  //}

  if(serial_flag==0)
  {
    //printf("App: data packet slot=%lu seq=%d flow=%u rec=%u\n", tsch_current_asn.ls4b, data_msg.seq, data_msg.flow, rec_addr);
  }
  
    //if(nodeID == 3) printf("1\n");
    
    //if(lock == 1) data_msg.us = 0;
    //else data_msg.us = 1;
    printf("%c %c\n",data_msg.data[67],data_msg.data[68]);
    int time_now = 0;
    time_now = (data_msg.data[67]-48) * 10 + (data_msg.data[68]-48);
    if(nodeID==3)  printf("time_now %d\n",time_now);
    if(time_now > 20 && nodeID == 3){
      update_sched_flag = 1;
      
    }
    data_msg.update_sched_flag = update_sched_flag;
    data_msg.us = 1;
    packetbuf_clear();//added by yehan
    packetbuf_copyfrom((uint8_t *)&data_msg, SDATA_LEN);
    if(nodeID == 1 || nodeID == 3 || nodeID == 4) printf("send %s %d\n",data_msg.data,data_msg.seq);
    unicast_send(&uc, get_node_address(rec_addr));
  
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static struct broadcast_conn bc;

tsch_bcast_struct bdata;  //yehan
// call when a node received a broadcast packet
static void recv_bc(struct broadcast_conn *c, const linkaddr_t *from)
{
  //tsch_bcast_struct data;  //yehan
  packetbuf_copyto(&bdata);

  if(serial_flag==0)
  {
     printf("App: broadcast message received from %u seq=%d source=%u slot=%lu assoc=%d\n", from->u8[0], bdata.seq, bdata.source, tsch_current_asn.ls4b, tsch_is_associated);
  }

  if(bdata.update_sched_flag == 1) update_sched_flag = 1;
  
}

static const struct broadcast_callbacks broadcast_call = {recv_bc};

tsch_bcast_struct bcast_msg;
// construct packet, put packet in the buffer, and call broadcast send //
void send_broadcast()
{
    bcast_seq++;
    bcast_msg.source = nodeID;
    bcast_msg.seq = bcast_seq;

    //added by yehan
    bcast_msg.update_sched_flag = update_sched_flag;
    uint8_t sched_size;
    for(sched_size =0; sched_size<SCHEDULE_SIZE;sched_size++)
    {
      bcast_msg.update_sched[sched_size] = update_sched1[sched_size];
    }
    
    if(serial_flag==0)
    {
      printf("App: broadcast packet sent src=%u seq=%d slot=%lu\n", bcast_msg.source, bcast_msg.seq, tsch_current_asn.ls4b);
      // printf("App: broadcast packet sent slot2=%d sender2=%d receiver2=%d\n", bcast_msg.update_sched[1].slot, bcast_msg.update_sched[1].sender, bcast_msg.update_sched[1].receiver);
      // printf("App: broadcast packet sent slot4=%d sender4=%d receiver4=%d\n", bcast_msg.update_sched[3].slot, bcast_msg.update_sched[3].sender, bcast_msg.update_sched[3].receiver);
    }
    packetbuf_clear();//added by yehan
    packetbuf_copyfrom((uint8_t *)&bcast_msg, SBCAST_LEN);
    broadcast_send(&bc);

}

/*---------------------------------------------------------------------------*/




PROCESS_THREAD(broadcast_test_process, ev, data)
{
  PROCESS_BEGIN();

  nodeID = linkaddr_node_addr.u8[0];

  init_schedule();    // read schedule from array and add each schedule entry into TSCH superframe
  set_coordinator(); 
  //printf("%d %d\n",countEntries,nodeID);

  NETSTACK_MAC.on();


  unicast_open(&uc, 146, &unicast_callbacks); // for unicast
  broadcast_open(&bc, 129, &broadcast_call);  // for broadcast

  static struct etimer et;
  etimer_set(&et, CLOCK_SECOND*0.3);
  // etimer_set(&et, CLOCK_SECOND*1.5);
  while(1) 
  {


    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_restart(&et);


    // every 1s --> check if a node needs to send a packet or not //
    uint8_t i;
    //printf("tsch_current_asn.ls4b %d\n",tsch_current_asn.ls4b);
     
    //if(tsch_current_asn.ls4b>8000)   update_sched_flag = 0;
     if(update_sched_flag != sched_state){

      // if(nodeID == 3)
      // {
      //   update_schedule(update_sched1);
      // }
      // else
      // {
      //   update_schedule(update_sched1);
      // }

      if(update_sched_flag == 1){
          update_schedule(update_sched1,frame_length1);
      }
      if(update_sched_flag == 0){
          update_schedule(update_sched0,frame_length0);
      }
      sched_state = update_sched_flag;
     }
     //if(tsch_current_asn.ls4b>8000 && nodeID == 3)   update_sched_flag = 1;
  
    for(i=0;i<countEntries;i++)
    {
      //if(nodeID == 4) printf("4"); 
      uint8_t index = schedIndex[i];
      //printf("%d %d\n",nodeID,(*get_node_address(sched[index].sender)).u8[0]);
      // entry that a node is a sender and a source --> generate packet
      if((*get_node_address(sched[index].sender)).u8[0]==nodeID && sched[index].role==1)
      {
          if(spare_node == nodeID){
            spare_num ++ ;
            spare_num %= longT;
            if(spare_num != 0) break;
          }

          if(sched[index].flow<99) // unicast
          {
            
            send_unicast(sched[index].receiver, sched[index].flow);
          }
          else{
            send_broadcast();
          }
      }
    //printf("%d\n",tsch_current_asn.ls4b);
    

//20200115
      // if(sched[index].receiver==nodeID && sched[index].role==1)
      // {
      //     if(sched[index].flow<99) // unicast
      //     {
            
      //         data_seq++;
             
            
      //     }
      // }
////


    }
     // printf("%08ld\n",(long int)data_seq);//20200115
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(serial_input_process, ev, data)
{
  PROCESS_BEGIN();
  // static struct etimer et2;
  //etimer_set(&et, CLOCK_SECOND*1.5);
  // etimer_set(&et2, CLOCK_SECOND*0.5);
  if (nodeID==1 || nodeID == 3 || nodeID == 4)
    {
      uart1_init(BAUD2UBR(115200));
      uart1_set_input(uart_rx_callback);
    }
  // while(1) 
  // {
  // PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et2));
  // etimer_restart(&et2);
  // unicast_open(&uc, 146, &unicast_callbacks); // for unicast
  // broadcast_open(&bc, 129, &broadcast_call);  // for braodcast

  // }
  PROCESS_END();
}

