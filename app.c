/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "em_common.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "gatt_db.h"
#include "app.h"


//1s is 32768 ticks - The Definition below sets up timer for 10ms
#define PERIODIC_TIMEOUT 328

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;


typedef union{
        struct{
            uint8_t     byte_0;
            uint8_t     byte_1;
            uint8_t     byte_2;
            uint8_t     byte_3;
            }Bytes;
        uint32_t value;
    } _32_8bit;

const uint8_t myText[] = {
      "It was close upon half-past six and the evening shadows were growing"
      "denser when two soldiers reached the little space, planted with trees,"
      "opposite the Musee Galliera, where the Rue de Chaillot and the Rue"
      "Pierre-Charron meet. One wore an infantryman's sky-blue great-coat; the"
      "other, a Senegalese, those clothes of undyed wool, with baggy breeches"
      "and a belted jacket, in which the Zouaves and the native African troops"
      "have been dressed since the war. One of them had lost his right leg, the"
      "other his left arm."
      "They walked round the open space, in the center of which stands a fine"
      "group of Silenus figures, and stopped. The infantryman threw away his"
      "cigarette. The Senegalese picked it up, took a few quick puffs at it,"
      "put it out by squeezing it between his fore-finger and thumb and stuffed"
      "it into his pocket. All this without a word."
      "Almost at the same time two more soldiers came out of the Rue Galliera."
      "It would have been impossible to say to what branch they belonged, for"
      "their military attire was composed of the most incongruous civilian"
      "garments. However, one of them sported a Zouave's _chechia_, the other"
      "an artilleryman's _kepi_. The first walked on crutches, the other on two"
      "sticks. These two kept near the newspaper-kiosk which stands at the edge"
      "of the pavement."
      "Three others came singly by the Rue Pierre-Charron, the Rue Brignoles"
      "and the Rue de Chaillot: a one-armed rifleman, a limping sapper and a"
      "marine with a hip that looked as if it was twisted. Each of them made"
      "straight for a tree and leant against it."
      "Not a word was uttered among them. None of the seven crippled soldiers"
      "seemed to know his companions or to trouble about or even perceive their"
      "presence. They stood behind their trees or behind the kiosk or behind"
      "the group of Silenus figures without stirring. And the few wayfarers"
      "who, on that evening of the 3rd of April, 1915, crossed this"
      "unfrequented square, which received hardly any light from the shrouded"
      "street-lamps, did not slacken pace to observe the men's motionless"
      "outlines."
      "A clock struck half-past six. At that moment the door of one of the"
      "houses overlooking the square opened. A man came out, closed the door"
      "behind him, crossed the Rue de Chaillot and walked round the open space"
      "in front of the museum. It was an officer in khaki. Under his red"
      "forage-cap, with its three lines of gold braid, his head was wrapped in"
      "a wide linen bandage, which hid his forehead and neck. He was tall and"
      "very slenderly built. His right leg ended in a wooden stump with a"
      "rubber foot to it.He leant on a stick."
      "Leaving the square,he stepped into the roadway of the Rue"
      "Here she comes, said the officer."
       "She took the road by which he had arrived and turned down the Rue"
       "Pierre-Charron, keeping to the right-hand pavement and thus making for"
       "the space where the street meets the Rue de Chaillot. Her walk was"
       "light, her step easy and wellbalanced. The wind, buffeting against her"
       "as she moved quickly on her way, swelled out the long blue veil floating"
       "around her shoulders. Notwithstanding the width of the cloak, the"
       "rhythmical swing of her body and the youthfulness of her figure were"
       "revealed. The officer kept behind her and walked along with an"
       "absent-minded air, twirling his stick, like a man taking an aimless"
       "stroll."
       "At this moment there was nobody in sight, in that part of the street,"
       "except him and her. But, just after she had crossed the Avenue Marceau"
       "and some time before he reached it, a motor standing in the avenue"
       "started driving in the same direction as the nurse, at a fixed distance"
       "from her."
       "It was a taxi-cab. And the officer noticed two things: first, that there"
       "were two men inside it and, next, that one of them leant out of the"
       "window almost the whole time, talking to the driver. He was able to"
       "catch a momentary glimpse of this mans face, cut in half by a heavy"
       "mustache and surmounted by a gray felt hat."
       "Meanwhile, the nurse walked on without turning round. The officer had"
       "crossed the street and now hurried his pace, the more so as it struck"
       "him that the cab was also increasing its speed as the girl drew near the"
       "space in front of the museum."
       "From where he was the officer could take in almost the whole of the"
       "little square at a glance and, however sharply he looked, he discerned"
       "nothing in the darkness that revealed the presence of the seven crippled"
       "men. No one, moreover, was passing on foot or driving. In the distance"
       "only, in the dusk of the wide crossing avenues, two tram-cars, with"
       "lowered blinds, disturbed the silence."
       "Nor did the girl, presuming that she was paying attention to the sights"
       "of the street, appear to see anything to alarm her. She gave not the"
       "least sign of hesitation. And the behavior of the motorcab following"
       "her did not seem to strike her either, for she did not look round once."
       "The cab, however, was gaining ground. When it neared the square,it was"
       "ten or fifteen yards, at most, from the nurse; and, by the time that"
       "she, still noticing nothing, had reached the first trees, it came"
       "closer yet and, leaving the middle of the road, began to hug the step."
        };

volatile _32_8bit tick_count;
// Handle for sleeptimer
sl_sleeptimer_timer_handle_t my_sleeptimer_handle;

volatile uint8_t data_send[] = {0,0,0,0,255,255,0,0,0,0,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
volatile uint8_t data_send2[] = {0xaa,0xaa,0xaa,0xaa,0xaa,0xbb};

volatile uint16_t TX_counter = 0;
volatile uint16_t RX_counter = 0;
volatile uint16_t CRCERR_counter = 0;
volatile uint16_t FAIL_counter = 0;

#define CHAR_HANDLE       21
#define PAYLOAD_LENGTH    64

static uint8_t Notification_enabled = 0;
static uint8_t Finished_TX = 0;
uint8_t connection;
uint8_t connection_MTU;
uint16_t myTextsize;
volatile int16_t offset = 0;

volatile uint16_t offset_log[60];

uint16_t payload_sent_len;
//static uint8_t payload[PAYLOAD_LENGTH] = {2};

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  myTextsize = sizeof(myText);
  myTextsize = myTextsize>>2;

  app_log("The size of the file is: %d \n\n\n\n",myTextsize );

}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{

  sl_status_t sc;

  static uint8_t i = 0;



   if (( Notification_enabled == 1)&&(sl_bt_event_pending()==false)&&(Finished_TX == 0))
     {

       if (offset <= myTextsize)
             {

           app_log("Transmitting \n\r");
          offset_log[i++]=offset;
          do {
               sc = sl_bt_gatt_server_send_notification(1, gattdb_Text,
                                                 connection_MTU-5,
                                                 &myText[offset]);

//               app_assert_status(sc);
//
//               if (offset >= myTextsize) break;
//              offset+= connection_MTU-5;

//               if (sc != SL_STATUS_NO_MORE_RESOURCE)
//               {
//                   app_assert_status(sc);
//               }

               if(sc ==0){
                      if (offset >= myTextsize) {
                          app_log("Finished Transmitting %d\n\r", i);
                          Finished_TX = 1;
                          for(int ii=0; ii<i;ii++)
                            {
                              app_log("%d \r\n", offset_log[ii]);
                            }

                          break;
                      }
                             offset+= connection_MTU-5;
               }


             }
             while(sc == 0);


             }

     }

   if (offset > myTextsize)
     {
       offset = 0;
     }

}


/**************************************************************************//**
 * @brief
 *   Sleeptimer callback function. Each time sleeptimer reaches timeout value,
 *   this callback is executed.
 *****************************************************************************/
void sleeptimer_cb(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  sl_status_t sc;

  tick_count.value+=1;
  //app_log("Tickcount = %d \n\r", tick_count.value);

  data_send[3] = tick_count.Bytes.byte_0;
  data_send[2] = tick_count.Bytes.byte_1;
  data_send[1] = tick_count.Bytes.byte_2;
  data_send[0] = tick_count.Bytes.byte_3;

  sc = sl_bt_system_get_counters(0, (uint16_t *)&TX_counter,  (uint16_t *)&RX_counter, (uint16_t *)&CRCERR_counter, (uint16_t *)&FAIL_counter );
  app_assert_status(sc);

  data_send[4] = 0xff;
  data_send[5] = 0xff;
  data_send[6] = TX_counter;
  data_send[7] = RX_counter;
  data_send[8] = CRCERR_counter;
  data_send[9] = FAIL_counter;


  sc = sl_bt_gatt_server_write_attribute_value(gattdb_Data_TX,
                                                                 0,
                                                                 sizeof(data_send),
                                                                 (unsigned char *)&data_send);
  app_assert_status(sc);

}


/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;
  bd_addr address;
  uint8_t address_type;
  uint8_t Dev_name[11] = "PeriDev0000";


  //app_log("event: %x \n\r", SL_BT_MSG_ID(evt->header));

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:

      app_log("boot \n\r");

      // Extract unique ID from BT Address.
      sc = sl_bt_system_get_identity_address(&address, &address_type);
      app_assert_status(sc);


      //Add last 2 bytes of the MAC ADDRESS to the Name
      Dev_name[7] = (0x0f & address.addr[0]);
      if(Dev_name[7]<=0x09) Dev_name[7]+=0x30;
      else Dev_name[7]+=0x37;
      Dev_name[8] = (address.addr[0]>>4);
      if(Dev_name[8]<=0x09) Dev_name[8]+=0x30;
      else Dev_name[8]+=0x37;
      Dev_name[9] = (0x0f & address.addr[1]);
      if(Dev_name[9]<=0x09) Dev_name[9]+=0x30;
      else Dev_name[9]+=0x37;
      Dev_name[10] = (address.addr[1]>>4);
      if(Dev_name[10]<=0x09) Dev_name[10]+=0x30;
      else Dev_name[10]+=0x37;

      data_send[10] = 0xFF;
      data_send[11] = 0xFF;
      data_send[12] = address.addr[5];
      data_send[13] = address.addr[4];
      data_send[14] = address.addr[3];
      data_send[15] = address.addr[2];
      data_send[16] = address.addr[1];
      data_send[17] = address.addr[0];
      data_send[18] = 0xFF;
      data_send[19] = 0xFF;



      sc = sl_bt_gatt_server_write_attribute_value(gattdb_device_name,0,sizeof(Dev_name),Dev_name);
      app_assert_status(sc);

      //Set the Max TX power to 0 dBm
      sc = sl_bt_system_set_tx_power(0, 0, 0, 0);
      app_assert_status(sc);

      // Create an advertising set.
      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);

      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Set advertising interval to 100ms.
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        160, // min. adv. interval (milliseconds * 1.6)
        160, // max. adv. interval (milliseconds * 1.6)
        0,   // adv. duration
        0);  // max. num. adv. events
      app_assert_status(sc);
      // Start advertising and enable connections.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_advertiser_connectable_scannable);
      app_assert_status(sc);

      //Writes Data regarding Data_TX to the internal GATT Database
      sc = sl_bt_gatt_server_write_attribute_value(gattdb_Data_TX,0,sizeof(data_send),(unsigned char *)&data_send);
      app_assert_status(sc);
      //Writes Data regarding Data_RX to the internal GATT Database
      sc = sl_bt_gatt_server_write_attribute_value(gattdb_Data_RX,0,sizeof(data_send2),(unsigned char *)&data_send2);
      app_assert_status(sc);

      //Resets Tick Counter
      tick_count.value = 0;
      //Initiates Tick Counter timer
      sc = sl_sleeptimer_start_periodic_timer(&my_sleeptimer_handle, PERIODIC_TIMEOUT, sleeptimer_cb, (void *)NULL,0,0);
      app_assert_status(sc);

      //Reset System Counters
      sc = sl_bt_system_get_counters(1, (uint16_t *)&TX_counter,  (uint16_t *)&RX_counter, (uint16_t *)&CRCERR_counter, (uint16_t *)&FAIL_counter );
      app_assert_status(sc);

      break;

    // -------------------------------
    // This event indicates that a new connection was opened.
    case sl_bt_evt_connection_opened_id:
      connection = evt->data.evt_connection_opened.connection;

      app_log("Connection Handle %d", connection);
      tick_count.value = 0;


      break;


    case sl_bt_evt_gatt_mtu_exchanged_id:

      connection_MTU = evt->data.evt_gatt_mtu_exchanged.mtu;
      app_log("MTU size from connection is %d \n\r", evt->data.evt_gatt_mtu_exchanged.mtu);

      break;

    case sl_bt_evt_connection_parameters_id:

      app_log("Connection interval set %d \n\r",evt->data.evt_connection_parameters.interval);
      app_log("Connection Latency set %d \n\r",evt->data.evt_connection_parameters.latency);
      app_log("Connection Timeout set %d \n\r",evt->data.evt_connection_parameters.timeout);


      break;


    case sl_bt_evt_gatt_procedure_completed_id:

         //app_log("Gatt Event Completed %x \n\r",evt->data.evt_gatt_procedure_completed.result );


         break;


    case sl_bt_evt_gatt_server_characteristic_status_id:

                if (gattdb_Text == evt->data.evt_gatt_server_characteristic_status.characteristic) {
                  // client characteristic configuration changed by remote GATT client
                  if (sl_bt_gatt_server_client_config == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags) {

                      if(sl_bt_gatt_notification == (sl_bt_gatt_client_config_flag_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags)
                        {
                          Notification_enabled = 1;
                          Finished_TX = 0;
                          offset = 0;
                          connection = evt->data.evt_gatt_server_characteristic_status.connection;
                          app_log(" Notification_enabled = 1");


                        }
                      else if(sl_bt_gatt_disable == (sl_bt_gatt_client_config_flag_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags)
                        {
                          Notification_enabled = 0;
                        }

                  }
                  // confirmation of indication received from remove GATT client
                 else {
                    app_assert(false,
                               "[E: 0x%04x] Unexpected status flag in evt_gatt_server_characteristic_status\n",
                               (int)evt->data.evt_gatt_server_characteristic_status.status_flags);
                  }
                }
                break;

    case sl_bt_evt_gatt_server_attribute_value_id:
      {


      sc =  sl_bt_gatt_write_characteristic_value_without_response(evt->data.evt_gatt_server_attribute_value.connection,
                                                      CHAR_HANDLE,
                                                      64,
                                                      data_send, &payload_sent_len);


               app_assert_status(sc);
      }
      break;

    // -------------------------------
    // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id:

      sl_sleeptimer_stop_timer(&my_sleeptimer_handle);

      // Generate data for advertising
      sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                 sl_bt_advertiser_general_discoverable);
      app_assert_status(sc);

      // Restart advertising after client has disconnected.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_advertiser_connectable_scannable);
      app_assert_status(sc);
      break;

    ///////////////////////////////////////////////////////////////////////////
    // Add additional event handlers here as your application requires!      //
    ///////////////////////////////////////////////////////////////////////////

    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}
