#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>

class Storage {
  public:
    static const int LED = 5;
    static const int UP = 6;
    static const int DOWN = 7;
    static const int CLUTCH_LEFT = 16;
    static const int CLUTCH_RIGHT = 17;
    static const int IA = 15;
    static const int IB = 14;
    static const int ECU_UP = 9;
    static const int ECU_DOWN = 8;
    static const int SERVO = 28;

    virtual void begin();

    /**
     * @brief Set the delay (ms) from when an upshift is started to when the solenoid is fired.
     * @param value int delay (ms)
     * @return true if the value is within bounds
     */
    virtual bool setUpDelay(int value);
    /**
     * @brief Get the delay (ms) from when an upshift is started to when the solenoid is fired.
     * @return int delay (ms)
     */
    virtual int upDelay() const;

    /**
     * @brief Set the delay (ms) from when a downshift is started to when the solenoid is fired.
     * @param value int delay (ms)
     * @return true if the value is within bounds
     */
    virtual bool setDownDelay(int value);
    /**
     * @brief Get the delay (ms) from when a downshift is started to when the solenoid is fired.
     * @return int delay (ms)
     */
    virtual int downDelay() const;

    /**
     * @brief Set the duration (ms) that the solenoid is powered for.
     * @param value int duration (ms)
     * @return true if the value is within bounds
     */
    virtual bool setOutput(int value);
    /**
     * @brief Get the duration (ms) that the solenoid is powered for.
     * @return int duration (ms)
     */
    virtual int output() const;

    /**
     * @brief Set the duration (ms) to wait for the clutch to return to the start position.
     * @param value int duration (ms)
     * @return true if the value is within bounds
     */
    virtual bool setTimeout(int value);
    /**
     * @brief Get the duration (ms) to wait for the clutch to return to the start position.
     * @return int duration (ms)
     */
    virtual int timeout() const;

    /**
     * @brief Set the clutch servo minimum position (μs).
     * @param value int position (μs)
     * @return true if the value is within bounds
     */
    virtual bool setStart(int value);
    /**
     * @brief Get the clutch servo minimum position (μs).
     * @return int position (μs)
     */
    virtual int start() const;

    /**
     * @brief Set the clutch servo maximum position (μs).
     * @param value int position (μs)
     * @return true if the value is within bounds
     */
    virtual bool setEnd(int value);
    /**
     * @brief Get the clutch servo maximum position (μs).
     * @return int position (μs)
     */
    virtual int end() const;

    /**
     * @brief Set the friction point position (μs).
     * @param value int position (μs)
     * @return true if the value is within bounds
     * @deprecated Implement higher resolution around the friction point
     */
    virtual bool setFriction(int value);
    /**
     * @brief Get the friction point position (μs).
     * @return int position (μs)
     * @deprecated Implement higher resolution around the friction point
     */
    virtual int friction() const;

    /**
     * @brief Set automatic clutch release state
     * @param value bool state
     * @return true
     * @deprecated Implement automatic clutch release
     */
    virtual bool setAutoLaunch(bool value);
    /**
     * @brief Get automatic clutch release state
     * @return bool state
     * @deprecated Implement automatic clutch release
     */
    virtual bool autoLaunch() const;

  private:
    enum Addresses {
        UP_DELAY_ADDRESS = 0,
        DOWN_DELAY_ADDRESS = 2,
        OUTPUT_ADDRESS = 4,
        TIMEOUT_ADDRESS = 6,
        START_ADDRESS = 8,
        END_ADDRESS = 10,
        FRICTION_ADDRESS = 12,
        AUTO_LAUNCH_ADDRESS = 14
    };

    int read(int address, uint16_t defaultValue);
    int read(int address) const;
    void save(int address, uint16_t value);
};

#endif
