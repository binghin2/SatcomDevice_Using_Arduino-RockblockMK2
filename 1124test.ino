#include <IridiumSBD.h>


#define IridiumSerial Serial
#define DIAGNOSTICS false // Change this to see diagnostics

// Declare the IridiumSBD object
IridiumSBD modem(IridiumSerial);

void setup()
{
  int signalQuality = -1;
  int err;
  
  // Start the console serial port
  Serial.begin(115200);
  while (!Serial);

  // Start the serial port connected to the satellite modem
  IridiumSerial.begin(19200);

  // Begin satellite modem operation
  Serial.println("Starting modem...");
  err = modem.begin();
  if (err != ISBD_SUCCESS)
  {
    Serial.print("Begin failed: error ");
    Serial.println(err);
    if (err == ISBD_NO_MODEM_DETECTED)
      Serial.println("No modem detected: check wiring.");
    return;
  }

  // Example: Print the firmware revision
  char version[12];
  err = modem.getFirmwareVersion(version, sizeof(version));
  if (err != ISBD_SUCCESS)
  {
     Serial.print("FirmwareVersion failed: error ");
     Serial.println(err);
     return;
  }
  Serial.print("Firmware Version is ");
  Serial.print(version);
  Serial.println(".");

  // Example: Test the signal quality.
  // This returns a number between 0 and 5.
  // 2 or better is preferred.
  err = modem.getSignalQuality(signalQuality);
  if (err != ISBD_SUCCESS)
  {
    Serial.print("SignalQuality failed: error ");
    Serial.println(err);
    return;
  }

  Serial.print("On a scale of 0 to 5, signal quality is currently ");
  Serial.print(signalQuality);
  Serial.println(".");

  // Send the message
  Serial.print("Trying to send the message.  This might take several minutes.\r\n");
  err = modem.sendSBDText("Hello, world!");
  if (err != ISBD_SUCCESS)
  {
    Serial.print("sendSBDText failed: error ");
    Serial.println(err);
    if (err == ISBD_SENDRECEIVE_TIMEOUT)
      Serial.println("Try again with a better view of the sky.");
  }

  else
  {
    Serial.println("Hey, it worked!");
  }
}

void loop()
{
}

#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c)
{












  /*    output
  출력이 약하다는 소리.
  연구실 실내에서 진행하였지만
  안테나가 실내에 있고
  창문쪽을 바라 보지않ㅇ ㅏ그런듯 
  
  
  Starting modem...  
AT
ATE1
AT&D0
AT&K0
AT+SBDMTA=0
AT+CGMR
AT+CGMR
Firmware Version is TA21004.
AT+CSQ
On a scale of 0 to 5, signal quality is currently 0.
Trying to send the message.  This might take several minutes.
AT+SBDWT
Hello, world!
AT+SBDIX */
  Serial.write(c);
}
#endif
