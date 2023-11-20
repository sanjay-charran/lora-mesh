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

/*  Modified by: Sanjay Charran <sanjaycharran@gmail.com>   */

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
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/event-id.h"

#include "ns3/lwb.h"
#include "ns3/glossy-header.h"

/* whether the initiator should retransmit the packet after a certain
 * time of no reception */
#ifndef GLOSSY_CONF_RETRANSMISSION_TIMEOUT
#define GLOSSY_CONF_RETRANSMISSION_TIMEOUT      1
#endif /* GLOSSY_CONF_RETRANSMISSION_TIMEOUT */

// #ifndef GLOSSY_CONF_RTIMER_ID
// #define GLOSSY_CONF_RTIMER_ID                   RTIMER_HF_3
// #endif /* GLOSSY_CONF_RTIMER_ID */

// #ifndef GLOSSY_CONF_COLLECT_STATS
// #define GLOSSY_CONF_COLLECT_STATS               1
// #endif /* GLOSSY_CONF_COLLECT_STATS */

#define GLOSSY_MAX_HEADER_LEN       4
#define GLOSSY_UNKNOWN_N_TX_MAX     0
#define GLOSSY_UNKNOWN_INITIATOR    0
#define GLOSSY_UNKNOWN_PAYLOAD_LEN  0
#define RF_CONF_MAX_PKT_LEN         63

#define TIMEOUT_EXTRA_TICKS         70

namespace ns3 {
namespace lora_mesh {

class LWB;
class LoRaPHY;

typedef enum {
  GLOSSY_WITHOUT_RF_CAL = 0,
  GLOSSY_WITH_RF_CAL = 1,
} glossy_rf_cal_t;

class Glossy : public Object
{
public:
    static TypeId GetTypeId(void);
    
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
                Ptr<Packet> packet,
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
    uint8_t IsActive(void) const;

    /**
     * @brief get the number of received packets
     * during the last flood
     */
    uint8_t GetNRx(void) const;

    /**
     * @brief get the number of transmitted packets during the last flood
     */
    uint8_t GetNTx(void) const;

    /**
     * @brief get the length of the payload of the received/transmitted packet
     */
    uint8_t GetPayloadLength(void) const;

    /**
     * @brief get the reference time (timestamp of the reception of the first byte)
     * @return 64-bit timestamp (type uint64_t)
     */
    uint64_t GetTRef(void) const;
    
    void RxHandler(Ptr<Packet> packet);
    
    void SetNode(Ptr<Node> node);
    Ptr<Node> GetNode(void) const;
    
    void SetLWB(Ptr<LWB> lwb);
    Ptr<LWB> GetLWB(void) const;
    
    void SetTimeoutDelay(double timeout);
    double GetTimeoutDelay(void) const;
    
    void SetNTxMax(uint8_t n_tx_max);
    uint8_t GetNTxMax(void) const;
    
    void SetRfCal(glossy_rf_cal_t rf_cal);
    glossy_rf_cal_t GetRfCal(void) const;
    
private:
    bool ProcessGlossyHeader(Ptr<Packet> packet);
    
    Ptr<Node>           m_node;
    Ptr<LWB>            m_lwb;
    
    glossy_rf_cal_t     m_rf_cal;
    bool                m_active;
    uint8_t             m_n_rx;
    uint8_t             m_n_tx;
    uint8_t             m_n_tx_max;
    double              m_T_slot
    double              m_T_ref;
    
    double              m_timeout_delay_seconds;
    EventId             m_last_event;
    Ptr<Packet>         m_last_packet;
};

}
}

#endif /* __GLOSSY_H__ */

/**
 * @}
 * @}
 */
