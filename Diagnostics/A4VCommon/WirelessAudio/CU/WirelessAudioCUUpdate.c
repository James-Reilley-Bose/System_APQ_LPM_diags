//
// WirelessAudioCUUpdate.c
//

#include "WirelessAudioCUUpdate.h"
#include "WirelessAudioVariant.h"
#include "WirelessAudioUtilities.h"
#include "WirelessAudioUpdateTransfer.h"

SCRIBE_DECL(wa_update);

static void WirelessAudioCUUpdate_HandleEventOccurred(void);
static void WirelessAudioCUUpdate_ProcessDataMessage(WA_DataMessage_t* message);

BOOL WirelessAudioCUUpdate_HandleMessage(GENERIC_MSG_t* msg)
{
    BOOL handled = TRUE;

    switch(msg->msgID)
    {
        case WA_MSG_ID_HandleEXTI:
            WirelessAudioCUUpdate_HandleEventOccurred();
            break;
        case WA_MSG_ID_Data_Request_Timeout:
            waTransfer_HandleTimer();
            break;
        default:
            handled = FALSE;
            break;
    }

    return handled;
}

static void WirelessAudioCUUpdate_HandleEventOccurred(void)
{
    WirelessAudioUtilities_ReadAllDataMessages(WirelessAudioCUUpdate_ProcessDataMessage);
}

static void WirelessAudioCUUpdate_ProcessDataMessage(WA_DataMessage_t* message)
{
    switch(message->opcode)
    {
        case WA_BCMD_UPD_DATA:
        case WA_BCMD_ENTER_UPDATE:
        case WA_BCMD_READY_TO_UPDATE:
        case WA_BCMD_MAP_REQUEST:
        case WA_BCMD_MAP_RESPONSE:
        case WA_BCMD_CLEAR_MAP:
        case WA_BCMD_MAP_CLEARED:
        case WA_BCMD_UPD_AUTHENTICATE:
        case WA_BCMD_UPD_INSTALL:
        case WA_BCMD_UPD_VERSION:
            waTransfer_HandleDarrMessage(message);
            if(!waGetUpdating()) WirelessAudioVariant_GoToState(WA_STATE_ON);
            break;
        default:
            LOG(wa_update, ROTTEN_LOGLEVEL_NORMAL, "Updating, dropping packet 0x%02X", message->opcode);
            break;
    }
}
