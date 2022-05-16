__Introduction__

Microprocessors are widely used in many complex and basic applications. A complex 
application might be building a home security system to protect the household and a 
basic application might be driving a stepper motor. This paper provides an application
in which a stepper motor is being driven using the TM4C123G board. The paper also 
provides the sub-systems of the project and their solution approach. In this article, using 
the functions of the evaluation board named TM4C123G, stepper motor driver software 
is described, which changes speed depending on the audio frequency. It also describes 
what the requirements of this project are and how it is done to meet those requirements. 
In addition, the connections and communication method between the TM4C123G and 
other components are also explained. The software in this project is written with 
assembly.

__Subsystems:__

      Audio Sampling
      
      Frequency Detection
      
      Stepper Motor and Led
      
      Change Direction of Stepper Motor
      
      User Interface
      
__Pin Assignments:__

      GY-MAX9814 Microphone:
      
      GND → GND 
      
      VDD → Vbus
      
      Gain → GND (Connecting GND provides 50 Db, VDD 40 Db and floating 60 Db gain)
      
      Out → PE3 (ADC0)
      
      AR → Empty
      
      Stepper Motor:
      
      GND → GND 
      
      V → Vbus
      
      IN2 → PB4
      
      IN3 → PB5
      
      IN4 → PB6
      
      IN5 → PB7
      
      LCD Screen:
      
      GND → GND 
      
      VCC → Vbus
      
      BL → Vbus
      
      RST → A7
      
      CE → A3
      
      D0 → A6
      
      DIN → A5
      
      CLK → A2
      
