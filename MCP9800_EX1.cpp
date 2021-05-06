/*----------------------------------------------------------------------*
 * Example sketch for Microchip MCP9800/1/2/3 2-Wire High-Accuracy      *
 * Temperature Sensors. Demonstrates reading of ambient temperature     *
 * in Celsius, changing device options via the configuration register   *
 * and changing the Limit-Set and Hysteresis registers.                 *
 *                                                                      *
 * Jack Christensen 28Aug2013                                           *
 * Dr.Olaf Hagendorf 05.2021                                            *
 *                                                                      *
 * Tested with Arduino 1.8.5 and an Arduino Uno.                        *
 * ported to and tested with Mbed OS 6.9                                *
 *                                                                      *
 * This work is licensed under CC BY-SA 4.0,                            *
 * http://creativecommons.org/licenses/by-sa/4.0/                       *
 *----------------------------------------------------------------------*/ 
#include "mbed.h" 
#include "MCP9800.h"      // http://github.com/JChristensen/MCP9800
                          // https://github.com/ATM-HSW/MCP9800

I2C i2c(I2C_SDA , I2C_SCL );
MCP9800 mySensor;

// print the values of all four registers on the serial monitor
void displayRegs(void) {
  int32_t C;
  uint8_t config;
  MCP9800_config cfg;
  
  config = mySensor.readConfig(&cfg);
  printf("Config=0x%x\n", config);
  printf(" ONE_SHOT:%d\n", cfg.ONE_SHOT);
  printf(" ADC_RESOLUTION:%d\n", cfg.ADC_RESOLUTION==3?12:(cfg.ADC_RESOLUTION==2?11:(cfg.ADC_RESOLUTION==1?10:9)));
  printf(" FAULT_QUEUE:%d\n", cfg.FAULT_QUEUE);
  printf(" ALERT_POLARITY:%d\n", cfg.ALERT_POLARITY);
  printf(" INT_MODE:%d\n", cfg.INT_MODE);
  printf(" SHUTDOWN:%d\n", cfg.SHUTDOWN);

  C = mySensor.readTemp(AMBIENT);
  printf("Ambient %dC\n", C);

  C = mySensor.readTemp(HYSTERESIS);
  printf("Hysteresis %dC\n", C);

  C = mySensor.readTemp(LIMITSET);
  printf("Limit-Set %dC\n", C);
}

int main() {
  printf("Start\n");
  
  // initialize the hardware
  mySensor.init(&i2c, MCP9800_ADDRESS8BIT);

  // max resolution, 0.0625 °C
  mySensor.writeConfig(ADC_RES_12BITS);

  // print the current register values
  displayRegs();
  printf("\n");

  mySensor.writeTempx(HYSTERESIS, 1.5f);   // almost freezing - use float number with 'real' value
  mySensor.writeTempx(LIMITSET, 1000000);  // boiling - use int number with temperatur*10000
  
  printf("HYSTERESIS %d\n", mySensor.readTemp(HYSTERESIS));
  printf("LIMITSET %d\n", mySensor.readTemp(LIMITSET));

  while(true) {  
    int C = mySensor.readTemp(AMBIENT);
    printf("Ambient %dC %fC\n", mySensor.readTemp(AMBIENT), mySensor.readTempF(AMBIENT));
    thread_sleep_for(2000);
  }
}
