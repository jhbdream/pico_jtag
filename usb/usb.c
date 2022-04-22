#include <tusb.h>
#include <usb_descriptors.h>
#include <led.h>
#include <hardware/gpio.h>
#include <hardware/uart.h>

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void)itf;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;
    printf("tud_hid_get_report_cb");

    return 0;
}

void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const *RxDataBuffer, uint16_t bufsize)
{
    static uint8_t TxDataBuffer[CFG_TUD_HID_EP_BUFSIZE];
    uint32_t response_size = TU_MIN(CFG_TUD_HID_EP_BUFSIZE, bufsize);

    // This doesn't use multiple report and report ID
    (void)itf;
    (void)report_type;

    if (bufsize == 0)
    {
        return;
    }

    memset(TxDataBuffer, 0, sizeof(TxDataBuffer));

    DAP_ProcessCommand(RxDataBuffer, TxDataBuffer);

    tud_hid_report(0, TxDataBuffer, response_size);

#if 0
    printf("recv: [%d]\n", bufsize);
    for (int i = 0; i < bufsize; i++)
    {
        printf("%02x ", RxDataBuffer[i]);
    }
    printf("\n");
#endif

}