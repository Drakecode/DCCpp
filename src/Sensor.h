/**********************************************************************

Sensor.h
COPYRIGHT (c) 2013-2016 Gregg E. Berman

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#ifndef Sensor_h
#define Sensor_h

#include "DCCpp.h"

#ifdef USE_SENSOR
#include "Arduino.h"

#define  SENSOR_DECAY  0.03

struct SensorData {
  int snum;
  byte pin;
  byte pullUp;
};

/**DCC++ BASE STATION supports Sensor inputs that can be connected to any Aruidno Pin
not in use by this program.  Sensors can be of any type (infrared, magentic, mechanical...).
The only requirement is that when "activated" the Sensor must force the specified Arduino
Pin LOW (i.e. to ground), and when not activated, this Pin should remain HIGH (e.g. 5V),
or be allowed to float HIGH if use of the Arduino Pin's internal pull-up resistor is specified.

To ensure proper voltage levels, some part of the Sensor circuitry
MUST be tied back to the same ground as used by the Arduino.

The Sensor code below utilizes exponential smoothing to "de-bounce" spikes generated by
mechanical switches and transistors.  This avoids the need to create smoothing circuitry
for each sensor.  You may need to change these parameters through trial and error for your specific sensors.

To have this sketch monitor one or more Arduino pins for sensor triggers, first define/edit/delete
sensor definitions using the following variation of the "S" command:

\verbatim
<S ID PIN PULLUP>
\endverbatim
creates a new sensor ID, with specified PIN and PULLUP
if sensor ID already exists, it is updated with specificed PIN and PULLUP
returns: <O> if successful and <X> if unsuccessful (e.g. out of memory)

\verbatim
<S ID>
\endverbatim
deletes definition of sensor ID
returns: <O> if successful and <X> if unsuccessful (e.g. ID does not exist)

\verbatim
<S>
\endverbatim
lists all defined sensors
returns: <Q ID PIN PULLUP> for each defined sensor or <X> if no sensors defined

where

\verbatim
ID: the numeric ID (0-32767) of the sensor
PIN: the arduino pin number the sensor is connected to
PULLUP: 1=use internal pull-up resistor for PIN, 0=don't use internal pull-up resistor for PIN
\endverbatim

Once all sensors have been properly defined, use the <E> command to store their definitions to EEPROM.
If you later make edits/additions/deletions to the sensor definitions, you must invoke the <E> command if you want those
new definitions updated in the EEPROM.  You can also clear everything stored in the EEPROM by invoking the <e> command.

All sensors defined as per above are repeatedly and sequentially checked within the main loop of this sketch.
If a Sensor Pin is found to have transitioned from one state to another, one of the following serial messages are generated:

\verbatim
<Q ID>     - for transition of Sensor ID from HIGH state to LOW state (i.e. the sensor is triggered)
<q ID>     - for transition of Sensor ID from LOW state to HIGH state (i.e. the sensor is no longer triggered)
\endverbatim

Depending on whether the physical sensor is acting as an "event-trigger" or a "detection-sensor," you may
decide to ignore the <q ID> return and only react to <Q ID> triggers.
*/
struct Sensor{
  static Sensor *firstSensor;
  SensorData data;
  boolean active;
  float signal;
  Sensor *nextSensor;
#ifdef USE_EEPROM
  static void load();
  static void store();
#endif
  static Sensor *create(int, int, int, int=0);
  static Sensor* get(int);  
  static void remove(int);  
  static void show();
  static void status();
  static void parse(char *c);
  static void check();   
}; // Sensor

#endif

#endif