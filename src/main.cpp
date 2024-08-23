
// Signal K application template file.
//
// This application demonstrates core SensESP concepts in a very
// concise manner. You can build and upload the application as is
// and observe the value changes on the serial port monitor.
//
// You can use this source file as a basis for your own projects.
// Remove the parts that are not relevant to you, and add your own code
// for external hardware libraries.

#define SERIALDEBUG
#define SENSESPV3
#include <Arduino.h>
#include "sensesp/signalk/signalk_listener.h"
#include "sensesp/signalk/signalk_value_listener.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/transforms/linear.h"
#include "sensesp_app_builder.h"

// specific to this example
#include <Adafruit_ADS1X15.h>
#define ADS1115_BANK1_ADDR 0x48

using namespace sensesp;

reactesp::ReactESP app;

// ADS1115 Module Instantiation
// ADS1115 @ 0x48 I2C
Adafruit_ADS1115 adsB1;

// volts for adsB1
float read_adsB1_ch0_callback() { return (adsB1 . computeVolts ( adsB1 . readADC_SingleEnded ( 0 ) ));}
float read_adsB1_ch1_callback() { return (adsB1 . computeVolts ( adsB1 . readADC_SingleEnded ( 1 ) ));}
float read_adsB1_ch2_callback() { return (adsB1 . computeVolts ( adsB1 . readADC_SingleEnded ( 2 ) ));}
float read_adsB1_ch3_callback() { return (adsB1 . computeVolts ( adsB1 . readADC_SingleEnded ( 3 ) ));}

// forward declarations
void setupADS1115();

// unique chip id
int32_t chipId=0;

double B1A0 = 0;
double B1A1 = 0;
double B1A2 = 0;
double B1A3 = 0;

// The setup function performs one-time application initialization.
void setup() {

  // used to create a unique id
  uint8_t chipid [ 6 ];
  #ifdef SENSESPV3
  SetupLogging();
  #else
  #ifndef SERIAL_DEBUG_DISABLED
    SetupSerialDebug(115200);
  #endif
  #endif
  // derive a unique chip id from the burned in MAC address
  esp_efuse_mac_get_default ( chipid );
  for ( int i = 0 ; i < 6 ; i++ )
    chipId += ( chipid [ i ] << ( 7 * i ));
  
  #ifdef SERIALDEBUG
  // dump chip id at startup
  Serial.printf ( "\nPoweron reset - Chip ID: %x\n" , chipId );
  #endif

  // Construct the global SensESPApp() object
  SensESPAppBuilder builder;
  sensesp_app = (&builder)
                    // Set a custom hostname for the app.
                    ->set_hostname("Sensesp ADS1115 example")
                    // Optionally, hard-code the WiFi and Signal K server
                    // settings. This is normally not needed.
                    ->set_button_pin(0)
                    ->get_app();

  //*************begin ads1115 I2C section
  // setup ads1115 on I2C bus, ads1115 is 16bit a/d. 
  // Raw inputs should not exceed 3.3V
  setupADS1115();

  // Create a new sensor for adsB1 channel 0
  const char *kAnalogInputB1A0LinearConfigPath = "/sensors/analog_input_adsB1_0/calibrate";
  const char *kAnalogInputB1A0SKPath = "/sensors/analog_input_adsB1_0/sk_path";
  const float AnalogInputB1A0multiplier = 1.515; // just read in volts for now
  const float AnalogInputB1A0offset = 0;

  auto* analog_input_adsB1_0 = new RepeatSensor<float>(
      500, read_adsB1_ch0_callback);

  // Connect the analog input to Signal K output. This will publish the
  // analog input value to the Signal K server every time it changes.
  analog_input_adsB1_0
      ->connect_to(new Linear(AnalogInputB1A0multiplier, AnalogInputB1A0offset, kAnalogInputB1A0LinearConfigPath))
  
      ->connect_to(new SKOutputFloat(
         "/sensors/analog_input_adsB1_0/voltage",         // Signal K path
         kAnalogInputB1A0SKPath,        
                                                  // web UI and for storing the
                                                  // configuration
          new SKMetadata("Volts",                     // Define output units
                        "Analog Input Volts")));  // Value description

  // Create a new sensor for adsB1 channel 1
  const char *kAnalogInputB1A1LinearConfigPath = "/sensors/analog_input_adsB1_1/calibrate";
  const char *kAnalogInputB1A1SKPath = "/sensors/analog_input_adsB1_1/sk_path";
  const float AnalogInputB1A1multiplier = 1.515; // just read in volts for now
  const float AnalogInputB1A1offset = 0;

  auto* analog_input_adsB1_1 = new RepeatSensor<float>(
      500, read_adsB1_ch1_callback);

  // Connect the analog input to Signal K output. This will publish the
  // analog input value to the Signal K server every time it changes.
  analog_input_adsB1_1
      ->connect_to(new Linear(AnalogInputB1A1multiplier, AnalogInputB1A1offset, kAnalogInputB1A1LinearConfigPath))
  
      ->connect_to(new SKOutputFloat(
          "/sensors/analog_input_adsB1_1/voltage",         // Signal K path
          kAnalogInputB1A1SKPath,        
                                                  // web UI and for storing the
                                                  // configuration
          new SKMetadata("Volts",                     // Define output units
                        "Analog Input Volts")));  // Value description

  // Create a new sensor for adsB1 channel 2
  const char *kAnalogInputB1A2LinearConfigPath = "/sensors/analog_input_adsB1_2/calibrate";
  const char *kAnalogInputB1A2SKPath = "/sensors/analog_input_adsB1_2/sk_path";
  const float AnalogInputB1A2multiplier = 1.515; // just read in volts for now
  const float AnalogInputB1A2offset = 0;

  auto* analog_input_adsB1_2 = new RepeatSensor<float>(
      500, read_adsB1_ch2_callback);

  // Connect the analog input to Signal K output. This will publish the
  // analog input value to the Signal K server every time it changes.
  analog_input_adsB1_2
      ->connect_to(new Linear(AnalogInputB1A2multiplier, AnalogInputB1A2offset, kAnalogInputB1A2LinearConfigPath))
  
      ->connect_to(new SKOutputFloat(
          "/sensors/analog_input_adsB1_2/voltage",         // Signal K path
          kAnalogInputB1A2SKPath,        
                                                  // web UI and for storing the
                                                  // configuration
          new SKMetadata("Volts",                     // Define output units
                        "Analog Input Volts")));  // Value description

  // Create a new sensor for adsB1 channel 3
  const char *kAnalogInputB1A3LinearConfigPath = "/sensors/analog_input_adsB1_3/calibrate";
  const char *kAnalogInputB1A3SKPath = "/sensors/analog_input_adsB1_3/sk_path";
  const float AnalogInputB1A3multiplier = 1.515; // just read in volts for now
  const float AnalogInputB1A3offset = 0;

  auto* analog_input_adsB1_3 = new RepeatSensor<float>(
      500, read_adsB1_ch3_callback);

  // Connect the analog input to Signal K output. This will publish the
  // analog input value to the Signal K server every time it changes.
  analog_input_adsB1_3
      ->connect_to(new Linear(AnalogInputB1A3multiplier, AnalogInputB1A3offset, kAnalogInputB1A3LinearConfigPath))
  
      ->connect_to(new SKOutputFloat(
          "/sensors/analog_input_adsB1_3/voltage",         // Signal K path
          kAnalogInputB1A3SKPath,        
                                                  // web UI and for storing the
                                                  // configuration
          new SKMetadata("Volts",                     // Define output units
                        "Analog Input Volts")));  // Value description

//*****************end ads115 I2C

  #ifndef SENSESPV3
  // Start networking, SK server connections and other SensESP internals
  sensesp_app->start();
  #endif
} // end setup

void loop() { 

  app.tick();
  
} // end loop


// Initialize the ADS1115 modules
void setupADS1115 ( void )
{
  // ads1115 stuff
  uint16_t locBank1Present;

  #if defined ( SERIALDEBUG )
    Serial . println ( "Getting single-ended readings from AIN0..3" );
    Serial . println ( "ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)" );
  #endif

  // Set the ADC gain to 1. By design we expect input range @ the ADS1115 from 0-3.3V 
  //                                                                   ADS1115
  //                                                                   -------
  adsB1 .setGain ( GAIN_ONE );        // 1x gain   +/- 4.096V  1 bit = 0.125mV
  if (!  adsB1  .  begin  ( ADS1115_BANK1_ADDR )) {
    Serial . println ( "Failed to initialize ADS @0x48." );
    locBank1Present = 0; // flag to indicate if bank 1 is available
  } else {
  #if defined ( SERIALDEBUG )
    Serial . println ( "Initialized ADS @0x48 - Bank 1" );
  #endif
    locBank1Present = 1; // flag to indicate if bank 1 is available
  } // end if

#if defined ( SERIALDEBUG )
  Serial . println ( "ADS1115 module setup complete." );
#endif

} // end setupADS1115
