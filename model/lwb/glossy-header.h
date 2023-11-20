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

#ifndef __GLOSSY_HEADER_H__
#define __GLOSSY_HEADER_H__

#include "ns3/header.h"

namespace ns3 {
namespace lora_mesh {
    
typedef enum 
{
    GLOSSY_COMMON_HEADER = 0x80,
} glossy_header_t;
    
typedef enum {
  GLOSSY_UNKNOWN_SYNC = 0x00,
  GLOSSY_WITH_SYNC = 0x10,
  GLOSSY_WITHOUT_SYNC = 0x20,
  GLOSSY_ONLY_RELAY_CNT = 0x30
} glossy_sync_t;

class GlossyHeader : public Header
{
public:
    GlossyHeader();
    ~GlossyHeader();
    
    static TypeId GetTypeId(void);
    TypeId GetInstanceTypeId(void) const;
    
    /*  virtual funcs   */
    uint32_t GetSerializedSize(void) const;
    uint32_t Deserialize(Buffer::Iterator start);
    void Serialize(Buffer::Iterator start) const;
    void Print(std::ostream &os) const;
    
    void SetPacketLen(uint8_t packet_len);
    uint8_t GetPacketLen(void) const;
    
    void SetInitiatorId(uint16_t initiator_id);
    uint16_t GetInitiatorId(void) const;
   
    void SetHeaderType(glossy_header_t header_type);
    glossy_header_t GetHeaderType(void) const;
    
    void SetSync(glossy_sync_t sync);
    glossy_sync_t GetSync(void) const;
    
    void SetNTxMax(uint8_t n_tx_max);
    uint8_t GetNTxMax(void) const;
    
    void SetRelayCnt(uint8_t relay_cnt);
    uint8_t GetRelayCnt(void) const;
    
private:
    uint8_t         m_packet_len;
    uint16_t        m_initiator_id;
    glossy_header_t m_header_type;
    glossy_sync_t   m_sync;
    uint8_t         m_n_tx_max;
    uint8_t         m_relay_cnt;
};

}
}


#endif /*   __GLOSSY_HEADER_H__ */
