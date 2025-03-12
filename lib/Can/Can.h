#ifndef CAN_H
#define CAN_H

#ifdef NATIVE
#include "../../test/mock/MockFlexCAN_T4.h"
#else
#include <FlexCAN_T4.h>
#endif

#include <Arduino.h>
#include <functional>
#include "../Storage/Storage.h"
#include "../Transmission/Transmission.h"
#include "../AnalogInput/AnalogInput.h"
#include "../dbc/r3.h"
#include "../dbc/tcs.h"

class Can {
  public:
    FlexCAN_T4<CAN1, RX_SIZE_16, TX_SIZE_16>& interface;
    Storage& storage;
    Transmission& transmission;
    AnalogInput& clutchRight;
    
    Can(
        FlexCAN_T4<CAN1, RX_SIZE_16, TX_SIZE_16>& interfaceRef,
        Storage& storageRef,
        Transmission& transmissionRef,
        AnalogInput& clutchRightRef
    ) : 
        interface(interfaceRef),
        storage(storageRef),
        transmission(transmissionRef),
        clutchRight(clutchRightRef) {
    }
    
    virtual void begin();
    virtual void update();

    virtual void broadcastShiftSettings();
    virtual void broadcastClutchSettings();    
    virtual void broadcastClutch();
    virtual void broadcastAnalogInput();

  private:
    std::function<void(int)> group0 = nullptr;
    std::function<void(int, int, int, int)> shiftSettings = nullptr;
    std::function<void(int, int, int, bool)> clutchSettings = nullptr;
    std::function<void(int, int)> setClutch = nullptr;

    void handleGroup0(const CAN_message_t &msg);
    void handleShiftSettings(const CAN_message_t &msg);
    void handleClutchSettings(const CAN_message_t &msg);
    void handleSetClutch(const CAN_message_t &msg);

    unsigned long lastEcuUpdate = 0;
};

#endif
