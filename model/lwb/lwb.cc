/*
 * Copyright (c) 2016, Swiss Federal Institute of Technology (ETH Zurich).
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
 * Author:  Reto Da Forno
 *          Federico Ferrari
 *          Marco Zimmerling
 */

#include "ns3/log.h"

#include "ns3/lwb.h"

namespace ns3 {
namespace lora_mesh {
        
TypeId 
LWB::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::LWB")
        .SetParent<Object>()
        .SetGroupName("lora_mesh");
        
    return tid;
}

LWB::LWB()
{
}

LWB::~LWB()
{
}

void
LWB::Start(void (*pre_lwb_func)(void), void *post_lwb_proc)
{
    NS_LOG_FUNCTION(this << pre_lwb_func << post_lwb_proc);
    //
    return;
}

lwb_conn_state_t 
LWB::GetState(void) const
{
    NS_LOG_FUNCTION_NOARGS();
    
    if (m_sync_state < SYNCED)  /*  BOOTSTRAP or QUASI_SYNCED   */
    {
        return LWB_STATE_INIT;
    }
    else if (m_sync_state < MISSED) /*  SYNCED or SYNCED_2  */
    {
        return LWB_STATE_CONNECTED;
    }
    
    return LWB_STATE_CONN_LOST; /*  UNSYNCED or UNSYNCED2   */
}

void
LWB::Pause(void)
{
    //cancel timers
    return;
}

void
LWB::Resume(void)
{
    //start timers
    return;
}

void
LWB::Send(Ptr<Packet> packet)
{
    /*  placeholder func call   */
    m_glossy->Start(GetId(), packet, 3, GLOSSY_WITHOUT_SYNC, GLOSSY_WITHOUT_RF_CAL);
    return;
}
    
void
LWB::Receive(Ptr<Packet> packet)
{
    m_glossy->RxHandler(packet);
    return;
}

uint8_t
LWB::GetReceiveBufferState(void)
{
    //
    return 0;
}

uint8_t
LWB::GetSendBufferState(void)
{
    //
    return 0;
}



}
}
