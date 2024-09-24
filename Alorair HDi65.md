**Alorair CAN bus**

CAN Bitrate 50kbs

RJ45 pinout

  ----------------------------------- -----------------------------------
  4                                   CANL

  5                                   CANH

  7                                   5v

  8                                   gnd
  ----------------------------------- -----------------------------------

Can Messages

  ------------------------ ----------------------- -----------------------
  Power off                123#01.01.00            

  Power on                 123#01.01.01            

  Pump out                 123#01.02.01            

  Continuous defrost on    123#01.03.01            

  Continuous defrost off   123#01.03.00            

  Set Target Humidity      123#01.05.xx            xx= target humidity
  ------------------------ ----------------------- -----------------------

Status returns

  ------------------------ ----------------------- -----------------------
  status                   Byte 5                  Byte 6

  Running, power on        0x01                    0x04

  Stopping, power on       0x22                    0x00

  Stopped, power on        0x02                    0x00

  Power off, plugged in    0x00                    0x00

  Pump running             0x10                    0x00

  Running, Pump Running                            
  ------------------------ ----------------------- -----------------------
