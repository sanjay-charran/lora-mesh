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
LWB::LWBStart(void (*pre_lwb_func)(void), void *post_lwb_proc)
{
    NS_LOG_FUNCTION(this << pre_lwb_func << post_lwb_proc);
    //
    return;
}

lwb_conn_state_t 
LWB::LWBGetState(void) const
{
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

    
}
}
