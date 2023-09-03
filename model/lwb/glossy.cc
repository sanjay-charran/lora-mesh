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

#include "ns3/glossy.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

namespace ns3 {
namespace lora_mesh {
 
Glossy::Glossy()
{    
}
 
Glossy::~Glossy()
{
}

Glossy::Start(uint16_t initiator_id, uint8_t *payload, uint8_t payload_len, uint8_t n_tx_max,
              glossy_sync_t sync, glossy_rf_cal_t rf_cal)
{
    /* reset the data structure */
    m_glossy_state.active = 1;
    m_glossy_state.payload = payload;
    m_glossy_state.payload_len = payload_len;
    m_glossy_state.n_rx = 0;
    m_glossy_state.n_tx = 0;
    m_glossy_state.relay_cnt_last_rx = 0;
    m_glossy_state.relay_cnt_last_tx = 0;
    m_glossy_state.t_ref_updated = 0;
    m_glossy_state.T_slot_sum = 0;
    m_glossy_state.n_T_slot = 0;
    
    m_glossy_state.header.initiator_id = initiator_id;
    m_glossy_state.header.relay_cnt = 0;
    
    m_glossy_state.header.pkt_type = GLOSSY_COMMON_HEADER | ((sync) & 0x30) | ((n_tx_max) & 0x0f);
    
    if (m_glossy_state.header.initiator_id == m_node->GetId())
    {
        /*  Glossy initiator    */
        if (((m_glossy_state.header.pkt_type & 0x30) == GLOSSY_UNKNOWN_SYNC) ||
            (m_glossy_state.payload_len + 
            (((m_glossy_state.header.pkt_type & 0x30) == GLOSSY_WITHOUT_SYNC) ? 3 : 4)) + 1 > RF_CONF_MAX_PKT_LEN)
        {
            /** 
             * the initiator must know whether there will be synchronization or
             * not and the packet length may not exceed the max. length 
             */
            Stop();
        }
        else
        {
            m_glossy_state.t_timeout = Simulator::Now() + TIMEOUT_EXTRA_TICKS;
            //add glossy transmission
            m_glossy_state.relay_cnt_timeout = 0;
        }
    }
    else
    {
        /*  Glossy receiver */
        
        //add glossy receive stuff if needded
        
    }
    
    //while(RF1AIN & BIT0);
}
 
}
}
