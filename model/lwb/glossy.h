/*
 * Copyright (c) 2015, Swiss Federal Institute of Technology (ETH Zurich).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author:  Federico Ferrari
 *          Reto Da Forno
 *          Marco Zimmerling
 */

/**
 * @addtogroup  Net
 * @{
 *
 * @defgroup    glossy Glossy
 * @{
 *
 * @file
 * 
 * @brief Wireless Communication Protocol 'Glossy'
 */

#ifndef __GLOSSY_H__
#define __GLOSSY_H__

#include <stdlib.h>

#include "ns3/ptr.h"
#include "ns3/node.h"

#include "ns3/lwb.h"

/* whether the initiator should retransmit the packet after a certain
 * time of no reception */
#ifndef GLOSSY_CONF_RETRANSMISSION_TIMEOUT
#define GLOSSY_CONF_RETRANSMISSION_TIMEOUT      1
#endif /* GLOSSY_CONF_RETRANSMISSION_TIMEOUT */

#ifndef GLOSSY_CONF_RTIMER_ID
#define GLOSSY_CONF_RTIMER_ID                   RTIMER_HF_3
#endif /* GLOSSY_CONF_RTIMER_ID */

#ifndef GLOSSY_CONF_COLLECT_STATS
#define GLOSSY_CONF_COLLECT_STATS               1
#endif /* GLOSSY_CONF_COLLECT_STATS */

/* max. header length (with sync) */
#define GLOSSY_MAX_HEADER_LEN                   4

#define RF_CONF_MAX_PKT_LEN  63
#define GLOSSY_COMMON_HEADER 0x80
#define TIMEOUT_EXTRA_TICKS 70

namespace ns3 {
namespace lora_mesh {

class LWB;
    
enum {
  GLOSSY_UNKNOWN_INITIATOR = 0
};

enum {
  GLOSSY_UNKNOWN_N_TX_MAX = 0
};

enum {
  GLOSSY_UNKNOWN_PAYLOAD_LEN = 0
};

typedef enum {
  GLOSSY_WITHOUT_RF_CAL = 0,
  GLOSSY_WITH_RF_CAL = 1,
} glossy_rf_cal_t;

typedef enum {
  GLOSSY_UNKNOWN_SYNC = 0x00,
  GLOSSY_WITH_SYNC = 0x10,
  GLOSSY_WITHOUT_SYNC = 0x20,
  GLOSSY_ONLY_RELAY_CNT = 0x30
} glossy_sync_t;

typedef struct {
  uint16_t initiator_id;
  uint8_t pkt_type;
  uint8_t relay_cnt;
} glossy_header_t;

typedef struct {
  uint64_t t_ref, t_tx_stop, t_rx_start, t_rx_stop, t_tx_start;
  uint64_t T_slot_sum;
  uint64_t T_slot_estimated;
  uint64_t t_timeout;
  glossy_header_t header;
  uint8_t *payload;
  uint8_t payload_len;
  uint8_t n_T_slot;
  uint8_t active;
  uint8_t t_ref_updated;
  uint8_t header_ok;
  uint8_t  relay_cnt_last_rx;
  uint8_t  relay_cnt_last_tx;
  uint8_t  relay_cnt_t_ref;
  uint8_t  relay_cnt_timeout;
  uint8_t  n_rx;                                /* rx counter for last flood */
  uint8_t  n_tx;
} glossy_state_t;


class Glossy
{
public:
    Glossy();
    ~Glossy();
    
   /**
    * @brief       start Glossy
    * @param[in]   initiator_id node ID of the initiator, use
    *              GLOSSY_UNKNOWN_INITIATOR if the initiator is unknown
    * @param[in]   payload pointer to the packet data
    * @param[in]   payload_len length of the data, must not exceed PACKET_LEN_MAX
    * @param[in]   n_tx_max maximum number of retransmissions
    * @param[in]   sync synchronization mode
    * @note        n_tx_max must be at most 15!
    *
    * start Glossy, i.e. initiate a flood (if node is initiator) or switch to RX
    * mode (receive/relay packets)
    */
    void Start( uint16_t initiator_id,
                uint8_t *payload,
                uint8_t payload_len,
                uint8_t n_tx_max,
                glossy_sync_t sync,
                glossy_rf_cal_t rf_cal);

    /**
     * @brief stop glossy
     */
    uint8_t Stop(void);

    /**
     * @brief query activity of glossy
     * @return the number of received bytes since glossy_start was called
     */
    uint8_t IsActive(void);

    /**
     * @brief get the number of received packets
     * during the last flood
     */
    uint8_t GetNRx(void);

    /**
     * @brief get the number of transmitted packets during the last flood
     */
    uint8_t GetTRx(void);

    /**
     * @brief get the length of the payload of the received/transmitted packet
     */
    uint8_t GetPayloadLength(void);

    /**
     * @brief checks whether the reference time has been updated in the last
     * glossy flood
     */
    uint8_t IsTRefUpdated(void);

    /**
     * @brief get the reference time (timestamp of the reception of the first byte)
     * @return 64-bit timestamp (type uint64_t)
     */
    uint64_t GetTRef(void);
    
    
private:
    glossy_state_t      m_glossy_state;
    Ptr<Node>           m_node;
    Ptr<LWB>            m_lwb;
};



}
}

#endif /* __GLOSSY_H__ */

/**
 * @}
 * @}
 */
