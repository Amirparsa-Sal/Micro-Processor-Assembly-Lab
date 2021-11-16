# 01- Interrupt
In this experiment we worked on some basic arduino concepts like interrupts, polling, pull-up & pull-down resistors and learnt how to use arduino functions to implement these features.

### Interrupt vs. Polling

In polling method, the value of a flag is checked continuously by the processor and if its value changes, the processor will notice and may do some stuff to respond that change. For example if an IO device finishes its job, it will change that flag to notify the processor that the job has finished. The downside of this method is the processor is always busy and cannot do other jobs while the IO operation is being completed.

In contrast, in interrupt method, IO device controllers will send an interrupt to the processor whenever the job finishes and the processor can do its other jobs while the IO operation is being completed.

### How to Implement Interrupts in Arduino

At first, we must know that which pins in our arduino can handle interrupts. In my case, Arduino UNO has 2 interrupt pins which are the pins 2 and 3. You can find the interrupt pins for all arduino types in table below which is extracted from [arduino website](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/).

| BOARD                             | DIGITAL PINS USABLE FOR INTERRUPTS                           |
| --------------------------------- | ------------------------------------------------------------ |
| Uno, Nano, Mini, other 328-based  | 2, 3                                                         |
| Uno WiFi Rev.2, Nano Every        | all digital pins                                             |
| Mega, Mega2560, MegaADK           | 2, 3, 18, 19, 20, 21 (**pins 20 & 21** are not available to use for interrupts while they are used for I2C communication) |
| Micro, Leonardo, other 32u4-based | 0, 1, 2, 3, 7                                                |
| Zero                              | all digital pins, except 4                                   |
| MKR Family boards                 | 0, 1, 4, 5, 6, 7, 8, 9, A1, A2                               |
| Nano 33 IoT                       | 2, 3, 9, 10, 11, 13, A1, A5, A7                              |
| Nano 33 BLE, Nano 33 BLE Sense    | all pins                                                     |
| Due                               | all digital pins                                             |
| 101                               | all digital pins (Only pins 2, 5, 7, 8, 10, 11, 12, 13 work with **CHANGE**) |

To set a pin to handle interrupts we must use `attachInterrupt` function. For example to set pin 3 to handle interrupts:

```C++
attachInterrupt(digitalPinToInterrupt(3), ISR_function, FALLING);
```

For the first parameter we should use `digitalPinToInterrupt` function to convert the digital pin number to the desired value for `attachInterrupt` function.

For the second parameter we must pass a function as an ISR handler of the interrupt. This function will be executed every time the interrupt happens. For example the following code snippet is an ISR function which prints a senetence every time the interrupt happens.

```C++
void ISR_Function() {
  Serial.println("In the ISR function!");
}
```

And the final parameter is the event to trigger the interrupt. According to [arduino website](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/) there are 5 types of event:

- **LOW** to trigger the interrupt whenever the pin is low,
- **CHANGE** to trigger the interrupt whenever the pin changes value
- **RISING** to trigger when the pin goes from low to high,
- **FALLING** for when the pin goes from high to low.

The Due, Zero and MKR1000 boards allow also:

- **HIGH** to trigger the interrupt whenever the pin is high.

### Pull-up & Pull-down Resistors

We use pull-up and pull-down resistors to ensure that the given data to input pins are valid. 

Lets suppose we have a switch that is connected to a 5V power supply from one side and to an arduino input pin from other side. In this case, when the switch is disconnected we can not ensure that the given data is zero. Because in this situation, the input pin is not connected to anywhere and it can easily bounce from 0 to 1 and 1 to 0 by external noises.  The same scenario happens when the switch is connected to ground from one side and to an arduino input pin from other side.

To solve this issue, we use pull-up or pull-down resistors. The following image illustrates how we should use pull-up and pull-down resistors:

<img width="832" alt="Pull-up & Pull-down" src="https://user-images.githubusercontent.com/60196448/140920253-b03e36e2-0028-4659-89db-2460cf49eaba.png">

Lets suppose we have a circuit like the first picture. In this case, if the switch is disconnected, knowing that there is a small amount of current due to limitations of PIO pins, there wont be a big voltage difference between the VCC and PIO pin. So, the pin will receive a logical one as input. In the other hand, if the switch is connected the PIO pin will receive the logial zero as input. So, using the pull-up resistor we can ensure that the given data to our PIO pin is a valid data. The same scenario goes for the pull-down resistor but with different values for switch to be on or off.

### Experiments

In each expermient we have 2 buttons and some LEDs. If the first button is pressed the LEDs will blink simultaneously multiple times(In here they blink 9 times cause my name has 9 characters!), And if the second button is pressed, the LEDs will be turned off.

In both expirements the circuit diagram is as following picture:

- Experiment 1: Polling

  In this experiment I used the polling method to do the experiment. This means that we check the value of input pins inside the `loop()` function of the arduino and whenever a change happens we can respond to that change inside the loop.

  Here is the [code](Polling/Polling.ino).

- Experiment 2: Interrupt

  In this experiment I used interrupts to implement the behaviour of the buttons. For each button I've wrote an ISR function to be triggered whenever the button is pressed.

  Here is the [code](Interrupt/Interrupt.ino).

  **Important note:** The `delay()` function can not work correctly inside the ISR functions. But, because this expirement is just focused on writing ISR functions and triggering them, this problem is not so important. I used 80000 as the value of the `delay()` function which is nearly a 1 second delay inside the ISR function.

  

